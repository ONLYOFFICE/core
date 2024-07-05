﻿/*
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
#ifndef _PDF_EDITOR_H
#define _PDF_EDITOR_H

#include "PdfWriter.h"
#include "PdfReader.h"

HRESULT _ChangePassword(const std::wstring& wsPath, const std::wstring& wsPassword, CPdfReader* _pReader, CPdfWriter* _pWriter);

class CPdfEditor
{
public:
	CPdfEditor(const std::wstring& _wsSrcFile, const std::wstring& _wsPassword, CPdfReader* _pReader, const std::wstring& _wsDstFile, CPdfWriter* _pWriter);

	int  GetError();
	void Close();
	bool EditPage(int nPageIndex, bool bSet = true);
	bool DeletePage(int nPageIndex);
	bool AddPage(int nPageIndex);
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

private:
	void GetPageTree(XRef* xref, Object* pPagesRefObj);

	std::wstring wsSrcFile;
	std::wstring wsPassword;
	std::map<std::wstring, std::wstring> m_mFonts;

	CPdfReader* pReader;
	CPdfWriter* pWriter;

	int nError;
	int m_nEditPage;
};

#endif // _PDF_EDITOR_H
