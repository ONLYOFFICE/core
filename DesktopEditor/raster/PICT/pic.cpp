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
#include "../pro/Graphics.h"

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
		/* 0x80 */ { "frameRgn", 2, "region" },
		/* 0x81 */ { "paintRgn", 2, "region" },
		/* 0x82 */ { "eraseRgn", 2, "region" },
		/* 0x83 */ { "invertRgn", 2, "region" },
		/* 0x84 */ { "fillRgn", 2, "region" },
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
		return c;
	return tolower((int) ((unsigned char) c));
}

int LocaleCompare(const char *p, const char *q)
{
	if (p == (char *) nullptr)
	{
		if (q == (char *) nullptr)
			return 0;
		return -1;
	}
	if (q == (char *) nullptr)
		return 1;
	// {
	// 	const unsigned char
	// 	*r = (const unsigned char *) p,
	// 	*s = (const unsigned char *) q;
	// 	for ( ; (*r != '\0') && (*s != '\0') && ((*r == *s) ||
	// 		(LocaleToLowercase((int) *r) == LocaleToLowercase((int) *s))); r++, s++);
	// 	return(LocaleToLowercase((int) *r)-LocaleToLowercase((int) *s));
	// }
}

void LocaleLower(char *string)
{
	char *q;
	for (q=string; *q != '\0'; q++)
		*q=(char) LocaleToLowercase((int) *q);
}

int LocaleNCompare(const char *p, const char *q, const size_t length)
{
	if (p == (char *) nullptr)
	{
		if (q == (char *) nullptr)
			return 0;
		return -1;
	}
	if (q == (char *) nullptr)
		return 1;
	if (length == 0)
		return 0;
	// {
	//   const unsigned char
	// 	*s = (const unsigned char *) p,
	// 	*t = (const unsigned char *) q;
	//   size_t
	// 	n = length;
	//   for (n--; (*s != '\0') && (*t != '\0') && (n != 0) && ((*s == *t) ||
	// 	(LocaleToLowercase((int) *s) == LocaleToLowercase((int) *t))); s++, t++, n--);
	//   return(LocaleToLowercase((int) *s)-LocaleToLowercase((int) *t));
	// }
}

char *ConstantString(const char *source)
{
	char *destination = nullptr;
	size_t length = 0;

	if (source != nullptr)
		length += strlen(source);

	if (~length >= 1UL)
		destination=(char *) malloc(length+1UL*sizeof(*destination));

	if (destination == nullptr)
		return nullptr;

	if (source != nullptr)
	  (void) memcpy(destination,source,length*sizeof(*destination));

	destination[length]='\0';
	return destination;
}

int IsStringTrue(const char *value)
{
	if (value == nullptr)
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

size_t CopyMagickString(char *destination, const char *source, const size_t length)
{
	char* q = destination;
	const char* p = source;

	size_t n;
	for (n = length; n > 4; n -= 4)
	{
		if (((*q++) = (*p++)) == '\0')
			return (size_t) (p - source - 1);
		if (((*q++) = (*p++)) == '\0')
			return (size_t) (p - source - 1);
		if (((*q++) = (*p++)) == '\0')
			return (size_t) (p - source - 1);
		if (((*q++) = (*p++)) == '\0')
			return (size_t) (p - source - 1);
	}
	if (length != 0)
	{
		while (--n != 0)
			if (((*q++) = (*p++)) == '\0')
				return (size_t) (p - source - 1);
		*q = '\0';
	}
	return (size_t) (p-source);
}

StringInfo *AcquireStringInfo(const size_t length)
{
	StringInfo *string_info = (StringInfo *) malloc(sizeof(*string_info));
	(void) memset(string_info, 0, sizeof(*string_info));
	string_info->signature = 0xabacadabUL;
	string_info->length = length;

	if (~string_info->length >= (4096-1))
		string_info->datum=(unsigned char *) malloc((string_info->length + 4096) * sizeof(*string_info->datum));
	if (string_info->datum == nullptr)
	{
		free(string_info);
		return nullptr;
	}
	(void) memset(string_info->datum, 0, (length + 4096) * sizeof(*string_info->datum));
	return string_info;
}

char *CloneString(char **destination, const char *source)
{
	size_t length;
	if (source == nullptr)
	{
		if (*destination != nullptr)
			free(*destination);
		return nullptr;
	}
	if (*destination == nullptr)
	{
		*destination = (char*) malloc((strlen(source) + 4096)/* * sizeof (*destination)*/);
		memcpy(*destination, source, strlen(source)/**sizeof(**destination)*/);
		(*destination)[strlen(source)]='\0';

		return *destination;
	}

	length=strlen(source);
	if (~length < 4096)
		return nullptr;

	free(*destination);
	*destination = (char*) malloc((length + 4096)/**sizeof (**destination)*/);

	if (*destination == nullptr)
		return nullptr;

	if (length != 0)
		memcpy(*destination, source, length/**sizeof(**destination)*/);

	(*destination)[length]='\0';
	return(*destination);
}

StringInfo *CloneStringInfo(const StringInfo *string_info)
{
	StringInfo *clone_info = AcquireStringInfo(string_info->length);

	CloneString(&clone_info->path,string_info->path);
	CloneString(&clone_info->name,string_info->name);

	if (string_info->length != 0)
		memcpy(clone_info->datum, string_info->datum, string_info->length + 1);

	return clone_info;
}

void DestroyStringInfo(StringInfo *string_info)
{
	if (string_info->datum != nullptr)
		free(string_info->datum);

	if (string_info->name != nullptr)
		free(string_info->name);

	if (string_info->path != nullptr)
		free(string_info->path);

	string_info->signature = (~0xabacadabUL);
	free(string_info);
}

StringInfo *BlobToStringInfo(const void *blob, const size_t length)
{
	if (~length < 4096)
		return nullptr;

	StringInfo *string_info = (StringInfo *) malloc(sizeof(*string_info));
	(void) memset(string_info, 0, sizeof(*string_info));
	string_info->signature = 0xabacadabUL;

	if (string_info == nullptr)
	  return nullptr;

	string_info->length=length;
	string_info->datum=(unsigned char *) malloc(length + 4096 * sizeof(*string_info->datum));
	if (string_info->datum == (unsigned char *) nullptr)
	{
		DestroyStringInfo(string_info);
		return nullptr;
	}

	if (blob != nullptr)
		(void) memcpy(string_info->datum,blob,length);
	else
		(void) memset(string_info->datum, 0, length * sizeof(*string_info->datum));

	(void) memset(string_info->datum+length, 0, 4096 * sizeof(*string_info->datum));
	return string_info;
}

void DeletePixelsMemory(ImagePICT* image)
{
	if (image->ppixels != nullptr)
	{
		free(image->ppixels);
		image->ppixels = nullptr;
		image->m_nPixelsSize = 0;
	}
}

PixelChannelMap *AcquirePixelChannelMap()
{
	PixelChannelMap *channel_map = (PixelChannelMap *) malloc(65 * sizeof(*channel_map));

	if (channel_map == nullptr)
	  return nullptr;

	(void) memset(channel_map,0,65*sizeof(*channel_map));
	for (size_t i=0; i <= 64; i++)
		channel_map[i].channel=(PixelChannel) i;

	channel_map[RedPixelChannel].offset = 0;
	channel_map[GreenPixelChannel].offset = 1;
	channel_map[BluePixelChannel].offset = 2;
	channel_map[AlphaPixelChannel].offset = 3;
	channel_map[IndexPixelChannel].offset = 5;

	return channel_map;
}

void GetPixelInfo(ImagePICT *image, PixelInfo *pixel)
{
	(void) memset(pixel, 0, sizeof(*pixel));
	pixel->storage_class=DirectClass;
	pixel->colorspace=sRGBColorspace;
	pixel->depth=8;
	pixel->alpha_trait=UndefinedPixelTrait;
	pixel->alpha=255.0;

	if (image == nullptr)
		return;

	pixel->storage_class=image->storage_class;
	pixel->colorspace=image->colorspace;
	pixel->alpha_trait=image->alpha_trait;
	pixel->depth=image->m_ndepth;
	pixel->fuzz=image->fuzz;
}

void GetPixelInfoRGBA(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha, PixelInfo *pixel)
{
	GetPixelInfo((ImagePICT *) nullptr, pixel);
	pixel->red=red;
	pixel->green=green;
	pixel->blue=blue;
	pixel->alpha=alpha;
}

inline unsigned char GetPixelWriteMask(const ImagePICT *image, const unsigned char *pixel)
{
	if (image->channel_map[WriteMaskPixelChannel].traits == UndefinedPixelTrait)
		return 255;
	return pixel[image->channel_map[WriteMaskPixelChannel].offset];
}

inline bool IsValidOffset(const long long x, const size_t a)
{
	if (a == 0)
		return false;
	if ((x >= (LLONG_MAX / 64 / (long long) a)) || (x <= ((-LLONG_MAX - 1) / 64 / (long long) a)))
		return false;
	return true;
}

bool ReadPixels(ImagePICT* image, const long long& x, const long long& y, const size_t width, const size_t height, unsigned char* pixels)
{
	if (IsValidOffset(y, image->m_nWidth) == 0)
		return false;

	long long offset = y * (long long) image->m_nWidth;
	if ((offset / (long long) image->m_nWidth) != y)
		return false;

	offset += x;
	size_t number_channels = image->number_channels;
	size_t length = (size_t) number_channels * width * sizeof(unsigned char);

	if ((length / number_channels / sizeof(unsigned char)) != width)
		return false;

	size_t rows = height;
	size_t extent = length * rows;

	if ((extent == 0) || ((extent/length) != rows))
		return false;

	unsigned char* q = pixels;
	if (image->m_nWidth == width)
	{
		length = extent;
		rows = 1UL;
	}

	size_t i;
	unsigned char* p = image->ppixels + image->number_channels * offset;
	for (i = 0; i < rows; i++)
	{
		(void) memcpy(q, p, length);
		p += image->number_channels * image->m_nWidth;
		q += image->number_channels * width;
	}

	if (i < rows)
		return false;

	return true;
}

unsigned char* GetPixels(ImagePICT* image, const long long& x, const long long& y, const size_t width, const size_t height)
{
	unsigned char *pixels = image->ppixels + image->number_channels * (y * image->m_nWidth + x);

	if (pixels == nullptr)
		return nullptr;

    if (ReadPixels(image, x, y, width, height, pixels) == 0)
		return nullptr;

    return pixels;
}

inline float PerceptibleReciprocal(const float x)
{
	float sign = x < 0.0f ? -1.0f : 1.0f;
	return((sign*x) >= 1.0e-12 ? 1.0f / x : sign * ((float) (1.0f / 1.0e-12)));
}

inline unsigned char GetPixelChannel(const ImagePICT *image, const PixelChannel& channel, const unsigned char *pixel)
{
	if ((size_t) channel >= 64)
		return 0;
	if (image->channel_map[channel].traits == UndefinedPixelTrait)
		return 0;
	return pixel[image->channel_map[channel].offset];
}

inline unsigned char GetPixelAlpha(const ImagePICT *image, const unsigned char *pixel)
{
	if (image->channel_map[AlphaPixelChannel].traits == UndefinedPixelTrait)
		return 0;
	return pixel[image->channel_map[AlphaPixelChannel].offset];
}

inline void SetPixelChannel(const ImagePICT *image, const PixelChannel& channel, const unsigned char Quantum, unsigned char *pixel)
{
	if ((size_t) channel >= 64)
		return;
	if (image->channel_map[channel].traits != UndefinedPixelTrait)
		pixel[image->channel_map[channel].offset] = Quantum;
}

inline PixelChannel GetPixelChannelChannel(const ImagePICT *image, const long long& offset)
{
	if ((offset < 0) || (offset >= 64))
		return UndefinedPixelChannel;
	return image->channel_map[offset].channel;
}

inline PixelTrait GetPixelChannelTraits(const ImagePICT *image, const PixelChannel& channel)
{
	if ((size_t) channel >= 64)
		return UndefinedPixelTrait;
	return image->channel_map[channel].traits;
}

inline void SetPixelIndex(const ImagePICT *image, const unsigned char index, unsigned char *pixel)
{
	if (image->channel_map[IndexPixelChannel].traits != UndefinedPixelTrait)
		pixel[image->channel_map[IndexPixelChannel].offset] = index;
}

inline void SetPixelAlpha(const ImagePICT *image, const unsigned char alpha, unsigned char *pixel)
{
	if (image->channel_map[AlphaPixelChannel].traits != UndefinedPixelTrait)
		pixel[image->channel_map[AlphaPixelChannel].offset] = alpha;
}

inline void SetPixelRed(const ImagePICT *image, const unsigned char red, unsigned char *pixel)
{
	pixel[image->channel_map[RedPixelChannel].offset] = red;
}

inline void SetPixelGreen(const ImagePICT *image, const unsigned char green, unsigned char *pixel)
{
	pixel[image->channel_map[GreenPixelChannel].offset] = green;
}

inline void SetPixelBlue(const ImagePICT *image, const unsigned char blue, unsigned char *pixel)
{
	pixel[image->channel_map[BluePixelChannel].offset] = blue;
}

bool AquirePixelsMemory(ImagePICT* image)
{
	if ((image->m_nHeight == 0) || (image->m_nWidth == 0))
		return false;

	image->m_nPixelsSize = image->m_nHeight * image->m_nWidth * image->number_channels;

	if (image->ppixels == nullptr)
		image->ppixels = (unsigned char*) malloc(image->m_nPixelsSize);
	else
		image->ppixels = (unsigned char*) realloc(image->ppixels, image->m_nPixelsSize);

	return true;
}

bool SetImageAlpha(ImagePICT* image, const unsigned char Alpha)
{
	bool status = true;

	if (AquirePixelsMemory(image) == 0)
		return 0;

	image->alpha_trait = BlendPixelTrait;
	image->channel_map[AlphaPixelChannel].traits = UpdatePixelTrait;
	for (int y = 0; y < image->m_nHeight; y++)
	{
		if (!status)
			continue;

		unsigned char* q = image->ppixels + image->number_channels * (y * image->m_nWidth);

		if (q == nullptr)
		{
			status = false;
			continue;
        }
		for (int x = 0; x < image->m_nWidth; x++)
		{
			if (GetPixelWriteMask(image, q) > (255 / 2))
				SetPixelAlpha(image, Alpha, q);
			q += image->number_channels;
		}
	}

	return status;
}

bool AquireImageColormap(ImagePICT* image, const size_t colors)
{
	if (image == nullptr)
		return false;

	if (colors > 256UL)
	{
		image->colors = 0;
		image->storage_class = DirectClass;
		return false;
	}

	if (colors > 1)
		image->colors = colors;
	else
		image->colors = 1;
	if (image->colormap == nullptr)
		image->colormap = (PixelInfo*) malloc((image->colors + 1) * sizeof(*image->colormap));
	else
		image->colormap = (PixelInfo*) realloc(image->colormap, (image->colors + 1) * sizeof(*image->colormap));

	if (image->colormap == nullptr)
	{
		image->colors = 0;
		image->storage_class = DirectClass;
		return false;
	}

	for (size_t i = 0; i < image->colors; i++)
	{
		double pixel;
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
	return true;
}

inline bool IsRGBColorspace(const ColorspaceType& colorspace)
{
	if ((colorspace == RGBColorspace) || (colorspace == scRGBColorspace) || (colorspace == LinearGRAYColorspace))
		return true;
	return false;
}

inline bool IsGrayColorspace(const ColorspaceType& colorspace)
{
	if ((colorspace == LinearGRAYColorspace) || (colorspace == GRAYColorspace))
		return 1;
	return 0;
}

int SetImageColorspace(ImagePICT *image, const ColorspaceType& colorspace)
{
	if (image->colorspace == colorspace)
		return 1;

	image->colorspace = colorspace;
	image->gamma = 1.000 / 2.200;
	(void) memset(&image->chromaticity, 0, sizeof(image->chromaticity));
	ImageType type=image->type;
	if (IsGrayColorspace(colorspace) != 0)
	{
		if (colorspace == LinearGRAYColorspace)
			image->gamma = 1.000;

		type=GrayscaleType;
	}
	else if ((IsRGBColorspace(colorspace) != 0) || (colorspace == XYZColorspace) || (colorspace == xyYColorspace))
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

	if (image->ppixels == nullptr)
		return 0;

	image->type = type;
	return 1;
}

long long CastDoubleToLong(const double x)
{
	if (floor(x) > ((double) LLONG_MAX - 1))
	{
		return (long long) LLONG_MAX;
	}

	if (ceil(x) < ((double) LLONG_MIN + 1))
	{
		return (long long) LLONG_MIN;
	}

	return (long long) x;
}

inline unsigned char ClampPixel(const double& pixel)
{
	if (pixel < 0.0)
		return ((unsigned char) 0);
	if (pixel >= 255.0)
		return ((unsigned char) 255);
	return (unsigned char) (pixel);
}

inline unsigned char ClampToQuantum(const double& pixel)
{
	if ((isnan(pixel) != 0) || (pixel <= 0.0))
		return 0;
	if (pixel >= 255.0)
		return 255;
	return (unsigned char) (pixel);
}

int Clamp(const double& x, const double& min, const double& max) {
	if (x < min)
		return min;
	else if (x > max)
		return max;
	else
		return x;
}

inline bool CopyPixel(const ImagePICT *image, const unsigned char *source, unsigned char *destination)
{
	if (source == (const unsigned char *) nullptr)
	{
		destination[RedPixelChannel]=(unsigned char) (Clamp(image->background_color.red, 0.0f, 255.0f) + 0.5f);
		destination[GreenPixelChannel]=(unsigned char) (Clamp(image->background_color.green, 0.0f, 255.0f) + 0.5f);
		destination[BluePixelChannel]=(unsigned char) (Clamp(image->background_color.blue, 0.0f, 255.0f) + 0.5f);
		destination[BlackPixelChannel]=(unsigned char) (Clamp(image->background_color.black, 0.0f, 255.0f) + 0.5f);
		destination[AlphaPixelChannel]=(unsigned char) (Clamp(image->background_color.alpha, 0.0f, 255.0f) + 0.5f);
		return false;
	}
	for (size_t i = 0; i < 4; i++)
	{
		PixelChannel channel = GetPixelChannelChannel(image,i);
		destination[channel] = source[i];
	}
	return true;
}

inline bool GetOneVirtualPixel(ImagePICT *image,const long long x,const long long y,unsigned char *pixel)
{
	(void) memset(pixel, 0, 64 * sizeof(*pixel));
	const unsigned char* p = GetPixels(image, x, y, 1UL, 1UL);
	return CopyPixel(image, p, pixel);
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
	image->profiles = nullptr;
	image->artifacts = nullptr;
	image->fuzz = 0.0;
	image->resolutionX = 0.0;
	image->resolutionY = 0.0;
	image->type = UndefinedType;
	GetPixelInfoRGBA(BackgroundColorRGBA, &image->background_color);
	image->alpha_trait = UndefinedPixelTrait;
	image->ppixels = nullptr;
	image->m_nPixelsSize = 0;
	image->colormap = nullptr;
	image->channel_map = AcquirePixelChannelMap();
	image->mask_trait = UndefinedPixelTrait;
	image->taint = 0;
	image->number_channels = 4;
}

size_t GetSize(FILE* file)
{
	struct stat st;

	long long file_discription = fileno(file);
	if (fstat(file_discription, &st) == 0)
		return st.st_size;

	return 0;
}

int Read(FILE* file, const int length, void* data)
{
    if (!file) return 0;
	if (data == nullptr) return 0;
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
	unsigned char buffer[2];
	long long count;
	*buffer='\0';

	const unsigned char* p = (const unsigned char*) ReadBlobStream(file, 2, buffer, &count);
	if (count != 2)
		return EOF;

	unsigned short value = (unsigned short) ((*p++) << 8);
	value |= (unsigned short) (*p++);
	return (unsigned short) (value & 0xffff);
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
	unsigned char buffer[4];
	*buffer='\0';
	int count = Read(file, 4, buffer);

	if (count != 4)
		return EOF;

	const unsigned char* p = (const unsigned char*) buffer;

	unsigned int value = (unsigned int) (*p++) << 24;
	value |= (unsigned int) (*p++) << 16;
	value |= (unsigned int) (*p++) << 8;
	value |= (unsigned int) (*p++);

	return value;
}

bool ReadRectangle(FILE* file, PICTrectangle *frame)
{
	frame->top = (short) ReadShortValue(file);
	frame->left = (short) ReadShortValue(file);
	frame->bottom = (short) ReadShortValue(file);
	frame->right = (short) ReadShortValue(file);

	if (feof(file) != 0)
		return false;

	if (frame->bottom < frame->top)
		return false;

	if (frame->right < frame->left)
		return false;

	return true;
}

bool ReadPixmap(FILE* file,PICTPixmap *pixmap)
{
	pixmap->version		= (short) ReadShortValue(file);
	pixmap->pack_type	= (short) ReadShortValue(file);
	pixmap->pack_size	= ReadLongValue(file);
	pixmap->horizontal_resolution	= 1UL * ReadShortValue(file);
	(void) ReadShortValue(file);
	pixmap->vertical_resolution		= 1UL * ReadShortValue(file);
	(void) ReadShortValue(file);
	pixmap->pixel_type		= (short) ReadShortValue(file);
	pixmap->bits_per_pixel	= (short) ReadShortValue(file);
	pixmap->component_count	= (short) ReadShortValue(file);
	pixmap->component_size	= (short) ReadShortValue(file);
	pixmap->plane_bytes		= ReadLongValue(file);
	pixmap->table			= ReadLongValue(file);
	pixmap->reserved		= ReadLongValue(file);

	if ((feof(file) != 0) || (pixmap->bits_per_pixel <= 0) || (pixmap->bits_per_pixel > 32) || (pixmap->component_count <= 0) ||
      (pixmap->component_count > 4) || (pixmap->component_size <= 0))
		return false;

	return true;
}

void DestroySplayTree(SplayTreeInfo *splay_tree)
{
	NodeInfo *node,	*active, *pend;

	if (splay_tree->root != nullptr)
	{
		for (pend=splay_tree->root; pend != nullptr; )
		{
			active=pend;
			for (pend = nullptr; active != nullptr; )
			{
				if (active->left != nullptr)
				{
					free(active->left->key);
					free(active->left->value);
					pend=active->left;
				}
				if (active->right != nullptr)
				{
					free(active->right->key);
					free(active->right->value);
					pend = active->right;
				}
				node = active;
				active = (NodeInfo *) node->key;
				free(node);
			}
		}
	}

	free(splay_tree);
}

SplayTreeInfo *NewSplayTree(int (*compare)(const void *,const void *))
{
	SplayTreeInfo *splay_tree = (SplayTreeInfo *) malloc(sizeof(*splay_tree));

	(void) memset(splay_tree,0,sizeof(*splay_tree));
	splay_tree->root		= nullptr;
	splay_tree->compare		= compare;
	splay_tree->balance		= 0;
	splay_tree->key			= nullptr;
	splay_tree->next		= nullptr;
	splay_tree->nodes		= 0;
	splay_tree->signature	= 0xabacadabUL;

	return splay_tree;
}

inline void *GetFirstSplayTreeNode(SplayTreeInfo *splay_tree)
{
	NodeInfo *node = splay_tree->root;
	if (splay_tree->root == nullptr)
		return nullptr;
	while (node->left != nullptr)
		node = node->left;
	return node->key;
}

NodeInfo *Splay(SplayTreeInfo *splay_tree,const size_t depth,const void *key,NodeInfo **node,NodeInfo **parent,NodeInfo **grandparent)
{
	int compare;
	NodeInfo **next;
	NodeInfo *n, *p;
	n = *node;
	if (n == nullptr)
	{
		if (parent != nullptr)
			return *parent;
		else
			return nullptr;
	}
	if (splay_tree->compare != (int (*)(const void *,const void *)) nullptr)
		compare=splay_tree->compare(n->key,key);
	else
		compare=(n->key > key) ? 1 : ((n->key < key) ? -1 : 0);

	next = nullptr;

	if (compare > 0)
		next = (&n->left);
	else if (compare < 0)
		next = (&n->right);

	if (next != nullptr)
	{
		if (depth >= 1024)
		{
			splay_tree->balance = 1;
			return n;
		}

		n = Splay(splay_tree,depth+1,key,next,node,parent);
		if ((n != *node) || (splay_tree->balance != 0))
			return n;
	}
	if (parent == nullptr)
		return n;
	if (grandparent == nullptr)
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
		return n;
	}
	if ((n == (*parent)->left) && (*parent == (*grandparent)->left))
	{
		p					 = (*parent);
		(*grandparent)->left = p->right;
		p->right			 = (*grandparent);
		p->left				 = n->right;
		n->right			 = p;
		*grandparent		 = n;

		return n;
	}
	if ((n == (*parent)->right) && (*parent == (*grandparent)->right))
	{
		p						= (*parent);
		(*grandparent)->right	= p->left;
		p->left					= (*grandparent);
		p->right				= n->left;
		n->left					= p;
		*grandparent			= n;

		return n;
	}
	if (n == (*parent)->left)
	{
		(*parent)->left			= n->right;
		n->right				= (*parent);
		(*grandparent)->right	= n->left;
		n->left					= (*grandparent);
		*grandparent			= n;

		return n;
	}
	(*parent)->right		= n->left;
	n->left					= (*parent);
	(*grandparent)->left	= n->right;
	n->right				= (*grandparent);
	*grandparent			= n;

	return n;
}

bool IterateOverSplayTree(SplayTreeInfo *splay_tree, int (*method)(NodeInfo *,const void *), const void *value)
{
	typedef enum
	{
		LeftTransition,
		RightTransition,
		DownTransition,
		UpTransition
	} TransitionType;

	bool status = false;

	if (splay_tree->root == nullptr)
		return false;

	NodeInfo** nodes = (NodeInfo **) malloc((size_t) splay_tree->nodes * sizeof(*nodes));
	unsigned char* transitions = (unsigned char *) malloc((size_t) splay_tree->nodes * sizeof(*transitions));
	if ((nodes == nullptr) || (transitions == nullptr))
		return false;

	int final_transition = 0;
	nodes[0] = splay_tree->root;
	transitions[0] = (unsigned char) LeftTransition;
	for (size_t i = 0; final_transition == 0; )
	{
		NodeInfo* node = nodes[i];
		TransitionType transition = (TransitionType) transitions[i];
		switch (transition)
		{
		case LeftTransition:
		{
			transitions[i] = (unsigned char) DownTransition;
			if (node->left == nullptr)
				break;
			i++;
			nodes[i] = node->left;
			transitions[i] = (unsigned char) LeftTransition;
			break;
		}
		case RightTransition:
		{
			transitions[i] = (unsigned char) UpTransition;
			if (node->right == nullptr)
				break;
			i++;
			nodes[i] = node->right;
			transitions[i] = (unsigned char) LeftTransition;
			break;
		}
		case DownTransition:
		default:
		{
			transitions[i] = (unsigned char) RightTransition;
			status = (*method)(node,value);
			if (status != 0)
				final_transition = 1;
			break;
		}
		case UpTransition:
		{
			if (i == 0)
			{
				final_transition = 1;
				break;
			}
			i--;
			break;
		}
		}
	}

	free(nodes);
	free(transitions);
	return status;
}

NodeInfo *LinkSplayTreeNodes(NodeInfo **nodes,const size_t low,const size_t high)
{
	size_t bisect = low + (high - low) / 2;
	NodeInfo* node = nodes[bisect];

	if ((low + 1) > bisect)
		node->left = nullptr;
	else
		node->left=LinkSplayTreeNodes(nodes, low, bisect - 1);

	if ((bisect + 1) > high)
		node->right = nullptr;
	else
		node->right = LinkSplayTreeNodes(nodes, bisect + 1, high);

	return node;
}

inline int SplayTreeToNodeArray(NodeInfo *node, const void *nodes)
{
	const NodeInfo*** p=(const NodeInfo ***) nodes;
	*(*p)=node;
	(*p)++;
	return 0;
}

void BalanceSplayTree(SplayTreeInfo *splay_tree)
{
	if (splay_tree->nodes <= 2)
	{
		splay_tree->balance=0;
		return;
	}

	NodeInfo** nodes = (NodeInfo **) malloc((size_t) splay_tree->nodes * sizeof(*nodes));

	if (nodes == nullptr)
		return;

	NodeInfo** node	= nodes;
	IterateOverSplayTree(splay_tree, SplayTreeToNodeArray, (const void *)&node);
	splay_tree->root = LinkSplayTreeNodes(nodes, 0, splay_tree->nodes - 1);
	splay_tree->balance = 0;
	free(nodes);
}

void SplaySplayTree(SplayTreeInfo *splay_tree,const void *key)
{
	if (splay_tree->root == nullptr)
		return;
	if (splay_tree->key != nullptr)
	{
		int compare;

		if (splay_tree->compare != (int (*)(const void *,const void *)) nullptr)
			compare=splay_tree->compare(splay_tree->root->key,key);
		else
			compare=(splay_tree->key > key) ? 1 : ((splay_tree->key < key) ? -1 : 0);
		if (compare == 0)
			return;
	}
	Splay(splay_tree, 0UL, key, &splay_tree->root, nullptr, nullptr);
	if (splay_tree->balance != 0)
	{
		BalanceSplayTree(splay_tree);
		Splay(splay_tree, 0UL, key, &splay_tree->root, nullptr, nullptr);
		if (splay_tree->balance != 0)
			return;
	}
	splay_tree->key = (void *) key;
}

bool AddValueToSplayTree(SplayTreeInfo *splay_tree,const void *key,const void *value)
{
	int compare = 0;

	SplaySplayTree(splay_tree,key);

	if (splay_tree->root != (NodeInfo *) nullptr)
	{
		if (splay_tree->compare != (int (*)(const void *,const void *)) nullptr)
			compare=splay_tree->compare(splay_tree->root->key,key);
		else
			compare=(splay_tree->root->key > key) ? 1 : ((splay_tree->root->key < key) ? -1 : 0);
		if (compare == 0)
		{
			splay_tree->root->key=(void *) key;
			splay_tree->root->value=(void *) value;
			return true;
		}
	}

	NodeInfo* node = (NodeInfo *) malloc(sizeof(*node));

	if (node == nullptr)
		return false;

	node->key   = (void *) key;
	node->value = (void *) value;
	if (splay_tree->root == nullptr)
	{
		node->left  = nullptr;
		node->right = nullptr;
	}
	else if (compare < 0)
	{
		node->left			= splay_tree->root;
		node->right			= node->left->right;
		node->left->right	= nullptr;
	}
	else
	{
		node->right			= splay_tree->root;
		node->left			= node->right->left;
		node->right->left	= nullptr;
	}
	splay_tree->root = node;
	splay_tree->key  = nullptr;
	splay_tree->nodes++;
	return true;
}

SplayTreeInfo *CloneSplayTree(SplayTreeInfo *splay_tree, void *(*clone_key)(void *), void *(*clone_value)(void *))
{
	NodeInfo *node;

	SplayTreeInfo* clone_tree=NewSplayTree(splay_tree->compare);
	if (splay_tree->root == nullptr)
	{
		return(clone_tree);
	}

	NodeInfo* next = (NodeInfo *) GetFirstSplayTreeNode(splay_tree);
	while (next != (NodeInfo *) nullptr)
	{
		SplaySplayTree(splay_tree,next);
		AddValueToSplayTree(clone_tree, clone_key(splay_tree->root->key), clone_value(splay_tree->root->value));
		next = nullptr;
		node = splay_tree->root->right;
		if (node != nullptr)
		{
			while (node->left != nullptr)
				node = node->left;
			next = (NodeInfo *) node->key;
      }
	}
	return clone_tree;
}

int CompareSplayTreeString(const void *target, const void *source)
{
	const char* p = (const char *) target;
	const char* q = (const char *) source;
	return LocaleCompare(p,q);
}

const void *GetValueFromSplayTree(SplayTreeInfo *splay_tree, const void *key)
{
	int compare;

	if (splay_tree->root == nullptr)
		return nullptr;

	SplaySplayTree(splay_tree,key);
	if (splay_tree->compare != (int (*)(const void *,const void *)) nullptr)
		compare = splay_tree->compare(splay_tree->root->key,key);
	else
		compare = (splay_tree->root->key > key) ? 1 : ((splay_tree->root->key < key) ? -1 : 0);
	if (compare != 0)
		return nullptr;

	return splay_tree->root->value;
}

const void *GetRootValueFromSplayTree(SplayTreeInfo *splay_tree)
{
	const void* value = nullptr;
	if (splay_tree->root != nullptr)
		value = splay_tree->root->value;

	return value;
}

const unsigned char *ReadResourceShort(const unsigned char *p, unsigned short *Quantum)
{
	*Quantum = (unsigned short) (*p++) << 8;
	*Quantum |= (unsigned short) (*p++);
	return p;
}

const unsigned char *ReadResourceByte(const unsigned char *p, unsigned char *Quantum)
{
	*Quantum = (*p++);
	return p;
}

const unsigned char *ReadResourceLong(const unsigned char *p, unsigned int *Quantum)
{
	*Quantum = (unsigned int) (*p++) << 24;
	*Quantum |= (unsigned int) (*p++) << 16;
	*Quantum |= (unsigned int) (*p++) << 8;
	*Quantum |= (unsigned int) (*p++);
	return p;
}

void WriteResourceLong(unsigned char *p, const unsigned int Quantum)
{
	unsigned char buffer[4];

	buffer[0]=(unsigned char) (Quantum >> 24);
	buffer[1]=(unsigned char) (Quantum >> 16);
	buffer[2]=(unsigned char) (Quantum >> 8);
	buffer[3]=(unsigned char) Quantum;

	memcpy(p,buffer,4);
}

void WriteTo8BimProfile(ImagePICT *image,const char *name, const StringInfo *profile)
{
	unsigned short profile_id;

	if (LocaleCompare(name,"icc") == 0)
		profile_id=0x040f;
	else if (LocaleCompare(name,"iptc") == 0)
		profile_id=0x0404;
	else if (LocaleCompare(name,"xmp") == 0)
		profile_id=0x0424;
	else
		return;

	StringInfo* profile_8bim = (StringInfo *) GetValueFromSplayTree((SplayTreeInfo *) image->profiles,"8bim");

	if (profile_8bim == nullptr)
		return;

	const unsigned char* datum = profile_8bim->datum;
	size_t length = profile_8bim->length;

	for (const unsigned char* p = datum; p < (datum + length - 16); )
	{
		unsigned short id;
		unsigned char length_byte;
		const unsigned char* q = p;
		if (LocaleNCompare((char *) p,"8BIM",4) != 0)
			break;

		p += 4;
		p = ReadResourceShort(p, &id);
		p = ReadResourceByte(p, &length_byte);
		p += length_byte;

		if (((length_byte + 1) & 0x01) != 0)
			p++;
		if (p > (datum+length - 4))
			break;

		unsigned int value;
		p = ReadResourceLong(p, &value);
		long long count = (long long) value;

		if ((count & 0x01) != 0)
			count++;
		if ((count < 0) || (p > (datum + length - count)) || (count > (long long) length))
			break;
		if (id != profile_id)
			p += count;
		else
		{
			size_t offset;

			StringInfo *extract_profile;

			long long extract_extent = 0;
			size_t extent = (size_t) ((datum + length) - (p + count));
			if (profile == nullptr)
			{
				offset = (size_t) (q - datum);
				extract_profile = AcquireStringInfo(offset + extent);
				memcpy(extract_profile->datum, datum, offset);
			}
			else
			{
				offset = (size_t) (p - datum);
				extract_extent = (long long) profile->length;
				if ((extract_extent & 0x01) != 0)
					extract_extent++;
				extract_profile = AcquireStringInfo(offset + (size_t) extract_extent + extent);
				memcpy(extract_profile->datum, datum, offset - 4);
				WriteResourceLong(extract_profile->datum + offset - 4, (unsigned int)profile->length);
				memcpy(extract_profile->datum+offset, profile->datum,profile->length);
			}

			memcpy(extract_profile->datum + offset + extract_extent, p + count,extent);
			AddValueToSplayTree((SplayTreeInfo *) image->profiles, ConstantString("8bim"),CloneStringInfo(extract_profile));
			DestroyStringInfo(extract_profile);
			break;
		}
	}
}

bool SetImageProfileInternal(ImagePICT *image, const char *name, const StringInfo *profile, const int recursive)
{
	char key[4096];

	StringInfo* clone_profile = CloneStringInfo(profile);
	if (image->profiles == (SplayTreeInfo *) nullptr)
		image->profiles = NewSplayTree(CompareSplayTreeString);
	CopyMagickString(key, name, 4096);
	LocaleLower(key);
	bool status=AddValueToSplayTree((SplayTreeInfo *) image->profiles,ConstantString(key), clone_profile);
	if (status)
		WriteTo8BimProfile(image, name, clone_profile);

	return status;
}

const char *GetImageArtifact(const ImagePICT *image,const char *artifact)
{
	const char* p = nullptr;
	if (image->artifacts != nullptr)
	{
		if (artifact == nullptr)
			return (const char *) GetRootValueFromSplayTree((SplayTreeInfo *) image->artifacts);
		p=(const char *) GetValueFromSplayTree((SplayTreeInfo *) image->artifacts, artifact);
		if (p != nullptr)
			return p;
	}

	return p;
}

void DestroyImage(ImagePICT *image)
{
	if (image->ppixels != nullptr)
		free(image->ppixels);
	if (image->channel_map != nullptr)
		free(image->channel_map);
	delete image;
}

ImagePICT* CloneImage(const ImagePICT* image, const size_t colums, const size_t rows)
{
	if (image == nullptr)
		return nullptr;

	ImagePICT* clone_image = new ImagePICT;
	AquireImage(clone_image);

	clone_image->storage_class				= image->storage_class;
	clone_image->fuzz						= image->fuzz;
	clone_image->colorspace					= image->colorspace;
	clone_image->chromaticity.blue_primary	= image->chromaticity.blue_primary;
	clone_image->chromaticity.green_primary	= image->chromaticity.green_primary;
	clone_image->chromaticity.red_primary	= image->chromaticity.red_primary;
	clone_image->chromaticity.white_point	= image->chromaticity.white_point;
	clone_image->gamma						= image->gamma;
	clone_image->m_nHeight					= image->m_nHeight;

	if (rows != 0)
		clone_image->m_nHeight = rows;
	clone_image->m_nWidth = image->m_nWidth;
	if (colums != 0)
		clone_image->m_nWidth = colums;

	clone_image->number_channels		= image->number_channels;
	clone_image->m_nPixelsSize			= image->m_nPixelsSize;
	clone_image->ppixels				= nullptr;
	clone_image->resolutionX			= image->resolutionX;
	clone_image->resolutionY			= image->resolutionY;
	clone_image->alpha_trait			= image->alpha_trait;
	clone_image->background_color.blue	= image->background_color.blue;
	clone_image->background_color.green	= image->background_color.green;
	clone_image->background_color.red	= image->background_color.red;
	clone_image->colors					= image->colors;

	if (image->colormap != nullptr)
    {
        clone_image->colormap = (PixelInfo*) malloc((clone_image->colors + 1) * sizeof(*clone_image->colormap));
        memcpy(clone_image->colormap, image->colormap, clone_image->colors * sizeof(*clone_image->colormap));
    }
	if (image->channel_map != nullptr)
    {
        clone_image->channel_map = (PixelChannelMap*) malloc(65 * sizeof (image->channel_map));
        memcpy(clone_image->channel_map, image->channel_map, 65 * sizeof (image->channel_map));
    }

	if (image->profiles != nullptr)
        clone_image->profiles = CloneSplayTree(image->profiles, (void *(*)(void *)) ConstantString,(void *(*)(void *)) CloneStringInfo);
	if (image->artifacts != nullptr)
        clone_image->artifacts = CloneSplayTree(image->artifacts, (void *(*)(void *)) ConstantString,(void *(*)(void *)) CloneStringInfo);

    clone_image->mask_trait = image->mask_trait;
    clone_image->taint = image->taint;

    return clone_image;
}

inline unsigned char GetPixelReadMask(const ImagePICT *image, const unsigned char *pixel)
{
	if (image->channel_map[ReadMaskPixelChannel].traits == UndefinedPixelTrait)
		return 255;
	return pixel[image->channel_map[ReadMaskPixelChannel].offset];
}

bool CompositeImage(ImagePICT *image, const ImagePICT *composite, const int clip_to_self, const long long& x_offset, const long long& y_offset)
{
	GeometryInfo geometry_info;

	bool status = false;

	image->storage_class = DirectClass;

	if (image->ppixels == (unsigned char*) nullptr)
		return status;

	ImagePICT* source_image = CloneImage(composite, 0, 0);
	if (!AquirePixelsMemory(source_image))
	{
		free(source_image);
		return status;
	}

	memcpy(source_image->ppixels, composite->ppixels, source_image->m_nPixelsSize);
	if (source_image == nullptr)
		return status;

	SetImageColorspace(source_image,image->colorspace);
	ImagePICT* canvas_image = nullptr;
	int clamp = 1;
	const char* value = GetImageArtifact(image, "compose:clamp");
	if (value != nullptr)
		clamp=IsStringTrue(value);

	value = GetImageArtifact(image, "compose:sync");
	if (value != nullptr)
		memset(&geometry_info, 0, sizeof(geometry_info));

	image->channel_map[RedPixelChannel].traits		= UpdatePixelTrait;
	image->channel_map[GreenPixelChannel].traits	= UpdatePixelTrait;
	image->channel_map[BluePixelChannel].traits		= UpdatePixelTrait;
	image->channel_map[AlphaPixelChannel].traits	= UpdatePixelTrait;

	source_image->channel_map[RedPixelChannel].traits	= CopyPixelTrait;
	source_image->channel_map[GreenPixelChannel].traits	= CopyPixelTrait;
	source_image->channel_map[BluePixelChannel].traits	= CopyPixelTrait;
	source_image->channel_map[AlphaPixelChannel].traits	= CopyPixelTrait;

	if (!((x_offset < 0) || (y_offset < 0)) && !((x_offset+(long long) source_image->m_nWidth) > (long long) image->m_nWidth) && !((y_offset+(long long) source_image->m_nHeight) > (long long) image->m_nHeight))
	{
		if ((source_image->alpha_trait == UndefinedPixelTrait) && (image->alpha_trait != UndefinedPixelTrait))
			SetImageAlpha(source_image, (const unsigned char) 255);
		status = true;

		for (long long y = 0; y < (long long) source_image->m_nHeight; y++)
		{
			if (!status)
				continue;
			const unsigned char* p = GetPixels(source_image, 0, y, source_image->m_nWidth, 1);
			unsigned char* q = GetPixels(image, x_offset, y + y_offset, source_image->m_nWidth, 1);
			if ((p == nullptr) || (q == nullptr))
			{
				status = false;
				continue;
			}
			for (long long x = 0; x < (long long) source_image->m_nWidth; x++)
			{
				if (GetPixelReadMask(source_image, p) <= (255.0 / 2.0))
				{
					p += source_image->number_channels;
					q += image->number_channels;
					continue;
				}
				for (long long i=0; i < source_image->number_channels; i++)
				{
					PixelChannel channel = GetPixelChannelChannel(source_image,i);
					PixelTrait source_traits = GetPixelChannelTraits(source_image, channel);
					PixelTrait traits = GetPixelChannelTraits(image,channel);
					if ((source_traits == UndefinedPixelTrait) || (traits == UndefinedPixelTrait))
						continue;
					SetPixelChannel(image, channel, p[i], q);
				}
				p += source_image->number_channels;
				q += image->number_channels;
			}
		}
		DestroyImage(source_image);
		return status;
    }

	status = true;
	for (long long y = 0; y < (long long) image->m_nHeight; y++)
	{
		if (!status)
			continue;

		if (clip_to_self != 0)
		{
			if (y < y_offset)
				continue;
			if ((y-(double) y_offset) >= (double) source_image->m_nHeight)
				continue;
		}

		const unsigned char* pixels = nullptr;
		const unsigned char* p = nullptr;
		if ((y >= y_offset) && ((y-(double) y_offset) < (double) source_image->m_nHeight))
		{
			p = GetPixels(source_image, 0, y - y_offset, source_image->m_nWidth, 1);
			if (p == nullptr)
			{
				status = false;
				continue;
			}
			pixels = p;
			if (x_offset < 0)
				p -= x_offset*source_image->number_channels;
		}

		unsigned char* q = GetPixels(image, 0, y, image->m_nWidth, 1);
		if (q == nullptr)
		{
			status = false;
			continue;
		}

		PixelInfo canvas_pixel, source_pixel;
		GetPixelInfo(image, &canvas_pixel);
		GetPixelInfo(source_image, &source_pixel);

		for (long long x = 0; x < (long long) image->m_nWidth; x++)
		{
			if (clip_to_self != 0)
			{
				if (x < x_offset)
				{
					q += image->number_channels;
					continue;
				}
				if ((x - (double) x_offset) >= (double) source_image->m_nWidth)
					break;
			}

			if ((pixels == nullptr) || (x < x_offset) || ((x - (double) x_offset) >= (double) source_image->m_nWidth))
			{
				for (long long i = 0; i < image->number_channels; i++)
				{
					double pixel = 0;

					PixelChannel channel = GetPixelChannelChannel(image, i);
					PixelTrait traits = GetPixelChannelTraits(image, channel);
					PixelTrait source_traits = GetPixelChannelTraits(source_image, channel);
					if ((traits == UndefinedPixelTrait) || (source_traits == UndefinedPixelTrait))
						continue;

					if (channel == AlphaPixelChannel)
						pixel =  TransparentAlpha;
					else
						pixel=0;

					q[i]=clamp != 0 ? ClampPixel(pixel) : ClampToQuantum(pixel);
				}

				q += image->number_channels;
				continue;
			}

			double Sa = (1.0/255.0)* GetPixelAlpha(source_image,p);
			Sa = Sa == 0 ? 1 : Sa;

			for (long long i = 0; i < image->number_channels; i++)
			{
				double pixel = 0.0;

				PixelChannel channel = GetPixelChannelChannel(image, i);
				PixelTrait traits = GetPixelChannelTraits(image, channel);
				PixelTrait source_traits = GetPixelChannelTraits(source_image, channel);

				if (traits == UndefinedPixelTrait)
					continue;

				if ((channel == AlphaPixelChannel) && ((traits & UpdatePixelTrait) != 0))
				{
					pixel = 255.0 * Sa;

					q[i]=clamp != 0 ? ClampPixel(pixel) :
							   ClampToQuantum(pixel);
					continue;
				}

				if (source_traits == UndefinedPixelTrait)
					continue;

				double Sc = GetPixelChannel(source_image, channel, p);
				double Dc = q[i];

				if ((traits & CopyPixelTrait) != 0)
				{
					q[i]=ClampToQuantum(Dc);
					continue;
				}

				double Sca = (1.0/255.0) * Sa * Sc;
				pixel = Dc;
				pixel = 255.0 * Sca;
				q[i] = clamp != 0 ? ClampPixel(pixel) : ClampToQuantum(pixel);
			}
			p += source_image->number_channels;
			size_t channels = source_image->number_channels;

			if (p >= (pixels+channels*source_image->m_nWidth))
				p=pixels;

			q += image->number_channels;
		}
	}
	if (canvas_image != nullptr)
		DestroyImage(canvas_image);
	else
		DestroyImage(source_image);

	return status;
}

bool DecodeHeader(FILE* hFile, ImagePICT* image)
{
	unsigned char header[4];

	unsigned char skip[1];

	int c;

	PICTrectangle frame;

	Read(hFile, 4, header);

	if (!((header[0] == 0x50) && (header[1] == 0x49) && (header[2] == 0x43) && (header[3] == 0x54 )))
	{
		frame.top		= (short) ((header[2] << 8) | header[3]);
		frame.left		= (short) ReadShortValue(hFile);
		frame.bottom	= (short) ReadShortValue(hFile);
		frame.right		= (short) ReadShortValue(hFile);

		Read(hFile, 1, skip);
		c = ReadByte(hFile);
		if (c != 0x11)
		{
			fseek(hFile, 512, 0);
			ReadShortValue(hFile);
			if (ReadRectangle(hFile, &frame) == 0)
				return false;

			Read(hFile, 1, skip);
			c = ReadByte(hFile);

			if (c == 0x11)
			{
				int version = ReadByte(hFile);
				if (version == 2)
				{
					int version2 = ReadByte(hFile);
					if (version2 != 0xff)
						return false;

					image->m_pctVersion = 2;

					if ((frame.left < 0) || (frame.right < 0) || (frame.top < 0) || (frame.bottom < 0) || (frame.left >= frame.right) || (frame.top >= frame.bottom))
						return false;

					image->m_nWidth  = (size_t) (frame.right-frame.left);
					image->m_nHeight = (size_t) (frame.bottom-frame.top);

				   return true;
				}
				else if (version == 1)
				{
					image->m_pctVersion = 1;
					if ((frame.left < 0) || (frame.right < 0) || (frame.top < 0) || (frame.bottom < 0) || (frame.left >= frame.right) || (frame.top >= frame.bottom))
						return false;

					image->m_nWidth	 = (size_t) (frame.right-frame.left);
					image->m_nHeight = (size_t) (frame.bottom-frame.top);

					return true;
				}
				else
					return 0;
			}
			else
				return 0;
		}
	}

	int version = ReadByte(hFile);
	if (version == 2)
	{
		int version2 = ReadByte(hFile);
		if (version2 != 0xff)
			return false;
		image->m_pctVersion = 2;

		if ((frame.left < 0) || (frame.right < 0) || (frame.top < 0) || (frame.bottom < 0) || (frame.left >= frame.right) || (frame.top >= frame.bottom))
			return false;

		image->m_nWidth  = (size_t) (frame.right - frame.left);
		image->m_nHeight = (size_t) (frame.bottom-frame.top);
	}
	else
	{
		image->m_pctVersion = 1;
		image->m_nWidth		= (size_t) (frame.right - frame.left);
		image->m_nHeight	= (size_t) (frame.bottom - frame.top);
   }

   return true;
}

const unsigned char *UnpackScanline(const unsigned char *pixels, const unsigned int bits_per_pixel, unsigned char* scanline, size_t *bytes_per_line)
{
	const unsigned char* p = pixels;
	unsigned char* q = scanline;
	switch (bits_per_pixel)
	{
	case 8:
	case 16:
	case 32:
		return pixels;
	case 4:
	{
		for (long long i = 0; i < (long long) *bytes_per_line; i++)
		{
			*q++ = (*p >> 4) & 0xff;
			*q++ = (*p & 15);
			p++;
		}
		*bytes_per_line *= 2;
		break;
	}
	case 2:
	{
		for (long long i = 0; i < (long long) *bytes_per_line; i++)
		{
			*q++ = (*p >> 6) & 0x03;
			*q++ = (*p >> 4) & 0x03;
			*q++ = (*p >> 2) & 0x03;
			*q++ = (*p & 3);
			p++;
		}
		*bytes_per_line *= 4;
		break;
	}
	case 1:
	{
		for (long long i = 0; i < (long long) *bytes_per_line; i++)
		{
			*q++ = (*p >> 7) & 0x01;
			*q++ = (*p >> 6) & 0x01;
			*q++ = (*p >> 5) & 0x01;
			*q++ = (*p >> 4) & 0x01;
			*q++ = (*p >> 3) & 0x01;
			*q++ = (*p >> 2) & 0x01;
			*q++ = (*p >> 1) & 0x01;
			*q++ = (*p & 0x01);
			p++;
		}
		*bytes_per_line *= 8;
		break;
	}
	default:
		break;
	}
	return scanline;
}

unsigned char *DecodeImage(FILE *blob,ImagePICT *image, size_t bytes_per_line, const unsigned int bits_per_pixel, size_t *extent)
{
	bool status = true;

	if (bits_per_pixel <= 8)
		bytes_per_line &= 0x7fff;

	size_t width = image->m_nWidth;
	size_t bytes_per_pixel = 1;

	if (bits_per_pixel == 16)
	{
		bytes_per_pixel = 2;
		width *= 2;
	}
	else if (bits_per_pixel == 32)
		width*=image->alpha_trait ? 4 : 3;

	if (bytes_per_line == 0)
		bytes_per_line=width;

	size_t row_bytes = (size_t) (image->m_nWidth | 0x8000);

	if (image->storage_class == DirectClass)
		row_bytes = (size_t) ((4*image->m_nWidth) | 0x8000);

	unsigned char* pixels = (unsigned char *) malloc(image->m_nHeight*row_bytes * sizeof(*pixels));
	if (pixels == nullptr)
		return nullptr;

	*extent=row_bytes*image->m_nHeight * sizeof(*pixels);
	memset(pixels, 0, *extent);
	unsigned char* scanline = (unsigned char *) malloc(row_bytes * 2 * sizeof(*scanline));

	if (scanline == nullptr)
	{
		free(pixels);
		return nullptr;
	}

	memset(scanline, 0, 2 * row_bytes * sizeof(*scanline));

	unsigned char unpack_buffer[8*256];
	memset(unpack_buffer, 0, sizeof(unpack_buffer));

	if (bytes_per_line < 8)
	{
		for (long long y = 0; y < (long long) image->m_nHeight; y++)
		{
			unsigned char*  q = pixels + y * (long long) width * image->number_channels;
			size_t number_pixels = bytes_per_line;
			long long count = Read(blob, (size_t) number_pixels,scanline);
			if (count != (long long) number_pixels)
			{
				status = false;
				break;
			}
			const unsigned char* p = UnpackScanline(scanline, bits_per_pixel, unpack_buffer, &number_pixels);
			if ((q+number_pixels) > (pixels+(*extent)))
			{
				status = false;
				break;
			}
			memcpy(q, p, (size_t) number_pixels);
		}
		free(scanline);
		if (!status)
			free(pixels);
		return pixels;
	}

	for (long long y = 0; y < (long long) image->m_nHeight; y++)
	{
		unsigned char* q = pixels + y * (long long) width;

		size_t scanline_length;
		if (bytes_per_line > 250)
			scanline_length = ReadShortValue(blob);
		else
			scanline_length = (size_t) ReadByte(blob);

		if ((scanline_length >= row_bytes) || (scanline_length == 0))
		{
			status = false;
			break;
		}

		long long count = Read(blob,scanline_length,scanline);
		if (count != (long long) scanline_length)
		{
			status = false;
			break;
		}

		for (long long j = 0; j < (long long) scanline_length; )
			if ((scanline[j] & 0x80) == 0)
			{
				size_t length = (size_t) ((scanline[j] & 0xff) + 1);
				size_t number_pixels = length * bytes_per_pixel;
				const unsigned char* p = UnpackScanline(scanline + j + 1, bits_per_pixel, unpack_buffer, &number_pixels);
				if ((size_t) (q-pixels+(long long) number_pixels) <= *extent)
					memcpy(q,p,(size_t) number_pixels);
				q += number_pixels;
				j += (long long) (length*bytes_per_pixel+1);
			}
			else
			{
				size_t length = (size_t) (((scanline[j] ^ 0xff) & 0xff)+2);
				size_t number_pixels = bytes_per_pixel;
				const unsigned char* p = UnpackScanline(scanline + j + 1, bits_per_pixel, unpack_buffer, &number_pixels);
				for (long long i = 0; i < (long long) length; i++)
				{
					if ((size_t) (q - pixels + (long long) number_pixels) <= *extent)
						memcpy(q, p, (size_t) number_pixels);
					q += number_pixels;
				}
				j += (long long) bytes_per_pixel + 1;
			}
	}
	free(scanline);

	if (!status)
	{
		free(pixels);
		pixels = nullptr;
	}

	return pixels;
}

void ReadPolygon(FILE* file, Polygon<short>& poly)
{
	size_t size = static_cast<size_t>(ReadShortValue(file));
	ReadShortValue(file);
	ReadShortValue(file);
	ReadShortValue(file);
	ReadShortValue(file);

	size = (size - 10) / 4;
	const size_t max_possible_points = GetSize(file) - ftell(file);
	if (size > max_possible_points)
		size = max_possible_points;

	poly.Size = size;
	poly.Points = (Point<short>*) malloc(size * sizeof(Point<short>));

	for (size_t i = 0; i < size; i++)
	{
		short y = ReadShortValue(file);
		short x = ReadShortValue(file);
		poly.Points[i] = Point(x, y);
		if (feof(file))
		{
			poly.Size = i;
			break;
		}
	}
}

PICTrectangle ContractRect(const PICTrectangle& rect, int penSize, bool isFrame)
{
	if (!isFrame)
		return rect;

	if (penSize * 2 > rect.right - rect.left)
		penSize = (rect.right - rect.left + 1) / 2;
	if (penSize * 2 > rect.bottom - rect.top)
		penSize = (rect.bottom - rect.top + 1) / 2;

	const long X[2] = {rect.left + penSize / 2, rect.right - (penSize + 1) / 2};
	const long Y[2] = {rect.top + penSize / 2, rect.bottom - (penSize + 1) / 2};

	return PICTrectangle(Y[0], X[0], Y[1], X[1]);
}

void DrawRect(NSGraphics::IGraphicsRenderer* renderer, const PICTrectangle& rect, size_t penWidth, size_t penHeight, bool isFrame)
{
	int pen_size = (penWidth + penHeight) / 2;
	PICTrectangle new_rect = ContractRect(rect, pen_size, isFrame);
	const long X[2] = {new_rect.left, new_rect.right};
	const long Y[2] = {new_rect.top, new_rect.bottom};

	renderer->PathCommandStart();
	renderer->BeginCommand(c_nPathType);
	Aggplus::CGraphicsPath path;
	path.AddRectangle(X[0], Y[0], X[1] - X[0], Y[1] - Y[0]);
	size_t points_count = path.GetPointCount();

	Aggplus::PointF* points = new Aggplus::PointF[points_count];
	path.GetPathPoints(points, points_count);

	renderer->PathCommandMoveTo(points[0].X, points[0].Y);

	for (size_t i = 1; i < points_count; i += 3)
		renderer->PathCommandCurveTo(points[i].X, points[i].Y,
									 points[i + 1].X, points[i + 1].Y,
									 points[i + 2].X, points[i + 2].Y);

	if (!isFrame)
		renderer->Fill();

	renderer->DrawPath(c_nStroke);
	renderer->EndCommand(c_nPathType);
	renderer->PathCommandClose();

	delete[] points;
}

void DrawRoundRect(NSGraphics::IGraphicsRenderer* renderer, const PICTrectangle& roundRect, size_t penWidth, size_t penHeight, bool isFrame)
{
	int pen_size = (penWidth + penHeight) / 2;

	PICTrectangle oval = ContractRect(roundRect, pen_size, isFrame);
	int ovalW = penWidth;
	int ovalH = penHeight;
	const long X[2] = {oval.left, oval.right};
	const long Y[2] = {oval.top, oval.bottom};
	long width = X[1] - X[0];
	long height = Y[1] - Y[0];

	if (ovalW > width)
		ovalW = static_cast<int>(width);
	if (ovalH > height)
		ovalH = static_cast<int>(height);

	renderer->PathCommandStart();
	renderer->BeginCommand(c_nPathType);
	Aggplus::CGraphicsPath path;
	path.AddRoundRectangle(X[0], Y[0], X[1] - X[0], Y[1] - Y[0], width == 0.0 ? 0.0 : ovalW / width, height == 0.0 ? 0.0 : ovalH / height);
	size_t points_count = path.GetPointCount();

	Aggplus::PointF* points = new Aggplus::PointF[points_count];
	path.GetPathPoints(points, points_count);

	renderer->PathCommandMoveTo(points[0].X, points[0].Y);

	for (size_t i = 1; i < points_count; i += 3)
		renderer->PathCommandCurveTo(points[i].X, points[i].Y,
									 points[i + 1].X, points[i + 1].Y,
									 points[i + 2].X, points[i + 2].Y);

	if (!isFrame)
		renderer->Fill();

	renderer->DrawPath(c_nStroke);
	renderer->EndCommand(c_nPathType);
	renderer->PathCommandClose();

	delete[] points;
}

void DrawOval(NSGraphics::IGraphicsRenderer* renderer, const PICTrectangle& oval, size_t penWidth, size_t penHeight, bool isFrame)
{
	int pen_size = (penWidth + penHeight) / 2;

	PICTrectangle rect = ContractRect(oval, pen_size, isFrame);
	const long X[2] = {rect.left, rect.right};
	const long Y[2] = {rect.top, rect.bottom};

	renderer->PathCommandStart();
	renderer->BeginCommand(c_nPathType);
	Aggplus::CGraphicsPath path;
	path.AddEllipse(X[0], Y[0], X[1] - X[0], Y[1] - Y[0]);
	size_t points_count = path.GetPointCount();

	Aggplus::PointF* points = new Aggplus::PointF[points_count];
	path.GetPathPoints(points, points_count);

	renderer->PathCommandMoveTo(points[0].X, points[0].Y);

	for (size_t i = 1; i < points_count; i += 3)
		renderer->PathCommandCurveTo(points[i].X, points[i].Y,
									 points[i + 1].X, points[i + 1].Y,
									 points[i + 2].X, points[i + 2].Y);

	if (!isFrame)
		renderer->Fill();

	renderer->DrawPath(c_nStroke);
	renderer->EndCommand(c_nPathType);
	renderer->PathCommandClose();

	delete[] points;
}

void DrawPolygon(NSGraphics::IGraphicsRenderer* renderer, const Polygon<short>& poly, size_t penWidth, size_t penHeight, bool isFrame)
{
	size_t pen_size = (penWidth + penHeight) / 2;
	long deca1TL[2] = {0, 0}, deca1BR[2] = {static_cast<long>(penWidth), static_cast<long>(penHeight)};

	if (isFrame)
	{
		deca1TL[0] += pen_size / 2;
		deca1TL[1] += pen_size / 2;
		deca1BR[0] -= (pen_size + 1) / 2;
		deca1BR[1] -= (pen_size + 1) / 2;
	}
	else
		deca1BR[0] = deca1BR[1] = 0;

	size_t points_count = poly.Size;
	if (points_count < 1)
		return;

	double bary[2] = {0.0, 0.0};
	for (int i = 0; i < points_count; i++)
	{
		const Point<short>& pt = poly.Points[i];
		bary[0] += double(pt.X);
		bary[1] += double(pt.Y);
	}
	bary[0] /= double(points_count);
	bary[1] /= double(points_count);


	Polygon<double> B2Dpoly;
	B2Dpoly.Points = (Point<double>*) malloc(points_count * sizeof(Point<double>));
	for (int i = 0; i < points_count; i++)
	{
		const Point<short>& pt = poly.Points[i];
		double x = (double(pt.X) < bary[0]) ? pt.X + deca1TL[0] : pt.X + deca1BR[0];
		double y = (double(pt.Y) < bary[1]) ? pt.Y + deca1TL[1] : pt.Y + deca1BR[1];
		B2Dpoly.Points[i] = Point(x, y);
	}

	renderer->PathCommandStart();
	renderer->BeginCommand(c_nPathType);
	renderer->PathCommandMoveTo(B2Dpoly.Points[0].X, B2Dpoly.Points[0].Y);

	for (int i = 1; i < points_count; i++)
		renderer->PathCommandLineTo(B2Dpoly.Points[i].X, B2Dpoly.Points[i].Y);


	if (!isFrame)
		renderer->Fill();

	renderer->DrawPath(c_nStroke);
	renderer->EndCommand(c_nPathType);
	renderer->PathCommandClose();
}

bool DecodePICT(FILE* hFile, ImagePICT* image)
{
	if (hFile == nullptr)
		return 0;

	PICTPixmap pixmap;
	pixmap.bits_per_pixel=0;
	pixmap.component_count=0;

	if (!DecodeHeader(hFile, image))
		return false;

	if (feof(hFile) != 0)
		return false;

	long long flags = 0;
	image->m_ndepth = 8;
	image->resolutionX = 72.0;
	image->resolutionY = 72.0;

	if (!AquirePixelsMemory(image))
		return false;

	NSGraphics::IGraphicsRenderer* renderer = NSGraphics::Create();
	NSFonts::IFontManager* fmp = NSFonts::NSFontManager::Create();
	renderer->SetFontManager(fmp);

	BYTE* data = new BYTE[4 * image->m_nWidth * image->m_nHeight];

	CBgraFrame frame;
	frame.put_Data(data);
	frame.put_Width(image->m_nWidth);
	frame.put_Height(image->m_nHeight);
	frame.put_Stride(4 * image->m_nWidth);

	renderer->CreateFromBgraFrame(&frame);
	renderer->SetSwapRGB(true);
	renderer->put_Width(image->m_nWidth);
	renderer->put_Height(image->m_nHeight);
	renderer->put_PenColor(0x000000);

	short pen_width = 0;
	short pen_height = 0;

	for (int code = 0; feof(hFile) == 0; )
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
				size_t length = (size_t) ReadShortValue(hFile);
				if (length > GetSize(hFile))
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}

				if (length != 0x000a)
				{
					for (size_t i = 0; i < length - 2; i++)
						if (ReadByte(hFile) == EOF)
							break;
					break;
				}

				PICTrectangle frame;
				if (ReadRectangle(hFile, &frame) == 0)
					return false;
				if (((frame.left & 0x8000) != 0) || ((frame.top & 0x8000) != 0))
					break;
				image->m_nHeight = (size_t) (frame.bottom - frame.top);
				image->m_nWidth = (size_t) (frame.right - frame.left);

				if (!AquirePixelsMemory(image))
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}
				break;
			}
			case 0x07:
			{
				pen_height = ReadShortValue(hFile);
				pen_width = ReadShortValue(hFile);
				break;
			}
			// case 0x0e:
			// {
			// 	long color = ReadLongValue(hFile);

			// 	switch(color)
			// 	{
			// 	case 33: renderer->put_PenColor(0x000000); break;
			// 	case 30: renderer->put_PenColor(0xFFFFFF); break;
			// 	case 205: renderer->put_PenColor(0xFF8080); break; //lightred
			// 	case 341: renderer->put_PenColor(0x90EE90); break; //lightgreen
			// 	case 409: renderer->put_PenColor(0xADD8E6); break; //lightblue
			// 	case 273: renderer->put_PenColor(0xE0FFFF); break; //lightcyan
			// 	case 137: renderer->put_PenColor(0xFF80FF); break; //lightmagenta
			// 	case 69: renderer->put_PenColor(0xFFFF00); break; //yellow
			// 	default: renderer->put_PenColor(0xD3D3D3); break; //lightgray
			// 	}
			// 	break;
			// }
			// case 0x0f:
			// {
			// 	long color = ReadLongValue(hFile);

			// 	switch(color)
			// 	{
			// 	case 33: renderer->put_PenColor(0x000000); break;
			// 	case 30: renderer->put_PenColor(0xFFFFFF); break;
			// 	case 205: renderer->put_PenColor(0xFF8080); break; //lightred
			// 	case 341: renderer->put_PenColor(0x90EE90); break; //lightgreen
			// 	case 409: renderer->put_PenColor(0xADD8E6); break; //lightblue
			// 	case 273: renderer->put_PenColor(0xE0FFFF); break; //lightcyan
			// 	case 137: renderer->put_PenColor(0xFF80FF); break; //lightmagenta
			// 	case 69: renderer->put_PenColor(0xFFFF00); break; //yellow
			// 	default: renderer->put_PenColor(0xD3D3D3); break; //lightgray
			// 	}
			// 	break;
			// }
			case 0x12:
			case 0x13:
			case 0x14:
			{
				int pattern;
				size_t height, width;
				pattern = (int) ReadShortValue(hFile);
				for (size_t i = 0; i < 8; i++)
					if (ReadByte(hFile) == EOF)
						break;
				if (pattern == 2)
				{
					for (size_t i = 0; i < 5; i++)
						if (ReadByte(hFile) == EOF)
							break;
					break;
				}
				if (pattern != 1)
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
                }

				size_t length = (size_t) ReadShortValue(hFile);
				if (length > GetSize(hFile))
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
                }

				PICTrectangle frame;
				if (ReadRectangle(hFile, &frame) == 0)
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}

				if (ReadPixmap(hFile, &pixmap) == 0)
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}

				image->m_ndepth = (size_t) pixmap.component_size;
				image->resolutionX = 1.0 * pixmap.horizontal_resolution;
				image->resolutionY = 1.0 * pixmap.vertical_resolution;

				ReadLongValue(hFile);
				flags = (long long) ReadShortValue(hFile);
				length=ReadShortValue(hFile);

				if (length > GetSize(hFile))
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}

				for (size_t i = 0; i < length; i++)
					ReadLongValue(hFile);
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
					for (size_t i = 0; i < (length*height); i++)
						if (ReadByte(hFile) == EOF)
							break;
				}
				else
					for (size_t i = 0; i < height; i++)
					{
						size_t scanline_length;

						if (feof(hFile) != 0)
							break;
						if (length > 200)
							scanline_length=ReadShortValue(hFile);
						else
							scanline_length = (size_t) ReadByte(hFile);
						if (scanline_length > GetSize(hFile))
						{
							DeletePixelsMemory(image);
							RELEASEINTERFACE(fmp);
							RELEASEINTERFACE(renderer);
							return false;
						}
						for (size_t j = 0; j < scanline_length; j++)
							if (ReadByte(hFile) == EOF)
								break;
					}
				break;
			}
			case 0x1a:
			{
				BYTE r = ReadShortValue(hFile) >> 8;
				BYTE g = ReadShortValue(hFile) >> 8;
				BYTE b = ReadShortValue(hFile) >> 8;
				long c = r << 16 | g << 8 | b;
				renderer->put_BrushColor1(c);
				break;
			}
			case 0x1b:
			{
				image->background_color.red = 257.0 * ReadShortValue(hFile);
				image->background_color.green = 257.0 * ReadShortValue(hFile);
				image->background_color.blue = 257.0 * ReadShortValue(hFile);
				break;
			}
			case 0x20:
			{
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				break;
			}
			case 0x21:
			{
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				break;
			}
			case 0x22:
			{
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				break;
			}
			case 0x23:
			{
				ReadShortValue(hFile);
				// TODO: Read and draw Line
				break;
			}
			case 0x30:
			case 0x31:
			case 0x32:
			case 0x33:
			case 0x34:
			{
				PICTrectangle rect;
				if (!ReadRectangle(hFile, &rect))
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}
				DrawRect(renderer, rect, pen_width, pen_height, code == 0x30);
				break;
			}
			case 0x38:
			case 0x39:
			case 0x3a:
			case 0x3b:
			case 0x3c:
			{
				// TODO: Read and draw same Rect
				break;
			}
			case 0x40:
			case 0x41:
			case 0x42:
			case 0x43:
			case 0x44:
			{
				PICTrectangle round_rect;
				if (!ReadRectangle(hFile, &round_rect))
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}
				DrawRoundRect(renderer, round_rect, pen_width, pen_height, code == 0x40);
				break;
			}
			case 0x48:
			case 0x49:
			case 0x4a:
			case 0x4b:
			case 0x4c:
			{
				// TODO: Read and draw same Roundrect
				break;
			}
			case 0x50:
			case 0x51:
			case 0x52:
			case 0x53:
			case 0x54:
			{
				PICTrectangle oval;
				if (!ReadRectangle(hFile, &oval))
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}
				DrawOval(renderer, oval, pen_width, pen_height, code == 0x50);
				break;
			}
			case 0x58:
			case 0x59:
			case 0x5a:
			case 0x5b:
			case 0x5c:
			{
				// TODO: Read and draw same Oval
				break;
			}
			case 0x60:
			case 0x61:
			case 0x62:
			case 0x63:
			case 0x64:
			{
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				// TODO: Read and draw Arc
				break;
			}
			case 0x68:
			case 0x69:
			case 0x6a:
			case 0x6b:
			case 0x6c:
			{
				ReadShortValue(hFile);
				ReadShortValue(hFile);
				// TODO: Read and draw same Arc
				break;
			}

            case 0x70:
            case 0x71:
            case 0x72:
            case 0x73:
            case 0x74:
			{
				Polygon<short> poly;
				ReadPolygon(hFile, poly);
				DrawPolygon(renderer, poly, pen_width, pen_height, code == 0x70);
				break;
			}
            case 0x75:
            case 0x76:
            case 0x77:
			{
				size_t length = (size_t) ReadShortValue(hFile);
				if (length > GetSize(hFile))
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}
				for (size_t i = 0; i < (length - 2); i++)
					if (ReadByte(hFile) == EOF)
						break;
				break;
			}
			case 0x78:
			case 0x79:
			case 0x7a:
			case 0x7b:
			case 0x7c:
			{
				// TODO: Read and draw same Poly
				break;
			}
			case 0x80:
			case 0x81:
			case 0x82:
			case 0x83:
			case 0x84:
			{
				// TODO: Read and draw Rgn
				short size = ReadShortValue(hFile);
				for (int i = 0; i < size - 2; i++)
					ReadByte(hFile);
				break;
			}
			case 0x88:
			case 0x89:
			case 0x8a:
			case 0x8b:
			case 0x8c:
			{
				// TODO: Read and draw same Rgn
				break;
			}
			case 0x90:
			case 0x91:
			case 0x98:
			case 0x99:
			case 0x9a:
			case 0x9b:
			{
				PICTrectangle source, destination;

				unsigned char *p;

				size_t k;

				long long bytes_per_line;

				unsigned char *pixels;

				bytes_per_line = 0;
				if ((code != 0x9a) && (code != 0x9b))
					bytes_per_line= (long long) ReadShortValue(hFile);
				else
				{
					ReadShortValue(hFile);
					ReadShortValue(hFile);
					ReadShortValue(hFile);
				}

				PICTrectangle frame;
				if (ReadRectangle(hFile, &frame) == 0)
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}

				ImagePICT* tile_image = CloneImage(image, (size_t) (frame.right-frame.left), (size_t) (frame.bottom-frame.top));
				if (tile_image == nullptr)
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
                    DeletePixelsMemory(image);
					return false;
				}

				if (!AquirePixelsMemory(tile_image))
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					DestroyImage(tile_image);
					return false;
				}

				if ((code == 0x9a) || (code == 0x9b) || ((bytes_per_line & 0x8000) != 0))
				{
					if (ReadPixmap(hFile, &pixmap) == 0)
					{
						RELEASEINTERFACE(fmp);
						RELEASEINTERFACE(renderer);
						DeletePixelsMemory(image);
						DestroyImage(tile_image);
						return false;
					}
					tile_image->m_ndepth=(size_t) pixmap.component_size;
					tile_image->alpha_trait=pixmap.component_count == 4 ? BlendPixelTrait : UndefinedPixelTrait;
					tile_image->resolutionX=(double) pixmap.horizontal_resolution;
					tile_image->resolutionY=(double) pixmap.vertical_resolution;
					if (tile_image->alpha_trait != UndefinedPixelTrait)
						SetImageAlpha(tile_image, 255);
				}
				if ((code != 0x9a) && (code != 0x9b))
				{
					tile_image->colors = 2;
					if ((bytes_per_line & 0x8000) != 0)
					{
						ReadLongValue(hFile);
						flags = (long long) ReadShortValue(hFile);
						tile_image->colors = 1UL * ReadShortValue(hFile) + 1;
					}
					if (!AquireImageColormap(tile_image, tile_image->colors))
					{
						RELEASEINTERFACE(fmp);
						RELEASEINTERFACE(renderer);
						DeletePixelsMemory(image);
						DestroyImage(tile_image);
						return false;
					}

					if ((bytes_per_line & 0x8000) != 0)
					{
						for (size_t i = 0; i < tile_image->colors; i++)
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
						for (size_t i = 0; i < tile_image->colors; i++)
						{
							tile_image->colormap[i].red=((double) 255 - tile_image->colormap[i].blue);
							tile_image->colormap[i].green=((double) 255 - tile_image->colormap[i].green);
							tile_image->colormap[i].blue=((double) 255 - tile_image->colormap[i].red);
						}
					}
				}
				if (feof(hFile) != 0)
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					DestroyImage(tile_image);
					return false;
				}

				if (ReadRectangle(hFile, &source) == 0)
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					DestroyImage(tile_image);
					return false;
				}

				if (ReadRectangle(hFile, &destination) == 0)
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					DestroyImage(tile_image);
					return false;
				}

				ReadShortValue(hFile);

				if ((code == 0x91) || (code == 0x99) || (code == 0x9b))
				{
					size_t length = (size_t) ReadShortValue(hFile);
					if ((size_t) length > GetSize(hFile))
					{
						RELEASEINTERFACE(fmp);
						RELEASEINTERFACE(renderer);
						DeletePixelsMemory(image);
						DestroyImage(tile_image);
						return false;
					}

					for (size_t i = 0; i < length - 2; i++)
						if (ReadByte(hFile) == EOF)
							break;
				}

				size_t extent;
				if ((code != 0x9a) && (code != 0x9b) && (bytes_per_line & 0x8000) == 0)
					pixels = DecodeImage(hFile, tile_image, (size_t) bytes_per_line, 1, &extent);
				else
					pixels = DecodeImage(hFile, tile_image, (size_t) bytes_per_line, (unsigned int) pixmap.bits_per_pixel, &extent);

				if (pixels == (unsigned char*) nullptr)
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					DestroyImage(tile_image);
					return false;
				}

				p = pixels;
				for (size_t y=0; y < (long long) tile_image->m_nHeight; y++)
				{
					if (p > (pixels+extent+image->m_nWidth))
					{
						RELEASEINTERFACE(fmp);
						RELEASEINTERFACE(renderer);
						free(pixels);
						DeletePixelsMemory(image);
						DeletePixelsMemory(tile_image);
						DestroyImage(tile_image);
						return 0;
					}
					unsigned char* q = tile_image->ppixels + tile_image->number_channels * (y * tile_image->m_nWidth);
					if (q == (unsigned char *) nullptr)
						break;
					for (size_t x=0; x < tile_image->m_nWidth; x++)
					{
						if (tile_image->storage_class == PseudoClass)
						{
							unsigned char index;
							if (((long long) *p < 0) || ((long long) *p >= (long long) tile_image->colors))
								index = 0;
							else
								index = *p;
							SetPixelIndex(tile_image,index,q);
							SetPixelRed(tile_image, tile_image->colormap[(long long) index].red,q);
							SetPixelGreen(tile_image, tile_image->colormap[(long long) index].green,q);
							SetPixelBlue(tile_image, tile_image->colormap[(long long) index].blue,q);
						}
						else
						{
							if (pixmap.bits_per_pixel == 16)
							{
								size_t i = (size_t) (*p++);
								k=(size_t) (*p);
								SetPixelRed(tile_image,(unsigned char) ((i & 0x7c) << 1),q);
								SetPixelGreen(tile_image,(unsigned char) ((size_t) ((i & 0x03) << 6) |((k & 0xe0) >> 2)),q);
								SetPixelBlue(tile_image,(unsigned char) ((k & 0x1f) << 3),q);
							}
							else if (tile_image->alpha_trait == UndefinedPixelTrait)
							{
								if (p > (pixels+extent+2*image->m_nWidth))
								{
									RELEASEINTERFACE(fmp);
									RELEASEINTERFACE(renderer);
									free(pixels);
									DeletePixelsMemory(image);
									DestroyImage(tile_image);
									return false;
								}
								SetPixelRed(tile_image,*p,q);
								SetPixelGreen(tile_image,*(p+tile_image->m_nWidth),q);
								SetPixelBlue(tile_image,*(p+2*tile_image->m_nWidth),q);
							}
							else
							{
								if (p > (pixels+extent+3*image->m_nWidth))
								{
									RELEASEINTERFACE(fmp);
									RELEASEINTERFACE(renderer);
									free(pixels);
									DeletePixelsMemory(image);
									DestroyImage(tile_image);
									return false;
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
					if ((tile_image->storage_class == DirectClass) && (pixmap.bits_per_pixel != 16))
					{
						p+=(pixmap.component_count-1)*(long long) tile_image->m_nWidth;
						if (p < pixels)
							break;
					}
				}

				if (tile_image->storage_class == PseudoClass)
					SetImageAlpha(tile_image, 255);
				else if (pixmap.bits_per_pixel == 16)
					SetImageAlpha(tile_image, 255);
				else if (tile_image->alpha_trait == UndefinedPixelTrait)
					SetImageAlpha(tile_image, 255);

				free(pixels);
				if (feof(hFile) == 0)
					if ((code == 0x9a) || (code == 0x9b) || ((bytes_per_line & 0x8000) != 0))
						CompositeImage(image,tile_image,1,(long long) destination.left,(long long)destination.top);
				DestroyImage(tile_image);
				break;
			}
			case 0xa1:
			{
				unsigned char *info;
				size_t type;

				type = ReadShortValue(hFile);
				size_t length = (size_t) ReadShortValue(hFile);
				if ((size_t) length > GetSize(hFile))
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					DeletePixelsMemory(image);
					return false;
				}
				if (length == 0)
					break;
				info = (unsigned char*) malloc(length * sizeof (*info));
				if (info == (unsigned char*) nullptr)
					break;
				int count = Read(hFile, length, info);
				if (count != length)
				{
					RELEASEINTERFACE(fmp);
					RELEASEINTERFACE(renderer);
					free(info);
					DeletePixelsMemory(image);
					return false;
				}
				switch (type)
				{
				case 0xe0:
				{
					StringInfo* profile = BlobToStringInfo(nullptr, length);
					if (profile->length != 0)
						memcpy(profile->datum,info,profile->length);

					if (!SetImageProfileInternal(image, "icc", profile, 0))
					{
						RELEASEINTERFACE(fmp);
						RELEASEINTERFACE(renderer);
						free(info);
						DeletePixelsMemory(image);
						DestroyStringInfo(profile);
						return false;
					}
					DestroyStringInfo(profile);
					break;
				}
				case 0x1f2:
				{
					StringInfo* profile	= BlobToStringInfo(nullptr,length);
					if (profile->length != 0)
						memcpy(profile->datum,info,profile->length);

					if (!SetImageProfileInternal(image,"iptc",profile,0))
					{
						RELEASEINTERFACE(fmp);
						RELEASEINTERFACE(renderer);
						free(info);
						DeletePixelsMemory(image);
						DestroyStringInfo(profile);
						return false;
					}
					DestroyStringInfo(profile);
					break;
				}
				default:
					break;
				}
				free(info);
				break;
			}
			default:
				if (codes[code].length == -1)
					ReadShortValue(hFile);
				else
					for (size_t i=0; i < codes[code].length; i++)
						if (ReadByte(hFile) == EOF)
							break;
			}
		}
		if (code == 0xc00)
		{
			for (size_t i = 0; i < 24; i++)
				if (ReadByte(hFile) == EOF)
					break;
			continue;
		}
		if (((code >= 0xb0) && (code <= 0xcf)) || ((code >= 0x8000) && (code <= 0x80ff)))
			continue;
		if ((code == 0xff) || (code == 0xffff))
			continue;
		if (((code >= 0xd0) && (code <= 0xfe)) || ((code >= 0x8100) && (code <= 0xffff)))
		{
			size_t length = (size_t) ReadShortValue(hFile);
			if (length > GetSize(hFile))
			{
				RELEASEINTERFACE(fmp);
				RELEASEINTERFACE(renderer);
				DeletePixelsMemory(image);
				return false;
			}
			for (size_t i = 0; i < length; i++)
				if (ReadByte(hFile) == EOF)
					break;
			continue;
		}
		if ((code >= 0x100) && (code <= 0x7fff))
		{
			size_t length = (size_t) ((code >> 7) & 0xff);
			if (length > GetSize(hFile))
			{
				RELEASEINTERFACE(fmp);
				RELEASEINTERFACE(renderer);
				DeletePixelsMemory(image);
				return false;
			}
			for (size_t i = 0; i <length; i++)
				if (ReadByte(hFile) == EOF)
					break;
			continue;
		}
	}


	RELEASEINTERFACE(fmp);
	RELEASEINTERFACE(renderer);

	return true;
}
