/*
 * Copyright 2020 Xinyue Lu
 *
 * Temporal Median implementation, public header.
 *
 */

#pragma once
#include <unordered_map>
#include <immintrin.h>
#include <smmintrin.h>
#include <stdint.h>

#define MAX_FRAMES 13

template<typename T>
void tmedian_SIMD_128(int radius, const unsigned char** in_ptr, unsigned char* out_ptr, int width, int height, int stride_bytes);
template<typename T>
void tmedian_SIMD_256(int radius, const unsigned char** in_ptr, unsigned char* out_ptr, int width, int height, int stride_bytes);
template<typename T>
void tmedian_SIMD_512(int radius, const unsigned char** in_ptr, unsigned char* out_ptr, int width, int height, int stride_bytes);
template <typename T, typename ST> ST min(ST a, ST b);
template <typename T, typename ST> ST max(ST a, ST b);
template <typename ST> ST load(const ST* mem);
template <typename ST> void store(ST* mem, ST a);
