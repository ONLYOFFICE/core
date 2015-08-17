#pragma once

#include "../../../Common/3dParty/pole/pole.h"


#include "CFRecordType.h"
#include "BinSmartPointers.h"

namespace XLS
{

// Binary representation of a stream in BIFF8
class CFStream
{
public:
	CFStream(POLE::Stream* stream);
	~CFStream();
	
	template<class Type>
	CFStream& operator>>(Type& val) // Read a simple type or an object (not array)
	{
		read(&val, sizeof(Type));
		return *this;
	}
	template<class Type>
	CFStream& operator<<(Type& val) // Write a simple type or an object (not array)
	{
		write(&val, sizeof(Type));
		return *this;
	}
	void read(void* buf, const size_t size); // Read 'size' unsigned chars from the stream
	void write(const void* buf, const size_t size); // Write 'size' unsigned chars to the stream
	void seekFromCurForward(const size_t offset);
	void seekFromBegin(const unsigned long offset);
	void seekToEnd();
	// If the current pointer is exactly at the end of the stream.
	const bool isEOF() const;
	// Stream pointer
	const unsigned long getStreamPointer() const;
	// Stream current size
	const unsigned long getStreamSize() const;

	typedef void (*DELAYED_DATA_SAVER)(CFStream& stream, const unsigned int data_place, const unsigned int data);
	typedef struct  
	{
		DELAYED_DATA_SAVER fn;
		unsigned int data_place;
		size_t data_size;
		CFRecordType::TypeId receiver_id;
	} ReceiverItem;
	typedef std::list<ReceiverItem> ReceiverItems;
	typedef struct  
	{
		unsigned int data;
		CFRecordType::TypeId receiver_id;
		CFRecordType::TypeId source_id;
		bool is_file_ptr;
	} SourceItem;
	typedef std::list<SourceItem> SourceItems;

	// Write 'size' unsigned chars to the stream and append passed delayed items and after that apply all stored delayed items
	void writeAndApplyDelayedItems(void* buf, const size_t size, const ReceiverItems& receiver_items_from_record, const SourceItems& source_items_from_record);
	// The file pointer of the start of the record will be appended to all receiver items 'data_place' fields 
	// Then all items will be appended to the common queue
	void appendDelayedItems(const ReceiverItems& receiver_items_from_record, const SourceItems& source_items_from_record);
	void applyDelayedItems();


private:
	POLE::Stream				* stream_;
	CFStream::ReceiverItems		receiver_items;
	CFStream::SourceItems		source_items;
};

} // namespace XLS

