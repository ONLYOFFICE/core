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

namespace ASCDocFileFormat
{
  union GRFSTD
  {
    private:
	  struct
	  {
	    unsigned short fAutoRedef:1;
		unsigned short fHidden:1;
		unsigned short f97LidsSet:1;
        unsigned short fCopyLang:1;
        unsigned short fPersonalCompose:1;
        unsigned short fPersonalReply:1;
        unsigned short fPersonal:1;
        unsigned short fNoHtmlExport:1;
        unsigned short fSemiHidden:1;
        unsigned short fLocked:1;
        unsigned short fInternalUse:1;
        unsigned short fUnhideWhenUsed:1;
        unsigned short fQFormat:1;
        unsigned short fReserved:3;
	  } GRFSTDStruct;
	  unsigned short GRFSTDUnsignedShort;

    public:
	  GRFSTD():
      GRFSTDUnsignedShort(0)
	  {
	  }

	  explicit GRFSTD( unsigned short _grfstd ):
      GRFSTDUnsignedShort(_grfstd)
	  {
	  }

	  explicit GRFSTD( bool _fAutoRedef, bool _fHidden, bool _fPersonalCompose, bool _fPersonalReply, bool _fPersonal, 
		      bool _fSemiHidden, bool _fLocked, bool _fUnhideWhenUsed, bool _fQFormat ):
      GRFSTDUnsignedShort(0)
	  {
	    ( _fAutoRedef ) ? ( this->GRFSTDStruct.fAutoRedef = 1 ) : ( this->GRFSTDStruct.fAutoRedef = 0 );
		( _fHidden ) ? ( this->GRFSTDStruct.fHidden = 1 ) : ( this->GRFSTDStruct.fHidden = 0 );
		this->GRFSTDStruct.f97LidsSet = 0;
		this->GRFSTDStruct.fCopyLang = 0;
		( _fPersonalCompose ) ? ( this->GRFSTDStruct.fPersonalCompose = 1 ) : ( this->GRFSTDStruct.fPersonalCompose = 0 );
		( _fPersonalReply ) ? ( this->GRFSTDStruct.fPersonalReply = 1 ) : ( this->GRFSTDStruct.fPersonal = 0 );
		( _fPersonal ) ? ( this->GRFSTDStruct.fPersonal = 1 ) : ( this->GRFSTDStruct.fPersonal = 0 );
		this->GRFSTDStruct.fNoHtmlExport = 0;
		( _fSemiHidden ) ? ( this->GRFSTDStruct.fSemiHidden = 1 ) : ( this->GRFSTDStruct.fSemiHidden = 0 );
		( _fLocked ) ? ( this->GRFSTDStruct.fLocked = 1 ) : ( this->GRFSTDStruct.fLocked = 0 );
		this->GRFSTDStruct.fInternalUse = 0;
		( _fUnhideWhenUsed ) ? ( this->GRFSTDStruct.fUnhideWhenUsed = 1 ) : ( this->GRFSTDStruct.fUnhideWhenUsed = 0 );
		( _fQFormat ) ? ( this->GRFSTDStruct.fQFormat = 1 ) : ( this->GRFSTDStruct.fQFormat = 0 );
		this->GRFSTDStruct.fReserved = 0;
	  }

	  operator unsigned short() const
	  {
	    return this->GRFSTDUnsignedShort;
	  }
  };
}