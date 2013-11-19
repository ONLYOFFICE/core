// AVSOfficeSpool.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "../Common/_Std.h"
#include "../Common/InstallPrinter.h"

// The module attribute causes WinMain to be automatically implemented for you
[ module(EXE, uuid = "{53AC170F-ACF5-4FA0-83FE-8FEA0D4D03DB}", 
		 name = "AVSOfficeSpool", 
		 helpstring = "AVSOfficeSpool 1.0 Type Library",
		 resource_name = "IDR_AVSOFFICESPOOL") ]
class CAVSOfficeSpoolModule
{
public:
// Override CAtlExeModuleT members

	int Run(int nShowCmd = 0)
	{
		CTempFile oTempFile;
		oTempFile.FillFromStdIn();
		CString sTempFile = oTempFile.GetFile();

		if ( !sTempFile )
			return E_FAIL;

		CInstallPrinter oPrinter;
		DWORD dwFormat = 0;
		CString sOutPutFile = oPrinter.GetPrinterOutPutFile("AVSPrinter", &dwFormat);
		if ( !(sOutPutFile.GetLength() > 0) )
		{
			OPENFILENAME OFN;
			OFN.lStructSize     = sizeof(OPENFILENAME);
			OFN.hwndOwner       = NULL; // no onwer window
			OFN.lpstrFilter     = (LPSTR)"Portable Document Format (*.pdf)\0*.pdf\0Portable Network Graphics (*.png)\0*.png\0JPEG-files(*.jpg)\0*.jpg\0\0";
			OFN.nFilterIndex    = 1;
			LPSTR sPath = new char[MAX_PATH];
			sPath[0] = '\0'; 
			OFN.lpstrFile       = sPath;
			OFN.nMaxFile        = MAX_PATH;
			OFN.lpstrFileTitle  = NULL;

			OFN.nMaxFileTitle   = 0;
			OFN.lpstrInitialDir = NULL;
			OFN.lpstrTitle      = (LPSTR)"SaveAsPDF";
			OFN.Flags           = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
			OFN.lpstrDefExt     = (LPSTR)"pdf";
			OFN.nFileOffset     = 0;
			OFN.nFileExtension  = 0;

			OFN.hInstance = 0;
			OFN.lCustData = 0;
			OFN.lpfnHook  = 0;
			OFN.lpstrCustomFilter = 0;
			OFN.lpTemplateName = 0;
			OFN.nMaxCustFilter = 0;


			if ( !GetSaveFileName(&OFN) )
			{			
				delete []sPath;
				return S_OK;
			}

			DWORD dwError = GetLastError();

			CString sTemp(OFN.lpstrFile);
			sOutPutFile = sTemp;
			dwFormat = OFN.nFilterIndex - 1;

			delete []sPath;
		}


		VirtualPrinter::IVirtualPrinter *pVirtualPrinter;
		HRESULT hr = ::CoCreateInstance(VirtualPrinter::CLSID_CVirtualPrinter, NULL, CLSCTX_INPROC, VirtualPrinter::IID_IVirtualPrinter, (void**)&pVirtualPrinter);

		if ( !pVirtualPrinter )
		{
			oTempFile.Delete();
			return E_FAIL;
		}

		BSTR bsTempFile = sTempFile.AllocSysString();		

		BSTR bsOutPutFile = sOutPutFile.AllocSysString();
		pVirtualPrinter->raw_PsTo(bsTempFile, bsOutPutFile, dwFormat);
		::SysFreeString(bsTempFile);
		::SysFreeString(bsOutPutFile);


		RELEASEINTERFACE( pVirtualPrinter );
		oTempFile.Delete();

		return S_OK;
	}

};
		 
