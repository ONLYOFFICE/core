/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#include "SinglePropertyModifier.h"

#include "../Common/FormatUtils.h"

namespace DocFileFormat
{
	SinglePropertyModifier::SinglePropertyModifier() : Arguments(NULL), OpCode(sprmPIstd), fSpec(false), Type(PAP), argumentsSize(0)
	{
	}

	/// parses the unsigned char to retrieve a SPRM
	SinglePropertyModifier::SinglePropertyModifier(unsigned char* bytes, int size) : Arguments(NULL), OpCode(sprmPIstd), fSpec(false), Type(PAP), argumentsSize(0)
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
		unsigned char spra = (unsigned char)( (int)this->OpCode >> 13 );
		unsigned char opSize = GetOperandSize( spra );

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
					//and the arguments start at the unsigned char after that (byte3)
					this->Arguments = new unsigned char[this->argumentsSize];
					//Arguments start at unsigned char 4
					memcpy( this->Arguments, ( bytes + 4 ), this->argumentsSize );
				}
				break;

			case sprmPChgTabs:
				{
					this->argumentsSize = bytes[2];
					this->Arguments = new unsigned char[this->argumentsSize];
					memcpy( this->Arguments, ( bytes + 3 ), this->argumentsSize );
				}
				break;

			default:
				{
					//the variable length stand in the unsigned char after the opcode (byte2)
					opSize = bytes[2];
					this->argumentsSize = opSize;
					//and the arguments start at the unsigned char after that (byte3)
					this->Arguments = new unsigned char[this->argumentsSize];
					memcpy( this->Arguments, ( bytes + 3 ), this->argumentsSize );
				}

				break;
			}
		}
		else
		{
			this->argumentsSize = opSize;
			this->Arguments = new unsigned char[this->argumentsSize];
			memcpy( this->Arguments, ( bytes + 2 ), this->argumentsSize );
		}
	}

	SinglePropertyModifier::SinglePropertyModifier(const SinglePropertyModifier& spm) : Arguments(NULL), OpCode(sprmPIstd), fSpec(false), Type(PAP), argumentsSize(0)
	{
		if ( spm.Arguments != NULL )
		{
			this->argumentsSize = spm.argumentsSize;
			this->Arguments = new unsigned char[this->argumentsSize];
			memcpy( this->Arguments, spm.Arguments, this->argumentsSize );
			this->fSpec = spm.fSpec;
			this->OpCode = spm.OpCode;
			this->Type = spm.Type;
		}
	}

	bool SinglePropertyModifier::operator == (const SinglePropertyModifier& spm) const
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

	bool SinglePropertyModifier::operator != (const SinglePropertyModifier& spm) const
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

			Arguments		=	new unsigned char[argumentsSize];
            memcpy(Arguments, spm.Arguments, spm.argumentsSize);
		}

		return *this;
	}

	SinglePropertyModifier::~SinglePropertyModifier()
	{
		RELEASEARRAYOBJECTS(Arguments);
	}

	/// Get be used to get the size of the sprm's operand.
	/// Returns 0 if the Operation failed and 255 if the size is variable
	unsigned char SinglePropertyModifier::GetOperandSize(unsigned char spra)
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
