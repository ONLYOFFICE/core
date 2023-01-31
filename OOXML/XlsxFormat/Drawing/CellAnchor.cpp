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

#include "CellAnchor.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CClientData::CClientData()
		{
		}
		CClientData::~CClientData()
		{
		}
		void CClientData::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CClientData::toXML() const
		{
			return L"";
		}
		void CClientData::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<xdr:clientData");
			WritingStringNullableAttrBool2(L"fLocksWithSheet", fLocksWithSheet);
			WritingStringNullableAttrBool2(L"fPrintsWithSheet", fPrintsWithSheet);
			writer.WriteString(L"/>");
		}
		void CClientData::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		EElementType CClientData::getType() const
		{
			return et_x_ClientData;
		}
		void CClientData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"fLocksWithSheet", fLocksWithSheet)
					WritingElement_ReadAttributes_Read_if(oReader, L"fPrintsWithSheet", fPrintsWithSheet)
				WritingElement_ReadAttributes_End(oReader)
			}


		CCellAnchor::CCellAnchor(const SimpleTypes::Spreadsheet::ECellAnchorType& oAnchorType) :	m_oAnchorType(oAnchorType),
																						m_bShapeOle(false), m_bShapeControl(false)
		{
		}
		CCellAnchor::CCellAnchor(const SimpleTypes::Spreadsheet::CCellAnchorType& oAnchorType) :	m_oAnchorType(oAnchorType),
																						m_bShapeOle(false), m_bShapeControl(false)
		{
		}
		CCellAnchor::~CCellAnchor()
		{
		}
		void CCellAnchor::fromXML(XmlUtils::CXmlNode& node)
		{
			m_bShapeOle = false;
			m_bShapeControl = false;
		}
		std::wstring CCellAnchor::toXML() const
		{
			return L"";
		}
		std::wstring CCellAnchor::toVmlXML() const
		{
			std::wstring sAnchor;

			if (m_oFrom.IsInit())
			{
				sAnchor += m_oFrom->m_oCol->ToString() + L",";
				sAnchor += std::to_wstring(m_oFrom->m_oColOff->ToPx()) + L",";
				sAnchor += m_oFrom->m_oRow->ToString() + L",";
				sAnchor += std::to_wstring(m_oFrom->m_oRowOff->ToPx()) + L",";
			}
			if (m_oTo.IsInit())
			{
				sAnchor += m_oTo->m_oCol->ToString() + L",";
				sAnchor += std::to_wstring(m_oTo->m_oColOff->ToPx()) + L",";
				sAnchor += m_oTo->m_oRow->ToString() + L",";
				sAnchor += std::to_wstring(m_oTo->m_oRowOff->ToPx());
			}
			return sAnchor;
		}
		void CCellAnchor::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(isValid() == false)	return;
			if(m_bShapeOle)			return;

			std::wstring sEnd;
			if(m_oFrom.IsInit() && m_oTo.IsInit())
			{
				sEnd = L"</xdr:twoCellAnchor>";

				writer.WriteString(L"<xdr:twoCellAnchor");
				WritingStringNullableAttrString(L"editAs", m_oEditAs, m_oEditAs->ToString());
				writer.WriteString(L">");
				if(m_oFrom.IsInit())
					m_oFrom->toXML2(writer, L"xdr:from");
				if(m_oTo.IsInit())
					m_oTo->toXML2(writer, L"xdr:to");
			}
			else if(m_oFrom.IsInit() && m_oExt.IsInit())
			{
				sEnd = L"</xdr:oneCellAnchor>";

				writer.WriteString(L"<xdr:oneCellAnchor>");
				if(m_oFrom.IsInit())
					m_oFrom->toXML2(writer, L"xdr:from");
				if(m_oExt.IsInit())
					m_oExt->toXML(writer);
			}
			else
			{
				sEnd = L"</xdr:absoluteAnchor>";

				writer.WriteString(L"<xdr:absoluteAnchor>");
				if(m_oPos.IsInit())
				m_oPos->toXML(writer);
				if(m_oExt.IsInit())
					m_oExt->toXML(writer);
			}
			if(m_oElement.IsInit())
			{
				std::wstring sRequires = m_oElement->GetSlicerRequires();
				if(L"a14" == sRequires)
				{
					writer.WriteString(L"<mc:AlternateContent xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\"><mc:Choice xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" Requires=\"a14\">");
				}
				else if(L"sle15" == sRequires)
				{
					writer.WriteString(L"<mc:AlternateContent xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\"><mc:Choice xmlns:sle15=\"http://schemas.microsoft.com/office/drawing/2012/slicer\" Requires=\"sle15\">");
				}
				NSBinPptxRW::CXmlWriter oXmlWriter(XMLWRITER_DOC_TYPE_XLSX);

				m_oElement->toXmlWriter(&oXmlWriter);
				writer.WriteString(oXmlWriter.GetXmlString());

				if(L"a14" == sRequires || L"sle15" == sRequires)
				{
					writer.WriteString(L"</mc:Choice><mc:Fallback xmlns=\"\"><xdr:sp macro=\"\" textlink=\"\"><xdr:nvSpPr><xdr:cNvPr id=\"0\" name=\"\"/><xdr:cNvSpPr><a:spLocks noTextEdit=\"1\"/></xdr:cNvSpPr></xdr:nvSpPr><xdr:spPr><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:solidFill><a:prstClr val=\"white\"/></a:solidFill><a:ln w=\"1\"><a:solidFill><a:prstClr val=\"green\"/></a:solidFill></a:ln></xdr:spPr><xdr:txBody><a:bodyPr vertOverflow=\"clip\" horzOverflow=\"clip\"/><a:lstStyle/><a:p><a:r><a:rPr lang=\"en-US\" sz=\"1100\"/><a:t>This shape represents a table slicer. Table slicers are not supported in this version of Excel.\n\nIf the shape was modified in an earlier version of Excel, or if the workbook was saved in Excel 2007 or earlier, the slicer can't be used.</a:t></a:r></a:p></xdr:txBody></xdr:sp></mc:Fallback></mc:AlternateContent>");
				}
			}
			if (m_oClientData.IsInit())
				m_oClientData->toXML(writer);
			else
				writer.WriteString(L"<xdr:clientData/>");
			writer.WriteString(sEnd);
		}
		void CCellAnchor::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_bShapeOle = false;
			m_bShapeControl = false;

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"from" == sName)
					m_oFrom = oReader;
				else if (L"to" == sName)
					m_oTo = oReader;
				else if (L"pos" == sName)
					m_oPos = oReader;
				else if (L"ext" == sName)
					m_oExt = oReader;
				else if (L"clientData" == sName)
					m_oClientData = oReader;
				else if (L"graphicFrame" == sName || L"pic" == sName || L"sp" == sName || L"grpSp" == sName || L"cxnSp" == sName || L"AlternateContent" == sName)
				{
					//Demo-2010WinterOlympics2.xlsx - AlternateContent
					m_oElement = oReader;

					if (m_oElement.IsInit())
					{
						smart_ptr<PPTX::Logic::Shape> shape = m_oElement->GetElem().smart_dynamic_cast<PPTX::Logic::Shape>();
						if (shape.IsInit())
						{
							m_nId = shape->nvSpPr.cNvPr.id;
							if (shape->nvSpPr.cNvPr.vmlSpid.IsInit())
							{
								//ссылка на объект или шейп в vmlDrawing
								m_sVmlSpId = shape->nvSpPr.cNvPr.vmlSpid.get();
							}
						}
						smart_ptr<PPTX::Logic::GraphicFrame> frame = m_oElement->GetElem().smart_dynamic_cast<PPTX::Logic::GraphicFrame>();
						if ((frame.IsInit()) && (frame->vmlSpid.IsInit()))
						{
							//ссылка на объект или шейп в vmlDrawing
							m_sVmlSpId = frame->vmlSpid.get();
						}
					}
				}
			}
		}
		EElementType CCellAnchor::getType () const
		{
			return et_x_CellAnchor;
		}
		void CCellAnchor::setAnchorType (SimpleTypes::Spreadsheet::ECellAnchorType eType)
		{
			m_oAnchorType.SetValue(eType);
		}
		SimpleTypes::Spreadsheet::ECellAnchorType CCellAnchor::getAnchorType () const
		{
			return m_oAnchorType.GetValue();
		}
		bool CCellAnchor::isValid() const
		{
			SimpleTypes::Spreadsheet::ECellAnchorType eAnchorType = m_oAnchorType.GetValue();
			if(!((m_oFrom.IsInit() && m_oTo.IsInit()) || (m_oFrom.IsInit() && m_oExt.IsInit()) || (m_oPos.IsInit() && m_oExt.IsInit())))
				return false;
			if(false == m_oElement.is_init())
				return false;
			if(false == m_oElement->is_init())
				return false;
			return true;
		}
		void CCellAnchor::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"editAs", m_oEditAs )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CCellAnchor::ReadAttributesRequire(XmlUtils::CXmlLiteReader& oReader, std::wstring& sRequire)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"Requires", sRequire )
				WritingElement_ReadAttributes_End( oReader )
			}

	} //Spreadsheet
} // namespace OOX
