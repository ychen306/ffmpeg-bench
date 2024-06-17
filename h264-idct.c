#include <stdint.h>
#include <string.h>

#define SUINT int

__attribute__((always_inline))  
static uint8_t av_clip_pixel(int x) {
  if (x < 0)
    return 0;
  if (x > 255)
    return 255;
  return x;
}

#define pixel uint8_t
#define dctcoef int16_t

// https://github.com/FFmpeg/FFmpeg/blob/release/7.0/libavcodec/h264idct_template.c#L32
void ff_h264_idct_add(uint8_t *__restrict__ _dst, int16_t *__restrict__ _block, int stride)
{
    int i;
    pixel *dst = (pixel*)_dst;
    dctcoef *block = (dctcoef*)_block;
    stride >>= sizeof(pixel)-1;

    block[0] += 1 << 5;

    for(i=0; i<4; i++){
        const SUINT z0=  block[i + 4*0]     +  (unsigned)block[i + 4*2];
        const SUINT z1=  block[i + 4*0]     -  (unsigned)block[i + 4*2];
        const SUINT z2= (block[i + 4*1]>>1) -  (unsigned)block[i + 4*3];
        const SUINT z3=  block[i + 4*1]     + (unsigned)(block[i + 4*3]>>1);

        block[i + 4*0]= z0 + z3;
        block[i + 4*1]= z1 + z2;
        block[i + 4*2]= z1 - z2;
        block[i + 4*3]= z0 - z3;
    }

    for(i=0; i<4; i++){
        const SUINT z0=  block[0 + 4*i]     +  (SUINT)block[2 + 4*i];
        const SUINT z1=  block[0 + 4*i]     -  (SUINT)block[2 + 4*i];
        const SUINT z2= (block[1 + 4*i]>>1) -  (SUINT)block[3 + 4*i];
        const SUINT z3=  block[1 + 4*i]     + (SUINT)(block[3 + 4*i]>>1);

        dst[i + 0*stride]= av_clip_pixel(dst[i + 0*stride] + ((int)(z0 + z3) >> 6));
        dst[i + 1*stride]= av_clip_pixel(dst[i + 1*stride] + ((int)(z1 + z2) >> 6));
        dst[i + 2*stride]= av_clip_pixel(dst[i + 2*stride] + ((int)(z1 - z2) >> 6));
        dst[i + 3*stride]= av_clip_pixel(dst[i + 3*stride] + ((int)(z0 - z3) >> 6));
    }

    memset(block, 0, 16 * sizeof(dctcoef));
}

// https://github.com/FFmpeg/FFmpeg/blob/94f2274a8b61438572f0873ccf430e55ce0e0e2b/libavcodec/h264idct_template.c#L251

void ff_h264_luma_dc_dequant_idct(int16_t *_output, int16_t *_input, int qmul){
#define stride 16
    int i;
    int temp[16];
    static const uint8_t x_offset[4]={0, 2*stride, 8*stride, 10*stride};
    dctcoef *input = (dctcoef*)_input;
    dctcoef *output = (dctcoef*)_output;

    for(i=0; i<4; i++){
        const int z0= input[4*i+0] + input[4*i+1];
        const int z1= input[4*i+0] - input[4*i+1];
        const int z2= input[4*i+2] - input[4*i+3];
        const int z3= input[4*i+2] + input[4*i+3];

        temp[4*i+0]= z0+z3;
        temp[4*i+1]= z0-z3;
        temp[4*i+2]= z1-z2;
        temp[4*i+3]= z1+z2;
    }

    for(i=0; i<4; i++){
        const int offset= x_offset[i];
        const SUINT z0= temp[4*0+i] + temp[4*2+i];
        const SUINT z1= temp[4*0+i] - temp[4*2+i];
        const SUINT z2= temp[4*1+i] - temp[4*3+i];
        const SUINT z3= temp[4*1+i] + temp[4*3+i];

        output[stride* 0+offset]= (int)((z0 + z3)*qmul + 128 ) >> 8;
        output[stride* 1+offset]= (int)((z1 + z2)*qmul + 128 ) >> 8;
        output[stride* 4+offset]= (int)((z1 - z2)*qmul + 128 ) >> 8;
        output[stride* 5+offset]= (int)((z0 - z3)*qmul + 128 ) >> 8;
    }
#undef stride
}
