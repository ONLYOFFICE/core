// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// @gyp_namespace(ui_surface)
// Compiles into C++ as 'accelerated_surface_transformer_win_hlsl_compiled.h'

struct Vertex {
  float4 position : POSITION;
  float2 texCoord : TEXCOORD0;
};

texture t;
sampler s;

extern uniform float2 kRenderTargetSize : c0;
extern uniform float2 kTextureScale : c1;

// @gyp_compile(vs_2_0, vsOneTexture)
//
// Passes a position and texture coordinate to the pixel shader.
Vertex vsOneTexture(Vertex input) {
  // Texture scale is typically just 1 (to do nothing) or -1 (to flip).
  input.texCoord = ((2 * (input.texCoord - 0.5) * kTextureScale) + 1) / 2;
  input.position.x += -1 / kRenderTargetSize.x;
  input.position.y +=  1 / kRenderTargetSize.y;
  return input;
};

// @gyp_compile(ps_2_0, psOneTexture)
//
// Samples a texture at the given texture coordinate and returns the result.
float4 psOneTexture(float2 texCoord : TEXCOORD0) : COLOR0 {
  return tex2D(s, texCoord);
};

// Return |value| rounded up to the nearest multiple of |multiple|.
float alignTo(float value, float multiple) {
  // |multiple| is usually a compile-time constant; this check allows
  // the compiler to avoid the fmod when possible.
  if (multiple == 1)
    return value;

  // Biasing the value provides numeric stability. We expect |value| to
  // be an integer; this prevents 4.001 from being rounded up to 8.
  float biased_value = value - 0.5;
  return biased_value + multiple - fmod(biased_value, multiple);
}

float4 packForByteOrder(float4 value) {
  return value.bgra;
}

// Adjust the input vertex to address the correct range of texels. This depends
// on the value of the shader constant |kRenderTargetSize|, as well as an
// alignment factor |align| that effectively specifies the footprint of the
// texel samples done by this shader pass, and is used to correct when that
// footprint size doesn't align perfectly with the actual input size.
Vertex adjustForAlignmentAndPacking(Vertex vtx, float2 align) {
  float src_width = kRenderTargetSize.x;
  float src_height = kRenderTargetSize.y;

  // Because our caller expects to be sampling |align.x| many pixels from src at
  // a time, if src's width isn't evenly divisible by |align.x|, it is necessary
  // to pretend that the source is slightly bigger than it is.
  float bloated_src_width = alignTo(src_width, align.x);
  float bloated_src_height = alignTo(src_height, align.y);

  // When bloated_src_width != src_width, we'll adjust the texture coordinates
  // to sample past the edge of the vtx; clamping will produce extra copies of
  // the last row.
  float texture_x_scale = bloated_src_width / src_width;
  float texture_y_scale = bloated_src_height / src_height;

  // Adjust positions so that we're addressing full fragments in the output, per
  // the top-left filling convention. The shifts would be equivalent to
  // 1/dst_width and 1/dst_height, if we were to calculate those explicitly.
  vtx.position.x -= align.x / bloated_src_width;
  vtx.position.y += align.y / bloated_src_height;

  // Apply the texture scale
  vtx.texCoord.x *= texture_x_scale;
  vtx.texCoord.y *= texture_y_scale;

  return vtx;
}

///////////////////////////////////////////////////////////////////////
// RGB24 to YV12 in two passes; writing two 8888 targets each pass.
//
// YV12 is full-resolution luma and half-resolution blue/red chroma.
//
//                  (original)
//    XRGB XRGB XRGB XRGB XRGB XRGB XRGB XRGB
//    XRGB XRGB XRGB XRGB XRGB XRGB XRGB XRGB
//    XRGB XRGB XRGB XRGB XRGB XRGB XRGB XRGB
//    XRGB XRGB XRGB XRGB XRGB XRGB XRGB XRGB
//    XRGB XRGB XRGB XRGB XRGB XRGB XRGB XRGB
//    XRGB XRGB XRGB XRGB XRGB XRGB XRGB XRGB
//      |
//      |      (y plane)    (temporary)
//      |      YYYY YYYY     UVUV UVUV
//      +--> { YYYY YYYY  +  UVUV UVUV }
//             YYYY YYYY     UVUV UVUV
//   First     YYYY YYYY     UVUV UVUV
//    pass     YYYY YYYY     UVUV UVUV
//             YYYY YYYY     UVUV UVUV
//                              |
//                              |  (u plane) (v plane)
//   Second                     |      UUUU   VVVV
//     pass                     +--> { UUUU + VVVV }
//                                     UUUU   VVVV
//
///////////////////////////////////////////////////////////////////////

// Phase one of RGB24->YV12 conversion: vsFetch4Pixels/psConvertRGBtoY8UV44
//
// @gyp_compile(vs_2_0, vsFetch4Pixels)
// @gyp_compile(ps_2_0, psConvertRGBtoY8UV44)
//
// Writes four source pixels at a time to a full-size Y plane and a half-width
// interleaved UV plane. After execution, the Y plane is complete but the UV
// planes still need to be de-interleaved and vertically scaled.
//
void vsFetch4Pixels(in Vertex vertex,
                    out float4 position  : POSITION,
                    out float2 texCoord0 : TEXCOORD0,
                    out float2 texCoord1 : TEXCOORD1,
                    out float2 texCoord2 : TEXCOORD2,
                    out float2 texCoord3 : TEXCOORD3) {
  Vertex adjusted = adjustForAlignmentAndPacking(vertex, float2(4, 1));

  // Set up four taps, aligned to texel centers if the src's true size is
  // |kRenderTargetSize|, and doing bilinear interpolation otherwise.
  float2 one_texel_x = float2(1 / kRenderTargetSize.x, 0);
  position = adjusted.position;
  texCoord0 = adjusted.texCoord - 1.5f * one_texel_x;
  texCoord1 = adjusted.texCoord - 0.5f * one_texel_x;
  texCoord2 = adjusted.texCoord + 0.5f * one_texel_x;
  texCoord3 = adjusted.texCoord + 1.5f * one_texel_x;
};

struct YV16QuadPixel
{
  float4 YYYY : COLOR0;
  float4 UUVV : COLOR1;
};

// Color conversion constants.
static const float3x1 rgb_to_y = float3x1( +0.257f, +0.504f, +0.098f );
static const float3x1 rgb_to_u = float3x1( -0.148f, -0.291f, +0.439f );
static const float3x1 rgb_to_v = float3x1( +0.439f, -0.368f, -0.071f );
static const float y_bias      = 0.0625f;
static const float uv_bias     = 0.5f;

YV16QuadPixel psConvertRGBtoY8UV44(float2 texCoord0 : TEXCOORD0,
                                   float2 texCoord1 : TEXCOORD1,
                                   float2 texCoord2 : TEXCOORD2,
                                   float2 texCoord3 : TEXCOORD3) {
  // Load the four texture samples into a matrix.
  float4x3 rgb_quad_pixel = float4x3(tex2D(s, texCoord0).rgb,
                                     tex2D(s, texCoord1).rgb,
                                     tex2D(s, texCoord2).rgb,
                                     tex2D(s, texCoord3).rgb);

  // RGB -> Y conversion (x4).
  float4 yyyy = mul(rgb_quad_pixel, rgb_to_y) + y_bias;

  // Average adjacent texture samples while converting RGB->UV. This is the same
  // as color converting then averaging, but slightly less math. These values
  // will be in the range [-0.439f, +0.439f] and still need to have the bias
  // term applied.
  float2x3 rgb_double_pixel = float2x3(rgb_quad_pixel[0] + rgb_quad_pixel[1],
                                       rgb_quad_pixel[2] + rgb_quad_pixel[3]);
  float2 uu = mul(rgb_double_pixel, rgb_to_u / 2);
  float2 vv = mul(rgb_double_pixel, rgb_to_v / 2);

  // Package the result to account for BGRA byte ordering.
  YV16QuadPixel result;
  result.YYYY = packForByteOrder(yyyy);
  result.UUVV.xyzw = float4(uu, vv) + uv_bias;  // Apply uv bias.
  return result;
};

// Phase two of RGB24->YV12 conversion: vsFetch2Pixels/psConvertUV44toU2V2
//
// @gyp_compile(vs_2_0, vsFetch2Pixels)
// @gyp_compile(ps_2_0, psConvertUV44toU2V2)
//
// Deals with UV only. Input is interleaved UV pixels, already scaled
// horizontally, packed two per RGBA texel. Output is two color planes U and V,
// packed four to a RGBA pixel.
//
// Vertical scaling happens via a half-texel offset and bilinear interpolation
// during texture sampling.
void vsFetch2Pixels(in Vertex vertex,
                    out float4 position : POSITION,
                    out float2 texCoord0 : TEXCOORD0,
                    out float2 texCoord1 : TEXCOORD1) {
  // We fetch two texels in the horizontal direction, and scale by 2 in the
  // vertical direction.
  Vertex adjusted = adjustForAlignmentAndPacking(vertex, float2(2, 2));

  // Setup the two texture coordinates. No need to adjust texCoord.y; it's
  // already at the mid-way point between the two rows. Horizontally, we'll
  // fetch two texels so that we have enough data to fill our output.
  float2 one_texel_x = float2(1 / kRenderTargetSize.x, 0);
  position = adjusted.position;
  texCoord0 = adjusted.texCoord - 0.5f * one_texel_x;
  texCoord1 = adjusted.texCoord + 0.5f * one_texel_x;
};

struct UV8QuadPixel {
  float4 UUUU : COLOR0;
  float4 VVVV : COLOR1;
};

UV8QuadPixel psConvertUV44toU2V2(float2 texCoord0 : TEXCOORD0,
                                 float2 texCoord1 : TEXCOORD1) {
  // We're just sampling two pixels and unswizzling them. There's no need to do
  // vertical scaling with math, since bilinear interpolation in the sampler
  // takes care of that.
  float4 lo_uuvv = tex2D(s, texCoord0);
  float4 hi_uuvv = tex2D(s, texCoord1);
  UV8QuadPixel result;
  result.UUUU = packForByteOrder(float4(lo_uuvv.xy, hi_uuvv.xy));
  result.VVVV = packForByteOrder(float4(lo_uuvv.zw, hi_uuvv.zw));
  return result;
};


///////////////////////////////////////////////////////////////////////
// RGB24 to YV12 in three passes, without MRT: one pass per output color plane.
// vsFetch4Pixels is the common vertex shader for all three passes.
//
// Note that this technique will not do full bilinear filtering on its RGB
// input (you'd get correctly filtered Y, but aliasing in U and V).
//
// Pass 1: vsFetch4Pixels + psConvertRGBToY
// Pass 2: vsFetch4Pixels_Scale2 + psConvertRGBToU
// Pass 3: vsFetch4Pixels_Scale2 + psConvertRGBToV
//
// @gyp_compile(vs_2_0, vsFetch4Pixels_Scale2)
// @gyp_compile(ps_2_0, psConvertRGBtoY)
// @gyp_compile(ps_2_0, psConvertRGBtoU)
// @gyp_compile(ps_2_0, psConvertRGBtoV)
//
///////////////////////////////////////////////////////////////////////
void vsFetch4Pixels_Scale2(in Vertex vertex,
                           out float4 position  : POSITION,
                           out float2 texCoord0 : TEXCOORD0,
                           out float2 texCoord1 : TEXCOORD1,
                           out float2 texCoord2 : TEXCOORD2,
                           out float2 texCoord3 : TEXCOORD3) {
  Vertex adjusted = adjustForAlignmentAndPacking(vertex, float2(8, 2));

  // Set up four taps, each of which samples a 2x2 texel quad at the midpoint.
  float2 one_texel_x = float2(1 / kRenderTargetSize.x, 0);
  position = adjusted.position;
  texCoord0 = adjusted.texCoord - 3 * one_texel_x;
  texCoord1 = adjusted.texCoord - 1 * one_texel_x;
  texCoord2 = adjusted.texCoord + 1 * one_texel_x;
  texCoord3 = adjusted.texCoord + 3 * one_texel_x;
};

// RGB -> Y, four samples at a time.
float4 psConvertRGBtoY(float2 texCoord0 : TEXCOORD0,
                       float2 texCoord1 : TEXCOORD1,
                       float2 texCoord2 : TEXCOORD2,
                       float2 texCoord3 : TEXCOORD3) : COLOR0 {
  float4x3 rgb_quad_pixel = float4x3(tex2D(s, texCoord0).rgb,
                                     tex2D(s, texCoord1).rgb,
                                     tex2D(s, texCoord2).rgb,
                                     tex2D(s, texCoord3).rgb);
  return packForByteOrder(mul(rgb_quad_pixel, rgb_to_y) + y_bias);
}

// RGB -> U, four samples at a time.
float4 psConvertRGBtoU(float2 texCoord0 : TEXCOORD0,
                       float2 texCoord1 : TEXCOORD1,
                       float2 texCoord2 : TEXCOORD2,
                       float2 texCoord3 : TEXCOORD3) : COLOR0 {
  float4x3 rgb_quad_pixel = float4x3(tex2D(s, texCoord0).rgb,
                                     tex2D(s, texCoord1).rgb,
                                     tex2D(s, texCoord2).rgb,
                                     tex2D(s, texCoord3).rgb);
  return packForByteOrder(mul(rgb_quad_pixel, rgb_to_u) + uv_bias);
}

// RGB -> V, four samples at a time.
float4 psConvertRGBtoV(float2 texCoord0 : TEXCOORD0,
                       float2 texCoord1 : TEXCOORD1,
                       float2 texCoord2 : TEXCOORD2,
                       float2 texCoord3 : TEXCOORD3) : COLOR0 {
  float4x3 rgb_quad_pixel = float4x3(tex2D(s, texCoord0).rgb,
                                     tex2D(s, texCoord1).rgb,
                                     tex2D(s, texCoord2).rgb,
                                     tex2D(s, texCoord3).rgb);
  return packForByteOrder(mul(rgb_quad_pixel, rgb_to_v) + uv_bias);
}
