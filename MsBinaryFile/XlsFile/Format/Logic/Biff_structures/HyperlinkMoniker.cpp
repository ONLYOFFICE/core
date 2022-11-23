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


} // namespace OSHARED

