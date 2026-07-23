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

#include "ListData.h"

#include <boost/make_shared.hpp>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace DocFileFormat
{
	ListData::~ListData()
	{
		for_each(rglvl->begin(), rglvl->end(), DeleteDynamicObject());

		RELEASEOBJECT(rglvl);
	}

	ListDataPtr ListData::create(VirtualStreamReader* reader, int length)
	{
		return boost::make_shared<ListData>(reader, length);
	}

	// Parses the StreamReader to retrieve a ListData

	ListData::ListData(VirtualStreamReader* reader, int length) : rglvl(NULL)
	{
		long startPos = reader->GetPosition();

		lsid = reader->ReadInt32();
		tplc = reader->ReadInt32();

		for ( int i = 0; i < 9; i++ )
		{
			rgistd.push_back( reader->ReadInt16() );
		}

		//parse flagbyte
		int flag = (int)reader->ReadByte();
		fSimpleList = FormatUtils::BitmaskToBool( flag, 0x01 );

		if ( fSimpleList )
		{
			rglvl = new std::vector<ListLevel*>( 1 );
		}
		else
		{
			rglvl = new std::vector<ListLevel*>( 9 );
		}

		fRestartHdn = FormatUtils::BitmaskToBool( flag, 0x02 );
		fAutoNum	= FormatUtils::BitmaskToBool( flag, 0x04 );
		fPreRTF		= FormatUtils::BitmaskToBool( flag, 0x08 );
		fHybrid		= FormatUtils::BitmaskToBool( flag, 0x10 );

		grfhic		= reader->ReadByte();
	}
	NumberingDescriptorPtr NumberingDescriptor::create(unsigned char * data, int length)
	{
		return boost::make_shared<NumberingDescriptor>(data, length);
	}
	bool NumberingDescriptor::operator == (const NumberingDescriptor & val) const
	{
		const bool res =     
		nfc				== val.nfc &&
		cbTextBefore	== val.cbTextBefore &&
		cbTextAfter		== val.cbTextAfter &&
		jc				== val.jc &&
		fPrev			== val.fPrev &&
		fHang			== val.fHang &&
		fSetBold		== val.fSetBold &&
		fSetItalic		== val.fSetItalic &&
		fSetSmallCaps	== val.fSetSmallCaps &&
		fSetCaps		== val.fSetCaps &&
		fSetStrike		== val.fSetStrike &&
		fSetKul			== val.fSetKul &&
		fPrevSpace		== val.fPrevSpace &&
		fBold			== val.fBold &&
		fItalic			== val.fItalic &&
		fSmallCaps		== val.fSmallCaps &&
		fCaps			== val.fCaps &&
		fStrike			== val.fStrike &&
		kul				== val.kul &&
		ico				== val.ico &&
		ftc				== val.ftc &&
		hps				== val.hps &&
		iStartAt		== val.iStartAt &&
		dxaIndent		== val.dxaIndent &&
		dxaSpace		== val.dxaSpace &&
		fNumber1		== val.fNumber1 &&
		fNumberAcross	== val.fNumberAcross &&
		fRestartHdn		== val.fRestartHdn &&
		fSpareX			== val.fSpareX &&
		xst				== val.xst;

		return res;
	}
	bool NumberingDescriptor::operator == (const NumberingDescriptorPtr & val) const
	{
		if (!val) return false;
		const bool res =     
		nfc				== val->nfc &&
		cbTextBefore	== val->cbTextBefore &&
		cbTextAfter		== val->cbTextAfter &&
		jc				== val->jc &&
		fPrev			== val->fPrev &&
		fHang			== val->fHang &&
		fSetBold		== val->fSetBold &&
		fSetItalic		== val->fSetItalic &&
		fSetSmallCaps	== val->fSetSmallCaps &&
		fSetCaps		== val->fSetCaps &&
		fSetStrike		== val->fSetStrike &&
		fSetKul			== val->fSetKul &&
		fPrevSpace		== val->fPrevSpace &&
		fBold			== val->fBold &&
		fItalic			== val->fItalic &&
		fSmallCaps		== val->fSmallCaps &&
		fCaps			== val->fCaps &&
		fStrike			== val->fStrike &&
		kul				== val->kul &&
		ico				== val->ico &&
		ftc				== val->ftc &&
		hps				== val->hps &&
		iStartAt		== val->iStartAt &&
		dxaIndent		== val->dxaIndent &&
		dxaSpace		== val->dxaSpace &&
		fNumber1		== val->fNumber1 &&
		fNumberAcross	== val->fNumberAcross &&
		fRestartHdn		== val->fRestartHdn &&
		fSpareX			== val->fSpareX &&
		xst				== val->xst;

		return res;

	}
	NumberingDescriptor::NumberingDescriptor( unsigned char * data, int length )
	{
		id				= 0;
		nfc				= FormatUtils::BytesToUChar(data, 0, length);
		cbTextBefore	= FormatUtils::BytesToUChar(data, 1, length);
		cbTextAfter		= FormatUtils::BytesToUChar(data, 2, length);

		int flag		= FormatUtils::BytesToUChar(data, 3, length);

		jc				= (unsigned char)( flag & 0x03 );

		fPrev			= FormatUtils::BitmaskToBool( flag, 0x04 );
		fHang			= FormatUtils::BitmaskToBool( flag, 0x08 );

		fSetBold		= FormatUtils::BitmaskToBool( flag, 0x10 );
		fSetItalic		= FormatUtils::BitmaskToBool( flag, 0x20 );
		fSetSmallCaps	= FormatUtils::BitmaskToBool( flag, 0x40 );
		fSetCaps		= FormatUtils::BitmaskToBool( flag, 0x80 );

		flag			= FormatUtils::BytesToUChar(data, 4, length);;

		fSetStrike		= FormatUtils::BitmaskToBool( flag, 0x01 );
		fSetKul			= FormatUtils::BitmaskToBool( flag, 0x02 );

		fPrevSpace		= FormatUtils::BitmaskToBool( flag, 0x04 );
		fBold			= FormatUtils::BitmaskToBool( flag, 0x08 );
		fItalic			= FormatUtils::BitmaskToBool( flag, 0x10 );
		fSmallCaps		= FormatUtils::BitmaskToBool( flag, 0x20 );
		fCaps			= FormatUtils::BitmaskToBool( flag, 0x40 );
		fStrike			= FormatUtils::BitmaskToBool( flag, 0x80 );

		flag			= FormatUtils::BytesToUChar(data, 5, length);

		kul				= FormatUtils::BitmaskToBool( flag, 0x07 );//3 bit
		ico				= FormatUtils::BitmaskToBool( flag, 0xf1 );//5 bit

		ftc				= FormatUtils::BytesToInt16 (data, 6, length);
		
		ftc = ftc & 0x0fff; // 0x8001 ???? file(31).doc

		hps				= FormatUtils::BytesToUInt16(data, 8, length);

		iStartAt		= FormatUtils::BytesToUInt16(data, 10, length);
		dxaIndent		= FormatUtils::BytesToUInt16(data, 12, length);
		dxaSpace		= FormatUtils::BytesToUInt16(data, 14, length);

		fNumber1		= FormatUtils::BytesToUChar(data, 16, length);
		fNumberAcross	= FormatUtils::BytesToUChar(data, 17, length);
		fRestartHdn		= FormatUtils::BytesToUChar(data, 18, length);
		fSpareX			= FormatUtils::BytesToUChar(data, 19, length);
	
	// fixed size = 20
	//read the number text
		short strLen = length - 20;
		
		while (strLen > 0)
		{
			if (data[strLen + 20 - 1] != 0)
				break;
			strLen--;
		}
		if (strLen > 0)
		{
			FormatUtils::GetWStringFromBytes( xst, data + 20, ( strLen ), ENCODING_WINDOWS_1250);
		}	
	}
	OutlineListDescriptor::~OutlineListDescriptor()
	{

	}
	OutlineListDescriptor::OutlineListDescriptor( unsigned char * data, int length )
	{
		if (length < 212)
		{
			//int sz  =  FormatUtils::BytesToUChar(data, 0, length);
			int pos = 0;

			for (int i = 0 ; i < 9; i++)
			{
				lvl[i].Parse(data + pos, 1);
				pos += 1;
			}
		}
		else
		{
			int pos = 0;

			for (int i = 0 ; i < 9; i++)
			{
				lvl[i].Parse(data + pos, length - pos);
				pos += 16;
			}

			fRestartHdr	= FormatUtils::BytesToUChar(data, pos, length); pos += 2;
			fSpareOlst2	= FormatUtils::BytesToUChar(data, pos, length); pos += 2;
			fSpareOlst3	= FormatUtils::BytesToUChar(data, pos, length); pos += 2;
			fSpareOlst4	= FormatUtils::BytesToUChar(data, pos, length); pos += 2;
			
			short strLen = length - pos;
			
			while (strLen > 0)
			{
				if (data[strLen + 20 - 1] != 0)
					break;
				strLen--;
			}
			if (strLen > 0)
			{
				FormatUtils::GetWStringFromBytes( xst, data + 20, ( strLen ), ENCODING_WINDOWS_1250);
			}	
		}
	}

	ByteStructure* OutlineListDescriptor::ConstructObject(VirtualStreamReader* reader, int length)
	{
		int sz = GetSize(reader->nWordVersion);

		unsigned char *data = reader->ReadBytes(sz, true);
		OutlineListDescriptor *newObject = new OutlineListDescriptor(data, sz);

		delete []data;

        return static_cast<ByteStructure*>(newObject);
	}

}
