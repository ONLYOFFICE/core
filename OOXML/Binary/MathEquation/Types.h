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

#include <map>
#include "../../../DesktopEditor/common/Types.h"
#include "../../Base/Base.h"

namespace MathEquation
{
	struct TMathFont
	{
		std::string sName;
		bool        bItalic;
		bool        bBold;
	};

    typedef std::map<unsigned char, TMathFont> TFontMap;

	enum MBRACKETSTYPE
	{
		bracketsAngle            = 0x00, // < > Angle brackets
		bracketsRound            = 0x01, // ( ) Round brackets
		bracketsCurve            = 0x02, // { } Curly brackets
		bracketsSquare           = 0x03, // [ ] Square brackets
		bracketsLine             = 0x04, // | | Line brackets
		bracketsDLine            = 0x05, // || || Double line brackets
		bracketsFloor            = 0x06, // Floor brackets (angle at bottom)
		bracketsCeil             = 0x07, // Ceiling brackets (angle at top)
		bracketsSquareOpenOpen   = 0x08, // [ [
		bracketsSquareCloseClose = 0x09, // ] ]
		bracketsSquareCloseOpen  = 0x0a, // ] [
		bracketsSquareRound      = 0x0b, // [ )
		bracketsRoundSquare      = 0x0c  // ( ]

	};

	enum MFRACTIONTYPES
	{
		fractionRegular = 0, // Regular fraction
		fractionSlanted = 1  // Slanted fraction
	};

	enum MSCRIPTALIGN
	{
		scriptalignRight  = 0,
		scriptalignCenter = 1,
		scriptalignLeft   = 2
	};

	enum MBARTYPE
	{
		bartypeLine        = 0, // simple line
		bartypeDoubleLine  = 1, // double line
		bartypeArrowLeft   = 2, // left arrow
		bartypeArrowRight  = 3, // right arrow
		bartypeArrowDouble = 4  // double arrow (both left and right)
	};

	enum MARROWTYPE
	{
		arrowtypeLeft   = 0, // left arrow
		arrowtypeRight  = 1, // right arrow
		arrowtypeDouble = 2  // double arrow (both left and right)
	};

	enum MINTEGRALTYPE
	{
		integraltypeSingle             = 0, // Regular single integral without indices
		integraltypeSingleRSub         = 1, // Single integral with right subscript
		integraltypeSingleRSubSup      = 2, // Single integral with right subscript and superscript
		integraltypeSingleOriented     = 3, // Single oriented integral
		integraltypeSingleOrientedRSub = 4, // Single oriented integral with right subscript

		integraltypeDouble             = 5,  // Double integral without indices
		integraltypeDoubleRSub         = 6,  // Double integral with right subscript
		integraltypeDoubleOriented     = 7,  // Double oriented integral
		integraltypeDoubleOrientedRSub = 8,  // Double oriented integral with right subscript

		integraltypeTriple             = 9,  // Triple integral without indices
		integraltypeTripleRSub         = 10, // Triple integral with right subscript
		integraltypeTripleOriented     = 11, // Triple oriented integral
		integraltypeTripleOrientedRSub = 12, // Triple oriented integral with right subscript

		integraltypeSingleCSubSup      = 13, // Single integral with center subscript and superscript
		integraltypeSingleCSub         = 14, // Single integral with center subscript
		integraltypeSingleOrientedCSub = 15, // Single oriented integral with center subscript

		integraltypeDoubleOrientedCSub = 16, // Double oriented integral with center subscript
		integraltypeDoubleCSub         = 17, // Double integral with center subscript

		integraltypeTripleOrientedCSub = 18, // Triple oriented integral with center subscript
		integraltypeTripleCSub         = 19  // Triple integral with center subscript
	};

	enum MNARRAYTYPE
	{
		narySumCSub          = 0,  // Summation sign with center subscript
		narySumCSubSup       = 1,  // Summation sign with center subscript and superscript
		narySum              = 2,  // Summation sign without indices

		narySumRSub          = 3,  // Summation sign with right subscript
		narySumRSubSup       = 4,  // Summation sign with right subscript and superscript

		naryProdCSub         = 5,  // Product sign with center subscript
		naryProdCSubSup      = 6,  // Product sign with center subscript and superscript
		naryProd             = 7,  // Product sign without indices

		naryProdRSub         = 8,  // Product sign with right subscript
		naryProdRSubSup      = 9,  // Product sign with right subscript and superscript

		naryCoProdCSub       = 10, // Coproduct sign with center subscript
		naryCoProdCSubSup    = 11, // Coproduct sign with center subscript and superscript
		naryCoProd           = 12, // Coproduct sign without indices

		naryCoProdRSub       = 13, // Coproduct sign with right subscript
		naryCoProdRSubSup    = 14, // Coproduct sign with right subscript and superscript

		naryUnionCSub        = 15, // Union sign with center subscript
		naryUnionCSubSup     = 16, // Union sign with center subscript and superscript
		naryUnion            = 17, // Union sign without indices

		naryUnionRSub        = 18, // Union sign with right subscript
		naryUnionRSubSup     = 19, // Union sign with right subscript and superscript

		naryIntersectCSub    = 20, // Intersection sign with center subscript
		naryIntersectCSubSup = 21, // Intersection sign with center subscript and superscript
		naryIntersect        = 22, // Intersection sign without indices

		naryIntersectRSub    = 23, // Intersection sign with right subscript
		naryIntersectRSubSup = 24  // Intersection sign with right subscript and superscript
	};

	enum MLONGDIVISION
	{
		longdivisionWithResult = 0, // Long division sign without quotient
		longdivisionRegular    = 1  // Long division sign with quotient
	};

	enum MANGLEBRACKETSWITHSEPARATORTYPE
	{
		angbrwithsepBoth  = 0, // < | >
		angbrwithsepLeft  = 1, // < |
		angbrwithsepRight = 2  // | >		
	};

	enum MEMBELTYPE
	{
		embelDot         = 0x02, // dot            .
		embelDDot        = 0x03, // two dots       ..  
		embelDDDot       = 0x04, // three dots     ...
		embelPrime       = 0x05, // prime          '
		embelDPrime      = 0x06, // double prime   "
		embelLPrime      = 0x07, // left prime     '
		embelTilde       = 0x08, // tilde          ~
		embelHat         = 0x09, // hat (caret)    ^
		embelSlash       = 0x0a, // slash          /
		embelRArrow      = 0x0b, // right arrow    ->
		embelLArrow      = 0x0c, // left arrow     <-
		embelDArrow      = 0x0d, // double arrow   <->
		embelRHarpoon    = 0x0e, // right harpoon
		embelLHarpoon    = 0x0f, // left harpoon
		embelStrikeout   = 0x10, // strikeout
		embelBar         = 0x11, // bar
		embelTPrime      = 0x12, // triple prime   '''
		embelFrown       = 0x13, // arc up (frown)
		embelSmile       = 0x14, // arc down (smile)

		embelMin         = 0x02,
		embelMax         = 0x14
	};

	enum MMATRIXBORDERTYPE
	{
		matrixborderNone  = 0x00,
		matrixborderSolid = 0x01,
		matrixborderDash  = 0x02,
		matrixborderDot   = 0x03
	};

	enum MMATRIXHORALIGN
	{
		matrixhoralignLeft      = 0x00, // left alignment
		matrixhoralignCenter    = 0x01, // center alignment
		matrixhoralignRight     = 0x02, // right alignment
		matrixhoralignEqualSign = 0x03, // alignment by = sign
		matrixhoralignCommaSign = 0x04  // alignment by , sign
	};

	enum MMATRIXVERALIGN
	{
		matrixveralignTop      = 0x00,
		matrixveralignBaseLine = 0x01,
		matrixveralginBottom   = 0x02
	};

	enum MSPECIALSYMBOLS
	{
		specialsymAlignment  = 0xeb00,
		specialsymZeroSpace  = 0xeb01,
		specialsymThinSpace  = 0xeb02,
		specialsymThickSpace = 0xeb04,
		specialsymLargeSpace = 0xeb05,
		specialsymOnePtSpace = 0xeb08
	};
}
