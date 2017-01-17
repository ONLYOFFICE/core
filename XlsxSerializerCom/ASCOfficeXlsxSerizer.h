/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#include "resource.h" 

#include "Reader/BinaryWriter.h"
#include "Writer/BinaryReader.h"

#include "../ASCOfficeDocxFile2/DocWrapper/XlsxSerializer.h"

#include "../ASCOfficePPTXFile/Editor/FontPicker.h"
#include "../ASCOfficeDocxFile2/DocWrapper/FontProcessor.h"

#include "../Common/OfficeFileTemplate.h"
int g_nCurFormatVersion = 0;

[
	object,
	uuid("87476A4D-6A42-44e9-A947-42B8E8613070"),
	dual,	helpstring("IAVSOfficeXlsxSerizer Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeXlsxSerizer : public IAVSOfficeFileTemplate
{
	[id(20)] HRESULT SetFontDir([in] BSTR bsFontDir);

	[id(30)] HRESULT LoadChart([in] BSTR bsFilename, [out, satype("BYTE")] SAFEARRAY** ppBinary);
	[id(31)] HRESULT SaveChart([in, satype("BYTE")] SAFEARRAY* pBinaryObj, [in] LONG lStart, [in] LONG lLength, [in] BSTR bsFilename, [in] BSTR bsContentTypePath, [out] BSTR* bsContentTypeElement);
	[id(32)] HRESULT SetDrawingConverter([in] IUnknown* pDocument);

	[id(120)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(130)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

[
	coclass,
	default(IAVSOfficeXlsxSerizer),
	event_receiver(native),
	threading(apartment),
	vi_progid("ASCOfficeXlsxSerizer.AVSDocume"),
	progid("ASCOfficeXlsxSerizer.AVSDocu.1"),
	version(1.0),
	uuid("2A6514F2-0C01-4759-89CB-68C38669337D"),
	helpstring("ASCOfficeXlsxSerizer Class")
]
class ATL_NO_VTABLE CAVSOfficeXlsxSerizer : public IAVSOfficeXlsxSerizer
{
private:
	BinXlsxRW::CXlsxSerializer				m_oXlsxSerializer;

public:
	CAVSOfficeXlsxSerizer()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
public:
	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR bstrDstPath, BSTR bstrXMLOptions)
	{
		CString sDstPath = bstrDstPath;
		CString sMediaPath;
		
		m_oXlsxSerializer.CreateXlsxFolders(CString(bstrXMLOptions), sDstPath, sMediaPath);

		bool bRes = m_oXlsxSerializer.loadFromFile(CString(sSrcFileName), sDstPath, CString(bstrXMLOptions), sMediaPath);
		return bRes ? S_OK : S_FALSE;
		return S_OK;
	}
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
	{
		bool bRes = m_oXlsxSerializer.saveToFile(CString(sDstFileName), CString(sSrcPath), CString(sXMLOptions)) ? S_OK : S_FALSE;
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(LoadChart)(BSTR bsFilename, SAFEARRAY** ppBinary)
	{
		unsigned char* pData = NULL;
		long lDataSize = 0;
		bool bRes = true;
		//m_oXlsxSerializer.loadChart(CString(bsFilename), &pData, lDataSize);
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
		return bRes ? S_OK : S_FALSE;	}
	STDMETHOD(SaveChart)(SAFEARRAY* pBinaryObj, LONG lStart, LONG lLength, BSTR bsFilename, BSTR bsContentTypePath, BSTR* bsContentTypeElement)
	{
		CString* sContentTypeElement = NULL;
		bool bRes = true;
		//m_oXlsxSerializer.saveChart(pBinaryObj, lStart, lLength, CString(bsFilename), CString(bsContentTypePath), &sContentTypeElement);
		if(NULL != sContentTypeElement)
			*bsContentTypeElement = sContentTypeElement->AllocSysString();
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(SetDrawingConverter)(IUnknown* pDocument)
	{
		return S_OK;
	}
	STDMETHOD(SetFontDir)(BSTR bsFontDir)
	{
		m_oXlsxSerializer.setFontDir(CString(bsFontDir));
		
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (_T("EmbeddedFontsDirectory") == sParamName && ParamValue.vt == VT_BSTR)
		{		
			m_oXlsxSerializer.setEmbeddedFontsDir(CString(ParamValue.bstrVal));
			return S_OK;
		}		
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

private:
	DocWrapper::FontProcessor m_oFontProcessor;
};