/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../WritingElement.h"
#include "../../Base/Nullable.h"

namespace XLS
{
	class BiffStructure;
}

namespace SimpleTypes
{
	class CRelationshipId;

	namespace Spreadsheet
	{
		class CTabularSlicerCacheSortOrder;
		class CSlicerCacheCrossFilter;
		class CSlicerStyleType;
	}
}

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}

	namespace Spreadsheet
	{
		class CSlicerCacheOlapLevelName : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSlicerCacheOlapLevelName)
			CSlicerCacheOlapLevelName(){}
			virtual ~CSlicerCacheOlapLevelName(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BiffStructure& obj);
			void toBin(XLS::BiffStructure* obj);
            void ReadAttributes(XLS::BiffStructure& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_SlicerCacheOlapLevelName;
			}
			//Attributes
			nullable_string m_oUniqueName;
			nullable_uint m_oCount;
		};

		class CSlicerCacheHideNoData : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSlicerCacheHideNoData)
            WritingElement_XlsbConstructors(CSlicerCacheHideNoData)
			CSlicerCacheHideNoData(){}
			virtual ~CSlicerCacheHideNoData(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_SlicerCacheHideNoData;
			}
			//Attributes
			nullable_uint m_oCount;//0
			//Members
			std::vector<CSlicerCacheOlapLevelName> m_oSlicerCacheOlapLevelName;
		};

		class CTableSlicerCache : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTableSlicerCache)
            WritingElement_XlsbConstructors(CTableSlicerCache)
			CTableSlicerCache(){}
			virtual ~CTableSlicerCache(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_TableSlicerCache;
			}
			//Attributes
			nullable_uint m_oTableId;
			nullable_uint m_oColumn;
			nullable<SimpleTypes::Spreadsheet::CTabularSlicerCacheSortOrder> m_oSortOrder;//ascending
			nullable_bool m_oCustomListSort;//True
			nullable<SimpleTypes::Spreadsheet::CSlicerCacheCrossFilter> m_oCrossFilter;//showItemsWithDataAtTop
			//Members
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};

		class CSlicerStyleElement : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSlicerStyleElement)
            WritingElement_XlsbConstructors(CSlicerStyleElement)
			CSlicerStyleElement(){}
			virtual ~CSlicerStyleElement(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);
            virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
            virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const
			{
				return et_x_SlicerStyleElement;
			}
			//Attributes
			nullable<SimpleTypes::Spreadsheet::CSlicerStyleType> m_oType;
			nullable_uint m_oDxfId;
		};

		class CSlicerCache : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSlicerCache)
            WritingElement_XlsbConstructors(CSlicerCache)
			CSlicerCache(){}
			virtual ~CSlicerCache(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toBinTable();
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_SlicerCache;
			}
			//Attributes
			nullable<SimpleTypes::CRelationshipId> m_oRId;
		};

		class CSlicerRef : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSlicerRef)
            WritingElement_XlsbConstructors(CSlicerRef)
			CSlicerRef(){}
			virtual ~CSlicerRef(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toBinTable();
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_SlicerRef;
			}
			//Attributes
			nullable<SimpleTypes::CRelationshipId> m_oRId;
		};

		class CSlicerStyle : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSlicerStyle)
            WritingElement_XlsbConstructors(CSlicerStyle)
			CSlicerStyle(){}
			virtual ~CSlicerStyle(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);
            virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
            virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const
			{
				return et_x_SlicerStyle;
			}
			//Attributes
			nullable_string m_oName;
			//Members
			std::vector<CSlicerStyleElement> m_oSlicerStyleElements;
		};

		class CSlicerCaches : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSlicerCaches)
            WritingElement_XlsbConstructors(CSlicerCaches)
			CSlicerCaches(){}
			virtual ~CSlicerCaches(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName, const std::wstring& sPrefix) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toBinTable();
            void fromBin(XLS::BaseObjectPtr& obj);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType() const
			{
				return et_x_SlicerCaches;
			}
			//Members
			std::vector<CSlicerCache> m_oSlicerCache;
		};

		class CSlicerRefs : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSlicerRefs)
            WritingElement_XlsbConstructors(CSlicerRefs)
			CSlicerRefs(){}
			virtual ~CSlicerRefs(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toBinTable();
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType() const
			{
				return et_x_SlicerRefs;
			}
			//Members
			std::vector<CSlicerRef> m_oSlicer;
		};

		class CSlicerStyles : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSlicerStyles)
            WritingElement_XlsbConstructors(CSlicerStyles)
			CSlicerStyles(){}
			virtual ~CSlicerStyles(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType() const
			{
				return et_x_SlicerStyles;
			}
			//Attributes
			nullable_string m_oDefaultSlicerStyle;
			//Members
			std::vector<CSlicerStyle> m_oSlicerStyle;
		};

		class CDrawingSlicer : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDrawingSlicer)
			CDrawingSlicer(){}
			virtual ~CDrawingSlicer(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSBinPptxRW::CXmlWriter& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_a_Slicer;
			}
			//Attributes
			nullable_string m_oName;
			//Members
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};

	} //Spreadsheet
} // namespace OOX
