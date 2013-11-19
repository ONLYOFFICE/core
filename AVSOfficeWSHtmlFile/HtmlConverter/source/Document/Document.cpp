#include "stdafx.h"
#include "Document.h"
#include "boost/filesystem.hpp"


//#import <msxml6.dll>

std::map<_bstr_t, Document*> Document::all_documents;


Document::Document(const _bstr_t& root_name)
:	xmlDoc(_T("Msxml2.DOMDocument.6.0"))
{
	xmlDoc->setProperty(L"SelectionNamespaces", L"xmlns:xlsx='http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure'");

	newXMLDoc(L"root");
}


Document::~Document()
{
	all_documents[guid_] = NULL;
}


MSXML2::IXMLDOMElementPtr Document::getRoot() const
{
	return xmlDoc->GetdocumentElement(); // current root independent on its name
}


MSXML2::IXMLDOMDocument3Ptr Document::getDoc() const
{
	return xmlDoc;
}


void Document::newXMLDoc(const _bstr_t& root_name)
{
	if(!!guid_)
	{
		all_documents.erase(guid_);
	}
	GUID guid = {0};
	CoCreateGuid(&guid);
	guid_ = STR::guid2bstr(guid);
	xmlDoc->loadXML(L"<?xml version=\"1.0\" encoding=\"utf-8\" ?> <" + root_name + 
		L" id=\"" + guid_ + L"\"/>"); // Initial tag;
	all_documents[guid_] = this;
}


void Document::setDoc(MSXML2::IXMLDOMDocument3Ptr doc)
{
	xmlDoc = doc;
}


const int Document::appendBinaryData(MSXML2::IXMLDOMElementPtr elem, const char * data, const size_t size)
{
	boost::shared_array<char> buffer(new char[size]);
	memcpy_s(buffer.get(), size, data, size);
	return appendBinaryData(elem, buffer, size);
}


const int Document::appendBinaryData(MSXML2::IXMLDOMElementPtr elem, boost::shared_array<char>& pre_allocated_data, const size_t size)
{
	Document* doc = findDocumentByXMLElement(elem);
	if(!doc)
	{
		return -1;
	}
	doc->bin_data.push_back(std::pair<boost::shared_array<char>, size_t>(pre_allocated_data, size));
	return doc->bin_data.size() - 1;
}


const std::pair<char*, size_t> Document::getBinaryData(const int index) const
{
	if(index < 0 || static_cast<size_t>(index) > bin_data.size() - 1)
	{
		return std::pair<char*, size_t>(NULL, 0);
	}
	return std::pair<char*, size_t>(bin_data[index].first.get(), bin_data[index].second);
}


Document* Document::findDocumentByXMLElement(MSXML2::IXMLDOMElementPtr elem)
{
	return all_documents[static_cast<_bstr_t>(elem->GetownerDocument()->GetdocumentElement()->getAttribute(L"id"))];
}


void Document::SetWorkbookNameFromFilename(_bstr_t file_path)
{
	workbook_name = boost::filesystem::wpath(static_cast<wchar_t*>(file_path)).stem().c_str();

}


