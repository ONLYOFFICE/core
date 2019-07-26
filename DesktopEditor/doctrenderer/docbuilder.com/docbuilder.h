/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
// docbuildercom.h : Declaration of the CDocbuilder
#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
#ifdef CreateFile
#undef CreateFile
#endif

#ifdef _WIN64
#pragma comment(lib, "../../../build/lib/win_64/doctrenderer.lib")
#else
#pragma comment(lib, "../../../build/lib/win_32/doctrenderer.lib")
#endif

#include "../../../DesktopEditor/doctrenderer/docbuilder.h"
#include "../../../Common/OfficeFileFormats.h"

// IONLYOFFICEDocBuilder
[object, uuid("0C07B7E7-86A4-42E1-8E42-2FA961992E0F"), dual, pointer_default(unique)]
__interface IONLYOFFICEDocBuilder : IDispatch
{
	[id(1)]   HRESULT CreateInstance([in] VARIANT_BOOL checkFonts);
	[id(100)] HRESULT OpenFile([in] BSTR path, [in] BSTR params, [out, retval] VARIANT_BOOL* result);
	[id(101)] HRESULT CreateFile([in] BSTR type, [out, retval] VARIANT_BOOL* result);
	[id(102)] HRESULT SetTmpFolder([in] BSTR folder);
	[id(103)] HRESULT SaveFile([in] BSTR type, [in] BSTR path, [out, retval] VARIANT_BOOL* result);
	[id(104)] HRESULT CloseFile(void);
	[id(105)] HRESULT ExecuteCommand([in] BSTR command, [out, retval] VARIANT_BOOL* result);
	[id(106)] HRESULT Run([in] BSTR path, [out, retval] VARIANT_BOOL* result);
	[id(107)] HRESULT RunText([in] BSTR commands, [out, retval] VARIANT_BOOL* result);
	[id(108)] HRESULT SetProperty([in] BSTR key, [in] BSTR value);

	[id(201)] HRESULT Initialize(void);
	[id(202)] HRESULT Dispose(void);

	[id(1001)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1002)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

static CStringW GetCurrentDllDirPath()
{
	CStringW thisPath = L"";
	WCHAR path[MAX_PATH];
	HMODULE hm;
	if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPWSTR)&GetCurrentDllDirPath, &hm))
	{
		GetModuleFileNameW(hm, path, sizeof(path));
		PathRemoveFileSpecW(path);
		thisPath = CStringW(path);
		if (!thisPath.IsEmpty() &&
			thisPath.GetAt(thisPath.GetLength() - 1) != '\\')
			thisPath += L"\\";
	}	
	return thisPath;
}

// CONLYOFFICEDocBuilder
[coclass, uuid("9BF69F3C-1506-41B9-B8EE-2839948C02E9"), threading(apartment), vi_progid("ONLYOFFICE.Builder"), progid("ONLYOFFICE.Builder.1"), version(1.0)]
class ATL_NO_VTABLE CONLYOFFICEDocBuilder : public IONLYOFFICEDocBuilder
{
protected:
	NSDoctRenderer::CDocBuilder* m_pBuilder;

public:

	CONLYOFFICEDocBuilder()
	{
		m_pBuilder = NULL;
		this->CreateInstance(VARIANT_TRUE);
	}

	~CONLYOFFICEDocBuilder()
	{
		if (NULL != m_pBuilder)
			delete m_pBuilder;
	}
	STDMETHOD(CreateInstance)(VARIANT_BOOL checkFonts)
	{
		if (NULL != m_pBuilder)
			delete m_pBuilder;

		m_pBuilder = new NSDoctRenderer::CDocBuilder();
		CStringW sCurrentDir = GetCurrentDllDirPath();
		BSTR bsCurrentDir = sCurrentDir.AllocSysString();
		m_pBuilder->SetProperty("--work-directory", (wchar_t*)bsCurrentDir);
		SysFreeString(bsCurrentDir);
		return S_OK;
	}
	STDMETHOD(OpenFile)(BSTR path, BSTR params, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		bool bRet = m_pBuilder->OpenFile(path, params);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(CreateFile)(BSTR type, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		int nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		CStringW sType = (CString)type;
		if (L"pptx" == sType)
			nType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
		else if (L"xlsx" == sType)
			nType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;

		bool bRet = m_pBuilder->CreateFile(nType);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(SetTmpFolder)(BSTR folder)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		m_pBuilder->SetTmpFolder(folder);
		return S_OK;
	}
	STDMETHOD(SaveFile)(BSTR type, BSTR path, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		int nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		CStringW sType = (CString)type;
		if (L"docx" == sType)
			nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		else if (L"doc" == sType)
			nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
		else if (L"odt" == sType)
			nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
		else if (L"rtf" == sType)
			nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
		else if (L"txt" == sType)
			nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
		else if (L"pptx" == sType)
			nType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
		else if (L"odp" == sType)
			nType = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
		else if (L"xlsx" == sType)
			nType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
		else if (L"xls" == sType)
			nType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
		else if (L"ods" == sType)
			nType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
		else if (L"csv" == sType)
			nType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
		else if (L"pdf" == sType)
			nType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
		else if (L"image" == sType)
			nType = AVS_OFFICESTUDIO_FILE_IMAGE;
		else if (L"jpg" == sType)
			nType = AVS_OFFICESTUDIO_FILE_IMAGE;
		else if (L"png" == sType)
			nType = AVS_OFFICESTUDIO_FILE_IMAGE;

		bool bRet = m_pBuilder->SaveFile(nType, path);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(CloseFile)()
	{
		if (NULL == m_pBuilder)
			return S_FALSE;
		m_pBuilder->CloseFile();
		return S_OK;
	}
	STDMETHOD(ExecuteCommand)(BSTR command, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		bool bRet = m_pBuilder->ExecuteCommand(command);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(Run)(BSTR path, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		bool bRet = m_pBuilder->Run(path);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(RunText)(BSTR commands, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		bool bRet = m_pBuilder->RunTextW(commands);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(SetProperty)(BSTR key, BSTR value)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;
		
		m_pBuilder->SetPropertyW(key, value);
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

	STDMETHOD(Initialize)()
	{
		NSDoctRenderer::CDocBuilder::Initialize();
		return S_OK;
	}
	STDMETHOD(Dispose)()
	{
		NSDoctRenderer::CDocBuilder::Dispose();
		return S_OK;
	}
};