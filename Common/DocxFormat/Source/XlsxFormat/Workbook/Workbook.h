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

#include "../Xlsx.h"
#include "../XlsxFlat.h"
#include "../../XlsbFormat/Xlsb.h"

#include "../CommonInclude.h"

#include "BookViews.h"
#include "CalcPr.h"
#include "DefinedNames.h"
#include "Sheets.h"
#include "WorkbookPr.h"
#include "ExternalReferences.h"

#include "../../XlsbFormat/WorkBookStream.h"

#include "../../XlsbFormat/Biff12_unions/BOOKVIEWS.h"
#include "../../XlsbFormat/Biff12_unions/BUNDLESHS.h"
#include "../../XlsbFormat/Biff12_unions/EXTERNALS.h"
#include "../../XlsbFormat/Biff12_unions/PIVOTCACHEIDS.h"
#include "../../XlsbFormat/Biff12_unions/PIVOTCACHEID.h"
#include "../../XlsbFormat/Biff12_records/FileVersion.h"
#include "../../XlsbFormat/Biff12_records/BeginPivotCacheID.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CPersonList;
	//необработанные child:
		//<customWorkbookViews>
		//<extLst>
		//<fileRecoveryPr>
		//<fileSharing>
		//<fileVersion>
		//<functionGroups>
		//<smartTagPr>
		//<smartTagTypes>
		//<webPublishing>
		//<webPublishObjects>
		class CWorkbookPivotCache : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWorkbookPivotCache)
			WritingElement_XlsbConstructors(CWorkbookPivotCache)
			CWorkbookPivotCache()
			{
			}
			virtual ~CWorkbookPivotCache()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<pivotCache");
					WritingStringNullableAttrInt(L"cacheId", m_oCacheId, m_oCacheId->GetValue());
					WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::PIVOTCACHEID*>(obj.get());
                if(ptr != nullptr)
                {
                    ReadAttributes(ptr->m_BrtBeginPivotCacheID);
                }
            }
			virtual EElementType getType() const
			{
				return et_x_WorkbookPivotCache;
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::BeginPivotCacheID*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oCacheId = ptr->idSx;

                    if(!ptr->irstcacheRelID.value.value().empty())
                        m_oRid = ptr->irstcacheRelID.value.value();
                }
            }
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, _T("cacheId"), m_oCacheId)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("r:id"), m_oRid)
				WritingElement_ReadAttributes_End(oReader)

			}
	//----------
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oCacheId;
			nullable<SimpleTypes::CRelationshipId>			m_oRid;
		};

		class CWorkbookPivotCaches : public WritingElementWithChilds<CWorkbookPivotCache>
		{
		public:
			WritingElement_AdditionConstructors(CWorkbookPivotCaches)
            WritingElement_XlsbConstructors(CWorkbookPivotCaches)
			CWorkbookPivotCaches(OOX::Document *pMain = NULL) : WritingElementWithChilds<CWorkbookPivotCache>(pMain)
			{
			}
			virtual ~CWorkbookPivotCaches()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if (m_arrItems.empty()) return;

				writer.WriteString(L"<pivotCaches>");

				for (size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if (m_arrItems[i])
					{
						m_arrItems[i]->toXML(writer);
					}
				}

				writer.WriteString(L"</pivotCaches>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if (oReader.IsEmptyNode())
					return;

				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (L"pivotCaches" == sName)
					{
						CWorkbookPivotCache *pPivotCache = new CWorkbookPivotCache();
						m_arrItems.push_back(pPivotCache);

						pPivotCache->fromXML(oReader);
					}
				}
			}

            void fromBin(XLS::BaseObjectPtr& obj)
			{
                auto ptr = static_cast<XLSB::PIVOTCACHEIDS*>(obj.get());
                if(ptr != nullptr)
                {
                    for(auto &item : ptr->m_arPIVOTCACHEID)
                        m_arrItems.push_back(new CWorkbookPivotCache(item));
                }

			}

			virtual EElementType getType() const
			{
				return et_x_WorkbookPivotCaches;
			}
		};
//-----------------------------------------------------------------------------------------------------------
		class CWorkbook : public OOX::File, public OOX::IFileContainer, public WritingElement
		{
		public:
			CWorkbook(OOX::Document* pMain);
			CWorkbook(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CWorkbook();

			void readBin(const CPath& oPath);
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const
			{
				if (m_bMacroEnabled)	return OOX::Spreadsheet::FileTypes::WorkbookMacro;
				else					return OOX::Spreadsheet::FileTypes::Workbook;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			virtual EElementType getType () const
			{
				return et_x_Workbook;
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}
			void PrepareToWrite();
			LONG GetActiveSheetIndex();
			CPath											m_oReadPath;

			nullable<OOX::Spreadsheet::CWorkbookProtection>	m_oWorkbookProtection;
			nullable<OOX::Spreadsheet::CBookViews>			m_oBookViews;
            nullable<OOX::Spreadsheet::CDefinedNames>		m_oDefinedNames;
			nullable<OOX::Spreadsheet::CSheets>				m_oSheets;
			nullable<OOX::Spreadsheet::CWorkbookPr>			m_oWorkbookPr;
			nullable<OOX::Spreadsheet::CExternalReferences>	m_oExternalReferences;
            nullable<OOX::Spreadsheet::CCalcPr>				m_oCalcPr;
			nullable_string									m_oAppName;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable_string									m_oOleSize;
			nullable<OOX::Spreadsheet::CWorkbookPivotCaches>m_oPivotCaches;
			nullable<std::wstring>							m_oPivotCachesXml;
			
			CPersonList*									m_pPersonList;
			bool											m_bMacroEnabled;
		};
	} //Spreadsheet
} // namespace OOX

