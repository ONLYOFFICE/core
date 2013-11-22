// rawconverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <conio.h>

#import "x:/AVS/Redist/AVSImageRaw3.dll" rename_namespace("AVS") raw_interfaces_only
#import "x:/AVS/Redist/AVSMediaCore3.dll" rename_namespace("AVS") raw_interfaces_only
#import "x:/AVS/Redist/AVSImageFile3.dll" rename_namespace("AVS") raw_interfaces_only

HRESULT LoadRawDefault(BSTR _FileName, IUnknown ** _UncompressedVideoFrame)
{
#define HR_RET(HR) do {if FAILED(hr = (HR)) { _ASSERTE(SUCCEEDED(hr)); return hr;}} while(0)
    HRESULT hr;
    _ASSERTE(NULL != _UncompressedVideoFrame);
    CComPtr<AVS::IImageRaw3> imageRaw3;
    HR_RET(imageRaw3.CoCreateInstance(__uuidof(AVS::CImageRaw3)));
    HR_RET(imageRaw3->OpenFile(CComBSTR(_FileName)));

    // Можно выставлять либо CameraWhiteBalance="1"
    // либо CameraMatrix="1"
    // (другое значение при этом ="0")
    //
    // CameraWhiteBalance в большинстве случаев получается больше похожей на preview

    const wchar_t * settingsXML = L"<ImageRaw3 mode=\"0\" xmlns=\"http://tempuri.org/ImageRaw3Options.xsd\">\
        <SimpleMode ver=\"1\"><CameraWhiteBalance use=\"1\"/><CameraMatrix use=\"0\"/>\
        </SimpleMode></ImageRaw3>";

    HR_RET(imageRaw3->ApplyXML(CComBSTR(settingsXML), 0 ));

    CComPtr<IUnknown> unkImage;
    if FAILED(hr = imageRaw3->GetPreviewImage(&unkImage))
    {
        _ASSERTE(SUCCEEDED(hr) && L"Failed IImageRaw3::GetPreviewImage");
        // может быть хотя бы thumbnail удастся открыть?
        HR_RET(imageRaw3->GetThumbnailImage(&unkImage));
    };

    return unkImage.CopyTo(_UncompressedVideoFrame);
#undef HR_RET
}

#define HR_RET_C(HR) do {if FAILED(hr = (HR)) { _ASSERTE(SUCCEEDED(hr)); std::cout << "failed. press any key..."; _getch(); return 1;}} while(0)

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc < 2)
        return 1;
        
    HRESULT hr;
    HR_RET_C(CoInitialize(NULL));

    std::wcout << L"Convert raw image\n\"" << argv[1] << "\"...";
    CComPtr<IUnknown> unkImage;
    HR_RET_C(LoadRawDefault(CComBSTR(argv[1]), &unkImage));
    std::wcout << L"succeed.\n";

    CComPtr<AVS::IImageFile3> imageFile;
    HR_RET_C(imageFile.CoCreateInstance(__uuidof(AVS::ImageFile3)));
    
    VARIANT_BOOL succeededRes;
    std::wstring outputName = std::wstring(argv[1]) + std::wstring(L".bmp");
    std::wcout << L"\nSave to\n\"" << outputName <<"\"\n";
    HR_RET_C(imageFile->SaveImage2(&(unkImage.p), 1, _bstr_t(outputName.c_str()), &succeededRes));
    if (VARIANT_TRUE != succeededRes)
        HR_RET_C(E_FAIL);
    
	return 0;
}

