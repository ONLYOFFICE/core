/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#pragma once

#include "../Common/Base/FormatUtils.h"

namespace Global
{
	static const wchar_t ColorIdentifier[17][12] = 
	{
        L"auto",
        L"000000",//L"black",
        L"0000FF",//L"blue",
        L"00FFFF",//L"cyan",
		L"00FF00",// L"green",
        L"FF00FF",//L"magenta",
        L"FF0000",//L"red",
        L"FFFF00",//L"yellow",
        L"FFFFFF",//L"white",
        L"darkBlue",
        L"darkCyan",
        L"darkGreen",
        L"darkMagenta",
        L"darkRed",
        L"darkYellow",
        L"darkGray",
        L"lightGray"
	};
	static const wchar_t ColorNameIdentifier[17][12] = 
	{
        L"auto",
        L"black",
        L"blue",
        L"cyan",
		L"green",
        L"magenta",
        L"red",
        L"yellow",
        L"white",
        L"darkBlue",
        L"darkCyan",
        L"darkGreen",
        L"darkMagenta",
        L"darkRed",
        L"darkYellow",
        L"darkGray",
        L"lightGray"
	};
	static const wchar_t UnderlineCode[56][16] =
	{
        L"none",
        L"single",
        L"words",
        L"double",
        L"dotted",
        L"notUsed1",
        L"thick",
        L"dash",
        L"notUsed2",
        L"dotDash",
        L"dotDotDash",
        L"wave",
        L"notUsed3",
        L"notUsed4",
        L"notUsed5",
        L"notUsed6",
        L"notUsed7",
        L"notUsed8",
        L"notUsed9",
        L"notUsed10",
        L"dottedHeavy",
        L"notUsed11",
        L"notUsed12",
        L"dashedHeavy",
        L"notUsed13",
        L"dashDotHeavy",
        L"dashDotDotHeavy",
        L"wavyHeavy",
        L"notUsed14",
        L"notUsed15",
        L"notUsed16",
        L"notUsed17",
        L"notUsed18",
        L"notUsed19",
        L"notUsed20",
        L"notUsed21",
        L"notUsed22",
        L"notUsed23",
        L"notUsed24",
        L"dashLong",
        L"notUsed25",
        L"notUsed26",
        L"notUsed27",
        L"wavyDouble",
        L"notUsed28",
        L"notUsed29",
        L"notUsed30",
        L"notUsed31",
        L"notUsed32",
        L"notUsed33",
        L"notUsed34",
        L"notUsed35",
        L"notUsed36",
        L"notUsed37",
        L"notUsed38",
        L"dashLongHeavy"
	};

	static const wchar_t TabLeader[8][11] =
	{
        L"none",
        L"dot",
        L"hyphen",
        L"underscore",
        L"heavy",
        L"middleDot",
        L"none",
        L"none"
	};

	static const wchar_t TabStop[7][8] =
	{
        L"left",
        L"center",
        L"right",
        L"decimal",
        L"bar",
        L"clear",
        L"num"
	};

	static const wchar_t TextAnimation[7][16] =
	{
        L"none",
        L"lights",
        L"blinkBackground",
        L"sparkle",
        L"antsBlack",
        L"antsRed",
        L"shimmer"
	};

	static const wchar_t JustificationCode[10][15] =
	{
        L"start",
        L"center",
        L"end",
        L"both",
        L"distribute",
        L"mediumKashida",
        L"numTab",
        L"highKashida",
        L"lowKashida",
        L"thaiDistribute"
	};

	static const wchar_t VerticalPositionCode[4][7] =
	{
        L"margin",
        L"page",
        L"text",
        L"none"
	};

	static const wchar_t HorizontalPositionCode[4][7] =
	{
        L"text",
        L"margin",
        L"page",
        L"none"
	};

	static const wchar_t TextFrameWrapping[6][10] =
	{
        L"auto",
        L"notBeside",
        L"around",
        L"none",
        L"tight",
        L"through"
	};

	static const wchar_t TextFrameDropCapLocation[3][7] =
	{
        L"none",
        L"drop",
        L"margin"
	};

	typedef enum _DashStyle
	{
		dashStyleSolid,
		dashStyleShortdash,
		dashStyleShortdot,
		dashStyleShortdashdot,
		dashStyleShortdashdotdot,
		dashStyleDot,
		dashStyleDash,
		dashStyleLongdash,
		dashStyleDashdot,
		dashStyleLongdashdot,
		dashStyleLongdashdotdot
	}DashStyle;

	static const wchar_t DashStyleMap[11][16] =
	{
        L"solid",
        L"shortdash",
        L"shortdot",
        L"shortdashdot",
        L"shortdashdotdot",
        L"dot",
        L"dash",
        L"longdash",
        L"dashdot",
        L"longdashdot",
        L"longdashdotdot"
	};

	typedef enum _TextFlow
	{
		lrTb = 0,
		tbRl = 1,
		btLr = 3,
		lrTbV = 4,
		tbRlV = 5,
	} TextFlow;

	static const wchar_t TextFlowMap[6][6] =
	{
        L"lrTb",
        L"tbRl",
        L"lrTb",//его на самом деле нету (2.9.323 TextFlow)
        L"btLr",
        L"lrTbV",
        L"tbRlV",
	};

	typedef enum _VerticalMergeFlag
	{
		fvmClear = 0,
		fvmMerge = 1,
		fvmRestart = 3
	} VerticalMergeFlag;

	typedef enum _VerticalAlign
	{ 
		top,
		center,
		bottom
	} VerticalAlign;

	static const wchar_t VerticalAlignMap[3][7] =
	{
        L"top",
        L"center",
        L"bottom"
	};

	typedef enum _CellWidthType
	{
		nil,
		Auto,
		pct,
		dxa
	} CellWidthType;

	static const wchar_t CellWidthTypeMap[4][5] =
	{
        L"nil",
        L"auto",
        L"pct",
        L"dxa"
	};

	typedef enum _BlipUsage
	{
		msoblipUsageDefault,
		msoblipUsageTexture,
		msoblipUsageMax = 255
	} BlipUsage;

	typedef enum _BlipType
	{
		msoblipERROR = 0, // An error occured during loading 
		msoblipUNKNOWN, // An unknown blip type 
		msoblipEMF, // Windows Enhanced Metafile 
		msoblipWMF, // Windows Metafile 
		msoblipPICT, // Macintosh PICT 
		msoblipJPEG, // JFIF 
		msoblipPNG, // PNG or GIF 
		msoblipDIB, // Windows DIB 
		msoblipTIFF = 17, // TIFF 
		msoblipCMYKJPEG = 18,// JPEG data in YCCK or CMYK color space 
		msoblipFirstClient = 32, // First client defined blip type 
		msoblipLastClient = 255 // Last client defined blip type 
	} BlipType;

	typedef enum _BlipSignature
	{
		msobiUNKNOWN = 0,
		msobiWMF = 0x216, // Metafile header then compressed WMF 
		msobiEMF = 0x3D4, // Metafile header then compressed EMF 
		msobiPICT = 0x542, // Metafile header then compressed PICT 
		msobiPNG = 0x6E0, // One unsigned char tag then PNG data 
		msobiJPEG = 0x46A,
		msobiJFIF = msobiJPEG, // One unsigned char tag then JFIF data 
		msobiDIB = 0x7A8, // One unsigned char tag then DIB data 
		msobiCMYKJPEG = 0x6E2, // One unsigned char tag then CMYK/YCCK JPEG data 
		msobiTIFF = 0x6e4, // One unsigned char tag then TIFF data 
		msobiClient = 0x800, // Clients should set this bit 
	} BlipSignature;

	typedef enum _BlipFilter
	{
		msofilterAdaptive = 0,
		msofilterNone = 254,
		msofilterTest = 255
	} BlipFilter;
}
