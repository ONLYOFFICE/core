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

#include "BinSmartPointers.h"
#include "../Logic/GlobalWorkbookInfo.h"

namespace XLS
{

// The purpose of the writer is to avoid seeking back through IStream when, for example, 
// we have write an offset of a record that would appear in processing much later.
// For example this is necessary in 'Index' record that shall reference DBCell records
// appearing later in the stream. So, this class will increase performance much

class CFStreamCacheWriter
{
public:
	CFStreamCacheWriter(CFStreamPtr stream, GlobalWorkbookInfoPtr global_info);
	~CFStreamCacheWriter();

	// Saves the next CFRecord to the CFStream or caches for later saving. Returns whether the record was saved to file or stored for later saving
	bool storeNextRecord(CFRecordPtr record);

	GlobalWorkbookInfoPtr getGlobalWorkbookInfo() { return global_info_; }

private:
	// Actual transporting record to the CFStream and then to IStream
	const size_t writeToStream(const size_t num_of_records_min_necessary);

private:
	CFStreamPtr stream_;
	CFRecordPtrList records_cache;
	GlobalWorkbookInfoPtr global_info_;
};

} // namespace XLS
