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

#include "resource.h" 
#include "../../Common/OfficeFileTemplate.h"

// IOfficeDocFile
[
	object,
	uuid("C694C5ED-201C-46BA-B159-1D7C907FA023"),
	dual,	helpstring("IOfficeDocFile Interface"),
	pointer_default(unique)
]
__interface IOfficeDocFile : IAVSOfficeFileTemplate
{
};

// CASCOfficeDocFile

[
	coclass,
	default(IOfficeDocFile, _IAVSOfficeFileTemplateEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeDocFile.DocFile"),
	progid("AVSOfficeDocFile.DocFile.1"),
	version(1.0),
	uuid("CFABE3C9-A61B-4C20-86F4-A19C8FA1546C"),
	helpstring("OfficeDocFile Class")
]

class ATL_NO_VTABLE CASCOfficeDocFile: public IOfficeDocFile
{
public:

	CASCOfficeDocFile()
	{

	}

	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	// OfficeFileTemplate Methods
public:

	STDMETHOD(LoadFromFile)(BSTR bsDocFile, BSTR bsDocxFilePath, BSTR bsXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

protected:

	static void OnProgressFunc (LPVOID lpParam, long nID, long nPercent);
	static void OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop);
};