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

#include "ParagraphPropertyExceptions.h"
#include "CharacterPropertyExceptions.h"
#include "VirtualStreamReader.h"

namespace DocFileFormat
{
	typedef enum _FollowingChar
	{
		tab = 0,
		space,
		nothing
	} FollowingChar;

	static const wchar_t FollowingCharMap[3][8] =
	{
        L"tab" ,
        L"space" ,
        L"nothing"
	};

	class ListLevel
	{
		friend class NumberingMapping;

	private:  
		/// Start at value for this list level
		int iStartAt;
		/// Number format code (see anld.nfc for a list of options)
		unsigned char nfc;
		/// Alignment (left, right, or centered) of the paragraph number.
		unsigned char jc;
		/// True if the level turns all inherited numbers to arabic, 
		/// false if it preserves their number format code (nfc)
		bool fLegal;
		/// True if the level‘s number sequence is not restarted by 
		/// higher (more significant) levels in the list
		bool fNoRestart;
		/// Word 6.0 compatibility option: equivalent to anld.fPrev (see ANLD)
		bool fPrev;
		/// Word 6.0 compatibility option: equivalent to anld.fPrevSpace (see ANLD)
		bool fPrevSpace;
		/// True if this level was from a converted Word 6.0 document.
		/// If it is true, all of the Word 6.0 compatibility options become 
		/// valid otherwise they are ignored.
		bool fWord6;
		/// Contains the character offsets into the LVL’s XST of the inherited numbers of previous levels.
		/// The XST contains place holders for any paragraph numbers contained in the text of the number, 
		/// and the place holder contains the ilvl of the inherited number, 
		/// so lvl.xst[lvl.rgbxchNums[0]] == the level of the first inherited number in this level.
		std::vector<unsigned char> rgbxchNums;
		/// The type of character following the number text for the paragraph.
		FollowingChar ixchFollow;
		/// Word 6.0 compatibility option: equivalent to anld.dxaSpace (see ANLD).
		/// For newer versions indent to remove if we remove this numbering.
		int dxaSpace;
		/// Word 6.0 compatibility option: equivalent to anld.dxaIndent (see ANLD).
		/// Unused in newer versions.
		int dxaIndent;
		/// Length, in bytes, of the LVL‘s grpprlChpx.
		unsigned char cbGrpprlChpx;
		/// Length, in bytes, of the LVL‘s grpprlPapx.
		unsigned char cbGrpprlPapx;
		/// Limit of levels that we restart after.
		unsigned char ilvlRestartLim;
		/// A grfhic that specifies HTML incompatibilities of the level.
		unsigned char grfhic;
		ParagraphPropertyExceptions* grpprlPapx;
		CharacterPropertyExceptions* grpprlChpx;
		std::wstring xst;

	public:
		virtual ~ListLevel();
		// Parses the given StreamReader to retrieve a LVL struct
		ListLevel( VirtualStreamReader* reader, int length );
	};

	class NumberingLevelDescriptor
	{
		friend class NumberingMapping;
	private:
		unsigned char	nfc;
		unsigned char	cbTextBefore;
		unsigned char	cbTextAfter;
		unsigned char	jc;

		bool			fPrev;
		bool			fHang;

		bool			fSetBold;
		bool			fSetItalic;
		bool			fSetSmallCaps;
		bool			fSetCaps;
		bool			fSetStrike;
		bool			fSetKul;

		bool			fPrevSpace;
		bool			fBold;
		bool			fItalic;
		bool			fSmallCaps;
		bool			fCaps;
		bool			fStrike;

		unsigned char	kul;
		unsigned char	ico;
		short			ftc;
		unsigned short	hps;
		unsigned short	iStartAt;
		unsigned short	dxaIndent;
		unsigned short	dxaSpace;
	public:
        bool            bEnabled;
		virtual ~NumberingLevelDescriptor();
		// Parses the given StreamReader to retrieve a ANLV struct
        NumberingLevelDescriptor();
        NumberingLevelDescriptor( unsigned char * data, int length ); //cbANLV is 16 bytes
        void Parse( unsigned char * data, int length );
    };
}
