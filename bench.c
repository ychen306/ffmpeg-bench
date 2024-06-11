#include <stdint.h>

void ff_h264_idct_add(uint8_t *_dst, int16_t *_block, int stride);
int scalarproduct_and_madd_int16(int16_t *v1, const int16_t *v2,
                                              const int16_t *v3,
                                              int order, int mul);
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

int bench_scalarproduct_and_madd_int16() {
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
  return scalarproduct_and_madd_int16(v1, v2, v3, order, mul);
}

int main() {
  bench_ff_h264_idct_add();
  bench_scalarproduct_and_madd_int16();
}
