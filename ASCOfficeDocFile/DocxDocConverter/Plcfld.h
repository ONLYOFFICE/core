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

#include "PLC.h"
#include "Constants.h"

namespace ASCDocFileFormat
{
	union grffldEnd
	{
	private:
		struct
		{
			unsigned char fDiffer:1;
			unsigned char fZombieEmbed:1;
			unsigned char fResultsDirty:1;
			unsigned char fResultsEdited:1;
			unsigned char fLocked:1;
			unsigned char fPrivateResult:1;
			unsigned char fNested:1;
			unsigned char fHasSep:1;
		} grffldEndStruct;
		unsigned char grffldEndByte;

	public:
		explicit grffldEnd( unsigned char _grffldEnd = 0 ):grffldEndByte(_grffldEnd)
		{
		}

		grffldEnd( bool _fDiffer, bool _fZombieEmbed, bool _fResultsDirty, bool _fResultsEdited, bool _fLocked, bool _fPrivateResult, bool _fNested, bool _fHasSep )
		{
			( _fDiffer ) ? ( this->grffldEndStruct.fDiffer = 1 ) : ( this->grffldEndStruct.fDiffer = 0 );
			( _fZombieEmbed ) ? ( this->grffldEndStruct.fZombieEmbed = 1 ) : ( this->grffldEndStruct.fZombieEmbed = 0 );
			( _fResultsDirty ) ? ( this->grffldEndStruct.fResultsDirty = 1 ) : ( this->grffldEndStruct.fResultsDirty = 0 );
			( _fResultsEdited ) ? ( this->grffldEndStruct.fResultsEdited = 1 ) : ( this->grffldEndStruct.fResultsEdited = 0 );
			( _fLocked ) ? ( this->grffldEndStruct.fLocked = 1 ) : ( this->grffldEndStruct.fLocked = 0 );
			( _fPrivateResult ) ? ( this->grffldEndStruct.fPrivateResult = 1 ) : ( this->grffldEndStruct.fPrivateResult = 0 );
			( _fNested ) ? ( this->grffldEndStruct.fNested = 1 ) : ( this->grffldEndStruct.fNested = 0 );
			( _fHasSep ) ? ( this->grffldEndStruct.fHasSep = 1 ) : ( this->grffldEndStruct.fHasSep = 0 );
		}

		operator unsigned char () const
		{
			return this->grffldEndByte;
		} 
	};

	class Fld
	{
	public:

		Fld ()
		{
			memset(bytes, 0, SIZE_IN_BYTES);
		}

		Fld (unsigned char _fldch, unsigned char _grffld)
		{
			memset( bytes, 0, SIZE_IN_BYTES );

			bytes[0] = _fldch;
			bytes[1] = _grffld;
		}

		Fld (const Fld& oFld)
		{
			memset ( bytes, 0, SIZE_IN_BYTES );
			memcpy ( bytes, oFld.bytes, SIZE_IN_BYTES );
		}

		Fld& operator = ( const Fld& _fld )
		{
			if ( this != &_fld )
			{
				memset( bytes, 0, SIZE_IN_BYTES );
				memcpy( bytes, _fld.bytes, SIZE_IN_BYTES );  
			}

			return *this;
		}

		inline unsigned char GetFieldCharacterType() const
		{
			return bytes[0];
		}

		inline unsigned char GetFieldCharacterProperties() const
		{
			return bytes[1];
		}

		inline void SetProperties (unsigned char grffld)
		{
			bytes [ 1 ]	=	grffld;
		}

	private:

		static const unsigned char SIZE_IN_BYTES = 2;
		unsigned char bytes[SIZE_IN_BYTES];
	};

	typedef PLC<Fld> Plcfld;
}