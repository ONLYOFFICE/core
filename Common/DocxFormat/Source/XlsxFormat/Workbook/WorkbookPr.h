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
#ifndef OOX_WORKBOOKPR_FILE_INCLUDE_H_
#define OOX_WORKBOOKPR_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "../../XlsbFormat/Biff12_records/WbProp.h"
#include "../../XlsbFormat/Biff12_records/BookProtectionIso.h"
#include "../../XlsbFormat/Biff12_records/BookProtection.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CWorkbookPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CWorkbookPr)
            WritingElement_XlsbConstructors(CWorkbookPr)
            CWorkbookPr()
            {
            }
			virtual ~CWorkbookPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<workbookPr"));
				WritingStringNullableAttrBool(L"allowRefreshQuery", m_oAllowRefreshQuery);
				WritingStringNullableAttrBool(L"autoCompressPictures", m_oAutoCompressPictures);
				WritingStringNullableAttrBool(L"backupFile", m_oBackupFile);
				WritingStringNullableAttrBool(L"checkCompatibility", m_oCheckCompatibility);
				WritingStringNullableAttrEncodeXmlString(L"codeName", m_oCodeName, m_oCodeName.get());
				WritingStringNullableAttrBool(L"date1904", m_oDate1904);
				WritingStringNullableAttrBool(L"dateCompatibility", m_oDateCompatibility);
				WritingStringNullableAttrInt(L"defaultThemeVersion", m_oDefaultThemeVersion, m_oDefaultThemeVersion->GetValue());
				WritingStringNullableAttrBool(L"filterPrivacy", m_oFilterPrivacy);
				WritingStringNullableAttrBool(L"hidePivotFieldList", m_oHidePivotFieldList);
				WritingStringNullableAttrBool(L"promptedSolutions", m_oPromptedSolutions);
				WritingStringNullableAttrBool(L"publishItems", m_oPublishItems);
				WritingStringNullableAttrBool(L"refreshAllConnections", m_oRefreshAllConnections);
				WritingStringNullableAttrBool(L"showBorderUnselectedTables", m_oShowBorderUnselectedTables);
				WritingStringNullableAttrBool(L"showInkAnnotation", m_oShowInkAnnotation);
				WritingStringNullableAttrBool(L"showObjects", m_oShowObjects);
				WritingStringNullableAttrBool(L"showPivotChartFilter", m_oShowPivotChartFilter);
				WritingStringNullableAttrString(L"updateLinks", m_oUpdateLinks, m_oUpdateLinks->ToString());
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			void toBin(XLS::BaseObjectPtr& obj)
			{
				if (obj == nullptr)
					obj = XLS::BaseObjectPtr(new XLSB::WbProp());

				auto ptr = static_cast<XLSB::WbProp*>(obj.get());
				if (ptr != nullptr)
				{
					if (m_oAutoCompressPictures.IsInit())
						ptr->fAutoCompressPictures = m_oAutoCompressPictures->GetValue();
					else
						ptr->fAutoCompressPictures = true;

					if (m_oBackupFile.IsInit())
						ptr->fBackup = m_oBackupFile->GetValue();
					else
						ptr->fBackup = false;

					if (m_oCheckCompatibility.IsInit())
						ptr->fCheckCompat = m_oCheckCompatibility->GetValue();
					else
						ptr->fCheckCompat = false;

					if (m_oCodeName.IsInit())
						ptr->strName = m_oCodeName.get();
					else
						ptr->strName = std::wstring(L"");

					if (m_oDate1904.IsInit())
						ptr->f1904 = m_oDate1904->GetValue();
					else
						ptr->f1904 = false;

					if (m_oDefaultThemeVersion.IsInit())
						ptr->dwThemeVersion = m_oDefaultThemeVersion->GetValue();
					else
						ptr->dwThemeVersion = 0;

					if (m_oFilterPrivacy.IsInit())
						ptr->fFilterPrivacy = m_oFilterPrivacy->GetValue();
					else
						ptr->fFilterPrivacy = false;

					if (m_oHidePivotFieldList.IsInit())
						ptr->fHidePivotTableFList = m_oHidePivotFieldList->GetValue();
					else
						ptr->fHidePivotTableFList = false;

					if (m_oPromptedSolutions.IsInit())
						ptr->fBuggedUserAboutSolution = m_oPromptedSolutions->GetValue();
					else
						ptr->fBuggedUserAboutSolution = false;

					if (m_oPublishItems.IsInit())
						ptr->fPublishedBookItems = m_oPublishItems->GetValue();
					else
						ptr->fPublishedBookItems = false;

					if (m_oRefreshAllConnections.IsInit())
						ptr->fRefreshAll = m_oRefreshAllConnections->GetValue();
					else
						ptr->fRefreshAll = false;

					if (m_oShowBorderUnselectedTables.IsInit())
						ptr->fHideBorderUnselLists = m_oShowBorderUnselectedTables->GetValue();
					else
						ptr->fHideBorderUnselLists = true;

					if (m_oShowInkAnnotation.IsInit())
						ptr->fShowInkAnnotation = m_oShowInkAnnotation->GetValue();
					else
						ptr->fShowInkAnnotation = true;

					if (m_oShowObjects.IsInit())
						ptr->mdDspObj = m_oShowObjects->GetValue();
					else
						ptr->mdDspObj = 0;

					if (m_oShowPivotChartFilter.IsInit())
						ptr->fShowPivotChartFilter = m_oShowPivotChartFilter->GetValue();
					else
						ptr->fShowPivotChartFilter = false;

					if (m_oUpdateLinks.IsInit())
						ptr->grbitUpdateLinks = m_oUpdateLinks->GetValue();
					else
						ptr->grbitUpdateLinks = SimpleTypes::Spreadsheet::EUpdateLinksType::updatelinksUserSet;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_WorkbookPr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("allowRefreshQuery"),		m_oAllowRefreshQuery )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("autoCompressPictures"),	m_oAutoCompressPictures )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("backupFile"),				m_oBackupFile )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("checkCompatibility"),      m_oCheckCompatibility )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("codeName"),				m_oCodeName )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("date1904"),				m_oDate1904 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("dateCompatibility"),		m_oDateCompatibility )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("defaultThemeVersion"),		m_oDefaultThemeVersion )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("filterPrivacy"),			m_oFilterPrivacy )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("hidePivotFieldList"),		m_oHidePivotFieldList )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("promptedSolutions"),		m_oPromptedSolutions )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("publishItems"),			m_oPublishItems )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("refreshAllConnections"),	m_oRefreshAllConnections )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("showBorderUnselectedTables"),m_oShowBorderUnselectedTables )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("showInkAnnotation"),		m_oShowInkAnnotation )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("showObjects"),				m_oShowObjects )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("showPivotChartFilter"),	m_oShowPivotChartFilter )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("updateLinks"),				m_oUpdateLinks )
				WritingElement_ReadAttributes_End( oReader )
			}

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::WbProp*>(obj.get());
				if (ptr != nullptr)
				{
					//m_oAllowRefreshQuery = ptr->fNoSaveSup;//?
					if(ptr->fAutoCompressPictures != true)
						m_oAutoCompressPictures = ptr->fAutoCompressPictures;

					if (ptr->fBackup != false)
						m_oBackupFile = ptr->fBackup;

					if (ptr->fCheckCompat != false)
						m_oCheckCompatibility = ptr->fCheckCompat;

					m_oCodeName = ptr->strName.value.value();

					if (ptr->f1904 != false)
						m_oDate1904 = ptr->f1904;

					//m_oDateCompatibility = true;//ptr->fNoSaveSup?
					m_oDefaultThemeVersion = ptr->dwThemeVersion;

					if (ptr->fFilterPrivacy != false)
						m_oFilterPrivacy = ptr->fFilterPrivacy;

					if (ptr->fHidePivotTableFList != false)
						m_oHidePivotFieldList = ptr->fHidePivotTableFList;

					if (ptr->fBuggedUserAboutSolution != false)
						m_oPromptedSolutions = ptr->fBuggedUserAboutSolution;

					if (ptr->fPublishedBookItems != false)
						m_oPublishItems = ptr->fPublishedBookItems;

					if (ptr->fRefreshAll != false)
						m_oRefreshAllConnections = ptr->fRefreshAll;

					if (ptr->fHideBorderUnselLists != true)
						m_oShowBorderUnselectedTables = ptr->fHideBorderUnselLists;

					if (ptr->fShowInkAnnotation != true)
						m_oShowInkAnnotation = ptr->fShowInkAnnotation;

					if (ptr->mdDspObj != 0)
						m_oShowObjects = ptr->mdDspObj;

					if (ptr->fShowPivotChartFilter != false)
						m_oShowPivotChartFilter = ptr->fShowPivotChartFilter;

					if ((SimpleTypes::Spreadsheet::EUpdateLinksType)ptr->grbitUpdateLinks != SimpleTypes::Spreadsheet::EUpdateLinksType::updatelinksUserSet)
						m_oUpdateLinks = (SimpleTypes::Spreadsheet::EUpdateLinksType)ptr->grbitUpdateLinks;
				}


            }

		public:
				nullable<SimpleTypes::COnOff<>>						m_oAllowRefreshQuery;
				nullable<SimpleTypes::COnOff<>>						m_oAutoCompressPictures;
				nullable<SimpleTypes::COnOff<>>						m_oBackupFile;
				nullable<SimpleTypes::COnOff<>>						m_oCheckCompatibility;
				nullable_string					  					m_oCodeName;
				nullable<SimpleTypes::COnOff<>>						m_oDate1904;
				nullable<SimpleTypes::COnOff<>>						m_oDateCompatibility;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oDefaultThemeVersion;
				nullable<SimpleTypes::COnOff<>>						m_oFilterPrivacy;
				nullable<SimpleTypes::COnOff<>>						m_oHidePivotFieldList;
				nullable<SimpleTypes::COnOff<>>						m_oPromptedSolutions;
				nullable<SimpleTypes::COnOff<>>						m_oPublishItems;
				nullable<SimpleTypes::COnOff<>>						m_oRefreshAllConnections;
				nullable<SimpleTypes::COnOff<>>						m_oShowBorderUnselectedTables;
				nullable<SimpleTypes::COnOff<>>						m_oShowInkAnnotation;
				nullable<SimpleTypes::COnOff<>>						m_oShowObjects;
				nullable<SimpleTypes::COnOff<>>						m_oShowPivotChartFilter;
				nullable<SimpleTypes::Spreadsheet::CUpdateLinksType<>>						m_oUpdateLinks;
		};

		class CWorkbookProtection : public WritingElement
		{
		public:
            WritingElement_AdditionConstructors(CWorkbookProtection)
            WritingElement_XlsbConstructors(CWorkbookProtection)
			CWorkbookProtection()
			{
			}            
			virtual ~CWorkbookProtection()
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
				writer.WriteString(L"<workbookProtection");
					WritingStringNullableAttrString(L"workbookAlgorithmName", m_oWorkbookAlgorithmName, m_oWorkbookAlgorithmName->ToString());
					WritingStringNullableAttrString(L"workbookHashValue", m_oWorkbookHashValue, m_oWorkbookHashValue.get());
					WritingStringNullableAttrString(L"workbookSaltValue", m_oWorkbookSaltValue, m_oWorkbookSaltValue.get());
					WritingStringNullableAttrInt(L"workbookSpinCount", m_oWorkbookSpinCount, m_oWorkbookSpinCount->GetValue());
					WritingStringNullableAttrInt(L"lockStructure", m_oLockStructure, m_oLockStructure->ToBool() ? 1 : 0);
					WritingStringNullableAttrInt(L"lockWindows", m_oLockWindows, m_oLockWindows->ToBool() ? 1 : 0);
					WritingStringNullableAttrString(L"workbookPassword", m_oPassword, m_oPassword.get());
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}

            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			void toBin(XLS::BaseObjectPtr& obj)
			{
				if (obj == nullptr)
				{
					if (m_oWorkbookSpinCount.IsInit() || m_oRevisionsSpinCount.IsInit())
						obj = XLS::BaseObjectPtr(new XLSB::BookProtectionIso());
					else
						obj = XLS::BaseObjectPtr(new XLSB::BookProtection());
				}
				if (obj->get_type() == XLS::typeBookProtection)
				{
					auto ptr = static_cast<XLSB::BookProtection*>(obj.get());
					if (ptr != nullptr)
					{
						if (m_oLockRevision.IsInit())
							ptr->wFlags.fLockRevision = m_oLockRevision->GetValue();
						else
							ptr->wFlags.fLockRevision = false;

						if (m_oLockStructure.IsInit())
							ptr->wFlags.fLockStructure = m_oLockStructure->GetValue();
						else
							ptr->wFlags.fLockStructure = false;

						if (m_oLockWindows.IsInit())
							ptr->wFlags.fLockWindow = m_oLockWindows->GetValue();
						else
							ptr->wFlags.fLockWindow = false;
					}
				}
				else if (obj->get_type() == XLS::typeBookProtectionIso)
				{
					auto ptr = static_cast<XLSB::BookProtectionIso*>(obj.get());
					if (ptr != nullptr)
					{
						if (m_oLockRevision.IsInit())
							ptr->wFlags.fLockRevision = m_oLockRevision->GetValue();
						else
							ptr->wFlags.fLockRevision = false;

						if (m_oLockStructure.IsInit())
							ptr->wFlags.fLockStructure = m_oLockStructure->GetValue();
						else
							ptr->wFlags.fLockStructure = false;

						if (m_oLockWindows.IsInit())
							ptr->wFlags.fLockWindow = m_oLockWindows->GetValue();
						else
							ptr->wFlags.fLockWindow = false;

						if (m_oWorkbookAlgorithmName.IsInit())
							ptr->ipdBookPasswordData.szAlgName = m_oWorkbookAlgorithmName->ToString();
						else
							ptr->ipdBookPasswordData.szAlgName = L"";

						if (m_oWorkbookSpinCount.IsInit())
							ptr->dwBookSpinCount = m_oWorkbookSpinCount->GetValue();
						else
							ptr->dwBookSpinCount = 0;

						if (m_oWorkbookHashValue.IsInit() && !m_oWorkbookHashValue.get().empty())
						{
							BYTE const* p = reinterpret_cast<BYTE const*>(&m_oWorkbookHashValue.get()[0]);
							std::size_t size = m_oWorkbookHashValue.get().size() * sizeof(m_oWorkbookHashValue.get().front());
							ptr->ipdBookPasswordData.rgbHash.cbLength = size;
							ptr->ipdBookPasswordData.rgbHash.rgbData = std::vector<BYTE>(p, p + size);
						}

						if (m_oWorkbookSaltValue.IsInit() && !m_oWorkbookSaltValue.get().empty())
						{
							BYTE const* p = reinterpret_cast<BYTE const*>(&m_oWorkbookSaltValue.get()[0]);
							std::size_t size = m_oWorkbookSaltValue.get().size() * sizeof(m_oWorkbookSaltValue.get().front());
							ptr->ipdBookPasswordData.rgbSalt.cbLength = size;
							ptr->ipdBookPasswordData.rgbSalt.rgbData = std::vector<BYTE>(p, p + size);
						}

						if (m_oRevisionsAlgorithmName.IsInit())
							ptr->ipdRevPasswordData.szAlgName = m_oRevisionsAlgorithmName->ToString();
						else
							ptr->ipdRevPasswordData.szAlgName = L"";

						if (m_oRevisionsSpinCount.IsInit())
							ptr->dwRevSpinCount = m_oRevisionsSpinCount->GetValue();
						else
							ptr->dwRevSpinCount = 0;

						if (m_oRevisionsHashValue.IsInit() && !m_oRevisionsHashValue.get().empty())
						{
							BYTE const* p = reinterpret_cast<BYTE const*>(&m_oRevisionsHashValue.get()[0]);
							std::size_t size = m_oRevisionsHashValue.get().size() * sizeof(m_oRevisionsHashValue.get().front());
							ptr->ipdRevPasswordData.rgbHash.cbLength = size;
							ptr->ipdRevPasswordData.rgbHash.rgbData = std::vector<BYTE>(p, p + size);
						}

						if (m_oRevisionsSaltValue.IsInit() && !m_oRevisionsSaltValue.get().empty())
						{
							BYTE const* p = reinterpret_cast<BYTE const*>(&m_oRevisionsSaltValue.get()[0]);
							std::size_t size = m_oRevisionsSaltValue.get().size() * sizeof(m_oRevisionsSaltValue.get().front());
							ptr->ipdRevPasswordData.rgbSalt.cbLength = size;
							ptr->ipdRevPasswordData.rgbSalt.rgbData = std::vector<BYTE>(p, p + size);
						}
					}
				}

			}
			virtual EElementType getType() const
			{
				return et_x_WorkbookProtection;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, (L"workbookAlgorithmName"), m_oWorkbookAlgorithmName)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"workbookHashValue"), m_oWorkbookHashValue)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"workbookSaltValue"), m_oWorkbookSaltValue)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"workbookSpinCount"), m_oWorkbookSpinCount)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"revisionsAlgorithmName"), m_oRevisionsAlgorithmName)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"revisionsHashValue"), m_oRevisionsHashValue)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"revisionsSaltValue"), m_oRevisionsSaltValue)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"revisionsSpinCount"), m_oRevisionsSpinCount)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"lockRevision"), m_oLockRevision)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"lockStructure"), m_oLockStructure)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"lockWindows"), m_oLockWindows)
					WritingElement_ReadAttributes_Read_else_if(oReader, (L"workbookPassword"), m_oPassword)
				WritingElement_ReadAttributes_End(oReader)
			}

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {              
				if (obj->get_type() == XLS::typeBookProtection)
                {
                    auto ptr = static_cast<XLSB::BookProtection*>(obj.get());
					if (ptr != nullptr)
					{
						if (ptr->wFlags.fLockRevision != false)
							m_oLockRevision = ptr->wFlags.fLockRevision;

						if (ptr->wFlags.fLockStructure != false)
							m_oLockStructure = ptr->wFlags.fLockStructure;

						if (ptr->wFlags.fLockWindow != false)
							m_oLockWindows = ptr->wFlags.fLockWindow;
					}
                }
                else if (obj->get_type() == XLS::typeBookProtectionIso)
                {
                    auto ptr = static_cast<XLSB::BookProtectionIso*>(obj.get());
					if (ptr != nullptr)
					{
						if(ptr->wFlags.fLockRevision != false)
							m_oLockRevision = ptr->wFlags.fLockRevision;

						if (ptr->wFlags.fLockStructure != false)
							m_oLockStructure = ptr->wFlags.fLockStructure;

						if (ptr->wFlags.fLockWindow != false)
							m_oLockWindows = ptr->wFlags.fLockWindow;

						m_oWorkbookAlgorithmName = ptr->ipdBookPasswordData.szAlgName.value();
						m_oWorkbookSpinCount = ptr->dwBookSpinCount;
						m_oWorkbookHashValue = std::wstring(ptr->ipdBookPasswordData.rgbHash.rgbData.begin(),
							ptr->ipdBookPasswordData.rgbHash.rgbData.end());
						m_oWorkbookSaltValue = std::wstring(ptr->ipdBookPasswordData.rgbSalt.rgbData.begin(),
							ptr->ipdBookPasswordData.rgbSalt.rgbData.end());

						m_oRevisionsAlgorithmName = ptr->ipdRevPasswordData.szAlgName.value();
						m_oRevisionsSpinCount = ptr->dwRevSpinCount;
						m_oRevisionsHashValue = std::wstring(ptr->ipdRevPasswordData.rgbHash.rgbData.begin(),
							ptr->ipdRevPasswordData.rgbHash.rgbData.end());
						m_oRevisionsSaltValue = std::wstring(ptr->ipdRevPasswordData.rgbSalt.rgbData.begin(),
							ptr->ipdRevPasswordData.rgbSalt.rgbData.end());
					}
                }


            }

			nullable<SimpleTypes::COnOff<>>		m_oLockRevision;
			nullable<SimpleTypes::COnOff<>>		m_oLockStructure;
			nullable<SimpleTypes::COnOff<>>		m_oLockWindows;

			nullable<SimpleTypes::CCryptAlgoritmName<>>		m_oWorkbookAlgorithmName;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oWorkbookSpinCount;
			nullable_string									m_oWorkbookHashValue;
			nullable_string									m_oWorkbookSaltValue;

			nullable_string									m_oPassword; //for old wrike protection
			
			nullable<SimpleTypes::CCryptAlgoritmName<>>		m_oRevisionsAlgorithmName;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>> m_oRevisionsSpinCount;
			nullable_string									m_oRevisionsHashValue;
			nullable_string									m_oRevisionsSaltValue;			
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_WORKBOOKPR_FILE_INCLUDE_H_
