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

#include "VirtualStreamReader.h"
#include "WideString.h"
#include "IVisitable.h"

#include "../Common/Base/FormatUtils.h"

namespace DocFileFormat
{
	template<class T> class StringTableEx;

	template<class T> class StringTable: public IVisitable
	{
	public:
		bool			fExtend = false;
		int				cbData = 0;
		unsigned short	cbExtra = 0;
		int				code_page = 1250;
		
		std::vector<ByteStructure*>	Data;
		std::vector<unsigned char*>	DataExtra;
		
		StringTable()
		{
		}		
		virtual ~StringTable()
		{
			for ( size_t i = 0; i < Data.size(); ++i )
			{
				RELEASEOBJECT( Data[i] );

				if (false == DataExtra.empty())
				{
					if (DataExtra[i])
					{
						delete []DataExtra[i];
						DataExtra[i] = NULL;
					}	
				}
			}
		}
		StringTable( VirtualStreamReader *reader, int code_page_ ) 
		{
			code_page = code_page_;
            parse( reader, (unsigned int)reader->GetPosition(), 0, false );
		}
		StringTable( POLE::Stream* tableStream, unsigned int fc, unsigned int lcb, int nWordVersion, bool bReadExta = false) 
		{
			if ( lcb > 0 )
			{
				VirtualStreamReader reader( tableStream, fc, nWordVersion);

				parse( &reader, fc, lcb, bReadExta ) ;
			}
		}
		std::vector<unsigned char*> & getDataExtra() {return DataExtra;}
		unsigned short getDataExtraSize() {return cbExtra;}

		ByteStructure* operator [] ( size_t index ) const
		{
			if ( index < Data.size() )
			{
				return Data.at( index );
			}
			else
			{
				return NULL;
			}
		}

	private:
		
		virtual void parse( VirtualStreamReader *reader, unsigned int fc, unsigned int lcb = 0, bool bReadExta = false )
		{
			if ( reader == NULL )		return;
			if (fc > reader->GetSize()) return;

			cbData = lcb;

			int count_elements = 0;

			if ( reader->ReadUInt16() == 0xFFFF )
			{
				//if the first 2 bytes are 0xFFFF the STTB contains unicode characters
				fExtend = true;
				code_page = ENCODING_UTF16;
			}
			else
			{
				//else the STTB contains 1byte characters and the fExtend field is non-existend
				//seek back to the beginning
				fExtend = false;
				code_page = ENCODING_WINDOWS_1250;

				reader->Seek( (int)fc, 0/*STREAM_SEEK_SET*/ );
			}

			long cDataStart = reader->GetPosition();
			unsigned short cb = 0, elem_sz = 0;
			
			if (reader->nWordVersion > 0)
			{
				cb = reader->ReadUInt16();
				cbData = cb; // all size 
			}
			else if (reader->nWordVersion == 0)
			{
				cb = reader->ReadUInt16();
				if ( cb != 0xFFFF )
				{
					count_elements = cb;				
				}
				else
				{
					reader->Seek( (int)( fc + cDataStart ), 0/*STREAM_SEEK_SET*/ );
					cbData = reader->ReadInt32();
				}
			}

			if (reader->nWordVersion == 0)
			{
				cbExtra = reader->ReadUInt16();
			}

			while (true)	
			{
				if (reader->GetPosition() >= fc + cbData + cbExtra && count_elements < 1)
					break; //1995 and older

				if (Data.size() == count_elements && count_elements > 0)
					break; //1997 and newer

				int cchData = 0;
				int cbData = 0;
        
				if ( fExtend )
				{
					cchData = (int)reader->ReadUInt16();
					cbData = cchData * 2;
				}
				else
				{
					cchData = (int)reader->ReadByte();
					cbData = cchData;
				}

				long posBeforeType = reader->GetPosition();

				Data.push_back( T().ConstructObject( reader, cbData ) );

				reader->Seek( (int)( posBeforeType + cbData ), 0/*STREAM_SEEK_SET */);
	        
				if (bReadExta)
				{
					unsigned char* pData = reader->ReadBytes( cbExtra, true );
					DataExtra.push_back(pData);
				}
				else
				{
					reader->ReadBytes( cbExtra, false );
				}
			}
		}
	};
	template<class T> class StringTableEx : public StringTable<T>
	{
	public:
		StringTableEx(VirtualStreamReader *reader, int code_page_)
		{
			StringTable<T>::code_page = code_page_;
			parse(reader, (unsigned int)reader->GetPosition(), 0, true);
		}

		StringTableEx(POLE::Stream* tableStream, unsigned int fc, unsigned int lcb, int nWordVersion)
		{
			if (lcb > 0)
			{
				VirtualStreamReader reader(tableStream, fc, nWordVersion);

				parse(&reader, fc, lcb, true);
			}
		}
	private:
		virtual void parse(VirtualStreamReader *reader, unsigned int fc, unsigned int lcb = 0, bool bReadExta = false)
		{
			if (reader == NULL)		return;
			if (fc > reader->GetSize()) return;

			if (reader->ReadUInt16() == 0xFFFF)
			{
				StringTable<T>::fExtend = true;
				StringTable<T>::code_page = ENCODING_UTF16;
			}
			else
			{
				StringTable<T>::fExtend = false;
				StringTable<T>::code_page = ENCODING_WINDOWS_1250;
			}

			_UINT32 cDataStart = reader->GetPosition(), count_elements = 0;
			_UINT32 cb = 0, elem_sz = 0;

			cb = reader->ReadUInt32();
			if (cb != 0xFFFF)
			{
				count_elements = cb;
				StringTable<T>::cbData = 0;
			}
			else
			{
				StringTable<T>::cbData = cb;
			}

			StringTable<T>::cbExtra = reader->ReadUInt16();

			while (true)
			{
				if (reader->GetPosition() >= fc + lcb)
					break; 

				int cchData = 0;

				if (StringTable<T>::fExtend)
				{
					cchData = (int)reader->ReadUInt16();
					if (cchData > 0) cchData -= 2; //???  cchData (2 bytes): This value MUST be 0
				}
				else
				{
					cchData = (int)reader->ReadByte(); //??
				}

				if (cchData > 0)
				{
					ByteStructure* object = T().ConstructObject(reader, cchData);
					StringTable<T>::Data.push_back(object);

					if (bReadExta)
					{
						unsigned char* pData = reader->ReadBytes(StringTable<T>::cbExtra, true);
						StringTable<T>::DataExtra.push_back(pData);
					}
					else
					{
						reader->ReadBytes(StringTable<T>::cbExtra, false);
					}
				}
				else
				{
					ByteStructure* object = T().ConstructObject(reader, StringTable<T>::cbExtra);
					StringTable<T>::Data.push_back(object);
				}
			}
			reader->Seek((int)(fc + lcb), 0/*STREAM_SEEK_SET */);
		}
	};
}
