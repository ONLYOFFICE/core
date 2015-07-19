#include "precompiled_xls.h"

#include <Logic\Biff_Structures\BiffStructure.h>
#include <../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h>

#include "Document.h"

std::map<std::wstring, Document*> Document::all_documents;

Document::Document(const std::wstring & root_name)
{
	//xmlDoc = boost::shared_ptr<XLS::BiffStructure>(new XLS::BiffStructure());
	
	newDoc(L"root");
}


Document::~Document()
{
	all_documents[uniq_] = NULL;
}

//
//BiffStructurePtr & Document::getRoot() const
//{
//	return xmlDoc->GetdocumentElement(); // current root independent on its name
//}
//

//BiffStructurePtr & Document::getDoc() const
//{
//	return this;
//}


void Document::newDoc(const std::wstring & root_name)
{
	if(!uniq_.empty())
	{
		all_documents.erase(uniq_);
	}

#if defined(_WIN32) || defined (_WIN64)
	UUID		uuid;
    RPC_WSTR	str_uuid;
    
	UuidCreate		(&uuid);
    UuidToString	(&uuid, &str_uuid);

	uniq_ = (TCHAR *) str_uuid;

	RpcStringFree (&str_uuid);
#else
        char pcRes[MAX_PATH] = "XXXXXX";
        pcRes[6] = '\0';

        int res = mkstemp( pcRes);

        std::string sRes = pcRes;

		uniq_ = stringUtf8ToWString(sRes);
#endif

	//xmlDoc->set_document_guid(guid_);
	//xmlDoc->set_document_name(root_name);

	//xmlDoc->loadXML(L"<?xml version=\"1.0\" encoding=\"utf-8\" ?> <" + root_name + L" id=\"" + guid_ + L"\"/>"); // Initial tag;
	
	all_documents[uniq_] = this;
}

const int Document::appendBinaryData(XLS::BiffStructurePtr &  elem, const char * data, const size_t size)
{
	if (elem == NULL) 
		return 0;

	boost::shared_array<char> buffer(new char[size]);
	memcpy_s(buffer.get(), size, data, size);
	return appendBinaryData(elem, buffer, size);
}


const int Document::appendBinaryData(XLS::BiffStructurePtr & elem, boost::shared_array<char>& pre_allocated_data, const size_t size)
{
	if (elem == NULL) 
		return 0;

	std::wstring name = elem->tagName_;
	Document* doc = findDocumentByElement(elem);
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


Document* Document::findDocumentByElement(XLS::BiffStructurePtr & elem)
{
	return all_documents[static_cast<std::wstring>(elem->get_document()->uniq_)];
}



