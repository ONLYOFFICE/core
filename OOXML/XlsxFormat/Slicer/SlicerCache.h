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
		class COlapSlicerCacheItemParent : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COlapSlicerCacheItemParent)
			COlapSlicerCacheItemParent(){}
			virtual ~COlapSlicerCacheItemParent(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_OlapSlicerCacheItemParent;
			}
			//Attributes
			nullable_string m_oN;
		};

		class COlapSlicerCacheItem : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COlapSlicerCacheItem)
			COlapSlicerCacheItem(){}
			virtual ~COlapSlicerCacheItem(){}
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
				return et_x_OlapSlicerCacheItem;
			}
			//Attributes
			nullable_string m_oN;
			nullable_string m_oC;
			nullable_bool m_oNd;//False
			//Members
			std::vector<COlapSlicerCacheItemParent> m_oP;
		};

		class COlapSlicerCacheRange : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COlapSlicerCacheRange)
			COlapSlicerCacheRange(){}
			virtual ~COlapSlicerCacheRange(){}
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
				return et_x_OlapSlicerCacheRange;
			}
			//Attributes
			nullable_uint m_oStartItem;
			//Members
			std::vector<COlapSlicerCacheItem> m_oI;
		};

		class CTabularSlicerCacheItem : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTabularSlicerCacheItem)
			CTabularSlicerCacheItem(){}
			virtual ~CTabularSlicerCacheItem(){}
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
				return et_x_TabularSlicerCacheItem;
			}
			//Attributes
			nullable_uint m_oX;
			nullable_bool m_oS;//False
			nullable_bool m_oNd;//False
		};

		class COlapSlicerCacheSelection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COlapSlicerCacheSelection)
			COlapSlicerCacheSelection(){}
			virtual ~COlapSlicerCacheSelection(){}
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
				return et_x_OlapSlicerCacheSelection;
			}
			//Attributes
			nullable_string m_oN;
			//Members
			std::vector<COlapSlicerCacheItemParent> m_oP;
		};

		class COlapSlicerCacheLevelData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COlapSlicerCacheLevelData)
			COlapSlicerCacheLevelData(){}
			virtual ~COlapSlicerCacheLevelData(){}
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
				return et_x_OlapSlicerCacheLevelData;
			}
			//Attributes
			nullable_string m_oUniqueName;
			nullable_string m_oSourceCaption;
			nullable_uint m_oCount;
			nullable<SimpleTypes::Spreadsheet::COlapSlicerCacheSortOrder> m_oSortOrder;//natural
			nullable<SimpleTypes::Spreadsheet::CSlicerCacheCrossFilter> m_oCrossFilter;//showItemsWithDataAtTop
			//Members
			std::vector<COlapSlicerCacheRange> m_oRanges;
		};

		class CTabularSlicerCacheItems : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTabularSlicerCacheItems)
            WritingElement_XlsbConstructors(CTabularSlicerCacheItems)
			CTabularSlicerCacheItems(){}
			virtual ~CTabularSlicerCacheItems(){}
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
				return et_x_TabularSlicerCacheItems;
			}
			//Attributes
			nullable_uint m_oCount;
			//Members
			std::vector<CTabularSlicerCacheItem> m_oI;
		};

		class COlapSlicerCacheSelections : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COlapSlicerCacheSelections)
            WritingElement_XlsbConstructors(COlapSlicerCacheSelections)
			COlapSlicerCacheSelections(){}
			virtual ~COlapSlicerCacheSelections(){}
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
				return et_x_OlapSlicerCacheSelections;
			}
			//Attributes
			nullable_uint m_oCount;
			//Members
			std::vector<COlapSlicerCacheSelection> m_oSelection;
		};

		class COlapSlicerCacheLevelsData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COlapSlicerCacheLevelsData)
            WritingElement_XlsbConstructors(COlapSlicerCacheLevelsData)
			COlapSlicerCacheLevelsData(){}
			virtual ~COlapSlicerCacheLevelsData(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_OlapSlicerCacheLevelsData;
			}
			//Attributes
			nullable_uint m_oCount;
			//Members
			std::vector<COlapSlicerCacheLevelData> m_oLevel;
		};

		class CTabularSlicerCache : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTabularSlicerCache)
            WritingElement_XlsbConstructors(CTabularSlicerCache)
			CTabularSlicerCache(){}
			virtual ~CTabularSlicerCache(){}
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
				return et_x_TabularSlicerCache;
			}
			//Attributes
			nullable_uint m_oPivotCacheId;
			nullable<SimpleTypes::Spreadsheet::CTabularSlicerCacheSortOrder> m_oSortOrder;//ascending
			nullable_bool m_oCustomListSort;//True
			nullable_bool m_oShowMissing;//True
			nullable<SimpleTypes::Spreadsheet::CSlicerCacheCrossFilter> m_oCrossFilter;//showItemsWithDataAtTop
			//Members
			nullable<CTabularSlicerCacheItems> m_oItems;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};

		class COlapSlicerCache : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COlapSlicerCache)
            WritingElement_XlsbConstructors(COlapSlicerCache)
			COlapSlicerCache(){}
			virtual ~COlapSlicerCache(){}
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
				return et_x_OlapSlicerCache;
			}
			//Attributes
			nullable_uint m_oPivotCacheId;
			//Members
			nullable<COlapSlicerCacheLevelsData> m_oLevels;
			nullable<COlapSlicerCacheSelections> m_oSelections;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};

		class CSlicerCacheData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSlicerCacheData)
            WritingElement_XlsbConstructors(CSlicerCacheData)
			CSlicerCacheData(){}
			virtual ~CSlicerCacheData(){}
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
				return et_x_SlicerCacheData;
			}
			//Members
			nullable<COlapSlicerCache> m_oOlap;
			nullable<CTabularSlicerCache> m_oTabular;
		};

		class CSlicerCachePivotTable : public WritingElement
		{
		public:
            WritingElement_AdditionConstructors(CSlicerCachePivotTable)
			CSlicerCachePivotTable(){}
			virtual ~CSlicerCachePivotTable(){}
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
				return et_x_SlicerCachePivotTable;
			}
			//Attributes
			nullable_uint m_oTabId;
			nullable_string m_oName;
		};

		class CSlicerCacheDefinition : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSlicerCacheDefinition)
            WritingElement_XlsbConstructors(CSlicerCacheDefinition)
			CSlicerCacheDefinition(){}
			virtual ~CSlicerCacheDefinition(){}
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
				return et_x_SlicerCacheDefinition;
			}
			//Attributes
			nullable_string m_oName;
			nullable<SimpleTypes::CGuid> m_oUid;
			nullable_string m_oSourceName;
			//Members
			std::vector<CSlicerCachePivotTable> m_oPivotTables;
			nullable<CSlicerCacheData> m_oData;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};

		class CSlicerCacheFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CSlicerCacheFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
			}
			CSlicerCacheFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::
IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
            void readBin(const CPath& oPath);
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::SlicerCache;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}

			nullable<CSlicerCacheDefinition> m_oSlicerCacheDefinition;

		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};

	} //Spreadsheet
} // namespace OOX
