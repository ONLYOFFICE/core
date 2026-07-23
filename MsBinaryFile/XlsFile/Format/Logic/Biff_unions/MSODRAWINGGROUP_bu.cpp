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

#include "MSODRAWINGGROUP.h"
#include "../Biff_records/MsoDrawingGroup.h"
#include "../Biff_records/Continue.h"
#include "../Biff_structures/ODRAW/OfficeArtBStoreContainer.h"
#include "../Biff_structures/ODRAW/OfficeArtBStoreContainerFileBlock.h"

namespace XLS
{


MSODRAWINGGROUP::MSODRAWINGGROUP(const bool is_inside_chart_sheet)
:	is_inside_chart_sheet_(is_inside_chart_sheet)
{
}


MSODRAWINGGROUP::~MSODRAWINGGROUP()
{
}


BaseObjectPtr MSODRAWINGGROUP::clone()
{
	return BaseObjectPtr(new MSODRAWINGGROUP(*this));
}


// MSODRAWINGGROUP = MsoDrawingGroup *Continue
const bool MSODRAWINGGROUP::loadContent(BinProcessor& proc)
{
    MsoDrawingGroup mso_drawing_group(is_inside_chart_sheet_);
    if(!proc.mandatory(mso_drawing_group))
	{		
		return false;
	}
	m_MsoDrawingGroup = elements_.back();
	elements_.pop_back();

	proc.repeated<Continue>(0, 0);

	return true;
}

const bool MSODRAWINGGROUP::saveContent(BinProcessor& proc)
{
	auto MaxRecSize = 8224;
	proc.mandatory(*m_MsoDrawingGroup);
	auto castedGroup = static_cast<MsoDrawingGroup*>(m_MsoDrawingGroup.get());
	if(castedGroup->rgChildRec.m_OfficeArtBStoreContainer)
	{
		auto bstore = static_cast<ODRAW::OfficeArtBStoreContainer*>(castedGroup->rgChildRec.m_OfficeArtBStoreContainer.get());
		for (auto* block : bstore->rgfb)
		{
			CFRecord continueRecord(60, proc.getGlobalWorkbookInfo());
			block->save(continueRecord);

			size_t headerSize = continueRecord.getRdPtr();
			size_t filePtr = 0;

			const BYTE* data = (BYTE*)block->pict_data;
			size_t dataSize = block->pict_size;

			while (filePtr < dataSize)
			{
				size_t freeSpace;

				if (filePtr == 0)
					freeSpace = MaxRecSize - headerSize;
				else
					freeSpace = MaxRecSize;

				size_t chunkSize = (std::min)(freeSpace, dataSize - filePtr);

				XLS::Continue continueRec;

				size_t recSize = (filePtr == 0)
					? headerSize + chunkSize
					: chunkSize;

				continueRec.m_iDataSize = recSize;
				continueRec.m_pData = new char[recSize];

				if (filePtr == 0)
				{
					memcpy(
						continueRec.m_pData,
						continueRecord.getCurStaticData<BYTE>() - headerSize,
						headerSize
					);

					memcpy(
						continueRec.m_pData + headerSize,
						data + filePtr,
						chunkSize
					);
				}
				else
				{
					auto datPtr = data + filePtr;
					memcpy(
						continueRec.m_pData,
						datPtr,
						chunkSize
					);
					auto testPt = datPtr;
					auto test2 = testPt;
				}

				proc.mandatory(continueRec);

				filePtr += chunkSize;
			}
		}
	}
	return true;
}

} // namespace XLS

