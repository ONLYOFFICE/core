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
#include "oMathContent.h"
namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CCtrlPr 22.1.2.23 (Control Properties)
		//--------------------------------------------------------------------------------

		CCtrlPr::CCtrlPr()
		{
		}
		CCtrlPr::~CCtrlPr()
		{
		}
		EElementType CCtrlPr::getType() const
		{
			return et_m_ctrlPr;
		}

		//--------------------------------------------------------------------------------
		// CAccPr 22.1.2.2 (Accent Properties)
		//--------------------------------------------------------------------------------

		CAccPr::CAccPr()
		{
		}
		CAccPr::~CAccPr()
		{
		}
		void CAccPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CAccPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CAccPr::getType() const
		{
			return et_m_accPr;
		}

		//--------------------------------------------------------------------------------
		// CAcc 22.1.2.1  (Accent)
		//--------------------------------------------------------------------------------

		CAcc::CAcc()
		{
		}
		CAcc::~CAcc()
		{
		}
		void CAcc::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CAcc::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CAcc::getType() const
		{
			return et_m_acc;
		}

		//--------------------------------------------------------------------------------
		// CArgPr 22.1.2.5  (Argument Properties)
		//--------------------------------------------------------------------------------

		CArgPr::CArgPr()
		{
		}
		CArgPr::~CArgPr()
		{
		}
		void CArgPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CArgPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CArgPr::toXML() const
		{
			std::wstring sResult = _T("<m:argPr>");

			if ( m_oArgSz.IsInit() )
				sResult += m_oArgSz->toXML();

			sResult += _T("</m:argPr>");

			return sResult;
		}

		EElementType CArgPr::getType() const
		{
			return et_m_argPr;
		}

		//--------------------------------------------------------------------------------
		// CBarPr 22.1.2.8 (Bar Properties)
		//--------------------------------------------------------------------------------

		CBarPr::CBarPr()
		{
		}
		CBarPr::~CBarPr()
		{
		}
		void CBarPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CBarPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CBarPr::getType() const
		{
			return et_m_barPr;
		}

		//--------------------------------------------------------------------------------
		// CBar 22.1.2.7 (Bar)
		//--------------------------------------------------------------------------------

		CBar::CBar()
		{
		}
		CBar::~CBar()
		{
		}
		void CBar::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CBar::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CBar::getType() const
		{
			return et_m_bar;
		}

		//--------------------------------------------------------------------------------
		// CBorderBoxPr 22.1.2.12 (Border-Box Properties)
		//--------------------------------------------------------------------------------

		CBorderBoxPr::CBorderBoxPr()
		{
		}
		CBorderBoxPr::~CBorderBoxPr()
		{
		}
		void CBorderBoxPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CBorderBoxPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CBorderBoxPr::getType() const
		{
			return et_m_borderBoxPr;
		}

		//--------------------------------------------------------------------------------
		// CBorderBox 22.1.2.11 (Border-Box Object)
		//--------------------------------------------------------------------------------

		CBorderBox::CBorderBox()
		{
		}
		CBorderBox::~CBorderBox()
		{
		}
		void CBorderBox::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CBorderBox::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CBorderBox::getType() const
		{
			return et_m_borderBox;
		}

		//--------------------------------------------------------------------------------
		// CBrk 22.1.2.15 (Break)
		//--------------------------------------------------------------------------------

		CBrk::CBrk()
		{
		}
		CBrk::~CBrk()
		{
		}
		void CBrk::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("m:alnAt"), m_alnAt );
		}
		void CBrk::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CBrk::toXML() const
		{
			std::wstring sResult = _T("<m:brk m:alnAt=\"");
			sResult += m_alnAt->ToString();
			sResult += _T("\" />");
			return sResult;
		}
		EElementType CBrk::getType() const
		{
			return et_m_brk;
		}
		void CBrk::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("m:alnAt"), m_alnAt )
					WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CBoxPr 22.1.2.14 (Box Properties)
		//--------------------------------------------------------------------------------

		CBoxPr::CBoxPr()
		{
		}
		CBoxPr::~CBoxPr()
		{
		}
		void CBoxPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CBoxPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CBoxPr::getType() const
		{
			return et_m_boxPr;
		}

		//--------------------------------------------------------------------------------
		// CBox 22.1.2.13 (Box Object)
		//--------------------------------------------------------------------------------

		CBox::CBox()
		{
		}
		CBox::~CBox()
		{
		}
		void CBox::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CBox::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CBox::getType() const
		{
			return et_m_box;
		}

		//--------------------------------------------------------------------------------
		// CDelimiterPr 22.1.2.31 (Delimiter Properties)
		//--------------------------------------------------------------------------------

		CDelimiterPr::CDelimiterPr()
		{
		}
		CDelimiterPr::~CDelimiterPr()
		{
		}
		void CDelimiterPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CDelimiterPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CDelimiterPr::toXML() const
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
		EElementType CDelimiterPr::getType() const
		{
			return et_m_dPr;
		}

		//--------------------------------------------------------------------------------
		// CDelimiter 22.1.2.24 (Delimiter Object)
		//--------------------------------------------------------------------------------

		CDelimiter::CDelimiter()
		{
			m_lColumn = 0;
		}
		CDelimiter::~CDelimiter()
		{
		}
		EElementType CDelimiter::getType() const
		{
			return et_m_d;
		}

		//--------------------------------------------------------------------------------
		// CEqArrPr 22.1.2.35 (Array Object)
		//--------------------------------------------------------------------------------

		CEqArrPr::CEqArrPr()
		{
		}
		CEqArrPr::~CEqArrPr()
		{
		}
		void CEqArrPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CEqArrPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CEqArrPr::getType() const
		{
			return et_m_eqArrPr;
		}

		//--------------------------------------------------------------------------------
		// CEqArr 22.1.2.34 (Array Object)
		//--------------------------------------------------------------------------------

		CEqArr::CEqArr()
		{
			m_lRow = 0;
		}
		CEqArr::~CEqArr()
		{
		}
		void CEqArr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CEqArr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CEqArr::toXML() const
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
		EElementType CEqArr::getType() const
		{
			return et_m_eqArr;
		}

		//--------------------------------------------------------------------------------
		// CFPr 22.1.2.38 (Fraction Properties)
		//--------------------------------------------------------------------------------

		CFPr::CFPr()
		{
		}
		CFPr::~CFPr()
		{
		}
		void CFPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CFPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CFPr::toXML() const
		{
			std::wstring sResult = _T("<m:fPr>");

			if ( m_oCtrlPr.IsInit() )
				sResult += m_oCtrlPr->toXML();

			if ( m_oType.IsInit() )
				sResult += m_oType->toXML();

			sResult += _T("</m:fPr>");

			return sResult;
		}
		EElementType CFPr::getType() const
		{
			return et_m_fPr;
		}

		//--------------------------------------------------------------------------------
		// CFraction 22.1.2.36 (Fraction Object)
		//--------------------------------------------------------------------------------

		CFraction::CFraction()
		{
		}
		CFraction::~CFraction()
		{
		}
		void CFraction::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CFraction::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CFraction::getType() const
		{
			return et_m_f;
		}

		//--------------------------------------------------------------------------------
		// CFuncPr 22.1.2.40 (Function Properties)
		//--------------------------------------------------------------------------------

		CFuncPr::CFuncPr()
		{
		}
		CFuncPr::~CFuncPr()
		{
		}
		void CFuncPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CFuncPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CFuncPr::toXML() const
		{
			std::wstring sResult = _T("<m:funcPr>");

			if ( m_oCtrlPr.IsInit() )
				sResult += m_oCtrlPr->toXML();

			sResult += _T("</m:funcPr>");

			return sResult;
		}
		EElementType CFuncPr::getType() const
		{
			return et_m_funcPr;
		}

		//--------------------------------------------------------------------------------
		// CFunc 22.1.2.39 (Function Apply Object)
		//--------------------------------------------------------------------------------

		CFunc::CFunc()
		{
		}
		CFunc::~CFunc()
		{
		}
		void CFunc::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CFunc::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CFunc::getType() const
		{
			return et_m_func;
		}

		//--------------------------------------------------------------------------------
		// CGroupChrPr 22.1.2.42 (Group-Character Properties )
		//--------------------------------------------------------------------------------

		CGroupChrPr::CGroupChrPr()
		{
		}
		CGroupChrPr::~CGroupChrPr()
		{
		}
		void CGroupChrPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CGroupChrPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CGroupChrPr::toXML() const
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
		EElementType CGroupChrPr::getType() const
		{
			return et_m_groupChrPr;
		}

		//--------------------------------------------------------------------------------
		// CGroupChr 22.1.2.41 (Group-Character Object)
		//--------------------------------------------------------------------------------

		CGroupChr::CGroupChr()
		{
		}
		CGroupChr::~CGroupChr()
		{
		}
		void CGroupChr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CGroupChr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CGroupChr::getType() const
		{
			return et_m_groupChr;
		}

		//--------------------------------------------------------------------------------
		// CLimLowPr 22.1.2.55 (Lower-Limit Properties)
		//--------------------------------------------------------------------------------

		CLimLowPr::CLimLowPr()
		{
		}
		CLimLowPr::~CLimLowPr()
		{
		}
		void CLimLowPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CLimLowPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CLimLowPr::toXML() const
		{
			std::wstring sResult = _T("<m:limLowPr>");

			if ( m_oCtrlPr.IsInit() )
				sResult += m_oCtrlPr->toXML();

			sResult += _T("</m:limLowPr>");

			return sResult;
		}

		EElementType CLimLowPr::getType() const
		{
			return et_m_limLowPr;
		}

		//--------------------------------------------------------------------------------
		// CLimLow 22.1.2.54 (Lower-Limit Object)
		//--------------------------------------------------------------------------------

		CLimLow::CLimLow()
		{
		}
		CLimLow::~CLimLow()
		{
		}
		void CLimLow::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CLimLow::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CLimLow::getType() const
		{
			return et_m_limLow;
		}

		//--------------------------------------------------------------------------------
		// CLimUppPr 22.1.2.57 (Upper-Limit Properties)
		//--------------------------------------------------------------------------------

		CLimUppPr::CLimUppPr()
		{
		}
		CLimUppPr::~CLimUppPr()
		{
		}
		void CLimUppPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CLimUppPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CLimUppPr::toXML() const
		{
			std::wstring sResult = _T("<m:limUppPr>");

			if ( m_oCtrlPr.IsInit() )
				sResult += m_oCtrlPr->toXML();

			sResult += _T("</m:limUppPr>");

			return sResult;
		}
		EElementType CLimUppPr::getType() const
		{
			return et_m_limUppPr;
		}

		//--------------------------------------------------------------------------------
		// CLimUpp 22.1.2.56 (Upper-Limit Object)
		//--------------------------------------------------------------------------------

		CLimUpp::CLimUpp()
		{
		}
		CLimUpp::~CLimUpp()
		{
		}
		EElementType CLimUpp::getType() const
		{
			return et_m_limUpp;
		}

		//--------------------------------------------------------------------------------
		// CMathFont 22.1.2.61 (Math Font)
		//--------------------------------------------------------------------------------

		CMathFont::CMathFont()
		{
		}
		CMathFont::~CMathFont()
		{
		}
		void CMathFont::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("m:val"), m_val );
		}
		void CMathFont::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CMathFont::toXML() const
		{
			std::wstring sResult = _T("<m:mathFont m:val=\"");
			sResult += *m_val;
			sResult += _T("\"/>");
			return sResult;
		}
		EElementType CMathFont::getType() const
		{
			return et_m_mathFont;
		}
		void CMathFont::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("m:val"), m_val )
					WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CMathPr 22.1.2.62 (Math Properties)
		//--------------------------------------------------------------------------------

		CMathPr::CMathPr()
		{
		}
		CMathPr::~CMathPr()
		{
		}
		EElementType CMathPr::getType() const
		{
			return et_m_mathPr;
		}

		//--------------------------------------------------------------------------------
		// CMcPr 22.1.2.66 (Matrix Column Properties)
		//--------------------------------------------------------------------------------

		CMcPr::CMcPr()
		{
		}
		CMcPr::~CMcPr()
		{
		}
		void CMcPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CMcPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CMcPr::toXML() const
		{
			std::wstring sResult = _T("<m:mcPr>");

			if ( m_oCount.IsInit() )
				sResult += m_oCount->toXML();

			if ( m_oMcJc.IsInit() )
				sResult += m_oMcJc->toXML();

			sResult += _T("</m:mcPr>");

			return sResult;
		}
		EElementType CMcPr::getType() const
		{
			return et_m_mcPr;
		}

		//--------------------------------------------------------------------------------
		// CMc 22.1.2.64  (Matrix Column)
		//--------------------------------------------------------------------------------

		CMc::CMc()
		{
		}
		CMc::~CMc()
		{
		}
		void CMc::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CMc::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CMc::getType() const
		{
			return et_m_mc;
		}

		//--------------------------------------------------------------------------------
		// CMcs 22.1.2.67 (Matrix Columns)
		//--------------------------------------------------------------------------------

		CMcs::CMcs()
		{
		}
		CMcs::~CMcs()
		{
		}
		void CMcs::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CMcs::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CMcs::toXML() const
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
		EElementType CMcs::getType() const
		{
			return et_m_mcs;
		}

		//--------------------------------------------------------------------------------
		// CMPr 22.1.2.68   (Matrix Properties)
		//--------------------------------------------------------------------------------

		CMPr::CMPr()
		{
		}
		CMPr::CMPr(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CMPr::CMPr(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		EElementType CMPr::getType() const
		{
			return et_m_mPr;
		}

		//--------------------------------------------------------------------------------
		// CMr 22.1.2.69 (Matrix Row)
		//--------------------------------------------------------------------------------

		CMr::CMr()
		{
			m_lCol = 0;
		}
		CMr::~CMr()
		{
		}
		void CMr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CMr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CMr::toXML() const
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
		EElementType CMr::getType() const
		{
			return et_m_mr;
		}
		LONG CMr::GetCol()
		{
			return m_lCol;
		}

		//--------------------------------------------------------------------------------
		// CMatrix 22.1.2.60 (Matrix Object)
		//--------------------------------------------------------------------------------

		CMatrix::CMatrix()
		{
			m_lRow = 0;
		}
		CMatrix::~CMatrix()
		{
		}
		void CMatrix::fromXML(XmlUtils::CXmlNode& oNode)
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
						if (pItem)
							m_arrItems.push_back(pItem);
					}
				}
			}
		}
		void CMatrix::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CMatrix::toXML() const
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
		EElementType CMatrix::getType() const
		{
			return et_m_m;
		}

		//--------------------------------------------------------------------------------
		// CNaryPr 22.1.2.72   (n-ary Properties)
		//--------------------------------------------------------------------------------

		CNaryPr::CNaryPr()
		{
		}
		CNaryPr::~CNaryPr()
		{
		}
		EElementType CNaryPr::getType() const
		{
			return et_m_naryPr;
		}

		//--------------------------------------------------------------------------------
		// CNary 22.1.2.70 (n-ary Operator Object)
		//--------------------------------------------------------------------------------

		CNary::CNary()
		{
		}
		CNary::~CNary()
		{
		}
		void CNary::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CNary::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CNary::getType() const
		{
			return et_m_nary;
		}

		//--------------------------------------------------------------------------------
		// COMathParaPr 22.1.2.79
		//--------------------------------------------------------------------------------

		COMathParaPr::COMathParaPr()
		{
		}
		COMathParaPr::~COMathParaPr()
		{
		}
		void COMathParaPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void COMathParaPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring COMathParaPr::toXML() const
		{
			std::wstring sResult = _T("<m:oMathParaPr>");

			if ( m_oMJc.IsInit() )
				sResult += m_oMJc->toXML();

			sResult += _T("</m:oMathParaPr>");

			return sResult;
		}
		EElementType COMathParaPr::getType() const
		{
			return et_m_oMathParaPr;
		}

		//--------------------------------------------------------------------------------
		// CPhantPr 22.1.2.82   (Phantom Properties)
		//--------------------------------------------------------------------------------

		CPhantPr::CPhantPr()
		{
		}
		CPhantPr::~CPhantPr()
		{
		}
		void CPhantPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CPhantPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CPhantPr::toXML() const
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
		EElementType CPhantPr::getType() const
		{
			return et_m_phantPr;
		}

		//--------------------------------------------------------------------------------
		// CPhant 22.1.2.81   (Phantom Object)
		//--------------------------------------------------------------------------------

		CPhant::CPhant()
		{
		}
		CPhant::~CPhant()
		{
		}
		void CPhant::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CPhant::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CPhant::getType() const
		{
			return et_m_phant;
		}

		//--------------------------------------------------------------------------------
		// CMText 22.1.2.116  (Math Text)
		//--------------------------------------------------------------------------------

		CMText::CMText()
		{
		}
		CMText::~CMText()
		{
		}
		void CMText::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("xml:space"), m_oSpace );

			m_sText = oNode.GetText();
		}
		void CMText::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			m_sText = oReader.GetText2();
		}
		std::wstring CMText::toXML() const
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
		EElementType CMText::getType() const
		{
			return et_m_t;
		}
		void CMText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

		//--------------------------------------------------------------------------------
		// CMRPr 22.1.2.91   (Run Properties)
		//--------------------------------------------------------------------------------

		CMRPr::CMRPr()
		{
		}
		CMRPr::~CMRPr()
		{
		}
		EElementType CMRPr::getType() const
		{
			return et_m_rPr;
		}

		//--------------------------------------------------------------------------------
		// CMRun 22.1.2.87  (Math Run)
		//--------------------------------------------------------------------------------

		CMRun::CMRun()
		{
		}
		CMRun::~CMRun()
		{
		}
		void CMRun::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CMRun::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CMRun::getType() const
		{
			return et_m_r;
		}

		//--------------------------------------------------------------------------------
		// CMDel
		//--------------------------------------------------------------------------------

		CMDel::CMDel()
		{
		}
		CMDel::~CMDel()
		{
		}
		void CMDel::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:author"), m_sAuthor );
			XmlMacroReadAttributeBase( oNode, _T("w:date"),   m_oDate  );
			XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );
			XmlMacroReadAttributeBase( oNode, _T("oouserid"), m_sUserId );

			m_oRun = oNode;
		}
		void CMDel::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			m_oRun = oReader;
		}
		std::wstring CMDel::toXML() const
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
		EElementType CMDel::getType() const
		{
			return et_w_ins;
		}
		void CMDel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate  )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
					WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CMIns
		//--------------------------------------------------------------------------------

		CMIns::CMIns()
		{
		}
		CMIns::~CMIns()
		{
		}
		void CMIns::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:author"), m_sAuthor );
			XmlMacroReadAttributeBase( oNode, _T("w:date"),   m_oDate  );
			XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );
			XmlMacroReadAttributeBase( oNode, _T("oouserid"), m_sUserId );

			m_oRun = oNode;
		}
		void CMIns::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			m_oRun = oReader;
		}
		std::wstring CMIns::toXML() const
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
		EElementType CMIns::getType() const
		{
			return et_w_ins;
		}
		void CMIns::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate  )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
					WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CRadPr 22.1.2.89   (Radical Properties)
		//--------------------------------------------------------------------------------

		CRadPr::CRadPr()
		{
		}
		CRadPr::~CRadPr()
		{
		}
		void CRadPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CRadPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CRadPr::toXML() const
		{
			std::wstring sResult = _T("<m:radPr>");

			if ( m_oCtrlPr.IsInit() )
				sResult += m_oCtrlPr->toXML();

			if ( m_oDegHide.IsInit() )
				sResult += m_oDegHide->toXML();

			sResult += _T("</m:radPr>");
			return sResult;
		}
		EElementType CRadPr::getType() const
		{
			return et_m_radPr;
		}

		//--------------------------------------------------------------------------------
		// CRad 22.1.2.88   (Radical Object)
		//--------------------------------------------------------------------------------

		CRad::CRad()
		{
		}
		CRad::~CRad()
		{
		}
		void CRad::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CRad::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CRad::toXML() const
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
		EElementType CRad::getType() const
		{
			return et_m_rad;
		}

		//--------------------------------------------------------------------------------
		// CSPrePr 22.1.2.100   (Pre-Sub-Superscript Properties))
		//--------------------------------------------------------------------------------

		CSPrePr::CSPrePr()
		{
		}
		CSPrePr::~CSPrePr()
		{
		}
		void CSPrePr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CSPrePr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CSPrePr::toXML() const
		{
			std::wstring sResult = _T("<m:sPrePr>");

			if (m_oCtrlPr.IsInit())
				sResult += m_oCtrlPr->toXML();

			sResult += _T("</m:sPrePr>");
			return sResult;
		}
		EElementType CSPrePr::getType() const
		{
			return et_m_sPrePr;
		}

		//--------------------------------------------------------------------------------
		// CSPre 22.1.2.99   (Pre-Sub-Superscript Object)
		//--------------------------------------------------------------------------------

		CSPre::CSPre()
		{
		}
		CSPre::~CSPre()
		{
		}
		void CSPre::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CSPre::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CSPre::toXML() const
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

		EElementType CSPre::getType() const
		{
			return et_m_sPre;
		}

		//--------------------------------------------------------------------------------
		// CSSubPr 22.1.2.102   (Subscript Properties)
		//--------------------------------------------------------------------------------

		CSSubPr::CSSubPr()
		{
		}
		CSSubPr::~CSSubPr()
		{
		}
		void CSSubPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CSSubPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CSSubPr::toXML() const
		{
			std::wstring sResult = _T("<m:sSubPr>");

			if (m_oCtrlPr.IsInit())
				sResult += m_oCtrlPr->toXML();

			sResult += _T("</m:sSubPr>");
			return sResult;
		}
		EElementType CSSubPr::getType() const
		{
			return et_m_sSubPr;
		}

		//--------------------------------------------------------------------------------
		// CSSub 22.1.2.101   (Subscript Object)
		//--------------------------------------------------------------------------------

		CSSub::CSSub()
		{
		}
		CSSub::~CSSub()
		{
		}
		void CSSub::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CSSub::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CSSub::toXML() const
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
		EElementType CSSub::getType() const
		{
			return et_m_sSub;
		}

		//--------------------------------------------------------------------------------
		// CSSubSupPr 22.1.2.104   (Sub-Superscript Properties)
		//--------------------------------------------------------------------------------

		CSSubSupPr::CSSubSupPr()
		{
		}
		CSSubSupPr::~CSSubSupPr()
		{
		}
		void CSSubSupPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CSSubSupPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CSSubSupPr::toXML() const
		{
			std::wstring sResult = _T("<m:sSubSupPr>");

			if (m_oAlnScr.IsInit())
				sResult += m_oAlnScr->toXML();

			if (m_oCtrlPr.IsInit())
				sResult += m_oCtrlPr->toXML();

			sResult += _T("</m:sSubSupPr>");
			return sResult;
		}
		EElementType CSSubSupPr::getType() const
		{
			return et_m_sSubSupPr;
		}

		//--------------------------------------------------------------------------------
		// CSSubSup 22.1.2.103   (Sub-Superscript Object)
		//--------------------------------------------------------------------------------

		CSSubSup::CSSubSup()
		{
		}
		CSSubSup::~CSSubSup()
		{
		}
		void CSSubSup::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CSSubSup::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CSSubSup::toXML() const
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
		EElementType CSSubSup::getType() const
		{
			return et_m_sSubSup;
		}

		//--------------------------------------------------------------------------------
		// CSSupPr 22.1.2.106   (Superscript Properties)
		//--------------------------------------------------------------------------------

		CSSupPr::CSSupPr()
		{
		}
		CSSupPr::~CSSupPr()
		{
		}
		void CSSupPr::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CSSupPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CSSupPr::toXML() const
		{
			std::wstring sResult = _T("<m:sSupPr>");

			if (m_oCtrlPr.IsInit())
				sResult += m_oCtrlPr->toXML();

			sResult += _T("</m:sSupPr>");
			return sResult;
		}
		EElementType CSSupPr::getType() const
		{
			return et_m_sSupPr;
		}

		//--------------------------------------------------------------------------------
		// CSSup 22.1.2.105   (Superscript Object)
		//--------------------------------------------------------------------------------

		CSSup::CSSup()
		{
		}
		CSSup::~CSSup()
		{
		}
		void CSSup::fromXML(XmlUtils::CXmlNode& oNode)
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
		void CSSup::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		std::wstring CSSup::toXML() const
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
		EElementType CSSup::getType() const
		{
			return et_m_sSup;
		}

		//

		void CMathPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNodes oChilds;
			if (oNode.GetNodes(L"*", oChilds))
			{
				XmlUtils::CXmlNode oItem;
				for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
				{
					if (oChilds.GetAt(nIndex, oItem))
					{
						std::wstring sName = XmlUtils::GetNameNoNS(oItem.GetName());
						WritingElement *pItem = NULL;

						if (L"brkBin" == sName)
							m_oBrkBin = oItem;
						else if (L"brkBinSub" == sName)
							m_oBrkBinSub = oItem;
						else if (L"defJc" == sName)
							m_oDefJc = oItem;
						else if (L"w:dispDef" == sName)
							m_oDispDef = oItem;
						else if (L"w:interSp" == sName)
							m_oInterSp = oItem;
						else if (L"intLim" == sName)
							m_oIntLim = oItem;
						else if (L"intraSp" == sName)
							m_oIntraSp = oItem;
						else if (L"lMargin" == sName)
							m_oLMargin = oItem;
						else if (L"mathFont" == sName)
							m_oMathFont = oItem;
						else if (L"naryLim" == sName)
							m_oNaryLim = oItem;
						else if (L"postSp" == sName)
							m_oPostSp = oItem;
						else if (L"preSp" == sName)
							m_oPreSp = oItem;
						else if (L"rMargin" == sName)
							m_oRMargin = oItem;
						else if (L"smallFrac" == sName)
							m_oSmallFrac = oItem;
						else if (L"wrapIndent" == sName)
							m_oWrapIndent = oItem;
						else if (L"wrapRight" == sName)
							m_oWrapRight = oItem;
					}
				}
			}
		}
		void CMathPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				WritingElement *pItem = NULL;

				if (L"brkBin" == sName)
					m_oBrkBin = oReader;
				else if (L"brkBinSub" == sName)
					m_oBrkBinSub = oReader;
				else if (L"defJc" == sName)
					m_oDefJc = oReader;
				else if (L"dispDef" == sName)
					m_oDispDef = oReader;
				else if (L"interSp" == sName)
					m_oInterSp = oReader;
				else if (L"intLim" == sName)
					m_oIntLim = oReader;
				else if (L"intraSp" == sName)
					m_oIntraSp = oReader;
				else if (L"lMargin" == sName)
					m_oLMargin = oReader;
				else if (L"mathFont" == sName)
					m_oMathFont = oReader;
				else if (L"naryLim" == sName)
					m_oNaryLim = oReader;
				else if (L"postSp" == sName)
					m_oPostSp = oReader;
				else if (L"preSp" == sName)
					m_oPreSp = oReader;
				else if (L"rMargin" == sName)
					m_oRMargin = oReader;
				else if (L"smallFrac" == sName)
					m_oSmallFrac = oReader;
				else if (L"wrapIndent" == sName)
					m_oWrapIndent = oReader;
				else if (L"wrapRight" == sName)
					m_oWrapRight = oReader;
			}
		}
		std::wstring CMathPr::toXML() const
		{
			std::wstring sResult = L"<m:mathPr>";

			if (m_oMathFont.IsInit()) sResult += m_oMathFont->toXML();
			if (m_oBrkBin.IsInit()) sResult += m_oBrkBin->toXML();
			if (m_oBrkBinSub.IsInit()) sResult += m_oBrkBinSub->toXML();
			if (m_oSmallFrac.IsInit()) sResult += m_oSmallFrac->toXML();
			if (m_oDispDef.IsInit()) sResult += m_oDispDef->toXML();
			if (m_oLMargin.IsInit()) sResult += m_oLMargin->toXML();
			if (m_oRMargin.IsInit()) sResult += m_oRMargin->toXML();
			if (m_oDefJc.IsInit()) sResult += m_oDefJc->toXML();
			if (m_oPreSp.IsInit()) sResult += m_oPreSp->toXML();
			if (m_oPostSp.IsInit()) sResult += m_oPostSp->toXML();
			if (m_oInterSp.IsInit()) sResult += m_oInterSp->toXML();
			if (m_oIntraSp.IsInit()) sResult += m_oIntraSp->toXML();
			if (m_oWrapIndent.IsInit()) sResult += m_oWrapIndent->toXML();
			if (m_oWrapRight.IsInit()) sResult += m_oWrapRight->toXML();
			if (m_oIntLim.IsInit()) sResult += m_oIntLim->toXML();
			if (m_oNaryLim.IsInit()) sResult += m_oNaryLim->toXML();

			sResult += L"</m:mathPr>";

			return sResult;
		}

		void CMRPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNodes oChilds;
			if (oNode.GetNodes(L"*", oChilds))
			{
				XmlUtils::CXmlNode oItem;
				for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
				{
					if (oChilds.GetAt(nIndex, oItem))
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if (L"m:aln" == sName)
							m_oAln = new OOX::Logic::CAln(oItem);
						else if (L"m:brk" == sName)
							m_oBrk = new OOX::Logic::CBrk(oItem);
						else if (L"m:lit" == sName)
							m_oLit = new OOX::Logic::CLit(oItem);
						else if (L"m:nor" == sName)
							m_oNor = new OOX::Logic::CNor(oItem);
						else if (L"m:scr" == sName)
							m_oScr = new OOX::Logic::CScr(oItem);
						else if (L"m:sty" == sName)
							m_oSty = new OOX::Logic::CSty(oItem);
					}
				}
			}
		}
		void CMRPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();
				if (L"m:aln" == sName)
					m_oAln = oReader;
				else if (L"m:brk" == sName)
					m_oBrk = oReader;
				else if (L"m:lit" == sName)
					m_oLit = oReader;
				else if (L"m:nor" == sName)
					m_oNor = oReader;
				else if (L"m:scr" == sName)
					m_oScr = oReader;
				else if (L"m:sty" == sName)
					m_oSty = oReader;
			}
		}
		std::wstring CMRPr::toXML() const
		{
			std::wstring sResult = L"<m:rPr>";

			if (m_oAln.IsInit())
				sResult += m_oAln->toXML();

			if (m_oBrk.IsInit())
				sResult += m_oBrk->toXML();

			if (m_oLit.IsInit())
				sResult += m_oLit->toXML();

			if (m_oNor.IsInit())
				sResult += m_oNor->toXML();

			if (m_oScr.IsInit())
				sResult += m_oScr->toXML();

			if (m_oSty.IsInit())
				sResult += m_oSty->toXML();

			sResult += L"</m:rPr>";
			return sResult;
		}

		void CNaryPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNodes oChilds;
			if (oNode.GetNodes(L"*", oChilds))
			{
				XmlUtils::CXmlNode oItem;
				for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
				{
					if (oChilds.GetAt(nIndex, oItem))
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if (L"m:ctrlPr" == sName)
							m_oCtrlPr = new OOX::Logic::CCtrlPr(oItem);
						else if (L"m:chr" == sName)
							m_oChr = new OOX::Logic::CChr(oItem);
						else if (L"m:grow" == sName)
							m_oGrow = new OOX::Logic::CGrow(oItem);
						else if (L"m:limLoc" == sName)
							m_oLimLoc = new OOX::Logic::CLimLoc(oItem);
						else if (L"m:subHide" == sName)
							m_oSubHide = new OOX::Logic::CSubHide(oItem);
						else if (L"m:supHide" == sName)
							m_oSupHide = new OOX::Logic::CSupHide(oItem);
					}
				}
			}
		}
		void CNaryPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();
				if (L"m:chr" == sName)
					m_oChr = oReader;
				else if (L"m:ctrlPr" == sName)
					m_oCtrlPr = oReader;
				else if (L"m:grow" == sName)
					m_oGrow = oReader;
				else if (L"m:limLoc" == sName)
					m_oLimLoc = oReader;
				else if (L"m:subHide" == sName)
					m_oSubHide = oReader;
				else if (L"m:supHide" == sName)
					m_oSupHide = oReader;
			}
		}
		std::wstring CNaryPr::toXML() const
		{
			std::wstring sResult = L"<m:naryPr>";

			if (m_oChr.IsInit())
				sResult += m_oChr->toXML();

			if (m_oCtrlPr.IsInit())
				sResult += m_oCtrlPr->toXML();

			if (m_oGrow.IsInit())
				sResult += m_oGrow->toXML();

			if (m_oLimLoc.IsInit())
				sResult += m_oLimLoc->toXML();

			if (m_oSubHide.IsInit())
				sResult += m_oSubHide->toXML();

			if (m_oSupHide.IsInit())
				sResult += m_oSupHide->toXML();

			sResult += L"</m:naryPr>";

			return sResult;
		}

		void CMPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNodes oChilds;
			if (oNode.GetNodes(L"*", oChilds))
			{
				XmlUtils::CXmlNode oItem;
				for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
				{
					if (oChilds.GetAt(nIndex, oItem))
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if (L"m:baseJc" == sName)
							m_oBaseJc = new OOX::Logic::CBaseJc(oItem);
						else if (L"m:cGp" == sName)
							m_oCGp = new OOX::Logic::CCGp(oItem);
						else if (L"m:cGpRule" == sName)
							m_oCGpRule = new OOX::Logic::CCGpRule(oItem);
						else if (L"m:cSp" == sName)
							m_oCSp = new OOX::Logic::CCSp(oItem);
						else if (L"m:ctrlPr" == sName)
							m_oCtrlPr = new OOX::Logic::CCtrlPr(oItem);
						else if (L"m:mcs" == sName)
							m_oMcs = new OOX::Logic::CMcs(oItem);
						else if (L"m:plcHide" == sName)
							m_oPlcHide = new OOX::Logic::CPlcHide(oItem);
						else if (L"m:rSp" == sName)
							m_oRSp = new OOX::Logic::CRSp(oItem);
						else if (L"m:rSpRule" == sName)
							m_oRSpRule = new OOX::Logic::CRSpRule(oItem);
					}
				}
			}
		}
		void CMPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"m:baseJc" == sName)
					m_oBaseJc = oReader;
				else if (L"m:cGp" == sName)
					m_oCGp = oReader;
				else if (L"m:cGpRule" == sName)
					m_oCGpRule = oReader;
				else if (L"m:cSp" == sName)
					m_oCSp = oReader;
				else if (L"m:ctrlPr" == sName)
					m_oCtrlPr = oReader;
				else if (L"m:mcs" == sName)
					m_oMcs = oReader;
				else if (L"m:plcHide" == sName)
					m_oPlcHide = oReader;
				else if (L"m:rSp" == sName)
					m_oRSp = oReader;
				else if (L"m:rSpRule" == sName)
					m_oRSpRule = oReader;
			}
		}
		std::wstring CMPr::toXML() const
		{
			std::wstring sResult = L"<m:mPr>";

			if (m_oBaseJc.IsInit())
				sResult += m_oBaseJc->toXML();
			if (m_oCGp.IsInit())
				sResult += m_oCGp->toXML();
			if (m_oCGpRule.IsInit())
				sResult += m_oCGpRule->toXML();
			if (m_oCSp.IsInit())
				sResult += m_oCSp->toXML();
			if (m_oCtrlPr.IsInit())
				sResult += m_oCtrlPr->toXML();
			if (m_oMcs.IsInit())
				sResult += m_oMcs->toXML();
			if (m_oPlcHide.IsInit())
				sResult += m_oPlcHide->toXML();
			if (m_oRSp.IsInit())
				sResult += m_oRSp->toXML();
			if (m_oRSpRule.IsInit())
				sResult += m_oRSpRule->toXML();

			sResult += L"</m:mPr>";

			return sResult;
		}

		std::wstring CAcc::toXML() const
		{
			std::wstring sResult = L"<m:acc>";

			if (  m_oAccPr.IsInit())
				sResult += m_oAccPr->toXML();

			if ( m_oElement.IsInit())
				sResult += m_oElement->toXML();

			sResult += L"</m:acc>";

			return sResult;
		}

		std::wstring CAccPr::toXML() const
		{
			std::wstring sResult = L"<m:accPr>";

			if ( m_oChr.IsInit() )
				sResult += m_oChr->toXML();

			if ( m_oCtrlPr.IsInit() )
				sResult += m_oCtrlPr->toXML();

			sResult += L"</m:accPr>";

			return sResult;
		}

		std::wstring CBar::toXML() const
		{
			std::wstring sResult = L"<m:bar>";

			if ( m_oBarPr.IsInit() )
				sResult += m_oBarPr->toXML();

			if ( m_oElement.IsInit() )
				sResult += m_oElement->toXML();

			sResult += L"</m:bar>";

			return sResult;
		}

		std::wstring CBarPr::toXML() const
		{
			std::wstring sResult = L"<m:barPr>";

			if ( m_oCtrlPr.IsInit() )
				sResult += m_oCtrlPr->toXML();

			if ( m_oPos.IsInit() )
				sResult += m_oPos->toXML();

			sResult += L"</m:barPr>";

			return sResult;
		}

		std::wstring CBorderBox::toXML() const
		{
			std::wstring sResult = L"<m:borderBox>";

			if ( m_oBorderBoxPr.IsInit() )
				sResult += m_oBorderBoxPr->toXML();

			if ( m_oElement.IsInit() )
				sResult += m_oElement->toXML();

			sResult += L"</m:borderBox>";

			return sResult;
		}

		std::wstring CBorderBoxPr::toXML() const
		{
			std::wstring sResult = L"<m:borderBoxPr>";

			if ( m_oCtrlPr.IsInit() )
				sResult += m_oCtrlPr->toXML();

			if ( m_oHideBot.IsInit() )
				sResult += m_oHideBot->toXML();

			if ( m_oHideLeft.IsInit() )
				sResult += m_oHideLeft->toXML();

			if ( m_oHideRight.IsInit() )
				sResult += m_oHideRight->toXML();

			if ( m_oHideTop.IsInit() )
				sResult += m_oHideTop->toXML();

			if ( m_oStrikeBLTR.IsInit() )
				sResult += m_oStrikeBLTR->toXML();

			if ( m_oStrikeH.IsInit() )
				sResult += m_oStrikeH->toXML();

			if ( m_oStrikeTLBR.IsInit() )
				sResult += m_oStrikeTLBR->toXML();

			if ( m_oStrikeV.IsInit() )
				sResult += m_oStrikeV->toXML();

			sResult += L"</m:borderBoxPr>";

			return sResult;
		}

		std::wstring CBox::toXML() const
		{
			std::wstring sResult = L"<m:box>";

			if ( m_oBoxPr.IsInit() )
				sResult += m_oBoxPr->toXML();

			if ( m_oElement.IsInit() )
				sResult += m_oElement->toXML();

			sResult += L"</m:box>";

			return sResult;
		}

		std::wstring CBoxPr::toXML() const
		{
			std::wstring sResult = L"<m:boxPr>";

			if ( m_oAln.IsInit() )
				sResult += m_oAln->toXML();

			if ( m_oBrk.IsInit() )
				sResult += m_oBrk->toXML();

			if ( m_oCtrlPr.IsInit() )
				sResult += m_oCtrlPr->toXML();

			if ( m_oDiff.IsInit() )
				sResult += m_oDiff->toXML();

			if ( m_oNoBreak.IsInit() )
				sResult += m_oNoBreak->toXML();

			if ( m_oOpEmu.IsInit() )
				sResult += m_oOpEmu->toXML();

			sResult += L"</m:boxPr>";

			return sResult;
		}

		void CCtrlPr::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNodes oChilds;
			if (oNode.GetNodes(L"*", oChilds))
			{
				XmlUtils::CXmlNode oItem;
				for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
				{
					if (oChilds.GetAt(nIndex, oItem))
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if (L"w:rP" == sName)
							m_oRPr = new OOX::Logic::CRunProperty(oItem);
						else if (L"a:rPr" == sName)
							m_oARPr = new PPTX::Logic::RunProperties(oItem);
						else if (L"w:ins" == sName)
							m_oIns = new OOX::Logic::CRPrChange(oItem);
						else if (L"w:del" == sName)
							m_oDel = new OOX::Logic::CRPrChange(oItem);

					}
				}
			}
		}
		void CCtrlPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();
				if (L"w:rPr" == sName)
					m_oRPr = oReader;
				else if (L"a:rPr" == sName)
				{
					std::wstring sXml = oReader.GetOuterXml();
					XmlUtils::CXmlNode node;
					node.FromXmlString(sXml);

					m_oARPr = node;
				}
				else if (L"w:ins" == sName)
					m_oIns = oReader;
				else if (L"w:del" == sName)
					m_oDel = oReader;
			}
		}
		std::wstring CCtrlPr::toXML() const
		{
			std::wstring sResult = L"<m:ctrlPr>";

			if (m_oRPr.IsInit())
				sResult += m_oRPr->toXML();
			if (m_oARPr.IsInit())
				sResult += m_oARPr->toXML();
			if (m_oIns.IsInit())
				sResult += m_oIns->toXML();
			if (m_oDel.IsInit())
				sResult += m_oDel->toXML();

			sResult += L"</m:ctrlPr>";

			return sResult;
		}

		void CDelimiter::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_lColumn = 0;

			XmlUtils::CXmlNodes oChilds;
			if (oNode.GetNodes(L"*", oChilds))
			{
				XmlUtils::CXmlNode oItem;
				for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
				{
					if (oChilds.GetAt(nIndex, oItem))
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if (L"m:dPr" == sName)
							pItem = new CDelimiterPr(oItem);
						else if (L"m:e" == sName)
						{
							pItem = new CElement(oItem);
							//нужно заранее знать колисество столбцов для отрисовки
							m_lColumn++;
						}

						if (pItem)
							m_arrItems.push_back(pItem);
					}
				}
			}
		}
		void CDelimiter::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_lColumn = 0;

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if (L"m:dPr" == sName)
					pItem = new CDelimiterPr(oReader);
				else if (L"m:e" == sName)
				{
					pItem = new CElement(oReader);
					//нужно заранее знать колисество столбцов для отрисовки
					m_lColumn++;
				}

				if (pItem)
					m_arrItems.push_back(pItem);

			}
		}
		std::wstring CDelimiter::toXML() const
		{
			std::wstring sResult = L"<m:d>";

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					sResult += m_arrItems[i]->toXML();
				}
			}

			sResult += L"</m:d>";

			return sResult;
		}

		std::wstring CEqArrPr::toXML() const
		{
			std::wstring sResult = L"<m:eqArrPr>";

			if (m_oBaseJc.IsInit())
				sResult += m_oBaseJc->toXML();

			if (m_oCtrlPr.IsInit())
				sResult += m_oCtrlPr->toXML();

			if (m_oMaxDist.IsInit())
				sResult += m_oMaxDist->toXML();

			if (m_oObjDist.IsInit())
				sResult += m_oObjDist->toXML();

			if (m_oRSp.IsInit())
				sResult += m_oRSp->toXML();

			if (m_oRSpRule.IsInit())
				sResult += m_oRSpRule->toXML();

			sResult += L"</m:eqArrPr>";

			return sResult;
		}

		std::wstring CFraction::toXML() const
		{
			std::wstring sResult = L"<m:f>";

			if ( m_oDen.IsInit() )
				sResult += m_oDen->toXML();

			if ( m_oFPr.IsInit() )
				sResult += m_oFPr->toXML();

			if ( m_oNum.IsInit() )
				sResult += m_oNum->toXML();

			sResult += L"</m:f>";

			return sResult;
		}

		std::wstring CFunc::toXML() const
		{
			std::wstring sResult = L"<m:func>";

			if ( m_oElement.IsInit() )
				sResult += m_oElement->toXML();

			if ( m_oFName.IsInit() )
				sResult += m_oFName->toXML();

			if ( m_oFuncPr.IsInit() )
				sResult += m_oFuncPr->toXML();

			sResult += L"</m:func>";

			return sResult;
		}

		std::wstring CGroupChr::toXML() const
		{
			std::wstring sResult = L"<m:groupChr>";

			if ( m_oElement.IsInit() )
				sResult += m_oElement->toXML();

			if ( m_oGroupChrPr.IsInit() )
				sResult += m_oGroupChrPr->toXML();

			sResult += L"</m:groupChr>";

			return sResult;
		}

		std::wstring CLimLow::toXML() const
		{
			std::wstring sResult = L"<m:limLow>";

			if ( m_oElement.IsInit() )
				sResult += m_oElement->toXML();

			if ( m_oLim.IsInit() )
				sResult += m_oLim->toXML();

			if ( m_oLimLowPr.IsInit() )
				sResult += m_oLimLowPr->toXML();

			sResult += L"</m:limLow>";

			return sResult;
		}
		void CLimUpp::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNodes oChilds;
			if (oNode.GetNodes(L"*", oChilds))
			{
				XmlUtils::CXmlNode oItem;
				for (int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++)
				{
					if (oChilds.GetAt(nIndex, oItem))
					{
						std::wstring sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if (L"m:e" == sName)
							m_oElement = new OOX::Logic::CElement(oItem);
						else if (L"m:lim" == sName)
							m_oLim = new OOX::Logic::CLim(oItem);
						else if (L"m:limUppPr" == sName)
							m_oLimUppPr = new OOX::Logic::CLimUppPr(oItem);
					}
				}
			}
		}
		void CLimUpp::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();
				if (L"m:e" == sName)
					m_oElement = oReader;
				else if (L"m:lim" == sName)
					m_oLim = oReader;
				else if (L"m:limUppPr" == sName)
					m_oLimUppPr = oReader;
			}
		}
		std::wstring CLimUpp::toXML() const
		{
			std::wstring sResult = L"<m:limUpp>";

			if ( m_oElement.IsInit() )
				sResult += m_oElement->toXML();

			if ( m_oLim.IsInit() )
				sResult += m_oLim->toXML();

			if ( m_oLimUppPr.IsInit() )
				sResult += m_oLimUppPr->toXML();

			sResult += L"</m:limUpp>";

			return sResult;
		}

		std::wstring      CMc::toXML() const
		{
			std::wstring sResult = L"<m:mc>";

			if ( m_oMcPr.IsInit() )
				sResult += m_oMcPr->toXML();

			sResult += L"</m:mc>";

			return sResult;
		}

		std::wstring      CNary::toXML() const
		{
			std::wstring sResult = L"<m:nary>";

			if ( m_oElement.IsInit() )
				sResult += m_oElement->toXML();

			if ( m_oNaryPr.IsInit() )
				sResult += m_oNaryPr->toXML();

			if ( m_oSub.IsInit() )
				sResult += m_oSub->toXML();

			if ( m_oSup.IsInit() )
				sResult += m_oSup->toXML();

			sResult += L"</m:nary>";

			return sResult;
		}

		std::wstring      CPhant::toXML() const
		{
			std::wstring sResult = L"<m:phant>";

			if ( m_oElement.IsInit() )
				sResult += m_oElement->toXML();

			if ( m_oPhantPr.IsInit() )
				sResult += m_oPhantPr->toXML();

			sResult += L"</m:phant>";

			return sResult;
		}

		std::wstring      CMRun::toXML() const
		{
			std::wstring sResult = L"<m:r>";

			sResult += toXMLInner();

			sResult += L"</m:r>";

			return sResult;
		}
		std::wstring      CMRun::toXMLInner() const
	{
		std::wstring sResult;

		if ( m_oAnnotationRef.IsInit() )
			sResult += m_oAnnotationRef->toXML();

		if ( m_oBr.IsInit() )
			sResult += m_oBr->toXML();

		if ( m_oCommentReference.IsInit() )
			sResult += m_oCommentReference->toXML();

		if ( m_oContentPart.IsInit() )
			sResult += m_oContentPart->toXML();

		if ( m_oContinuationSeparator.IsInit() )
			sResult += m_oContinuationSeparator->toXML();

		if ( m_oCr.IsInit() )
			sResult += m_oCr->toXML();

		if ( m_oDayLong.IsInit() )
			sResult += m_oDayLong->toXML();

		if ( m_oDayShort.IsInit() )
			sResult += m_oDayShort->toXML();

		if ( m_oDelInstrText.IsInit() )
			sResult += m_oDelInstrText->toXML();

		if ( m_oDelText.IsInit() )
			sResult += m_oDelText->toXML();

		if ( m_oDrawing.IsInit() )
			sResult += m_oDrawing->toXML();

		if ( m_oEndnoteRef.IsInit() )
			sResult += m_oEndnoteRef->toXML();

		if ( m_oEndnoteReference.IsInit() )
			sResult += m_oEndnoteReference->toXML();

		if ( m_oFldChar.IsInit() )
			sResult += m_oFldChar->toXML();

		if ( m_oFootnoteRef.IsInit() )
			sResult += m_oFootnoteRef->toXML();

		if ( m_oFootnoteReference.IsInit() )
			sResult += m_oFootnoteReference->toXML();

		if ( m_oInstrText.IsInit() )
			sResult += m_oInstrText->toXML();

		if ( m_oLastRenderedPageBreak.IsInit() )
			sResult += m_oLastRenderedPageBreak->toXML();

		if ( m_oMonthLong.IsInit() )
			sResult += m_oMonthLong->toXML();

		if ( m_oMonthShort.IsInit() )
			sResult += m_oMonthShort->toXML();

		if ( m_oNoBreakHyphen.IsInit() )
			sResult += m_oNoBreakHyphen->toXML();

		if ( m_oObject.IsInit() )
			sResult += m_oObject->toXML();

		if ( m_oPgNum.IsInit() )
			sResult += m_oPgNum->toXML();

		if ( m_oPtab.IsInit() )
			sResult += m_oPtab->toXML();

		if ( m_oMRPr.IsInit() )
			sResult += m_oMRPr->toXML();

		if ( m_oARPr.IsInit() )
			sResult += m_oARPr->toXML();

		if ( m_oRPr.IsInit() )
			sResult += m_oRPr->toXML();

		if ( m_oRuby.IsInit() )
			sResult += m_oRuby->toXML();

		if ( m_oSeparator.IsInit() )
			sResult += m_oSeparator->toXML();

		if ( m_oSoftHyphen.IsInit() )
			sResult += m_oSoftHyphen->toXML();

		if ( m_oSym.IsInit() )
			sResult += m_oSym->toXML();

		if ( m_oMText.IsInit() )
			sResult += m_oMText->toXML();

		if ( m_oText.IsInit() )
			sResult += m_oText->toXML();

		if ( m_oTab.IsInit() )
			sResult += m_oTab->toXML();

		if ( m_oYearLong.IsInit() )
			sResult += m_oYearLong->toXML();

		if ( m_oYearShort.IsInit() )
			sResult += m_oYearShort->toXML();

		return sResult;
	}
	}
}
