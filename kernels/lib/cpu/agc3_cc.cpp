#include <gnuradio/kernels/cpu/agc3_cc.hpp>
#include <volk/volk.h>
#include <cmath>
#include <cstring>
#include <iostream>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

agc3_cc::agc3_cc(float attack_rate,
                 float decay_rate,
                 float reference,
                 float max_gain,
                 unsigned int num_warmup_samples,
                 unsigned int iir_update_decim)
    : _attack_rate(attack_rate),
      _decay_rate(decay_rate),
      _reference(reference),
      _gain(1.0f),
      _max_gain(max_gain),
      _num_warmup_samples(num_warmup_samples),
      _num_warmup_processed(0),
      _iir_update_decim(iir_update_decim)
{
    unsigned int alignment = volk_get_alignment();
    _magnitudes = reinterpret_cast<float*>(
        volk_malloc(_num_warmup_samples * sizeof(float), alignment));

    _warmup_samples = reinterpret_cast<std::complex<float>*>(
        volk_malloc(_num_warmup_samples * sizeof(std::complex<float>), alignment));
};

void agc3_cc::reset()
{
    _gain = 1.0;
    _num_warmup_processed = 0;
    _state = state_t::WARMUP;
};

float agc3_cc::get_initial_gain()
{
    float initial_gain = 0.0f;

    // Calculate magnitudes
    volk_32fc_magnitude_32f(_magnitudes, _warmup_samples, _num_warmup_samples);

    // Sum the magnitudes
    float sum = 0.0f;
    for (unsigned int idx = 0; idx < _num_warmup_samples; idx++)
        sum += _magnitudes[idx];

    initial_gain = _reference * (static_cast<float>(_num_warmup_samples) / sum);

    // Negative gain is prohibited
    initial_gain = initial_gain < 0.0f ? 0.0f : initial_gain;

    // Clip the gain at _max_gain
    initial_gain = initial_gain > _max_gain ? _max_gain : initial_gain;

    return initial_gain;
}

void agc3_cc::operator()(void* in_buffer, void* out_buffer, size_t num_items)
{
    std::complex<float>* in = reinterpret_cast<std::complex<float>*>(in_buffer);
    std::complex<float>* out = reinterpret_cast<std::complex<float>*>(out_buffer);

    bool warmup_done = false;
    unsigned int num_samples_left = 0;
    if (_state == state_t::WARMUP) {
        // determine how many samples are available versus how many I need to
        // leave "warmup"
        unsigned int num_need_samples = _num_warmup_samples - _num_warmup_processed;
        unsigned int num_copy_samples =
            std::min(static_cast<unsigned int>(num_items), num_need_samples);

        // Copy over samples into our buffer of warmup samples
        std::memcpy(&_warmup_samples[_num_warmup_processed], in, num_copy_samples);

        // Copy samples into output buffer while in warmup. (Equivalent to gain
        // of 1.0)
        std::memcpy(out, in, num_copy_samples);

        // Calculate initial gain and start to IIR.
        warmup_done = num_need_samples < num_items;
        num_items -= num_copy_samples;
        _num_warmup_processed += warmup_done ? num_copy_samples : 0;
        _state = warmup_done ? state_t::WARMUP : state_t::IIR;
        _gain = warmup_done ? 1.0f : get_initial_gain();

        if (!warmup_done)
            return;
    }

    unsigned int alignment = volk_get_alignment();
    float* magnitude_squared =
        reinterpret_cast<float*>(volk_malloc(num_items * sizeof(float), alignment));
    float* inverse_sqrt =
        reinterpret_cast<float*>(volk_malloc(num_items * sizeof(float), alignment));

    // We cannot control the multiple of _iir_update_decim we get from client code,
    // so we keep track of how many samples we processed last time.
    static unsigned int offset = 0;

    // generate squared magnitudes at decimated rate (gather operation)
    for (int i = 0; i < num_items / _iir_update_decim; i++) {
        int idx = i * _iir_update_decim + offset;
        magnitude_squared[i] =
            in[idx].real() * in[idx].real() + in[idx].imag() * in[idx].imag();
    }
    offset = _iir_update_decim - (num_items % _iir_update_decim);

    // compute inverse square roots
    volk_32f_invsqrt_32f(inverse_sqrt, magnitude_squared, num_items / _iir_update_decim);

    // apply updates
    for (int idx = 0; idx < num_items / _iir_update_decim; idx++) {
        if (std::isfinite(inverse_sqrt[idx])) {
            float rate =
                (inverse_sqrt[idx] > _gain / _reference) ? _decay_rate : _attack_rate;
            _gain = _gain * (1 - rate) + _reference * inverse_sqrt[idx] * rate;
        } else {
            _gain = _gain * (1 - _decay_rate);
        }
        for (int j = idx * _iir_update_decim; j < (idx + 1) * _iir_update_decim; j++) {
            out[j] = in[j] * _gain;
        }
    }
}

} // namespace cpu

} // namespace analog
} // namespace kernels
} // namespace gr
