#pragma once
#include <gnuradio/kernels/common/kernel.hpp>
#include <fftw3.h>
#include <volk/volk.h>
#include <cstddef>
#include <cstdint>


namespace gr {
namespace kernels {
namespace math {
namespace cpu {

struct fft : kernel_interface {
    fft(bool forward, unsigned int fft_size);

    void operator()(void* in_buffer, void* out_buffer, size_t num_items);
    void operator()(void* buffer, size_t num_items)
    {
        operator()(buffer, buffer, num_items);
    };

private:
    bool _forward;
    unsigned int _fft_size;
    void* _plan;
    void* _input;
    void* _output;
};

} // namespace cpu
} // namespace math
} // namespace kernels
} // namespace gr
