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

#include "WorkbookPr.h"

#include "../../XlsbFormat/Biff12_records/WbProp.h"
#include "../../XlsbFormat/Biff12_records/BookProtectionIso.h"
#include "../../XlsbFormat/Biff12_records/BookProtection.h"
#include "../../XlsbFormat/Biff12_records/FileSharingIso.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/PROTECTION.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/WinProtect.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Protect.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Password.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Prot4Rev.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Prot4RevPass.h"

#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

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
			WritingStringNullableAttrString(L"codeName", m_oCodeName, *m_oCodeName);
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
			WritingStringNullableAttrString(L"showObjects", m_oShowObjects, m_oShowObjects->ToBool() ? L"all" : L"none");
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
		XLS::BaseObjectPtr CWorkbookPr::toBin()
		{
			auto ptr(new XLSB::WbProp);
			XLS::BaseObjectPtr objectPtr(ptr);

			if(m_oAllowRefreshQuery.IsInit())
				ptr->fNoSaveSup = m_oAllowRefreshQuery->GetValue();
            else
                ptr->fNoSaveSup = false;
			if(m_oAutoCompressPictures.IsInit())
                ptr->fAutoCompressPictures = m_oAutoCompressPictures->GetValue();
			if(m_oBackupFile.IsInit())
                ptr->fBackup = m_oBackupFile->GetValue();
            else
                ptr->fBackup = false;
			if(m_oCheckCompatibility.IsInit())
                ptr->fCheckCompat = m_oCheckCompatibility->GetValue();
            else
                ptr->fCheckCompat = false;
			if(m_oCodeName.IsInit())
				ptr->strName.value = *m_oCodeName;
			else
				ptr->strName.value = false;
			if(m_oDate1904.IsInit())
                ptr->f1904 = m_oDate1904->GetValue();
            else
                ptr->f1904 = false;
			if(m_oDateCompatibility.IsInit())
                ptr->fNoSaveSup = m_oDateCompatibility->GetValue();
            else
                ptr->fNoSaveSup = false;
			if(m_oDefaultThemeVersion.IsInit())
				ptr->dwThemeVersion = m_oDefaultThemeVersion->GetValue();
			else
				ptr->dwThemeVersion = 0;
			if(m_oFilterPrivacy.IsInit())
                ptr->fFilterPrivacy = m_oFilterPrivacy->GetValue();
            else
                ptr->fFilterPrivacy = false;
			if(m_oHidePivotFieldList.IsInit())
                ptr->fHidePivotTableFList = m_oHidePivotFieldList->GetValue();
            else
                ptr->fHidePivotTableFList = false;
			if(m_oPromptedSolutions.IsInit())
                ptr->fBuggedUserAboutSolution = m_oPromptedSolutions->GetValue();
            else
                ptr->fBuggedUserAboutSolution = false;
			if(m_oPublishItems.IsInit())
                ptr->fPublishedBookItems = m_oPublishItems->GetValue();
            else
                ptr->fPublishedBookItems = false;
			if(m_oRefreshAllConnections.IsInit())
                ptr->fRefreshAll = m_oRefreshAllConnections->GetValue();
            else
                ptr->fRefreshAll = false;
			if(m_oShowBorderUnselectedTables.IsInit())
                ptr->fHideBorderUnselLists = m_oShowBorderUnselectedTables->GetValue();
            else
                ptr->fHideBorderUnselLists = false;
			if(m_oShowInkAnnotation.IsInit())
                ptr->fShowInkAnnotation = m_oShowInkAnnotation->GetValue();
			if(m_oShowObjects.IsInit())
			ptr->mdDspObj = m_oShowObjects->GetValue() ? 1 : 2;
            else
                ptr->mdDspObj = 0;
			if(m_oShowPivotChartFilter.IsInit())
                ptr->fShowPivotChartFilter = m_oShowPivotChartFilter->GetValue();
            else
                ptr->fShowPivotChartFilter = false;
			if(m_oUpdateLinks.IsInit())
                ptr->grbitUpdateLinks = m_oUpdateLinks->GetValue();
            else
                ptr->grbitUpdateLinks = 0;

			return objectPtr;
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
		XLS::BaseObjectPtr CWorkbookProtection::toBin()
		{
			XLS::BaseObjectPtr objectPtr;
			if(m_oWorkbookAlgorithmName.IsInit() || m_oWorkbookSpinCount.IsInit() || m_oRevisionsAlgorithmName.IsInit() ||
				m_oRevisionsSpinCount.IsInit())
			{
				auto ptr(new XLSB::BookProtectionIso);
				objectPtr = XLS::BaseObjectPtr{ptr};

				if(m_oLockRevision.IsInit())
					ptr->wFlags.fLockRevision = m_oLockRevision->GetValue();
				else
					ptr->wFlags.fLockRevision = false;
				if(m_oLockStructure.IsInit())
					ptr->wFlags.fLockStructure = m_oLockStructure->GetValue();
				else
					ptr->wFlags.fLockStructure = false;
				if(m_oLockWindows.IsInit())
					ptr->wFlags.fLockWindow = m_oLockWindows->GetValue();
				else
					ptr->wFlags.fLockWindow = false;
				if(m_oWorkbookAlgorithmName.IsInit())
					ptr->ipdBookPasswordData.szAlgName = m_oWorkbookAlgorithmName->GetValue();
				else
					ptr->ipdBookPasswordData.szAlgName.setSize(0xFFFFFFFF);
				if(m_oWorkbookSpinCount.IsInit())
					ptr->dwBookSpinCount = m_oWorkbookSpinCount->GetValue();
				else
					ptr->dwBookSpinCount = 100000;

				if(m_oWorkbookHashValue.IsInit())
				{
					auto len = 0;
					auto bytes = ptr->ipdBookPasswordData.rgbHash.rgbData.data();
					std::string strData = {m_oWorkbookHashValue.get().begin(), m_oWorkbookHashValue.get().end()};
					NSFile::CBase64Converter::Decode(strData.c_str(), strData.size(),
						bytes, len);
					ptr->ipdBookPasswordData.rgbHash.cbLength = len;
				}

				if(m_oWorkbookSaltValue.IsInit())
				{
					auto len1 = 0;
					auto bytes1 = ptr->ipdBookPasswordData.rgbHash.rgbData.data();
					std::string strData1 = {m_oWorkbookSaltValue.get().begin(), m_oWorkbookSaltValue.get().end()};
					NSFile::CBase64Converter::Decode(strData1.c_str(), strData1.size(),
						bytes1, len1);
					ptr->ipdBookPasswordData.rgbSalt.cbLength = len1;
				}
				if(m_oRevisionsAlgorithmName.IsInit())
					ptr->ipdRevPasswordData.szAlgName = m_oRevisionsAlgorithmName->GetValue();
				else
					ptr->ipdRevPasswordData.szAlgName.setSize(0xFFFFFFFF);
				if(m_oRevisionsSpinCount.IsInit())
					ptr->dwRevSpinCount = m_oRevisionsSpinCount->GetValue();
				else
					ptr->dwRevSpinCount = 100000;

				if(m_oRevisionsHashValue.IsInit())
				{
					auto len2 = 0;
					auto bytes2 = ptr->ipdRevPasswordData.rgbHash.rgbData.data();
					std::string strData2 = {m_oRevisionsHashValue.get().begin(), m_oRevisionsHashValue.get().end()};
					NSFile::CBase64Converter::Decode(strData2.c_str(), strData2.size(),
						bytes2, len2);
					ptr->ipdRevPasswordData.rgbHash.cbLength = len2;
				}
				if(m_oRevisionsSaltValue.IsInit())
				{
					auto len3 = 0;
					auto bytes3 = ptr->ipdRevPasswordData.rgbSalt.rgbData.data();
					std::string strData3 = {m_oRevisionsSaltValue.get().begin(), m_oRevisionsSaltValue.get().end()};
					NSFile::CBase64Converter::Decode(strData3.c_str(), strData3.size(),
						bytes3, len3);
					ptr->ipdRevPasswordData.rgbSalt.cbLength = len3;
				}
			}
			else
			{
				auto ptr(new XLSB::BookProtection);
				objectPtr = XLS::BaseObjectPtr{ptr};
				if(m_oLockRevision.IsInit())
				ptr->wFlags.fLockRevision = m_oLockRevision->GetValue();
				if(m_oLockStructure.IsInit())
				ptr->wFlags.fLockStructure = m_oLockStructure->GetValue();
				if(m_oLockWindows.IsInit())
				ptr->wFlags.fLockWindow = m_oLockWindows->GetValue();
			}
			return objectPtr;
		}
		XLS::BaseObjectPtr CWorkbookProtection::toXLS()
		{
			auto ptr = new XLS::PROTECTION;
			if(m_oLockWindows.IsInit())
			{
				auto winPtr = new XLS::WinProtect;
				winPtr->fLockWn = m_oLockWindows->GetValue();
				ptr->m_WinProtect = XLS::BaseObjectPtr(winPtr);
			}
			if(m_oLockStructure.IsInit())
			{
				auto protPtr = new XLS::Protect;
				protPtr->fLock = m_oLockStructure->GetValue();
				ptr->m_Protect = XLS::BaseObjectPtr(protPtr);
			}
			if(m_oPassword.IsInit())
			{
				auto pass = new XLS::Password;
				pass->wPassword = m_oPassword.get();
				ptr->m_Password = XLS::BaseObjectPtr(pass);
			}
			if(m_oLockRevision.IsInit() && m_oPassword.IsInit())
			{
				auto rev = new XLS::Prot4Rev;
				rev->fRevLock = m_oLockRevision->GetValue();
				ptr->m_Prot4Rev = XLS::BaseObjectPtr(rev);
				auto revPass = new XLS::Prot4RevPass;
				revPass->protPwdRev = m_oPassword.get();
				ptr->m_Prot4RevPass = XLS::BaseObjectPtr(revPass);
			}
			return XLS::BaseObjectPtr(ptr);
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

			if (ptrRecord->getTypeId() == XLSB::rt_BookProtection)
			{
				auto ptr = static_cast<XLSB::BookProtection*>(obj.get());
				m_oLockRevision = ptr->wFlags.fLockRevision;
				m_oLockStructure = ptr->wFlags.fLockStructure;
				m_oLockWindows = ptr->wFlags.fLockWindow;
			}
			else if (ptrRecord->getTypeId() == XLSB::rt_BookProtectionIso)
			{
				auto ptr = static_cast<XLSB::BookProtectionIso*>(obj.get());
				m_oLockRevision = ptr->wFlags.fLockRevision;
				m_oLockStructure = ptr->wFlags.fLockStructure;
				m_oLockWindows = ptr->wFlags.fLockWindow;

				m_oWorkbookAlgorithmName = ptr->ipdBookPasswordData.szAlgName.value();
				m_oWorkbookSpinCount = ptr->dwBookSpinCount;
				m_oWorkbookHashValue = ptr->ipdBookPasswordData.rgbHash.GetBase64();
				m_oWorkbookSaltValue = ptr->ipdBookPasswordData.rgbSalt.GetBase64();

				m_oRevisionsAlgorithmName = ptr->ipdRevPasswordData.szAlgName.value();
				m_oRevisionsSpinCount = ptr->dwRevSpinCount;
				m_oRevisionsHashValue = ptr->ipdRevPasswordData.rgbHash.GetBase64();
				m_oRevisionsSaltValue = ptr->ipdRevPasswordData.rgbSalt.GetBase64();
			}
		}
		CFileSharing::CFileSharing()
		{
		}
		CFileSharing::~CFileSharing()
		{
		}
		void CFileSharing::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CFileSharing::toXML() const
		{
			return L"";
		}
		void CFileSharing::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<fileSharing");
			WritingStringNullableAttrEncodeXmlString2(L"userName", m_oUserName);
			WritingStringNullableAttrBool2(L"readOnlyRecommended", m_oReadOnlyRecommended);
			WritingStringNullableAttrString(L"algorithmName", m_oAlgorithmName, m_oAlgorithmName->ToString());
			WritingStringNullableAttrString(L"hashValue", m_oHashValue, m_oHashValue.get());
			WritingStringNullableAttrString(L"saltValue", m_oSaltValue, m_oSaltValue.get());
			WritingStringNullableAttrInt(L"spinCount", m_oSpinCount, m_oSpinCount->GetValue());
			WritingStringNullableAttrString(L"password", m_oPassword, m_oPassword.get());
			writer.WriteString(L"/>");
		}
		void CFileSharing::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (!oReader.IsEmptyNode())
				oReader.ReadTillEnd();
		}
        std::vector<XLS::BaseObjectPtr> CFileSharing::toBin()
		{
            std::vector<XLS::BaseObjectPtr> objectVector;
			if(m_oSpinCount.IsInit() || m_oAlgorithmName.IsInit() || m_oHashValue.IsInit())
			{
				auto ptr(new XLSB::FileSharingIso);
                XLS::BaseObjectPtr objectPtr = XLS::BaseObjectPtr{ptr};

				if (m_oReadOnlyRecommended.IsInit())
					ptr->fReadOnlyRec = m_oReadOnlyRecommended.get();
				else
					ptr->fReadOnlyRec = false;
                if(m_oUserName.IsInit())
                    ptr->stUserName = m_oUserName.get();
                else
                    ptr->stUserName = L"";
                if(m_oAlgorithmName.IsInit())
                    ptr->ipdPasswordData.szAlgName = m_oAlgorithmName->GetValue();
                else
                    ptr->ipdPasswordData.szAlgName = L"";
                if(m_oSpinCount.IsInit())
                    ptr->dwSpinCount = m_oSpinCount->GetValue();
                else
                    ptr->dwSpinCount = 0;

				auto len = 0;
                if(m_oHashValue.IsInit())
                {
                    auto bytes = ptr->ipdPasswordData.rgbHash.rgbData.data();
                    std::string strData = {m_oHashValue.get().begin(), m_oHashValue.get().end()};
                    NSFile::CBase64Converter::Decode(strData.c_str(), strData.size(),
                        bytes, len);
                }
				ptr->ipdPasswordData.rgbHash.cbLength = len;

				auto len1 = 0;
                if(m_oSaltValue.IsInit())
                {
                    auto bytes1 = ptr->ipdPasswordData.rgbSalt.rgbData.data();
                    std::string strData1 = {m_oSaltValue.get().begin(), m_oSaltValue.get().end()};
                    NSFile::CBase64Converter::Decode(strData1.c_str(), strData1.size(),
                        bytes1, len1);
                }
				ptr->ipdPasswordData.rgbSalt.cbLength = len1;
                auto ptr1(new XLSB::FileSharing);
                ptr1->wResPass = L"";
                ptr1->fReadOnlyRec = ptr->fReadOnlyRec;
                ptr1->stUserName =  ptr->stUserName;
                objectVector.push_back(objectPtr);
                objectVector.push_back(XLS::BaseObjectPtr{ptr1});

			}
			else
			{
				auto ptr(new XLSB::FileSharing);
                XLS::BaseObjectPtr objectPtr = XLS::BaseObjectPtr{ptr};
				if (m_oReadOnlyRecommended.IsInit())
					ptr->fReadOnlyRec = m_oReadOnlyRecommended.get();
                else
                    ptr->fReadOnlyRec = false;
                if(m_oUserName.IsInit())
                    ptr->stUserName = m_oUserName.get();
                else
                    ptr->stUserName.setSize(0xFFFFFFFF);
                if(m_oPassword.IsInit())
                    ptr->wResPass = m_oPassword.get();
                else
                    ptr->wResPass = L"";
                objectVector.push_back(objectPtr);
			}
            return objectVector;
		}
		void CFileSharing::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CFileSharing::getType() const
		{
			return et_x_FileSharing;
		}
		void CFileSharing::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, (L"algorithmName"), m_oAlgorithmName)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"hashValue"), m_oHashValue)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"saltValue"), m_oSaltValue)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"spinCount"), m_oSpinCount)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"password"), m_oPassword)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"userName"), m_oUserName)
				WritingElement_ReadAttributes_Read_else_if(oReader, (L"readOnlyRecommended"), m_oReadOnlyRecommended)
			WritingElement_ReadAttributes_End(oReader)
		}
		void CFileSharing::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptrRecord = static_cast<XLS::BiffRecord*>(obj.get());

			if (ptrRecord->getTypeId() == XLSB::rt_FileSharing)
			{
				auto ptr = static_cast<XLSB::FileSharing*>(obj.get());
				if (ptr->fReadOnlyRec.value())
					m_oReadOnlyRecommended = *ptr->fReadOnlyRec.value();
				m_oUserName = ptr->stUserName.value();
				m_oPassword = ptr->wResPass;
			}
			else if (ptrRecord->getTypeId() == XLSB::rt_FileSharingIso)
			{
				auto ptr = static_cast<XLSB::FileSharingIso*>(obj.get());
				m_oUserName = ptr->stUserName.value();
				if (ptr->fReadOnlyRec.value())
					m_oReadOnlyRecommended = *ptr->fReadOnlyRec.value();

				m_oSpinCount = ptr->dwSpinCount;
				m_oAlgorithmName = ptr->ipdPasswordData.szAlgName.value();
				m_oHashValue = ptr->ipdPasswordData.rgbHash.GetBase64();
				m_oSaltValue = ptr->ipdPasswordData.rgbSalt.GetBase64();
			}

		}
	} //Spreadsheet
} // namespace OOX
