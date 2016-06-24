/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

namespace MetaFile
{
    enum ActionType 
	{
        META_NULL_ACTION                  = 0,
        META_PIXEL_ACTION                 = 100,
        META_POINT_ACTION                 = 101,
        META_LINE_ACTION                  = 102,
        META_RECT_ACTION                  = 103,
        META_ROUNDRECT_ACTION             = 104,
        META_ELLIPSE_ACTION               = 105,
        META_ARC_ACTION                   = 106,
        META_PIE_ACTION                   = 107,
        META_CHORD_ACTION                 = 108,
        META_POLYLINE_ACTION              = 109,
        META_POLYGON_ACTION               = 110,
        META_POLYPOLYGON_ACTION           = 111,
        META_TEXT_ACTION                  = 112,
        META_TEXTARRAY_ACTION             = 113,
        META_STRETCHTEXT_ACTION           = 114,
        META_TEXTRECT_ACTION              = 115,
        META_BMP_ACTION                   = 116,
        META_BMPSCALE_ACTION              = 117,
        META_BMPSCALEPART_ACTION          = 118,
        META_BMPEX_ACTION                 = 119,
        META_BMPEXSCALE_ACTION            = 120,
        META_BMPEXSCALEPART_ACTION        = 121,
        META_MASK_ACTION                  = 122,
        META_MASKSCALE_ACTION             = 123,
        META_MASKSCALEPART_ACTION         = 124,
        META_GRADIENT_ACTION              = 125,
        META_HATCH_ACTION                 = 126,
        META_WALLPAPER_ACTION             = 127,
        META_CLIPREGION_ACTION            = 128,
        META_ISECTRECTCLIPREGION_ACTION   = 129,
        META_ISECTREGIONCLIPREGION_ACTION = 130,
        META_MOVECLIPREGION_ACTION        = 131,
        META_LINECOLOR_ACTION             = 132,
        META_FILLCOLOR_ACTION             = 133,
        META_TEXTCOLOR_ACTION             = 134,
        META_TEXTFILLCOLOR_ACTION         = 135,
        META_TEXTALIGN_ACTION             = 136,
        META_MAPMODE_ACTION               = 137,
        META_FONT_ACTION                  = 138,
        META_PUSH_ACTION                  = 139,
        META_POP_ACTION                   = 140,
        META_RASTEROP_ACTION              = 141,
        META_TRANSPARENT_ACTION           = 142,
        META_EPS_ACTION                   = 143,
        META_REFPOINT_ACTION              = 144,
        META_TEXTLINECOLOR_ACTION         = 145,
        META_TEXTLINE_ACTION              = 146,
        META_FLOATTRANSPARENT_ACTION      = 147,
        META_GRADIENTEX_ACTION            = 148,
        META_LAYOUTMODE_ACTION            = 149,
        META_TEXTLANGUAGE_ACTION          = 150,
        META_OVERLINECOLOR_ACTION         = 151,
        META_RENDERGRAPHIC_ACTION         = 152,
        META_COMMENT_ACTION               = 512
    };

	#define META_LAST_ACTION  153

    enum MtfType 
	{
        MTF_CONVERSION_NONE           = 0,
        MTF_CONVERSION_1BIT_THRESHOLD = 1,
        MTF_CONVERSION_8BIT_GREYS     = 2
    };

	enum MtfMirrorType 
	{
        MTF_MIRROR_NONE = 0x00000000,
        MTF_MIRROR_HORZ = 0x00000001,
        MTF_MIRROR_VERT = 0x00000002
    };

}

