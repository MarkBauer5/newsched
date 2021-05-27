#include <gnuradio/kernels/cpu/fft.hpp>
#include <cmath>
#include <complex>

namespace gr {
namespace kernels {
namespace math {
namespace cpu {


fft::fft(bool forward, unsigned int fft_size)
{
    auto alignment = volk_get_alignment();
    _input = reinterpret_cast<fftwf_complex*>(
        volk_malloc(fft_size * sizeof(std::complex<float>), alignment));

    _output = reinterpret_cast<fftwf_complex*>(
        volk_malloc(fft_size * sizeof(std::complex<float>), alignment));

    _plan = fftwf_plan_dft_1d(fft_size,
                              reinterpret_cast<fftwf_complex*>(_input),
                              reinterpret_cast<fftwf_complex*>(_output),
                              FFTW_FORWARD,
                              FFTW_MEASURE);
}

void fft::operator()(void* in_buffer, void* out_buffer, size_t num_items)
{
    std::complex<float>* in = reinterpret_cast<std::complex<float>*>(in_buffer);
    std::complex<float>* out = reinterpret_cast<std::complex<float>*>(out_buffer);
}

} // namespace cpu
} // namespace math
} // namespace kernels
} // namespace gr
