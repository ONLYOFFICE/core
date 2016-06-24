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
// OfficePPTFile3.h : Declaration of the CPPTFile3
#pragma once
#include "resource.h"       // main symbols

#include "../../Common/OfficeFileTemplate.h"
#include "../../Common/ATLDefine.h"
#include "../../Common/ASCUtils.h"

#include "../../Common/VideoFileErrorDescription.h"

#include "../PPTFormatLib/PPTFormatLib.h"

// IAVSOfficePPTFile
[object, uuid("01AB4BA0-7AE9-4539-8B12-608233D307BC"), dual, pointer_default(unique)]
__interface IAVSOfficePPTFile :  IAVSOfficeFileTemplate
{
	[id(100 + 0), propget] HRESULT DrawingXml( [out, retval] BSTR* pbstrXml );
    [id(100 + 1), propget] HRESULT TempDirectory( [out, retval] BSTR* pbstrDir );
    [id(100 + 1), propput] HRESULT TempDirectory( [in] BSTR bstrDir );
	
	[id(1000 + 0)] HRESULT Is_PPTFile([in] BSTR fileName, [out, retval] VARIANT_BOOL* Result);
	
	[id(1000 + 1)] HRESULT GetDVDXml([out, retval] BSTR* bstrPTTXml);
	[id(1000 + 2)] HRESULT GetBluRayXml([out, retval] BSTR* bstrDVDXml);

	[id(2000 + 0)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001 + 1)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};


// _IAVSOfficePPTFileEvents
[dispinterface,	uuid("DF3BE363-8404-4173-830A-984BF11790F5")]
__interface _IAVSOfficePPTFileEvents
{
	[id(1)] HRESULT ProcessVideoFile([in] LONG ID, [in] LONG Percent, [in, out, ref] SHORT* Cancel);
};	

// CAVSOfficePPTFile
[coclass, uuid("FC1AD867-FFA6-454a-82A0-1538465A55EB"), event_source(com), threading(apartment), vi_progid("AVSOfficePPTFile.OfficePPTFile"), progid("AVSOfficePPTFile.OfficePPTFile.1"), version(1.0)]
class ATL_NO_VTABLE CAVSOfficePPTFile : 
	public IAVSOfficePPTFile, 
	public CAVSATLError
{
public:
	CAVSOfficePPTFile() : 
	  CAVSATLError(__uuidof(CAVSOfficePPTFile), __uuidof(IAVSOfficePPTFile)), m_hSynchMutex(NULL)
	{
	}

	__event __interface _IAVSOfficePPTFileEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{	
		m_hSynchMutex = CreateMutex(NULL, FALSE, NULL);

		m_Status = 0;

		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEHANDLE ( m_hSynchMutex );
	}

private:

	COfficePPTFile PPTFile;

	HANDLE m_hSynchMutex;
	LONG m_Status;
	
public:
	STDMETHOD(get_DrawingXml)(BSTR* pbstrXml)
	{
		if (NULL == pbstrXml)
			return S_FALSE;
		return S_OK;
	}
	STDMETHOD(get_TempDirectory)(BSTR* pbstrDir)
	{
		if (NULL == pbstrDir)
			return S_FALSE;

		*pbstrDir = SysAllocString(PPTFile.get_TempDirectory().c_str());
		return S_OK;
	}
	STDMETHOD(put_TempDirectory)(BSTR bstrDir)
	{
		PPTFile.put_TempDirectory((std::wstring)bstrDir);
		return S_OK;
	}
	STDMETHOD(Is_PPTFile)(BSTR fileName, VARIANT_BOOL* Result)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);
		return S_OK;
	}

	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);

		return PPTFile.LoadFromFile(std::wstring(sSrcFileName), std::wstring(sDstPath));
	}
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
	{
		return S_OK;
	}
	
	// для конвертации с поддержкой интерактива...	
	STDMETHOD(GetDVDXml)(BSTR* bstrDVDXml)
	{
		return S_OK;
	}
	STDMETHOD(GetBluRayXml)(BSTR* bstrBluRayXml)
	{
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		if (NULL == ParamValue)
			return S_FALSE;

		PPTFile.GetAdditionalParam(CString(ParamName), ParamValue);

		return S_OK;
	}
};

