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

#include "BiffRecord.h"
#include "../../Binary/CFStream.h"
#include "../../Binary/CFStreamCacheReader.h"


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

void BiffRecord::readFollowingContinue(StreamCacheReaderPtr reader)
{
}


const bool BiffRecord::storeRecordAndDecideProceeding(CFRecordPtr record)
{
	return true;
}


} // namespace XLS

