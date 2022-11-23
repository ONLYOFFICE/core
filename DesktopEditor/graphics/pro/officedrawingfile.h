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
#ifndef _OFFICE_DRAWING_FILE_H
#define _OFFICE_DRAWING_FILE_H

#include "./Fonts.h"
class IRenderer;

enum OfficeDrawingFileType
{
    odftPDF = 0,
    odftXPS = 1,
    odftDJVU = 2,
    odftUndefined = 255
};

class GRAPHICS_DECL IOfficeDrawingFile
{
public:
    virtual ~IOfficeDrawingFile() {}

    // Open
    virtual bool LoadFromFile(const std::wstring& file, const std::wstring& options = L"",
                                const std::wstring& owner_password = L"", const std::wstring& user_password = L"") = 0;
    virtual bool LoadFromMemory(unsigned char* data, unsigned long length, const std::wstring& options = L"",
                                const std::wstring& owner_password = L"", const std::wstring& user_password = L"") = 0;

    // Close
    virtual void Close() = 0;

    // Get IApplicationFonts for wrappers
    virtual NSFonts::IApplicationFonts* GetFonts() = 0;

    // Type
    virtual OfficeDrawingFileType GetType() = 0;

    // Temp directory
    virtual std::wstring GetTempDirectory() = 0;
    virtual void SetTempDirectory(const std::wstring& directory) = 0;

    // Pages info/draw
    virtual int GetPagesCount() = 0;
    virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) = 0;
    virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak) = 0;

    // Common methods/wrappers on GetPageInfo + DrawPageOnRenderer
    virtual unsigned char* ConvertToPixels(int nPageIndex, int nRasterW, int nRasterH,
                                           bool bIsFlip = false,
                                           NSFonts::IFontManager* pFonts = NULL,
                                           int nBackgroundColor = 0xFFFFFF,
                                           bool bIsDarkMode = false);
    virtual void ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType, const int nRasterW = -1, const int nRasterH = -1,
                                 bool bIsFlip = false,
                                 NSFonts::IFontManager* pFonts = NULL,
                                 int nBackgroundColor = 0xFFFFFF,
                                 bool bIsDarkMode = false);

    // Common methods for viewer
    virtual std::wstring GetInfo() = 0;
    virtual unsigned char* GetStructure() = 0;
    virtual unsigned char* GetLinks(int nPageIndex) = 0;
};

#endif // _OFFICE_DRAWING_FILE_H
