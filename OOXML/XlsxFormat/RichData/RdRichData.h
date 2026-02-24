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

#include "../Table/Autofilter.h"
#include "../../DocxFormat/IFileContainer.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}

	namespace Spreadsheet
	{
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueTypeReservedKeyFlag : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRichValueTypeReservedKeyFlag)
			CRichValueTypeReservedKeyFlag();
			virtual ~CRichValueTypeReservedKeyFlag();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_bool m_oValue;
			nullable_string m_oName;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueTypeReservedKey : public WritingElementWithChilds<CRichValueTypeReservedKeyFlag>
		{
		public:
			WritingElement_AdditionMethods(CRichValueTypeReservedKey)
			CRichValueTypeReservedKey();
			virtual ~CRichValueTypeReservedKey();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_string m_oName;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueTypeKeyFlags : public WritingElementWithChilds<CRichValueTypeReservedKey>
		{
		public:
			WritingElement_AdditionMethods(CRichValueTypeKeyFlags)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueType : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRichValueType)
			CRichValueType();
			virtual ~CRichValueType();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_string m_oName;
			nullable<CRichValueTypeKeyFlags> m_oKeyFlags;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueTypes : public WritingElementWithChilds<CRichValueType>
		{
		public:
			WritingElement_AdditionMethods(CRichValueTypes)
			CRichValueTypes();
			virtual ~CRichValueTypes();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueGlobalType : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRichValueGlobalType)
			CRichValueGlobalType();
			virtual ~CRichValueGlobalType();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable<CRichValueTypeKeyFlags> m_oKeyFlags;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueTypesInfo : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRichValueTypesInfo)
			CRichValueTypesInfo();
			virtual ~CRichValueTypesInfo();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable<CRichValueGlobalType> m_oGlobal;
			nullable<CRichValueTypes> m_oTypes;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueKey : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRichValueKey)
			CRichValueKey();
			virtual ~CRichValueKey();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<SimpleTypes::Spreadsheet::CRichValueValueType> m_oT;
			nullable_string m_oN;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueStructure : public WritingElementWithChilds<CRichValueKey>
		{
		public:
			WritingElement_AdditionMethods(CRichValueStructure)
			CRichValueStructure();
			virtual ~CRichValueStructure();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oT;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueStructures : public WritingElementWithChilds<CRichValueStructure>
		{
		public:
			WritingElement_AdditionMethods(CRichValueStructures)
			CRichValueStructures();
			virtual ~CRichValueStructures();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_uint m_oCount;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueFallback : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRichValueFallback)
			CRichValueFallback();
			virtual ~CRichValueFallback();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<SimpleTypes::Spreadsheet::CRichValueFallbackType> m_oT;
			std::wstring m_sContent;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValue : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRichValue)
			CRichValue();
			virtual ~CRichValue();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_uint m_oS;
			
			nullable<CRichValueFallback> m_oFb;
			std::vector<std::wstring> m_arrV; //todooo types ...
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRichValueData : public WritingElementWithChilds<CRichValue>
		{
		public:
			WritingElement_AdditionMethods(CRichValueData)
			CRichValueData();
			virtual ~CRichValueData();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_uint m_oCount;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRdRichValueFile : public OOX::File
		{
		public:
			CRdRichValueFile(OOX::Document* pMain);
			CRdRichValueFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CRdRichValueFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			nullable<CRichValueData> m_oRvData;
		private:
			CPath m_oReadPath;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRdRichValueStructureFile : public OOX::File
		{
		public:
			CRdRichValueStructureFile(OOX::Document* pMain);
			CRdRichValueStructureFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CRdRichValueStructureFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();
			
			nullable<CRichValueStructures> m_oRvStructures;

		private:
			CPath m_oReadPath;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CRdRichValueTypesFile : public OOX::File
		{
		public:
			CRdRichValueTypesFile(OOX::Document* pMain);
			CRdRichValueTypesFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CRdRichValueTypesFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			nullable<CRichValueTypesInfo> m_oRvTypesInfo;
		private:
			CPath m_oReadPath;
		};
	} //Spreadsheet
} // namespace OOX
