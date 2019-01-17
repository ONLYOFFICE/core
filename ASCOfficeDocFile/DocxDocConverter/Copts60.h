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

namespace ASCDocFileFormat
{
  union Copts60
  {
    private:
	  struct
	  {
	    unsigned short fNoTabForInd:1;
		unsigned short fNoSpaceRaiseLower:1;
		unsigned short fSuppressSpBfAfterPgBrk:1;
		unsigned short fWrapTrailSpaces:1;
		unsigned short fMapPrintTextColor:1;
		unsigned short fNoColumnBalance:1;
		unsigned short fConvMailMergeEsc:1;
		unsigned short fSuppressTopSpacing:1;
		unsigned short fOrigWordTableRules:1;
		unsigned short unused14:1;
		unsigned short fShowBreaksInFrames:1;
		unsigned short fSwapBordersFacingPgs:1;
		unsigned short fLeaveBackslashAlone:1;
		unsigned short fExpShRtn:1;
		unsigned short fDntULTrlSpc:1;
		unsigned short fDntBlnSbDbWid:1;
	  } Copts60Struct;
	  unsigned short Copts60Short;

    public:
	  explicit Copts60( unsigned short _copts60 = 0 ):
      Copts60Short(_copts60)
	  {
	  }

      Copts60( bool _fNoTabForInd, bool _fNoSpaceRaiseLower, bool _fSuppressSpBfAfterPgBrk, bool _fWrapTrailSpaces,
		       bool _fMapPrintTextColor, bool _fNoColumnBalance, bool _fConvMailMergeEsc, bool _fSuppressTopSpacing,
		       bool _fOrigWordTableRules, bool _fShowBreaksInFrames, bool _fSwapBordersFacingPgs, bool _fLeaveBackslashAlone,
			   bool _fExpShRtn, bool _fDntULTrlSpc, bool _fDntBlnSbDbWid ):
      Copts60Short(0)
	  {
	    this->Copts60Struct.fNoTabForInd = ( ( _fNoTabForInd ) ? (1) : (0) );
		this->Copts60Struct.fNoSpaceRaiseLower = ( ( _fNoSpaceRaiseLower ) ? (1) : (0) );
        this->Copts60Struct.fSuppressSpBfAfterPgBrk = ( ( _fSuppressSpBfAfterPgBrk ) ? (1) : (0) );
        this->Copts60Struct.fWrapTrailSpaces = ( ( _fWrapTrailSpaces ) ? (1) : (0) );
        this->Copts60Struct.fMapPrintTextColor = ( ( _fMapPrintTextColor ) ? (1) : (0) );
        this->Copts60Struct.fNoColumnBalance = ( ( _fNoColumnBalance ) ? (1) : (0) );
        this->Copts60Struct.fConvMailMergeEsc = ( ( _fConvMailMergeEsc ) ? (1) : (0) );
        this->Copts60Struct.fSuppressTopSpacing = ( ( _fSuppressTopSpacing ) ? (1) : (0) );
        this->Copts60Struct.fOrigWordTableRules = ( ( _fOrigWordTableRules ) ? (1) : (0) );
        this->Copts60Struct.unused14 = 0;
        this->Copts60Struct.fShowBreaksInFrames = ( ( _fShowBreaksInFrames ) ? (1) : (0) );
        this->Copts60Struct.fSwapBordersFacingPgs = ( ( _fSwapBordersFacingPgs ) ? (1) : (0) );
        this->Copts60Struct.fLeaveBackslashAlone = ( ( _fLeaveBackslashAlone ) ? (1) : (0) );
        this->Copts60Struct.fExpShRtn = ( ( _fExpShRtn ) ? (1) : (0) );
        this->Copts60Struct.fDntULTrlSpc = ( ( _fDntULTrlSpc ) ? (1) : (0) );
        this->Copts60Struct.fDntBlnSbDbWid = ( ( _fDntBlnSbDbWid ) ? (1) : (0) );
	  }

	  operator unsigned short () const
	  {
	    return this->Copts60Short;
	  }
  };
}