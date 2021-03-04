#include <cuComplex.h>
#include <cuda.h>
#include <cuda_runtime.h>


__global__ void
apply_copy_kernel(cuFloatComplex* in, cuFloatComplex* out, int batch_size, int load = 1)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int n = batch_size;
    if (i < n) {
        for (int x = 0; x < load; x++) {
            out[i].x = in[i].x;
            out[i].y = in[i].y;
        }
    }
}

void apply_copy(cuFloatComplex* in,
                cuFloatComplex* out,
                int grid_size,
                int block_size,
                int load,
                cudaStream_t stream)
{
    int batch_size = block_size * grid_size;
    apply_copy_kernel<<<grid_size, block_size, 0, stream>>>(in, out, batch_size, load);
}

void get_block_and_grid(int* minGrid, int* minBlock)
{
    cudaOccupancyMaxPotentialBlockSize(minGrid, minBlock, apply_copy_kernel, 0, 0);
}