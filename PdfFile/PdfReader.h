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
#ifndef _PDF_READER_H
#define _PDF_READER_H

#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/graphics/pro/officedrawingfile.h"
#include "../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"
#include "../DesktopEditor/xmlsec/src/include/Certificate.h"
#include "SrcReader/RendererOutputDev.h"

#include <map>
#include <vector>

class PDFDoc;
struct CPdfReaderContext
{
	PDFDoc* m_pDocument;
	PdfReader::CPdfFontList* m_pFontList;
	unsigned int m_nStartID;
	std::string m_sPrefixForm;

	CPdfReaderContext() : m_pDocument(NULL), m_pFontList(NULL), m_nStartID(0) {}
	~CPdfReaderContext();
};

struct CPdfRedact
{
	int m_nPageIndex;
	BYTE* m_pChanges;
	int m_nChangeLength;
	std::vector<double> m_arrRedactBox;

	CPdfRedact() : m_nPageIndex(-1), m_pChanges(NULL), m_nChangeLength(0) {}
	~CPdfRedact();
};

class CPdfReader
{
public:
	CPdfReader(NSFonts::IApplicationFonts* pAppFonts);
	~CPdfReader();

	bool LoadFromFile  (NSFonts::IApplicationFonts* pAppFonts, const std::wstring& file, const wchar_t* owner_password = NULL, const wchar_t* user_password = NULL);
	bool LoadFromMemory(NSFonts::IApplicationFonts* pAppFonts, BYTE* data, DWORD length, const wchar_t* owner_password = NULL, const wchar_t* user_password = NULL);

	void Close();
	void CleanUp();

	void SetParams(COfficeDrawingPageParams* pParams);
	std::wstring GetTempDirectory();
	void SetTempDirectory(const std::wstring& directory);

	bool IsNeedCMap();
	void SetCMapMemory(BYTE* pData, DWORD nSizeData);
	void SetCMapFolder(const std::wstring& sFolder);
	void SetCMapFile(const std::wstring& sFile);

	int GetError();
	int GetRotate(int nPageIndex);
	int GetMaxRefID();
	int GetNumPages();
	bool ValidMetaData();

	// Takes ownership of malloc data memory
	bool MergePages(BYTE* pData, DWORD nLength, const wchar_t* wsPassword = NULL, int nMaxID = 0, const std::string& sPrefixForm = "");
	bool MergePages(const std::wstring& wsFile, const wchar_t* wsPassword = NULL, int nMaxID = 0, const std::string& sPrefixForm = "");
	bool UnmergePages();
	bool RedactPage(int nPageIndex, double* arrRedactBox, int nLengthX8, BYTE* pChanges, int nLength);
	bool UndoRedact();
	bool CheckOwnerPassword(const wchar_t* sPassword);
	bool CheckPerm(int nPerm);
	void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
	void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
	void SetPainter(IOfficeDrawingFilePainter* pPainter);
	std::wstring GetInfo();
	BYTE* GetGIDByUnicode(const std::wstring& wsFontName);
	std::wstring GetFontPath(const std::wstring& wsFontName, bool bSave = true);
	std::wstring ToXml(const std::wstring& wsXmlPath, bool isPrintStreams = false);
	void ChangeLength(DWORD nLength) { m_nFileLength = nLength; }

	NSFonts::IFontManager* GetFontManager() { return m_pFontManager; }
	PDFDoc* GetLastPDFDocument();
	PDFDoc* GetPDFDocument(int PDFIndex);
	int GetStartRefID(PDFDoc* pDoc);
	int GetNumPagesBefore(PDFDoc* pDoc);
	PdfReader::CPdfFontList* GetFontList(PDFDoc* pDoc);
	std::string GetPrefixForm(PDFDoc* pDoc);
	int FindRefNum(int nObjID, PDFDoc** pDoc = NULL, int* nStartRefID = NULL);
	int GetPageIndex(int nPageIndex, PDFDoc** pDoc = NULL, PdfReader::CPdfFontList** pFontList = NULL, int* nStartRefID = NULL);

	void SetFonts(PdfReader::CPdfFontList* pFontList);
	BYTE* GetStructure();
	BYTE* GetLinks(int nPageIndex);
	BYTE* GetWidgets();
	BYTE* GetFonts(bool bStandart);
	BYTE* GetAllFonts();
	BYTE* GetAnnots(int nPageIndex = -1);
	BYTE* VerifySign(const std::wstring& sFile, ICertificate* pCertificate, int nWidget = -1);
	BYTE* GetAPWidget  (int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nWidget  = -1, const char* sView  = NULL, const char* sBView = NULL);
	BYTE* GetAPAnnots  (int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nAnnot   = -1, const char* sView  = NULL);
	BYTE* GetButtonIcon(int nBackgroundColor, int nPageIndex, bool bBase64 = false, int nBWidget = -1, const char* sIView = NULL);
	BYTE* StreamToCData(BYTE* pSteam, int nLength);
	const std::map<std::wstring, std::wstring>& GetFonts() { return m_mFonts; }

private:
	void Clear();

	CPdfReaderContext* CreateContext(const std::string& sPrefixForm, int nMaxID);
	bool FinalizeMerge(CPdfReaderContext* pContext);
	CPdfReaderContext* FindContext(PDFDoc* pDoc) const;

private:
	std::wstring           m_wsTempFolder;
	NSFonts::IFontManager* m_pFontManager;
	DWORD                  m_nFileLength;
	int                    m_eError;
	IOfficeDrawingFilePainter*      m_pPainter;
	std::vector<CPdfReaderContext*> m_vPDFContext;
	std::vector<CPdfRedact*>        m_vRedact;
	std::map<std::wstring, std::wstring> m_mFonts;
};

#endif // _PDF_READER_H
