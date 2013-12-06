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
			byte fDiffer:1;
			byte fZombieEmbed:1;
			byte fResultsDirty:1;
			byte fResultsEdited:1;
			byte fLocked:1;
			byte fPrivateResult:1;
			byte fNested:1;
			byte fHasSep:1;
		} grffldEndStruct;
		byte grffldEndByte;

	public:
		explicit grffldEnd( byte _grffldEnd = 0 ):grffldEndByte(_grffldEnd)
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

		operator byte () const
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

		Fld (byte _fldch, byte _grffld)
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

		inline byte GetFieldCharacterType() const
		{
			return bytes[0];
		}

		inline byte GetFieldCharacterProperties() const
		{
			return bytes[1];
		}

		inline void SetProperties (byte grffld)
		{
			bytes [ 1 ]	=	grffld;
		}

	private:

		static const byte SIZE_IN_BYTES = 2;
		byte bytes[SIZE_IN_BYTES];
	};

	typedef PLC<Fld> Plcfld;
}