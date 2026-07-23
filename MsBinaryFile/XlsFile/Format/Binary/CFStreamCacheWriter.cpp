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

#include "CFStreamCacheWriter.h"
#include "CFRecord.h"
#include "CFStream.h"
#include "../Logic/Biff_records/BOF.h"


namespace XLS
{

StreamCacheWriter::StreamCacheWriter(const GlobalWorkbookInfoPtr global_info)
	: global_info_(global_info)
{
}

StreamCacheWriter::~StreamCacheWriter()
{
}

// Reads the next CFRecord from the CFStream and if its type is not the desired one, caches it for the next call
CFRecordPtr StreamCacheWriter::getNextRecord(const CFRecordType::TypeId desirable_type)
{	
	return CFRecordPtr(new CFRecord(desirable_type, global_info_));
}

// Saves the next CFRecord to the stream or caches for later saving.
// Returns whether the record was saved to file or stored for later saving
bool StreamCacheWriter::storeNextRecord(CFRecordPtr record)
{
	records_cache.push_back(record);
	writeToStream(1);
	return true;
}

//---------------------------------------------------------------------------------------------------------
CFStreamCacheWriter::CFStreamCacheWriter(CFStreamPtr stream, GlobalWorkbookInfoPtr global_info)
	: StreamCacheWriter(global_info),	stream_(stream)	
{
}

CFStreamCacheWriter::~CFStreamCacheWriter()
{
}

// Saves the next CFRecord to the CFStream or caches for later saving.
// Returns whether the record was saved to file or stored for later saving
bool CFStreamCacheWriter::storeNextRecord(CFRecordPtr record)
{
	record->save(stream_);
	return true;
}

const size_t CFStreamCacheWriter::GetRecordPosition()
{
	return stream_->getStreamPointer();
}

// Actual transporting record to the CFStream and then to IStream
const size_t CFStreamCacheWriter::writeToStream(const size_t num_of_records_min_necessary)
{
	return 0;
}

//---------------------------------------------------------------------------------------------------------
BinaryStreamCacheWriter::BinaryStreamCacheWriter(boost::shared_ptr<NSBinPptxRW::CXlsbBinaryWriter> binaryStream, GlobalWorkbookInfoPtr global_info)
	: StreamCacheWriter(global_info), binaryStream_(binaryStream)
{

}

BinaryStreamCacheWriter::~BinaryStreamCacheWriter()
{

}

const size_t BinaryStreamCacheWriter::writeToStream(const size_t num_of_records_min_necessary)
{
	if(records_cache.size() >= num_of_records_min_necessary)
	{
		for (int i = 0; i < num_of_records_min_necessary; ++i)
		{
			records_cache.front()->save(*binaryStream_);
			records_cache.pop_front();
		}
	}

	return records_cache.size();
}


} // namespace XLS
