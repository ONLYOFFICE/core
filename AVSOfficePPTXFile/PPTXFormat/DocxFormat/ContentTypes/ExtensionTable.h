#pragma once
#ifndef OOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_
#define OOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_

#include "../../../../Common/DocxFormat/Source/Base/Base.h"


namespace OOX
{
	namespace ContentTypes
	{
		class ExtensionTable
		{
		public:
			ExtensionTable()
			{
				m_table.SetAt(_T("gif"), _T("image/gif"));
				m_table.SetAt(_T("png"), _T("image/png"));
				m_table.SetAt(_T("tif"), _T("image/tiff"));
				m_table.SetAt(_T("tiff"), _T("image/tiff"));
				m_table.SetAt(_T("jpeg"), _T("image/jpeg"));
				m_table.SetAt(_T("jpg"), _T("image/jpeg"));
				m_table.SetAt(_T("jpe"), _T("image/jpeg"));
				m_table.SetAt(_T("jfif"), _T("image/jpeg"));
				m_table.SetAt(_T("rels"), _T("application/vnd.openxmlformats-package.relationships+xml"));
				m_table.SetAt(_T("bin"), _T("application/vnd.openxmlformats-officedocument.oleObject"));
				m_table.SetAt(_T("xml"), _T("application/xml"));
				m_table.SetAt(_T("emf"), _T("image/x-emf"));
				m_table.SetAt(_T("emz"), _T("image/x-emz"));
				m_table.SetAt(_T("wmf"), _T("image/x-wmf"));
				m_table.SetAt(_T("svm"), _T("image/svm"));
				m_table.SetAt(_T("wav"), _T("audio/wav"));
				m_table.SetAt(_T("xls"), _T("application/vnd.ms-excel"));
				m_table.SetAt(_T("xlsm"), _T("application/vnd.ms-excel.sheet.macroEnabled.12"));
				m_table.SetAt(_T("xlsb"), _T("application/vnd.ms-excel.sheet.binary.macroEnabled.12"));
				m_table.SetAt(_T("xlsx"), _T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
				m_table.SetAt(_T("ppt"), _T("application/vnd.ms-powerpoint"));
				m_table.SetAt(_T("pptm"), _T("application/vnd.ms-powerpoint.presentation.macroEnabled.12"));			
				m_table.SetAt(_T("pptx"), _T("application/vnd.openxmlformats-officedocument.presentationml.presentation"));
				m_table.SetAt(_T("sldm"), _T("application/vnd.ms-powerpoint.slide.macroEnabled.12"));			
				m_table.SetAt(_T("sldx"), _T("application/vnd.openxmlformats-officedocument.presentationml.slide"));
				m_table.SetAt(_T("doc"), _T("application/msword"));
				m_table.SetAt(_T("docm"), _T("aapplication/vnd.ms-word.document.macroEnabled.12"));
				m_table.SetAt(_T("docx"), _T("application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
				m_table.SetAt(_T("vml"), _T("application/vnd.openxmlformats-officedocument.vmlDrawing"));
			}
			const CString operator[] (const CString& extension) const
			{
				const CAtlMap<CString, CString>::CPair* pPair = m_table.Lookup(extension);
				if (NULL == pPair)
					return _T("");
				return pPair->m_value;
			}

		private:
			CAtlMap<CString, CString>	m_table;
		};
	} // namespace ContentTypes
} // namespace OOX

#endif // OOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_