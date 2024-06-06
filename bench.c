#include <stdint.h>

void ff_h264_idct_add(uint8_t *_dst, int16_t *_block, int stride);
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

int main() {
  bench_ff_h264_idct_add();
}
