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
#pragma once
#include "../DesktopEditor/graphics/IRenderer.h"
#include "../DesktopEditor/graphics/pro/officedrawingfile.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"

#include "./convert_params.h"

#ifndef DOCXRENDERER_USE_DYNAMIC_LIBRARY
#define DOCXRENDERER_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define DOCXRENDERER_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "convert_params.h"
#include "src/logic/managers/ExternalImageStorage.h"
#include "../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"

class CDocxRenderer_Private;
class DOCXRENDERER_DECL_EXPORT CDocxRenderer : public IRenderer
{
public:
	CDocxRenderer(NSFonts::IApplicationFonts* pAppFonts);
	virtual ~CDocxRenderer();

	HRESULT      Compress();
	HRESULT      SetTempFolder(const std::wstring& wsPath);

	virtual HRESULT get_Type(LONG* lType);

	virtual HRESULT NewPage();
	virtual HRESULT get_Height(double* dHeight);
	virtual HRESULT put_Height(const double& dHeight);
	virtual HRESULT get_Width(double* dWidth);
	virtual HRESULT put_Width(const double& dWidth);
	virtual HRESULT get_DpiX(double* dDpiX);
	virtual HRESULT get_DpiY(double* dDpiY);

	virtual HRESULT get_PenColor(LONG* lColor);
	virtual HRESULT put_PenColor(const LONG& lColor);
	virtual HRESULT get_PenAlpha(LONG* lAlpha);
	virtual HRESULT put_PenAlpha(const LONG& lAlpha);
	virtual HRESULT get_PenSize(double* dSize);
	virtual HRESULT put_PenSize(const double& dSize);
	virtual HRESULT get_PenDashStyle(BYTE* nDashStyle);
	virtual HRESULT put_PenDashStyle(const BYTE& nDashStyle);
	virtual HRESULT get_PenLineStartCap(BYTE* nCapStyle);
	virtual HRESULT put_PenLineStartCap(const BYTE& nCapStyle);
	virtual HRESULT get_PenLineEndCap(BYTE* nCapStyle);
	virtual HRESULT put_PenLineEndCap(const BYTE& nCapStyle);
	virtual HRESULT get_PenLineJoin(BYTE* nJoinStyle);
	virtual HRESULT put_PenLineJoin(const BYTE& nJoinStyle);
	virtual HRESULT get_PenDashOffset(double* dOffset);
	virtual HRESULT put_PenDashOffset(const double& dOffset);
	virtual HRESULT get_PenAlign(LONG* lAlign);
	virtual HRESULT put_PenAlign(const LONG& lAlign);
	virtual HRESULT get_PenMiterLimit(double* dMiter);
	virtual HRESULT put_PenMiterLimit(const double& dMiter);
	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount);

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
	virtual HRESULT get_BrushTexturePath(std::wstring* wsPath);
	virtual HRESULT put_BrushTexturePath(const std::wstring& wsPath);
	virtual HRESULT get_BrushTextureMode(LONG* lMode);
	virtual HRESULT put_BrushTextureMode(const LONG& lMode);
	virtual HRESULT get_BrushTextureAlpha(LONG* lAlpha);
	virtual HRESULT put_BrushTextureAlpha(const LONG& lAlpha);
	virtual HRESULT get_BrushLinearAngle(double* dAngle);
	virtual HRESULT put_BrushLinearAngle(const double& dAngle);

	virtual HRESULT BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight);
	virtual HRESULT BrushBounds(const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight);

	virtual HRESULT put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount);
	virtual HRESULT get_BrushTextureImage(Aggplus::CImage** pImage);
	virtual HRESULT put_BrushTextureImage(Aggplus::CImage* pImage);
	virtual HRESULT get_BrushTransform(Aggplus::CMatrix& oMatrix);
	virtual HRESULT put_BrushTransform(const Aggplus::CMatrix& oMatrix);
	virtual void put_BrushGradInfo(void* pGradInfo) override;

	virtual HRESULT get_FontName(std::wstring* wsName);
	virtual HRESULT put_FontName(const std::wstring& wsName);
	virtual HRESULT get_FontPath(std::wstring* wsPath);
	virtual HRESULT put_FontPath(const std::wstring& wsPath);
	virtual HRESULT get_FontSize(double* dSize);
	virtual HRESULT put_FontSize(const double& dSize);
	virtual HRESULT get_FontStyle(LONG* lStyle);
	virtual HRESULT put_FontStyle(const LONG& lStyle);
	virtual HRESULT get_FontStringGID(INT* bGid);
	virtual HRESULT put_FontStringGID(const INT& bGid);
	virtual HRESULT get_FontCharSpace(double* dSpace);
	virtual HRESULT put_FontCharSpace(const double& dSpace);
	virtual HRESULT get_FontFaceIndex(int* lFaceIndex);
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex);

	virtual HRESULT CommandDrawTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT CommandDrawText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT CommandDrawTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);

	virtual HRESULT BeginCommand(const DWORD& lType);
	virtual HRESULT EndCommand(const DWORD& lType);

	virtual HRESULT PathCommandMoveTo(const double& dX, const double& dY);
	virtual HRESULT PathCommandLineTo(const double& dX, const double& dY);
	virtual HRESULT PathCommandLinesTo(double* pPoints, const int& nCount);
	virtual HRESULT PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe);
	virtual HRESULT PathCommandCurvesTo(double* pPoints, const int& nCount);
	virtual HRESULT PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle);
	virtual HRESULT PathCommandClose();
	virtual HRESULT PathCommandEnd();
	virtual HRESULT DrawPath(const LONG& lType);
	virtual HRESULT PathCommandStart();
	virtual HRESULT PathCommandGetCurrentPoint(double* dX, double* dY);
	virtual HRESULT PathCommandTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT PathCommandText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT PathCommandTextEx (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);

	virtual HRESULT DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha = 255);

	virtual HRESULT SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY);
	virtual HRESULT GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY);
	virtual HRESULT ResetTransform();

	virtual HRESULT get_ClipMode(LONG* lMode);
	virtual HRESULT put_ClipMode(const LONG& lMode);

	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand);
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand);
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand);

	virtual HRESULT IsSupportAdvancedCommand(const IAdvancedCommand::AdvancedCommandType& type);
	virtual HRESULT AdvancedCommand(IAdvancedCommand* command);

	// methods to be used by the converter
	HRESULT SetTextAssociationType(const NSDocxRenderer::TextAssociationType& eType);
	int Convert(IOfficeDrawingFile* pFile, const std::wstring& sDstFile, bool bIsOutCompress = true);
	std::vector<std::wstring> ScanPage(IOfficeDrawingFile* pFile, size_t nPage);
	std::vector<std::wstring> ScanPagePptx(IOfficeDrawingFile* pFile, size_t nPage);
	NSWasm::CData ScanPageBin(IOfficeDrawingFile* pFile, size_t nPage);

	void SetExternalImageStorage(NSDocxRenderer::IImageStorage* pStorage);
private:
	CDocxRenderer_Private* m_pInternal;
	void DrawPage(IOfficeDrawingFile* pFile, size_t nPage);
};
