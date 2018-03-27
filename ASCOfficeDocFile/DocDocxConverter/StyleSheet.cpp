/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

				if (sz_name != 0 && sz_name < 0xff)
				{
					unsigned char *bytes = tableReader.ReadBytes( sz_name, true );
					FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &std->xstzName, bytes, sz_name, ENCODING_WINDOWS_1250 );
					RELEASEARRAYOBJECTS( bytes );
				}
				else
				{
					if (Styles->empty())
					{
						std->sti = Normal;
						std->xstzName = L"Normal";
					}
				}

				Styles->push_back(std);

				sz += sz_name != 0xff ? sz_name : 0;			
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
