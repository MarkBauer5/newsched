#pragma once
#include <gnuradio/kernels/common/kernel.hpp>
#include <complex>
#include <cstdint>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

/**
 * @brief Automatic Gain Control
 * \ingroup level_controllers
 *
 *
 */
struct agc2_ff : stateful_kernel_interface {
public:
    agc2_ff(
        float attack_rate, float decay_rate, float reference, float gain, float max_gain)
        : _attack_rate(attack_rate),
          _decay_rate(decay_rate),
          _reference(reference),
          _gain(gain),
          _default_gain(gain),
          _max_gain(max_gain){};

    void operator()(void* in_buffer, void* out_buffer, size_t num_items);
    void operator()(void* buffer, size_t num_items)
    {
        operator()(buffer, buffer, num_items);
    };
    void reset() { _gain = _default_gain; };

protected:
    float _attack_rate; // adjustment rate
    float _decay_rate;
    float _reference;    // reference value
    float _gain;         // current gain
    float _default_gain; // default gain when re-setting
    float _max_gain;     // max allowable gain
};

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
