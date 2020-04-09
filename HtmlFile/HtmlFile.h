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
#ifndef _HTMLFILE_HTMLFILE_H_
#define _HTMLFILE_HTMLFILE_H_

#include <string>
#include <vector>

#ifndef HTMLFILE_USE_DYNAMIC_LIBRARY
#define HTMLFILE_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define HTMLFILE_DECL_EXPORT Q_DECL_EXPORT
#endif

class CHtmlFile_Private;
class HTMLFILE_DECL_EXPORT CHtmlFile
{
private:
    CHtmlFile_Private* m_internal;

public:

    CHtmlFile();
    ~CHtmlFile();

    /// \param sPathInternal - path (subprocesspath = path + HtmlFileInternal.exe) ("" -> GetProcessDirectory()/HtmlFileInternal/HtmlFileInternal.exe)
    /// \return 1 error, 0 - success

    int Convert(const std::vector<std::wstring>& arFiles, const std::wstring& sDstfolder);
    int ConvertEpub(const std::wstring& sFolder, std::wstring& sMetaInfo, const std::wstring& sDstfolder);
    int ConvertMht(const std::wstring& sFile, const std::wstring& sDstfolder);
};

#endif // _HTMLFILE_HTMLFILE_H_
