/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "../../DocxFormat/IFileContainer.h"

namespace SimpleTypes
{
	class COnOff;
	class CRelationshipId;
	class CUnsignedDecimalNumber;

	namespace Spreadsheet
	{
		class CCellTypeType;
		class CDdeValueType;
	}
}

namespace ComplexTypes
{
	namespace Spreadsheet
	{
		class String;
	}
}

namespace OOX
{
	namespace Spreadsheet
	{
		class CText;

		class CExternalSheetNames : public WritingElementWithChilds<ComplexTypes::Spreadsheet::String>
		{
		public:
			WritingElement_AdditionMethods(CExternalSheetNames)
            WritingElement_XlsbConstructors(CExternalSheetNames)

			CExternalSheetNames();
			virtual ~CExternalSheetNames();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const;
		};
		class CAlternateUrls : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CAlternateUrls)
			WritingElement_XlsbConstructors(CAlternateUrls)

			CAlternateUrls();
			virtual ~CAlternateUrls();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj) {}

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			virtual EElementType getType() const;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader, nullable<SimpleTypes::CRelationshipId> & rid);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CRelationshipId> m_oAbsoluteUrlRid;
			nullable<SimpleTypes::CRelationshipId> m_oRelativeUrlRid;

			nullable_string m_oDriveId;
			nullable_string m_oItemId;
		};
		class CExternalDefinedName : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CExternalDefinedName)
            WritingElement_XlsbConstructors(CExternalDefinedName)
			CExternalDefinedName();;
			virtual ~CExternalDefinedName();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<std::wstring> m_oName;
			nullable<std::wstring> m_oRefersTo;
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oSheetId;
		};

		class CExternalDefinedNames : public WritingElementWithChilds<CExternalDefinedName>
		{
		public:
			WritingElement_AdditionMethods(CExternalDefinedNames)
            WritingElement_XlsbVectorConstructors(CExternalDefinedNames)
			CExternalDefinedNames();
			virtual ~CExternalDefinedNames();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;
			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			std::vector<XLS::BaseObjectPtr> toBin();
			virtual EElementType getType() const;
		};

		class CExternalCell : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CExternalCell)
            WritingElement_XlsbConstructors(CExternalCell)
			CExternalCell();
			virtual ~CExternalCell();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			
			virtual EElementType getType() const;
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string m_oRef;
			nullable<SimpleTypes::Spreadsheet::CCellTypeType> m_oType;
			nullable_uint m_oValueMetadata;

			nullable<CText> m_oValue;
		};

		class CExternalRow : public WritingElementWithChilds<CExternalCell>
		{
		public:
			WritingElement_AdditionMethods(CExternalRow)
            WritingElement_XlsbConstructors(CExternalRow)
			CExternalRow();
			virtual ~CExternalRow();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oR;
		};

		class CExternalSheetData : public WritingElementWithChilds<CExternalRow>
		{
		public:
			WritingElement_AdditionMethods(CExternalSheetData)
            WritingElement_XlsbConstructors(CExternalSheetData)
			CExternalSheetData();
			virtual ~CExternalSheetData();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oSheetId;
			nullable<SimpleTypes::COnOff> m_oRefreshError;
		};

		class CExternalSheetDataSet : public WritingElementWithChilds<CExternalSheetData>
		{
		public:
			WritingElement_AdditionMethods(CExternalSheetDataSet)
            WritingElement_XlsbVectorConstructors(CExternalSheetDataSet)
			CExternalSheetDataSet();
			virtual ~CExternalSheetDataSet();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			std::vector<XLS::BaseObjectPtr> toBin();
			virtual EElementType getType() const;
		};

		class CExternalBook : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CExternalBook)
            WritingElement_XlsbConstructors(CExternalBook)
			CExternalBook();
			virtual ~CExternalBook();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CRelationshipId > m_oRid;

			nullable<CExternalSheetNames> m_oSheetNames;
			nullable<CExternalDefinedNames> m_oDefinedNames;
			nullable<CExternalSheetDataSet> m_oSheetDataSet;
			nullable<CAlternateUrls> m_oAlternateUrls;
		};

		class CDdeValue : public WritingElementWithChilds<CText>
		{
		public:
			WritingElement_AdditionMethods(CDdeValue)
            WritingElement_XlsbConstructors(CDdeValue)
			CDdeValue();
			virtual ~CDdeValue();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;
			void ReadAttributes(XLS::BaseObjectPtr& obj);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			nullable<SimpleTypes::Spreadsheet::CDdeValueType > m_oType;
		};

		class CDdeValues : public WritingElementWithChilds<CDdeValue>
		{
		public:
			WritingElement_AdditionMethods(CDdeValues)
            WritingElement_XlsbConstructors(CDdeValues)
			CDdeValues();
			virtual ~CDdeValues();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const;

			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oRows;
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCols;
		};

		class CDdeItem : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDdeItem)
            WritingElement_XlsbConstructors(CDdeItem)
			CDdeItem();
			virtual ~CDdeItem();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;
			void ReadAttributes(XLS::BaseObjectPtr& obj);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<std::wstring > m_oName;
			nullable<SimpleTypes::COnOff> m_oOle;
			nullable<SimpleTypes::COnOff> m_oAdvise;
			nullable<SimpleTypes::COnOff> m_oPreferPic;

			nullable<CDdeValues> m_oDdeValues;
		};

		class CDdeItems : public WritingElementWithChilds<CDdeItem>
		{
		public:
			WritingElement_AdditionMethods(CDdeItems)
            WritingElement_XlsbConstructors(CDdeItems)
			CDdeItems();
			virtual ~CDdeItems();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const;
		};

		class CDdeLink : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDdeLink)
            WritingElement_XlsbConstructors(CDdeLink)
			CDdeLink();
			virtual ~CDdeLink();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<std::wstring > m_oDdeService;
			nullable<std::wstring > m_oDdeTopic;

			nullable<CDdeItems > m_oDdeItems;
		};

		class COleItem : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(COleItem)
            WritingElement_XlsbConstructors(COleItem)
			COleItem();
			virtual ~COleItem();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType() const;
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<std::wstring > m_oName;
			nullable<SimpleTypes::COnOff> m_oIcon;
			nullable<SimpleTypes::COnOff> m_oAdvise;
			nullable<SimpleTypes::COnOff> m_oPreferPic;
		};

		class COleItems : public WritingElementWithChilds<COleItem>
		{
		public:
			WritingElement_AdditionMethods(COleItems)
            WritingElement_XlsbConstructors(COleItems)
			COleItems();
			virtual ~COleItems();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const;
		};

		class COleLink : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(COleLink)
            WritingElement_XlsbConstructors(COleLink)
			COleLink();
			virtual ~COleLink();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual std::wstring toXML() const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const;
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CRelationshipId > m_oRid;
			nullable<std::wstring > m_oProgId;

			nullable<COleItems > m_oOleItems;
		};

		class CExternalLink : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CExternalLink(OOX::Document* pMain);
			CExternalLink(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath, const std::wstring & rId);
			virtual ~CExternalLink();

			XLS::BaseObjectPtr writeBin() const;
			void readBin(const CPath& oPath);

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;

			bool isValid() const;
			virtual const OOX::FileType type() const;
			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;
			const CPath& GetReadPath();

			nullable<CExternalBook>					m_oExternalBook;
			nullable<COleLink>						m_oOleLink;
			nullable<CDdeLink>						m_oDdeLink;

			nullable_string							m_oFileKey;
			nullable_string							m_oInstanceId;
		private:
			CPath									m_oReadPath;
            std::wstring                            m_rId;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
	} //ExternalLink
} // namespace OOX
