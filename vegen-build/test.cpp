#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "random_ints_arrays.h"
#include <inttypes.h>

#ifdef _MSC_VER
# include <intrin.h>
#else
# include <x86intrin.h>
#endif

#define num_tests 1000

// check all original kernels for return values and save them for evlauating correctness

#define INTFLOAT int
#define UINTFLOAT unsigned

// #define NUM_TESTS 100
// #define cycle_t unsigned long long
#define cycle_t uint64_t

int *rand_array(int size);

void ff_h264_idct_add(uint8_t *_dst, int16_t *_block, int stride);

// int32_t scalarproduct_and_madd_int16(int16_t *v1, const int16_t *v2,
//                                               const int16_t *v3,
//                                               int order, int mul);

// int32_t scalarproduct_and_madd_int32(int16_t *v1, const int32_t *v2,
//                                               const int16_t *v3,
//                                               int order, int mul);
// void quantize_bands(int *out, const float *in, const float *scaled,
//     int size, int is_signed, int maxval, const float Q34,
//     const float rounding);

void ff_h264_luma_dc_dequant_idct(int16_t *_output, int16_t *_input, int qmul);

void ps_stereo_interpolate(INTFLOAT (*l)[2], INTFLOAT (*r)[2],INTFLOAT h[2][4],
    INTFLOAT h_step[2][4], int len);

void ps_stereo_interpolate_ipdopd(INTFLOAT (*l)[2], INTFLOAT (*r)[2],
                                           INTFLOAT h[2][4], INTFLOAT h_step[2][4],
                                           int len);
void h263_h_loop_filter(uint8_t *src, int stride, int qscale);

void h263_v_loop_filter(uint8_t *src, int stride, int qscale);

void weight_h264_pixels16_8_c(uint8_t *_block, ptrdiff_t stride, int height, int log2_denom, int weight, int offset);

void biweight_h264_pixels16_8_c(uint8_t *_dst, uint8_t *_src, ptrdiff_t stride, int height,
                                             int log2_denom, int weightd, int weights, int offset);

void avg_h264_chroma_mc8(uint8_t *_dst /*align 8*/, const uint8_t *_src /*align 1*/, ptrdiff_t stride, int h, int x, int y);

void put_h264_chroma_mc8(uint8_t *_dst /*align 8*/, const uint8_t *_src /*align 1*/, ptrdiff_t stride, int h, int x, int y);

void cavs_idct8_add_c(uint8_t *dst, int16_t *block, ptrdiff_t stride);

RAND_INT(8)
RAND_INT(16)
RAND_ARRAY(8)
RAND_ARRAY(16)
RAND_UINT(8)
RAND_ARRAY_U(8)
RAND_INT(32)
RAND_ARRAY(32)



void test_ff_h264_idct_add(){
    int stride = 4;
    srand(42);
    uint8_t *dst = rand_array_u8(4*4);
    int16_t *block = rand_array_16(4 * 4);
    ff_h264_idct_add(dst, block, stride);
    // size is 16
    for(int i=0; i<16; i++){
        printf("%" PRIu8 " ", dst[i]);
    }
    printf("\n \n");
    free(dst);
    free(block);
}

// void test_scalarproduct_and_madd_int16(){
//     srand(63);
//     int16_t *v1 = rand_array_16(8);
//     int16_t *v2 = rand_array_16(8);
//     int16_t *v3 = rand_array_16(8);
//     int order = 8;
//     int mul = 2;
//     int32_t result = scalarproduct_and_madd_int16(v1, v2, v3, order, mul);
//     printf("%" PRId32 " ", result);
//     printf("\n \n");
//     free(v1);
//     free(v2);
//     free(v3);
// }

// void test_scalarproduct_and_madd_int32(){
//     srand(84);
//     int16_t *v1 = rand_array_16(8);
//     int32_t *v2 = rand_array_32(8);
//     int16_t *v3 = rand_array_16(8);
//     int order = 8;
//     int mul = 2;
//     int32_t result = scalarproduct_and_madd_int32(v1, v2, v3, order, mul);
//     printf("%" PRId32 " ", result);
//     printf("\n \n");
//     free(v1);
//     free(v2);
//     free(v3);
// }

void test_ff_h264_luma_dc_dequant_idct(){
    srand(77);
    int16_t output[15 * 16 + 1] = { 0 }; //not sure, stride is 16 and the final x_offset is 10*stride, so it should be 15*stride=240
    int16_t *input = rand_array_16(16);
    int qmul = 16;
    ff_h264_luma_dc_dequant_idct(output, input, qmul);
    for(int i = 0; i < (15 * 16 + 1); i++){
        printf("%" PRId16 " ", output[i]);
    }
    printf("\n \n");
    free(input);
}

void test_ps_stereo_interpolate() {
    const int len = 42;
    srand(22);
    INTFLOAT l[len][2];
    INTFLOAT r[len][2];
    INTFLOAT h[2][4];
    INTFLOAT h_step[2][4];
    for (int i = 0; i < len; i++) {
      for (int j = 0; j < 2; j++) {
        l[i][j] = ((unsigned)rand()) >>16;
        r[i][j] = ((unsigned)rand()) >>16;
      }
    }
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 4; j++) {
        h[i][j] = ((unsigned)rand()) >>16;
        h_step[i][j] = ((unsigned)rand()) >>16;
      }
    }
    ps_stereo_interpolate(l, r, h, h_step, len);
    for(int i = 0; i < len; i++){
        printf("%d ", l[i][0]);
        printf("%d ", l[i][1]);
        printf("%d ", r[i][0]);
        printf("%d ", r[i][1]);
    }
    printf("\n \n");
}

void test_ps_stereo_interpolate_ipdopd() {
    const int len = 42;
    srand(99);
    INTFLOAT l[len][2];
    INTFLOAT r[len][2];
    INTFLOAT h[2][4];
    INTFLOAT h_step[2][4];
    for (int i = 0; i < len; i++) {
      for (int j = 0; j < 2; j++) {
        l[i][j] = ((unsigned)rand()) >>16;
        r[i][j] = ((unsigned)rand()) >>16;
      }
    }
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 4; j++) {
        h[i][j] = ((unsigned)rand()) >>16;
        h_step[i][j] = ((unsigned)rand()) >>16;
      }
    }
    ps_stereo_interpolate_ipdopd(l, r, h, h_step, len);
    for (int i = 0; i < len; i++) {
        printf("%d ", l[i][0]);
        printf("%d ", l[i][1]);
        printf("%d ", r[i][0]);
        printf("%d ", r[i][1]);
    }
    printf("\n \n");
}


void test_h263_h_loop_filter() {
    srand(71);
    const int stride = 8;
    const int qscale = 24;
    uint8_t *src = rand_array_u8(7*stride+4);
    h263_h_loop_filter(src + 2, stride, qscale);
    for(int i = 0; i< (7*stride+4); i++){
        printf("%" PRIu8 " ", src[i]);
    }
    printf("\n \n");
    free(src);
}

void test_h263_v_loop_filter(){
    srand(13);
    const int stride = 8;
    const int qscale = 24;
    uint8_t *src = rand_array_u8(3*stride + 8);
    h263_v_loop_filter(src + 2 * stride, stride, qscale);
    for(int i=0; i < (3*stride + 8); i++){
        printf("%" PRIu8 " ", src[i]);
    }
    printf("\n \n");
    free(src);
}

void test_weight_h264_pixels16_8_c(){
  srand(59);
  uint8_t *block = rand_array_u8(64);
  ptrdiff_t stride = 16;
  int height = 4;
  int log2_denom = 4;
  int weight = 8;
  int offset = 4;
  weight_h264_pixels16_8_c(block, stride, height, log2_denom, weight, offset);
  for(int i = 0; i<64; i++){
    printf("%" PRIu8 " ", block[i]);
  }
  printf("\n \n");
  free(block);
}

void test_biweight_h264_pixels16_8_c(){
    srand(59);
    uint8_t *_src = rand_array_u8(64);
    uint8_t _dst[64] = { 0 };
    ptrdiff_t stride = 16;
    int height = 4;
    int log2_denom = 4;
    int weight = 8;
    int offset = 4;
    biweight_h264_pixels16_8_c(_dst, _src, stride, height, log2_denom, weight, weight, offset);
    for(int i = 0; i<64; i++){
        printf("%" PRIu8 " ", _dst[i]);
    }
    printf("\n \n");
    free(_src);
}

void test_put_h264_chroma_mc8(){
    srand(49);
    uint8_t *_src = rand_array_u8(128);
    int h = 8;
    ptrdiff_t stride = 8;
    uint8_t _dst[64] = { 0 };
    int x = 2;
    int y = 4;
    put_h264_chroma_mc8(_dst, _src, stride, h, x, y);
    for(int i = 0; i<64; i++){
        printf("%" PRIu8 " ", _dst[i]);
    }
    printf("\n \n");
    free(_src);
}

void test_avg_h264_chroma_mc8(){
    srand(38);
    uint8_t *_src = rand_array_u8(128);
    int h = 8;
    ptrdiff_t stride = 8;
    uint8_t _dst[64] = { 0 };
    int x = 2;
    int y = 4;
    avg_h264_chroma_mc8(_dst, _src, stride, h, x, y);
    for(int i = 0; i<64; i++){
        printf("%" PRIu8 " ", _dst[i]);
    }
    printf("\n \n");
    free(_src);
}

void test_cavs_idct8_add_c(){
    int stride = 8;
    srand(71);
    uint8_t dst[64] = { 0 };
    int16_t *block = rand_array_16(64);
    for (int i = 0; i < 64; i++)
	    block[i] >>= 8;
    cavs_idct8_add_c(dst, block, stride);
    for(int i = 0; i<64; i++){
        printf("%" PRIu8 " ", dst[i]);
    }
    printf("\n \n");
    free(block);
}





int main(){
    test_ff_h264_idct_add();
    // test_scalarproduct_and_madd_int16();
    // test_scalarproduct_and_madd_int32();
    test_ff_h264_luma_dc_dequant_idct();
    test_ps_stereo_interpolate();
    test_ps_stereo_interpolate_ipdopd();
    test_h263_h_loop_filter();
    test_h263_v_loop_filter();
    test_weight_h264_pixels16_8_c();
    test_biweight_h264_pixels16_8_c();  
    test_put_h264_chroma_mc8();
    test_avg_h264_chroma_mc8();
    test_cavs_idct8_add_c();
}
