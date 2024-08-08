#include <stdint.h>

// assuming USED_FIXED is 1, if 0 see link below
#define AAC_MADD30(x, y, a, b) (int)((((int64_t)(x) * (y)) + \
                               ((int64_t)(a) * (b)) + \
                               0x20000000) >> 30)

#define AAC_MADD30_V8(x, y, a, b, c, d, e, f) (int)((((int64_t)(x) * (y)) + \
                        ((int64_t)(a) * (b)) + \
                        ((int64_t)(c) * (d)) + \
                        ((int64_t)(e) * (f)) + \
                        0x20000000) >> 30)

#define AAC_MSUB30_V8(x, y, a, b, c, d, e, f) (int)((((int64_t)(x) * (y)) + \
                        ((int64_t)(a) * (b)) - \
                        ((int64_t)(c) * (d)) - \
                        ((int64_t)(e) * (f)) + \
                        0x20000000) >> 30)


typedef int                 INTFLOAT;
typedef unsigned            UINTFLOAT;

// found definition in https://github.com/FFmpeg/FFmpeg/blob/94f2274a8b61438572f0873ccf430e55ce0e0e2b/libavcodec/aac_defines.h#L34C1-L35C39


// https://github.com/FFmpeg/FFmpeg/blob/94f2274a8b61438572f0873ccf430e55ce0e0e2b/libavcodec/aacpsdsp_template.c#L146

// stack and heap stuff

void ps_stereo_interpolate(INTFLOAT (*l)[2], INTFLOAT (*r)[2],INTFLOAT h[2][4],
    INTFLOAT h_step[ 2][4], int len)
{
    INTFLOAT h0 = h[0][0];
    INTFLOAT h1 = h[0][1];
    INTFLOAT h2 = h[0][2];
    INTFLOAT h3 = h[0][3];
    UINTFLOAT hs0 = h_step[0][0];
    UINTFLOAT hs1 = h_step[0][1];
    UINTFLOAT hs2 = h_step[0][2];
    UINTFLOAT hs3 = h_step[0][3];
    int n;

    with_threads(n, len, 2){
        //l is s, r is d
        INTFLOAT l_re = l[n][0];
        INTFLOAT l_im = l[n][1];
        INTFLOAT r_re = r[n][0];
        INTFLOAT r_im = r[n][1];
        h0 = h0 + (n+1)*hs0;
        h1 = h1 + (n+1)*hs1;
        h2 = h2 + (n+1)*hs2;
        h3 = h3 + (n+1)*hs3;
        l[n][0] = AAC_MADD30(h0, l_re, h2, r_re);
        l[n][1] = AAC_MADD30(h0, l_im, h2, r_im);
        r[n][0] = AAC_MADD30(h1, l_re, h3, r_re);
        r[n][1] = AAC_MADD30(h1, l_im, h3, r_im);
    }
}

void ps_stereo_interpolate_ipdopd(INTFLOAT (* l)[2], INTFLOAT (* r)[2],
                                           INTFLOAT h[ 2][4], INTFLOAT h_step[ 2][4],
                                           int len)
{
    INTFLOAT h00  = h[0][0],      h10  = h[1][0];
    INTFLOAT h01  = h[0][1],      h11  = h[1][1];
    INTFLOAT h02  = h[0][2],      h12  = h[1][2];
    INTFLOAT h03  = h[0][3],      h13  = h[1][3];
    UINTFLOAT hs00 = h_step[0][0], hs10 = h_step[1][0];
    UINTFLOAT hs01 = h_step[0][1], hs11 = h_step[1][1];
    UINTFLOAT hs02 = h_step[0][2], hs12 = h_step[1][2];
    UINTFLOAT hs03 = h_step[0][3], hs13 = h_step[1][3];
    int n;

    with_threads(n, len, 2){
        //l is s, r is d
        INTFLOAT l_re = l[n][0];
        INTFLOAT l_im = l[n][1];
        INTFLOAT r_re = r[n][0];
        INTFLOAT r_im = r[n][1];
        h00 = h00 + (n+1)* hs00;
        h01 = h01 + (n+1)* hs01;
        h02 = h02 + (n+1)* hs02;
        h03 = h03 + (n+1)* hs03;
        h10 = h10 + (n+1)* hs10;
        h11 = h11 + (n+1)* hs11;
        h12 = h12 + (n+1)* hs12;
        h13 = h13 + (n+1)* hs13;

        l[n][0] = AAC_MSUB30_V8(h00, l_re, h02, r_re, h10, l_im, h12, r_im);
        l[n][1] = AAC_MADD30_V8(h00, l_im, h02, r_im, h10, l_re, h12, r_re);
        r[n][0] = AAC_MSUB30_V8(h01, l_re, h03, r_re, h11, l_im, h13, r_im);
        r[n][1] = AAC_MADD30_V8(h01, l_im, h03, r_im, h11, l_re, h13, r_re);
    }
}