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
#pragma once

#include "VirtualStreamReader.h"
#include "ByteStructure.h"

namespace DocFileFormat
{
  typedef struct FontSignature
  {
    unsigned int UnicodeSubsetBitfield0;
    unsigned int UnicodeSubsetBitfield1;
    unsigned int UnicodeSubsetBitfield2;
    unsigned int UnicodeSubsetBitfield3;
    unsigned int CodePageBitfield0;
    unsigned int CodePageBitfield1;
  } FontSignature;

  class FontFamilyName: public ByteStructure
  {
	friend class WordDocument;
	friend class CharacterPropertiesMapping;
	friend class DocumentMapping;
	friend class FontTableMapping;
	friend class StyleSheetMapping;
	friend class NumberingMapping;
  
    private:
      bool fTrueType;			// When true, font is a TrueType font
      unsigned char ff;			// Font family id
      short wWeight;			// Base weight of font
      unsigned char chs;		// Character set identifier
      unsigned char prq;		// Pitch request
      std::wstring xszFtn;		// Name of font
      std::wstring xszAlt;		// Alternative name of the font
      unsigned char *panose;	// Panose
	  unsigned int panoseSize;	// Panose size     
      FontSignature fs;			// Font sinature
	  
	  long searchTerminationZero( VirtualStreamReader *reader );
    public:
	  FontFamilyName();
	  virtual ~FontFamilyName();
      virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length );
	
    private:
  };
}
