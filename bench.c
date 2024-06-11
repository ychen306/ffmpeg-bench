#include <stdint.h>

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

void bench_ff_h264_idct_add() {
  uint8_t dst[4 * 4];
  int16_t block[4 * 4];
  int stride = 4;

  for (int i = 0; i < 16; i++) {
    dst[i] = i;
    block[i] = i;
  }

  ff_h264_idct_add(dst, block, stride);
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
  int size;
  int is_signed;
  int maxval;
  float Q34;
  float rounding;
  /// execute function here, not sure of values
}

void bench_ff_h264_luma_dc_dequant_idct() {
  int16_t output[240]; //not sure, stride is 16 and the final x_offset is 10*stride, so it should be 15*stride=240
  int16_t input[4 * 4];
  int qmul = 16; // just chose a number
  for (int i = 0; i < 16; i++) {
    input[i] = i;

  ff_h264_luma_dc_dequant_idct(output, input, qmul);
  }

}




int main() {
  bench_ff_h264_idct_add();
  bench_scalarproduct_and_madd_int16();
  bench_scalarproduct_and_madd_int32();
  // bench_quantize_bands();
  bench_ff_h264_luma_dc_dequant_idct();
}
