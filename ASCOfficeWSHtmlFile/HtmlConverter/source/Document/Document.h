#pragma once

#include <boost/shared_array.hpp>

class Document;
typedef boost::shared_ptr<Document> DocumentPtr;

class Document
{
public:
	Document(const _bstr_t& root_name);
	~Document();

	MSXML2::IXMLDOMElementPtr getRoot() const;
	MSXML2::IXMLDOMDocument3Ptr getDoc() const;
	void newXMLDoc(const _bstr_t& root_name);
	void setDoc(MSXML2::IXMLDOMDocument3Ptr doc);

	static const int appendBinaryData(MSXML2::IXMLDOMElementPtr elem, const char * data, const size_t size);
	static const int appendBinaryData(MSXML2::IXMLDOMElementPtr elem, boost::shared_array<char>& pre_allocated_data, const size_t size);
	static Document* findDocumentByXMLElement(MSXML2::IXMLDOMElementPtr elem);

	const std::pair<char*, size_t> getBinaryData(const int index) const;
	void SetWorkbookNameFromFilename(_bstr_t file_path);
	_bstr_t GetWorkbookName() { return workbook_name; }

private:
	MSXML2::IXMLDOMDocument3Ptr xmlDoc;
	std::vector<std::pair<boost::shared_array<char>, size_t> > bin_data;
	_bstr_t guid_;
	_bstr_t workbook_name;

private:
	static std::map<_bstr_t, Document*> all_documents;
};

