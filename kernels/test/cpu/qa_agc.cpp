#include <gnuradio/kernels/cpu/agc2_ff.hpp>
#include <gnuradio/kernels/cpu/agc_cc.hpp>
#include <gnuradio/kernels/cpu/agc_ff.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

using namespace gr;
namespace analog = gr::kernels::analog::cpu;

TEST(AnalogKernel, agc_float_identity_test)
{
    auto agc_kernel = analog::agc_ff(.001f, 1.0f, 1.0f, 65536.0f);
    float input[1000];
    float output[1000];
    float expected_output[1000];

    for (unsigned int idx = 0; idx < 1000; idx++) {
        input[idx] = 1.0f;
        output[idx] = 1.0f;
        expected_output[idx] = 1.0f;
    }

    agc_kernel(input, output, 1000);

    for (unsigned int idx = 0; idx < 1000; idx++)
        EXPECT_NEAR(output[idx], expected_output[idx], .0001f);
}

TEST(AnalogKernel, agc_float_ramp_test)
{
    auto agc_kernel = analog::agc_ff(.001f, 1.0f, 1.0f, 65536.0f);
    float input[50];
    float output[50];
    float expected_output[50] = {
        0.5,      0.50025,  0.5005,   0.50075,  0.500999, 0.501249, 0.501498, 0.501747,
        0.501997, 0.502246, 0.502494, 0.502743, 0.502992, 0.50324,  0.503489, 0.503737,
        0.503985, 0.504233, 0.504481, 0.504729, 0.504976, 0.505224, 0.505471, 0.505718,
        0.505966, 0.506213, 0.506459, 0.506706, 0.506953, 0.507199, 0.507446, 0.507692,
        0.507938, 0.508184, 0.50843,  0.508676, 0.508922, 0.509167, 0.509413, 0.509658,
        0.509903, 0.510148, 0.510393, 0.510638, 0.510882, 0.511127, 0.511371, 0.511616,
        0.51186,  0.512104
    };


    for (unsigned int idx = 0; idx < 50; idx++)
        input[idx] = .5f;

    agc_kernel(input, output, 50);

    for (unsigned int idx = 0; idx < 50; idx++)
        EXPECT_NEAR(output[idx], expected_output[idx], .0001f);
}

TEST(AnalogKernel, agc_complex_identity_test)
{
    auto agc_kernel = analog::agc_cc(.001f, 1.0f, 1.0f, 65536.0f);
    std::complex<float> input[1000];
    std::complex<float> output[1000];
    std::complex<float> expected_output[1000];

    for (unsigned int idx = 0; idx < 1000; idx++) {
        input[idx] = 1.0f;
        output[idx] = 1.0f;
        expected_output[idx] = 1.0f;
    }

    agc_kernel(input, output, 1000);

    for (unsigned int idx = 0; idx < 1000; idx++) {
        EXPECT_NEAR(output[idx].real(), expected_output[idx].real(), .0001f);
        EXPECT_NEAR(output[idx].imag(), expected_output[idx].imag(), .0001f);
    }
}

TEST(AnalogKernel, agc_complex_ramp_test)
{
    auto agc_kernel = analog::agc_cc(.001f, 1.0f, 1.0f, 65536.0f);
    std::complex<float> input[50];
    std::complex<float> output[50];
    std::complex<float> expected_output[50] = {
        { 0.5, 0.5 },           { 0.500146, 0.500146 }, { 0.500293, 0.500293 },
        { 0.500439, 0.500439 }, { 0.500585, 0.500585 }, { 0.500731, 0.500731 },
        { 0.500877, 0.500877 }, { 0.501023, 0.501023 }, { 0.501169, 0.501169 },
        { 0.501314, 0.501314 }, { 0.50146, 0.50146 },   { 0.501605, 0.501605 },
        { 0.501751, 0.501751 }, { 0.501896, 0.501896 }, { 0.502041, 0.502041 },
        { 0.502186, 0.502186 }, { 0.502331, 0.502331 }, { 0.502476, 0.502476 },
        { 0.50262, 0.50262 },   { 0.502765, 0.502765 }, { 0.502909, 0.502909 },
        { 0.503054, 0.503054 }, { 0.503198, 0.503198 }, { 0.503342, 0.503342 },
        { 0.503486, 0.503486 }, { 0.50363, 0.50363 },   { 0.503774, 0.503774 },
        { 0.503918, 0.503918 }, { 0.504062, 0.504062 }, { 0.504205, 0.504205 },
        { 0.504349, 0.504349 }, { 0.504492, 0.504492 }, { 0.504635, 0.504635 },
        { 0.504778, 0.504778 }, { 0.504921, 0.504921 }, { 0.505064, 0.505064 },
        { 0.505207, 0.505207 }, { 0.50535, 0.50535 },   { 0.505493, 0.505493 },
        { 0.505635, 0.505635 }, { 0.505778, 0.505778 }, { 0.50592, 0.50592 },
        { 0.506062, 0.506062 }, { 0.506204, 0.506204 }, { 0.506347, 0.506347 },
        { 0.506489, 0.506489 }, { 0.50663, 0.50663 },   { 0.506772, 0.506772 },
        { 0.506914, 0.506914 }, { 0.507055, 0.507055 }
    };

    for (unsigned int idx = 0; idx < 50; idx++)
        input[idx] = std::complex<float>(.5f, .5f);

    agc_kernel(input, output, 50);


    // std::cout << "{";
    // for (unsigned int i = 0; i < 50; i++) {
    //     std::cout << "{" << output[i].real() << "," << output[i].imag() << "},";
    // }
    // std::cout << "}";

    for (unsigned int idx = 0; idx < 50; idx++) {
        EXPECT_NEAR(output[idx].real(), expected_output[idx].real(), .0001f);
        EXPECT_NEAR(output[idx].imag(), expected_output[idx].imag(), .0001f);
    }
}

TEST(AnalogKernel, agc2_float_identity_test)
{
    auto agc_kernel = analog::agc2_ff(1.0f, .001f, 1.0f, 1.0f, 65536.0f);
    float input[1000];
    float output[1000];
    float expected_output[1000];

    for (unsigned int idx = 0; idx < 1000; idx++) {
        input[idx] = 1.0f;
        output[idx] = 1.0f;
        expected_output[idx] = 1.0f;
    }

    agc_kernel(input, output, 1000);

    for (unsigned int idx = 0; idx < 1000; idx++)
        EXPECT_NEAR(output[idx], expected_output[idx], .0001f);
}

TEST(AnalogKernel, agc2_float_ramp_test)
{
    auto agc_kernel = analog::agc2_ff(.1f, .001f, 2.0f, 1.1f, 1.2f);
    float input[50];
    float output[50];
    float expected_output[50] = {
        0.55,     0.5775,   0.603625, 0.628444, 0.652022, 0.674421, 0.6957,   0.715915,
        0.735119, 0.753363, 0.770695, 0.78716,  0.802802, 0.817662, 0.831779, 0.84519,
        0.85793,  0.870034, 0.881532, 0.892456, 0.902833, 0.912691, 0.922057, 0.930954,
        0.939406, 0.947436, 0.955064, 0.962311, 0.969195, 0.975735, 0.981949, 0.987851,
        0.993459, 0.998786, 1.00385,  1.00865,  1.01322,  1.01756,  1.02168,  1.0256,
        1.02932,  1.03285,  1.03621,  1.0394,   1.04243,  1.04531,  1.04804,  1.05064,
        1.05311,  1.05545
    };


    for (unsigned int idx = 0; idx < 50; idx++)
        input[idx] = .5f;

    agc_kernel(input, output, 50);

    std::cout << "{";
    for (unsigned int i = 0; i < 50; i++) {
        std::cout << output[i] << ",";
    }
    std::cout << "}";

    // for (unsigned int idx = 0; idx < 50; idx++)
    //     EXPECT_NEAR(output[idx], expected_output[idx], .0001f);
}