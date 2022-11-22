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

#include "SimpleTypes_OMath.h"
#include "../Base/Unit.h"

// Здесь представлены все простые типы Drawing-Main из спецификации Office Open Xml (22.1.3)
namespace SimpleTypes
{
	EBreakBin CBreakBin::FromString(const std::wstring &sValue)
	{
		if       ( L"after"  == sValue ) this->m_eValue = breakBinAfter;
		else if  ( L"before" == sValue ) this->m_eValue = breakBinBefore;
		else							 this->m_eValue = breakBinRepeat;

		return this->m_eValue;
	}

	std::wstring CBreakBin::ToString() const
	{
		switch(this->m_eValue)
		{
		case breakBinAfter   : return L"after";
		case breakBinBefore  : return L"before";
		case breakBinRepeat  : return L"repeat";
		default              : return L"repeat";
		}
	}

	//--------------------------------------------------------------------------------
	// BreakBinSub 22.1.3.2 (Break Binary Subtraction Operators)
	//--------------------------------------------------------------------------------

	EBreakBinSub CBreakBinSub::FromString(const std::wstring &sValue)
	{
		if       ( L"+-"  == sValue )	this->m_eValue = breakBinPlusMinus;
		else if  ( L"-+"  == sValue )	this->m_eValue = breakBinMinusPlus;
		else							this->m_eValue = breakBinMinusMinus;

		return this->m_eValue;
	}

	std::wstring CBreakBinSub::ToString() const
	{
		switch(this->m_eValue)
		{
		case breakBinPlusMinus  : return L"+-";
		case breakBinMinusPlus  : return L"-+";
		case breakBinMinusMinus : return L"--";
		default					: return L"--";
		}
	}

	//--------------------------------------------------------------------------------
	// CMChar 22.1.3.3 (Character)
	//--------------------------------------------------------------------------------

	CMChar::CMChar() {}

	std::wstring CMChar::GetValue() const
	{
		return m_sValue;
	}

	void CMChar::SetValue(const std::wstring &sValue)
	{
		m_sValue = sValue;
	}

	std::wstring CMChar::FromString(const std::wstring &sValue)
	{
		m_sValue = sValue;
		return m_sValue;
	}

	std::wstring CMChar::ToString() const
	{
		return m_sValue;
	}

	//--------------------------------------------------------------------------------
	// CFType 22.1.3.4 (Fraction Type)
	//--------------------------------------------------------------------------------

	EFType CFType::FromString(const std::wstring &sValue)
	{
		if       ( L"bar"	== sValue )	this->m_eValue = fTypeBar;
		else if  ( L"lin"	== sValue )	this->m_eValue = fTypeLin;
		else if  ( L"noBar" == sValue )	this->m_eValue = fTypeNoBar;
		else							this->m_eValue = fTypeSkw;

		return this->m_eValue;
	}

	std::wstring CFType::ToString() const
	{
		switch(this->m_eValue)
		{
		case fTypeBar	: return L"bar";
		case fTypeLin	: return L"lin";
		case fTypeNoBar : return L"noBar";
		case fTypeSkw	: return L"skw";
		default			: return L"bar";
		}
	}

	//--------------------------------------------------------------------------------
	// CInteger2 22.1.3.5
	//--------------------------------------------------------------------------------

	int CInteger2::FromString(const std::wstring &sValue)
	{
		this->m_eValue = XmlUtils::GetInteger( sValue );

		if (this->m_eValue < -2)
			this->m_eValue = -2;
		if (this->m_eValue > 2)
			this->m_eValue = 2;

		return this->m_eValue;
	}

	std::wstring CInteger2::ToString() const
	{
		std::wstring sResult = std::to_wstring( this->m_eValue);

		return sResult;
	}

	//--------------------------------------------------------------------------------
	// CInteger255 22.1.3.6 (Integer value (1 to 255))
	//--------------------------------------------------------------------------------

	int CInteger255::FromString(const std::wstring &sValue)
	{
		this->m_eValue = XmlUtils::GetInteger64( sValue.c_str() );

		if (this->m_eValue < 1)
			this->m_eValue = 1;
		if (this->m_eValue > 255)
			this->m_eValue = 255;

		return this->m_eValue;
	}

	std::wstring CInteger255::ToString() const
	{
		std::wstring sResult = std::to_wstring( this->m_eValue);

		return sResult;
	}

	//--------------------------------------------------------------------------------
	// CMJc 22.1.3.7  (Justification Math)
	//--------------------------------------------------------------------------------	

	EMJc CMJc::FromString(const std::wstring &sValue)
	{
		if       ( L"center"	  == sValue ) this->m_eValue = mjcCenter;
		else if  ( L"centerGroup" == sValue ) this->m_eValue = mjcCenterGroup;
		else if  ( L"left"		  == sValue ) this->m_eValue = mjcLeft;
		else								  this->m_eValue = mjcRight;

		return this->m_eValue;
	}

	std::wstring CMJc::ToString() const
	{
		switch(this->m_eValue)
		{
		case mjcCenter		: return L"center";
		case mjcCenterGroup : return L"centerGroup";
		case mjcLeft		: return L"left";
		case mjcRight		: return L"right";
		default             : return L"centerGroup";
		}
	}
	
	//--------------------------------------------------------------------------------
	// CLimLoc 22.1.3.8 (Limit Location)
	//--------------------------------------------------------------------------------

	ELimLoc CLimLoc::FromString(const std::wstring &sValue)
	{
		if       ( L"subSup"		 == sValue ) this->m_eValue = limLocSubSup;
		else									 this->m_eValue = limLocUndOvr;

		return this->m_eValue;
	}

	std::wstring CLimLoc::ToString() const
	{
		switch(this->m_eValue)
		{
		case limLocSubSup		: return L"subSup";
		case limLocUndOvr		: return L"undOvr";
		default					: return L"subSup";
		}
	}

	//--------------------------------------------------------------------------------
	// CScript 22.1.3.9 (Script)
	//--------------------------------------------------------------------------------

	EScript CScript::FromString(const std::wstring &sValue)
	{
		if       ( L"double-struck"		== sValue ) this->m_eValue = scriptDoubleStruck;
		else if  ( L"fraktur"			== sValue ) this->m_eValue = scriptFraktur;
		else if  ( L"monospace"			== sValue ) this->m_eValue = scriptMonospace;
		else if  ( L"roman"				== sValue ) this->m_eValue = scriptRoman;
		else if  ( L"sans-serif"		== sValue ) this->m_eValue = scriptSansSerif;
		else										this->m_eValue = scriptScript;

		return this->m_eValue;
	}

	std::wstring CScript::ToString() const
	{
		switch(this->m_eValue)
		{
		case scriptDoubleStruck	: return L"double-struck";
		case scriptFraktur		: return L"fraktur";
		case scriptMonospace	: return L"monospace";
		case scriptRoman		: return L"roman";
		case scriptSansSerif	: return L"sans-serif";
		case scriptScript		: return L"script";
		default					: return L"roman";
		}
	}
	
	//--------------------------------------------------------------------------------
	// CShp 22.1.3.10 (Shape (Delimiters))
	//--------------------------------------------------------------------------------

	EShp CShp::FromString(const std::wstring &sValue)
	{
		if       ( L"centered"	== sValue )	this->m_eValue = shpCentered;
		else								this->m_eValue = shpMatch;

		return this->m_eValue;
	}

	std::wstring CShp::ToString() const
	{
		switch(this->m_eValue)
		{
		case shpCentered		: return L"centered";
		case shpMatch			: return L"match";
		default					: return L"centered";
		}
	}
	
	//--------------------------------------------------------------------------------
	// CSpacingRule 22.1.3.11 (Spacing Rule)
	//--------------------------------------------------------------------------------

	int CSpacingRule::FromString(const std::wstring &sValue)
	{
		this->m_eValue = XmlUtils::GetInteger( sValue );

		if (this->m_eValue < 0)
			this->m_eValue = 0;
		if (this->m_eValue > 4)
			this->m_eValue = 4;

		return this->m_eValue;
	}

	std::wstring CSpacingRule::ToString() const
	{
		std::wstring sResult = std::to_wstring( this->m_eValue);

		return sResult;
	}
	
	//--------------------------------------------------------------------------------
	// CStyle 22.1.3.12 (Style)
	//--------------------------------------------------------------------------------

	EStyle CStyle::FromString(const std::wstring &sValue)
	{
		if       ( L"b"			== sValue ) this->m_eValue = styleBold;
		else if  ( L"bi"		== sValue ) this->m_eValue = styleBoldItalic;
		else if  ( L"i"			== sValue ) this->m_eValue = styleItalic;
		else								this->m_eValue = stylePlain;

		return this->m_eValue;
	}

	std::wstring CStyle::ToString() const
	{
		switch(this->m_eValue)
		{
		case styleBold			: return L"b";
		case styleBoldItalic	: return L"bi";
		case styleItalic		: return L"i";
		case stylePlain			: return L"p";
		default					: return L"i";
		}
	}
	
	//--------------------------------------------------------------------------------
	// CTopBot 22.1.3.13 (Top-Bottom)
	//--------------------------------------------------------------------------------

	ETopBot CTopBot::FromString(const std::wstring &sValue)
	{
		if       ( L"bot"	== sValue )	this->m_eValue = tbBot;
		else if  ( L"bottom"== sValue )	this->m_eValue = tbBot;
		else							this->m_eValue = tbTop;

		return this->m_eValue;
	}

	std::wstring CTopBot::ToString() const
	{
		switch(this->m_eValue)
		{
		case tbBot		: return L"bot";
		case tbTop		: return L"top";
		default			: return L"bot";
		}
	}

	//--------------------------------------------------------------------------------
	// CUnSignedInteger 22.1.3.14 (Part 1)
	//--------------------------------------------------------------------------------

	unsigned int CUnSignedInteger::FromString(const std::wstring &sValue)
	{
		this->m_eValue = XmlUtils::GetUInteger( sValue );

		return this->m_eValue;
	}

	std::wstring CUnSignedInteger::ToString() const
	{
		return std::to_wstring( this->m_eValue);
	}
}
