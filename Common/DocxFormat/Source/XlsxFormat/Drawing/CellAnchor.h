#pragma once
#ifndef OOX_CELLANCHOR_FILE_INCLUDE_H_
#define OOX_CELLANCHOR_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "FromTo.h"
#include "Pic.h"
#include "GraphicFrame.h"
#include "Pos.h"
#include "Shape.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CCellAnchor : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CCellAnchor)
			CCellAnchor(SimpleTypes::Spreadsheet::CCellAnchorType<>& oAnchorType = SimpleTypes::Spreadsheet::CCellAnchorType<>()):m_oAnchorType(oAnchorType)
			{
			}
			virtual ~CCellAnchor()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(isValid())
				{
					CString sStart;
					CString sEnd;
					if(m_oFrom.IsInit() && m_oTo.IsInit())
					{
						sStart.Format(_T("<xdr:twoCellAnchor editAs=\"%s\">"), m_oAnchorType.ToString());
						sEnd = _T("</xdr:twoCellAnchor>");
						writer.WriteString(sStart);
						if(m_oFrom.IsInit())
							m_oFrom->toXML2(writer, _T("xdr:from"));
						if(m_oTo.IsInit())
							m_oTo->toXML2(writer, _T("xdr:to"));
					}
					else if(m_oFrom.IsInit() && m_oExt.IsInit())
					{
						sStart.Append(_T("<xdr:oneCellAnchor>"));
						sEnd = _T("</xdr:oneCellAnchor>");
						writer.WriteString(sStart);
						if(m_oFrom.IsInit())
							m_oFrom->toXML2(writer, _T("xdr:from"));
						if(m_oExt.IsInit())
							m_oExt->toXML(writer);
					}
					else if(m_oPos.IsInit() && m_oExt.IsInit())
					{
						sStart.Append(_T("<xdr:absoluteAnchor>"));
						sEnd = _T("</xdr:absoluteAnchor>");
						writer.WriteString(sStart);
						if(m_oPos.IsInit())
						m_oPos->toXML(writer);
						if(m_oExt.IsInit())
							m_oExt->toXML(writer);
					}
					else
						return;
					if(m_oXml.IsInit())
						writer.WriteString(m_oXml.get());	
					if(m_oGraphicFrame.IsInit())
						m_oGraphicFrame->toXML(writer);
					writer.WriteString(sEnd);	
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("xdr:from") == sName )
						m_oFrom = oReader;
					else if ( _T("xdr:to") == sName )
						m_oTo = oReader;
					else if ( _T("xdr:pos") == sName )
						m_oPos = oReader;
					else if ( _T("xdr:ext") == sName )
						m_oExt = oReader;
					else if ( _T("xdr:graphicFrame") == sName )
						m_oGraphicFrame = oReader;
	//Так читать правильнее ... но для совместимости нужно хранить и все xml !!!!
					//else if (_T("xdr:pic") == sName )
					//	m_oPicture = oReader;
					//else if (_T("xdr:sp") == sName)
					//	m_oShape = oReader;
					//else if (_T("xdr:cxnSp") == sName)
					//	m_oConnShape = oReader;
					//else if (_T("xdr:grpSp") == sName || _T("mc:AlternateContent") == sName)
					//	m_oXml = oReader.GetOuterXml();
/////////////////////////////////
					//
					else if ( _T("xdr:pic") == sName || _T("xdr:sp") == sName || _T("xdr:grpSp") == sName || _T("xdr:cxnSp") == sName || _T("mc:AlternateContent") == sName)
					{			
						m_oXml = oReader.GetOuterXml();
						{
							XmlUtils::CXmlLiteReader oShapeReader;
							//сформируем полноценную xml-строку
							CString xmlString = L"<?xml version=\"1.0\" encoding=\"UTF-16\"?>";
							xmlString += L"<xdr:cellAnchor ";
								xmlString += L"xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" ";
								xmlString += L"xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" ";	
								xmlString += L"xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\" ";
								xmlString += L"xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" ";
							xmlString += L">";
							xmlString += *m_oXml;
							xmlString += L"</xdr:cellAnchor>";
							BOOL result =oShapeReader.FromString(xmlString);

							result = oShapeReader.ReadNextNode();//root ... skiping
							result = oShapeReader.ReadNextNode();

							CWCharWrapper sName = oShapeReader.GetName();
							
							if (_T("xdr:pic") == sName)
								m_oPicture = oShapeReader;
							else if (_T("xdr:sp") == sName)
								m_oShape = oShapeReader;
							else if (_T("xdr:cxnSp") == sName)
								m_oConnShape = oShapeReader;
							else if (_T("xdr:grpSp") == sName)
								m_oGroupShape = oShapeReader;
						}
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_CellAnchor;
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
				if(false == m_oXml.IsInit() && false == m_oGraphicFrame.IsInit())
					return false;
				return true;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			SimpleTypes::Spreadsheet::CCellAnchorType<>		m_oAnchorType;
			nullable<OOX::Spreadsheet::CFromTo>				m_oFrom;
			nullable<OOX::Spreadsheet::CFromTo>				m_oTo;
			nullable<OOX::Spreadsheet::CPos>				m_oPos;
			nullable<OOX::Spreadsheet::CExt>				m_oExt;
			nullable<OOX::Spreadsheet::CGraphicFrame>		m_oGraphicFrame;
			nullable<OOX::Spreadsheet::CPic>				m_oPicture;
			nullable<OOX::Spreadsheet::CGroupShape>			m_oGroupShape;
			nullable<OOX::Spreadsheet::CShape>				m_oShape;
			nullable<OOX::Spreadsheet::CConnShape>			m_oConnShape;
			nullable<CString>								m_oXml;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CELLANCHOR_FILE_INCLUDE_H_