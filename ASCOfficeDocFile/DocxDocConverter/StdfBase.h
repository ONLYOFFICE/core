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

#include "GRFSTD.h"
#include "Constants.h"

namespace ASCDocFileFormat
{
  union StdfBase
  {
    private:
	  static const unsigned char SIZE_IN_BYTES = 10;

	  struct
	  {
	    unsigned short sti:12;
		unsigned short fScratch:1;
		unsigned short fInvalHeight:1;
		unsigned short fHasUpe:1;
		unsigned short fMassCopy:1;
		unsigned short stk:4;
		unsigned short istdBase:12;
		unsigned short cupx:4;
		unsigned short istdNext:12;
		unsigned short bchUpe:16;
		unsigned short grfstd:16;
	  } StdfBaseStruct;
	  unsigned char StdfBaseBytes[SIZE_IN_BYTES];

    public:
	  StdfBase()
	  {
	    memset( this->StdfBaseBytes, 0, SIZE_IN_BYTES );
	  }

	  explicit StdfBase( unsigned short _sti, Constants::StyleType _stk, unsigned short _istdBase, 
		        unsigned short _istdNext, const GRFSTD& _grfstd )
	  {
	    memset( this->StdfBaseBytes, 0, SIZE_IN_BYTES );

	    if ( _sti > 0x0FFE )
	    {
	      this->StdfBaseStruct.sti = 0x0FFE;
	    }
	    else
	    {
	      this->StdfBaseStruct.sti = _sti;
	    }

	    this->StdfBaseStruct.stk = (unsigned short)_stk;

	    if ( _istdBase > 0x0FFF )
	    {
	      this->StdfBaseStruct.istdBase = 0x0FFF;
	    }
	    else
	    {
	      this->StdfBaseStruct.istdBase = _istdBase;
	    }

	    //!!!TODO: cupx for revision-marked-style!!!
	    switch ( _stk )
	    {
	      case Constants::styleTypeParagraph:
		  {
		    this->StdfBaseStruct.cupx = 2;
		  }
		  break;

		  case Constants::styleTypeCharacter:
		  {
		    this->StdfBaseStruct.cupx = 1;
		  }
		  break;

		  case Constants::styleTypeTable:
		  {
		    this->StdfBaseStruct.cupx = 3;
		  }
		  break;

		  case Constants::styleTypeNumbering:
		  {
		    this->StdfBaseStruct.cupx = 1;
	 	  }
	      break;
	    }

	    if ( _istdNext > 0x0FFF )
	    {
	      this->StdfBaseStruct.istdNext = 0x0FFF;
	    }
	    else
	    {
	      this->StdfBaseStruct.istdNext = _istdNext;
	    }

	    this->StdfBaseStruct.grfstd = (unsigned short)_grfstd;
	  }

	  StdfBase( const StdfBase& _stdfBase )
	  {
	    memset( this->StdfBaseBytes, 0, SIZE_IN_BYTES );
		memcpy( this->StdfBaseBytes, _stdfBase.StdfBaseBytes, SIZE_IN_BYTES );
	  }

	  operator unsigned char*() const
	  {
	    return (unsigned char*)(this->StdfBaseBytes);
	  }
  };
}