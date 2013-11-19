#include "App.h"
#include <string>

namespace FB2Converter
{
	static const CString s_filename = _T("\\docProps\\app.xml");

	App::App(const CString filepath)
		:m_filepath(filepath)
	{
	}

	App::~App()
	{
	}


	bool App::Read()
	{
		if (m_pXmlDoc->load(static_cast<_bstr_t>(m_filepath + s_filename))  == VARIANT_TRUE)
			return true;
		return false;
	}


	bool App::Write(const CString filepath)
	{
		if (m_pXmlDoc->save(static_cast<_bstr_t>(filepath + s_filename)) == S_OK)
			return true;
		return false;
	}
}