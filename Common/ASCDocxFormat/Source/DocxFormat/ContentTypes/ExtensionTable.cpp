
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ExtensionTable.h"
#include "Exception/log_range_error.h"
#include "ToString.h"


namespace OOX
{
	namespace ContentTypes
	{

		ExtensionTable::ExtensionTable()
		{
			m_table.insert(std::make_pair(L"gif",	"image/gif"));
			m_table.insert(std::make_pair(L"png",	"image/png"));
			m_table.insert(std::make_pair(L"tif",	"image/tiff"));
			m_table.insert(std::make_pair(L"tiff",	"image/tiff"));
			m_table.insert(std::make_pair(L"jpeg",	"image/jpeg"));
			m_table.insert(std::make_pair(L"jpg",	"image/jpeg"));
			m_table.insert(std::make_pair(L"jpe",	"image/jpeg"));
			m_table.insert(std::make_pair(L"jfif",	"image/jpeg"));
			m_table.insert(std::make_pair(L"rels",	"application/vnd.openxmlformats-package.relationships+xml"));
			m_table.insert(std::make_pair(L"bin",	"application/vnd.openxmlformats-officedocument.oleObject"));
			m_table.insert(std::make_pair(L"xml",	"application/xml"));
			m_table.insert(std::make_pair(L"emf",	"image/x-emf"));
			m_table.insert(std::make_pair(L"emz",	"image/x-emz"));
			m_table.insert(std::make_pair(L"wmf",	"image/x-wmf"));
			m_table.insert(std::make_pair(L"svm",	"image/svm"));
			m_table.insert(std::make_pair(L"wav",	"audio/wav"));
			m_table.insert(std::make_pair(L"xls",	"application/vnd.ms-excel"));
			m_table.insert(std::make_pair(L"xlsm",	"application/vnd.ms-excel.sheet.macroEnabled.12"));
			m_table.insert(std::make_pair(L"xlsb",	"application/vnd.ms-excel.sheet.binary.macroEnabled.12"));
			m_table.insert(std::make_pair(L"xlsx",	"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
			m_table.insert(std::make_pair(L"ppt",	"application/vnd.ms-powerpoint"));
			m_table.insert(std::make_pair(L"pptm",	"application/vnd.ms-powerpoint.presentation.macroEnabled.12"));			
			m_table.insert(std::make_pair(L"pptx",	"application/vnd.openxmlformats-officedocument.presentationml.presentation"));
			m_table.insert(std::make_pair(L"sldm",	"application/vnd.ms-powerpoint.slide.macroEnabled.12"));			
			m_table.insert(std::make_pair(L"sldx",	"application/vnd.openxmlformats-officedocument.presentationml.slide"));
			m_table.insert(std::make_pair(L"doc",	"application/msword"));
			m_table.insert(std::make_pair(L"docm",	"aapplication/vnd.ms-word.document.macroEnabled.12"));
			m_table.insert(std::make_pair(L"docx",	"application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
			m_table.insert(std::make_pair(L"vml",	"application/vnd.openxmlformats-officedocument.vmlDrawing"));
		}


		const std::string& ExtensionTable::operator[] (const std::wstring& extension) const
		{
			std::map<std::wstring, std::string>::const_iterator iter = m_table.find(ToLower(extension));
			if (iter == m_table.end())
				throw log_range_error(L"not found " + extension + L" in extension table"); 
			return iter->second;
		}

	} // namespace ContentTypes
} // namespace OOX