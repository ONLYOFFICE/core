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
#pragma once

#include "CFRecordType.h"
#include "BinSmartPointers.h"
#include <Logic/GlobalWorkbookInfo.h>
#include <Logic/Biff_structures/ODRAW/OfficeArtRecordHeader.h>

namespace XLS
{

// The only purpose of the reader is to avoid seeking back through IStream when, for example, 
// we have read a record from a binary stream to the CFRecord, but the record of this type is
// not waitable right at the moment and shall be read a little bit later.
// Example of a construction: 
// PIVOTTH = SXTH *ContinueFrt
// ContinueFrt may appear in the stream and may not. So, this class will increase performance much

class CFStreamCacheReader
{
public:
	CFStreamCacheReader(CFStreamPtr stream, const GlobalWorkbookInfoPtr global_info);
	~CFStreamCacheReader();

	// Reads the next CFRecord from the CFStream and if it's type is not the desired one, caches it for the next call
	CFRecordPtr getNextRecord(const CFRecordType::TypeId desirable_type, const bool gen_except = false);
	// Check if the next read record would be of desired type
	const bool checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check = 1);
	// Checks whether the next record is Continue and append its data to the real record if so
	void checkAndAppendContinueData();

	const bool isEOF() const;
	// Skip the specified number of unsigned chars without processing
	void skipNunBytes(const size_t n);

	void SkipRecord();
	// Seek to the next substream (Read all records till EOF then skip EOF)
	// Doesn't generate EndOfStreamReached if the stream is the last one
	const bool SeekToEOF(); 
	// Extract the next record from the stream leaving it in the cache for future read.
	// Always call resetPointerToBegin for the extracted CFRecord after using it
	CFRecordPtr touchTheNextRecord();
	// Check the next record type
	const CFRecordType::TypeId getNextRecordType();

	GlobalWorkbookInfoPtr getGlobalWorkbookInfo() { return global_info_; }
private:
	// Make sure the internal buffer has concrete number of records. Returns number of records read
	const size_t readFromStream(const size_t num_of_records_min_necessary);

private:
	CFStreamPtr stream_;
	CFRecordPtrList records_cache;
	GlobalWorkbookInfoPtr global_info_;
	std::vector<std::string> skippable_records_names;	
};

} // namespace XLS
