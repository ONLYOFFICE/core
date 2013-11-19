#include "DocxRels.h"
#include <string>

namespace FB2Converter
{
	static const CString s_filename = _T("\\_rels\\.rels");

	DocxRels::DocxRels(const CString filepath)
		:m_filepath(filepath)
	{
	}

	DocxRels::~DocxRels()
	{
	}


	bool DocxRels::Read()
	{
		if (m_pXmlDoc->load(static_cast<_bstr_t>(m_filepath + s_filename))  == VARIANT_TRUE)
			return true;
		return false;
	}


	bool DocxRels::Write(const CString filepath)
	{
		if (m_pXmlDoc->save(static_cast<_bstr_t>(filepath + s_filename)) == S_OK)
			return true;
		return false;
	}
}