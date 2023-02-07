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
#ifndef _BUILD_IRENDERER_H_
#define _BUILD_IRENDERER_H_

#pragma once

#include <string>
#include <vector>
#include "../common/IGrObject.h"
#include "structures.h"
#include "Matrix.h"

// тип в DrawPath
const long c_nStroke			= 0x0001;
const long c_nWindingFillMode   = 0x0100;
const long c_nEvenOddFillMode   = 0x0200;

// тип в BeginCommand, EndCommand
const long c_nNone              = 0x0000;
const long c_nPageType			= 0x0001;
const long c_nTextType			= 0x0002;
const long c_nImageType			= 0x0003;
const long c_nPathType			= 0x0004;
const long c_nNumberingMarker	= 0x0005;
const long c_nHyperlinkType		= 0x0008;
const long c_nWatermarkType     = 0x0010;
const long c_nClipType			= 0x0020;
const long c_nResetClipType		= 0x0040;
const long c_nTextGraphicType	= 0x0080;
const long c_nStrokeTextType    = 0x0100;
const long c_nSimpleGraphicType	= 0x0200;
const long c_nParagraphType		= 0x0400;
const long c_nComplexFigureType = 0x0800;
const long c_nPDFGradientType   = 0x1000;
const long c_nTableCell			= 0x2000;

const long c_nPDFTilingFill				= 0x2001;
const long c_nPDFTilingFillIteration	= 0x2002;

const long c_nHeader			= 0xa000;
const long c_nColumnCount		= 0xa001;
const long c_nFootnote			= 0xa002;
const long c_nPageBreak			= 0xa003;

const long c_nShape				= 0xa010;
const long c_nShapeGroup		= 0xa011;
const long c_nShapeWrap			= 0xa012;

const long c_nTable				= 0xa020;
const long c_nTableXml			= 0xa021;
const long c_nTableTop			= 0xa022;

const long c_nBlock					= 0xa030;
const long c_nBlockHeadingLevel		= 0xa031;
const long c_nBlockNumberingLevel	= 0xa032;
const long c_nBlockHorzAlign		= 0xa033;

const long c_nLine				= 0xa040;
const long c_nBaselineShift		= 0xa041;

// типы клипа
const long c_nClipRegionTypeWinding		= 0x0000;
const long c_nClipRegionTypeEvenOdd		= 0x0001;
// тип объединения клипов
const long c_nClipRegionIntersect		= 0x0000;
const long c_nClipRegionUnion			= 0x0100;
const long c_nClipRegionXor             = 0x0200;
const long c_nClipRegionDiff            = 0x0400;

// флаги в CommandDrawTextEx
const long c_nFlagNone			= 0x0000;
const long c_nFlagHyperlink		= 0x0001;

// флаги в CommandParams
const long c_nParamFlipX		= 0x0001;
const long c_nParamFlipY		= 0x0002;
const long c_nFlipNextRotate	= 0x0004;
const long c_nDarkMode          = 0x0008;
const long c_nUseDictionaryFonts = 0x0010;
const long c_nPenWidth0As1px	= 0x0020;

// типы рендерера
const long c_nUnknownRenderer   = 0x0000;
const long c_nPDFWriter         = 0x0001;
const long c_nHtmlRendrerer		= 0x0002;
const long c_nSVGRendrerer		= 0x0003;
const long c_nSWFRenderer       = 0x0004;
const long c_nSVGConverter      = 0x0005;
const long c_nHtmlRendrerer2	= 0x0006;
const long c_nEbookWriter		= 0x0007;
const long c_nDocxWriter		= 0x0008;
const long c_nVifFormat			= 0x0009;
const long c_nGrRenderer		= 0x0010;
const long c_nHtmlRendrerer3    = 0x0011;
const long c_nHtmlRendrererText = 0x0012;
const long c_nQRenderer         = 0x0013;

// типы команд
const long c_nCommandLongTypeOnlyText = 0x1000;

class IFormField
{
public:
	IFormField() {}
	virtual ~IFormField() {}
};

// IRenderer
class IRenderer : public IGrObject
{
public:
	bool m_bUseTransformCoordsToIdentity;

public:
	IRenderer()
	{
		m_bUseTransformCoordsToIdentity = false;
	}

public:
	// тип рендерера-----------------------------------------------------------------------------
	virtual HRESULT get_Type(LONG* lType)				= 0;
	//-------- Функции для работы со страницей --------------------------------------------------
	virtual HRESULT NewPage()							= 0;
	virtual HRESULT get_Height(double* dHeight)			= 0;
	virtual HRESULT put_Height(const double& dHeight)	= 0;
	virtual HRESULT get_Width(double* dWidth)			= 0;
	virtual HRESULT put_Width(const double& dWidth)		= 0;
	virtual HRESULT get_DpiX(double* dDpiX)				= 0;
	virtual HRESULT get_DpiY(double* dDpiY)				= 0;

	// pen --------------------------------------------------------------------------------------
	virtual HRESULT get_PenColor(LONG* lColor)					= 0;
	virtual HRESULT put_PenColor(const LONG& lColor)			= 0;
	virtual HRESULT get_PenAlpha(LONG* lAlpha)					= 0;
	virtual HRESULT put_PenAlpha(const LONG& lAlpha)			= 0;
	virtual HRESULT get_PenSize(double* dSize)					= 0;
	virtual HRESULT put_PenSize(const double& dSize)			= 0;
	virtual HRESULT get_PenDashStyle(BYTE* val)					= 0;
	virtual HRESULT put_PenDashStyle(const BYTE& val)			= 0;
	virtual HRESULT get_PenLineStartCap(BYTE* val)				= 0;
	virtual HRESULT put_PenLineStartCap(const BYTE& val)		= 0;
	virtual HRESULT get_PenLineEndCap(BYTE* val)				= 0;
	virtual HRESULT put_PenLineEndCap(const BYTE& val)			= 0;
	virtual HRESULT get_PenLineJoin(BYTE* val)					= 0;
	virtual HRESULT put_PenLineJoin(const BYTE& val)			= 0;
	virtual HRESULT get_PenDashOffset(double* dOffset)			= 0;
	virtual HRESULT put_PenDashOffset(const double& dOffset)	= 0;
	virtual HRESULT get_PenAlign(LONG* lAlign)					= 0;
	virtual HRESULT put_PenAlign(const LONG& lAlign)			= 0;
	virtual HRESULT get_PenMiterLimit(double* dOffset)			= 0;
	virtual HRESULT put_PenMiterLimit(const double& dOffset)	= 0;
	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount)= 0;

	// brush ------------------------------------------------------------------------------------
	virtual HRESULT get_BrushType(LONG* lType)							= 0;
	virtual HRESULT put_BrushType(const LONG& lType)					= 0;
	virtual HRESULT get_BrushColor1(LONG* lColor)						= 0;
	virtual HRESULT put_BrushColor1(const LONG& lColor)					= 0;
	virtual HRESULT get_BrushAlpha1(LONG* lAlpha)						= 0;
	virtual HRESULT put_BrushAlpha1(const LONG& lAlpha)					= 0;
	virtual HRESULT get_BrushColor2(LONG* lColor)						= 0;
	virtual HRESULT put_BrushColor2(const LONG& lColor)					= 0;
	virtual HRESULT get_BrushAlpha2(LONG* lAlpha)						= 0;
	virtual HRESULT put_BrushAlpha2(const LONG& lAlpha)					= 0;
	virtual HRESULT get_BrushTexturePath(std::wstring* bsPath)			= 0;
	virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath)	= 0;
	virtual HRESULT get_BrushTextureMode(LONG* lMode)					= 0;
	virtual HRESULT put_BrushTextureMode(const LONG& lMode)				= 0;
	virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)				= 0;
	virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha)			= 0;
	virtual HRESULT get_BrushLinearAngle(double* dAngle)				= 0;
	virtual HRESULT put_BrushLinearAngle(const double& dAngle)			= 0;
	virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height) = 0;
	virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height) = 0;

	virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount) = 0;
	//virtual void put_BrushGradInfo(const NSStructures::GradientInfo &_ginfo) {};

	// font -------------------------------------------------------------------------------------
	virtual HRESULT get_FontName(std::wstring* bsName)			= 0;
	virtual HRESULT put_FontName(const std::wstring& bsName)	= 0;
	virtual HRESULT get_FontPath(std::wstring* bsName)			= 0;
	virtual HRESULT put_FontPath(const std::wstring& bsName)	= 0;
	virtual HRESULT get_FontSize(double* dSize)					= 0;
	virtual HRESULT put_FontSize(const double& dSize)			= 0;
	virtual HRESULT get_FontStyle(LONG* lStyle)					= 0;
	virtual HRESULT put_FontStyle(const LONG& lStyle)			= 0;
	virtual HRESULT get_FontStringGID(INT* bGID)				= 0;
	virtual HRESULT put_FontStringGID(const INT& bGID)			= 0;
	virtual HRESULT get_FontCharSpace(double* dSpace)			= 0;
	virtual HRESULT put_FontCharSpace(const double& dSpace)		= 0;
	virtual HRESULT get_FontFaceIndex(int* lFaceIndex)			= 0;
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex)	= 0;

	//-------- Функции для вывода текста --------------------------------------------------------
	virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h) = 0;
	virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h) = 0;

	virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h) = 0;
	virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h) = 0;

	virtual HRESULT CommandDrawTextCHAR2(unsigned int* codepoints, const unsigned int& codepointscount, const unsigned int& gid, const double& x, const double& y, const double& w, const double& h)
	{
		LONG c = (NULL == codepoints) ? 32 : codepoints[0];
		return CommandDrawTextExCHAR(c, (LONG)gid, x, y, w, h);
	}

	//-------- Маркеры для команд ---------------------------------------------------------------
	virtual HRESULT BeginCommand(const DWORD& lType)	= 0;
	virtual HRESULT EndCommand(const DWORD& lType)		= 0;

	//-------- Функции для работы с Graphics Path -----------------------------------------------
	virtual HRESULT PathCommandMoveTo(const double& x, const double& y)			= 0;
	virtual HRESULT PathCommandLineTo(const double& x, const double& y)			= 0;
	virtual HRESULT PathCommandLinesTo(double* points, const int& count)		= 0;
	virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)	 = 0;
	virtual HRESULT PathCommandCurvesTo(double* points, const int& count)		= 0;
	virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle) = 0;
	virtual HRESULT PathCommandClose()											= 0;
	virtual HRESULT PathCommandEnd()											= 0;
	virtual HRESULT DrawPath(const LONG& nType)									= 0;
	virtual HRESULT PathCommandStart()											= 0;
	virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y)			= 0;

	virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h) = 0;
	virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h) = 0;

	virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h) = 0;
	virtual HRESULT PathCommandTextEx(const std::wstring& sText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h) = 0;

	//-------- Функции для вывода изображений ---------------------------------------------------
	virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)		= 0;
	virtual HRESULT DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255)	= 0;

	// transform --------------------------------------------------------------------------------
	virtual HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		return S_OK;
	}
	virtual HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
	{
		if ((dWidth <= 1) || (dHeight <= 1))
			lFlags = 0;

		INT bFlipX = (0 != (c_nParamFlipX & lFlags));
		INT bFlipY = (0 != (c_nParamFlipY & lFlags));

		float m11 = bFlipX ? -1.0f : 1.0f;
		float m22 = bFlipY ? -1.0f : 1.0f;

		Aggplus::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

		if ((0 != dAngle) || (0 != lFlags))
		{
			double dCentreX = (double)(dLeft + dWidth / 2.0);
			double dCentreY = (double)(dTop + dHeight / 2.0);

			oMatrix.Translate(-dCentreX, -dCentreY	, Aggplus::MatrixOrderAppend);

			oMatrix.Rotate(dAngle	, Aggplus::MatrixOrderAppend);
			oMatrix.Scale(m11, m22					, Aggplus::MatrixOrderAppend);

			oMatrix.Translate(dCentreX, dCentreY	, Aggplus::MatrixOrderAppend);
		}
		double mass[6];
		oMatrix.GetElements(mass);
		SetTransform(mass[0], mass[1], mass[2], mass[3], mass[4], mass[5]);
		return S_OK;
	}
	virtual HRESULT SetBaseTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6) { return S_OK; }
	virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6) = 0;
	virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)	= 0;
	virtual HRESULT ResetTransform() = 0;

	// -----------------------------------------------------------------------------------------
	virtual HRESULT get_ClipMode(LONG* plMode)			= 0;
	virtual HRESULT put_ClipMode(const LONG& lMode)		= 0;

	// additiaonal params ----------------------------------------------------------------------
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand)			= 0;
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand)		= 0;
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand)	= 0;

	virtual HRESULT StartConvertCoordsToIdentity()
	{
		m_bUseTransformCoordsToIdentity = true;
		return S_OK;
	}
	virtual HRESULT EndConvertCoordsToIdentity()
	{
		m_bUseTransformCoordsToIdentity = false;
		return S_OK;
	}
	virtual HRESULT AddHyperlink(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsUrl, const std::wstring& wsTooltip) {return S_OK;}
	virtual HRESULT AddLink(const double& dX, const double& dY, const double& dW, const double& dH, const double& dDestX, const double& dDestY, const int& nPage) {return S_OK;}
	virtual HRESULT AddFormField(IFormField* pField) {return S_OK;}
	virtual HRESULT DocInfo(const std::wstring& wsTitle, const std::wstring& wsCreator, const std::wstring& wsSubject, const std::wstring& wsKeywords) {return S_OK;}
};

#define PROPERTY_RENDERER(NameBase, Name, Type)			\
	STDMETHOD(get_##NameBase##Name)(Type* pVal)			\
{													\
	if (NULL == pVal)								\
	return S_FALSE;								\
	*pVal =	m_o##NameBase.##Name;					\
	return S_OK;									\
	}													\
	STDMETHOD(put_##NameBase##Name)(Type Val)			\
{													\
	m_o##NameBase.##Name = Val;						\
	return S_OK;									\
	}

// exapmle:
// PROPERTY_RENDERER(Pen, Color, LONG)
// PROPERTY_RENDERER(Pen, Alpha, LONG)

#endif // _BUILD_IRENDERER_H_
