/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "../docbuilder.h"
#include "../../common/File.h"

// IASCDocBuilder
[object, uuid("B0C0E80B-2076-4C33-BC6D-3DB1B0FE9687"), dual, pointer_default(unique)]
__interface IASCDocBuilder : IDispatch
{
	[id(1)]   HRESULT CreateInstance([in] VARIANT_BOOL checkFonts);
	[id(100)] HRESULT OpenFile([in] BSTR path, [in] BSTR params, [out, retval] VARIANT_BOOL* result);
	[id(101)] HRESULT CreateFile([in] int type, [out, retval] VARIANT_BOOL* result);
	[id(102)] HRESULT SetTmpFolder([in] BSTR folder);
	[id(103)] HRESULT SaveFile([in] int type, [in] BSTR path, [out, retval] VARIANT_BOOL* result);
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

// CASCDocBuilder
[coclass, uuid("227DE4AD-D992-4CCD-9704-0F041CE908F3"), threading(apartment), vi_progid("ASCDocBuilder.DocBuilder"), progid("ASCDocBuilder.DocBuilder.1"), version(1.0)]
class ATL_NO_VTABLE CASCDocBuilder : public IASCDocBuilder
{
protected:
	NSDoctRenderer::CDocBuilder* m_pBuilder;

public:

	CASCDocBuilder()
	{
		m_pBuilder = NULL;
	}

	~CASCDocBuilder()
	{
		if (NULL != m_pBuilder)
			delete m_pBuilder;
	}
	STDMETHOD(CreateInstance)(VARIANT_BOOL checkFonts)
	{
		if (NULL != m_pBuilder)
			delete m_pBuilder;

		m_pBuilder = new NSDoctRenderer::CDocBuilder();
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
	STDMETHOD(CreateFile)(int type, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		bool bRet = m_pBuilder->CreateFile(type);
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
	STDMETHOD(SaveFile)(int type, BSTR path, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		bool bRet = m_pBuilder->SaveFile(type, path);
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