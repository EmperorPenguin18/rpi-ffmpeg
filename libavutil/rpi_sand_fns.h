#ifndef AVUTIL_RPI_SAND_FNS
#define AVUTIL_RPI_SAND_FNS

#include "libavutil/frame.h"

// For all these fns _x & _w are measured as coord * PW
// For the C fns coords are in chroma pels (so luma / 2)
// Strides are in bytes

void av_rpi_sand_to_planar_y8(uint8_t * dst, const unsigned int dst_stride,
                             const uint8_t * src,
                             unsigned int stride1, unsigned int stride2,
                             unsigned int _x, unsigned int y,
                             unsigned int _w, unsigned int h);
void av_rpi_sand_to_planar_y16(uint8_t * dst, const unsigned int dst_stride,
                             const uint8_t * src,
                             unsigned int stride1, unsigned int stride2,
                             unsigned int _x, unsigned int y,
                             unsigned int _w, unsigned int h);

void av_rpi_sand_to_planar_c8(uint8_t * dst_u, const unsigned int dst_stride_u,
                             uint8_t * dst_v, const unsigned int dst_stride_v,
                             const uint8_t * src,
                             unsigned int stride1, unsigned int stride2,
                             unsigned int _x, unsigned int y,
                             unsigned int _w, unsigned int h);
void av_rpi_sand_to_planar_c16(uint8_t * dst_u, const unsigned int dst_stride_u,
                             uint8_t * dst_v, const unsigned int dst_stride_v,
                             const uint8_t * src,
                             unsigned int stride1, unsigned int stride2,
                             unsigned int _x, unsigned int y,
                             unsigned int _w, unsigned int h);

void av_rpi_planar_to_sand_c8(uint8_t * dst_c,
                             unsigned int stride1, unsigned int stride2,
                             const uint8_t * src_u, const unsigned int src_stride_u,
                             const uint8_t * src_v, const unsigned int src_stride_v,
                             unsigned int _x, unsigned int y,
                             unsigned int _w, unsigned int h);
void av_rpi_planar_to_sand_c16(uint8_t * dst_c,
                             unsigned int stride1, unsigned int stride2,
                             const uint8_t * src_u, const unsigned int src_stride_u,
                             const uint8_t * src_v, const unsigned int src_stride_v,
                             unsigned int _x, unsigned int y,
                             unsigned int _w, unsigned int h);

// w/h in pixels
void av_rpi_sand16_to_sand8(uint8_t * dst, const unsigned int dst_stride1, const unsigned int dst_stride2,
                         const uint8_t * src, const unsigned int src_stride1, const unsigned int src_stride2,
                         unsigned int w, unsigned int h, const unsigned int shr);


static inline unsigned int av_rpi_sand_frame_stride1(const AVFrame * const frame)
{
    // * We could repl;ace thios with a fixed 128 whic would allow the compiler
    //   to optimize a whole lot better
    return frame->linesize[0];
}

static inline unsigned int av_rpi_sand_frame_stride2(const AVFrame * const frame)
{
    return frame->linesize[3];
}


static inline int av_rpi_is_sand_format(const int format)
{
    return (format >= AV_PIX_FMT_SAND128 && format <= AV_PIX_FMT_SAND64_16);
}

static inline int av_rpi_is_sand_frame(const AVFrame * const frame)
{
    return av_rpi_is_sand_format(frame->format);
}

static inline int av_rpi_is_sand8_frame(const AVFrame * const frame)
{
    return (frame->format == AV_PIX_FMT_SAND128);
}

static inline int av_rpi_is_sand16_frame(const AVFrame * const frame)
{
    return (frame->format >= AV_PIX_FMT_SAND64_10 && frame->format <= AV_PIX_FMT_SAND64_16);
}

static inline int av_rpi_sand_frame_xshl(const AVFrame * const frame)
{
    return av_rpi_is_sand8_frame(frame) ? 0 : 1;
}

// If x is measured in bytes (not pixels) then this works for sand64_16 as
// well as sand128 - but in the general case we work that out

static inline unsigned int av_rpi_sand_frame_off_y(const AVFrame * const frame, const unsigned int x_y, const unsigned int y)
{
    const unsigned int stride1 = av_rpi_sand_frame_stride1(frame);
    const unsigned int stride2 = av_rpi_sand_frame_stride2(frame);
    const unsigned int x = x_y << av_rpi_sand_frame_xshl(frame);
    const unsigned int x1 = x & (stride1 - 1);
    const unsigned int x2 = x ^ x1;

    return x1 + stride1 * y + stride2 * x2;
}

static inline unsigned int av_rpi_sand_frame_off_c(const AVFrame * const frame, const unsigned int x_c, const unsigned int y_c)
{
    const unsigned int stride1 = av_rpi_sand_frame_stride1(frame);
    const unsigned int stride2 = av_rpi_sand_frame_stride2(frame);
    const unsigned int x = x_c << (av_rpi_sand_frame_xshl(frame) + 1);
    const unsigned int x1 = x & (stride1 - 1);
    const unsigned int x2 = x ^ x1;

    return x1 + stride1 * y_c + stride2 * x2;
}

static inline uint8_t * av_rpi_sand_frame_pos_y(const AVFrame * const frame, const unsigned int x, const unsigned int y)
{
    return frame->data[0] + av_rpi_sand_frame_off_y(frame, x, y);
}

static inline uint8_t * av_rpi_sand_frame_pos_c(const AVFrame * const frame, const unsigned int x, const unsigned int y)
{
    return frame->data[1] + av_rpi_sand_frame_off_c(frame, x, y);
}

#endif

