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
#ifndef OOX_CELLANCHOR_FILE_INCLUDE_H_
#define OOX_CELLANCHOR_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "FromTo.h"
#include "Pos.h"

#include "../../../../../ASCOfficePPTXFile/PPTXFormat/Logic/GraphicFrame.h"
#include "../../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Shape.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CCellAnchor : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCellAnchor)
           
			CCellAnchor(const SimpleTypes::Spreadsheet::ECellAnchorType& oAnchorType) :	m_oAnchorType(oAnchorType),
																							m_bShapeOle(false), m_bShapeControl(false)
			{
			}
			CCellAnchor(const SimpleTypes::Spreadsheet::CCellAnchorType<>& oAnchorType) :	m_oAnchorType(oAnchorType),
																							m_bShapeOle(false), m_bShapeControl(false)
			{
			}
			virtual ~CCellAnchor()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_bShapeOle = false;
				m_bShapeControl = false;
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(isValid() == false)	return;				
				if(m_bShapeOle)			return;

				if(m_oAlternateContent.IsInit() && m_oAlternateContent->ToBool())
				{
					writer.WriteString(L"<mc:AlternateContent xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\"><mc:Choice xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" Requires=\"a14\">");
				}
				std::wstring sEnd;
				if(m_oFrom.IsInit() && m_oTo.IsInit())
				{
					sEnd	= L"</xdr:twoCellAnchor>";
					
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
					sEnd	= L"</xdr:oneCellAnchor>";
					
					writer.WriteString(L"<xdr:oneCellAnchor>");
					if(m_oFrom.IsInit())
						m_oFrom->toXML2(writer, L"xdr:from");
					if(m_oExt.IsInit())
						m_oExt->toXML(writer);
				}
				else
				{
					sEnd	= L"</xdr:absoluteAnchor>";
					
					writer.WriteString(L"<xdr:absoluteAnchor>");
					if(m_oPos.IsInit())
					m_oPos->toXML(writer);
					if(m_oExt.IsInit())
						m_oExt->toXML(writer);
				}
				if(m_oElement.IsInit())
				{
					NSBinPptxRW::CXmlWriter oXmlWriter(XMLWRITER_DOC_TYPE_XLSX);

					m_oElement->toXmlWriter(&oXmlWriter);
					writer.WriteString(oXmlWriter.GetXmlString());	
				}
				writer.WriteString(L"<xdr:clientData/>");
				writer.WriteString(sEnd);

				if(m_oAlternateContent.IsInit() && m_oAlternateContent->ToBool())
				{
					writer.WriteString(L"</mc:Choice><mc:Fallback/></mc:AlternateContent>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

					if ( L"from" == sName )
						m_oFrom = oReader;
					else if ( L"to" == sName )
						m_oTo = oReader;
					else if ( L"pos" == sName )
						m_oPos = oReader;
					else if ( L"ext" == sName )
						m_oExt = oReader;
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

			virtual EElementType getType () const
			{
				return et_x_CellAnchor;
			}

			virtual void setAnchorType (SimpleTypes::Spreadsheet::ECellAnchorType eType)
			{
				m_oAnchorType.SetValue(eType);
			}
			virtual SimpleTypes::Spreadsheet::ECellAnchorType getAnchorType () const
			{
				return m_oAnchorType.GetValue();
			}
			bool isValid() const
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
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("editAs"), m_oEditAs )
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributesRequire(XmlUtils::CXmlLiteReader& oReader, std::wstring& sRequire)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("Requires"), sRequire )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			bool											m_bShapeOle;
			bool											m_bShapeControl;

			SimpleTypes::Spreadsheet::CCellAnchorType<>		m_oAnchorType;
			nullable<SimpleTypes::Spreadsheet::CCellAnchorType<>> m_oEditAs;
			nullable<OOX::Spreadsheet::CFromTo>				m_oFrom;
			nullable<OOX::Spreadsheet::CFromTo>				m_oTo;
			nullable<OOX::Spreadsheet::CPos>				m_oPos;
			nullable<OOX::Spreadsheet::CExt>				m_oExt;

			nullable<PPTX::Logic::SpTreeElem>				m_oElement;

			nullable<SimpleTypes::COnOff<>>					m_oAlternateContent;

		//для удобства
			nullable<std::wstring>							m_sVmlSpId;
			nullable<int>									m_nId; 
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CELLANCHOR_FILE_INCLUDE_H_
