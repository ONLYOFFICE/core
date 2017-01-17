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

#include "../../Common/OfficeFileTemplate.h"
#include "../TxtXmlFormatLib/Source/TxtXmlEvent.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IXmlFile
[
	object,
	//uuid("313EFCD4-D6D2-4E5D-A99A-47890DA1FAAA"),
	uuid("AA15FAE9-AB71-4f01-B3A9-59FC4D12AB34"),
	dual,	helpstring("IXmlFile Interface"),
	pointer_default(unique)
]
__interface IXmlFile : IAVSOfficeFileTemplate
{
};


// CXmlFile

[
	coclass,
	event_source(com),
	default(IXmlFile),
	threading(apartment),
	vi_progid("AVSXmlFile.XmlFile"),
	progid("AVSXmlFile.XmlFile.1"),
	version(1.0),
	//uuid("91D835EB-A37E-4AF7-8B53-F56D353E2161"),
	uuid("DD887DF2-E1A4-492A-9D87-05E0E0BC485D"),
	helpstring("XmlFile Class")
]
class ATL_NO_VTABLE CXmlFile : public IXmlFile, public TxtXml::ITxtXmlEvent
{
public:
	__event __interface _IAVSOfficeFileTemplateEvents2;
	virtual bool Progress(long ID, long Percent);

	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

	CXmlFile();
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct();
	void FinalRelease();

};