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

#include "src/logic/Document.h"

class CDocxRenderer_Private
{
public:
    NSDocxRenderer::CDocument m_oDocument;
    CApplicationFonts* m_pApplicationFonts;

    std::wstring m_strDstFilePath;
    std::wstring m_strTempFileDir;
    std::wstring m_strTempFileName;

public:
    CDocxRenderer_Private(CApplicationFonts* pFonts)
    {
        m_pApplicationFonts = pFonts;
    }
    ~CDocxRenderer_Private()
    {

    }
};

CDocxRenderer::CDocxRenderer(CApplicationFonts* pAppFonts)
{
    m_pInternal = new CDocxRenderer_Private(pAppFonts);
}

CDocxRenderer::~CDocxRenderer()
{
    RELEASEOBJECT(m_pInternal);
}

void CDocxRenderer::CreateFile(const std::wstring& wsPath)
{
    return S_OK;
}
void CDocxRenderer::Close()
{
    return S_OK;
}

void CDocxRenderer::SetTextAssociationType(const NSDocxRenderer::TextAssociationType& eType)
{
    return S_OK;
}

void CDocxRenderer::SetTempFolder(const std::wstring& wsPath)
{
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Тип рендерера
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::get_Type(LONG* lType)
{
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы со страницей
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::NewPage()
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_Height(double* dHeight)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_Height(const double& dHeight)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_Width(double* dWidth)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_Width(const double& dWidth)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_DpiX(double* dDpiX)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_DpiY(double* dDpiY)
{
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с Pen
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::get_PenColor(LONG* lColor)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_PenColor(const LONG& lColor)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_PenAlpha(LONG* lAlpha)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_PenAlpha(const LONG& lAlpha)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_PenSize(double* dSize)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_PenSize(const double& dSize)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_PenDashStyle(BYTE* nDashStyle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_PenDashStyle(const BYTE& nDashStyle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_PenLineStartCap(BYTE* nCapStyle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_PenLineStartCap(const BYTE& nCapStyle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_PenLineEndCap(BYTE* nCapStyle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_PenLineEndCap(const BYTE& nCapStyle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_PenLineJoin(BYTE* nJoinStyle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_PenLineJoin(const BYTE& nJoinStyle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_PenDashOffset(double* dOffset)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_PenDashOffset(const double& dOffset)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_PenAlign(LONG* lAlign)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_PenAlign(const LONG& lAlign)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_PenMiterLimit(double* dMiter)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_PenMiterLimit(const double& dMiter)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PenDashPattern(double* pPattern, LONG lCount)
{
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с Brush
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::get_BrushType(LONG* lType)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_BrushType(const LONG& lType)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_BrushColor1(LONG* lColor)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_BrushColor1(const LONG& lColor)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_BrushAlpha1(LONG* lAlpha)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_BrushAlpha1(const LONG& lAlpha)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_BrushColor2(LONG* lColor)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_BrushColor2(const LONG& lColor)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_BrushAlpha2(LONG* lAlpha)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_BrushAlpha2(const LONG& lAlpha)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_BrushTexturePath(std::wstring* wsPath)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_BrushTexturePath(const std::wstring& wsPath)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_BrushTextureMode(LONG* lMode)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_BrushTextureMode(const LONG& lMode)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_BrushTextureAlpha(LONG* lAlpha)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_BrushTextureAlpha(const LONG& lAlpha)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_BrushLinearAngle(double* dAngle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_BrushLinearAngle(const double& dAngle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::BrushBounds(const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount)
{
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы со шрифтами
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::get_FontName(std::wstring* wsName)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_FontName(const std::wstring& wsName)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_FontPath(std::wstring* wsPath)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_FontPath(const std::wstring& wsPath)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_FontSize(double* dSize)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_FontSize(const double& dSize)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_FontStyle(LONG* lStyle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_FontStyle(const LONG& lStyle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_FontStringGID(INT* bGid)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_FontStringGID(const INT& bGid)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_FontCharSpace(double* dSpace)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_FontCharSpace(const double& dSpace)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::get_FontFaceIndex(int* lFaceIndex)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_FontFaceIndex(const int& lFaceIndex)
{
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для вывода текста
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::CommandDrawTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::CommandDrawText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::CommandDrawTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Маркеры команд
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::BeginCommand(const DWORD& lType)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::EndCommand(const DWORD& lType)
{
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с патом
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::PathCommandMoveTo(const double& dX, const double& dY)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandLineTo(const double& dX, const double& dY)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandLinesTo(double* pPoints, const int& nCount)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandCurvesTo(double* pPoints, const int& nCount)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandClose()
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandEnd()
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::DrawPath(const LONG& lType)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandStart()
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandGetCurrentPoint(double* dX, double* dY)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::PathCommandTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для вывода изображений
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha = 255)
{
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для выставления преобразования
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::ResetTransform()
{
    return S_OK;
}

//----------------------------------------------------------------------------------------
// Тип клипа
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::get_ClipMode(LONG* lMode)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::put_ClipMode(const LONG& lMode)
{
    return S_OK;
}

//----------------------------------------------------------------------------------------
// Дополнительные функции
//----------------------------------------------------------------------------------------
virtual HRESULT CDocxRenderer::CommandLong(const LONG& lType, const LONG& lCommand)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::CommandDouble(const LONG& lType, const double& dCommand)
{
    return S_OK;
}
virtual HRESULT CDocxRenderer::CommandString(const LONG& lType, const std::wstring& sCommand)
{
    return S_OK;
}
