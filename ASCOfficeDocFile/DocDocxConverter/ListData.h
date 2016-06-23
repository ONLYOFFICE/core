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
#pragma once

#include "ListLevel.h"

namespace DocFileFormat
{
  class ListData
  {
    friend class ListTable;
	friend class NumberingMapping;

    private:
	  /// Unique List ID
      int lsid;
      /// Unique template code
      int tplc;
      /// Array of shorts containing the istd‘s linked to each level of the list, 
      /// or ISTD_NIL (4095) if no style is linked.
      vector<short> rgistd;
      /// True if this is a simple (one-level) list.
      /// False if this is a multilevel (nine-level) list.
      bool fSimpleList;
      /// Word 6.0 compatibility option:
      /// True if the list should start numbering over at the beginning of each section.
      bool fRestartHdn;
      /// To emulate Word 6.0 numbering:
      /// True if Auto numbering
      bool fAutoNum;
      /// When true, this list was there before we started reading RTF.
      bool fPreRTF;
      /// When true, list is a hybrid multilevel/simple (UI=simple, internal=multilevel)
      bool fHybrid;
      /// Array of ListLevel describing the several levels of the list.
      vector<ListLevel*>* rglvl;
	  /// A grfhic that specifies HTML incompatibilities of the list.
      unsigned char grfhic;
	  unsigned char* _rawBytes;

    public:
	  static const int LSTF_LENGTH = 28;
      static const short ISTD_NIL = 4095;
	  static const int VARIABLE_LENGTH = INT_MAX;

      virtual ~ListData();
	  /// Parses the StreamReader to retrieve a ListData
	  ListData( VirtualStreamReader* reader, int length );
  };
}