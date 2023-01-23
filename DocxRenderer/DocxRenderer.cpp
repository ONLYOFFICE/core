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

#include "DocxRenderer.h"
#include "../DesktopEditor/common/Directory.h"
#include "../OfficeUtils/src/OfficeUtils.h"
#include "src/logic/Document.h"

class CDocxRenderer_Private
{
    public:
        NSDocxRenderer::CDocument m_oDocument;
        std::wstring m_sTempDirectory;

    public:
        CDocxRenderer_Private(NSFonts::IApplicationFonts* pFonts, IRenderer* pRenderer) : m_oDocument(pRenderer, pFonts)
        {
        }
        ~CDocxRenderer_Private()
        {

        }
};

CDocxRenderer::CDocxRenderer(NSFonts::IApplicationFonts* pAppFonts)
{
    m_pInternal = new CDocxRenderer_Private(pAppFonts, this);
}

CDocxRenderer::~CDocxRenderer()
{
    RELEASEOBJECT(m_pInternal);
}

HRESULT CDocxRenderer::CreateNewFile(const std::wstring& wsPath, bool bIsOutCompress)
{
    m_pInternal->m_oDocument.m_strDstFilePath = wsPath;
    m_pInternal->m_oDocument.m_strTempDirectory = bIsOutCompress ?
                NSDirectory::CreateDirectoryWithUniqueName(m_pInternal->m_sTempDirectory) :
                m_pInternal->m_sTempDirectory;
    m_pInternal->m_oDocument.CreateDocument();
    return S_OK;
}
HRESULT CDocxRenderer::Close()
{
    COfficeUtils oCOfficeUtils(nullptr);
    HRESULT hr = oCOfficeUtils.CompressFileOrDirectory(m_pInternal->m_oDocument.m_strTempDirectory, m_pInternal->m_oDocument.m_strDstFilePath, true);
    if (!m_pInternal->m_oDocument.m_strTempDirectory.empty())
        NSDirectory::DeleteDirectory(m_pInternal->m_oDocument.m_strTempDirectory);
    m_pInternal->m_oDocument.m_strTempDirectory = L"";
    return hr;
}

HRESULT CDocxRenderer::SetTextAssociationType(const NSDocxRenderer::TextAssociationType& eType)
{
    m_pInternal->m_oDocument.m_oCurrentPage.m_eTextAssociationType = eType;
    return S_OK;
}

int CDocxRenderer::Convert(IOfficeDrawingFile* pFile, const std::wstring& sDstFile, bool bIsOutCompress)
{
    CreateNewFile(sDstFile, bIsOutCompress);

    if (odftPDF == pFile->GetType())
        m_pInternal->m_oDocument.m_bIsNeedPDFTextAnalyzer = true;

    int nPagesCount = pFile->GetPagesCount();
    for (int i = 0; i < nPagesCount; ++i)
    {
        //std::cout << "Page " << i + 1 << "/" << nPagesCount << std::endl;
        NewPage();
        BeginCommand(c_nPageType);
        m_pInternal->m_oDocument.m_bIsDisablePageCommand = true;

        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        pFile->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

        dWidth  *= 25.4 / dPageDpiX;
        dHeight *= 25.4 / dPageDpiY;

        put_Width(dWidth);
        put_Height(dHeight);

        pFile->DrawPageOnRenderer(this, i, nullptr);

        m_pInternal->m_oDocument.m_bIsDisablePageCommand = false;
        EndCommand(c_nPageType);
    }

    HRESULT hr = S_OK;
    m_pInternal->m_oDocument.Close();
    if (bIsOutCompress)
        hr = Close();
    return (hr == S_OK) ? 0 : 1;
}

HRESULT CDocxRenderer::SetTempFolder(const std::wstring& wsPath)
{
    m_pInternal->m_sTempDirectory = wsPath;
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Тип рендерера
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::get_Type(LONG* lType)
{
    *lType = c_nDocxWriter;
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы со страницей
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::NewPage()
{
    return m_pInternal->m_oDocument.NewPage();
}
HRESULT CDocxRenderer::get_Height(double* dHeight)
{
    return m_pInternal->m_oDocument.get_Height(dHeight);
}
HRESULT CDocxRenderer::put_Height(const double& dHeight)
{
    return m_pInternal->m_oDocument.put_Height(dHeight);
}
HRESULT CDocxRenderer::get_Width(double* dWidth)
{
    return m_pInternal->m_oDocument.get_Width(dWidth);
}
HRESULT CDocxRenderer::put_Width(const double& dWidth)
{
    return m_pInternal->m_oDocument.put_Width(dWidth);
}
HRESULT CDocxRenderer::get_DpiX(double* dDpiX)
{
    return m_pInternal->m_oDocument.get_DpiX(dDpiX);
}
HRESULT CDocxRenderer::get_DpiY(double* dDpiY)
{
    return m_pInternal->m_oDocument.get_DpiY(dDpiY);
}
//----------------------------------------------------------------------------------------
// Функции для работы с Pen
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::get_PenColor(LONG* lColor)
{
    return m_pInternal->m_oDocument.get_PenColor(lColor);
}
HRESULT CDocxRenderer::put_PenColor(const LONG& lColor)
{
    return m_pInternal->m_oDocument.put_PenColor(lColor);
}
HRESULT CDocxRenderer::get_PenAlpha(LONG* lAlpha)
{
    return m_pInternal->m_oDocument.get_PenAlpha(lAlpha);
}
HRESULT CDocxRenderer::put_PenAlpha(const LONG& lAlpha)
{
    return m_pInternal->m_oDocument.put_PenAlpha(lAlpha);
}
HRESULT CDocxRenderer::get_PenSize(double* dSize)
{
    return m_pInternal->m_oDocument.get_PenSize(dSize);
}
HRESULT CDocxRenderer::put_PenSize(const double& dSize)
{
    return m_pInternal->m_oDocument.put_PenSize(dSize);
}
HRESULT CDocxRenderer::get_PenDashStyle(BYTE* nDashStyle)
{
    return m_pInternal->m_oDocument.get_PenDashStyle(nDashStyle);
}
HRESULT CDocxRenderer::put_PenDashStyle(const BYTE& nDashStyle)
{
    return m_pInternal->m_oDocument.put_PenDashStyle(nDashStyle);
}
HRESULT CDocxRenderer::get_PenLineStartCap(BYTE* nCapStyle)
{
    return m_pInternal->m_oDocument.get_PenLineStartCap(nCapStyle);
}
HRESULT CDocxRenderer::put_PenLineStartCap(const BYTE& nCapStyle)
{
    return m_pInternal->m_oDocument.put_PenLineStartCap(nCapStyle);
}
HRESULT CDocxRenderer::get_PenLineEndCap(BYTE* nCapStyle)
{
    return m_pInternal->m_oDocument.get_PenLineEndCap(nCapStyle);
}
HRESULT CDocxRenderer::put_PenLineEndCap(const BYTE& nCapStyle)
{
    return m_pInternal->m_oDocument.put_PenLineEndCap(nCapStyle);
}
HRESULT CDocxRenderer::get_PenLineJoin(BYTE* nJoinStyle)
{
    return m_pInternal->m_oDocument.get_PenLineJoin(nJoinStyle);
}
HRESULT CDocxRenderer::put_PenLineJoin(const BYTE& nJoinStyle)
{
    return m_pInternal->m_oDocument.put_PenLineJoin(nJoinStyle);
}
HRESULT CDocxRenderer::get_PenDashOffset(double* dOffset)
{
    return m_pInternal->m_oDocument.get_PenDashOffset(dOffset);
}
HRESULT CDocxRenderer::put_PenDashOffset(const double& dOffset)
{
    return m_pInternal->m_oDocument.put_PenDashOffset(dOffset);
}
HRESULT CDocxRenderer::get_PenAlign(LONG* lAlign)
{
    return m_pInternal->m_oDocument.get_PenAlign(lAlign);
}
HRESULT CDocxRenderer::put_PenAlign(const LONG& lAlign)
{
    return m_pInternal->m_oDocument.put_PenAlign(lAlign);
}
HRESULT CDocxRenderer::get_PenMiterLimit(double* dMiter)
{
    return m_pInternal->m_oDocument.get_PenMiterLimit(dMiter);
}
HRESULT CDocxRenderer::put_PenMiterLimit(const double& dMiter)
{
    return m_pInternal->m_oDocument.put_PenMiterLimit(dMiter);
}
HRESULT CDocxRenderer::PenDashPattern(double* pPattern, LONG lCount)
{
    return m_pInternal->m_oDocument.PenDashPattern(pPattern, lCount);
}
//----------------------------------------------------------------------------------------
// Функции для работы с Brush
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::get_BrushType(LONG* lType)
{
    return m_pInternal->m_oDocument.get_BrushType(lType);
}
HRESULT CDocxRenderer::put_BrushType(const LONG& lType)
{
    return m_pInternal->m_oDocument.put_BrushType(lType);
}
HRESULT CDocxRenderer::get_BrushColor1(LONG* lColor)
{
    return m_pInternal->m_oDocument.get_BrushColor1(lColor);
}
HRESULT CDocxRenderer::put_BrushColor1(const LONG& lColor)
{
    return m_pInternal->m_oDocument.put_BrushColor1(lColor);
}
HRESULT CDocxRenderer::get_BrushAlpha1(LONG* lAlpha)
{
    return m_pInternal->m_oDocument.get_BrushAlpha1(lAlpha);
}
HRESULT CDocxRenderer::put_BrushAlpha1(const LONG& lAlpha)
{
    return m_pInternal->m_oDocument.put_BrushAlpha1(lAlpha);
}
HRESULT CDocxRenderer::get_BrushColor2(LONG* lColor)
{
    return m_pInternal->m_oDocument.get_BrushColor2(lColor);
}
HRESULT CDocxRenderer::put_BrushColor2(const LONG& lColor)
{
    return m_pInternal->m_oDocument.put_BrushColor2(lColor);
}
HRESULT CDocxRenderer::get_BrushAlpha2(LONG* lAlpha)
{
    return m_pInternal->m_oDocument.get_BrushAlpha2(lAlpha);
}
HRESULT CDocxRenderer::put_BrushAlpha2(const LONG& lAlpha)
{
    return m_pInternal->m_oDocument.put_BrushAlpha2(lAlpha);
}
HRESULT CDocxRenderer::get_BrushTexturePath(std::wstring* wsPath)
{
    return m_pInternal->m_oDocument.get_BrushTexturePath(wsPath);
}
HRESULT CDocxRenderer::put_BrushTexturePath(const std::wstring& wsPath)
{
    return m_pInternal->m_oDocument.put_BrushTexturePath(wsPath);
}
HRESULT CDocxRenderer::get_BrushTextureMode(LONG* lMode)
{
    return m_pInternal->m_oDocument.get_BrushTextureMode(lMode);
}
HRESULT CDocxRenderer::put_BrushTextureMode(const LONG& lMode)
{
    return m_pInternal->m_oDocument.put_BrushTextureMode(lMode);
}
HRESULT CDocxRenderer::get_BrushTextureAlpha(LONG* lAlpha)
{
    return m_pInternal->m_oDocument.get_BrushTextureAlpha(lAlpha);
}
HRESULT CDocxRenderer::put_BrushTextureAlpha(const LONG& lAlpha)
{
    return m_pInternal->m_oDocument.put_BrushTextureAlpha(lAlpha);
}
HRESULT CDocxRenderer::get_BrushLinearAngle(double* dAngle)
{
    return m_pInternal->m_oDocument.get_BrushLinearAngle(dAngle);
}
HRESULT CDocxRenderer::put_BrushLinearAngle(const double& dAngle)
{
    return m_pInternal->m_oDocument.put_BrushLinearAngle(dAngle);
}
HRESULT CDocxRenderer::BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
    return m_pInternal->m_oDocument.BrushRect(nVal, dLeft, dTop, dWidth, dHeight);
}
HRESULT CDocxRenderer::BrushBounds(const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
    // TODO:
    return S_OK;
}
HRESULT CDocxRenderer::put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount)
{
    // TODO:
    return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы со шрифтами
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::get_FontName(std::wstring* wsName)
{
    return m_pInternal->m_oDocument.get_FontName(wsName);
}
HRESULT CDocxRenderer::put_FontName(const std::wstring& wsName)
{
    return m_pInternal->m_oDocument.put_FontName(wsName);
}
HRESULT CDocxRenderer::get_FontPath(std::wstring* wsPath)
{
    return m_pInternal->m_oDocument.get_FontPath(wsPath);
}
HRESULT CDocxRenderer::put_FontPath(const std::wstring& wsPath)
{
    return m_pInternal->m_oDocument.put_FontPath(wsPath);
}
HRESULT CDocxRenderer::get_FontSize(double* dSize)
{
    return m_pInternal->m_oDocument.get_FontSize(dSize);
}
HRESULT CDocxRenderer::put_FontSize(const double& dSize)
{
    return m_pInternal->m_oDocument.put_FontSize(dSize);
}
HRESULT CDocxRenderer::get_FontStyle(LONG* lStyle)
{
    return m_pInternal->m_oDocument.get_FontStyle(lStyle);
}
HRESULT CDocxRenderer::put_FontStyle(const LONG& lStyle)
{
    return m_pInternal->m_oDocument.put_FontStyle(lStyle);
}
HRESULT CDocxRenderer::get_FontStringGID(INT* bGid)
{
    return m_pInternal->m_oDocument.get_FontStringGID(bGid);
}
HRESULT CDocxRenderer::put_FontStringGID(const INT& bGid)
{
    return m_pInternal->m_oDocument.put_FontStringGID(bGid);
}
HRESULT CDocxRenderer::get_FontCharSpace(double* dSpace)
{
    return m_pInternal->m_oDocument.get_FontCharSpace(dSpace);
}
HRESULT CDocxRenderer::put_FontCharSpace(const double& dSpace)
{
    return m_pInternal->m_oDocument.put_FontCharSpace(dSpace);
}
HRESULT CDocxRenderer::get_FontFaceIndex(int* lFaceIndex)
{
    return m_pInternal->m_oDocument.get_FontFaceIndex(lFaceIndex);
}
HRESULT CDocxRenderer::put_FontFaceIndex(const int& lFaceIndex)
{
    return m_pInternal->m_oDocument.put_FontFaceIndex(lFaceIndex);
}
//----------------------------------------------------------------------------------------
// Функции для вывода текста
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::CommandDrawTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return m_pInternal->m_oDocument.CommandDrawTextCHAR((int)lUnicode, dX, dY, dW, dH);
}
HRESULT CDocxRenderer::CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return m_pInternal->m_oDocument.CommandDrawTextExCHAR((int)lUnicode, (int)lGid, dX, dY, dW, dH);
}
HRESULT CDocxRenderer::CommandDrawText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return m_pInternal->m_oDocument.CommandDrawText(wsUnicodeText, dX, dY, dW, dH);
}
HRESULT CDocxRenderer::CommandDrawTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return m_pInternal->m_oDocument.CommandDrawTextEx(wsUnicodeText, pGids, nGidsCount, dX, dY, dW, dH);
}
//----------------------------------------------------------------------------------------
// Маркеры команд
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::BeginCommand(const DWORD& lType)
{
    return m_pInternal->m_oDocument.BeginCommand(lType);
}
HRESULT CDocxRenderer::EndCommand(const DWORD& lType)
{
    return m_pInternal->m_oDocument.EndCommand(lType);
}
//----------------------------------------------------------------------------------------
// Функции для работы с патом
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::PathCommandMoveTo(const double& dX, const double& dY)
{
    return m_pInternal->m_oDocument.PathCommandMoveTo(dX, dY);
}
HRESULT CDocxRenderer::PathCommandLineTo(const double& dX, const double& dY)
{
    return m_pInternal->m_oDocument.PathCommandLineTo(dX, dY);
}
HRESULT CDocxRenderer::PathCommandLinesTo(double* pPoints, const int& nCount)
{
    return m_pInternal->m_oDocument.PathCommandLinesTo(pPoints, nCount);
}
HRESULT CDocxRenderer::PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe)
{
    return m_pInternal->m_oDocument.PathCommandCurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
}
HRESULT CDocxRenderer::PathCommandCurvesTo(double* pPoints, const int& nCount)
{
    return m_pInternal->m_oDocument.PathCommandCurvesTo(pPoints, nCount);
}
HRESULT CDocxRenderer::PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle)
{
    return m_pInternal->m_oDocument.PathCommandArcTo(dX, dY, dW, dH, dStartAngle, dSweepAngle);
}
HRESULT CDocxRenderer::PathCommandClose()
{
    return m_pInternal->m_oDocument.PathCommandClose();
}
HRESULT CDocxRenderer::PathCommandEnd()
{
    return m_pInternal->m_oDocument.PathCommandEnd();
}
HRESULT CDocxRenderer::DrawPath(const LONG& lType)
{
    return m_pInternal->m_oDocument.DrawPath(lType);
}
HRESULT CDocxRenderer::PathCommandStart()
{
    return m_pInternal->m_oDocument.PathCommandStart();
}
HRESULT CDocxRenderer::PathCommandGetCurrentPoint(double* dX, double* dY)
{
    return m_pInternal->m_oDocument.PathCommandGetCurrentPoint(dX, dY);
}
HRESULT CDocxRenderer::PathCommandTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return m_pInternal->m_oDocument.PathCommandTextCHAR((int)lUnicode, dX, dY, dW, dH);
}
HRESULT CDocxRenderer::PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return m_pInternal->m_oDocument.PathCommandTextExCHAR((int)lUnicode, (int)lGid, dX, dY, dW, dH);
}
HRESULT CDocxRenderer::PathCommandText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return m_pInternal->m_oDocument.PathCommandText(wsUnicodeText, dX, dY, dW, dH);
}
HRESULT CDocxRenderer::PathCommandTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return m_pInternal->m_oDocument.PathCommandTextEx(wsUnicodeText, pGids, nGidsCount, dX, dY, dW, dH);
}
//----------------------------------------------------------------------------------------
// Функции для вывода изображений
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH)
{
    return m_pInternal->m_oDocument.DrawImage(pImage, dX, dY, dW, dH);
}
HRESULT CDocxRenderer::DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha)
{
    return m_pInternal->m_oDocument.DrawImageFromFile(wsImagePath,dX, dY, dW, dH);
}
//----------------------------------------------------------------------------------------
// Функции для выставления преобразования
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY)
{
    return m_pInternal->m_oDocument.SetTransform(dM11, dM12, dM21, dM22, dX, dY);
}
HRESULT CDocxRenderer::GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY)
{
    return m_pInternal->m_oDocument.GetTransform(dM11, dM12, dM21, dM22, dX, dY);
}
HRESULT CDocxRenderer::ResetTransform()
{
    return m_pInternal->m_oDocument.ResetTransform();
}

//----------------------------------------------------------------------------------------
// Тип клипа
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::get_ClipMode(LONG* lMode)
{
    return m_pInternal->m_oDocument.get_ClipMode(lMode);
}
HRESULT CDocxRenderer::put_ClipMode(const LONG& lMode)
{
    return m_pInternal->m_oDocument.put_ClipMode(lMode);
}

//----------------------------------------------------------------------------------------
// Дополнительные функции
//----------------------------------------------------------------------------------------
HRESULT CDocxRenderer::CommandLong(const LONG& lType, const LONG& lCommand)
{
    return S_OK;
}
HRESULT CDocxRenderer::CommandDouble(const LONG& lType, const double& dCommand)
{
    return S_OK;
}
HRESULT CDocxRenderer::CommandString(const LONG& lType, const std::wstring& sCommand)
{
    return S_OK;
}
