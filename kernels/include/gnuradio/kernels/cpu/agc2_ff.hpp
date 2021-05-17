#pragma once
#include <gnuradio/kernels/common/kernel.hpp>
#include <complex>
#include <cstdint>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

/**
 * @brief Automatic Gain Control class with attack and decay rates.
 *
 * @ingroup level_controllers_blk
 *
 * @details For Power the absolute value of the complex number is used.
 *
 */
struct agc2_ff : kernel_interface {
public:
    /**
     * @brief Construct a new agc2 ff object
     *
     * @param attack_rate
     * @param decay_rate
     * @param reference
     * @param gain
     * @param max_gain
     */
    agc2_ff(
        float attack_rate, float decay_rate, float reference, float gain, float max_gain)
        : _attack_rate(attack_rate),
          _decay_rate(decay_rate),
          _reference(reference),
          _gain(gain),
          _max_gain(max_gain){};

    void operator()(void* in_buffer, void* out_buffer, size_t num_items);
    void operator()(void* buffer, size_t num_items)
    {
        operator()(buffer, buffer, num_items);
    };

protected:
    float _attack_rate;
    float _decay_rate;
    float _reference;
    float _gain;
    float _max_gain;
};

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
