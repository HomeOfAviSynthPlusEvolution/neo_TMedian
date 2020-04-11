/*
 * Copyright 2020 Xinyue Lu
 *
 * Temporal Median - filter.
 *
 */

#pragma once

#include "tmedian_common.h"

int GetCPUFlags();

struct TMedian final : Filter {
  int process[4] {2, 2, 2, 2};
  int radius {1};
  InDelegator* _in;
  bool crop;

  void (*tmedian_SIMD)(int radius, const unsigned char** in_ptr, unsigned char* out_ptr, int width, int height, int stride_bytes);

  const char* VSName() const override { return "TemporalMedian"; }
  const char* AVSName() const override { return "neo_tmedian"; }
  const MtMode AVSMode() const override { return MT_NICE_FILTER; }
  const VSFilterMode VSMode() const override { return fmParallel; }
  const std::vector<Param> Params() const override {
    return std::vector<Param> {
      Param {"clip", Clip, false, true, true, false},
      Param {"radius", Integer},
      Param {"planes", Integer, true},
      Param {"y", Integer, false, true, false},
      Param {"u", Integer, false, true, false},
      Param {"v", Integer, false, true, false}
    };
  }
  void Initialize(InDelegator* in, DSVideoInfo in_vi, FetchFrameFunctor* fetch_frame) override
  {
    Filter::Initialize(in, in_vi, fetch_frame);
    in->Read("radius", radius);

    try {
      process[0] =
      process[1] =
      process[2] =
      process[3] = 2;
      std::vector<int> user_planes {0, 1, 2};
      in->Read("planes", user_planes);
      for (auto &&p : user_planes)
      {
        if (p < in_vi.Format.Planes)
          process[p] = 3;
      }
    }
    catch (const char *) {
      process[0] =
      process[1] =
      process[2] = 3;
      in->Read("y", process[0]);
      in->Read("u", process[1]);
      in->Read("v", process[2]);
    }

    int CPUFlags = GetCPUFlags();
    if (CPUFlags & CPUF_SSE4_1) {
      switch (in_vi.Format.BytesPerSample) {
        case 1: tmedian_SIMD = tmedian_SIMD_128<uint8_t>; break;
        case 2: tmedian_SIMD = tmedian_SIMD_128<uint16_t>; break;
        case 4: tmedian_SIMD = tmedian_SIMD_128<float_t>; break;
      }
    }
    if (CPUFlags & CPUF_AVX2) {
      switch (in_vi.Format.BytesPerSample) {
        case 1: tmedian_SIMD = tmedian_SIMD_256<uint8_t>; break;
        case 2: tmedian_SIMD = tmedian_SIMD_256<uint16_t>; break;
        case 4: tmedian_SIMD = tmedian_SIMD_256<float_t>; break;
      }
    }
    if (CPUFlags & CPUF_AVX512F) {
      switch (in_vi.Format.BytesPerSample) {
        case 1: tmedian_SIMD = tmedian_SIMD_512<uint8_t>; break;
        case 2: tmedian_SIMD = tmedian_SIMD_512<uint16_t>; break;
        case 4: tmedian_SIMD = tmedian_SIMD_512<float_t>; break;
      }
    }
  }

  std::vector<int> RequestReferenceFrames(int n) const override
  {
    std::vector<int> req;
    int rad = radius;
    if (rad > n) rad = n;
    if (rad > in_vi.Frames - 1 - n) rad = in_vi.Frames - 1 - n;

    for (int i = n-rad; i <= n+rad; i++)
      req.push_back(i);
    
    return req;
  }

  DSFrame GetFrame(int n, std::unordered_map<int, DSFrame> in_frames) override
  {
    auto src = in_frames[n];
    unsigned int real_radius = static_cast<int>(in_frames.size()) / 2; // Real radius
    if (real_radius == 0)
      return src;
    auto dst = src.Create(false);
    std::vector<unsigned char> samples(in_frames.size());
    for (int p = 0; p < in_vi.Format.Planes; p++)
    {
      bool chroma = in_vi.Format.IsFamilyYUV && p > 0 && p < 3;
      auto height = in_vi.Height;
      auto width = in_vi.Width;
      auto stride = src.StrideBytes[p];
      auto src_ptr = src.SrcPointers[p];
      auto dst_ptr = dst.DstPointers[p];

      if (chroma) {
        height >>= in_vi.Format.SSH;
        width >>= in_vi.Format.SSW;
      }

      if (process[p] == 2) {
        memcpy(dst_ptr, src_ptr, stride * height);
        continue;
      }
      if (process[p] != 3)
        continue;

      // Main code
      const unsigned char* src_ptrp[MAX_FRAMES];
      int idx = 0;
      if (real_radius <= MAX_FRAMES / 2) {
        // SIMD
        for (auto &&fv : in_frames)
          src_ptrp[idx++] = fv.second.SrcPointers[p];
        tmedian_SIMD(real_radius, src_ptrp, dst_ptr, width, height, stride);
      }
      else
        // C
        for (int h = 0; h < height; h++)
          for (int w = 0; w < width; w++)
          {
            int idx = 0;
            for (auto &&fv : in_frames)
              samples[idx++] = fv.second.SrcPointers[p][h * stride + w];
            std::nth_element(samples.begin(), samples.begin() + real_radius, samples.end());
            dst_ptr[h * stride + w] = samples[real_radius];
          }
    }

    return dst;
  }

  ~TMedian() = default;
};
