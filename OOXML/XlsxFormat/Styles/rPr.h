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

#include "../CommonInclude.h"

namespace NSBinPptxRW
{
	class CBinaryFileReader;
	class CXlsbBinaryWriter;
}

namespace OOX
{
	namespace Spreadsheet
	{
        class CFont;

		class CRgbColor : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRgbColor)
            WritingElement_XlsbConstructors(CRgbColor)
			CRgbColor();
			virtual ~CRgbColor();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CHexColor> m_oRgb;
		};
		
		class CIndexedColors : public WritingElementWithChilds<CRgbColor>
		{
		public:
			WritingElement_AdditionConstructors(CIndexedColors)
            WritingElement_XlsbVectorConstructors(CIndexedColors)
			CIndexedColors();
			virtual ~CIndexedColors();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			virtual EElementType getType () const;

			static bool GetDefaultRGBAByIndex(int index, unsigned char& unR, unsigned char& unG, unsigned char& unB, unsigned char& unA);
			static int GetDefaultIndexByRGBA( unsigned char unR, unsigned char unG, unsigned char unB, unsigned char unA);

			std::map<int, CRgbColor*> mapIndexedColors;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class CColor : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColor)
			WritingElement_XlsbConstructors(CColor)
			CColor(OOX::Document *pMain = NULL);
			virtual ~CColor();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			void fromBin(XLS::BaseObject* obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XLS::BaseObject* obj);

		public:
			nullable<SimpleTypes::COnOff>						m_oAuto;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oIndexed;
			nullable<SimpleTypes::Spreadsheet::CHexColor>		m_oRgb;
			nullable<SimpleTypes::Spreadsheet::CThemeColor>		m_oThemeColor;
			nullable<SimpleTypes::CDouble>						m_oTint;
		};

		class CMruColors : public WritingElementWithChilds<CColor>
		{
		public:
			WritingElement_AdditionConstructors(CMruColors)
            WritingElement_XlsbVectorConstructors(CMruColors)
			CMruColors();
			virtual ~CMruColors();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class CCharset : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCharset)
			CCharset(OOX::Document *pMain = NULL);
			virtual ~CCharset();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CFontCharset>	m_oCharset;
		};

		class CVerticalAlign : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CVerticalAlign)
			CVerticalAlign(OOX::Document *pMain = NULL);
			virtual ~CVerticalAlign();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CVerticalAlignRun>	m_oVerticalAlign;
		};

		class CFontFamily : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFontFamily)
			CFontFamily(OOX::Document *pMain = NULL);
			virtual ~CFontFamily();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CFontFamily>	m_oFontFamily;
		};

		class CFontScheme : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFontScheme)
			CFontScheme();
			virtual ~CFontScheme();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CFontScheme>	m_oFontScheme;
		};

		class CUnderline : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CUnderline)
			CUnderline(OOX::Document *pMain = NULL);
			virtual ~CUnderline();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CUnderline>	m_oUnderline;
		};

		//необработано:
		class CRPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRPr)
			CRPr(OOX::Document *pMain = NULL);
			virtual ~CRPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

            void fromFont(CFont* font);
			void fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType);
			void toXLSB (NSBinPptxRW::CXlsbBinaryWriter& oStream) const;
			_UINT32 getXLSBSize() const;

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oBold;
            nullable<CCharset>                                                      m_oCharset;
            nullable<CColor>                                                        m_oColor;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oCondense;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oExtend;
            nullable<CFontFamily >                                                  m_oFamily;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oItalic;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oOutline;
            nullable<ComplexTypes::Spreadsheet::String >                            m_oRFont;
            nullable<CFontScheme>                                                   m_oScheme;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oShadow;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oStrike;
			nullable<ComplexTypes::Spreadsheet::CDouble>							m_oSz;
            nullable<CUnderline>                                                    m_oUnderline;
            nullable<CVerticalAlign>                                                m_oVertAlign;

            //////
			nullable<SimpleTypes::CDecimalNumber>									m_nFontIndex;
		};

	} //Spreadsheet
} // namespace OOX

