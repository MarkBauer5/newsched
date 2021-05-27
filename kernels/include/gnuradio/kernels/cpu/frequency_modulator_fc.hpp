#pragma once
#include <gnuradio/kernels/common/kernel.hpp>
#include <complex>
#include <cstdint>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

struct frequency_modulator_fc : stateful_kernel_interface {
public:
    frequency_modulator_fc(float sensitivity, float phase)
        : _sensitivity(sensitivity),
          _default_sensitivity(sensitivity),
          _phase(phase),
          _default_phase(phase){};

    void operator()(void* in_buffer, void* out_buffer, size_t num_items);
    void operator()(void* buffer, size_t num_items)
    {
        operator()(buffer, buffer, num_items);
    }
    void reset()
    {
        _sensitivity = _default_sensitivity;
        _phase = _default_phase;
    };

protected:
    float _sensitivity;
    float _default_sensitivity;
    float _phase;
    float _default_phase;
};

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
