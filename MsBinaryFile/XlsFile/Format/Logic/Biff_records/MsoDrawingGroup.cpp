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

#include "MsoDrawingGroup.h"
#include "../Biff_structures/ODRAW/SimpleOfficeArtContainers.h"
#include "../Biff_structures/ODRAW/OfficeArtFDGGBlock.h"
#include "../Biff_structures/ODRAW/OfficeArtBStoreContainer.h"

namespace XLS
{

MsoDrawingGroup::MsoDrawingGroup(const bool is_inside_chart_sheet)
:	rgChildRec(is_inside_chart_sheet ? ODRAW::OfficeArtRecord::CA_Chart : ODRAW::OfficeArtRecord::CA_Sheet)
{
}


MsoDrawingGroup::~MsoDrawingGroup()
{
}


BaseObjectPtr MsoDrawingGroup::clone()
{
	return BaseObjectPtr(new MsoDrawingGroup(*this));
}


void MsoDrawingGroup::readFields(CFRecord& record)
{
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	while(!recs.empty())
	{
		record.appendRawData(recs.front());
		recs.pop_front();
	}

	record >> rgChildRec;
}

void MsoDrawingGroup::writeFields(CFRecord& record)
{
	rgChildRec.save(record);
}

void MsoDrawingGroup::prepareChart(unsigned int count)
{
	if(!drawingCount)
		return;
	auto fdggblock = new ODRAW::OfficeArtFDGGBlock;
	rgChildRec.m_OfficeArtFDGGBlock = ODRAW::OfficeArtRecordPtr(fdggblock);
	fdggblock->cdgSaved = count;
	fdggblock->cspSaved = count;
	for(auto i = 0; i < count; i++)
	{
		ODRAW::OfficeArtIDCL idcl;
		idcl.cspidCur = i;
		idcl.dgid = i;
		fdggblock->Rgidcl.push_back(idcl);
	}
}

int MsoDrawingGroup::AddPict(OOX::CPath& picPath)
{
	int pictNum = -1;
	ODRAW::OfficeArtBStoreContainer *bstore;
	if(rgChildRec.m_OfficeArtBStoreContainer == nullptr)
	{
		bstore = new ODRAW::OfficeArtBStoreContainer;
		rgChildRec.m_OfficeArtBStoreContainer = ODRAW::OfficeArtRecordPtr(bstore);
	}
	else
		bstore = static_cast<ODRAW::OfficeArtBStoreContainer*>(rgChildRec.m_OfficeArtBStoreContainer.get());
	if(!drawingNames.IsInit())
		drawingNames.Init();
	if(drawingNames->find(picPath.GetPath()) == drawingNames->end())
	{
		auto fileBlock = new ODRAW::OfficeArtBStoreContainerFileBlock;
		bstore->rgfb.push_back(fileBlock);

		pictNum = bstore->rgfb.size();
		drawingNames->emplace(picPath.GetPath(), pictNum);

		DWORD fileSize = 0;
		auto result = NSFile::CFileBinary::ReadAllBytes(picPath.GetPath(), (BYTE**)&fileBlock->pict_data, fileSize);
		fileBlock->pict_size = fileSize;
		fileBlock->pict_type = picPath.GetExtention();
	}
	else
		pictNum = drawingNames->find(picPath.GetPath())->second;
	return  pictNum;
}

} // namespace XLS

