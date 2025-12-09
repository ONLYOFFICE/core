/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#ifndef _PDF_READER_H
#define _PDF_READER_H

#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/graphics/pro/officedrawingfile.h"
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

	bool LoadFromFile  (NSFonts::IApplicationFonts* pAppFonts, const std::wstring& file, const std::wstring& owner_password = L"", const std::wstring& user_password = L"");
	bool LoadFromMemory(NSFonts::IApplicationFonts* pAppFonts, BYTE* data, DWORD length, const std::wstring& owner_password = L"", const std::wstring& user_password = L"");

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
	// Захватывает полученную память malloc data
	bool MergePages(BYTE* pData, DWORD nLength, const std::wstring& wsPassword = L"", int nMaxID = 0, const std::string& sPrefixForm = "");
	bool MergePages(const std::wstring& wsFile, const std::wstring& wsPassword = L"", int nMaxID = 0, const std::string& sPrefixForm = "");
	bool UnmergePages();
	bool RedactPage(int nPageIndex, double* arrRedactBox, int nLengthX8, BYTE* pChanges, int nLength);
	bool UndoRedact();
	void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
	void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
	std::wstring GetInfo();
	std::wstring GetFontPath(const std::wstring& wsFontName, bool bSave = true);
	std::wstring ToXml(const std::wstring& wsXmlPath, bool isPrintStreams = false);
	void ChangeLength(DWORD nLength) { m_nFileLength = nLength; }

	NSFonts::IFontManager* GetFontManager() { return m_pFontManager; }
	PDFDoc* GetLastPDFDocument();
	PDFDoc* GetPDFDocument(int PDFIndex);
	int GetStartRefID(PDFDoc* pDoc);
	int GetNumPagesBefore(PDFDoc* pDoc);
	std::string GetPrefixForm(PDFDoc* pDoc);
	int FindRefNum(int nObjID, PDFDoc** pDoc = NULL, int* nStartRefID = NULL);
	int GetPageIndex(int nPageIndex, PDFDoc** pDoc = NULL, PdfReader::CPdfFontList** pFontList = NULL, int* nStartRefID = NULL);

	void SetFonts(int nPageIndex);
	BYTE* GetStructure();
	BYTE* GetLinks(int nPageIndex);
	BYTE* GetWidgets();
	BYTE* GetFonts(bool bStandart);
	BYTE* GetAnnots(int nPageIndex = -1);
	BYTE* VerifySign(const std::wstring& sFile, ICertificate* pCertificate, int nWidget = -1);
	BYTE* GetAPWidget  (int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nWidget  = -1, const char* sView  = NULL, const char* sBView = NULL);
	BYTE* GetAPAnnots  (int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nAnnot   = -1, const char* sView  = NULL);
	BYTE* GetButtonIcon(int nBackgroundColor, int nPageIndex, bool bBase64 = false, int nBWidget = -1, const char* sIView = NULL);
	BYTE* StreamToCData(BYTE* pSteam, int nLength);
	const std::map<std::wstring, std::wstring>& GetFonts() { return m_mFonts; }

private:
	void Clear();

	std::wstring           m_wsTempFolder;
	NSFonts::IFontManager* m_pFontManager;
	DWORD                  m_nFileLength;
	int                    m_eError;
	std::vector<CPdfReaderContext*> m_vPDFContext;
	std::vector<CPdfRedact*> m_vRedact;
	std::map<std::wstring, std::wstring> m_mFonts;
};

#endif // _PDF_READER_H
