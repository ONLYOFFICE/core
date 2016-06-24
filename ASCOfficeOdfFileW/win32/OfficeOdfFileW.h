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
// OfficeOdfFileW.h : Declaration of the COfficeOdfFileW

#pragma once
#include "resource.h"       // main symbols

#include <string>

#include "../../Common/OfficeFileTemplate.h"
#include "../../Common/OfficeFileErrorDescription.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IOfficeOdfFileW
[
	object,
	uuid("E171F3C8-5715-41AB-9678-85AF4B7CE2FE"),
	dual,	helpstring("IOfficeOdfFileW Interface"),
	pointer_default(unique)
]
__interface IOfficeOdfFileW : IAVSOfficeFileTemplate
{
};


// _IOfficeOdfFileWEvents
[
	dispinterface,
	uuid("BF816058-77B7-497A-9204-1AE5A37956FC"),
	helpstring("_IOfficeOdfFileWEvents Interface")
]
__interface _IOfficeOdfFileWEvents
{
};


// COfficeOdfFileW

[
	coclass,
	default(IOfficeOdfFileW, _IAVSOfficeFileTemplateEvents),
	threading(apartment),
	event_source(com),
	vi_progid("ASCOfficeOdfFileW.OfficeOdfFileW"),
	progid("ASCOfficeOdfFileW.OfficeOdfFileW.1"),
	version(1.0),
	uuid("E1D4E9D1-69A7-494B-B2A0-0F7F37449044"),
	helpstring("OfficeOdfFileW Class")
]
class ATL_NO_VTABLE COfficeOdfFileW :
	public IOfficeOdfFileW
{
public:
	COfficeOdfFileW()
	{
	}

	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		defaultWinFontPath = GetDefWinFontDirectory();
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
    STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

private:
	HRESULT SaveToFileImpl(const std::wstring & srcPath, const std::wstring & srcTempPath,
						   const std::wstring & dstTempPath,	const std::wstring & fontPath, const std::wstring & dstFileName);

	std::wstring DetectTypeDocument(const std::wstring & Path);
	std::wstring GetDefWinFontDirectory();

	std::wstring defaultWinFontPath;

protected:

	static void OnProgressFunc (LPVOID lpParam, long nID, long nPercent);
	static void OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop);

};

