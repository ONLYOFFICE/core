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
#pragma once

#ifndef DJVU_USE_DYNAMIC_LIBRARY
#define DJVU_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define DJVU_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "../DesktopEditor/graphics/pro/officedrawingfile.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"

// работаем через класс CDjVuFileImplementation, чтобы когда цеплялся данный h-файл, ничего лишнего не инклюдилось
class CDjVuFileImplementation;

class DJVU_DECL_EXPORT CDjVuFile : public IOfficeDrawingFile
{
private:
	CDjVuFileImplementation* m_pImplementation;

public:

    CDjVuFile(NSFonts::IApplicationFonts* fonts);
    virtual ~CDjVuFile();

    virtual bool LoadFromFile(const std::wstring& file, const std::wstring& options = L"",
                                    const std::wstring& owner_password = L"", const std::wstring& user_password = L"");
    virtual bool LoadFromMemory(BYTE* data, DWORD length, const std::wstring& options = L"",
                                const std::wstring& owner_password = L"", const std::wstring& user_password = L"");

    virtual void Close();
    virtual NSFonts::IApplicationFonts* GetFonts();

    virtual OfficeDrawingFileType GetType();

    virtual std::wstring GetTempDirectory();
    virtual void SetTempDirectory(const std::wstring& directory);

    virtual int GetPagesCount();
    virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
    virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
    virtual std::wstring GetInfo();

    void ConvertToPdf(const std::wstring& path);

    virtual BYTE* GetStructure();
    virtual BYTE* GetLinks (int nPageIndex);
};
