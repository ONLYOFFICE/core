#include "stdafx.h"
#include "EWSDocument.h"
#include "Document.h"
#include <FormatFactory/Producer.h>
#include <Serializer/Serializer.h>
#include <Converter/Converter.h>
#include <Exception/exceptions.h>
#include "boost/filesystem.hpp"


namespace EWSCORE
{;

EWSDocument::EWSDocument()
{
	doc_ = new Document(L"root");
}


EWSDocument::~EWSDocument()
{
	delete doc_;
}


bool EWSDocument::loadFrom(const _bstr_t& from_filename, const EWS_FORMAT format)
{
	try
	{
		if(Producer::getSerializer(format).read(*doc_, from_filename))
		{
			doc_->SetWorkbookNameFromFilename(from_filename);
#ifdef _DEBUG
			Producer::getSerializer(ef_XML).write(*doc_, from_filename + L"_in.xml");
#endif
		 
			if(Producer::getConverter(format).convertToX(*doc_))
			{
				return true;
			}
		}
		return false;
	}
	catch(EXCEPT::LogicalException&)
	{
		return false;
	}
	catch(EXCEPT::RuntimeException&)
	{
		return false;
	}
	catch(_com_error error)
	{
		std::string additional_descr = error.Description().length() ? std::string("Description: ") + static_cast<char*>(error.Description()) : "";
		Log::error(std::string("_com_error exception!!! HRESULT: ") + STR::int2hex_str(error.Error()) + "(" + static_cast<char*>(_bstr_t(error.ErrorMessage())) + ") "+ additional_descr);
		return false;
	}
}


bool EWSDocument::saveTo(const _bstr_t& to_file_path, const EWS_FORMAT format, _bstr_t xml_options)
{
	try
	{
		doc_->SetWorkbookNameFromFilename(to_file_path);
		boost::filesystem::wpath file_path(static_cast<wchar_t*>(to_file_path));
		if(0 == file_path.extension().length())
		{
			file_path = boost::filesystem::wpath(to_file_path + L".html");
		}
		if(Producer::getConverter(format).convertFromX(*doc_, xml_options, _bstr_t(file_path.filename().c_str())))
		{
#ifdef _DEBUG
			Producer::getSerializer(ef_XML).write(*doc_, to_file_path + L"_out.xml");
#endif

			if(Producer::getSerializer(format).write(*doc_, to_file_path))
			{
				return true;
			}
		}
		return false;
	}
	catch(EXCEPT::LogicalException&)
	{
		return false;
	}
	catch(EXCEPT::RuntimeException&)
	{
		return false;
	}
	catch(_com_error error)
	{
		std::string additional_descr = error.Description().length() ? std::string("Description: ") + static_cast<char*>(error.Description()) : "";
		Log::error(std::string("_com_error exception!!! HRESULT: ") + STR::int2hex_str(error.Error()) + "(" + static_cast<char*>(_bstr_t(error.ErrorMessage())) + ") "+ additional_descr);
		return false;
	}
}

}