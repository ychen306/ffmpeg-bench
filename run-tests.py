import subprocess
# same order as reurn values of the functions
kernels = ["scalarproduct_and_madd_int16", "scalarproduct_and_madd_int32", "ff_h264_idct_add", "ff_h264_luma_dc_dequant_idct", 
           "ps_stereo_interpolate", "ps_stereo_interpolate_ipdopd", "h263_h_loop_filter", "h263_v_loop_filter"]
# getting initial results
output = subprocess.run("./bench", capture_output=True).stdout.split()
output_novec = subprocess.run("./bench.novec", capture_output=True).stdout.split()
output = [int(val) for val in output]
output_novec = [int(val) for val in output_novec]
num_tests = 10000
anomaly_threshold = 10000
anomalies = 0
for _ in range(num_tests):
    vec_val = [int(out) for out in subprocess.run("./bench", capture_output=True).stdout.split()]
    novec_val = [int(out) for out in subprocess.run("./bench.novec", capture_output=True).stdout.split()]
    for i in range(len(output)):
        if vec_val[i] < anomaly_threshold:
            output[i] += vec_val[i]
        else:
            anomalies += 1
            
        if novec_val[i] < anomaly_threshold:
            output_novec[i] += novec_val[i]
        else:
            anomalies += 1
vectorized_throughput = [val/(num_tests+anomalies) for val in output]
non_vectorized_throughput = [val/(num_tests+anomalies) for val in output_novec]
print("Vectorized speedup")
for i in range(len(kernels)):
    print(f"{kernels[i]}: {non_vectorized_throughput[i]/vectorized_throughput[i]}")
print(f"anomalies = {anomalies}")