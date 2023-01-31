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

#include "FormattedDiskPage.h"
#include "ParagraphHeight.h"
#include "ParagraphPropertyExceptions.h"
#include "FileInformationBlock.h"

namespace DocFileFormat
{
  typedef struct BX
  {
    unsigned char wordOffset;
    ParagraphHeight phe;
  } BX;

  class FormattedDiskPagePAPX: public FormattedDiskPage
  {
    friend class MainDocumentMapping;
	friend class WordDocument;
	friend class HeaderMapping;
	friend class FooterMapping;
	friend class TextboxMapping;

    private:
      /// An array of the BX data structure.
      /// BX is a 13 unsigned char data structure. The first unsigned char of each is the word offset of the PAPX.
      BX* rgbx;
      /// grppapx consists of all of the PAPXs stored in FKP concatenated end to end. 
      /// Each PAPX begins with a count of words which records its length padded to a word boundary.
      unsigned int grppapxSize;
	  ParagraphPropertyExceptions** grppapx;

    public:
      virtual ~FormattedDiskPagePAPX();
	  FormattedDiskPagePAPX( POLE::Stream* wordStream, int offset, POLE::Stream* dataStream, int nWordVersion, bool fComplex);
      /// Parses the 0Table (or 1Table) for FKP _entries containing PAPX
      static std::list<FormattedDiskPagePAPX*>* GetAllPAPXFKPs( FileInformationBlock* fib, POLE::Stream* wordStream, POLE::Stream* tableStream, POLE::Stream* dataStream);
      /// Returns a list of all PAPX FCs between they given boundaries.
      static std::list<int>* GetFileCharacterPositions( int fcMin, int fcMax, FileInformationBlock* fib, POLE::Stream* wordStream, POLE::Stream* tableStream, POLE::Stream* dataStream );
      /// Returnes a list of all ParagraphPropertyExceptions which correspond to text 
      /// between the given offsets.
      static std::list<ParagraphPropertyExceptions*>* GetParagraphPropertyExceptions( int fcMin, int fcMax, FileInformationBlock* fib, POLE::Stream* wordStream, POLE::Stream* tableStream, POLE::Stream* dataStream );
  };
}

//#endif //FORMATTED_DISK_PAGE_PAPX_H