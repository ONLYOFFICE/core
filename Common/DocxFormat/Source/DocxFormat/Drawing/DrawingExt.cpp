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
#include "../DocxFlat.h"
#include "../Document.h"

#include "DrawingExt.h"
#include "Drawing.h"
#include "../../XlsxFormat/Worksheets/Sparkline.h"
#include "../../XlsxFormat/Table/Table.h"
#include "../../XlsxFormat/Table/Connections.h"
#include "../Diagram/DiagramData.h"
#include "../../XlsxFormat/Worksheets/ConditionalFormatting.h"
#include "../../XlsxFormat/Worksheets/DataValidation.h"
#include "../../XlsxFormat/Slicer/SlicerCache.h"
#include "../../XlsxFormat/Slicer/SlicerCacheExt.h"
#include "../Comments.h"

namespace OOX
{
	namespace Drawing
	{
		void CWrapSquare::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"distB",	m_oDistB );
			XmlMacroReadAttributeBase(node, L"distL",	m_oDistL );
			XmlMacroReadAttributeBase(node, L"distR",	m_oDistR );
			XmlMacroReadAttributeBase(node, L"distT",	m_oDistT );
			XmlMacroReadAttributeBase(node, L"wrapText",	m_oWrapText );

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

					if ( _T("effectExtents") == sName )
						m_oEffectExtent = oNode;
				}
			}
		}
		void CWrapSquare::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
                std::wstring sName = oReader.GetName();
				if ( _T("wp:effectExtents") == sName )
					m_oEffectExtent = oReader;
			}
		}
		std::wstring CWrapSquare::toXML() const
		{
            std::wstring sResult = _T("<wp:wrapSquare ");
			if(m_oWrapText.IsInit())
				sResult += _T("wrapText=\"") + m_oWrapText->ToString() + _T("\" ");

			if ( m_oDistB.IsInit() ) sResult += _T("distB=\"") + m_oDistB->ToString() + _T("\" ");
			if ( m_oDistL.IsInit() ) sResult += _T("distL=\"") + m_oDistL->ToString() + _T("\" ");
			if ( m_oDistR.IsInit() ) sResult += _T("distR=\"") + m_oDistR->ToString() + _T("\" ");
			if ( m_oDistT.IsInit() ) sResult += _T("distT=\"") + m_oDistT->ToString() + _T("\" ");

			if ( m_oEffectExtent.IsInit() )
				sResult += m_oEffectExtent->toXML();

			sResult += _T("</wp:wrapSquare>");

			return sResult;
		}
		void CWrapSquare::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("distB"),    m_oDistB )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distL"),    m_oDistL )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),    m_oDistR )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"),    m_oDistT )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"), m_oWrapText )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CWrapPath::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"edited",	m_oEdited );

			XmlUtils::CXmlNodes oNodes;
			
			bool bStart = false;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

					if ( _T("start") == sName )
					{
						m_oStart = oNode;
						bStart = true;
					}
					else if ( bStart && _T("lineTo") == sName )
					{
						ComplexTypes::Drawing::CPoint2D *oPoint = new ComplexTypes::Drawing::CPoint2D(oNode);
						if (oPoint) m_arrLineTo.push_back( oPoint );
					}
				}
			}
		}
		void CWrapPath::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();

			bool bStart = false;
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
                std::wstring sName = oReader.GetName();
				if ( _T("wp:start") == sName )
				{
					m_oStart = oReader;
					bStart = true;
				}
				else if ( bStart && _T("wp:lineTo") == sName )
				{
					ComplexTypes::Drawing::CPoint2D *oPoint = new ComplexTypes::Drawing::CPoint2D(oReader);
					if (oPoint) m_arrLineTo.push_back( oPoint );
				}
			}
		}
		std::wstring CWrapPath::toXML() const
		{
            std::wstring sResult = _T("");
            //std::wstring sResult = _T("<wp:wrapPolygon ");
			//
			//if ( m_oEdited.IsInit() ) sResult += _T("edited=\"") + m_oEdited->ToString() + _T("\">");
			//else                      sResult += _T(">");

			//sResult += _T("<wp:start ") +  m_oStart.ToString() + _T("/>");

			//for ( int nIndex = 0; nIndex < m_arrLineTo.GetSize(); nIndex++ )
			//	sResult += _T("<wp:lineTo ") + m_arrLineTo[nIndex].ToString() + _T("/>");

			//sResult += _T("</wp:wrapPolygon>");

			return sResult;
		}
		void CWrapPath::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, _T("edited"), m_oEdited )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CWrapThrough::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"distL",		m_oDistL );
			XmlMacroReadAttributeBase(node, L"distR",		m_oDistR );
			XmlMacroReadAttributeBase(node, L"wrapText",	m_oWrapText );

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

					if ( _T("wrapPolygon") == sName )
					{
						m_oWrapPolygon = oNode;
					}
				}
			}
		}
		void CWrapThrough::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
                std::wstring sName = oReader.GetName();
				if ( _T("wp:wrapPolygon") == sName )
					m_oWrapPolygon = oReader;
			}
		}
		std::wstring CWrapThrough::toXML() const
		{
            std::wstring sResult = _T("<wp:wrapThrough ");
			
			if ( m_oDistL.IsInit()    ) sResult += _T("distL=\"")    + m_oDistL->ToString()    + _T("\" ");
			if ( m_oDistR.IsInit()    ) sResult += _T("distR=\"")    + m_oDistR->ToString()    + _T("\" ");
			if ( m_oWrapText.IsInit() ) sResult += _T("wrapText=\"") + m_oWrapText->ToString() + _T("\" ");
			
			sResult += _T(">");
			if(m_oWrapPolygon.IsInit())
				sResult += m_oWrapPolygon->toXML();
			sResult += _T("</wp:wrapThrough>");

			return sResult;
		}
		void CWrapThrough::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("distL"),    m_oDistL )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),    m_oDistR )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"), m_oWrapText )
			WritingElement_ReadAttributes_End( oReader )
		}

		void CWrapTight::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

					if ( _T("wrapPolygon") == sName )
					{
						m_oWrapPolygon = oNode;
					}
				}
			}
		}
		void CWrapTight::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
                std::wstring sName = oReader.GetName();
				if ( _T("wp:wrapPolygon") == sName )
					m_oWrapPolygon = oReader;
			}
		}
		std::wstring CWrapTight::toXML() const
		{
            std::wstring sResult = _T("<wp:wrapTight ");
			
			if ( m_oDistL.IsInit()    ) sResult += _T("distL=\"")    + m_oDistL->ToString()    + _T("\" ");
			if ( m_oDistR.IsInit()    ) sResult += _T("distR=\"")    + m_oDistR->ToString()    + _T("\" ");
			if ( m_oWrapText.IsInit() ) sResult += _T("wrapText=\"") + m_oWrapText->ToString() + _T("\" ");

			sResult += _T(">");
			if(m_oWrapPolygon.IsInit())
				sResult += m_oWrapPolygon->toXML();
			sResult += _T("</wp:wrapTight>");

			return sResult;
		}
		void CWrapTight::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("distL"),    m_oDistL )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),    m_oDistR )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"), m_oWrapText )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CWrapTopBottom::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"distB",	m_oDistB );
			XmlMacroReadAttributeBase(node, L"distT",	m_oDistT );

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

					if ( _T("effectExtents") == sName )
						m_oEffectExtent = oNode;
				}
			}
		}
		void CWrapTopBottom::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
                std::wstring sName = oReader.GetName();
				if ( _T("wp:effectExtent") == sName )
					m_oEffectExtent = oReader;
			}
		}
		std::wstring CWrapTopBottom::toXML() const
		{
            std::wstring sResult = _T("<wp:wrapTopAndBottom ");
			
			if ( m_oDistB.IsInit() ) sResult += _T("distB=\"") + m_oDistB->ToString() + _T("\" ");
			if ( m_oDistT.IsInit() ) sResult += _T("distT=\"") + m_oDistT->ToString() + _T("\" ");

			sResult += _T(">");
			
			if ( m_oEffectExtent.IsInit() )
				sResult += m_oEffectExtent->toXML();
			
			sResult += _T("</wp:wrapTopAndBottom>");

			return sResult;
		}
		void CWrapTopBottom::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("distB"), m_oDistB )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"), m_oDistT )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CAnchor::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("allowOverlap"),   m_oAllowOverlap )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("behindDoc"),      m_oBehindDoc )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distB"),          m_oDistB )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distL"),          m_oDistL )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),          m_oDistR )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"),          m_oDistT )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("hidden"),         m_oHidden )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("layoutInCell"),   m_oLayoutInCell )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("locked"),         m_oLocked )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("relativeHeight"), m_oRelativeHeight )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("simplePos"),      m_bSimplePos )
			WritingElement_ReadAttributes_End( oReader )

			if (m_oRelativeHeight.IsInit() && m_pMainDocument)
			{
				CDocument *pDocument = NULL;
				OOX::CDocx* docx = dynamic_cast<OOX::CDocx*>(m_pMainDocument);
				if (docx)
				{
					pDocument = docx->m_oMain.document;
				}
				OOX::CDocxFlat *docx_flat = dynamic_cast<OOX::CDocxFlat*>(m_pMainDocument);
				if (docx_flat)
				{
					pDocument = docx_flat->m_pDocument.GetPointer();
				}
				if (pDocument)
				{
					if (m_oRelativeHeight->GetValue() > pDocument->m_nDrawingMaxZIndex)
					{
						pDocument->m_nDrawingMaxZIndex = m_oRelativeHeight->GetValue();
					}
				}
			}
		}
		void CAnchor::ReadAttributes(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"allowOverlap",	m_oAllowOverlap );
			XmlMacroReadAttributeBase(oNode, L"behindDoc",      m_oBehindDoc );
			XmlMacroReadAttributeBase(oNode, L"distB",          m_oDistB );
			XmlMacroReadAttributeBase(oNode, L"distL",          m_oDistL );
			XmlMacroReadAttributeBase(oNode, L"distR",          m_oDistR );
			XmlMacroReadAttributeBase(oNode, L"distT",          m_oDistT );
			XmlMacroReadAttributeBase(oNode, L"hidden",         m_oHidden );
			XmlMacroReadAttributeBase(oNode, L"layoutInCell",   m_oLayoutInCell );
			XmlMacroReadAttributeBase(oNode, L"locked",         m_oLocked );
			XmlMacroReadAttributeBase(oNode, L"relativeHeight", m_oRelativeHeight );
			XmlMacroReadAttributeBase(oNode, L"simplePos",      m_bSimplePos );
		}
		void CAnchor::fromXML(XmlUtils::CXmlNode& node)
		{
			ReadAttributes(node);
			m_eWrapType = anchorwrapUnknown;

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

					if ( _T("docPr") == sName )
						m_oDocPr = oNode;
					else if ( _T("effectExtent") == sName )
						m_oEffectExtent = oNode;
					else if ( _T("extent") == sName )
						m_oExtent = oNode;
					else if ( _T("positionH") == sName )
						m_oPositionH = oNode;
					else if ( _T("positionV") == sName )
						m_oPositionV = oNode;
					else if ( _T("simplePos") == sName )
						m_oSimplePos = oNode;
					else if ( _T("sizeRelH") == sName )
						m_oSizeRelH = oNode;
					else if ( _T("sizeRelV") == sName )
						m_oSizeRelV = oNode;
					else if ( false == m_eWrapType.IsInit() )
					{
						if ( _T("wrapNone") == sName )
						{
							m_oWrapNone = oNode;
							m_eWrapType = anchorwrapNone;
						}
						else if ( _T("wrapSquare") == sName )
						{
							m_oWrapSquare = oNode;
							m_eWrapType = anchorwrapSquare;
						}
						else if ( _T("wrapThrough") == sName )
						{
							m_oWrapThrough = oNode;
							m_eWrapType = anchorwrapThrough;
						}
						else if ( _T("wrapTight") == sName )
						{
							m_oWrapTight = oNode;
							m_eWrapType = anchorwrapTight;
						}
						else if ( _T("wrapTopAndBottom") == sName )
						{
							m_oWrapTopAndBottom = oNode;
							m_eWrapType = anchorwrapTopAndBottom;
						}
					}
				}
				m_oGraphic.fromXML(node);
			}
		}
		void CAnchor::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
                std::wstring sName = oReader.GetName();
				if ( _T("wp:docPr") == sName )
					m_oDocPr = oReader;
				else if ( _T("wp:effectExtent") == sName )
					m_oEffectExtent = oReader;
				else if ( _T("wp:extent") == sName )
					m_oExtent = oReader;
				else if ( _T("wp:positionH") == sName )
					m_oPositionH = oReader;
				else if ( _T("wp:positionV") == sName )
					m_oPositionV = oReader;
				else if ( _T("wp:simplePos") == sName )
					m_oSimplePos = oReader;
				else if ( _T("wp14:sizeRelH") == sName )
					m_oSizeRelH = oReader;
				else if ( _T("wp14:sizeRelV") == sName )
					m_oSizeRelV = oReader;
				else if ( _T("mc:AlternateContent") == sName )
				//ПРИВЛЕЧЕНИЕ СРЕДСТВ ИЗ МЕСТНЫХ ИСТОЧНИКОВ.docx
				//вариативность на разные версии офиса части параметров - кстати ... это может встретиться в ЛЮБОМ месте 
				//todooo сделать чтение не обязательно fallback, по выбору версии нужной нам (w14, ..)
				{
					nCurDepth++;
					while( oReader.ReadNextSiblingNode( nCurDepth ) )
					{
						std::wstring sName = oReader.GetName();
						if ( _T("mc:Fallback") == sName || _T("mc:Choice") == sName )
						{
                            std::wstring strXml = _T("<root xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");
								strXml += oReader.GetOuterXml();
								strXml += _T("</root>");
							
							XmlUtils::CXmlLiteReader oSubReader;
							
							if (oSubReader.FromString(strXml))
							{
								oSubReader.ReadNextNode();//root
								oSubReader.ReadNextNode();//fallback

								fromXML(oSubReader);
								//break чтобы не читать сразу оба Choice и Fallback
								break;
							}
						}
					}
					nCurDepth--;
				}
				else if ( false == m_eWrapType.IsInit() )
				{
					if ( _T("wp:wrapNone") == sName )
					{
						m_oWrapNone = oReader;
						m_eWrapType = anchorwrapNone;
					}
					else if ( _T("wp:wrapSquare") == sName )
					{
						m_oWrapSquare = oReader;
						m_eWrapType = anchorwrapSquare;
					}
					else if ( _T("wp:wrapThrough") == sName )
					{
						m_oWrapThrough = oReader;
						m_eWrapType = anchorwrapThrough;
					}
					else if ( _T("wp:wrapTight") == sName )
					{
						m_oWrapTight = oReader;
						m_eWrapType = anchorwrapTight;
					}
					else if ( _T("wp:wrapTopAndBottom") == sName )
					{
						m_oWrapTopAndBottom = oReader;
						m_eWrapType = anchorwrapTopAndBottom;
					}
				}
				else
				{
					m_oGraphic.fromXML2(oReader);
				}
			}
		}
		std::wstring CAnchor::toXML() const
		{
            std::wstring sResult = _T("");
			return sResult;
		}

		void CInline::ReadAttributes(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"distB",          m_oDistB );
			XmlMacroReadAttributeBase(oNode, L"distL",          m_oDistL );
			XmlMacroReadAttributeBase(oNode, L"distR",          m_oDistR );
			XmlMacroReadAttributeBase(oNode, L"distT",          m_oDistT );
		}
		void CInline::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("distB"),          m_oDistB )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distL"),          m_oDistL )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),          m_oDistR )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"),          m_oDistT )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CInline::fromXML(XmlUtils::CXmlNode& node)
		{
			ReadAttributes( node );

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

					if ( _T("docPr") == sName )
						m_oDocPr = oNode;
					else if ( _T("effectExtent") == sName )
						m_oEffectExtent = oNode;
					else if ( _T("extent") == sName )
						m_oExtent = oNode;
				}
				m_oGraphic.fromXML(node);
			}
		}
		void CInline::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
                std::wstring sName = oReader.GetName();
				if ( _T("wp:docPr") == sName )
					m_oDocPr = oReader;
				else if ( _T("wp:effectExtent") == sName )
					m_oEffectExtent = oReader;
				else if ( _T("wp:extent") == sName ) 
					m_oExtent = oReader;
				else
				{
					m_oGraphic.fromXML2(oReader);
				}
			}
		}
        std::wstring CInline::toXML() const
		{
            std::wstring sResult = _T("<wp:inline ");
			
			if ( m_oDistB.IsInit() ) sResult += _T("distB=\"") + m_oDistB->ToString() + _T("\" ");
			if ( m_oDistL.IsInit() ) sResult += _T("distL=\"") + m_oDistL->ToString() + _T("\" ");
			if ( m_oDistR.IsInit() ) sResult += _T("distR=\"") + m_oDistR->ToString() + _T("\" ");
			if ( m_oDistT.IsInit() ) sResult += _T("distT=\"") + m_oDistT->ToString() + _T("\" ");

			sResult += _T(">");
			
			if ( m_oExtent.IsInit() )  
				sResult += _T("<wp:extent ") + m_oExtent->ToString() + _T("/>");

			if ( m_oEffectExtent.IsInit() )
				sResult += m_oEffectExtent->toXML();

			if ( m_oDocPr.IsInit() )
				sResult += m_oDocPr->toXML();

			m_oGraphic.m_namespace == L"wp"; 
			sResult += m_oGraphic.toXML2();	

			sResult += _T("</wp:inline>");

			return sResult;
		}

		CCompatExt::~CCompatExt()
		{
		}

		COfficeArtExtension::~COfficeArtExtension()
		{
			m_oSparklineGroups.reset();
			m_oAltTextTable.reset();
			m_oDataModelExt.reset();
			m_oCompatExt.reset();
			m_oDataValidations.reset();
			m_oConnection.reset();
			m_oDxfs.reset();
			m_oTableSlicerCache.reset();
			m_oSlicerList.reset();
			m_oSlicerListExt.reset();
			m_oSlicerCaches.reset();
			m_oSlicerCachesExt.reset();
			m_oSlicerStyles.reset();
			m_oTableSlicerCache.reset();
			m_oSlicerCacheHideItemsWithNoData.reset();

			for (size_t nIndex = 0; nIndex < m_arrConditionalFormatting.size(); ++nIndex)
			{
				delete m_arrConditionalFormatting[nIndex];
			}
			m_arrConditionalFormatting.clear();

			for (size_t nIndex = 0; nIndex < m_oSlicerCachePivotTables.size(); ++nIndex)
			{
				delete m_oSlicerCachePivotTables[nIndex];
			}
			m_oSlicerCachePivotTables.clear();
		}
		void COfficeArtExtension::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

            if ((m_sUri.IsInit()) && (*m_sUri == L"{63B3BB69-23CF-44E3-9099-C40C66FF867C}"	||
                                      *m_sUri == L"{05C60535-1F16-4fd2-B633-F4F36F0B64E0}"	|| 
									  *m_sUri == L"{504A1905-F514-4f6f-8877-14C23A59335A}"	|| 
									  *m_sUri == L"{78C0D931-6437-407d-A8EE-F0AAD7539E65}"	||
									  *m_sUri == L"{B025F937-C7B1-47D3-B67F-A62EFF666E3E}"	||
									  *m_sUri == L"{CCE6A557-97BC-4b89-ADB6-D9C93CAAB3DF}"	||
									  *m_sUri == L"{A8765BA9-456A-4dab-B4F3-ACF838C121DE}"	||
									  *m_sUri == L"{3A4CF648-6AED-40f4-86FF-DC5316D8AED3}"	||
									  *m_sUri == L"{BBE1A952-AA13-448e-AADC-164F8A28A991}"	||
									  *m_sUri == L"{46BE6895-7355-4a93-B00E-2C351335B9C9}"	||
									  *m_sUri == L"{EB79DEF2-80B8-43e5-95BD-54CBDDF9020C}"	||
									  *m_sUri == L"{03082B11-2C62-411c-B77F-237D8FCFBE4C}"	||
									  *m_sUri == L"{2F2917AC-EB37-4324-AD4E-5DD8C200BD13}"	||
									  *m_sUri == L"{470722E0-AACD-4C17-9CDC-17EF765DBC7E}"	||
									  *m_sUri == L"{46F421CA-312F-682f-3DD2-61675219B42D}"	||
									  *m_sUri == L"{DE250136-89BD-433C-8126-D09CA5730AF9}"	||
									  *m_sUri == L"{19B8F6BF-5375-455C-9EA6-DF929625EA0E}"	||
									  *m_sUri == L"http://schemas.microsoft.com/office/drawing/2008/diagram"))   
			{
				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (sName == L"compatExt")//2.3.1.2 compatExt
					{	//attributes spid -https://msdn.microsoft.com/en-us/library/hh657207(v=office.12).aspx
						m_oCompatExt = oReader;
					}
					else if (sName == L"sparklineGroups")
					{
						m_oSparklineGroups = oReader;
					}
					else if (sName == L"dataModelExt")
					{
						m_oDataModelExt = oReader;
					}
					else if (sName == L"table")
					{
						m_oAltTextTable = oReader;
					}
					else if (sName == L"conditionalFormattings")
					{
						if ( oReader.IsEmptyNode() )
							continue;

						int nCurDepth1 = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nCurDepth1 ) )
						{						
							m_arrConditionalFormatting.push_back(new OOX::Spreadsheet::CConditionalFormatting(oReader));
						}
					}
					else if (sName == L"dataValidations")
					{
						m_oDataValidations = oReader;
					}
					else if (sName == L"connection")
					{
						m_oConnection = oReader;
					}
					else if (sName == L"slicerList")
					{
						if (L"{A8765BA9-456A-4dab-B4F3-ACF838C121DE}" == *m_sUri)
						{
							m_oSlicerList = oReader;
						}
						else
						{
							m_oSlicerListExt = oReader;
						}
					}
					else if (sName == L"slicerCaches")
					{
						if (L"{BBE1A952-AA13-448e-AADC-164F8A28A991}" == *m_sUri)
						{
							m_oSlicerCaches = oReader;
						}
						else
						{
							m_oSlicerCachesExt = oReader;
						}
					}
					else if (sName == L"dxfs")
					{
						m_oDxfs = oReader;
					}
					else if (sName == L"slicerStyles")
					{
						m_oSlicerStyles = oReader;
					}
					else if (sName == L"slicerCachePivotTables")
					{
						if ( oReader.IsEmptyNode() )
							continue;

						int nCurDepth1 = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nCurDepth1 ) )
						{
							m_oSlicerCachePivotTables.push_back(new OOX::Spreadsheet::CSlicerCachePivotTable(oReader));
						}
					}
					else if (sName == L"tableSlicerCache")
					{
						m_oTableSlicerCache = oReader;
					}
					else if (sName == L"slicerCacheHideItemsWithNoData")
					{
						m_oSlicerCacheHideItemsWithNoData = oReader;
					}
					else if (sName == L"id")
					{
						m_oId = oReader.GetText2();
					}
					else if (sName == L"presenceInfo")
					{
						m_oPresenceInfo = oReader;
					}
				}
			}
			else
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
		}
        std::wstring COfficeArtExtension::toXML() const
        {
            return toXMLWithNS(L"a:");
        }
        std::wstring COfficeArtExtension::toXMLWithNS(const std::wstring& sNamespace) const
		{
            std::wstring sResult = L"<" + sNamespace + L"ext";

            if ( m_sUri.IsInit() )
			{
                sResult += L" uri=\"" + m_sUri.get2() + L"\"";
			}

			if (!m_sAdditionalNamespace.empty())
			{
				sResult += L" " + m_sAdditionalNamespace;
			}

			sResult += L">";

			if(m_oCompatExt.IsInit())
			{
				sResult += m_oCompatExt->toXML();
			}
			if(m_oSparklineGroups.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSparklineGroups->toXML(writer);
                sResult += writer.GetData().c_str();
			}
			if(m_oAltTextTable.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oAltTextTable->toXML(writer);
				sResult += writer.GetData().c_str();
			}			
			if (false == m_arrConditionalFormatting.empty())
			{
				sResult += L"<x14:conditionalFormattings>";
				NSStringUtils::CStringBuilder writer;
				for (size_t i = 0; i < m_arrConditionalFormatting.size(); i++)
				{
					m_arrConditionalFormatting[i]->toXML2(writer, true);
				}
				sResult += writer.GetData().c_str();
				sResult += L"</x14:conditionalFormattings>";
			}
			if(m_oDataValidations.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oDataValidations->toXML2(writer, true);
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerList.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerList->toXML(writer, L"x14:slicerList");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerListExt.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerListExt->toXML(writer, L"x14:slicerList");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerCaches.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerCaches->toXML(writer, L"slicerCaches", L"x14:");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerCachesExt.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerCachesExt->toXML(writer, L"slicerCaches", L"x15:");
				sResult += writer.GetData().c_str();
			}
			if(m_oDxfs.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oDxfs->toXML2(writer, L"x14:dxfs");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerStyles.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerStyles->toXML(writer, L"x14:slicerStyles");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerCachePivotTables.size() > 0)
			{
				NSStringUtils::CStringBuilder writer;
				writer.StartNode(L"x15:slicerCachePivotTables");
				writer.StartAttributes();
				writer.EndAttributes();
				for(size_t i = 0; i < m_oSlicerCachePivotTables.size(); ++i)
				{
					m_oSlicerCachePivotTables[i]->toXML(writer, L"pivotTable");
				}
				writer.EndNode(L"x15:slicerCachePivotTables");
				sResult += writer.GetData().c_str();
			}
			if(m_oTableSlicerCache.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oTableSlicerCache->toXML(writer, L"x15:tableSlicerCache");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerCacheHideItemsWithNoData.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerCacheHideItemsWithNoData->toXML(writer, L"x15:slicerCacheHideItemsWithNoData");
				sResult += writer.GetData().c_str();
			}
			if (m_oId.IsInit())
			{
				sResult += L"<" + sNamespace + L"id>" + m_oId.get2() + L"</" + sNamespace + L"id>";
			}
            sResult += L"</" + sNamespace + L"ext>";

			return sResult;
		}
	}
}
