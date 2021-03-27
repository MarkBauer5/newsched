#include <gnuradio/kernels/cpu/agc_cc.hpp>
#include <cmath>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

void agc_cc::operator()(void* in_buffer, void* out_buffer, size_t num_items)
{
    std::complex<float>* in = reinterpret_cast<std::complex<float>*>(in_buffer);
    std::complex<float>* out = reinterpret_cast<std::complex<float>*>(out_buffer);


    for (unsigned int idx = 0; idx < num_items; idx++) {
        out[idx] = in[idx] * _gain;
        _gain += _rate * (_reference - std::sqrt(out[idx].real() * out[idx].real() +
                                                 out[idx].imag() * out[idx].imag()));
        if (_max_gain > 0.0 && _gain > _max_gain)
            _gain = _max_gain;
    }
}

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
