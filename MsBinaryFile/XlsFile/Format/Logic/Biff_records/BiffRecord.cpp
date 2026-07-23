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

#include "BiffRecord.h"
#include "../../Binary/CFStream.h"
#include "../../Binary/CFStreamCacheReader.h"
#include "../../Binary/CFStreamCacheWriter.h"

namespace XLS
{

BiffRecord::BiffRecord()
{
}


BiffRecord::~BiffRecord()
{
}


const bool BiffRecord::read(StreamCacheReaderPtr reader, BaseObject* parent, const bool is_mandatory)
{
	parent_ = parent;		

	if (!reader)
		return false;

	// Find and read the required record
	CFRecordPtr record = reader->getNextRecord(getTypeId(), is_mandatory);
	if(!record)
	{
		return false; // Required record hasn't been found
	}
		
	readFollowingContinue(reader);

	if(storeRecordAndDecideProceeding(record))
	{
		// Read fields data
		readFields(*record); // defined in derived classes

		size_t dataSize = record->getDataSize();
		size_t rdPtr = record->getRdPtr();
		size_t typeId = getTypeId();

		if(record->getDataSize() != record->getRdPtr() && getTypeId() != rt_ANY_TYPE/* && getTypeId() != rt_MsoDrawing*/)
		{
			Log::warning(STR::int2str(record->getDataSize() - record->getRdPtr(), 10) + " unsigned chars were not processed while reading from " + record->getTypeString());
		}


		parent->add_child(this->clone());
	}

	return true; // Record reading OK
}
const bool BiffRecord::write(StreamCacheWriterPtr writer, BaseObject* parent)
{
	parent_ = parent;

	if (!writer)
		return false;

	// Create and write the required record
	CFRecordPtr record = writer->getNextRecord(getTypeId());
	// Write fields data
	writeFields(*record); // defined in derived classes

	size_t dataSize = record->getDataSize();
	size_t rdPtr = record->getRdPtr();
	size_t typeId = getTypeId();

	writer->storeNextRecord(record);
	//parent->add_child(this->clone());

	return true; // Record reading OK
}
void BiffRecord::readFollowingContinue(StreamCacheReaderPtr reader)
{
}


const bool BiffRecord::storeRecordAndDecideProceeding(CFRecordPtr record)
{
	return true;
}


} // namespace XLS

