#pragma once
#include <gnuradio/kernels/common/kernel.hpp>
#include <complex>
#include <cstdint>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

/**
 * @brief high performance Automatic Gain Control class with
 * attack and decay rates.
 *
 * @ingroup level_controllers_blk
 *
 * @details For Power the absolute value of the complex number is used.
 *
 */
struct agc_cc : kernel_interface {
public:
    /**
     * @brief Build a complex value AGC loop block with attack and decay rates.
     *
     * @param rate the update rate of the loop.
     * @param reference reference value to adjust signal power to.
     * @param gain initial gain value.
     * @param max_gain maximum gain.
     */
    agc_cc(float rate, float reference, float gain, float max_gain)
        : _rate(rate), _reference(reference), _gain(gain), _max_gain(max_gain){};

    void operator()(void* in_buffer, void* out_buffer, size_t num_items);
    void operator()(void* buffer, size_t num_items)
    {
        operator()(buffer, buffer, num_items);
    }

protected:
    float _rate;
    float _reference;
    float _gain;
    float _max_gain;
};

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
