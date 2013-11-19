#include "stdafx.h"
#include "SinglePropertyModifier.h"

namespace DocFileFormat
{
	SinglePropertyModifier::SinglePropertyModifier() : Arguments(NULL), OpCode(sprmPIstd), fSpec(false), Type(PAP), argumentsSize(0)
	{
	}

	/// parses the byte to retrieve a SPRM
	SinglePropertyModifier::SinglePropertyModifier(byte* bytes, int size) : Arguments(NULL), OpCode(sprmPIstd), fSpec(false), Type(PAP), argumentsSize(0)
	{
		//first 2 bytes are the operation code ...
		this->OpCode = (OperationCode)FormatUtils::BytesToUInt16( bytes, 0, size );

		//... whereof bit 9 is fSpec ...
		unsigned int j = (unsigned int)this->OpCode << 22;
		j = j >> 31;

		if ( j == 1 )
		{
			this->fSpec = true;
		}
		else
		{
			this->fSpec = false;
		}

		//... and bits 10,11,12 are the type ...
		unsigned int i = (unsigned int)this->OpCode << 19;

		i = i >> 29;
		this->Type = (SprmType)i;

		//... and last 3 bits are the spra
		byte spra = (byte)( (int)this->OpCode >> 13 );
		byte opSize = GetOperandSize( spra );

		if ( opSize == 255 )
		{
			switch ( OpCode )
			{
			case sprmTDefTable:
			case sprmTDefTable10:
				{
					//the variable length stand in the bytes 2 and 3
					short opSizeTable = FormatUtils::BytesToInt16( bytes, 2, size );
					this->argumentsSize = opSizeTable-1;
					//and the arguments start at the byte after that (byte3)
					this->Arguments = new byte[this->argumentsSize];
					//Arguments start at byte 4
					memcpy( this->Arguments, ( bytes + 4 ), this->argumentsSize );
				}
				break;

			case sprmPChgTabs:
				{
					this->argumentsSize = bytes[2];
					this->Arguments = new byte[this->argumentsSize];
					memcpy( this->Arguments, ( bytes + 3 ), this->argumentsSize );
				}
				break;

			default:
				{
					//the variable length stand in the byte after the opcode (byte2)
					opSize = bytes[2];
					this->argumentsSize = opSize;
					//and the arguments start at the byte after that (byte3)
					this->Arguments = new byte[this->argumentsSize];
					memcpy( this->Arguments, ( bytes + 3 ), this->argumentsSize );
				}

				break;
			}
		}
		else
		{
			this->argumentsSize = opSize;
			this->Arguments = new byte[this->argumentsSize];
			memcpy( this->Arguments, ( bytes + 2 ), this->argumentsSize );
		}
	}

	SinglePropertyModifier::SinglePropertyModifier(const SinglePropertyModifier& spm) : Arguments(NULL), OpCode(sprmPIstd), fSpec(false), Type(PAP), argumentsSize(0)
	{
		if ( spm.Arguments != NULL )
		{
			this->argumentsSize = spm.argumentsSize;
			this->Arguments = new byte[this->argumentsSize];
			memcpy( this->Arguments, spm.Arguments, this->argumentsSize );
			this->fSpec = spm.fSpec;
			this->OpCode = spm.OpCode;
			this->Type = spm.Type;
		}
	}

	bool SinglePropertyModifier::operator == (const SinglePropertyModifier& spm)
	{
		if ( ( this->argumentsSize == spm.argumentsSize ) && ( memcmp( this->Arguments, spm.Arguments, this->argumentsSize ) == 0 ) &&
			( this->fSpec == spm.fSpec ) && ( this->OpCode == spm.OpCode ) && ( this->Type == spm.Type ) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool SinglePropertyModifier::operator != (const SinglePropertyModifier& spm)
	{
		return !( *this == spm );
	}

	SinglePropertyModifier& SinglePropertyModifier::operator = (const SinglePropertyModifier& spm)
	{
		if ((*this != spm) && (NULL != spm.Arguments))
		{
			RELEASEARRAYOBJECTS(Arguments);

			OpCode			=	spm.OpCode;
			fSpec			=	spm.fSpec;
			Type			=	spm.Type;
			argumentsSize	=	spm.argumentsSize;

			Arguments		=	new byte[argumentsSize];
			memcpy_s(Arguments, argumentsSize, spm.Arguments, spm.argumentsSize);
		}

		return *this;
	}

	SinglePropertyModifier::~SinglePropertyModifier()
	{
		RELEASEARRAYOBJECTS(Arguments);
	}

	/// Get be used to get the size of the sprm's operand.
	/// Returns 0 if the Operation failed and 255 if the size is variable
	byte SinglePropertyModifier::GetOperandSize(byte spra)
	{
		switch (spra)
		{
		case 0: return 1;
		case 1: return 1;
		case 2: return 2;
		case 3: return 4;
		case 4: return 2;
		case 5: return 2;
		case 6: return 255;
		case 7: return 3;

		default: return 0;
		}
	}
}
