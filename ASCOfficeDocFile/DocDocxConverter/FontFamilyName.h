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
      /// When true, font is a TrueType font
      bool fTrueType;
      /// Font family id
      unsigned char ff;
      /// Base weight of font
      short wWeight;
      /// Character set identifier
      unsigned char chs;
      /// Pitch request
      unsigned char prq;
      /// Name of font
      std::wstring xszFtn;
      /// Alternative name of the font
      std::wstring xszAlt;
      /// Panose
      unsigned char *panose;
	  /// Panose size
	  unsigned int panoseSize;
      /// Font sinature
      FontSignature fs;
	  
    public:
	  FontFamilyName();
	  virtual ~FontFamilyName();
      virtual ByteStructure* ConstructObject( VirtualStreamReader* reader, int length );
	
    private:
	  long searchTerminationZero( VirtualStreamReader *reader );
  };
}
