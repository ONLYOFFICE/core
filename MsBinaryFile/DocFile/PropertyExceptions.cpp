/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "PropertyExceptions.h"

#include "../Common/Base/FormatUtils.h"

namespace DocFileFormat
{
	PropertyExceptions::~PropertyExceptions()
	{
		RELEASEOBJECT( grpprl );
	}

	PropertyExceptions::PropertyExceptions() : grpprl(NULL)
	{
		grpprl = new std::vector<SinglePropertyModifier>();
	}

	PropertyExceptions::PropertyExceptions( const std::vector<SinglePropertyModifier>& _grpprl ) : grpprl(NULL)
	{
		grpprl = new std::vector<SinglePropertyModifier>( _grpprl );
	}

	PropertyExceptions::PropertyExceptions( unsigned char* bytes, int size, int nWordVersion ) : grpprl(NULL)
	{
		if (nWordVersion >= 2)//word 2.0 or 1.0
			return;

		ReadExceptions(bytes, size, nWordVersion);
	}
	void PropertyExceptions::ReadExceptions(unsigned char* bytes, int size, int nWordVersion)
	{
 		RELEASEOBJECT( grpprl );

		grpprl = new std::vector<SinglePropertyModifier>();

		if ( ( bytes == NULL ) || ( size == 0 ) ) return;

		//read the sprms
	              
		int sprmStart = 0;
		bool goOn = true;

		int opCodeSize = (nWordVersion > 0) ? 1 : 2;

		while ( goOn )
		{
			if ( ( sprmStart + opCodeSize ) <= size )
			{
				unsigned short code  = (nWordVersion > 0) ?	FormatUtils::BytesToUChar	( bytes, sprmStart, size ) :
															FormatUtils::BytesToUInt16	( bytes, sprmStart, size ) ;

				OperationCode opCode = (OperationCode)(nWordVersion == 2 ?  OpCode93To95[code] : code);
				
				if (nWordVersion > 0 && opCode == 0) 
				{
					sprmStart++;
					continue;
				}				

				//if (nWordVersion == 2)
				//	bytes[sprmStart]= (unsigned char)opCode;
				short opSize = -1;

				if (nWordVersion > 0)
				{
					opSize = (short)SinglePropertyModifier::GetOldOperandSize( (unsigned char)opCode );
				}
				else
				{
					unsigned char spra = (unsigned char)( (int)opCode >> 13 ); 
					opSize = (short)SinglePropertyModifier::GetOperandSize( spra );
				}
				unsigned char lenByte = 0;

				//operand has variable size
				if ( opSize == 255 )
				{
					//some opCode need special treatment
					switch ( opCode )
					{
						case sprmOldTDefTable:
						case sprmOldTDefTable10:
						case sprmTDefTable:
						case sprmTDefTable10:
						{
							//The opSize of the table definition is stored in 2 bytes instead of 1
							lenByte = 2;
							opSize = FormatUtils::BytesToInt16( bytes, ( sprmStart + opCodeSize ), size );
							//Word adds an additional unsigned char to the opSize to compensate the additional
							//unsigned char needed for the length
							opSize--;
						}break;				  
				      
						case sprmOldPChgTabs:
						case sprmPChgTabs:
						{
							//The tab operand can be bigger than 255 bytes (length unsigned char is set to 255).
							//In this case a special calculation of the opSize is needed
							lenByte = 1;
							opSize = bytes[sprmStart + opCodeSize];

							if ( opSize == 255 )
							{
							  unsigned char itbdDelMax = bytes[sprmStart + opCodeSize + 1];
							  unsigned char itbdAddMax = bytes[sprmStart + opCodeSize + 1 + 2 * itbdDelMax];
							  
							  opSize = (short)( ( itbdDelMax * 4 + itbdAddMax * 3 ) - 1 );
							}
						}break;	

						case sprmOldPAnld:
							{
								lenByte = 1;
								opSize	= bytes[sprmStart + opCodeSize];
							}break;
				      
						default:
						{  
							//The variable length stand in the unsigned char after the opcode
							lenByte = 1;
							opSize = bytes[sprmStart + opCodeSize];
						}
						break;
					}
				}

				if (opSize < 0)
				{
					break;
				}

				//length is 2byte for the opCode, lenByte for the length, opSize for the length of the operand
				int sprmBytesSize = opCodeSize + lenByte + opSize;
				unsigned char* sprmBytes = NULL;

				sprmBytes = new unsigned char[sprmBytesSize];

				//if ( size >= ( sprmStart + sprmBytesSize ) )
				{
					int sz = (std::min)(sprmBytesSize, size - sprmStart);

					memcpy( sprmBytes, ( bytes + sprmStart ), sz );

					SinglePropertyModifier sprm( sprmBytes, sz, nWordVersion);
					grpprl->push_back( sprm );

					sprmStart += sz;
				}
				//else
				//{
				//	goOn = false;
				//}

				RELEASEARRAYOBJECTS( sprmBytes );
			}
			else
			{
				goOn = false;
			}		
		}
	}

}