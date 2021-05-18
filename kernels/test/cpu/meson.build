incdir = include_directories('../../include')

if get_option('enable_testing')
    srcs = ['qa_agc.cpp']
    e = executable('qa_agc', 
        srcs, 
        include_directories : incdir, 
        dependencies: [newsched_kernels_cpu_dep, gtest_dep], 
        install : true)
    test('AGC Tests', e)
endif
