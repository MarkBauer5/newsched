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

    float rate = 0.0f;
    float diff = 0.0f;
    for (unsigned int idx = 0; idx < num_items; idx++) {
        out[idx] = in[idx] * _gain;

        diff = fabs(out[idx]) - _reference;
        rate = _decay_rate;
        if (diff > _gain)
            rate = _attack_rate;

        _gain -= diff * rate;

        // Not sure about this; will blow up if _gain < 0 (happens
        // when rates are too high), but is this the solution?
        if (_gain < 0.0)
            _gain = 10e-5;

        if (_max_gain > 0.0 && _gain > _max_gain)
            _gain = _max_gain;
    }
}

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr