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

#include "AnnotationReferenceDescriptor.h"

namespace DocFileFormat
{
	ByteStructure* AnnotationReferenceDescriptor::ConstructObject(VirtualStreamReader* reader, int length)
	{
		AnnotationReferenceDescriptor *newObject = new AnnotationReferenceDescriptor();

		//read the user initials (LPXCharBuffer9)

		if (reader->nWordVersion > 0)
		{
			short cch = reader->ReadByte();
		
			unsigned char *chars = reader->ReadBytes(cch, true);
			FormatUtils::GetWStringFromBytes(newObject->m_UserInitials, chars, cch , ENCODING_WINDOWS_1250);

			newObject->m_AuthorIndex = reader->ReadUInt16();
			newObject->m_BookmarkId = reader->ReadInt16();
			
			RELEASEARRAYOBJECTS(chars);
			chars = reader->ReadBytes(length - cch - 1 - 4, true);
			

			RELEASEARRAYOBJECTS(chars);
		}
		else
		{
			short cch = reader->ReadInt16(); 
			unsigned char *chars = reader->ReadBytes(18, true);

			newObject->m_UserInitials = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)(chars), cch);
			RELEASEARRAYOBJECTS(chars);

			newObject->m_AuthorIndex = reader->ReadUInt16();

			//skip 4 bytes
			unsigned int skip = reader->ReadUInt32();

			newObject->m_BookmarkId = reader->ReadInt32(); //-1 - comment is on a length zero text range in the Main Document
		}


		return static_cast<ByteStructure*>(newObject);
	}
	AnnotationReferenceExDescriptors::AnnotationReferenceExDescriptors(size_t count, POLE::Stream* stream, unsigned int fc, unsigned int lcb)
	{
		if ((lcb > 0) && (NULL != stream))
		{
			VirtualStreamReader reader(stream, (ULONG)fc, 0);

			if (fc > reader.GetSize()) return;

			for (size_t i = 0; i < (std::min)((unsigned int)count, lcb / 18); i++)
			{
				_desc desc;
				
				desc.nDTTM = DateAndTime(reader.ReadUInt32());
				reader.ReadUInt16(); //padding1
				desc.nDepth	= reader.ReadUInt32(); 
				desc.nDiatrdParent = reader.ReadUInt32(); 
				unsigned int flag = reader.ReadUInt32(); 
				
				desc.fInkAtn = GETBIT(flag, 1);
				
				m_ReferencesEx.push_back(desc);
			}
		}
	}
}
