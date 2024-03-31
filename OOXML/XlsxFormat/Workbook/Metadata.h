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
//-----------------------------------------------------------------------------------------------------------------------
		class CMdxKPI : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CMdxKPI)
			CMdxKPI();
			virtual ~CMdxKPI();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_uint m_oN;
			nullable_uint m_oNp;
			nullable<SimpleTypes::Spreadsheet::CMdxKPIProperty> m_oP;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//-----------------------------------------------------------------------------------------------------------------------
		class CMdxMemeberProp : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CMdxMemeberProp)
			CMdxMemeberProp();
			virtual ~CMdxMemeberProp();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_uint m_oN;
			nullable_uint m_oNp;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//-----------------------------------------------------------------------------------------------------------------------
		class CMetadataStringIndex : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CMetadataStringIndex)
			CMetadataStringIndex();
			virtual ~CMetadataStringIndex();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_uint m_oX;
			nullable_bool m_oS;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMdxSet : public WritingElementWithChilds<CMetadataStringIndex>
		{
		public:
			WritingElement_AdditionMethods(CMdxSet)
			CMdxSet();
			virtual ~CMdxSet();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_uint m_oNs;
			nullable_uint m_oC;
			nullable<SimpleTypes::Spreadsheet::CMdxSetOrder> m_oO;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMdxTuple : public WritingElementWithChilds<CMetadataStringIndex>
		{
		public:
			WritingElement_AdditionMethods(CMdxTuple)
			CMdxTuple();
			virtual ~CMdxTuple();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_uint m_oC;
			nullable_string m_oCt;
			nullable_uint m_oSi;
			nullable_uint m_oFi;
			nullable_uint m_oBc;
			nullable_uint m_oFc;
			nullable_bool m_oI;
			nullable_bool m_oU;
			nullable_bool m_oSt;
			nullable_bool m_oB;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMdx : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CMdx)
			CMdx();
			virtual ~CMdx();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<CMdxTuple> m_oMdxTuple;
			nullable<CMdxSet> m_oMdxSet;
			nullable<CMdxKPI> m_oCMdxKPI;
			nullable<CMdxMemeberProp> m_oMdxMemeberProp;
				
			nullable_uint m_oN;
			nullable<SimpleTypes::Spreadsheet::CMdxFunctionType> m_oF;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMdxMetadata : public WritingElementWithChilds<CMdx>
		{
		public:
			WritingElement_AdditionMethods(CMdxMetadata)
			CMdxMetadata();
			virtual ~CMdxMetadata();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_uint m_oCount;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMetadataString : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CMetadataString)
			CMetadataString();
			virtual ~CMetadataString();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oV;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMetadataRecord : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CMetadataRecord)
			CMetadataRecord();
			virtual ~CMetadataRecord();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_uint m_oT;
			nullable_uint m_oV;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CFutureMetadataBlock : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CFutureMetadataBlock)
			CFutureMetadataBlock();
			virtual ~CFutureMetadataBlock();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMetadataType : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CMetadataType)
			CMetadataType();
			virtual ~CMetadataType();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string m_oName;
			nullable_uint m_oMinSupportedVersion;
			nullable_bool m_oGhostRow;
			nullable_bool m_oGhostCol;
			nullable_bool m_oEdit;
			nullable_bool m_oDelete;
			nullable_bool m_oCopy;
			nullable_bool m_oPasteAll;
			nullable_bool m_oPasteFormulas;
			nullable_bool m_oPasteValues;
			nullable_bool m_oPasteFormats;
			nullable_bool m_oPasteComments;
			nullable_bool m_oPasteDataValidation;
			nullable_bool m_oPasteBorders;
			nullable_bool m_oPasteColWidths;
			nullable_bool m_oPasteNumberFormats;
			nullable_bool m_oMerge;
			nullable_bool m_oSplitFirst;
			nullable_bool m_oSplitAll;
			nullable_bool m_oRowColShift;
			nullable_bool m_oClearAll;
			nullable_bool m_oClearFormats;
			nullable_bool m_oClearContents;
			nullable_bool m_oClearComments;
			nullable_bool m_oAssign;
			nullable_bool m_oCoerce;
			nullable_bool m_oCellMeta;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMetadataTypes : public WritingElementWithChilds<CMetadataType>
		{
		public:
			WritingElement_AdditionMethods(CMetadataTypes)
			CMetadataTypes();
			virtual ~CMetadataTypes();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
			
			nullable_uint m_oCount;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMetadataBlock : public WritingElementWithChilds<CMetadataRecord>
		{
		public:
			WritingElement_AdditionMethods(CMetadataBlock)
			CMetadataBlock();
			virtual ~CMetadataBlock();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMetadataBlocks : public WritingElementWithChilds<CMetadataBlock>
		{
		public:
			WritingElement_AdditionMethods(CMetadataBlocks)
			CMetadataBlocks();
			virtual ~CMetadataBlocks();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
			
			nullable_uint m_oCount;

			std::wstring m_sName;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMetadataStrings : public WritingElementWithChilds<CMetadataString>
		{
		public:
			WritingElement_AdditionMethods(CMetadataStrings)
			CMetadataStrings();
			virtual ~CMetadataStrings();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_uint m_oCount;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CFutureMetadata : public WritingElementWithChilds<CFutureMetadataBlock>
		{
		public:
			WritingElement_AdditionMethods(CFutureMetadata)
			CFutureMetadata();
			virtual ~CFutureMetadata();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		
			nullable_string m_oName;
			nullable_uint m_oCount;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//--------------------------------------------------------------------------------------------------------------
		class CMetadata : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CMetadata)
			CMetadata();
			virtual ~CMetadata();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

		public:
			nullable<CMetadataTypes> m_oMetadataTypes;
			nullable<CMetadataStrings> m_oMetadataStrings;
			nullable<CMdxMetadata> m_oMdxMetadata;
			nullable<CMetadataBlocks> m_oCellMetadata;
			nullable<CMetadataBlocks> m_oValueMetadata;

			std::vector<CFutureMetadata*> m_arFutureMetadata;

			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CMetadataFile : public OOX::File
		{
		public:
			CMetadataFile(OOX::Document* pMain);
			CMetadataFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CMetadataFile();

			//void readBin(const CPath& oPath);
			//XLS::BaseObjectPtr WriteBin() const;
			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			nullable<CMetadata> m_oMetadata;
		private:
			CPath m_oReadPath;
		};
//-----------------------------------------------------------------------------------------------------------------------
		class CDynamicArrayProperties : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDynamicArrayProperties)
			CDynamicArrayProperties();
			virtual ~CDynamicArrayProperties();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_bool m_oFDynamic;
			nullable_bool m_oFCollapsed;
			
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
//-----------------------------------------------------------------------------------------------------------------------
		class CRichValueBlock : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CRichValueBlock)
			CRichValueBlock();
			virtual ~CRichValueBlock();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_uint m_oI;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
	} //Spreadsheet
} // namespace OOX
