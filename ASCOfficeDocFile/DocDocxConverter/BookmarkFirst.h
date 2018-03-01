/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

#include "VirtualStreamReader.h"
#include "ByteStructure.h"

namespace DocFileFormat
{
  class BookmarkFirst: public ByteStructure
  {
    private:
      /// An unsigned integer that specifies a zero-based index into the PlcfBkl or PlcfBkld 
      /// that is paired with the PlcfBkf  or PlcfBkfd containing this FBKF.
      /// The entry found at said index specifies the location of the end of the bookmark associated with this FBKF.
      /// Ibkl MUST be unique for all FBKFs inside a given PlcfBkf or PlcfBkfd.
      short ibkl;
      /// A BKC that specifies further information about the bookmark associated with this FBKF.
      short bkc;

    public:
      static const int STRUCTURE_SIZE = 4;

	  BookmarkFirst()
	  {
	  }

	  short GetIndex() const
	  {
	    return this->ibkl;
	  }

	  short GetInformation() const
	  {
	    return this->bkc;
	  }
	
	  virtual ~BookmarkFirst()
	  {
	  }

      virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length )
      {
        BookmarkFirst *newObject = new BookmarkFirst();

	    newObject->ibkl = reader->ReadInt16();
        newObject->bkc = reader->ReadInt16();

	    return static_cast<ByteStructure*>( newObject );
      }
  };
}
