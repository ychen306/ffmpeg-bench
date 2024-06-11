//https://github.com/FFmpeg/FFmpeg/blob/9dac8495b0020f7326bbe79efa070a0150109afc/libavcodec/lossless_audiodsp.c#L27

#include <stdint.h>

int scalarproduct_and_madd_int16(int16_t *v1, const int16_t *v2,
                                              const int16_t *v3,
                                              int order, int mul)
{
    unsigned res = 0;

    do {
        res   += *v1 * *v2++;
        *v1++ += mul * *v3++;
        res   += *v1 * *v2++;
        *v1++ += mul * *v3++;
    } while (order-=2);
    return res;
}
