#include "Core.h"
#include <string>

namespace FB2Converter
{
	static const CString s_filename = _T("\\docProps\\core.xml");

	Core::Core(const CString filepath)
		:m_filepath(filepath)
	{
	}

	Core::~Core()
	{
	}


	bool Core::Read()
	{
		if (m_pXmlDoc->load(static_cast<_bstr_t>(m_filepath + s_filename))  == VARIANT_TRUE)
			return true;
		return false;
	}


	bool Core::Write(const CString filepath)
	{
		if (m_pXmlDoc->save(static_cast<_bstr_t>(filepath + s_filename)) == S_OK)
			return true;
		return false;
	}
}