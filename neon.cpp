#include "common.h"
#include <arm_neon.h>

/* SATD SA8D variants - based on x264 */
inline static void SUMSUB_AB(int16x8_t &sum, int16x8_t &sub, const int16x8_t a, const int16x8_t b)
{
    sum = vaddq_s16(a, b);
    sub = vsubq_s16(a, b);
}

inline static void transpose_8h_8h(int16x8_t &t1, int16x8_t &t2,
                                   const int16x8_t s1, const int16x8_t s2)
{
    t1 = vtrn1q_s16(s1, s2);
    t2 = vtrn2q_s16(s1, s2);
}

inline static void transpose_4s_8h(int16x8_t &t1, int16x8_t &t2,
                                   const int16x8_t s1, const int16x8_t s2)
{
    int32x4_t tmp1 = vreinterpretq_s32_s16(s1);
    int32x4_t tmp2 = vreinterpretq_s32_s16(s2);

    t1 = vreinterpretq_s16_s32(vtrn1q_s32(tmp1, tmp2));
    t2 = vreinterpretq_s16_s32(vtrn2q_s32(tmp1, tmp2));
}

inline static void transpose_2d_8h(int16x8_t &t1, int16x8_t &t2,
                                   const int16x8_t s1, const int16x8_t s2)
{
    int64x2_t tmp1 = vreinterpretq_s64_s16(s1);
    int64x2_t tmp2 = vreinterpretq_s64_s16(s2);

    t1 = vreinterpretq_s16_s64(vtrn1q_s64(tmp1, tmp2));
    t2 = vreinterpretq_s16_s64(vtrn2q_s64(tmp1, tmp2));
}

inline static int _satd_4x4_neon(int16x8_t v0, int16x8_t v1)
{
    int16x8_t v2, v3;
    SUMSUB_AB(v2,  v3,  v0,  v1);

    transpose_2d_8h(v0, v1, v2, v3);
    SUMSUB_AB(v2,  v3,  v0,  v1);

    transpose_8h_8h(v0, v1, v2, v3);
    SUMSUB_AB(v2,  v3,  v0,  v1);

    transpose_4s_8h(v0, v1, v2, v3);

    uint16x8_t abs0 = vreinterpretq_u16_s16(vabsq_s16(v0));
    uint16x8_t abs1 = vreinterpretq_u16_s16(vabsq_s16(v1));
    uint16x8_t max = vmaxq_u16(abs0, abs1);

    return vaddlvq_u16(max);
}

inline static uint8x8_t load_u8x4x2(const uint8_t *s, intptr_t stride)
{
    uint8x8_t ret = vdup_n_u8(0);

    ret = vreinterpret_u8_u32(vld1_lane_u32((const uint32_t*)s,
                                            vreinterpret_u32_u8(ret), 0));
    s += stride;
    ret = vreinterpret_u8_u32(vld1_lane_u32((const uint32_t*)s,
                                            vreinterpret_u32_u8(ret), 1));

    return ret;
}

int pixel_satd_4x4_neon(const uint8_t *pix1, intptr_t stride_pix1, const uint8_t *pix2, intptr_t stride_pix2)
{
    uint8x8_t t0 = load_u8x4x2(pix1, 2 * stride_pix1);
    uint8x8_t t1 = load_u8x4x2(pix1 + stride_pix1, 2 * stride_pix1);

    uint8x8_t r0 = load_u8x4x2(pix2, 2 * stride_pix2);
    uint8x8_t r1 = load_u8x4x2(pix2 + stride_pix2, 2 * stride_pix2);

    return _satd_4x4_neon(vreinterpretq_s16_u16(vsubl_u8(t0, r0)),
                          vreinterpretq_s16_u16(vsubl_u8(r1, t1)));
}