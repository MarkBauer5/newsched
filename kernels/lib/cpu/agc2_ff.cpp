#include <gnuradio/kernels/cpu/agc2_ff.hpp>
#include <cmath>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

void agc2_ff::operator()(void* in_buffer, void* out_buffer, size_t num_items)
{
    float* in = reinterpret_cast<float*>(in_buffer);
    float* out = reinterpret_cast<float*>(out_buffer);

    for (unsigned int i = 0; i < num_items; i++) {
        out[i] = in[i] * _gain;

        float diff = fabsf(out[i]) - _reference;
        float rate = fabsf(diff) > _gain ? _attack_rate : _decay_rate;

        // Adjust gain according to difference between signal level and reference
        // Gain depends on attack or decay mode
        _gain -= diff * rate;

        // Negative gain is prohibited
        _gain = _gain < 0.0f ? 0.0f : _gain;

        // Clip the gain at _max_gain
        _gain = _gain > _max_gain ? _max_gain : _gain;

        // std::cout << (_gain > _max_gain ? "Hit max gain" : "Not saturated.") <<
        // std::endl;
    }
}

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr