/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#ifndef _ASC_HTMLRENDERER3_TEXT_H_
#define _ASC_HTMLRENDERER3_TEXT_H_

#include "../../../../IRenderer.h"
#include "../../../officedrawingfile.h"

#ifndef HTMLRENDERER_USE_DYNAMIC_LIBRARY
#define HTMLRENDERER_DECL_EXPORT
#else
#include "../../DesktopEditor/common/base_export.h"
#define HTMLRENDERER_DECL_EXPORT Q_DECL_EXPORT
#endif

namespace NSHtmlRenderer
{
	class CHTMLRendererText_Private;
	class HTMLRENDERER_DECL_EXPORT CHTMLRendererText : public IRenderer
	{
	public:
		CHTMLRendererText();
		virtual ~CHTMLRendererText();

	public:
		void Init(IOfficeDrawingFile* pFile, int nCacheSize = 0);
		BYTE* GetBuffer();

	public:
		// renderer type-----------------------------------------------------------------------------
		virtual HRESULT get_Type(LONG* lType);
		//-------- Functions for working with page --------------------------------------------------
		virtual HRESULT NewPage();
		virtual HRESULT get_Height(double* dHeight);
		virtual HRESULT put_Height(const double& dHeight);
		virtual HRESULT get_Width(double* dWidth);
		virtual HRESULT put_Width(const double& dWidth);
		virtual HRESULT get_DpiX(double* dDpiX);
		virtual HRESULT get_DpiY(double* dDpiY);

		// pen --------------------------------------------------------------------------------------
		virtual HRESULT get_PenColor(LONG* lColor);
		virtual HRESULT put_PenColor(const LONG& lColor);
		virtual HRESULT get_PenAlpha(LONG* lAlpha);
		virtual HRESULT put_PenAlpha(const LONG& lAlpha);
		virtual HRESULT get_PenSize(double* dSize);
		virtual HRESULT put_PenSize(const double& dSize);
		virtual HRESULT get_PenDashStyle(BYTE* val);
		virtual HRESULT put_PenDashStyle(const BYTE& val);
		virtual HRESULT get_PenLineStartCap(BYTE* val);
		virtual HRESULT put_PenLineStartCap(const BYTE& val);
		virtual HRESULT get_PenLineEndCap(BYTE* val);
		virtual HRESULT put_PenLineEndCap(const BYTE& val);
		virtual HRESULT get_PenLineJoin(BYTE* val);
		virtual HRESULT put_PenLineJoin(const BYTE& val);
		virtual HRESULT get_PenDashOffset(double* dOffset);
		virtual HRESULT put_PenDashOffset(const double& dOffset);
		virtual HRESULT get_PenAlign(LONG* lAlign);
		virtual HRESULT put_PenAlign(const LONG& lAlign);
		virtual HRESULT get_PenMiterLimit(double* dOffset);
		virtual HRESULT put_PenMiterLimit(const double& dOffset);
		virtual HRESULT PenDashPattern(double* pPattern, LONG lCount);
		virtual HRESULT get_BrushTextureImage(Aggplus::CImage** pImage);
		virtual HRESULT put_BrushTextureImage(Aggplus::CImage* pImage);
		virtual HRESULT get_BrushTransform(Aggplus::CMatrix& oMatrix);
		virtual HRESULT put_BrushTransform(const Aggplus::CMatrix& oMatrix);

		// brush ------------------------------------------------------------------------------------
		virtual HRESULT get_BrushType(LONG* lType);
		virtual HRESULT put_BrushType(const LONG& lType);
		virtual HRESULT get_BrushColor1(LONG* lColor);
		virtual HRESULT put_BrushColor1(const LONG& lColor);
		virtual HRESULT get_BrushAlpha1(LONG* lAlpha);
		virtual HRESULT put_BrushAlpha1(const LONG& lAlpha);
		virtual HRESULT get_BrushColor2(LONG* lColor);
		virtual HRESULT put_BrushColor2(const LONG& lColor);
		virtual HRESULT get_BrushAlpha2(LONG* lAlpha);
		virtual HRESULT put_BrushAlpha2(const LONG& lAlpha);
		virtual HRESULT get_BrushTexturePath(std::wstring* bsPath);
		virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath);
		virtual HRESULT get_BrushTextureMode(LONG* lMode);
		virtual HRESULT put_BrushTextureMode(const LONG& lMode);
		virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha);
		virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha);
		virtual HRESULT get_BrushLinearAngle(double* dAngle);
		virtual HRESULT put_BrushLinearAngle(const double& dAngle);
		virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height);
		virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height);

		virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount);

		// font -------------------------------------------------------------------------------------
		virtual HRESULT get_FontName(std::wstring* bsName);
		virtual HRESULT put_FontName(const std::wstring& bsName);
		virtual HRESULT get_FontPath(std::wstring* bsName);
		virtual HRESULT put_FontPath(const std::wstring& bsName);
		virtual HRESULT get_FontSize(double* dSize);
		virtual HRESULT put_FontSize(const double& dSize);
		virtual HRESULT get_FontStyle(LONG* lStyle);
		virtual HRESULT put_FontStyle(const LONG& lStyle);
		virtual HRESULT get_FontStringGID(INT* bGID);
		virtual HRESULT put_FontStringGID(const INT& bGID);
		virtual HRESULT get_FontCharSpace(double* dSpace);
		virtual HRESULT put_FontCharSpace(const double& dSpace);
		virtual HRESULT get_FontFaceIndex(int* lFaceIndex);
		virtual HRESULT put_FontFaceIndex(const int& lFaceIndex);

		//-------- Functions for text output --------------------------------------------------------
		virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h);
		virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h);

		virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h);
		virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h);

		virtual HRESULT CommandDrawType3CHAR(const std::wstring& wsUnicodeText, const double& x, const double& y, const double& w, const double& h, const double& ascent, const double& descent, const double& unitsPerEm);

		//-------- Command markers ---------------------------------------------------------------
		virtual HRESULT BeginCommand(const DWORD& lType);
		virtual HRESULT EndCommand(const DWORD& lType);

		//-------- Functions for working with Graphics Path -----------------------------------------------
		virtual HRESULT PathCommandMoveTo(const double& x, const double& y);
		virtual HRESULT PathCommandLineTo(const double& x, const double& y);
		virtual HRESULT PathCommandLinesTo(double* points, const int& count);
		virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3);
		virtual HRESULT PathCommandCurvesTo(double* points, const int& count);
		virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle);
		virtual HRESULT PathCommandClose();
		virtual HRESULT PathCommandEnd();
		virtual HRESULT DrawPath(const LONG& nType);
		virtual HRESULT PathCommandStart();
		virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y);

		virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h);
		virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h);

		virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h);
		virtual HRESULT PathCommandTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h);

		//-------- Functions for image output ---------------------------------------------------
		virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h);
		virtual HRESULT DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255);

		// transform --------------------------------------------------------------------------------
		virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6);
		virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
		virtual HRESULT ResetTransform();

		// -----------------------------------------------------------------------------------------
		virtual HRESULT get_ClipMode(LONG* plMode);
		virtual HRESULT put_ClipMode(const LONG& lMode);

		// additiaonal params ----------------------------------------------------------------------
		virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand);
		virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand);
		virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand);

	protected:
		CHTMLRendererText_Private* m_pInternal;
	};
}

#endif // _ASC_HTMLRENDERER3_TEXT_H_
