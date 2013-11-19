#include "stdafx.h"
#include "EWSDocument.h"
#include "Document.h"
#include <FormatFactory/Producer.h>
#include <Serializer/Serializer.h>
#include <Converter/Converter.h>
#include <Exception/exceptions.h>


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
#ifdef _DEBUG
		DWORD time_before = GetTickCount();
#endif
		if(Producer::getSerializer(format).read(*doc_, from_filename))
		{
#ifdef _DEBUG
			DWORD  ticks_spent = GetTickCount() - time_before;
			Log::info("Binary reading took " + STR::int2str(ticks_spent / 1000) + " seconds (" + STR::int2str(ticks_spent) + " ms)");

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
	catch(EXCEPT::StopConversionException&)
	{
		Log::error("Conversion stopped.");
		return false;
	}
	catch(_com_error error)
	{
		std::string additional_descr = error.Description().length() ? std::string("Description: ") + static_cast<char*>(error.Description()) : "";
		Log::error(std::string("_com_error exception!!! HRESULT: ") + STR::int2hex_str(error.Error()) + "(" + static_cast<char*>(_bstr_t(error.ErrorMessage())) + ") "+ additional_descr);
		return false;
	}
}


bool EWSDocument::saveTo(const _bstr_t& to_folder, const EWS_FORMAT format)
{
	try
	{
		if(Producer::getConverter(format).convertFromX(*doc_))
		{
#ifdef _DEBUG
			Producer::getSerializer(ef_XML).write(*doc_, to_folder + L"_out.xml");
#endif

			if(Producer::getSerializer(format).write(*doc_, to_folder))
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
	catch(EXCEPT::StopConversionException&)
	{
		Log::error("Conversion stopped.");
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