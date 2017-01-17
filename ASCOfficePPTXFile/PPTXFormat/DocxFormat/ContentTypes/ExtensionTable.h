/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#pragma once
#ifndef PPTXOOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_
#define PPTXOOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_

#include "../../../../Common/DocxFormat/Source/Base/Base.h"
#include <map>

namespace PPTX
{
	namespace ContentTypes
	{
		class ExtensionTable
		{
		public:
			ExtensionTable()
			{
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("gif"),  _T("image/gif")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("png"),  _T("image/png")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("tif"),  _T("image/tiff")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("tiff"), _T("image/tiff")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("jpeg"), _T("image/jpeg")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("jpg"),  _T("image/jpeg")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("jpe"),  _T("image/jpeg")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("jfif"), _T("image/jpeg")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("rels"), _T("application/vnd.openxmlformats-package.relationships+xml")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("bin"),  _T("application/vnd.openxmlformats-officedocument.oleObject")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("xml"),  _T("application/xml")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("emf"),  _T("image/x-emf")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("emz"),  _T("image/x-emz")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("wmf"),  _T("image/x-wmf")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("svm"),  _T("image/svm")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("wav"),  _T("audio/wav")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("xls"),  _T("application/vnd.ms-excel")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("xlsm"), _T("application/vnd.ms-excel.sheet.macroEnabled.12")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("xlsb"), _T("application/vnd.ms-excel.sheet.binary.macroEnabled.12")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("xlsx"), _T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("ppt"),  _T("application/vnd.ms-powerpoint")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("pptm"), _T("application/vnd.ms-powerpoint.presentation.macroEnabled.12")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("pptx"), _T("application/vnd.openxmlformats-officedocument.presentationml.presentation")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("sldm"), _T("application/vnd.ms-powerpoint.slide.macroEnabled.12")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("sldx"), _T("application/vnd.openxmlformats-officedocument.presentationml.slide")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("doc"),  _T("application/msword")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("docm"), _T("aapplication/vnd.ms-word.document.macroEnabled.12")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("docx"), _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document")));
                m_table.insert( std::pair<std::wstring,std::wstring>(  _T("vml"),  _T("application/vnd.openxmlformats-officedocument.vmlDrawing")));
			}
			const std::wstring operator[] (const std::wstring& extension) const
			{
                std::map<std::wstring, std::wstring>::const_iterator it = m_table.find(extension);
                if (it == m_table.end())
                    return _T("");

                return it->second;
			}

		private:
            std::map<std::wstring, std::wstring> m_table;
		};
	} // namespace ContentTypes
} // namespace PPTX

#endif // PPTXOOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_
