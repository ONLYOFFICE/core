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

#include "ListLevel.h"

namespace DocFileFormat
{
	  ListLevel::~ListLevel()
	  {
	    RELEASEOBJECT( this->grpprlPapx );
        RELEASEOBJECT( this->grpprlChpx );

		RELEASEARRAYOBJECTS( this->_rawBytes );
	  }

	  /*========================================================================================================*/
      
      /// Parses the given StreamReader to retrieve a LVL struct
	  ListLevel::ListLevel( VirtualStreamReader* reader, int length ):
	  grpprlPapx(NULL), grpprlChpx(NULL), _rawBytes(NULL)
      {
	    long startPos = reader->GetPosition();

        //parse the fix part
        this->iStartAt = reader->ReadInt32();
        this->nfc = reader->ReadByte();
        int flag = reader->ReadByte();
        this->jc = (unsigned char)( flag & 0x03 );
		this->fLegal = FormatUtils::BitmaskToBool( flag, 0x04 );
        this->fNoRestart = FormatUtils::BitmaskToBool( flag, 0x08 );
        this->fPrev = FormatUtils::BitmaskToBool( flag, 0x10 );
        this->fPrevSpace = FormatUtils::BitmaskToBool( flag, 0x20 );
        this->fWord6 = FormatUtils::BitmaskToBool( flag, 0x40 );
        
		for ( int i = 0; i < 9; i++ )
        {
		  rgbxchNums.push_back( reader->ReadByte() );
        }

        this->ixchFollow = (FollowingChar)reader->ReadByte();

        this->dxaSpace = reader->ReadInt32();
        this->dxaIndent = reader->ReadInt32();

        this->cbGrpprlChpx = reader->ReadByte();
        this->cbGrpprlPapx = reader->ReadByte();

        this->ilvlRestartLim = reader->ReadByte();
        this->grfhic = reader->ReadByte();
            
        //parse the variable part

        //read the group of papx sprms
        //this papx has no istd, so use PX to parse it
		unsigned char *bytes = reader->ReadBytes( this->cbGrpprlPapx, true );
        PropertyExceptions* px = new PropertyExceptions( bytes, this->cbGrpprlPapx );
		this->grpprlPapx = new ParagraphPropertyExceptions( *(px->grpprl) );
		RELEASEOBJECT( px );
        RELEASEARRAYOBJECTS( bytes );

        //read the group of chpx sprms
		bytes = reader->ReadBytes( this->cbGrpprlChpx, true );
        this->grpprlChpx = new CharacterPropertyExceptions( bytes, this->cbGrpprlChpx );
		RELEASEARRAYOBJECTS( bytes );

        //read the number text
		short strLen = reader->ReadInt16();
		if (strLen > 0)//file(14).doc
		{
			bytes = reader->ReadBytes( ( strLen * 2 ), true );
			FormatUtils::GetSTLCollectionFromBytes<wstring>( &(this->xst), bytes, ( strLen * 2 ), ENCODING_UNICODE );
			RELEASEARRAYOBJECTS( bytes );
		}

		long endPos = reader->GetPosition();
        reader->Seek( startPos, 0/*STREAM_SEEK_SET*/ );
        _rawBytes = reader->ReadBytes( (int)( endPos - startPos ), true );
      }
}
