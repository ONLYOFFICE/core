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
#ifndef OOX_LOGIC_FLD_CHAR_INCLUDE_H_
#define OOX_LOGIC_FLD_CHAR_INCLUDE_H_

#include "../WritingElement.h"
#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/ComplexTypes.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// MacroName 17.16.16 (Part 1)
		//--------------------------------------------------------------------------------
		class CMacroName : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CMacroName)
			CMacroName()
			{
			}
			virtual ~CMacroName()
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

				if ( m_oVal.IsInit() )
				{
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

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

			nullable<SimpleTypes::CMacroName > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// FFHelpText 17.16.16 (Part 1)
		//--------------------------------------------------------------------------------
		class CFFHelpText : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFFHelpText)
			CFFHelpText()
			{
			}
			virtual ~CFFHelpText()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:type"), m_oType );
				XmlMacroReadAttributeBase( oNode, _T("w:val"),  m_oVal );
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

				if ( m_oVal.IsInit() )
				{
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				if ( m_oType.IsInit() )
				{
					sResult += _T("w:type=\"");
					sResult += m_oType->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:type"), m_oType )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),  m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CInfoTextType<> > m_oType;
			nullable<SimpleTypes::CFFHelpTextVal  > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// FFName 17.16.27 (Part 1)
		//--------------------------------------------------------------------------------
		class CFFName : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFFName)
			CFFName()
			{
			}
			virtual ~CFFName()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"),  m_oVal );
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

				if ( m_oVal.IsInit() )
				{
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

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

			nullable<SimpleTypes::CFFName > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// FFStatusText 17.16.31 (Part 1)
		//--------------------------------------------------------------------------------
		class CFFStatusText : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFFStatusText)
			CFFStatusText()
			{
			}
			virtual ~CFFStatusText()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:type"), m_oType );
				XmlMacroReadAttributeBase( oNode, _T("w:val"),  m_oVal );
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

				if ( m_oVal.IsInit() )
				{
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

				if ( m_oType.IsInit() )
				{
					sResult += _T("w:type=\"");
					sResult += m_oType->ToString();
					sResult += _T("\" ");
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:type"), m_oType )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:val"),  m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CInfoTextType<>  > m_oType;
			nullable<SimpleTypes::CFFStatusTextVal > m_oVal;
		};

		//--------------------------------------------------------------------------------
		// FFTextType 17.16.34 (Part 1)
		//--------------------------------------------------------------------------------
		class CFFTextType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFFTextType)
			CFFTextType()
			{
			}
			virtual ~CFFTextType()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:val"),  m_oVal );
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

				if ( m_oVal.IsInit() )
				{
					sResult += _T("w:val=\"");
					sResult += m_oVal->ToString();
					sResult += _T("\" ");
				}

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

			nullable<SimpleTypes::CFFTextType<> > m_oVal;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CFFCheckBox 17.16.7 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFFCheckBox : public WritingElement
		{
		public:
			CFFCheckBox()
			{
			}
			CFFCheckBox(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CFFCheckBox(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CFFCheckBox()
			{
			}

		public:
			const CFFCheckBox &operator=(const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode&) oNode );
				return *this;
			}
			const CFFCheckBox &operator=(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;
				WritingElement_ReadNode( oNode, oChild, _T("w:checked"),  m_oChecked );
				WritingElement_ReadNode( oNode, oChild, _T("w:default"),  m_oDefault );
				WritingElement_ReadNode( oNode, oChild, _T("w:size"),     m_oSize );
				WritingElement_ReadNode( oNode, oChild, _T("w:sizeAuto"), m_oSizeAuto );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
                    std::wstring sName = oReader.GetName();
					if ( _T("w:checked") == sName )
						m_oChecked = oReader;
					else if ( _T("w:default") == sName )
						m_oDefault = oReader;
					else if ( _T("w:size") == sName )
						m_oSize = oReader;
					else if ( _T("w:sizeAuto") == sName )
						m_oSizeAuto = oReader;
				}
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<w:checkBox>");

				WritingElement_WriteNode_1( _T("<w:checked "),  m_oChecked );
				WritingElement_WriteNode_1( _T("<w:default "),  m_oDefault );
				WritingElement_WriteNode_1( _T("<w:size "),     m_oSize );
				WritingElement_WriteNode_1( _T("<w:sizeAuto "), m_oSizeAuto );

				sResult += _T("</w:checkBox>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_checkBox;
			}

		public:

			// Childs
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oChecked;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oDefault;
			nullable<ComplexTypes::Word::CHpsMeasure                     > m_oSize;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oSizeAuto;

		};

		//--------------------------------------------------------------------------------
		// CFFDDList 17.16.9 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFFDDList : public WritingElement
		{
		public:
			CFFDDList()
			{
			}
			CFFDDList(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CFFDDList(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CFFDDList()
			{
				ClearItems();
			}
			virtual void ClearItems()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrListEntry.size(); nIndex++ )
				{
					if ( m_arrListEntry[nIndex] ) delete m_arrListEntry[nIndex];
					m_arrListEntry[nIndex] = NULL;
				}
				m_arrListEntry.clear();
			}

		public:
			const CFFDDList &operator=(const XmlUtils::CXmlNode &oNode)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlNode&) oNode );
				return *this;
			}
			const CFFDDList &operator=(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;
				WritingElement_ReadNode( oNode, oChild, _T("w:default"), m_oDefault );
				WritingElement_ReadNode( oNode, oChild, _T("w:result"),  m_oResult );

				XmlUtils::CXmlNodes oListEntryNodes;
				if ( oNode.GetNodes( _T("w:listEntry"), oListEntryNodes ) )
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
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
                    std::wstring sName = oReader.GetName();
					if ( _T("w:default") == sName )
						m_oDefault = oReader;
					else if ( _T("w:result") == sName )
						m_oResult = oReader;
					else if ( _T("w:listEntry") == sName )
					{
                        ComplexTypes::Word::String *oListEntry = new ComplexTypes::Word::String(oReader);
						if (oListEntry) m_arrListEntry.push_back( oListEntry );
					}
				}
			}				
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<w:ddList>");

				WritingElement_WriteNode_1( _T("<w:default "), m_oDefault );
				WritingElement_WriteNode_1( _T("<w:result "),  m_oResult );

				for (unsigned int nIndex = 0; nIndex < m_arrListEntry.size(); nIndex++ )
				{
					sResult += _T("<w:listEntry ");
					if (m_arrListEntry[nIndex])
						sResult += m_arrListEntry[nIndex]->ToString();
					sResult += _T("/>");
				}

				sResult += _T("</w:ddList>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_ddList;
			}

		public:

			// Childs
			nullable<ComplexTypes::Word::CDecimalNumber >	m_oDefault;
			nullable<ComplexTypes::Word::CDecimalNumber >	m_oResult;
            std::vector<ComplexTypes::Word::String  *>	m_arrListEntry;
		};

		//--------------------------------------------------------------------------------
		// CFFTextInput 17.16.33 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFFTextInput : public WritingElement
		{
		public:
			CFFTextInput()
			{
			}
			CFFTextInput(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CFFTextInput(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CFFTextInput()
			{
			}

		public:
			const CFFTextInput &operator=(const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode&) oNode );
				return *this;
			}
			const CFFTextInput &operator=(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;
				WritingElement_ReadNode( oNode, oChild, _T("w:default"),   m_oDefault );
				WritingElement_ReadNode( oNode, oChild, _T("w:format"),    m_oFormat );
				WritingElement_ReadNode( oNode, oChild, _T("w:maxLength"), m_oMaxLength );
				WritingElement_ReadNode( oNode, oChild, _T("w:type"),      m_oType );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
                    std::wstring sName = oReader.GetName();
					if ( _T("w:default") == sName )
						m_oDefault = oReader;
					else if ( _T("w:format") == sName )
						m_oFormat = oReader;
					else if ( _T("w:maxLength") == sName )
						m_oMaxLength = oReader;
					else if ( _T("w:type") == sName )
						m_oType = oReader;
				}
			}				
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<w:textInput>");

				WritingElement_WriteNode_1( _T("<w:default "),   m_oDefault );
				WritingElement_WriteNode_1( _T("<w:format "),    m_oFormat );
				WritingElement_WriteNode_1( _T("<w:maxLength "), m_oMaxLength );
				WritingElement_WriteNode_1( _T("<w:type "),      m_oType );

				sResult += _T("</w:textInput>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_textInput;
			}

		public:

			// Childs
            nullable<ComplexTypes::Word::String       > m_oDefault;
            nullable<ComplexTypes::Word::String       > m_oFormat;
			nullable<ComplexTypes::Word::CDecimalNumber > m_oMaxLength;
			nullable<ComplexTypes::Word::CFFTextType    > m_oType;

		};

		//--------------------------------------------------------------------------------
		// CFFData 17.16.17 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFFData : public WritingElement
		{
		public:
			CFFData()
			{
			}
			CFFData(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CFFData(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CFFData()
			{
			}

		public:
			const CFFData &operator=(const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode&) oNode );
				return *this;
			}
			const CFFData &operator=(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;
				WritingElement_ReadNode( oNode, oChild, _T("w:calcOnExit"), m_oCalcOnExit );
				WritingElement_ReadNode( oNode, oChild, _T("w:checkBox"),   m_oCheckBox );
				WritingElement_ReadNode( oNode, oChild, _T("w:ddList"),     m_oDDList );
				WritingElement_ReadNode( oNode, oChild, _T("w:enabled"),    m_oEnabled );
				WritingElement_ReadNode( oNode, oChild, _T("w:entryMacro"), m_oEntryMacro );
				WritingElement_ReadNode( oNode, oChild, _T("w:exitMacro"),  m_oExitMacro );
				WritingElement_ReadNode( oNode, oChild, _T("w:helpText"),   m_oHelpText );
				WritingElement_ReadNode( oNode, oChild, _T("w:label"),      m_oLabel );
				WritingElement_ReadNode( oNode, oChild, _T("w:name"),       m_oName );
				WritingElement_ReadNode( oNode, oChild, _T("w:statusText"), m_oStatusText );
				WritingElement_ReadNode( oNode, oChild, _T("w:tabIndex"),   m_oTabIndex );
				WritingElement_ReadNode( oNode, oChild, _T("w:textInput"),  m_oTextInput );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
                    std::wstring sName = oReader.GetName();

					if      ( _T("w:calcOnExit") == sName ) m_oCalcOnExit = oReader;
					else if ( _T("w:checkBox")   == sName ) m_oCheckBox = oReader;
					else if ( _T("w:ddList")     == sName ) m_oDDList = oReader;
					else if ( _T("w:enabled")    == sName ) m_oEnabled = oReader;
					else if ( _T("w:entryMacro") == sName ) m_oEntryMacro = oReader;
					else if ( _T("w:exitMacro")  == sName ) m_oExitMacro = oReader;
					else if ( _T("w:helpText")   == sName ) m_oHelpText = oReader;
					else if ( _T("w:label")      == sName ) m_oLabel = oReader;
					else if ( _T("w:name")       == sName ) m_oName = oReader;
					else if ( _T("w:statusText") == sName ) m_oStatusText = oReader;
					else if ( _T("w:tabIndex")   == sName ) m_oTabIndex = oReader;
					else if ( _T("w:textInput")  == sName ) m_oTextInput = oReader;
				}
			}				
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<w:ffData>");

				WritingElement_WriteNode_1( _T("<w:calcOnExit "), m_oCalcOnExit );
				WritingElement_WriteNode_2( m_oCheckBox );
				WritingElement_WriteNode_2( m_oDDList );
				WritingElement_WriteNode_1( _T("<w:enabled "),    m_oEnabled );
				WritingElement_WriteNode_1( _T("<w:entryMacro "), m_oEntryMacro );
				WritingElement_WriteNode_1( _T("<w:exitMacro "),  m_oExitMacro );
				WritingElement_WriteNode_1( _T("<w:helpText "),   m_oHelpText );
				WritingElement_WriteNode_1( _T("<w:label "),      m_oLabel );
				WritingElement_WriteNode_1( _T("<w:name "),       m_oName );
				WritingElement_WriteNode_1( _T("<w:statusText "), m_oStatusText );
				WritingElement_WriteNode_1( _T("<w:tabIndex "),   m_oTabIndex );
				WritingElement_WriteNode_2( m_oTextInput );

				sResult += _T("</w:ffData>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_ffData;
			}

		public:

			// Childs
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oCalcOnExit;
			nullable<OOX::Logic::CFFCheckBox                             > m_oCheckBox;
			nullable<OOX::Logic::CFFDDList                               > m_oDDList;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oEnabled;
			nullable<ComplexTypes::Word::CMacroName                      > m_oEntryMacro;
			nullable<ComplexTypes::Word::CMacroName                      > m_oExitMacro;
			nullable<ComplexTypes::Word::CFFHelpText                     > m_oHelpText;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oLabel;
			nullable<ComplexTypes::Word::CFFName                         > m_oName;
			nullable<ComplexTypes::Word::CFFStatusText                   > m_oStatusText;
			nullable<ComplexTypes::Word::CUnsignedDecimalNumber          > m_oTabIndex;
			nullable<OOX::Logic::CFFTextInput                            > m_oTextInput;

		};

		//--------------------------------------------------------------------------------
		// CFldChar 17.16.18 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFldChar : public WritingElement
		{
		public:
			CFldChar()
			{
			}
			CFldChar(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CFldChar(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CFldChar()
			{
			}

		public:
			const CFldChar &operator=(const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode&) oNode );
				return *this;
			}
			const CFldChar &operator=(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:dirty"),       m_oDirty );
				XmlMacroReadAttributeBase( oNode, _T("w:fldCharType"), m_oFldCharType );
				XmlMacroReadAttributeBase( oNode, _T("w:fldLock"),     m_oFldLock );

				XmlUtils::CXmlNode oChild;
				WritingElement_ReadNode( oNode, oChild, _T("w:ffData"), m_oFFData );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth )  )
				{
                    std::wstring sName = oReader.GetName();
					WritingElement *pItem = NULL;

					if ( _T("w:ffData") == sName )
						m_oFFData = oReader;
				}
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = _T("<w:fldChar ");

				ComplexTypes_WriteAttribute( _T("w:dirty=\""),       m_oDirty );
				ComplexTypes_WriteAttribute( _T("w:fldCharType=\""), m_oFldCharType );
				ComplexTypes_WriteAttribute( _T("w:fldLock=\""),     m_oFldLock );

				if ( m_oFFData.IsInit() )
				{
					sResult += _T(">");
					sResult += m_oFFData->toXML();
					sResult += _T("</w:fldChar>");
				}
				else
				{
					sResult += _T("/>");
				}

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_fldChar;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:dirty"),       m_oDirty )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:fldCharType"), m_oFldCharType )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:fldLock"),     m_oFldLock )

				WritingElement_ReadAttributes_End( oReader )
			}


		public:

			// Attributes
			nullable<SimpleTypes::COnOff<>       > m_oDirty;
			nullable<SimpleTypes::CFldCharType<> > m_oFldCharType;
			nullable<SimpleTypes::COnOff<>       > m_oFldLock;

			// Childs
			nullable<OOX::Logic::CFFData         > m_oFFData;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_FLD_CHAR_INCLUDE_H_
