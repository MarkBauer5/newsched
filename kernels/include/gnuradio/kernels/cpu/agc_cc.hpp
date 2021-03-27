#pragma once
#include <gnuradio/kernels/common/kernel.hpp>
#include <complex>
#include <cstdint>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

struct agc_cc : stateful_kernel_interface {
public:
    agc_cc(float rate, float reference, float gain, float max_gain)
        : _rate(rate),
          _reference(reference),
          _gain(gain),
          _default_gain(gain),
          _max_gain(max_gain){};

    void operator()(void* in_buffer, void* out_buffer, size_t num_items);
    void operator()(void* buffer, size_t num_items)
    {
        operator()(buffer, buffer, num_items);
    }
    void reset() { _gain = _default_gain; };

protected:
    float _rate;         // adjustment rate
    float _reference;    // reference value
    float _gain;         // current gain
    float _default_gain; // default gain when re-setting
    float _max_gain;     // max allowable gain
};

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
