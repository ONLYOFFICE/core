/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "Sparkline.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

#include "../../XlsbFormat/Biff12_unions/SPARKLINEGROUPS.h"
#include "../../XlsbFormat/Biff12_unions/SPARKLINEGROUP.h"
#include "../../XlsbFormat/Biff12_records/BeginSparklineGroup.h"
#include "../../XlsbFormat/Biff12_records/Sparkline.h"
#include "../../XlsbFormat/Biff12_records/FRTBegin.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CSparkline::CSparkline()
		{
		}
		CSparkline::~CSparkline()
		{
		}
		void CSparkline::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSparkline::toXML() const
		{
			return _T("");
		}
		void CSparkline::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<x14:sparkline>"));
			if(m_oRef.IsInit())
			{
				writer.WriteString(_T("<xm:f>"));
				writer.WriteEncodeXmlString(m_oRef.get());
				writer.WriteString(_T("</xm:f>"));
			}
			if(m_oSqRef.IsInit())
			{
				writer.WriteString(_T("<xm:sqref>"));
				writer.WriteEncodeXmlString(m_oSqRef.get());
				writer.WriteString(_T("</xm:sqref>"));
			}
			writer.WriteString(_T("</x14:sparkline>"));
		}
		void CSparkline::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		XLS::BaseObjectPtr CSparkline::toBin()
		{
			auto ptr(new XLSB::Sparkline);
			XLS::BaseObjectPtr objectPtr(ptr);
			ptr->FRTheader.fFormula = false;
			ptr->FRTheader.fRef = false;
			ptr->FRTheader.fRelID = false;
			ptr->FRTheader.fSqref = false;

			if(m_oRef.IsInit())
			{
				XLSB::FRTFormula formula;
				formula.formula = m_oRef.get();
				ptr->FRTheader.rgFormulas.array.push_back(formula);
				ptr->FRTheader.fFormula = true;
			}
			if(m_oSqRef.IsInit())
			{
				XLSB::FRTSqref sqref;
				sqref.fAdjDelete = false;
				sqref.fDoAdjust = false;
				sqref.fAdjChange = false;
				sqref.fEdit = false;
				sqref.sqrfx.strValue = m_oSqRef.get();
				ptr->FRTheader.rgSqrefs.array.push_back(sqref);
				ptr->FRTheader.fSqref = true;
			}

			return objectPtr;
		}
		void CSparkline::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CSparkline::getType () const
		{
			return et_x_Sparkline;
		}
		void CSparkline::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::Sparkline*>(obj.get());
			if(ptr != nullptr)
			{
				if(ptr->FRTheader.rgFormulas.array.size() > 0)
				{
					m_oRef = ptr->FRTheader.rgFormulas.array[0].formula.getAssembledFormula();
				}

				if(ptr->FRTheader.fSqref && !ptr->FRTheader.rgSqrefs.array.empty())
					m_oSqRef = ptr->FRTheader.rgSqrefs.array[0].sqrfx.strValue;

			}
		}
		void CSparkline::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		CSparklines::CSparklines()
		{
		}
		CSparklines::~CSparklines()
		{
		}
		void CSparklines::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSparklines::toXML() const
		{
			return _T("");
		}
		void CSparklines::toXML(NSStringUtils::CStringBuilder& writer) const
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
		void CSparklines::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("sparkline") == sName )
				{
					CSparkline* pSparkline = new CSparkline();
					*pSparkline = oReader;
					m_arrItems.push_back(pSparkline);
				}
			}
		}
		void CSparklines::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::SPARKLINEGROUP*>(obj.get());
			if(ptr != nullptr)
			{
				for(auto &sparkline : ptr->m_arBrtSparkline)
					m_arrItems.push_back(new CSparkline(sparkline));
			}
		}
		std::vector<XLS::BaseObjectPtr> CSparklines::toBin()
		{
			std::vector<XLS::BaseObjectPtr> sparclineVector;

			for(auto i:m_arrItems)
			{
				sparclineVector.push_back(i->toBin());
			}

			return sparclineVector; 
		}
		EElementType CSparklines::getType () const
		{
			return et_x_Sparklines;
		}
		void CSparklines::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

		CSparklineGroup::CSparklineGroup()
		{
		}
		CSparklineGroup::~CSparklineGroup()
		{
		}
		void CSparklineGroup::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSparklineGroup::toXML() const
		{
			return _T("");
		}
		void CSparklineGroup::toXML(NSStringUtils::CStringBuilder& writer) const
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
		void CSparklineGroup::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		void CSparklineGroup::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::SPARKLINEGROUP*>(obj.get());
			if(ptr != nullptr)
			{
				ReadAttributes(ptr->m_BrtBeginSparklineGroup);

				m_oSparklines = obj;
			}
		}
		XLS::BaseObjectPtr CSparklineGroup::toBin()
		{
			auto ptr1(new XLSB::SPARKLINEGROUP);
			XLS::BaseObjectPtr objectPtr(ptr1);

			auto ptr(new XLSB::BeginSparklineGroup);
			ptr1->m_BrtBeginSparklineGroup = XLS::BaseObjectPtr{ptr};

            if(m_oManualMax.IsInit())
                ptr->dManualMax.data.value = m_oManualMax->GetValue();
            else
                ptr->dManualMax.data.value = 0;
            if(m_oManualMin.IsInit())
                ptr->dManualMin.data.value = m_oManualMin->GetValue();
            else
                ptr->dManualMin.data.value = 0;
            if(m_oLineWeight.IsInit())
                ptr->dLineWeight.data.value = m_oLineWeight->GetValue();
            else
                ptr->dLineWeight.data.value = 0;
            if(m_oType.IsInit())
                ptr->isltype = static_cast<decltype(ptr->isltype)>(m_oType->GetValue());
            else
                ptr->isltype = 0;
            if(m_oDateAxis.IsInit())
                ptr->fDateAxis = m_oDateAxis->GetValue();
            else
                ptr->fDateAxis = 0;
            if(m_oDisplayEmptyCellsAs.IsInit())
			{
				switch(m_oDisplayEmptyCellsAs.get())
				{
					case st_dispblanksasZERO:
						ptr->fShowEmptyCellAsZero = 0;
						break;
					case st_dispblanksasSPAN:
						ptr->fShowEmptyCellAsZero = 2;
						break;
					case st_dispblanksasGAP:
					default:
						ptr->fShowEmptyCellAsZero = 1;
						break;
				}
			}
            if(m_oMarkers.IsInit())
                ptr->fMarkers = m_oMarkers->GetValue();
            else
                ptr->fMarkers = false;
            if(m_oHigh.IsInit())
                ptr->fHigh = m_oHigh->GetValue();
            else
                ptr->fHigh = false;
            if(m_oLow.IsInit())
                ptr->fLow = m_oLow->GetValue();
            else
                ptr->fLow = false;
            if(m_oFirst.IsInit())
                ptr->fFirst = m_oFirst->GetValue();
            else
                ptr->fFirst = false;
            if(m_oLast.IsInit())
                ptr->fLast = m_oLast->GetValue();
            else
                ptr->fLast = false;
            if(m_oNegative.IsInit())
                ptr->fNegative = m_oNegative->GetValue();
            else
                ptr->fNegative = false;
            if(m_oDisplayXAxis.IsInit())
                ptr->fAxis = m_oDisplayXAxis->GetValue();
            else
                ptr->fAxis = false;
            if(m_oDisplayHidden.IsInit())
                ptr->fDisplayHidden = m_oDisplayHidden->GetValue();
            else
                ptr->fDisplayHidden = false;
            if(m_oRightToLeft.IsInit())
                ptr->fRTL = m_oRightToLeft->GetValue();
            else
                ptr->fRTL = false;
			if(m_oMaxAxisType.IsInit())
			{
				
				if(m_oMaxAxisType.get()== SimpleTypes::Spreadsheet::ESparklineAxisMinMax::Group)
				{
					ptr->fIndividualAutoMax = false;
					ptr->fGroupAutoMax = true;
				}
				else 
				{
					ptr->fIndividualAutoMax = true;
					ptr->fGroupAutoMax = false;
				}
			}
			else
			{
				ptr->fIndividualAutoMax = true;
				ptr->fGroupAutoMax = false;
			}
			if(m_oMinAxisType.IsInit())
			{
				if(m_oMinAxisType.get()== SimpleTypes::Spreadsheet::ESparklineAxisMinMax::Group)
				{
					ptr->fIndividualAutoMin = false;
					ptr->fGroupAutoMin = true;
				}
				else
				{
					ptr->fIndividualAutoMin = true;
					ptr->fGroupAutoMin = false;
				}
			}
			else
			{
				ptr->fIndividualAutoMin = true;
				ptr->fGroupAutoMin = false;
			}

            if(m_oColorSeries.IsInit())
                ptr->brtcolorSeries =  m_oColorSeries->toColor();
            else
                ptr->brtcolorSeries = m_oColorSeries->GetDefaultColor();

            if(m_oColorNegative.IsInit())
                ptr->brtcolorNegative = m_oColorNegative->toColor();
            else
                ptr->brtcolorNegative = m_oColorSeries->GetDefaultColor();

            if(m_oColorAxis.IsInit())
                ptr->brtcolorAxis = m_oColorAxis->toColor();
            else
                ptr->brtcolorAxis = m_oColorSeries->GetDefaultColor();

            if(m_oColorMarkers.IsInit())
                ptr->brtcolorMarkers = m_oColorMarkers->toColor();
            else
                ptr->brtcolorMarkers = m_oColorSeries->GetDefaultColor();

            if(m_oColorFirst.IsInit())
                ptr->brtcolorFirst = m_oColorFirst->toColor();
            else
                ptr->brtcolorFirst = m_oColorSeries->GetDefaultColor();

            if(m_oColorLast.IsInit())
                ptr->brtcolorLast = m_oColorLast->toColor();
            else
                ptr->brtcolorLast = m_oColorSeries->GetDefaultColor();

            if(m_oColorHigh.IsInit())
                ptr->brtcolorHigh = m_oColorHigh->toColor();
            else
                ptr->brtcolorHigh = m_oColorHigh->GetDefaultColor();

            if(m_oColorLow.IsInit())
                ptr->brtcolorLow = m_oColorLow->toColor();
            else
                ptr->brtcolorLow = m_oColorLow->GetDefaultColor();

			ptr->FRTheader.fFormula = false;
			ptr->FRTheader.fRef = false;
			ptr->FRTheader.fSqref = false;
			ptr->FRTheader.fRelID = false;
            if(m_oRef.IsInit())
            {
				XLSB::FRTFormula fmla;
				ptr->FRTheader.rgFormulas.array.push_back(fmla);
                ptr->FRTheader.rgFormulas.array[0].formula = m_oRef.get();
				ptr->FRTheader.fFormula = true;
            }
            for(auto i:m_oSparklines->m_arrItems)
            {
                ptr1->m_arBrtSparkline.push_back(i->toBin());
            }

			return objectPtr;
		}
		EElementType CSparklineGroup::getType () const
		{
			return et_x_SparklineGroup;
		}
		void CSparklineGroup::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BeginSparklineGroup*>(obj.get());
			if(ptr != nullptr)
			{
				m_oManualMax            = ptr->dManualMax.data.value;
				m_oManualMin            = ptr->dManualMin.data.value;
				m_oLineWeight           = ptr->dLineWeight.data.value;
				m_oType                 = (SimpleTypes::Spreadsheet::ESparklineType)ptr->isltype;
				m_oDateAxis             = ptr->fDateAxis;
				if(ptr->fShowEmptyCellAsZero == 0x01)
					m_oDisplayEmptyCellsAs  = OOX::Spreadsheet::ST_DispBlanksAs::st_dispblanksasZERO;
				else
					m_oDisplayEmptyCellsAs  = OOX::Spreadsheet::ST_DispBlanksAs::st_dispblanksasGAP;
				m_oMarkers              = ptr->fMarkers;
				m_oHigh                 = ptr->fHigh;
				m_oLow                  = ptr->fLow;
				m_oFirst                = ptr->fFirst;
				m_oLast                 = ptr->fLast;
				m_oNegative             = ptr->fNegative;
				m_oDisplayXAxis         = ptr->fAxis;
				m_oDisplayHidden        = ptr->fDisplayHidden;
				m_oRightToLeft          = ptr->fRTL;

				if(ptr->fIndividualAutoMax)
					m_oMaxAxisType      = SimpleTypes::Spreadsheet::ESparklineAxisMinMax::Individual;
				else if(ptr->fGroupAutoMax)
					m_oMaxAxisType      = SimpleTypes::Spreadsheet::ESparklineAxisMinMax::Group;
				else
					m_oMaxAxisType      = SimpleTypes::Spreadsheet::ESparklineAxisMinMax::Custom;

				if(ptr->fIndividualAutoMin)
					m_oMinAxisType      = SimpleTypes::Spreadsheet::ESparklineAxisMinMax::Individual;
				else if(ptr->fGroupAutoMin)
					m_oMinAxisType      = SimpleTypes::Spreadsheet::ESparklineAxisMinMax::Group;
				else
					m_oMinAxisType      = SimpleTypes::Spreadsheet::ESparklineAxisMinMax::Custom;

				m_oColorSeries.Init();
				m_oColorSeries->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorSeries));

				m_oColorNegative.Init();
				m_oColorNegative->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorNegative));

				m_oColorAxis.Init();
				m_oColorAxis->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorAxis));

				m_oColorMarkers.Init();
				m_oColorMarkers->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorMarkers));

				m_oColorFirst.Init();
				m_oColorFirst->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorFirst));

				m_oColorLast.Init();
				m_oColorLast->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorLast));

				m_oColorHigh.Init();
				m_oColorHigh->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorHigh));

				m_oColorLow.Init();
				m_oColorLow->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtcolorLow));

				if(ptr->FRTheader.rgFormulas.array.size() > 0)
				{
					m_oRef = ptr->FRTheader.rgFormulas.array[0].formula.getAssembledFormula();
				}

			}
		}
		void CSparklineGroup::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("manualMax"),	m_oManualMax )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("manualMin"),	m_oManualMin )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("lineWeight"),m_oLineWeight )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("type"),		m_oType )
				WritingElement_ReadAttributes_Read_else_if  (oReader,  _T("xr2:uid"),	m_oUId )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("dateAxis"),	m_oDateAxis )
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
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("markers"),	m_oMarkers )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("high"),		m_oHigh )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("low"),		m_oLow )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("first"),		m_oFirst )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("last"),		m_oLast )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("negative"),	m_oNegative )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("displayXAxis"),	m_oDisplayXAxis )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("displayHidden"),	m_oDisplayHidden )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("minAxisType"),	m_oMinAxisType )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("maxAxisType"),	m_oMaxAxisType )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rightToLeft"),	m_oRightToLeft )

			WritingElement_ReadAttributes_End( oReader )
		}

		CSparklineGroups::CSparklineGroups()
		{
		}
		CSparklineGroups::~CSparklineGroups()
		{
		}
		void CSparklineGroups::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSparklineGroups::toXML() const
		{
			return _T("");
		}
		void CSparklineGroups::toXML(NSStringUtils::CStringBuilder& writer) const
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
		void CSparklineGroups::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("sparklineGroup") == sName )
				{
					CSparklineGroup* pSparklineGroup = new CSparklineGroup();
					*pSparklineGroup = oReader;
					m_arrItems.push_back(pSparklineGroup);
				}
			}
		}
		void CSparklineGroups::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::SPARKLINEGROUPS*>(obj.get());
			if(ptr != nullptr)
			{
				for(auto &sparklineGroup : ptr->m_arSPARKLINEGROUP)
					m_arrItems.push_back(new CSparklineGroup(sparklineGroup));
			}
		}
		XLS::BaseObjectPtr CSparklineGroups::toBin()
		{
			auto ptr(new XLSB::SPARKLINEGROUPS);
			XLS::BaseObjectPtr objectPtr(ptr);
            auto sparklineVersion(new XLSB::FRTBegin);
            sparklineVersion->productVersion.product = 0;
            sparklineVersion->productVersion.version = 0;
            ptr->m_BrtFRTBegin = XLS::BaseObjectPtr{sparklineVersion};
			for(auto i:m_arrItems)
				ptr->m_arSPARKLINEGROUP.push_back(i->toBin());
			return objectPtr;
		}
		EElementType CSparklineGroups::getType () const
		{
			return et_x_SparklineGroups;
		}
		void CSparklineGroups::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

	} //Spreadsheet
} // namespace OOX
