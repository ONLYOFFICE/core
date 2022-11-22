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

#include "MsoDrawing.h"
#include "../Biff_structures/ODRAW/OfficeArtRecord.h"

namespace XLS
{

MsoDrawing::MsoDrawing(const bool is_inside_chart_sheet)
:	rgChildRec(is_inside_chart_sheet ? ODRAW::OfficeArtRecord::CA_Chart : ODRAW::OfficeArtRecord::CA_Sheet)
{
	isReading = false;
}


MsoDrawing::~MsoDrawing()
{
}


BaseObjectPtr MsoDrawing::clone()
{
	return BaseObjectPtr(new MsoDrawing(*this));
}

void MsoDrawing::resetToBegin()
{
	if (stored_record)
	{
		stored_record->resetPointerToBegin();
	}
}

void MsoDrawing::readFields()
{
	if (stored_record)
	{
		rgChildRec.rh_own.recLen = stored_record->getDataSize();

		rgChildRec.loadFields(*stored_record);
		
		if (stored_record->getRdPtr()  < stored_record->getDataSize())
		{
			int g = 0;
		}
	}
	isReading = true;
}

void MsoDrawing::readFields(CFRecord& record)
{
	ODRAW::OfficeArtRecordHeader rh_test;
	record >> rh_test;
	record.RollRdPtrBack(8);//sizeof(OfficeArtRecordHeader)
	
	if (rh_test.recType == 0xF002) //OfficeArtDgContainer
	{
		record >> rgChildRec;
		isReading = true;
	}
	else if ((rh_test.recType & 0xF000) == 0xF000)
	{
		//074_JKH.OPEN.INFO.PRICE.VO_зПТПДУЛЙЕ ПЛТХЗБ юЕМСВЙОУЛПК ПВМБУФЙ_пбп юЕМСВЙОУЛПЕ БЧЙБРТЕДРТЙСФЙЕ.xls
		rgChildRec.rh_own.recLen = record.getDataSize();
		rgChildRec.loadFields(record); 
		isReading = true;
	}
	else
	{
	}
}


void MsoDrawing::useContinueRecords(CFRecord& record)
{
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	while(!recs.empty())
	{
		record.appendRawData(recs.front());
		recs.pop_front();
	}
}


const bool MsoDrawing::isStartingRecord(CFRecord& record)
{
	return ODRAW::OfficeArtDgContainer::CheckIfContainerStartFound(record);
}


const bool MsoDrawing::isEndingRecord(CFRecord& record)
{
	return ODRAW::OfficeArtDgContainer::CheckIfContainerSizeOK(record);
}



} // namespace XLS

