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
