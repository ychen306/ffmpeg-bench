import subprocess
import statistics
# same order as reurn values of the functions
kernels = ["scalarproduct_and_madd_int16", "scalarproduct_and_madd_int32", "ff_h264_idct_add", "ff_h264_luma_dc_dequant_idct", 
           "ps_stereo_interpolate", "ps_stereo_interpolate_ipdopd", "h263_h_loop_filter", "h263_v_loop_filter", "weight_h264_pixels16_8_c"]
vectorized_throughput = {name: [] for name in kernels}
non_vectorized_throughput = {name: [] for name in kernels}
num_tests = 1000 #number of tests to be run
for _ in range(num_tests):
    #capturing values and saving to dictionary
    vec_val = [int(out) for out in subprocess.run("./bench", capture_output=True).stdout.split()]
    novec_val = [int(out) for out in subprocess.run("./bench.novec", capture_output=True).stdout.split()]
    for ind, name in enumerate(kernels):
        vectorized_throughput[name].append(vec_val[ind])
        non_vectorized_throughput[name].append(novec_val[ind])

print("Vectorized speedup")
for name in kernels:
    print(f"{name}: {statistics.median(non_vectorized_throughput[name]) / statistics.median(vectorized_throughput[name])}")