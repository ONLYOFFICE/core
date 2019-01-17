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

#include "CFRecord.h"
#include "CFStream.h"


namespace XLS
{

char CFRecord::intData[MAX_RECORD_SIZE];

// Create a record and read its data from the stream
CFRecord::CFRecord(CFStreamPtr stream, GlobalWorkbookInfoPtr global_info)
:	rdPtr(0), // seek to the start
	global_info_(global_info)
{
	file_ptr = static_cast<unsigned int>(stream->getStreamPointer());
	*stream >> type_id_;
	unsigned short size_short;
	*stream >> size_short;
	size_ = size_short;
	data_ = new char[size_];
	
	unsigned long rec_data_pos = stream->getStreamPointer();

	stream->read(data_, size_);
	if(global_info->decryptor && 0 != size_)
	{
		size_t block_size = global_info->Version == 0x0500 ? 16 : 1024;

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
			{
				if (global_info->Version == 0x0500)
					global_info->decryptor->Decrypt(data_/* + sizeof(unsigned int)*/, size_/* - sizeof(unsigned int)*/, rec_data_pos, block_size);
				else
					global_info->decryptor->Decrypt(data_ + sizeof(unsigned int), size_ - sizeof(unsigned int), rec_data_pos + sizeof(unsigned int), block_size);
			}break;
			default:
			{
				global_info->decryptor->Decrypt(data_, size_, rec_data_pos, block_size);
			}break;
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
	unsigned short size_short = static_cast<unsigned short>(size_);
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
        memcpy(data_, intData, size_);
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

	if (data_ && size_ > 0)
	{
		memcpy(data_new, data_, size_);
	}
	if (raw_data && size > 0)
	{
		memcpy(data_new + size_, raw_data, size);
	}
	size_ += size;
	std::swap(data_, data_new);

	if (data_new)
	{
		delete[] data_new;
		data_new = NULL;
	}

}

bool CFRecord::loadAnyData(wchar_t & val)
{
    if (checkFitRead(2))
	{
	#if defined(_WIN32) || defined(_WIN64)
		 val = * getCurData<wchar_t>();
	#else
		unsigned short val_utf16 = * getCurData<unsigned short>();

		val = val_utf16;
	#endif
		rdPtr += 2;

		return true;
	}
	return false;
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


const bool CFRecord::isEOF() const
{
	if(rdPtr > size_)
	{
		true;//throw;
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
bool CFRecord::checkFitRead(const size_t size) const
{
	if(!checkFitReadSafe(size))
	{
		return false;// EXCEPT::RT::WrongBiffRecord("Wrong record size.", getTypeString());
	}
	return true;
}


const bool CFRecord::checkFitWriteSafe(const size_t size) const
{
	return (size_ + size <= MAX_RECORD_SIZE);
}


void CFRecord::checkFitWrite(const size_t size) const
{
	if(!checkFitWriteSafe(size))
	{
		throw;// EXCEPT::RT::WrongBiffRecord("Some of the stored data doesn't fit the intermediate buffer.", getTypeString());
	}
}


void CFRecord::skipNunBytes(const size_t n)
{
	//ASSERT(data_); // This throws if we use skipNunBytes instead of reserveNunBytes
	if (checkFitRead(n))
	{
		rdPtr += n;
	}
}


void CFRecord::RollRdPtrBack(const size_t n)
{
	//ASSERT(data_); // This throws if we use RollRdPtrBack while storing data
	if(rdPtr - n < 0)
	{
		throw;// EXCEPT::RT::WrongBiffRecord("Wrong record pointer rolling back size.", getTypeString());
	}
	rdPtr -= n;
}


void CFRecord::reserveNunBytes(const size_t n)
{
	reserveNunBytes(n, static_cast<unsigned int>(0));
}


void CFRecord::resetPointerToBegin()
{
	rdPtr = 0;
}


CFRecord& CFRecord::operator>>(bool& val)
{
	throw;// EXCEPT::LE::WrongAPIUsage("This function may only be called by mistake.", __FUNCTION__);
}



void CFRecord::registerDelayedDataReceiver(CFStream::DELAYED_DATA_SAVER fn, const size_t n, const CFRecordType::TypeId receiver_id)
{
	//ASSERT(!data_); // This throws if used after Commit or while reading of binary
	CFStream::ReceiverItem item;
	item.fn = fn;
	item.data_place = size_ + sizeof(unsigned short)/*size_short*/ + sizeof(CFRecordType::TypeId); // set offset relative to record beginning. 
	item.data_size = n;
	item.receiver_id = rt_NONE == receiver_id ? getTypeId() : receiver_id;
	receiver_items.push_back(item);
	reserveNunBytes(n);
}


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

    if (getMaxRecordSize() - size_ >= size)
    {
        memcpy(&intData[size_], buf, size);
        size_ += size;
    }
}
#if !defined(_WIN32) && !defined(_WIN64)
CFRecord& operator>>(CFRecord & record, std::string & str)
{
    str.clear();
    char symbol;
    do
    {
        if (record.loadAnyData(symbol) == false) break;
        str += symbol;
    } while (symbol);
    return record;
}

CFRecord& operator>>(CFRecord & record, std::wstring & str)
{
    std::vector<unsigned short> utf16;
    str.clear();
    unsigned short symbol;
    do
    {
        if (record.loadAnyData(symbol) == false) break;
        utf16.push_back(symbol);
    } while (symbol);

    str = convertUtf16ToWString(utf16.data(),utf16.size());
    utf16.clear();
    return record;
}
#endif
} // namespace XLS


