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
#include "pic.h"

static const PICTCode
  codes[] =
  {
    /* 0x00 */ { "NOP", 0, "nop" },
    /* 0x01 */ { "ClipRgn", 0, "clip" },
    /* 0x02 */ { "BkPat", 8, "background pattern" },
    /* 0x03 */ { "TxFont", 2, "text font (word)" },
    /* 0x04 */ { "TxFace", 1, "text face (byte)" },
    /* 0x05 */ { "TxMode", 2, "text mode (word)" },
    /* 0x06 */ { "SpExtra", 4, "space extra (fixed point)" },
    /* 0x07 */ { "PnSize", 4, "pen size (point)" },
    /* 0x08 */ { "PnMode", 2, "pen mode (word)" },
    /* 0x09 */ { "PnPat", 8, "pen pattern" },
    /* 0x0a */ { "FillPat", 8, "fill pattern" },
    /* 0x0b */ { "OvSize", 4, "oval size (point)" },
    /* 0x0c */ { "Origin", 4, "dh, dv (word)" },
    /* 0x0d */ { "TxSize", 2, "text size (word)" },
    /* 0x0e */ { "FgColor", 4, "foreground color (long longword)" },
    /* 0x0f */ { "BkColor", 4, "background color (long longword)" },
    /* 0x10 */ { "TxRatio", 8, "numerator (point), denominator (point)" },
    /* 0x11 */ { "Version", 1, "version (byte)" },
    /* 0x12 */ { "BkPixPat", 0, "color background pattern" },
    /* 0x13 */ { "PnPixPat", 0, "color pen pattern" },
    /* 0x14 */ { "FillPixPat", 0, "color fill pattern" },
    /* 0x15 */ { "PnLocHFrac", 2, "fractional pen position" },
    /* 0x16 */ { "ChExtra", 2, "extra for each character" },
    /* 0x17 */ { "reserved", 0, "reserved for Apple use" },
    /* 0x18 */ { "reserved", 0, "reserved for Apple use" },
    /* 0x19 */ { "reserved", 0, "reserved for Apple use" },
    /* 0x1a */ { "RGBFgCol", 6, "RGB foreColor" },
    /* 0x1b */ { "RGBBkCol", 6, "RGB backColor" },
    /* 0x1c */ { "HiliteMode", 0, "hilite mode flag" },
    /* 0x1d */ { "HiliteColor", 6, "RGB hilite color" },
    /* 0x1e */ { "DefHilite", 0, "Use default hilite color" },
    /* 0x1f */ { "OpColor", 6, "RGB OpColor for arithmetic modes" },
    /* 0x20 */ { "Line", 8, "pnLoc (point), newPt (point)" },
    /* 0x21 */ { "LineFrom", 4, "newPt (point)" },
    /* 0x22 */ { "ShortLine", 6, "pnLoc (point, dh, dv (-128 .. 127))" },
    /* 0x23 */ { "ShortLineFrom", 2, "dh, dv (-128 .. 127)" },
    /* 0x24 */ { "reserved", -1, "reserved for Apple use" },
    /* 0x25 */ { "reserved", -1, "reserved for Apple use" },
    /* 0x26 */ { "reserved", -1, "reserved for Apple use" },
    /* 0x27 */ { "reserved", -1, "reserved for Apple use" },
    /* 0x28 */ { "LongText", 0, "txLoc (point), count (0..255), text" },
    /* 0x29 */ { "DHText", 0, "dh (0..255), count (0..255), text" },
    /* 0x2a */ { "DVText", 0, "dv (0..255), count (0..255), text" },
    /* 0x2b */ { "DHDVText", 0, "dh, dv (0..255), count (0..255), text" },
    /* 0x2c */ { "reserved", -1, "reserved for Apple use" },
    /* 0x2d */ { "reserved", -1, "reserved for Apple use" },
    /* 0x2e */ { "reserved", -1, "reserved for Apple use" },
    /* 0x2f */ { "reserved", -1, "reserved for Apple use" },
    /* 0x30 */ { "frameRect", 8, "rect" },
    /* 0x31 */ { "paintRect", 8, "rect" },
    /* 0x32 */ { "eraseRect", 8, "rect" },
    /* 0x33 */ { "invertRect", 8, "rect" },
    /* 0x34 */ { "fillRect", 8, "rect" },
    /* 0x35 */ { "reserved", 8, "reserved for Apple use" },
    /* 0x36 */ { "reserved", 8, "reserved for Apple use" },
    /* 0x37 */ { "reserved", 8, "reserved for Apple use" },
    /* 0x38 */ { "frameSameRect", 0, "rect" },
    /* 0x39 */ { "paintSameRect", 0, "rect" },
    /* 0x3a */ { "eraseSameRect", 0, "rect" },
    /* 0x3b */ { "invertSameRect", 0, "rect" },
    /* 0x3c */ { "fillSameRect", 0, "rect" },
    /* 0x3d */ { "reserved", 0, "reserved for Apple use" },
    /* 0x3e */ { "reserved", 0, "reserved for Apple use" },
    /* 0x3f */ { "reserved", 0, "reserved for Apple use" },
    /* 0x40 */ { "frameRRect", 8, "rect" },
    /* 0x41 */ { "paintRRect", 8, "rect" },
    /* 0x42 */ { "eraseRRect", 8, "rect" },
    /* 0x43 */ { "invertRRect", 8, "rect" },
    /* 0x44 */ { "fillRRrect", 8, "rect" },
    /* 0x45 */ { "reserved", 8, "reserved for Apple use" },
    /* 0x46 */ { "reserved", 8, "reserved for Apple use" },
    /* 0x47 */ { "reserved", 8, "reserved for Apple use" },
    /* 0x48 */ { "frameSameRRect", 0, "rect" },
    /* 0x49 */ { "paintSameRRect", 0, "rect" },
    /* 0x4a */ { "eraseSameRRect", 0, "rect" },
    /* 0x4b */ { "invertSameRRect", 0, "rect" },
    /* 0x4c */ { "fillSameRRect", 0, "rect" },
    /* 0x4d */ { "reserved", 0, "reserved for Apple use" },
    /* 0x4e */ { "reserved", 0, "reserved for Apple use" },
    /* 0x4f */ { "reserved", 0, "reserved for Apple use" },
    /* 0x50 */ { "frameOval", 8, "rect" },
    /* 0x51 */ { "paintOval", 8, "rect" },
    /* 0x52 */ { "eraseOval", 8, "rect" },
    /* 0x53 */ { "invertOval", 8, "rect" },
    /* 0x54 */ { "fillOval", 8, "rect" },
    /* 0x55 */ { "reserved", 8, "reserved for Apple use" },
    /* 0x56 */ { "reserved", 8, "reserved for Apple use" },
    /* 0x57 */ { "reserved", 8, "reserved for Apple use" },
    /* 0x58 */ { "frameSameOval", 0, "rect" },
    /* 0x59 */ { "paintSameOval", 0, "rect" },
    /* 0x5a */ { "eraseSameOval", 0, "rect" },
    /* 0x5b */ { "invertSameOval", 0, "rect" },
    /* 0x5c */ { "fillSameOval", 0, "rect" },
    /* 0x5d */ { "reserved", 0, "reserved for Apple use" },
    /* 0x5e */ { "reserved", 0, "reserved for Apple use" },
    /* 0x5f */ { "reserved", 0, "reserved for Apple use" },
    /* 0x60 */ { "frameArc", 12, "rect, startAngle, arcAngle" },
    /* 0x61 */ { "paintArc", 12, "rect, startAngle, arcAngle" },
    /* 0x62 */ { "eraseArc", 12, "rect, startAngle, arcAngle" },
    /* 0x63 */ { "invertArc", 12, "rect, startAngle, arcAngle" },
    /* 0x64 */ { "fillArc", 12, "rect, startAngle, arcAngle" },
    /* 0x65 */ { "reserved", 12, "reserved for Apple use" },
    /* 0x66 */ { "reserved", 12, "reserved for Apple use" },
    /* 0x67 */ { "reserved", 12, "reserved for Apple use" },
    /* 0x68 */ { "frameSameArc", 4, "rect, startAngle, arcAngle" },
    /* 0x69 */ { "paintSameArc", 4, "rect, startAngle, arcAngle" },
    /* 0x6a */ { "eraseSameArc", 4, "rect, startAngle, arcAngle" },
    /* 0x6b */ { "invertSameArc", 4, "rect, startAngle, arcAngle" },
    /* 0x6c */ { "fillSameArc", 4, "rect, startAngle, arcAngle" },
    /* 0x6d */ { "reserved", 4, "reserved for Apple use" },
    /* 0x6e */ { "reserved", 4, "reserved for Apple use" },
    /* 0x6f */ { "reserved", 4, "reserved for Apple use" },
    /* 0x70 */ { "framePoly", 0, "poly" },
    /* 0x71 */ { "paintPoly", 0, "poly" },
    /* 0x72 */ { "erasePoly", 0, "poly" },
    /* 0x73 */ { "invertPoly", 0, "poly" },
    /* 0x74 */ { "fillPoly", 0, "poly" },
    /* 0x75 */ { "reserved", 0, "reserved for Apple use" },
    /* 0x76 */ { "reserved", 0, "reserved for Apple use" },
    /* 0x77 */ { "reserved", 0, "reserved for Apple use" },
    /* 0x78 */ { "frameSamePoly", 0, "poly (NYI)" },
    /* 0x79 */ { "paintSamePoly", 0, "poly (NYI)" },
    /* 0x7a */ { "eraseSamePoly", 0, "poly (NYI)" },
    /* 0x7b */ { "invertSamePoly", 0, "poly (NYI)" },
    /* 0x7c */ { "fillSamePoly", 0, "poly (NYI)" },
    /* 0x7d */ { "reserved", 0, "reserved for Apple use" },
    /* 0x7e */ { "reserved", 0, "reserved for Apple use" },
    /* 0x7f */ { "reserved", 0, "reserved for Apple use" },
    /* 0x80 */ { "frameRgn", 0, "region" },
    /* 0x81 */ { "paintRgn", 0, "region" },
    /* 0x82 */ { "eraseRgn", 0, "region" },
    /* 0x83 */ { "invertRgn", 0, "region" },
    /* 0x84 */ { "fillRgn", 0, "region" },
    /* 0x85 */ { "reserved", 0, "reserved for Apple use" },
    /* 0x86 */ { "reserved", 0, "reserved for Apple use" },
    /* 0x87 */ { "reserved", 0, "reserved for Apple use" },
    /* 0x88 */ { "frameSameRgn", 0, "region (NYI)" },
    /* 0x89 */ { "paintSameRgn", 0, "region (NYI)" },
    /* 0x8a */ { "eraseSameRgn", 0, "region (NYI)" },
    /* 0x8b */ { "invertSameRgn", 0, "region (NYI)" },
    /* 0x8c */ { "fillSameRgn", 0, "region (NYI)" },
    /* 0x8d */ { "reserved", 0, "reserved for Apple use" },
    /* 0x8e */ { "reserved", 0, "reserved for Apple use" },
    /* 0x8f */ { "reserved", 0, "reserved for Apple use" },
    /* 0x90 */ { "BitsRect", 0, "copybits, rect clipped" },
    /* 0x91 */ { "BitsRgn", 0, "copybits, rgn clipped" },
    /* 0x92 */ { "reserved", -1, "reserved for Apple use" },
    /* 0x93 */ { "reserved", -1, "reserved for Apple use" },
    /* 0x94 */ { "reserved", -1, "reserved for Apple use" },
    /* 0x95 */ { "reserved", -1, "reserved for Apple use" },
    /* 0x96 */ { "reserved", -1, "reserved for Apple use" },
    /* 0x97 */ { "reserved", -1, "reserved for Apple use" },
    /* 0x98 */ { "PackBitsRect", 0, "packed copybits, rect clipped" },
    /* 0x99 */ { "PackBitsRgn", 0, "packed copybits, rgn clipped" },
    /* 0x9a */ { "DirectBitsRect", 0, "PixMap, srcRect, dstRect, mode, PixData" },
    /* 0x9b */ { "DirectBitsRgn", 0, "PixMap, srcRect, dstRect, mode, maskRgn, PixData" },
    /* 0x9c */ { "reserved", -1, "reserved for Apple use" },
    /* 0x9d */ { "reserved", -1, "reserved for Apple use" },
    /* 0x9e */ { "reserved", -1, "reserved for Apple use" },
    /* 0x9f */ { "reserved", -1, "reserved for Apple use" },
    /* 0xa0 */ { "ShortComment", 2, "kind (word)" },
    /* 0xa1 */ { "LongComment", 0, "kind (word), size (word), data" }
  };

#define BackgroundColorRGBA  255,255,255,255
#define TransparentAlpha ((unsigned char) 0)

int LocaleToLowercase(const int c)
{
  if ((c == EOF) || (c != (unsigned char) c))
    return(c);
  return(tolower((int) ((unsigned char) c)));
}

int LocaleCompare(const char *p,const char *q)
{
  if (p == (char *) NULL)
    {
      if (q == (char *) NULL)
        return(0);
      return(-1);
    }
  if (q == (char *) NULL)
    return(1);
  {
    const unsigned char
      *r = (const unsigned char *) p,
      *s = (const unsigned char *) q;

    for ( ; (*r != '\0') && (*s != '\0') && ((*r == *s) ||
      (LocaleToLowercase((int) *r) == LocaleToLowercase((int) *s))); r++, s++);
    return(LocaleToLowercase((int) *r)-LocaleToLowercase((int) *s));
  }
}

void LocaleLower(char *string)
{
  char
    *q;

  for (q=string; *q != '\0'; q++)
    *q=(char) LocaleToLowercase((int) *q);
}

int LocaleNCompare(const char *p,const char *q,const size_t length)
{
  if (p == (char *) NULL)
    {
      if (q == (char *) NULL)
        return(0);
      return(-1);
    }
  if (q == (char *) NULL)
    return(1);
  if (length == 0)
    return(0);
  {
    const unsigned char
      *s = (const unsigned char *) p,
      *t = (const unsigned char *) q;

    size_t
      n = length;

    for (n--; (*s != '\0') && (*t != '\0') && (n != 0) && ((*s == *t) ||
      (LocaleToLowercase((int) *s) == LocaleToLowercase((int) *t))); s++, t++, n--);
    return(LocaleToLowercase((int) *s)-LocaleToLowercase((int) *t));
  }
}

char *ConstantString(const char *source)
{
  char
    *destination;

  size_t
    length;

  length=0;
  if (source != (char *) NULL)
    length+=strlen(source);
  destination=(char *) NULL;
  if (~length >= 1UL)
    destination=(char *) malloc(length+1UL*sizeof(*destination));
  if (destination == (char *) NULL)
    return NULL;
  if (source != (char *) NULL)
    (void) memcpy(destination,source,length*sizeof(*destination));
  destination[length]='\0';
  return(destination);
}

int IsStringTrue(const char *value)
{
  if (value == (const char *) NULL)
    return 0;
  if (LocaleCompare(value,"true") == 0)
    return 1;
  if (LocaleCompare(value,"on") == 0)
    return 1;
  if (LocaleCompare(value,"yes") == 0)
    return 1;
  if (LocaleCompare(value,"1") == 0)
    return 1;
  return 0;
}

size_t CopyMagickString(char *destination,const char *source,const size_t length)
{
  char
    *q;

  const char
    *p;

  size_t
    n;

  p=source;
  q=destination;
  for (n=length; n > 4; n-=4)
  {
    if (((*q++)=(*p++)) == '\0')
      return((size_t) (p-source-1));
    if (((*q++)=(*p++)) == '\0')
      return((size_t) (p-source-1));
    if (((*q++)=(*p++)) == '\0')
      return((size_t) (p-source-1));
    if (((*q++)=(*p++)) == '\0')
      return((size_t) (p-source-1));
  }
  if (length != 0)
    {
      while (--n != 0)
        if (((*q++)=(*p++)) == '\0')
          return((size_t) (p-source-1));
      *q='\0';
    }
  return((size_t) (p-source));
}

StringInfo *AcquireStringInfo(const size_t length)
{
    StringInfo
        *string_info;

      string_info=(StringInfo *) malloc(sizeof(*string_info));
      (void) memset(string_info,0,sizeof(*string_info));
      string_info->signature=0xabacadabUL;
      string_info->length=length;
      if (~string_info->length >= (4096-1))
        string_info->datum=(unsigned char *) malloc((string_info->length+4096)*sizeof(*string_info->datum));
      if (string_info->datum == (unsigned char *) NULL)
        return (StringInfo *) NULL;
      (void) memset(string_info->datum,0,(length+4096)*
        sizeof(*string_info->datum));
      return(string_info);
}

char *CloneString(char **destination,const char *source)
{
  size_t length;

  if (source == (const char *) NULL)
    {
      if (*destination != (char *) NULL)
        free(*destination);
      return(NULL);
    }
  if (*destination == (char *) NULL)
    {
      *destination=(char*) malloc((strlen(source) + 4096)/* * sizeof (*destination)*/);

      memcpy(*destination,source,strlen(source)/**sizeof(**destination)*/);
      (*destination)[strlen(source)]='\0';
      return(*destination);
    }
  length=strlen(source);
  if (~length < 4096)
    return NULL;
  free(*destination);
  *destination = (char*) malloc((length+4096)/**sizeof (**destination)*/);
  if (*destination == (char *) NULL)
    return NULL;
  if (length != 0)
    memcpy(*destination,source,length/**sizeof(**destination)*/);
  (*destination)[length]='\0';
  return(*destination);
}

StringInfo *CloneStringInfo(const StringInfo *string_info)
{
  StringInfo    *clone_info;

  clone_info=AcquireStringInfo(string_info->length);
  CloneString(&clone_info->path,string_info->path);
  CloneString(&clone_info->name,string_info->name);
  if (string_info->length != 0)
    memcpy(clone_info->datum,string_info->datum,string_info->length+1);
  return(clone_info);
}

StringInfo *DestroyStringInfo(StringInfo *string_info)
{
    if (string_info->datum != (unsigned char *) NULL)
          free(string_info->datum);
    if (string_info->name != (char *) NULL)
      free(string_info->name);
    if (string_info->path != (char *) NULL)
      free(string_info->path);
    string_info->signature=(~0xabacadabUL);
    free(string_info);
  return(string_info);
}

StringInfo *BlobToStringInfo(const void *blob,const size_t length)
{
  StringInfo
    *string_info;

  if (~length < 4096)
      return (StringInfo*) NULL;

  string_info=(StringInfo *) malloc(sizeof(*string_info));
  (void) memset(string_info,0,sizeof(*string_info));
  string_info->signature=0xabacadabUL;
  if (string_info == (StringInfo*) NULL)
    return (StringInfo*) NULL;
  string_info->length=length;
  string_info->datum=(unsigned char *) malloc(length+4096*sizeof(*string_info->datum));
  if (string_info->datum == (unsigned char *) NULL)
    {
      string_info = DestroyStringInfo(string_info);
      return((StringInfo *) NULL);
    }
  if (blob != (const void *) NULL)
    (void) memcpy(string_info->datum,blob,length);
  else
    (void) memset(string_info->datum,0,length*sizeof(*string_info->datum));
  (void) memset(string_info->datum+length,0,4096*sizeof(*string_info->datum));
  return(string_info);
}

void DeletePixelsMemory(ImagePICT* image)
{
    if (image->ppixels != NULL)
    {
        free(image->ppixels);

        image->ppixels = NULL;
        image->m_nPixelsSize = 0;
    }
}

PixelChannelMap *AcquirePixelChannelMap()
{
  PixelChannelMap
    *channel_map;

  long long
    i;

  channel_map=(PixelChannelMap *) malloc(65*sizeof(*channel_map));
  if (channel_map == (PixelChannelMap *) NULL)
    return (PixelChannelMap *) NULL;
  (void) memset(channel_map,0,65*sizeof(*channel_map));
  for (i=0; i <= 64; i++)
    channel_map[i].channel=(PixelChannel) i;
  channel_map[RedPixelChannel].offset = 0;
  channel_map[GreenPixelChannel].offset = 1;
  channel_map[BluePixelChannel].offset = 2;
  channel_map[AlphaPixelChannel].offset = 3;
  channel_map[IndexPixelChannel].offset = 5;

  return(channel_map);
}

void GetPixelInfo(ImagePICT *image,PixelInfo *pixel)
{
  (void) memset(pixel,0,sizeof(*pixel));
  pixel->storage_class=DirectClass;
  pixel->colorspace=sRGBColorspace;
  pixel->depth=8;
  pixel->alpha_trait=UndefinedPixelTrait;
  pixel->alpha=255.0;
  if (image == (const ImagePICT *) NULL)
    return;
  pixel->storage_class=image->storage_class;
  pixel->colorspace=image->colorspace;
  pixel->alpha_trait=image->alpha_trait;
  pixel->depth=image->m_ndepth;
  pixel->fuzz=image->fuzz;
}

void GetPixelInfoRGBA(const unsigned char red,const unsigned char green, const unsigned char blue,const unsigned char alpha,PixelInfo *pixel)
{
  GetPixelInfo((ImagePICT *) NULL,pixel);
  pixel->red=red;
  pixel->green=green;
  pixel->blue=blue;
  pixel->alpha=alpha;
}

unsigned char GetPixelWriteMask(const ImagePICT *image,const unsigned char *pixel)
{
  if (image->channel_map[WriteMaskPixelChannel].traits == UndefinedPixelTrait)
    return((unsigned char) 255);
  return(pixel[image->channel_map[WriteMaskPixelChannel].offset]);
}

int IsValidOffset(const long long x, const size_t a)
{
    if (a == 0)
        return 0;

    if ((x >= (LLONG_MAX / 64 / (long long) a)) ||
            (x <= ((-LLONG_MAX - 1) / 64 / (long long) a)))
        return 0;
    return 1;
}

int ReadPixels(ImagePICT* image, const long long x, const long long y, const size_t width, const size_t height, unsigned char* pixels)
{
    long long
            offset,
            i;

    size_t
            extent,
            length,
            number_channels,
            rows;

    unsigned char
            *q,
            *p;

    if (IsValidOffset(y, image->m_nWidth) == 0)
        return 0;
    offset = y * (long long) image->m_nWidth;

    if ((offset/ (long long) image->m_nWidth) != y)
    {
        strcpy(image->error, "UncorrectOffset");

        return 0;
    }

    offset += x;
    number_channels = image->number_channels;
    length = (size_t) number_channels * width * sizeof(unsigned char);

    if ((length / number_channels / sizeof(unsigned char)) != width)
    {
        strcpy(image->error, "UncorrectLength");

        return 0;
    }

    rows = height;
    extent = length * rows;

    if ((extent == 0) || ((extent/length) != rows))
    {
        strcpy(image->error, "UncorrectExtent");

        return 0;
    }

    i = 0;
    q = pixels;

    /*
      Read pixels from memory.
    */
    if ((image->m_nWidth == width) &&
        (extent == (size_t) extent))
      {
        length=extent;
        rows=1UL;
      }
    p = image->ppixels + image->number_channels * offset;
    for (i=0; i < (long long) rows; i++)
    {
      (void) memcpy(q,p,(size_t) length);
      p += image->number_channels * image->m_nWidth;
      q += image->number_channels * width;
    }

    if (i < rows)
    {
        strcpy(image->error, "Overflow");

        return 0;
    }

    return 1;
}

unsigned char* GetPixels(ImagePICT* image, const long long x, const long long y, const size_t width, const size_t height)
{
    unsigned char
            *pixels;

    pixels = image->ppixels + image->number_channels * (y * image->m_nWidth + x);

    if (pixels == (unsigned char *) NULL)
      return((unsigned char *) NULL);

    if (ReadPixels(image, x, y, width, height, pixels) == 0)
    {
        return ((unsigned char *) NULL);
    }

    return pixels;
}

static inline float PerceptibleReciprocal(const float x)
{
  float sign = x < (float) 0.0 ? (float) -1.0 : (float) 1.0;
  return((sign*x) >= 1.0e-12 ? (float) 1.0/x : sign*((float) 1.0/1.0e-12));
}


unsigned char GetPixelChannel(const ImagePICT *image,const PixelChannel channel,const unsigned char *pixel)
{
  if ((size_t) channel >= 64)
    return((unsigned char) 0);
  if (image->channel_map[channel].traits == UndefinedPixelTrait)
    return((unsigned char) 0);
  return(pixel[image->channel_map[channel].offset]);
}

unsigned char GetPixelAlpha(const ImagePICT *image,const unsigned char *pixel)
{
  if (image->channel_map[AlphaPixelChannel].traits == UndefinedPixelTrait)
    return (unsigned char) 0;
  return(pixel[image->channel_map[AlphaPixelChannel].offset]);
}

void SetPixelChannel(const ImagePICT *image,const PixelChannel channel,const unsigned char Quantum,unsigned char *pixel)
{
  if ((size_t) channel >= 64)
    return;
  if (image->channel_map[channel].traits != UndefinedPixelTrait)
    pixel[image->channel_map[channel].offset]=Quantum;
}

PixelChannel GetPixelChannelChannel(const ImagePICT *image,const long long offset)
{
  if ((offset < 0) || (offset >= 64))
    return(UndefinedPixelChannel);
  return(image->channel_map[offset].channel);
}

PixelTrait GetPixelChannelTraits(const ImagePICT *image,const PixelChannel channel)
{
  if ((size_t) channel >= 64)
    return(UndefinedPixelTrait);
  return(image->channel_map[channel].traits);
}

static inline void SetPixelIndex(const ImagePICT *image,
  const unsigned char index,unsigned char *pixel)
{
  if (image->channel_map[IndexPixelChannel].traits != UndefinedPixelTrait)
    pixel[image->channel_map[IndexPixelChannel].offset]=index;
}

static inline void SetPixelAlpha(const ImagePICT *image,
  const unsigned char alpha,unsigned char *pixel)
{
  if (image->channel_map[AlphaPixelChannel].traits != UndefinedPixelTrait)
    pixel[image->channel_map[AlphaPixelChannel].offset]=alpha;
}

static inline void SetPixelRed(const ImagePICT *image,
  const unsigned char red,unsigned char *pixel)
{
  pixel[image->channel_map[RedPixelChannel].offset]=red;
}

static inline void SetPixelGreen(const ImagePICT *image,
  const unsigned char green,unsigned char *pixel)
{
  pixel[image->channel_map[GreenPixelChannel].offset]=green;
}

static inline void SetPixelBlue(const ImagePICT *image,
  const unsigned char blue,unsigned char *pixel)
{
  pixel[image->channel_map[BluePixelChannel].offset]=blue;
}

int AquirePixelsMemory(ImagePICT* image)
{
    if ((image->m_nHeight == 0) || (image->m_nWidth == 0))
    {
        strcpy(image->error, "PixelSizeIsNull");

        return 0;
    }

    int nPixelsSize_new = image->m_nHeight * image->m_nWidth * image->number_channels;

    if (image->ppixels == NULL)
    {
        image->ppixels = (unsigned char*)malloc(nPixelsSize_new);
        memset(image->ppixels, 0xff, nPixelsSize_new);
    }
    else if (nPixelsSize_new != image->m_nPixelsSize)
    {
        image->ppixels = (unsigned char*)realloc(image->ppixels, nPixelsSize_new);
    }
    image->m_nPixelsSize = nPixelsSize_new;

    return 1;
}

int SetImageAlpha(ImagePICT* image, const unsigned char Alpha)
{
    int
            status,
            y;

    if (AquirePixelsMemory(image) == 0)
        return 0;

    image->alpha_trait = BlendPixelTrait;
    image->channel_map[AlphaPixelChannel].traits = UpdatePixelTrait;
    status = 1;
    for (y = 0; y < image->m_nHeight; y++)
    {
        unsigned char
                *q;

        int
                x;

        if (status == 0)
            continue;
        q = image->ppixels + image->number_channels * (y * image->m_nWidth);
        if (q == (unsigned char*) NULL)
        {
            status = 0;
            continue;
        }
        for (x = 0; x < image->m_nWidth; x++)
        {
            if (GetPixelWriteMask(image, q) > (255 / 2))
                SetPixelAlpha(image, Alpha, q);
            q += image->number_channels;
        }
    }

    return status;
}

int AquireImageColormap(ImagePICT* image, const size_t colors)
{
    long long
            i;

    if (image == (ImagePICT*) NULL)
        return 0;

    if (colors > 256UL)
    {
        image->colors = 0;
        image->storage_class = DirectClass;
        strcpy(image->error, "UnableToCreateColormap");

        return 0;
    }

    if (colors > 1)
        image->colors = colors;
    else
        image->colors = 1;
    if (image->colormap == (PixelInfo*) NULL)
        image->colormap = (PixelInfo*) malloc((image->colors + 1) * sizeof(*image->colormap));
    else
        image->colormap = (PixelInfo*) realloc(image->colormap, (image->colors + 1) * sizeof(*image->colormap));

    if (image->colormap == (PixelInfo*) NULL)
    {
        image->colors = 0;
        image->storage_class = DirectClass;
        strcpy(image->error, "MemoryAllocationFailed");

        return 0;
    }

    for (i = 0; i < image->colors; i++)
    {
        double
                pixel;

        GetPixelInfo(image, image->colormap+i);
        if (colors - 1 > 1)
            pixel = ((double) i * (255 / (colors - 1)));
        else
            pixel = ((double) i * 255);

        image->colormap[i].red = pixel;
        image->colormap[i].green = pixel;
        image->colormap[i].blue = pixel;
        image->colormap[i].alpha = 255.0;
        image->colormap[i].alpha_trait = BlendPixelTrait;
    }
    image->storage_class=PseudoClass;
    return 1;
}

static inline int IsRGBColorspace(const ColorspaceType colorspace)
{
  if ((colorspace == RGBColorspace) || (colorspace == scRGBColorspace) ||
      (colorspace == LinearGRAYColorspace))
    return 1;
  return 0;
}

static inline int IssRGBColorspace(
  const ColorspaceType colorspace)
{
  if ((colorspace == sRGBColorspace) || (colorspace == TransparentColorspace))
    return 1;
  return 0;
}

static inline int IsGrayColorspace(
  const ColorspaceType colorspace)
{
  if ((colorspace == LinearGRAYColorspace) || (colorspace == GRAYColorspace))
    return 1;
  return 0;
}

int SetImageColorspace(ImagePICT *image, const ColorspaceType colorspace)
{
  ImageType
    type;

  if (image->colorspace == colorspace)
    return 1;
  image->colorspace=colorspace;
  image->gamma=1.000/2.200;
  (void) memset(&image->chromaticity,0,sizeof(image->chromaticity));
  type=image->type;
  if (IsGrayColorspace(colorspace) != 0)
    {
      if (colorspace == LinearGRAYColorspace)
        image->gamma=1.000;
      type=GrayscaleType;
    }
  else
    if ((IsRGBColorspace(colorspace) != 0) ||
        (colorspace == XYZColorspace) || (colorspace == xyYColorspace))
      image->gamma=1.000;
    else
      {
        image->chromaticity.red_primary.x=0.6400;
        image->chromaticity.red_primary.y=0.3300;
        image->chromaticity.red_primary.z=0.0300;
        image->chromaticity.green_primary.x=0.3000;
        image->chromaticity.green_primary.y=0.6000;
        image->chromaticity.green_primary.z=0.1000;
        image->chromaticity.blue_primary.x=0.1500;
        image->chromaticity.blue_primary.y=0.0600;
        image->chromaticity.blue_primary.z=0.7900;
        image->chromaticity.white_point.x=0.3127;
        image->chromaticity.white_point.y=0.3290;
        image->chromaticity.white_point.z=0.3583;
      }
  if (image->ppixels == (unsigned char*) NULL)
      return 0;
  image->type=type;
  return 1;
}

long long CastDoubleToLong(const double x)
{
  if (floor(x) > ((double) LLONG_MAX-1))
    {
      return((long long) LLONG_MAX);
    }
  if (ceil(x) < ((double) LLONG_MIN+1))
    {
      return((long long) LLONG_MIN);
    }
  return((long long) x);
}

static inline unsigned char ClampPixel(const double pixel)
{
    if (pixel < 0.0)
        return ((unsigned char) 0);
    if (pixel >= 255.0)
        return ((unsigned char) 255);
    return ((unsigned char) (pixel));
}

static inline unsigned char ClampToQuantum(const double pixel)
{
    if ((isnan(pixel) != 0) || (pixel <= 0.0))
        return ((unsigned char) 0);
    if (pixel >= 255.0)
        return (unsigned char) 255;
    return ((unsigned char) (pixel));
}

int Clamp(double x, double min, double max) {
    if (x < min) {
        return min;
    } else if (x > max) {
        return max;
    } else {
        return x;
    }
}

static inline int CopyPixel(const ImagePICT *image,
  const unsigned char *source,unsigned char *destination)
{
  long long
    i;

  if (source == (const unsigned char *) NULL)
    {
      destination[RedPixelChannel]=(unsigned char) (Clamp(image->background_color.red, 0.0f, 255.0f) + 0.5f);
      destination[GreenPixelChannel]=(unsigned char) (Clamp(image->background_color.green, 0.0f, 255.0f) + 0.5f);
      destination[BluePixelChannel]=(unsigned char) (Clamp(image->background_color.blue, 0.0f, 255.0f) + 0.5f);
      destination[BlackPixelChannel]=(unsigned char) (Clamp(image->background_color.black, 0.0f, 255.0f) + 0.5f);
      destination[AlphaPixelChannel]=(unsigned char) (Clamp(image->background_color.alpha, 0.0f, 255.0f) + 0.5f);
      return 0;
    }
  for (i=0; i < 4; i++)
  {
    PixelChannel channel = GetPixelChannelChannel(image,i);
    destination[channel]=source[i];
  }
  return 1;
}

int GetOneVirtualPixel(ImagePICT *image,const long long x,const long long y,unsigned char *pixel)
{
  const unsigned char
    *p;

  (void) memset(pixel,0,64*sizeof(*pixel));
  p = GetPixels(image, x, y, 1UL, 1UL);
  return(CopyPixel(image,p,pixel));
}

void AquireImage(ImagePICT* image)
{
    image->storage_class = DirectClass;
    image->colorspace = sRGBColorspace;
    image->gamma=1.000/2.200;
    image->chromaticity.red_primary.x=0.6400;
    image->chromaticity.red_primary.y=0.3300;
    image->chromaticity.red_primary.z=0.0300;
    image->chromaticity.green_primary.x=0.3000;
    image->chromaticity.green_primary.y=0.6000;
    image->chromaticity.green_primary.z=0.1000;
    image->chromaticity.blue_primary.x=0.1500;
    image->chromaticity.blue_primary.y=0.0600;
    image->chromaticity.blue_primary.z=0.7900;
    image->chromaticity.white_point.x=0.3127;
    image->chromaticity.white_point.y=0.3290;
    image->chromaticity.white_point.z=0.3583;
    image->m_pctVersion = 0;
    image->m_nHeight = 0;
    image->m_nWidth = 0;
    image->m_ndepth = 8;
    image->colors = 0;
    image->profiles = NULL;
    image->artifacts = NULL;
    image->fuzz = 0.0;
    image->resolutionX = 0.0;
    image->resolutionY = 0.0;
    image->type = UndefinedType;
    GetPixelInfoRGBA(BackgroundColorRGBA, &image->background_color);
    image->alpha_trait = UndefinedPixelTrait;
    image->ppixels = NULL;
    image->m_nPixelsSize = 0;
    image->colormap = NULL;
    image->channel_map = AcquirePixelChannelMap();
    image->mask_trait = UndefinedPixelTrait;
    image->taint = 0;
    image->number_channels = 4;
}

size_t GetSize(FILE* file)
{
    long long
            file_discription;

    struct stat
            st;

    file_discription = fileno(file);
    if (fstat(file_discription, &st) == 0)
        return st.st_size;

    return(0);
}

int Read(FILE* file, const int length, void* data)
{
    if (!file) return 0;
    if (data == NULL) return 0;
    unsigned char* q;
    q = (unsigned char*) data;
    return fread(q, 1, length, file);
}

int ReadByte(FILE* file)
{
    return getc(file);
}

const void *ReadBlobStream(FILE* file, const size_t length, void *data, long long* count)
{
    *count = Read(file, length, (unsigned char*) data);
    return data;
}

unsigned short ReadShortValue(FILE* file)
{
    unsigned short
            value;

    unsigned char
            buffer[2];

    const unsigned char
            *p;

    long long
            count;

    *buffer='\0';

    p = (const unsigned char*) ReadBlobStream(file, 2, buffer, &count);
    if (count != 2)
    {
        return(EOF);
    }
    value = (unsigned short) ((*p++) << 8);
    value |= (unsigned short) (*p++);
    return ((unsigned short) (value & 0xffff));
}

signed short ReadSignedShortValue(FILE* file)
{
    union
    {
        unsigned short unsigned_value;

        signed short signed_value;
    } Quantum;

    Quantum.unsigned_value = ReadShortValue(file);
    return (Quantum.signed_value);
}

unsigned int ReadLongValue(FILE* file)
{
    const unsigned char
            *p;

    unsigned int
            value;

    int
            count;
    unsigned char
            buffer[4];

    *buffer='\0';
    count = Read(file, 4, buffer);

    if (count != 4)
    {
        return(EOF);
    }

    p = (const unsigned char*) buffer;

    value=(unsigned int) (*p++) << 24;
    value|=(unsigned int) (*p++) << 16;
    value|=(unsigned int) (*p++) << 8;
    value|=(unsigned int) (*p++);

    return value;
}

int ReadRectangle(FILE* file, PICTrectangle *frame)
{
    frame->top = (short) ReadShortValue(file);
    frame->left = (short) ReadShortValue(file);
    frame->bottom = (short) ReadShortValue(file);
    frame->right = (short) ReadShortValue(file);

    if (feof(file) != 0)
    {
        return 0;
    }

    if (frame->bottom < frame->top)
    {
        return 0;
    }

    if (frame->right < frame->left)
    {
        return 0;
    }

    return 1;
}

int ReadPixmap(FILE* file,PICTPixmap *pixmap)
{
  pixmap->version=(short) ReadShortValue(file);
  pixmap->pack_type=(short) ReadShortValue(file);
  pixmap->pack_size=ReadLongValue(file);
  pixmap->horizontal_resolution=1UL*ReadShortValue(file);
  (void) ReadShortValue(file);
  pixmap->vertical_resolution=1UL*ReadShortValue(file);
  (void) ReadShortValue(file);
  pixmap->pixel_type=(short) ReadShortValue(file);
  pixmap->bits_per_pixel=(short) ReadShortValue(file);
  pixmap->component_count=(short) ReadShortValue(file);
  pixmap->component_size=(short) ReadShortValue(file);
  pixmap->plane_bytes=ReadLongValue(file);
  pixmap->table=ReadLongValue(file);
  pixmap->reserved=ReadLongValue(file);
  if ((feof(file) != 0) || (pixmap->bits_per_pixel <= 0) ||
      (pixmap->bits_per_pixel > 32) || (pixmap->component_count <= 0) ||
      (pixmap->component_count > 4) || (pixmap->component_size <= 0))
    return(0);
  return(1);
}

SplayTreeInfo *DestroySplayTree(SplayTreeInfo *splay_tree)
{
  NodeInfo
    *node;

  NodeInfo
    *active,
    *pend;

  if (splay_tree->root != (NodeInfo *) NULL)
    {
      for (pend=splay_tree->root; pend != (NodeInfo *) NULL; )
      {
        active=pend;
        for (pend=(NodeInfo *) NULL; active != (NodeInfo *) NULL; )
        {
          if (active->left != (NodeInfo *) NULL)
          {
              free(active->left->key);
              free(active->left->value);
              pend=active->left;
          }
          if (active->right != (NodeInfo *) NULL)
          {
              free(active->right->key);
              free(active->right->value);
              pend=active->right;
          }
          node=active;
          active=(NodeInfo *) node->key;
          free(node);
        }
      }
    }

  free(splay_tree);
  return(splay_tree);
}

SplayTreeInfo *NewSplayTree(int (*compare)(const void *,const void *))
{
  SplayTreeInfo
    *splay_tree;

  splay_tree=(SplayTreeInfo *) malloc(sizeof(*splay_tree));
  (void) memset(splay_tree,0,sizeof(*splay_tree));
  splay_tree->root=(NodeInfo *) NULL;
  splay_tree->compare=compare;
  splay_tree->balance=0;
  splay_tree->key=(void *) NULL;
  splay_tree->next=(void *) NULL;
  splay_tree->nodes=0;
  splay_tree->signature=0xabacadabUL;
  return(splay_tree);
}

static inline void *GetFirstSplayTreeNode(SplayTreeInfo *splay_tree)
{
  NodeInfo
    *node;

  node=splay_tree->root;
  if (splay_tree->root == (NodeInfo *) NULL)
    return((NodeInfo *) NULL);
  while (node->left != (NodeInfo *) NULL)
    node=node->left;
  return(node->key);
}

NodeInfo *Splay(SplayTreeInfo *splay_tree,const size_t depth,const void *key,NodeInfo **node,NodeInfo **parent,NodeInfo **grandparent)
{
  int
    compare;

  NodeInfo
    **next;

  NodeInfo
    *n,
    *p;

  n=(*node);
  if (n == (NodeInfo *) NULL)
    {
      if (parent != (NodeInfo **) NULL)
        return(*parent);
      else
        return((NodeInfo *) NULL);
    }
  if (splay_tree->compare != (int (*)(const void *,const void *)) NULL)
    compare=splay_tree->compare(n->key,key);
  else
    compare=(n->key > key) ? 1 : ((n->key < key) ? -1 : 0);
  next=(NodeInfo **) NULL;
  if (compare > 0)
    next=(&n->left);
  else
    if (compare < 0)
      next=(&n->right);
  if (next != (NodeInfo **) NULL)
    {
      if (depth >= 1024)
        {
          splay_tree->balance=1;
          return(n);
        }
      n=Splay(splay_tree,depth+1,key,next,node,parent);
      if ((n != *node) || (splay_tree->balance != 0))
        return(n);
    }
  if (parent == (NodeInfo **) NULL)
    return(n);
  if (grandparent == (NodeInfo **) NULL)
    {
      if (n == (*parent)->left)
        {
          *node=n->right;
          n->right=(*parent);
        }
      else
        {
          *node=n->left;
          n->left=(*parent);
        }
      *parent=n;
      return(n);
    }
  if ((n == (*parent)->left) && (*parent == (*grandparent)->left))
    {
      p=(*parent);
      (*grandparent)->left=p->right;
      p->right=(*grandparent);
      p->left=n->right;
      n->right=p;
      *grandparent=n;
      return(n);
    }
  if ((n == (*parent)->right) && (*parent == (*grandparent)->right))
    {
      p=(*parent);
      (*grandparent)->right=p->left;
      p->left=(*grandparent);
      p->right=n->left;
      n->left=p;
      *grandparent=n;
      return(n);
    }
  if (n == (*parent)->left)
    {
      (*parent)->left=n->right;
      n->right=(*parent);
      (*grandparent)->right=n->left;
      n->left=(*grandparent);
      *grandparent=n;
      return(n);
    }
  (*parent)->right=n->left;
  n->left=(*parent);
  (*grandparent)->left=n->right;
  n->right=(*grandparent);
  *grandparent=n;
  return(n);
}

int IterateOverSplayTree(SplayTreeInfo *splay_tree,
  int (*method)(NodeInfo *,const void *),const void *value)
{
  typedef enum
  {
    LeftTransition,
    RightTransition,
    DownTransition,
    UpTransition
  } TransitionType;

  int
    status;

  int
    final_transition;

  NodeInfo
    **nodes;

  long long
    i;

  NodeInfo
    *node;

  TransitionType
    transition;

  unsigned char
    *transitions;

  if (splay_tree->root == (NodeInfo *) NULL)
    return(0);
  nodes=(NodeInfo **) malloc((size_t) splay_tree->nodes*sizeof(*nodes));
  transitions=(unsigned char *) malloc((size_t) splay_tree->nodes*sizeof(*transitions));
  if ((nodes == (NodeInfo **) NULL) || (transitions == (unsigned char *) NULL))
    return 0;
  status=0;
  final_transition=0;
  nodes[0]=splay_tree->root;
  transitions[0]=(unsigned char) LeftTransition;
  for (i=0; final_transition == 0; )
  {
    node=nodes[i];
    transition=(TransitionType) transitions[i];
    switch (transition)
    {
      case LeftTransition:
      {
        transitions[i]=(unsigned char) DownTransition;
        if (node->left == (NodeInfo *) NULL)
          break;
        i++;
        nodes[i]=node->left;
        transitions[i]=(unsigned char) LeftTransition;
        break;
      }
      case RightTransition:
      {
        transitions[i]=(unsigned char) UpTransition;
        if (node->right == (NodeInfo *) NULL)
          break;
        i++;
        nodes[i]=node->right;
        transitions[i]=(unsigned char) LeftTransition;
        break;
      }
      case DownTransition:
      default:
      {
        transitions[i]=(unsigned char) RightTransition;
        status=(*method)(node,value);
        if (status != 0)
          final_transition=1;
        break;
      }
      case UpTransition:
      {
        if (i == 0)
          {
            final_transition=1;
            break;
          }
        i--;
        break;
      }
    }
  }
  free(nodes);
  free(transitions);
  return(status);
}

NodeInfo *LinkSplayTreeNodes(NodeInfo **nodes,const size_t low,const size_t high)
{
  NodeInfo
    *node;

  size_t
    bisect;

  bisect=low+(high-low)/2;
  node=nodes[bisect];
  if ((low+1) > bisect)
    node->left=(NodeInfo *) NULL;
  else
    node->left=LinkSplayTreeNodes(nodes,low,bisect-1);
  if ((bisect+1) > high)
    node->right=(NodeInfo *) NULL;
  else
    node->right=LinkSplayTreeNodes(nodes,bisect+1,high);
  return(node);
}

inline int SplayTreeToNodeArray(NodeInfo *node,const void *nodes)
{
  const NodeInfo
    ***p;

  p=(const NodeInfo ***) nodes;
  *(*p)=node;
  (*p)++;
  return(0);
}

void BalanceSplayTree(SplayTreeInfo *splay_tree)
{
  NodeInfo
    **node,
    **nodes;

  if (splay_tree->nodes <= 2)
    {
      splay_tree->balance=0;
      return;
    }
  nodes=(NodeInfo **) malloc((size_t) splay_tree->nodes*sizeof(*nodes));
  if (nodes == (NodeInfo **) NULL)
    return;
  node=nodes;
  (void) IterateOverSplayTree(splay_tree,SplayTreeToNodeArray,(const void *)
    &node);
  splay_tree->root=LinkSplayTreeNodes(nodes,0,splay_tree->nodes-1);
  splay_tree->balance=0;
  free(nodes);
}

void SplaySplayTree(SplayTreeInfo *splay_tree,const void *key)
{
  if (splay_tree->root == (NodeInfo *) NULL)
    return;
  if (splay_tree->key != (void *) NULL)
    {
      int
        compare;

      if (splay_tree->compare != (int (*)(const void *,const void *)) NULL)
        compare=splay_tree->compare(splay_tree->root->key,key);
      else
        compare=(splay_tree->key > key) ? 1 :
          ((splay_tree->key < key) ? -1 : 0);
      if (compare == 0)
        return;
    }
  (void) Splay(splay_tree,0UL,key,&splay_tree->root,(NodeInfo **) NULL,
    (NodeInfo **) NULL);
  if (splay_tree->balance != 0)
    {
      BalanceSplayTree(splay_tree);
      (void) Splay(splay_tree,0UL,key,&splay_tree->root,(NodeInfo **) NULL,
        (NodeInfo **) NULL);
      if (splay_tree->balance != 0)
        return;
    }
  splay_tree->key=(void *) key;
}

int AddValueToSplayTree(SplayTreeInfo *splay_tree,const void *key,const void *value)
{
  int
    compare;

  NodeInfo
    *node;

  SplaySplayTree(splay_tree,key);
  compare=0;
  if (splay_tree->root != (NodeInfo *) NULL)
    {
      if (splay_tree->compare != (int (*)(const void *,const void *)) NULL)
        compare=splay_tree->compare(splay_tree->root->key,key);
      else
        compare=(splay_tree->root->key > key) ? 1 :
          ((splay_tree->root->key < key) ? -1 : 0);
      if (compare == 0)
        {
          splay_tree->root->key=(void *) key;
          splay_tree->root->value=(void *) value;
          return(1);
        }
    }
  node=(NodeInfo *) malloc(sizeof(*node));
  if (node == (NodeInfo *) NULL)
      return(0);
  node->key=(void *) key;
  node->value=(void *) value;
  if (splay_tree->root == (NodeInfo *) NULL)
    {
      node->left=(NodeInfo *) NULL;
      node->right=(NodeInfo *) NULL;
    }
  else
    if (compare < 0)
      {
        node->left=splay_tree->root;
        node->right=node->left->right;
        node->left->right=(NodeInfo *) NULL;
      }
    else
      {
        node->right=splay_tree->root;
        node->left=node->right->left;
        node->right->left=(NodeInfo *) NULL;
      }
  splay_tree->root=node;
  splay_tree->key=(void *) NULL;
  splay_tree->nodes++;
  return(1);
}

SplayTreeInfo *CloneSplayTree(SplayTreeInfo *splay_tree, void *(*clone_key)(void *),void *(*clone_value)(void *))
{
  NodeInfo
    *next,
    *node;

  SplayTreeInfo
    *clone_tree;

  clone_tree=NewSplayTree(splay_tree->compare);
  if (splay_tree->root == (NodeInfo *) NULL)
    {
      return(clone_tree);
    }
  next=(NodeInfo *) GetFirstSplayTreeNode(splay_tree);
  while (next != (NodeInfo *) NULL)
  {
    SplaySplayTree(splay_tree,next);
    (void) AddValueToSplayTree(clone_tree,clone_key(splay_tree->root->key),
      clone_value(splay_tree->root->value));
    next=(NodeInfo *) NULL;
    node=splay_tree->root->right;
    if (node != (NodeInfo *) NULL)
      {
        while (node->left != (NodeInfo *) NULL)
          node=node->left;
        next=(NodeInfo *) node->key;
      }
  }
  return(clone_tree);
}

int CompareSplayTreeString(const void *target,const void *source)
{
  const char
    *p,
    *q;

  p=(const char *) target;
  q=(const char *) source;
  return(LocaleCompare(p,q));
}

const void *GetValueFromSplayTree(SplayTreeInfo *splay_tree, const void *key)
{
  int
    compare;

  void
    *value;

  if (splay_tree->root == (NodeInfo *) NULL)
    return((void *) NULL);

  SplaySplayTree(splay_tree,key);
  if (splay_tree->compare != (int (*)(const void *,const void *)) NULL)
    compare=splay_tree->compare(splay_tree->root->key,key);
  else
    compare=(splay_tree->root->key > key) ? 1 :
      ((splay_tree->root->key < key) ? -1 : 0);
  if (compare != 0)
      return((void *) NULL);
  value=splay_tree->root->value;
  return(value);
}

const void *GetRootValueFromSplayTree(SplayTreeInfo *splay_tree)
{
  const void
    *value;

  value=(const void *) NULL;
  if (splay_tree->root != (NodeInfo *) NULL)
    value=splay_tree->root->value;

  return(value);
}

const unsigned char *ReadResourceShort(const unsigned char *p, unsigned short *Quantum)
{
  *Quantum=(unsigned short) (*p++) << 8;
  *Quantum|=(unsigned short) (*p++);
  return(p);
}

const unsigned char *ReadResourceByte(const unsigned char *p, unsigned char *Quantum)
{
  *Quantum=(*p++);
  return(p);
}

const unsigned char *ReadResourceLong(const unsigned char *p, unsigned int *Quantum)
{
  *Quantum=(unsigned int) (*p++) << 24;
  *Quantum|=(unsigned int) (*p++) << 16;
  *Quantum|=(unsigned int) (*p++) << 8;
  *Quantum|=(unsigned int) (*p++);
  return(p);
}

void WriteResourceLong(unsigned char *p,
  const unsigned int Quantum)
{
  unsigned char
    buffer[4];

  buffer[0]=(unsigned char) (Quantum >> 24);
  buffer[1]=(unsigned char) (Quantum >> 16);
  buffer[2]=(unsigned char) (Quantum >> 8);
  buffer[3]=(unsigned char) Quantum;
  (void) memcpy(p,buffer,4);
}

void WriteTo8BimProfile(ImagePICT *image,const char *name, const StringInfo *profile)
{
  const unsigned char
    *datum,
    *q;

  const unsigned char
    *p;

  size_t
    length;

  StringInfo
    *profile_8bim;

  long long
    count;

  unsigned char
    length_byte;

  unsigned int
    value;

  unsigned short
    id,
    profile_id;

  if (LocaleCompare(name,"icc") == 0)
    profile_id=0x040f;
  else
    if (LocaleCompare(name,"iptc") == 0)
      profile_id=0x0404;
    else
      if (LocaleCompare(name,"xmp") == 0)
        profile_id=0x0424;
      else
        return;
  profile_8bim=(StringInfo *) GetValueFromSplayTree((SplayTreeInfo *) image->profiles,"8bim");
  if (profile_8bim == (StringInfo *) NULL)
    return;
  datum=profile_8bim->datum;
  length=profile_8bim->length;
  for (p=datum; p < (datum+length-16); )
  {
    q=p;
    if (LocaleNCompare((char *) p,"8BIM",4) != 0)
      break;
    p+=4;
    p=ReadResourceShort(p,&id);
    p=ReadResourceByte(p,&length_byte);
    p+=length_byte;
    if (((length_byte+1) & 0x01) != 0)
      p++;
    if (p > (datum+length-4))
      break;
    p=ReadResourceLong(p,&value);
    count=(long long) value;
    if ((count & 0x01) != 0)
      count++;
    if ((count < 0) || (p > (datum+length-count)) || (count > (long long) length))
      break;
    if (id != profile_id)
      p+=count;
    else
      {
        size_t
          extent,
          offset;

        long long
          extract_extent;

        StringInfo
          *extract_profile;

        extract_extent=0;
        extent=(size_t) ((datum+length)-(p+count));
        if (profile == (StringInfo *) NULL)
          {
            offset=(size_t) (q-datum);
            extract_profile=AcquireStringInfo(offset+extent);
            (void) memcpy(extract_profile->datum,datum,offset);
          }
        else
          {
            offset=(size_t) (p-datum);
            extract_extent=(long long) profile->length;
            if ((extract_extent & 0x01) != 0)
              extract_extent++;
            extract_profile=AcquireStringInfo(offset+(size_t) extract_extent+
              extent);
            (void) memcpy(extract_profile->datum,datum,offset-4);
            WriteResourceLong(extract_profile->datum+offset-4,(unsigned int)
              profile->length);
            (void) memcpy(extract_profile->datum+offset,
              profile->datum,profile->length);
          }
        (void) memcpy(extract_profile->datum+offset+extract_extent,
          p+count,extent);
        (void) AddValueToSplayTree((SplayTreeInfo *) image->profiles, ConstantString("8bim"),CloneStringInfo(extract_profile));
        extract_profile=DestroyStringInfo(extract_profile);
        break;
      }
  }
}

int SetImageProfileInternal(ImagePICT *image,const char *name,const StringInfo *profile,const int recursive)
{
  char
    key[4096];

  int
    status;

  StringInfo
    *clone_profile;

  clone_profile=CloneStringInfo(profile);
  if (image->profiles == (SplayTreeInfo *) NULL)
    image->profiles=NewSplayTree(CompareSplayTreeString);
  (void) CopyMagickString(key,name,4096);
  LocaleLower(key);
  status=AddValueToSplayTree((SplayTreeInfo *) image->profiles,ConstantString(key),clone_profile);
  if (status != 0)
    {
        WriteTo8BimProfile(image,name,clone_profile);
    }
  return(status);
}

const char *GetImageArtifact(const ImagePICT *image,const char *artifact)
{
  const char
    *p;

  p=(const char *) NULL;
  if (image->artifacts != (void *) NULL)
    {
      if (artifact == (const char *) NULL)
        return((const char *) GetRootValueFromSplayTree((SplayTreeInfo *)
          image->artifacts));
      p=(const char *) GetValueFromSplayTree((SplayTreeInfo *) image->artifacts,
        artifact);
      if (p != (const char *) NULL)
        return(p);
    }

  return(p);
}

ImagePICT *DestroyImage(ImagePICT *image)
{
  /*
    Destroy image.
  */
  free(image->ppixels);
  free(image->channel_map);
  delete image;
  return(ImagePICT *) NULL;
}

ImagePICT* CloneImage(const ImagePICT* image, const size_t colums, const size_t rows)
{
    if (image == (const ImagePICT*) NULL)
    {
        return((ImagePICT*) NULL);
    }

    ImagePICT* clone_image = new ImagePICT;

    AquireImage(clone_image);

    clone_image->storage_class = image->storage_class;
    clone_image->fuzz = image->fuzz;
    clone_image->colorspace = image->colorspace;
    clone_image->chromaticity.blue_primary = image->chromaticity.blue_primary;
    clone_image->chromaticity.green_primary = image->chromaticity.green_primary;
    clone_image->chromaticity.red_primary = image->chromaticity.red_primary;
    clone_image->chromaticity.white_point = image->chromaticity.white_point;
    clone_image->gamma = image->gamma;
    clone_image->m_nHeight = image->m_nHeight;
    if (rows != 0)
        clone_image->m_nHeight = rows;
    clone_image->m_nWidth = image->m_nWidth;
    if (colums != 0)
        clone_image->m_nWidth = colums;
    clone_image->number_channels = image->number_channels;
    clone_image->m_nPixelsSize = image->m_nPixelsSize;
    clone_image->ppixels = NULL;
    clone_image->resolutionX = image->resolutionX;
    clone_image->resolutionY = image->resolutionY;
    clone_image->alpha_trait = image->alpha_trait;
    clone_image->background_color.blue = image->background_color.blue;
    clone_image->background_color.green = image->background_color.green;
    clone_image->background_color.red = image->background_color.red;

    clone_image->colors = image->colors;
    if (image->colormap != NULL)
    {
        clone_image->colormap = (PixelInfo*) malloc((clone_image->colors + 1) * sizeof(*clone_image->colormap));
        memcpy(clone_image->colormap, image->colormap, clone_image->colors * sizeof(*clone_image->colormap));
    }
    if (image->channel_map != NULL)
    {
        clone_image->channel_map = (PixelChannelMap*) malloc(65 * sizeof (image->channel_map));
        memcpy(clone_image->channel_map, image->channel_map, 65 * sizeof (image->channel_map));
    }

    if (image->profiles != NULL)
        clone_image->profiles = CloneSplayTree(image->profiles, (void *(*)(void *)) ConstantString,(void *(*)(void *)) CloneStringInfo);
    if (image->artifacts != NULL)
        clone_image->artifacts = CloneSplayTree(image->artifacts, (void *(*)(void *)) ConstantString,(void *(*)(void *)) CloneStringInfo);

    clone_image->mask_trait = image->mask_trait;
    clone_image->taint = image->taint;

    return clone_image;
}

static inline unsigned char GetPixelReadMask(const ImagePICT *image,
  const unsigned char *pixel)
{
  if (image->channel_map[ReadMaskPixelChannel].traits == UndefinedPixelTrait)
    return((unsigned char) 255);
  return(pixel[image->channel_map[ReadMaskPixelChannel].offset]);
}

int CompositeImage(ImagePICT *image, const ImagePICT *composite, const int clip_to_self,const long long x_offset,const long long y_offset)
{
#define CompositeImageTag  "Composite/Image"

  const char
    *value;

  GeometryInfo
    geometry_info;

  ImagePICT
    *canvas_image,
    *source_image;

  int
    clamp,
    compose_sync,
    status;

  long long
    progress;

  double
    amount,
    canvas_dissolve,
    midpoint,
    percent_luma,
    percent_chroma,
    source_dissolve,
    threshold;

  long long
    y;

  image->storage_class = DirectClass;

  if (image->ppixels == (unsigned char*) NULL)
    return 0;
  source_image=CloneImage(composite,0,0);
  if (!AquirePixelsMemory(source_image))
      return 0;
  (void) memcpy(source_image->ppixels, composite->ppixels, source_image->m_nPixelsSize);
  if (source_image == (const ImagePICT *) NULL)
    return 0;
  (void) SetImageColorspace(source_image,image->colorspace);
  amount = 0.5;
  canvas_image = (ImagePICT *) NULL;
  canvas_dissolve = 1.0;
  clamp = 1;
  value = GetImageArtifact(image,"compose:clamp");
  if (value != (const char *) NULL)
    clamp=IsStringTrue(value);
  compose_sync = 1;
  value = GetImageArtifact(image,"compose:sync");
  if (value != (const char *) NULL)
    compose_sync = IsStringTrue(value);
  (void) memset(&geometry_info,0,sizeof(geometry_info));
  percent_luma=100.0;
  percent_chroma=100.0;
  source_dissolve=1.0;
  threshold=0.05f;

  image->channel_map[RedPixelChannel].traits = UpdatePixelTrait;
  image->channel_map[GreenPixelChannel].traits = UpdatePixelTrait;
  image->channel_map[BluePixelChannel].traits = UpdatePixelTrait;
  image->channel_map[AlphaPixelChannel].traits = UpdatePixelTrait;

  source_image->channel_map[RedPixelChannel].traits = CopyPixelTrait;
  source_image->channel_map[GreenPixelChannel].traits = CopyPixelTrait;
  source_image->channel_map[BluePixelChannel].traits = CopyPixelTrait;
  source_image->channel_map[AlphaPixelChannel].traits = CopyPixelTrait;

  if (!((x_offset < 0) || (y_offset < 0)) && !((x_offset+(long long) source_image->m_nWidth) > (long long) image->m_nWidth) && !((y_offset+(long long) source_image->m_nHeight) > (long long) image->m_nHeight))
    {
      if ((source_image->alpha_trait == UndefinedPixelTrait) &&
          (image->alpha_trait != UndefinedPixelTrait))
        (void) SetImageAlpha(source_image, (const unsigned char) 255);
      status = 1;

      for (y=0; y < (long long) source_image->m_nHeight; y++)
      {
        const unsigned char
          *p;

        unsigned char
          *q;

        long long
          x;

        if (status == 0)
          continue;
        p=GetPixels(source_image,0,y,source_image->m_nWidth,1);
        q=GetPixels(image,x_offset,y+y_offset, source_image->m_nWidth,1);
        if ((p == (const unsigned char *) NULL) || (q == (unsigned char *) NULL))
          {
            status=0;
            continue;
          }
        for (x=0; x < (long long) source_image->m_nWidth; x++)
        {
          long long
            i;
            if (GetPixelReadMask(source_image, p) <= (255/2))
            {
              p+=source_image->number_channels;
              q+=image->number_channels;
              continue;
            }
          for (i=0; i < source_image->number_channels; i++)
          {
            PixelChannel channel = GetPixelChannelChannel(source_image,i);
            PixelTrait source_traits = GetPixelChannelTraits(source_image,
              channel);
            PixelTrait traits = GetPixelChannelTraits(image,channel);
            if ((source_traits == UndefinedPixelTrait) ||
                (traits == UndefinedPixelTrait))
              continue;
            SetPixelChannel(image,channel,p[i],q);
          }
          p+=source_image->number_channels;
          q+=image->number_channels;
        }
      }
      source_image=DestroyImage(source_image);
      return(status);
    }

  /*
    Composite image.
  */

  status=1;
  progress=0;
  midpoint=128.0;
  for (y=0; y < (long long) image->m_nHeight; y++)
  {
    const unsigned char
      *pixels;

    PixelInfo
      canvas_pixel,
      source_pixel;

    const unsigned char
      *p;

    unsigned char
      *q;

    long long
      x;

    if (status == 0)
      continue;
    if (clip_to_self != 0)
      {
        if (y < y_offset)
          continue;
        if ((y-(double) y_offset) >= (double) source_image->m_nHeight)
          continue;
      }
    /*
      If pixels is NULL, y is outside overlay region.
    */
    pixels=(unsigned char *) NULL;
    p=(unsigned char *) NULL;
    if ((y >= y_offset) &&
        ((y-(double) y_offset) < (double) source_image->m_nHeight))
      {
        p=GetPixels(source_image,0,y-y_offset,source_image->m_nWidth,1);
        if (p == (const unsigned char *) NULL)
          {
            status=0;
            continue;
          }
        pixels=p;
        if (x_offset < 0)
          p-=x_offset*source_image->number_channels;
      }
    q=GetPixels(image,0,y,image->m_nWidth,1);
    if (q == (unsigned char *) NULL)
      {
        status=0;
        continue;
      }
    GetPixelInfo(image,&canvas_pixel);
    GetPixelInfo(source_image,&source_pixel);
    for (x=0; x < (long long) image->m_nWidth; x++)
    {
      double
        gamma = 0.0;

      double
        alpha = 0.0,
        Da = 0.0,
        Dc = 0.0,
        Dca = 0.0,
        DcaDa = 0.0,
        Sa = 0.0,
        SaSca = 0.0,
        Sc = 0.0,
        Sca = 0.0;

      size_t
        channels;

      long long
        i;

      if (clip_to_self != 0)
        {
          if (x < x_offset)
            {
              q+=image->number_channels;
              continue;
            }
          if ((x-(double) x_offset) >= (double) source_image->m_nWidth)
            break;
        }
      if ((pixels == (unsigned char *) NULL) || (x < x_offset) ||
          ((x-(double) x_offset) >= (double) source_image->m_nWidth))
        {
          unsigned char
            source[64];

          /*
            Virtual composite:
              Sc: source color.
              Dc: canvas color.
          */
          for (i=0; i < image->number_channels; i++)
          {
            double
              pixel = 0;

            PixelChannel channel = GetPixelChannelChannel(image,i);
            PixelTrait traits = GetPixelChannelTraits(image,channel);
            PixelTrait source_traits = GetPixelChannelTraits(source_image,
              channel);
            if ((traits == UndefinedPixelTrait) ||
                (source_traits == UndefinedPixelTrait))
              continue;
//////////////////////////////////////////////////////////////////////////
            if (channel == AlphaPixelChannel)
                pixel =  TransparentAlpha;
            else
                pixel=0;
/////////////////////////////////////////////////////////////////////////
            q[i]=clamp != 0 ? ClampPixel(pixel) :
              ClampToQuantum(pixel);
          }
          q+=image->number_channels;
          continue;
        }
      /*
        Authentic composite:
          Sa:  normalized source alpha.
          Da:  normalized canvas alpha.
      */
      Sa = (1.0/255.0)* GetPixelAlpha(source_image,p);
      Sa = Sa == 0 ? 1 : Sa;
      Da = (1.0/255.0)* GetPixelAlpha(image,q);
///////////////////////////////////////////////////////////////////////
      alpha = 1.0;
//////////////////////////////////////////////////////////////////////
      for (i=0; i < image->number_channels; i++)
      {
        double
          pixel = 0.0;

        PixelChannel channel = GetPixelChannelChannel(image,i);
        PixelTrait traits = GetPixelChannelTraits(image,channel);
        PixelTrait source_traits = GetPixelChannelTraits(source_image,channel);
        if (traits == UndefinedPixelTrait)
          continue;
        if ((channel == AlphaPixelChannel) &&
            ((traits & UpdatePixelTrait) != 0))
          {
            /*
              Set alpha channel.
            */
/////////////////////////////////////////////////////////////////////
             pixel=255.0*Sa;
////////////////////////////////////////////////////////////////////
             q[i]=clamp != 0 ? ClampPixel(pixel) :
               ClampToQuantum(pixel);
            continue;
          }
        if (source_traits == UndefinedPixelTrait)
          continue;
        /*
          Sc: source color.
          Dc: canvas color.
        */
        Sc= GetPixelChannel(source_image,channel,p);
        Dc=q[i];
        if ((traits & CopyPixelTrait) != 0)
          {
            /*
              Copy channel.
            */
            q[i]=ClampToQuantum(Dc);
            continue;
          }
        /*
          Porter-Duff compositions:
            Sca: source normalized color multiplied by alpha.
            Dca: normalized canvas color multiplied by alpha.
        */
        Sca=(1.0/255.0)*Sa*Sc;
        Dca=(1.0/255.0)*Da*Dc;
        SaSca=Sa*PerceptibleReciprocal(Sca);
        DcaDa=Dca*PerceptibleReciprocal(Da);
/////////////////////////////////////////////////////////////////////
        gamma=PerceptibleReciprocal(alpha);
////////////////////////////////////////////////////////////////////
        pixel=Dc;
///////////////////////////////////////////////////////////////////
        pixel=255.0*Sca;
//////////////////////////////////////////////////////////////////
        q[i]=clamp != 0 ? ClampPixel(pixel) :
          ClampToQuantum(pixel);
      }
      p+=source_image->number_channels;
      channels=source_image->number_channels;
      if (p >= (pixels+channels*source_image->m_nWidth))
        p=pixels;
      q+=image->number_channels;
    }
  }
  if (canvas_image != (ImagePICT * ) NULL)
    canvas_image=DestroyImage(canvas_image);
  else
    source_image=DestroyImage(source_image);
  return(status);
}

int DecodeHeader(FILE* hFile, ImagePICT* image)
{
   unsigned char
           header[4];

   unsigned char
           skip[1];

   int
           c;

   PICTrectangle
           frame;

   Read(hFile, 4, header);

   if (!((header[0] == 0x50) && (header[1] == 0x49) &&
         (header[2] == 0x43) && (header[3] == 0x54 )))
   {

       frame.top =(short) ((header[2] << 8) | header[3]);
       frame.left = (short) ReadShortValue(hFile);
       frame.bottom = (short) ReadShortValue(hFile);
       frame.right = (short) ReadShortValue(hFile);

       Read(hFile, 1, skip);
       c = ReadByte(hFile);
       if (c != 0x11)
       {
           fseek(hFile, 512, 0);

           (void) ReadShortValue(hFile);

           if (ReadRectangle(hFile, &frame) == 0)
           {
               strcpy(image->error, "ImproperImageHeader");

               return 0;
           }

           Read(hFile, 1, skip);
           c = ReadByte(hFile);

           if (c == 0x11)
           {
               long long version = ReadByte(hFile);

               if (version == 2)
               {
                   long long version2 = ReadByte(hFile);
                   if (version2 != 0xff)
                       return 0;
                   image->m_pctVersion = 2;
                   if ((frame.left < 0) || (frame.right < 0) || (frame.top < 0) ||
                       (frame.bottom < 0) || (frame.left >= frame.right) ||
                       (frame.top >= frame.bottom))
                   {
                        strcpy(image->error, "ImproperImageHeader");

                        return 0;
                   }
                   image->m_nWidth=(size_t) (frame.right-frame.left);
                   image->m_nHeight=(size_t) (frame.bottom-frame.top);

                   return 1;
               }
               else if (version == 1)
               {
                  image->m_pctVersion = 1;
                  if ((frame.left < 0) || (frame.right < 0) || (frame.top < 0) ||
                      (frame.bottom < 0) || (frame.left >= frame.right) ||
                      (frame.top >= frame.bottom))
                  {
                       strcpy(image->error, "ImproperImageHeader");

                       return 0;
                  }
                  image->m_nWidth=(size_t) (frame.right-frame.left);
                  image->m_nHeight=(size_t) (frame.bottom-frame.top);

                  return 1;
               }
               else
               {
                   strcpy(image->error, "ImproperImageHeader");

                   return 0;
               }
           }
           else
           {
               strcpy(image->error, "ImproperImageHeader");

               return 0;
           }
       }
   }

   long long version = ReadByte(hFile);

   if (version == 2)
   {
       long long version2 = ReadByte(hFile);
       if (version2 != 0xff)
           return 0;
       image->m_pctVersion = 2;

       if ((frame.left < 0) || (frame.right < 0) || (frame.top < 0) ||
             (frame.bottom < 0) || (frame.left >= frame.right) ||
             (frame.top >= frame.bottom))
       {
           strcpy(image->error, "ImproperImageHeader");

           return 0;
       }

       image->m_nWidth = (size_t) (frame.right - frame.left);
       image->m_nHeight=(size_t) (frame.bottom-frame.top);
   }
   else
   {
       image->m_pctVersion = 1;

       image->m_nWidth = (size_t) (frame.right - frame.left);
       image->m_nHeight=(size_t) (frame.bottom-frame.top);
   }

   return 1;
}

static const unsigned char *UnpackScanline(
  const unsigned char *pixels,const unsigned int bits_per_pixel,
  unsigned char* scanline,size_t *bytes_per_line)
{
  const unsigned char
    *p;

  long long
    i;

  unsigned char
    *q;

  p=pixels;
  q=scanline;
  switch (bits_per_pixel)
  {
    case 8:
    case 16:
    case 32:
      return(pixels);
    case 4:
    {
      for (i=0; i < (long long) *bytes_per_line; i++)
      {
        *q++=(*p >> 4) & 0xff;
        *q++=(*p & 15);
        p++;
      }
      *bytes_per_line*=2;
      break;
    }
    case 2:
    {
      for (i=0; i < (long long) *bytes_per_line; i++)
      {
        *q++=(*p >> 6) & 0x03;
        *q++=(*p >> 4) & 0x03;
        *q++=(*p >> 2) & 0x03;
        *q++=(*p & 3);
        p++;
      }
      *bytes_per_line*=4;
      break;
    }
    case 1:
    {
      for (i=0; i < (long long) *bytes_per_line; i++)
      {
        *q++=(*p >> 7) & 0x01;
        *q++=(*p >> 6) & 0x01;
        *q++=(*p >> 5) & 0x01;
        *q++=(*p >> 4) & 0x01;
        *q++=(*p >> 3) & 0x01;
        *q++=(*p >> 2) & 0x01;
        *q++=(*p >> 1) & 0x01;
        *q++=(*p & 0x01);
        p++;
      }
      *bytes_per_line*=8;
      break;
    }
    default:
      break;
  }
  return(scanline);
}

static unsigned char *DecodeImage(FILE *blob,ImagePICT *image,
  size_t bytes_per_line,const unsigned int bits_per_pixel,size_t *extent)
{
  int
    status;

  size_t
    number_pixels;

  const unsigned char
    *p;

  long long
    i;

  unsigned char
    *q;

  size_t
    bytes_per_pixel,
    length,
    row_bytes,
    scanline_length,
    width;

  long long
    count,
    j,
    y;

  unsigned char
    *pixels,
    *scanline,
    unpack_buffer[8*256];

  /*
    Determine pixel buffer size.
  */
  if (bits_per_pixel <= 8)
    bytes_per_line&=0x7fff;
  width=image->m_nWidth;
  bytes_per_pixel=1;
  if (bits_per_pixel == 16)
    {
      bytes_per_pixel=2;
      width*=2;
    }
  else
    if (bits_per_pixel == 32)
      width*=image->alpha_trait ? 4 : 3;
  if (bytes_per_line == 0)
    bytes_per_line=width;
  row_bytes=(size_t) (image->m_nWidth | 0x8000);
  if (image->storage_class == DirectClass)
    row_bytes=(size_t) ((4*image->m_nWidth) | 0x8000);
  /*
    Allocate pixel and scanline buffer.
  */
  pixels=(unsigned char *) malloc(image->m_nHeight*row_bytes*
    sizeof(*pixels));
  if (pixels == (unsigned char *) NULL)
    return((unsigned char *) NULL);
  *extent=row_bytes*image->m_nHeight*sizeof(*pixels);
  (void) memset(pixels,0,*extent);
  scanline=(unsigned char *) malloc(row_bytes*2*
    sizeof(*scanline));
  if (scanline == (unsigned char *) NULL)
    {
      free(pixels);
      return((unsigned char *) NULL);
    }
  (void) memset(scanline,0,2*row_bytes*sizeof(*scanline));
  (void) memset(unpack_buffer,0,sizeof(unpack_buffer));
  status=1;
  if (bytes_per_line < 8)
    {
      /*
        Pixels are already uncompressed.
      */
      for (y=0; y < (long long) image->m_nHeight; y++)
      {
        q=pixels+y*(long long) width*image->number_channels;
        number_pixels=bytes_per_line;
        count=Read(blob,(size_t) number_pixels,scanline);
        if (count != (long long) number_pixels)
          {
            status=0;
            break;
          }
        p=UnpackScanline(scanline,bits_per_pixel,unpack_buffer,&number_pixels);
        if ((q+number_pixels) > (pixels+(*extent)))
          {
            status=0;
            break;
          }
        (void) memcpy(q,p,(size_t) number_pixels);
      }
      free(scanline);
      if (status == 0)
        free(pixels);
      return(pixels);
    }
  /*
    Uncompress RLE pixels into uncompressed pixel buffer.
  */
  for (y=0; y < (long long) image->m_nHeight; y++)
  {
    q=pixels+y*(long long) width;
    if (bytes_per_line > 250)
      scanline_length=ReadShortValue(blob);
    else
      scanline_length=(size_t) ReadByte(blob);
    if ((scanline_length >= row_bytes) || (scanline_length == 0))
      {
        status=0;
        break;
      }
    count=Read(blob,scanline_length,scanline);
    if (count != (long long) scanline_length)
      {
        status=0;
        break;
      }
    for (j=0; j < (long long) scanline_length; )
      if ((scanline[j] & 0x80) == 0)
        {
          length=(size_t) ((scanline[j] & 0xff)+1);
          number_pixels=length*bytes_per_pixel;
          p=UnpackScanline(scanline+j+1,bits_per_pixel,unpack_buffer,
            &number_pixels);
          if ((size_t) (q-pixels+(long long) number_pixels) <= *extent)
            (void) memcpy(q,p,(size_t) number_pixels);
          q+=number_pixels;
          j+=(long long) (length*bytes_per_pixel+1);
        }
      else
        {
          length=(size_t) (((scanline[j] ^ 0xff) & 0xff)+2);
          number_pixels=bytes_per_pixel;
          p=UnpackScanline(scanline+j+1,bits_per_pixel,unpack_buffer,
            &number_pixels);
          for (i=0; i < (long long) length; i++)
          {
            if ((size_t) (q-pixels+(long long) number_pixels) <= *extent)
              (void) memcpy(q,p,(size_t) number_pixels);
            q+=number_pixels;
          }
          j+=(long long) bytes_per_pixel+1;
        }
  }
  free(scanline);
  if (status == 0)
  {
     free(pixels);
     pixels = NULL;
  }
  return(pixels);
}

int DecodePICT(FILE* hFile, ImagePICT* image)
{
    long long
            flags,
            i,
            j,
            count,
            x,
            y;
    int
            jpeg,
            code,
            status;

    size_t
            extent,
            length;

    unsigned char
            index;

    unsigned char
            *q;

    PICTrectangle
            frame;

    PICTPixmap
            pixmap;

    ImagePICT*
            tile_image;

    StringInfo
            *profile;

    if (hFile == NULL)
    {
        strcpy(image->error, "FileError");

        return 0;
    }

    pixmap.bits_per_pixel=0;
    pixmap.component_count=0;

    if (!DecodeHeader(hFile, image))
    {
        return 0;
    }

    if (feof(hFile) != 0)
    {
        strcpy(image->error, "EOFfile");

        return 0;
    }


    flags = 0;
    image->m_ndepth = 8;
    image->resolutionX = 72.0;
    image->resolutionY = 72.0;

    if (!AquirePixelsMemory(image))
    {
        return 0;
    }

    jpeg = 0;
    for (code = 0; feof(hFile) == 0; )
    {
        if ((image->m_pctVersion == 1) || ((ftell(hFile) % 2) != 0))
            code = ReadByte(hFile);
        if (image->m_pctVersion == 2)
            code = ReadSignedShortValue(hFile);
        code &= 0xffff;
        if (code < 0)
            break;
        if (code == 0)
            continue;
        if (code <= 0xa1)
        {
            switch(code)
            {
            case 0x01:
            {
                /*
                    Clipping rectangle.
                */
                length = ReadShortValue(hFile);
                if (length > GetSize(hFile))
                {
                    strcpy(image->error, "InsufficientImageDataInFile");

                    return 0;
                }

                if (length != 0x000a)
                {
                    for (i = 0; i < length - 2; i++)
                        if (ReadByte(hFile) == EOF)
                            break;
                    break;
                }
                if (ReadRectangle(hFile, &frame) == 0)
                    return 0;
                if (((frame.left & 0x8000) != 0) || ((frame.top & 0x8000) != 0))
                    break;
                image->m_nHeight = (size_t) (frame.bottom - frame.top);
                image->m_nWidth = (size_t) (frame.right - frame.left);

                if (!AquirePixelsMemory(image))
                {
                    return 0;
                }
                break;
            }
            case 0x12:
            case 0x13:
            case 0x14:
            {
                int
                        pattern;

                size_t
                        height,
                        width;

                /*
                    Skip pattern definition.
                */
                pattern = (int) ReadShortValue(hFile);
                for (i = 0; i < 8; i++)
                    if (ReadByte(hFile) == EOF)
                        break;
                if (pattern == 2)
                {
                    for (i=0; i < 5; i++)
                      if (ReadByte(hFile) == EOF)
                        break;
                    break;
                }
                if (pattern != 1)
                {
                    DeletePixelsMemory(image);

                    strcpy(image->error, "UnknownPatternType");

                    return 0;
                }

                length = ReadShortValue(hFile);
                if (length > GetSize(hFile))
                {
                    DeletePixelsMemory(image);

                    strcpy(image->error, "InsufficientImageDataInFile");

                    return 0;
                }
                if (ReadRectangle(hFile, &frame) == 0)
                {
                    DeletePixelsMemory(image);

                    strcpy(image->error, "ImproperImageHeader");

                    return 0;
                }
                if (ReadPixmap(hFile, &pixmap) == 0)
                {
                    DeletePixelsMemory(image);

                    strcpy(image->error, "ImproperImageHeader");

                    return 0;
                }

                image->m_ndepth = (size_t) pixmap.component_size;
                image->resolutionX = 1.0 * pixmap.horizontal_resolution;
                image->resolutionY = 1.0 * pixmap.vertical_resolution;

                (void) ReadLongValue(hFile);
                flags=(long long) ReadShortValue(hFile);
                length=ReadShortValue(hFile);

                if (length > GetSize(hFile))
                {
                    DeletePixelsMemory(image);
                    strcpy(image->error, "InsufficientImageDataInFile");

                    return 0;
                }

                for (i = 0; i < length; i++)
                    (void) ReadLongValue(hFile);
                width=(size_t) (frame.bottom-frame.top);
                height=(size_t) (frame.right-frame.left);

                if (pixmap.bits_per_pixel <= 8)
                    length &= 0x7fff;
                if (pixmap.bits_per_pixel == 16)
                    width <<= 1;
                if (length == 0)
                    length = width;
                if (length < 8)
                {
                    for (i = 0; i < (length*height); i++)
                      if (ReadByte(hFile) == EOF)
                        break;
                }
                else
                    for (i = 0; i < height; i++)
                    {
                      size_t
                        scanline_length;

                      if (feof(hFile) != 0)
                        break;
                      if (length > 200)
                        scanline_length=ReadShortValue(hFile);
                      else
                        scanline_length=(size_t) ReadByte(hFile);
                      if (scanline_length > GetSize(hFile))
                      {
                          DeletePixelsMemory(image);

                          strcpy(image->error, "InsufficientImageDataInFile");

                          return 0;
                      }
                      for (j=0; j < scanline_length; j++)
                        if (ReadByte(hFile) == EOF)
                          break;
                    }
                break;
            }
            case 0x1b:
            {
                /*
                    Initialize image background color.
                */
                image->background_color.red = 257.0 * ReadShortValue(hFile);
                image->background_color.green = 257.0 * ReadShortValue(hFile);
                image->background_color.blue = 257.0 * ReadShortValue(hFile);
                break;
            }
            case 0x70:
            case 0x71:
            case 0x72:
            case 0x73:
            case 0x74:
            case 0x75:
            case 0x76:
            case 0x77:
            {
                /*
                    Skip polygon or region.
                */
                length = ReadShortValue(hFile);
                if (length > GetSize(hFile))
                {
                    DeletePixelsMemory(image);

                    strcpy(image->error, "InsufficientImageDataInFile");

                    return 0;
                }
                for (i=0; i < (length-2); i++)
                    if (ReadByte(hFile) == EOF)
                        break;
                break;
            }
            case 0x90:
            case 0x91:
            case 0x98:
            case 0x99:
            case 0x9a:
            case 0x9b:
            {
                PICTrectangle
                  source,
                  destination;

                unsigned char
                  *p;

                size_t
                  k;

                long long
                  bytes_per_line;

                unsigned char
                  *pixels;

                /*
                  Pixmap clipped by a rectangle.
                */
                bytes_per_line = 0;
                if ((code != 0x9a) && (code != 0x9b))
                  bytes_per_line= (long long) ReadShortValue(hFile);
                else
                  {
                    (void) ReadShortValue(hFile);
                    (void) ReadShortValue(hFile);
                    (void) ReadShortValue(hFile);
                  }
                if (ReadRectangle(hFile, &frame) == 0)
                {
                    DeletePixelsMemory(image);

                    strcpy(image->error, "ImproperImageHeader");

                    return 0;
                }

                /*
                    Initialize tile image.
                */
                tile_image = CloneImage(image, (size_t) (frame.right-frame.left), (size_t) (frame.bottom-frame.top));
                if (tile_image == (ImagePICT*) NULL)
                {
                    DeletePixelsMemory(image);

                    strcpy(image->error, "ImproperImageHeader");

                    return 0;
                }
                if (!AquirePixelsMemory(tile_image))
                {
                    DeletePixelsMemory(image);

                    return 0;
                }

                if ((code == 0x9a) || (code == 0x9b) ||
                                ((bytes_per_line & 0x8000) != 0))
                {
                    if (ReadPixmap(hFile, &pixmap) == 0)
                    {
                        DeletePixelsMemory(image);
                        DeletePixelsMemory(tile_image);

                        strcpy(image->error, "ImproperImageHeader");

                        return 0;
                    }
                    tile_image->m_ndepth=(size_t) pixmap.component_size;
                    tile_image->alpha_trait=pixmap.component_count == 4 ?
                      BlendPixelTrait : UndefinedPixelTrait;
                    tile_image->resolutionX=(double) pixmap.horizontal_resolution;
                    tile_image->resolutionY=(double) pixmap.vertical_resolution;
                    if (tile_image->alpha_trait != UndefinedPixelTrait)
                      (void) SetImageAlpha(tile_image, 255);
                }
                if ((code != 0x9a) && (code != 0x9b))
                {
                    /*
                      Initialize colormap.
                    */
                    tile_image->colors = 2;
                    if ((bytes_per_line & 0x8000) != 0)
                    {
                        (void) ReadLongValue(hFile);
                        flags = (long long) ReadShortValue(hFile);
                        tile_image->colors = 1UL * ReadShortValue(hFile) + 1;
                    }
                    status = AquireImageColormap(tile_image, tile_image->colors);
                    if (status == 0)
                    {
                        DeletePixelsMemory(image);
                        DeletePixelsMemory(tile_image);

                        return 0;
                    }

                    if ((bytes_per_line & 0x8000) != 0)
                      {
                        for (i = 0; i < tile_image->colors; i++)
                        {
                          k = ReadShortValue(hFile) % tile_image->colors;
                          if ((flags & 0x8000) != 0)
                            k = (size_t) i;
                          tile_image->colormap[k].red = (unsigned char) ReadShortValue(hFile)/* + 128U / 257U*/;
                          tile_image->colormap[k].green = (unsigned char) ReadShortValue(hFile)/* + 128U / 257U*/;
                          tile_image->colormap[k].blue = (unsigned char) ReadShortValue(hFile)/* + 128U / 257U*/;
                        }
                      }
                    else
                      {
                        for (i=0; i < (long long) tile_image->colors; i++)
                        {
                          tile_image->colormap[i].red=((double) 255 - tile_image->colormap[i].blue);
                          tile_image->colormap[i].green=((double) 255 - tile_image->colormap[i].green);
                          tile_image->colormap[i].blue=((double) 255 - tile_image->colormap[i].red);
                        }
                      }
                }
                if (feof(hFile) != 0)
                {
                    DeletePixelsMemory(image);
                    DeletePixelsMemory(tile_image);

                    strcpy(image->error, "EOFfile");

                    return 0;
                }

                if (ReadRectangle(hFile, &source) == 0)
                {
                    DeletePixelsMemory(image);
                    DeletePixelsMemory(tile_image);

                    strcpy(image->error, "ImproperImageHeader");

                    return 0;
                }

                if (ReadRectangle(hFile, &destination) == 0)
                {
                    DeletePixelsMemory(image);
                    DeletePixelsMemory(tile_image);

                    strcpy(image->error, "ImproperImageHeader");

                    return 0;
                }

                (void) ReadShortValue(hFile);

                if ((code == 0x91) || (code == 0x99) || (code == 0x9b))
                {
                    /*
                      Skip region.
                    */
                    length = ReadShortValue(hFile);
                    if ((size_t) length > GetSize(hFile))
                    {
                        DeletePixelsMemory(image);
                        DeletePixelsMemory(tile_image);

                        strcpy(image->error, "InsufficientImageDataInFile");

                        return 0;
                    }

                    for (i = 0; i < length - 2; i++)
                        if (ReadByte(hFile) == EOF)
                            break;
                }

                if ((code != 0x9a) && (code != 0x9b) && (bytes_per_line & 0x8000) == 0)
                    pixels = DecodeImage(hFile, tile_image, (size_t) bytes_per_line, 1, &extent);
                else
                    pixels = DecodeImage(hFile, tile_image, (size_t) bytes_per_line, (unsigned int) pixmap.bits_per_pixel, &extent);

                if (pixels == (unsigned char*) NULL)
                {
                    DeletePixelsMemory(image);
                    DeletePixelsMemory(tile_image);

                    strcpy(image->error, "UnableToUncompressImage");

                    return 0;
                }

                /*
                  Convert PICT tile image to pixel packets.
                */
                p = pixels;
                for (y=0; y < (long long) tile_image->m_nHeight; y++)
                {
                  if (p > (pixels+extent+image->m_nWidth))
                    {
                      free(pixels);
                      DeletePixelsMemory(image);
                      DeletePixelsMemory(tile_image);

                      strcpy(image->error, "NotEnoughPixelData");

                      return 0;
                    }
                  q = tile_image->ppixels + tile_image->number_channels * (y * tile_image->m_nWidth);
                  if (q == (unsigned char *) NULL)
                    break;
                  for (x=0; x < tile_image->m_nWidth; x++)
                  {
                    if (tile_image->storage_class == PseudoClass)
                      {
                        if (((long long) *p < 0) || ((long long) *p >= (long long) tile_image->colors))
                            index=0;
                        else
                            index=(long long) *p;
                        SetPixelIndex(tile_image,index,q);
                        SetPixelRed(tile_image,
                          tile_image->colormap[(long long) index].red,q);
                        SetPixelGreen(tile_image,
                          tile_image->colormap[(long long) index].green,q);
                        SetPixelBlue(tile_image,
                          tile_image->colormap[(long long) index].blue,q);
                      }
                    else
                      {
                        if (pixmap.bits_per_pixel == 16)
                          {
                            i=(long long) (*p++);
                            k=(size_t) (*p);
                            SetPixelRed(tile_image,(unsigned char) ((i & 0x7c) << 1),q);
                            SetPixelGreen(tile_image,(unsigned char) ((size_t) ((i & 0x03) << 6) |((k & 0xe0) >> 2)),q);
                            SetPixelBlue(tile_image,(unsigned char) ((k & 0x1f) << 3),q);
                          }
                        else
                          if (tile_image->alpha_trait == UndefinedPixelTrait)
                            {
                              if (p > (pixels+extent+2*image->m_nWidth))
                              {
                                free(pixels);
                                DeletePixelsMemory(image);
                                DeletePixelsMemory(tile_image);

                                strcpy(image->error, "NotEnoughPixelData");

                                return 0;
                              }
                              SetPixelRed(tile_image,*p,q);
                              SetPixelGreen(tile_image,*(p+tile_image->m_nWidth),q);
                              SetPixelBlue(tile_image,*(p+2*tile_image->m_nWidth),q);
                            }
                          else
                            {
                              if (p > (pixels+extent+3*image->m_nWidth))
                              {
                                free(pixels);
                                DeletePixelsMemory(image);
                                DeletePixelsMemory(tile_image);

                                strcpy(image->error, "NotEnoughPixelData");

                                return 0;
                              }
                              SetPixelAlpha(tile_image,*p,q);
                              SetPixelRed(tile_image,*(p+1*tile_image->m_nWidth),q);
                              SetPixelGreen(tile_image,*(p+2*tile_image->m_nWidth),q);
                              SetPixelBlue(tile_image,*(p+3*tile_image->m_nWidth),q);
                            }
                      }
                    p++;
                    q+=tile_image->number_channels;
                  }
                  if ((tile_image->storage_class == DirectClass) &&
                      (pixmap.bits_per_pixel != 16))
                    {
                      p+=(pixmap.component_count-1)*(long long) tile_image->m_nWidth;
                      if (p < pixels)
                        break;
                    }
                }

                if (tile_image->storage_class == PseudoClass)
                    (void) SetImageAlpha(tile_image, 255);
                else if (pixmap.bits_per_pixel == 16)
                    (void) SetImageAlpha(tile_image, 255);
                else if (tile_image->alpha_trait == UndefinedPixelTrait)
                    (void) SetImageAlpha(tile_image, 255);

                free(pixels);
                if ((jpeg == 0) && (feof(hFile) == 0))
                  if ((code == 0x9a) || (code == 0x9b) ||
                      ((bytes_per_line & 0x8000) != 0))
                    (void) CompositeImage(image,tile_image,1,(long long) destination.left,(long long)destination.top);
                tile_image=DestroyImage(tile_image);
                break;
            }
            case 0xa1:
            {
                unsigned char
                        *info;

                size_t
                        type;

                /*
                  Comment.
                */
                type = ReadShortValue(hFile);
                length = ReadShortValue(hFile);
                if ((size_t) length > GetSize(hFile))
                {
                    DeletePixelsMemory(image);

                    strcpy(image->error, "InsufficientImageDataInFile");

                    return 0;
                }
                if (length == 0)
                    break;
                info = (unsigned char*) malloc(length * sizeof (*info));
                if (info == (unsigned char*) NULL)
                    break;
                count = Read(hFile, length, info);
                if (count != length)
                {
                    free(info);

                    DeletePixelsMemory(image);

                    strcpy(image->error, "UnableToReadImageData");

                    return 0;
                }
                switch (type)
                {
                case 0xe0:
                {
                    profile = BlobToStringInfo((const void *) NULL, length);
                    if (profile->length != 0)
                        (void) memcpy(profile->datum,info,profile->length);
                    status = SetImageProfileInternal(image, "icc", profile, 0);
                    profile = DestroyStringInfo(profile);
                    if (status == 0)
                    {
                        free(info);

                        DeletePixelsMemory(image);

                        strcpy(image->error, "MemoryAllocationFailed");

                        return 0;
                    }
                    break;
                }
                case 0x1f2:
                {
                    profile=BlobToStringInfo((const void *) NULL,length);
                    if (profile->length != 0)
                        (void) memcpy(profile->datum,info,profile->length);
                    status=SetImageProfileInternal(image,"iptc",profile,0);
                    if (status == 0)
                      {
                        free(info);

                        DeletePixelsMemory(image);

                        strcpy(image->error, "MemoryAllocationFailed");

                        return 0;
                      }
                    profile=DestroyStringInfo(profile);
                    break;
                }
                default:
                    break;
                }
                free(info);
                break;
            }
            default:
                /*
                  Skip to next op code.
                */
                if (codes[code].length == -1)
                  (void) ReadShortValue(hFile);
                else
                  for (i=0; i < (long long) codes[code].length; i++)
                    if (ReadByte(hFile) == EOF)
                      break;
            }
        }
        if (code == 0xc00)
          {
            /*
              Skip header.
            */
            for (i=0; i < 24; i++)
              if (ReadByte(hFile) == EOF)
                break;
            continue;
          }
        if (((code >= 0xb0) && (code <= 0xcf)) ||
            ((code >= 0x8000) && (code <= 0x80ff)))
          continue;
        if ((code == 0xff) || (code == 0xffff))
          continue;
        if (((code >= 0xd0) && (code <= 0xfe)) ||
            ((code >= 0x8100) && (code <= 0xffff)))
          {
            /*
              Skip reserved.
            */
            length=ReadShortValue(hFile);
            if (length > GetSize(hFile))
            {
                DeletePixelsMemory(image);

                strcpy(image->error, "ImproperImageHeader");

                return 0;
            }
            for (i=0; i < (long long) length; i++)
              if (ReadByte(hFile) == EOF)
                break;
            continue;
          }
        if ((code >= 0x100) && (code <= 0x7fff))
          {
            /*
              Skip reserved.
            */
            length=(size_t) ((code >> 7) & 0xff);
            if (length > GetSize(hFile))
            {
                DeletePixelsMemory(image);

                strcpy(image->error, "ImproperImageHeader");

                return 0;
            }
            for (i=0; i < (long long) length; i++)
              if (ReadByte(hFile) == EOF)
                break;
            continue;
          }
    }
    return 1;
}

