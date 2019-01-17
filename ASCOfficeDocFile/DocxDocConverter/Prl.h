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

#include "IStreamWriter.h"
#include "../Common/SPRMCodes.h"
#include "../Common/FormatUtils.h"

namespace ASCDocFileFormat
{
	class Prl: public IStreamWriter
	{
	private:
		union
		{
			struct
			{
				unsigned int ispmd:9;
				unsigned int fSpec:1;
				unsigned int sgc:3;
				unsigned int spra:3;
			} sprmstruct;
			unsigned short sprmshort;
		} sprm;

		DocFileFormat::OperationCode OpCode;
		unsigned char* operand;
		unsigned short operandSize;

		/// Get be used to get the size of the sprm's operand.
		/// Returns 0 if the Operation failed and 255 if the size is variable
		unsigned char GetOperandSize( unsigned char spra )
		{
			switch ( spra )
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

	public:

		Prl ()		  
		{
			operand			=	NULL;
			operandSize		=	0;

			sprm.sprmshort	=	0;
			OpCode			=	(DocFileFormat::OperationCode)0;
		}

		Prl (short inSprm, unsigned char* operandBytes)
		{
			operand			=	NULL;
			operandSize		=	0;

			sprm.sprmshort	=	inSprm;
			OpCode			=	(DocFileFormat::OperationCode)(sprm.sprmshort);
			unsigned char opSize		=	GetOperandSize(sprm.sprmstruct.spra);

			if ( opSize == 255 )
			{
				switch (sprm.sprmshort)
				{
				case 0xD608: //sprmTDefTable
				case 0xD606: //sprmTDefTable10
					{
						//!!!TODO!!!
						this->operandSize = FormatUtils::BytesToUInt16( operandBytes, 0, 2 ) + 1;
						this->operand = new unsigned char[this->operandSize];
						memset( this->operand, 0, this->operandSize );

						if ( operandBytes != NULL )
						{
							memcpy( this->operand, operandBytes, this->operandSize );
						}
					}
					break;

				case 0xC615: //sprmPChgTabs 
					{
						//!!!TODO!!!
					}
					break;

				default:
					{
						//!!!TODO!!!
						this->operandSize = ( operandBytes[0] + 1 );
						this->operand = new unsigned char[this->operandSize];
						memset( this->operand, 0, this->operandSize );

						if ( operandBytes != NULL )
						{
							memcpy( this->operand,  operandBytes, this->operandSize );
						}
					}

					break;
				}
			}
			else if ( opSize != 0 )
			{
				operandSize =	opSize;
				operand		=	new unsigned char[operandSize];
				
				if (NULL != operand)
					memset(operand, 0, operandSize);

				if (NULL != operandBytes)
					memcpy(operand,  operandBytes, operandSize);
			}
		}

		Prl (const Prl& oPrl)
		{
			sprm.sprmshort	=	oPrl.sprm.sprmshort;
			OpCode			=	oPrl.OpCode;
			operandSize		=	oPrl.operandSize;

			operand = new unsigned char[operandSize];
			if (operand)
			{
				memcpy(operand, oPrl.operand, operandSize);
			}
		}

		bool operator == (const Prl& oPrl)
		{
			unsigned short minOperandSize = (std::min)(operandSize, oPrl.operandSize);	 
			return ((sprm.sprmshort == oPrl.sprm.sprmshort) && (0 == memcmp(operand, oPrl.operand, minOperandSize)));
		}

		bool operator != (const Prl& oPrl)
		{
			return !( this->operator == (oPrl) );
		}

		Prl& operator = (const Prl& oPrl)
		{
			if (*this != oPrl)
			{
				sprm.sprmshort	=	oPrl.sprm.sprmshort;
				OpCode			=	oPrl.OpCode;
				operandSize		=	oPrl.operandSize;

				RELEASEARRAYOBJECTS (operand);
				
				operand			=	new unsigned char[operandSize];
				if(operand)
				{
					memcpy(operand, oPrl.operand, operandSize);
				}
			}

			return *this;
		}

		virtual ~Prl()
		{
			RELEASEARRAYOBJECTS (operand);
		}

		virtual unsigned char* GetBytes (unsigned long* size) const
		{
			unsigned char* bytes = NULL;

			if ( size != NULL )
			{
				*size = ( sizeof(sprm.sprmshort) + operandSize );
				bytes = new unsigned char[*size];

				if ( bytes != NULL )
				{
					memset( bytes, 0, *size );
					FormatUtils::SetBytes( bytes, sprm.sprmshort );
					memcpy( ( bytes + 2 ), operand, operandSize );
				}
			}

			return bytes;
		}

		unsigned long Size() const
		{
			return ( sizeof(sprm.sprmshort) + operandSize );
		}

		unsigned short GetSprmCode() const
		{
			return sprm.sprmshort;
		}

		//
		inline unsigned short GetOpTwo ()	const							//	return two bytes value
		{
			return (unsigned short)(operand[0] | operand[1] << 8);
		}
	};
}