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

#include "SHAREDSTRINGS.h"
#include "..//Biff_records/SST.h"
#include "../Biff_records/Continue.h"

namespace XLS
{


SHAREDSTRINGS::SHAREDSTRINGS(const unsigned short code_page)
: code_page_(code_page)
{
	size_ = 0 ;
}


SHAREDSTRINGS::~SHAREDSTRINGS()
{
}


BaseObjectPtr SHAREDSTRINGS::clone()
{
	return BaseObjectPtr(new SHAREDSTRINGS(*this));
}


// SHAREDSTRINGS = SST *Continue
const bool SHAREDSTRINGS::loadContent(BinProcessor& proc)
{
    SST sst(code_page_);
    if(!proc.mandatory(sst))
	{
		return false;
	}
	proc.repeated<Continue>(0, 0);

	size_ = sst.rgb.size();
	return true;
}

const bool SHAREDSTRINGS::saveContent(BinProcessor& proc)
{
    if(sstPtr == nullptr)
        return false;
    proc.mandatory(*sstPtr);
    auto castedSst = static_cast<SST*>(sstPtr.get());
    if(!castedSst->rgb.empty())
    {
		const auto MaxRecordSize = 8000;
        while(!castedSst->rgb.empty())
        {
            CFRecordPtr tempRecord(new CFRecord(rt_Continue, proc.getGlobalWorkbookInfo()));
            Continue continueRecord;
            while(!castedSst->rgb.empty())
            {
                auto oldPose = tempRecord->getRdPtr();
				castedSst->rgb.at(0)->save(*tempRecord);
				if(tempRecord->getRdPtr() >= MaxRecordSize && oldPose)
                {
                    tempRecord->RollRdPtrBack(tempRecord->getRdPtr() - oldPose);
                    break;
                }
				else if(tempRecord->getRdPtr() >= MaxRecordSize && oldPose == 0)
				{
					auto totalSize = tempRecord->getRdPtr();
					const char* rawData = tempRecord->getCurStaticData<char>() - totalSize;

					// 1. Header parsing
					UINT16 cch = *reinterpret_cast<const UINT16*>(rawData);
					BYTE grbit = *reinterpret_cast<const BYTE*>(rawData + 2);

					bool fHighByte = (grbit & 0x01) != 0;
					bool fExtSt    = (grbit & 0x04) != 0;
					bool fRichSt   = (grbit & 0x08) != 0;

					size_t headerSize = 3; // 2cch + 1rbit
					if (fRichSt) headerSize += 2; // + cRun
					if (fExtSt)  headerSize += 4; // + cbExtRst

					size_t bytesPerChar = fHighByte ? 2 : 1;
					size_t totalRgbBytes = cch * bytesPerChar;

					size_t rgbStart = headerSize;
					size_t rgbEnd   = rgbStart + totalRgbBytes;

					// 2.first Chunk
					// text size calculation
					size_t availForRgbInFirstChunk = (MaxRecordSize > headerSize) ? (MaxRecordSize - headerSize) : 0;
					size_t rgbInFirstChunk = std::min(totalRgbBytes, availForRgbInFirstChunk);

					// chars alignment UTF-16
					if (fHighByte && (rgbInFirstChunk % 2 != 0)) {
						rgbInFirstChunk--;
					}

					size_t firstChunkSize = headerSize + rgbInFirstChunk;

					Continue firstChunk;
					firstChunk.m_iDataSize = firstChunkSize;
					firstChunk.m_pData = new char[firstChunkSize];
					memcpy(firstChunk.m_pData, rawData, firstChunkSize);
					proc.mandatory(firstChunk);

					size_t currentOffset = firstChunkSize;

					// 3. rgb chunk fraction
					while (currentOffset < rgbEnd)
					{
						size_t remainingRgb = rgbEnd - currentOffset;
						size_t maxPayload = MaxRecordSize - 1; // 1 byte for fHighByte!

						size_t rgbToTake = std::min(remainingRgb, maxPayload);

						// chars alignment UTF-16
						if (fHighByte && (rgbToTake % 2 != 0)) {
							rgbToTake--;
						}

						Continue textChunk;
						textChunk.m_iDataSize = rgbToTake + 1; // 1 byte for fHighByte
						textChunk.m_pData = new char[textChunk.m_iDataSize];

						// fHighByte
						textChunk.m_pData[0] = fHighByte ? 0x01 : 0x00;
						memcpy(textChunk.m_pData + 1, rawData + currentOffset, rgbToTake);

						proc.mandatory(textChunk);
						currentOffset += rgbToTake;
					}

					// 4. writing rgRun/ExtRst
					// without fHighByte!
					while (currentOffset < totalSize)
					{
						size_t tailToTake = std::min(totalSize - currentOffset, (size_t)MaxRecordSize);

						Continue tailChunk;
						tailChunk.m_iDataSize = tailToTake;
						tailChunk.m_pData = new char[tailToTake];
						memcpy(tailChunk.m_pData, rawData + currentOffset, tailToTake);

						proc.mandatory(tailChunk);
						currentOffset += tailToTake;
					}
					// 5. removing big structure and rest tempRecord
					castedSst->rgb.pop_front();
					tempRecord = CFRecordPtr(new CFRecord(rt_Continue, proc.getGlobalWorkbookInfo()));
					continue;
				}
				castedSst->rgb.pop_front();
            }
			if(tempRecord->getRdPtr())
			{
				continueRecord.m_iDataSize = tempRecord->getRdPtr();
				continueRecord.m_pData = new char[continueRecord.m_iDataSize];
				auto copyData = tempRecord->getCurStaticData<char>() - continueRecord.m_iDataSize;
				memcpy(continueRecord.m_pData, copyData, continueRecord.m_iDataSize);
				proc.mandatory(continueRecord);
			}
        }

    }
    return true;
}

int SHAREDSTRINGS::serialize(std::wostream & stream)
{
	for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); ++it)
	{
		(*it)->serialize(stream);
	}
	return 0;
}


} // namespace XLS

