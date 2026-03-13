/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "../Biff_structures/ODRAW/OfficeArtFDG.h"
#include "../Biff_structures/ODRAW/OfficeArtFSP.h"
#include "../Biff_structures/ODRAW/OfficeArtFSPGR.h"
#include "../Biff_structures/ODRAW/SimpleOfficeArtContainers.h"
#include "../Biff_structures/ODRAW/OfficeArtFOPT.h"
#include "../Biff_structures/ODRAW/OfficeArtClientAnchorChart.h"
#include "../Biff_structures/ODRAW/OfficeArtClientAnchorSheet.h"
#include "../Biff_structures/ODRAW/SimpleOfficeArtContainers.h"

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

void MsoDrawing::writeFields(CFRecord& record)
{
	rgChildRec.save(record);
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

void MsoDrawing::prepareComment(const unsigned int CommentId, const unsigned int row, const unsigned int col)
{
	if(rgChildRec.first)
	{
		auto spgrContainer = new ODRAW::OfficeArtSpgrContainer(ODRAW::OfficeArtRecord::CA_Sheet);
		rgChildRec.m_OfficeArtSpgrContainer = ODRAW::OfficeArtRecordPtr(spgrContainer);
		auto ShapeGroup = new ODRAW::OfficeArtSpContainer(ODRAW::OfficeArtRecord::CA_Sheet);
		auto groupFsp = new ODRAW::OfficeArtFSP;
		ShapeGroup->m_OfficeArtFSP = ODRAW::OfficeArtRecordPtr(groupFsp);
		groupFsp->shape_id = 0;
		groupFsp->fGroup = true;
		groupFsp->fPatriarch = true;
		groupFsp->spid = CommentId;

		auto groupFSPGR = new ODRAW::OfficeArtFSPGR;
		ShapeGroup->m_OfficeArtFSPGR = ODRAW::OfficeArtRecordPtr(groupFSPGR);

		auto fdgPtr = new ODRAW::OfficeArtFDG;
		fdgPtr->rh_own.recInstance = CommentId;
		rgChildRec.m_OfficeArtFDG = ODRAW::OfficeArtRecordPtr(fdgPtr);

		spgrContainer->m_OfficeArtSpgrContainerFileBlock.push_back(ODRAW::OfficeArtContainerPtr(ShapeGroup));
	}

	auto TextboxContainer = new ODRAW::OfficeArtSpContainer(ODRAW::OfficeArtRecord::CA_Sheet);
	TextboxContainer->extraSize += 8;

	if(rgChildRec.first)
	{
		auto spgrContainer = static_cast<ODRAW::OfficeArtSpgrContainer*>(rgChildRec.m_OfficeArtSpgrContainer.get());
		spgrContainer->m_OfficeArtSpgrContainerFileBlock.push_back(ODRAW::OfficeArtContainerPtr(TextboxContainer));
	}
	else
		rgChildRec.m_OfficeArtSpContainer.push_back(ODRAW::OfficeArtContainerPtr(TextboxContainer));

	auto fsprPtr = new ODRAW::OfficeArtFSP;
	TextboxContainer->m_OfficeArtFSP = ODRAW::OfficeArtRecordPtr(fsprPtr);
	fsprPtr->shape_id = 0xCA;
	fsprPtr->spid = CommentId+1;
	fsprPtr->fHaveAnchor = true;
	fsprPtr->fHaveSpt = true;

	auto clientAnchor = new ODRAW::OfficeArtClientAnchorSheet;
	clientAnchor->colL = col+1;
	clientAnchor->colR = col+3;
	clientAnchor->rwT = row;
	clientAnchor->rwB = row+4;

	{
		auto commentOptions = new ODRAW::OfficeArtFOPT;
		{
			auto txId = new ODRAW::OfficeArtFOPTE;
			txId->opid = 0x0080;
			txId->op = CommentId;
			commentOptions->fopt.Text_props.push_back(ODRAW::OfficeArtFOPTEPtr(txId));
		}
		{
			auto txId = new ODRAW::OfficeArtFOPTE;
			txId->opid = 0x008B;
			txId->op = 2;
			commentOptions->fopt.Text_props.push_back(ODRAW::OfficeArtFOPTEPtr(txId));
		}
		{
			auto txId = new ODRAW::OfficeArtFOPTE;
			txId->opid = 0x00BF;
			txId->op = 0x00080008;
			commentOptions->fopt.Text_props.push_back(ODRAW::OfficeArtFOPTEPtr(txId));
		}
		{
			auto txId = new ODRAW::OfficeArtFOPTE;
			txId->opid = 0x0158;
			txId->op = 0x0000;
			commentOptions->fopt.Text_props.push_back(ODRAW::OfficeArtFOPTEPtr(txId));
		}

		{
			auto txId = new ODRAW::OfficeArtFOPTE;
			txId->opid = 0x0181;
			txId->op = 0x08000050;
			commentOptions->fopt.Text_props.push_back(ODRAW::OfficeArtFOPTEPtr(txId));
		}
		{
			auto txId = new ODRAW::OfficeArtFOPTE;
			txId->opid = 0x03BF;
			txId->op = 0x00020002;
			commentOptions->fopt.Text_props.push_back(ODRAW::OfficeArtFOPTEPtr(txId));
		}

		commentOptions->fopt.options_count += 6;
		TextboxContainer->m_oOfficeArtFOPT = ODRAW::OfficeArtRecordPtr(commentOptions);
	}

	TextboxContainer->m_OfficeArtAnchor = ODRAW::OfficeArtRecordPtr(clientAnchor);
	auto clientData = new ODRAW::OfficeArtClientData;
	TextboxContainer->m_oOfficeArtClientData = ODRAW::OfficeArtRecordPtr(clientData);
}

void MsoDrawing::prepareChart(const unsigned int chartId, const unsigned int x1, const unsigned int x2,
		const unsigned int y1, const unsigned int y2, const unsigned int x1Offset, const unsigned int x2Offset,
		const unsigned int y1Offset,const unsigned int y2Offset)
{
	if(rgChildRec.first)
	{
		auto fdgPtr = new ODRAW::OfficeArtFDG;
		fdgPtr->rh_own.recInstance = chartId;
		fdgPtr->spidCur = chartId;
		rgChildRec.m_OfficeArtFDG = ODRAW::OfficeArtRecordPtr(fdgPtr);

		auto spgrContainer = new ODRAW::OfficeArtSpgrContainer(ODRAW::OfficeArtRecord::CA_Chart);
		rgChildRec.m_OfficeArtSpgrContainer = ODRAW::OfficeArtRecordPtr(spgrContainer);

		{
			auto SpContainer = new ODRAW::OfficeArtSpContainer(ODRAW::OfficeArtRecord::CA_Chart);
			spgrContainer->m_OfficeArtSpgrContainerFileBlock.push_back(ODRAW::OfficeArtContainerPtr(SpContainer));
			auto groupFSPGR = new ODRAW::OfficeArtFSPGR;
			groupFSPGR->xLeft = x1;
			groupFSPGR->xRight = x2;
			groupFSPGR->yTop = y1;
			groupFSPGR->yBottom = y2;
			SpContainer->m_OfficeArtFSPGR = ODRAW::OfficeArtRecordPtr(groupFSPGR);

			auto fsprPtr = new ODRAW::OfficeArtFSP;
			SpContainer->m_OfficeArtFSP = ODRAW::OfficeArtRecordPtr(fsprPtr);
			fsprPtr->shape_id = 0;
			fsprPtr->spid = chartId-1;
			fsprPtr->fGroup = true;
			fsprPtr->fPatriarch = true;
		}
	}
	{
		auto SpContainer = new ODRAW::OfficeArtSpContainer(ODRAW::OfficeArtRecord::CA_Chart);
		if(rgChildRec.first && rgChildRec.m_OfficeArtSpgrContainer != nullptr)
		{
			auto spgrContainer = static_cast<ODRAW::OfficeArtSpgrContainer*>(rgChildRec.m_OfficeArtSpgrContainer.get());
			spgrContainer->m_OfficeArtSpgrContainerFileBlock.push_back(ODRAW::OfficeArtContainerPtr(SpContainer));
		}
		else
			rgChildRec.m_OfficeArtSpContainer.push_back(ODRAW::OfficeArtContainerPtr(SpContainer));

		auto fsprPtr = new ODRAW::OfficeArtFSP;
		SpContainer->m_OfficeArtFSP = ODRAW::OfficeArtRecordPtr(fsprPtr);
		fsprPtr->shape_id = 1;
		fsprPtr->spid = chartId;
		fsprPtr->fHaveMaster = true;
		auto clientAnchor = new ODRAW::OfficeArtClientAnchorSheet;
		clientAnchor->colL = x1;
		clientAnchor->dxL = x1Offset;
		clientAnchor->colR = x2;
		clientAnchor->dxR = x2Offset;
		clientAnchor->rwT = y1;
		clientAnchor->dyT = y1Offset;
		clientAnchor->rwB = y2;
		clientAnchor->dyB = y2Offset;
		SpContainer->m_OfficeArtAnchor = ODRAW::OfficeArtRecordPtr(clientAnchor);
		auto clientData = new ODRAW::OfficeArtClientData;
		SpContainer->m_oOfficeArtClientData = ODRAW::OfficeArtRecordPtr(clientData);
	}
}

} // namespace XLS

