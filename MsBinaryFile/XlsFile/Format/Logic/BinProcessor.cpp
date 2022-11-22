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

#include "BinProcessor.h"
#include "../Binary/CFStream.h"
#include "../Binary/CFStreamCacheReader.h"
#include "../Logic/Biff_structures/BiffString.h"


namespace XLS
{;

BinProcessor::BinProcessor(BaseObject* parent, GlobalWorkbookInfoPtr global_info)
:	parent_(parent),
	global_info_(global_info)
{
}
BinProcessor::BinProcessor( GlobalWorkbookInfoPtr global_info)
:	global_info_(global_info)
{
}


BinProcessor::~BinProcessor()
{
}


const int BinProcessor::repeated(BaseObject& object, const int fromN, const int toN)
{
	int count = 0;

	bool at_least_one_read = false;

	for(int i = 0; i < fromN; ++i)
    {
        mandatory(*object.clone());
		count++;
        at_least_one_read = true;
	}
	for(int j = fromN; j < toN || !toN/*infinity*/; ++j)
    {
        if(!optional(*object.clone()))
		{
			break;
		}
		count++;
        at_least_one_read = true;
	}
	return count;
}

bool BinProcessor::isBOF(CFRecordType::TypeId type)
{
	switch(type)
	{
	case rt_BOF_BIFF8:
	case rt_BOF_BIFF4:
	case rt_BOF_BIFF3:
	case rt_BOF_BIFF2:
		return true;
	default:
		return false;
	}
	return false;
}
// =========================== Reader ======================================


BinReaderProcessor::BinReaderProcessor(StreamCacheReaderPtr reader, BaseObject* parent, const bool is_mandatory)
:	reader_(reader),
	BinProcessor(parent, reader ? reader->getGlobalWorkbookInfo() : NULL),
	is_mandatory_(is_mandatory)
{
}

//BinReaderProcessor::BinReaderProcessor(StreamCacheReader& reader, const bool is_mandatory)
//:	reader_(reader),
//	BinProcessor(reader.getGlobalWorkbookInfo()),
//	is_mandatory_(is_mandatory)
//{
//}

 // Load the specified child object specified number of times
 // fromN = 0 means the object is optional
 // toN = 0 means no upper limit
 template<class T>
 const int repeated(const int fromN, const int toN)
 {
     T obj;
     return repeated(obj, fromN, toN);
 }
const bool BinReaderProcessor::optional(BaseObject& object)
{
	return readChild(object, false);
}


const bool BinReaderProcessor::mandatory(BaseObject& object)
{
	if(is_mandatory_) // if the composite object is mandatory, do as usual - log warning if the record is not found
	{
		bool res = readChild(object, true);
		return res;
	}
	else
	{
		is_mandatory_ = true; // The whole union becomes mandatory in case we have found at least one mandatory record in there
		return readChild(object, false);
	}
}


// object_copy is necessary in case we haven't found the desired record and have to put it to the queue
const bool BinReaderProcessor::readChild(BaseObject& object, const bool is_mandatory)
{
	if (!reader_)
		return false;

	bool ret_val = false;
	try
	{
		ret_val = object.read(reader_, parent_, is_mandatory /* log warning if mandatory tag absent*/);
		if(!ret_val && is_mandatory)
		{
			if (global_info_->decryptor)
			{
				if (global_info_->decryptor->IsVerify() == false)
					return false;
			}
			// We don't update ret_val here because we are reading to the copy of the object.
			// And the real object will remain uninitialized
			wanted_objects.push_back(object.clone()); // store the copy of the object that was not found (this line is here to take another chance to be read after some trash processed)			
			for(BaseObjectPtrList::iterator it = wanted_objects.begin()/*, itEnd = */; 
							it != wanted_objects.end();)
			{
				const BaseObjectPtr w_object = *it;				
				if(w_object->read(reader_, parent_, false))
				{
					// Remove successfully read object from the wanted objects list
					wanted_objects.erase(++it);
				}
				else
				{
					++it;
				}
			}
		}
	}
	catch(...)
	{
	}
	return ret_val;
}


// Check if the next read record would be of desired type
const bool BinReaderProcessor::checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check)
{
	if (!reader_)
		return false;

	return reader_->checkNextRecord(desirable_type, num_records_to_check);
}


// Assume that the next record is BOF (if not - return false) and get the type without extracting is from cache
// In the case of stream end returns false
const bool BinReaderProcessor::getNextSubstreamType(unsigned short& type)
{
	if (!reader_)
		return false;

	CFRecordPtr record = reader_->touchTheNextRecord();
	if(!record)
	{
		return false; // EOF
	}
	if (false == record->isBOF())
	{
		//test-file.xls
		while (rt_Blank == record->getTypeId())
		{
			SkipRecord();
			record = reader_->touchTheNextRecord();
			if (!record)
			{
				return false; // EOF
			}
			if (true == record->isBOF())
			{
				break;
			}
		}
		if (false == record->isBOF())
			return false;
	}
	unsigned short vers;
	*record >> vers >> type;
	record->resetPointerToBegin();
	return true;
}

// Check the next record type
const CFRecordType::TypeId BinReaderProcessor::getNextRecordType()
{
	if (!reader_)
		return rt_NONE;

	CFRecordPtr record = reader_->touchTheNextRecord();
	if(!record)
	{
		return rt_NONE; // EOF
	}
	return record->getTypeId();
}
void BinReaderProcessor::SeekToEOF()
{
	if (reader_)
		reader_->SeekToEOF();
}
void BinReaderProcessor::SkipRecord(bool log_debug)
{
	if (reader_)
		reader_->SkipRecord(log_debug);
}
const int BinReaderProcessor::GetRecordPosition()
{
 if (reader_)
     return reader_->GetRecordPosition();
}
void BinReaderProcessor::SetRecordPosition(const int position)
{
  if (reader_)
      reader_->SetRecordPosition(position);
}


} // namespace XLS
