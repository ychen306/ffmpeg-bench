#include <iostream>
#include <stdint.h>
#include <string.h>
#define SUINT int16_t
#define pixel uint8_t
#define dctcoef int16_t
static inline uint8_t av_clip_pixel(int x) {
  if (x < 0)
    return 0;
  if (x > 255)
    return 255;
  return x;
}

void ff_h264_idct_add(uint8_t *_dst, int16_t *_block, int stride){
	int i;
	pixel *dst = (pixel*)_dst;
    dctcoef *block = (dctcoef*)_block;
   	stride >>= sizeof(pixel)-1;
   	block[0] += 1 << 5;

	with_threads(i, 4, 4){
		vegen::block<int, 16> int_block;
		const SUINT z0=  block[i + 4*0]     +  (uint16_t)block[i + 4*2];
        const SUINT z1=  block[i + 4*0]     -  (uint16_t)block[i + 4*2];
        const SUINT z2= (block[i + 4*1]>>1) -  (uint16_t)block[i + 4*3];
        const SUINT z3=  block[i + 4*1]     + (uint16_t)(block[i + 4*3]>>1);

		block[i + 4*0] = int_block(i + 4*0)= z0 + z3;
		block[i + 4*1] = int_block(i + 4*1)= z1 + z2;
		block[i + 4*2] = int_block(i + 4*2)= z1 - z2;
		block[i + 4*3] = int_block(i + 4*3)= z0 - z3;

		vegen::sync();

		const SUINT z4=  int_block(0 + 4*i)     +  (SUINT)int_block(2 + 4*i);
        const SUINT z5=  int_block(0 + 4*i)     -  (SUINT)int_block(2 + 4*i);
        const SUINT z6= (int_block(1 + 4*i)>>1) -  (SUINT)int_block(3 + 4*i);
	    const SUINT z7=  int_block(1 + 4*i)     + (SUINT)(int_block(3 + 4*i)>>1);

		dst[i + 0*stride]= av_clip_pixel(dst[i + 0*stride] + ((int)(z4 + z7) >> 6));
		dst[i + 1*stride]= av_clip_pixel(dst[i + 1*stride] + ((int)(z5 + z6) >> 6));
		dst[i + 2*stride]= av_clip_pixel(dst[i + 2*stride] + ((int)(z5 - z6) >> 6));
		dst[i + 3*stride]= av_clip_pixel(dst[i + 3*stride] + ((int)(z4 - z7) >> 6));

	}
}


void ff_h264_luma_dc_dequant_idct(int16_t *_output, int16_t *_input,int qmul){
	#define stride 16
	int i;
    static const uint8_t x_offset[4]={0, 2*stride, 8*stride, 10*stride};
    dctcoef *input = (dctcoef*)_input;
    dctcoef *output = (dctcoef*)_output;

	with_threads(i, 4, 4){
		vegen::block<int, 16> temp;

		const int16_t z0= input[4*i+0] + input[4*i+1];
        const int16_t z1= input[4*i+0] - input[4*i+1];
        const int16_t z2= input[4*i+2] - input[4*i+3];
        const int16_t z3= input[4*i+2] + input[4*i+3];

        temp(4*i+0)= z0+z3;
        temp(4*i+1)= z0-z3;
        temp(4*i+2)= z1-z2;
        temp(4*i+3)= z1+z2;

		vegen::sync();

		const int16_t offset= x_offset[i];
        const SUINT z4= temp(4*0+i) + temp(4*2+i);
        const SUINT z5= temp(4*0+i) - temp(4*2+i);
        const SUINT z6= temp(4*1+i) - temp(4*3+i);
        const SUINT z7= temp(4*1+i) + temp(4*3+i);

		output[stride* 0+offset]= (int16_t)((z4 + z7)*qmul + 128 ) >> 8;
        output[stride* 1+offset]= (int16_t)((z5 + z6)*qmul + 128 ) >> 8;
        output[stride* 4+offset]= (int16_t)((z5 - z6)*qmul + 128 ) >> 8;
        output[stride* 5+offset]= (int16_t)((z4 - z7)*qmul + 128 ) >> 8;
	}
#undef stride
}



