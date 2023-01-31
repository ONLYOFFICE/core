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

#include "SimpleTypes_Base.h"

// Здесь представлены все простые типы Drawing-Main из спецификации Office Open Xml (22.1.3)
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
