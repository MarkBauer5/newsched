#include <gnuradio/kernels/common/fixed_point.hpp>
#include <gnuradio/kernels/cpu/frequency_modulator_fc.hpp>
#include <cmath>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

void frequency_modulator_fc::operator()(void* in_buffer,
                                        void* out_buffer,
                                        size_t num_items)
{
    float* in = reinterpret_cast<float*>(in_buffer);
    std::complex<float>* out = reinterpret_cast<std::complex<float>*>(out_buffer);

    for (unsigned int idx = 0; idx < num_items; idx++) {
        _phase = _phase + _sensitivity * in[idx];
        _phase = std::fmod(_phase + M_PI, 2.0f * M_PI) - M_PI;

        float output_i, output_q;
        int32_t angle = gr::kernels::fixed_point::float_to_fixed(_phase);
        gr::kernels::fixed_point::sincos(angle, &output_i, &output_q);
        out[idx] = std::complex<float>(output_i, output_q);
    }
}

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
