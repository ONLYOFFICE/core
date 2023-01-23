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

			CMacroName();
			virtual ~CMacroName();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

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

			CFFHelpText();
			virtual ~CFFHelpText();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CInfoTextType>	m_oType;
			nullable<SimpleTypes::CFFHelpTextVal>	m_oVal;
		};

		//--------------------------------------------------------------------------------
		// FFName 17.16.27 (Part 1)
		//--------------------------------------------------------------------------------
		class CFFName : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFFName)

			CFFName();
			virtual ~CFFName();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

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

			CFFStatusText();
			virtual ~CFFStatusText();

			virtual void FromXML(XmlUtils::CXmlNode& oNode);
			virtual void FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CInfoTextType>	m_oType;
			nullable<SimpleTypes::CFFStatusTextVal>	m_oVal;
		};

		//--------------------------------------------------------------------------------
		// FFTextType 17.16.34 (Part 1)
		//--------------------------------------------------------------------------------
		class CFFTextType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CFFTextType)

			CFFTextType();
			virtual ~CFFTextType();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CFFTextType> m_oVal;
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
			CFFCheckBox();
			CFFCheckBox(XmlUtils::CXmlNode &oNode);
			CFFCheckBox(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CFFCheckBox();

			const CFFCheckBox &operator=(const XmlUtils::CXmlNode &oNode);
			const CFFCheckBox &operator=(const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			nullable<ComplexTypes::Word::COnOff2     > m_oChecked;
			nullable<ComplexTypes::Word::COnOff2     > m_oDefault;
			nullable<ComplexTypes::Word::CHpsMeasure > m_oSize;
			nullable<ComplexTypes::Word::COnOff2     > m_oSizeAuto;

		};

		//--------------------------------------------------------------------------------
		// CFFDDList 17.16.9 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFFDDList : public WritingElement
		{
		public:
			CFFDDList();
			CFFDDList(XmlUtils::CXmlNode &oNode);
			CFFDDList(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CFFDDList();

			virtual void ClearItems();

			const CFFDDList &operator=(const XmlUtils::CXmlNode &oNode);
			const CFFDDList &operator=(const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			nullable<ComplexTypes::Word::CDecimalNumber> m_oDefault;
			nullable<ComplexTypes::Word::CDecimalNumber> m_oResult;
            std::vector<ComplexTypes::Word::String*> m_arrListEntry;
		};

		//--------------------------------------------------------------------------------
		// CFFTextInput 17.16.33 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFFTextInput : public WritingElement
		{
		public:
			CFFTextInput();
			CFFTextInput(XmlUtils::CXmlNode &oNode);
			CFFTextInput(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CFFTextInput();

			const CFFTextInput &operator=(const XmlUtils::CXmlNode &oNode);
			const CFFTextInput &operator=(const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

            nullable<ComplexTypes::Word::String> m_oDefault;
            nullable<ComplexTypes::Word::String> m_oFormat;
			nullable<ComplexTypes::Word::CDecimalNumber> m_oMaxLength;
			nullable<ComplexTypes::Word::CFFTextType> m_oType;
		};

		//--------------------------------------------------------------------------------
		// CFFData 17.16.17 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFFData : public WritingElement
		{
		public:
			CFFData();
			CFFData(XmlUtils::CXmlNode &oNode);
			CFFData(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CFFData();

			const CFFData &operator=(const XmlUtils::CXmlNode &oNode);
			const CFFData &operator=(const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		public:
			nullable<ComplexTypes::Word::COnOff2> m_oCalcOnExit;
			nullable<OOX::Logic::CFFCheckBox> m_oCheckBox;
			nullable<OOX::Logic::CFFDDList> m_oDDList;
			nullable<ComplexTypes::Word::COnOff2> m_oEnabled;
			nullable<ComplexTypes::Word::CMacroName> m_oEntryMacro;
			nullable<ComplexTypes::Word::CMacroName> m_oExitMacro;
			nullable<ComplexTypes::Word::CFFHelpText> m_oHelpText;
			nullable<ComplexTypes::Word::CDecimalNumber> m_oLabel;
			nullable<ComplexTypes::Word::CFFName> m_oName;
			nullable<ComplexTypes::Word::CFFStatusText> m_oStatusText;
			nullable<ComplexTypes::Word::CUnsignedDecimalNumber> m_oTabIndex;
			nullable<OOX::Logic::CFFTextInput> m_oTextInput;

		};

		//--------------------------------------------------------------------------------
		// CFldChar 17.16.18 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFldChar : public WritingElement
		{
		public:
			CFldChar(OOX::Document *pMain = NULL);
			CFldChar(XmlUtils::CXmlNode &oNode);
			CFldChar(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CFldChar();

			const CFldChar &operator=(const XmlUtils::CXmlNode &oNode);
			const CFldChar &operator=(const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff         > m_oDirty;
			nullable<SimpleTypes::CFldCharType   > m_oFldCharType;
			nullable<SimpleTypes::COnOff         > m_oFldLock;

			nullable<OOX::Logic::CFFData         > m_oFFData;
		};
	} // namespace Logic
} // namespace OOX
