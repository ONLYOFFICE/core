#include "stdafx.h"
#include "BinProcessor.h"
#include <XLS_bin/CFStream.h>
#include <XLS_bin/CFStreamCacheReader.h>
#include <XLS_bin/CFStreamCacheWriter.h>
#include <XLS_logic/Biff_structures/BiffString.h>


namespace XLS
{;

BinProcessor::BinProcessor(MSXML2::IXMLDOMElementPtr parent, GlobalWorkbookInfoPtr global_info)
:	parent_(parent),
	global_info_(global_info)
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


BinReaderProcessor::BinReaderProcessor(CFStreamCacheReader& reader, MSXML2::IXMLDOMElementPtr parent, const bool is_mandatory)
:	reader_(reader),
	BinProcessor(parent, reader.getGlobalWorkbookInfo()),
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


void BinReaderProcessor::markAttribute(BiffAttribute& attrib, const _bstr_t& attrib_name)
{
	attrib.setName(attrib_name);
	attrib.toXML(parent_);
}


void BinReaderProcessor::markTaggedAttribute(BiffStructure& attrib)
{
	attrib.toXML(parent_);
}


void BinReaderProcessor::markVector(BiffStructurePtrVector& vec, BiffStructure& exClone)
{
	for(std::vector<BiffStructurePtr>::const_iterator it = vec.begin(), itEnd = vec.end(); it != itEnd; ++it)
	{
		(*it)->toXML(parent_);
	}
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
			for(BaseObjectPtrList::iterator it = wanted_objects.begin()/*, itEnd = */;
				it != wanted_objects.end();)
			{
				const BaseObjectPtr w_object = *it;
				if(w_object->read(reader_, parent_, false))
				{
					// Remove successfully read object from the wanted objects list
					wanted_objects.erase(it++);
				}
				else
				{
					++it;
				}
			}
		}
	}
	catch(EXCEPT::LogicalException&)
	{
		Log::message(std::string(static_cast<char*>(object.getClassName())) + " record has generated a logical exception while loading and has been skipped. Sorry.");
		return true; // This doesn't mean the operation is successful, but indicate the parent container to continue repeated operations
	}
	catch(EXCEPT::RuntimeException&)
	{
		Log::message(std::string(static_cast<char*>(object.getClassName())) + " record has generated a run-time exception while loading and has been skipped. Sorry.");
		return true; // This doesn't mean the operation is successful, but indicate the parent container to continue repeated operations
	}
	catch(_com_error error)
	{
		std::string additional_descr = error.Description().length() ? std::string("Description: ") + static_cast<char*>(error.Description()) : "";
		Log::error(std::string("_com_error exception!!! HRESULT: ") + STR::int2hex_str(error.Error()) + "(" + static_cast<char*>(_bstr_t(error.ErrorMessage())) + ") "+ additional_descr);
		Log::message(std::string(static_cast<char*>(object.getClassName())) + " record has generated a _com_error exception while loading and has been skipped. Sorry.");
		return true; // This doesn't mean the operation is successful, but indicate the parent container to continue repeated operations
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
const bool BinReaderProcessor::getNextSubstreamType(WORD& type)
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
	WORD vers;
	*record >> vers >> type;
	record->resetPointerToBegin();
	return true;
}


void BinReaderProcessor::SeekToEOF()
{
	reader_.SeekToEOF();
}


// =========================== Writer ======================================

BinWriterProcessor::BinWriterProcessor(CFStreamCacheWriter& writer, MSXML2::IXMLDOMElementPtr parent, const bool is_mandatory)
:	writer_(writer),
	BinProcessor(parent, writer.getGlobalWorkbookInfo()),
	substreams_processed(0),
	is_mandatory_(is_mandatory),
	processed_tags_count(processed_tags_count_own),
	processed_tags_count_own(1),
	processed_tags(processed_tags_own) // Points to own counters map
{
}


BinWriterProcessor::BinWriterProcessor(BinWriterProcessor& copy_from_proc, MSXML2::IXMLDOMElementPtr own_tag, const bool is_mandatory)
:	writer_(copy_from_proc.writer_),
	BinProcessor(own_tag, copy_from_proc.getGlobalWorkbookInfo()),
	substreams_processed(0),
	is_mandatory_(is_mandatory),
	processed_tags_count(processed_tags_count_own),
	processed_tags_count_own(1),
	processed_tags(processed_tags_own) // Points to own counters map
{
}


BinWriterProcessor::BinWriterProcessor(BinWriterProcessor& copy_from_proc, const bool is_mandatory)
:	writer_(copy_from_proc.writer_),
	BinProcessor(copy_from_proc.parent_, copy_from_proc.getGlobalWorkbookInfo()),
	substreams_processed(0),
	is_mandatory_(is_mandatory),
	processed_tags_count(copy_from_proc.processed_tags_count),
	processed_tags(copy_from_proc.processed_tags) // Points to parent's counters map
{
}


const bool BinWriterProcessor::optional(BaseObject& object)
{
	return writeChild(object, false);
}


const bool BinWriterProcessor::mandatory(BaseObject& object)
{
	if(is_mandatory_) // if the composite object is mandatory, do as usual - log warning if the record is not found
	{
		return writeChild(object, true);
	}
	else
	{
		is_mandatory_ = true; // The whole union becomes mandatory in case we have found at least one mandatory record in there
		return writeChild(object, false);
	}
}


void BinWriterProcessor::markAttribute(BiffAttribute& attrib, const _bstr_t& attrib_name)
{
	attrib.setName(attrib_name);
	attrib.fromXML(parent_);
}


void BinWriterProcessor::markTaggedAttribute(BiffStructure& attrib)
{
	attrib.fromXML(parent_, ++processed_tags[attrib.getClassName()], true);
}


void BinWriterProcessor::markVector(BiffStructurePtrVector& vec, BiffStructure& exClone)
{
	BiffStructurePtr clone;
	size_t number = 0;
	while((clone = exClone.clone())->fromXML(parent_, ++number, false))
	{
		vec.push_back(clone);
	}
}



// Check if we have the specified record tag among siblings
const bool BinWriterProcessor::checkNextRecord(const CFRecordType::TypeId desirable_type, const size_t num_records_to_check)
{
	return false;
}


// Determine the next substream type
const bool BinWriterProcessor::getNextSubstreamType(WORD& type)
{
	MSXML2::IXMLDOMNodePtr substream_tag = parent_->selectSingleNode(L"*[position() = " + 
		_bstr_t(STR::int2str(substreams_processed + 1).c_str()) + L"]/BOF/@dt");
	if(substream_tag)
	{
		++substreams_processed;
		type = atoi(static_cast<char*>(substream_tag->Gettext()));
		return true;
	}
	return false;
}


void BinWriterProcessor::SeekToEOF()
{
}

// object_copy is necessary in case we haven't found the desired record and have to put it to the queue
const bool BinWriterProcessor::writeChild(BaseObject& object, const bool is_mandatory)
{
	try
	{
		return object.write(*this, is_mandatory);
	}
	catch(EXCEPT::LogicalException&)
	{
		Log::message(std::string(static_cast<char*>(object.getClassName())) + " record has generated a logical exception while storing and has been skipped. Sorry.");
		return true; // This doesn't mean the operation is successful, but indicate the parent container to continue repeated operations
	}
	catch(EXCEPT::RuntimeException&)
	{
		Log::message(std::string(static_cast<char*>(object.getClassName())) + " record has generated a run-time exception while storing and has been skipped. Sorry.");
		return true; // This doesn't mean the operation is successful, but indicate the parent container to continue repeated operations
	}
	catch(_com_error error)
	{
		std::string additional_descr = error.Description().length() ? std::string("Description: ") + static_cast<char*>(error.Description()) : "";
		Log::error(std::string("_com_error exception!!! HRESULT: ") + STR::int2hex_str(error.Error()) + "(" + static_cast<char*>(_bstr_t(error.ErrorMessage())) + ") "+ additional_descr);
		Log::message(std::string(static_cast<char*>(object.getClassName())) + " record has generated a _com_error exception while storing and has been skipped. Sorry.");
		return true; // This doesn't mean the operation is successful, but indicate the parent container to continue repeated operations
	}
}


CFStreamCacheWriter& BinWriterProcessor::getWriter() const
{
	return writer_;
}


MSXML2::IXMLDOMElementPtr BinWriterProcessor::getChildNode(const _bstr_t& tag_name, const bool is_mandatory)
{
	//_bstr_t query = tag_name + L"[position() = " + _bstr_t(STR::int2str(++processed_tags[static_cast<char*>(tag_name)]).c_str()) + L"]";
	_bstr_t query = L"*[position() = " + _bstr_t(STR::int2str(processed_tags_count).c_str()) + L"]";
	MSXML2::IXMLDOMElementPtr own_tag = parent_->selectSingleNode(query);
	if(own_tag && own_tag->GetnodeName() == tag_name)
	{
		++processed_tags_count;
		return own_tag;
	}

	if(is_mandatory) // If the tag hasn't been found and it is mandatory, log warning and skip
	{
// 		Log::warning(std::string("Mandatory tag <") + static_cast<char*>(tag_name) + "> number " + 
// 			STR::int2str(processed_tags[static_cast<char*>(tag_name)]) + " was not found in the intermediate XML file");
		Log::warning(std::string("Mandatory tag <") + static_cast<char*>(tag_name) + "> number " + 
			STR::int2str(processed_tags_count) + " was not found in the intermediate XML file");
	}

	return MSXML2::IXMLDOMElementPtr();
}

} // namespace XLS
