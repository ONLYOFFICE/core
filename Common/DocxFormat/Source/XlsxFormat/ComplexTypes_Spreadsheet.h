﻿/*
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
#include "../Common/ComplexTypes.h"

#include "SimpleTypes_Spreadsheet.h"

namespace ComplexTypes
{
	namespace Spreadsheet
	{
		class ComplexType
		{
		public:
			ComplexType(){};
            virtual ~ComplexType() {}

			virtual void			FromXML(XmlUtils::CXmlNode& oNode) = 0;
			virtual std::wstring	ToString() const = 0;
		};

		template<SimpleTypes::EOnOff eDefValue = SimpleTypes::onoffTrue>
		class COnOff2 : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(COnOff2)
				COnOff2()
			{
			}
			virtual ~COnOff2()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				sResult += _T("val=\"");
				sResult += m_oVal.ToString();
				sResult += _T("\" ");

				return sResult;
			}
			void FromBool(bool bVal)
			{
				m_oVal.FromBool(bVal);
			}
			bool ToBool()
			{
				return m_oVal.ToBool();
			}
			SimpleTypes::COnOff<eDefValue> m_oVal;

		};
        class String : public ComplexType
		{
		public:
            ComplexTypes_AdditionConstructors(String)
			String()
			{
			}
            virtual ~String()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_sVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_sVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += *m_sVal;
					sResult += _T("\" ");
				}

				return sResult;
			}
			std::wstring ToString2() const
			{
				std::wstring sResult;

				if ( m_sVal.IsInit() )
					sResult += *m_sVal;

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_sVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable_string m_sVal;
		};
		class CDouble : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDouble)
			CDouble()
			{
			}
			virtual ~CDouble()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:
		
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CDouble > m_oVal;
		};
		class CPointMeasure : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CPointMeasure)
			CPointMeasure()
			{
			}
			virtual ~CPointMeasure()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CPointMeasure<> > m_oVal;
		};
		class CChartLayoutTarget : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartLayoutTarget)
			CChartLayoutTarget()
			{
			}
			virtual ~CChartLayoutTarget()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::Spreadsheet::CChartLayoutTarget<>> m_oVal;
		};
		class CChartHMode : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartHMode)
			CChartHMode()
			{
			}
			virtual ~CChartHMode()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::Spreadsheet::CChartHMode<>> m_oVal;
		};
		class CChartAxPos : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartAxPos)
			CChartAxPos()
			{
			}
			virtual ~CChartAxPos()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::Spreadsheet::CChartAxPos<>> m_oVal;
		};
		class CChartBarGrouping : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartBarGrouping)
			CChartBarGrouping()
			{
			}
			virtual ~CChartBarGrouping()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
					WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::Spreadsheet::CChartBarGrouping<> > m_oVal;
		};
		class CChartBarDerection : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartBarDerection)
			CChartBarDerection()
			{
			}
			virtual ~CChartBarDerection()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::Spreadsheet::CChartBarDirection<> > m_oVal;
		};
		class CChartSymbol : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CChartSymbol)
			
			CChartSymbol(){}
			virtual ~CChartSymbol() {}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::Spreadsheet::CChartSymbol<> > m_oVal;
		};

		class CRegionLabelLayout : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CRegionLabelLayout)
			
			CRegionLabelLayout(){}
			virtual ~CRegionLabelLayout() {}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::Spreadsheet::CRegionLabelLayout<>> m_oVal;
		};
		class CParentLabelLayout : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CParentLabelLayout)
			
			CParentLabelLayout(){}
			virtual ~CParentLabelLayout() {}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("val"), m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += _T("val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}
				return sResult;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::Spreadsheet::CParentLabelLayout<>> m_oVal;
		};
	}
} // ComplexTypes
