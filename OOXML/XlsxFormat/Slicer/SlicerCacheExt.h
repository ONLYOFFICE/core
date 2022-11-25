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


namespace OOX
{
	namespace Spreadsheet
	{
		class CSlicerCacheOlapLevelName : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSlicerCacheOlapLevelName)
			CSlicerCacheOlapLevelName(){}
			virtual ~CSlicerCacheOlapLevelName(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BiffStructure& obj);
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
			WritingElement_AdditionConstructors(CSlicerCacheHideNoData)
            WritingElement_XlsbConstructors(CSlicerCacheHideNoData)
			CSlicerCacheHideNoData(){}
			virtual ~CSlicerCacheHideNoData(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
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
			WritingElement_AdditionConstructors(CTableSlicerCache)
            WritingElement_XlsbConstructors(CTableSlicerCache)
			CTableSlicerCache(){}
			virtual ~CTableSlicerCache(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
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
			WritingElement_AdditionConstructors(CSlicerStyleElement)
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
			WritingElement_AdditionConstructors(CSlicerCache)
            WritingElement_XlsbConstructors(CSlicerCache)
			CSlicerCache(){}
			virtual ~CSlicerCache(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
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
			WritingElement_AdditionConstructors(CSlicerRef)
            WritingElement_XlsbConstructors(CSlicerRef)
			CSlicerRef(){}
			virtual ~CSlicerRef(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
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
			WritingElement_AdditionConstructors(CSlicerStyle)
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
			WritingElement_AdditionConstructors(CSlicerCaches)
            WritingElement_XlsbConstructors(CSlicerCaches)
			CSlicerCaches(){}
			virtual ~CSlicerCaches(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName, const std::wstring& sPrefix) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
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
			WritingElement_AdditionConstructors(CSlicerRefs)
            WritingElement_XlsbConstructors(CSlicerRefs)
			CSlicerRefs(){}
			virtual ~CSlicerRefs(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
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
			WritingElement_AdditionConstructors(CSlicerStyles)
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
			WritingElement_AdditionConstructors(CDrawingSlicer)
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
