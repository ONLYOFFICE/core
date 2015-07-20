#pragma once

#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/shared_array.hpp>

class Document;
typedef boost::shared_ptr<Document> DocumentPtr;

namespace XLS
{
	class BaseObject;
	typedef boost::shared_ptr<BaseObject> BaseObjectPtr;
};

class Document
{
public:
	Document(const std::wstring & root_name);
	~Document();

	void newDoc(const std::wstring & root_name);

	static const int appendBinaryData(XLS::BaseObject* elem, const char * data, const size_t size);
	static const int appendBinaryData(XLS::BaseObject* elem, boost::shared_array<char>& pre_allocated_data, const size_t size);
	
	static Document* findDocumentByElement(XLS::BaseObject* elem);

	const std::pair<char*, size_t> getBinaryData(const int index) const;
	
//-------------------------------------------------------------
	std::wstring												uniq_;

	XLS::BaseObjectPtr											objectDoc;
	std::vector<std::pair<boost::shared_array<char>, size_t> >	bin_data;

	static std::map<std::wstring, Document*>					all_documents;
};

