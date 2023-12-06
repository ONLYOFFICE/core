#ifndef PIC_H
#define PIC_H

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <cstddef>

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>

#include <sys/stat.h>
#include <stdarg.h>

typedef long long ssize_t ;

typedef enum
{
  UndefinedType,
  BilevelType,
  GrayscaleType,
  GrayscaleAlphaType,
  PaletteType,
  PaletteAlphaType,
  TrueColorType,
  TrueColorAlphaType,
  ColorSeparationType,
  ColorSeparationAlphaType,
  OptimizeType,
  PaletteBilevelAlphaType
} ImageType;

typedef struct
{
  char
    *path;

  unsigned char
    *datum;

  size_t
    length,
    signature;

  char
    *name;
} StringInfo;

typedef struct _NodeInfo
{
  void
    *key;

  void
    *value;

  struct _NodeInfo
    *left,
    *right;
} NodeInfo;

typedef struct
{
  NodeInfo
    *root;

  int
    (*compare)(const void *,const void *);

  int
    balance;

  void
    *key,
    *next;

  size_t
    nodes;

  size_t
    signature;
} SplayTreeInfo;

typedef struct _PrimaryInfo
{
  double
    x,
    y,
    z;
} PrimaryInfo;

typedef struct _ChromaticityInfo
{
  PrimaryInfo
    red_primary,
    green_primary,
    blue_primary,
    white_point;
} ChromaticityInfo;

typedef struct _PICTCode
{
  const char
    *name;

  ssize_t
    length;

  const char
    *description;
} PICTCode;



typedef struct
{
    short
      top,
      left,
      bottom,
      right;
} PICTrectangle;



typedef struct
{
  short
    version,
    pack_type;

  size_t
    pack_size,
    horizontal_resolution,
    vertical_resolution;

  short
    pixel_type,
    bits_per_pixel,
    component_count,
    component_size;

  size_t
    plane_bytes,
    table,
    reserved;
} PICTPixmap;

typedef struct
{
    size_t
      width,
      height;

    ssize_t
      x,
      y;
} RectangleInfo;

typedef struct _NexusInfo
{
    RectangleInfo
      region;

    size_t
      length;

    unsigned char
      *pixels;

    int
      authentic_pixel_cache;

    size_t
      signature;

    struct _NexusInfo
      *virtual_nexus;
} NexusInfo;



typedef struct _GeometryInfo
{
  double
    rho,
    sigma,
    xi,
    psi,
    chi;
} GeometryInfo;

typedef enum
{
  UndefinedChannel = 0x0000,
  RedChannel = 0x0001,
  GrayChannel = 0x0001,
  CyanChannel = 0x0001,
  LChannel = 0x0001,
  GreenChannel = 0x0002,
  MagentaChannel = 0x0002,
  aChannel = 0x0002,
  BlueChannel = 0x0004,
  bChannel = 0x0002,
  YellowChannel = 0x0004,
  BlackChannel = 0x0008,
  AlphaChannel = 0x0010,
  OpacityChannel = 0x0010,
  IndexChannel = 0x0020,             /* Color Index Table? */
  ReadMaskChannel = 0x0040,          /* Pixel is Not Readable? */
  WriteMaskChannel = 0x0080,         /* Pixel is Write Protected? */
  MetaChannel = 0x0100,              /* not used */
  CompositeMaskChannel = 0x0200,     /* SVG mask */
  CompositeChannels = 0x001F,
  AllChannels = 0X7FFFFFF,
  /*
    Special purpose channel types.
    FUTURE: are these needed any more - they are more like hacks
    SyncChannels for example is NOT a real channel but a 'flag'
    It really says -- "User has not defined channels"
    Though it does have extra meaning in the "-auto-level" operator
  */
  TrueAlphaChannel = 0x0100, /* extract actual alpha channel from opacity */
  RGBChannels = 0x0200,      /* set alpha from grayscale mask in RGB */
  GrayChannels = 0x0400,
  SyncChannels = 0x20000,    /* channels modified as a single unit */
  DefaultChannels = AllChannels
} ChannelType;

typedef enum
{
  ReadMode,
  WriteMode,
  IOMode,
  PersistMode
} MapMode;

typedef enum
{
  UndefinedClass,
  DirectClass,
  PseudoClass
} ClassType;

typedef enum
{
  UndefinedColorspace,
  CMYColorspace,           /* negated linear RGB colorspace */
  CMYKColorspace,          /* CMY with Black separation */
  GRAYColorspace,          /* Single Channel greyscale (non-linear) image */
  HCLColorspace,
  HCLpColorspace,
  HSBColorspace,
  HSIColorspace,
  HSLColorspace,
  HSVColorspace,           /* alias for HSB */
  HWBColorspace,
  LabColorspace,
  LCHColorspace,           /* alias for LCHuv */
  LCHabColorspace,         /* Cylindrical (Polar) Lab */
  LCHuvColorspace,         /* Cylindrical (Polar) Luv */
  LogColorspace,
  LMSColorspace,
  LuvColorspace,
  OHTAColorspace,
  Rec601YCbCrColorspace,
  Rec709YCbCrColorspace,
  RGBColorspace,           /* Linear RGB colorspace */
  scRGBColorspace,         /* ??? */
  sRGBColorspace,          /* Default: non-linear sRGB colorspace */
  TransparentColorspace,
  xyYColorspace,
  XYZColorspace,           /* IEEE Color Reference colorspace */
  YCbCrColorspace,
  YCCColorspace,
  YDbDrColorspace,
  YIQColorspace,
  YPbPrColorspace,
  YUVColorspace,
  LinearGRAYColorspace,     /* Single Channel greyscale (linear) image */
  JzazbzColorspace,
  DisplayP3Colorspace,
  Adobe98Colorspace,
  ProPhotoColorspace,
  OklabColorspace,
  OklchColorspace
} ColorspaceType;

typedef enum
{
  UndefinedPixelTrait = 0x000000,
  CopyPixelTrait = 0x000001,
  UpdatePixelTrait = 0x000002,
  BlendPixelTrait = 0x000004
} PixelTrait;

typedef enum
{
  UndefinedPixelChannel = 0,
  RedPixelChannel = 0,
  CyanPixelChannel = 0,
  GrayPixelChannel = 0,
  LPixelChannel = 0,
  LabelPixelChannel = 0,
  YPixelChannel = 0,
  aPixelChannel = 1,
  GreenPixelChannel = 1,
  MagentaPixelChannel = 1,
  CbPixelChannel = 1,
  bPixelChannel = 2,
  BluePixelChannel = 2,
  YellowPixelChannel = 2,
  CrPixelChannel = 2,
  BlackPixelChannel = 3,
  AlphaPixelChannel = 4,
  IndexPixelChannel = 5,
  ReadMaskPixelChannel = 6,
  WriteMaskPixelChannel = 7,
  MetaPixelChannel = 8, /* deprecated */
  CompositeMaskPixelChannel = 9,
  MetaPixelChannels = 10,
  IntensityPixelChannel = 64,  /* ???? */
  CompositePixelChannel = 64,  /* ???? */
  SyncPixelChannel = 65      /* not a real channel */
} PixelChannel;

typedef struct
{
  PixelChannel
    channel;

  PixelTrait
    traits;

  ssize_t
    offset;
} PixelChannelMap;

typedef struct
{
  ClassType
    storage_class;

  ColorspaceType
    colorspace;

  PixelTrait
    alpha_trait;

  double
    fuzz;

  size_t
    depth,
    count;

  double
    red,
    green,
    blue,
    black,
    alpha,
    index;
} PixelInfo;

typedef struct
{
    ClassType
      storage_class;

    ColorspaceType
      colorspace;

    ChromaticityInfo
      chromaticity;

    int
      m_pctVersion;

    size_t
      m_nHeight,
      m_nWidth,
      m_ndepth,
      m_nPixelsSize,
      colors;

    SplayTreeInfo
      *profiles,
      *artifacts;

    double
      fuzz;

    unsigned char
      *ppixels;

    double
      resolutionX,
      resolutionY,
      gamma;

    ImageType
      type;

    PixelInfo
      background_color,
      *colormap;

    PixelTrait
      alpha_trait;

    NexusInfo
      *nexus;

    PixelChannelMap
      *channel_map;

    PixelTrait
      mask_trait;

    int
      taint;

    void
      *cache;

    size_t
      number_channels;

    char
      error[256];
}ImagePICT;

int DecodePICT(FILE* hFile, ImagePICT* image);
void AquireImage(ImagePICT* image);
ImagePICT *DestroyImage(ImagePICT *image);

#endif // PIC_H
