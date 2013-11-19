#include "stdafx.h"
#include "HTMLSerializer.h"
#include <fstream>
#include <iostream>      // endl
#include <direct.h>
#include "boost/filesystem.hpp"


namespace HTML
{;
const bool loadDir(const Document& doc, MSXML2::IXMLDOMElementPtr cur_dir, const boost::filesystem::wpath& path);
const bool storeDir(const Document& doc, MSXML2::IXMLDOMElementPtr cur_dir, const boost::filesystem::wpath& path_for_dir);


const bool HTMLSerializer::read(Document& doc, const _bstr_t& from)
{
	if(!boost::filesystem::exists(static_cast<wchar_t*>(from)) || !boost::filesystem::is_directory(static_cast<wchar_t*>(from)))
	{
		return false;
	}
	doc.newXMLDoc(L"root");
	MSXML2::IXMLDOMElementPtr cur_dir = doc.getRoot();

	bool res =  loadDir(doc, cur_dir, static_cast<wchar_t*>(from));
	if(res)
	{
		Log::event("\"" + std::string(static_cast<char*>(from)) + "\" loaded successfully");
		return true;
	}
	return false;
}


const bool loadDir(const Document& doc, MSXML2::IXMLDOMElementPtr cur_dir, const boost::filesystem::wpath& path)
{
	for(boost::filesystem::wdirectory_iterator it(path), itEnd; it != itEnd; ++it)
	{
		if(boost::filesystem::is_directory(it->status()))
		{
			MSXML2::IXMLDOMElementPtr dir_tag = cur_dir->GetownerDocument()->createElement(L"dir");
			dir_tag->setAttribute(L"name", it->filename().c_str());
			cur_dir->appendChild(dir_tag);
			if(!loadDir(doc, dir_tag, path / it->filename()))
			{
				return false;
			}
			continue;
		}
		if(boost::filesystem::is_regular_file(it->status()))
		{
			if(L".xml" == it->path().extension() || L".rels" == it->path().extension())
			{
				MSXML2::IXMLDOMElementPtr file_tag = cur_dir->GetownerDocument()->createElement(L"file");
				file_tag->setAttribute(L"name", it->filename().c_str());
				cur_dir->appendChild(file_tag);

				MSXML2::IXMLDOMDocument3Ptr file_xml(_T("Msxml2.DOMDocument.6.0"));
				VARIANT_BOOL res = file_xml->load(static_cast<std::wstring>(it->string()).c_str());
				if(VARIANT_TRUE != res)
				{
					Log::warning("\"" + std::string(static_cast<char*>(_bstr_t(it->string().c_str()))) + "\" file is not a valid XML file.");
					continue;
				}
				MSXML2::IXMLDOMNodePtr ins_elem  = file_xml->removeChild(file_xml->GetdocumentElement());
				file_tag->appendChild(ins_elem);
//				Log::info("\"" + std::string(static_cast<char*>(_bstr_t(it->string().c_str()))) + "\" file has been read.");
			}
			if(L".bin" == it->path().extension())
			{
				MSXML2::IXMLDOMElementPtr file_tag = cur_dir->GetownerDocument()->createElement(L"file");
				file_tag->setAttribute(L"name", it->filename().c_str());
				cur_dir->appendChild(file_tag);

				size_t file_size = static_cast<size_t>(boost::filesystem::file_size(it->path()));
				boost::shared_array<char> buffer(new char[file_size]);

				std::ifstream bin_file(static_cast<char*>(_bstr_t(it->string().c_str())));
				bin_file.read(buffer.get(), file_size);

				file_tag->setAttribute(L"format", L"bin");
				size_t bin_data_id = doc.appendBinaryData(file_tag, buffer, file_size);
				MSXML2::IXMLDOMElementPtr bin_data_id_tag = cur_dir->GetownerDocument()->createElement(L"bin_data_id");
				bin_data_id_tag->setAttribute(L"id", STR::int2str(bin_data_id).c_str());
				file_tag->appendChild(bin_data_id_tag);

//				Log::info("\"" + std::string(static_cast<char*>(_bstr_t(it->string().c_str()))) + "\" file has been read.");
			}
		}
	}
	return true;
}

const bool HTMLSerializer::write(const Document& doc, const _bstr_t& file_path)
{
	MSXML2::IXMLDOMElementPtr cur_dir = doc.getRoot();
	return storeDir(doc, cur_dir, boost::filesystem::wpath(static_cast<wchar_t*>(file_path)).remove_filename());
}


const bool storeDir(const Document& doc, MSXML2::IXMLDOMElementPtr cur_dir, const boost::filesystem::wpath& path_for_dir)
{
	if(!cur_dir)
	{
		return false;
	}
	const _bstr_t node_type = cur_dir->GetnodeName();
	_variant_t name_attrib = cur_dir->getAttribute(L"name");
	const _bstr_t node_name = _bstr_t(VT_NULL != name_attrib.vt ? name_attrib : L"");
	const boost::filesystem::wpath current_elem_path = path_for_dir / static_cast<wchar_t*>(node_name);
	if(_bstr_t(L"dir") == node_type ||  _bstr_t(L"root") == node_type)
	{
		if(node_name.length())
		{
			_wmkdir(current_elem_path.string().c_str());
		}
		MSXML2::IXMLDOMNodeListPtr children = cur_dir->GetchildNodes();
		MSXML2::IXMLDOMNodePtr item;
		while((item = children->nextNode()))
		{
			storeDir(doc, item, current_elem_path);
		}
	}
	if(_bstr_t(L"file") == node_type)
	{
		const _variant_t file_format = cur_dir->getAttribute(L"format");
		if(VT_NULL == file_format.vt || _bstr_t(file_format) == _bstr_t(L"xml"))
		{
			MSXML2::IXMLDOMDocument3Ptr xml_doc(_T("Msxml2.DOMDocument.6.0"));
			//xml_doc->PutpreserveWhiteSpace(VARIANT_TRUE); // Stores xml exactly as it was composed.
			xml_doc->loadXML(L"<?xml version=\"1.0\" standalone=\"yes\" ?> <root/>");
			xml_doc->PutRefdocumentElement(MSXML2::IXMLDOMElementPtr(cur_dir->removeChild(cur_dir->GetfirstChild())));
			xml_doc->save(current_elem_path.string().c_str());

// 			std::wofstream text_file(current_elem_path.string().c_str());
// 			MSXML2::IXMLDOMNodePtr root_node = cur_dir->GetownerDocument();
// 			if(root_node)
// 			{
// 				_bstr_t text = root_node->Getxml();
// 				text_file.write(static_cast<wchar_t*>(text), text.length());
// 			}
// 			text_file.close();
// 
// 
		}
		else if(_bstr_t(file_format) == _bstr_t(L"bin"))
		{
			const _bstr_t bin_data_id_str = _bstr_t(cur_dir->getAttribute(L"bin_data_id"));
			if(!!bin_data_id_str)
			{
				int bin_data_id = atoi(static_cast<char*>(bin_data_id_str));

				std::ofstream bin_file(current_elem_path.string().c_str());
				const std::pair<char*, size_t> bin_data = doc.getBinaryData(bin_data_id);
				bin_file.write(bin_data.first, bin_data.second);
			}
		}
		else if(_bstr_t(file_format) == _bstr_t(L"text"))
		{
			std::wofstream text_file(current_elem_path.string().c_str());
			MSXML2::IXMLDOMNodePtr root_node = cur_dir->GetfirstChild();
			if(root_node)
			{
				_bstr_t text = root_node->Gettext();
				text_file.write(static_cast<wchar_t*>(text), text.length());
			}
			text_file.close();
		}
	}
	return true;
}


} // namespace XLSX
