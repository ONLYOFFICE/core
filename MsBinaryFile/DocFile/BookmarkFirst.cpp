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

#include "BookmarkFirst.h"

namespace DocFileFormat
{
	  AtnBookmark::AtnBookmark()
	  {
	  }
	  AtnBookmark::~AtnBookmark()
	  {
	  }
	  ByteStructure* AtnBookmark::ConstructObject(VirtualStreamReader* reader, int length)
	  {
		  AtnBookmark *newObject = new AtnBookmark();

		  newObject->bmc = reader->ReadUInt16(); //0x0100
		  newObject->lTag = reader->ReadUInt32();

		  unsigned int lTagOld = reader->ReadUInt32();

		  return static_cast<ByteStructure*>(newObject);
	  }
//---------------------------------------------------------------------------------------------
	  ProtInfoBookmark::ProtInfoBookmark()
	  {
	  }
	  ProtInfoBookmark::~ProtInfoBookmark()
	  {
	  }
	  ByteStructure* ProtInfoBookmark::ConstructObject(VirtualStreamReader* reader, int length)
	  {
		  ProtInfoBookmark *newObject = new ProtInfoBookmark();

		  newObject->id = reader->ReadUInt32(); 
		  
		  newObject->uidSel = reader->ReadUInt16();
		  newObject->iProt = reader->ReadUInt16();
		  
		  newObject->i = reader->ReadUInt16();
		  newObject->fUseMe = reader->ReadUInt16();

		  return static_cast<ByteStructure*>(newObject);
	  }
//---------------------------------------------------------------------------------------------
	  BookmarkFirst::BookmarkFirst()
	  {
	  }
	  BookmarkFirst::~BookmarkFirst()
	  {
	  }
	  ByteStructure* BookmarkFirst::ConstructObject(VirtualStreamReader* reader, int length)
	  {
		  BookmarkFirst *newObject = new BookmarkFirst();

		  newObject->id = reader->ReadUInt32();
		  
		  newObject->itcFirst = reader->ReadByte();
		  newObject->fPub = GETBIT(newObject->itcFirst, 7);
		  newObject->itcFirst = GETBITS(newObject->itcFirst, 0, 6);
		  
		  newObject->itcLim = reader->ReadByte();
		  newObject->fNative = GETBIT(newObject->itcLim, 6);
		  newObject->fCol = GETBIT(newObject->itcLim, 7);
		  newObject->itcLim = GETBITS(newObject->itcLim, 0, 5);

		  return static_cast<ByteStructure*>(newObject);
	  }

}
