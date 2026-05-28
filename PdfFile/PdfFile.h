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
#ifndef _PDF_FILE_H
#define _PDF_FILE_H

#ifndef PDFFILE_USE_DYNAMIC_LIBRARY
#define PDFFILE_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define PDFFILE_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "../DesktopEditor/graphics/IRenderer.h"
#include "../DesktopEditor/graphics/pro/Image.h"
#include "../DesktopEditor/graphics/pro/officedrawingfile.h"
#include "../DesktopEditor/xmlsec/src/include/Certificate.h"

class CPdfFile_Private;
class CConvertFromBinParams
{
public:
	std::wstring m_sMediaDirectory;
	std::wstring m_sInternalMediaDirectory;
	std::wstring m_sThemesDirectory;
	bool m_bIsUsePicker;

public:
	CConvertFromBinParams()
	{
		m_bIsUsePicker = false;
	}
};
class CPDFSignatureInfo
{
public:
	std::wstring m_wsReason;
	std::wstring m_wsContact;
	std::wstring m_wsName;
	std::wstring m_wsLocation;
};

namespace PdfFile
{
	typedef enum
	{
		errorNone          = 0, // No errors
		errorOpenFile      = 1, // Error opening PDF file
		errorBadCatalog    = 2, // Couldn't read the page catalog
		errorDamaged       = 3, // PDF file was damaged and cannot be recovered
		errorEncrypted     = 4, // File is encrypted, authorization failed
		errorHighlightFile = 5, // Nonexistent or invalid highlight file
		errorBadPrinter    = 6, // Bad printer
		errorPrinting      = 7, // Error during printing
		errorPermission    = 8, // Error related to file restrictions
		errorBadPageNum    = 9, // Invalid page number
		errorFileIO        = 10,// Error reading/writing
		errorMemory        = 11 // Memory exceed
	} EError;
}

class PDFFILE_DECL_EXPORT CPdfFile : public IOfficeDrawingFile, public IRenderer
{
public:
	CPdfFile(NSFonts::IApplicationFonts* pAppFonts);
	virtual ~CPdfFile();
	NSFonts::IFontManager* GetFontManager();
	// In read mode closes reader, allows opening a new one
	// In edit mode closes writer, allows using reader
	virtual void Close();

	// --- EDIT ---
	// Switches to edit mode. Pdf must already be opened for reading - LoadFromFile/LoadFromMemory
	bool EditPdf(const std::wstring& wsDstFile = L"");
	void EditClose();
	void SetEditType(int nType);
	void RedactInfo(int nFlag, const std::vector<IAdvancedCommand*>& arrForms);
	// Page manipulations are possible in edit mode
	bool EditPage  (int nPageIndex);
	bool DeletePage(int nPageIndex);
	bool AddPage   (int nPageIndex);
	bool MovePage  (int nPageIndex, int nPos);
	bool MergePages(const std::wstring& wsPath, int nMaxID = 0, const std::wstring& wsPrefixForm = L"");
	bool PrintPages(const std::vector<bool>& arrPages, int nFlag);
	HRESULT ChangePassword(const std::wstring& wsPath, const std::wstring& wsPassword = L"");

	// --- READER ---

	int GetError();
	bool IsNeedCMap();
	void SetCMapMemory(BYTE* pData, DWORD nSizeData);
	void SetCMapFolder(const std::wstring& sFolder);
	void SetCMapFile(const std::wstring& sFile);
	void ToXml(const std::wstring& sFile, bool bSaveStreams = false);

	static bool GetMetaData(const std::wstring& sFile, const std::wstring& sMetaName, BYTE** pMetaData, DWORD& nMetaLength);
	virtual bool LoadFromFile  (const std::wstring& file, const std::wstring& options = L"", const wchar_t* owner_password = NULL, const wchar_t* user_password = NULL);
	virtual bool LoadFromMemory(BYTE* data, DWORD length, const std::wstring& options = L"", const wchar_t* owner_password = NULL, const wchar_t* user_password = NULL);
	virtual NSFonts::IApplicationFonts* GetFonts();

	virtual OfficeDrawingFileType GetType();

	virtual std::wstring GetTempDirectory();
	virtual void SetTempDirectory(const std::wstring& wsPath);

	virtual int GetPagesCount();
	virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
	virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak, COfficeDrawingPageParams* pParams = NULL);
	virtual std::wstring GetInfo();
	virtual BYTE* GetStructure();
	virtual BYTE* GetLinks(int nPageIndex);
	virtual void SetPainter(IOfficeDrawingFilePainter* pPainter);

	bool ValidMetaData();
	// Takes ownership of malloc data memory
	bool MergePages(BYTE* data, DWORD length, int nMaxID = 0, const std::string& sPrefixForm = "");
	bool UnmergePages();
	bool RedactPage(int nPageIndex, double* arrRedactBox, int nLengthX8, BYTE* pChanges = NULL, int nLength = 0);
	bool UndoRedact();
	bool CheckOwnerPassword(const wchar_t* sPassword);
	bool CheckPerm(int nPerm);
	int GetRotate(int nPageIndex);
	int GetMaxRefID();
	void SetPageFonts(int nPageIndex);
	BYTE* GetWidgets();
	BYTE* GetAnnotEmbeddedFonts();
	BYTE* GetAnnotStandardFonts();
	BYTE* GetAnnots    (int nPageIndex = -1);
	BYTE* SplitPages   (const int* arrPageIndex, unsigned int unLength, BYTE* pChanges = NULL, DWORD nLength = 0);
	BYTE* VerifySign   (const std::wstring& sFile, ICertificate* pCertificate, int nWidget = -1);
	BYTE* GetAPWidget  (int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nWidget  = -1, const char* sView  = NULL, const char* sBView = NULL);
	BYTE* GetAPAnnots  (int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nAnnot   = -1, const char* sView  = NULL);
	BYTE* GetButtonIcon(int nBackgroundColor, int nPageIndex, bool bBase64 = false, int nBWidget = -1, const char* sIView = NULL);
	BYTE* GetGIDByUnicode(const std::wstring& wsFontName);
	std::wstring GetFontPath(const std::wstring& wsFontName);
	std::wstring GetEmbeddedFontPath(const std::wstring& wsFontName);

	// --- WRITER ---

	void CreatePdf    (bool isPDFA = false);
	int  SaveToFile   (const std::wstring& wsPath);
	void RotatePage   (int nRotate);
	void SetPassword  (const std::wstring& wsPassword);
	void SetDocumentID(const std::wstring& wsDocumentID);
	void Sign(const double& dX = 0, const double& dY = 0, const double& dW = 0, const double& dH = 0, const std::wstring& wsPicturePath = L"", CPDFSignatureInfo* pSigInfo = NULL);
	bool PrepareSignature(const std::wstring& wsPath);
	bool FinalizeSignature(BYTE* pSignedData, DWORD dwDataLength);
	void SetDocumentInfo(const std::wstring& wsTitle, const std::wstring& wsCreator, const std::wstring& wsSubject, const std::wstring& wsKeywords);
	void AddMetaData(const std::wstring& sMetaName, BYTE* pMetaData, DWORD nMetaLength);

	HRESULT OnlineWordToPdf          (const std::wstring& wsSrcFile, const std::wstring& wsDstFile, CConvertFromBinParams* pParams = NULL);
	HRESULT OnlineWordToPdfFromBinary(const std::wstring& wsSrcFile, const std::wstring& wsDstFile, CConvertFromBinParams* pParams = NULL);
	HRESULT AddToPdfFromBinary(BYTE* pBuffer, unsigned int nLen, CConvertFromBinParams* pParams = NULL);
	HRESULT DrawImageWith1bppMask(IGrObject* pImage, NSImages::CPixJbig2* pMaskBuffer, const unsigned int& unMaskWidth, const unsigned int& unMaskHeight, const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT DrawImage1bpp(NSImages::CPixJbig2* pImageBuffer, const unsigned int& unWidth, const unsigned int& unHeight, const double& dX, const double& dY, const double& dW, const double& dH);
	HRESULT SetLinearGradient(const double& dX1, const double& dY1, const double& dX2, const double& dY2);
	HRESULT SetRadialGradient(const double& dX1, const double& dY1, const double& dR1, const double& dX2, const double& dY2, const double& dR2);

	//----------------------------------------------------------------------------------------
	// Renderer type
	//----------------------------------------------------------------------------------------
	virtual HRESULT get_Type(LONG* lType);
	//----------------------------------------------------------------------------------------
	// Functions for working with page
	//----------------------------------------------------------------------------------------
	virtual HRESULT NewPage();
	virtual HRESULT get_Height(double* dHeight);
	virtual HRESULT put_Height(const double& dHeight);
	virtual HRESULT get_Width(double* dWidth);
	virtual HRESULT put_Width(const double& dWidth);
	virtual HRESULT get_DpiX(double* dDpiX);
	virtual HRESULT get_DpiY(double* dDpiY);
	//----------------------------------------------------------------------------------------
	// Functions for working with Pen
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
	// Functions for working with Brush
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
	virtual HRESULT get_BrushTextureImage(Aggplus::CImage** pImage);
	virtual HRESULT put_BrushTextureImage(Aggplus::CImage* pImage);
	virtual HRESULT get_BrushTransform(Aggplus::CMatrix& oMatrix);
	virtual HRESULT put_BrushTransform(const Aggplus::CMatrix& oMatrix);
	virtual HRESULT get_BrushOffset(double& offsetX, double& offsetY) const;
	virtual HRESULT put_BrushOffset(const double& offsetX, const double& offsetY);
	virtual HRESULT get_BrushScale(bool& isScale, double& scaleX, double& scaleY) const;
	virtual HRESULT put_BrushScale(bool isScale, const double& scaleX, const double& scaleY);
	//----------------------------------------------------------------------------------------
	// Functions for working with fonts
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
	// Functions for text output
	//----------------------------------------------------------------------------------------
	virtual HRESULT CommandDrawTextCHAR  (const LONG& lUnicode,                   const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT CommandDrawText      (const std::wstring& wsUnicodeText,                                                           const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT CommandDrawTextEx    (const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT CommandDrawTextCHAR2 (unsigned int* unUnicode, const unsigned int& unUnicodeCount, const unsigned int& unGid, const double& dX, const double& dY, const double& dW, const double& dH);
	//----------------------------------------------------------------------------------------
	// Command markers
	//----------------------------------------------------------------------------------------
	virtual HRESULT BeginCommand(const DWORD& lType);
	virtual HRESULT EndCommand(const DWORD& lType);
	//----------------------------------------------------------------------------------------
	// Functions for working with path
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
	// Functions for image output
	//----------------------------------------------------------------------------------------
	virtual HRESULT DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH);
	virtual HRESULT DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha = 255);
	//----------------------------------------------------------------------------------------
	// Functions for setting transformation
	//----------------------------------------------------------------------------------------
	virtual HRESULT SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY);
	virtual HRESULT GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY);
	virtual HRESULT ResetTransform();
	//----------------------------------------------------------------------------------------
	// Clip type
	//----------------------------------------------------------------------------------------
	virtual HRESULT get_ClipMode(LONG* lMode);
	virtual HRESULT put_ClipMode(const LONG& lMode);
	//----------------------------------------------------------------------------------------
	// Additional functions
	//----------------------------------------------------------------------------------------
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand);
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand);
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand);

	virtual HRESULT IsSupportAdvancedCommand(const IAdvancedCommand::AdvancedCommandType& type);
	virtual HRESULT AdvancedCommand(IAdvancedCommand* command);

private:
	CPdfFile_Private* m_pInternal;
};

#endif // _PDF_FILE_H
