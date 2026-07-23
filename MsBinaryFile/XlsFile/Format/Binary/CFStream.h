/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../../../../Common/3dParty/pole/pole.h"


#include "CFRecordType.h"
#include "BinSmartPointers.h"

namespace XLS
{

class CFStream
{
public:
	CFStream(POLE::Stream* stream);
	~CFStream();

	void copy( std::wstring streamNameCreate, POLE::Storage * storageOut);
	
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

	POLE::Stream				*stream_;
private:
	CFStream::ReceiverItems		receiver_items;
	CFStream::SourceItems		source_items;
};

} // namespace XLS

