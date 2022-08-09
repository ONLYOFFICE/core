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
#include "../DesktopEditor/graphics/IRenderer.h"
#include "../DesktopEditor/graphics/pro/officedrawingfile.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"

#include "src/logic/elements/Paragraph.h"

#ifndef DOCXRENDERER_USE_DYNAMIC_LIBRARY
#define DOCXRENDERER_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define DOCXRENDERER_DECL_EXPORT Q_DECL_EXPORT
#endif

class CDocxRenderer_Private;
class DOCXRENDERER_DECL_EXPORT CDocxRenderer : public IRenderer
{
    public:
        CDocxRenderer(NSFonts::IApplicationFonts* pAppFonts);
        virtual ~CDocxRenderer();

        HRESULT      CreateNewFile(const std::wstring& wsPath, bool bIsOutCompress = true);
        HRESULT      Close();

        HRESULT      SetTempFolder(const std::wstring& wsPath);
        //----------------------------------------------------------------------------------------
        // Тип рендерера
        //----------------------------------------------------------------------------------------
        virtual HRESULT get_Type(LONG* lType);
        //----------------------------------------------------------------------------------------
        // Функции для работы со страницей
        //----------------------------------------------------------------------------------------
        virtual HRESULT NewPage();
        virtual HRESULT get_Height(double* dHeight);
        virtual HRESULT put_Height(const double& dHeight);
        virtual HRESULT get_Width(double* dWidth);
        virtual HRESULT put_Width(const double& dWidth);
        virtual HRESULT get_DpiX(double* dDpiX);
        virtual HRESULT get_DpiY(double* dDpiY);
        //----------------------------------------------------------------------------------------
        // Функции для работы с Pen
        //----------------------------------------------------------------------------------------
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
        //----------------------------------------------------------------------------------------
        // Функции для работы с Brush
        //----------------------------------------------------------------------------------------
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
        //----------------------------------------------------------------------------------------
        // Функции для работы со шрифтами
        //----------------------------------------------------------------------------------------
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
        //----------------------------------------------------------------------------------------
        // Функции для вывода текста
        //----------------------------------------------------------------------------------------
        virtual HRESULT CommandDrawTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
        virtual HRESULT CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
        virtual HRESULT CommandDrawText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
        virtual HRESULT CommandDrawTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
        //----------------------------------------------------------------------------------------
        // Маркеры команд
        //----------------------------------------------------------------------------------------
        virtual HRESULT BeginCommand(const DWORD& lType);
        virtual HRESULT EndCommand(const DWORD& lType);
        //----------------------------------------------------------------------------------------
        // Функции для работы с патом
        //----------------------------------------------------------------------------------------
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
        virtual HRESULT PathCommandTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
        virtual HRESULT PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
        virtual HRESULT PathCommandText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
        virtual HRESULT PathCommandTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
        //----------------------------------------------------------------------------------------
        // Функции для вывода изображений
        //----------------------------------------------------------------------------------------
        virtual HRESULT DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH);
        virtual HRESULT DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha = 255);
        //----------------------------------------------------------------------------------------
        // Функции для выставления преобразования
        //----------------------------------------------------------------------------------------
        virtual HRESULT SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY);
        virtual HRESULT GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY);
        virtual HRESULT ResetTransform();
        //----------------------------------------------------------------------------------------
        // Тип клипа
        //----------------------------------------------------------------------------------------
        virtual HRESULT get_ClipMode(LONG* lMode);
        virtual HRESULT put_ClipMode(const LONG& lMode);
        //----------------------------------------------------------------------------------------
        // Дополнительные функции
        //----------------------------------------------------------------------------------------
        virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand);
        virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand);
        virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand);

        // методы, которыми будет пользоваться конвертер
        HRESULT SetTextAssociationType(const NSDocxRenderer::TextAssociationType& eType);
        int Convert(IOfficeDrawingFile* pFile, const std::wstring& sDstFile, bool bIsOutCompress = true);

    private:
        CDocxRenderer_Private* m_pInternal;
};
