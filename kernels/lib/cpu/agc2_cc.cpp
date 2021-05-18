#include <gnuradio/kernels/cpu/agc2_cc.hpp>
#include <cmath>
#include <iostream>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

void agc2_cc::operator()(void* in_buffer, void* out_buffer, size_t num_items)
{
    std::complex<float>* in = reinterpret_cast<std::complex<float>*>(in_buffer);
    std::complex<float>* out = reinterpret_cast<std::complex<float>*>(out_buffer);

    for (unsigned int idx = 0; idx < num_items; idx++) {
        out[idx] = in[idx] * _gain;

        float diff =
            sqrt(out[idx].real() * out[idx].real() + out[idx].imag() * out[idx].imag()) -
            _reference;
        float rate = fabsf(diff) > _gain ? _attack_rate : _decay_rate;

        // Adjust gain according to difference between signal level and reference
        // Gain depends on attack or decay mode
        _gain -= diff * rate;

        // Negative gain is prohibited
        _gain = _gain < 0.0f ? 0.0f : _gain;

        // Clip the gain at _max_gain
        _gain = _gain > _max_gain ? _max_gain : _gain;
    }
}

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr