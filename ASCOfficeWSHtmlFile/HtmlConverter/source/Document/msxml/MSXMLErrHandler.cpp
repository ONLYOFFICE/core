#include "stdafx.h"
#include "MSXMLErrHandler.h"
#include <Exception/MSXMLError.h>

namespace MSXML2
{;


MSXMLErrHandler::MSXMLErrHandler(const HRESULT hres, IXMLDOMNode* object, REFIID iid, const std::string& func_name)
:	hres_(hres),
	object_(object),
	iid_(iid),
	func_name_(func_name)
{
}


MSXMLErrHandler::~MSXMLErrHandler()
{
}


const std::string& MSXMLErrHandler::toString()
{
	if(!message.empty())
	{
		return message;
	}
	if (S_OK == hres_)
	{
		return message; // I assume it is empty here always
	}
	IXMLDOMDocument3Ptr document = object_->GetownerDocument();
	if(!document)
	{
		document = IXMLDOMDocument3Ptr(object_);
	}
	IXMLDOMParseErrorPtr error = document->GetparseError();
	_bstr_t reason = error->Getreason();
	if(!reason)
	{
		message = func_name_ + " failed! Unknown error! HRESULT = " + STR::int2hex_str(hres_);
	}
	else
	{
		message = func_name_ + " failed! Error code: 0x" + STR::int2hex_str(error->GeterrorCode()) + 
			" Line: " + STR::int2str(error->Getline()) + " Message: " + static_cast<char*>(error->Getreason());
	}

	return message;
}


void MSXMLErrHandler::logErrorAndThrow()
{
	toString();
	if (S_FALSE == hres_)
	{
		Log::warning(message);
	}
	if(FAILED(hres_))
	{
		Log::error(message);
		_com_issue_errorex(hres_, object_, iid_);
	}
}

} // namespace MSXML2