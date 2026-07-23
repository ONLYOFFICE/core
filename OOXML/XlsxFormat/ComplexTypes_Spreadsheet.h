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
#include "../Common/ComplexTypes.h"
#include "../Common/SimpleTypes_Spreadsheet.h"

namespace ComplexTypes
{
	namespace Spreadsheet
	{
		class ComplexType
		{
		public:
			ComplexType();
			virtual ~ComplexType();

			virtual void			FromXML(XmlUtils::CXmlNode& oNode) = 0;
			virtual std::wstring	ToString() const = 0;
		};

		class COnOff2 : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(COnOff2)

			COnOff2();
			virtual ~COnOff2();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

			void FromBool(bool bVal);
			bool ToBool();

            SimpleTypes::COnOff m_oVal = SimpleTypes::onoffTrue;
		};

        class String : public ComplexType
		{
		public:
            ComplexTypes_AdditionConstructors(String)
			String();
			virtual ~String();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

			std::wstring ToString2() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string m_sVal;
		};

		class CDouble : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDouble)
			CDouble();
			virtual ~CDouble();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:		
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDouble > m_oVal;
		};

		class CPointMeasure : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPointMeasure)
			CPointMeasure();
			virtual ~CPointMeasure();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CPointMeasure> m_oVal;
		};

		class CChartLayoutTarget : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartLayoutTarget)
			CChartLayoutTarget();
			virtual ~CChartLayoutTarget();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CChartLayoutTarget> m_oVal;
		};

		class CChartHMode : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartHMode)
			CChartHMode();
			virtual ~CChartHMode();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CChartHMode> m_oVal;
		};

		class CChartAxPos : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartAxPos)
			CChartAxPos();
			virtual ~CChartAxPos();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CChartAxPos> m_oVal;
		};

		class CChartBarGrouping : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartBarGrouping)
			CChartBarGrouping();
			virtual ~CChartBarGrouping();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CChartBarGrouping> m_oVal;
		};

		class CChartBarDerection : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartBarDerection)
			CChartBarDerection();
			virtual ~CChartBarDerection();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CChartBarDirection> m_oVal;
		};

		class CChartSymbol : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartSymbol)
			
			CChartSymbol();
			virtual ~CChartSymbol();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CChartSymbol> m_oVal;
		};

		class CRegionLabelLayout : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CRegionLabelLayout)
			
			CRegionLabelLayout();
			virtual ~CRegionLabelLayout();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CRegionLabelLayout> m_oVal;
		};

		class CParentLabelLayout : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CParentLabelLayout)
			
			CParentLabelLayout();
			virtual ~CParentLabelLayout();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CParentLabelLayout> m_oVal;
		};

	}
} // ComplexTypes
