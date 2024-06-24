#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "random_ints_arrays.h"

// check all original kernels for return values and save them for evlauating correctness

#define INTFLOAT int
#define UINTFLOAT unsigned

// #define NUM_TESTS 100
#define cycle_t unsigned long long

int *rand_array(int size);

void ff_h264_idct_add(uint8_t *_dst, int16_t *_block, int stride);

int32_t scalarproduct_and_madd_int16(int16_t *v1, const int16_t *v2,
                                              const int16_t *v3,
                                              int order, int mul);

int32_t scalarproduct_and_madd_int32(int16_t *v1, const int32_t *v2,
                                              const int16_t *v3,
                                              int order, int mul);
void quantize_bands(int *out, const float *in, const float *scaled,
    int size, int is_signed, int maxval, const float Q34,
    const float rounding);

void ff_h264_luma_dc_dequant_idct(int16_t *_output, int16_t *_input, int qmul);

void ps_stereo_interpolate(INTFLOAT (*l)[2], INTFLOAT (*r)[2],INTFLOAT h[2][4],
    INTFLOAT h_step[2][4], int len);

void ps_stereo_interpolate_ipdopd(INTFLOAT (*l)[2], INTFLOAT (*r)[2],
                                           INTFLOAT h[2][4], INTFLOAT h_step[2][4],
                                           int len);
void h263_h_loop_filter(uint8_t *__restrict__ src, int stride, int qscale);

void h263_v_loop_filter(uint8_t *__restrict__ src, int stride, int qscale);

#define BENCH_FUNC(FUNC_W_ARGS, NUM_TESTS, THROUGHPUT) \
        cycle_t THROUGHPUT;\
        do {cycle_t begin, end, elapsed1, elapsed2;     \
        begin = __builtin_readcyclecounter();                             \
        for (int i = 0; i < NUM_TESTS; i++)                               \
          FUNC_W_ARGS;                                                    \
        end = __builtin_readcyclecounter();                               \
        elapsed1 = end - begin;                                           \
        begin = __builtin_readcyclecounter();                             \
        for (int i = 0; i < 2 * NUM_TESTS; i++)                           \
          FUNC_W_ARGS;                                                    \
        end = __builtin_readcyclecounter();                               \
        elapsed2 = end - begin;                                           \
        THROUGHPUT = (elapsed2 - elapsed1) / NUM_TESTS;           \
        }while(0)

RAND_INT(8)
RAND_INT(16)
RAND_ARRAY(8)
RAND_ARRAY(16)
RAND_UINT(8)
RAND_ARRAY_U(8)

cycle_t bench_ff_h264_idct_add() {
  // set input size
  int stride = 4;
  srand(42);
  uint8_t *dst = rand_array_8(4 * 4);
  int16_t *block = rand_array_16(4 * 4);
  BENCH_FUNC(ff_h264_idct_add(dst, block, stride), 100, throughput);
  free(dst);
  free(block);
  printf("%llu ", throughput);
  return throughput;
}

cycle_t bench_scalarproduct_and_madd_int16() {
  // var input size
  srand(63);
  int16_t *v1 = rand_array_16(8);
  int16_t *v2 = rand_array_16(8);
  int16_t *v3 = rand_array_16(8);
  int order = 8;
  int mul = 2;
  BENCH_FUNC(scalarproduct_and_madd_int16(v1, v2, v3, order, mul), 100,throughput);
  free(v1);
  free(v2);
  free(v3);
  printf("%llu ", throughput);
  return throughput;
}

cycle_t bench_scalarproduct_and_madd_int32() {
  // var input size
  srand(84);
  int16_t *v1 = rand_array_16(8);
  int16_t *v2 = rand_array_16(8);
  int16_t *v3 = rand_array_16(8);
  int order = 8;
  int mul = 2;
  BENCH_FUNC(scalarproduct_and_madd_int16(v1, v2, v3, order, mul), 100, throughput);
  free(v1);
  free(v2);
  free(v3);
  printf("%llu ", throughput);
  return throughput;
}

void bench_quantize_bands() {
  // floats and stuff, leave untouched
  int out[4*4];
  float in[4*4];
  float scaled[4*4];
  int size = 16;
  int is_signed;
  int maxval;
  float Q34;
  float rounding;
  /// execute function here, not sure of values
}


cycle_t bench_ff_h264_luma_dc_dequant_idct() {
  // set input size
  srand(77);
  int16_t output[15 * 16 + 1]; //not sure, stride is 16 and the final x_offset is 10*stride, so it should be 15*stride=240
  int16_t *input = rand_array_16(16);
  int qmul = 16; // just chose a number
  BENCH_FUNC(ff_h264_luma_dc_dequant_idct(output, input, qmul), 100, throughput);
  free(input);
  printf("%llu ", throughput);
  return throughput;
}

RAND_INT(64)





// still having issues with pointers and pointer conversion
cycle_t bench_ps_stereo_interpolate() {
  const int len = 42;
  srand(22);
  INTFLOAT (**l)[2] = malloc(sizeof(uintptr_t) * len);
  INTFLOAT (**r)[2] = malloc(sizeof(uintptr_t) * len);
  for (int i = 0; i < len; i++) {
    l[i] = rand_array(2);
    r[i] = rand_array(2);
  }

  // len arrays of size 2^^
  INTFLOAT (*h)[2][4] = rand_array(2 * 4);

  // then go thorugh and make each one their own random values
  INTFLOAT h_step[2][4] = {*rand_array(4), *rand_array(4)};
  // dont make len a random variable, set at certain value

  BENCH_FUNC(ps_stereo_interpolate(l, r, h, h_step, len), 100, throughput);
  printf("%llu ", throughput);

  // llu is long long unsigned int
  
  //printf("%d \n", 9);
  //for (int i = 0; i<2; i++)
  //  printf("%d", *h[i]);

  // for (int i = 0; i < len; i++) {
  //   free(l[i]);
  //   free(r[i]);}
  free(l);
  free(r);

  free(h);
  // free(h_step);
  return throughput;
}

// these two memory leak

cycle_t bench_ps_stereo_interpolate_ipdopd() {
  const int len = 42;
  srand(99);
  INTFLOAT (**l)[2] = malloc(sizeof(uintptr_t) * len);
  INTFLOAT (**r)[2] = malloc(sizeof(uintptr_t) * len);
  for (int i = 0; i < len; i++) {
    l[i] = rand_array(2);
    r[i] = rand_array(2);
  }

  INTFLOAT (*h)[2][4] = rand_array(2 * 4);

  INTFLOAT h_step[2][4] = {*rand_array(4), *rand_array(4)};

  BENCH_FUNC(ps_stereo_interpolate_ipdopd(l, r, h, h_step, len), 100, throughput);
  printf("%llu ", throughput);

  free(l);
  free(r);
  free(h);
  return throughput;
}

cycle_t bench_h263_h_loop_filter() {
  srand(71);
  const int stride = 3;
  const int qscale = 24;
  uint8_t *src = rand_array_u8(7*stride+4);
  BENCH_FUNC(h263_h_loop_filter(src + 2, stride, qscale), 100, throughput);
  free(src);
  printf("%llu ", throughput);
  return throughput;
}

cycle_t bench_h263_v_loop_filter() {
  srand(13);
  const int stride = 3;
  const int qscale = 24;
  uint8_t *src = rand_array_u8(3*stride + 8);
  BENCH_FUNC(h263_v_loop_filter(src + 2 * stride, stride, qscale), 100, throughput);
  free(src);
  printf("%llu ", throughput);
  return throughput;
}





int main() {
  bench_scalarproduct_and_madd_int16();
  bench_scalarproduct_and_madd_int32();
  bench_ff_h264_idct_add();
  bench_ff_h264_luma_dc_dequant_idct();
  bench_ps_stereo_interpolate();
  bench_ps_stereo_interpolate_ipdopd();
  bench_h263_h_loop_filter();
  bench_h263_v_loop_filter();
}
