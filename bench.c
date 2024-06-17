#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "random_ints_arrays.h"

// check all original kernels for return values and save them for evlauating correctness

#define INTFLOAT int
#define UINTFLOAT unsigned


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

RAND_INT(8)
RAND_INT(16)
RAND_ARRAY(16)
RAND_ARRAY(8)

void bench_ff_h264_idct_add() {
  int stride = 4;

  srand(42);

  uint8_t *dst = rand_array_8(4 * 4);
  int16_t *block = rand_array_16(4 * 4);
  

  ff_h264_idct_add(dst, block, stride);
  for (int i = 0; i < 16; i++)
  	printf("%d ", dst[i]);
  printf("\n");

  free(dst);
  free(block);
}

void bench_scalarproduct_and_madd_int16() {
  int16_t v1[4*4];
  int16_t v2[4*4];
  int16_t v3[4*4];
  int order = 16;
  int mul = 2;
  for (int i = 0; i < 16; i++) {
    v1[i] = i;
    v2[i] = i;
    v3[i] = i;
  }
  scalarproduct_and_madd_int16(v1, v2, v3, order, mul);
}

void bench_scalarproduct_and_madd_int32() {
  int16_t v1[4*4];
  int16_t v2[4*4];
  int16_t v3[4*4];
  int order = 16;
  int mul = 2;
  for (int i = 0; i < 16; i++) {
    v1[i] = i;
    v2[i] = i;
    v3[i] = i;
  }
  scalarproduct_and_madd_int16(v1, v2, v3, order, mul);
}

void bench_quantize_bands() {
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


void bench_ff_h264_luma_dc_dequant_idct() {
  int16_t output[15 * 16 + 1]; //not sure, stride is 16 and the final x_offset is 10*stride, so it should be 15*stride=240
  int16_t input[16];
  int qmul = 16; // just chose a number
  for (int i = 0; i < 16; i++) {
    input[i] = i;

  ff_h264_luma_dc_dequant_idct(output, input, qmul);
  }
}

RAND_INT(64)






// still having issues with pointers and pointer conversion
void bench_ps_stereo_interpolate() {
  srand(22);
  INTFLOAT (*l)[2] = {rand_array(2), rand_array(2)};
  INTFLOAT (*r)[2] =

    // len arrays of size 2^^
      // can't just use

  INTFLOAT h[2][4] = {rand_array(4), rand_array(4)};
  // INTFLOAT (*h)[2][4] = rand_array(2 * 4);

  // then go thorugh and make each one their own random values
  INTFLOAT h_step[2][4] = {*rand_array(4), *rand_array(4)};
  // dont make len a random variable, set at certain value
  const int len = rand64();
  ps_stereo_interpolate(l, r, h, h_step, len);
  printf("%d \n", 9);
  for (int i = 0; i<2; i++)
    printf("%d", *h[i]);
  free(l);
  free(r);
  // free(h);
  // free(h_step);
}




int main() {
  bench_scalarproduct_and_madd_int16();
  bench_scalarproduct_and_madd_int32();
  // bench_quantize_bands();
  bench_ff_h264_idct_add();
  bench_ff_h264_luma_dc_dequant_idct();
  bench_ps_stereo_interpolate();
}
