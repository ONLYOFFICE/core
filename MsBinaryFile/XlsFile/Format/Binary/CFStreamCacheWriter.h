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
#pragma once

#include "BinSmartPointers.h"
#include "../Logic/GlobalWorkbookInfo.h"
#include "CFRecordType.h"
#include "../../../../OOXML/Binary/Presentation/BinaryFileReaderWriter.h"

namespace XLS
{

// The purpose of the writer is to avoid seeking back through IStream when, for example, 
// we have write an offset of a record that would appear in processing much later.
// For example this is necessary in 'Index' record that shall reference DBCell records
// appearing later in the stream. So, this class will increase performance much

class StreamCacheWriter
{
public:
        StreamCacheWriter(const GlobalWorkbookInfoPtr global_info);
	virtual ~StreamCacheWriter();

	// Return the next new CFRecord
	virtual CFRecordPtr getNextRecord(const CFRecordType::TypeId desirable_type);
	// Saves the next CFRecord to the stream or caches for later saving. Returns whether the record was saved to file or stored for later saving
	virtual bool storeNextRecord(CFRecordPtr record);

	virtual const size_t GetRecordPosition() = 0;
	GlobalWorkbookInfoPtr getGlobalWorkbookInfo() { return global_info_; }

private:
	// Actual transporting record to the stream
	virtual const size_t writeToStream(const size_t num_of_records_min_necessary) = 0;

protected:

	CFRecordPtrList records_cache;
	GlobalWorkbookInfoPtr global_info_;
};

class CFStreamCacheWriter : public StreamCacheWriter
{
public:
        CFStreamCacheWriter(CFStreamPtr stream, const GlobalWorkbookInfoPtr global_info);
	~CFStreamCacheWriter();

	// Return the next new CFRecord
	//CFRecordPtr getNextRecord(const CFRecordType::TypeId desirable_type) override;
	// Saves the next CFRecord to the CFStream or caches for later saving. Returns whether the record was saved to file or stored for later saving
	bool storeNextRecord(CFRecordPtr record) override;

	const size_t GetRecordPosition()override;

private:
	// Actual transporting record to the CFStream and then to IStream
	const size_t writeToStream(const size_t num_of_records_min_necessary) override;

private:
	CFStreamPtr stream_;
};

class BinaryStreamCacheWriter : public StreamCacheWriter
{
public:
        BinaryStreamCacheWriter(boost::shared_ptr<NSBinPptxRW::CXlsbBinaryWriter> binaryStream, const GlobalWorkbookInfoPtr global_info);
        ~BinaryStreamCacheWriter();
		const size_t GetRecordPosition()override{return 0;}
private:
	const size_t writeToStream(const size_t num_of_records_min_necessary) override;

	boost::shared_ptr<NSBinPptxRW::CXlsbBinaryWriter> binaryStream_;
};

} // namespace XLS
