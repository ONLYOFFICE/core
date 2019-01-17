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
#pragma once

#include "Common.h"

namespace OfficeArt
{
#pragma pack(push, 1)
	
  class OfficeArtMetafileHeader
  {
    private:
	  unsigned int cbSize;
	  RECT rcBounds;
	  POINT ptSize;
	  unsigned int cbSave;
	  unsigned char compression;
      const unsigned char filter;

    public:
	  OfficeArtMetafileHeader():
	  cbSize(0), rcBounds(), ptSize(), cbSave(0), compression((unsigned char)COMPRESSION_METHOD_DEFLATE), filter(0xFE)
	  {
	  }

      explicit OfficeArtMetafileHeader( unsigned int _cbSize, RECT _rcBounds, POINT _ptSize, unsigned int _cbSave, COMPRESSION_METHOD _compression ):
	  cbSize(_cbSize), rcBounds(_rcBounds), ptSize(_ptSize), cbSave(_cbSave), compression((unsigned char)_compression), filter(0xFE)
	  {
	  }

	  unsigned int GetUncompressedSize() const
	  {
	    return this->cbSize;
	  }

	  RECT GetClippingRegion() const
	  {
	    return this->rcBounds;
	  }

	  POINT GetSizeToRender() const
	  {
	    return this->ptSize;
	  }

	  unsigned int GetCompressedSize() const
	  {
	    return this->cbSave;
	  }

	  COMPRESSION_METHOD GetCompressionMethod() const
	  {
	    return (COMPRESSION_METHOD)(this->compression);
	  }
  };

#pragma pack(pop)  
}