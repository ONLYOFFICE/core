/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef _XPS_FILE_H
#define _XPS_FILE_H

#ifndef XPS_USE_DYNAMIC_LIBRARY
#define XPS_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define XPS_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "../DesktopEditor/common/officedrawingfile.h"

class CXpsFile_Private;
class XPS_DECL_EXPORT CXpsFile : public IOfficeDrawingFile
{
public:
    CXpsFile(CApplicationFonts* fonts);
    virtual ~CXpsFile();

    virtual bool LoadFromFile(const std::wstring& file, const std::wstring& options = L"",
                                    const std::wstring& owner_password = L"", const std::wstring& user_password = L"");

    virtual void Close();

    virtual std::wstring GetTempDirectory();
    virtual void SetTempDirectory(const std::wstring& directory);

    virtual int GetPagesCount();
    virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
    virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
    virtual void ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType);

	void         ConvertToPdf(const std::wstring& wsDstPath);

private:
    CXpsFile_Private* m_pInternal;
};

#endif // _XPS_FILE_H
