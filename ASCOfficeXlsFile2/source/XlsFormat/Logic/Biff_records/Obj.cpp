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

#include "Obj.h"
#include <Logic/Biff_records/MsoDrawing.h>
#include <Logic/Biff_records/TxO.h>
#include <Logic/Biff_structures/ODRAW/SimpleOfficeArtContainers.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtClientAnchorSheet.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtFOPT.h>


#include "../../../XlsXlsxConverter/XlsConverter.h"

#define OBJ_Group			0x0000
#define OBJ_Line			0x0001 
#define OBJ_Rectangle		0x0002 
#define OBJ_Oval			0x0003 
#define OBJ_Arc				0x0004 
#define OBJ_Text			0x0006 
#define OBJ_OfficeArt		0x001E  
#define OBJ_Polygon			0x0009 
#define OBJ_Picture			0x0008 
#define OBJ_Chart			0x0005   
#define OBJ_Button			0x0007 
#define OBJ_CheckBox		0x000B 
#define OBJ_RadioButton		0x000C  
#define OBJ_EditBox			0x000D 
#define OBJ_Label			0x000E 
#define OBJ_DialogBox		0x000F 
#define OBJ_SpinControl		0x0010 
#define OBJ_List			0x0012 
#define OBJ_GroupBox		0x0013 
#define OBJ_Scrollbar		0x0011 
#define OBJ_DropdownList	0x0014 
#define OBJ_Note			0x0019 

namespace XLS
{


Obj::~Obj()
{
}


BaseObjectPtr Obj::clone()
{
	return BaseObjectPtr(new Obj(*this));
}


void Obj::writeFields(CFRecord& record)
{
	record << cmo;
	if(0 == cmo.ot)
	{
		// FtGmo
		unsigned short ft = 0x06;
		unsigned short cb = 0x02;
		record << ft << cb; // reserved
		record.reserveNunBytes(2); // unused
	}
	if(OBJ_Picture == cmo.ot)
	{
		record << pictFormat << pictFlags;
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot)
	{
		// FtCbls
		unsigned short ft = 0x0A;
		unsigned short cb = 0x0C;
		record << ft << cb; // reserved
		record.reserveNunBytes(cb); // unused
	}
	if(0x0C == cmo.ot)
	{
		// FtRbo
		unsigned short ft = 0x0B;
		unsigned short cb = 0x06;
		record << ft << cb; // reserved
		record.reserveNunBytes(cb); // unused
	}
	if(0x10 == cmo.ot || 0x11 == cmo.ot || 0x12 == cmo.ot || 0x14 == cmo.ot)
	{
		record << sbs;
	}
	if(0x19 == cmo.ot)
	{
		record << nts;
	}
	if(false) // TODO: Find out the condition
	{
		macro.store(record);
	}
	if(OBJ_Picture != cmo.ot)
	{
		pictFmla.store(record, pictFlags);
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot || 0x10 == cmo.ot || 0x11 == cmo.ot || 0x12 == cmo.ot || 0x14 == cmo.ot)
	{
		linkFmla.store(record, cmo.ot);
	}
	if(0x0B == cmo.ot || 0x0C == cmo.ot)
	{
		checkBox.store(record);
	}
	if(0x0C == cmo.ot)
	{
		radioButton.store(record);
	}
	if(0x0D == cmo.ot)
	{
		edit.store(record);
	}
	if(0x12 == cmo.ot || 0x14 == cmo.ot)
	{
		list.store(record, cmo.ot);
	}
	if(0x13 == cmo.ot)
	{
		gbo.store(record);
	}
	if(0x12 != cmo.ot && 0x14 != cmo.ot)
	{
		record.reserveNunBytes(4); // reserved
	}
}


void Obj::readFields(CFRecord& record)
{
	record >> cmo;
	size_t rdPtr = record.getRdPtr();
	
	if (record.getGlobalWorkbookInfo()->Version >= 0x0600)
	{
		if(0 == cmo.ot)
		{
			record.skipNunBytes(6); // Skip FtGmo (obsolete)
		}
		if(OBJ_Picture == cmo.ot) 
		{
			record >> pictFormat;
			record >> pictFlags;
		}
		if(0x0B == cmo.ot || 0x0C == cmo.ot)
		{
			record.skipNunBytes(16); // Skip FtCbls (obsolete)
		}
		if(0x0C == cmo.ot)
		{
			record.skipNunBytes(10); // Skip FtRbo (obsolete)
		}
		if(0x10 == cmo.ot || 0x11 == cmo.ot || 0x12 == cmo.ot || 0x14 == cmo.ot)
		{
			record >> sbs;
		}
		if(0x19 == cmo.ot)
		{
			record >> nts;
		}
		//if(false) // TODO: Find out the condition
		//{
		//	macro.load(record);
		//}

		if(0x0B == cmo.ot || 0x0C == cmo.ot || 0x10 == cmo.ot || 0x11 == cmo.ot || 0x12 == cmo.ot /*|| 0x14 == cmo.ot*/)
		{
			linkFmla.load(record);
		}
		if(0x0B == cmo.ot || 0x0C == cmo.ot)
		{
			checkBox.load(record);
		}
		if(0x0C == cmo.ot)
		{
			radioButton.load(record);
		}
		if(0x0D == cmo.ot)
		{
			edit.load(record);
		}
		if(0x12 == cmo.ot || 0x14 == cmo.ot)
		{
			list.load(record, cmo.ot);
		}
		if(0x13 == cmo.ot)
		{
			gbo.load(record);
		}
		
		if(0x12 != cmo.ot && 0x14 != cmo.ot)
		{
			record.skipNunBytes(4); // reserved
		}
		
		if (continue_records.size() > 0)
		{
			std::list<CFRecordPtr>& recs = continue_records[rt_Continue];

			if (recs.size())
			{
				while( !recs.empty() )
				{
					mso_drawing_->storeRecordAndDecideProceeding(recs.front());
					recs.pop_front();
				}
			}
		}
	}
	else
	{
		_UINT16 nLinkSize = 0, nNameLen = 0, nMacrosSize = 0;
		
		old_version.enabled = true;
		old_version.anchor = ODRAW::OfficeArtRecordPtr(new ODRAW::OfficeArtClientAnchorSheet());
		old_version.anchor->loadFields(record);
		
		record >> nMacrosSize;
		record.skipNunBytes(2);
		record >> nNameLen;
		record.skipNunBytes(2);


		if(OBJ_Line == cmo.ot)
		{
			record >> old_version.line;
			record >> old_version.flag;
			record >> old_version.flag2;
		}	
		else
		{
			old_version.bFill = true;
			record >> old_version.fill;
			record >> old_version.line;
			
			if(OBJ_Arc == cmo.ot)
			{
				unsigned char f;
				record >> f;
				old_version.flag = f;
				record.skipNunBytes(1);
			}
			else			
				record >> old_version.flag;
		}

		if(OBJ_Picture == cmo.ot)
		{
			_UINT16 type_picture;	//2 - wm or pict;  9 - bmp
			record >> type_picture;
			record.skipNunBytes(4);
			record >> nLinkSize;
			record.skipNunBytes(2);
			record >> old_version.flag2; //0 bit - auto-resize ; 1 bit - dde enabled
			record.skipNunBytes(4);

			if (nNameLen > 0)
			{
				record >> old_version.name;
			}
			record.skipNunBytes(nMacrosSize);

			//pictFmla.load(record, nLinkSize);
			record.skipNunBytes(nLinkSize);

			if (continue_records.size() > 0)
			{//embedded
				std::list<CFRecordPtr>& recs = continue_records[rt_Continue];

				if (recs.size())
				{
					while( !recs.empty() )
					{
						record.appendRawData(recs.front()->getData(), recs.front()->getDataSize());
						recs.pop_front();
					}
				}
				int size			= record.getDataSize() - record.getRdPtr();
				const char* data	= record.getData() + record.getRdPtr();
	
				std::wstring str_type_picture;
				if (type_picture == 2) str_type_picture = L".wmf";
				if (type_picture == 9) str_type_picture = L"dib_data";
				
				std::wstring id = record.getGlobalWorkbookInfo()->xls_converter->WriteMediaFile((char*)data, size, str_type_picture);

			}
		}
		else if(OBJ_Text == cmo.ot)
		{
			TxO * txO = new TxO(mso_drawing_);
		
			txO->macrosSize = nMacrosSize;
			txO->nameLength = nNameLen;

			txO->readFields(record);
			old_version.additional.push_back(BaseObjectPtr(txO));

			old_version.name = txO->name;
		}
		else
		{
			if (nNameLen > 0)
			{
				record >> old_version.name;
			}
			record.skipNunBytes(nMacrosSize);
		}
	}

}

} // namespace XLS

