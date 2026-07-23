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


#include "OfficeArtContainer.h"
#include "OfficeArtDgContainer.h"
#include "OfficeArtFDG.h"
#include "OfficeArtFSPGR.h"
#include "OfficeArtFSP.h"
#include "OfficeArtFOPT.h"
#include "OfficeArtClientAnchorSheet.h"
#include "OfficeArtClientAnchorChart.h"
#include "OfficeArtClientAnchorHF.h"
#include "OfficeArtFRITContainer.h"
#include "OfficeArtFDGGBlock.h"
#include "OfficeArtColorMRUContainer.h"
#include "OfficeArtSplitMenuColorContainer.h"
#include "SimpleOfficeArtContainers.h"
//#include "Exception/WrongBiffRecord.h"
#include "OfficeArtBStoreContainer.h"
#include "OfficeArtTertiaryFOPT.h"

namespace ODRAW
{


OfficeArtContainer::OfficeArtContainer(const unsigned char recVer, const unsigned short recType, const OfficeArtClientAnchorType anchor_type)
:	OfficeArtRecord(recVer, recType), anchor_type_(anchor_type)
{
}

OfficeArtRecordPtr OfficeArtContainer::CreateOfficeArt(unsigned short type)
{
	OfficeArtRecordPtr art_record;
	switch (type)
	{
		case DggContainer:
			art_record = OfficeArtRecordPtr(new OfficeArtDggContainer(anchor_type_));	break;
		case DgContainer:
			art_record = OfficeArtRecordPtr(new OfficeArtDgContainer(anchor_type_));	break;
		case FDG:
			art_record = OfficeArtRecordPtr(new OfficeArtFDG);							break;
		case FRITContainer:
			art_record = OfficeArtRecordPtr(new OfficeArtFRITContainer);				break;
		case SpgrContainer:
			art_record = OfficeArtRecordPtr(new OfficeArtSpgrContainer(anchor_type_));	break;
		case SpContainer:
			art_record = OfficeArtRecordPtr(new OfficeArtSpContainer(anchor_type_));	break;
		case SolverContainer:
			art_record = OfficeArtRecordPtr(new OfficeArtSolverContainer);				break;
		case FSPGR:
			art_record = OfficeArtRecordPtr(new OfficeArtFSPGR);						break;
		case FSP:
			art_record = OfficeArtRecordPtr(new OfficeArtFSP);							break;
		case FOPT:
		case SecondaryFOPT:
			art_record = OfficeArtRecordPtr(new OfficeArtFOPT);							break;
		case ChildAnchor:
			art_record = OfficeArtRecordPtr(new OfficeArtChildAnchor);					break;
		case ClientAnchor:
			switch(anchor_type_)
			{
				case CA_Chart:
					art_record = OfficeArtRecordPtr(new OfficeArtClientAnchorChart);	break;
				case CA_Sheet:
					art_record = OfficeArtRecordPtr(new OfficeArtClientAnchorSheet);	break;
				case CA_HF:
					art_record = OfficeArtRecordPtr(new OfficeArtClientAnchorHF);		break;					
			}
			break;
		case ClientData:
			art_record = OfficeArtRecordPtr(new OfficeArtClientData);					break;
		case FConnectorRule:
			art_record = OfficeArtRecordPtr(new OfficeArtFConnectorRule);				break;
		case FArcRule:
			art_record = OfficeArtRecordPtr(new OfficeArtFArcRule);						break;
		case FCalloutRule:
			art_record = OfficeArtRecordPtr(new OfficeArtFCalloutRule);					break;
		case ClientTextbox:
			art_record = OfficeArtRecordPtr(new OfficeArtClientTextbox);				break;
		case FDGGBlock:
			art_record = OfficeArtRecordPtr(new OfficeArtFDGGBlock);					break;
		case ColorMRUContainer:
			art_record = OfficeArtRecordPtr(new OfficeArtColorMRUContainer);			break;
		case SplitMenuColorContainer:
			art_record = OfficeArtRecordPtr(new OfficeArtSplitMenuColorContainer);		break;
		case BStoreContainer:
			art_record = OfficeArtRecordPtr(new OfficeArtBStoreContainer);				break;
		case TertiaryFOPT:
			art_record = OfficeArtRecordPtr(new OfficeArtTertiaryFOPT);					break;
		case FPSPL:
		case FDGSL:
		case FBSE:
		case BlipJPEG_clone:
		case BlipDIB:
		case BlipTIFF:
		case BlipEMF:
		case BlipWMF:
		case BlipPICT:
		case BlipJPEG:
		case BlipPNG:
			break;
		default:
			break;
	}
	return art_record;
}

void OfficeArtContainer::loadFields(XLS::CFRecord& record)
{
	size_t container_beginning_ptr = record.getRdPtr();

	while(record.getRdPtr() < container_beginning_ptr + rh_own.recLen - 8)
	{
		if (record.getRdPtr() >= record.getDataSize()) break;

		OfficeArtRecordHeader rh_child;
		record >> rh_child;

		size_t child_beginning_ptr = record.getRdPtr();
		record.RollRdPtrBack(rh_child.size());

		OfficeArtRecordPtr art_record = CreateOfficeArt(rh_child.recType);
		
		if(art_record)
		{
			record >> *art_record;
			child_records.push_back(art_record);

			if(record.getRdPtr() != child_beginning_ptr + rh_child.recLen)
			{
				size_t record_pos = record.getRdPtr();
				OfficeArtRecordHeader rh_test;
				record >> rh_test;
				record.RollRdPtrBack(8);//sizeof(OfficeArtRecordHeader)

				OfficeArtRecordPtr test_officeArt = CreateOfficeArt(rh_test.recType);	
				
				if(record.getRdPtr() < child_beginning_ptr + rh_child.recLen)
				{
					if (record_pos + 8 < child_beginning_ptr + rh_child.recLen)
					{
						Log::warning(STR::int2wstr(child_beginning_ptr + rh_child.recLen  - record_pos, 10) + 
							L" unsigned chars were not processed while reading from OfficeArt record of type 0x" + 
							STR::int2hex_wstr(rh_child.recType, sizeof(rh_child.recType)));
					}
					if (child_beginning_ptr + rh_child.recLen  - record_pos == 8 && test_officeArt)
						record.skipNunBytes(8);
					else 
						record.skipNunBytes(child_beginning_ptr + rh_child.recLen  - record_pos);
				}
				else if(record.getRdPtr() > child_beginning_ptr + rh_child.recLen)
				{
					Log::warning(std::wstring(L"Wrong data parsed in OfficeArt record of type 0x") + 
						STR::int2hex_wstr(rh_child.recType, sizeof(rh_child.recType)));

					//Calculadora.xls
					//test next record !!!

					if (!test_officeArt)
					{
						record.RollRdPtrBack( record_pos - (child_beginning_ptr + rh_child.recLen));

						record >> rh_test;
						record.RollRdPtrBack(8);//sizeof(OfficeArtRecordHeader)
						
						OfficeArtRecordPtr test_officeArt = CreateOfficeArt(rh_test.recType);
						if (!test_officeArt)
						{
							//oops ... and where is the correct start???
							record.skipNunBytes(record.getDataSize() - record.getRdPtr());
							return;
						}
					}
				}
			}
		}
		else // If the found record is not implemented or unknown
		{
			if (rh_child.recType == 0xf150)
				break;
			if (rh_child.size() > record.getDataSize() - container_beginning_ptr)
				break;
			try
			{
				size_t sz = rh_child.size();

				record.skipNunBytes(sz); // skip art record header + data
				Log::warning(std::wstring(L"Unsupported OfficeArtRecord skipped (recType=0x") + 
					STR::int2hex_wstr(rh_child.recType, sizeof(rh_child.recType)) + std::wstring(L")"));

				if (sz < 1)
					break;
			}
			catch(...)
			{
			}
		}
	}


}

} // namespace ODRAW
