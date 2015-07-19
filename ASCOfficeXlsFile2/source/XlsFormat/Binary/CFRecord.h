#pragma once

#include "CFRecordType.h"
#include "CFStream.h"
#include "BinSmartPointers.h"
#include "../Logic/GlobalWorkbookInfo.h"

#include <common.h>
#include <Auxiliary/HelpFunc.h>


namespace XLS
{;

// Binary representation of a record in BIFF8
class CFRecord
{
public:
	CFRecord(CFStreamPtr stream, GlobalWorkbookInfoPtr global_info); // Create a record an read its data from the stream
	CFRecord(CFRecordType::TypeId type_id, GlobalWorkbookInfoPtr global_info); // Create an empty record
	~CFRecord();
	void save(CFStreamPtr stream);
	void commitData();
/* ID */
	const CFRecordType::TypeId getTypeId() const;
	const CFRecordType::TypeString& getTypeString() const;
	// File Pointer to the start of the record in file
	const unsigned int getStreamPointer() const;
/* Data */
	// Pointer to the beginning of the cached data
	const char* getData() const ;
	const size_t getDataSize() const;
	const size_t getMaxRecordSize() const;
	void appendRawData(CFRecordPtr where_from);
	void appendRawData(const char* raw_data, const size_t size);
	void insertDataFromRecordToBeginning(CFRecordPtr where_from);

/* Pointers */
	const bool isEOF() const; // whether all the data have bean read
	// Checks whether the specified number of unsigned chars present in the non-read part of the buffer
	// Doesn't generate an exception
	const bool checkFitReadSafe(const size_t size) const;
	// Checks whether the specified number of unsigned chars present in the non-read part of the buffer
	// Generates an exception
	void checkFitRead(const size_t size) const;
	// Checks whether the specified number of unsigned chars fits in max size of the buffer
	// Doesn't generate an exception
	const bool checkFitWriteSafe(const size_t size) const;
	// Checks whether the specified number of unsigned chars fits in max size of the buffer
	// Generates an exception
	void checkFitWrite(const size_t size) const;
	void skipNunBytes(const size_t n); // Skip the specified number of unsigned chars without reading
	void RollRdPtrBack(const size_t n); // Move read pointer back to reread some data
	void resetPointerToBegin();
	void reserveNunBytes(const size_t n); // Skip the specified number of unsigned chars filled them in with zeros
	template<class DataType>
	void reserveNunBytes(const size_t n, const DataType fill_data) // Skip the specified number of unsigned chars filled them in with specified data
	{
		checkFitWrite(n);
		size_t odd_size = n / sizeof(DataType) * sizeof(DataType);
		for(size_t offset = 0; offset < odd_size; offset += sizeof(DataType))
		{
			reinterpret_cast<DataType*>(&intData[size_ + offset])[0] = fill_data;
		}
		for(size_t i = 0; i < n % sizeof(DataType); ++i)
		{
			intData[size_ + odd_size + i] = 0;
		}
		size_ += n;

	}

/* Markup delayed data */

	// Registers data receiver place and reserves n unsigned chars for it
	void registerDelayedDataReceiver(CFStream::DELAYED_DATA_SAVER fn, const size_t n, const CFRecordType::TypeId receiver_id = rt_NONE);
	// Registers delayed source data as any user-defined unsigned int
	void registerDelayedDataSource(const unsigned int data,  const CFRecordType::TypeId receiver_id);
	// Registers delayed source data as file pointer of the start of the record
	void registerDelayedFilePointerSource(const CFRecordType::TypeId receiver_id);
	// Registers delayed source data as file pointer of the start of the record and the specified offset added
	void registerDelayedFilePointerAndOffsetSource(const unsigned int offset,  const CFRecordType::TypeId receiver_id);


	// Extract the specified type of data without moving rdPtr
	template<class T>
	const T* getCurData() const
	{
		return reinterpret_cast<T*>(&data_[rdPtr]);
	}
	// Obtain the current rdPtr
	const size_t getRdPtr() const;

	template<class T>
	void loadAnyData(T& val)
	{
		////ASSERT(data_); // This throws if we use >> instead of <<
		checkFitRead(sizeof(T));
		val = * getCurData<T>();
		rdPtr += sizeof(T);
	}

	template<class T>
	void storeAnyData(const T& val)
	{
		checkFitWrite(sizeof(T));
		*reinterpret_cast<T*>(&intData[size_]) = val;
		size_ += sizeof(T);
	}
	void storeLongData(const char* buf, const size_t size);

	GlobalWorkbookInfoPtr getGlobalWorkbookInfo() { return global_info_; }

	CFRecord& operator>>(unsigned char& val)	{ loadAnyData(val);	return *this; };
	CFRecord& operator>>(unsigned short& val)	{ loadAnyData(val);	return *this; };
	CFRecord& operator>>(unsigned int& val)		{ loadAnyData(val);	return *this; };
	CFRecord& operator>>(long& val)				{ loadAnyData(val);	return *this; };
	CFRecord& operator>>(double& val)			{ loadAnyData(val);	return *this; };
	CFRecord& operator>>(_GUID_& val)			{ loadAnyData(val);	return *this; };
	//CFRecord& operator>>(unsigned int& val)	{ loadAnyData(val);	return *this; };
	CFRecord& operator>>(short& val)			{ loadAnyData(val);	return *this; };
	CFRecord& operator>>(char& val)				{ loadAnyData(val);	return *this; };
	CFRecord& operator>>(bool& val);
	CFRecord& operator>>(std::wstring & val);

	CFRecord& operator<<(unsigned char& val)	{ storeAnyData(val);	return *this; };
	CFRecord& operator<<(unsigned short& val)	{ storeAnyData(val);	return *this; };
	CFRecord& operator<<(unsigned int& val)		{ storeAnyData(val);	return *this; };
	CFRecord& operator<<(long& val)				{ storeAnyData(val);	return *this; };
	CFRecord& operator<<(double& val)			{ storeAnyData(val);	return *this; };
	CFRecord& operator<<(_GUID_& val)			{ storeAnyData(val);	return *this; };
	//CFRecord& operator<<(unsigned int& val)	{ storeAnyData(val);	return *this; };
	CFRecord& operator<<(short& val)			{ storeAnyData(val);	return *this; };
	CFRecord& operator<<(char& val)				{ storeAnyData(val);	return *this; };
	CFRecord& operator<<(wchar_t& val)			{ storeAnyData(val);	return *this; };
	CFRecord& operator<<(bool& val);
	CFRecord& operator<<(std::wstring & val);

private:
	static const size_t MAX_RECORD_SIZE = 8224;

private:
	CFStream::ReceiverItems receiver_items;
	CFStream::SourceItems source_items;

private: // parts of the record
	unsigned int file_ptr;
	CFRecordType::TypeId type_id_;
	size_t size_;
	char* data_;
	size_t rdPtr;
	static char intData[MAX_RECORD_SIZE];

private:
	GlobalWorkbookInfoPtr global_info_;
};

// moved out of the class to be higher in priority than the universal operator
template<class T>
CFRecord& operator>>(CFRecord& record, std::vector<T>& vec)
{
	while(!record.isEOF())
	{
		T element;
		record >> element;
		vec.push_back(element);
	}
	return record;
}


// moved out of the class to be higher in priority than the universal operator
template<class T>
CFRecord& operator<<(CFRecord& record, std::vector<T>& vec)
{
	for(std::vector<T>::iterator it = vec.begin(), itEnd = vec.end(); it != itEnd; ++it)
	{
		record << *it;
	}
	return record;
}


// moved out of the class to be higher in priority than the universal operator
template<class T>
CFRecord& operator>>(CFRecord & record, std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str)
{
	str.clear();
	T symbol;
	do 
	{
		record.loadAnyData(symbol);
		str += symbol;
	} while (symbol);
	return record; 
}


// moved out of the class to be higher in priority than the universal operator
template<class T>
CFRecord& operator<<(CFRecord & record, std::basic_string<T, std::char_traits<T>, std::allocator<T> >& str)
{
	for(std::basic_string<T, std::char_traits<T>, std::allocator<T> >::iterator it = str.begin(), itEnd = str.end(); it != itEnd; ++it)
	{
		record << *it;
	}
	record.storeAnyData(static_cast<T>(0));
	return record; 
}


// moved out of the class to be higher in priority than the universal operator
template<class T>
CFRecord& operator>>(CFRecord & record, _CP_OPT(T)& val)
{
	T temp_val;
	record.loadAnyData(temp_val);
	val = temp_val;
	return record; 
}


// moved out of the class to be higher in priority than the universal operator
template<class T>
CFRecord& operator<<(CFRecord & record,		_CP_OPT(T)& val)
{
	T temp_val(*val);
	record.storeAnyData(temp_val);
	return record; 
}

#define DET_TYPE(num_bits) num_bits <= 8 ? unsigned char : num_bits <=16 ? unsigned short : unsigned int 
#define GETBIT(from, num) ((from & (1 << num)) != 0)
#define GETBITS(from, numL, numH) ((from & (((1 << (numH - numL + 1)) - 1) << numL)) >> numL)
#define SETBIT(to, num, setorclear) {setorclear ? to |= (1 << num) : to &= ~(1 << num);}
#define SETBITS(to, numL, numH, val) {to &= ~(((1 << (numH - numL + 1)) - 1) << numL); to |= ((val & ((1 << (numH - numL + 1)) - 1)) << numL);}

} // namespace XLS
