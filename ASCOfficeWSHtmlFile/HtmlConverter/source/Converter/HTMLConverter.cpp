#include "stdafx.h"
#include "HTMLConverter.h"
#include <Document/Document.h>
#include <fstream>
#include <iostream>      // endl
#include "boost/filesystem.hpp"

namespace HTML
{;

// static const GUID CLSID_XSLTemplate = { 0x2933bf93, 0x7b36, 0x11d2, { 0xb2, 0x0e, 0x00, 0xc0, 0x4f, 0x98, 0x3e, 0x60 } };

HTMLConverter::HTMLConverter()
//:	xslt_x2h(_T("Msxml2.XSLTemplate.6.0"))
{
// 	CoCreateInstance(CLSID_XSLTemplate, NULL, CLSCTX_SERVER, 
// 		__uuidof(MSXML2::IXSLTemplate), (LPVOID*)(&xslt_x2h));
// 	MSXML2::IXMLDOMDocument3Ptr xslt_from_X = PrepareX2H_XSLT();
// 	xslt_x2h->PutRefstylesheet(xslt_from_X);

}

MSXML2::IXMLDOMDocument3Ptr HTMLConverter::PrepareX2H_XSLT()
{
	MSXML2::IXMLDOMDocument3Ptr xslt_from_X(_T("Msxml2.FreeThreadedDOMDocument.6.0"));//DOMDocument
	xslt_from_X->Putasync(VARIANT_FALSE);
	xslt_from_X->setProperty(L"SelectionNamespaces", L"xmlns:xsl='http://www.w3.org/1999/XSL/Transform'");

	wchar_t file_path[MAX_PATH];
	GetModuleFileNameW(ATL::_AtlBaseModule.GetModuleInstance(), file_path, MAX_PATH);

	std::wstring dll_file_name =  boost::filesystem::wpath(file_path).filename();
	
	//std::vector<std::wstring> resources;
	//resources.push_back(L"res://" + dll_file_name + L"/XSL/#2016"); // x2htm.xml
	//resources.push_back(L"res://" + dll_file_name + L"/XSL/#2017"); // x2htm_index.xml
	//resources.push_back(L"res://" + dll_file_name + L"/XSL/#2018"); // x2htm_sheet.xml
	//resources.push_back(L"res://" + dll_file_name + L"/XSL/#2019"); // x2htm_tabs.xml
	//resources.push_back(L"res://" + dll_file_name + L"/XSL/#2020"); // x2htm_css.xml
	//resources.push_back(L"res://" + dll_file_name + L"/XSL/#2021"); // x2htm_tools.xml
	//if(!XMLSTUFF::loadXSLTFromResources(xslt_from_X, resources))
	//{
	//	return MSXML2::IXMLDOMDocument3Ptr(NULL);
	//}	
	std::vector<int> resources;
	resources.push_back(2016); // x2htm.xml
	resources.push_back(2017); // x2htm_index.xml
	resources.push_back(2018); // x2htm_sheet.xml
	resources.push_back(2019); // x2htm_tabs.xml
	resources.push_back(2020); // x2htm_css.xml
	resources.push_back(2021); // x2htm_tools.xml

	if(!XMLSTUFF::loadXSLTFromResources2(xslt_from_X, resources))
	{
		return MSXML2::IXMLDOMDocument3Ptr(NULL);
	}	
	Log::event("XLSX to HTML XSLT table loaded from resources successfully");
#ifdef _DEBUG
	// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
	//xslt_from_X->save(L"../InFiles/convert.xsl");
	// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
#endif
	return xslt_from_X;
}


const bool HTMLConverter::convertFromX(Document& doc, const _bstr_t& xml_options, const _bstr_t& file_name)
{
	MSXML2::IXSLTemplatePtr xslt_x2h(_T("Msxml2.XSLTemplate.6.0"));
	MSXML2::IXMLDOMDocument3Ptr xslt_from_X = PrepareX2H_XSLT();
	if(NULL == xslt_from_X)
	{
		return false;
	}
	xslt_x2h->PutRefstylesheet(xslt_from_X);

	#pragma region Parse converter options
	_bstr_t single_file = L"false";
	_bstr_t print_file = L"false";
	_bstr_t print_index = L"1";
	MSXML2::IXMLDOMDocument3Ptr options(_T("Msxml2.DOMDocument.6.0"));
	if(xml_options.length() != 0 && VARIANT_TRUE == options->loadXML(xml_options))
	{
		MSXML2::IXMLDOMElementPtr single_file_xml = options->selectSingleNode(L"Options/HTML/SingleFile");
		if(NULL != single_file_xml)
		{
			single_file = single_file_xml->getAttribute(L"val");
		}
		MSXML2::IXMLDOMElementPtr print_file_xml = options->selectSingleNode(L"Options/HTML/Print");
		if(NULL != print_file_xml)
		{
			print_file = print_file_xml->getAttribute(L"val");
			print_index = print_file_xml->getAttribute(L"ind");
		}
	}
	#pragma endregion

	MSXML2::IXSLProcessorPtr processor = xslt_x2h->createProcessor();
	processor->Putinput(_variant_t(doc.getDoc().GetInterfacePtr()));
	processor->addParameter("workbook_filename", file_name, _bstr_t(L""));
	processor->addParameter("charset", _bstr_t(L"UTF-8"), _bstr_t(L""));
	processor->addParameter("single_file", single_file, _bstr_t(L""));
	processor->addParameter("print_file", print_file, _bstr_t(L""));
	processor->addParameter("print_index", print_index, _bstr_t(L""));
	if(VARIANT_TRUE != processor->transform())
	{
		return false;
	}

	if(VARIANT_TRUE != doc.getDoc()->loadXML(_bstr_t(processor->Getoutput())))
	{
		return false;
	}

	Log::event("The internal XLSX XML converted to HTML XML successfully");
	return true;
}


const bool HTMLConverter::convertToX(Document& doc)
{
	return false;
}


} // namespace XLS

