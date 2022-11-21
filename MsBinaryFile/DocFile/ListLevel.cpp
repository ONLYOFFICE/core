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

#include "ListLevel.h"

namespace DocFileFormat
{
	ListLevel::~ListLevel()
	{
		RELEASEOBJECT( grpprlPapx );
		RELEASEOBJECT( grpprlChpx );
	}

	ListLevel::ListLevel( VirtualStreamReader* reader, int length ):
							grpprlPapx(NULL), grpprlChpx(NULL)
	{
		//parse the fix part
		iStartAt	= reader->ReadInt32();
		nfc			= reader->ReadByte();
		int flag	= reader->ReadByte();
		jc			= (unsigned char)( flag & 0x03 );

		fLegal		= FormatUtils::BitmaskToBool( flag, 0x04 );
		fNoRestart	= FormatUtils::BitmaskToBool( flag, 0x08 );
		fPrev		= FormatUtils::BitmaskToBool( flag, 0x10 );
		fPrevSpace	= FormatUtils::BitmaskToBool( flag, 0x20 );
		fWord6		= FormatUtils::BitmaskToBool( flag, 0x40 );

		for ( int i = 0; i < 9; i++ )
		{
		  rgbxchNums.push_back( reader->ReadByte() );
		}

		ixchFollow		= (FollowingChar)reader->ReadByte();

		dxaSpace		= reader->ReadInt32();
		dxaIndent		= reader->ReadInt32();

		cbGrpprlChpx	= reader->ReadByte();
		cbGrpprlPapx	= reader->ReadByte();

		ilvlRestartLim	= reader->ReadByte();
		grfhic = reader->ReadByte();
		    
		//parse the variable part

		//read the group of papx sprms
		//this papx has no istd, so use PX to parse it
		unsigned char *bytes	= reader->ReadBytes( cbGrpprlPapx, true );

		PropertyExceptions* px	= new PropertyExceptions( bytes, cbGrpprlPapx, reader->nWordVersion);
		grpprlPapx				= new ParagraphPropertyExceptions( *(px->grpprl) );

		RELEASEOBJECT( px );
		RELEASEARRAYOBJECTS( bytes );

		//read the group of chpx sprms
		bytes = reader->ReadBytes( cbGrpprlChpx, true );
		grpprlChpx = new CharacterPropertyExceptions( bytes, cbGrpprlChpx, reader->nWordVersion );
		RELEASEARRAYOBJECTS( bytes );

		//read the number text
		short strLen = reader->ReadInt16();
		if (strLen > 0)//file(14).doc
		{
			bytes = reader->ReadBytes( ( strLen * 2 ), true );
			FormatUtils::GetSTLCollectionFromBytes<std::wstring>( &(xst), bytes, ( strLen * 2 ), ENCODING_UTF16 );
			RELEASEARRAYOBJECTS( bytes );
		}
	}
    NumberingLevelDescriptor::NumberingLevelDescriptor()
    {
        //default set
        bEnabled = false;
    }

	NumberingLevelDescriptor::~NumberingLevelDescriptor()
	{
	}

    NumberingLevelDescriptor::NumberingLevelDescriptor( unsigned char * data, int length )
	{
        bEnabled = false;

        Parse(data, length);
	}

    void NumberingLevelDescriptor::Parse(unsigned char * data, int length )
    {
		if (length < 16)
		{
			return;
		}
        bEnabled        = true;

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
        hps				= FormatUtils::BytesToUInt16(data, 8, length);

        iStartAt		= FormatUtils::BytesToUInt16(data, 10, length);
        dxaIndent		= FormatUtils::BytesToUInt16(data, 12, length);
        dxaSpace		= FormatUtils::BytesToUInt16(data, 14, length);
    }
}
