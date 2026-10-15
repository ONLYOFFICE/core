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

void MsoDrawing::prepareDrawing(const DrawingType Type, const unsigned int DrawingtId, const unsigned int row1, const unsigned int col1,
		const unsigned int row2, const unsigned int col2, const unsigned int rx1, const unsigned int rx2, const unsigned int ry1,
		const unsigned int ry2, const unsigned int param)
{
	if(rgChildRec.first)
	{
		auto fdgPtr = new ODRAW::OfficeArtFDG;
		fdgPtr->rh_own.recInstance = DrawingtId;
		fdgPtr->spidCur = DrawingtId;
		rgChildRec.m_OfficeArtFDG = ODRAW::OfficeArtRecordPtr(fdgPtr);

		auto spgrContainer = new ODRAW::OfficeArtSpgrContainer(ODRAW::OfficeArtRecord::CA_Sheet);
		rgChildRec.m_OfficeArtSpgrContainer = ODRAW::OfficeArtRecordPtr(spgrContainer);

		{
			auto SpContainer = new ODRAW::OfficeArtSpContainer(ODRAW::OfficeArtRecord::CA_Sheet);
			spgrContainer->m_OfficeArtSpgrContainerFileBlock.push_back(ODRAW::OfficeArtContainerPtr(SpContainer));
			auto groupFSPGR = new ODRAW::OfficeArtFSPGR;
			groupFSPGR->xLeft = col1;
			groupFSPGR->xRight = col2;
			groupFSPGR->yTop = row1;
			groupFSPGR->yBottom = row2;
			SpContainer->m_OfficeArtFSPGR = ODRAW::OfficeArtRecordPtr(groupFSPGR);

			auto fsprPtr = new ODRAW::OfficeArtFSP;
			SpContainer->m_OfficeArtFSP = ODRAW::OfficeArtRecordPtr(fsprPtr);
			fsprPtr->shape_id = 0;
			fsprPtr->spid = DrawingtId;
			fsprPtr->fGroup = true;
			fsprPtr->fPatriarch = true;
		}
	}
	{
		auto SpContainer = new ODRAW::OfficeArtSpContainer(ODRAW::OfficeArtRecord::CA_Sheet);
		if(rgChildRec.first && rgChildRec.m_OfficeArtSpgrContainer != nullptr)
		{
			auto spgrContainer = static_cast<ODRAW::OfficeArtSpgrContainer*>(rgChildRec.m_OfficeArtSpgrContainer.get());
			spgrContainer->m_OfficeArtSpgrContainerFileBlock.push_back(ODRAW::OfficeArtContainerPtr(SpContainer));
		}
		else
			rgChildRec.m_OfficeArtSpContainer.push_back(ODRAW::OfficeArtContainerPtr(SpContainer));

		auto fsprPtr = new ODRAW::OfficeArtFSP;
		SpContainer->m_OfficeArtFSP = ODRAW::OfficeArtRecordPtr(fsprPtr);
		if(Type == DrawingType::comment)
			fsprPtr->shape_id = 0xCA;
		else
			fsprPtr->shape_id = 1;
		fsprPtr->spid = DrawingtId+1;
		fsprPtr->fHaveMaster = true;
		auto clientAnchor = new ODRAW::OfficeArtClientAnchorSheet;
		clientAnchor->colL = col1;
		clientAnchor->colR = col2;
		clientAnchor->rwT = row1;
		clientAnchor->rwB = row2;

		{
			clientAnchor->dxL = rx1;
			clientAnchor->dxR = rx2;
			clientAnchor->dyT = ry1;
			clientAnchor->dyB = ry2;
		}
		SpContainer->m_OfficeArtAnchor = ODRAW::OfficeArtRecordPtr(clientAnchor);
		auto clientData = new ODRAW::OfficeArtClientData;
		SpContainer->m_oOfficeArtClientData = ODRAW::OfficeArtRecordPtr(clientData);
		if(Type == DrawingType::comment)
		{
			auto commentOptions = new ODRAW::OfficeArtFOPT;
			{
				auto txId = new ODRAW::OfficeArtFOPTE;
				txId->opid = 0x0080;
				txId->op = DrawingtId;
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
			SpContainer->m_oOfficeArtFOPT = ODRAW::OfficeArtRecordPtr(commentOptions);
			SpContainer->extraSize += 8;
		}
		else if(Type == DrawingType::pic)
		{
			auto commentOptions = new ODRAW::OfficeArtFOPT;
			{
				auto PicOp = new ODRAW::OfficeArtFOPTE;//pib
				PicOp->opid = 0x0104;
				PicOp->fComplex = false;
				PicOp->fBid = true;
				PicOp->op = param;
				commentOptions->fopt.Text_props.push_back(ODRAW::OfficeArtFOPTEPtr(PicOp));
			}
			{
				auto PicOp = new ODRAW::OfficeArtFOPTE;
				PicOp->opid = 0x01BF;
				PicOp->fComplex = false;
				PicOp->fBid = false;
				PicOp->op = 0x100000;
				commentOptions->fopt.Text_props.push_back(ODRAW::OfficeArtFOPTEPtr(PicOp));
			}

			commentOptions->fopt.options_count += 2;

			SpContainer->m_oOfficeArtFOPT = ODRAW::OfficeArtRecordPtr(commentOptions);
		}
	}
}

} // namespace XLS

