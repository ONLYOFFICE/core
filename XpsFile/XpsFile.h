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
#ifndef _XPS_FILE_H
#define _XPS_FILE_H

#ifndef XPS_USE_DYNAMIC_LIBRARY
#define XPS_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define XPS_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "../DesktopEditor/graphics/pro/officedrawingfile.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"

class CXpsFile_Private;
class XPS_DECL_EXPORT CXpsFile : public IOfficeDrawingFile
{
public:
    CXpsFile(NSFonts::IApplicationFonts* fonts);
    virtual ~CXpsFile();

    virtual bool LoadFromFile(const std::wstring& file, const std::wstring& options = L"",
							  const wchar_t* owner_password = NULL, const wchar_t* user_password = NULL);
    virtual bool LoadFromMemory(BYTE* data, DWORD length, const std::wstring& options = L"",
								const wchar_t* owner_password = NULL, const wchar_t* user_password = NULL);

    virtual void Close();
    virtual NSFonts::IApplicationFonts* GetFonts();

    virtual OfficeDrawingFileType GetType();

    virtual std::wstring GetTempDirectory();
    virtual void SetTempDirectory(const std::wstring& directory);

    virtual int GetPagesCount();
    virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
    virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak, COfficeDrawingPageParams* pParams = NULL);
    virtual std::wstring GetInfo();

#ifndef DISABLE_PDF_CONVERTATION
    void ConvertToPdf(const std::wstring& wsDstPath);
#endif

    virtual BYTE* GetStructure();
    virtual BYTE* GetLinks(int nPageIndex);

private:
    CXpsFile_Private* m_pInternal;
};

#endif // _XPS_FILE_H
