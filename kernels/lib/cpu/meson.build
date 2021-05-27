incdir = include_directories([
    '../../include/gnuradio/kernels/common',
    '../../include/gnuradio/kernels/cpu', 
    '../../include'
])

srcs = [
    'agc_ff.cpp',
    'agc_cc.cpp',
    'agc2_ff.cpp',
    'agc2_cc.cpp',
    'agc3_cc.cpp',
    'copy.cpp'
]

newsched_kernels_cpu = library('newsched-kernels-cpu', 
    srcs, 
    include_directories : incdir, 
    install : true, 
    dependencies : [volk_dep, fftw3_dep]
)

newsched_kernels_cpu_dep = declare_dependency(
    include_directories : incdir, 
    link_with : newsched_kernels_cpu,
    dependencies: [volk_dep, fftw3_dep]
)
