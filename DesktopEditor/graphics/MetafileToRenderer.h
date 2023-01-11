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
#ifndef _BUILD_METAFILE_TO_IRENDERER_H_
#define _BUILD_METAFILE_TO_IRENDERER_H_

#pragma once

#include "pro/Image.h"

#ifndef INT32
typedef int INT32;
#endif

class GRAPHICS_DECL IMetafileToRenderter
{
public:
	IRenderer* m_pRenderer;
	void* m_pPicker;

protected:
	std::wstring m_sTempDir;
	std::vector<std::wstring> m_arTempFiles;

	std::wstring m_sThemesDir;
	std::wstring m_sMediaDir;
	std::wstring m_sInternalMediaDir;

public:
	IMetafileToRenderter(IRenderer* pRenderer);
	virtual ~IMetafileToRenderter();

public:
	virtual std::wstring GetImagePath(const std::wstring& sPath);
	virtual void SetLinearGradiant(const double& x0, const double& y0, const double& x1, const double& y1);
	virtual void SetRadialGradiant(const double& dX0, const double& dY0, const double& dR0, const double& dX1, const double& dY1, const double& dR1);
	virtual void InitPicker(const std::wstring& sFontsFolder);
	virtual void InitPicker(NSFonts::IApplicationFonts* pFonts);

	void SetTempDirectory(const std::wstring& sDir);
	std::wstring GetTempDirectory();

	void SetMediaDirectory(const std::wstring& sDir);
	std::wstring GetMediaDirectory();
	void SetInternalMediaDirectory(const std::wstring& sDir);
	std::wstring GetInternalMediaDirectory();
	void SetThemesDirectory(const std::wstring& sDir);
	std::wstring GetThemesDirectory();
};

namespace NSOnlineOfficeBinToPdf
{
	enum CommandType
	{
		ctPenXML						= 0,
		ctPenColor						= 1,
		ctPenAlpha						= 2,
		ctPenSize						= 3,
		ctPenDashStyle					= 4,
		ctPenLineStartCap				= 5,
		ctPenLineEndCap				    = 6,
		ctPenLineJoin					= 7,
		ctPenDashPatern				    = 8,
		ctPenDashPatternCount			= 9,
		ctPenDashOffset				    = 10,
		ctPenAlign						= 11,
		ctPenMiterLimit				    = 12,

		// brush
		ctBrushXML						= 20,
		ctBrushType					    = 21,
		ctBrushColor1					= 22,
		ctBrushColor2					= 23,
		ctBrushAlpha1					= 24,
		ctBrushAlpha2					= 25,
		ctBrushTexturePathOld			= 26, // для совместимости
		ctBrushTextureAlpha			    = 27,
		ctBrushTextureMode				= 28,
		ctBrushRectable				    = 29,
		ctBrushRectableEnabled 		    = 30,
		ctBrushGradient                 = 31,
		ctBrushTexturePath              = 32,

		// font
		ctFontXML						= 40,
		ctFontName						= 41,
		ctFontSize						= 42,
		ctFontStyle					    = 43,
		ctFontPath						= 44,
		ctFontGID						= 45,
		ctFontCharSpace				    = 46,

		// shadow
		ctShadowXML					    = 50,
		ctShadowVisible				    = 51,
		ctShadowDistanceX				= 52,
		ctShadowDistanceY				= 53,
		ctShadowBlurSize				= 54,
		ctShadowColor					= 55,
		ctShadowAlpha					= 56,

		// edge
		ctEdgeXML						= 70,
		ctEdgeVisible					= 71,
		ctEdgeDistance					= 72,
		ctEdgeColor					    = 73,
		ctEdgeAlpha					    = 74,

		// text
		ctDrawText						= 80,
		ctDrawTextEx					= 81,
		ctDrawTextCodeGid				= 83,

		// pathcommands
		ctPathCommandMoveTo			    = 91,
		ctPathCommandLineTo			    = 92,
		ctPathCommandLinesTo			= 93,
		ctPathCommandCurveTo			= 94,
		ctPathCommandCurvesTo			= 95,
		ctPathCommandArcTo		        = 96,
		ctPathCommandClose				= 97,
		ctPathCommandEnd				= 98,
		ctDrawPath						= 99,
		ctPathCommandStart				= 100,
		ctPathCommandGetCurrentPoint	= 101,
		ctPathCommandText				= 102,
		ctPathCommandTextEx			    = 103,

		// image
		ctDrawImage					    = 110,
		ctDrawImageFromFile			    = 111,

		ctSetParams					    = 120,

		ctBeginCommand					= 121,
		ctEndCommand					= 122,

		ctSetTransform					= 130,
		ctResetTransform				= 131,

		ctClipMode						= 140,

		ctCommandLong1					= 150,
		ctCommandDouble1				= 151,
		ctCommandString1				= 152,
		ctCommandLong2					= 153,
		ctCommandDouble2				= 154,
		ctCommandString2				= 155,

		ctHyperlink                     = 160,
		ctLink                          = 161,
		ctFormField                     = 162,
		ctDocInfo                       = 163,

		ctPageWidth                     = 200,
		ctPageHeight                    = 201,

		ctPageStart                     = 202,
		ctPageEnd                       = 203,

		// gradients

		ctGradientFill					= 220,
		ctGradientFillXML				= 221,

		ctGradientStroke				= 222,
		ctGradientStrokeXML				= 223,

		ctError						    = 255
	};

	bool GRAPHICS_DECL ConvertBufferToRenderer(BYTE* pBuffer, LONG lBufferLen, IMetafileToRenderter* pCorrector);
}

#endif // _BUILD_METAFILE_TO_IRENDERER_H_
