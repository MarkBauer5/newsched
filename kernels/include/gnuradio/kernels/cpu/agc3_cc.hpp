#pragma once
#include <gnuradio/kernels/common/kernel.hpp>
#include <volk/volk.h>
#include <complex>
#include <cstdint>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

/**
 * @brief Automatic Gain Control class with attack and decay rates.
 *
 * @ingroup level_controllers
 *
 * @details The issue with this block as it was originally written is that
 * it essentially had a randoms warm-up time as it would just take whatever samples it's
 * first given by the scheduler and then uses those to get it's initial gain. However, the
 * scheduler very well could give the block two samples on the first run, making it
 * useless. I think, unfortunately, this will need three states to work in a way that
 * appears stateless to client code. It needs a state in which it's first gathering
 * samples to collect an average magnitude. In this state it just outputs
 *
 */
struct agc3_cc : stateful_kernel_interface {
public:
    agc3_cc(float attack_rate,
            float decay_rate,
            float reference,
            float max_gain,
            unsigned int num_warmup_samples,
            unsigned int iir_update_decim);

    ~agc3_cc() { volk_free(_magnitudes); }

    void operator()(void* in_buffer, void* out_buffer, size_t num_items);
    void operator()(void* buffer, size_t num_items)
    {
        operator()(buffer, buffer, num_items);
    };
    void reset(){};
    float get_initial_gain();

    typedef enum state_enum { WARMUP, IIR } state_t;


protected:
    float _attack_rate;
    float _decay_rate;
    float _reference;
    float _gain;
    float _max_gain;
    float* _magnitudes;
    std::complex<float>* _warmup_samples;
    unsigned int _num_warmup_samples;
    unsigned int _num_warmup_processed;
    unsigned int _iir_update_decim;
    state_t _state;
};

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
