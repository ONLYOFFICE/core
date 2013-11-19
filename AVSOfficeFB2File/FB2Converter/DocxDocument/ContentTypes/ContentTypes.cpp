#include "ContentTypes.h"
#include <string>

namespace FB2Converter
{
	static const CString s_filename = _T("\\[Content_Types].xml");

	ContentTypes::ContentTypes(const CString filepath)
		:m_filepath(filepath)
	{
	}

	ContentTypes::~ContentTypes()
	{
	}


	bool ContentTypes::Read()
	{
		if (m_pXmlDoc->load(static_cast<_bstr_t>(m_filepath + s_filename))  == VARIANT_TRUE)
			return true;
		return false;
	}


	bool ContentTypes::Write(const CString filepath)
	{
		m_pXmlDoc->save(static_cast<_bstr_t>(filepath + s_filename) );
		return true;
	}
}