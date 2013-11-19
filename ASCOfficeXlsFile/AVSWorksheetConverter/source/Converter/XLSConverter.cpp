#include "stdafx.h"
#include "XLSConverter.h"
#include <Document/Document.h>
#include <fstream>
#include <iostream>      // endl
#include "boost/filesystem.hpp"

namespace XLS
{;


XLSConverter::XLSConverter()
{
}


const bool XLSConverter::convertFromX(Document& doc)
{
	MSXML2::IXMLDOMDocument3Ptr xslt_from_X(_T("Msxml2.FreeThreadedDOMDocument.6.0"));
	xslt_from_X->Putasync(VARIANT_TRUE);
	xslt_from_X->setProperty(L"SelectionNamespaces", L"xmlns:xsl='http://www.w3.org/1999/XSL/Transform'");
	std::vector<std::wstring> resources;

	wchar_t file_path[MAX_PATH];
	GetModuleFileNameW(ATL::_AtlBaseModule.GetModuleInstance(), file_path, MAX_PATH);
	std::wstring dll_file_name = boost::filesystem::wpath(file_path).filename();

	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2012"); // x2xls.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2013"); // x2xls_Globals.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2014"); // propertiesTree.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2015"); // x2xls_Worksheet.xsl

	if(!XMLSTUFF::loadXSLTFromResources(xslt_from_X, resources))
	{
		return false;
	}
	Log::event("XLSX to XLS XSLT table loaded from resources successfully");
#ifdef _DEBUG
	// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
	//xslt_from_X->save(L"../InFiles/convert.xsl");
	// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
#endif
	if(!convert(doc, xslt_from_X))
	{
		return false;
	}
	Log::event("The internal XLSX XML converted to XLS XML successfully");
	return true;
}


const bool XLSConverter::convertToX(Document& doc)
{
	MSXML2::IXMLDOMDocument3Ptr xslt_to_X(_T("Msxml2.FreeThreadedDOMDocument.6.0"));
	xslt_to_X->setProperty(L"SelectionNamespaces", L"xmlns:xsl='http://www.w3.org/1999/XSL/Transform'");
	xslt_to_X->Putasync(VARIANT_TRUE);
	std::vector<std::wstring> resources;

	wchar_t file_path[MAX_PATH];
	GetModuleFileNameW(ATL::_AtlBaseModule.GetModuleInstance(), file_path, MAX_PATH);
	std::wstring dll_file_name = boost::filesystem::wpath(file_path).filename();

	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2001"); // xls2x.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2003"); // xls2x_rels.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2005"); // xls2x_wb.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2006"); // xls2x_ws.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2008"); // xls2x_styles.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2009"); // xls2x_sst.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2011"); // xls2x_comments.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2014"); // propertiesTree.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2018"); // xls2x_drawing.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2019"); // xls2x_chart.xsl
	resources.push_back(L"res://" + dll_file_name + L"/XSL/#2020"); // xls2x_cs.xsl

	if(!XMLSTUFF::loadXSLTFromResources(xslt_to_X, resources))
	{
		return false;
	}
	Log::event("XLS to XLSX XSLT table loaded from resources successfully");

#ifdef _DEBUG
// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
	//xslt_to_X->save(L"../InFiles/convert.xsl");	

	
	boost::filesystem::path curr_path = boost::filesystem::current_path();		
	xslt_to_X->save(L"c:/Temp/for_test/InFiles/convert.xsl");

//	Log::event("../InFiles/convert.xsl saved successfully");
// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
	DWORD time_before = GetTickCount();
#endif

	if(!convert(doc, xslt_to_X))
	{
		return false;
	}
#ifdef _DEBUG
	DWORD  ticks_spent = GetTickCount() - time_before;
	std::cout << "XSLT conversion took " << ticks_spent / 1000 << " seconds (" << ticks_spent << " ms)" << std::endl;
#endif
	Log::event("The internal XLS XML converted to XLSX XML successfully");
	return true;
}


} // namespace XLS

