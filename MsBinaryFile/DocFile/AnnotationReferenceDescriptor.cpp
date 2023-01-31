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
			FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &(newObject->m_UserInitials), chars,  cch , ENCODING_WINDOWS_1250);
			
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

			FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &(newObject->m_UserInitials), chars, ( cch * 2 ), ENCODING_UTF16);
			
			newObject->m_AuthorIndex = reader->ReadUInt16();

			//skip 4 bytes
			unsigned int skip = reader->ReadUInt32();

			newObject->m_BookmarkId = reader->ReadInt32(); //-1 - comment is on a length zero text range in the Main Document
			RELEASEARRAYOBJECTS(chars);
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
