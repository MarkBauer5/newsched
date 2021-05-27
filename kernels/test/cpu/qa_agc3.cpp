#include <gnuradio/kernels/cpu/agc3_cc.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

namespace analog = gr::kernels::analog::cpu;

TEST(AnalogKernel, agc3_complex_identity_test)
{
    auto agc_kernel = analog::agc3_cc(.001f, .1f, 1.0f, 1.0f, 25, 4);
    std::complex<float> input[100];
    std::complex<float> output[100];
    std::complex<float> expected_output[100];

    for (unsigned int idx = 0; idx < 100; idx++) {
        input[idx] = std::complex<float>(1.0f, 0.0f);
        output[idx] = std::complex<float>(1.0f, 0.0f);
        expected_output[idx] = std::complex<float>(1.0f, 0.0f);
    }

    agc_kernel(input, output, 100);

    for (unsigned int idx = 0; idx < 100; idx++) {
        EXPECT_NEAR(output[idx].real(), expected_output[idx].real(), .01f);
        EXPECT_NEAR(output[idx].imag(), expected_output[idx].imag(), .01f);
    }
}

TEST(AnalogKernel, agc3_complex_ramp_test)
{
    auto agc_kernel = analog::agc3_cc(.001f, .1f, 2.0f, 1.9f, 25, 4);
    std::complex<float> input[50];
    std::complex<float> output[50];
    std::complex<float> expected_output[50] = { { 0.5, 0.5 },   { 0.5, 0.5 },
                                                { 0.5, 0.5 },   { 0, 0 },
                                                { 0, 0 },       { 0, 0 },
                                                { 0, 0 },       { 0, 0 },
                                                { 0, 0 },       { 0, 0 },
                                                { 0, 0 },       { 0, 0 },
                                                { 0, 0 },       { 0, 0 },
                                                { 0, 0 },       { 0, 0 },
                                                { 0, 0 },       { 0, 0 },
                                                { 0, 0 },       { 0, 0 },
                                                { 0, 0 },       { 0, 0 },
                                                { 0, 0 },       { 0, 0 },
                                                { 0, 0 },       { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.95, 0.95 },
                                                { 0.95, 0.95 }, { 0.94905, 0.94905 } };

    for (unsigned int idx = 0; idx < 50; idx++)
        input[idx] = std::complex<float>(.5f, .5f);

    agc_kernel(input, output, 50);

    std::cout << "{";
    for (unsigned int i = 0; i < 50; i++) {
        std::cout << "{" << output[i].real() << "," << output[i].imag() << "},";
    }
    std::cout << "}";

    // for (unsigned int idx = 0; idx < 100; idx++) {
    //     EXPECT_NEAR(output[idx].real(), expected_output[idx].real(), .0001f);
    //     EXPECT_NEAR(output[idx].imag(), expected_output[idx].imag(), .0001f);
    // }
}