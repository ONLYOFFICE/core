#include "WebSettings.h"
#include <string>

namespace FB2Converter
{
	static const CString s_filename = _T("\\word\\webSettings.xml");

	WebSettings::WebSettings(const CString filepath)
		:m_filepath(filepath)
	{
	}

	WebSettings::~WebSettings()
	{
	}


	bool WebSettings::Read()
	{
		if (m_pXmlDoc->load(static_cast<_bstr_t>(m_filepath + s_filename))  == VARIANT_TRUE)
			return true;
		return false;
	}


	bool WebSettings::Write(const CString filepath)
	{
		if (m_pXmlDoc->save(static_cast<_bstr_t>(filepath + s_filename)) == S_OK)
			return true;
		return false;
	}
}