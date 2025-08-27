/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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
    char* path;
    unsigned char* datum;

    size_t    length;
    size_t    signature;

    char* name;
} StringInfo;

typedef struct _NodeInfo
{
    void* key;
    void* value;

    struct _NodeInfo* left;
    struct _NodeInfo* right;
} NodeInfo;

typedef struct
{
    NodeInfo* root;

    int (*compare)(const void*, const void*);

    int   balance;

    void* key;
    void* next;

    size_t   nodes;

    size_t  signature;
} SplayTreeInfo;

typedef struct _PrimaryInfo
{
    double x;
    double y;
    double z;
} PrimaryInfo;

typedef struct _ChromaticityInfo
{
    PrimaryInfo red_primary;
    PrimaryInfo green_primary;
    PrimaryInfo blue_primary;
    PrimaryInfo white_point;
} ChromaticityInfo;

typedef struct _PICTCode
{
    const char* name;
    long long length;

    const char* description;
} PICTCode;

typedef struct _PICTrectangle
{
	short top{0};
	short left{0};
	short bottom{0};
	short right{0};
	_PICTrectangle() {}
	_PICTrectangle(short _top, short _left, short _bottom, short _right) :
		top(_top), left(_left), bottom(_bottom), right(_right) {}
} PICTrectangle;

typedef struct
{
    short  version;
    short  pack_type;

    size_t pack_size;
    size_t horizontal_resolution;
    size_t vertical_resolution;

    short    pixel_type;
    short bits_per_pixel;
    short  component_count;
    short  component_size;

    size_t plane_bytes;
    size_t table;
    size_t reserved;
} PICTPixmap;

typedef struct
{
    size_t  width;
    size_t  height;

    long long  x;
    long long  y;
} RectangleInfo;

typedef struct _NexusInfo
{
    RectangleInfo   region;
    size_t  length;
    unsigned char* pixels;
    int   authentic_pixel_cache;
    size_t   signature;
    struct _NexusInfo* virtual_nexus;
} NexusInfo;


typedef struct _GeometryInfo
{
    double   rho;
    double sigma;
    double xi;
    double psi;
    double chi;
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
    PixelChannel  channel;
    PixelTrait  traits;
    long long  offset;
} PixelChannelMap;

typedef struct
{
    ClassType storage_class;

    ColorspaceType  colorspace;

    PixelTrait alpha_trait;

    double fuzz;

    size_t  depth;
    size_t count;

    double red;
    double green;
    double blue;
    double black;
    double alpha;
    double index;
} PixelInfo;

template<typename T>
struct Point {
	T X{0};
	T Y{0};
	Point() {}
	Point(T x, T y) : X(x), Y(y) {}
};

template<typename T>
struct Polygon
{
	size_t Size;
	Point<T>* Points;
	Polygon() {}
	~Polygon()
	{
		if (Points)
			free(Points);
	}
};

typedef struct
{
    ClassType storage_class;
    ColorspaceType colorspace;
    ChromaticityInfo chromaticity;
    int m_pctVersion;

    size_t m_nHeight;
    size_t m_nWidth;
    size_t m_ndepth;
    size_t m_nPixelsSize;
    size_t colors;

    SplayTreeInfo* artifacts;

    double fuzz;

    unsigned char* ppixels;

    double resolutionX;
    double resolutionY;
    double gamma;

    ImageType type;

    PixelInfo  background_color;
    PixelInfo* colormap;

    PixelTrait  alpha_trait;

    PixelChannelMap* channel_map;

    PixelTrait  mask_trait;

    int  taint;

    void* cache;

    size_t number_channels;

    char error[256];
}ImagePICT;

bool DecodePICT(FILE* hFile, ImagePICT* image);
void AquireImage(ImagePICT* image);
void DestroyImage(ImagePICT* image);

#endif // PIC_H
