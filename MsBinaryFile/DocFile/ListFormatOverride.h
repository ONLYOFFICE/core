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
#include "ListFormatOverrideLevel.h"

namespace DocFileFormat
{
  class ListFormatOverride
  {
    friend class ListFormatOverrideTable;
	friend class NumberingMapping;
  
    private: 
	  static const int LFO_LENGTH = 16;
	  /// List ID of corresponding ListData
      int lsid;
      /// Count of levels whose format is overridden
      unsigned char clfolvl;
      /// Specifies the field this LFO represents. 
      /// MUST be a value from the following table:
      /// 0x00:   This LFO is not used for any field.
      /// 0xFC:   This LFO is used for the AUTONUMLGL field.
      /// 0xFD:   This LFO is used for the AUTONUMOUT field.
      /// 0xFE:   This LFO is used for the AUTONUM field.
      /// 0xFF:   This LFO is not used for any field.
      unsigned char ibstFltAutoNum;
      /// A grfhic that specifies HTML incompatibilities.
      unsigned char grfhic;
      /// Array of all levels whose format is overridden
      std::vector<ListFormatOverrideLevel*> rgLfoLvl;

    public:
	  /// Parses the given Stream Reader to retrieve a ListFormatOverride
	  ListFormatOverride( VirtualStreamReader* reader, int length );
	  virtual ~ListFormatOverride();
  };
}
