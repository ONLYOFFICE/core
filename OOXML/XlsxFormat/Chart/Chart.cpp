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

#include "Chart.h"
#include "../../DocxFormat/Drawing/DrawingExt.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CChartFile::CChartFile(OOX::Document* pMain, bool bDocument) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = bDocument;
			m_bSpreadsheets = (NULL != dynamic_cast<OOX::Spreadsheet::CXlsx*>(pMain));
		}
		CChartFile::CChartFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = (NULL != dynamic_cast<OOX::CDocument*>(pMain));
			m_bSpreadsheets = (NULL != dynamic_cast<OOX::Spreadsheet::CXlsx*>(pMain));
			read( oRootPath, oPath );
		}
		CChartFile::~CChartFile()
		{
		}
		void CChartFile::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void CChartFile::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read( oRootPath, oPath );

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			m_oChartSpace.fromXML(oReader);
		}
		void CChartFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			std::wstring sPath = oPath.GetPath();
			write2(sPath);
			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write(oPath, oDirectory, oContent);
		}
		void CChartFile::write2(const std::wstring& sFilename) const
		{
			NSStringUtils::CStringBuilder sXml;
			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
			toXML(sXml);
			NSFile::CFileBinary::SaveToFile(sFilename, sXml.GetData());
		}
		void CChartFile::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			m_oChartSpace.toXML(L"c:chartSpace", writer);
		}
		const OOX::FileType CChartFile::type() const
		{
			return OOX::FileTypes::Chart;
		}
		const CPath CChartFile::DefaultDirectory() const
		{
			if (m_bDocument)
				return type().DefaultDirectory();
			else
				return L"../" + type().DefaultDirectory();
		}
		const CPath CChartFile::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CChartFile::GetReadPath()
		{
			return m_oReadPath;
		}
		void CChartFile::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		CChartExFile::CChartExFile(OOX::Document* pMain, bool bDocument) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = bDocument;
			m_bSpreadsheets = (NULL != dynamic_cast<OOX::Spreadsheet::CXlsx*>(pMain));
		}
		CChartExFile::CChartExFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bDocument = (NULL != dynamic_cast<OOX::CDocument*>(pMain));
			m_bSpreadsheets = (NULL != dynamic_cast<OOX::Spreadsheet::CXlsx*>(pMain));
			read( oRootPath, oPath );
		}
		CChartExFile::~CChartExFile()
		{
		}
		void CChartExFile::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void CChartExFile::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read( oRootPath, oPath );

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			m_oChartSpace.fromXML(oReader);
		}
		void CChartExFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			std::wstring sPath = oPath.GetPath();
			write2(sPath);
			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write(oPath, oDirectory, oContent);
		}
		void CChartExFile::write2(const std::wstring& sFilename) const
		{
			NSStringUtils::CStringBuilder sXml;
			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
			toXML(sXml);
			NSFile::CFileBinary::SaveToFile(sFilename, sXml.GetData());
		}
		void CChartExFile::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			m_oChartSpace.toXML(writer);
		}
		const OOX::FileType CChartExFile::type() const
		{
			return OOX::FileTypes::ChartEx;
		}
		const CPath CChartExFile::DefaultDirectory() const
		{
			if (m_bDocument)
				return type().DefaultDirectory();
			else
				return L"../" + type().DefaultDirectory();
		}
		const CPath CChartExFile::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CChartExFile::GetReadPath()
		{
			return m_oReadPath;
		}
		void CChartExFile::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		CChartStyleFile::CChartStyleFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain)/*, OOX::IFileContainer(pMain)*/
		{
			//m_bSpreadsheets = true;
		}
		CChartStyleFile::CChartStyleFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain)/*, OOX::IFileContainer(pMain)*/
		{
			//m_bSpreadsheets = true;
			read( oRootPath, oPath );
		}
		CChartStyleFile::~CChartStyleFile()
		{
		}
		void CChartStyleFile::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void CChartStyleFile::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			//IFileContainer::Read( oRootPath, oPath );

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			m_oChartStyle.fromXML(oReader);
		}
		void CChartStyleFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			std::wstring sPath = oPath.GetPath();
			write2(sPath);
			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			//IFileContainer::Write(oPath, oDirectory, oContent);
		}
		void CChartStyleFile::write2(const std::wstring& sFilename) const
		{
			NSStringUtils::CStringBuilder sXml;
			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
			toXML(sXml);
			NSFile::CFileBinary::SaveToFile(sFilename, sXml.GetData());
		}
		void CChartStyleFile::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			m_oChartStyle.toXML(writer);
		}
		bool CChartStyleFile::isValid() const
		{
			return true;
		}
		const OOX::FileType CChartStyleFile::type() const
		{
			return OOX::FileTypes::ChartStyle;
		}
		const CPath CChartStyleFile::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CChartStyleFile::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CChartStyleFile::GetReadPath()
			{
				return m_oReadPath;
			}

		CChartColorsFile::CChartColorsFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain)/*, OOX::IFileContainer(pMain)*/
		{
			//m_bSpreadsheets = true;
		}
		CChartColorsFile::CChartColorsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain)/*, OOX::IFileContainer(pMain)*/
		{
			//m_bSpreadsheets = true;
			read( oRootPath, oPath );
		}
		CChartColorsFile::~CChartColorsFile()
		{
		}
		void CChartColorsFile::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		void CChartColorsFile::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			//IFileContainer::Read( oRootPath, oPath );

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			m_oColorStyle.fromXML(oReader);
		}
		void CChartColorsFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			std::wstring sPath = oPath.GetPath();
			write2(sPath);
			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			//IFileContainer::Write(oPath, oDirectory, oContent);
		}
		void CChartColorsFile::write2(const std::wstring& sFilename) const
		{
			NSStringUtils::CStringBuilder sXml;
			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
			toXML(sXml);
			NSFile::CFileBinary::SaveToFile(sFilename, sXml.GetData());
		}
		void CChartColorsFile::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			m_oColorStyle.toXML(writer);
		}
		bool CChartColorsFile::isValid() const
		{
			return true;
		}
		const OOX::FileType CChartColorsFile::type() const
		{
			return OOX::FileTypes::ChartColors;
		}
		const CPath CChartColorsFile::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CChartColorsFile::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CChartColorsFile::GetReadPath()
			{
				return m_oReadPath;
			}

	} //Spreadsheet
} // namespace OOX

