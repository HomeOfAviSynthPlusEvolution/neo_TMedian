# Neo Temporal Median

Neo Temporal Median Copyright(C) 2020 Xinyue Lu

Temporal Median is a temporal denoising filter. It replaces every pixel with the median of its temporal neighbourhood.

SSE4.1 is required to run optimized routine up to radius=6. AVX2 and AVX512BW routine is also available. AVX512BW is untested due to lack of hardware.

## Usage

```python
# AviSynth+
LoadPlugin("neo-tmedian.dll")
neo_tmedian(clip, radius=1, y=3, u=3, v=3, ...)
# VapourSynth
core.neo_tmedian.TemporalMedian(clip, radius=1, planes=[0,1,2], ...)
```

Parameters:

- *clip*

    Input clip. Currently supporting 8..16 bit integer and 32 bit float format.

- *radius*

    Neighbor frames radius. SIMD optimization supports up to 6.

    Default: 1.

- *y*, *u*, *v* (AviSynth+ only)

    Whether a plane is to be filtered.

        1 - Do not touch, leaving garbage data
        2 - Copy from origin
        3 - Process

    Default: 3.

- *planes* (VapourSynth only)

    Planes to be filtered.

    Default: [0,1,2].

## License

* MIT.
