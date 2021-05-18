#include <gnuradio/kernels/cpu/agc2_ff.hpp>
#include <cmath>
#include <iostream>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

void agc2_ff::operator()(void* in_buffer, void* out_buffer, size_t num_items)
{
    float* in = reinterpret_cast<float*>(in_buffer);
    float* out = reinterpret_cast<float*>(out_buffer);

    for (unsigned int idx = 0; idx < num_items; idx++) {
        out[idx] = in[idx] * _gain;

        float diff = fabsf(out[idx]) - _reference;
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