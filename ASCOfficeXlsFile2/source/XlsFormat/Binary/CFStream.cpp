
#include "CFStream.h"
#include "CFRecordType.h"
//#include <Exception/CompoundFileFormatError.h>
//#include <Exception/EndOfStreamReached.h>

#include <boost/bind.hpp>

#include <algorithm>
#include <sstream>

namespace XLS
{

CFStream::CFStream(POLE::Stream* stream)
{
	if(NULL == stream)
	{
		throw;// EXCEPT::RT::CompoundFileFormatError("Wrong IStream pointer (NULL)");
	}
	stream_ = stream;
}


CFStream::~CFStream()
{
	if (stream_) delete stream_;
	stream_ = NULL;
}


// Read 'size' unsigned chars from the stream
void CFStream::read(void* buf, const size_t size)
{
	if(NULL == buf || stream_ == NULL)
	{
		throw;// EXCEPT::RT::CompoundFileFormatError("Wrong buffer pointer (NULL)");
	}
	POLE::uint64 num_read = stream_->read((unsigned char*)buf, size);

	if(num_read < size)
	{
		throw;// EXCEPT::RT::EndOfStreamReached(stream_->fullName(), num_read, size);
	}
	// Tipa successful
}


// Write 'size' unsigned chars to the stream
void CFStream::write(const void* buf, const size_t size)
{
	if(NULL == buf || stream_ == NULL)
	{
		throw;// EXCEPT::RT::CompoundFileFormatError("Wrong buffer pointer (NULL)");
	}
	POLE::uint64 num_written = stream_->write((unsigned char*)buf, size);
	if(num_written != size)
	{
		//std::wstringstream str;
		//str << L"Impossible to write " << size << L" unsigned chars to \"" << stream_->fullName() << L"\" stream";
		throw;// EXCEPT::RT::CompoundFileFormatError(str.str(), hres);
	}
	if(num_written < size)
	{
		std::wstringstream str;
		str << L"Only " << num_written << L" unsigned chars were saved instead of " << size << L" unsigned chars requested.";
		throw;// EXCEPT::RT::CompoundFileFormatError(str.str(), hres);
	}
	// Tipa successful
}


// If the current pointer is exactly at the end of the stream.
const bool CFStream::isEOF() const
{
	return getStreamPointer() == getStreamSize();
}


// Stream pointer
const unsigned long CFStream::getStreamPointer() const
{
	POLE::uint64 pos = stream_->tell();
	return pos;
}


void CFStream::seekFromCurForward(const size_t offset)
{
	POLE::uint64 pos = offset + stream_->tell();

	stream_->seek(pos);
}


void CFStream::seekFromBegin(const unsigned long offset)
{
	POLE::uint64 pos = offset;

	stream_->seek(pos);
}


void CFStream::seekToEnd()
{
	stream_->seek(stream_->size());
}

// Stream current size
const unsigned long CFStream::getStreamSize() const
{
	return stream_->size();
}


void CFStream::writeAndApplyDelayedItems(void* buf, const size_t size, const ReceiverItems& receiver_items_from_record, const SourceItems& source_items_from_record)
{
	appendDelayedItems(receiver_items_from_record, source_items_from_record);
	write(buf, size);
	if(receiver_items_from_record.size() || source_items_from_record.size())
	{
		applyDelayedItems();
	}
}


// The file pointer of the start of the record will be appended to all receiver items 'data_place' fields 
// Then all items will be appended to the common queue
void CFStream::appendDelayedItems(const ReceiverItems& receiver_items_from_record, const SourceItems& source_items_from_record)
{
	const unsigned int record_start_pointer = static_cast<unsigned int>(getStreamPointer()) - sizeof(unsigned short)/*size_short*/ - sizeof(CFRecordType::TypeId);

	for(ReceiverItems::const_iterator it = receiver_items_from_record.begin(), itEnd = receiver_items_from_record.end(); it != itEnd; ++it)
	{
		ReceiverItem copy_item(*it);
		copy_item.data_place += record_start_pointer;
		receiver_items.push_back(copy_item);
	}
	for(SourceItems::const_iterator it = source_items_from_record.begin(), itEnd = source_items_from_record.end(); it != itEnd; ++it)
	{
		SourceItem copy_item(*it);
		if(copy_item.is_file_ptr)
		{
			copy_item.data += record_start_pointer;
		}
		source_items.push_back(copy_item);
	}
}


// Try to apply
void CFStream::applyDelayedItems()
{
	ReceiverItems& res = receiver_items;
	SourceItems& src = source_items;
	for(ReceiverItems::iterator it = receiver_items.begin(); it != receiver_items.end();)
	{
		const CFRecordType::TypeId receiver_id = it->receiver_id;
		SourceItems::iterator its = std::find_if(source_items.begin(), source_items.end(), boost::bind(&SourceItem::receiver_id, _1) == receiver_id || boost::bind(&SourceItem::receiver_id, _1) == rt_ANY_TYPE);
		if(its != source_items.end())
		{
			if(NULL != it->fn)
			{
				it->fn(*this, it->data_place, its->data); // receiver_items and source_items may be expanded here.
			}
			else
			{
				seekFromBegin(it->data_place);
				write(&its->data, it->data_size);
			}
			receiver_items.erase(it++);
			source_items.erase(its);
			continue;
		}
		++it;
	}
	seekToEnd();
}


} // namespace XLS

