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

#include "SinglePropertyModifier.h"

#include "../Common/Base/FormatUtils.h"

namespace DocFileFormat
{
	SinglePropertyModifier::SinglePropertyModifier(OperationCode opCode, int argumentsSize, unsigned char* arguments) :
					Arguments(NULL), OpCode(opCode), fSpec(false), Type(PAP), argumentsSize(argumentsSize), nWordVersion (2)
	{
		Arguments = new unsigned char[argumentsSize];
		memcpy(Arguments, arguments, argumentsSize);
	}

	SinglePropertyModifier::SinglePropertyModifier(unsigned char* bytes, int size, int nWordVersion_) : 
					Arguments(NULL), OpCode(sprmPIstd), fSpec(false), Type(PAP), argumentsSize(0), nWordVersion (nWordVersion_)
	{
		unsigned char opSize		= 0;
		unsigned char opCodeSize	= 0;
		
		if (nWordVersion > 0)
		{
			opCodeSize = 1;
			//first 1 byte are the operation code ...
			unsigned char code = FormatUtils::BytesToUChar( bytes, 0, size );
			
			OpCode = (OperationCode)(nWordVersion == 2 ?  OpCode93To95[code] : code);

			if (OpCode == 0 && size == 4)
			{
				//так записывается rgb цвет (
				OpCode = sprmCCv;
				opSize = 3;
			}
			else
				opSize = GetOldOperandSize( (unsigned char)OpCode );		
		}
		else
		{
			opCodeSize = 2;
			//first 2 bytes are the operation code ...
			OpCode = (OperationCode)FormatUtils::BytesToUInt16( bytes, 0, size );
			//... whereof bit 9 is fSpec ...
			unsigned int j = (unsigned int)this->OpCode << 22;
			j = j >> 31;

			if ( j == 1 )
			{
				fSpec = true;
			}
			else
			{
				fSpec = false;
			}

			//... and bits 10,11,12 are the type ...
			unsigned int i = (unsigned int)OpCode << 19;

			i = i >> 29;
			Type = (SprmType)i;

			//... and last 3 bits are the spra
			unsigned char spra = (unsigned char)( (int)OpCode >> 13 );
			opSize = GetOperandSize( spra );		
		}

		if ( opSize == 255 )
		{
			switch ( OpCode )
			{
			case sprmOldTDefTable:
			case sprmOldTDefTable10:
			case sprmTDefTable:
			case sprmTDefTable10:
				{
					//the variable length stand in the bytes 2 and 3
					short opSizeTable = FormatUtils::BytesToInt16( bytes, opCodeSize, size );
					argumentsSize = opSizeTable-1;
					//and the arguments start at the unsigned char after that (byte3)
					Arguments = new unsigned char[argumentsSize];
					//Arguments start at unsigned char 4
					memcpy( Arguments, ( bytes + opCodeSize  + 2 ), argumentsSize );
				}
				break;

			case sprmOldPChgTabs:
			case sprmPChgTabs:
				{
					argumentsSize = bytes[2];
					Arguments = new unsigned char[argumentsSize];
					memcpy( Arguments, ( bytes + opCodeSize + 1 ), argumentsSize );
				}
				break;

			default:
				{
					//the variable length stand in the unsigned char after the opcode (byte2)
					opSize = bytes[opCodeSize];
					argumentsSize = opSize;
					//and the arguments start at the unsigned char after that (byte3)
					Arguments = new unsigned char[argumentsSize];
					memcpy( Arguments, ( bytes + opCodeSize + 1 ), argumentsSize );
				}

				break;
			}
		}
		else
		{
            argumentsSize = (std::min)(size - opCodeSize, (int)opSize);
			if (argumentsSize > 0)
			{
				Arguments = new unsigned char[argumentsSize];
				memcpy( Arguments, ( bytes + opCodeSize ), argumentsSize );
			}
		}
	}

	SinglePropertyModifier::SinglePropertyModifier(const SinglePropertyModifier& spm) : Arguments(NULL), OpCode(sprmPIstd), fSpec(false), Type(PAP), argumentsSize(0)
	{
		if ( spm.Arguments != NULL )
		{
			argumentsSize	= spm.argumentsSize;
			Arguments		= new unsigned char[argumentsSize];
			memcpy( Arguments, spm.Arguments, argumentsSize );
			fSpec			= spm.fSpec;
			OpCode			= spm.OpCode;
			Type			= spm.Type;
			nWordVersion 	= spm.nWordVersion;
		}
	}

	bool SinglePropertyModifier::operator == (const SinglePropertyModifier& spm) const
	{
		if ( ( argumentsSize == spm.argumentsSize ) && ( memcmp( Arguments, spm.Arguments, argumentsSize ) == 0 ) &&
			( fSpec == spm.fSpec ) && ( OpCode == spm.OpCode ) && ( Type == spm.Type ) )
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
			nWordVersion	=	spm.nWordVersion;

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
	static const unsigned char OldOperandSizeTable[] = 
	{
	//	0,0,2,255,1,1,1,1,1,1,1,1,255,1,1,255,2,2,2,2,4,2,2,255,1,1,2,2,2,1,2,2,2,2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,255,2,4,1,2,3,255,1,0,0,0,0,2,255,255,0,0,1,1,1,1,1,1,1,1,2,1,3,2,2,1,1,1,1,1,255,1,255,255,2,255,2,2,0,0,0,0,0,0,1,1,1,255,2,2,2,2,0,0,0,0,0,0,1,1,255,0,0,3,3,1,1,2,2,1,1,2,2,1,1,2,2,1,1,1,1,2,2,2,2,1,1,2,2,1,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,2,1,1,12,255,2,255,255,4,5,4,2,4,2,2,5,4,0,0,0,0,0,0,0,0
		0,0,2,255,1,1,1,1,1,1,1,1,255,1,1,255,2,2,2,2,4,2,2,255,1,1,2,2,2,1,2,2,2,2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,255,2,4,1,2,3,255,1,0,2,0,0,2,255,255,0,0,1,1,1,1,1,1,1,1,2,1,3,2,2,1,1,1,1,1,255,1,255,255,2,255,2,2,0,0,0,0,0,0,1,1,1,255,2,2,2,2,0,0,0,0,0,0,1,1,255,0,0,3,3,1,1,2,2,1,1,2,2,1,1,2,2,1,1,1,1,2,2,2,2,1,1,2,2,1,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,2,2,2,1,1,12,255,2,255,255,4,5,4,2,4,2,2,5,4,0,0,0,0,0,0,0,0

	};

	unsigned char SinglePropertyModifier::GetOldOperandSize(unsigned char code)
	{
		if (code < 2 || code > 207) return -1;

		return OldOperandSizeTable [code];
	}
}
