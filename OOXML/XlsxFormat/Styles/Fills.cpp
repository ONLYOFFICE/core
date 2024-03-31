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

#include "Fills.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

#include "../../XlsbFormat/Biff12_records/Fill.h"
#include "../../XlsbFormat/Biff12_records/BeginFills.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BiffStructure.h"

#include "../../XlsbFormat/Biff12_unions/FILLS.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CPatternFill::CPatternFill()
		{
		}
		CPatternFill::~CPatternFill()
		{
		}
		void CPatternFill::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPatternFill::toXML() const
		{
			return L"";
		}
		void CPatternFill::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXMLWithNS(writer, L"", L"patternFill", L"");
		}
		void CPatternFill::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
		{
			writer.StartNodeWithNS(node_ns, node_name);
			writer.StartAttributes();
			WritingStringNullableAttrString(L"patternType", m_oPatternType, m_oPatternType->ToString());
			if(m_oBgColor.IsInit() || m_oFgColor.IsInit())
			{
				writer.EndAttributes();
				if(m_oBgColor.IsInit() && m_oFgColor.IsInit())
				{
					m_oFgColor->toXMLWithNS(writer, child_ns, L"fgColor", child_ns);
					m_oBgColor->toXMLWithNS(writer, child_ns, L"bgColor", child_ns);
				}
				else if(m_oFgColor.IsInit())
				{
					m_oFgColor->toXMLWithNS(writer, child_ns, L"fgColor", child_ns);
				}
				else if(m_oBgColor.IsInit())
				{
					m_oBgColor->toXMLWithNS(writer, child_ns, L"bgColor", child_ns);
				}

				writer.EndNodeWithNS(node_ns, node_name);
			}
			else
				writer.EndAttributesAndNode();
		}
		void CPatternFill::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"bgColor" == sName )
					m_oBgColor = oReader;
				else if ( L"fgColor" == sName )
					m_oFgColor = oReader;
			}
		}
		void CPatternFill::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		void CPatternFill::toBin(XLS::BaseObjectPtr obj)
		{
			auto ptr = static_cast<XLSB::Fill*>(obj.get());
			if(m_oPatternType.IsInit())
			{
				if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeNone)
					ptr->fls = 0x00;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeSolid)
					ptr->fls = 0x01;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeMediumGray)
					ptr->fls = 0x02;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkGray)
					ptr->fls = 0x03;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeLightGray)
					ptr->fls = 0x04;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkHorizontal)
					ptr->fls = 0x05;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkVertical)
					ptr->fls = 0x06;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkDown)
					ptr->fls = 0x07;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkUp)
					ptr->fls = 0x08;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkGrid)
					ptr->fls = 0x09;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkTrellis)
					ptr->fls = 0x0A;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeLightHorizontal)
					ptr->fls = 0x0B;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeLightVertical)
					ptr->fls = 0x0C;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeLightDown)
					ptr->fls = 0x0D;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeLightUp)
					ptr->fls = 0x0E;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeLightGrid)
					ptr->fls = 0x0F;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeLightTrellis)
					ptr->fls = 0x10;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeGray125)
					ptr->fls = 0x11;
				else if (m_oPatternType == SimpleTypes::Spreadsheet::EPatternType::patterntypeGray0625)
					ptr->fls = 0x12;
				else
					ptr->fls = 0x00;
			}
			else
					ptr->fls = 0x00;

			if(m_oBgColor.IsInit())
				ptr->brtColorBack = m_oBgColor->toColor();
            else
            {   m_oBgColor.Init();
                ptr->brtColorBack = m_oBgColor->GetDefaultColor();
            }
			if(m_oFgColor.IsInit())
				ptr->brtColorFore = m_oFgColor->toColor();
            else
            {
                m_oFgColor.Init();
                ptr->brtColorFore = m_oFgColor->GetDefaultColor();
            }
		}
		EElementType CPatternFill::getType () const
		{
			return et_x_PatternFill;
		}
		void CPatternFill::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, L"patternType", m_oPatternType )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CPatternFill::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::Fill*>(obj.get());
				if(ptr != nullptr)
				{
					switch(ptr->fls)
					{
						case 0x00:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeNone; break;
						case 0x01:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeSolid; break;
						case 0x02:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeMediumGray; break;
						case 0x03:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkGray; break;
						case 0x04:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeLightGray; break;
						case 0x05:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkHorizontal; break;
						case 0x06:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkVertical; break;
						case 0x07:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkDown; break;
						case 0x08:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkUp; break;
						case 0x09:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkGrid; break;
						case 0x0A:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeDarkTrellis; break;
						case 0x0B:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeLightHorizontal; break;
						case 0x0C:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeLightVertical; break;
						case 0x0D:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeLightDown; break;
						case 0x0E:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeLightUp; break;
						case 0x0F:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeLightGrid; break;
						case 0x10:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeLightTrellis; break;
						case 0x11:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeGray125; break;
						case 0x12:
							m_oPatternType = SimpleTypes::Spreadsheet::EPatternType::patterntypeGray0625; break;
					}

					m_oBgColor.Init();
					m_oBgColor->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtColorBack));

					m_oFgColor.Init();
					m_oFgColor->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtColorFore));
				}
			}

		CGradientStop::CGradientStop()
		{
		}
		CGradientStop::~CGradientStop()
		{
		}
		void CGradientStop::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CGradientStop::toXML() const
		{
			return L"";
		}
		void CGradientStop::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXMLWithNS(writer, L"", L"stop", L"");
		}
		void CGradientStop::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
		{
			writer.StartNodeWithNS(node_ns, node_name);
			writer.StartAttributes();
			WritingStringNullableAttrDouble(L"position", m_oPosition, m_oPosition->GetValue());
			writer.EndAttributes();
			if (m_oColor.IsInit())
			{
				m_oColor->toXMLWithNS(writer, child_ns, L"color", child_ns);
			}
			writer.EndNodeWithNS(node_ns, node_name);
		}
		void CGradientStop::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"color" == sName )
					m_oColor = oReader;
			}
		}
		void CGradientStop::fromBin(XLS::BiffStructure* obj)
		{
			auto ptr = static_cast<XLSB::GradientStop*>(obj);
			if(ptr != nullptr)
			{
				m_oPosition = ptr->xnumPosition.data.value;
				m_oColor.Init();
				m_oColor->fromBin(dynamic_cast<XLS::BaseObject*>(&ptr->brtColor));
			}
		}
		void CGradientStop::toBin(XLS::BaseObjectPtr obj)
		{
			auto ptr = static_cast<XLSB::Fill*>(obj.get());
			XLSB::GradientStop stop;
			if(m_oPosition.IsInit())
				stop.xnumPosition.data.value = m_oPosition->GetValue();
			else
				stop.xnumPosition.data.value = 0;
			if(m_oColor.IsInit())
			{
				stop.brtColor = m_oColor->toColor();
			}
			ptr->xfillGradientStop.push_back(stop);
		}

		EElementType CGradientStop::getType () const
		{
			return et_x_GradientStop;
		}
		void CGradientStop::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, L"position", m_oPosition )
				WritingElement_ReadAttributes_End( oReader )
			}

		CGradientFill::CGradientFill()
		{
		}
		CGradientFill::~CGradientFill()
		{
		}
		void CGradientFill::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CGradientFill::toXML() const
		{
			return L"";
		}
		void CGradientFill::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXMLWithNS(writer, L"", L"gradientFill", L"");
		}
		void CGradientFill::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
		{
			writer.StartNodeWithNS(node_ns, node_name);
			writer.StartAttributes();
			WritingStringNullableAttrString(L"type", m_oType, m_oType->ToString());
			WritingStringNullableAttrDouble(L"left", m_oLeft, m_oLeft->GetValue());
			WritingStringNullableAttrDouble(L"right", m_oRight, m_oRight->GetValue());
			WritingStringNullableAttrDouble(L"top", m_oTop, m_oTop->GetValue());
			WritingStringNullableAttrDouble(L"bottom", m_oBottom, m_oBottom->GetValue());
			WritingStringNullableAttrDouble(L"degree", m_oDegree, m_oDegree->GetValue());
			writer.EndAttributes();
			for(size_t i = 0; i < m_arrItems.size(); ++i)
			{
				m_arrItems[i]->toXMLWithNS(writer, child_ns, L"stop", child_ns);
			}
			writer.EndNodeWithNS(node_ns, node_name);
		}
		void CGradientFill::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"stop" == sName )
				{
					CGradientStop* pGradientStop = new CGradientStop();
					*pGradientStop = oReader;
					m_arrItems.push_back(pGradientStop);
				}
			}
		}
		void CGradientFill::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);

			auto ptr = static_cast<XLSB::Fill*>(obj.get());
			if(ptr != nullptr)
			{
				for(auto &gradStop : ptr->xfillGradientStop)
				{
					auto ptrGradStop = new CGradientStop();
					auto ptrBiffStruct = dynamic_cast<XLS::BiffStructure*>(&gradStop);
					ptrGradStop->fromBin(ptrBiffStruct);
					m_arrItems.push_back(ptrGradStop);
				}
			}
		}
		void CGradientFill::toBin(XLS::BaseObjectPtr obj)
		{
			auto ptr = static_cast<XLSB::Fill*>(obj.get());

			if(m_oType.IsInit())
				ptr->iGradientType = m_oType->GetValue();
			else
				ptr->iGradientType = 0;
			if(m_oDegree.IsInit())
				ptr->xnumDegree.data.value = m_oDegree->GetValue();
			else
				ptr->xnumDegree.data.value = 0;
			if(m_oLeft.IsInit())
				ptr->xnumFillToLeft.data.value = m_oLeft->GetValue();
			else
				ptr->xnumFillToLeft.data.value = 0;
			if(m_oRight.IsInit())
				ptr->xnumFillToRight.data.value = m_oRight->GetValue();
			else
				ptr->xnumFillToRight.data.value = 0;
			if(m_oTop .IsInit())
				ptr->xnumFillToTop.data.value = m_oTop->GetValue();
			else
				ptr->xnumFillToTop.data.value = 0;
			if(m_oBottom.IsInit())
				ptr->xnumFillToBottom.data.value = m_oBottom->GetValue();
			else
				ptr->xnumFillToBottom.data.value = 0;

			for(auto i:m_arrItems)
			{
				i->toBin(obj);
			}


		}
		EElementType CGradientFill::getType () const
		{
			return et_x_GradientFill;
		}
		void CGradientFill::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"bottom",	m_oBottom )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"degree",	m_oDegree )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"left",	m_oLeft )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"right",	m_oRight )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"top",	m_oTop )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"type",	m_oType )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CGradientFill::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::Fill*>(obj.get());
				if(ptr != nullptr && ptr->fls == 0x28)
				{
					m_oType         = (SimpleTypes::Spreadsheet::EGradientType)ptr->iGradientType;

					m_oDegree       = ptr->xnumDegree.data.value;
					m_oLeft         = ptr->xnumFillToLeft.data.value;
					m_oRight        = ptr->xnumFillToRight.data.value;
					m_oTop          = ptr->xnumFillToTop.data.value;
					m_oBottom       = ptr->xnumFillToBottom.data.value;
				}
			}

		CFill::CFill()
		{
		}
		CFill::~CFill()
		{
		}
		void CFill::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CFill::toXML() const
		{
			return L"";
		}
		void CFill::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXMLWithNS(writer, L"", L"fill", L"");
		}
		void CFill::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
		{
			writer.StartNodeWithNS(node_ns, node_name);
			writer.StartAttributes();
			writer.EndAttributes();
			if (m_oPatternFill.IsInit())
				m_oPatternFill->toXMLWithNS(writer, child_ns, L"patternFill", child_ns);
			if (m_oGradientFill.IsInit())
				m_oGradientFill->toXMLWithNS(writer, child_ns, L"gradientFill", child_ns);
			writer.EndNodeWithNS(node_ns, node_name);
		}
		void CFill::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"gradientFill" == sName )
					m_oGradientFill = oReader;
				else if ( L"patternFill" == sName )
					m_oPatternFill = oReader;
			}
		}
		void CFill::fromBin(XLS::BaseObjectPtr& obj)
		{
			m_oPatternFill  = obj;
			m_oGradientFill = obj;

			if ((m_oGradientFill.IsInit()) && (false == m_oGradientFill->m_oType.IsInit()))
				m_oGradientFill.reset();
		}
		XLS::BaseObjectPtr CFill::toBin()
		{
			auto ptr(new XLSB::Fill);
			XLS::BaseObjectPtr objectPtr(ptr);

			if(m_oPatternFill.IsInit())
			{
				m_oPatternFill->toBin(objectPtr);
			}
            else
            {
                CColor color;
                ptr->fls = 0;
                ptr->brtColorBack = color.GetDefaultColor();
                ptr->brtColorFore = color.GetDefaultColor();
            }

			if(m_oGradientFill.IsInit())
			{
				m_oGradientFill->toBin(objectPtr);
			}
            else
            {
                ptr->iGradientType = 0;
            }
            ptr->cNumStop = 0;
			return objectPtr;
		}
		EElementType CFill::getType () const
		{
			return et_x_Fill;
		}
		void CFill::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0) return;

				nullable_string sColor, sPatternColor, sPattern;

				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ss:PatternColor"), sPatternColor )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("ss:Pattern"), sPattern )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("ss:Color"), sColor )
				WritingElement_ReadAttributes_End( oReader )

				if (sColor.IsInit() == false && sPattern.IsInit() == false && sPatternColor.IsInit() == false) return;

				m_oPatternFill.Init();
				if (sColor.IsInit())
				{
					m_oPatternFill->m_oFgColor.Init(); m_oPatternFill->m_oFgColor->m_oRgb.Init();
					m_oPatternFill->m_oFgColor->m_oRgb->FromString(*sColor);
				}
				if (sPatternColor.IsInit())
				{
					m_oPatternFill->m_oBgColor.Init(); m_oPatternFill->m_oBgColor->m_oRgb.Init();
					m_oPatternFill->m_oBgColor->m_oRgb->FromString(*sPatternColor);
				}
				m_oPatternFill->m_oPatternType.Init();
				if (sPattern.IsInit())
				{
					m_oPatternFill->m_oPatternType->FromString(*sPattern);
				}
				else m_oPatternFill->m_oPatternType->FromString(L"Solid");
			}

		CFills::CFills()
		{
		}
		CFills::~CFills()
		{
		}
		void CFills::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CFills::toXML() const
		{
			return L"";
		}
		void CFills::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<fills");
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			writer.WriteString(L">");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(L"</fills>");
		}
		void CFills::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int index = 0;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"fill" == sName )
				{
					CFill* pFill = new CFill();
					*pFill = oReader;

					m_arrItems.push_back( pFill );
					m_mapFills.insert(std::make_pair(index++, pFill));
				}
			}
		}
		void CFills::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			ReadAttributes(obj);

			int index = 0;

			for(auto &fill : obj)
			{
				CFill *pFill = new CFill(fill);
				m_arrItems.push_back(pFill);
				m_mapFills.insert(std::make_pair(index++, pFill));
			}
		}
		XLS::BaseObjectPtr CFills::toBin()
		{
			auto ptr(new XLSB::FILLS);
			auto ptr1(new XLSB::BeginFills);
			ptr->m_BrtBeginFills = XLS::BaseObjectPtr{ptr1};

			XLS::BaseObjectPtr objectPtr(ptr);
			for(auto i : m_arrItems)
				ptr->m_arBrtFill.push_back(i->toBin());
			ptr1->cfills = ptr->m_arBrtFill.size();
			return objectPtr;

		}
		EElementType CFills::getType () const
		{
			return et_x_Fills;
		}
		void CFills::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, L"count", m_oCount )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CFills::ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
			{
				m_oCount = (_UINT32)obj.size();
			}

	} //Spreadsheet
} // namespace OOX
