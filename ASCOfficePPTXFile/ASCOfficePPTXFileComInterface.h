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
// CAVSOfficePPTXFile.h : Declaration of the CAVSOfficePPTXFile

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols

#include "../Common/OfficeFileTemplate.h"
#include "ASCOfficePPTXFile.h"
#include "../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"
#include "../Common/DocxFormat/Source/SystemUtility/File.h"

bool comExtractFile(void* pArg, CString& sFile, CString& sDir);
bool comCompressFile(void* pArg, CString& sDir, CString& sFile);
bool comProgress(void* pArg, long ID, long Percent);

// IAVSOfficePPTXFile
[object, uuid("ED1EC17E-EE0E-4cae-9E63-1C57235CE286"), dual, pointer_default(unique)]
__interface IAVSOfficePPTXFile : IAVSOfficeFileTemplate
{
//	[id(3), helpstring("method SaveAsDrawingXML")] HRESULT SaveAsDrawingXML([in] BSTR sSrcFileName, [in] BSTR sDestPath, [in] BSTR sDestXMLFileName);

	[propget, id(4), helpstring("property TempDirectory")] HRESULT TempDirectory([out, retval] BSTR* pVal);
	[propput, id(4), helpstring("property TempDirectory")] HRESULT TempDirectory([in] BSTR newVal);
	[id(5), helpstring("method GetDVDXml")] HRESULT GetDVDXml([out,retval] BSTR* pbstrPTTXml);
	[id(6), helpstring("method GetBluRayXml")] HRESULT GetBluRayXml([out,retval] BSTR* pbstrDVDXml);
	[propget, id(7), helpstring("property DrawingXml")] HRESULT DrawingXml([out, retval] BSTR* pVal);

	[id(2000 + 0)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001 + 1)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// IAVSOfficePPTXFile2
[object, uuid("4F4EA472-EC78-495c-B627-5798EA364468"), dual, pointer_default(unique)]
__interface IAVSOfficePPTXFile2 : IDispatch
{
	[id(10000 + 0)] HRESULT OpenFileToPPTY([in] BSTR bsInputDir, [in] BSTR bsFileDst);
	[id(10000 + 1)] HRESULT OpenDirectoryToPPTY([in] BSTR bsInputDir, [in] BSTR bsFileDst);

	[id(10000 + 2)] HRESULT SetMediaDir([in] BSTR bsMediaDir);
	[id(10000 + 3)] HRESULT SetFontDir([in] BSTR bsFontDir);

	[id(10000 + 4)] HRESULT SetUseSystemFonts([in] VARIANT_BOOL useSystemFonts);
	[id(10000 + 5)] HRESULT ConvertPPTYToPPTX([in] BSTR bsInputFile, [in] BSTR bsFileDst);

	[id(10000 + 6)] HRESULT SetThemesDir([in] BSTR bsThemesPath);
};

// CAVSOfficePPTXFile
[coclass, uuid("5731F488-94FF-44b7-8A3E-54CBB746F5B1"), event_source(com), threading(apartment), vi_progid("AVSOfficePPTXFile.OfficePPTXFile"), progid("AVSOfficePPTXFile.OfficePPTXFile.1"), version(1.0), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSOfficePPTXFile 
	:	public IAVSOfficePPTXFile
	,	public IAVSOfficePPTXFile2
{
private:
	CPPTXFile m_oCPPTXFile;
public:

	__event __interface _IAVSOfficeFileTemplateEvents2;
	CAVSOfficePPTXFile() : m_oCPPTXFile(comExtractFile, comCompressFile, comProgress, this)
	{
	}

	~CAVSOfficePPTXFile()
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
	HRESULT LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
	{
		return m_oCPPTXFile.LoadFromFile(sSrcFileName, sDstPath, sXMLOptions);
	}
public:
	HRESULT SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
	{
		return m_oCPPTXFile.SaveToFile(sDstFileName, sSrcPath, sXMLOptions);
	}

public:
	STDMETHOD(get_TempDirectory)(BSTR* pVal)
	{
		return m_oCPPTXFile.get_TempDirectory(pVal);
	}
	STDMETHOD(put_TempDirectory)(BSTR newVal)
	{
		return m_oCPPTXFile.put_TempDirectory(newVal);
	}
public:
	STDMETHOD(GetDVDXml)(BSTR* pbstrPTTXml)
	{
		return m_oCPPTXFile.GetDVDXml(pbstrPTTXml);
	}
	STDMETHOD(GetBluRayXml)(BSTR* pbstrDVDXml)
	{
		return m_oCPPTXFile.GetBluRayXml(pbstrDVDXml);
	}
public:
	STDMETHOD(get_DrawingXml)(BSTR* pVal)
	{
		return m_oCPPTXFile.get_DrawingXml(pVal);
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return m_oCPPTXFile.SetAdditionalParam(ParamName, ParamValue);
	}

	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return m_oCPPTXFile.GetAdditionalParam(ParamName, ParamValue);
	}

	// to PPTY
	STDMETHOD(SetMediaDir)(BSTR bsMediaDir)
	{
		return m_oCPPTXFile.SetMediaDir(bsMediaDir);
	}
	STDMETHOD(SetFontDir)(BSTR bsFontDir)
	{
		return m_oCPPTXFile.SetFontDir(bsFontDir);
	}
	STDMETHOD(SetThemesDir)(BSTR bsDir)
	{
		return m_oCPPTXFile.SetThemesDir(bsDir);
	}
	STDMETHOD(SetUseSystemFonts)(VARIANT_BOOL useSystemFonts)
	{
		return m_oCPPTXFile.SetUseSystemFonts(useSystemFonts);
	}
	STDMETHOD(OpenFileToPPTY)(BSTR bsInput, BSTR bsOutput)
	{
		return m_oCPPTXFile.OpenFileToPPTY(bsInput, bsOutput);
	}
	STDMETHOD(OpenDirectoryToPPTY)(BSTR bsInput, BSTR bsOutput)
	{
		return m_oCPPTXFile.OpenDirectoryToPPTY(bsInput, bsOutput);
	}
	STDMETHOD(ConvertPPTYToPPTX)(BSTR bsInput, BSTR bsOutput)
	{
		return m_oCPPTXFile.ConvertPPTYToPPTX(bsInput, bsOutput);
	}
	void LoadResourceFile(HINSTANCE hInst, LPCTSTR sResName, LPCTSTR sResType, const CString& strDstFile)
	{
		HRSRC hrRes = FindResource(hInst, sResName, sResType);
		if (!hrRes)
			return;

		HGLOBAL hGlobal = LoadResource(hInst, hrRes);
		DWORD sz = SizeofResource(hInst, hrRes);
		void* ptrRes = LockResource(hGlobal);

		CFile oFile;
		oFile.CreateFile(strDstFile);
		oFile.WriteFile(ptrRes, sz);

		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}
};

bool comExtractFile(void* pArg, CString& sFile, CString& sDir)
{
	COfficeUtils *pOfficeUtils = new COfficeUtils(NULL);
	if (pOfficeUtils == NULL) return false;

	BSTR bsFile = sFile.AllocSysString();
	BSTR bsDir = sDir.AllocSysString();
	
	HRESULT hr = pOfficeUtils->ExtractToDirectory( bsFile, bsDir, NULL, 0);
	
	SysFreeString(bsFile);
	SysFreeString(bsDir);

	delete pOfficeUtils;
	if(hr != S_OK)
		return false;

	return true;
}
bool comCompressFile(void* pArg, CString& sDir, CString& sFile)
{
	COfficeUtils *pOfficeUtils = new COfficeUtils(NULL);
	if (pOfficeUtils == NULL) return false;
	
	BSTR bsDir = sDir.AllocSysString();
	BSTR bsFile = sFile.AllocSysString();

	HRESULT hr = pOfficeUtils->CompressFileOrDirectory( bsDir, bsFile, -1 );
	
	SysFreeString(bsDir);
	SysFreeString(bsFile);

	delete pOfficeUtils;

	if(hr != S_OK)
		return false;
	return true;
}
bool comProgress(void* pArg, long ID, long Percent)
{
	CAVSOfficePPTXFile* pCAVSOfficePPTXFile = static_cast<CAVSOfficePPTXFile*>(pArg);
	SHORT res = 0;
	pCAVSOfficePPTXFile->OnProgressEx(ID, Percent, &res);
	return (res != 0);
}