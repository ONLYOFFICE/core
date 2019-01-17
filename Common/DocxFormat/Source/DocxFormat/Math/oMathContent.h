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

#include "../../Common/ComplexTypes.h"

#include "oMathBottomNodes.h"
#include "OMath.h"

#include "../Logic/RunContent.h"
#include "../Logic/FldChar.h"
#include "../Logic/RunProperty.h"
#include "../Logic/Annotations.h"
#include "../Logic/Pict.h"
#include "../Drawing/Drawing.h"
#include "../../../../../ASCOfficePPTXFile/PPTXFormat/Logic/RunProperties.h"


namespace OOX
{	
	// Elements 22.1.2
	namespace Logic
	{
		#define CMathArgNodesEx(name, type)\
			class name : public CMathArgNodes\
			{\
			public:\
				name(XmlUtils::CXmlNode& oNode)\
				{\
					fromXML( oNode );\
				}\
				name(XmlUtils::CXmlLiteReader& oReader)\
				{\
					fromXML( oReader );\
				}\
				virtual EElementType getType() const\
				{\
					return type;\
				}\
			};
		CMathArgNodesEx(CDeg, OOX::et_m_deg)
		CMathArgNodesEx(CDen, OOX::et_m_den)
		CMathArgNodesEx(CElement, OOX::et_m_e)
		CMathArgNodesEx(CFName, OOX::et_m_fName)
		CMathArgNodesEx(CLim, OOX::et_m_lim)
		CMathArgNodesEx(CNum, OOX::et_m_num)
		CMathArgNodesEx(COMath, OOX::et_m_oMath)
		CMathArgNodesEx(CSub, OOX::et_m_sub)
		CMathArgNodesEx(CSup, OOX::et_m_sup)
//-------------------------------------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------
		// CCtrlPr 22.1.2.23 (Control Properties) 
		//--------------------------------------------------------------------------------
		class CCtrlPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCtrlPr)
			CCtrlPr()
			{
			}
			virtual ~CCtrlPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("w:rP") == sName )
								m_oRPr = new OOX::Logic::CRunProperty( oItem );
							else if ( _T("a:rPr") == sName )
								m_oARPr = new PPTX::Logic::RunProperties( oItem );
							else if ( _T("w:ins") == sName )
								m_oIns = new OOX::Logic::CRPrChange( oItem );
							else if ( _T("w:del") == sName )
								m_oDel = new OOX::Logic::CRPrChange( oItem );

						}
					}
				}			
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("w:rPr") == sName )
						m_oRPr = oReader;
					else if ( _T("a:rPr") == sName )
					{
						std::wstring sXml = oReader.GetOuterXml();
						XmlUtils::CXmlNode node;
						node.FromXmlString(sXml);

						m_oARPr = node;
					}
					else if ( _T("w:ins") == sName )
						m_oIns = oReader;
					else if ( _T("w:del") == sName )
						m_oDel = oReader;
				}
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<m:ctrlPr>");

				if ( m_oRPr.IsInit() )
					sResult += m_oRPr->toXML();
				if ( m_oARPr.IsInit() )
					sResult += m_oARPr->toXML();
				if ( m_oIns.IsInit() )
					sResult += m_oIns->toXML();
				if ( m_oDel.IsInit() )
					sResult += m_oDel->toXML();
				
				sResult += _T("</m:ctrlPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_ctrlPr;
			}
		public:			
			//Childs
			nullable<OOX::Logic::CRunProperty> m_oRPr;
			nullable<PPTX::Logic::RunProperties> m_oARPr;
			nullable<OOX::Logic::CRPrChange> m_oIns;
			nullable<OOX::Logic::CRPrChange> m_oDel;
		};
		//--------------------------------------------------------------------------------
		// CAccPr 22.1.2.2 (Accent Properties)  
		//--------------------------------------------------------------------------------
		class CAccPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAccPr)
			CAccPr()
			{
			}
			virtual ~CAccPr()
			{
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:chr") == sName )
								m_oChr = new OOX::Logic::CChr( oItem );
							else if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
						}
					}
				}			
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:chr") == sName )
						m_oChr = oReader;
					else if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
				}
			}
			virtual std::wstring      toXML() const;

			virtual EElementType getType() const
			{
				return et_m_accPr;
			}
		public:
			//Childs
			nullable<OOX::Logic::CChr>			m_oChr;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
		};

		//--------------------------------------------------------------------------------
		// CAcc 22.1.2.1  (Accent) 
		//--------------------------------------------------------------------------------
		class CAcc : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAcc)
			CAcc()
			{
			}
			virtual ~CAcc()
			{
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:accPr") == sName )
								m_oAccPr = new OOX::Logic::CAccPr( oItem );
							else if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );
						}
					}
				}		
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:accPr") == sName )
						m_oAccPr = oReader;
					else if ( _T("m:e") == sName )
						m_oElement = oReader;
				}
			}
			 std::wstring      toXML() const;
			
			virtual EElementType getType() const
			{
				return et_m_acc;
			}
		public:
			//Childs
			nullable<OOX::Logic::CAccPr>		m_oAccPr;
			nullable<OOX::Logic::CElement>		m_oElement;
		};

		//--------------------------------------------------------------------------------
		// CArgPr 22.1.2.5  (Argument Properties) 
		//--------------------------------------------------------------------------------
		class CArgPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CArgPr)
			CArgPr()
			{
			}
			virtual ~CArgPr()
			{
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:argSz") == sName )
								m_oArgSz = new OOX::Logic::CArgSz( oItem );
						}
					}
				}		
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:argSz") == sName )
						m_oArgSz = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:argPr>");
				
				if ( m_oArgSz.IsInit() )
					sResult += m_oArgSz->toXML();
				
				sResult += _T("</m:argPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_argPr;
			}
		public:
			//Childs
			nullable<OOX::Logic::CArgSz>		m_oArgSz;

		};		

		//--------------------------------------------------------------------------------
		// CBarPr 22.1.2.8 (Bar Properties) 
		//--------------------------------------------------------------------------------
		class CBarPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBarPr)
			CBarPr()
			{
			}
			virtual ~CBarPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
							else if ( _T("m:pos") == sName )
								m_oPos = new OOX::Logic::CPos( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:pos") == sName )
						m_oPos = oReader;
				}
			}
			virtual std::wstring      toXML() const;

			virtual EElementType getType() const
			{
				return et_m_barPr;
			}
		public:
			//Childs
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
			nullable<OOX::Logic::CPos>			m_oPos;
		};		

		//--------------------------------------------------------------------------------
		// CBar 22.1.2.7 (Bar) 
		//--------------------------------------------------------------------------------
		class CBar : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBar)
			CBar()
			{
			}
			virtual ~CBar()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:barPr") == sName )
								m_oBarPr = new OOX::Logic::CBarPr( oItem );
							else if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:barPr") == sName )
						m_oBarPr = oReader;
					else if ( _T("m:e") == sName )
						m_oElement = oReader;
				}
			}
			virtual std::wstring      toXML() const;

			virtual EElementType getType() const
			{
				return et_m_bar;
			}
		public:
			//Childs
			nullable<OOX::Logic::CBarPr>		m_oBarPr;
			nullable<OOX::Logic::CElement>		m_oElement;

		};
		//--------------------------------------------------------------------------------
		// CBorderBoxPr 22.1.2.12 (Border-Box Properties) 
		//--------------------------------------------------------------------------------
		class CBorderBoxPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBorderBoxPr)
			CBorderBoxPr()
			{
			}
			virtual ~CBorderBoxPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
							else if ( _T("m:hideBot") == sName )
								m_oHideBot = new OOX::Logic::CHideBot( oItem );
							else if ( _T("m:hideLeft") == sName )
								m_oHideLeft = new OOX::Logic::CHideLeft( oItem );
							else if ( _T("m:hideRight") == sName )
								m_oHideRight = new OOX::Logic::CHideRight( oItem );
							else if ( _T("m:hideTop") == sName )
								m_oHideTop = new OOX::Logic::CHideTop( oItem );
							else if ( _T("m:strikeBLTR") == sName )
								m_oStrikeBLTR = new OOX::Logic::CStrikeBLTR( oItem );
							else if ( _T("m:strikeH") == sName )
								m_oStrikeH = new OOX::Logic::CStrikeH( oItem );
							else if ( _T("m:strikeTLBR") == sName )
								m_oStrikeTLBR = new OOX::Logic::CStrikeTLBR( oItem );
							else if ( _T("m:strikeV") == sName )
								m_oStrikeV = new OOX::Logic::CStrikeV( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:hideBot") == sName )
						m_oHideBot = oReader;
					else if ( _T("m:hideLeft") == sName )
						m_oHideLeft = oReader;
					else if ( _T("m:hideRight") == sName )
						m_oHideRight = oReader;
					else if ( _T("m:hideTop") == sName )
						m_oHideTop = oReader;
					else if ( _T("m:strikeBLTR") == sName )
						m_oStrikeBLTR = oReader;
					else if ( _T("m:strikeH") == sName )
						m_oStrikeH = oReader;
					else if ( _T("m:strikeTLBR") == sName )
						m_oStrikeTLBR = oReader;
					else if ( _T("m:strikeV") == sName )
						m_oStrikeV = oReader;
				}
			}
			virtual std::wstring      toXML() const;

			virtual EElementType getType() const
			{
				return et_m_borderBoxPr;
			}
		public:
			// Childs
			nullable<OOX::Logic::CCtrlPr>			 m_oCtrlPr;
			nullable<OOX::Logic::CHideBot>			 m_oHideBot;
			nullable<OOX::Logic::CHideLeft>			 m_oHideLeft;
			nullable<OOX::Logic::CHideRight>		 m_oHideRight;
			nullable<OOX::Logic::CHideTop>			 m_oHideTop;
			nullable<OOX::Logic::CStrikeBLTR>		 m_oStrikeBLTR;
			nullable<OOX::Logic::CStrikeH>			 m_oStrikeH;
			nullable<OOX::Logic::CStrikeTLBR>		 m_oStrikeTLBR;
			nullable<OOX::Logic::CStrikeV>			 m_oStrikeV;
		};
		//--------------------------------------------------------------------------------
		// CBorderBox 22.1.2.11 (Border-Box Object) 
		//--------------------------------------------------------------------------------
		class CBorderBox : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBorderBox)
			CBorderBox()
			{
			}
			virtual ~CBorderBox()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:borderBoxPr") == sName )
								m_oBorderBoxPr = new OOX::Logic::CBorderBoxPr( oItem );
							else if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:borderBoxPr") == sName )
						m_oBorderBoxPr = oReader;
					else if ( _T("m:e") == sName )
						m_oElement = oReader;
				}
			}
			virtual std::wstring      toXML() const;

			virtual EElementType getType() const
			{
				return et_m_borderBox;
			}
		public:
			// Childs
			nullable<OOX::Logic::CBorderBoxPr>	 m_oBorderBoxPr;
			nullable<OOX::Logic::CElement>		 m_oElement;
		};		 
		//--------------------------------------------------------------------------------
		// CBrk 22.1.2.15 (Break) 
		//--------------------------------------------------------------------------------
		class CBrk : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBrk)
			CBrk()
			{
			}
			virtual ~CBrk()
			{
			}
			virtual void  fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("m:alnAt"), m_alnAt );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<m:brk m:alnAt=\"");
				sResult += m_alnAt->ToString();
				sResult += _T("\" />");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_brk;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("m:alnAt"), m_alnAt )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			// Attr
			nullable<SimpleTypes::CInteger255<>> m_alnAt;
		};
		//--------------------------------------------------------------------------------
		// CBoxPr 22.1.2.14 (Box Properties)
		//--------------------------------------------------------------------------------
		class CBoxPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBoxPr)
			CBoxPr()
			{
			}
			virtual ~CBoxPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:aln") == sName )
								m_oAln = new OOX::Logic::CAln( oItem );
							else if ( _T("m:brk") == sName )
								m_oBrk = new OOX::Logic::CBrk( oItem );
							else if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
							else if ( _T("m:diff") == sName )
								m_oDiff = new OOX::Logic::CDiff( oItem );
							else if ( _T("m:noBreak") == sName )
								m_oNoBreak = new OOX::Logic::CNoBreak( oItem );
							else if ( _T("m:opEmu") == sName )
								m_oOpEmu = new OOX::Logic::COpEmu( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:aln") == sName )
						m_oAln = oReader;
					else if ( _T("m:brk") == sName )
						m_oBrk = oReader;
					else if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:diff") == sName )
						m_oDiff = oReader;
					else if ( _T("m:noBreak") == sName )
						m_oNoBreak = oReader;
					else if ( _T("m:opEmu") == sName )
						m_oOpEmu = oReader;
				}
			}
			virtual std::wstring      toXML() const;

			virtual EElementType getType() const
			{
				return et_m_boxPr;
			}

		public:
			// Childs
			nullable<OOX::Logic::CAln>			 m_oAln;
			nullable<OOX::Logic::CBrk>			 m_oBrk;
			nullable<OOX::Logic::CCtrlPr>		 m_oCtrlPr;
			nullable<OOX::Logic::CDiff>			 m_oDiff;
			nullable<OOX::Logic::CNoBreak>		 m_oNoBreak;
			nullable<OOX::Logic::COpEmu>		 m_oOpEmu;
		};		
		//--------------------------------------------------------------------------------
		// CBox 22.1.2.13 (Box Object) 
		//--------------------------------------------------------------------------------
		class CBox : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBox)
			CBox()
			{
			}
			virtual ~CBox()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:boxPr") == sName )
								m_oBoxPr = new OOX::Logic::CBoxPr( oItem );
							else if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:boxPr") == sName )
						m_oBoxPr = oReader;
					else if ( _T("m:e") == sName )
						m_oElement = oReader;
				}
			}
			virtual std::wstring      toXML() const;

			virtual EElementType getType() const
			{
				return et_m_box;
			}
		public:
			// Childs
			nullable<OOX::Logic::CBoxPr>			 m_oBoxPr;
			nullable<OOX::Logic::CElement>			 m_oElement;
		};		 

		//--------------------------------------------------------------------------------
		// CDelimiterPr 22.1.2.31 (Delimiter Properties) 
		//--------------------------------------------------------------------------------
		class CDelimiterPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDelimiterPr)
			CDelimiterPr()
			{
			}
			virtual ~CDelimiterPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:begChr") == sName )
								m_oBegChr = new OOX::Logic::CBegChr( oItem );
							else if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
							else if ( _T("m:endChr") == sName )
								m_oEndChr = new OOX::Logic::CEndChr( oItem );
							else if ( _T("m:grow") == sName )
								m_oGrow = new OOX::Logic::CGrow( oItem );
							else if ( _T("m:sepChr") == sName )
								m_oSepChr = new OOX::Logic::CSepChr( oItem );
							else if ( _T("m:shp") == sName )
								m_oShp = new OOX::Logic::CShp( oItem );

						}
					}
				}			
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:begChr") == sName )
						m_oBegChr = oReader;
					else if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:endChr") == sName )
						m_oEndChr = oReader;
					else if ( _T("m:grow") == sName )
						m_oGrow = oReader;
					else if ( _T("m:sepChr") == sName )
						m_oSepChr = oReader;
					else if ( _T("m:shp") == sName )
						m_oShp = oReader;
				}
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<m:dPr>");

				if ( m_oBegChr.IsInit() )
					sResult += m_oBegChr->toXML();

				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();

				if ( m_oEndChr.IsInit() )
					sResult += m_oEndChr->toXML();

				if ( m_oGrow.IsInit() )
					sResult += m_oGrow->toXML();

				if ( m_oSepChr.IsInit() )
					sResult += m_oSepChr->toXML();

				if ( m_oShp.IsInit() )
					sResult += m_oShp->toXML();
				
				sResult += _T("</m:dPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_dPr;
			}
		public:				
				//Childs
				nullable<OOX::Logic::CBegChr>		m_oBegChr;
				nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
				nullable<OOX::Logic::CEndChr>		m_oEndChr;
				nullable<OOX::Logic::CGrow>			m_oGrow;
				nullable<OOX::Logic::CSepChr>		m_oSepChr;
				nullable<OOX::Logic::CShp>			m_oShp;
		};		
		//--------------------------------------------------------------------------------
		// CDelimiter 22.1.2.24 (Delimiter Object) 
		//--------------------------------------------------------------------------------
		class CDelimiter : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CDelimiter)
			CDelimiter()
			{
				m_lColumn = 0;
			}
			virtual ~CDelimiter()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_lColumn = 0;
				
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:dPr") == sName )
								pItem = new CDelimiterPr( oItem );
							else if ( _T("m:e") == sName )
							{
								pItem = new CElement( oItem );
								//нужно заранее знать колисество столбцов для отрисовки
								m_lColumn++;
							}

							if ( pItem )
								m_arrItems.push_back( pItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				m_lColumn = 0;

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();				
					WritingElement *pItem = NULL;

					if ( _T("m:dPr") == sName )
						pItem = new CDelimiterPr( oReader );
					else if ( _T("m:e") == sName )
					{
						pItem = new CElement( oReader );
						//нужно заранее знать колисество столбцов для отрисовки
						m_lColumn++;
					}

					if ( pItem )
						m_arrItems.push_back( pItem );
					
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:d>");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }
				
				sResult += _T("</m:d>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_m_d;
			}
		public:
			// Childs
			LONG						   m_lColumn;
		};
		//--------------------------------------------------------------------------------
		// CEqArrPr 22.1.2.35 (Array Object)  
		//--------------------------------------------------------------------------------
		class CEqArrPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEqArrPr)
			CEqArrPr()
			{
			}
			virtual ~CEqArrPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:baseJc") == sName )
								m_oBaseJc = new OOX::Logic::CBaseJc( oItem );
							else if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
							else if ( _T("m:maxDist") == sName )
								m_oMaxDist = new OOX::Logic::CMaxDist( oItem );
							else if ( _T("m:objDist") == sName )
								m_oObjDist = new OOX::Logic::CObjDist( oItem );
							else if ( _T("m:rSp") == sName )
								m_oRSp = new OOX::Logic::CRSp( oItem );
							else if ( _T("m:rSpRule") == sName )
								m_oRSpRule = new OOX::Logic::CRSpRule( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:baseJc") == sName )
						m_oBaseJc = oReader;
					else if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:maxDist") == sName )
						m_oMaxDist = oReader;
					else if ( _T("m:objDist") == sName )
						m_oObjDist = oReader;
					else if ( _T("m:rSp") == sName )
						m_oRSp = oReader;
					else if ( _T("m:rSpRule") == sName )
						m_oRSpRule = oReader;
				}
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<m:eqArrPr>");

				if ( m_oBaseJc.IsInit() )
					sResult += m_oBaseJc->toXML();

				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();

				if ( m_oMaxDist.IsInit() )
					sResult += m_oMaxDist->toXML();

				if ( m_oObjDist.IsInit() )
					sResult += m_oObjDist->toXML();

				if ( m_oRSp.IsInit() )
					sResult += m_oRSp->toXML();

				if ( m_oRSpRule.IsInit() )
					sResult += m_oRSpRule->toXML();
				
				sResult += _T("</m:eqArrPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_eqArrPr;
			}
		public:				
			//Childs
			nullable<OOX::Logic::CBaseJc>		m_oBaseJc;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
			nullable<OOX::Logic::CMaxDist>		m_oMaxDist;
			nullable<OOX::Logic::CObjDist>		m_oObjDist;
			nullable<OOX::Logic::CRSp>			m_oRSp;
			nullable<OOX::Logic::CRSpRule>		m_oRSpRule;
		};
		//--------------------------------------------------------------------------------
		// CEqArr 22.1.2.34 (Array Object)  
		//--------------------------------------------------------------------------------
		class CEqArr : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CEqArr)
			CEqArr()
			{
				m_lRow = 0;
			}
			virtual ~CEqArr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_lRow = 0;
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:eqArrPr") == sName )
								pItem = new CEqArrPr( oItem );
							else if ( _T("m:e") == sName )
								pItem = new CElement( oItem );

							if ( pItem )
								m_arrItems.push_back( pItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				m_lRow = 0;
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();				
					WritingElement *pItem = NULL;

					if ( _T("m:eqArrPr") == sName )
						pItem = new CEqArrPr( oReader );
					else if ( _T("m:e") == sName )
					{
						pItem = new CElement( oReader );
						m_lRow++;
					}
					if ( pItem )
						m_arrItems.push_back( pItem );					
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:eqArr>");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if ( m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }

				sResult += _T("</m:eqArr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_eqArr;
			}
		public:
			// Childs
			LONG    m_lRow;
		};				
		//--------------------------------------------------------------------------------
		// CFPr 22.1.2.38 (Fraction Properties) 
		//--------------------------------------------------------------------------------
		class CFPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFPr)
			CFPr()
			{
			}
			virtual ~CFPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:type") == sName )
								m_oType = new OOX::Logic::CType( oItem );
							else if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:type") == sName )
						m_oType = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:fPr>");

				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();

				if ( m_oType.IsInit() )
					sResult += m_oType->toXML();
				
				sResult += _T("</m:fPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_fPr;
			}
	//Childs
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
			nullable<OOX::Logic::CType>			m_oType;
		};		
		//--------------------------------------------------------------------------------
		// CFraction 22.1.2.36 (Fraction Object)  
		//--------------------------------------------------------------------------------
		class CFraction : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFraction)
			CFraction()
			{
			}
			virtual ~CFraction()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:den") == sName )
								m_oDen = new OOX::Logic::CDen( oItem );
							else if ( _T("m:fPr") == sName )
								m_oFPr = new OOX::Logic::CFPr( oItem );
							else if ( _T("m:num") == sName )
								m_oNum = new OOX::Logic::CNum( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:den") == sName )
						m_oDen = oReader;
					else if ( _T("m:fPr") == sName )
						m_oFPr = oReader;
					else if ( _T("m:num") == sName )
						m_oNum = oReader;
				}
			}
			virtual std::wstring      toXML() const;

			virtual EElementType getType() const
			{
				return et_m_f;
			}
		
			//Childs
			nullable<OOX::Logic::CDen>			m_oDen;
			nullable<OOX::Logic::CFPr>			m_oFPr;
			nullable<OOX::Logic::CNum>			m_oNum;
		};		
		//--------------------------------------------------------------------------------
		// CFuncPr 22.1.2.40 (Function Properties) 
		//--------------------------------------------------------------------------------
		class CFuncPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFuncPr)
			CFuncPr()
			{
			}
			virtual ~CFuncPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:funcPr>");

				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();
				
				sResult += _T("</m:funcPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_funcPr;
			}
	//Childs
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
		};		
		//--------------------------------------------------------------------------------
		// CFunc 22.1.2.39 (Function Apply Object) 
		//--------------------------------------------------------------------------------
		class CFunc : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFunc)
			CFunc()
			{
			}
			virtual ~CFunc()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );
							else if ( _T("m:fName") == sName )
								m_oFName = new OOX::Logic::CFName( oItem );
							else if ( _T("m:funcPr") == sName )
								m_oFuncPr = new OOX::Logic::CFuncPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:fName") == sName )
						m_oFName = oReader;
					else if ( _T("m:funcPr") == sName )
						m_oFuncPr = oReader;
				}
			}
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_m_func;
			}
	//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CFName>		m_oFName;
			nullable<OOX::Logic::CFuncPr>		m_oFuncPr;
		};		
		//--------------------------------------------------------------------------------
		// CGroupChrPr 22.1.2.42 (Group-Character Properties ) 
		//--------------------------------------------------------------------------------
		class CGroupChrPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGroupChrPr)
			CGroupChrPr()
			{
			}
			virtual ~CGroupChrPr()
			{
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:chr") == sName )
								m_oChr = new OOX::Logic::CChr( oItem );
							else if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
							else if ( _T("m:pos") == sName )
								m_oPos = new OOX::Logic::CPos( oItem );
							else if ( _T("m:vertJc") == sName )
								m_oVertJc = new OOX::Logic::CVertJc( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:chr") == sName )
						m_oChr = oReader;
					else if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:pos") == sName )
						m_oPos = oReader;
					else if ( _T("m:vertJc") == sName )
						m_oVertJc = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:groupChrPr>");

				if ( m_oChr.IsInit() )
					sResult += m_oChr->toXML();

				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();

				if ( m_oPos.IsInit() )
					sResult += m_oPos->toXML();

				if ( m_oVertJc.IsInit() )
					sResult += m_oVertJc->toXML();
				
				sResult += _T("</m:groupChrPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_groupChrPr;
			}
		public:				
			//Childs
			nullable<OOX::Logic::CChr>			m_oChr;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
			nullable<OOX::Logic::CPos>			m_oPos;
			nullable<OOX::Logic::CVertJc>		m_oVertJc;
		};

		//--------------------------------------------------------------------------------
		// CGroupChr 22.1.2.41 (Group-Character Object) 
		//--------------------------------------------------------------------------------
		class CGroupChr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGroupChr)
			CGroupChr()
			{
			}
			virtual ~CGroupChr()
			{
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );
							else if ( _T("m:groupChrPr") == sName )
								m_oGroupChrPr = new OOX::Logic::CGroupChrPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:groupChrPr") == sName )
						m_oGroupChrPr = oReader;
				}
			}
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_m_groupChr;
			}
		public:				
			//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CGroupChrPr>	m_oGroupChrPr;
		};

		//--------------------------------------------------------------------------------
		// CLimLowPr 22.1.2.55 (Lower-Limit Properties)
		//--------------------------------------------------------------------------------
		class CLimLowPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLimLowPr)
			CLimLowPr()
			{
			}
			virtual ~CLimLowPr()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:limLowPr>");

				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();
				
				sResult += _T("</m:limLowPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_limLowPr;
			}				
	//Childs
			nullable<OOX::Logic::CCtrlPr> m_oCtrlPr;
		};		 
		//--------------------------------------------------------------------------------
		// CLimLow 22.1.2.54 (Lower-Limit Object) 
		//--------------------------------------------------------------------------------
		class CLimLow : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLimLow)
			CLimLow()
			{
			}
			virtual ~CLimLow()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );
							else if ( _T("m:lim") == sName )
								m_oLim = new OOX::Logic::CLim( oItem );
							else if ( _T("m:limLowPr") == sName )
								m_oLimLowPr = new OOX::Logic::CLimLowPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:lim") == sName )
						m_oLim = oReader;
					else if ( _T("m:limLowPr") == sName )
						m_oLimLowPr = oReader;
				}
			}
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_m_limLow;
			}	
			
	//Childs
			nullable<OOX::Logic::CElement>		 m_oElement;
			nullable<OOX::Logic::CLim>			 m_oLim;
			nullable<OOX::Logic::CLimLowPr>		 m_oLimLowPr;
		};		
		//--------------------------------------------------------------------------------
		// CLimUppPr 22.1.2.57 (Upper-Limit Properties) 
		//--------------------------------------------------------------------------------
		class CLimUppPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLimUppPr)
			CLimUppPr()
			{
			}
			virtual ~CLimUppPr()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:limUppPr>");

				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();
				
				sResult += _T("</m:limUppPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_limUppPr;
			}
		public:				
			//Childs
			nullable<OOX::Logic::CCtrlPr> m_oCtrlPr;
		};		
		//--------------------------------------------------------------------------------
		// CLimUpp 22.1.2.56 (Upper-Limit Object)
		//--------------------------------------------------------------------------------
		class CLimUpp : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLimUpp)
			CLimUpp()
			{
			}
			virtual ~CLimUpp()
			{
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );
							else if ( _T("m:lim") == sName )
								m_oLim = new OOX::Logic::CLim( oItem );
							else if ( _T("m:limUppPr") == sName )
								m_oLimUppPr = new OOX::Logic::CLimUppPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:lim") == sName )
						m_oLim = oReader;
					else if ( _T("m:limUppPr") == sName )
						m_oLimUppPr = oReader;
				}
			}
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_m_limUpp;
			}
		public:				
			//Childs
			nullable<OOX::Logic::CElement>		 m_oElement;
			nullable<OOX::Logic::CLim>			 m_oLim;
			nullable<OOX::Logic::CLimUppPr>		 m_oLimUppPr;
		};			
		//--------------------------------------------------------------------------------
		// CMathFont 22.1.2.61 (Math Font) 
		//--------------------------------------------------------------------------------
		class CMathFont : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMathFont)
			CMathFont()
			{
			}
			virtual ~CMathFont()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("m:val"), m_val );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:mathFont m:val=\"");
				sResult += m_val.get2();
				sResult += _T("\" />");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_m_mathFont;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("m:val"), m_val )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			// Attr
			nullable<std::wstring> m_val;
		};			 

		//--------------------------------------------------------------------------------
		// CMathPr 22.1.2.62 (Math Properties) 
		//--------------------------------------------------------------------------------
		class CMathPr : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CMathPr)
			CMathPr()
			{
			}
			virtual ~CMathPr()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:brkBin") == sName )
								pItem = new Logic::CBrkBin( oItem );
							else if ( _T("m:brkBinSub") == sName )
								pItem = new CBrkBinSub( oItem );
							else if ( _T("m:defJc") == sName )
								pItem = new CDefJc( oItem );
							else if ( _T("w:dispDef") == sName )
								pItem = new CDispDef( oItem );
							else if ( _T("w:interSp") == sName )
								pItem = new CInterSp( oItem );
							else if ( _T("m:intLim") == sName )
								pItem = new CIntLim( oItem );
							else if ( _T("m:intraSp") == sName )
								pItem = new CIntraSp( oItem );							
							else if ( _T("m:lMargin") == sName )
								pItem = new CLMargin( oItem );
							else if ( _T("m:mathFont") == sName )
								pItem = new CMathFont( oItem );
							else if ( _T("m:naryLim") == sName )
								pItem = new CNaryLim( oItem );
							else if ( _T("m:postSp") == sName )
								pItem = new CPostSp( oItem );
							else if ( _T("m:preSp") == sName )
								pItem = new CPreSp( oItem );
							else if ( _T("m:rMargin") == sName )
								pItem = new CRMargin( oItem );
							else if ( _T("m:smallFrac") == sName )
								pItem = new CSmallFrac( oItem );
							else if ( _T("m:wrapIndent") == sName )
								pItem = new CWrapIndent( oItem );
							else if ( _T("m:wrapRight") == sName )
								pItem = new CWrapRight( oItem );

							if ( pItem )
								m_arrItems.push_back( pItem );
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
					WritingElement *pItem = NULL;

					if ( _T("m:brkBin") == sName )
						pItem = new Logic::CBrkBin( oReader );
					else if ( _T("m:brkBinSub") == sName )
						pItem = new CBrkBinSub( oReader );
					else if ( _T("m:defJc") == sName )
						pItem = new CDefJc( oReader );
					else if ( _T("w:dispDef") == sName )
						pItem = new CDispDef( oReader );
					else if ( _T("w:interSp") == sName )
						pItem = new CInterSp( oReader );
					else if ( _T("m:intLim") == sName )
						pItem = new CIntLim( oReader );
					else if ( _T("m:intraSp") == sName )
						pItem = new CIntraSp( oReader );							
					else if ( _T("m:lMargin") == sName )
						pItem = new CLMargin( oReader );
					else if ( _T("m:mathFont") == sName )
						pItem = new CMathFont( oReader );
					else if ( _T("m:naryLim") == sName )
						pItem = new CNaryLim( oReader );
					else if ( _T("m:postSp") == sName )
						pItem = new CPostSp( oReader );
					else if ( _T("m:preSp") == sName )
						pItem = new CPreSp( oReader );
					else if ( _T("m:rMargin") == sName )
						pItem = new CRMargin( oReader );
					else if ( _T("m:smallFrac") == sName )
						pItem = new CSmallFrac( oReader );
					else if ( _T("m:wrapIndent") == sName )
						pItem = new CWrapIndent( oReader );
					else if ( _T("m:wrapRight") == sName )
						pItem = new CWrapRight( oReader );

					if ( pItem )
						m_arrItems.push_back( pItem );
					
				}
			}
			virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("<m:mathPr>");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }

				sResult += _T("</m:mathPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_mathPr;
			}
		public:
			// Childs
		};		
		//--------------------------------------------------------------------------------
		// CMcPr 22.1.2.66 (Matrix Column Properties) 
		//--------------------------------------------------------------------------------
		class CMcPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMcPr)
			CMcPr()
			{
			}
			virtual ~CMcPr()
			{
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:count") == sName )
								m_oCount = new OOX::Logic::CCount( oItem );
							else if ( _T("m:mcJc") == sName )
								m_oMcJc = new OOX::Logic::CMcJc( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:count") == sName )
						m_oCount = oReader;
					else if ( _T("m:mcJc") == sName )
						m_oMcJc = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:mcPr>");

				if ( m_oCount.IsInit() )
					sResult += m_oCount->toXML();

				if ( m_oMcJc.IsInit() )
					sResult += m_oMcJc->toXML();
				
				sResult += _T("</m:mcPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_mcPr;
			}
		public:				
			//Childs
			nullable<OOX::Logic::CCount>		m_oCount;
			nullable<OOX::Logic::CMcJc>			m_oMcJc;
		};		
		//--------------------------------------------------------------------------------
		// CMc 22.1.2.64  (Matrix Column)
		//--------------------------------------------------------------------------------
		class CMc : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMc)
			CMc()
			{
			}
			virtual ~CMc()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:mcPr") == sName )
								m_oMcPr = new OOX::Logic::CMcPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:mcPr") == sName )
						m_oMcPr = oReader;
				}
			}
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return et_m_mc;
			}
	//Childs
			nullable<OOX::Logic::CMcPr>		m_oMcPr;
		};		
		//--------------------------------------------------------------------------------
		// CMcs 22.1.2.67 (Matrix Columns) 
		//--------------------------------------------------------------------------------
		class CMcs : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CMcs)
			CMcs()
			{
			}
			virtual ~CMcs()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;
							
							if ( _T("m:mc") == sName )
								pItem = new Logic::CMc( oItem );

							if ( pItem )
								m_arrItems.push_back( pItem );
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
					WritingElement *pItem = NULL;

					if ( _T("m:mc") == sName )
						pItem = new Logic::CMc( oReader );

					if ( pItem )
						m_arrItems.push_back( pItem );
					
				}
			}

			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:mcs>");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }

				sResult += _T("</m:mcs>");

				return sResult;
			}


			virtual EElementType getType() const
			{
				return et_m_mcs;
			}
		public:				
			//Childs
		};
		//--------------------------------------------------------------------------------
		// CMPr 22.1.2.68   (Matrix Properties) 
		//--------------------------------------------------------------------------------
		class CMPr : public WritingElement
		{
		public:
			CMPr()
			{
			}
			CMPr(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CMPr(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:baseJc") == sName )
								m_oBaseJc = new OOX::Logic::CBaseJc( oItem );
							else if ( _T("m:cGp") == sName )
								m_oCGp = new OOX::Logic::CCGp( oItem );
							else if ( _T("m:cGpRule") == sName )
								m_oCGpRule = new OOX::Logic::CCGpRule( oItem );
							else if ( _T("m:cSp") == sName )
								m_oCSp = new OOX::Logic::CCSp( oItem );
							else if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
							else if ( _T("m:mcs") == sName )
								m_oMcs = new OOX::Logic::CMcs( oItem );
							else if ( _T("m:plcHide") == sName )
								m_oPlcHide = new OOX::Logic::CPlcHide( oItem );
							else if ( _T("m:rSp") == sName )
								m_oRSp = new OOX::Logic::CRSp( oItem );
							else if ( _T("m:rSpRule") == sName )
								m_oRSpRule = new OOX::Logic::CRSpRule( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if ( _T("m:baseJc") == sName )
						m_oBaseJc = oReader;
					else if ( _T("m:cGp") == sName )
						m_oCGp = oReader;						
					else if ( _T("m:cGpRule") == sName )
						m_oCGpRule = oReader;
					else if ( _T("m:cSp") == sName )
						m_oCSp = oReader;
					else if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:mcs") == sName )
						m_oMcs = oReader;
					else if ( _T("m:plcHide") == sName )
						m_oPlcHide = oReader;
					else if ( _T("m:rSp") == sName )
						m_oRSp = oReader;
					else if ( _T("m:rSpRule") == sName )
						m_oRSpRule = oReader;							
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:mPr>");

				if ( m_oBaseJc.IsInit() )
					sResult += m_oBaseJc->toXML();
				if ( m_oCGp.IsInit() )
					sResult += m_oCGp->toXML();
				if ( m_oCGpRule.IsInit() )
					sResult += m_oCGpRule->toXML();
				if ( m_oCSp.IsInit() )
					sResult += m_oCSp->toXML();
				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();
				if ( m_oMcs.IsInit() )
					sResult += m_oMcs->toXML();
				if ( m_oPlcHide.IsInit() )
					sResult += m_oPlcHide->toXML();
				if ( m_oRSp.IsInit() )
					sResult += m_oRSp->toXML();
				if ( m_oRSpRule.IsInit() )
					sResult += m_oRSpRule->toXML();
				
				sResult += _T("</m:mPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_mPr;
			}
		public:
			// Childs
			nullable<OOX::Logic::CBaseJc>		m_oBaseJc;
			nullable<OOX::Logic::CCGp>			m_oCGp;
			nullable<OOX::Logic::CCGpRule>		m_oCGpRule;
			nullable<OOX::Logic::CCSp>			m_oCSp;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
			nullable<OOX::Logic::CMcs>			m_oMcs;
			nullable<OOX::Logic::CPlcHide>		m_oPlcHide;
			nullable<OOX::Logic::CRSp>			m_oRSp;
			nullable<OOX::Logic::CRSpRule>		m_oRSpRule;

		};
		//--------------------------------------------------------------------------------
		// CMr 22.1.2.69 (Matrix Row)
		//--------------------------------------------------------------------------------
		class CMr : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CMr)
			CMr()
			{
				m_lCol = 0;
			}
			virtual ~CMr()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_lCol = 0;
				
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;
							
							if ( _T("m:e") == sName )
							{
								pItem = new Logic::CElement( oItem );
								m_lCol++;
							}

							if ( pItem )
								m_arrItems.push_back( pItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				m_lCol = 0;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();				
					WritingElement *pItem = NULL;

					if ( _T("m:e") == sName )
					{
						pItem = new Logic::CElement( oReader );
						m_lCol++;
					}

					if ( pItem )
						m_arrItems.push_back( pItem );
					
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:mr>");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }
				
				sResult += _T("</m:mr>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_m_mr;
			}
			LONG GetCol()
			{
				return m_lCol;
			}
		public:
			// Childs
			LONG	 m_lCol;
		};
		//--------------------------------------------------------------------------------
		// CMatrix 22.1.2.60 (Matrix Object) 
		//--------------------------------------------------------------------------------
		class CMatrix : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CMatrix)
			CMatrix()
			{
				m_lRow = 0;
			}
			virtual ~CMatrix()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_lRow = 0;

				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;
							
							if ( _T("m:mPr") == sName )
								pItem = new CMPr( oItem );
							else if ( _T("m:mr") == sName )
							{
								pItem = new CMr( oItem );
								m_lRow++;
							}
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				m_lRow = 0;
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();				
					WritingElement *pItem = NULL;

					if ( _T("m:mPr") == sName )
						pItem = new CMPr( oReader );
					else if ( _T("m:mr") == sName )
					{
						pItem = new CMr( oReader );
						m_lRow++;
					}

					if ( pItem )
						m_arrItems.push_back( pItem );
					
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:m>");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }
				
				sResult += _T("</m:m>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_m;
			}
	// Childs
			LONG m_lRow;
		};
		//--------------------------------------------------------------------------------
		// CNaryPr 22.1.2.72   (n-ary Properties) 
		//--------------------------------------------------------------------------------
		class CNaryPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNaryPr)
			CNaryPr()
			{
			}
			virtual ~CNaryPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
							else if ( _T("m:chr") == sName )
								m_oChr = new OOX::Logic::CChr( oItem );
							else if ( _T("m:grow") == sName )
								m_oGrow = new OOX::Logic::CGrow( oItem );
							else if ( _T("m:limLoc") == sName )
								m_oLimLoc = new OOX::Logic::CLimLoc( oItem );
							else if ( _T("m:subHide") == sName )
								m_oSubHide = new OOX::Logic::CSubHide( oItem );
							else if ( _T("m:supHide") == sName )
								m_oSupHide = new OOX::Logic::CSupHide( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:chr") == sName )
						m_oChr = oReader;
					else if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:grow") == sName )
						m_oGrow = oReader;
					else if ( _T("m:limLoc") == sName )
						m_oLimLoc = oReader;
					else if ( _T("m:subHide") == sName )
						m_oSubHide = oReader;
					else if ( _T("m:supHide") == sName )
						m_oSupHide = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:naryPr>");

				if ( m_oChr.IsInit() )
					sResult += m_oChr->toXML();

				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();

				if ( m_oGrow.IsInit() )
					sResult += m_oGrow->toXML();

				if ( m_oLimLoc.IsInit() )
					sResult += m_oLimLoc->toXML();

				if ( m_oSubHide.IsInit() )
					sResult += m_oSubHide->toXML();

				if ( m_oSupHide.IsInit() )
					sResult += m_oSupHide->toXML();
				
				sResult += _T("</m:naryPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_naryPr;
			}
	//Childs
			nullable<OOX::Logic::CChr>				m_oChr;
			nullable<OOX::Logic::CCtrlPr>			m_oCtrlPr;
			nullable<OOX::Logic::CGrow>				m_oGrow;
			nullable<OOX::Logic::CLimLoc>			m_oLimLoc;
			nullable<OOX::Logic::CSubHide>			m_oSubHide;
			nullable<OOX::Logic::CSupHide>			m_oSupHide;
		};		
		//--------------------------------------------------------------------------------
		// CNary 22.1.2.70 (n-ary Operator Object)
		//--------------------------------------------------------------------------------
		class CNary : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNary)
			CNary()
			{
			}
			virtual ~CNary()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );
							else if ( _T("m:naryPr") == sName )
								m_oNaryPr = new OOX::Logic::CNaryPr( oItem );
							else if ( _T("m:sub") == sName )
								m_oSub = new OOX::Logic::CSub( oItem );
							else if ( _T("m:sup") == sName )
								m_oSup = new OOX::Logic::CSup( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:naryPr") == sName )
						m_oNaryPr = oReader;
					else if ( _T("m:sub") == sName )
						m_oSub = oReader;
					else if ( _T("m:sup") == sName )
						m_oSup = oReader;
				}
			}
			virtual std::wstring      toXML() const;

			virtual EElementType getType() const
			{
				return et_m_nary;
			}
	//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CNaryPr>		m_oNaryPr;
			nullable<OOX::Logic::CSub>			m_oSub;
			nullable<OOX::Logic::CSup>			m_oSup;
		};		
		//--------------------------------------------------------------------------------
		// COMathParaPr 22.1.2.79 
		//--------------------------------------------------------------------------------
		class COMathParaPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COMathParaPr)
			COMathParaPr()
			{
			}
			virtual ~COMathParaPr()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:jc") == sName )
								m_oMJc = new OOX::Logic::CMJc( oItem );
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
					if ( _T("m:jc") == sName )
						m_oMJc = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:oMathParaPr>");

				if ( m_oMJc.IsInit() )
					sResult += m_oMJc->toXML();
				
				sResult += _T("</m:oMathParaPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_oMathParaPr;
			}
	//Childs			
			nullable<OOX::Logic::CMJc>  m_oMJc;
		};			
		//--------------------------------------------------------------------------------
		// CPhantPr 22.1.2.82   (Phantom Properties) 
		//--------------------------------------------------------------------------------
		class CPhantPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPhantPr)
			CPhantPr()
			{
			}
			virtual ~CPhantPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
							else if ( _T("m:show") == sName )
								m_oShow = new OOX::Logic::CShow( oItem );
							else if ( _T("m:transp") == sName )
								m_oTransp = new OOX::Logic::CTransp( oItem );
							else if ( _T("m:zeroAsc") == sName )
								m_oZeroAsc = new OOX::Logic::CZeroAsc( oItem );
							else if ( _T("m:zeroDesc") == sName )
								m_oZeroDesc = new OOX::Logic::CZeroDesc( oItem );
							else if ( _T("m:zeroWid") == sName )
								m_oZeroWid = new OOX::Logic::CZeroWid( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:show") == sName )
						m_oShow = oReader;
					else if ( _T("m:transp") == sName )
						m_oTransp = oReader;
					else if ( _T("m:zeroAsc") == sName )
						m_oZeroAsc = oReader;
					else if ( _T("m:zeroDesc") == sName )
						m_oZeroDesc = oReader;
					else if ( _T("m:zeroWid") == sName )
						m_oZeroWid = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:phantPr>");

				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();

				if ( m_oShow.IsInit() )
					sResult += m_oShow->toXML();

				if ( m_oTransp.IsInit() )
					sResult += m_oTransp->toXML();

				if ( m_oZeroAsc.IsInit() )
					sResult += m_oZeroAsc->toXML();

				if ( m_oZeroDesc.IsInit() )
					sResult += m_oZeroDesc->toXML();

				if ( m_oZeroWid.IsInit() )
					sResult += m_oZeroWid->toXML();
				
				sResult += _T("</m:phantPr>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_phantPr;
			}
	//Childs			
			nullable<OOX::Logic::CCtrlPr>			m_oCtrlPr;
			nullable<OOX::Logic::CShow>				m_oShow;
			nullable<OOX::Logic::CTransp>			m_oTransp;
			nullable<OOX::Logic::CZeroAsc>			m_oZeroAsc;
			nullable<OOX::Logic::CZeroDesc>			m_oZeroDesc;
			nullable<OOX::Logic::CZeroWid>			m_oZeroWid;
		};		 
		//--------------------------------------------------------------------------------
		// CPhant 22.1.2.81   (Phantom Object) 
		//--------------------------------------------------------------------------------
		class CPhant : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPhant)
			CPhant()
			{
			}
			virtual ~CPhant()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );
							else if ( _T("m:phantPr") == sName )
								m_oPhantPr = new OOX::Logic::CPhantPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:phantPr") == sName )
						m_oPhantPr = oReader;
				}
			}
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_m_phant;
			}
	//Childs			
			nullable<OOX::Logic::CElement>  m_oElement;
			nullable<OOX::Logic::CPhantPr>  m_oPhantPr;
		};

		//--------------------------------------------------------------------------------
		// CMText 22.1.2.116  (Math Text) 
		//--------------------------------------------------------------------------------	
		class CMText : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMText)
			CMText() 
			{
			}
			virtual ~CMText() 
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("xml:space"), m_oSpace );

				m_sText = oNode.GetText();
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sText = oReader.GetText2();
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult;
				
				if ( m_oSpace.IsInit() )
					sResult = _T("<m:t xml:space=\"") + m_oSpace->ToString() + _T("\">");
				else
					sResult = _T("<m:t>");

				sResult += XmlUtils::EncodeXmlString(m_sText);
				sResult += _T("</m:t>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_t;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("xml:space") == wsName )
					{
						m_oSpace = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}
		public:
	// Attributes
			nullable<SimpleTypes::CXmlSpace<> > m_oSpace;
	// Value
			std::wstring						m_sText;

		};		
		//--------------------------------------------------------------------------------
		// CMRPr 22.1.2.91   (Run Properties) 
		//--------------------------------------------------------------------------------
		class CMRPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMRPr)
			CMRPr()
			{
			}
			virtual ~CMRPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:aln") == sName )
								m_oAln = new OOX::Logic::CAln( oItem );
							else if ( _T("m:brk") == sName )
								m_oBrk = new OOX::Logic::CBrk( oItem );
							else if ( _T("m:lit") == sName )
								m_oLit = new OOX::Logic::CLit( oItem );
							else if ( _T("m:nor") == sName )
								m_oNor = new OOX::Logic::CNor( oItem );
							else if ( _T("m:scr") == sName )
								m_oScr = new OOX::Logic::CScr( oItem );
							else if ( _T("m:sty") == sName )
								m_oSty = new OOX::Logic::CSty( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:aln") == sName )
						m_oAln = oReader;
					else if ( _T("m:brk") == sName )
						m_oBrk = oReader;
					else if ( _T("m:lit") == sName )
						m_oLit = oReader;
					else if ( _T("m:nor") == sName )
						m_oNor = oReader;
					else if ( _T("m:scr") == sName )
						m_oScr = oReader;
					else if ( _T("m:sty") == sName )
						m_oSty = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:rPr>");
				
				if ( m_oAln.IsInit() )
					sResult += m_oAln->toXML();

				if ( m_oBrk.IsInit() )
					sResult += m_oBrk->toXML();

				if ( m_oLit.IsInit() )
					sResult += m_oLit->toXML();

				if ( m_oNor.IsInit() )
					sResult += m_oNor->toXML();

				if ( m_oScr.IsInit() )
					sResult += m_oScr->toXML();

				if ( m_oSty.IsInit() )
					sResult += m_oSty->toXML();

				sResult += _T("</m:rPr>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_rPr;
			}
	//Childs
			nullable<OOX::Logic::CAln>			m_oAln;
			nullable<OOX::Logic::CBrk>			m_oBrk;
			nullable<OOX::Logic::CLit>			m_oLit;
			nullable<OOX::Logic::CNor>			m_oNor;
			nullable<OOX::Logic::CScr>			m_oScr;
			nullable<OOX::Logic::CSty>			m_oSty;
		};		
		class CMDel;
		class CMIns;
		//--------------------------------------------------------------------------------
		// CMRun 22.1.2.87  (Math Run)
		//--------------------------------------------------------------------------------
		class CMRun : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMRun)
			CMRun()
			{
			}
			virtual ~CMRun()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							
							if ( _T("w:annotationRef") == sName )
								m_oAnnotationRef = oItem;
							else if ( _T("w:br") == sName )
								m_oBr = oItem;
							else if ( _T("w:commentReference") == sName )
								m_oCommentReference = oItem;
							else if ( _T("w:contentPart") == sName )
								m_oContentPart = oItem;
							else if ( _T("w:continuationSeparator") == sName )
								m_oContinuationSeparator = oItem;
							else if ( _T("w:cr") == sName )
								m_oCr = oItem;
							else if ( _T("w:dayLong") == sName )
								m_oDayLong = oItem;
							else if ( _T("w:dayShort") == sName )
								m_oDayShort = oItem;
							else if ( _T("w:del") == sName )
								m_oDel = oItem;
							else if ( _T("w:delInstrText") == sName )
								m_oDelInstrText = oItem;
							else if ( _T("w:delText") == sName )
								m_oDelText = oItem;
							else if ( _T("w:drawing") == sName ) 
								m_oDrawing = oItem;
							else if ( _T("w:endnoteRef") == sName )
								m_oEndnoteRef = oItem;
							else if ( _T("w:endnoteReference") == sName )
								m_oEndnoteReference = oItem;
							else if ( _T("w:fldChar") == sName )
								m_oFldChar = oItem;
							else if ( _T("w:footnoteRef") == sName )
								m_oFootnoteRef = oItem;
							else if ( _T("w:footnoteReference") == sName )
								m_oFootnoteReference = oItem;
							else if ( _T("w:ins") == sName )
								m_oIns = oItem;
							else if ( _T("w:instrText") == sName )
								m_oInstrText = oItem;
							else if ( _T("w:lastRenderedPageBreak") == sName )
								m_oLastRenderedPageBreak = oItem;
							else if ( _T("w:monthLong") == sName )
								m_oMonthLong = oItem;
							else if ( _T("w:monthShort") == sName )
								m_oMonthShort = oItem;
							else if ( _T("w:noBreakHyphen") == sName )
								m_oNoBreakHyphen = oItem;
							else if ( _T("w:object") == sName )
								m_oObject = oItem;
							else if ( _T("w:pgNum") == sName )
								m_oPgNum = oItem;
							else if ( _T("w:ptab") == sName )
								m_oPtab = oItem;
							else if ( _T("m:rPr") == sName )
								m_oMRPr = oItem ;
							else if ( _T("w:rPr") == sName )
								m_oRPr = oItem;
							else if ( _T("a:rPr") == sName )
								m_oARPr = oItem;
							else if ( _T("w:ruby") == sName )
								m_oRuby = oItem;
							else if ( _T("w:separator") == sName )
								m_oSeparator = oItem;
							else if ( _T("w:softHyphen") == sName )
								m_oSoftHyphen = oItem;
							else if ( _T("w:sym") == sName )
								m_oSym = oItem;
							else if ( _T("m:t") == sName )
								m_oMText = oItem ;
							else if ( _T("w:t") == sName )
								m_oText = oItem;
							else if ( _T("w:tab") == sName )
								m_oTab = oItem;
							else if ( _T("w:yearLong") == sName )
								m_oYearLong = oItem;
							else if ( _T("w:yearShort") == sName )
								m_oYearShort = oItem;
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("w:annotationRef") == sName )
						m_oAnnotationRef = oReader;
					else if ( _T("w:br") == sName )
						m_oBr = oReader;
					else if ( _T("w:commentReference") == sName )
						m_oCommentReference = oReader;
					else if ( _T("w:contentPart") == sName )
						m_oContentPart = oReader;
					else if ( _T("w:continuationSeparator") == sName )
						m_oContinuationSeparator = oReader;
					else if ( _T("w:cr") == sName )
						m_oCr = oReader;
					else if ( _T("w:dayLong") == sName )
						m_oDayLong = oReader;
					else if ( _T("w:dayShort") == sName )
						m_oDayShort = oReader;
					else if ( _T("w:del") == sName )
						m_oDel = oReader;
					else if ( _T("w:delInstrText") == sName )
						m_oDelInstrText = oReader;
					else if ( _T("w:delText") == sName )
						m_oDelText = oReader;
					else if ( _T("w:drawing") == sName ) 
						m_oDrawing = oReader;
					else if ( _T("w:endnoteRef") == sName )
						m_oEndnoteRef = oReader;
					else if ( _T("w:endnoteReference") == sName )
						m_oEndnoteReference = oReader;
					else if ( _T("w:fldChar") == sName )
						m_oFldChar = oReader;
					else if ( _T("w:footnoteRef") == sName )
						m_oFootnoteRef = oReader;
					else if ( _T("w:footnoteReference") == sName )
						m_oFootnoteReference = oReader;
					else if ( _T("w:ins") == sName )
						m_oIns = oReader;
					else if ( _T("w:instrText") == sName )
						m_oInstrText = oReader;
					else if ( _T("w:lastRenderedPageBreak") == sName )
						m_oLastRenderedPageBreak = oReader;
					else if ( _T("w:monthLong") == sName )
						m_oMonthLong = oReader;
					else if ( _T("w:monthShort") == sName )
						m_oMonthShort = oReader;
					else if ( _T("w:noBreakHyphen") == sName )
						m_oNoBreakHyphen = oReader;
					else if ( _T("w:object") == sName )
						m_oObject = oReader;
					else if ( _T("w:pgNum") == sName )
						m_oPgNum = oReader;
					else if ( _T("w:ptab") == sName )
						m_oPtab = oReader;
					else if ( _T("m:rPr") == sName )
						m_oMRPr = oReader ;
					else if ( _T("w:rPr") == sName )
						m_oRPr = oReader;
					else if ( _T("a:rPr") == sName )
					{
						std::wstring sXml = oReader.GetOuterXml();
						XmlUtils::CXmlNode node;
						node.FromXmlString(sXml);

						m_oARPr = node;
					}
					else if ( _T("w:ruby") == sName )
						m_oRuby = oReader;
					else if ( _T("w:separator") == sName )
						m_oSeparator = oReader;
					else if ( _T("w:softHyphen") == sName )
						m_oSoftHyphen = oReader;
					else if ( _T("w:sym") == sName )
						m_oSym = oReader;
					else if ( _T("m:t") == sName )
						m_oMText = oReader ;
					else if ( _T("w:t") == sName )
						m_oText = oReader;
					else if ( _T("w:tab") == sName )
						m_oTab = oReader;
					else if ( _T("w:yearLong") == sName )
						m_oYearLong = oReader;
					else if ( _T("w:yearShort") == sName )
						m_oYearShort = oReader;
				}
			}
			virtual std::wstring toXML() const;			
			virtual std::wstring toXMLInner() const;

			virtual EElementType getType() const
			{
				return et_m_r;
			}
	// Childs
			nullable<OOX::Logic::CAnnotationRef>			m_oAnnotationRef;
			nullable<OOX::Logic::CBr>						m_oBr;
			nullable<OOX::Logic::CCommentReference>			m_oCommentReference;
			nullable<OOX::Logic::CContentPart>				m_oContentPart;
			nullable<OOX::Logic::CContinuationSeparator>	m_oContinuationSeparator;
			nullable<OOX::Logic::CCr>						m_oCr;
			nullable<OOX::Logic::CDayLong>					m_oDayLong;
			nullable<OOX::Logic::CDayShort>					m_oDayShort;
			nullable<OOX::Logic::CMDel>						m_oDel;
			nullable<OOX::Logic::CDelInstrText>				m_oDelInstrText;
			nullable<OOX::Logic::CDelText>					m_oDelText;
			nullable<OOX::Logic::CDrawing>					m_oDrawing;
			nullable<OOX::Logic::CEndnoteRef>				m_oEndnoteRef;
			nullable<OOX::Logic::CEndnoteReference>			m_oEndnoteReference;
			nullable<OOX::Logic::CFldChar>					m_oFldChar;
			nullable<OOX::Logic::CFootnoteRef>				m_oFootnoteRef;
			nullable<OOX::Logic::CFootnoteReference>		m_oFootnoteReference;
			nullable<OOX::Logic::CMIns>						m_oIns;
			nullable<OOX::Logic::CInstrText>				m_oInstrText;
			nullable<OOX::Logic::CLastRenderedPageBreak>	m_oLastRenderedPageBreak;
			nullable<OOX::Logic::CMonthLong>				m_oMonthLong;
			nullable<OOX::Logic::CMonthShort>				m_oMonthShort;
			nullable<OOX::Logic::CNoBreakHyphen>			m_oNoBreakHyphen;
			nullable<OOX::Logic::CObject>					m_oObject;
			nullable<OOX::Logic::CPgNum>					m_oPgNum;
			nullable<OOX::Logic::CPTab>						m_oPtab;
			nullable<OOX::Logic::CRunProperty>				m_oRPr;
			nullable<OOX::Logic::CMRPr>						m_oMRPr;
			nullable<PPTX::Logic::RunProperties>			m_oARPr;
			nullable<OOX::Logic::CRuby>						m_oRuby;
			nullable<OOX::Logic::CSeparator>				m_oSeparator;
			nullable<OOX::Logic::CSoftHyphen>				m_oSoftHyphen;
			nullable<OOX::Logic::CSym>						m_oSym;
			nullable<OOX::Logic::CMText>					m_oMText;
			nullable<OOX::Logic::CText>						m_oText;
			nullable<OOX::Logic::CTab>						m_oTab;
			nullable<OOX::Logic::CYearLong>					m_oYearLong;
			nullable<OOX::Logic::CYearShort>				m_oYearShort;
		};
		class CMDel : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMDel)
			CMDel()
			{
			}
			virtual ~CMDel()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:author"), m_sAuthor );
				XmlMacroReadAttributeBase( oNode, _T("w:date"),   m_oDate  );
				XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );
				XmlMacroReadAttributeBase( oNode, _T("oouserid"), m_sUserId );

				m_oRun = oNode;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				m_oRun = oReader;
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult;

				sResult += _T("<w:del");

				if ( m_sAuthor.IsInit() )
				{
					sResult += _T(" w:author=\"");
					sResult += XmlUtils::EncodeXmlString(m_sAuthor.get2());
					sResult += _T("\" ");
				}

				if ( m_oDate.IsInit() )
				{
					sResult += _T(" w:date=\"");
					sResult += XmlUtils::EncodeXmlString(m_oDate->ToString());
					sResult += _T("\" ");
				}

				if ( m_oId.IsInit() )
				{
					sResult += _T(" w:id=\"");
					sResult += m_oId->ToString();
					sResult += _T("\" ");
				}

				if ( m_sUserId.IsInit() )
				{
					sResult += _T(" oouserid=\"");
					sResult += XmlUtils::EncodeXmlString(m_sUserId.get2());
					sResult += _T("\" ");
				}
				sResult += _T(">");

				if(m_oRun.IsInit())
				{
					sResult += m_oRun->toXMLInner();
				}

				sResult += _T("</w:del>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_ins;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate  )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring>				m_sAuthor;
			nullable<SimpleTypes::CDateTime>	m_oDate;
			nullable<SimpleTypes::CDecimalNumber<> > m_oId;
			nullable<std::wstring>				m_sUserId;
	// Childs
			nullable<CMRun>						m_oRun;
		};
		class CMIns : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CMIns)
			CMIns()
			{
			}
			virtual ~CMIns()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:author"), m_sAuthor );
				XmlMacroReadAttributeBase( oNode, _T("w:date"),   m_oDate  );
				XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );
				XmlMacroReadAttributeBase( oNode, _T("oouserid"), m_sUserId );

				m_oRun = oNode;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				m_oRun = oReader;
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult;

				sResult += _T("<w:ins");

				if ( m_sAuthor.IsInit() )
				{
					sResult += _T(" w:author=\"");
					sResult += XmlUtils::EncodeXmlString(m_sAuthor.get2());
					sResult += _T("\" ");
				}

				if ( m_oDate.IsInit() )
				{
					sResult += _T(" w:date=\"");
					sResult += XmlUtils::EncodeXmlString(m_oDate->ToString());
					sResult += _T("\" ");
				}

				if ( m_oId.IsInit() )
				{
					sResult += _T(" w:id=\"");
					sResult += m_oId->ToString();
					sResult += _T("\" ");
				}

				if ( m_sUserId.IsInit() )
				{
					sResult += _T(" oouserid=\"");
					sResult += XmlUtils::EncodeXmlString(m_sUserId.get2());
					sResult += _T("\" ");
				}
				sResult += _T(">");

				if(m_oRun.IsInit())
				{
					sResult += m_oRun->toXMLInner();
				}

				sResult += L"</w:ins>";
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_ins;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate  )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring                  >	m_sAuthor;
			nullable<SimpleTypes::CDateTime        >	m_oDate;
			nullable<SimpleTypes::CDecimalNumber<> >	m_oId;
			nullable<std::wstring                  >	m_sUserId;
			// Childs
			nullable<CMRun>								m_oRun;
		};
		//--------------------------------------------------------------------------------
		// CRadPr 22.1.2.89   (Radical Properties)
		//--------------------------------------------------------------------------------
		class CRadPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRadPr)
			CRadPr()
			{
			}
			virtual ~CRadPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
							else if ( _T("m:degHide") == sName )
								m_oDegHide = new OOX::Logic::CDegHide( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
					else if ( _T("m:degHide") == sName )
						m_oDegHide = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:radPr>");

				if ( m_oCtrlPr.IsInit() )
					sResult += m_oCtrlPr->toXML();

				if ( m_oDegHide.IsInit() )
					sResult += m_oDegHide->toXML();

				sResult += _T("</m:radPr>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_radPr;
			}
	//Childs
			nullable<OOX::Logic::CCtrlPr>				m_oCtrlPr;
			nullable<OOX::Logic::CDegHide>				m_oDegHide;
		};		
		//--------------------------------------------------------------------------------
		// CRad 22.1.2.88   (Radical Object)
		//--------------------------------------------------------------------------------
		class CRad : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRad)
			CRad()
			{
			}
			virtual ~CRad()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:radPr") == sName )
								m_oRadPr = new OOX::Logic::CRadPr( oItem );
							else if ( _T("m:deg") == sName )
								m_oDeg = new OOX::Logic::CDeg( oItem );
							else if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:deg") == sName )
						m_oDeg = oReader;
					else if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:radPr") == sName )
						m_oRadPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:rad>");

				if ( m_oRadPr.IsInit())
					sResult += m_oRadPr->toXML();

				if ( m_oDeg.IsInit() )
					sResult += m_oDeg->toXML();

				if ( m_oElement.IsInit() )
					sResult += m_oElement->toXML();

				sResult += _T("</m:rad>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_rad;
			}
	//Childs
			nullable<OOX::Logic::CDeg>				m_oDeg;
			nullable<OOX::Logic::CElement>			m_oElement;
			nullable<OOX::Logic::CRadPr>			m_oRadPr;
		};		 
		//--------------------------------------------------------------------------------
		// CSPrePr 22.1.2.100   (Pre-Sub-Superscript Properties)) 
		//--------------------------------------------------------------------------------
		class CSPrePr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSPrePr)
			CSPrePr()
			{
			}
			virtual ~CSPrePr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:sPrePr>");

				if (m_oCtrlPr.IsInit())
					sResult += m_oCtrlPr->toXML();

				sResult += _T("</m:sPrePr>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_sPrePr;
			}
	//Childs
			nullable<OOX::Logic::CCtrlPr>	m_oCtrlPr;
		};		
		//--------------------------------------------------------------------------------
		// CSPre 22.1.2.99   (Pre-Sub-Superscript Object) 
		//--------------------------------------------------------------------------------
		class CSPre : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSPre)
			CSPre()
			{
			}
			virtual ~CSPre()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:sPrePr") == sName )
								m_oSPrePr = new OOX::Logic::CSPrePr( oItem );
							else if ( _T("m:sub") == sName )
								m_oSub = new OOX::Logic::CSub( oItem );
							else if ( _T("m:sup") == sName )
								m_oSup = new OOX::Logic::CSup( oItem );
							else if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:sPrePr") == sName )
						m_oSPrePr = oReader;
					else if ( _T("m:sub") == sName )
						m_oSub = oReader;
					else if ( _T("m:sup") == sName )
						m_oSup = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:sPre>");

				if ( m_oSPrePr.IsInit() )
					sResult += m_oSPrePr->toXML();

				if ( m_oElement.IsInit() )
					sResult += m_oElement->toXML();

				if ( m_oSub.IsInit() )
					sResult += m_oSub->toXML();

				if ( m_oSup.IsInit() )
					sResult += m_oSup->toXML();

				sResult += _T("</m:sPre>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_sPre;
			}
	//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CSPrePr>		m_oSPrePr;
			nullable<OOX::Logic::CSub>			m_oSub;
			nullable<OOX::Logic::CSup>			m_oSup;
		};


		//--------------------------------------------------------------------------------
		// CSSubPr 22.1.2.102   (Subscript Properties) 
		//--------------------------------------------------------------------------------
		class CSSubPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSubPr)
			CSSubPr()
			{
			}
			virtual ~CSSubPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:sSubPr>");

				if (m_oCtrlPr.IsInit())
					sResult += m_oCtrlPr->toXML();

				sResult += _T("</m:sSubPr>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_sSubPr;
			}
	//Childs
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
		};		
		//--------------------------------------------------------------------------------
		// CSSub 22.1.2.101   (Subscript Object) 
		//--------------------------------------------------------------------------------
		class CSSub : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSub)
			CSSub()
			{
			}
			virtual ~CSSub()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:sSubPr") == sName )
								m_oSSubPr = new OOX::Logic::CSSubPr( oItem );
							else if ( _T("m:sub") == sName )
								m_oSub = new OOX::Logic::CSub( oItem );
							else if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:sSubPr") == sName )
						m_oSSubPr = oReader;
					else if ( _T("m:sub") == sName )
						m_oSub = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:sSub>");

				if (m_oSSubPr.IsInit())
					sResult += m_oSSubPr->toXML();

				if (m_oElement.IsInit())
					sResult += m_oElement->toXML();

				if (m_oSub.IsInit())
					sResult += m_oSub->toXML();

				sResult += _T("</m:sSub>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_sSub;
			}
	//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CSSubPr>		m_oSSubPr;
			nullable<OOX::Logic::CSub>			m_oSub;
		};

		//--------------------------------------------------------------------------------
		// CSSubSupPr 22.1.2.104   (Sub-Superscript Properties) 
		//--------------------------------------------------------------------------------
		class CSSubSupPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSubSupPr)
			CSSubSupPr()
			{
			}
			virtual ~CSSubSupPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:alnScr") == sName )
								m_oAlnScr = new OOX::Logic::CAlnScr( oItem );
							else if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:alnScr") == sName )
						m_oAlnScr = oReader;
					else if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:sSubSupPr>");

				if (m_oAlnScr.IsInit())
					sResult += m_oAlnScr->toXML();

				if (m_oCtrlPr.IsInit())
					sResult += m_oCtrlPr->toXML();

				sResult += _T("</m:sSubSupPr>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_m_sSubSupPr;
			}
	//Childs
			nullable<OOX::Logic::CAlnScr>		m_oAlnScr;
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
		};		
		//--------------------------------------------------------------------------------
		// CSSubSup 22.1.2.103   (Sub-Superscript Object)  
		//--------------------------------------------------------------------------------
		class CSSubSup : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSubSup)
			CSSubSup()
			{
			}
			virtual ~CSSubSup()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:sSubSupPr") == sName )
								m_oSSubSupPr = new OOX::Logic::CSSubSupPr( oItem );
							else if ( _T("m:sup") == sName )
								m_oSup = new OOX::Logic::CSup( oItem );
							else if ( _T("m:sub") == sName )
								m_oSub = new OOX::Logic::CSub( oItem );
							else if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:sSubSupPr") == sName )
						m_oSSubSupPr = oReader;
					else if ( _T("m:sub") == sName )
						m_oSub = oReader;
					else if ( _T("m:sup") == sName )
						m_oSup = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:sSubSup>");

				if (m_oSSubSupPr.IsInit())
					sResult += m_oSSubSupPr->toXML();

				if (m_oElement.IsInit())
					sResult += m_oElement->toXML();

				if (m_oSub.IsInit())
					sResult += m_oSub->toXML();

				if (m_oSup.IsInit())
					sResult += m_oSup->toXML();

				sResult += _T("</m:sSubSup>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_sSubSup;
			}
	//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CSSubSupPr>	m_oSSubSupPr;
			nullable<OOX::Logic::CSub>			m_oSub;
			nullable<OOX::Logic::CSup>			m_oSup;
		};		
		//--------------------------------------------------------------------------------
		// CSSupPr 22.1.2.106   (Superscript Properties)
		//--------------------------------------------------------------------------------
		class CSSupPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSupPr)
			CSSupPr()
			{
			}
			virtual ~CSSupPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:ctrlPr") == sName )
								m_oCtrlPr = new OOX::Logic::CCtrlPr( oItem );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:ctrlPr") == sName )
						m_oCtrlPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:sSupPr>");

				if (m_oCtrlPr.IsInit())
					sResult += m_oCtrlPr->toXML();

				sResult += _T("</m:sSupPr>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_sSupPr;
			}
	//Childs
			nullable<OOX::Logic::CCtrlPr>		m_oCtrlPr;
		};		
		//--------------------------------------------------------------------------------
		// CSSup 22.1.2.105   (Superscript Object)
		//--------------------------------------------------------------------------------
		class CSSup : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSSup)
			CSSup()
			{
			}
			virtual ~CSSup()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( _T("*"), oChilds ) )
				{
					XmlUtils::CXmlNode oItem;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItem ) )
						{
							std::wstring sName = oItem.GetName();
							WritingElement *pItem = NULL;

							if ( _T("m:sSupPr") == sName )
								m_oSSupPr = new OOX::Logic::CSSupPr( oItem );
							else if ( _T("m:sup") == sName )
								m_oSup = new OOX::Logic::CSup( oItem );
							else if ( _T("m:e") == sName )
								m_oElement = new OOX::Logic::CElement( oItem );

						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( _T("m:e") == sName )
						m_oElement = oReader;
					else if ( _T("m:sSupPr") == sName )
						m_oSSupPr = oReader;
					else if ( _T("m:sup") == sName )
						m_oSup = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<m:sSup>");

				if (m_oSSupPr.IsInit())
					sResult += m_oSSupPr->toXML();

				if (m_oElement.IsInit())
					sResult += m_oElement->toXML();

				if (m_oSup.IsInit())
					sResult += m_oSup->toXML();

				sResult += _T("</m:sSup>");
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_m_sSup;
			}
	//Childs
			nullable<OOX::Logic::CElement>		m_oElement;
			nullable<OOX::Logic::CSSupPr>		m_oSSupPr;
			nullable<OOX::Logic::CSup>			m_oSup;
		};
	} // namespace OMath	
} // namespace OOX

