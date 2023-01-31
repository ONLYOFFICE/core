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

#include "Logic/ParagraphProperty.h"
#include "Logic/RunProperty.h"
#include "Logic/Pict.h"
#include "Drawing/Drawing.h"

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
			CLvlLegacy();
			virtual ~CLvlLegacy();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<SimpleTypes::COnOff               > m_oLegacy;
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
			CLevelText();
			virtual ~CLevelText();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable< SimpleTypes::COnOff>	m_oNull;
            nullable_string						m_sVal;
		};

		//--------------------------------------------------------------------------------
		// LevelSuffix 17.9.29 (Part 1)
		//--------------------------------------------------------------------------------
		class CLevelSuffix : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CLevelSuffix)
			CLevelSuffix();
			virtual ~CLevelSuffix();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CLevelSuffix> m_oVal;
		};

		//--------------------------------------------------------------------------------
		// MultiLevelType 17.9.13 (Part 1)
		//--------------------------------------------------------------------------------
		class CMultiLevelType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CMultiLevelType)
			CMultiLevelType();
			virtual ~CMultiLevelType();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CMultiLevelType> m_oVal;
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
			CLvl();
			virtual ~CLvl();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			void ReadElements(XmlUtils::CXmlLiteReader& oReader, int nDepth);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_int								m_oIlvl;
			nullable<SimpleTypes::COnOff>				m_oTentative;
			nullable<SimpleTypes::CLongHexNumber>		m_oTplc;

			nullable<ComplexTypes::Word::COnOff2>	m_oIsLgl;
			nullable<ComplexTypes::Word::CLvlLegacy>						m_oLegacy;
			nullable<ComplexTypes::Word::CJc>								m_oLvlJc;
			nullable<ComplexTypes::Word::CDecimalNumber>					m_oLvlPicBulletId;
			nullable<ComplexTypes::Word::CDecimalNumber>					m_oLvlRestart;
			nullable<ComplexTypes::Word::CLevelText>						m_oLvlText;
			nullable<ComplexTypes::Word::CNumFmt>							m_oNumFmt;
			nullable<OOX::Logic::CParagraphProperty>						m_oPPr;
			nullable<ComplexTypes::Word::String>							m_oPStyle;
			nullable<OOX::Logic::CRunProperty>								m_oRPr;
			nullable<ComplexTypes::Word::CDecimalNumber>					m_oStart;
			nullable<ComplexTypes::Word::CLevelSuffix>						m_oSuffix;

		};
		//--------------------------------------------------------------------------------
		// AbstractNum 17.9.1 (Part 1); listDef element [Word 2003 XML]
		//--------------------------------------------------------------------------------
		class CAbstractNum : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAbstractNum)
			CAbstractNum();
			virtual ~CAbstractNum();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			void copy(CAbstractNum* copies);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_int									m_oAbstractNumId;

			std::vector<OOX::Numbering::CLvl*>				m_arrLvl;
			nullable<ComplexTypes::Word::CMultiLevelType>	m_oMultiLevelType;
			nullable<ComplexTypes::Word::String>			m_oName;
			nullable<ComplexTypes::Word::CLongHexNumber>	m_oNsid;
			nullable<ComplexTypes::Word::String>			m_oNumStyleLink;
			nullable<ComplexTypes::Word::String>			m_oStyleLink;
			nullable<ComplexTypes::Word::CLongHexNumber>	m_oTmpl;
			
			std::map<int, size_t>							m_mapLvl; //ilvl, index m_arrLvl
		};
		//--------------------------------------------------------------------------------
		// NumLvl 17.9.9 (Part 1)
		//--------------------------------------------------------------------------------
		class CNumLvl : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNumLvl)
			CNumLvl();
			virtual ~CNumLvl();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_int									m_oIlvl;

			nullable<OOX::Numbering::CLvl>					m_oLvl;
			nullable<ComplexTypes::Word::CDecimalNumber>	m_oStartOverride;
		};
		//--------------------------------------------------------------------------------
		// Num 17.9.16 (Part 1)
		//--------------------------------------------------------------------------------
		class CNum : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNum)
			CNum();
			virtual ~CNum();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_int									m_oNumId;

			nullable<ComplexTypes::Word::CDecimalNumber>	m_oAbstractNumId;
			std::vector<OOX::Numbering::CNumLvl*>			m_arrLvlOverride;

			std::map<int, size_t>							m_mapLvlOverride; //ilvl, index m_arrLvlOverride
		};
		//--------------------------------------------------------------------------------
		// NumPicBullet 17.9.21 (Part 1)
		//--------------------------------------------------------------------------------
		class CNumPicBullet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNumPicBullet)
			CNumPicBullet();
			virtual ~CNumPicBullet();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_int					m_oNumPicBulletId;

			nullable<OOX::Logic::CDrawing>	m_oDrawing;
			nullable<OOX::Logic::CPicture>	m_oVmlDrawing;
		};
	} // Numbering
} // OOX 

namespace OOX
{
	class CNumbering : public OOX::File, public IFileContainer, public WritingElement
	{
	public:
		CNumbering(OOX::Document *pMain);
		CNumbering(OOX::Document *pMain, const CPath& oRootPath, const CPath& oPath);
		CNumbering(XmlUtils::CXmlNode& oNode);
		CNumbering(XmlUtils::CXmlLiteReader& oReader);
		virtual ~CNumbering();

		const CNumbering& operator =(const XmlUtils::CXmlNode& oNode);
		const CNumbering& operator =(const XmlUtils::CXmlLiteReader& oReader);

		virtual void read(const CPath& oFilePath);
		virtual void read(const CPath& oRootPath, const CPath& oFilePath);

		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
		virtual std::wstring toXML() const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		virtual EElementType getType() const;

		std::map<int, std::pair<int, size_t>>			m_mapNum; // numId, abstractNumId/index
		std::map<int, size_t>							m_mapAbstractNum; //abstractNumId, index m_arrAbstractNum
		std::vector<OOX::Numbering::CAbstractNum*>		m_arrAbstractNum;
		std::vector<OOX::Numbering::CNum*>				m_arrNum;
		nullable<ComplexTypes::Word::CDecimalNumber>	m_oNumIdMacAtCleanup;
		std::vector<OOX::Numbering::CNumPicBullet*>		m_arrNumPicBullet;

		std::vector<std::map<int, int>>					m_mapEmbeddedNames;

	};
} // namespace OOX
