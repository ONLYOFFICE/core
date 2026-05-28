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
#ifndef _PDF_WRITER_H
#define _PDF_WRITER_H

#include <string>
#include <vector>
#include <math.h>

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/pro/Image.h"
#include "../../DesktopEditor/graphics/commands/FormField.h"
#include "../../DesktopEditor/graphics/commands/DocInfo.h"
#include "../../DesktopEditor/graphics/commands/AnnotField.h"
#include "../../DesktopEditor/xmlsec/src/include/Certificate.h"
#include "SrcWriter/States.h"
#include "SrcWriter/Annotation.h"

namespace PdfWriter
{
	class CDocument;
	class CPage;
	class CFontCidTrueType;
	class CFont14;
	class CFontEmbedded;
	class CImageDict;
	class CShading;
	class CExtGrState;
	class RedactOutputDev;
}

namespace Aggplus
{
	class CImage;
}

class CPdfWriter
{
public:
	CPdfWriter(NSFonts::IApplicationFonts* pAppFonts, bool isPDFA = false, IRenderer* pRenderer = NULL, bool bCreate = true, const std::wstring& wsTempDirectory = L"");
	~CPdfWriter();
	int          SaveToFile(const std::wstring& wsPath);
	int          SaveToMemory(BYTE** pData, int* pLength);
	void         SetPassword(const std::wstring& wsPassword);
	void         SetDocumentID(const std::wstring& wsDocumentID);
	void         SetDocumentInfo(const std::wstring& wsTitle, const std::wstring& wsCreator, const std::wstring& wsSubject, const std::wstring& wsKeywords);
	std::wstring GetTempFile(const std::wstring& wsDirectory);
	void SetTempDirectory(const std::wstring& wsTempDirectory);
	//----------------------------------------------------------------------------------------
	// Functions for working with page
	//----------------------------------------------------------------------------------------
	HRESULT NewPage();
	HRESULT get_Height(double* dHeight);
	HRESULT put_Height(const double& dHeight, bool bMM2PT = true);
	HRESULT get_Width(double* dWidth);
	HRESULT put_Width(const double& dWidth, bool bMM2PT = true);
	//----------------------------------------------------------------------------------------
	// Functions for working with Pen
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
	// Functions for working with Brush
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
	HRESULT get_BrushTextureImage(Aggplus::CImage** pImage);
	HRESULT put_BrushTextureImage(Aggplus::CImage* pImage);
	HRESULT get_BrushTransform(Aggplus::CMatrix& oMatrix);
	HRESULT put_BrushTransform(const Aggplus::CMatrix& oMatrix);
	HRESULT get_BrushOffset(double& offsetX, double& offsetY) const;
	HRESULT put_BrushOffset(const double& offsetX, const double& offsetY);
	HRESULT get_BrushScale(bool& isScale, double& scaleX, double& scaleY) const;
	HRESULT put_BrushScale(bool isScale, const double& scaleX, const double& scaleY);
	//----------------------------------------------------------------------------------------
	// Functions for working with fonts
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
	// Functions for text output
	//----------------------------------------------------------------------------------------
	HRESULT CommandDrawTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT CommandDrawText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT CommandDrawTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT CommandDrawTextCHAR2 (unsigned int* unUnicode, const unsigned int& unUnicodeCount, const unsigned int& unGid, const double& dX, const double& dY, const double& dW, const double& dH);
	//----------------------------------------------------------------------------------------
	// Command markers
	//----------------------------------------------------------------------------------------
	HRESULT EndCommand(const DWORD& lType);
	//----------------------------------------------------------------------------------------
	// Functions for working with path
	//----------------------------------------------------------------------------------------
	HRESULT PathCommandMoveTo(const double& dX, const double& dY);
	HRESULT PathCommandLineTo(const double& dX, const double& dY);
	HRESULT PathCommandLinesTo(double* pPoints, const int& nCount);
	HRESULT PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe);
	HRESULT PathCommandCurvesTo(double* pPoints, const int& nCount);
	HRESULT PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle);
	HRESULT PathCommandClose();
	HRESULT PathCommandEnd();
	HRESULT DrawPath(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory, const LONG& lType, bool bIgnoreRedact = false);
	HRESULT PathCommandStart();
	HRESULT PathCommandGetCurrentPoint(double* dX, double* dY);
	HRESULT PathCommandTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT PathCommandText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT PathCommandTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
	//----------------------------------------------------------------------------------------
	// Functions for image output
	//----------------------------------------------------------------------------------------
	HRESULT DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT DrawImageFromFile(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsTempDirectory, const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha = 255);
	//----------------------------------------------------------------------------------------
	// Functions for setting transformation
	//----------------------------------------------------------------------------------------
	HRESULT SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY);
	HRESULT GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY);
	HRESULT ResetTransform();
	//----------------------------------------------------------------------------------------
	// Additional functions
	//----------------------------------------------------------------------------------------
	HRESULT AddHyperlink(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsUrl, const std::wstring& wsTooltip);
	HRESULT AddLink(const double& dX, const double& dY, const double& dW, const double& dH, const double& dDestX, const double& dDestY, const int& nPage);
	HRESULT AddFormField (NSFonts::IApplicationFonts* pAppFonts, CFormFieldInfo* pFieldInfo, const std::wstring& wsTempDirectory);
	HRESULT AddAnnotField(NSFonts::IApplicationFonts* pAppFonts, CAnnotFieldInfo* pFieldInfo, bool bRedact = false);
	HRESULT AddMetaData(const std::wstring& sMetaName, BYTE* pMetaData, DWORD nMetaLength);
	HRESULT AddRedact(const std::vector<double>& arrRedact);
	HRESULT get_ClipMode(LONG* lMode);
	HRESULT put_ClipMode(const LONG& lMode);
	//----------------------------------------------------------------------------------------
	// Additional Pdf renderer functions
	//----------------------------------------------------------------------------------------
	HRESULT DrawImage1bpp(NSImages::CPixJbig2* pImageBuffer, const unsigned int& unWidth, const unsigned int& unHeight, const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT EnableBrushRect(const LONG& lEnable);
	HRESULT SetLinearGradient(const double& dX1, const double& dY1, const double& dX2, const double& dY2);
	HRESULT SetRadialGradient(const double& dX1, const double& dY1, const double& dR1, const double& dX2, const double& dY2, const double& dR2);
	HRESULT DrawImageWith1bppMask(IGrObject* pImage, NSImages::CPixJbig2* pMaskBuffer, const unsigned int& unMaskWidth, const unsigned int& unMaskHeight, const double& dX, const double& dY, const double& dW, const double& dH);
	//----------------------------------------------------------------------------------------
	// Additional functions for incremental Pdf writing
	//----------------------------------------------------------------------------------------
	HRESULT EditWidgetParents(NSFonts::IApplicationFonts* pAppFonts, CWidgetsInfo* pFieldInfo, const std::wstring& wsTempDirectory);
	void SetPage(PdfWriter::CPage* pPage);
	bool EditPage(PdfWriter::CPage* pNewPage);
	bool AddPage(int nPageIndex);
	bool EditClose();
	void PageRotate(int nRotate);
	void Sign(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsPicturePath,
			  const std::wstring &wsReason, const std::wstring &wsContact, const std::wstring &wsName, const std::wstring &wsLocation);
	bool PrepareSignature(const std::wstring& wsPath);
	bool FinalizeSignature(BYTE* pSignedData, DWORD dwDataLength);
	PdfWriter::CDocument* GetDocument();
	PdfWriter::CPage*     GetPage();
	IRenderer*            GetRenderer();
	void AddFont(const std::wstring& wsFontName, const bool& bBold, const bool& bItalic, const std::wstring& wsFontPath, const LONG& lFaceIndex);
	void SetHeadings(CHeadings* pCommand);
	void SetNeedAddHelvetica(bool bNeedAddHelvetica);
	void SetSplit(bool bSplit) { m_bSplit = bSplit; }

	void DrawWidgetAP(PdfWriter::CAnnotation* pAnnot, BYTE* pRender, LONG nLenRender, int nRotate = 0, bool bDiff = true);

private:
	PdfWriter::CAction* GetAction(CAnnotFieldInfo::CActionFieldPr* pAction, bool bDeferred = false);
	bool SkipRedact(const double& dX, const double& dY, const double& dW, const double& dH);
	bool SkipRedact(const double& dX, const double& dY);
	PdfWriter::CImageDict* LoadImage(Aggplus::CImage* pImage, BYTE nAlpha);
	PdfWriter::CImageDict* DrawImage(Aggplus::CImage* pImage, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha);
	bool DrawText(unsigned char* pCodes, const unsigned int& unLen, const double& dX, const double& dY, const std::string& sPUA);
	bool DrawTextToRenderer(const unsigned int* unGid, const unsigned int& unLen, const double& dX, const double& dY, const std::wstring& wsUnicodeText = L"");
	bool PathCommandDrawText(unsigned int* pUnicodes, unsigned int unLen, const double& dX, const double& dY, const unsigned int* pGids = NULL);
	int  IsEmbeddedBase14(const std::wstring& wsFontName);
	bool GetBaseFont14(const std::wstring& wsFontName, int nBase14);
	bool IsEmbeddedFont(const std::wstring& wsName);
	bool GetEmbeddedFont(const std::wstring& wsFontName);
	bool UpdateFont();
	bool FindFontPath(const std::wstring& wsFontName, const bool& bBold, const bool& bItalic, std::wstring& wsFontPath, LONG& lFaceIndex);
	bool GetFontPath(const std::wstring& wsFontName, const bool& bBold, const bool& bItalic, std::wstring& wsFontPath, LONG& lFaceIndex);
	PdfWriter::CFontCidTrueType* GetFont(const std::wstring& wsFontPath, const LONG& lFontIndex);
	PdfWriter::CFontCidTrueType* GetFont(const std::wstring& wsFontName, const bool& bBold, const bool& bItalic);
	bool GetFontData(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsValue, PdfWriter::CFontCidTrueType* pFont, bool bBold, bool bItalic,
					 unsigned int*& pUnicodes, unsigned int& unLen, unsigned short*& pCodes, PdfWriter::CFontCidTrueType**& ppFonts);
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
	PdfWriter::CAnnotAppearanceObject* DrawAP(PdfWriter::CAnnotation* pAnnot, BYTE* pRender, LONG nLenRender);
	void DrawTextWidget  (NSFonts::IApplicationFonts* pAppFonts, PdfWriter::CTextWidget* pTextWidget, const std::wstring& wsValue);
	void DrawChoiceWidget(NSFonts::IApplicationFonts* pAppFonts, PdfWriter::CChoiceWidget* pChoiceWidget, const std::vector<std::wstring>& arrValue);
	void DrawButtonWidget(NSFonts::IApplicationFonts* pAppFonts, PdfWriter::CPushButtonWidget* pButtonWidget, BYTE nAP, PdfWriter::CXObject* pForm);

private:
	NSFonts::IFontManager*       m_pFontManager;
	IRenderer*                   m_pRenderer;
	PdfWriter::CDocument*        m_pDocument;
	PdfWriter::CPage*            m_pPage;
	PdfWriter::CFontCidTrueType* m_pFont;
	PdfWriter::CFont14*          m_pFont14;
	PdfWriter::CFontEmbedded*    m_pFontEmbedded;
	PdfWriter::CShading*         m_pShading;
	PdfWriter::CExtGrState*      m_pShadingExtGrState;

	std::wstring                 m_wsTempDirectory;
	CCommandManager              m_oCommandManager;
	CPenState                    m_oPen;
	CBrushState                  m_oBrush;
	CFontState                   m_oFont;
	CPath                        m_oPath;
	CTransform                   m_oTransform;
	bool                         m_bNeedUpdateTextFont;
	bool                         m_bNeedAddHelvetica;
	double                       m_dPageHeight;
	double                       m_dPageWidth;
	LONG                         m_lClipDepth;
	LONG                         m_lClipMode;
	std::vector<TFontInfo>       m_vFonts;
	std::vector<TDestinationInfo>m_vDestinations;
	unsigned int                 m_unFieldsCounter;
	CMultiLineTextManager        m_oLinesManager;
	std::vector<double>          m_arrRedact;

	bool                         m_bValid;
	bool                         m_bSplit;

	friend class PdfWriter::RedactOutputDev;
};

#endif // _PDF_WRITER_H
