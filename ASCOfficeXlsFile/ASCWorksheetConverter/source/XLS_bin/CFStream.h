#pragma once

#include <XLS_bin/CFRecordType.h>
#include <XLS_bin/BinSmartPointers.h>

namespace XLS
{;

// Binary representation of a stream in BIFF8
class CFStream
{
public:
	CFStream(IStream* stream);
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
	void read(void* buf, const size_t size); // Read 'size' bytes from the stream
	void write(const void* buf, const size_t size); // Write 'size' bytes to the stream
	void seekFromCurForward(const size_t offset);
	void seekFromBegin(const ULONGLONG offset);
	void seekToEnd();
	// If the current pointer is exactly at the end of the stream.
	const bool isEOF() const;
	// Stream pointer
	const ULONGLONG getStreamPointer() const;
	// Stream current size
	const ULONGLONG getStreamSize() const;

	typedef void (*DELAYED_DATA_SAVER)(CFStream& stream, const DWORD data_place, const DWORD data);
	typedef struct  
	{
		DELAYED_DATA_SAVER fn;
		DWORD data_place;
		size_t data_size;
		CFRecordType::TypeId receiver_id;
	} ReceiverItem;
	typedef std::list<ReceiverItem> ReceiverItems;
	typedef struct  
	{
		DWORD data;
		CFRecordType::TypeId receiver_id;
		CFRecordType::TypeId source_id;
		bool is_file_ptr;
	} SourceItem;
	typedef std::list<SourceItem> SourceItems;

	// Write 'size' bytes to the stream and append passed delayed items and after that apply all stored delayed items
	void writeAndApplyDelayedItems(void* buf, const size_t size, const ReceiverItems& receiver_items_from_record, const SourceItems& source_items_from_record);
	// The file pointer of the start of the record will be appended to all receiver items 'data_place' fields 
	// Then all items will be appended to the common queue
	void appendDelayedItems(const ReceiverItems& receiver_items_from_record, const SourceItems& source_items_from_record);
	void applyDelayedItems();

private:
	// Stream name
	const std::string getStreamName() const;

private:
	IStream* stream_;
	CFStream::ReceiverItems receiver_items;
	CFStream::SourceItems source_items;
};

} // namespace XLS

