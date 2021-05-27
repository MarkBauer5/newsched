#include <gnuradio/kernels/cpu/agc2_cc.hpp>
#include <gnuradio/kernels/cpu/agc2_ff.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

namespace analog = gr::kernels::analog::cpu;

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
    auto agc_kernel = analog::agc2_ff(.1f, .01f, 2.0f, 1.1f, 1.9f);
    float input[50];
    float output[50];
    float expected_output[50] = {
        0.55,     0.6225,   0.691375, 0.697918, 0.704429, 0.710906, 0.717352, 0.723765,
        0.730146, 0.736495, 0.742813, 0.749099, 0.755353, 0.761577, 0.767769, 0.77393,
        0.78006,  0.78616,  0.792229, 0.798268, 0.804277, 0.810255, 0.816204, 0.822123,
        0.828012, 0.833872, 0.839703, 0.845504, 0.851277, 0.85702,  0.862735, 0.868422,
        0.87408,  0.879709, 0.885311, 0.890884, 0.89643,  0.901948, 0.907438, 0.912901,
        0.918336, 0.923744, 0.929126, 0.93448,  0.939808, 0.945109, 0.95,     0.95,
        0.95,     0.95
    };


    for (unsigned int idx = 0; idx < 50; idx++)
        input[idx] = .5f;

    agc_kernel(input, output, 50);

    // std::cout << "{";
    // for (unsigned int i = 0; i < 50; i++) {
    //     std::cout << output[i] << ",";
    // }
    // std::cout << "}";

    for (unsigned int idx = 0; idx < 50; idx++)
        EXPECT_NEAR(output[idx], expected_output[idx], .0001f);
}

TEST(AnalogKernel, agc2_float_decay_test)
{
    auto agc_kernel = analog::agc2_ff(.1f, .01f, 2.0f, 1.0f, 1.9f);
    float input[50];
    float output[50];
    float expected_output[50] = {
        2.5,     2.4875,  2.47531, 2.46343, 2.45184, 2.44055, 2.42953, 2.4188,  2.40833,
        2.39812, 2.38816, 2.37846, 2.369,   2.35977, 2.35078, 2.34201, 2.33346, 2.32512,
        2.317,   2.30907, 2.30134, 2.29381, 2.28646, 2.2793,  2.27232, 2.26551, 2.25887,
        2.2524,  2.24609, 2.23994, 2.23394, 2.22809, 2.22239, 2.21683, 2.21141, 2.20613,
        2.20097, 2.19595, 2.19105, 2.18627, 2.18162, 2.17708, 2.17265, 2.16833, 2.16412,
        2.16002, 2.15602, 2.15212, 2.14832, 2.14461
    };


    for (unsigned int idx = 0; idx < 50; idx++)
        input[idx] = 2.5f;

    agc_kernel(input, output, 50);

    for (unsigned int idx = 0; idx < 50; idx++)
        EXPECT_NEAR(output[idx], expected_output[idx], .0001f);
}


TEST(AnalogKernel, agc2_complex_identity_test)
{
    auto agc_kernel = analog::agc2_cc(.001f, .1f, 1.0f, 1.0f, 65536.0f);
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
        EXPECT_NEAR(output[idx].real(), expected_output[idx].real(), .0001f);
        EXPECT_NEAR(output[idx].imag(), expected_output[idx].imag(), .0001f);
    }
}

TEST(AnalogKernel, agc2_complex_ramp_test)
{
    auto agc_kernel = analog::agc2_cc(.1f, .01f, 2.0f, 1.1f, 1.5f);
    std::complex<float> input[50];
    std::complex<float> output[50];
    std::complex<float> expected_output[50] = {
        { 0.55, 0.55 },         { 0.611109, 0.611109 }, { 0.616788, 0.616788 },
        { 0.622427, 0.622427 }, { 0.628025, 0.628025 }, { 0.633585, 0.633585 },
        { 0.639104, 0.639104 }, { 0.644585, 0.644585 }, { 0.650027, 0.650027 },
        { 0.655431, 0.655431 }, { 0.660796, 0.660796 }, { 0.666124, 0.666124 },
        { 0.671414, 0.671414 }, { 0.676666, 0.676666 }, { 0.681881, 0.681881 },
        { 0.68706, 0.68706 },   { 0.692201, 0.692201 }, { 0.697307, 0.697307 },
        { 0.702376, 0.702376 }, { 0.70741, 0.70741 },   { 0.712407, 0.712407 },
        { 0.71737, 0.71737 },   { 0.722297, 0.722297 }, { 0.72719, 0.72719 },
        { 0.732048, 0.732048 }, { 0.736872, 0.736872 }, { 0.741661, 0.741661 },
        { 0.746417, 0.746417 }, { 0.75, 0.75 },         { 0.75, 0.75 },
        { 0.75, 0.75 },         { 0.75, 0.75 },         { 0.75, 0.75 },
        { 0.75, 0.75 },         { 0.75, 0.75 },         { 0.75, 0.75 },
        { 0.75, 0.75 },         { 0.75, 0.75 },         { 0.75, 0.75 },
        { 0.75, 0.75 },         { 0.75, 0.75 },         { 0.75, 0.75 },
        { 0.75, 0.75 },         { 0.75, 0.75 },         { 0.75, 0.75 },
        { 0.75, 0.75 },         { 0.75, 0.75 },         { 0.75, 0.75 },
        { 0.75, 0.75 },         { 0.75, 0.75 }
    };

    for (unsigned int idx = 0; idx < 50; idx++)
        input[idx] = std::complex<float>(.5f, .5f);

    agc_kernel(input, output, 50);

    for (unsigned int idx = 0; idx < 50; idx++) {
        EXPECT_NEAR(output[idx].real(), expected_output[idx].real(), .0001f);
        EXPECT_NEAR(output[idx].imag(), expected_output[idx].imag(), .0001f);
    }
}

TEST(AnalogKernel, agc2_complex_decay_test)
{
    auto agc_kernel = analog::agc2_cc(.1f, .01f, 2.0f, 1.0f, 1.9f);
    std::complex<float> input[50];
    std::complex<float> output[50];
    std::complex<float> expected_output[50] = {
        { 2.5, 2.5 },         { 2.11612, 2.11612 }, { 1.86796, 1.86796 },
        { 1.85191, 1.85191 }, { 1.83644, 1.83644 }, { 1.82151, 1.82151 },
        { 1.80711, 1.80711 }, { 1.79322, 1.79322 }, { 1.77982, 1.77982 },
        { 1.76689, 1.76689 }, { 1.75442, 1.75442 }, { 1.7424, 1.7424 },
        { 1.73079, 1.73079 }, { 1.7196, 1.7196 },   { 1.7088, 1.7088 },
        { 1.69839, 1.69839 }, { 1.68834, 1.68834 }, { 1.67865, 1.67865 },
        { 1.6693, 1.6693 },   { 1.66028, 1.66028 }, { 1.65158, 1.65158 },
        { 1.64319, 1.64319 }, { 1.63509, 1.63509 }, { 1.62728, 1.62728 },
        { 1.61975, 1.61975 }, { 1.61248, 1.61248 }, { 1.60547, 1.60547 },
        { 1.59871, 1.59871 }, { 1.59219, 1.59219 }, { 1.5859, 1.5859 },
        { 1.57983, 1.57983 }, { 1.57397, 1.57397 }, { 1.56832, 1.56832 },
        { 1.56288, 1.56288 }, { 1.55762, 1.55762 }, { 1.55255, 1.55255 },
        { 1.54766, 1.54766 }, { 1.54294, 1.54294 }, { 1.53839, 1.53839 },
        { 1.534, 1.534 },     { 1.52976, 1.52976 }, { 1.52568, 1.52568 },
        { 1.52174, 1.52174 }, { 1.51794, 1.51794 }, { 1.51427, 1.51427 },
        { 1.51073, 1.51073 }, { 1.50732, 1.50732 }, { 1.50403, 1.50403 },
        { 1.50085, 1.50085 }, { 1.49779, 1.49779 }
    };

    for (unsigned int idx = 0; idx < 50; idx++)
        input[idx] = std::complex<float>(2.5f, 2.5f);

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
