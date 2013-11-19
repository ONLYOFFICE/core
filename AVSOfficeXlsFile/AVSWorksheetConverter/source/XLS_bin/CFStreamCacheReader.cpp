#include "stdafx.h"
#include "CFStreamCacheReader.h"
#include "CFRecord.h"
#include "CFStream.h"
#include <XLS_logic/Biff_records/BOF.h>
#include <Exception/RequestedRecordNotFound.h>



namespace XLS
{;


CFStreamCacheReader::CFStreamCacheReader(CFStreamPtr stream, GlobalWorkbookInfoPtr global_info)
:	stream_(stream),
global_info_(global_info)
{
	skippable_records_names.push_back("StartBlock");
	skippable_records_names.push_back("EndBlock");
	skippable_records_names.push_back("ChartFrtInfo");
	//skippable_records_names.push_back("TextPropsStream");
	//skippable_records_names.push_back("ShapePropsStream");
}


CFStreamCacheReader::~CFStreamCacheReader()
{
}


// Reads the next CFRecord from the CFStream and if its type is not the desired one, caches it for the next call
CFRecordPtr CFStreamCacheReader::getNextRecord(const CFRecordType::TypeId desirable_type, const bool gen_except)
{
	CFRecordType::TypeId what_we_actually_read = rt_NONE;	

	while(readFromStream(1))
	{
		CFRecordType::TypeString rec_name = records_cache.front()->getTypeString();

		if (desirable_type == rt_MsoDrawingGroup)	// объединяем rt_MsoDrawingGroup + rt_Continue в один блок 
		{
			if (checkNextRecord(desirable_type, 1))
			{				
				readFromStream(2);

				for(std::list<CFRecordPtr>::iterator good = records_cache.begin(), it = ++records_cache.begin();
					it != records_cache.end(); ++it, ++good)
				{
					while ((it != records_cache.end()) && (desirable_type == (*it)->getTypeId()))
					{
						(*good)->appendRawData(*it);
						records_cache.erase(it);
						if(!stream_->isEOF())
						{
							records_cache.push_back(CFRecordPtr(new CFRecord(stream_, global_info_)));
						}
						it = good;
						++it;
					}
				}
			}
		}
		
		if(0 == rec_name.length())
		{
			Log::warning("The extracted record has obsoleted or unknown type(0x" + STR::int2hex_str(records_cache.front()->getTypeId(), sizeof(CFRecordType::TypeId)) + ")");
			records_cache.pop_front();
			continue;
		}
		if(skippable_records_names.end() != std::find(skippable_records_names.begin(), skippable_records_names.end(), rec_name))
		{
			Log::warning("The extracted record has been skipped (" + rec_name + ")");
			records_cache.pop_front();
			continue;
		}
		break;
	}

	if(0 != records_cache.size())
	{
		what_we_actually_read = records_cache.front()->getTypeId();

		// if we get what was requested
		if(desirable_type == what_we_actually_read || desirable_type == CFRecordType::ANY_TYPE)
		{
			CFRecordPtr ret = records_cache.front();
			records_cache.pop_front();
			return ret;
		}		
	}

	if(gen_except)
	{
		// теги разные
		std::string inType = XLS::CFRecordType::getStringById(desirable_type);
		std::string outType = CFRecordType::getStringById(what_we_actually_read);		

		Log::warning("The extracted record is not of requested type.\nRequested: \"" + 
			XLS::CFRecordType::getStringById(desirable_type) + "\" Extracted: \"" + CFRecordType::getStringById(what_we_actually_read) + "\"");		
	}
	return CFRecordPtr();
}


// Check if the next read record would be of desired type
const bool CFStreamCacheReader::checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check)
{
	readFromStream(num_records_to_check);
	// if we would get what was requested
	// ANY_TYPE is checked just to make the fake read from stream and get an error if the stream is ended
	for(std::list<CFRecordPtr>::const_iterator it = records_cache.begin(), itEnd = records_cache.end(); it != itEnd; ++it)
	{
		size_t type = (*it)->getTypeId();
		if(desirable_type == (*it)->getTypeId() || desirable_type == CFRecordType::ANY_TYPE)
		{
			return true;
		}
	}
	return false;
}


// Make sure the internal buffer has concrete number of records. Returns number of records read
const size_t CFStreamCacheReader::readFromStream(const size_t num_of_records_min_necessary)
{
	const size_t current_records_num = records_cache.size();
	if(current_records_num >= num_of_records_min_necessary)
	{
		return current_records_num;
	}

	while(records_cache.size() < num_of_records_min_necessary && !stream_->isEOF())
	{
		records_cache.push_back(CFRecordPtr(new CFRecord(stream_, global_info_)));
	}

	checkAndAppendContinueData();
	return records_cache.size();
}


// Checks whether the next record is Continue and append its data to the real record if so
void CFStreamCacheReader::checkAndAppendContinueData()
{
	if(!records_cache.size())
	{
		return;
	}

	for(std::list<CFRecordPtr>::iterator good = records_cache.begin(), it = ++records_cache.begin();
		it != records_cache.end(); ++it, ++good)
	{
		size_t typeRecord = (*it)->getTypeId();
		while(it != records_cache.end() && (rt_Continue == (*it)->getTypeId()) )
		{
			(*good)->appendRawData(*it);
			records_cache.erase(it); // now 'it' is invalid
			if(!stream_->isEOF())
			{
				records_cache.push_back(CFRecordPtr(new CFRecord(stream_, global_info_)));
			}
			it = good;
			++it; // Now it may became end(), checked in 'for'
		}
	}
}


const bool CFStreamCacheReader::isEOF() const
{
	return !records_cache.size() && stream_->isEOF();
}


// Skip the specified number of bytes without processing
void CFStreamCacheReader::skipNBytes(const size_t n)
{
	stream_->seekFromCurForward(n);
}


// Seek to the next substream (Read all records till EOF then skip EOF)
// Doesn't generate EndOfStreamReached if the stream is the last one
const bool CFStreamCacheReader::SeekToEOF()
{
	while(readFromStream(1))
	{
		if(rt_EOF == records_cache.front()->getTypeId())
		{
			//			records_cache.pop_front(); // Skip EOF
			break;
		}
		records_cache.pop_front(); // Skip non-EOF
	}

	return true;
}


// Extract the next record from the stream leaving it in the cache for future read.
// Always call resetPointerToBegin for the extracted CFRecord after using it
CFRecordPtr CFStreamCacheReader::touchTheNextRecord()
{
	if(readFromStream(1))// make sure something is in cache
	{
		return records_cache.front();
	}
	return CFRecordPtr();
}


// Check the next record type
const CFRecordType::TypeId CFStreamCacheReader::getNextRecordType()
{
	if(isEOF())
	{
		return rt_NONE;
	}
	readFromStream(1); // I don't check the result of readFromStream() because it was done implicitly in isEOF()
	return records_cache.front()->getTypeId();
}


} // namespace XLS
