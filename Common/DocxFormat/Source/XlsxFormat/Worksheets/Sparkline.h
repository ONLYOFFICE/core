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
#ifndef OOX_SPARKLINE_FILE_INCLUDE_H_
#define OOX_SPARKLINE_FILE_INCLUDE_H_

#include "../CommonInclude.h"
#include "../Chart/ChartSerialize.h"
#include "../Styles/rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CSparkline : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSparkline)
			CSparkline()
			{
			}
			virtual ~CSparkline()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<x14:sparkline>"));
				if(m_oRef.IsInit())
				{
					writer.WriteString(_T("<xm:f>"));
					writer.WriteString(m_oRef.get());
					writer.WriteString(_T("</xm:f>"));
				}
				if(m_oSqRef.IsInit())
				{
					writer.WriteString(_T("<xm:sqref>"));
					writer.WriteString(m_oSqRef.get());
					writer.WriteString(_T("</xm:sqref>"));
				}
				writer.WriteString(_T("</x14:sparkline>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

                    if ( _T("f") == sName )
                        m_oRef = oReader.GetText3();
                    else if ( _T("sqref") == sName )
                        m_oSqRef = oReader.GetText3();
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Sparkline;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:
				nullable<std::wstring>										m_oRef;
				nullable<std::wstring>										m_oSqRef;
		};
		class CSparklines  : public WritingElementWithChilds<CSparkline>
		{
		public:
			WritingElement_AdditionConstructors(CSparklines)
			CSparklines()
			{
			}
			virtual ~CSparklines()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<x14:sparklines>"));

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }

				writer.WriteString(_T("</x14:sparklines>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

                    if ( _T("sparkline") == sName )
						m_arrItems.push_back(new CSparkline( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Sparklines;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CSparklineGroup : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSparklineGroup)
			CSparklineGroup()
			{
			}
			virtual ~CSparklineGroup()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<x14:sparklineGroup"));
				if(m_oManualMax.IsInit())
				{
					writer.WriteString(_T(" manualMax=\""));
					writer.WriteString(m_oManualMax->ToString2());
					writer.WriteString(_T("\""));
				}
				if(m_oManualMin.IsInit())
				{
					writer.WriteString(_T(" manualMin=\""));
					writer.WriteString(m_oManualMin->ToString2());
					writer.WriteString(_T("\""));
				}
				if(m_oLineWeight.IsInit())
				{
					writer.WriteString(_T(" lineWeight=\""));
					writer.WriteString(m_oLineWeight->ToString2());
					writer.WriteString(_T("\""));
				}
				if(m_oType.IsInit())
				{
					writer.WriteString(_T(" type=\""));
					writer.WriteString(m_oType->ToString());
					writer.WriteString(_T("\""));
				}
				if(m_oDateAxis.IsInit())
				{
					writer.WriteString(_T(" dateAxis=\""));
                    writer.WriteString(m_oDateAxis->ToString3(SimpleTypes::onofftostring1));
					writer.WriteString(_T("\""));
				}
				if(m_oDisplayEmptyCellsAs.IsInit())
				{
					writer.WriteString(_T(" displayEmptyCellsAs=\""));
					std::wstring sVal;
					ToXml_ST_DispBlanksAs(m_oDisplayEmptyCellsAs.get2(), sVal);
					writer.WriteString(sVal);
					writer.WriteString(_T("\""));
				}
				if(m_oMarkers.IsInit())
				{
					writer.WriteString(_T(" markers=\""));
                    writer.WriteString(m_oMarkers->ToString3(SimpleTypes::onofftostring1));
					writer.WriteString(_T("\""));
				}
				if(m_oHigh.IsInit())
				{
					writer.WriteString(_T(" high=\""));
                    writer.WriteString(m_oHigh->ToString3(SimpleTypes::onofftostring1));
					writer.WriteString(_T("\""));
				}
				if(m_oLow.IsInit())
				{
					writer.WriteString(_T(" low=\""));
                    writer.WriteString(m_oLow->ToString3(SimpleTypes::onofftostring1));
					writer.WriteString(_T("\""));
				}
				if(m_oFirst.IsInit())
				{
					writer.WriteString(_T(" first=\""));
                    writer.WriteString(m_oFirst->ToString3(SimpleTypes::onofftostring1));
					writer.WriteString(_T("\""));
				}
				if(m_oLast.IsInit())
				{
					writer.WriteString(_T(" last=\""));
                    writer.WriteString(m_oLast->ToString3(SimpleTypes::onofftostring1));
					writer.WriteString(_T("\""));
				}
				if(m_oNegative.IsInit())
				{
					writer.WriteString(_T(" negative=\""));
                    writer.WriteString(m_oNegative->ToString3(SimpleTypes::onofftostring1));
					writer.WriteString(_T("\""));
				}
				if(m_oDisplayXAxis.IsInit())
				{
					writer.WriteString(_T(" displayXAxis=\""));
                    writer.WriteString(m_oDisplayXAxis->ToString3(SimpleTypes::onofftostring1));
					writer.WriteString(_T("\""));
				}
				if(m_oDisplayHidden.IsInit())
				{
					writer.WriteString(_T(" displayHidden=\""));
                    writer.WriteString(m_oDisplayHidden->ToString3(SimpleTypes::onofftostring1));
					writer.WriteString(_T("\""));
				}
				if(m_oMinAxisType.IsInit())
				{
					writer.WriteString(_T(" minAxisType=\""));
					writer.WriteString(m_oMinAxisType->ToString());
					writer.WriteString(_T("\""));
				}
				if(m_oMaxAxisType.IsInit())
				{
					writer.WriteString(_T(" maxAxisType=\""));
					writer.WriteString(m_oMaxAxisType->ToString());
					writer.WriteString(_T("\""));
				}
				if(m_oRightToLeft.IsInit())
				{
					writer.WriteString(_T(" rightToLeft=\""));
                    writer.WriteString(m_oRightToLeft->ToString3(SimpleTypes::onofftostring1));
					writer.WriteString(_T("\""));
				}

				writer.WriteString(_T(">"));
				if(m_oColorSeries.IsInit())
				{
					m_oColorSeries->toXML2(writer, _T("x14:colorSeries"));
				}
				if(m_oColorNegative.IsInit())
				{
                    m_oColorNegative->toXML2(writer, _T("x14:colorNegative"));
				}
				if(m_oColorAxis.IsInit())
				{
                    m_oColorAxis->toXML2(writer, _T("x14:colorAxis"));
				}
				if(m_oColorMarkers.IsInit())
				{
                    m_oColorMarkers->toXML2(writer, _T("x14:colorMarkers"));
				}
				if(m_oColorFirst.IsInit())
				{
                    m_oColorFirst->toXML2(writer, _T("x14:colorFirst"));
				}
				if(m_oColorLast.IsInit())
				{
                    m_oColorLast->toXML2(writer, _T("x14:colorLast"));
				}
				if(m_oColorHigh.IsInit())
				{
                    m_oColorHigh->toXML2(writer, _T("x14:colorHigh"));
				}
				if(m_oColorLow.IsInit())
				{
                    m_oColorLow->toXML2(writer, _T("x14:colorLow"));
				}
				if(m_oRef.IsInit())
				{
					writer.WriteString(_T("<xm:f>"));
					writer.WriteString(m_oRef.get());
					writer.WriteString(_T("</xm:f>"));
				}
				if(m_oSparklines.IsInit())
				{
					m_oSparklines->toXML(writer);
				}
				writer.WriteString(_T("</x14:sparklineGroup>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

                    if ( _T("colorSeries") == sName )
						m_oColorSeries = oReader;
                    else if ( _T("colorNegative") == sName )
						m_oColorNegative = oReader;
                    else if ( _T("colorAxis") == sName )
						m_oColorAxis = oReader;
                    else if ( _T("colorMarkers") == sName )
						m_oColorMarkers = oReader;
                    else if ( _T("colorFirst") == sName )
						m_oColorFirst = oReader;
                    else if ( _T("colorLast") == sName )
						m_oColorLast = oReader;
                    else if ( _T("colorHigh") == sName )
						m_oColorHigh = oReader;
                    else if ( _T("colorLow") == sName )
						m_oColorLow = oReader;
                    else if ( _T("f") == sName )
                        m_oRef = oReader.GetText3();
                    else if ( _T("sparklines") == sName )
						m_oSparklines = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_SparklineGroup;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("manualMax"),      m_oManualMax )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("manualMin"),      m_oManualMin )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("lineWeight"),      m_oLineWeight )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("type"),      m_oType )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("dateAxis"),      m_oDateAxis )
						else if(_T("displayEmptyCellsAs") == wsName)
						{
							ST_DispBlanksAs eVal;
							std::wstring sNodeName = oReader.GetText();
							if(FromXml_ST_DispBlanksAs(sNodeName, eVal))
							{
								m_oDisplayEmptyCellsAs.Init();
								m_oDisplayEmptyCellsAs.get2() = eVal;
							}
						}
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("markers"),      m_oMarkers )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("high"),      m_oHigh )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("low"),      m_oLow )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("first"),      m_oFirst )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("last"),      m_oLast )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("negative"),      m_oNegative )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("displayXAxis"),      m_oDisplayXAxis )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("displayHidden"),      m_oDisplayHidden )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("minAxisType"),      m_oMinAxisType )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("maxAxisType"),      m_oMaxAxisType )
					WritingElement_ReadAttributes_Read_else_if     ( oReader, _T("rightToLeft"),      m_oRightToLeft )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:

				nullable<SimpleTypes::CDouble>						m_oManualMax;
				nullable<SimpleTypes::CDouble>						m_oManualMin;
				nullable<SimpleTypes::CDouble>						m_oLineWeight;
                nullable<SimpleTypes::Spreadsheet::ST_SparklineType<>>			m_oType;
				nullable<SimpleTypes::COnOff<>>							m_oDateAxis;
				nullable<OOX::Spreadsheet::ST_DispBlanksAs>				m_oDisplayEmptyCellsAs;
				nullable<SimpleTypes::COnOff<>>							m_oMarkers;
				nullable<SimpleTypes::COnOff<>>							m_oHigh;
				nullable<SimpleTypes::COnOff<>>							m_oLow;
				nullable<SimpleTypes::COnOff<>>							m_oFirst;
				nullable<SimpleTypes::COnOff<>>							m_oLast;
				nullable<SimpleTypes::COnOff<>>							m_oNegative;
				nullable<SimpleTypes::COnOff<>>							m_oDisplayXAxis;
				nullable<SimpleTypes::COnOff<>>							m_oDisplayHidden;
                nullable<SimpleTypes::Spreadsheet::ST_SparklineAxisMinMax<>>			m_oMinAxisType;
                nullable<SimpleTypes::Spreadsheet::ST_SparklineAxisMinMax<>>			m_oMaxAxisType;
				nullable<SimpleTypes::COnOff<>>							m_oRightToLeft;

				nullable<OOX::Spreadsheet::CColor>					m_oColorSeries;
				nullable<OOX::Spreadsheet::CColor>					m_oColorNegative;
				nullable<OOX::Spreadsheet::CColor>					m_oColorAxis;
				nullable<OOX::Spreadsheet::CColor>					m_oColorMarkers;
				nullable<OOX::Spreadsheet::CColor>					m_oColorFirst;
				nullable<OOX::Spreadsheet::CColor>					m_oColorLast;
				nullable<OOX::Spreadsheet::CColor>					m_oColorHigh;
				nullable<OOX::Spreadsheet::CColor>					m_oColorLow;
				nullable<std::wstring>								m_oRef;
				nullable<CSparklines>								m_oSparklines;
		};
		class CSparklineGroups  : public WritingElementWithChilds<CSparklineGroup>
		{
		public:
			WritingElement_AdditionConstructors(CSparklineGroups)
			CSparklineGroups()
			{
			}
			virtual ~CSparklineGroups()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<x14:sparklineGroups xmlns:xm=\"http://schemas.microsoft.com/office/excel/2006/main\">"));

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }

				writer.WriteString(_T("</x14:sparklineGroups>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

                    if ( _T("sparklineGroup") == sName )
						m_arrItems.push_back(new CSparklineGroup( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_SparklineGroups;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SPARKLINE_FILE_INCLUDE_H_
