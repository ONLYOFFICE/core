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

#include "../Base/Base.h"

namespace SimpleTypes
{
#define SimpleTypes_DefaultString(Class)																		\
	Class(const std::wstring &sValue) { FromString(sValue); }													\
	Class& operator=(const std::wstring& sValue) { FromString(sValue); return *this; }

#define SimpleTypes_Default(Class)																				\
	bool operator==(const Class& oOther) const { return (this->m_eValue == oOther.m_eValue) ? true : false; }	\
	SimpleTypes_DefaultString(Class)

#define SimpleTypes_DefaultD(Class)																				\
	bool operator==(const Class& oOther) const { return (this->m_dValue == oOther.m_dValue) ? true : false; }	\
	SimpleTypes_DefaultString(Class)

#define SimpleTypes_DefaultS(Class)																				\
	bool operator==(const Class& oOther) const { return (this->m_sValue == oOther.m_sValue) ? true : false; }	\
	SimpleTypes_DefaultString(Class)

#define DEFINE_SIMPLE_TYPE_BASE(Class, Type, Default)															\
	class Class                                                                                                 \
	{                                                                                                           \
	public:                                                                                                     \
		Class(const std::wstring& sValue) { m_eValue = Default; FromString(sValue); }                           \
		Class& operator=(const std::wstring& sValue) { FromString(sValue); return *this; }                      \
		Class(const Type& sValue) { m_eValue = sValue; }                                                        \
		Class& operator=(const Type& sValue) { m_eValue = sValue; return *this; }                               \
		bool operator==(const Class& oOther) const { return (m_eValue == oOther.m_eValue) ? true : false; }     \
		Type GetValue() const { return m_eValue; }                                                              \
		void SetValueFromByte(const unsigned char& value) { m_eValue = (Type)value; }                           \
		Type FromString(const std::wstring &sValue);                                                            \
		Type FromStringA(const char* pValue);                                                                   \
		std::wstring ToString() const;                                                                          \
	public:                                                                                                     \
		Type m_eValue;

#define DEFINE_SIMPLE_TYPE_START(Class, Type, Default)															\
	DEFINE_SIMPLE_TYPE_BASE(Class, Type, Default)                                                               \
	public:                                                                                                     \
		Class() { m_eValue = Default; }                                                                         \
		void SetValue(const Type& value) { m_eValue = value; }

#define DEFINE_SIMPLE_TYPE_START_NSV(Class, Type, Default)											            \
	DEFINE_SIMPLE_TYPE_BASE(Class, Type, Default)                                                               \
	public:                                                                                                     \
		Class() { m_eValue = Default; }                                                                         \
		void SetValue(const Type value);

#define DEFINE_SIMPLE_TYPE_START_NC(Class, Type, Default)												        \
	DEFINE_SIMPLE_TYPE_BASE(Class, Type, Default)                                                               \
	public:                                                                                                     \
		void SetValue(const Type& value) { m_eValue = value; }

#define DEFINE_SIMPLE_TYPE(Class, Type, Default)															    \
	DEFINE_SIMPLE_TYPE_START(Class, Type, Default)                                                              \
	};

#define DEFINE_SIMPLE_TYPE_NSV(Class, Type, Default)															\
	DEFINE_SIMPLE_TYPE_START_NSV(Class, Type, Default)                                                          \
	};

#define DEFINE_SIMPLE_TYPE_NC(Class, Type, Default)																\
	DEFINE_SIMPLE_TYPE_START_NC(Class, Type, Default)															\
	};

	//--------------------------------------------------------------------------------
	// SimpleType
	//--------------------------------------------------------------------------------

	template<typename E, E DefValue = 0>
	class CSimpleType
	{
	public:
		CSimpleType()
		{
			m_eValue = DefValue;
		}
		virtual ~CSimpleType() {}

		virtual E GetValue () const
		{
			return m_eValue;
		}
		void SetValueFromByte(unsigned char value)
		{
			m_eValue = (E)value;
		}
		virtual void SetValue(E eValue)
		{
			m_eValue = eValue;
		}

		virtual E            FromString(const std::wstring &sValue) = 0;
		virtual std::wstring ToString  () const                     = 0;

	protected:

		E m_eValue;
	};

	//--------------------------------------------------------------------------------
	// UniversalMeasure
	//--------------------------------------------------------------------------------

	class CUniversalMeasure
	{
	public:
		CUniversalMeasure();
		virtual ~CUniversalMeasure();

		virtual double FromString(const std::wstring &sValue)	= 0;
		virtual std::wstring ToString() const					= 0;

		virtual void SetValue(double val)						= 0;
		double GetValue() const;

		double ToPoints() const;
		double ToInches() const;
		double ToMm() const;
		long ToTwips() const;
		long ToHps() const;
		unsigned long ToUnsignedTwips() const;

		virtual double FromHps(double dValue);
		virtual double FromPoints(double dValue);
		virtual double FromTwips(double dValue);
		virtual double FromMm(double dValue);
		virtual double FromInches(double dValue);
		virtual double FromEmu(double dValue);

		bool IsUnits() const;

	protected:
		void Parse(const std::wstring &sValue, double dKoef);

	protected:
		bool   m_bUnit = false;
		double m_dValue = 0; // Value in points
	};

	//--------------------------------------------------------------------------------
	// UniversalMeasureOrPercent
	//--------------------------------------------------------------------------------

	class CUniversalMeasureOrPercent : public CUniversalMeasure
	{
	public:
		CUniversalMeasureOrPercent();
		virtual ~CUniversalMeasureOrPercent();
		
		virtual void SetValue(double dValue);
		virtual double FromString(const std::wstring &sValue);
		virtual std::wstring ToString  () const;

		bool IsPercent() const;

	protected:
		bool m_bTrailingPercentSign;
	};

	//--------------------------------------------------------------------------------
	// Class inherited from CUniversalMeasure, for which default values are points.
	//--------------------------------------------------------------------------------

	class CPoint : public CUniversalMeasure
	{
	public:
		CPoint();

		virtual double FromString(const std::wstring &sValue);
		virtual void SetValue(double dValue);
		virtual std::wstring ToString () const;

		virtual double FromPoints(double dValue);
		virtual double FromInches(double dValue);

		SimpleTypes_DefaultD(CPoint)
	};

	//--------------------------------------------------------------------------------
	// Inch
	//--------------------------------------------------------------------------------

	class CInch : public CUniversalMeasure
	{
	public:
		CInch();

		virtual double FromString(const std::wstring &sValue);
		virtual void SetValue(double dValue);
		virtual std::wstring ToString() const;

		SimpleTypes_DefaultD(CInch)
	};

	//--------------------------------------------------------------------------------
	// Class inherited from CUniversalMeasure, for which default values are EMU.
	//--------------------------------------------------------------------------------

	class CEmu : public CUniversalMeasure
	{
	public:
		CEmu();

		virtual double FromString(const std::wstring &sValue);
		virtual void SetValue(double dValue);
		virtual std::wstring ToString() const;

		virtual double FromPoints(double dValue);
		virtual double FromInches(double dValue);

		virtual double FromEmu(double dValue);
		virtual double FromPx(double dValue);
		virtual double ToMm();
        virtual _INT64 ToEmu();
		virtual long ToPx();
		double GetValue () const;

		SimpleTypes_DefaultD(CEmu)
	};

	//--------------------------------------------------------------------------------
	// Class that reads double values
	//--------------------------------------------------------------------------------

	class CDouble
	{
	public:
		CDouble();
		virtual ~CDouble();

		CDouble(const double& val);

		virtual double FromString(const std::wstring &sValue);

		virtual std::wstring ToString  () const;
		virtual std::wstring ToString2() const;

		void SetValue(double dValue);
		double GetValue () const;

		SimpleTypes_DefaultD(CDouble)

	private:
		double m_dValue;
	};

	//--------------------------------------------------------------------------------
	// DecimalNumber 17.18.10 (Part 1)
	//--------------------------------------------------------------------------------

	DEFINE_SIMPLE_TYPE(CDecimalNumber, int, 0)

} // SimpleTypes
