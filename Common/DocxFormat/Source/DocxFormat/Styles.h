#pragma once
#ifndef OOX_STYLES_INCLUDE_H_
#define OOX_STYLES_INCLUDE_H_

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
				oNode.ReadAttributeBase( _T("w:locked"),         m_oLocked );
				oNode.ReadAttributeBase( _T("w:name"),           m_sName );
				oNode.ReadAttributeBase( _T("w:qFormat"),        m_oQFormat );
				oNode.ReadAttributeBase( _T("w:semiHidden"),     m_oSemiHidden );
				oNode.ReadAttributeBase( _T("w:uiPriority"),     m_oUiPriority );
				oNode.ReadAttributeBase( _T("w:unhideWhenUsed"), m_oUnhideWhenUsed );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString ToString() const
			{
				CString sResult;

				ComplexTypes_WriteAttribute( _T("w:locked=\""),         m_oLocked );

				if ( m_sName.IsInit() )
				{
					sResult += _T("w:name=\"");
					sResult += m_sName->GetString();
					sResult += _T("\" ");
				}

				ComplexTypes_WriteAttribute( _T("w:qFormat=\""),        m_oQFormat );
				ComplexTypes_WriteAttribute( _T("w:semiHidden=\""),     m_oSemiHidden );
				ComplexTypes_WriteAttribute( _T("w:uiPriority=\""),     m_oUiPriority );
				ComplexTypes_WriteAttribute( _T("w:unhideWhenUsed=\""), m_oUnhideWhenUsed );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:locked"),         m_oLocked )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:name"),           m_sName )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:qFormat"),        m_oQFormat )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:semiHidden"),     m_oSemiHidden )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:uiPriority"),     m_oUiPriority )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:unhideWhenUsed"), m_oUnhideWhenUsed )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::COnOff<>           > m_oLocked;
			nullable<CString                         > m_sName;
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
				*this;
			}
			const CTableStyleProperties& operator=(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				*this;
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( _T("w:tblStylePr") != oNode.GetName() )
					return;

				oNode.ReadAttributeBase( _T("w:type"), m_oType );

				XmlUtils::CXmlNode oChild;
				WritingElement_ReadNode( oNode, oChild, _T("w:pPr"),   m_oParPr );
				WritingElement_ReadNode( oNode, oChild, _T("w:rPr"),   m_oRunPr );
				WritingElement_ReadNode( oNode, oChild, _T("w:tblPr"), m_oTblPr );
				WritingElement_ReadNode( oNode, oChild, _T("w:tcPr"),  m_oTcPr );
				WritingElement_ReadNode( oNode, oChild, _T("w:trPr"),  m_oTrPr );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if      ( _T("w:pPr")   == sName ) m_oParPr = oReader;
					else if ( _T("w:rPr")   == sName ) m_oRunPr = oReader;
					else if ( _T("w:tblPr") == sName ) m_oTblPr = oReader;
					else if ( _T("w:tcPr")  == sName ) m_oTcPr = oReader;
					else if ( _T("w:trPr")  == sName ) m_oTrPr = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult;

				if ( m_oType.IsInit() )
				{
					sResult = _T("<w:tblStylePr w:type=\"");
					sResult += m_oType->ToString();
					sResult += _T("\">");
				}
				else
					sResult = _T("<w:tblStylePr>");

				WritingElement_WriteNode_2( m_oParPr );
				WritingElement_WriteNode_2( m_oRunPr );
				WritingElement_WriteNode_2( m_oTblPr );
				WritingElement_WriteNode_2( m_oTcPr );
				WritingElement_WriteNode_2( m_oTrPr );

				sResult += _T("</w:tblStylePr>");

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
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:type"), m_oType )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CTblStyleOverrideType<> > m_oType;

			// Childs
			nullable<OOX::Logic::CParagraphProperty  >      m_oParPr;
			nullable<OOX::Logic::CRunProperty        >      m_oRunPr;
			nullable<OOX::Logic::CTableProperty      >      m_oTblPr;
			nullable<OOX::Logic::CTableCellProperties>      m_oTcPr;
			nullable<OOX::Logic::CTableRowProperties >      m_oTrPr;
		};

	}
} // OOX
namespace OOX
{
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
	public:

		virtual void         fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:docDefaults") != oNode.GetName() )
				return;

			XmlUtils::CXmlNode oDefault;
			XmlUtils::CXmlNode oPr;

			if ( oNode.GetNode( _T("w:pPrDefault"), oDefault ) )
			{
				if ( oDefault.GetNode( _T("w:pPr"), oPr ) )
					m_oParPr = oPr;
			}

			if ( oNode.GetNode( _T("w:rPrDefault"), oDefault ) )
			{
				if ( oDefault.GetNode( _T("w:rPr"), oPr ) )
					m_oRunPr = oPr;
			}

		}
		virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				if ( _T("w:pPrDefault") == sName && !oReader.IsEmptyNode() )
				{
					int nParentDepth2 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth2 ) )
					{
						sName = oReader.GetName();
						if ( _T("w:pPr") == sName )
							m_oParPr = oReader;
					}
				}
				else if ( _T("w:rPrDefault") == sName && !oReader.IsEmptyNode() )
				{
					int nParentDepth2 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth2 ) )
					{
						sName = oReader.GetName();
						if ( _T("w:rPr") == sName )
							m_oRunPr = oReader;
					}
				}
			}
		}
        virtual CString      toXML() const
		{
			CString sResult = _T("<w:docDefaults>");

			if ( m_oParPr.IsInit() )
			{
				sResult += _T("<w:pPrDefault>");
				sResult += m_oParPr->toXML();
				sResult += _T("</w:pPrDefault>");
			}

			if ( m_oRunPr.IsInit() )
			{
				sResult += _T("<w:rPrDefault>");
				sResult += m_oRunPr->toXML();
				sResult += _T("</w:rPrDefault>");
			}

			sResult += _T("</w:docDefaults>");

			return sResult;
		}

		virtual EElementType getType() const
		{
			return et_w_docDefaults;
		}
	public:

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
	public:

		virtual void         fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:latentStyles") != oNode.GetName() )
				return;

			oNode.ReadAttributeBase( _T("w:count"),             m_oCount );
			oNode.ReadAttributeBase( _T("w:defLockedState"),    m_oDefLockedState );
			oNode.ReadAttributeBase( _T("w:defQFormat"),        m_oDefQFormat );
			oNode.ReadAttributeBase( _T("w:defSemiHidden"),     m_oDefSemiHidden );
			oNode.ReadAttributeBase( _T("w:defUIPriority"),     m_oDefUiPriority );
			oNode.ReadAttributeBase( _T("w:defUnhideWhenUsed"), m_oDefUnhideWhenUsed );

			XmlUtils::CXmlNodes oList;
			if ( oNode.GetNodes( _T("w:lsdException"), oList ) )
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
		virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				if ( _T("w:lsdException") == sName )
				{
					ComplexTypes::Word::CLsdException *oLsdException = new ComplexTypes::Word::CLsdException (oReader);
					if (oLsdException) m_arrLsdException.push_back( oLsdException );
				}
			}
		}
        virtual CString      toXML() const
		{
			CString sResult;
			sResult.Format( _T("<w:latentStyles w:count=\"%d\" w:defLockedState=\"%ls\" w:defQFormat=\"%ls\" w:defSemiHidden=\"%ls\" w:defUIPriority=\"%d\" w:defUnhideWhenUsed=\"%ls\">"), m_oCount.GetValue(), m_oDefLockedState.ToString(), m_oDefQFormat.ToString(), m_oDefSemiHidden.ToString(), m_oDefUiPriority.GetValue(), m_oDefUnhideWhenUsed.ToString() );

			for (unsigned int nIndex = 0; nIndex < m_arrLsdException.size(); nIndex++ )
			{
				sResult += _T("<w:lsdException ");
				if (m_arrLsdException[nIndex])
					sResult += m_arrLsdException[nIndex]->ToString();
				sResult += _T("/>");
			}

			sResult += _T("</w:latentStyles>");

			return sResult;
		}

		virtual EElementType getType() const
		{
			return et_w_latentStyles;
		}

	private:

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:count"),             m_oCount )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:defLockedState"),    m_oDefLockedState )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:defQFormat"),        m_oDefQFormat )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:defSemiHidden"),     m_oDefSemiHidden )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:defUIPriority"),     m_oDefUiPriority )
			WritingElement_ReadAttributes_Read_else_if(oReader,  _T("w:defUnhideWhenUsed"), m_oDefUnhideWhenUsed )
			WritingElement_ReadAttributes_End( oReader )
		}

	public:

		// Attributes

		SimpleTypes::CDecimalNumber<0>                  m_oCount;
		SimpleTypes::COnOff<SimpleTypes::onoffFalse>    m_oDefLockedState;
		SimpleTypes::COnOff<SimpleTypes::onoffFalse>    m_oDefQFormat;
		SimpleTypes::COnOff<SimpleTypes::onoffFalse>    m_oDefSemiHidden;
		SimpleTypes::CDecimalNumber<99>                 m_oDefUiPriority;
		SimpleTypes::COnOff<SimpleTypes::onoffFalse>    m_oDefUnhideWhenUsed;

		// Childs

		std::vector<ComplexTypes::Word::CLsdException*> m_arrLsdException;
	};

	//--------------------------------------------------------------------------------
	// Style 17.7.4.17 (Part 1)
	//--------------------------------------------------------------------------------
	class CStyle : public WritingElement
	{
	public:

		CStyle()
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
	public:

		virtual void         fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:style") != oNode.GetName() )
				return;

			oNode.ReadAttributeBase( _T("w:customStyle"), m_oCustomStyle );
			oNode.ReadAttributeBase( _T("w:default"),     m_oDefault );
			oNode.ReadAttributeBase( _T("w:styleId"),     m_sStyleId );
			oNode.ReadAttributeBase( _T("w:type"),        m_oType );

			XmlUtils::CXmlNode oChild;

			WritingElement_ReadNode( oNode, oChild, _T("w:aliases"),         m_oAliases );
			WritingElement_ReadNode( oNode, oChild, _T("w:autoRedefine"),    m_oAutoRedefine );
			WritingElement_ReadNode( oNode, oChild, _T("w:basedOn"),         m_oBasedOn );
			WritingElement_ReadNode( oNode, oChild, _T("w:hidden"),          m_oHidden );
			WritingElement_ReadNode( oNode, oChild, _T("w:link"),            m_oLink );
			WritingElement_ReadNode( oNode, oChild, _T("w:locked"),          m_oLocked );
			WritingElement_ReadNode( oNode, oChild, _T("w:name"),            m_oName );
			WritingElement_ReadNode( oNode, oChild, _T("w:next"),            m_oNext );
			WritingElement_ReadNode( oNode, oChild, _T("w:personal"),        m_oPersonal );
			WritingElement_ReadNode( oNode, oChild, _T("w:personalCompose"), m_oPersonalCompose );
			WritingElement_ReadNode( oNode, oChild, _T("w:personalReply"),   m_oPersonalReply );
			WritingElement_ReadNode( oNode, oChild, _T("w:pPr"),             m_oParPr );
			WritingElement_ReadNode( oNode, oChild, _T("w:qFormat"),         m_oQFormat );
			WritingElement_ReadNode( oNode, oChild, _T("w:rPr"),             m_oRunPr );
			WritingElement_ReadNode( oNode, oChild, _T("w:rsid"),            m_oRsid );
			WritingElement_ReadNode( oNode, oChild, _T("w:semiHidden"),      m_oSemiHidden );
			WritingElement_ReadNode( oNode, oChild, _T("w:tblPr"),           m_oTblPr );

			XmlUtils::CXmlNodes oList;
			if ( oNode.GetNodes( _T("w:tblStylePr"), oList ) )
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

			WritingElement_ReadNode( oNode, oChild, _T("w:tcPr"),            m_oTcPr );
			WritingElement_ReadNode( oNode, oChild, _T("w:trPr"),            m_oTrPr );
			WritingElement_ReadNode( oNode, oChild, _T("w:uiPriority"),      m_oUiPriority );
			WritingElement_ReadNode( oNode, oChild, _T("w:unhideWhenUsed"),  m_oUnhideWhenUsed );
		}
		virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();

				if      ( _T("w:aliases")        == sName ) m_oAliases = oReader;
				else if ( _T("w:autoRedefine")   == sName ) m_oAutoRedefine = oReader;
				else if ( _T("w:basedOn")        == sName ) m_oBasedOn = oReader;
				else if ( _T("w:hidden")         == sName ) m_oHidden = oReader;
				else if ( _T("w:link")           == sName ) m_oLink = oReader;
				else if ( _T("w:locked")         == sName ) m_oLocked = oReader;
				else if ( _T("w:name")           == sName ) m_oName = oReader;
				else if ( _T("w:next")           == sName ) m_oNext = oReader;
				else if ( _T("w:personal")       == sName ) m_oPersonal = oReader;
				else if ( _T("w:personalCompose")== sName ) m_oPersonalCompose = oReader;
				else if ( _T("w:personalReply")  == sName ) m_oPersonalReply = oReader;
				else if ( _T("w:pPr")            == sName ) m_oParPr = oReader;
				else if ( _T("w:qFormat")        == sName ) m_oQFormat = oReader;
				else if ( _T("w:rPr")            == sName ) m_oRunPr = oReader;
				else if ( _T("w:rsid")           == sName ) m_oRsid = oReader;
				else if ( _T("w:semiHidden")     == sName ) m_oSemiHidden = oReader;
				else if ( _T("w:tblPr")          == sName ) m_oTblPr = oReader;
				else if ( _T("w:tblStylePr")     == sName )
				{
					OOX::Logic::CTableStyleProperties *oTblStylePr = new OOX::Logic::CTableStyleProperties(oReader);
					m_arrTblStylePr.push_back( oTblStylePr );
				}
				else if ( _T("w:tcPr")           == sName ) m_oTcPr = oReader;
				else if ( _T("w:trPr")           == sName ) m_oTrPr = oReader;
				else if ( _T("w:uiPriority")     == sName ) m_oUiPriority = oReader;
				else if ( _T("w:unhideWhenUsed") == sName ) m_oUnhideWhenUsed = oReader;
			}
		}
        virtual CString      toXML() const
		{
			CString sResult = _T("<w:style ");

			ComplexTypes_WriteAttribute( _T("w:customStyle=\""), m_oCustomStyle );
			ComplexTypes_WriteAttribute( _T("w:default=\""),     m_oDefault );
			if ( m_sStyleId.IsInit() )
			{
				sResult += _T("w:styleId=\"");
				sResult += m_sStyleId->GetString();
				sResult += _T("\" ");
			}
			ComplexTypes_WriteAttribute( _T("w:type=\""),        m_oType );

			sResult += _T(">");

			WritingElement_WriteNode_1( _T("<w:aliases "),         m_oAliases );
			WritingElement_WriteNode_1( _T("<w:autoRedefine "),    m_oAutoRedefine );
			WritingElement_WriteNode_1( _T("<w:basedOn "),         m_oBasedOn );
			WritingElement_WriteNode_1( _T("<w:hidden "),          m_oHidden );
			WritingElement_WriteNode_1( _T("<w:link "),            m_oLink );
			WritingElement_WriteNode_1( _T("<w:locked "),          m_oLocked );
			WritingElement_WriteNode_1( _T("<w:name "),            m_oName );
			WritingElement_WriteNode_1( _T("<w:next "),            m_oNext );
			WritingElement_WriteNode_1( _T("<w:personal "),        m_oPersonal );
			WritingElement_WriteNode_1( _T("<w:personalCompose "), m_oPersonalCompose );
			WritingElement_WriteNode_1( _T("<w:personalReply "),   m_oPersonalReply );
			WritingElement_WriteNode_2( m_oParPr );
			WritingElement_WriteNode_1( _T("<w:qFormat "),         m_oQFormat );
			WritingElement_WriteNode_2( m_oRunPr );
			WritingElement_WriteNode_1( _T("<w:rsid "),            m_oRsid );
			WritingElement_WriteNode_1( _T("<w:semiHidden "),      m_oSemiHidden );
			WritingElement_WriteNode_2( m_oTblPr );

			for ( unsigned int nIndex = 0; nIndex < m_arrTblStylePr.size(); nIndex++ )
			{
				if (m_arrTblStylePr[nIndex])
					sResult += m_arrTblStylePr[nIndex]->toXML();
			}

			WritingElement_WriteNode_2( m_oTcPr );
			WritingElement_WriteNode_2( m_oTrPr );
			WritingElement_WriteNode_1( _T("<w:uiPriority "),      m_oUiPriority );
			WritingElement_WriteNode_1( _T("<w:unhideWhenUsed "),  m_oUnhideWhenUsed );

			sResult += _T("</w:style>");

			return sResult;
		}

		virtual EElementType getType() const
		{
			return et_w_style;
		}
	private:

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:customStyle"), m_oCustomStyle )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:default"),     m_oDefault )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:styleId"),     m_sStyleId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:type"),        m_oType )
			WritingElement_ReadAttributes_End( oReader )
		}

	public:

		// Attributes

		nullable<SimpleTypes::COnOff<>     > m_oCustomStyle;
		nullable<SimpleTypes::COnOff<>     > m_oDefault;
		nullable<CString                   > m_sStyleId;
		nullable<SimpleTypes::CStyleType<> > m_oType;

		// Childs
		nullable<ComplexTypes::Word::CString_                       > m_oAliases;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oAutoRedefine;
		nullable<ComplexTypes::Word::CString_                       > m_oBasedOn;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oHidden;
		nullable<ComplexTypes::Word::CString_                       > m_oLink;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oLocked;
		nullable<ComplexTypes::Word::CString_                       > m_oName;
		nullable<ComplexTypes::Word::CString_                       > m_oNext;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oPersonal;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oPersonalCompose;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oPersonalReply;
		nullable<OOX::Logic::CParagraphProperty                     > m_oParPr;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oQFormat;
		nullable<OOX::Logic::CRunProperty                           > m_oRunPr;
		nullable<ComplexTypes::Word::CLongHexNumber                 > m_oRsid;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oSemiHidden;
		nullable<OOX::Logic::CTableProperty                         > m_oTblPr;		
		std::vector<OOX::Logic::CTableStyleProperties              *> m_arrTblStylePr;
		nullable<OOX::Logic::CTableCellProperties                   > m_oTcPr;
		nullable<OOX::Logic::CTableRowProperties                    > m_oTrPr;
		nullable<ComplexTypes::Word::CDecimalNumber                 > m_oUiPriority;
		nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oUnhideWhenUsed;

	};

	//--------------------------------------------------------------------------------
	// Styles 17.7.4.18 (Part 1)
	//--------------------------------------------------------------------------------
	class CStyles : public OOX::File
	{
	public:
		
		CStyles()
		{
		}
		CStyles(const CPath& oPath)
		{
			read( oPath );
		}
		virtual ~CStyles()
		{
			for (unsigned int nIndex = 0; nIndex < m_arrStyle.size(); nIndex++ )
			{
				if ( m_arrStyle[nIndex] )delete m_arrStyle[nIndex];
				m_arrStyle[nIndex] = NULL;
			}
			m_arrStyle.clear();
		}

	public:
		virtual void read(const CPath& oFilePath)
		{
#ifdef USE_LITE_READER

			XmlUtils::CXmlLiteReader oReader;
			
			if ( !oReader.FromFile( oFilePath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			CWCharWrapper sName = oReader.GetName();
			if ( _T("w:styles") == sName && !oReader.IsEmptyNode() )
			{
				int nStylesDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
				{
					sName = oReader.GetName();

					if ( _T("w:style") == sName )
					{
						OOX::CStyle *oStyle = new OOX::CStyle (oReader);
						if (oStyle) m_arrStyle.push_back( oStyle );
					}
					else if ( _T("w:docDefaults") == sName )
						m_oDocDefaults = oReader;
					else if ( _T("w:latentStyles") == sName )
						m_oLatentStyles = oReader;
				}
			}
#else
			XmlUtils::CXmlNode oStyles;
			oStyles.FromXmlFile( oFilePath.GetPath(), true );

			if ( _T("w:styles") == oStyles.GetName() )
			{
				XmlUtils::CXmlNode oChild;

				if ( oStyles.GetNode( _T("w:docDefaults"), oChild ) )
					m_oDocDefaults = oChild;

				if ( oStyles.GetNode( _T("w:latentStyles"), oChild ) )
					m_oLatentStyles = oChild;

				XmlUtils::CXmlNodes oStyleList;
				oStyles.GetNodes( _T("w:style"), oStyleList );

				for ( int nStlyeIndex = 0; nStlyeIndex < oStyleList.GetCount(); nStlyeIndex++ )
				{
					XmlUtils::CXmlNode oStyleNode;
					if ( oStyleList.GetAt( nStlyeIndex, oStyleNode ) )
					{
                        //OOX::CStyle oStyle = oStyleNode;
                        m_arrStyle.push_back( /*oStyle*/ new OOX::CStyle (oStyleNode) );
					}
				}
			}
#endif
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			CString sXml;
			sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" mc:Ignorable=\"w14\">");

			if ( m_oDocDefaults.IsInit() )
				sXml += m_oDocDefaults->toXML();

			if ( m_oLatentStyles.IsInit() )
				sXml += m_oLatentStyles->toXML();

			for (unsigned int nIndex = 0; nIndex < m_arrStyle.size(); nIndex++ )
			{
				if (m_arrStyle[nIndex])
					sXml += m_arrStyle[nIndex]->toXML();
			}
			sXml += _T("</w:styles>");

			CDirectory::SaveToFile( oFilePath.GetPath(), sXml );
		}

	public:
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

	public:

		nullable<OOX::CDocDefaults  > m_oDocDefaults;
		nullable<OOX::CLatentStyles > m_oLatentStyles;
		std::vector<OOX::CStyle    *> m_arrStyle;

	};
} // namespace OOX

#endif // OOX_STYLES_INCLUDE_H_
