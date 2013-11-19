#pragma once

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

private:
	MSXML2::IXMLDOMDocument3Ptr xmlDoc;
	std::vector<std::pair<boost::shared_array<char>, size_t> > bin_data;
	_bstr_t guid_;

private:
	static std::map<_bstr_t, Document*> all_documents;
};

