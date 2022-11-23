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

#include "CFStream.h"
#include "CFRecordType.h"

#include <boost/bind.hpp>

#include <algorithm>
#include <sstream>

namespace XLS
{

CFStream::CFStream(POLE::Stream* stream)
{
	if(NULL == stream)
	{
		return; // Wrong IStream pointer (NULL)
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
		return;// Wrong buffer pointer (NULL)
	} 
	POLE::uint64 num_read = stream_->read((unsigned char*)buf, size);

	if(num_read < size)
	{
		stream_->seek(stream_->size()); // 5217417.xls
		return;// EndOfStreamReached
	}
}
void CFStream::copy( std::wstring streamNameCreate, POLE::Storage * storageOut)
{
	stream_->seek(0);
	int size_stream = (int)stream_->size();

	POLE::Stream *streamNew = new POLE::Stream(storageOut, streamNameCreate, true, size_stream);
	if (!streamNew) return;

	unsigned char buffer[4096];
	int bytesRead = 0;

	while(true)
	{
		int bytesToRead = size_stream - bytesRead;
		if (bytesToRead <= 0)
			break;
		if (bytesToRead > 4096)
			bytesToRead = 4096;
	
		stream_->read(buffer, bytesToRead);
		streamNew->write(buffer, bytesToRead);
		
		bytesRead += bytesToRead;
	}
	//unsigned char* data_stream = new unsigned char[size_stream + 64];
	//memset(data_stream, 0, size_stream + 64);
	//if (data_stream)
	//{
	//	stream->read(data_stream, size_stream);

	//	streamNew->write(data_stream, size_stream);

	//	delete []data_stream;
	//	data_stream = NULL;
	//}

	streamNew->flush();
			
	delete streamNew;
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
	stream_->flush();
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
	return (unsigned long)pos;
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
	return (unsigned long)stream_->size();
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
			receiver_items.erase(++it);
			source_items.erase(its);
			continue;
		}
		++it;
	}
	seekToEnd();
}


} // namespace XLS

