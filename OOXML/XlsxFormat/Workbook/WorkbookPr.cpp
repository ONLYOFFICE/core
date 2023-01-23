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

#include "WorkbookPr.h"

#include "../../XlsbFormat/Biff12_records/WbProp.h"
#include "../../XlsbFormat/Biff12_records/BookProtectionIso.h"
#include "../../XlsbFormat/Biff12_records/BookProtection.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CWorkbookPr::CWorkbookPr()
		{
		}
		CWorkbookPr::~CWorkbookPr()
		{
		}
		void CWorkbookPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CWorkbookPr::toXML() const
		{
			return _T("");
		}
		void CWorkbookPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<workbookPr"));
			WritingStringNullableAttrBool(L"allowRefreshQuery", m_oAllowRefreshQuery);
			WritingStringNullableAttrBool(L"autoCompressPictures", m_oAutoCompressPictures);
			WritingStringNullableAttrBool(L"backupFile", m_oBackupFile);
			WritingStringNullableAttrBool(L"checkCompatibility", m_oCheckCompatibility);
			WritingStringNullableAttrBool(L"codeName", m_oCodeName);
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
		void CWorkbookPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CWorkbookPr::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CWorkbookPr::getType () const
		{
			return et_x_WorkbookPr;
		}
		void CWorkbookPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
		void CWorkbookPr::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::WbProp*>(obj.get());
			m_oAllowRefreshQuery            = ptr->fNoSaveSup;//?
			m_oAutoCompressPictures         = ptr->fAutoCompressPictures;
			m_oBackupFile                   = ptr->fBackup;
			m_oCheckCompatibility           = ptr->fCheckCompat;
			m_oCodeName                     = ptr->strName.value.value();
			m_oDate1904                     = ptr->f1904;
			m_oDateCompatibility            = true;//ptr->fNoSaveSup?
			m_oDefaultThemeVersion          = ptr->dwThemeVersion;
			m_oFilterPrivacy                = ptr->fFilterPrivacy;
			m_oHidePivotFieldList           = ptr->fHidePivotTableFList;
			m_oPromptedSolutions            = ptr->fBuggedUserAboutSolution;
			m_oPublishItems                 = ptr->fPublishedBookItems;
			m_oRefreshAllConnections        = ptr->fRefreshAll;
			m_oShowBorderUnselectedTables   = ptr->fHideBorderUnselLists;
			m_oShowInkAnnotation            = ptr->fShowInkAnnotation;
			m_oShowObjects                  = ptr->mdDspObj == 2?false:true;
			m_oShowPivotChartFilter         = ptr->fShowPivotChartFilter;
			m_oUpdateLinks                  = (SimpleTypes::Spreadsheet::EUpdateLinksType)ptr->grbitUpdateLinks;


		}

		CWorkbookProtection::CWorkbookProtection()
		{
		}
		CWorkbookProtection::~CWorkbookProtection()
		{
		}
		void CWorkbookProtection::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CWorkbookProtection::toXML() const
		{
			return L"";
		}
		void CWorkbookProtection::toXML(NSStringUtils::CStringBuilder& writer) const
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
		void CWorkbookProtection::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
		void CWorkbookProtection::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CWorkbookProtection::getType() const
		{
			return et_x_WorkbookProtection;
		}
		void CWorkbookProtection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
		void CWorkbookProtection::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptrRecord = static_cast<XLS::BiffRecord*>(obj.get());

				if(ptrRecord->getTypeId() == XLSB::rt_BookProtection)
				{
					auto ptr = static_cast<XLSB::BookProtection*>(obj.get());
					m_oLockRevision            = ptr->wFlags.fLockRevision;
					m_oLockStructure           = ptr->wFlags.fLockStructure;
					m_oLockWindows             = ptr->wFlags.fLockWindow;
				}
				else if(ptrRecord->getTypeId() == XLSB::rt_BookProtectionIso)
				{
					auto ptr = static_cast<XLSB::BookProtectionIso*>(obj.get());
					m_oLockRevision            = ptr->wFlags.fLockRevision;
					m_oLockStructure           = ptr->wFlags.fLockStructure;
					m_oLockWindows             = ptr->wFlags.fLockWindow;

					m_oWorkbookAlgorithmName   = ptr->ipdBookPasswordData.szAlgName.value();
					m_oWorkbookSpinCount       = ptr->dwBookSpinCount;
					m_oWorkbookHashValue       = std::wstring(ptr->ipdBookPasswordData.rgbHash.rgbData.begin(),
															  ptr->ipdBookPasswordData.rgbHash.rgbData.end());
					m_oWorkbookSaltValue       = std::wstring(ptr->ipdBookPasswordData.rgbSalt.rgbData.begin(),
															  ptr->ipdBookPasswordData.rgbSalt.rgbData.end());

					m_oRevisionsAlgorithmName   = ptr->ipdRevPasswordData.szAlgName.value();
					m_oRevisionsSpinCount       = ptr->dwRevSpinCount;
					m_oRevisionsHashValue       = std::wstring(ptr->ipdRevPasswordData.rgbHash.rgbData.begin(),
															  ptr->ipdRevPasswordData.rgbHash.rgbData.end());
					m_oRevisionsSaltValue       = std::wstring(ptr->ipdRevPasswordData.rgbSalt.rgbData.begin(),
															  ptr->ipdRevPasswordData.rgbSalt.rgbData.end());
				}


			}

	} //Spreadsheet
} // namespace OOX
