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

			nullable<SimpleTypes::CRelFromH<>>			m_oRelativeFrom;

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

			nullable<SimpleTypes::CRelFromV<>>			m_oRelativeFrom;

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
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapSquare;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<SimpleTypes::CWrapDistance<>> m_oDistB;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistL;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistR;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistT;
			nullable<SimpleTypes::CWrapText<>>     m_oWrapText;

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
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapPolygon;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<SimpleTypes::COnOff<> >				m_oEdited;

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
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapThrough;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<SimpleTypes::CWrapDistance<>>  m_oDistL;
			nullable<SimpleTypes::CWrapDistance<>>	m_oDistR;
			nullable<SimpleTypes::CWrapText<>>		m_oWrapText;

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
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapTight;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
            nullable<SimpleTypes::CWrapDistance<>>	m_oDistL;
            nullable<SimpleTypes::CWrapDistance<>>	m_oDistR;
            nullable<SimpleTypes::CWrapText<>>		m_oWrapText;

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
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_wp_wrapTopAndBottom;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<SimpleTypes::CWrapDistance<>> m_oDistB;
			nullable<SimpleTypes::CWrapDistance<>> m_oDistT;

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
			CAnchor(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CAnchor()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_wp_anchor;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlNode& oNode);
		public:
			nullable<EAnchorWrapType>								m_eWrapType;

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
			CInline(OOX::Document *pMain = NULL) : WritingElement(pMain) 
			{
			}
			virtual ~CInline()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return OOX::et_wp_inline;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlNode& oNode);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
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
			CDrawing(OOX::Document *pMain = NULL) : m_bAnchor(false), WritingElement(pMain) {}

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
						m_oInline = new OOX::Drawing::CInline(this->m_pMainDocument);
						m_oInline->fromXML(oReader);
						m_bAnchor = false;
					}
					else if ( _T("wp:anchor") == sName )
					{
						m_oAnchor = new OOX::Drawing::CAnchor(this->m_pMainDocument);
						m_oAnchor->fromXML(oReader);
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

