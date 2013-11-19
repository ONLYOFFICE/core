// TestAVSOfficeFOFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\utils\helpers.h"
using namespace AVS::Utils;

namespace AVS
{
	namespace Utils
	{
		std::string ComErrorFormat_(const char * szMessage, IUnknown * pObj, LPCGUID pGuid, HRESULT hr)
		{
			std::string res = (boost::format("%1%: %2%(hresult: %3%)") 
				% std::string(szMessage)
				% WideStringToANSI(GetComErrorDescription(pObj, pGuid))
				% boost::io::group(std::hex, std::showbase, hr)
				).str();
			return res;		
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	

	if (argc != 3)
	{
		std::cout << "Usage: <input docx folder> <output fo file>\n";
		_getch();
		return -1;
	}

	CoInitialize(NULL);
	CoUninitializeOnDestroy __couninit;
	HRESULT hr;
	CComPtr<AVSOffice::IOfficeFOFile> officeFOFile;
	if FAILED(hr = officeFOFile.CoCreateInstance(__uuidof(AVSOffice::COfficeFOFile)))
	{
		std::cout << "" << (boost::format("Cannot create instance of COfficeFOFile (%1%)\n") % boost::io::group(std::hex, std::showbase, hr)).str();
		_getch();
		return -1;
	}

	try
	{
		hr = officeFOFile->SaveToFile(_bstr_t(argv[2]), _bstr_t(argv[1]), _bstr_t(""));
	}
	catch(_com_error & comException)
	{
		std::cout << AVS::Utils::ComErrorFormat_("Operation failed ", officeFOFile, &__uuidof(AVSOffice::IOfficeFOFile), comException.WCode());
		_getch();
		return -1;		
	}

	return 0;
}

