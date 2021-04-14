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

#include "File.h"
#include "WritingElement.h"
#include "FileTypes.h"

#include "Logic/RunProperty.h"
#include "Logic/ParagraphProperty.h"
#include "Logic/TableProperty.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// LsdException 17.7.4.8 (Part 1)
		//--------------------------------------------------------------------------------
		class CLsdException : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLsdException)
			CLsdException()
			{
			}
			virtual ~CLsdException()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:locked",         m_oLocked );
				XmlMacroReadAttributeBase( oNode, L"w:name",           m_sName );
				XmlMacroReadAttributeBase( oNode, L"w:qFormat",        m_oQFormat );
				XmlMacroReadAttributeBase( oNode, L"w:semiHidden",     m_oSemiHidden );
				XmlMacroReadAttributeBase( oNode, L"w:uiPriority",     m_oUiPriority );
				XmlMacroReadAttributeBase( oNode, L"w:unhideWhenUsed", m_oUnhideWhenUsed );
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

				ComplexTypes_WriteAttribute( L"w:locked=\"", m_oLocked );

				if ( m_sName.IsInit() )
				{
					sResult += L"w:name=\"";
                    sResult += m_sName.get2();
					sResult += L"\" ";
				}

				ComplexTypes_WriteAttribute( L"w:qFormat=\"",        m_oQFormat );
				ComplexTypes_WriteAttribute( L"w:semiHidden=\"",     m_oSemiHidden );
				ComplexTypes_WriteAttribute( L"w:uiPriority=\"",     m_oUiPriority );
				ComplexTypes_WriteAttribute( L"w:unhideWhenUsed=\"", m_oUnhideWhenUsed );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:locked",         m_oLocked )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:name",           m_sName )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:qFormat",        m_oQFormat )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:semiHidden",     m_oSemiHidden )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:uiPriority",     m_oUiPriority )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:unhideWhenUsed", m_oUnhideWhenUsed )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::COnOff<>           > m_oLocked;
			nullable<std::wstring                         > m_sName;
			nullable<SimpleTypes::COnOff<>           > m_oQFormat;
			nullable<SimpleTypes::COnOff<>           > m_oSemiHidden;
			nullable<SimpleTypes::CDecimalNumber<>   > m_oUiPriority;
			nullable<SimpleTypes::COnOff<>           > m_oUnhideWhenUsed;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CTableStyleProperties
		//--------------------------------------------------------------------------------
		class CTableStyleProperties : public WritingElement
		{
		public:
			CTableStyleProperties()
			{
			}
			CTableStyleProperties(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CTableStyleProperties(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CTableStyleProperties()
			{
			}
			const CTableStyleProperties& operator=(const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode &)oNode );
				return (*this);
			}
			const CTableStyleProperties& operator=(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return (*this);
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:tblStylePr" != oNode.GetName() )
					return;

				XmlMacroReadAttributeBase( oNode, L"w:type", m_oType );

				XmlUtils::CXmlNode oChild;
				WritingElement_ReadNode( oNode, oChild, L"w:pPr",   m_oParPr );
				WritingElement_ReadNode( oNode, oChild, L"w:rPr",   m_oRunPr );
				WritingElement_ReadNode( oNode, oChild, L"w:tblPr", m_oTblPr );
				WritingElement_ReadNode( oNode, oChild, L"w:tcPr",  m_oTcPr );
				WritingElement_ReadNode( oNode, oChild, L"w:trPr",  m_oTrPr );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if      ( L"w:pPr" == sName ) m_oParPr = oReader;
					else if ( L"w:rPr" == sName ) m_oRunPr = oReader;
					else if ( L"w:tblPr" == sName ) m_oTblPr = oReader;
					else if ( L"w:tcPr" == sName ) m_oTcPr = oReader;
					else if ( L"w:trPr" == sName ) m_oTrPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult;

				if ( m_oType.IsInit() )
				{
					sResult = L"<w:tblStylePr w:type=\"";
					sResult += m_oType->ToString();
					sResult += L"\">";
				}
				else
					sResult = L"<w:tblStylePr>";

				WritingElement_WriteNode_2( m_oParPr );
				WritingElement_WriteNode_2( m_oRunPr );
				WritingElement_WriteNode_2( m_oTblPr );
				WritingElement_WriteNode_2( m_oTcPr );
				WritingElement_WriteNode_2( m_oTrPr );

				sResult += L"</w:tblStylePr>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_tblStylePr;
			}
			static const CTableStyleProperties Merge(const CTableStyleProperties& oPrev, const CTableStyleProperties& oCurrent)
			{
				CTableStyleProperties oProperties;
				oProperties.m_oType            = Merge( oPrev.m_oType,            oCurrent.m_oType );
				oProperties.m_oParPr            = Merge( oPrev.m_oParPr,            oCurrent.m_oParPr );
				oProperties.m_oRunPr            = Merge( oPrev.m_oRunPr,            oCurrent.m_oRunPr );
				oProperties.m_oTblPr            = Merge( oPrev.m_oTblPr,            oCurrent.m_oTblPr );
				oProperties.m_oTcPr            = Merge( oPrev.m_oTcPr,            oCurrent.m_oTcPr );
				oProperties.m_oTrPr            = Merge( oPrev.m_oTrPr,            oCurrent.m_oTrPr );
				return oProperties;
			}
			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:type", m_oType )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CTblStyleOverrideType<> > m_oType;

			nullable<OOX::Logic::CParagraphProperty  >      m_oParPr;
			nullable<OOX::Logic::CRunProperty        >      m_oRunPr;
			nullable<OOX::Logic::CTableProperty      >      m_oTblPr;
			nullable<OOX::Logic::CTableCellProperties>      m_oTcPr;
			nullable<OOX::Logic::CTableRowProperties >      m_oTrPr;
		};

	}
	//--------------------------------------------------------------------------------
	// DocDefaults 17.7.5.1 (Part 1)
	//--------------------------------------------------------------------------------
	class CDocDefaults : public WritingElement
	{
	public:

		CDocDefaults()
		{
		}
		CDocDefaults(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CDocDefaults(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		virtual ~CDocDefaults()
		{
		}
		const CDocDefaults& operator=(const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode &)oNode );
			return *this;
		}
		const CDocDefaults& operator=(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( L"w:docDefaults" != oNode.GetName() )
				return;

			XmlUtils::CXmlNode oDefault;
			XmlUtils::CXmlNode oPr;

			if ( oNode.GetNode( L"w:pPrDefault", oDefault ) )
			{
				if ( oDefault.GetNode( L"w:pPr", oPr ) )
					m_oParPr = oPr;
			}

			if ( oNode.GetNode( L"w:rPrDefault", oDefault ) )
			{
				if ( oDefault.GetNode( L"w:rPr", oPr ) )
					m_oRunPr = oPr;
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
				if ( L"w:pPrDefault" == sName && !oReader.IsEmptyNode() )
				{
					int nParentDepth2 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth2 ) )
					{
						sName = oReader.GetName();
						if ( L"w:pPr" == sName )
							m_oParPr = oReader;
					}
				}
				else if ( L"w:rPrDefault" == sName && !oReader.IsEmptyNode() )
				{
					int nParentDepth2 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth2 ) )
					{
						sName = oReader.GetName();
						if ( L"w:rPr" == sName )
							m_oRunPr = oReader;
					}
				}
			}
		}
        virtual std::wstring toXML() const
		{
			std::wstring sResult = L"<w:docDefaults>";

			if ( m_oParPr.IsInit() )
			{
				sResult += L"<w:pPrDefault>";
				sResult += m_oParPr->toXML();
				sResult += L"</w:pPrDefault>";
			}

			if ( m_oRunPr.IsInit() )
			{
				sResult += L"<w:rPrDefault>";
				sResult += m_oRunPr->toXML();
				sResult += L"</w:rPrDefault>";
			}

			sResult += L"</w:docDefaults>";

			return sResult;
		}

		virtual EElementType getType() const
		{
			return et_w_docDefaults;
		}
		nullable<OOX::Logic::CParagraphProperty> m_oParPr;
		nullable<OOX::Logic::CRunProperty      > m_oRunPr;
	};

	//--------------------------------------------------------------------------------
	// LatentStyles 17.7.4.6 (Part 1)
	//--------------------------------------------------------------------------------
	class CLatentStyles : public WritingElement
	{
	public:

		CLatentStyles()
		{
		}
		CLatentStyles(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CLatentStyles(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		virtual ~CLatentStyles()
		{
            for ( unsigned int nIndex = 0; nIndex < m_arrLsdException.size(); nIndex++ )
			{
				if ( m_arrLsdException[nIndex] ) delete m_arrLsdException[nIndex];
				m_arrLsdException[nIndex] = NULL;
			}
			m_arrLsdException.clear();
		}
		const CLatentStyles& operator=(const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode &)oNode );
			return *this;
		}
		const CLatentStyles& operator=(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( L"w:latentStyles" != oNode.GetName() )
				return;

			XmlMacroReadAttributeBase( oNode, L"w:count",             m_oCount );
			XmlMacroReadAttributeBase( oNode, L"w:defLockedState",    m_oDefLockedState );
			XmlMacroReadAttributeBase( oNode, L"w:defQFormat",        m_oDefQFormat );
			XmlMacroReadAttributeBase( oNode, L"w:defSemiHidden",     m_oDefSemiHidden );
			XmlMacroReadAttributeBase( oNode, L"w:defUIPriority",     m_oDefUiPriority );
			XmlMacroReadAttributeBase( oNode, L"w:defUnhideWhenUsed", m_oDefUnhideWhenUsed );

			XmlUtils::CXmlNodes oList;
			if ( oNode.GetNodes( L"w:lsdException", oList ) )
			{
				XmlUtils::CXmlNode oLsdNode;
				for ( int nIndex = 0; nIndex < oList.GetCount(); nIndex++ )
				{
					if ( oList.GetAt( nIndex, oLsdNode ) )
					{
						ComplexTypes::Word::CLsdException *oLsdException = new ComplexTypes::Word::CLsdException(oLsdNode);
						if (oLsdException ) m_arrLsdException.push_back( oLsdException );
					}
				}
			}
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:lsdException" == sName )
				{
					ComplexTypes::Word::CLsdException *oLsdException = new ComplexTypes::Word::CLsdException (oReader);
					if (oLsdException) m_arrLsdException.push_back( oLsdException );
				}
			}
		}
        virtual std::wstring toXML() const
		{
			std::wstring sResult;
            sResult += L"<w:latentStyles w:count=\"" + std::to_wstring(m_oCount.GetValue())	+ L"\"";
			sResult += L" w:defLockedState=\""		+ m_oDefLockedState.ToString()	+ L"\"";
			sResult += L" w:defQFormat=\""			+ m_oDefQFormat.ToString()		+ L"\"";
			sResult += L" w:defSemiHidden=\""		+ m_oDefSemiHidden.ToString()	+ L"\"";
            sResult += L" w:defUIPriority=\""		+ std::to_wstring(m_oDefUiPriority.GetValue()) + L"\"";
			sResult += L" w:defUnhideWhenUsed=\""	+ m_oDefUnhideWhenUsed.ToString();
			sResult += L"\">";

			for (unsigned int nIndex = 0; nIndex < m_arrLsdException.size(); nIndex++ )
			{
				sResult += L"<w:lsdException ";
				if (m_arrLsdException[nIndex])
					sResult += m_arrLsdException[nIndex]->ToString();
				sResult += L"/>";
			}

			sResult += L"</w:latentStyles>";

			return sResult;
		}

		virtual EElementType getType() const
		{
			return et_w_latentStyles;
		}
	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:count",             m_oCount )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:defLockedState",    m_oDefLockedState )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:defQFormat",        m_oDefQFormat )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:defSemiHidden",     m_oDefSemiHidden )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:defUIPriority",     m_oDefUiPriority )
			WritingElement_ReadAttributes_Read_else_if(oReader,  L"w:defUnhideWhenUsed", m_oDefUnhideWhenUsed )
			WritingElement_ReadAttributes_End( oReader )
		}
	public:
		SimpleTypes::CDecimalNumber<0>                  m_oCount;
		SimpleTypes::COnOff<SimpleTypes::onoffFalse>    m_oDefLockedState;
		SimpleTypes::COnOff<SimpleTypes::onoffFalse>    m_oDefQFormat;
		SimpleTypes::COnOff<SimpleTypes::onoffFalse>    m_oDefSemiHidden;
		SimpleTypes::CDecimalNumber<99>                 m_oDefUiPriority;
		SimpleTypes::COnOff<SimpleTypes::onoffFalse>    m_oDefUnhideWhenUsed;

		std::vector<ComplexTypes::Word::CLsdException*> m_arrLsdException;
	};

	//--------------------------------------------------------------------------------
	// Style 17.7.4.17 (Part 1)
	//--------------------------------------------------------------------------------
	class CStyle : public WritingElement
	{
	public:

		CStyle(OOX::Document *pMain = NULL) : WritingElement(pMain) 
		{
		}
		CStyle(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CStyle(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		virtual ~CStyle()
		{
			ClearItems();
		}
		void ClearItems()
		{
            for ( unsigned int nIndex = 0; nIndex < m_arrTblStylePr.size(); nIndex++ )
			{
				if ( m_arrTblStylePr[nIndex] ) delete m_arrTblStylePr[nIndex];
				m_arrTblStylePr[nIndex] = NULL;
			}
			m_arrTblStylePr.clear();
		}
		const CStyle& operator=(const XmlUtils::CXmlNode &oNode)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlNode &)oNode );
			return *this;
		}
		const CStyle& operator=(const XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( L"w:style" != oNode.GetName() )
				return;

			XmlMacroReadAttributeBase( oNode, L"w:customStyle", m_oCustomStyle );
			XmlMacroReadAttributeBase( oNode, L"w:default",     m_oDefault );
			XmlMacroReadAttributeBase( oNode, L"w:styleId",     m_sStyleId );
			XmlMacroReadAttributeBase( oNode, L"w:type",        m_oType );

			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, L"w:aliases",         m_oAliases );
			WritingElement_ReadNode( oNode, oChild, L"w:autoRedefine",    m_oAutoRedefine );
			WritingElement_ReadNode( oNode, oChild, L"w:basedOn",         m_oBasedOn );
			WritingElement_ReadNode( oNode, oChild, L"w:hidden",          m_oHidden );
			WritingElement_ReadNode( oNode, oChild, L"w:link",            m_oLink );
			WritingElement_ReadNode( oNode, oChild, L"w:locked",          m_oLocked );
			WritingElement_ReadNode( oNode, oChild, L"w:name",            m_oName );
			WritingElement_ReadNode( oNode, oChild, L"w:next",            m_oNext );
			WritingElement_ReadNode( oNode, oChild, L"w:personal",        m_oPersonal );
			WritingElement_ReadNode( oNode, oChild, L"w:personalCompose", m_oPersonalCompose );
			WritingElement_ReadNode( oNode, oChild, L"w:personalReply",   m_oPersonalReply );
			WritingElement_ReadNode( oNode, oChild, L"w:pPr",             m_oParPr );
			WritingElement_ReadNode( oNode, oChild, L"w:qFormat",         m_oQFormat );
			WritingElement_ReadNode( oNode, oChild, L"w:rPr",             m_oRunPr );
			WritingElement_ReadNode( oNode, oChild, L"w:rsid",            m_oRsid );
			WritingElement_ReadNode( oNode, oChild, L"w:semiHidden",      m_oSemiHidden );
			WritingElement_ReadNode( oNode, oChild, L"w:tblPr",           m_oTblPr );

			XmlUtils::CXmlNodes oList;
			if ( oNode.GetNodes( L"w:tblStylePr", oList ) )
			{
				XmlUtils::CXmlNode oTblStylePrNode;
				for ( int nIndex = 0; nIndex < oList.GetCount(); nIndex++ )
				{
					if ( oList.GetAt( nIndex, oTblStylePrNode ) )
					{
						OOX::Logic::CTableStyleProperties *oTblStylePr = new OOX::Logic::CTableStyleProperties(oTblStylePrNode);
						if (oTblStylePr) m_arrTblStylePr.push_back( oTblStylePr );
					}
				}
			}

			WritingElement_ReadNode( oNode, oChild, L"w:tcPr",            m_oTcPr );
			WritingElement_ReadNode( oNode, oChild, L"w:trPr",            m_oTrPr );
			WritingElement_ReadNode( oNode, oChild, L"w:uiPriority",      m_oUiPriority );
			WritingElement_ReadNode( oNode, oChild, L"w:unhideWhenUsed",  m_oUnhideWhenUsed );
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if      ( L"w:aliases" == sName ) m_oAliases = oReader;
				else if ( L"w:autoRedefine" == sName ) m_oAutoRedefine = oReader;
				else if ( L"w:basedOn" == sName ) m_oBasedOn = oReader;
				else if ( L"w:hidden" == sName ) m_oHidden = oReader;
				else if ( L"w:link" == sName ) m_oLink = oReader;
				else if ( L"w:locked" == sName ) m_oLocked = oReader;
				else if ( L"w:name" == sName ) m_oName = oReader;
				else if ( L"w:next" == sName ) m_oNext = oReader;
				else if ( L"w:personal" == sName ) m_oPersonal = oReader;
				else if ( L"w:personalCompose" == sName ) m_oPersonalCompose = oReader;
				else if ( L"w:personalReply" == sName ) m_oPersonalReply = oReader;
				else if ( L"w:pPr" == sName ) m_oParPr = oReader;
				else if ( L"w:qFormat" == sName ) m_oQFormat = oReader;
				else if ( L"w:rPr" == sName ) m_oRunPr = oReader;
				else if ( L"w:rsid" == sName ) m_oRsid = oReader;
				else if ( L"w:semiHidden" == sName ) m_oSemiHidden = oReader;
				else if ( L"w:tblPr" == sName ) m_oTblPr = oReader;
				else if ( L"w:tblStylePr" == sName )
				{
					OOX::Logic::CTableStyleProperties *oTblStylePr = new OOX::Logic::CTableStyleProperties(oReader);
					m_arrTblStylePr.push_back( oTblStylePr );
				}
				else if ( L"w:tcPr" == sName ) m_oTcPr = oReader;
				else if ( L"w:trPr" == sName ) m_oTrPr = oReader;
				else if ( L"w:uiPriority" == sName ) m_oUiPriority = oReader;
				else if ( L"w:unhideWhenUsed" == sName ) m_oUnhideWhenUsed = oReader;
			}
		}
        virtual std::wstring toXML() const
		{
			std::wstring sResult = L"<w:style ";

			ComplexTypes_WriteAttribute( L"w:customStyle=\"", m_oCustomStyle );
			ComplexTypes_WriteAttribute( L"w:default=\"",     m_oDefault );
			if ( m_sStyleId.IsInit() )
			{
				sResult += L"w:styleId=\"" + *m_sStyleId + L"\" ";
			}
			ComplexTypes_WriteAttribute( L"w:type=\"", m_oType );

			sResult += L">";

			WritingElement_WriteNode_1( L"<w:aliases ",         m_oAliases );
			WritingElement_WriteNode_1( L"<w:autoRedefine ",    m_oAutoRedefine );
			WritingElement_WriteNode_1( L"<w:basedOn ",         m_oBasedOn );
			WritingElement_WriteNode_1( L"<w:hidden ",          m_oHidden );
			WritingElement_WriteNode_1( L"<w:link ",            m_oLink );
			WritingElement_WriteNode_1( L"<w:locked ",          m_oLocked );
			WritingElement_WriteNode_1( L"<w:name ",            m_oName );
			WritingElement_WriteNode_1( L"<w:next ",            m_oNext );
			WritingElement_WriteNode_1( L"<w:personal ",        m_oPersonal );
			WritingElement_WriteNode_1( L"<w:personalCompose ", m_oPersonalCompose );
			WritingElement_WriteNode_1( L"<w:personalReply ",   m_oPersonalReply );
			WritingElement_WriteNode_2( m_oParPr );
			WritingElement_WriteNode_1( L"<w:qFormat ",         m_oQFormat );
			WritingElement_WriteNode_2( m_oRunPr );
			WritingElement_WriteNode_1( L"<w:rsid ",            m_oRsid );
			WritingElement_WriteNode_1( L"<w:semiHidden ",      m_oSemiHidden );
			WritingElement_WriteNode_2( m_oTblPr );

			for ( unsigned int nIndex = 0; nIndex < m_arrTblStylePr.size(); nIndex++ )
			{
				if (m_arrTblStylePr[nIndex])
					sResult += m_arrTblStylePr[nIndex]->toXML();
			}

			WritingElement_WriteNode_2( m_oTcPr );
			WritingElement_WriteNode_2( m_oTrPr );
			WritingElement_WriteNode_1( L"<w:uiPriority ",      m_oUiPriority );
			WritingElement_WriteNode_1( L"<w:unhideWhenUsed ",  m_oUnhideWhenUsed );

			sResult += L"</w:style>";

			return sResult;
		}

		virtual EElementType getType() const
		{
			return et_w_style;
		}
	private:

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:customStyle", m_oCustomStyle )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:default",     m_oDefault )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:styleId",     m_sStyleId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:type",        m_oType )
			WritingElement_ReadAttributes_End( oReader )
		}

	public:

		nullable<SimpleTypes::COnOff<>>		m_oCustomStyle;
		nullable<SimpleTypes::COnOff<>>		m_oDefault;
		nullable_string						m_sStyleId;
		nullable<SimpleTypes::CStyleType<>> m_oType;

		nullable<ComplexTypes::Word::String>							m_oAliases;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oAutoRedefine;
		nullable<ComplexTypes::Word::String>							m_oBasedOn;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oHidden;
		nullable<ComplexTypes::Word::String>							m_oLink;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oLocked;
		nullable<ComplexTypes::Word::String>							m_oName;
		nullable<ComplexTypes::Word::String>							m_oNext;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oPersonal;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oPersonalCompose;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oPersonalReply;
		nullable<OOX::Logic::CParagraphProperty>						m_oParPr;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oQFormat;
		nullable<OOX::Logic::CRunProperty>								m_oRunPr;
		nullable<ComplexTypes::Word::CLongHexNumber>					m_oRsid;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oSemiHidden;
		nullable<OOX::Logic::CTableProperty>							m_oTblPr;		
		std::vector<OOX::Logic::CTableStyleProperties*>					m_arrTblStylePr;
		nullable<OOX::Logic::CTableCellProperties>						m_oTcPr;
		nullable<OOX::Logic::CTableRowProperties>						m_oTrPr;
		nullable<ComplexTypes::Word::CDecimalNumber>					m_oUiPriority;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oUnhideWhenUsed;
	};

	//--------------------------------------------------------------------------------
	// Styles 17.7.4.18 (Part 1)
	//--------------------------------------------------------------------------------
	class CStyles : public OOX::File, public WritingElement
	{
	public:
		CStyles(OOX::Document *pMain) : OOX::File(pMain), WritingElement(pMain)
		{
			CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
			if (docx)
			{
				if (docx->m_bGlossaryRead)	docx->m_oGlossary.styles = this;
				else						docx->m_oMain.styles = this;
			}
		}
		CStyles(OOX::Document *pMain, const CPath& oPath) : OOX::File(pMain), WritingElement(pMain)
		{
			CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
			if (docx)
			{
				if (docx->m_bGlossaryRead)	docx->m_oGlossary.styles = this;
				else						docx->m_oMain.styles = this;
			}

			read( oPath );
		}
		CStyles(XmlUtils::CXmlNode& oNode) : File(NULL), WritingElement(NULL)
		{
			fromXML( oNode );
		}
		CStyles(XmlUtils::CXmlLiteReader& oReader) : File(NULL), WritingElement(NULL)
		{
			fromXML( oReader );
		}
		virtual ~CStyles()
		{
			for (size_t nIndex = 0; nIndex < m_arrStyle.size(); nIndex++ )
			{
				if ( m_arrStyle[nIndex] )delete m_arrStyle[nIndex];
				m_arrStyle[nIndex] = NULL;
			}
			m_arrStyle.clear();

			m_mapStyleNames.clear();
			m_mapStyleDefaults.clear();
			m_mapEmbeddedStyleNames.clear();
		}
		const CStyles& operator =(const XmlUtils::CXmlNode& oNode)
		{
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}

		const CStyles& operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		virtual void read(const CPath& oFilePath)
		{
			XmlUtils::CXmlLiteReader oReader;
			
			if ( !oReader.FromFile( oFilePath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			std::wstring sName = oReader.GetName();
			if ( L"w:styles" == sName)
			{
				fromXML(oReader);
			}
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
			sXml  += toXML();

			CDirectory::SaveToFile( oFilePath.GetPath(), sXml );
		}
		virtual const OOX::FileType type() const
		{
			return FileTypes::Style;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		virtual void fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nStylesDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if ( L"w:style" == sName )
				{
					OOX::CStyle *oStyle = new OOX::CStyle (oReader);
					if (oStyle)
					{
						if (oStyle->m_sStyleId.IsInit())
						{
							m_mapStyleNames.insert(std::make_pair(oStyle->m_sStyleId.get(), m_arrStyle.size()));
						}
						if ((oStyle->m_oType.IsInit() && oStyle->m_oDefault.IsInit()) && (oStyle->m_oDefault->ToBool()))
						{
							m_mapStyleDefaults.insert(std::make_pair(oStyle->m_oType->GetValue(), m_arrStyle.size()));
						}
						m_arrStyle.push_back( oStyle );
					}
				}
				else if ( L"w:docDefaults" == sName )
					m_oDocDefaults = oReader;
				else if ( L"w:latentStyles" == sName )
					m_oLatentStyles = oReader;
			}
		}
        virtual std::wstring toXML() const
		{
			std::wstring sXml = L"<w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\">";

			if ( m_oDocDefaults.IsInit() )
				sXml += m_oDocDefaults->toXML();

			if ( m_oLatentStyles.IsInit() )
				sXml += m_oLatentStyles->toXML();

			for (size_t nIndex = 0; nIndex < m_arrStyle.size(); nIndex++ )
			{
				if (m_arrStyle[nIndex])
					sXml += m_arrStyle[nIndex]->toXML();
			}
			sXml += L"</w:styles>";
			return sXml;
		}
		virtual EElementType getType() const
		{
			return et_w_styles;
		}
//------------------------------------------------------------------------
		nullable<OOX::CDocDefaults>		m_oDocDefaults;
		nullable<OOX::CLatentStyles>	m_oLatentStyles;
		std::vector<OOX::CStyle*>		m_arrStyle;
//------------------------------------------------------------------------
		std::map<SimpleTypes::EStyleType, size_t>			m_mapStyleDefaults;
		std::map<std::wstring, size_t>						m_mapStyleNames;
		std::vector<std::map<std::wstring, std::wstring>>	m_mapEmbeddedStyleNames;
	};
} // namespace OOX