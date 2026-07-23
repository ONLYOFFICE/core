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

#include "Types.h"

namespace MathEquation
{
	struct TMathFont;
	class MathEquation;
	class IOutputDev
	{
	public:

		// Start and end of equation
		virtual void BeginEquation() = 0;
		virtual void EndEquation()   = 0;

		// Start and end of block.
		virtual void BeginBlock() = 0;
		virtual void EndBlock()   = 0;

		// Set size in current block
        virtual void SetSize(_UINT16 nSize) = 0;

		// Add symbol. Various elements can be added to symbol. Strikethrough, accent, etc.
        virtual void BeginChar(unsigned short uChar, unsigned char nTypeFace, bool bSpecialSymbol) = 0;
		virtual void AddCharEmbel(MEMBELTYPE eType) = 0;
		virtual void EndChar() = 0;

		// Matrix. Number of blocks here equals nRows * nCol, sent sequentially in normal order (first row left to right, second row left to right, etc.)
        virtual void BeginMatrix(unsigned char nVAlign, MMATRIXHORALIGN eHorAlign, MMATRIXVERALIGN eVerAlign, bool bEqualRows, bool bEqualCols, unsigned char nRows, unsigned char nCols, unsigned char* pVerBorders, unsigned char* pHorBorders) = 0;
		virtual void EndMatrix  () = 0;

        virtual void StartPile(unsigned char nHAlign, unsigned char nVAlign) = 0;
		virtual void EndPile() = 0;

		// Brackets with element inside
		virtual void BeginBrackets(MBRACKETSTYPE eType, bool bOpen, bool bClose) = 0;
		virtual void EndBrackets  (MBRACKETSTYPE eType, bool bOpen, bool bClose) = 0;

		// Root. First block - base, second if present - degree.
		virtual void BeginRoot(bool bDegree) = 0;
		virtual void EndRoot  () = 0;

		// Fractions. First block - numerator, second block - denominator.
		virtual void BeginFraction(MFRACTIONTYPES eType, bool bInline) = 0;
		virtual void EndFraction  () = 0;

		// If bInline = true, then block sequence is Base, Sub, Sup
		// If bInline = false, then block sequence is Sub, Sup, Base
		virtual void BeginScript(MSCRIPTALIGN eAlign, bool bBase = false, bool bSup = false, bool bSub = false, bool bInline = true) = 0;
		virtual void EndScript  () = 0;

		// Line above or below text
		virtual void BeginBar(MBARTYPE eType, bool bTop) = 0;
		virtual void EndBar  () = 0;

		// Arrow with label above or below (differs from BeginBar/EndBar in that here arrow is main
		// object, and text is small label, while those functions are opposite)
		virtual void BeginArrow(MARROWTYPE eType, bool bTop) = 0;
		virtual void EndArrow  () = 0;

		// Integrals. Block sequence is always following (if block present) Base, Sub, Sup
		virtual void BeginIntegral(MINTEGRALTYPE eType) = 0;
		virtual void EndIntegral  () = 0;

		// Curly brace with text above/below object. First comes main object, then curly brace label.
		virtual void BeginVerticalBrace(bool bTop) = 0;
		virtual void EndVerticalBrace  () = 0;

		// Sum, product, coproduct, union, intersection. Block sequence Base, Sub, Sup
		virtual void BeingNArray(MNARRAYTYPE eType) = 0;
		virtual void EndNArray  () = 0;

		// Long division. With quotient or without. First always comes base, then quotient if present.
		virtual void BeginLongDivision(MLONGDIVISION eType) = 0;
		virtual void EndLongDivision  () = 0;

		// < | > - this type of element
		virtual void BeginAngleBracketsWithSeparator(MANGLEBRACKETSWITHSEPARATORTYPE eType) = 0;
		virtual void EndAngleBracketsWithSeparator  () = 0;

        void AddFont(unsigned char nTypeFace, std::string sName, bool bBold, bool bItalic)
		{
			TMathFont aFont;
			aFont.sName   = sName;
			aFont.bBold   = bBold;
			aFont.bItalic = bItalic;
			m_mFonts[nTypeFace] = aFont;
		}

        TMathFont* GetFont(unsigned char nTypeFace)
		{
			TFontMap::iterator itFind = m_mFonts.find(nTypeFace);
			if (itFind != m_mFonts.end())			
				return &(itFind->second);

			return NULL;
		}

	protected:

		TFontMap m_mFonts;
	};

}
