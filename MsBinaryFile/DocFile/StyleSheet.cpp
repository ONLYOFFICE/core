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

#include "StyleSheet.h"

namespace DocFileFormat
{
	StyleSheet::~StyleSheet()
	{
		RELEASEOBJECT (stshi);

		if ( NULL != Styles )
		{
			for ( std::vector<StyleSheetDescription*>::iterator iter = Styles->begin(); iter != Styles->end(); iter++ )
			{
				RELEASEOBJECT( *iter );  
			}

			RELEASEOBJECT( Styles );
		}
	}

	/*========================================================================================================*/

	/// Parses the streams to retrieve a StyleSheet.
	StyleSheet::StyleSheet (FileInformationBlock* fib, POLE::Stream* tableStream, POLE::Stream* dataStream) : stshi(NULL), Styles(NULL)
	{
		VirtualStreamReader tableReader( tableStream, fib->m_FibWord97.fcStshf, fib->m_nWordVersion);

		short cbStshi = fib->m_FibWord97.lcbStshf;
		//unsigned char* test = tableReader.ReadBytes( cbStshi, true );

		//create list of STDs
		Styles = new std::vector<StyleSheetDescription*>();

		if (fib->m_nWordVersion < 2)
		{
			//read size of the STSHI
			int stshiLengthBytesSize = 2;
			unsigned char* stshiLengthBytes = tableReader.ReadBytes( stshiLengthBytesSize, true );
			short cbStshi = FormatUtils::BytesToInt16( stshiLengthBytes, 0, stshiLengthBytesSize );
			RELEASEARRAYOBJECTS( stshiLengthBytes );
		
			//read the bytes of the STSHI        
			tableReader.Seek( ( fib->m_FibWord97.fcStshf + 2 ), 0/*STREAM_SEEK_SET*/ );
			unsigned char* stshi = tableReader.ReadBytes( cbStshi, true );

			//parses STSHI
			this->stshi = new StyleSheetInformation( stshi, cbStshi );
			RELEASEARRAYOBJECTS( stshi );

			for ( int i = 0; i < this->stshi->cstd; i++ )
			{
				//get the cbStd
				unsigned short cbStd = tableReader.ReadUInt16();

				if ( cbStd != 0 )
				{
					//read the STD bytes
					unsigned char* std = tableReader.ReadBytes( cbStd, true );

					//parse the STD bytes
					Styles->push_back( new StyleSheetDescription( std, cbStd, (int)this->stshi->cbSTDBaseInFile, dataStream, fib->m_nWordVersion) );

					RELEASEARRAYOBJECTS( std );
				}
				else
				{
					Styles->push_back( NULL );
				}
			}
		}
		else
		{
			unsigned short i, sz = 0;
			tableReader.ReadUInt16();
			
			unsigned short sz_names = tableReader.ReadUInt16();
			sz = 2;
			while(sz < sz_names)
			{
				unsigned short sz_name = tableReader.ReadByte(); sz++;
		
				StyleSheetDescription* std = new StyleSheetDescription();

				if (sz_name != 0 && sz_name < 0xde)
				{
					//user style
					unsigned char *bytes = tableReader.ReadBytes( sz_name, true );
					FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &std->xstzName, bytes, sz_name, ENCODING_WINDOWS_1250 );
					RELEASEARRAYOBJECTS( bytes );
				}
				// ms style
				else if (sz_name == 0)
				{
					//defpr
				}
				else
				{
					int ind = 255 - sz_name;
					std::wstring names [] = {L"Normal", L"Heading1", L"Heading2", L"Heading3", L"Heading4", L"Heading5", L"Heading6", L"Heading7", 
						L"Heading8", L"Heading9", L"FootnoteText", L"FootnoteReference", L"Header", L"Footer", L"IndexHeading", L"LineNumber"};
					_StyleIdentifier sti[] = {Normal, Heading1, Heading2, Heading3, Heading4, Heading5, Heading6, Heading7, Heading8, Heading9,
					FootnoteText, FootnoteReference, Header, Footer, IndexHeading, LineNumber};
					
					std->sti = sti[ind];
					std->xstzName = names[ind];
				}
//	0	Normal	standard PAP(standard PAP has all fields cleared to 0), standard CHP ( chp.hps = 20, all other fields set to 0).
//
//	255	Normal indent 	pap.dxaLeft = 720.
///* Heading levels */
//	254	heading 1	pap.dyaBefore = 240 (12 points), chp.fBold = negation of Normal style's chp.fBold, chp.kul = 1 (single underline), chp.hps = 24, chp.ftc = 2 .
//	253	heading 2	pap.dyaBefore = 120 (6 points), chp.fBold = negation of Normal style's chp.fBold, chp.hps = 24,  chp.ftc = 2
//	252	heading 3	pap.dxaLeft = 360, chp.fBold = negation of Normal style's chp.fBold, chp.hps = 24; 
//	251	heading 4	pap.dxaLeft = 360, chp.kul = 1 (single underline), chp.hps = 24; 
//	250	heading 5	pap.dxaLeft = 720, chp.fBold = negation of Normal style's chp.fBold, chp.hps = 20; 
//	249	heading 6	pap.dxaLeft = 720, chp.kul = 1 (single underline), chp.hps = 20; 
//	248	heading 7	pap.dxaLeft = 720, chp.fItalic = negation of Normal style's chp.fItalic, chp.hps = 20; 
//	247	heading 8	pap.dxaLeft = 720, chp.fItalic = negation of Normal style's chp.fItalic, chp.hps = 20; 
//	246	heading 9	pap.dxaLeft = 720, chp.fItalic = negation of Normal style's chp.fItalic, chp.hps = 20; 
//	245	footnote text	chp.hps = 20
//	244	footnote reference	chp.hps = 16; hpsPos = 6
//	243	header	When running a U.S. system file:
//				pap.itbdMac = 2, pap.rgdxaTab[0] = 3 * 1440, pap.rgtbd[0].jc = 1, pap.rgtbd[0].tlc = 0, pap.rgdxaTab[1] = 6* 1440, pap.rgtbd[1].jc = 1, pap.rgtbd[1].tlc = 0; 
//			When running an International metric system:
//				pap.itbdMac = 2, pap.rgdxaTab[0] =3969, pap.rgtbd[0].jc = 1, pap.rgtbd[0].tlc = 0, pap.rgdxaTab[1] = 8504, pap.rgtbd[1].jc = 1, pap.rgtbd[1].tlc = 0;
//	242	footer	When running a U.S. system file:
//				pap.itbdMac = 2, pap.rgdxaTab[0] = 3 * 1440, pap.rgtbd[0].jc = 1, pap.rgtbd[0].tlc = 0, pap.rgdxaTab[1] = 6* 1440, pap.rgtbd[1].jc = 1, pap.rgtbd[1].tlc = 0; 
//			When running an International metric system:
//				pap.itbdMac = 2, pap.rgdxaTab[0] =3969, pap.rgtbd[0].jc = 1, pap.rgtbd[0].tlc = 0, pap.rgdxaTab[1] = 8504, pap.rgtbd[1].jc = 1, pap.rgtbd[1].tlc = 0;
//	241	index heading 	same as properties for Normal style (stc == 0)
//	240	line number	same as properties for Normal style (stc == 0)

				Styles->push_back(std);

				sz += sz_name < 0xde ? sz_name : 0;			
			}
			unsigned short sz_chpxs = tableReader.ReadUInt16();
			i = 0;
			sz = 2;
			while(sz < sz_chpxs)
			{
				unsigned short sz_chpx = tableReader.ReadByte();
				sz++;

				if (Styles->size() <= i)
				{
					StyleSheetDescription* std = new StyleSheetDescription();
					Styles->push_back(std);
				}
				if (sz_chpx != 0 && sz_chpx != 255)
				{				
					unsigned char *bytes = tableReader.ReadBytes( sz_chpx, true );
					Styles->at(i)->chpx = new CharacterPropertyExceptions( bytes, sz_chpx , fib->m_nWordVersion); 
					RELEASEARRAYOBJECTS( bytes );
					sz += sz_chpx;			
				}
				i++;
			}			
 			unsigned short sz_papxs = tableReader.ReadUInt16();
			i = 0;
			sz = 2;
			while(sz < sz_papxs)
			{
				unsigned short sz_papx = tableReader.ReadByte();
				sz++;

				if ( sz_papx != 255)
				{				
					unsigned char *bytes = tableReader.ReadBytes( sz_papx, true );
					Styles->at(i)->papx = new ParagraphPropertyExceptions( bytes, sz_papx, tableStream, fib->m_nWordVersion );
					RELEASEARRAYOBJECTS( bytes );
				
					sz += sz_papx;			
				}
				i++;
			}			
 			unsigned short sz_Estcps = tableReader.ReadUInt16();
			i = 0;
			sz = 2;
			while(sz < sz_Estcps)
			{
				Styles->at(i++)->istdBase = tableReader.ReadByte();
				sz += 1;	
			}
		}
	}
}
