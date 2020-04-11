/*
 * Copyright 2020 Xinyue Lu
 *
 * Temporal Median implementation, shared SIMD code.
 *
 */

#pragma once
#include "tmedian_common.h"

#if defined(__clang__)
  #ifndef __AVX512F__
    class __m512 {};
    class __m512i {};
  #endif
  #ifndef __AVX2__
    class __m256 {};
    class __m256i {};
  #endif
#endif

template <typename T, typename ST> 
static inline void swap2(ST &a, ST &b) {
  auto minab = min<T>(a, b);
  auto maxab = max<T>(a, b);
  a = minab;
  b = maxab;
}

template <typename T, typename ST> 
static inline ST median3(ST a, ST b, ST c) {
  swap2<T>(a, b);
  return max<T>(a, min<T>(c, b));
}

template<typename T, typename ST>
static inline void tmedian_SIMD_r1(const T** in_ptr, T* out_ptr, int width, int height, int offset)
{
  ST _s[3];
  for (int i = 0; i < 3; i++)
    _s[i] = load<ST>(reinterpret_cast<const ST*>(in_ptr[i] + offset));

  auto med = median3<T, ST>(_s[0], _s[1], _s[2]);
  store<ST>(reinterpret_cast<ST*>(out_ptr + offset), med);
}

template<typename T, typename ST>
static inline void tmedian_SIMD_r2(const T** in_ptr, T* out_ptr, int width, int height, int offset)
{
  ST _s[5];
  for (int i = 0; i < 5; i++)
    _s[i] = load<ST>(reinterpret_cast<const ST*>(in_ptr[i] + offset));

  swap2<T>(_s[0], _s[1]); swap2<T>(_s[2], _s[3]);
  swap2<T>(_s[0], _s[2]); swap2<T>(_s[1], _s[3]); // Throw s0 s3
  // Note: thrown variables are optimized away by compiler.
  auto med = median3<T, ST>(_s[1], _s[2], _s[4]);
  store<ST>(reinterpret_cast<ST*>(out_ptr + offset), med);
}

template<typename T, typename ST>
static inline void tmedian_SIMD_r3(const T** in_ptr, T* out_ptr, int width, int height, int offset)
{
  ST _s[7];
  for (int i = 0; i < 7; i++)
    _s[i] = load<ST>(reinterpret_cast<const ST*>(in_ptr[i] + offset));

  swap2<T>(_s[1], _s[2]); swap2<T>(_s[3], _s[4]);
  swap2<T>(_s[0], _s[2]); swap2<T>(_s[3], _s[5]);
  swap2<T>(_s[0], _s[1]); swap2<T>(_s[4], _s[5]);
  swap2<T>(_s[0], _s[4]); swap2<T>(_s[1], _s[5]); // Throw s0 s5
  swap2<T>(_s[1], _s[3]); swap2<T>(_s[2], _s[4]); // Throw s1 s4
  auto med = median3<T, ST>(_s[2], _s[3], _s[6]);
  store<ST>(reinterpret_cast<ST*>(out_ptr + offset), med);
}

template<typename T, typename ST>
static inline void tmedian_SIMD_r4(const T** in_ptr, T* out_ptr, int width, int height, int offset)
{
  ST _s[9];
  for (int i = 0; i < 9; i++)
    _s[i] = load<ST>(reinterpret_cast<const ST*>(in_ptr[i] + offset));

  swap2<T>(_s[0], _s[1]); swap2<T>(_s[2], _s[3]);
  swap2<T>(_s[4], _s[5]); swap2<T>(_s[6], _s[7]);

  swap2<T>(_s[0], _s[2]); swap2<T>(_s[1], _s[3]);
  swap2<T>(_s[4], _s[6]); swap2<T>(_s[5], _s[7]);

  swap2<T>(_s[0], _s[4]); swap2<T>(_s[1], _s[2]); // Throw s0
  swap2<T>(_s[5], _s[6]); swap2<T>(_s[3], _s[7]); // Throw s7

  swap2<T>(_s[1], _s[5]); swap2<T>(_s[2], _s[6]); // Throw s1 s6
  swap2<T>(_s[2], _s[4]); swap2<T>(_s[3], _s[5]); // Throw s2 s5

  auto med = median3<T, ST>(_s[3], _s[4], _s[8]);
  store<ST>(reinterpret_cast<ST*>(out_ptr + offset), med);
}

template<typename T, typename ST>
static inline void tmedian_SIMD_r5(const T** in_ptr, T* out_ptr, int width, int height, int offset)
{
  ST _s[11];
  for (int i = 0; i < 11; i++)
    _s[i] = load<ST>(reinterpret_cast<const ST*>(in_ptr[i] + offset));

  swap2<T>(_s[0], _s[9]);
  swap2<T>(_s[1], _s[2]); swap2<T>(_s[3], _s[4]);
  swap2<T>(_s[5], _s[6]); swap2<T>(_s[7], _s[8]);

  swap2<T>(_s[1], _s[8]);
  swap2<T>(_s[0], _s[2]); swap2<T>(_s[3], _s[5]);
  swap2<T>(_s[0], _s[3]); // Throw s0
  swap2<T>(_s[4], _s[6]); swap2<T>(_s[7], _s[9]);
  swap2<T>(_s[6], _s[9]); // Throw s9
  swap2<T>(_s[2], _s[7]);

  swap2<T>(_s[4], _s[5]);
  swap2<T>(_s[4], _s[8]);
  swap2<T>(_s[1], _s[5]);

  swap2<T>(_s[1], _s[2]); swap2<T>(_s[3], _s[4]); // Throw s1 s3
  swap2<T>(_s[5], _s[6]); swap2<T>(_s[7], _s[8]); // Throw s6 s8
  swap2<T>(_s[2], _s[4]); swap2<T>(_s[5], _s[7]); // Throw s2 s7

  auto med = median3<T, ST>(_s[4], _s[5], _s[10]);
  store<ST>(reinterpret_cast<ST*>(out_ptr + offset), med);
}

template<typename T, typename ST>
static inline void tmedian_SIMD_r6(const T** in_ptr, T* out_ptr, int width, int height, int offset)
{
  ST _s[13];
  for (int i = 0; i < 13; i++)
    _s[i] = load<ST>(reinterpret_cast<const ST*>(in_ptr[i] + offset));

  swap2<T>(_s[0], _s[1]); swap2<T>(_s[2], _s[3]);
  swap2<T>(_s[4], _s[5]); swap2<T>(_s[6], _s[7]);
  swap2<T>(_s[8], _s[9]); swap2<T>(_s[10], _s[11]);

  swap2<T>(_s[0], _s[2]); swap2<T>(_s[1], _s[3]);
  swap2<T>(_s[4], _s[6]); swap2<T>(_s[5], _s[7]);
  swap2<T>(_s[8], _s[10]); swap2<T>(_s[9], _s[11]);
  swap2<T>(_s[7], _s[11]); // Throw s11

  swap2<T>(_s[2], _s[6]);
  swap2<T>(_s[1], _s[5]);
  swap2<T>(_s[1], _s[2]);
  swap2<T>(_s[9], _s[10]);
  swap2<T>(_s[6], _s[10]);
  swap2<T>(_s[5], _s[9]);
  swap2<T>(_s[0], _s[4]); // Throw s0
  swap2<T>(_s[9], _s[10]); // Throw s10
  swap2<T>(_s[4], _s[8]); // Throw s4
  swap2<T>(_s[3], _s[7]); // Throw s7
  swap2<T>(_s[2], _s[6]);
  swap2<T>(_s[1], _s[5]); // Throw s1
  swap2<T>(_s[0], _s[4]);

  swap2<T>(_s[3], _s[8]);

  swap2<T>(_s[2], _s[3]); // Throw s2
  swap2<T>(_s[5], _s[6]);
  swap2<T>(_s[8], _s[9]); // Throw s9

  swap2<T>(_s[3], _s[5]); // Throw s3
  swap2<T>(_s[6], _s[8]); // Throw s8

  auto med = median3<T, ST>(_s[4], _s[5], _s[12]);
  store<ST>(reinterpret_cast<ST*>(out_ptr + offset), med);
}

template<typename T, typename ST>
void tmedian_SIMD_impl(int radius, const unsigned char** in_ptr, unsigned char* out_ptr, int width, int height, int stride_bytes)
{
  int stride = stride_bytes / sizeof(T);
  const T* src_ptr[MAX_FRAMES];
  for (int i = 0; i <= radius * 2; i++)
    src_ptr[i] = reinterpret_cast<const T*>(in_ptr[i]);
  T* dst_ptr = reinterpret_cast<T*>(out_ptr);

  int length = 16;
  if constexpr(std::is_same<ST, __m128>::value || std::is_same<ST, __m128i>::value)
    length = 16;
  else if constexpr(std::is_same<ST, __m256>::value || std::is_same<ST, __m256i>::value)
    length = 32;
  else if constexpr(std::is_same<ST, __m512>::value || std::is_same<ST, __m512i>::value)
    length = 64;
  int step = length / sizeof(T);
  
  for (int h = 0; h < height; h++)
    for (int w = 0; w < width; w += step) {
      int offset = h * stride + w;
      switch(radius) {
        case 1: tmedian_SIMD_r1<T, ST>(src_ptr, dst_ptr, width, height, offset); break;
        case 2: tmedian_SIMD_r2<T, ST>(src_ptr, dst_ptr, width, height, offset); break;
        case 3: tmedian_SIMD_r3<T, ST>(src_ptr, dst_ptr, width, height, offset); break;
        case 4: tmedian_SIMD_r4<T, ST>(src_ptr, dst_ptr, width, height, offset); break;
        case 5: tmedian_SIMD_r5<T, ST>(src_ptr, dst_ptr, width, height, offset); break;
        case 6: tmedian_SIMD_r6<T, ST>(src_ptr, dst_ptr, width, height, offset); break;
      }
    }
}

template<typename T>
void tmedian_SIMD_128(int radius, const unsigned char** in_ptr, unsigned char* out_ptr, int width, int height, int stride_bytes) {
  if constexpr(std::is_same<T, float>::value)
    tmedian_SIMD_impl<T, __m128>(radius, in_ptr, out_ptr, width, height, stride_bytes);
  else
    tmedian_SIMD_impl<T, __m128i>(radius, in_ptr, out_ptr, width, height, stride_bytes);
}
template<typename T>
void tmedian_SIMD_256(int radius, const unsigned char** in_ptr, unsigned char* out_ptr, int width, int height, int stride_bytes) {
  if constexpr(std::is_same<T, float>::value)
    tmedian_SIMD_impl<T, __m256>(radius, in_ptr, out_ptr, width, height, stride_bytes);
  else
    tmedian_SIMD_impl<T, __m256i>(radius, in_ptr, out_ptr, width, height, stride_bytes);
}
template<typename T>
void tmedian_SIMD_512(int radius, const unsigned char** in_ptr, unsigned char* out_ptr, int width, int height, int stride_bytes) {
  if constexpr(std::is_same<T, float>::value)
    tmedian_SIMD_impl<T, __m512>(radius, in_ptr, out_ptr, width, height, stride_bytes);
  else
    tmedian_SIMD_impl<T, __m512i>(radius, in_ptr, out_ptr, width, height, stride_bytes);
}
