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

#include "StyleSheetDescription.h"

namespace DocFileFormat
{
	/// Creates an empty STD object
	StyleSheetDescription::StyleSheetDescription():	papx(NULL), chpx(NULL), tapx(NULL), sti(User),
													fAutoRedef(false),
													fHidden(false),
													f97LidsSet(false),
													fCopyLang(false),
													fPersonalCompose(false),
													fPersonalReply(false),
													fPersonal(false),
													fNoHtmlExport(false),
													fSemiHidden(false),
													fLocked(false),
													fInternalUse(false), fUnhideWhenUsed(false),
													stk(paragraph_style), uiPriority(0), fQFormat(false),
													istdBase(0xffffff), cupx (0), istdNext(0xffffff), bchUpe(0)
	{

	}

	StyleSheetDescription::~StyleSheetDescription()
	{
		RELEASEOBJECT(papx);
		RELEASEOBJECT(chpx);
		RELEASEOBJECT(tapx);
	}

	/// Parses the bytes to retrieve a StyleSheetDescription
	StyleSheetDescription::StyleSheetDescription (unsigned char* bytes, int size, int cbStdBase, POLE::Stream* dataStream, int nWordVersion) :
													papx(NULL), chpx(NULL), tapx(NULL), sti(User),
													fAutoRedef(false),
													fHidden(false),
													f97LidsSet(false),
													fCopyLang(false),
													fPersonalCompose(false),
													fPersonalReply(false),
													fPersonal(false),
													fNoHtmlExport(false),
													fSemiHidden(false),
													fLocked(false),
													fInternalUse(false),
													stk(paragraph_style), uiPriority(0), fQFormat(false),
													istdBase(0xffffff), cupx (0), istdNext(0xffffff), bchUpe(0)
	{
//parsing the base (fix part)

		if ( cbStdBase >= 2 )
		{
			sti						=	(StyleIdentifier)FormatUtils::GetUIntFromBytesBits( bytes, size, 0, 12 );
			fScratch				=	FormatUtils::GetBitFromBytes( bytes, size, 12 );
			fInvalHeight			=	FormatUtils::GetBitFromBytes( bytes, size, 13 );
			fHasUpe					=	FormatUtils::GetBitFromBytes( bytes, size, 14 );
			fMassCopy				=	FormatUtils::GetBitFromBytes( bytes, size, 15 );
		}

		if ( cbStdBase >= 4 )
		{
			stk						=	(StyleKind)FormatUtils::GetUIntFromBytesBits( bytes, size, 16, 4 );
			istdBase				=	FormatUtils::GetUIntFromBytesBits( bytes, size, 20, 12 );
		}
		if ( cbStdBase >= 6 )
		{
			cupx					=	(unsigned short)FormatUtils::GetUIntFromBytesBits( bytes, size, 32, 4 );
			istdNext				=	FormatUtils::GetUIntFromBytesBits( bytes, size, 36, 12 );
		}
		if ( cbStdBase >= 8 )
		{
			bchUpe					=	(unsigned short)FormatUtils::GetUIntFromBytesBits( bytes, size, 48, 16 );
		}
		if ( cbStdBase >= 10 )
		{
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
			fUnhideWhenUsed			=	FormatUtils::GetBitFromBytes(bytes, size, 75);
			fQFormat				=	FormatUtils::GetBitFromBytes(bytes, size, 76);
			//fReserved 3 bits
		}
		if ( cbStdBase == 0x0012)
		{//StdfPost2000
			istdLink =	FormatUtils::GetUIntFromBytesBits( bytes, size, 80, 12 );
			fHasOriginalStyle =	FormatUtils::GetBitFromBytes( bytes, size, 92 );
			short fSpare = FormatUtils::GetUIntFromBytesBits(bytes, size, 93, 3); //ignored
			rsid =	FormatUtils::GetUIntFromBytesBits( bytes, size, 96, 32 );
			iftcHtml = FormatUtils::GetUIntFromBytesBits(bytes, size, 128, 3);
			uiPriority = FormatUtils::GetUIntFromBytesBits(bytes, size, 132, 12);
		}

//parsing the variable part
		unsigned char	*name = NULL;
		unsigned char	characterCount = bytes[cbStdBase];
		int				upxOffset = 0;

		if (nWordVersion > 0)
		{
			name = new unsigned char[characterCount];//characters are zero-terminated, so 1 char has 2 bytes:
			memcpy( name, ( bytes + cbStdBase + 1 ), ( characterCount  ) );
			FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &(xstzName), name, ( characterCount ), ENCODING_WINDOWS_1250 );
			upxOffset = cbStdBase + 1 + ( characterCount /** 2*/ ) + 1;
		}
		else
		{
			name = new unsigned char[characterCount * 2];//characters are zero-terminated, so 1 char has 2 bytes:
			memcpy( name, ( bytes + cbStdBase + 2 ), ( characterCount * 2 ) );
			//remove zero-termination
			FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &(xstzName), name, ( characterCount * 2 ), ENCODING_UTF16 );
			
			//parse the UPX structs
			upxOffset = cbStdBase + 1 + ( characterCount * 2 ) + 2;
		}
		RELEASEARRAYOBJECTS( name );

		for ( int i = 0; i < cupx; i++ )
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

				if ( stk == table_style )
				{
					//first upx is TAPX; second PAPX, third CHPX
					switch ( i )
					{
					case 0:
						{
							//todooo не реализовано
							//RELEASEOBJECT( tapx );
							//tapx = new TablePropertyExceptions( upxBytes, cbUPX,  dataStream, nWordVersion); 
						}
						break;

					case 1:
						{
							RELEASEOBJECT( papx );
							papx = new ParagraphPropertyExceptions( upxBytes, cbUPX, dataStream, nWordVersion);
						}
						break;

					case 2: 
						{
							RELEASEOBJECT( chpx ); 
							chpx = new CharacterPropertyExceptions( upxBytes, cbUPX , nWordVersion); 
						}
						break;
					}
				}
				else if ( stk == paragraph_style )
				{
					//first upx is PAPX, second CHPX
					switch ( i )
					{
					case 0:
						{  
							RELEASEOBJECT( papx );
							papx = new ParagraphPropertyExceptions( upxBytes, cbUPX, dataStream, nWordVersion ); 
						}
						break;

					case 1: 
						{
							RELEASEOBJECT( chpx );
							chpx = new CharacterPropertyExceptions( upxBytes, cbUPX, nWordVersion); 
						}
						break;
					}
				}
				else if ( stk == numbering_style )
				{
					//list styles have only one PAPX
					switch ( i )
					{
					case 0:
						{
							RELEASEOBJECT( papx );
							papx = new ParagraphPropertyExceptions( upxBytes, cbUPX, dataStream, nWordVersion );
						}
						break;
					}
				}
				else if ( stk == character_style )
				{
					//character styles have only one CHPX
					switch ( i )
					{
					case 0: 
						{  
							RELEASEOBJECT( chpx );
							chpx = new CharacterPropertyExceptions( upxBytes, cbUPX, nWordVersion); 
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