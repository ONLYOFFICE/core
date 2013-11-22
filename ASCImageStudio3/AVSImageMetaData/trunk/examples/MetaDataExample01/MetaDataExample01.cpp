// MetaDataExample01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "atlcomcli.h"
#include <iostream>
#include "conio.h"
#include "../../common/utils.h"

#pragma warning(push)
#pragma warning(disable:4192)
#import "../../bin/AVSImageMetaData.dll" rename_namespace("ImageMetaData")
#pragma warning(pop)

#define HR_RET(HR) if FAILED(hr = (HR)) {_ASSERTE(SUCCEEDED(hr)); std::cout << "failed at line " << __LINE__ << std::endl << std::flush; _getch(); return hr;}

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr;
	
	if (argc <= 1)
		return 0;

	HR_RET(CoInitialize(NULL));
	
	CComPtr<ImageMetaData::IImageMetaDataParser> MetaDataParser;
	HR_RET(MetaDataParser.CoCreateInstance(__uuidof(ImageMetaData::CImageMetaDataParser)));
	
	try
	{
		MetaDataParser->ParseFile(_bstr_t(argv[1]),0);
		long nMetaDataCount = MetaDataParser->AvailDataTypeCount;
		

		for (int i = 0; i < nMetaDataCount; i++)
		{
			long lMetaDataType = MetaDataParser->MetaDataType(i);
			std::wcout << L"Meta data type: " << lMetaDataType << std::endl;
		
			std::wcout << L"Specific XML:\n"; 
			CComQIPtr<ImageMetaData::IImageMetaDataCommon> iMetaData = MetaDataParser->MetaData(i);
			_bstr_t bstrResult = iMetaData->GetXML(i);
			std::wcout << bstrResult << std::endl << std::endl;

			std::wcout << L"Common XML:\n"; 
			bstrResult = iMetaData->GetCommonXML(i);
			std::wcout << bstrResult << std::endl << std::endl;
		}

			/*_bstr_t bstrCommonResult = iMetaData->GetCommonXML(0);
			std::wcout << bstrCommonResult;
			*/

			//CComQIPtr<ImageMetaData::IImageMetaDataExif> iExifMetaData = iMetaData;
		
		/*	int nOrientation = AVS::Utils::lexical_cast<int>(iExifMetaData->GetValue(274));
			int nImageWidth = AVS::Utils::lexical_cast<int>(iExifMetaData->GetValue(256));*/
		
	}
	catch(_com_error & err)
	{
		HR_RET(err.Error());
	}

	_getch();
	
	return 0;
}

