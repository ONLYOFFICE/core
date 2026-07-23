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

#include "SimpleTypes_Base.h"

// Here are all simple Drawing-Main types from the Office Open XML specification (22.1.3)
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// BreakBin 22.1.3.1 (Break Binary Operators)
	//--------------------------------------------------------------------------------

	enum EBreakBin
	{
		breakBinAfter  = 0,
		breakBinBefore = 1,
		breakBinRepeat = 2
	};

	DEFINE_SIMPLE_TYPE(CBreakBin, EBreakBin, breakBinRepeat)

	//--------------------------------------------------------------------------------
	// BreakBinSub 22.1.3.2 (Break Binary Subtraction Operators)
	//--------------------------------------------------------------------------------

	enum EBreakBinSub
	{
		breakBinPlusMinus  = 0,
		breakBinMinusPlus  = 1,
		breakBinMinusMinus = 2
	};

	DEFINE_SIMPLE_TYPE(CBreakBinSub, EBreakBinSub, breakBinMinusMinus)

	//--------------------------------------------------------------------------------
	// CMChar 22.1.3.3 (Character)
	//--------------------------------------------------------------------------------

	class CMChar
	{
	public:
		CMChar();

		std::wstring GetValue() const;
		void    SetValue(const std::wstring &sValue);

		std::wstring FromString(const std::wstring &sValue);
		std::wstring ToString  () const;

		SimpleTypes_DefaultS(CMChar)

		private:
			std::wstring m_sValue;
	};

	//--------------------------------------------------------------------------------
	// CFType 22.1.3.4 (Fraction Type)
	//--------------------------------------------------------------------------------

	enum EFType
	{
		fTypeBar	= 0,
		fTypeLin	= 1,
		fTypeNoBar	= 2,
		fTypeSkw	= 3
	};

	DEFINE_SIMPLE_TYPE(CFType, EFType, fTypeBar)

	//--------------------------------------------------------------------------------
	// CInteger2 22.1.3.5
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE(CInteger2, int, 0)

	//--------------------------------------------------------------------------------
	// CInteger255 22.1.3.6 (Integer value (1 to 255))
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE(CInteger255, int, 1)

	//--------------------------------------------------------------------------------
	// CMJc 22.1.3.7  (Justification Math)
	//--------------------------------------------------------------------------------

	enum EMJc
	{
		mjcCenter  = 0,
		mjcCenterGroup = 1,
		mjcLeft = 2,
		mjcRight = 3
	};

	DEFINE_SIMPLE_TYPE(CMJc, EMJc, mjcCenterGroup)
	
	//--------------------------------------------------------------------------------
	// CLimLoc 22.1.3.8 (Limit Location)
	//--------------------------------------------------------------------------------

	enum ELimLoc
	{
		limLocSubSup  = 0,
		limLocUndOvr  = 1
	};

	DEFINE_SIMPLE_TYPE(CLimLoc, ELimLoc, limLocSubSup)

	//--------------------------------------------------------------------------------
	// CScript 22.1.3.9 (Script)
	//--------------------------------------------------------------------------------

	enum EScript
	{
		scriptDoubleStruck  = 0,
		scriptFraktur		= 1,
		scriptMonospace		= 2,
		scriptRoman			= 3,
		scriptSansSerif		= 4,
		scriptScript		= 5
	};

	DEFINE_SIMPLE_TYPE(CScript, EScript, scriptRoman)
	
	//--------------------------------------------------------------------------------
	// CShp 22.1.3.10 (Shape (Delimiters))
	//--------------------------------------------------------------------------------

	enum EShp
	{
		shpCentered  = 0,
		shpMatch	 = 1
	};

	DEFINE_SIMPLE_TYPE(CShp, EShp, shpCentered)
	
	//--------------------------------------------------------------------------------
	// CSpacingRule 22.1.3.11 (Spacing Rule)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE(CSpacingRule, int, 0)
	
	//--------------------------------------------------------------------------------
	// CStyle 22.1.3.12 (Style)
	//--------------------------------------------------------------------------------

	enum EStyle
	{
		styleBold		= 0,
		styleBoldItalic = 1,
		styleItalic		= 2,
		stylePlain		= 3
	};

	DEFINE_SIMPLE_TYPE(CStyle, EStyle, styleBoldItalic)
	
	//--------------------------------------------------------------------------------
	// CTopBot 22.1.3.13 (Top-Bottom)
	//--------------------------------------------------------------------------------

	enum ETopBot
	{
		tbBot	 = 0,
		tbTop	 = 1
	};

	DEFINE_SIMPLE_TYPE(CTopBot, ETopBot, tbBot)

	//--------------------------------------------------------------------------------
	// CUnSignedInteger 22.1.3.14 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE(CUnSignedInteger, unsigned int, 0)
}
