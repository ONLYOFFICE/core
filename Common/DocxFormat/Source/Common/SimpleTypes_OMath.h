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

// «десь представлены все простые типы Drawing-Main из спецификации Office Open Xml (22.1.3)
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

	template<EBreakBin eDefValue = breakBinRepeat>
	class CBreakBin : public CSimpleType<EBreakBin, eDefValue>
	{
	public:

		CBreakBin() {} 

		virtual EBreakBin FromString(std::wstring &sValue)
		{
            if       ( _T("after")  == sValue ) this->m_eValue = breakBinAfter;
            else if  ( _T("before") == sValue ) this->m_eValue = breakBinBefore;
            else								this->m_eValue = breakBinRepeat;

            return this->m_eValue;
		}

		virtual std::wstring ToString() const 
		{
            switch(this->m_eValue)
			{
			case breakBinAfter   : return _T("after");
			case breakBinBefore  : return _T("before");
			case breakBinRepeat  : return _T("repeat");
			default              : return _T("repeat");
			}
		}

		SimpleType_FromString     (EBreakBin)
		SimpleType_Operator_Equal (CBreakBin)
	};	

	//--------------------------------------------------------------------------------
	// BreakBinSub 22.1.3.2 (Break Binary Subtraction Operators) 
	//--------------------------------------------------------------------------------

	enum EBreakBinSub
	{
		breakBinPlusMinus  = 0,
		breakBinMinusPlus  = 1,
		breakBinMinusMinus = 2
	};

	template<EBreakBinSub eDefValue = breakBinMinusMinus>
	class CBreakBinSub : public CSimpleType<EBreakBinSub, eDefValue>
	{
	public:

		CBreakBinSub() {} 

		virtual EBreakBinSub FromString(std::wstring &sValue)
		{
            if       ( _T("+-")  == sValue )	this->m_eValue = breakBinPlusMinus;
            else if  ( _T("-+")  == sValue )	this->m_eValue = breakBinMinusPlus;
            else								this->m_eValue = breakBinMinusMinus;

            return this->m_eValue;
		}

		virtual std::wstring ToString() const 
		{
            switch(this->m_eValue)
			{
			case breakBinPlusMinus  : return _T("+-");
			case breakBinMinusPlus  : return _T("-+");
			case breakBinMinusMinus : return _T("--");
			default					: return _T("--");
			}
		}

		SimpleType_FromString     (EBreakBinSub)
		SimpleType_Operator_Equal (CBreakBinSub)
	};	

	//--------------------------------------------------------------------------------
	// CMChar 22.1.3.3 (Character)  
	//--------------------------------------------------------------------------------

    class CMChar
    {
    public:
        CMChar() {}

        std::wstring GetValue() const
		{
			return m_sValue;
		}

		void    SetValue(std::wstring &sValue)
		{
			m_sValue = sValue;
		}


		std::wstring FromString(std::wstring &sValue)
		{
			m_sValue = sValue;

			return m_sValue;
		}

		std::wstring ToString  () const 
		{
			return m_sValue;
		}

        SimpleType_FromString     (std::wstring)
        SimpleType_Operator_Equal (CMChar)

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

	template<EFType eDefValue = fTypeBar>
	class CFType : public CSimpleType<EFType, eDefValue>
	{
	public:

		CFType() {} 

		virtual EFType FromString(std::wstring &sValue)
		{
            if       ( _T("bar")	== sValue )	this->m_eValue = fTypeBar;
            else if  ( _T("lin")	== sValue )	this->m_eValue = fTypeLin;
            else if  ( _T("noBar")  == sValue )	this->m_eValue = fTypeNoBar;
            else								this->m_eValue = fTypeSkw;

            return this->m_eValue;
		}

		virtual std::wstring ToString() const 
		{
            switch(this->m_eValue)
			{
			case fTypeBar	: return _T("bar");
			case fTypeLin	: return _T("lin");
			case fTypeNoBar : return _T("noBar");
			case fTypeSkw	: return _T("skw");
			default			: return _T("bar");
			}
		}

		SimpleType_FromString     (EFType)
		SimpleType_Operator_Equal (CFType)
	};	

	//--------------------------------------------------------------------------------
	// CInteger2 22.1.3.5
	//--------------------------------------------------------------------------------

	template<int nDefValue = 0>
	class CInteger2 : public CSimpleType<int, nDefValue>
	{
	public:
		CInteger2() {}

		virtual int FromString(std::wstring &sValue)
		{
            this->m_eValue = _wtoi( sValue.c_str() );

            if (this->m_eValue < -2)
                this->m_eValue = -2;
            if (this->m_eValue > 2)
                this->m_eValue = 2;

            return this->m_eValue;
		}

		virtual std::wstring ToString  () const 
		{
            std::wstring sResult = std::to_wstring( this->m_eValue);

			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CInteger2)

	};
	 
	//--------------------------------------------------------------------------------
	// CInteger255 22.1.3.6 (Integer value (1 to 255))
	//--------------------------------------------------------------------------------

	template<int nDefValue = 1>
	class CInteger255 : public CSimpleType<int, nDefValue>
	{
	public:
		CInteger255() {}

		virtual int FromString(std::wstring &sValue)
		{
            this->m_eValue = _wtoi( sValue.c_str() );

            if (this->m_eValue < 1)
                this->m_eValue = 1;
            if (this->m_eValue > 255)
                this->m_eValue = 255;

            return this->m_eValue;
		}

		virtual std::wstring ToString  () const 
		{
            std::wstring sResult = std::to_wstring( this->m_eValue);

			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CInteger255)

	};

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

	template<EMJc eDefValue = mjcCenterGroup>
	class CMJc : public CSimpleType<EMJc, eDefValue>
	{
	public:

		CMJc() {} 

		virtual EMJc FromString(std::wstring &sValue)
		{
            if       ( _T("center")		 == sValue ) this->m_eValue = mjcCenter;
            else if  ( _T("centerGroup") == sValue ) this->m_eValue = mjcCenterGroup;
            else if  ( _T("left")		 == sValue ) this->m_eValue = mjcLeft;
            else									 this->m_eValue = mjcRight;

            return this->m_eValue;
		}

		virtual std::wstring ToString() const 
		{
            switch(this->m_eValue)
			{
			case mjcCenter		: return _T("center");
			case mjcCenterGroup : return _T("centerGroup");
			case mjcLeft		: return _T("left");
			case mjcRight		: return _T("right");
			default             : return _T("centerGroup");
			}
		}

		SimpleType_FromString     (EMJc)
		SimpleType_Operator_Equal (CMJc)
	};
	
	//--------------------------------------------------------------------------------
	// CLimLoc 22.1.3.8 (Limit Location)  
	//--------------------------------------------------------------------------------

	enum ELimLoc
	{
		limLocSubSup  = 0,
		limLocUndOvr  = 1
	};

	template<ELimLoc eDefValue = limLocSubSup>
	class CLimLoc : public CSimpleType<ELimLoc, eDefValue>
	{
	public:

		CLimLoc() {} 

		virtual ELimLoc FromString(std::wstring &sValue)
		{
            if       ( _T("subSup")		 == sValue ) this->m_eValue = limLocSubSup;
            else									 this->m_eValue = limLocUndOvr;

            return this->m_eValue;
		}

		virtual std::wstring ToString() const 
		{
            switch(this->m_eValue)
			{
			case limLocSubSup		: return _T("subSup");
			case limLocUndOvr		: return _T("undOvr");
			default					: return _T("subSup");
			}
		}

		SimpleType_FromString     (ELimLoc)
		SimpleType_Operator_Equal (CLimLoc)
	};
	 
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

	template<EScript eDefValue = scriptRoman>
	class CScript : public CSimpleType<EScript, eDefValue>
	{
	public:

		CScript() {} 

		virtual EScript FromString(std::wstring &sValue)
		{
            if       ( _T("double-struck")		== sValue ) this->m_eValue = scriptDoubleStruck;
            else if  ( _T("fraktur")			== sValue ) this->m_eValue = scriptFraktur;
            else if  ( _T("monospace")			== sValue ) this->m_eValue = scriptMonospace;
            else if  ( _T("roman")				== sValue ) this->m_eValue = scriptRoman;
            else if  ( _T("sans-serif")			== sValue ) this->m_eValue = scriptSansSerif;
            else											this->m_eValue = scriptScript;

            return this->m_eValue;
		}

		virtual std::wstring ToString() const 
		{
            switch(this->m_eValue)
			{
			case scriptDoubleStruck	: return _T("double-struck");
			case scriptFraktur		: return _T("fraktur");
			case scriptMonospace	: return _T("monospace");	
			case scriptRoman		: return _T("roman");
			case scriptSansSerif	: return _T("sans-serif");
			case scriptScript		: return _T("script");
			default					: return _T("roman");
			}
		}

		SimpleType_FromString     (EScript)
		SimpleType_Operator_Equal (CScript)
	};
	
	//--------------------------------------------------------------------------------
	// CShp 22.1.3.10 (Shape (Delimiters)) 
	//--------------------------------------------------------------------------------

	enum EShp
	{
		shpCentered  = 0,
		shpMatch	 = 1
	};

	template<EShp eDefValue = shpCentered>
	class CShp : public CSimpleType<EShp, eDefValue>
	{
	public:

		CShp() {} 

		virtual EShp FromString(std::wstring &sValue)
		{
            if       ( _T("centered")	== sValue )	 this->m_eValue = shpCentered;
            else									 this->m_eValue = shpMatch;

            return this->m_eValue;
		}

		virtual std::wstring ToString() const 
		{
            switch(this->m_eValue)
			{
			case shpCentered		: return _T("centered");
			case shpMatch			: return _T("match");
			default					: return _T("centered");
			}
		}

		SimpleType_FromString     (EShp)
		SimpleType_Operator_Equal (CShp)
	};
	
	//--------------------------------------------------------------------------------
	// CSpacingRule 22.1.3.11 (Spacing Rule) 
	//--------------------------------------------------------------------------------		

	template<int unDefValue = 0>
	class CSpacingRule : public CSimpleType<int, unDefValue>
	{
	public:
		CSpacingRule() {}

		virtual int FromString(std::wstring &sValue)
		{
            this->m_eValue = _wtoi( sValue.c_str() );

            if (this->m_eValue < 0)
                this->m_eValue = 0;
            if (this->m_eValue > 4)
                this->m_eValue = 4;

            return this->m_eValue;
		}

		virtual std::wstring ToString  () const 
		{
            std::wstring sResult = std::to_wstring( this->m_eValue);

			return sResult;
		}

		SimpleType_FromString     (int)
		SimpleType_Operator_Equal (CSpacingRule)
	};
	
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

	template<EStyle eDefValue = styleBoldItalic>
	class CStyle : public CSimpleType<EStyle, eDefValue>
	{
	public:

		CStyle() {} 

		virtual EStyle FromString(std::wstring &sValue)
		{
            if       ( _T("b")			== sValue ) this->m_eValue = styleBold;
            else if  ( _T("bi")			== sValue ) this->m_eValue = styleBoldItalic;
            else if  ( _T("i")			== sValue ) this->m_eValue = styleItalic;
            else									this->m_eValue = stylePlain;

            return this->m_eValue;
		}

		virtual std::wstring ToString() const 
		{
            switch(this->m_eValue)
			{
			case styleBold			: return _T("b");
			case styleBoldItalic	: return _T("bi");
			case styleItalic		: return _T("i");	
			case stylePlain			: return _T("p");
			default					: return _T("i");
			}
		}

		SimpleType_FromString     (EStyle)
		SimpleType_Operator_Equal (CStyle)
	};
	
	//--------------------------------------------------------------------------------
	// CTopBot 22.1.3.13 (Top-Bottom) 
	//--------------------------------------------------------------------------------

	enum ETopBot
	{
		tbBot	 = 0,
		tbTop	 = 1
	};

	template<ETopBot eDefValue = tbBot>
	class CTopBot : public CSimpleType<ETopBot, eDefValue>
	{
	public:

		CTopBot() {} 

		virtual ETopBot FromString(std::wstring &sValue)
		{
            if       ( _T("bot")	== sValue )	this->m_eValue = tbBot;
			else if  ( _T("bottom")	== sValue )	this->m_eValue = tbBot;
            else								this->m_eValue = tbTop;

            return this->m_eValue;
		}

		virtual std::wstring ToString() const 
		{
            switch(this->m_eValue)
			{
			case tbBot		: return _T("bot");
			case tbTop		: return _T("top");
			default			: return _T("bot");
			}
		}

		SimpleType_FromString     (ETopBot)
		SimpleType_Operator_Equal (CTopBot)
	};

	//--------------------------------------------------------------------------------
	// CUnSignedInteger 22.1.3.14 (Part 1)
	//--------------------------------------------------------------------------------		

	template<unsigned int unDefValue = 0>
	class CUnSignedInteger : public CSimpleType<unsigned int, unDefValue>
	{
	public:
		CUnSignedInteger() {}

		virtual unsigned int FromString(std::wstring &sValue)
		{
            this->m_eValue = _wtoi( sValue.c_str() );

            return this->m_eValue;
		}

		virtual std::wstring ToString  () const 
		{
			return std::to_wstring( this->m_eValue);
		}

		SimpleType_FromString     (unsigned int)
		SimpleType_Operator_Equal (CUnSignedInteger)
	};
}
