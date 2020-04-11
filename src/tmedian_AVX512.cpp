/*
 * Copyright 2020 Xinyue Lu
 *
 * Temporal Median implementation, AVX512 SIMD code.
 *
 */

#include "tmedian_SIMD.hpp"

template <> __m512i min<uint8_t,  __m512i>(__m512i a, __m512i b) { return _mm512_min_epu8 (a, b); }
template <> __m512i max<uint8_t,  __m512i>(__m512i a, __m512i b) { return _mm512_max_epu8 (a, b); }
template <> __m512i min<uint16_t, __m512i>(__m512i a, __m512i b) { return _mm512_min_epu16(a, b); }
template <> __m512i max<uint16_t, __m512i>(__m512i a, __m512i b) { return _mm512_max_epu16(a, b); }
template <> __m512  min<float,    __m512> (__m512  a, __m512  b) { return _mm512_min_ps   (a, b); }
template <> __m512  max<float,    __m512> (__m512  a, __m512  b) { return _mm512_max_ps   (a, b); }
template <> __m512i load<__m512i>(const __m512i* mem) { return _mm512_load_si512(mem); }
template <> void store<__m512i>(__m512i* mem, __m512i a) { return _mm512_store_si512(mem, a); }
template <> __m512 load<__m512>(const __m512* mem) { return _mm512_castsi512_ps(_mm512_load_si512((const __m512i*)mem)); }
template <> void store<__m512>(__m512* mem, __m512 a) { return _mm512_store_si512((__m512i*)mem, _mm512_castps_si512(a)); }

template void tmedian_SIMD_512<uint8_t >(int, const unsigned char**, unsigned char*, int, int, int);
template void tmedian_SIMD_512<uint16_t>(int, const unsigned char**, unsigned char*, int, int, int);
template void tmedian_SIMD_512<float   >(int, const unsigned char**, unsigned char*, int, int, int);
