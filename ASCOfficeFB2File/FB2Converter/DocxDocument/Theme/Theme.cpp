#include "Theme.h"
#include <string>

namespace FB2Converter
{
	static const CString s_filename = _T("\\word\\theme\\theme1.xml");

	Theme::Theme(const CString filepath)
		:m_filepath(filepath)
	{
	}

	Theme::~Theme()
	{
	}


	bool Theme::Read()
	{		
		if (m_pXmlDoc->load(static_cast<_bstr_t>(m_filepath + s_filename))  == VARIANT_TRUE)
			return true;
		return false;
	}


	bool Theme::Write(const CString filepath)
	{
		if (m_pXmlDoc->save(static_cast<_bstr_t>(filepath + s_filename)) == S_OK)
			return true;
		return false;
	}
}