#pragma once
#include <gnuradio/kernels/common/kernel_description.hpp>
#include <cstddef>

namespace gr {
namespace kernels {

/**
 * @brief An interface for all kernels to implement
 *
 */
struct kernel_interface {

    /**
     * @brief So all kernels will need meta-data like this so that it can be matched, at
     * run-time, with appropriate blocks/ports.
     *
     */
    kernel_description description_;

    /**
     * @brief A signature for when the input and output buffers are different, but of the
     * same size
     *
     * @param in_buffer
     * @param out_buffer
     * @param num_items
     */
    virtual void operator()(void* in_buffer, void* out_buffer, size_t num_items) = 0;

    /**
     * @brief A signature for in-place operations
     *
     * @param buffer
     * @param num_items
     */
    virtual void operator()(void* buffer, size_t num_items) = 0;

    /**
     * @brief Destroy the kernel interface object
     *
     */
    virtual ~kernel_interface() = default;
};

/**
 * @brief An interface for kernels that are stateful
 *
 */
struct stateful_kernel_interface : kernel_interface {
    /**
     * @brief Resets internal state to defaults
     *
     */
    virtual void reset() = 0;
};

/**
 * @brief An interface for kernels that have rate changes to implement
 *
 */
struct multi_rate_kernel_interface : kernel_interface {
    /**
     * @brief A signature for when the input and output buffers are different, but of the
     * same size
     *
     * @param in_buffer
     * @param out_buffer
     * @param num_input_items
     * @param num_output_items
     */
    virtual void operator()(void* in_buffer,
                            void* out_buffer,
                            size_t num_input_items,
                            size_t num_output_items) = 0;
};


} // namespace kernels
} // namespace gr
