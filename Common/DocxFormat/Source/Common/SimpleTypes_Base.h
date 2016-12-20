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

#include "math.h"
#include "../Common/Unit.h"
#include "../XML/xmlutils.h"
#include "../../../../DesktopEditor/common/Types.h"

namespace SimpleTypes
{
	// TO DO: В будущем надо улучшить функции FromString. Надо отказаться от сравнения
	//        строк, вместо этого высчитываем хэш и сравниваем по его значению.

#define SimpleType_FromString(Enum) \
	virtual Enum FromString(const wchar_t* cwsStr)\
	{\
		std::wstring wsTemp = cwsStr;\
		return FromString( (const std::wstring&)wsTemp );\
	}\
	virtual Enum FromString(const std::wstring& wsStr)\
	{\
		CString sTemp( wsStr.c_str() );\
		return FromString( (CString&)sTemp );\
	}

#define SimpleType_FromString2(Enum) \
	Enum FromString(const wchar_t* cwsStr)\
	{\
		std::wstring wsTemp = cwsStr;\
		return FromString( (const std::wstring&)wsTemp );\
	}\
	Enum FromString(const std::wstring& wsStr)\
	{\
		CString sTemp( wsStr.c_str() );\
		return FromString( (CString&)sTemp );\
	}

#define SimpleTypes_AdditionalOpearators(Class) \
	const bool operator==(const Class& oOther) const\
	{\
        if ( this->m_eValue == oOther.m_eValue )\
			return true;\
		return false;\
	}

#define UniversalMeasure_AdditionalOpearators(Class) \
	const bool operator==(const Class& oOther) const\
	{\
		if ( m_dValue == oOther.m_dValue )\
			return true;\
		return false;\
	}

//#if defined(_WIN32) || defined (_WIN64)
//#define SimpleType_Operator_Equal(Class) \
//	Class(CString &sValue)\
//	{\
//		FromString( sValue );\
//	}\
//	Class(const wchar_t* cwsValue)\
//	{\
//		FromString( cwsValue );\
//	}\
//	Class(const std::wstring& wsStr)\
//	{\
//		FromString( wsStr );\
//	}\
//	const Class &operator =(CString &sValue)\
//	{\
//		FromString( sValue );\
//		return *this;\
//	}\
//	const Class &operator =(const wchar_t* cwsString)\
//	{\
//		FromString( cwsString );\
//		return *this;\
//	}\
//	const Class &operator =(const std::wstring& wsStr)\
//	{\
//		FromString( wsStr );\
//		return *this;\
//	}
//#else
#define SimpleType_Operator_Equal(Class) \
    Class(CString &sValue)\
    {\
        FromString( sValue );\
    }\
    Class(const wchar_t* cwsValue)\
    {\
        FromString( cwsValue );\
    }\
    Class(const std::wstring& wsStr)\
    {\
        FromString( wsStr );\
    }\
    const Class &operator =(CString &sValue)\
    {\
        FromString( sValue );\
        return *this;\
    }\
    const Class &operator =(const wchar_t* cwsString)\
    {\
        FromString( cwsString );\
        return *this;\
    }\
    const Class &operator =(const std::wstring& wsStr)\
    {\
        FromString( wsStr );\
        return *this;\
    }
//#endif

	template<typename E, E DefValue = 0>
	class CSimpleType
	{
	public:

		CSimpleType()
		{
			m_eValue = DefValue;
		}

		virtual E       GetValue  () const
		{
			return m_eValue;
		}

		virtual void    SetValue  (E eValue)
		{
			m_eValue = eValue;
		}

		virtual E       FromString(CString &sValue)     = 0;
		virtual CString ToString  () const              = 0;

	protected:

		E m_eValue;
	};

	class CUniversalMeasure
	{
	public:

		CUniversalMeasure() {}

		virtual double  FromString(CString &sValue)     = 0;
		virtual CString ToString  () const              = 0;

		double ToPoints() const
		{
			return m_dValue;
		}
		double ToInches() const
		{
			return m_dValue / 72.0;
		}
		double ToMm() const
		{
			return m_dValue * 25.4 / 72;
		}
		long ToTwips() const
		{
			return (long)Pt_To_Dx(m_dValue);
		}
		virtual double FromPoints(double dValue)
		{
			m_dValue = dValue;
			return m_dValue;
		}
		virtual double FromTwips(double dValue)
		{
			m_dValue = Dx_To_Pt(dValue);
			return m_dValue;
		}
		virtual double FromMm(double dValue)
		{
			m_dValue = Mm_To_Pt(dValue);
			return m_dValue;
		}
		virtual double FromInches(double dValue)
		{
			m_dValue = Inch_To_Pt( dValue );
			return m_dValue;
		}
		virtual double FromEmu   (double dValue)
		{
			m_dValue = Emu_To_Pt( dValue );
			return m_dValue;
		}

	protected:

		void Parse(CString &sValue, double dKoef)
		{
            m_bUnit = false;
            m_dValue = 0;

            if (sValue.IsEmpty()) return;

            if ( sValue.GetLength() <= 2 )
			{
				try
				{
					m_dValue = _wtof( sValue ) / dKoef;
				}
				catch(...)
				{
				}
				return;
			}

			// Проверим последние два символа
			CString sUnit = sValue.Mid( sValue.GetLength() - 2, 2 );
			m_bUnit = true;

			if ( _T("cm") == sUnit )
			{
				double dValue = _wtof( sValue.Mid( 0, sValue.GetLength() - 2 ) );
				m_dValue = Cm_To_Pt( dValue );
			}
			else if ( _T("mm") == sUnit )
			{
				double dValue = _wtof( sValue.Mid( 0, sValue.GetLength() - 2 ) );  
				m_dValue = Mm_To_Pt( dValue );
			}
			else if ( _T("in") == sUnit )
			{
				double dValue = _wtof( sValue.Mid( 0, sValue.GetLength() - 2 ) );  
				m_dValue = Inch_To_Pt( dValue );
			}
			else if ( _T("pt") == sUnit )
			{
				m_dValue = _wtof( sValue.Mid( 0, sValue.GetLength() - 2 ) );  
			}
			else if ( _T("pc") == sUnit )
			{
				double dValue = _wtof( sValue.Mid( 0, sValue.GetLength() - 2 ) );  
				m_dValue = dValue * 12.0;
			}
			else if ( _T("pi") == sUnit )
			{
				double dValue = _wtof( sValue.Mid( 0, sValue.GetLength() - 2 ) );  
				m_dValue = dValue * 12.0;
			}
			else
			{
				m_bUnit = false;
				try
				{
					m_dValue = _wtof( sValue ) / dKoef;
				}
				catch(...)
				{
				}
				return;
			}
		}


	protected:

		bool   m_bUnit;  
		double m_dValue; // Значение в пунктах
	};

	//--------------------------------------------------------------------------------
	// Класс наследуемый от CUniversalMeasure, для которого обычные значения - пункты.
	//--------------------------------------------------------------------------------	
	class CPoint : public CUniversalMeasure
	{
	public:
		CPoint() {}

		virtual double  FromString(CString &sValue)
		{
			Parse(sValue, 1);
			return m_dValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;

			sResult.Format( _T("%fpt"), m_dValue);

			return sResult;
		}

		virtual double FromPoints(double dValue)
		{
			m_dValue = dValue;
			return m_dValue;
		}
		virtual double FromInches(double dValue)
		{
			m_dValue = dValue * 72;
			return m_dValue;
		}

  
		void SetValue(double dValue)
		{
			m_dValue = dValue;
		}
		double GetValue () const
		{
			return m_dValue;
		}
		SimpleType_FromString          (double)
		SimpleType_Operator_Equal      (CPoint)
		UniversalMeasure_AdditionalOpearators(CPoint)
	};

	class CInch : public CUniversalMeasure
	{
	public:
		CInch() {}

		virtual double  FromString(CString &sValue)
		{
			Parse(sValue, 1.0 / 72);
			return m_dValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;

			sResult.Format( _T("%fin"), ToInches());

			return sResult;
		}

		SimpleType_FromString          (double)
		SimpleType_Operator_Equal      (CInch)
		UniversalMeasure_AdditionalOpearators(CInch)
	};


	//--------------------------------------------------------------------------------
	// Класс наследуемый от CUniversalMeasure, для которого обычные значения - emu.
	//--------------------------------------------------------------------------------	
	class CEmu : public CUniversalMeasure
	{
	public:
		CEmu() {}

		virtual double  FromString(CString &sValue)
		{
			Parse(sValue, 12700);
			return m_dValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;

			sResult.Format( _T("%fpt"), m_dValue);

			return sResult;
		}

		virtual double FromPoints(double dValue)
		{
			m_dValue = dValue;
			return m_dValue;
		}
		virtual double FromInches(double dValue)
		{
			m_dValue = dValue * 72;
			return m_dValue;
		}

		virtual double FromEmu(double dValue)
		{
			m_dValue = Emu_To_Pt(dValue);
			return  m_dValue;
		}
		virtual double FromPx(double dValue)
		{
			m_dValue = Px_To_Pt(dValue);
			return  m_dValue;
		}
  		virtual double ToMm()
		{
			return  Pt_To_Mm(m_dValue);
		}
		virtual __int64 ToEmu()
		{
			return  (__int64)Pt_To_Emu(m_dValue);
		}
		virtual long ToPx()
		{
			return  (long)Pt_To_Px(m_dValue);
		}
		void SetValue(double dValue)
		{
			m_dValue = dValue;
		}
		double GetValue () const
		{
			return m_dValue;
		}
		SimpleType_FromString          (double)
		SimpleType_Operator_Equal      (CEmu)
		UniversalMeasure_AdditionalOpearators(CEmu)
	};


	//--------------------------------------------------------------------------------
	// Класс читающий double
	//--------------------------------------------------------------------------------	
	class CDouble
	{
	public:
		CDouble() {}

		virtual double  FromString(CString &sValue)
		{
			m_dValue = _wtof( sValue );
			return m_dValue;
		}

		virtual CString ToString  () const 
		{
			CString sResult;
			sResult.Format( _T("%f"), m_dValue);
			return sResult;
		}
		virtual std::wstring ToString2() const
		{
			return std::to_wstring(m_dValue);
		}
  
		void SetValue(double dValue)
		{
			m_dValue = dValue;
		}
		double GetValue () const
		{
			return m_dValue;
		}
		SimpleType_FromString          (double)
		SimpleType_Operator_Equal      (CDouble)

	private:

		double m_dValue;
	};


} // SimpleTypes
