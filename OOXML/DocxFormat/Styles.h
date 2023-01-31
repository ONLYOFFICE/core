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
			CLsdException();
			virtual ~CLsdException();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff             > m_oLocked;
			nullable<std::wstring                    > m_sName;
			nullable<SimpleTypes::COnOff             > m_oQFormat;
			nullable<SimpleTypes::COnOff             > m_oSemiHidden;
			nullable<SimpleTypes::CDecimalNumber     > m_oUiPriority;
			nullable<SimpleTypes::COnOff             > m_oUnhideWhenUsed;
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
			CTableStyleProperties();
			CTableStyleProperties(XmlUtils::CXmlNode &oNode);
			CTableStyleProperties(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTableStyleProperties();

			const CTableStyleProperties& operator=(const XmlUtils::CXmlNode &oNode);
			const CTableStyleProperties& operator=(const XmlUtils::CXmlLiteReader& oReader);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			static const CTableStyleProperties Merge(const CTableStyleProperties& oPrev, const CTableStyleProperties& oCurrent);

			template<typename Type>
			static nullable<Type> Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTblStyleOverrideType> m_oType;

			nullable<OOX::Logic::CParagraphProperty> m_oParPr;
			nullable<OOX::Logic::CRunProperty> m_oRunPr;
			nullable<OOX::Logic::CTableProperty> m_oTblPr;
			nullable<OOX::Logic::CTableCellProperties> m_oTcPr;
			nullable<OOX::Logic::CTableRowProperties> m_oTrPr;
		};

	}
	//--------------------------------------------------------------------------------
	// DocDefaults 17.7.5.1 (Part 1)
	//--------------------------------------------------------------------------------
	class CDocDefaults : public WritingElement
	{
	public:

		CDocDefaults();
		CDocDefaults(XmlUtils::CXmlNode &oNode);
		CDocDefaults(XmlUtils::CXmlLiteReader& oReader);
		virtual ~CDocDefaults();

		const CDocDefaults& operator=(const XmlUtils::CXmlNode &oNode);
		const CDocDefaults& operator=(const XmlUtils::CXmlLiteReader& oReader);

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

		nullable<OOX::Logic::CParagraphProperty> m_oParPr;
		nullable<OOX::Logic::CRunProperty      > m_oRunPr;
	};

	//--------------------------------------------------------------------------------
	// LatentStyles 17.7.4.6 (Part 1)
	//--------------------------------------------------------------------------------
	class CLatentStyles : public WritingElement
	{
	public:

		CLatentStyles();
		CLatentStyles(XmlUtils::CXmlNode &oNode);
		CLatentStyles(XmlUtils::CXmlLiteReader& oReader);
		virtual ~CLatentStyles();

		const CLatentStyles& operator=(const XmlUtils::CXmlNode &oNode);
		const CLatentStyles& operator=(const XmlUtils::CXmlLiteReader& oReader);

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		SimpleTypes::CDecimalNumber		                m_oCount;
		SimpleTypes::COnOff								m_oDefLockedState;
		SimpleTypes::COnOff								m_oDefQFormat;
		SimpleTypes::COnOff							    m_oDefSemiHidden;
		SimpleTypes::CDecimalNumber                     m_oDefUiPriority;
		SimpleTypes::COnOff							    m_oDefUnhideWhenUsed;

		std::vector<ComplexTypes::Word::CLsdException*> m_arrLsdException;
	};

	//--------------------------------------------------------------------------------
	// Style 17.7.4.17 (Part 1)
	//--------------------------------------------------------------------------------
	class CStyle : public WritingElement
	{
	public:

		CStyle(OOX::Document *pMain = NULL);
		CStyle(XmlUtils::CXmlNode &oNode);
		CStyle(XmlUtils::CXmlLiteReader& oReader);
		virtual ~CStyle();

		void ClearItems();

		const CStyle& operator=(const XmlUtils::CXmlNode &oNode);
		const CStyle& operator=(const XmlUtils::CXmlLiteReader& oReader);

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		nullable<SimpleTypes::COnOff>		m_oCustomStyle;
		nullable<SimpleTypes::COnOff>		m_oDefault;
		nullable_string						m_sStyleId;
		nullable<SimpleTypes::CStyleType>	m_oType;

		nullable<ComplexTypes::Word::String>							m_oAliases;
        nullable<ComplexTypes::Word::COnOff2>	m_oAutoRedefine;
		nullable<ComplexTypes::Word::String>							m_oBasedOn;
        nullable<ComplexTypes::Word::COnOff2>	m_oHidden;
		nullable<ComplexTypes::Word::String>							m_oLink;
        nullable<ComplexTypes::Word::COnOff2>	m_oLocked;
		nullable<ComplexTypes::Word::String>							m_oName;
		nullable<ComplexTypes::Word::String>							m_oNext;
        nullable<ComplexTypes::Word::COnOff2>	m_oPersonal;
        nullable<ComplexTypes::Word::COnOff2>	m_oPersonalCompose;
        nullable<ComplexTypes::Word::COnOff2>	m_oPersonalReply;
		nullable<OOX::Logic::CParagraphProperty>						m_oParPr;
        nullable<ComplexTypes::Word::COnOff2>	m_oQFormat;
		nullable<OOX::Logic::CRunProperty>								m_oRunPr;
		nullable<ComplexTypes::Word::CLongHexNumber>					m_oRsid;
        nullable<ComplexTypes::Word::COnOff2>	m_oSemiHidden;
		nullable<OOX::Logic::CTableProperty>							m_oTblPr;		
		std::vector<OOX::Logic::CTableStyleProperties*>					m_arrTblStylePr;
		nullable<OOX::Logic::CTableCellProperties>						m_oTcPr;
		nullable<OOX::Logic::CTableRowProperties>						m_oTrPr;
		nullable<ComplexTypes::Word::CDecimalNumber>					m_oUiPriority;
        nullable<ComplexTypes::Word::COnOff2>	m_oUnhideWhenUsed;
	};

	//--------------------------------------------------------------------------------
	// Styles 17.7.4.18 (Part 1)
	//--------------------------------------------------------------------------------
	class CStyles : public OOX::File, public WritingElement
	{
	public:
		CStyles(OOX::Document *pMain);
		CStyles(OOX::Document *pMain, const CPath& oPath);
		CStyles(XmlUtils::CXmlNode& oNode);
		CStyles(XmlUtils::CXmlLiteReader& oReader);
		virtual ~CStyles();

		const CStyles& operator =(const XmlUtils::CXmlNode& oNode);
		const CStyles& operator =(const XmlUtils::CXmlLiteReader& oReader);

		virtual void read(const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

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
