/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#pragma once

#include "resource.h"       // main symbols
#include "DocWrapper/DocxSerializer.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IAVSOfficeDocxFile2
[ object, uuid("98B1ECA8-9575-4eec-B327-8F8BA3FA232C"), dual, pointer_default(unique) ]
__interface IAVSOfficeDocxFile2: IDispatch
{
	[id(1)] HRESULT OpenFile([in] BSTR bsInputDir, [in] BSTR bsFileDst);
	[id(2)] HRESULT GetJfdoc([in] BSTR bsInputDir, [out] BSTR* bsJfdoc);

	[id(3)] HRESULT SetMediaDir([in] BSTR bsMediaDir);
	[id(4)] HRESULT SetFontDir([in] BSTR bsFontDir);

	[id(5)] HRESULT SetUseSystemFonts([in] BOOL useSystemFonts);
	[id(6)] HRESULT GetBinaryContent([in] BSTR bsTxContent, [out, satype("BYTE")] SAFEARRAY** ppBinary);

	[id(10)] HRESULT Write([in] BSTR bstrFileIn, [in] BSTR bstrFileOut);
	[id(20)] HRESULT GetXmlContent([in, satype("BYTE")] SAFEARRAY* pBinaryObj, [in] LONG lStart, [in] LONG lLength, [out] BSTR* bsXml);

	[id(2000 + 0)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001 + 1)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// _IAVSOfficeDocxFile2Events
[uuid("C3CF75C3-28FE-4b2c-A770-5952ADF4EAC2"), dispinterface]
__interface _IAVSOfficeDocxFile2Events
{
};

// CAVSOfficeDocxFile2
[ coclass, default(IAVSOfficeDocxFile2), threading(apartment), event_source(com), vi_progid("DocxFile2"), progid("DocxFile2.Rend.1"), version(1.0), uuid("CD07583A-6362-454f-A14E-542AE706FFBC") ]
class ATL_NO_VTABLE CAVSOfficeDocxFile2 : public IAVSOfficeDocxFile2
{
public:
	__event __interface _IAVSOfficeDocxFile2Events;

private:
	BinDocxRW::CDocxSerializer m_oCDocxSerializer;
	IUnknown* m_pThis;
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	
	CAVSOfficeDocxFile2()
	{
		m_pThis = NULL;
	}
	~CAVSOfficeDocxFile2()
	{
		RELEASEINTERFACE(m_pThis);
	}

public:
	STDMETHOD(OpenFile)(BSTR bsInputDir, BSTR bsFileDst)
	{
		bool bRes = m_oCDocxSerializer.saveToFile(CString(bsFileDst), CString(bsInputDir), CString(_T("")));
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(GetJfdoc)(BSTR bsInputDir, BSTR* bsJfdoc)
	{
		return S_OK;
	}
	STDMETHOD(SetMediaDir)(BSTR bsMediaDir)
	{
		return S_OK;
	}
	STDMETHOD(SetFontDir)(BSTR bsFontDir)
	{
		m_oCDocxSerializer.setFontDir(CString(bsFontDir));
		return S_OK;
	}

	STDMETHOD(SetUseSystemFonts)(BOOL useSystemFonts)
	{
		//DocWrapper::FontProcessor::useSystemFonts = (useSystemFonts == TRUE);
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (_T("EmbeddedFontsDirectory") == sParamName && ParamValue.vt == VT_BSTR)
		{		
			m_oCDocxSerializer.setEmbeddedFontsDir(CString(ParamValue.bstrVal));
			return S_OK;
		}
		else if (_T("FontDir") == sParamName && ParamValue.vt == VT_BSTR)
		{
			m_oCDocxSerializer.setFontDir(CString(ParamValue.bstrVal));
		}
		else if (_T("SaveChartAsImg") == sParamName && ParamValue.vt == VT_BOOL)
		{
			m_oCDocxSerializer.setSaveChartAsImg(VARIANT_FALSE != ParamValue.boolVal);
		}
		else if (_T("NoBase64Save") == sParamName && ParamValue.vt == VT_BOOL)
		{
			m_oCDocxSerializer.setIsNoBase64Save(VARIANT_FALSE != ParamValue.boolVal);
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetBinaryContent)(BSTR bsTxContent, SAFEARRAY** ppBinary)
	{
		unsigned char* pData = NULL;
		long lDataSize = 0;
		bool bRes = true;
		//m_oCDocxSerializer.getBinaryContent(CString(bsTxContent), &pData, lDataSize);
		if(NULL != pData && lDataSize > 0)
		{
			SAFEARRAYBOUND	rgsabound[1];
			rgsabound[0].lLbound = 0;
			rgsabound[0].cElements = lDataSize;
			LPSAFEARRAY pArray = SafeArrayCreate(VT_UI1, 1, rgsabound);

			BYTE* pDataD = (BYTE*)pArray->pvData;
			BYTE* pDataS = pData;
			memcpy(pDataD, pDataS, lDataSize);

			*ppBinary = pArray;
		}
		RELEASEARRAYOBJECTS(pData);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(Write)(BSTR bstrFileIn, BSTR bstrDirectoryOut)
	{
		CString sDirectoryOut = bstrDirectoryOut;
		CString sThemePath;
		CString sMediaPath;
		CString sEmbedPath;
		
		if (!m_oCDocxSerializer.CreateDocxFolders(sDirectoryOut, sThemePath, sMediaPath, sEmbedPath))
			return S_FALSE;
		bool bRes = m_oCDocxSerializer.loadFromFile(CString(bstrFileIn), CString(bstrDirectoryOut), CString(_T("")), CString(sThemePath.GetString()), CString(sMediaPath.GetString()), sEmbedPath);
		
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(GetXmlContent)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, BSTR* bsXml)
	{
		CString sRes;
		bool bRes = true;
		//m_oCDocxSerializer.getXmlContent((BYTE*)pBinaryObj->pvData, pBinaryObj->rgsabound[0].cElements, lStart, lLength, sRes);
		(*bsXml) = sRes.AllocSysString();
		return bRes ? S_OK : S_FALSE;
	}
private:
};