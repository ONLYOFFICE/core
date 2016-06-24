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

#include "StyleSheetDescription.h"

namespace DocFileFormat
{
	/// Creates an empty STD object
	StyleSheetDescription::StyleSheetDescription() : papx(NULL), chpx(NULL), tapx(NULL)
	{

	}

	StyleSheetDescription::~StyleSheetDescription()
	{
		RELEASEOBJECT(papx);
		RELEASEOBJECT(chpx);
		RELEASEOBJECT(tapx);
	}

	/// Parses the bytes to retrieve a StyleSheetDescription
	StyleSheetDescription::StyleSheetDescription (unsigned char* bytes, int size, int cbStdBase, POLE::Stream* dataStream) : papx(NULL), chpx(NULL), tapx(NULL)
	{
		//parsing the base (fix part)

		if ( cbStdBase >= 2 )
		{
			//sti
			sti						=	(StyleIdentifier)FormatUtils::GetUIntFromBytesBits( bytes, size, 0, 12 );
			//flags
			fScratch				=	FormatUtils::GetBitFromBytes( bytes, size, 12 );
			fInvalHeight			=	FormatUtils::GetBitFromBytes( bytes, size, 13 );
			fHasUpe					=	FormatUtils::GetBitFromBytes( bytes, size, 14 );
			fMassCopy				=	FormatUtils::GetBitFromBytes( bytes, size, 15 );
		}

		if ( cbStdBase >= 4 )
		{
			//stk
			stk						=	(StyleKind)FormatUtils::GetUIntFromBytesBits( bytes, size, 16, 4 );
			//istdBase	
			istdBase				=	FormatUtils::GetUIntFromBytesBits( bytes, size, 20, 12 );
		}

		if ( cbStdBase >= 6 )
		{
			//cupx
			cupx					=	(unsigned short)FormatUtils::GetUIntFromBytesBits( bytes, size, 32, 4 );
			//istdNext
			istdNext				=	FormatUtils::GetUIntFromBytesBits( bytes, size, 36, 12 );
		}

		if ( cbStdBase >= 8 )
		{
			//bchUpe
			bchUpe					=	(unsigned short)FormatUtils::GetUIntFromBytesBits( bytes, size, 48, 16 );
		}

		if ( cbStdBase >= 10 )
		{
			//flags
			fAutoRedef				=	FormatUtils::GetBitFromBytes( bytes, size, 64 );
			fHidden					=	FormatUtils::GetBitFromBytes( bytes, size, 65 );
			f97LidsSet				=	FormatUtils::GetBitFromBytes( bytes, size, 66 );
			fCopyLang				=	FormatUtils::GetBitFromBytes( bytes, size, 67 );
			fPersonalCompose		=	FormatUtils::GetBitFromBytes( bytes, size, 68 );
			fPersonalReply			=	FormatUtils::GetBitFromBytes( bytes, size, 69 );
			fPersonal				=	FormatUtils::GetBitFromBytes( bytes, size, 70 );
			fNoHtmlExport			=	FormatUtils::GetBitFromBytes( bytes, size, 71 );
			fSemiHidden				=	FormatUtils::GetBitFromBytes( bytes, size, 72 );
			fLocked					=	FormatUtils::GetBitFromBytes( bytes, size, 73 );
			fInternalUse			=	FormatUtils::GetBitFromBytes( bytes, size, 74 );
		}

		if ( cbStdBase >= 12 )
		{
			//istdLink
			istdLink				=	FormatUtils::GetUIntFromBytesBits( bytes, size, 80, 12 );
			//fHasOriginalStyle
			fHasOriginalStyle		=	FormatUtils::GetBitFromBytes( bytes, size, 92 );
		}

		if ( cbStdBase >= 16 )
		{
			//rsid
			rsid					=	FormatUtils::GetUIntFromBytesBits( bytes, size, 96, 32 );
		}

		//parsing the variable part

		//xstz
		unsigned char characterCount = bytes[cbStdBase];
		//characters are zero-terminated, so 1 char has 2 bytes:
		unsigned char* name = new unsigned char[characterCount * 2];
		memcpy( name, ( bytes + cbStdBase + 2 ), ( characterCount * 2 ) );
		//remove zero-termination
		FormatUtils::GetSTLCollectionFromBytes<wstring>( &(this->xstzName), name, ( characterCount * 2 ), ENCODING_UNICODE );
		RELEASEARRAYOBJECTS( name );

		//parse the UPX structs
		int upxOffset = cbStdBase + 1 + ( characterCount * 2 ) + 2;

		for ( int i = 0; i < this->cupx; i++ )
		{
			//find the next even unsigned char border
			if ( ( upxOffset % 2 ) != 0 )
			{
				upxOffset++;
			}

			//get the count of bytes for UPX
			unsigned short cbUPX = FormatUtils::BytesToUInt16( bytes, upxOffset, size );

			if ( cbUPX > 0 )
			{
				//copy the bytes
				unsigned char* upxBytes = new unsigned char[cbUPX];
				memcpy( upxBytes, ( bytes + upxOffset + 2 ), cbUPX );

				if ( this->stk == table_style )
				{
					//first upx is TAPX; second PAPX, third CHPX
					switch ( i )
					{
					case 0:
						{
							RELEASEOBJECT( this->tapx );
							this->tapx = new TablePropertyExceptions( upxBytes, cbUPX ); 
						}
						break;

					case 1:
						{
							RELEASEOBJECT( this->papx );
							this->papx = new ParagraphPropertyExceptions( upxBytes, cbUPX, dataStream );
						}
						break;

					case 2: 
						{
							RELEASEOBJECT( this->chpx ); 
							this->chpx = new CharacterPropertyExceptions( upxBytes, cbUPX ); 
						}
						break;
					}
				}
				else if ( this->stk == paragraph_style )
				{
					//first upx is PAPX, second CHPX
					switch ( i )
					{
					case 0:
						{  
							RELEASEOBJECT( this->papx );
							this->papx = new ParagraphPropertyExceptions( upxBytes, cbUPX, dataStream ); 
						}
						break;

					case 1: 
						{
							RELEASEOBJECT( this->chpx );
							this->chpx = new CharacterPropertyExceptions( upxBytes, cbUPX ); 
						}
						break;
					}
				}
				else if ( this->stk == numbering_style )
				{
					//list styles have only one PAPX
					switch ( i )
					{
					case 0:
						{
							RELEASEOBJECT( this->papx );
							this->papx = new ParagraphPropertyExceptions( upxBytes, cbUPX, dataStream );
						}
						break;
					}
				}
				else if ( this->stk == character_style )
				{
					//character styles have only one CHPX
					switch ( i )
					{
					case 0: 
						{  
							RELEASEOBJECT( this->chpx );
							this->chpx = new CharacterPropertyExceptions( upxBytes, cbUPX ); 
						}
						break;
					}
				}

				RELEASEARRAYOBJECTS( upxBytes );
			}

			//increase the offset for the next run
			upxOffset += ( 2 + cbUPX );
		}
	}
}