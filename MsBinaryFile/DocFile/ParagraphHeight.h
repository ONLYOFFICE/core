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

namespace DocFileFormat
{
  class ParagraphHeight
  {
    private:
      /// Complex shape layout in this paragraph
      bool fVolatile;
      /// ParagraphHeight is valid when fUnk is true
      bool fUnk;
      /// When true, total height of paragraph is known but lines in 
      /// paragraph have different heights
      bool fDiffLines;
      /// When fDiffLines is 0, is number of lines in paragraph
      short clMac;
      /// Width of lines in paragraph
      int dxaCol;
      /// When fDiffLines is true, is height of every line in paragraph in pixels
      int dymLine;
      /// When fDiffLines is true, is the total height in pixels of the paragraph
      int dymHeight;
      /// If not == 0, used as a hint when finding the next row.
      /// (this value is only set if the PHE is stored in a PAP whose fTtp field is set)
      short dcpTtpNext;
      /// Height of table row.
      /// (this value is only set if the PHE is stored in a PAP whose fTtp field is set)
      int dymTableHeight;
      /// Reserved
      bool fSpare;
      /// Creates a new empty ParagraphHeight with default values

    public:
	  ParagraphHeight();
	  /// Parses the bytes to retrieve a ParagraphHeight
      /// The flag which indicates if the 
      /// ParagraphHeight is stored in a ParagraphProperties whose fTtp field is set
      ParagraphHeight( unsigned char* bytes, int size, bool fTtpMode );

    private:
      void setDefaultValues();
  };
}
