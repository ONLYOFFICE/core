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
#ifndef OOX_NUMBERING_FILE_INCLUDE_H_
#define OOX_NUMBERING_FILE_INCLUDE_H_

#include "File.h"
#include "WritingElement.h"

#include "Logic/ParagraphProperty.h"
#include "Logic/RunProperty.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// LvlLegacy 17.9.5 (Part 1)
		//--------------------------------------------------------------------------------
		class CLvlLegacy : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLvlLegacy)
			CLvlLegacy()
			{
			}
			virtual ~CLvlLegacy()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:legacy"),       m_oLegacy );
				XmlMacroReadAttributeBase( oNode, _T("w:legacyIndent"), m_oLegacyIndent );
				XmlMacroReadAttributeBase( oNode, _T("w:legacySpace"),  m_oLegacySpace );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
			{
                std::wstring sResult;

				ComplexTypes_WriteAttribute( _T("w:legacy=\""),       m_oLegacy );
				ComplexTypes_WriteAttribute( _T("w:legacyIndent=\""), m_oLegacyIndent );
				ComplexTypes_WriteAttribute( _T("w:legacySpace=\""),  m_oLegacySpace );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:legacy"),       m_oLegacy )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:legacyIndent"), m_oLegacyIndent )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:legacySpace"),  m_oLegacySpace )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::COnOff<>             > m_oLegacy;
			nullable<SimpleTypes::CSignedTwipsMeasure  > m_oLegacyIndent;
			nullable<SimpleTypes::CTwipsMeasure        > m_oLegacySpace;
		};

		//--------------------------------------------------------------------------------
		// LevelText 17.9.12 (Part 1)
		//--------------------------------------------------------------------------------
		class CLevelText : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLevelText)
			CLevelText()
			{
			}
			virtual ~CLevelText()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:null"), m_oNull );
				XmlMacroReadAttributeBase( oNode, _T("w:val"),  m_sVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
			{
                std::wstring sResult;

				ComplexTypes_WriteAttribute( _T("w:null=\""), m_oNull );

				if ( m_sVal.IsInit() )
				{
					sResult += _T("w:val=\"");
                    sResult += m_sVal.get2();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:null"), m_oNull )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),  m_sVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable< SimpleTypes::COnOff<> >	m_oNull;
            nullable< std::wstring >			m_sVal;
		};

		//--------------------------------------------------------------------------------
		// LevelSuffix 17.9.29 (Part 1)
		//--------------------------------------------------------------------------------
		class CLevelSuffix : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLevelSuffix)
			CLevelSuffix()
			{
			}
			virtual ~CLevelSuffix()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
			{
                std::wstring sResult;

				ComplexTypes_WriteAttribute( _T("w:val=\""), m_oVal );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CLevelSuffix<>> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// MultiLevelType 17.9.13 (Part 1)
		//--------------------------------------------------------------------------------
		class CMultiLevelType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CMultiLevelType)
			CMultiLevelType()
			{
			}
			virtual ~CMultiLevelType()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"), m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring ToString() const
			{
                std::wstring sResult;

				ComplexTypes_WriteAttribute( _T("w:val=\""), m_oVal );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CMultiLevelType<>> m_oVal;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Numbering
	{
		//--------------------------------------------------------------------------------
		// Lvl 17.9.6 (Part 1)
		//--------------------------------------------------------------------------------
		class CLvl : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLvl)
			CLvl()
			{
			}
			virtual ~CLvl()
			{
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( _T("w:lvl") != oNode.GetName() )
					return;

				XmlMacroReadAttributeBase( oNode, _T("w:ilvl"),      m_oIlvl );
				XmlMacroReadAttributeBase( oNode, _T("w:tentative"), m_oTentative );
				XmlMacroReadAttributeBase( oNode, _T("w:tplc"),      m_oTplc );

				XmlUtils::CXmlNode oChild;
				
				WritingElement_ReadNode( oNode, oChild, _T("w:isLgl"),          m_oIsLgl );
				WritingElement_ReadNode( oNode, oChild, _T("w:legacy"),         m_oLegacy );
				WritingElement_ReadNode( oNode, oChild, _T("w:lvlJc"),          m_oLvlJc );
				WritingElement_ReadNode( oNode, oChild, _T("w:lvlPicBulletId"), m_oLvlPicBulletId );
				WritingElement_ReadNode( oNode, oChild, _T("w:lvlRestart"),     m_oLvlRestart );
				WritingElement_ReadNode( oNode, oChild, _T("w:lvlText"),        m_oLvlText );
				WritingElement_ReadNode( oNode, oChild, _T("w:numFmt"),         m_oNumFmt );
				WritingElement_ReadNode( oNode, oChild, _T("w:pPr"),            m_oPPr );
				WritingElement_ReadNode( oNode, oChild, _T("w:pStyle"),         m_oPStyle );
				WritingElement_ReadNode( oNode, oChild, _T("w:rPr"),            m_oRPr );
				WritingElement_ReadNode( oNode, oChild, _T("w:start"),          m_oStart );
				WritingElement_ReadNode( oNode, oChild, _T("w:suff"),           m_oSuffix );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if      ( _T("w:isLgl")          == sName ) m_oIsLgl = oReader;
					else if ( _T("w:legacy")         == sName ) m_oLegacy = oReader;
					else if ( _T("w:lvlJc")          == sName ) m_oLvlJc = oReader;
					else if ( _T("w:lvlPicBulletId") == sName ) m_oLvlPicBulletId = oReader;
					else if ( _T("w:lvlRestart")     == sName ) m_oLvlRestart = oReader;
					else if ( _T("w:lvlText")        == sName ) m_oLvlText = oReader;
					else if ( _T("w:numFmt")         == sName ) m_oNumFmt = oReader;
					else if ( _T("w:pPr")            == sName ) m_oPPr = oReader;
					else if ( _T("w:pStyle")         == sName ) m_oPStyle = oReader;
					else if ( _T("w:rPr")            == sName ) m_oRPr = oReader;
					else if ( _T("w:start")          == sName ) m_oStart = oReader;
					else if ( _T("w:suff")           == sName ) m_oSuffix = oReader;

				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:lvl ");

				ComplexTypes_WriteAttribute( _T("w:ilvl=\""),      m_oIlvl );
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

			virtual EElementType getType() const
			{
				return et_w_lvl;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:ilvl"),      m_oIlvl )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:tentative"), m_oTentative )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:tplc"),      m_oTplc )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes

			nullable<SimpleTypes::CDecimalNumber<> > m_oIlvl;
			nullable<SimpleTypes::COnOff<>         > m_oTentative;
			nullable<SimpleTypes::CLongHexNumber<> > m_oTplc;

			// Childs
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oIsLgl;
			nullable<ComplexTypes::Word::CLvlLegacy                      > m_oLegacy;
			nullable<ComplexTypes::Word::CJc                             > m_oLvlJc;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oLvlPicBulletId;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oLvlRestart;
			nullable<ComplexTypes::Word::CLevelText                      > m_oLvlText;
			nullable<ComplexTypes::Word::CNumFmt                         > m_oNumFmt;
			nullable<OOX::Logic::CParagraphProperty                      > m_oPPr;
			nullable<ComplexTypes::Word::String                        > m_oPStyle;
			nullable<OOX::Logic::CRunProperty                            > m_oRPr;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oStart;
			nullable<ComplexTypes::Word::CLevelSuffix                    > m_oSuffix;

		};
		//--------------------------------------------------------------------------------
		// AbstractNum 17.9.1 (Part 1)
		//--------------------------------------------------------------------------------
		class CAbstractNum : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAbstractNum)
			CAbstractNum()
			{
			}
			virtual ~CAbstractNum()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrLvl.size(); nIndex++ )
				{
					if ( m_arrLvl[nIndex] ) delete m_arrLvl[nIndex];
					m_arrLvl[nIndex] = NULL;
				}
				m_arrLvl.clear();
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( _T("w:abstractNum") != oNode.GetName() )
					return;

				XmlMacroReadAttributeBase( oNode, _T("w:abstractNumId"), m_oAbstractNumId );

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
							OOX::Numbering::CLvl *oLvl = new OOX::Numbering::CLvl(oLvlNode);
							if (oLvl)m_arrLvl.push_back( oLvl );
						}
					}
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
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
						OOX::Numbering::CLvl *oLvl = new OOX::Numbering::CLvl(oReader);
						if (oLvl)m_arrLvl.push_back( oLvl );
					}
					else if ( _T("w:multiLevelType") == sName ) m_oMultiLevelType = oReader;
					else if ( _T("w:name")           == sName ) m_oName = oReader;
					else if ( _T("w:nsid")           == sName ) m_oNsid = oReader;
					else if ( _T("w:numStyleLink")   == sName ) m_oNumStyleLink = oReader;
					else if ( _T("w:styleLink")      == sName ) m_oStyleLink = oReader;
					else if ( _T("w:tmpl")           == sName ) m_oTmpl = oReader;
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:abstractNum ");

				ComplexTypes_WriteAttribute( _T("w:abstractNumId=\""), m_oAbstractNumId );

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

			virtual EElementType getType() const
			{
				return et_w_abstractNum;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:abstractNumId"), m_oAbstractNumId )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes

			nullable<SimpleTypes::CDecimalNumber<> > m_oAbstractNumId;

			// Childs
			std::vector<OOX::Numbering::CLvl*                            > m_arrLvl;
			nullable<ComplexTypes::Word::CMultiLevelType                 > m_oMultiLevelType;
			nullable<ComplexTypes::Word::String                        > m_oName;
			nullable<ComplexTypes::Word::CLongHexNumber                  > m_oNsid;
			nullable<ComplexTypes::Word::String                        > m_oNumStyleLink;
			nullable<ComplexTypes::Word::String                        > m_oStyleLink;
			nullable<ComplexTypes::Word::CLongHexNumber                  > m_oTmpl;
		};
		//--------------------------------------------------------------------------------
		// NumLvl 17.9.9 (Part 1)
		//--------------------------------------------------------------------------------
		class CNumLvl : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNumLvl)
			CNumLvl()
			{
			}
			virtual ~CNumLvl()
			{
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( _T("w:lvlOverride") != oNode.GetName() )
					return;

				XmlMacroReadAttributeBase( oNode, _T("w:ilvl"), m_oIlvl );

				XmlUtils::CXmlNode oChild;
				
				WritingElement_ReadNode( oNode, oChild, _T("w:lvl"),           m_oLvl );
				WritingElement_ReadNode( oNode, oChild, _T("w:startOverride"), m_oStartOverride );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
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
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:lvlOverride ");

				ComplexTypes_WriteAttribute( _T("w:ilvl=\""), m_oIlvl );

				sResult += _T(">");

				WritingElement_WriteNode_2( m_oLvl );
				WritingElement_WriteNode_1( _T("<w:startOverride "), m_oStartOverride );
	
				sResult += _T("</w:lvlOverride>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_lvlOverride;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:ilvl"), m_oIlvl )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes

			nullable<SimpleTypes::CDecimalNumber<> > m_oIlvl;

			// Childs
			nullable<OOX::Numbering::CLvl               > m_oLvl;
			nullable<ComplexTypes::Word::CDecimalNumber > m_oStartOverride;
		};
		//--------------------------------------------------------------------------------
		// Num 17.9.16 (Part 1)
		//--------------------------------------------------------------------------------
		class CNum : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNum)
			CNum()
			{
			}
			virtual ~CNum()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrLvlOverride.size(); nIndex++ )
				{
					if ( m_arrLvlOverride[nIndex] ) delete m_arrLvlOverride[nIndex];
					m_arrLvlOverride[nIndex] = NULL;
				}
				m_arrLvlOverride.clear();
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( _T("w:num") != oNode.GetName() )
					return;

				XmlMacroReadAttributeBase( oNode, _T("w:numId"), m_oNumId );

				XmlUtils::CXmlNode oChild;
				
				WritingElement_ReadNode( oNode, oChild, _T("w:abstractNumId"), m_oAbstractNumId );

				XmlUtils::CXmlNodes oLvlList;
				if ( oNode.GetNodes( _T("w:lvlOverride"), oLvlList ) )
				{
					XmlUtils::CXmlNode oLvlNode;
					for ( int nIndex = 0; nIndex < oLvlList.GetCount(); nIndex++ )
					{
						if ( oLvlList.GetAt( nIndex, oLvlNode ) )
						{
							OOX::Numbering::CNumLvl *oNumLvl = new OOX::Numbering::CNumLvl (oLvlNode);
							if (oNumLvl) m_arrLvlOverride.push_back( oNumLvl );

						}
					}
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
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
						OOX::Numbering::CNumLvl *oNumLvl = new OOX::Numbering::CNumLvl (oReader);
						if (oNumLvl) m_arrLvlOverride.push_back( oNumLvl );
					}
					else if ( _T("w:abstractNumId") == sName )
						m_oAbstractNumId = oReader;
				}
			}
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:num ");

				ComplexTypes_WriteAttribute( _T("w:numId=\""), m_oNumId );

				sResult += _T(">");

				WritingElement_WriteNode_1( _T("<w:abstractNumId "), m_oAbstractNumId );

				for (unsigned int nIndex = 0; nIndex < m_arrLvlOverride.size(); nIndex++ )
				{
					if (m_arrLvlOverride[nIndex])
						sResult += m_arrLvlOverride[nIndex]->toXML();
				}

				sResult += _T("</w:num>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_num;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:numId"), m_oNumId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:

			// Attributes

			nullable<SimpleTypes::CDecimalNumber<> >		m_oNumId;

			// Childs
			nullable<ComplexTypes::Word::CDecimalNumber >	m_oAbstractNumId;
			std::vector<OOX::Numbering::CNumLvl*        >	m_arrLvlOverride;
		};
		//--------------------------------------------------------------------------------
		// NumPicBullet 17.9.21 (Part 1)
		//--------------------------------------------------------------------------------
		class CNumPicBullet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNumPicBullet)
			CNumPicBullet()
			{
			}
			virtual ~CNumPicBullet()
			{
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:numPicBulletId"), m_oNumPicBulletId );

				XmlUtils::CXmlNode oChild;
				
				WritingElement_ReadNode( oNode, oChild, _T("w:drawing"), m_oDrawing );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
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
            virtual std::wstring      toXML() const
			{
                std::wstring sResult = _T("<w:numPicBullet ");

				sResult += m_oNumPicBulletId.ToString() + _T(">");

				sResult += m_oDrawing->toXML();

				sResult += _T("</w:numPicBullet>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_numPicBullet;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:numPicBulletId"), m_oNumPicBulletId )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:

			// Attributes
			SimpleTypes::CDecimalNumber<> m_oNumPicBulletId;

			// Childs
			nullable<OOX::Logic::CDrawing>			m_oDrawing;
			nullable<OOX::Logic::CPicture>			m_oVmlDrawing;
		};
	} // Numbering
} // OOX 

namespace OOX
{
	class CNumbering : public OOX::File, public IFileContainer
	{
	public:
		CNumbering(OOX::Document *pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
		{
			CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
			if (docx) docx->m_pNumbering = this;			
		}
		CNumbering(OOX::Document *pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
		{
			CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
			if (docx) docx->m_pNumbering = this;			

			read( oRootPath, oPath );
		}
		virtual ~CNumbering()
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
	public:

		virtual void read(const CPath& oFilePath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oFilePath);
		}
		virtual void read(const CPath& oRootPath, const CPath& oFilePath)
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
				int nNumberingDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nNumberingDepth ) )
				{
					sName = oReader.GetName();
					if ( _T("w:abstractNum") == sName )
					{
						OOX::Numbering::CAbstractNum *oAbstractNum = new OOX::Numbering::CAbstractNum(oReader);
						if (oAbstractNum) m_arrAbstractNum.push_back( oAbstractNum );
					}
					else if ( _T("w:num") == sName )
					{
						OOX::Numbering::CNum *oNum = new OOX::Numbering::CNum(oReader);
						if (oNum) m_arrNum.push_back( oNum );
					}
					else if ( _T("w:numIdMacAtCleanup") == sName )
						m_oNumIdMacAtCleanup = oReader;
					else if ( _T("w:numPicBullet") == sName )
					{
						OOX::Numbering::CNumPicBullet *oNumPic =  new OOX::Numbering::CNumPicBullet(oReader);
						if (oNumPic) m_arrNumPicBullet.push_back( oNumPic );
					}
				}
			}
		}

		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
            std::wstring sXml;
			sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:numbering xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\">");

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

			CDirectory::SaveToFile( oFilePath.GetPath(), sXml );
		}

	public:

		virtual const OOX::FileType type() const
		{
			return FileTypes::Numbering;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

	public:

		std::vector<OOX::Numbering::CAbstractNum  *> m_arrAbstractNum;
		std::vector<OOX::Numbering::CNum          *> m_arrNum;
		nullable<ComplexTypes::Word::CDecimalNumber> m_oNumIdMacAtCleanup;
		std::vector<OOX::Numbering::CNumPicBullet *> m_arrNumPicBullet;

	};
} // namespace OOX

#endif // OOX_NUMBERING_FILE_INCLUDE_H_
