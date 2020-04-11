/*
 * Copyright 2020 Xinyue Lu
 *
 * Temporal Median implementation, SSE4.1 SIMD code.
 *
 */

#include "tmedian_SIMD.hpp"

template <> __m128i min<uint8_t,  __m128i>(__m128i a, __m128i b) { return _mm_min_epu8 (a, b); }
template <> __m128i max<uint8_t,  __m128i>(__m128i a, __m128i b) { return _mm_max_epu8 (a, b); }
template <> __m128i min<uint16_t, __m128i>(__m128i a, __m128i b) { return _mm_min_epu16(a, b); }
template <> __m128i max<uint16_t, __m128i>(__m128i a, __m128i b) { return _mm_max_epu16(a, b); }
template <> __m128  min<float,    __m128> (__m128  a, __m128  b) { return _mm_min_ps   (a, b); }
template <> __m128  max<float,    __m128> (__m128  a, __m128  b) { return _mm_max_ps   (a, b); }
template <> __m128i load<__m128i>(const __m128i* mem) { return _mm_load_si128(mem); }
template <> void store<__m128i>(__m128i* mem, __m128i a) { return _mm_store_si128(mem, a); }
template <> __m128 load<__m128>(const __m128* mem) { return _mm_castsi128_ps(_mm_load_si128((const __m128i*)mem)); }
template <> void store<__m128>(__m128* mem, __m128 a) { return _mm_store_si128((__m128i*)mem, _mm_castps_si128(a)); }

template void tmedian_SIMD_128<uint8_t >(int, const unsigned char**, unsigned char*, int, int, int);
template void tmedian_SIMD_128<uint16_t>(int, const unsigned char**, unsigned char*, int, int, int);
template void tmedian_SIMD_128<float   >(int, const unsigned char**, unsigned char*, int, int, int);
