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

#include "FontFamilyName.h"

namespace DocFileFormat
{
	FontFamilyName::~FontFamilyName()
	{
	  RELEASEARRAYOBJECTS( this->panose );

	  this->xszFtn.clear();
      this->xszAlt.clear();
	}

	/*========================================================================================================*/

	FontFamilyName::FontFamilyName():
	fTrueType(false), ff(0), wWeight(0), chs(0), prq(0), panose(NULL), panoseSize(0)
	{
	  this->fs.CodePageBitfield0 = 0;
	  this->fs.CodePageBitfield1 = 0;
	  this->fs.UnicodeSubsetBitfield0 = 0;
	  this->fs.UnicodeSubsetBitfield1 = 0;
	  this->fs.UnicodeSubsetBitfield2 = 0;
	  this->fs.UnicodeSubsetBitfield3 = 0;
	}

	/*========================================================================================================*/

	ByteStructure* FontFamilyName::ConstructObject( VirtualStreamReader* reader, int length )
	{
		FontFamilyName *newObject = new FontFamilyName();

		long startPos = reader->GetPosition();

		//FFID
		int ffid = (int)reader->ReadByte();

		newObject->prq			= (unsigned char)	FormatUtils::BitmaskToInt	( ffid, 0x03 );
		newObject->fTrueType	=					FormatUtils::BitmaskToBool	( ffid, 0x04 );
		newObject->ff			= (unsigned char)	FormatUtils::BitmaskToInt	( ffid, 0x70 );

		//int sz_fonts = 150; //..  need to generate unique todooo
		int szAlt = 0;
		if (reader->nWordVersion == 2)
		{
			newObject->wWeight = reader->ReadByte();
		}
		else
		{
			newObject->wWeight		= reader->ReadInt16();
			newObject->chs			= reader->ReadByte();
		
			szAlt = reader->ReadByte();
		}

		if (reader->nWordVersion == 0)
		{
			//read the 10 bytes panose
			newObject->panoseSize = 10;
			newObject->panose = reader->ReadBytes( newObject->panoseSize, true );

			//read the 24 bytes FontSignature
			newObject->fs.UnicodeSubsetBitfield0	= reader->ReadUInt32();
			newObject->fs.UnicodeSubsetBitfield1	= reader->ReadUInt32();
			newObject->fs.UnicodeSubsetBitfield2	= reader->ReadUInt32();
			newObject->fs.UnicodeSubsetBitfield3	= reader->ReadUInt32();
			newObject->fs.CodePageBitfield0			= reader->ReadUInt32();
			newObject->fs.CodePageBitfield1			= reader->ReadUInt32();
		}
	//read the next \0 terminated string
		long strStart	= reader->GetPosition();
		long strEnd		= searchTerminationZero( reader );	


		unsigned char *bytes = reader->ReadBytes( (int)( strEnd - strStart ), true );

		if (reader->nWordVersion > 0)
		{
			FormatUtils::GetWStringFromBytes(newObject->xszFtn, bytes, (int)( strEnd - strStart ), ENCODING_WINDOWS_1250 );
		}
		else
		{
			newObject->xszFtn = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)(bytes), (strEnd - strStart) / 2);
		}

		if (newObject->xszFtn.length() > 0)
		{
			  if ((int) newObject->xszFtn.at(0) < 31) //DDToneWebService.doc
			  {
                  newObject->xszFtn = L"";
			  }
		}

		if (newObject->xszFtn.length() < 2 && szAlt < 1)//programo.doc 
		{
            newObject->xszFtn = L"font";
		}

		RELEASEARRAYOBJECTS( bytes );

		long readBytes = reader->GetPosition() - startPos;

		if( readBytes < length && szAlt > 0)
		{
	//read the next \0 terminated string		
			strStart	= reader->GetPosition();
			strEnd		= searchTerminationZero( reader );

			bytes = reader->ReadBytes( (int)( strEnd - strStart ), true );

			if (reader->nWordVersion > 0)
			{
				FormatUtils::GetWStringFromBytes( newObject->xszAlt, bytes, (int)( strEnd - strStart ), ENCODING_WINDOWS_1250);
			}
			else
			{
				newObject->xszAlt = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)(bytes), (strEnd - strStart) / 2);
			}
			RELEASEARRAYOBJECTS( bytes );
		 }

		return static_cast<ByteStructure*>( newObject );
	}

	/*========================================================================================================*/

	long FontFamilyName::searchTerminationZero( VirtualStreamReader *reader )
    {
	  long strStart = reader->GetPosition();
      
	  if (reader->nWordVersion > 0)
	  {//ansi string only
		  while ( reader->ReadByte() != 0 )
		  {
			;
		  }
	  }
	  else
	  {//unicode string
		  while ( reader->ReadInt16() != 0 )
		  {
			;
		  }
	  }
      
	  long pos = reader->GetPosition();
	  
      reader->Seek( (int)strStart, 0 /*STREAM_SEEK_SET*/ );
      
	  return pos;
    }
}
