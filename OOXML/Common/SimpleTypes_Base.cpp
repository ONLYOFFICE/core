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

#include "math.h"
#include "../Base/Unit.h"
#include "./SimpleTypes_Base.h"
#include <boost/lexical_cast.hpp>

namespace SimpleTypes
{
	CUniversalMeasure::CUniversalMeasure() {}
	CUniversalMeasure::~CUniversalMeasure() {}

	double CUniversalMeasure::GetValue() const
	{
		return m_dValue;
	}
	double CUniversalMeasure::ToPoints() const
	{
		return m_dValue;
	}
	double CUniversalMeasure::ToInches() const
	{
		return m_dValue / 72.0;
	}
	double CUniversalMeasure::ToMm() const
	{
		return m_dValue * 25.4 / 72;
	}
	long CUniversalMeasure::ToTwips() const
	{
		return (long)Pt_To_Dx(m_dValue);
	}
	long CUniversalMeasure::ToHps() const
	{
		return (long)(m_dValue * 2);
	}
	unsigned long CUniversalMeasure::ToUnsignedTwips() const
	{
		return (unsigned long)Pt_To_Dx(m_dValue);
	}
	double CUniversalMeasure::FromHps(double dValue)
	{
		m_dValue = (dValue / 2);
		return m_dValue;
	}
	double CUniversalMeasure::FromPoints(double dValue)
	{
		m_dValue = dValue;
		return m_dValue;
	}
	double CUniversalMeasure::FromTwips(double dValue)
	{
		m_dValue = Dx_To_Pt(dValue);
		return m_dValue;
	}
	double CUniversalMeasure::FromMm(double dValue)
	{
		m_dValue = Mm_To_Pt(dValue);
		return m_dValue;
	}
	double CUniversalMeasure::FromInches(double dValue)
	{
		m_dValue = Inch_To_Pt( dValue );
		return m_dValue;
	}
	double CUniversalMeasure::FromEmu   (double dValue)
	{
		m_dValue = Emu_To_Pt( dValue );
		return m_dValue;
	}

	bool CUniversalMeasure::IsUnits() const
	{
		return m_bUnit;
	}

	void CUniversalMeasure::Parse(const std::wstring &sValue, double dKoef)
	{
		m_bUnit = false;
		m_dValue = 0;

		if (sValue.empty()) return;

		if ( sValue.length() <= 2 )
		{
			m_dValue = XmlUtils::GetDouble( sValue.c_str() ) / dKoef;

			return;
		}

		// Проверим последние два символа
		std::wstring sUnit = sValue.substr( sValue.length() - 2, 2 );
		m_bUnit = true;

		if ( L"cm" == sUnit )
		{
			double dValue = XmlUtils::GetDouble( sValue.substr( 0, sValue.length() - 2 ).c_str() );
			m_dValue = Cm_To_Pt( dValue );
		}
		else if ( L"mm" == sUnit )
		{
			double dValue = XmlUtils::GetDouble( sValue.substr( 0, sValue.length() - 2 ).c_str() );
			m_dValue = Mm_To_Pt( dValue );
		}
		else if ( L"in" == sUnit )
		{
			double dValue = XmlUtils::GetDouble( sValue.substr( 0, sValue.length() - 2 ).c_str() );
			m_dValue = Inch_To_Pt( dValue );
		}
		else if ( L"pt" == sUnit )
		{
			m_dValue = XmlUtils::GetDouble( sValue.substr( 0, sValue.length() - 2 ).c_str() );
		}
		else if ( L"pc" == sUnit )
		{
			double dValue = XmlUtils::GetDouble( sValue.substr( 0, sValue.length() - 2 ).c_str() );
			m_dValue = dValue * 12.0;
		}
		else if ( L"pi" == sUnit )
		{
			double dValue = XmlUtils::GetDouble( sValue.substr( 0, sValue.length() - 2 ).c_str() );
			m_dValue = dValue * 12.0;
		}
		else
		{
			m_bUnit = false;
			m_dValue = XmlUtils::GetDouble( sValue.c_str() ) / dKoef;

			return;
		}
	}

	CUniversalMeasureOrPercent::CUniversalMeasureOrPercent() {}
	CUniversalMeasureOrPercent::~CUniversalMeasureOrPercent() {}

	void CUniversalMeasureOrPercent::SetValue(double dValue)
	{
		m_bUnit = false;
		m_dValue = dValue;
	}
	double CUniversalMeasureOrPercent::FromString(const std::wstring &sValue)
	{
		m_bUnit = false;
		m_bTrailingPercentSign = false;
		if ( sValue.empty() )
		{
			m_dValue = 0;
			return m_dValue;
		}
		if('%' == sValue[sValue.length() - 1])
		{
			m_bTrailingPercentSign = true;
			m_dValue = XmlUtils::GetDouble( sValue.substr(0, sValue.length() - 1).c_str() );
		}
		else
		{
			Parse(sValue, 1);
		}
		return m_dValue;
	}
	std::wstring CUniversalMeasureOrPercent::ToString() const
	{
		std::wstring sResult;

		if ( m_bUnit )
			sResult = boost::lexical_cast<std::wstring>(m_dValue) + L"pt";
		else if ( m_bTrailingPercentSign )
			sResult = boost::lexical_cast<std::wstring>(m_dValue) + L"%";
		else
			sResult = std::to_wstring( (int)(m_dValue) );

		return sResult;
	}
	bool CUniversalMeasureOrPercent::IsPercent() const
	{
		return m_bTrailingPercentSign;
	}

	//--------------------------------------------------------------------------------
	// Класс наследуемый от CUniversalMeasure, для которого обычные значения - пункты.
	//--------------------------------------------------------------------------------
	CPoint::CPoint() {}

	double CPoint::FromString(const std::wstring &sValue)
	{
		Parse(sValue, 1);
		return m_dValue;
	}
	void CPoint::SetValue(double dValue)
	{
		m_bUnit = false;
		m_dValue = dValue;
	}
	std::wstring CPoint::ToString() const
	{
		return boost::lexical_cast<std::wstring>(m_dValue) + L"pt";
	}

	double CPoint::FromPoints(double dValue)
	{
		m_dValue = dValue;
		return m_dValue;
	}
	double CPoint::FromInches(double dValue)
	{
		m_dValue = dValue * 72;
		return m_dValue;
	}

	//--------------------------------------------------------------------------------
	// Класс наследуемый от CUniversalMeasure, для которого обычные значения - emu.
	//--------------------------------------------------------------------------------
	CInch::CInch() {}

	double CInch::FromString(const std::wstring &sValue)
	{
		Parse(sValue, 1.0 / 72);
		return m_dValue;
	}
	void CInch::SetValue(double dValue)
	{
		m_bUnit = false;
		m_dValue = FromInches(dValue);
	}
	std::wstring CInch::ToString() const
	{
		return boost::lexical_cast<std::wstring>(ToInches()) + L"in";
	}

	CEmu::CEmu() {}

	double CEmu::FromString(const std::wstring &sValue)
	{
		Parse(sValue, 12700);
		return m_dValue;
	}
	void CEmu::SetValue(double dValue)
	{
		m_bUnit = false;
		m_dValue = FromEmu(dValue);
	}
	std::wstring CEmu::ToString  () const
	{
		return boost::lexical_cast<std::wstring>(m_dValue) + L"pt";
	}

	double CEmu::FromPoints(double dValue)
	{
		m_dValue = dValue;
		return m_dValue;
	}
	double CEmu::FromInches(double dValue)
	{
		m_dValue = dValue * 72;
		return m_dValue;
	}

	double CEmu::FromEmu(double dValue)
	{
		m_dValue = Emu_To_Pt(dValue);
		return  m_dValue;
	}
	double CEmu::FromPx(double dValue)
	{
		m_dValue = Px_To_Pt(dValue);
		return  m_dValue;
	}
	double CEmu::ToMm()
	{
		return  Pt_To_Mm(m_dValue);
	}
	__int64 CEmu::ToEmu()
	{
		return  (__int64)Pt_To_Emu(m_dValue);
	}
	long CEmu::ToPx()
	{
		return  (long)Pt_To_Px(m_dValue);
	}

	double CEmu::GetValue () const
	{
		return m_dValue;
	}

	//--------------------------------------------------------------------------------
	// Класс читающий double
	//--------------------------------------------------------------------------------
	CDouble::CDouble() {}
	CDouble::~CDouble() {}

	CDouble::CDouble(const double& val)
	{
		this->m_dValue = (double)val;
	}

	double CDouble::FromString(const std::wstring &sValue)
	{
		m_dValue = XmlUtils::GetDouble( sValue );
		return m_dValue;
	}

	std::wstring CDouble::ToString  () const
	{
		if (std::isnan(m_dValue)) return L"NaN";
		else if (std::isinf(m_dValue)) return L"INF";
		else return boost::lexical_cast<std::wstring>(m_dValue);
	}
	std::wstring CDouble::ToString2() const
	{
		return boost::lexical_cast<std::wstring>(m_dValue);
	}

	void CDouble::SetValue(double dValue)
	{
		m_dValue = dValue;
	}
	double CDouble::GetValue () const
	{
		return m_dValue;
	}

	//--------------------------------------------------------------------------------
	// DecimalNumber 17.18.10 (Part 1)
	//--------------------------------------------------------------------------------

	int CDecimalNumber::FromString(const std::wstring &sValue)
	{
		try
		{
            this->m_eValue = XmlUtils::GetInteger(sValue);
			return this->m_eValue;
		}
		catch (...)
		{
		}

		try
		{
            this->m_eValue = static_cast<int>(XmlUtils::GetInteger64(sValue));
		}
		catch (...)
		{
			this->m_eValue = 0;
		}

		return this->m_eValue;
	}

	std::wstring CDecimalNumber::ToString() const
	{
		return std::to_wstring(this->m_eValue);
	}

} // SimpleTypes
