#include "precompiled_xls.h"
#include "BinProcessor.h"
#include <Binary/CFStream.h>
#include <Binary/CFStreamCacheReader.h>
#include <Binary/CFStreamCacheWriter.h>
#include <Logic/Biff_structures/BiffString.h>


namespace XLS
{;

BinProcessor::BinProcessor(BaseObjectPtr & parent, GlobalWorkbookInfoPtr global_info)
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


const bool BinProcessor::repeated(BaseObject& object, const int fromN, const int toN)
{
	bool at_least_one_read = false;

	for(int i = 0; i < fromN; ++i)
	{
		mandatory(*object.clone());
		at_least_one_read = true;
	}
	for(int j = fromN; j < toN || !toN/*infinity*/; ++j)
	{
		if(!optional(*object.clone()))
		{
			break;
		}
		at_least_one_read = true;
	}
	return at_least_one_read;
}


// =========================== Reader ======================================


BinReaderProcessor::BinReaderProcessor(CFStreamCacheReader& reader, BaseObjectPtr parent, const bool is_mandatory)
:	reader_(reader),
	BinProcessor(parent, reader.getGlobalWorkbookInfo()),
	is_mandatory_(is_mandatory)
{
}

BinReaderProcessor::BinReaderProcessor(CFStreamCacheReader& reader, const bool is_mandatory)
:	reader_(reader),
	BinProcessor(reader.getGlobalWorkbookInfo()),
	is_mandatory_(is_mandatory)
{
}

const bool BinReaderProcessor::optional(BaseObject& object)
{
	return readChild(object, false);
}


const bool BinReaderProcessor::mandatory(BaseObject& object)
{
	if(is_mandatory_) // if the composite object is mandatory, do as usual - log warning if the record is not found
	{
		readChild(object, true);
		return true;
	}
	else
	{
		is_mandatory_ = true; // The whole union becomes mandatory in case we have found at least one mandatory record in there
		return readChild(object, false);
	}
}


void BinReaderProcessor::markAttribute(BiffAttribute& attrib, const std::wstring & attrib_name)
{
	attrib.setName(attrib_name);
}


void BinReaderProcessor::markTaggedAttribute(BiffStructure& attrib)
{
}


void BinReaderProcessor::markVector(BiffStructurePtrVector& vec, BiffStructure& exClone)
{
	//if (parent_ == NULL) return;

	//for(std::vector<BiffStructurePtr>::iterator it = vec.begin(), itEnd = vec.end(); it != itEnd; ++it)
	//{
	//	parent_->add_child(boost::shared_ptr<BiffStructure>(it->get()));
	//}
}


// object_copy is necessary in case we haven't found the desired record and have to put it to the queue
const bool BinReaderProcessor::readChild(BaseObject& object, const bool is_mandatory)
{
	bool ret_val;
	try
	{
		ret_val = object.read(reader_, parent_, is_mandatory /* log warning if mandatory tag absent*/);
		if(!ret_val && is_mandatory)
		{
			// We don't update ret_val here because we are reading to the copy of the object.
			// And the real object will remain uninitialized
			wanted_objects.push_back(object.clone()); // store the copy of the object that was not found (this line is here to take another chance to be read after some trash processed)
			
			for(BaseObjectPtrList::iterator it = wanted_objects.begin()/*, itEnd = */; it != wanted_objects.end();)
			{
				const BaseObjectPtr w_object = *it;
				
				if(w_object->read(reader_, parent_, false))
				{
					// Remove successfully read object from the wanted objects list
					parent_->add_child(w_object);
					wanted_objects.erase(it++);
				}
				else
				{
					++it;
				}
			}
		}
	}
	//catch(EXCEPT::LogicalException&)
	//{
	//	Log::message(std::string(static_cast<char*>(object.getClassName())) + " record has generated a logical exception while loading and has been skipped. Sorry.");
	//	return true; // This doesn't mean the operation is successful, but indicate the parent container to continue repeated operations
	//}
	//catch(EXCEPT::RuntimeException&)
	//{
	//	Log::message(std::string(static_cast<char*>(object.getClassName())) + " record has generated a run-time exception while loading and has been skipped. Sorry.");
	//	return true; // This doesn't mean the operation is successful, but indicate the parent container to continue repeated operations
	//}
	//catch(_com_error error)
	//{
	//	std::string additional_descr = error.Description().length() ? std::string("Description: ") + static_cast<char*>(error.Description()) : "";
	//	Log::error(std::string("_com_error exception!!! HRESULT: ") + STR::int2hex_wstr(error.Error()) + "(" + static_cast<char*>(std::wstring (error.ErrorMessage())) + ") "+ additional_descr);
	//	Log::message(std::string(static_cast<char*>(object.getClassName())) + " record has generated a _com_error exception while loading and has been skipped. Sorry.");
	//	return true; // This doesn't mean the operation is successful, but indicate the parent container to continue repeated operations
	//}
	catch(...)
	{
	}
	return ret_val;
}


// Check if the next read record would be of desired type
const bool BinReaderProcessor::checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check)
{
	return reader_.checkNextRecord(desirable_type, num_records_to_check);
}


// Assume that the next record is BOF (if not - return false) and get the type without extracting is from cache
// In the case of stream end returns false
const bool BinReaderProcessor::getNextSubstreamType(unsigned __int16& type)
{
	CFRecordPtr record = reader_.touchTheNextRecord();
	if(!record)
	{
		return false; // EOF
	}
	if(rt_BOF != record->getTypeId())
	{
		//throw XLS::RequestedRecordNotFound(rt_BOF, record->getTypeId());
		return false;
	}
	unsigned __int16 vers;
	*record >> vers >> type;
	record->resetPointerToBegin();
	return true;
}


void BinReaderProcessor::SeekToEOF()
{
	reader_.SeekToEOF();
}



} // namespace XLS
