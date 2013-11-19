#include "stdafx.h"
#include "XMLSerializer.h"

namespace XLS
{;


const bool XMLSerializer::read(Document& doc, const _bstr_t& from)
{
	return S_OK == doc.getDoc()->load(from);
}

const bool XMLSerializer::write(const Document& doc, const _bstr_t& to)
{
//	Log::info("XMLSerializer::write started \\n transformation.");
	_bstr_t xml = doc.getDoc()->Getxml();
	std::wstring wstr(static_cast<wchar_t*>(xml));
	boost::algorithm::replace_all(wstr, L"><", L">\n<");
	MSXML2::IXMLDOMDocument3Ptr full_indented_doc(_T("Msxml2.DOMDocument.6.0"));
	full_indented_doc->loadXML(wstr.c_str());
//	Log::info("XMLSerializer::write finished \\n transformation.");

	return S_OK == full_indented_doc->save(to);
}

} // namespace XLS
