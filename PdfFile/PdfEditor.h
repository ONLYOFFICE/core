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
#ifndef _PDF_EDITOR_H
#define _PDF_EDITOR_H

#include "PdfWriter.h"
#include "PdfReader.h"

HRESULT _ChangePassword(const std::wstring& wsPath, const std::wstring& wsPassword, CPdfReader* _pReader, CPdfWriter* _pWriter);

struct CObjectInfo
{
	PdfWriter::CObjectBase* pObj;
	int nRefCount;

	CObjectInfo() { pObj = NULL; nRefCount = 0; }
	CObjectInfo(PdfWriter::CObjectBase* _pObj, int _nRefCount) : pObj(_pObj), nRefCount(_nRefCount) {}
};

class CObjectsManager
{
public:
	CObjectsManager() : m_pDoc(NULL) {}

	void AddObj(int nID, PdfWriter::CObjectBase* pObj);
	void RemoveObj(int nID);
	PdfWriter::CObjectBase* GetObj(int nID);
	bool IncRefCount(int nID);
	bool DecRefCount(int nID);
	int FindObj(PdfWriter::CObjectBase* pObj);
	void DeleteObjTree(Object* obj, XRef* xref, int nStartRefID);
	void SetDoc(PdfWriter::CDocument* pDoc);

	std::vector<int> m_arrSplitAddPages;
	std::vector<int> m_arrRedactFormFieldsID;
	std::vector<IAdvancedCommand*> m_arrRedactFormFields;

private:
	PdfWriter::CDocument* m_pDoc;
	std::map<int, CObjectInfo> m_mUniqueRef; // map of unique objects
};

class CPdfEditor
{
public:
	enum class Mode
	{
		Unknown,
		ReadOnly,
		Split,
		WriteAppend,
		WriteNew
	};

	CPdfEditor(const std::wstring& _wsSrcFile, const wchar_t* _wsPassword, const std::wstring& _wsDstFile, CPdfReader* _pReader, CPdfWriter* _pWriter, Mode nMode = Mode::Unknown);

	void SetMode(Mode nMode);
	void RedactInfo(int nFlag, const std::vector<IAdvancedCommand*>& arrForms);

	int  GetError();
	void Close();
	bool EditPage(int nPageIndex, bool bSet = true, bool bActualPos = false);
	bool DeletePage(int nPageIndex);
	bool AddPage(int nPageIndex);
	bool MovePage(int nPageIndex, int nPos);
	bool EditAnnot(int nPageIndex, int nID);
	bool DeleteAnnot(int nID, Object* oAnnots = NULL);
	bool EditWidgets(IAdvancedCommand* pCommand);
	int  GetPagesCount();
	void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
	int  GetRotate(int nPageIndex);
	bool IsEditPage();
	void ClearPage();
	void AddShapeXML(const std::string& sXML);
	void EndMarkedContent();
	bool IsBase14(const std::wstring& wsFontName, bool& bBold, bool& bItalic, std::wstring& wsFontPath);
	void Redact(IAdvancedCommand* pCommand);
	std::vector<double> WriteRedact(const std::vector<std::wstring>& arrID);

	bool SplitPages(const int* arrPageIndex, unsigned int unLength);
	void AfterSplitPages();
	bool MergePages(const std::wstring& wsPath);

	bool PrintPages(const std::vector<bool>& arrPages, int nFlag);

private:
	bool IncrementalUpdates();
	void NewFrom();
	void GetPageTree(XRef* xref, Object* pPagesRefObj, PdfWriter::CPageTree* pPageParent = NULL);
	bool SplitPages(const int* arrPageIndex, unsigned int unLength, PDFDoc* _pDoc, int nStartRefID);
	bool ChangeFullNameParent(int nParent, const std::string& sPrefixForm, std::vector<int>& arrRename);
	void ScanAndProcessFonts(PDFDoc* pPDFDocument, XRef* xref, Dict* pResources, int nDepth, std::vector<int>& arrUniqueResources, PdfReader::CPdfFontList* pFontList, int nStartRefID);
	void ScanFonts(PDFDoc* pPDFDocument, Dict* pResources);

	struct CRedactData
	{
		std::wstring sID;
		std::vector<double> arrQuads;
		LONG nLenRender;
		BYTE* pRender;
		bool bDraw = false;
	};

	std::wstring m_wsSrcFile;
	std::wstring m_wsDstFile;
	const wchar_t* m_wsPassword;
	std::vector<CRedactData> m_arrRedact;
	std::map<std::wstring, std::wstring> m_mFonts;
	CObjectsManager m_mObjManager;

	CPdfReader* m_pReader;
	CPdfWriter* m_pWriter;

	int m_nError;
	// 0 - Append. pReader and pWriter work with the same file
	// 1 - Split. pReader and pWriter work with different files
	Mode m_nMode;
	int m_nRedactInfo;
	int m_nEditPage;
	int m_nOriginIndex;
};

#endif // _PDF_EDITOR_H
