// VirtualPrinter.h : Declaration of the CVirtualPrinter

#pragma once
#include "resource.h"       // main symbols
#include "PostScriptAPI.h"
#include "../Common/InstallPrinter.h"
#include <vector>
#include <map>
#include <hash_map>
#include <list>
#include "../Common/Registry.h"
#include <Exdisp.h>
#include <docobj.h.>
#include <process.h>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// _IVirtualPrinterEvents
[ uuid("F25244D8-0B1A-4192-9402-8676F104A144"), dispinterface]
__interface _IVirtualPrinterEvents
{
	[id(10)]		HRESULT StartConverting();
	[id(11)]		HRESULT EndConverting();
	[id(12)]        HRESULT HTMLFile();
	[id(13)]        HRESULT OpenHTMLinIE();
	[id(14)]        HRESULT PreparingDataForPrinting();
	[id(15)]        HRESULT Printing(); // после этого сообщения печать нельзя останавливать


	[id(30)]		HRESULT CriticalError();
	[id(31)]        HRESULT Cancel();
};


// IVirtualPrinter
[
	object,
	uuid("64FC40A7-DA41-4048-8C3E-EA280ADB3A05"),
	dual,	helpstring("IVirtualPrinter Interface"),
	pointer_default(unique)
]
__interface IVirtualPrinter : IDispatch
{
	[id(101), helpstring("method PsTo")] HRESULT PsTo(BSTR bsSrcPath, BSTR bsDstPath, LONG nFormat);
	[id(102), helpstring("method InstallPrinter")] HRESULT InstallPrinter(void);
	[id(103), helpstring("method UnInstallPrinter")] HRESULT UnInstallPrinter(void);
	[id(104), helpstring("method SaveAs")] HRESULT SaveAs(BSTR bsSrcPath, BSTR bsDstPath, LONG nFormat);
	[id(105), helpstring("method CancelConverting")] HRESULT CancelConverting(void);
};



// CVirtualPrinter

[
	coclass,
	default(IVirtualPrinter),
	threading(apartment),
	vi_progid("AVSVirtualPrinter.VirtualPrinter"),
	progid("AVSVirtualPrinter.VirtualPrinter.1"),
	version(1.0),
	event_source(com),
	uuid("3FB83529-7AD3-4DEB-BA5C-BE8CE870BCE5"),
	helpstring("VirtualPrinter Class")
]
class ATL_NO_VTABLE CVirtualPrinter :
	public IVirtualPrinter
{
public:

	__event __interface _IVirtualPrinterEvents;

protected:

	BOOL    m_bConverting;

	HANDLE  m_hThread;
	DWORD   m_dwThreadID;

	CString m_sDst;
	CString m_sSrc;
	LONG    m_nDstFormat;

public:
	CVirtualPrinter()
	{
	}



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_bConverting = FALSE;

		m_sDst = "";
		m_sSrc = "";
		m_nDstFormat = 0;
		m_hThread = NULL;
		
		return S_OK;
	}

	void FinalRelease()
	{
		if ( m_hThread )
		{
			RELEASEHANDLE(m_hThread);
		}
	}

public:

	/*
	Функция конвертирующая Post Script формат в формат указанный в параметре nFormat. 
	nFormat:
	0 - PDF
	1 - PNG
	2 - JPEG (Quality = 90)
	*/
	STDMETHOD(PsTo)(BSTR bsSrcPath, BSTR bsDstPath, LONG nFormat)
	{
		
		PS::CSettings oSettings;
		switch( nFormat )
		{
		case 0:
			oSettings.SetDevice( PS::CSettings::EDevice::PDFWriter );
			break;
		case 1:
			oSettings.SetDevice( PS::CSettings::EDevice::PNGWriter );
			break;
		case 2:
			oSettings.SetDevice( PS::CSettings::EDevice::JPEGWriter );
			break;
		default:
			return S_OK;
		}
		oSettings.SetOutputFile( CString(bsDstPath) );
		PS::CAPI oMainDevice(oSettings);
		if ( !oMainDevice.IsDllLoaded() )
			return FALSE;
		oMainDevice.RunFile(CString(bsSrcPath));

		return S_OK;

	}
	/*
	Функция установки принтера. Для ОС WinXP, WinVista
	*/
	STDMETHOD(InstallPrinter)(void)
	{
		CInstallPrinter oPrinter;

		if ( oPrinter.InstallWindowsPrinter("AVSMonitor", "AVSPort:", "AVSDriver", "AVSPrinter") )
			return S_OK;
		else
			return E_FAIL;
	}
	/*
	Деинсталяцция принтера
	*/
	STDMETHOD(UnInstallPrinter)(void)
	{
		CInstallPrinter oPrinter;
		if ( oPrinter.UnInstallWindowsPrinter("AVSMonitor", "AVSPort:", "AVSDriver", "AVSPrinter") )
			return S_OK;
		else
			return E_FAIL;
	}
	/*
	Конвертация html файлов в формат указанный в nFormat.
	nFormat:
	0 - PDF
	1 - PNG
	2 - JPEG (Quality = 90)
	*/
	STDMETHOD(SaveAs)(BSTR bsSrcPath, BSTR bsDstPath, LONG nFormat)
	{
		HANDLE        hThread;
		unsigned int  dwThreadID;

		m_sDst = CString(bsDstPath);
		m_sSrc = CString(bsSrcPath);
		m_nDstFormat = nFormat;

		m_hThread = (HANDLE)CreateThread( NULL, 0, &_ThreadProc, (void*)this, 0, &m_dwThreadID);
		WaitForSingleObject( m_hThread, 0);

		return S_OK;
	}

	/*
	Отмена конвертации
	*/

	STDMETHOD(CancelConverting)(void)
	{
		Cancel(); // event
		m_bConverting = FALSE;
		return S_OK;
	}
protected:

	static DWORD WINAPI _ThreadProc(void *pArgs)
	{
		CVirtualPrinter *pThis = (CVirtualPrinter *)pArgs;
		return pThis->ConvertingToPDF(pThis);
	}

	DWORD ConvertingToPDF(CVirtualPrinter *pThis)
	{		
		
		m_bConverting = TRUE;
		
		BSTR bsDstPath = m_sDst.AllocSysString();
		BSTR bsSrcPath = m_sSrc.AllocSysString();

		BOOL bReturn = TRUE;

		if( m_bConverting )
			StartConverting(); // event

		// сначала проверяем установлен ли нужный принтер
		CInstallPrinter oPrinter;
		if ( !oPrinter.WindowsPrinterIsInstalled("AVSMonitor", "AVSPort:", "AVSDriver", "AVSPrinter") )
			return E_FAIL;


		CString sDstPath(bsDstPath);
		oPrinter.SetPrinterOutPutFile("AVSPrinter", "AVSDriver", "AVSPort:", sDstPath, m_nDstFormat);

		DWORD nBufLen = 0;
		GetDefaultPrinter( NULL, &nBufLen);
		LPSTR sDefaultPrinter = new char[nBufLen];
		GetDefaultPrinter( sDefaultPrinter, &nBufLen);

		SetDefaultPrinter((LPSTR)"AVSPrinter");

		CString sSrcPath(bsSrcPath);

		if ( -1 != sSrcPath.Find(".htm") )
		{
			HTMLFile(); // event

			IWebBrowser2 *pBrowser2;
			::CoCreateInstance( CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pBrowser2);
			if (pBrowser2)
			{
				VARIANT vEmpty;
				VariantInit(&vEmpty);       
				HRESULT hr = pBrowser2->Navigate(bsSrcPath, &vEmpty, &vEmpty, &vEmpty, &vEmpty);
				if (SUCCEEDED(hr))
				{
					// надо избавиться от заголовков IE


					LPCSTR sHeader = (LPCSTR)"&d";
					LPCSTR sFooter = (LPCSTR)"&t";

					VARIANT     vHeader;
					VariantInit(&vHeader);
					vHeader.vt      = VT_BSTR;
					vHeader.bstrVal = CString(sHeader).AllocSysString(); 

					BSTR bsHeader = L"&";//CString(sHeader).AllocSysString();
					BSTR bsFooter = L"&";//CString(sFooter).AllocSysString();

					VARIANT     vFooter;
					VariantInit(&vFooter);
					vFooter.vt      = VT_BSTR;
					vFooter.bstrVal = CString(sFooter).AllocSysString();

					long nIndex;

					SAFEARRAYBOUND   parameter_array_bound[1];
					SAFEARRAY       *parameter_array = NULL;

					parameter_array_bound[0].cElements = 2;
					parameter_array_bound[0].lLbound   = 0;

					//parameter_array = SafeArrayCreate( VT_VARIANT, 1, parameter_array_bound);

					//nIndex = 0;
					//hr    = SafeArrayPutElement(parameter_array, &nIndex, &vHeader);
					//nIndex = 1;
					//hr    = SafeArrayPutElement(parameter_array, &nIndex, &vFooter);

					parameter_array = SafeArrayCreate( VT_VARIANT, 1, parameter_array_bound);

					nIndex = 0;
					hr    = SafeArrayPutElement(parameter_array, &nIndex, &vHeader);
					nIndex = 1;
					hr    = SafeArrayPutElement(parameter_array, &nIndex, &vFooter);

					VARIANT vParameter;
					VariantInit(&vParameter);
					V_VT(&vParameter) = VT_ARRAY | VT_BYREF;
					//V_ARRAY(&vParameter) = parameter_array;
					vParameter.pparray = &parameter_array;



					OpenHTMLinIE(); // event

					VARIANT_BOOL bBusy = TRUE;
					while ( bBusy && m_bConverting )
					{
						Sleep(10);
						pBrowser2->get_Busy( &bBusy );
					}


					OLECMDF oleCommand = OLECMDF_INVISIBLE;
					while ( oleCommand != (OLECMDF_SUPPORTED | OLECMDF_ENABLED) && m_bConverting )
					{
						Sleep(10);
						pBrowser2->QueryStatusWB( OLECMDID_PRINT, &oleCommand );
					}

					if ( m_bConverting )
					{
						hr = pBrowser2->ExecWB( OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, &vEmpty, &vEmpty);

						if ( SUCCEEDED(hr) )
						{
							PreparingDataForPrinting(); // event

							while ( 0 == oPrinter.GetPrinterJobsCount("AVSPrinter") && m_bConverting )
								Sleep(10);

							if ( m_bConverting )
							{
								Printing(); // event 

								while ( 0 != oPrinter.GetPrinterJobsCount("AVSPrinter") )
									Sleep(10);
							}
						}
					}
				}
				else
				{
					pBrowser2->Quit();
				}
				pBrowser2->Quit();
				pBrowser2->Release();
			}
			bReturn = TRUE;
		}
		else
		{
			SHELLEXECUTEINFO oShellInfo;
			oShellInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
			oShellInfo.fMask        = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
			oShellInfo.hwnd         = NULL;
			oShellInfo.lpVerb       = (LPSTR)"Print";
			oShellInfo.lpFile       = (LPSTR)sSrcPath.GetBuffer();
			oShellInfo.lpParameters = NULL;
			oShellInfo.lpDirectory  = 0;
			oShellInfo.nShow        = SW_HIDE;

			bReturn = ShellExecuteEx( &oShellInfo );

			if ( m_bConverting )
				PreparingDataForPrinting(); // event 

			while ( 0 == oPrinter.GetPrinterJobsCount("AVSPrinter") )
			{
				Sleep(10);
				if ( !m_bConverting )
				{
					if (oShellInfo.hProcess)
					{
				
						BOOL bReturn = TerminateProcess(oShellInfo.hProcess, 0);
						bReturn = TerminateProcess(oShellInfo.hProcess, 0);
						bReturn = TerminateProcess(oShellInfo.hProcess, 0);
						DWORD dwError = GetLastError();
						int i = 1;
						RELEASEHANDLE(oShellInfo.hProcess);

					}
					break;
				}
			}

			if ( m_bConverting )
				Printing(); // event

			while ( 0 != oPrinter.GetPrinterJobsCount("AVSPrinter") )
			{
				Sleep(10);
				if ( !m_bConverting )
				{

					break;
				}
			}
			if (oShellInfo.hProcess)
			{
				RELEASEHANDLE(oShellInfo.hProcess);
			}
			//while ( 1 )
			//{ 
			//	DWORD nWaitResult = WaitForSingleObject(oShellInfo.hProcess, 0);
			//	if ( WAIT_TIMEOUT != nWaitResult && WAIT_OBJECT_0 != nWaitResult )
			//	{
			//		bReturn = FALSE;
			//		break;
			//	}
			//	else if ( WAIT_OBJECT_0 == nWaitResult )
			//		break;
			//}
		}
		SetDefaultPrinter(sDefaultPrinter);
		delete []sDefaultPrinter;

		oPrinter.SetPrinterOutPutFile("AVSPrinter", "AVSDriver", "AVSPort:", "");


		::SysFreeString(bsDstPath);
		::SysFreeString(bsSrcPath);

		if (m_bConverting) 
			EndConverting(); // event

		m_bConverting = FALSE;

		if ( bReturn )
			return S_OK;
		else 
			return E_FAIL;

		return 0;

	}
};

