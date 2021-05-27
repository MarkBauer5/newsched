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
      _offset(0),
      _state(state_t::WARMUP),
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
    _offset = 0;
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
        _num_warmup_processed += num_copy_samples;
        if (num_items >= num_need_samples) {
            in += num_copy_samples;
            out += num_copy_samples;
            num_items -= num_copy_samples;
            _state = state_t::IIR;
            _gain = get_initial_gain();
        } else {
            return;
        }
    }

    // Do the single-pole IIR filtering with decimation on the magnitude
    // and scale the output accordingly.
    unsigned int num_gain_updates = (num_items / _iir_update_decim) + 1;
    unsigned int alignment = volk_get_alignment();
    float* magnitude_squared = reinterpret_cast<float*>(
        volk_malloc(num_gain_updates * sizeof(float), alignment));
    float* inverse_mag = reinterpret_cast<float*>(
        volk_malloc(num_gain_updates * sizeof(float), alignment));

    // generate squared magnitudes at decimated rate (gather operations
    for (unsigned int i = 0; i < num_gain_updates; i++) {
        unsigned int idx = i * _iir_update_decim + _offset;
        magnitude_squared[i] =
            in[idx].real() * in[idx].real() + in[idx].imag() * in[idx].imag();
    }

    // Use previous _gain on first set of samples using "offset"
    for (unsigned int idx = 0; idx < (_iir_update_decim - _offset) % _iir_update_decim;
         idx++)
        out[idx] = in[idx] * _gain;

    // compute inverse square roots of the magnitude. In other words,
    // the 1/magnitude
    volk_32f_invsqrt_32f(inverse_mag, magnitude_squared, num_items / _iir_update_decim);

    for (unsigned int decim_idx = 0; decim_idx < num_gain_updates; decim_idx++) {
        if (std::isfinite(inverse_mag[decim_idx])) {
            float rate = (inverse_mag[decim_idx] > _gain / _reference) ? _decay_rate
                                                                       : _attack_rate;
            _gain -= rate * (_gain - _reference * inverse_mag[decim_idx]);
        } else {
            _gain -= _decay_rate * _gain;
        }

        // Negative gain is prohibited
        _gain = _gain < 0.0f ? 0.0f : _gain;

        // Clip the gain at _max_gain
        _gain = _gain > _max_gain ? _max_gain : _gain;

        unsigned int num_samples = decim_idx * (_iir_update_decim + 1) < num_items
                                       ? _iir_update_decim
                                       : num_items - (decim_idx * _iir_update_decim);

        for (unsigned int sample_idx = 0; sample_idx < num_samples; sample_idx++) {
            unsigned int idx = sample_idx + decim_idx * _iir_update_decim + _offset;
            out[idx] = in[idx] * _gain;
        }
    }

    // Update offset according to how many items should not
    // have their gains adjusted for the next call
    _offset = num_items % _iir_update_decim;

    // Free the allocated arrays.
    volk_free(magnitude_squared);
    volk_free(inverse_mag);
}

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
