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
#ifndef _PDF_WRITER_H
#define _PDF_WRITER_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/pro/Image.h"
#include "../../DesktopEditor/xmlsec/src/include/Certificate.h"
#include "SrcWriter/States.h"

#include <string>
#include <vector>
#include <math.h>

namespace PdfWriter
{
    class CDocument;
    class CPage;
    class CFontCidTrueType;
    class CImageDict;
    class CShading;
    class CExtGrState;
}

namespace Aggplus
{
    class CImage;
}

class CPdfWriter
{
public:
    CPdfWriter(NSFonts::IApplicationFonts* pAppFonts, bool isPDFA = false);
    ~CPdfWriter();
    int          SaveToFile(const std::wstring& wsPath);
    void         SetPassword(const std::wstring& wsPassword);
    void         SetDocumentID(const std::wstring& wsDocumentID);
    void         SetDocumentInfo(const std::wstring& wsTitle, const std::wstring& wsCreator, const std::wstring& wsSubject, const std::wstring& wsKeywords);
    std::wstring GetTempFile(const std::wstring& wsDirectory);
    //----------------------------------------------------------------------------------------
    // Функции для работы со страницей
    //----------------------------------------------------------------------------------------
    HRESULT NewPage();
    HRESULT get_Height(double* dHeight);
    HRESULT put_Height(const double& dHeight);
    HRESULT get_Width(double* dWidth);
    HRESULT put_Width(const double& dWidth);
    //----------------------------------------------------------------------------------------
    // Функции для работы с Pen
    //----------------------------------------------------------------------------------------
    HRESULT get_PenColor(LONG* lColor);
    HRESULT put_PenColor(const LONG& lColor);
    HRESULT get_PenAlpha(LONG* lAlpha);
    HRESULT put_PenAlpha(const LONG& lAlpha);
    HRESULT get_PenSize(double* dSize);
    HRESULT put_PenSize(const double& dSize);
    HRESULT get_PenDashStyle(BYTE* nDashStyle);
    HRESULT put_PenDashStyle(const BYTE& nDashStyle);
    HRESULT get_PenLineStartCap(BYTE* nCapStyle);
    HRESULT put_PenLineStartCap(const BYTE& nCapStyle);
    HRESULT get_PenLineEndCap(BYTE* nCapStyle);
    HRESULT put_PenLineEndCap(const BYTE& nCapStyle);
    HRESULT get_PenLineJoin(BYTE* nJoinStyle);
    HRESULT put_PenLineJoin(const BYTE& nJoinStyle);
    HRESULT get_PenDashOffset(double* dOffset);
    HRESULT put_PenDashOffset(const double& dOffset);
    HRESULT get_PenAlign(LONG* lAlign);
    HRESULT put_PenAlign(const LONG& lAlign);
    HRESULT get_PenMiterLimit(double* dMiter);
    HRESULT put_PenMiterLimit(const double& dMiter);
    HRESULT PenDashPattern(double* pPattern, LONG lCount);
    //----------------------------------------------------------------------------------------
    // Функции для работы с Brush
    //----------------------------------------------------------------------------------------
    HRESULT get_BrushType(LONG* lType);
    HRESULT put_BrushType(const LONG& lType);
    HRESULT get_BrushColor1(LONG* lColor);
    HRESULT put_BrushColor1(const LONG& lColor);
    HRESULT get_BrushAlpha1(LONG* lAlpha);
    HRESULT put_BrushAlpha1(const LONG& lAlpha);
    HRESULT get_BrushColor2(LONG* lColor);
    HRESULT put_BrushColor2(const LONG& lColor);
    HRESULT get_BrushAlpha2(LONG* lAlpha);
    HRESULT put_BrushAlpha2(const LONG& lAlpha);
    HRESULT get_BrushTexturePath(std::wstring* wsPath);
    HRESULT put_BrushTexturePath(const std::wstring& wsPath);
    HRESULT get_BrushTextureMode(LONG* lMode);
    HRESULT put_BrushTextureMode(const LONG& lMode);
    HRESULT get_BrushTextureAlpha(LONG* lAlpha);
    HRESULT put_BrushTextureAlpha(const LONG& lAlpha);
    HRESULT get_BrushLinearAngle(double* dAngle);
    HRESULT put_BrushLinearAngle(const double& dAngle);
    HRESULT BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight);
    HRESULT put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount);
    //----------------------------------------------------------------------------------------
    // Функции для работы со шрифтами
    //----------------------------------------------------------------------------------------
    HRESULT get_FontName(std::wstring* wsName);
    HRESULT put_FontName(const std::wstring& wsName);
    HRESULT get_FontPath(std::wstring* wsPath);
    HRESULT put_FontPath(const std::wstring& wsPath);
    HRESULT get_FontSize(double* dSize);
    HRESULT put_FontSize(const double& dSize);
    HRESULT get_FontStyle(LONG* lStyle);
    HRESULT put_FontStyle(const LONG& lStyle);
    HRESULT get_FontStringGID(INT* bGid);
    HRESULT put_FontStringGID(const INT& bGid);
    HRESULT get_FontCharSpace(double* dSpace);
    HRESULT put_FontCharSpace(const double& dSpace);
    HRESULT get_FontFaceIndex(int* lFaceIndex);
    HRESULT put_FontFaceIndex(const int& lFaceIndex);
    //----------------------------------------------------------------------------------------
    // Функции для вывода текста
    //----------------------------------------------------------------------------------------
    HRESULT CommandDrawTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
    HRESULT CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
    HRESULT CommandDrawText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
    HRESULT CommandDrawTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
    HRESULT CommandDrawTextCHAR2 (unsigned int* unUnicode, const unsigned int& unUnicodeCount, const unsigned int& unGid, const double& dX, const double& dY, const double& dW, const double& dH);
    //----------------------------------------------------------------------------------------
    // Маркеры команд
    //----------------------------------------------------------------------------------------
    HRESULT EndCommand(const DWORD& lType, const LONG& lClipMode);
    //----------------------------------------------------------------------------------------
    // Функции для работы с патом
    //----------------------------------------------------------------------------------------
    HRESULT PathCommandMoveTo(const double& dX, const double& dY);
    HRESULT PathCommandLineTo(const double& dX, const double& dY);
    HRESULT PathCommandLinesTo(double* pPoints, const int& nCount);
    HRESULT PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe);
    HRESULT PathCommandCurvesTo(double* pPoints, const int& nCount);
    HRESULT PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle);
    HRESULT PathCommandClose();
    HRESULT PathCommandEnd();
    HRESULT DrawPath(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory, const LONG& lType);
    HRESULT PathCommandStart();
    HRESULT PathCommandGetCurrentPoint(double* dX, double* dY);
    HRESULT PathCommandTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
    HRESULT PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
    HRESULT PathCommandText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
    HRESULT PathCommandTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
    //----------------------------------------------------------------------------------------
    // Функции для вывода изображений
    //----------------------------------------------------------------------------------------
    HRESULT DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH);
    HRESULT DrawImageFromFile(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory, const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha = 255);
    //----------------------------------------------------------------------------------------
    // Функции для выставления преобразования
    //----------------------------------------------------------------------------------------
    HRESULT SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY);
    HRESULT GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY);
    HRESULT ResetTransform();
    //----------------------------------------------------------------------------------------
    // Дополнительные функции
    //----------------------------------------------------------------------------------------
    HRESULT AddHyperlink(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsUrl, const std::wstring& wsTooltip);
    HRESULT AddLink(const double& dX, const double& dY, const double& dW, const double& dH, const double& dDestX, const double& dDestY, const int& nPage);
    HRESULT AddFormField(NSFonts::IApplicationFonts* pAppFonts, IFormField* pFiledInfo);
    //----------------------------------------------------------------------------------------
    // Дополнительные функции Pdf рендерера
    //----------------------------------------------------------------------------------------
    HRESULT DrawImage1bpp(NSImages::CPixJbig2* pImageBuffer, const unsigned int& unWidth, const unsigned int& unHeight, const double& dX, const double& dY, const double& dW, const double& dH);
    HRESULT EnableBrushRect(const LONG& lEnable);
    HRESULT SetLinearGradient(const double& dX1, const double& dY1, const double& dX2, const double& dY2);
    HRESULT SetRadialGradient(const double& dX1, const double& dY1, const double& dR1, const double& dX2, const double& dY2, const double& dR2);
    HRESULT DrawImageWith1bppMask(IGrObject* pImage, NSImages::CPixJbig2* pMaskBuffer, const unsigned int& unMaskWidth, const unsigned int& unMaskHeight, const double& dX, const double& dY, const double& dW, const double& dH);

    //----------------------------------------------------------------------------------------
    // Дополнительные функции для дозаписи Pdf
    //----------------------------------------------------------------------------------------
    bool EditPage(PdfWriter::CPage* pNewPage);
    bool AddPage(int nPageIndex);
    bool EditClose();
    void PageRotate(int nRotate);
    void Sign(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsPicturePath, ICertificate* pCertificate);


    PdfWriter::CDocument* m_pDocument;
    PdfWriter::CPage*     m_pPage;

private:
    PdfWriter::CImageDict* LoadImage(Aggplus::CImage* pImage, const BYTE& nAlpha);
    bool DrawImage(Aggplus::CImage* pImage, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha);
    bool DrawText(unsigned char* pCodes, const unsigned int& unLen, const double& dX, const double& dY);
    bool PathCommandDrawText(unsigned int* pUnicodes, unsigned int unLen, const double& dX, const double& dY, const unsigned int* pGids = NULL);
    void UpdateFont();
    void GetFontPath(const std::wstring& wsFontName, const bool& bBold, const bool& bItalic, std::wstring& wsFontPath, LONG& lFaceIndex);
    PdfWriter::CFontCidTrueType* GetFont(const std::wstring& wsFontPath, const LONG& lFontIndex);
    PdfWriter::CFontCidTrueType* GetFont(const std::wstring& wsFontName, const bool& bBold, const bool& bItalic);
    void UpdateTransform();
    void UpdatePen();
    void UpdateBrush(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory);
    void Reset();
    bool IsValid();
    bool IsPageValid();
    void SetError();
    void AddLink(PdfWriter::CPage* pPage, const double& dX, const double& dY, const double& dW, const double& dH, const double& dDestX, const double& dDestY, const unsigned int& unDestPage);
    unsigned char* EncodeString(const unsigned int* pUnicodes, const unsigned int& unUnicodesCount, const unsigned int* pGIDs = NULL);
    unsigned char* EncodeGID(const unsigned int& unGID, const unsigned int* pUnicodes, const unsigned int& unUnicodesCount);
    std::wstring GetDownloadFile(const std::wstring& sUrl, const std::wstring& wsTempDirectory);

private:
    NSFonts::IFontManager*       m_pFontManager;

    PdfWriter::CFontCidTrueType* m_pFont;
    PdfWriter::CShading*         m_pShading;
    PdfWriter::CExtGrState*      m_pShadingExtGrState;

    bool                         m_bNeedUpdateTextFont;
    bool                         m_bNeedUpdateTextColor;
    bool                         m_bNeedUpdateTextAlpha;
    bool                         m_bNeedUpdateTextCharSpace;
    bool                         m_bNeedUpdateTextSize;

    CCommandManager              m_oCommandManager;

    CPenState                    m_oPen;
    CBrushState                  m_oBrush;
    CFontState                   m_oFont;
    CPath                        m_oPath;
    CTransform                   m_oTransform;
    double                       m_dPageHeight;
    double                       m_dPageWidth;
    LONG                         m_lClipDepth;
    std::vector<TFontInfo>       m_vFonts;
    std::vector<TDestinationInfo>m_vDestinations;
    unsigned int                 m_unFieldsCounter;
    CMultiLineTextManager        m_oLinesManager;

    bool                         m_bValid;
};

#endif // _PDF_WRITER_H
