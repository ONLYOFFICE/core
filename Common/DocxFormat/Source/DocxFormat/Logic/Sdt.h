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
#ifndef OOX_LOGIC_SDT_INCLUDE_H_
#define OOX_LOGIC_SDT_INCLUDE_H_

#include "../WritingElement.h"
#include "RunProperty.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// SdtListItem 17.5.2.21 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtListItem : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSdtListItem)
			CSdtListItem()
			{
			}
			virtual ~CSdtListItem()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:displayText", m_sDisplayText );
				XmlMacroReadAttributeBase( oNode, L"w:value",       m_sValue );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_sDisplayText.IsInit() )
				{
					sResult += L"w:displayText=\"";
                    sResult += XmlUtils::EncodeXmlString(*m_sDisplayText);
					sResult += L"\" ";
				}

				if ( m_sValue.IsInit() )
				{
					sResult += L"w:value=\"";
                    sResult += XmlUtils::EncodeXmlString(*m_sValue);
					sResult += L"\" ";
				}

				return sResult;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:displayText", m_sDisplayText )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:value",       m_sValue )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable_string	m_sDisplayText;
			nullable_string	m_sValue;
		};

		//--------------------------------------------------------------------------------
		// DataBinding 17.5.2.6 (Part 1)
		//--------------------------------------------------------------------------------
		class CDataBinding : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CDataBinding)
			CDataBinding()
			{
			}
			virtual ~CDataBinding()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:prefixMappings", m_sPrefixMappings );
				XmlMacroReadAttributeBase( oNode, L"w:storeItemID",    m_sStoreItemID );
				XmlMacroReadAttributeBase( oNode, L"w:xpath",          m_sXPath );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_sPrefixMappings.IsInit() )
				{
					sResult += L"w:prefixMappings=\"";
                    sResult += m_sPrefixMappings.get2();
					sResult += L"\" ";
				}

				if ( m_sXPath.IsInit() )
				{
					sResult += L"w:xpath=\"";
                    sResult += m_sXPath.get2();
					sResult += L"\" ";
				}

				if ( m_sStoreItemID.IsInit() )
				{
					sResult += L"w:storeItemID=\"";
                    sResult += m_sStoreItemID.get2();
					sResult += L"\" ";
				}
				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:prefixMappings", m_sPrefixMappings )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:storeItemID",    m_sStoreItemID )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:xpath",          m_sXPath )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<std::wstring > m_sPrefixMappings;
			nullable<std::wstring > m_sStoreItemID;
			nullable<std::wstring > m_sXPath;
		};

		//--------------------------------------------------------------------------------
		// CalendarType 17.5.2.3 (Part 1)
		//--------------------------------------------------------------------------------
		class CCalendarType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CCalendarType)
			CCalendarType() 
			{
			}
			virtual ~CCalendarType()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult = L"w:val=\"" + m_oVal.ToString() + L"\"";
				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			SimpleTypes::CCalendarType<SimpleTypes::calendartypeGregorian> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// SdtDateMappingType 17.5.2.40 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtDateMappingType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSdtDateMappingType)
			CSdtDateMappingType()
			{
			}
			virtual ~CSdtDateMappingType()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult = L"w:val=\"" + m_oVal.ToString() + L"\"";
				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			SimpleTypes::CSdtDateMappingType<SimpleTypes::sdtdatemappingtypeText> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// Lock 17.5.2.23 (Part 1)
		//--------------------------------------------------------------------------------
		class CLock : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLock)
			CLock() 
			{
			}
			virtual ~CLock()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult = L"w:val=\"" + m_oVal.ToString() + L"\"";

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			SimpleTypes::CLock<SimpleTypes::lockUnlocked> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// SdtText 17.5.2.44 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtText : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSdtText)
			CSdtText()
			{
			}
			virtual ~CSdtText()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:multiLine", m_oMultiLine );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;
				if(m_oMultiLine.IsInit())
				{
					sResult = L"w:multiLine=\"" + m_oMultiLine->ToString() + L"\"";
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:multiLine", m_oMultiLine )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>> m_oMultiLine;
		};
		//--------------------------------------------------------------------------------
		// SdtAppearance 2.5.4.2 ([MS-DOCX])
		//--------------------------------------------------------------------------------
		class CSdtAppearance : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CSdtAppearance)
			CSdtAppearance()
			{
			}
			virtual ~CSdtAppearance()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w15:val", m_oVal );
			}
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;
				if(m_oVal.IsInit())
				{
					sResult = L"w15:val=\"" + m_oVal->ToString() + L"\"";
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w15:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CSdtAppearance<>> m_oVal;
		};

		//Not from specification
		class CComb : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CComb)
			CComb()
			{
			}
			virtual ~CComb()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHeightRule<>>	m_oWRule;
			nullable_int							m_oWidth;
			nullable_string							m_oSym;
			nullable_string							m_oFont;
		};
		//Not from specification
		class CTextFormFormat : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTextFormFormat)
			CTextFormFormat()
			{
			}
			virtual ~CTextFormFormat()
			{
			}

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring ToString() const;
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTextFormFormatType<>> m_oType;
			nullable_string m_oVal;
			nullable_string m_oSymbols;
		};
	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//Not from specification
		class CFormPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFormPr)
			CFormPr(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CFormPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oKey;
			nullable_string m_oLabel;
			nullable_string m_oHelpText;
			nullable_bool m_oRequired;

			nullable<ComplexTypes::Word::CBorder> m_oBorder;
			nullable<ComplexTypes::Word::CShading> m_oShd;
		};
		//Not from specification
		class CTextFormPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTextFormPr)
			CTextFormPr(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CTextFormPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<ComplexTypes::Word::CComb> m_oComb;
			nullable<ComplexTypes::Word::CDecimalNumber> m_oMaxCharacters;
			nullable<ComplexTypes::Word::CBorder> m_oCombBorder;
			nullable_bool m_oAutoFit;
			nullable_bool m_oMultiLine;
			nullable<ComplexTypes::Word::CTextFormFormat> m_oFormat;
		};
		//Not from specification
		class CComplexFormPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CComplexFormPr)
			CComplexFormPr(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CComplexFormPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CComplexFormType<>> m_oType;
		};

		//--------------------------------------------------------------------------------
		// CSdtComboBox 17.5.2.5 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtComboBox : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtComboBox)
			CSdtComboBox(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CSdtComboBox()
			{
				for ( size_t nIndex = 0; nIndex < m_arrListItem.size(); nIndex++ )
				{
					if ( m_arrListItem[nIndex] )
						delete m_arrListItem[nIndex];

					m_arrListItem[nIndex] = NULL;
				}

				m_arrListItem.clear();
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:lastValue", m_sLastValue );

				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( L"w:listItem", oChilds ) )
				{
					XmlUtils::CXmlNode oItemNode;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItemNode ) )
						{
							ComplexTypes::Word::CSdtListItem *oListItem = new ComplexTypes::Word::CSdtListItem(oItemNode);
							if (oListItem) m_arrListItem.push_back( oListItem );
						}
					}
				}
				XmlUtils::CXmlNode oChild;
				if (oNode.GetNode(L"w:format", oChild))
				{
					m_oFormat = oChild;
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
					if ( L"w:listItem" == sName )
					{
						ComplexTypes::Word::CSdtListItem *oListItem = new ComplexTypes::Word::CSdtListItem(oReader);
						m_arrListItem.push_back( oListItem );
					}
					else if (L"w:format" == sName)
						m_oFormat = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult;
				
				if ( m_sLastValue.IsInit() )
				{
					sResult = L"<w:comboBox w:lastValue=\"";
                    sResult += m_sLastValue.get2();
					sResult += L"\">";
				}
				else
					sResult = L"<w:comboBox>";

				WritingElement_WriteNode_1(L"<w:format ", m_oFormat);
				for (size_t nIndex = 0; nIndex < m_arrListItem.size(); nIndex++ )
				{
					sResult += L"<w:listItem ";
					if (m_arrListItem[nIndex])
						sResult += m_arrListItem[nIndex]->ToString();
					sResult += L"/>";
				}
				sResult += L"</w:comboBox>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_comboBox;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:lastValue", m_sLastValue )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<ComplexTypes::Word::CTextFormFormat> m_oFormat;
			nullable<std::wstring > m_sLastValue;

			std::vector<ComplexTypes::Word::CSdtListItem*> m_arrListItem;
		};

		//--------------------------------------------------------------------------------
		// CDate 17.5.2.7 (Part 1)
		//--------------------------------------------------------------------------------
		class CDate : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDate)
			CDate(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CDate()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:fullDate", m_oFullDate );

				XmlUtils::CXmlNode oChild;

				WritingElement_ReadNode( oNode, oChild, L"w:calendar",          m_oCalendar );
				WritingElement_ReadNode( oNode, oChild, L"w:dateFormat",        m_oDateFormat );
				WritingElement_ReadNode( oNode, oChild, L"w:lid",               m_oLid );
				WritingElement_ReadNode( oNode, oChild, L"w:storeMappedDateAs", m_oStoreMappedDateAs );
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
					if ( L"w:calendar" == sName )
						m_oCalendar = oReader;
					else if ( L"w:dateFormat" == sName )
						m_oDateFormat = oReader;
					else if ( L"w:lid" == sName )
						m_oLid = oReader;
					else if ( L"w:storeMappedDateAs" == sName )
						m_oStoreMappedDateAs = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult;
				
				if ( m_oFullDate.IsInit() )
				{
					sResult = L"<w:date w:fullDate=\"";
					sResult += m_oFullDate->ToString();
					sResult += L"\">";
				}
				else
					sResult = L"<w:date>";

				WritingElement_WriteNode_1( L"<w:calendar ",          m_oCalendar );
				WritingElement_WriteNode_1( L"<w:dateFormat ",        m_oDateFormat );
				WritingElement_WriteNode_1( L"<w:lid ",               m_oLid );
				WritingElement_WriteNode_1( L"<w:storeMappedDateAs ", m_oStoreMappedDateAs );

				sResult += L"</w:date>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_date;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:fullDate", m_oFullDate )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::CDateTime> m_oFullDate;

			nullable<ComplexTypes::Word::CCalendarType> m_oCalendar;
			nullable<ComplexTypes::Word::String> m_oDateFormat;
			nullable<ComplexTypes::Word::CLang> m_oLid;
			nullable<ComplexTypes::Word::CSdtDateMappingType> m_oStoreMappedDateAs;
		};

		//--------------------------------------------------------------------------------
		// CSdtDocPart 17.5.2.12 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtDocPart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtDocPart)
			CSdtDocPart(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CSdtDocPart()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;

				WritingElement_ReadNode( oNode, oChild, L"w:docPartCategory", m_oDocPartCategory );
				WritingElement_ReadNode( oNode, oChild, L"w:docPartGallery",  m_oDocPartGallery );
				WritingElement_ReadNode( oNode, oChild, L"w:docPartUnique",   m_oDocPartUnique );
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( L"w:docPartCategory" == sName )
						m_oDocPartCategory = oReader;
					else if ( L"w:docPartGallery" == sName )
						m_oDocPartGallery = oReader;
					else if ( L"w:docPartUnique" == sName )
						m_oDocPartUnique = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = L"<w:docPartList>";

				WritingElement_WriteNode_1( L"<w:docPartCategory ", m_oDocPartCategory );
				WritingElement_WriteNode_1( L"<w:docPartGallery ",  m_oDocPartGallery );
				WritingElement_WriteNode_1( L"<w:docPartUnique ",   m_oDocPartUnique );

				sResult += L"</w:docPartList>";

				return sResult;
			}
			std::wstring toXML2(const std::wstring& sName) const
			{
				std::wstring sResult = L"<" + sName + L">";

				WritingElement_WriteNode_1( L"<w:docPartCategory ", m_oDocPartCategory );
				WritingElement_WriteNode_1( L"<w:docPartGallery ",  m_oDocPartGallery );
				WritingElement_WriteNode_1( L"<w:docPartUnique ",   m_oDocPartUnique );

				sResult += L"</" + sName + L">";;

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_docPartList;
			}
		public:

 			nullable<ComplexTypes::Word::String> m_oDocPartCategory;
			nullable<ComplexTypes::Word::String> m_oDocPartGallery;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oDocPartUnique;
		};

		//--------------------------------------------------------------------------------
		// CSdtDropDownList 17.5.2.15 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtDropDownList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtDropDownList)
			CSdtDropDownList(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{ 
			}
			virtual ~CSdtDropDownList()
			{
				for ( size_t nIndex = 0; nIndex < m_arrListItem.size(); nIndex++ )
				{
					if ( m_arrListItem[nIndex] )
						delete m_arrListItem[nIndex];

					m_arrListItem[nIndex] = NULL;
				}

				m_arrListItem.clear();
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:lastValue", m_sLastValue );

				XmlUtils::CXmlNodes oChilds;
				if ( oNode.GetNodes( L"w:listItem", oChilds ) )
				{
					XmlUtils::CXmlNode oItemNode;
					for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
					{
						if ( oChilds.GetAt( nIndex, oItemNode ) )
						{
							ComplexTypes::Word::CSdtListItem *oListItem = new ComplexTypes::Word::CSdtListItem(oItemNode);
							if (oListItem) m_arrListItem.push_back( oListItem );
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
					if ( L"w:listItem" == sName )
					{
						ComplexTypes::Word::CSdtListItem *oListItem = new ComplexTypes::Word::CSdtListItem(oReader);
						if (oListItem)m_arrListItem.push_back( oListItem );
					}
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult;
				
				if ( m_sLastValue.IsInit() )
				{
					sResult = L"<w:dropDownList w:lastValue=\"";
                    sResult += *m_sLastValue;
					sResult += L"\">";
				}
				else
					sResult = L"<w:dropDownList>";

				for (size_t nIndex = 0; nIndex < m_arrListItem.size(); nIndex++ )
				{
					sResult += L"<w:listItem ";
					if (m_arrListItem[nIndex])
						sResult += m_arrListItem[nIndex]->ToString();
					sResult += L"/>";
				}

				sResult += L"</w:dropDownList>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_dropDownList;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:lastValue", m_sLastValue )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable_string									m_sLastValue;
			std::vector<ComplexTypes::Word::CSdtListItem*>	m_arrListItem;
		};

		//--------------------------------------------------------------------------------
		// CPlaceHolder 17.5.2.25 (Part 1)
		//--------------------------------------------------------------------------------
		class CPlaceHolder : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPlaceHolder)
			CPlaceHolder(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CPlaceHolder()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;

				WritingElement_ReadNode( oNode, oChild, L"w:docPart", m_oDocPart );
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( L"w:docPart" == sName )
						m_oDocPart = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = L"<w:placeholder>";

				WritingElement_WriteNode_1( L"<w:docPart ", m_oDocPart );

				sResult += L"</w:placeholder>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_placeholder;
			}
		public:

			// Nodes
			nullable<ComplexTypes::Word::String> m_oDocPart;
		};

		//--------------------------------------------------------------------------------
		// SdtEndPr 17.5.2.37 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtEndPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtEndPr)
			CSdtEndPr(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CSdtEndPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;

				if ( oNode.GetNode( L"w:rPr", oChild ) )
					m_oRPr = oChild;
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( L"w:rPr" == sName )
						m_oRPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = L"<w:sdtEndPr>";

				if ( m_oRPr.IsInit() )
					sResult += m_oRPr->toXML();

				sResult += L"</w:sdtEndPr>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_sdtEndPr;
			}
		public:

			nullable<OOX::Logic::CRunProperty> m_oRPr;
		};

		class CSdtCheckBoxSymbol : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtCheckBoxSymbol)
			CSdtCheckBoxSymbol(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CSdtCheckBoxSymbol()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w14:val", m_oVal );
				XmlMacroReadAttributeBase( oNode, L"w14:font", m_oFont );
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring toXML() const
			{
				return L"<w14:checkedState " + ToString() + L"/>";
			}
			std::wstring ToString() const
			{
				std::wstring sResult;
				ComplexTypes_WriteAttribute( L"w14:val=\"", m_oVal );
				ComplexTypes_WriteAttribute2( L"w14:font=\"", m_oFont );
				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_sdtCheckboxSymbol;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, L"w14:val", m_oVal )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w14:font", m_oFont )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:

			nullable<SimpleTypes::CShortHexNumber<> >   m_oVal;
			nullable<std::wstring>                      m_oFont;
		};

		class CSdtCheckBox : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtCheckBox)
			CSdtCheckBox(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CSdtCheckBox()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_w_sdtCheckbox;
			}
		public:

			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oChecked;
			nullable<CSdtCheckBoxSymbol> m_oCheckedState;
			nullable<CSdtCheckBoxSymbol> m_oUncheckedState;

			nullable<ComplexTypes::Word::String> m_oGroupKey;//Not from specification
		};
		class CSdtPicture : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtPicture)
			CSdtPicture(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CSdtPicture()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return et_w_sdtPicture;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_int m_oScaleFlag;
			nullable_bool m_oLockProportions;
			nullable_bool m_oRespectBorders;
			nullable_double m_oShiftX;
			nullable_double m_oShiftY;
		};

		//--------------------------------------------------------------------------------
		// SdtEndPr 17.5.2.38 (Part 1)
		//--------------------------------------------------------------------------------
		enum ESdtType
		{
			sdttypeUnknown      = -1,
			sdttypeBibliography =  0,
			sdttypeCitation     =  1,
			sdttypeComboBox     =  2,
			sdttypeDate         =  3,
			sdttypeDocPartList  =  4,
			sdttypeDocPartObj   =  5,
			sdttypeDropDownList =  6,
			sdttypeEquation     =  7,
			sdttypeGroup        =  8,
			sdttypePicture      =  9,
			sdttypeRichText     = 10,
			sdttypeText         = 11,
			sdttypeCheckBox     = 12
		};

		class CSdtPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdtPr)
			CSdtPr(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
				m_eType = sdttypeUnknown;
			}
			virtual ~CSdtPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			std::wstring toXMLStart() const;
			std::wstring toXMLEnd() const
			{
				return L"</w:sdtPr>";
			}

			virtual std::wstring toXML() const
			{
				return toXMLStart() + toXMLEnd();
			}
			virtual EElementType getType() const
			{
				return et_w_sdtPr;
			}
		public:

			ESdtType m_eType;

			nullable<ComplexTypes::Word::String> m_oAlias;
			nullable<ComplexTypes::Word::CSdtAppearance> m_oAppearance;
			nullable<OOX::Logic::CSdtComboBox> m_oComboBox;
			nullable<ComplexTypes::Word::CColor> m_oColor;
			nullable<ComplexTypes::Word::CDataBinding> m_oDataBinding;
			nullable<OOX::Logic::CDate> m_oDate;
			nullable<OOX::Logic::CSdtDocPart> m_oDocPartList;
			nullable<OOX::Logic::CSdtDocPart> m_oDocPartObj;
			nullable<OOX::Logic::CSdtDropDownList> m_oDropDownList;
			nullable<ComplexTypes::Word::CDecimalNumber> m_oId;
			nullable<ComplexTypes::Word::CDecimalNumber> m_oLabel;
			nullable<ComplexTypes::Word::CLock> m_oLock;
			nullable<OOX::Logic::CPlaceHolder> m_oPlaceHolder;
			nullable<OOX::Logic::CRunProperty> m_oRPr;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oShowingPlcHdr;
			nullable<ComplexTypes::Word::CUnsignedDecimalNumber> m_oTabIndex;
			nullable<ComplexTypes::Word::String> m_oTag;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oTemporary;
			nullable<ComplexTypes::Word::CSdtText> m_oText;
			nullable<CSdtCheckBox> m_oCheckbox;

			nullable<CFormPr> m_oFormPr; //Not from specification
			nullable<CTextFormPr> m_oTextFormPr; //Not from specification
			nullable<CSdtPicture> m_oPicture; //Not from specification
			nullable<CComplexFormPr> m_oComplexFormPr; // Not from specification
		};
		//--------------------------------------------------------------------------------
		// SdtContent 17.5.2.38 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdtContent : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CSdtContent)
			CSdtContent(OOX::Document *pMain = NULL) : WritingElementWithChilds<>(pMain)
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return et_w_sdtContent;
			}
			// Childs
		};

		//--------------------------------------------------------------------------------
		// Sdt 17.5.2.29 - 17.5.2.32 (Part 1)
		//--------------------------------------------------------------------------------
		class CSdt : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSdt)
			CSdt(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CSdt()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;

				if ( oNode.GetNode( L"w:sdtContent", oChild ) )
					m_oSdtContent = oChild;

				if ( oNode.GetNode( L"w:sdtEndPr", oChild ) )
					m_oSdtEndPr = oChild;

				if ( oNode.GetNode( L"w:sdtPr", oChild ) )
					m_oSdtPr = oChild;
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( L"w:sdtContent" == sName )
						m_oSdtContent = oReader;
					else if ( L"w:sdtEndPr" == sName )
						m_oSdtEndPr = oReader;
					else if ( L"w:sdtPr" == sName )
						m_oSdtPr = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = L"<w:sdt>";

				if ( m_oSdtPr.IsInit() )
					sResult += m_oSdtPr->toXML();

				if ( m_oSdtEndPr.IsInit() )
					sResult += m_oSdtEndPr->toXML();

				if ( m_oSdtContent.IsInit() )
					sResult += m_oSdtContent->toXML();

				sResult += L"</w:sdt>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_sdt;
			}
		public:

			nullable<OOX::Logic::CSdtContent> m_oSdtContent;
			nullable<OOX::Logic::CSdtEndPr  > m_oSdtEndPr;
			nullable<OOX::Logic::CSdtPr     > m_oSdtPr;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SDT_INCLUDE_H_
