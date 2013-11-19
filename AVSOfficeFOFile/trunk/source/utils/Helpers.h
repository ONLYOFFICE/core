/*!	\file	Helpers.h
*/

#pragma once

#include <string>

namespace AVS
{
	namespace Utils
	{

		class CoUninitializeOnDestroy
		{
		public:
			~CoUninitializeOnDestroy()
			{
				CoUninitialize();
			}
		};	



		static std::string WideStringToMultiByte(const std::wstring & input_string, unsigned int codepage)
		{
			int utf8Len = WideCharToMultiByte(codepage, 0, input_string.c_str(), input_string.length(), 0, 0, 0, 0);
			std::string utf8String;
			utf8String.resize(utf8Len);
			WideCharToMultiByte(codepage, 0, input_string.c_str(), input_string.length(), (char*)utf8String.c_str(), utf8Len, 0, 0);
			return utf8String;
		}

		std::string WideStringToUTF8(const std::wstring & input_string)
		{
			return WideStringToMultiByte(input_string, CP_UTF8);
		}

		std::string WideStringToANSI(const std::wstring & input_string)
		{
			return WideStringToMultiByte(input_string, CP_ACP);		
		}

		static std::wstring MultiByteToWideString(const std::string & input_string, unsigned int codepage)
		{
			int wideStringLen = MultiByteToWideChar(codepage, 0, input_string.c_str(), input_string.length(), 0, 0);
			std::wstring wideString;
			wideString.resize(wideStringLen);
			MultiByteToWideChar(codepage, 0, input_string.c_str(), input_string.length(), (wchar_t*)wideString.c_str(), wideStringLen);
			return wideString;	
		}

		std::wstring UTF8ToWideString(const std::string & input_string)
		{
			return MultiByteToWideString(input_string, CP_UTF8);
		}

		std::wstring ANSIToWideString(const std::string & input_string)
		{
			return MultiByteToWideString(input_string, CP_ACP);
		}

		std::wstring GetComErrorDescription(void * iunknown, LPCGUID interface_uuid)
		{
			if (NULL == iunknown)
				return L"";

			std::wstring resultString = L"";
			IUnknown * inst = static_cast<IUnknown *>(iunknown);
			ISupportErrorInfo * pSupportErrorInfo = NULL;
			if SUCCEEDED(inst->QueryInterface(__uuidof(ISupportErrorInfo), (void**)&pSupportErrorInfo))
			{
				if (S_OK == pSupportErrorInfo->InterfaceSupportsErrorInfo(*interface_uuid))
				{
					IErrorInfo * pErrorInfo = NULL;

					if (S_OK == GetErrorInfo(0, &pErrorInfo))		
					{
						BSTR bsErrorInfo = NULL;
						pErrorInfo->GetDescription(&bsErrorInfo);
						if (bsErrorInfo)
						{
							resultString =  bsErrorInfo;
							::SysFreeString(bsErrorInfo);
						}
						pErrorInfo->Release();
					}				
				}
				pSupportErrorInfo->Release();			
			}
			return resultString;
		}

	}
}