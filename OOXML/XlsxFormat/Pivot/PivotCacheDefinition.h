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
		class CSharedItems : public WritingElementWithChilds<WritingElement>
		{
		public:
			WritingElement_AdditionConstructors(CSharedItems)
            WritingElement_XlsbConstructors(CSharedItems)
			CSharedItems(){}
			virtual ~CSharedItems() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_SharedItems;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_bool		m_oContainsBlank;
			nullable_bool		m_oContainsDate;
			nullable_bool		m_oContainsInteger;
			nullable_bool		m_oContainsMixedTypes;
			nullable_bool		m_oContainsNonDate;
			nullable_bool		m_oContainsNumber;
			nullable_bool		m_oContainsSemiMixedTypes;
			nullable_bool		m_oContainsString;
			
			nullable_bool									m_oLongText;
			nullable_double									m_oMinValue;
			nullable_double									m_oMaxValue;
			nullable<SimpleTypes::CDateTime>				m_oMinDate;
			nullable<SimpleTypes::CDateTime>				m_oMaxDate;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
		};
		class COLAPGroupItems : public WritingElementWithChilds<WritingElement>
		{
		public:
			WritingElement_AdditionConstructors(COLAPGroupItems)
            WritingElement_XlsbConstructors(COLAPGroupItems)
			COLAPGroupItems(){}
			virtual ~COLAPGroupItems() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_OLAPGroupItems;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CDiscreteGroupingProperties : public WritingElementWithChilds<CSharedItemsIndex>
		{
		public:
			WritingElement_AdditionConstructors(CDiscreteGroupingProperties)
            WritingElement_XlsbConstructors(CDiscreteGroupingProperties)
			CDiscreteGroupingProperties(){}
			virtual ~CDiscreteGroupingProperties() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
                return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_DiscreteGroupingProperties;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CRangeGroupingProperties : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRangeGroupingProperties)
            WritingElement_XlsbConstructors(CRangeGroupingProperties)
			CRangeGroupingProperties(){}
			virtual ~CRangeGroupingProperties() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_RangeGroupingProperties;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::Spreadsheet::CValuesGroupBy> m_oGroupBy;

			nullable_bool						m_oAutoStart;
			nullable_bool						m_oAutoEnd;

			nullable<SimpleTypes::CDateTime>	m_oStartDate;
			nullable<SimpleTypes::CDateTime>	m_oEndDate;

			nullable_double						m_oStartNum;
			nullable_double						m_oEndNum;			
			nullable_double						m_oGroupInterval;
			
		};
		class CFieldGroupProperties : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFieldGroupProperties)
            WritingElement_XlsbConstructors(CFieldGroupProperties)
			CFieldGroupProperties(){}
			virtual ~CFieldGroupProperties() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_FieldGroupProperties;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oBase;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oPar;
			
			nullable<CDiscreteGroupingProperties>	m_oDiscretePr;
			nullable<CRangeGroupingProperties>		m_oRangePr;
			nullable<COLAPGroupItems>				m_oGroupItems;
		};
		class CPivotCacheField : public WritingElement
		{
        public:
			WritingElement_AdditionConstructors(CPivotCacheField)
            WritingElement_XlsbConstructors(CPivotCacheField)
			CPivotCacheField(){}
			virtual ~CPivotCacheField() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotCacheField;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string		m_oName;
			nullable_string		m_oCaption;
			nullable_bool		m_oDatabaseField;
			nullable_bool		m_oServerField;
			nullable_string		m_oFormula;
			nullable_int		m_oHierarchy;
			nullable_bool		m_oMemberPropertyField;
			nullable_string		m_oPropertyName;
			nullable_int		m_oSqlType;
			nullable_bool		m_oUniqueList;

			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oLevel;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oMappingCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oNumFmtId;
			
			nullable<CSharedItems>							m_oSharedItems;
			//nullable<CMemberPropertiesMap>				m_oMpMap;
			nullable<CFieldGroupProperties>					m_oFieldGroup;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
		class CPivotCacheFields : public WritingElementWithChilds<CPivotCacheField>
		{
		public:
			WritingElement_AdditionConstructors(CPivotCacheFields)
            WritingElement_XlsbConstructors(CPivotCacheFields)
			CPivotCacheFields(){}
			virtual ~CPivotCacheFields() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotCacheFields;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CRangeSet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRangeSet)
            WritingElement_XlsbConstructors(CRangeSet)
			CRangeSet(){}
			virtual ~CRangeSet() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_RangeSet;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string	m_oSheet;
			nullable_string	m_oRef;
			nullable_string	m_oName;
			nullable<SimpleTypes::CRelationshipId>			m_oRid;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oI1;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oI2;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oI3;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oI4;
		};
		class CRangeSets : public WritingElementWithChilds<CRangeSet>
		{
		public:
			WritingElement_AdditionConstructors(CRangeSets)
            WritingElement_XlsbConstructors(CRangeSets)
			CRangeSets(){}
			virtual ~CRangeSets() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_RangeSets;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CPageItem : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPageItem)
            WritingElement_XlsbConstructors(CPageItem)
			CPageItem(){}
			virtual ~CPageItem() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PageItem;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string	m_oName;
		};
		class CPageItems : public WritingElementWithChilds<CPageItem>
		{
		public:
			WritingElement_AdditionConstructors(CPageItems)
            WritingElement_XlsbConstructors(CPageItems)
			CPageItems(){}
			virtual ~CPageItems() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PageItems;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};
		class CPageItemValues : public WritingElementWithChilds<CPageItems>
		{
		public:
			WritingElement_AdditionConstructors(CPageItemValues)
            WritingElement_XlsbConstructors(CPageItemValues)
			CPageItemValues(){}
			virtual ~CPageItemValues() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PageItemValues;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};

		class CConsolidationSource : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CConsolidationSource)
            WritingElement_XlsbConstructors(CConsolidationSource)
			CConsolidationSource()
			{
			}
			virtual ~CConsolidationSource()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_ConsolidationSource;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);		
	//----------
			nullable_bool				m_oAutoPage;

			nullable<CPageItemValues>	m_oPages;
			nullable<CRangeSets>		m_oRangeSets;
		};
		class CWorksheetSource : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWorksheetSource)
            WritingElement_XlsbConstructors(CWorksheetSource)
			CWorksheetSource(){}
			virtual ~CWorksheetSource() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_WorksheetSource;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string							m_oSheet;
			nullable_string							m_oRef;
			nullable_string							m_oName;
			nullable<SimpleTypes::CRelationshipId>	m_oRid;
		};
		class CPivotCacheSource : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPivotCacheSource)
            WritingElement_XlsbConstructors(CPivotCacheSource)
			CPivotCacheSource()
			{
			}
			virtual ~CPivotCacheSource()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotCacheSource;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);		
	//----------
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oConnectionId;
			nullable<SimpleTypes::Spreadsheet::CSourceCacheType>	m_oType;

			nullable<CConsolidationSource>							m_oConsolidation;
			nullable<CWorksheetSource>								m_oWorksheetSource;
			nullable<OOX::Drawing::COfficeArtExtensionList>			m_oExtLst;
		};        
		class CPivotCacheDefinition : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPivotCacheDefinition)
            WritingElement_XlsbConstructors(CPivotCacheDefinition)
			CPivotCacheDefinition()
			{
			}
			virtual ~CPivotCacheDefinition()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotCacheDefinition;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);		
	//----------
			nullable_bool									m_oBackgroundQuery;
			nullable_bool									m_oEnableRefresh;
			nullable<SimpleTypes::CRelationshipId>			m_oRid;
			nullable_bool									m_oInvalid;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCreatedVersion;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oMinRefreshableVersion;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oMissingItemsLimit;
			nullable_bool									m_oOptimizeMemory;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oRecordCount;
			nullable_string									m_oRefreshedBy;
			nullable<SimpleTypes::CDateTime>				m_oRefreshedDateIso;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oRefreshedVersion;
			nullable_bool									m_oRefreshOnLoad;
			nullable_bool									m_oSaveData;
			nullable_bool									m_oSupportAdvancedDrill;
			nullable_bool									m_oSupportSubquery;
			nullable_bool									m_oTupleCache;
			nullable_bool									m_oUpgradeOnRefresh;

			nullable<CPivotCacheFields>		m_oCacheFields;
			nullable<CPivotCacheSource>		m_oCacheSource;
//calculatedItems (Calculated Items) §18.10.1.9
//calculatedMembers (Calculated Members) §18.10.1.11
//cacheHierarchies (PivotCache Hierarchies) §18.10.1.5
//dimensions (OLAP Dimensions) §18.10.1.25
//kpis (OLAP KPIs) §18.10.1.48
//maps (OLAP Measure Group) §18.10.1.52
//measureGroups (OLAP Measure Groups) §18.10.1.54
//tupleCache (Tuple Cache)
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
		class CPivotCacheDefinitionFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CPivotCacheDefinitionFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				
				m_pData = NULL;
				m_nDataLength = 0;
               
				bIsWritten = false;
			}
			CPivotCacheDefinitionFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;

				m_pData = NULL;
				m_nDataLength = 0;

                bIsWritten = false;

				read( oRootPath, oPath );
			}
			virtual ~CPivotCacheDefinitionFile()
			{
				m_nDataLength = 0;
				RELEASEARRAYOBJECTS(m_pData)
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
           void setData(BYTE* pData, long length, const std::wstring& srIdRecords)
            {
                if(srIdRecords.length() > 0)
                {
                    const std::string srIdRecordsA( srIdRecords.begin(), srIdRecords.end() );
                    std::string rIdAttr = " r:id=\""+ srIdRecordsA +"\"";
                    m_nDataLength = length + (long)rIdAttr.length();
                   
					m_pData = new BYTE[m_nDataLength];
                   
					long nTreshold = 220;
                    memcpy(m_pData, pData, nTreshold);
                    memcpy(m_pData + nTreshold, rIdAttr.c_str(), rIdAttr.length());
                    memcpy(m_pData + nTreshold + rIdAttr.length(), pData + nTreshold, length - nTreshold);
                }
                else
                {
                    m_nDataLength = length;
                    m_pData = new BYTE[m_nDataLength];
                    memcpy(m_pData, pData, length);
                }
            }
            void readBin(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::PivotCacheDefinition;
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

			nullable<CPivotCacheDefinition> m_oPivotCashDefinition;
		private:
			CPath m_oReadPath;
			
			BYTE *m_pData;
			long m_nDataLength;

			mutable bool bIsWritten;
		};

		class CPivotCharacterValue : public WritingElementWithChilds<CMemberPropertyIndex>
		{
		public:
			WritingElement_AdditionConstructors(CPivotCharacterValue)
            WritingElement_XlsbConstructors(CPivotCharacterValue)
			CPivotCharacterValue(){}
			virtual ~CPivotCharacterValue() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotCharacterValue;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			
			nullable_string		m_oValue;
			nullable_string		m_oCaption;
			nullable_bool		m_oCalculated;
			nullable_bool		m_oUnused;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
			
			nullable_bool									m_oBold;
			nullable_bool									m_oItalic;
			nullable_bool									m_oStrike;
			nullable_bool									m_oUnderline;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oBackColor;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oForeColor;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFormatIndex;

			//tpls
		};	
		class CPivotBooleanValue : public WritingElementWithChilds<CMemberPropertyIndex>
		{
		public:
			WritingElement_AdditionConstructors(CPivotBooleanValue)
            WritingElement_XlsbConstructors(CPivotBooleanValue)
			CPivotBooleanValue(){}
			virtual ~CPivotBooleanValue() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotBooleanValue;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

			nullable_bool		m_oValue;
			nullable_string		m_oCaption;
			nullable_bool		m_oCalculated;
			nullable_bool		m_oUnused;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
		};		
		class CPivotNumericValue : public WritingElementWithChilds<CMemberPropertyIndex>
		{
		public:
			WritingElement_AdditionConstructors(CPivotNumericValue)
            WritingElement_XlsbConstructors(CPivotNumericValue)
			CPivotNumericValue(){}
			virtual ~CPivotNumericValue() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotNumericValue;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			
			nullable_double		m_oValue;
			nullable_string		m_oCaption;
			nullable_bool		m_oCalculated;
			nullable_bool		m_oUnused;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
			
			nullable_bool									m_oBold;
			nullable_bool									m_oItalic;
			nullable_bool									m_oStrike;
			nullable_bool									m_oUnderline;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oBackColor;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oForeColor;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFormatIndex;

			//tpls
		};		
		class CPivotDateTimeValue : public WritingElementWithChilds<CMemberPropertyIndex>
		{
		public:
			WritingElement_AdditionConstructors(CPivotDateTimeValue)
            WritingElement_XlsbConstructors(CPivotDateTimeValue)
			CPivotDateTimeValue(){}
			virtual ~CPivotDateTimeValue() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotBooleanValue;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			
			nullable<SimpleTypes::CDateTime>	m_oValue;
			nullable_string						m_oCaption;
			nullable_bool						m_oCalculated;
			nullable_bool						m_oUnused;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
		};
		class CPivotErrorValue : public WritingElementWithChilds<CMemberPropertyIndex>
		{
		public:
			WritingElement_AdditionConstructors(CPivotErrorValue)
            WritingElement_XlsbConstructors(CPivotErrorValue)
			CPivotErrorValue(){}
			virtual ~CPivotErrorValue() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotErrorValue;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			
			nullable_string		m_oValue;
			nullable_string		m_oCaption;
			nullable_bool		m_oCalculated;
			nullable_bool		m_oUnused;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
			
			nullable_bool									m_oBold;
			nullable_bool									m_oItalic;
			nullable_bool									m_oStrike;
			nullable_bool									m_oUnderline;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oBackColor;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oForeColor;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFormatIndex;

			//tpls
		};	
		class CPivotNoValue : public WritingElementWithChilds<CMemberPropertyIndex>
		{
		public:
			WritingElement_AdditionConstructors(CPivotNoValue)
            WritingElement_XlsbConstructors(CPivotNoValue)
			CPivotNoValue(){}
			virtual ~CPivotNoValue() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const
			{
				return et_x_PivotNoValue;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			
			nullable_string		m_oCaption;
			nullable_bool		m_oCalculated;
			nullable_bool		m_oUnused;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
			
			nullable_bool									m_oBold;
			nullable_bool									m_oItalic;
			nullable_bool									m_oStrike;
			nullable_bool									m_oUnderline;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oBackColor;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oForeColor;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFormatIndex;

			//tpls
		};		
		
	} //Spreadsheet
} // namespace OOX
