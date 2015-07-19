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
	class BiffStructure;
	typedef boost::shared_ptr<BiffStructure> BiffStructurePtr;
};

class Document
{
public:
	Document(const std::wstring & root_name);
	~Document();

	void newDoc(const std::wstring & root_name);

	static const int appendBinaryData(XLS::BiffStructurePtr & elem, const char * data, const size_t size);
	static const int appendBinaryData(XLS::BiffStructurePtr & elem, boost::shared_array<char>& pre_allocated_data, const size_t size);
	
	static Document* findDocumentByElement(XLS::BiffStructurePtr & elem);

	const std::pair<char*, size_t> getBinaryData(const int index) const;
	
	std::wstring uniq_;

private:
	//XLS::BiffStructurePtr xmlDoc;
	std::vector<std::pair<boost::shared_array<char>, size_t> > bin_data;

private:
	static std::map<std::wstring, Document*> all_documents;
};

