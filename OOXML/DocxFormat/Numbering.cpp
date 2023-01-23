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

#include "Numbering.h"
#include "Docx.h"
#include "../../DesktopEditor/common/File.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// LvlLegacy 17.9.5 (Part 1)
		//--------------------------------------------------------------------------------

		CLvlLegacy::CLvlLegacy()
		{
		}
		CLvlLegacy::~CLvlLegacy()
		{
		}
		void CLvlLegacy::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:legacy"),       m_oLegacy );
			XmlMacroReadAttributeBase( oNode, _T("w:legacyIndent"), m_oLegacyIndent );
			XmlMacroReadAttributeBase( oNode, _T("w:legacySpace"),  m_oLegacySpace );
		}
		void CLvlLegacy::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CLvlLegacy::ToString() const
		{
			std::wstring sResult;

			ComplexTypes_WriteAttribute( _T("w:legacy=\""),       m_oLegacy );
			ComplexTypes_WriteAttribute( _T("w:legacyIndent=\""), m_oLegacyIndent );
			ComplexTypes_WriteAttribute( _T("w:legacySpace=\""),  m_oLegacySpace );

			return sResult;
		}
		void CLvlLegacy::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:legacy"),       m_oLegacy )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:legacyIndent"), m_oLegacyIndent )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:legacySpace"),  m_oLegacySpace )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// LevelText 17.9.12 (Part 1)
		//--------------------------------------------------------------------------------

		CLevelText::CLevelText()
		{
		}
		CLevelText::~CLevelText()
		{
		}
		void CLevelText::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:null"), m_oNull );
			XmlMacroReadAttributeBase( oNode, _T("w:val"),  m_sVal );
		}
		void CLevelText::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CLevelText::ToString() const
		{
			std::wstring sResult;

			ComplexTypes_WriteAttribute( L"w:null=\"", m_oNull );

			if ( m_sVal.IsInit() )
			{
				sResult += L"w:val=\"" + *m_sVal + L"\" ";
			}
			return sResult;
		}
		void CLevelText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:null"), m_oNull )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),  m_sVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// LevelSuffix 17.9.29 (Part 1)
		//--------------------------------------------------------------------------------

		CLevelSuffix::CLevelSuffix()
		{
		}
		CLevelSuffix::~CLevelSuffix()
		{
		}
		void CLevelSuffix::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
		}
		void CLevelSuffix::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CLevelSuffix::ToString() const
		{
			std::wstring sResult;

			ComplexTypes_WriteAttribute( _T("w:val=\""), m_oVal );

			return sResult;
		}
		void CLevelSuffix::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// MultiLevelType 17.9.13 (Part 1)
		//--------------------------------------------------------------------------------

		CMultiLevelType::CMultiLevelType()
		{
		}
		CMultiLevelType::~CMultiLevelType()
		{
		}
		void CMultiLevelType::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
		}
		void CMultiLevelType::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		std::wstring CMultiLevelType::ToString() const
		{
			std::wstring sResult;

			ComplexTypes_WriteAttribute(_T("w:val=\""), m_oVal);

			return sResult;
		}
		void CMultiLevelType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_ReadSingle(oReader, _T("w:val"), m_oVal)
			WritingElement_ReadAttributes_End(oReader)
		}

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Numbering
	{
		//--------------------------------------------------------------------------------
		// Lvl 17.9.6 (Part 1)
		//--------------------------------------------------------------------------------

		CLvl::CLvl()
		{
		}
		CLvl::~CLvl()
		{
		}
		void CLvl::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if (_T("w:lvl") != oNode.GetName())
				return;

			XmlMacroReadAttributeBase(oNode, _T("w:ilvl"), m_oIlvl);
			XmlMacroReadAttributeBase(oNode, _T("w:tentative"), m_oTentative);
			XmlMacroReadAttributeBase(oNode, _T("w:tplc"), m_oTplc);

			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode(oNode, oChild, _T("w:isLgl"), m_oIsLgl);
			WritingElement_ReadNode(oNode, oChild, _T("w:legacy"), m_oLegacy);
			WritingElement_ReadNode(oNode, oChild, _T("w:lvlJc"), m_oLvlJc);
			WritingElement_ReadNode(oNode, oChild, _T("w:lvlPicBulletId"), m_oLvlPicBulletId);
			WritingElement_ReadNode(oNode, oChild, _T("w:lvlRestart"), m_oLvlRestart);
			WritingElement_ReadNode(oNode, oChild, _T("w:lvlText"), m_oLvlText);
			WritingElement_ReadNode(oNode, oChild, _T("w:numFmt"), m_oNumFmt);
			WritingElement_ReadNode(oNode, oChild, _T("w:pPr"), m_oPPr);
			WritingElement_ReadNode(oNode, oChild, _T("w:pStyle"), m_oPStyle);
			WritingElement_ReadNode(oNode, oChild, _T("w:rPr"), m_oRPr);
			WritingElement_ReadNode(oNode, oChild, _T("w:start"), m_oStart);
			WritingElement_ReadNode(oNode, oChild, _T("w:suff"), m_oSuffix);
		}
		void CLvl::ReadElements(XmlUtils::CXmlLiteReader& oReader, int nDepth)
		{
			while (oReader.ReadNextSiblingNode(nDepth))
			{
				std::wstring sName = oReader.GetName();
				if (L"w:isLgl" == sName && !m_oIsLgl.IsInit())
				{
					m_oIsLgl = oReader;
				}
				else if (L"w:legacy" == sName && !m_oLegacy.IsInit())
				{
					m_oLegacy = oReader;
				}
				else if (L"w:lvlJc" == sName && !m_oLvlJc.IsInit())
				{
					m_oLvlJc = oReader;
				}
				else if (L"w:lvlPicBulletId" == sName && !m_oLvlPicBulletId.IsInit())
				{
					m_oLvlPicBulletId = oReader;
				}
				else if (L"w:lvlRestart" == sName)
				{
					m_oLvlRestart = oReader;
				}
				else if (L"w:lvlText" == sName && !m_oLvlText.IsInit())
				{
					m_oLvlText = oReader;
				}
				else if (L"w:numFmt" == sName && !m_oNumFmt.IsInit())
				{
					m_oNumFmt = oReader;
				}
				else if (L"w:pPr" == sName && !m_oPPr.IsInit())
				{
					m_oPPr = oReader;
				}
				else if (L"w:pStyle" == sName && !m_oPStyle.IsInit())
				{
					m_oPStyle = oReader;
				}
				else if (L"w:rPr" == sName && !m_oRPr.IsInit())
				{
					m_oRPr = oReader;
				}
				else if (L"w:start" == sName && !m_oStart.IsInit())
				{
					m_oStart = oReader;
				}
				else if (L"w:suff" == sName && !m_oSuffix.IsInit())
				{
					m_oSuffix = oReader;
				}
				else if (L"mc:AlternateContent" == sName)
				{
					while (oReader.ReadNextSiblingNode(nDepth + 1))
					{
						std::wstring sName = oReader.GetName();

						if (L"mc:Choice" == sName)
						{
							ReadElements(oReader, nDepth + 2);
						}
						else if (L"mc:Fallback" == sName)
						{
							ReadElements(oReader, nDepth + 2);
						}
					}
				}
			}
		}
		void CLvl::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			ReadElements(oReader, oReader.GetDepth());
		}
		std::wstring CLvl::toXML() const
		{
			std::wstring sResult = _T("<w:lvl ");

			if (m_oIlvl.IsInit())
				sResult += L"w:ilvl=\"" + std::to_wstring(*m_oIlvl) + L"\" ";

			ComplexTypes_WriteAttribute( _T("w:tentative=\""), m_oTentative );
			ComplexTypes_WriteAttribute( _T("w:tplc=\""),      m_oTplc );

			sResult += _T(">");

			WritingElement_WriteNode_1( _T("<w:isLgl "),          m_oIsLgl );
			WritingElement_WriteNode_1( _T("<w:legacy "),         m_oLegacy );
			WritingElement_WriteNode_1( _T("<w:lvlJc "),          m_oLvlJc );
			WritingElement_WriteNode_1( _T("<w:lvlPicBulletId "), m_oLvlPicBulletId );
			WritingElement_WriteNode_1( _T("<w:lvlRestart "),     m_oLvlRestart );
			WritingElement_WriteNode_1( _T("<w:lvlText "),        m_oLvlText );
			WritingElement_WriteNode_1( _T("<w:numFmt "),         m_oNumFmt );
			WritingElement_WriteNode_2( m_oPPr );
			WritingElement_WriteNode_1( _T("<w:pStyle "),         m_oPStyle );
			WritingElement_WriteNode_2( m_oRPr );
			WritingElement_WriteNode_1( _T("<w:start "),          m_oStart );
			WritingElement_WriteNode_1( _T("<w:suff "),           m_oSuffix );

			sResult += _T("</w:lvl>");

			return sResult;
		}
		EElementType CLvl::getType() const
		{
			return et_w_lvl;
		}
		void CLvl::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:ilvl"),      m_oIlvl )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:tentative"), m_oTentative )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:tplc"),      m_oTplc )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// AbstractNum 17.9.1 (Part 1); listDef element [Word 2003 XML]
		//--------------------------------------------------------------------------------

		CAbstractNum::CAbstractNum()
		{
		}
		CAbstractNum::~CAbstractNum()
		{
			for ( size_t nIndex = 0; nIndex < m_arrLvl.size(); nIndex++ )
			{
				if ( m_arrLvl[nIndex] ) delete m_arrLvl[nIndex];
				m_arrLvl[nIndex] = NULL;
			}
			m_arrLvl.clear();
		}
		void CAbstractNum::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( L"w:abstractNum" != oNode.GetName() && L"listDef" != oNode.GetName())
				return;

			XmlMacroReadAttributeBase( oNode, _T("w:abstractNumId"),	m_oAbstractNumId );
			XmlMacroReadAttributeBase( oNode, _T("w:listDefId"),		m_oAbstractNumId );

			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, _T("w:multiLevelType"), m_oMultiLevelType );
			WritingElement_ReadNode( oNode, oChild, _T("w:name"),           m_oName );
			WritingElement_ReadNode( oNode, oChild, _T("w:nsid"),           m_oNsid );
			WritingElement_ReadNode( oNode, oChild, _T("w:numStyleLink"),   m_oNumStyleLink );
			WritingElement_ReadNode( oNode, oChild, _T("w:styleLink"),      m_oStyleLink );
			WritingElement_ReadNode( oNode, oChild, _T("w:tmpl"),           m_oTmpl );

			XmlUtils::CXmlNodes oLvlList;
			if ( oNode.GetNodes( _T("w:lvl"), oLvlList ) )
			{
				XmlUtils::CXmlNode oLvlNode;
				for ( int nIndex = 0; nIndex < oLvlList.GetCount(); nIndex++ )
				{
					if ( oLvlList.GetAt( nIndex, oLvlNode ) )
					{
						OOX::Numbering::CLvl *pLvl = new OOX::Numbering::CLvl(oLvlNode);
						if (pLvl)
						{
							if (pLvl->m_oIlvl.IsInit())
								m_mapLvl.insert(std::make_pair(*pLvl->m_oIlvl, m_arrLvl.size()));

							m_arrLvl.push_back(pLvl);
						}
					}
				}
			}
		}
		void CAbstractNum::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( _T("w:lvl") == sName )
				{
					OOX::Numbering::CLvl *pLvl = new OOX::Numbering::CLvl(oReader);

					if (pLvl)
					{
						if (pLvl->m_oIlvl.IsInit())
							m_mapLvl.insert(std::make_pair(*pLvl->m_oIlvl, m_arrLvl.size()));

						m_arrLvl.push_back(pLvl);
					}
				}
				else if (L"w:multiLevelType"	== sName ||
							L"w:plt"			== sName ) m_oMultiLevelType = oReader;
				else if ( L"w:name"				== sName ) m_oName = oReader;
				else if ( L"w:nsid"				== sName ) m_oNsid = oReader;
				else if ( L"w:numStyleLink"		== sName ) m_oNumStyleLink = oReader;
				else if ( L"w:styleLink"		== sName ) m_oStyleLink = oReader;
				else if ( L"w:tmpl"				== sName ) m_oTmpl = oReader;
			}
		}
		std::wstring CAbstractNum::toXML() const
		{
			std::wstring sResult = _T("<w:abstractNum ");

			if (m_oAbstractNumId.IsInit())
				sResult += L"w:abstractNumId=\"" + std::to_wstring(*m_oAbstractNumId) + L"\" ";

			sResult += _T(">");

			WritingElement_WriteNode_1( _T("<w:multiLevelType "), m_oMultiLevelType );
			WritingElement_WriteNode_1( _T("<w:name "),           m_oName );
			WritingElement_WriteNode_1( _T("<w:nsid "),           m_oNsid );
			WritingElement_WriteNode_1( _T("<w:numStyleLink "),   m_oNumStyleLink );
			WritingElement_WriteNode_1( _T("<w:styleLink "),      m_oStyleLink );
			WritingElement_WriteNode_1( _T("<w:tmpl "),           m_oTmpl );

			for (unsigned int nIndex = 0; nIndex < m_arrLvl.size(); nIndex++ )
			{
				if (m_arrLvl[nIndex])
					sResult += m_arrLvl[nIndex]->toXML();
			}
			sResult += _T("</w:abstractNum>");

			return sResult;
		}
		EElementType CAbstractNum::getType() const
		{
			return et_w_abstractNum;
		}
		void CAbstractNum::copy(CAbstractNum* copies)
		{
			if (!copies) return;

			copies->m_oAbstractNumId = m_oAbstractNumId;
			copies->m_oMultiLevelType = m_oMultiLevelType;
			copies->m_oName = m_oName;
			copies->m_oNsid = m_oNsid;
			copies->m_oNumStyleLink = m_oNumStyleLink;
			copies->m_oStyleLink = m_oStyleLink;
			copies->m_oTmpl = m_oTmpl;

			for (size_t i = 0; i < m_arrLvl.size(); i++)
			{
				copies->m_arrLvl.push_back(new CLvl(*m_arrLvl[i]));
			}
			copies->m_mapLvl = m_mapLvl;
		}
		void CAbstractNum::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("w:abstractNumId"), m_oAbstractNumId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("w:listDefId"), m_oAbstractNumId )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// NumLvl 17.9.9 (Part 1)
		//--------------------------------------------------------------------------------

		CNumLvl::CNumLvl()
		{
		}
		CNumLvl::~CNumLvl()
		{
		}
		void CNumLvl::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:lvlOverride") != oNode.GetName() )
				return;

			XmlMacroReadAttributeBase( oNode, _T("w:ilvl"), m_oIlvl );

			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, _T("w:lvl"),           m_oLvl );
			WritingElement_ReadNode( oNode, oChild, _T("w:startOverride"), m_oStartOverride );
		}
		void CNumLvl::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( _T("w:lvl") == sName )
					m_oLvl = oReader;
				else if ( _T("w:startOverride") == sName )
					m_oStartOverride = oReader;
			}
		}
		std::wstring CNumLvl::toXML() const
		{
			std::wstring sResult = _T("<w:lvlOverride ");

			if (m_oIlvl.IsInit())
				sResult += L"w:ilvl=\"" + std::to_wstring(*m_oIlvl) + L"\" ";

			sResult += L">";

			WritingElement_WriteNode_2( m_oLvl );
			WritingElement_WriteNode_1( L"<w:startOverride ", m_oStartOverride );

			sResult += L"</w:lvlOverride>";

			return sResult;
		}
		EElementType CNumLvl::getType() const
		{
			return et_w_lvlOverride;
		}
		void CNumLvl::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:ilvl"), m_oIlvl )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// Num 17.9.16 (Part 1)
		//--------------------------------------------------------------------------------

		CNum::CNum()
		{
		}
		CNum::~CNum()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrLvlOverride.size(); nIndex++ )
			{
				if ( m_arrLvlOverride[nIndex] ) delete m_arrLvlOverride[nIndex];
				m_arrLvlOverride[nIndex] = NULL;
			}
			m_arrLvlOverride.clear();
		}
		void CNum::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( L"w:num" != oNode.GetName() && L"w:list" != oNode.GetName())
				return;

			XmlMacroReadAttributeBase( oNode, _T("w:numId"), m_oNumId );
			XmlMacroReadAttributeBase( oNode, _T("w:ilfo"), m_oNumId );

			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, L"w:abstractNumId", m_oAbstractNumId );
			WritingElement_ReadNode( oNode, oChild, L"w:ilst", m_oAbstractNumId );

			XmlUtils::CXmlNodes oLvlList;
			if ( oNode.GetNodes( L"w:lvlOverride", oLvlList ) )
			{
				XmlUtils::CXmlNode oLvlNode;
				for ( int nIndex = 0; nIndex < oLvlList.GetCount(); nIndex++ )
				{
					if ( oLvlList.GetAt( nIndex, oLvlNode ) )
					{
						OOX::Numbering::CNumLvl *pNumLvl = new OOX::Numbering::CNumLvl (oLvlNode);
						if (pNumLvl)
						{
							if (pNumLvl->m_oIlvl.IsInit())
								m_mapLvlOverride.insert(std::make_pair(*pNumLvl->m_oIlvl, m_arrLvlOverride.size()));

							m_arrLvlOverride.push_back(pNumLvl);
						}

					}
				}
			}
		}
		void CNum::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( _T("w:lvlOverride") == sName )
				{
					OOX::Numbering::CNumLvl *pNumLvl = new OOX::Numbering::CNumLvl (oReader);
					if (pNumLvl)
					{
						if (pNumLvl->m_oIlvl.IsInit())
							m_mapLvlOverride.insert(std::make_pair(*pNumLvl->m_oIlvl, m_arrLvlOverride.size()));

						m_arrLvlOverride.push_back(pNumLvl);
					}
				}
				else if ( L"w:abstractNumId" == sName || L"w:ilst" == sName)
					m_oAbstractNumId = oReader;
			}
		}
		std::wstring CNum::toXML() const
		{
			std::wstring sResult = L"<w:num ";

			if (m_oNumId.IsInit())
				sResult += L"w:numId=\"" + std::to_wstring(*m_oNumId) + L"\" ";

			sResult += _T(">");

			WritingElement_WriteNode_1( L"<w:abstractNumId ", m_oAbstractNumId );

			for (unsigned int nIndex = 0; nIndex < m_arrLvlOverride.size(); nIndex++ )
			{
				if (m_arrLvlOverride[nIndex])
					sResult += m_arrLvlOverride[nIndex]->toXML();
			}

			sResult += L"</w:num>";

			return sResult;
		}
		EElementType CNum::getType() const
		{
			return et_w_num;
		}
		void CNum::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("w:numId"), m_oNumId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("w:ilfo"), m_oNumId )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// NumPicBullet 17.9.21 (Part 1)
		//--------------------------------------------------------------------------------

		CNumPicBullet::CNumPicBullet()
		{
		}
		CNumPicBullet::~CNumPicBullet()
		{
		}
		void CNumPicBullet::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, _T("w:numPicBulletId"), m_oNumPicBulletId );

			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, _T("w:drawing"), m_oDrawing );
		}
		void CNumPicBullet::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( _T("w:drawing") == sName )
				{
					m_oDrawing = oReader;
				}
				if ( _T("w:pict") == sName )
				{
					m_oVmlDrawing = oReader;
				}
			}
		}
		std::wstring CNumPicBullet::toXML() const
		{
			if (false == m_oNumPicBulletId.IsInit()) return L"";

			std::wstring sResult = L"<w:numPicBullet ";

			sResult += std::to_wstring(*m_oNumPicBulletId) + L">";
			sResult += m_oDrawing->toXML();
			sResult += L"</w:numPicBullet>";

			return sResult;
		}
		EElementType CNumPicBullet::getType() const
		{
			return et_w_numPicBullet;
		}
		void CNumPicBullet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:numPicBulletId", m_oNumPicBulletId )
				WritingElement_ReadAttributes_End( oReader )
			}

	} // Numbering
} // OOX

namespace OOX
{
	CNumbering::CNumbering(OOX::Document *pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);

		if (docx)
		{
			if (docx->m_bGlossaryRead)	docx->m_oGlossary.numbering = this;
			else						docx->m_oMain.numbering = this;
		}
	}
	CNumbering::CNumbering(OOX::Document *pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);

		if (docx)
		{
			if (docx->m_bGlossaryRead)	docx->m_oGlossary.numbering = this;
			else						docx->m_oMain.numbering = this;
		}
		read( oRootPath, oPath );
	}
	CNumbering::CNumbering(XmlUtils::CXmlNode& oNode) : File(NULL), OOX::IFileContainer(NULL)
	{
		fromXML( oNode );
	}
	CNumbering::CNumbering(XmlUtils::CXmlLiteReader& oReader) : File(NULL), OOX::IFileContainer(NULL)
	{
		fromXML( oReader );
	}
	CNumbering::~CNumbering()
	{
		for ( unsigned int nIndex = 0; nIndex < m_arrNumPicBullet.size(); nIndex++ )
		{
			if (m_arrNumPicBullet[nIndex])delete m_arrNumPicBullet[nIndex];
			m_arrNumPicBullet[nIndex] = NULL;
		}
		m_arrNumPicBullet.clear();

		for ( unsigned int nIndex = 0; nIndex < m_arrNum.size(); nIndex++ )
		{
			if (m_arrNum[nIndex])delete m_arrNum[nIndex];
			m_arrNum[nIndex] = NULL;
		}
		m_arrNum.clear();
		for ( unsigned int nIndex = 0; nIndex < m_arrAbstractNum.size(); nIndex++ )
		{
			if (m_arrAbstractNum[nIndex])delete m_arrAbstractNum[nIndex];
			m_arrAbstractNum[nIndex] = NULL;
		}

		m_arrAbstractNum.clear();
	}
	const CNumbering& CNumbering::operator =(const XmlUtils::CXmlNode& oNode)
	{
		fromXML( (XmlUtils::CXmlNode&)oNode );
		return *this;
	}
	const CNumbering& CNumbering::operator =(const XmlUtils::CXmlLiteReader& oReader)
	{
		fromXML( (XmlUtils::CXmlLiteReader&)oReader );
		return *this;
	}
	void CNumbering::read(const CPath& oFilePath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CNumbering::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read( oRootPath, oFilePath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( _T("w:numbering") == sName && !oReader.IsEmptyNode() )
		{
			fromXML(oReader);
		}
	}
	void CNumbering::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml = toXML();
        NSFile::CFileBinary::SaveToFile( oFilePath.GetPath(), sXml );

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
	}
	void CNumbering::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	void CNumbering::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		//w:numbering or w:lists (lists element [Word 2003 XML])
		if ( oReader.IsEmptyNode() )
			return;

		int nNumberingDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nNumberingDepth ) )
		{
			std::wstring sName = oReader.GetName();
			if ( L"w:abstractNum" == sName || L"w:listDef" == sName)
			{
				OOX::Numbering::CAbstractNum *pAbstractNum = new OOX::Numbering::CAbstractNum(oReader);
				if ((pAbstractNum) && (pAbstractNum->m_oAbstractNumId.IsInit()))
				{
					m_mapAbstractNum.insert(std::make_pair(*pAbstractNum->m_oAbstractNumId, m_arrAbstractNum.size()));
					m_arrAbstractNum.push_back(pAbstractNum);
				}
			}
			else if ( L"w:num" == sName || L"w:list" == sName)
			{
				OOX::Numbering::CNum *oNum = new OOX::Numbering::CNum(oReader);
				if (oNum)
				{
					if (oNum->m_oNumId.IsInit() && (oNum->m_oAbstractNumId.IsInit()) && (oNum->m_oAbstractNumId->m_oVal.IsInit()))
					{
						m_mapNum.insert(std::make_pair(*oNum->m_oNumId, std::make_pair(*oNum->m_oAbstractNumId->m_oVal, m_arrNum.size())));
					}
					m_arrNum.push_back(oNum);
				}
			}
			else if ( L"w:numIdMacAtCleanup" == sName || L"w:ilfoMacAtCleanup" == sName)
				m_oNumIdMacAtCleanup = oReader;
			else if ( L"w:numPicBullet" == sName || L"w:listPicBullet" == sName)
			{
				OOX::Numbering::CNumPicBullet *oNumPic =  new OOX::Numbering::CNumPicBullet(oReader);
				if (oNumPic) m_arrNumPicBullet.push_back( oNumPic );
			}
		}
	}
	std::wstring CNumbering::toXML() const
	{
		std::wstring sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<w:numbering \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
mc:Ignorable=\"w14 w15\">");

		for (unsigned int nIndex = 0; nIndex < m_arrAbstractNum.size(); nIndex++ )
		{
			if (m_arrAbstractNum[nIndex])
				sXml += m_arrAbstractNum[nIndex]->toXML();
		}

		for (unsigned  int nIndex = 0; nIndex < m_arrNum.size(); nIndex++ )
		{
			if (m_arrNum[nIndex])
				sXml += m_arrNum[nIndex]->toXML();
		}

		if ( m_oNumIdMacAtCleanup.IsInit() )
		{
			sXml += _T("<w:numIdMacAtCleanup ");
			sXml += m_oNumIdMacAtCleanup->ToString();
			sXml += _T("/>");
		}

		for (unsigned int nIndex = 0; nIndex < m_arrNumPicBullet.size(); nIndex++ )
		{
			if (m_arrNumPicBullet[nIndex])
				sXml += m_arrNumPicBullet[nIndex]->toXML();
		}

		sXml += _T("</w:numbering>");
		return sXml;
	}
	const OOX::FileType CNumbering::type() const
	{
		return FileTypes::Numbering;
	}
	const CPath CNumbering::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CNumbering::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	EElementType CNumbering::getType() const
		{
			return et_w_numbering;
		}

} // namespace OOX
