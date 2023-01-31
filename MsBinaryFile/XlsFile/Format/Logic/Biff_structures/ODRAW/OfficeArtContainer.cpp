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
		case SecondaryFOPT:
		
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
							//упс ... и где начало верное???
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
