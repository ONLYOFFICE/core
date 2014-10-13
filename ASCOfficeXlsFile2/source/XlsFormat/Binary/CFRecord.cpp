#include "precompiled_xls.h"
#include "CFRecord.h"
#include "CFStream.h"
//#include <Exception/WrongFormatInterpretation.h>
//#include <Exception/WrongAPIUsage.h>
//#include <Exception/WrongBiffRecord.h>

namespace XLS
{;

char CFRecord::intData[MAX_RECORD_SIZE];

// Create a record and read its data from the stream
CFRecord::CFRecord(CFStreamPtr stream, GlobalWorkbookInfoPtr global_info)
:	rdPtr(0), // seek to the start
	global_info_(global_info)
{
	file_ptr = static_cast<unsigned int>(stream->getStreamPointer()); // Assume that files have size < 4Gb
	*stream >> type_id_;
	unsigned __int16 size_short;
	*stream >> size_short;
	size_ = size_short;
	data_ = new char[size_];
	
	unsigned __int64 rec_data_pos = stream->getStreamPointer();

	stream->read(data_, size_);
	if(global_info->decryptor && 0 != size_)
	{
		switch (type_id_) // this would decrease number of checks
		{
			case rt_BOF:
			case rt_FilePass:
			case rt_UsrExcl:
			case rt_FileLock:
			case rt_InterfaceHdr:
			case rt_RRDInfo:
			case rt_RRDHead:
				break;
			case rt_BoundSheet8:
				global_info->decryptor->Decrypt(data_ + sizeof(unsigned int), size_ - sizeof(unsigned int), rec_data_pos + sizeof(unsigned int));
				break;
			default:
				global_info->decryptor->Decrypt(data_, size_, rec_data_pos);
				break;
		}
	}
}


// Create an empty record
CFRecord::CFRecord(CFRecordType::TypeId type_id, GlobalWorkbookInfoPtr global_info)
:	type_id_(type_id),
	size_(0),
	rdPtr(0),
	data_(NULL),
	file_ptr(-1),
	global_info_(global_info)
{
}


void CFRecord::save(CFStreamPtr stream)
{
	file_ptr = static_cast<unsigned int>(stream->getStreamPointer()); // Assume that files have size < 4Gb
	*stream << type_id_;
	unsigned __int16 size_short = static_cast<unsigned __int16>(size_);
	*stream << size_short;
	if(data_)
	{
		stream->writeAndApplyDelayedItems(data_, size_, receiver_items, source_items);
	}
	rdPtr = 0; // seek to the start
}


void CFRecord::commitData()
{
	if(!data_)
	{
		if(size_ > MAX_RECORD_SIZE)
		{
			throw;// EXCEPT::RT::WrongBiffRecord("Too much data written to CFRecord.", getTypeString());
		}
		data_ = new char[size_];
		memcpy_s(data_, size_, intData, size_);
	}
}


CFRecord::~CFRecord()
{
	delete[] data_;
}


const CFRecordType::TypeId CFRecord::getTypeId() const
{
	return type_id_;
}


const CFRecordType::TypeString& CFRecord::getTypeString() const
{
	return CFRecordType::getStringById(type_id_);
}


// File Pointer to the start of the record in file
const unsigned int CFRecord::getStreamPointer() const
{
	if(-1 == file_ptr)
	{
		throw;// EXCEPT::LE::WrongAPIUsage("Stream pointer was not initialized", __FUNCTION__);
	}
	return file_ptr;
}


const char* CFRecord::getData() const
{
	if(data_)
	{
		return data_;
	}
	else
	{
		return intData;
	}
}


const size_t CFRecord::getDataSize() const
{
	return size_;
}


const size_t CFRecord::getMaxRecordSize() const
{
	return MAX_RECORD_SIZE;
}


void CFRecord::appendRawData(CFRecordPtr where_from)
{
	appendRawData(where_from->getData(), where_from->getDataSize());
}


void CFRecord::appendRawData(const char* raw_data, const size_t size)
{
	char* data_new = new char[size_ + size];
	memcpy(data_new, data_, size_);
	memcpy(data_new + size_, raw_data, size);
	size_ += size;
	std::swap(data_, data_new);
	delete[] data_new;

}

void CFRecord::insertDataFromRecordToBeginning(CFRecordPtr where_from)
{
	const char* src_data = where_from->getData();
	const size_t src_size = where_from->getDataSize();

	char* data_new = new char[size_ + src_size];
	memcpy(data_new, src_data, src_size);
	memcpy(data_new + src_size, data_, size_);
	size_ += src_size;
	std::swap(data_, data_new);
	delete[] data_new;
}


const size_t CFRecord::getRdPtr() const
{
	return rdPtr;
}


// whether all the data have bean read
const bool CFRecord::isEOF() const
{
	if(rdPtr > size_) // data buffer bounds crossing
	{
		throw;// EXCEPT::LE::WrongFormatInterpretation(__FUNCTION__);
	}
	return rdPtr >= size_;
}


// Checks whether the specified number of unsigned chars present in the non-read part of the buffer
// Doesn't generate an exception
const bool CFRecord::checkFitReadSafe(const size_t size) const
{
	return (rdPtr + size <= size_);
}


// Checks whether the specified number of unsigned chars present in the non-read part of the buffer
// Generates an exception
void CFRecord::checkFitRead(const size_t size) const
{
	if(!checkFitReadSafe(size))
	{
		throw;// EXCEPT::RT::WrongBiffRecord("Wrong record size.", getTypeString());
	}
}


// Checks whether the specified number of unsigned chars fits in max size of the buffer
// Doesn't generate an exception
const bool CFRecord::checkFitWriteSafe(const size_t size) const
{
	return (size_ + size <= MAX_RECORD_SIZE);
}


// Checks whether the specified number of unsigned chars fits in max size of the buffer
// Generates an exception
void CFRecord::checkFitWrite(const size_t size) const
{
	if(!checkFitWriteSafe(size))
	{
		throw;// EXCEPT::RT::WrongBiffRecord("Some of the stored data doesn't fit the intermediate buffer.", getTypeString());
	}
}


// Skip the specified number of unsigned chars without reading
void CFRecord::skipNunBytes(const size_t n)
{
	//ASSERT(data_); // This throws if we use skipNunBytes instead of reserveNunBytes
	checkFitRead(n);
	rdPtr += n;
}


// Skip the specified number of unsigned chars without reading
void CFRecord::RollRdPtrBack(const size_t n)
{
	//ASSERT(data_); // This throws if we use RollRdPtrBack while storing data
	if(rdPtr - n < 0)
	{
		throw;// EXCEPT::RT::WrongBiffRecord("Wrong record pointer rolling back size.", getTypeString());
	}
	rdPtr -= n;
}


// Skip the specified number of unsigned chars filled them in with zeros
void CFRecord::reserveNunBytes(const size_t n)
{
	reserveNunBytes(n, static_cast<unsigned int>(0));
}


// The following func is called every time the record is read again
void CFRecord::resetPointerToBegin()
{
	rdPtr = 0;
}


CFRecord& CFRecord::operator>>(bool& val)
{
	throw;// EXCEPT::LE::WrongAPIUsage("This function may only be called by mistake.", __FUNCTION__);
}


CFRecord& CFRecord::operator>>(std::wstring & val)
{
	throw;// EXCEPT::LE::WrongAPIUsage("This function may only be called by mistake.", __FUNCTION__);
}


CFRecord& CFRecord::operator<<(bool& val)
{
	throw;// EXCEPT::LE::WrongAPIUsage("This function may only be called by mistake.", __FUNCTION__);
}


CFRecord& CFRecord::operator<<(std::wstring & val)
{
	throw;// EXCEPT::LE::WrongAPIUsage("This function may only be called by mistake.", __FUNCTION__);
}


// Registers data receiver place and reserves n unsigned chars for it
void CFRecord::registerDelayedDataReceiver(CFStream::DELAYED_DATA_SAVER fn, const size_t n, const CFRecordType::TypeId receiver_id)
{
	//ASSERT(!data_); // This throws if used after Commit or while reading of binary
	CFStream::ReceiverItem item;
	item.fn = fn;
	item.data_place = size_ + sizeof(unsigned __int16)/*size_short*/ + sizeof(CFRecordType::TypeId); // set offset relative to record beginning. 
	item.data_size = n;
	item.receiver_id = rt_NONE == receiver_id ? getTypeId() : receiver_id;
	receiver_items.push_back(item);
	reserveNunBytes(n);
}


// Registers delayed source data as any user-defined unsigned int
void CFRecord::registerDelayedDataSource(const unsigned int data,  const CFRecordType::TypeId receiver_id)
{
	//ASSERT(!data_); // This throws if used after Commit or while reading of binary
	CFStream::SourceItem item;
	item.data = data;
	item.is_file_ptr = false;
	item.receiver_id = receiver_id;
	item.source_id = getTypeId();
	source_items.push_back(item);
}


// Registers delayed source data as file pointer of the start of the record
void CFRecord::registerDelayedFilePointerSource(const CFRecordType::TypeId receiver_id)
{
	//ASSERT(!data_); // This throws if used after Commit or while reading of binary
	CFStream::SourceItem item;
	item.data = 0;
	item.is_file_ptr = true;
	item.receiver_id = receiver_id;
	item.source_id = getTypeId();
	source_items.push_back(item);
}


// Registers delayed source data as file pointer of the start of the record and the specified offset added
void CFRecord::registerDelayedFilePointerAndOffsetSource(const unsigned int offset,  const CFRecordType::TypeId receiver_id)
{
	//ASSERT(!data_); // This throws if used after Commit or while reading of binary
	CFStream::SourceItem item;
	item.data = offset;
	item.is_file_ptr = true;
	item.receiver_id = receiver_id;
	item.source_id = getTypeId();
	source_items.push_back(item);
}


void CFRecord::storeLongData(const char* buf, const size_t size)
{
	checkFitWrite(size);
	memcpy_s(&intData[size_], getMaxRecordSize() - size_, buf, size);
	size_ += size;
}


} // namespace XLS


