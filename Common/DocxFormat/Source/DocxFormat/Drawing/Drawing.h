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
#ifndef OOX_LOGIC_DRAWING_INCLUDE_H_
#define OOX_LOGIC_DRAWING_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"
#include "../RId.h"

#include "../../../../../ASCOfficePPTXFile/PPTXFormat/Logic/GraphicFrame.h"

#include "../../SystemUtility/SystemUtility.h"

namespace OOX
{
	namespace Drawing
	{
		//-----------------------------------------------------------------------
		// EffectExtent 20.4.2.6
		//-----------------------------------------------------------------------
		class CEffectExtent : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEffectExtent)
			CEffectExtent()
			{
			}
			virtual ~CEffectExtent()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("l"), m_oL );
				XmlMacroReadAttributeBase( oNode, _T("t"), m_oT );
				XmlMacroReadAttributeBase( oNode, _T("r"), m_oR );
				XmlMacroReadAttributeBase( oNode, _T("b"), m_oB );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("<wp:effectExtent ");

				sResult += _T("l=\"") + m_oL.ToString() + _T("\" ");
				sResult += _T("t=\"") + m_oT.ToString() + _T("\" ");
				sResult += _T("r=\"") + m_oR.ToString() + _T("\" ");
				sResult += _T("b=\"") + m_oB.ToString() + _T("\" ");

				sResult += _T("</wp:effectExtent>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_effectExtent;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("l"), m_oL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("t"), m_oT )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r"), m_oR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("b"), m_oB )
				WritingElement_ReadAttributes_End( oReader )
			}
			SimpleTypes::CCoordinate m_oB;
			SimpleTypes::CCoordinate m_oL;
			SimpleTypes::CCoordinate m_oR;
			SimpleTypes::CCoordinate m_oT;
		};
		//--------------------------------------------------------------------------------
		// CPosH 20.4.2.10 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPosH : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPosH)
			CPosH()
			{
			}
			virtual ~CPosH()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"relativeFrom",	m_oRelativeFrom );

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

						if ( _T("align") == sName )
						{
							m_oAlign = oNode.GetText();
							m_bAlign = true;
						}
						else if ( _T("posOffset") == sName )
						{
							m_oPosOffset = oNode.GetText();
							m_bAlign = false;
						}
						else if ( _T("pctPosHOffset") == sName )
						{
							m_oPctOffset = oNode.GetText();
							m_bAlign = false;
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    std::wstring sName = oReader.GetName();
					if ( _T("wp:align") == sName )
					{
                        m_oAlign = oReader.GetText2();
						m_bAlign = true;
					}
					else if ( _T("wp:posOffset") == sName )
					{
                        m_oPosOffset = oReader.GetText2();
						m_bAlign = false;
					}
					else if ( _T("wp14:pctPosHOffset") == sName )
					{//Cuaderno del maestro.docx
                        m_oPctOffset = oReader.GetText2();
						m_bAlign = false;
					}
				}
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("<wp:positionH ");

				if ( m_oRelativeFrom.IsInit() )
					sResult += _T("relativeFrom=\"") + m_oRelativeFrom->ToString() + _T("\">");
				else
					sResult += _T(">");

				if ( m_bAlign && m_oAlign.IsInit() )
					sResult += _T("<wp:align>") + m_oAlign->ToString() + _T("</wp:align>");
				else if ( !m_bAlign && m_oPosOffset.IsInit() )
					sResult += _T("<wp:posOffset>") + m_oPosOffset->ToString() + _T("</wp:posOffset>");
				else if (!m_bAlign && m_oPctOffset.IsInit())
					sResult += _T("<wp14:pctPosHOffset>") + m_oPctOffset->ToString() + _T("</wp14:pctPosHOffset>");
					

				sResult += _T("</wp:positionH>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_positionH;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
	// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("relativeFrom"), m_oRelativeFrom )
				WritingElement_ReadAttributes_End( oReader )
			}
			bool IsAlign() const
			{
				return m_bAlign;
			}
			bool IsPosOffset() const
			{
				return !m_bAlign;
			}

			bool										m_bAlign; // используем Align или (PosOffset или PctOffset)
	// Attributes
			nullable<SimpleTypes::CRelFromH<>>			m_oRelativeFrom;
	// Childs
			nullable<SimpleTypes::CAlignH<>>			m_oAlign;
			nullable<SimpleTypes::CPositionOffset<>>	m_oPosOffset;
			nullable<SimpleTypes::CPercentage>			m_oPctOffset;
		};
		//--------------------------------------------------------------------------------
		// CPosV 20.4.2.11 (Part 1)
		//--------------------------------------------------------------------------------	
		class CPosV : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPosV)
			CPosV()
			{
			}
			virtual ~CPosV()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"relativeFrom",	m_oRelativeFrom );

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

						if ( _T("align") == sName )
						{
							m_oAlign = oNode.GetText();
							m_bAlign = true;
						}
						else if ( _T("posOffset") == sName )
						{
							m_oPosOffset = oNode.GetText();
							m_bAlign = false;
						}
						else if ( _T("pctPosVOffset") == sName )
						{
							m_oPctOffset = oNode.GetText();
							m_bAlign = false;
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    std::wstring sName = oReader.GetName();
					if ( _T("wp:align") == sName )
					{
                        m_oAlign = oReader.GetText2();
						m_bAlign = true;
					}
					else if ( _T("wp:posOffset") == sName )
					{
                        m_oPosOffset = oReader.GetText2();
						m_bAlign = false;
					}
					else if ( _T("wp14:pctPosVOffset") == sName )
					{
                        m_oPctOffset = oReader.GetText2();
						m_bAlign = false;
					}
				}
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("<wp:positionV ");

				if ( m_oRelativeFrom.IsInit() )
					sResult += _T("relativeFrom=\"") + m_oRelativeFrom->ToString() + _T("\">");
				else
					sResult += _T(">");

				if ( m_bAlign && m_oAlign.IsInit() )
					sResult += _T("<wp:align>") + m_oAlign->ToString() + _T("</wp:align>");
				else if ( !m_bAlign && m_oPosOffset.IsInit() )
					sResult += _T("<wp:posOffset>") + m_oPosOffset->ToString() + _T("</wp:posOffset>");
				else if (!m_bAlign && m_oPctOffset.IsInit())
					sResult += _T("<wp14:pctPosVOffset>") + m_oPctOffset->ToString() + _T("</wp14:pctPosVOffset>");

				sResult += _T("</wp:positionV>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_positionV;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("relativeFrom"), m_oRelativeFrom )
				WritingElement_ReadAttributes_End( oReader )
			}
			bool IsAlign() const
			{
				return m_bAlign;
			}
			bool IsPosOffset() const
			{
				return !m_bAlign;
			}

			bool										m_bAlign; // используем Align или PosOffset
	// Attributes
			nullable<SimpleTypes::CRelFromV<>>			m_oRelativeFrom;
	// Childs
			nullable<SimpleTypes::CAlignV<>>			m_oAlign;
			nullable<SimpleTypes::CPositionOffset<>>	m_oPosOffset;
			nullable<SimpleTypes::CPercentage>			m_oPctOffset;
		};

		class CSizeRelH : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSizeRelH)
			CSizeRelH()
			{
			}
			virtual ~CSizeRelH()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"relativeFrom",	m_oRelativeFrom );
				
				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

						if ( _T("pctHeight") == sName )
						{
							m_oPctWidth = oNode.GetText();
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("wp14:pctWidth") == sName )
					{
                        m_oPctWidth = oReader.GetText2();
					}
				}
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("<wp14:sizeRelH ");

				if ( m_oRelativeFrom.IsInit() )
					sResult += _T("relativeFrom=\"") + m_oRelativeFrom->ToString() + _T("\">");
				else
					sResult += _T(">");

				if (m_oPctWidth.IsInit())
					sResult += _T("<wp14:pctWidth>") + m_oPctWidth->ToStringDecimalNumber() + _T("</wp14:pctWidth>");


				sResult += _T("</wp14:sizeRelH>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_sizeRelH;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("relativeFrom"), m_oRelativeFrom )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
	// Attributes
			nullable<SimpleTypes::CSizeRelFromH<>>	m_oRelativeFrom;
	// Childs
			nullable<SimpleTypes::CPercentage>		m_oPctWidth;
		};
		class CSizeRelV : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSizeRelV)
			CSizeRelV()
			{
			}
			virtual ~CSizeRelV()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"relativeFrom",	m_oRelativeFrom );
				
				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						std::wstring sName = XmlUtils::GetNameNoNS(oNode.GetName());

						if ( _T("pctHeight") == sName )
						{
							m_oPctHeight = oNode.GetText();
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("wp14:pctHeight") == sName )
					{
                        m_oPctHeight = oReader.GetText2();
					}
				}
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("<wp14:sizeRelV ");

				if ( m_oRelativeFrom.IsInit() )
					sResult += _T("relativeFrom=\"") + m_oRelativeFrom->ToString() + _T("\">");
				else
					sResult += _T(">");

				if (m_oPctHeight.IsInit())
					sResult += _T("<wp14:pctHeight>") + m_oPctHeight->ToStringDecimalNumber() + _T("</wp14:pctHeight>");


				sResult += _T("</wp14:sizeRelV>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_sizeRelH;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("relativeFrom"), m_oRelativeFrom )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
	// Attributes
			nullable<SimpleTypes::CSizeRelFromV<>>	m_oRelativeFrom;
	// Childs
			nullable<SimpleTypes::CPercentage>		m_oPctHeight;
		};
		//--------------------------------------------------------------------------------
		// CWrapNone 20.4.2.15 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapNone : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapNone)
			CWrapNone()
			{
			}
			virtual ~CWrapNone()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapNone;
			}
		};
		//--------------------------------------------------------------------------------
		// CWrapSquare 20.4.2.17 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapSquare : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapSquare)
			CWrapSquare()
			{
			}
			virtual ~CWrapSquare()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapSquare;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("distB"),    m_oDistB )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distL"),    m_oDistL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),    m_oDistR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"),    m_oDistT )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"), m_oWrapText )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:

	// Attributes
			nullable<SimpleTypes::CWrapDistance<>> m_oDistB;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistL;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistR;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistT;
			nullable<SimpleTypes::CWrapText<>>     m_oWrapText;
	// Childs
			nullable<OOX::Drawing::CEffectExtent>  m_oEffectExtent;
		};
		//--------------------------------------------------------------------------------
		// CWrapPath 20.4.2.16 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapPath : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapPath)
			CWrapPath()
			{
			}
			virtual ~CWrapPath()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrLineTo.size(); nIndex++ )
				{
					if ( m_arrLineTo[nIndex] )delete m_arrLineTo[nIndex];
					m_arrLineTo[nIndex] = NULL;
				}
				m_arrLineTo.clear();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapPolygon;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("edited"), m_oEdited )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
	// Attributes
			nullable<SimpleTypes::COnOff<> >				m_oEdited;
	// Childs
			nullable<ComplexTypes::Drawing::CPoint2D>       m_oStart;
			std::vector<ComplexTypes::Drawing::CPoint2D*>	m_arrLineTo;
		};
		//--------------------------------------------------------------------------------
		// CWrapThrough 20.4.2.18 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapThrough : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapThrough)
			CWrapThrough()
			{
			}
			virtual ~CWrapThrough()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapThrough;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("distL"),    m_oDistL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),    m_oDistR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"), m_oWrapText )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
	// Attributes
			nullable<SimpleTypes::CWrapDistance<>>  m_oDistL;
			nullable<SimpleTypes::CWrapDistance<>>	m_oDistR;
			nullable<SimpleTypes::CWrapText<>>		m_oWrapText;
	// Childs
			nullable<OOX::Drawing::CWrapPath>		m_oWrapPolygon;
		};
		//--------------------------------------------------------------------------------
		// CWrapTight 20.4.2.19 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapTight : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapTight)
			CWrapTight()
			{
			}
			virtual ~CWrapTight()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapTight;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("distL"),    m_oDistL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),    m_oDistR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"), m_oWrapText )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
	// Attributes
            nullable<SimpleTypes::CWrapDistance<>>	m_oDistL;
            nullable<SimpleTypes::CWrapDistance<>>	m_oDistR;
            nullable<SimpleTypes::CWrapText<>>		m_oWrapText;
	// Childs
            nullable<OOX::Drawing::CWrapPath>		m_oWrapPolygon;
		};
		//--------------------------------------------------------------------------------
		// CWrapTopBottom 20.4.2.20 (Part 1)
		//--------------------------------------------------------------------------------	
		class CWrapTopBottom : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWrapTopBottom)
			CWrapTopBottom()
			{
			}
			virtual ~CWrapTopBottom()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapTopAndBottom;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("distB"), m_oDistB )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"), m_oDistT )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
	// Attributes
			nullable<SimpleTypes::CWrapDistance<>> m_oDistB;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistT;
	// Childs
			nullable<OOX::Drawing::CEffectExtent>  m_oEffectExtent;
		};
		//--------------------------------------------------------------------------------
		// CAnchor 20.4.2.3 (Part 1)
		//--------------------------------------------------------------------------------	
		enum EAnchorWrapType
		{
			anchorwrapUnknown      = 0,
			anchorwrapNone         = 1,
			anchorwrapSquare       = 2,
			anchorwrapThrough      = 3,
			anchorwrapTight        = 4,
			anchorwrapTopAndBottom = 5
		};

		class CAnchor : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAnchor)
			CAnchor()
			{
			}
			virtual ~CAnchor()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_wp_anchor;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
			}
			void ReadAttributes(XmlUtils::CXmlNode& oNode)
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
		public:
			nullable<EAnchorWrapType>								m_eWrapType;

	// Attributes
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>	m_oAllowOverlap;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>	m_oBehindDoc;
			nullable<SimpleTypes::CWrapDistance<>>					m_oDistB;
			nullable<SimpleTypes::CWrapDistance<>>					m_oDistL;
			nullable<SimpleTypes::CWrapDistance<>>					m_oDistR;
			nullable<SimpleTypes::CWrapDistance<>>					m_oDistT;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>	m_oHidden;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>	m_oLayoutInCell;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>	m_oLocked;
			nullable<SimpleTypes::CUnsignedDecimalNumber<0> >		m_oRelativeHeight;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>	m_bSimplePos;

	// Childs
			nullable<PPTX::Logic::CNvPr>						m_oDocPr;			
			nullable<OOX::Drawing::CEffectExtent>				m_oEffectExtent;
			nullable<ComplexTypes::Drawing::CPositiveSize2D>	m_oExtent;
			nullable<OOX::Drawing::CPosH>						m_oPositionH;
			nullable<OOX::Drawing::CPosV>						m_oPositionV;
			nullable<OOX::Drawing::CSizeRelH>					m_oSizeRelH;
			nullable<OOX::Drawing::CSizeRelV>					m_oSizeRelV;
			nullable<ComplexTypes::Drawing::CPoint2D>			m_oSimplePos;
			nullable<OOX::Drawing::CWrapNone>					m_oWrapNone;
			nullable<OOX::Drawing::CWrapSquare>					m_oWrapSquare;
			nullable<OOX::Drawing::CWrapThrough>				m_oWrapThrough;
			nullable<OOX::Drawing::CWrapTight>					m_oWrapTight;
			nullable<OOX::Drawing::CWrapTopBottom>				m_oWrapTopAndBottom;

			PPTX::Logic::GraphicFrame							m_oGraphic;
		};
		//--------------------------------------------------------------------------------
		// CInline 20.4.2.8 (Part 1)
		//--------------------------------------------------------------------------------	
		class CInline : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CInline)
			CInline()
			{
			}
			virtual ~CInline()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
            virtual std::wstring toXML() const
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
			virtual EElementType getType() const
			{
				return OOX::et_wp_inline;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase(oNode, L"distB",          m_oDistB );
				XmlMacroReadAttributeBase(oNode, L"distL",          m_oDistL );
				XmlMacroReadAttributeBase(oNode, L"distR",          m_oDistR );
				XmlMacroReadAttributeBase(oNode, L"distT",          m_oDistT );
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("distB"),          m_oDistB )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distL"),          m_oDistL )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distR"),          m_oDistR )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("distT"),          m_oDistT )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
	// Attributes
			nullable<SimpleTypes::CWrapDistance<>>				m_oDistB;
			nullable<SimpleTypes::CWrapDistance<>>				m_oDistL;
			nullable<SimpleTypes::CWrapDistance<>>				m_oDistR;
			nullable<SimpleTypes::CWrapDistance<>>				m_oDistT;
	// Childs
			nullable<PPTX::Logic::CNvPr>						m_oDocPr;
			nullable<OOX::Drawing::CEffectExtent>				m_oEffectExtent;
			nullable<ComplexTypes::Drawing::CPositiveSize2D>	m_oExtent;
			PPTX::Logic::GraphicFrame							m_oGraphic;
		};
	} // Drawing
} // OOX

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CDrawing 17.3.3.9 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDrawing : public WritingElement
		{
		public:
			CDrawing()
			{
			}
			CDrawing(XmlUtils::CXmlNode& oNode)
			{
				fromXML( oNode );
			}
			CDrawing(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CDrawing()
			{
			}
			const CDrawing& operator =(const XmlUtils::CXmlNode& oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CDrawing& operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("inline") == strName)
						{
							m_oInline = oNode;
							m_bAnchor = false;
						}
						else if (_T("anchor") == strName)
						{
							m_oAnchor = oNode;
							m_bAnchor = true;
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;
			
				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    std::wstring sName = oReader.GetName();
					if ( _T("wp:inline") == sName )
					{
						m_oInline = oReader;
						m_bAnchor = false;
					}
					else if ( _T("wp:anchor") == sName )
					{
						m_oAnchor = oReader;
						m_bAnchor = true;
					}
				}
			}
            virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("<w:drawing>");

				if ( m_bAnchor && m_oAnchor.IsInit() )
					sResult += m_oAnchor->toXML();
				else if ( !m_bAnchor && m_oInline.IsInit() )
					sResult += m_oInline->toXML();

				sResult += _T("</w:drawing>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_w_drawing;
			}
			bool IsAnchor() const
			{
				return m_bAnchor;
			}
			bool IsInline() const
			{
				return !m_bAnchor;
			}

			bool                            m_bAnchor; // используем Anchor или Inline
	// Childs
			nullable<OOX::Drawing::CAnchor> m_oAnchor;
			nullable<OOX::Drawing::CInline> m_oInline;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_INCLUDE_H_
