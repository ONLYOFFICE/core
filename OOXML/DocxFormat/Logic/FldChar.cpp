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

#include "FldChar.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// MacroName 17.16.16 (Part 1)
		//--------------------------------------------------------------------------------

		CMacroName::CMacroName()
		{
		}
		CMacroName::~CMacroName()
		{
		}
		void CMacroName::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
		}
		void CMacroName::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CMacroName::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += L"w:val=\"";
				sResult += m_oVal->ToString();
				sResult += L"\" ";
			}

			return sResult;
		}
		void CMacroName::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// FFHelpText 17.16.16 (Part 1)
		//--------------------------------------------------------------------------------

		CFFHelpText::CFFHelpText()
		{
		}
		CFFHelpText::~CFFHelpText()
		{
		}
		void CFFHelpText::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:type", m_oType );
			XmlMacroReadAttributeBase( oNode, L"w:val",  m_oVal );
		}
		void CFFHelpText::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CFFHelpText::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += L"w:val=\"";
				sResult += m_oVal->ToXmlString();
				sResult += L"\" ";
			}

			if ( m_oType.IsInit() )
			{
				sResult += L"w:type=\"";
				sResult += m_oType->ToString();
				sResult += L"\" ";
			}

			return sResult;
		}
		void CFFHelpText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:type", m_oType )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",  m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// FFName 17.16.27 (Part 1)
		//--------------------------------------------------------------------------------

		CFFName::CFFName()
		{
		}
		CFFName::~CFFName()
		{
		}

		void CFFName::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:val",  m_oVal );
		}
		void CFFName::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CFFName::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += L"w:val=\"";
				sResult += m_oVal->ToXmlString();
				sResult += L"\" ";
			}

			return sResult;
		}
		void CFFName::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// FFStatusText 17.16.31 (Part 1)
		//--------------------------------------------------------------------------------

		CFFStatusText::CFFStatusText()
		{
		}
		CFFStatusText::~CFFStatusText()
		{
		}
		void CFFStatusText::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:type", m_oType );
			XmlMacroReadAttributeBase( oNode, L"w:val",  m_oVal );
		}
		void CFFStatusText::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CFFStatusText::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += L"w:val=\"";
				sResult += m_oVal->ToXmlString();
				sResult += L"\" ";
			}

			if ( m_oType.IsInit() )
			{
				sResult += L"w:type=\"";
				sResult += m_oType->ToString();
				sResult += L"\" ";
			}

			return sResult;
		}
		void CFFStatusText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:type", m_oType )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",  m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// FFTextType 17.16.34 (Part 1)
		//--------------------------------------------------------------------------------

		CFFTextType::CFFTextType()
		{
		}
		CFFTextType::~CFFTextType()
		{
		}

		void CFFTextType::FromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:val",  m_oVal );
		}
		void CFFTextType::FromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CFFTextType::ToString() const
		{
			std::wstring sResult;

			if ( m_oVal.IsInit() )
			{
				sResult += L"w:val=\"";
				sResult += m_oVal->ToString();
				sResult += L"\" ";
			}

			return sResult;
		}
		void CFFTextType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CFFCheckBox 17.16.7 (Part 1)
		//--------------------------------------------------------------------------------

		CFFCheckBox::CFFCheckBox()
		{
		}
		CFFCheckBox::CFFCheckBox(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CFFCheckBox::CFFCheckBox(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CFFCheckBox::~CFFCheckBox()
		{
		}
		const CFFCheckBox& CFFCheckBox::operator=(const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode&) oNode );
			return *this;
		}
		const CFFCheckBox& CFFCheckBox::operator=(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CFFCheckBox::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;
			WritingElement_ReadNode( oNode, oChild, L"w:checked",  m_oChecked );
			WritingElement_ReadNode( oNode, oChild, L"w:default",  m_oDefault );
			WritingElement_ReadNode( oNode, oChild, L"w:size",     m_oSize );
			WritingElement_ReadNode( oNode, oChild, L"w:sizeAuto", m_oSizeAuto );
		}
		void CFFCheckBox::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:checked" == sName )
					m_oChecked = oReader;
				else if ( L"w:default" == sName )
					m_oDefault = oReader;
				else if ( L"w:size" == sName )
					m_oSize = oReader;
				else if ( L"w:sizeAuto" == sName )
					m_oSizeAuto = oReader;
			}
		}
		std::wstring CFFCheckBox::toXML() const
		{
			std::wstring sResult = L"<w:checkBox>";

			WritingElement_WriteNode_1( L"<w:checked ",  m_oChecked );
			WritingElement_WriteNode_1( L"<w:default ",  m_oDefault );
			WritingElement_WriteNode_1( L"<w:size ",     m_oSize );
			WritingElement_WriteNode_1( L"<w:sizeAuto ", m_oSizeAuto );

			sResult += L"</w:checkBox>";

			return sResult;
		}
		EElementType CFFCheckBox::getType() const
		{
			return et_w_checkBox;
		}

		//--------------------------------------------------------------------------------
		// CFFDDList 17.16.9 (Part 1)
		//--------------------------------------------------------------------------------

		CFFDDList::CFFDDList()
		{
		}
		CFFDDList::CFFDDList(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CFFDDList::CFFDDList(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CFFDDList::~CFFDDList()
		{
			ClearItems();
		}
		void CFFDDList::ClearItems()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrListEntry.size(); nIndex++ )
			{
				if ( m_arrListEntry[nIndex] ) delete m_arrListEntry[nIndex];
				m_arrListEntry[nIndex] = NULL;
			}
			m_arrListEntry.clear();
		}
		const CFFDDList& CFFDDList::operator=(const XmlUtils::CXmlNode &oNode)
		{
			ClearItems();
			fromXML( (XmlUtils::CXmlNode&) oNode );
			return *this;
		}
		const CFFDDList& CFFDDList::operator=(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CFFDDList::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;
			WritingElement_ReadNode( oNode, oChild, L"w:default", m_oDefault );
			WritingElement_ReadNode( oNode, oChild, L"w:result",  m_oResult );

			XmlUtils::CXmlNodes oListEntryNodes;
			if ( oNode.GetNodes( L"w:listEntry", oListEntryNodes ) )
			{
				XmlUtils::CXmlNode oListEntryNode;
				for ( int nIndex = 0; nIndex < oListEntryNodes.GetCount(); nIndex++ )
				{
					if ( oListEntryNodes.GetAt( nIndex, oListEntryNode ) )
					{
						ComplexTypes::Word::String *oListEntry = new ComplexTypes::Word::String(oListEntryNode);
						if (oListEntry) m_arrListEntry.push_back( oListEntry );
					}
				}
			}
		}
		void CFFDDList::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:default" == sName )
					m_oDefault = oReader;
				else if ( L"w:result" == sName )
					m_oResult = oReader;
				else if ( L"w:listEntry" == sName )
				{
					ComplexTypes::Word::String *oListEntry = new ComplexTypes::Word::String(oReader);
					if (oListEntry) m_arrListEntry.push_back( oListEntry );
				}
			}
		}
		std::wstring CFFDDList::toXML() const
		{
			std::wstring sResult = L"<w:ddList>";

			WritingElement_WriteNode_1( L"<w:default ", m_oDefault );
			WritingElement_WriteNode_1( L"<w:result ",  m_oResult );

			for (unsigned int nIndex = 0; nIndex < m_arrListEntry.size(); nIndex++ )
			{
				sResult += L"<w:listEntry ";
				if (m_arrListEntry[nIndex])
					sResult += m_arrListEntry[nIndex]->ToString();
				sResult += L"/>";
			}

			sResult += L"</w:ddList>";

			return sResult;
		}
		EElementType CFFDDList::getType() const
		{
			return et_w_ddList;
		}

		//--------------------------------------------------------------------------------
		// CFFTextInput 17.16.33 (Part 1)
		//--------------------------------------------------------------------------------

		CFFTextInput::CFFTextInput()
		{
		}
		CFFTextInput::CFFTextInput(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CFFTextInput::CFFTextInput(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CFFTextInput::~CFFTextInput()
		{
		}
		const CFFTextInput& CFFTextInput::operator=(const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode&) oNode );
			return *this;
		}
		const CFFTextInput& CFFTextInput::operator=(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CFFTextInput::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;
			WritingElement_ReadNode( oNode, oChild, L"w:default",   m_oDefault );
			WritingElement_ReadNode( oNode, oChild, L"w:format",    m_oFormat );
			WritingElement_ReadNode( oNode, oChild, L"w:maxLength", m_oMaxLength );
			WritingElement_ReadNode( oNode, oChild, L"w:type",      m_oType );
		}
		void CFFTextInput::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:default" == sName )
					m_oDefault = oReader;
				else if ( L"w:format" == sName )
					m_oFormat = oReader;
				else if ( L"w:maxLength" == sName )
					m_oMaxLength = oReader;
				else if ( L"w:type" == sName )
					m_oType = oReader;
			}
		}
		std::wstring CFFTextInput::toXML() const
		{
			std::wstring sResult = L"<w:textInput>";

			WritingElement_WriteNode_1( L"<w:type ",      m_oType );
			WritingElement_WriteNode_1( L"<w:default ",   m_oDefault );
			WritingElement_WriteNode_1( L"<w:format ",    m_oFormat );
			WritingElement_WriteNode_1( L"<w:maxLength ", m_oMaxLength );

			sResult += L"</w:textInput>";

			return sResult;
		}
		EElementType CFFTextInput::getType() const
		{
			return et_w_textInput;
		}

		//--------------------------------------------------------------------------------
		// CFFData 17.16.17 (Part 1)
		//--------------------------------------------------------------------------------

		CFFData::CFFData()
		{
		}
		CFFData::CFFData(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CFFData::CFFData(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CFFData::~CFFData()
		{
		}
		const CFFData& CFFData::operator=(const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode&) oNode );
			return *this;
		}
		const CFFData& CFFData::operator=(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CFFData::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oChild;
			WritingElement_ReadNode( oNode, oChild, L"w:calcOnExit", m_oCalcOnExit );
			WritingElement_ReadNode( oNode, oChild, L"w:checkBox",   m_oCheckBox );
			WritingElement_ReadNode( oNode, oChild, L"w:ddList",     m_oDDList );
			WritingElement_ReadNode( oNode, oChild, L"w:enabled",    m_oEnabled );
			WritingElement_ReadNode( oNode, oChild, L"w:entryMacro", m_oEntryMacro );
			WritingElement_ReadNode( oNode, oChild, L"w:exitMacro",  m_oExitMacro );
			WritingElement_ReadNode( oNode, oChild, L"w:helpText",   m_oHelpText );
			WritingElement_ReadNode( oNode, oChild, L"w:label",      m_oLabel );
			WritingElement_ReadNode( oNode, oChild, L"w:name",       m_oName );
			WritingElement_ReadNode( oNode, oChild, L"w:statusText", m_oStatusText );
			WritingElement_ReadNode( oNode, oChild, L"w:tabIndex",   m_oTabIndex );
			WritingElement_ReadNode( oNode, oChild, L"w:textInput",  m_oTextInput );
		}
		void CFFData::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if      ( L"w:calcOnExit" == sName ) m_oCalcOnExit = oReader;
				else if ( L"w:checkBox"   == sName ) m_oCheckBox = oReader;
				else if ( L"w:ddList"     == sName ) m_oDDList = oReader;
				else if ( L"w:enabled"    == sName ) m_oEnabled = oReader;
				else if ( L"w:entryMacro" == sName ) m_oEntryMacro = oReader;
				else if ( L"w:exitMacro"  == sName ) m_oExitMacro = oReader;
				else if ( L"w:helpText"   == sName ) m_oHelpText = oReader;
				else if ( L"w:label"      == sName ) m_oLabel = oReader;
				else if ( L"w:name"       == sName ) m_oName = oReader;
				else if ( L"w:statusText" == sName ) m_oStatusText = oReader;
				else if ( L"w:tabIndex"   == sName ) m_oTabIndex = oReader;
				else if ( L"w:textInput"  == sName ) m_oTextInput = oReader;
			}
		}
		std::wstring CFFData::toXML() const
		{
			std::wstring sResult = L"<w:ffData>";

			WritingElement_WriteNode_1( L"<w:name ",       m_oName );
			WritingElement_WriteNode_1( L"<w:label ",      m_oLabel );
			WritingElement_WriteNode_1( L"<w:enabled ",    m_oEnabled );
			WritingElement_WriteNode_1( L"<w:calcOnExit ", m_oCalcOnExit );
			WritingElement_WriteNode_1( L"<w:helpText ",   m_oHelpText );
			WritingElement_WriteNode_1( L"<w:statusText ", m_oStatusText );
			WritingElement_WriteNode_1( L"<w:entryMacro ", m_oEntryMacro );
			WritingElement_WriteNode_1( L"<w:exitMacro ",  m_oExitMacro );
			WritingElement_WriteNode_1( L"<w:tabIndex ",   m_oTabIndex );
			WritingElement_WriteNode_2( m_oCheckBox );
			WritingElement_WriteNode_2( m_oDDList );
			WritingElement_WriteNode_2( m_oTextInput );

			sResult += L"</w:ffData>";

			return sResult;
		}
		EElementType CFFData::getType() const
		{
			return et_w_ffData;
		}

		//--------------------------------------------------------------------------------
		// CFldChar 17.16.18 (Part 1)
		//--------------------------------------------------------------------------------

		CFldChar::CFldChar(OOX::Document *pMain) : WritingElement(pMain) {}

		CFldChar::CFldChar(XmlUtils::CXmlNode &oNode)
		{
			fromXML( oNode );
		}
		CFldChar::CFldChar(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( oReader );
		}
		CFldChar::~CFldChar()
		{
		}
		const CFldChar& CFldChar::operator=(const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode&) oNode );
			return *this;
		}
		const CFldChar& CFldChar::operator=(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		void CFldChar::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"w:dirty",       m_oDirty );
			XmlMacroReadAttributeBase( oNode, L"w:fldCharType", m_oFldCharType );
			XmlMacroReadAttributeBase( oNode, L"w:fldLock",     m_oFldLock );

			XmlUtils::CXmlNode oChild;
			WritingElement_ReadNode( oNode, oChild, L"w:ffData", m_oFFData );
		}
		void CFldChar::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth )  )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( L"w:ffData" == sName )
					m_oFFData = oReader;
			}
		}
		std::wstring CFldChar::toXML() const
		{
			std::wstring sResult = L"<w:fldChar";

			ComplexTypes_WriteAttribute( L" w:dirty=\"",       m_oDirty );
			ComplexTypes_WriteAttribute( L" w:fldCharType=\"", m_oFldCharType );
			ComplexTypes_WriteAttribute( L" w:fldLock=\"",     m_oFldLock );

			if ( m_oFFData.IsInit() )
			{
				sResult += L">";
				sResult += m_oFFData->toXML();
				sResult += L"</w:fldChar>";
			}
			else
			{
				sResult += L"/>";
			}

			return sResult;
		}
		EElementType CFldChar::getType() const
		{
			return et_w_fldChar;
		}
		void CFldChar::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"w:dirty",       m_oDirty )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:fldCharType", m_oFldCharType )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:fldLock",     m_oFldLock )
					WritingElement_ReadAttributes_End( oReader )
		}

	} // namespace Logic
} // namespace OOX
