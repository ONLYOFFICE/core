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
