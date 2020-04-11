/*
 * Copyright 2020 Xinyue Lu
 *
 * Temporal Median implementation, AVX2 SIMD code.
 *
 */

#include "tmedian_SIMD.hpp"

template <> __m256i min<uint8_t,  __m256i>(__m256i a, __m256i b) { return _mm256_min_epu8 (a, b); }
template <> __m256i max<uint8_t,  __m256i>(__m256i a, __m256i b) { return _mm256_max_epu8 (a, b); }
template <> __m256i min<uint16_t, __m256i>(__m256i a, __m256i b) { return _mm256_min_epu16(a, b); }
template <> __m256i max<uint16_t, __m256i>(__m256i a, __m256i b) { return _mm256_max_epu16(a, b); }
template <> __m256  min<float,    __m256> (__m256  a, __m256  b) { return _mm256_min_ps   (a, b); }
template <> __m256  max<float,    __m256> (__m256  a, __m256  b) { return _mm256_max_ps   (a, b); }
template <> __m256i load<__m256i>(const __m256i* mem) { return _mm256_load_si256(mem); }
template <> void store<__m256i>(__m256i* mem, __m256i a) { return _mm256_store_si256(mem, a); }
template <> __m256 load<__m256>(const __m256* mem) { return _mm256_castsi256_ps(_mm256_load_si256((const __m256i*)mem)); }
template <> void store<__m256>(__m256* mem, __m256 a) { return _mm256_store_si256((__m256i*)mem, _mm256_castps_si256(a)); }

template void tmedian_SIMD_256<uint8_t >(int, const unsigned char**, unsigned char*, int, int, int);
template void tmedian_SIMD_256<uint16_t>(int, const unsigned char**, unsigned char*, int, int, int);
template void tmedian_SIMD_256<float   >(int, const unsigned char**, unsigned char*, int, int, int);
