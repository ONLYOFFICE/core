#pragma once
#ifndef OOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_
#define OOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_

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
                m_table.insert( std::pair<CString,CString>(  _T("gif"),  _T("image/gif")));
                m_table.insert( std::pair<CString,CString>(  _T("png"),  _T("image/png")));
                m_table.insert( std::pair<CString,CString>(  _T("tif"),  _T("image/tiff")));
                m_table.insert( std::pair<CString,CString>(  _T("tiff"), _T("image/tiff")));
                m_table.insert( std::pair<CString,CString>(  _T("jpeg"), _T("image/jpeg")));
                m_table.insert( std::pair<CString,CString>(  _T("jpg"),  _T("image/jpeg")));
                m_table.insert( std::pair<CString,CString>(  _T("jpe"),  _T("image/jpeg")));
                m_table.insert( std::pair<CString,CString>(  _T("jfif"), _T("image/jpeg")));
                m_table.insert( std::pair<CString,CString>(  _T("rels"), _T("application/vnd.openxmlformats-package.relationships+xml")));
                m_table.insert( std::pair<CString,CString>(  _T("bin"),  _T("application/vnd.openxmlformats-officedocument.oleObject")));
                m_table.insert( std::pair<CString,CString>(  _T("xml"),  _T("application/xml")));
                m_table.insert( std::pair<CString,CString>(  _T("emf"),  _T("image/x-emf")));
                m_table.insert( std::pair<CString,CString>(  _T("emz"),  _T("image/x-emz")));
                m_table.insert( std::pair<CString,CString>(  _T("wmf"),  _T("image/x-wmf")));
                m_table.insert( std::pair<CString,CString>(  _T("svm"),  _T("image/svm")));
                m_table.insert( std::pair<CString,CString>(  _T("wav"),  _T("audio/wav")));
                m_table.insert( std::pair<CString,CString>(  _T("xls"),  _T("application/vnd.ms-excel")));
                m_table.insert( std::pair<CString,CString>(  _T("xlsm"), _T("application/vnd.ms-excel.sheet.macroEnabled.12")));
                m_table.insert( std::pair<CString,CString>(  _T("xlsb"), _T("application/vnd.ms-excel.sheet.binary.macroEnabled.12")));
                m_table.insert( std::pair<CString,CString>(  _T("xlsx"), _T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")));
                m_table.insert( std::pair<CString,CString>(  _T("ppt"),  _T("application/vnd.ms-powerpoint")));
                m_table.insert( std::pair<CString,CString>(  _T("pptm"), _T("application/vnd.ms-powerpoint.presentation.macroEnabled.12")));
                m_table.insert( std::pair<CString,CString>(  _T("pptx"), _T("application/vnd.openxmlformats-officedocument.presentationml.presentation")));
                m_table.insert( std::pair<CString,CString>(  _T("sldm"), _T("application/vnd.ms-powerpoint.slide.macroEnabled.12")));
                m_table.insert( std::pair<CString,CString>(  _T("sldx"), _T("application/vnd.openxmlformats-officedocument.presentationml.slide")));
                m_table.insert( std::pair<CString,CString>(  _T("doc"),  _T("application/msword")));
                m_table.insert( std::pair<CString,CString>(  _T("docm"), _T("aapplication/vnd.ms-word.document.macroEnabled.12")));
                m_table.insert( std::pair<CString,CString>(  _T("docx"), _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document")));
                m_table.insert( std::pair<CString,CString>(  _T("vml"),  _T("application/vnd.openxmlformats-officedocument.vmlDrawing")));
			}
			const CString operator[] (const CString& extension) const
			{
                std::map<CString, CString>::const_iterator it = m_table.find(extension);
                if (it == m_table.end())
                    return _T("");

                return it->second;
			}

		private:
            std::map<CString, CString> m_table;
		};
	} // namespace ContentTypes
} // namespace PPTX

#endif // OOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_
