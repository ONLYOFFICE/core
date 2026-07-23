/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "HyperlinkMoniker.h"

#include "URLMoniker.h"
#include "FileMoniker.h"
#include "CompositeMoniker.h"
#include "AntiMoniker.h"
#include "ItemMoniker.h"

namespace OSHARED
{
	BYTE Moniker_data_tmp[8] = { 0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B };
	
	static _GUID_ URLMoniker_CLSID(0x79EAC9E0, 0xBAF9, 0x11CE, Moniker_data_tmp);
	static _GUID_ FileMoniker_CLSID(0x00000303, 0x0000, 0x0000, Moniker_data_tmp);	
	static _GUID_ CompositeMoniker_CLSID(0x00000309, 0x0000, 0x0000, Moniker_data_tmp);
	static _GUID_ AntiMoniker_CLSID(0x00000305, 0x0000, 0x0000, Moniker_data_tmp);
	static _GUID_ ItemMoniker_CLSID(0x00000304, 0x0000, 0x0000, Moniker_data_tmp);


XLS::BiffStructurePtr HyperlinkMoniker::clone()
{
	return XLS::BiffStructurePtr(new HyperlinkMoniker(*this));
}
void HyperlinkMoniker::load(IBinaryReader* reader)
{
    _GUID_ clsid={};
	
	clsid.Data1 = reader->ReadUInt32();
	clsid.Data2 = reader->ReadUInt16();
	clsid.Data3 = reader->ReadUInt16();
    
	unsigned char* pData = reader->ReadBytes(8, true);
	memcpy(clsid.getData4(), pData, 8) ;
	delete pData;

	if(URLMoniker_CLSID == clsid)
	{
		data.reset(new URLMoniker);
	}
	else if (FileMoniker_CLSID == clsid)
	{
		data.reset(new FileMoniker);
	} 
	else if (CompositeMoniker_CLSID == clsid)
	{
		data.reset(new CompositeMoniker);
	} 
	else if (AntiMoniker_CLSID == clsid)
	{
		data.reset(new AntiMoniker);
	} 
	else if (ItemMoniker_CLSID == clsid)
	{
		data.reset(new ItemMoniker);
	}
	else
	{
		//throw;
	}

	if (data)
	{
		data->load(reader);
	}
}

void HyperlinkMoniker::load(XLS::CFRecord& record)
{
    _GUID_ clsid={};
	record >> clsid;

	if(URLMoniker_CLSID == clsid)
	{
		data.reset(new URLMoniker);
	}
	else if (FileMoniker_CLSID == clsid)
	{
		data.reset(new FileMoniker);
	} 
	else if (CompositeMoniker_CLSID == clsid)
	{
		data.reset(new CompositeMoniker);
	} 
	else if (AntiMoniker_CLSID == clsid)
	{
		data.reset(new AntiMoniker);
	} 
	else if (ItemMoniker_CLSID == clsid)
	{
		data.reset(new ItemMoniker);
	}
	else
	{
		//throw;
	}

	if (data)
	{
		data->load(record);
	}
}

void HyperlinkMoniker::save(XLS::CFRecord& record)
{
    _GUID_ clsid(0, 0, 0, 0);
    STR::bstr2guid(monikerClsid, clsid);
    record << clsid;
    if (data)
    {
        data->save(record);
    }
}

} // namespace OSHARED

