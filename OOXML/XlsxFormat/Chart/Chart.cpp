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

#include "Chart.h"

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

