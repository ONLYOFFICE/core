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

#include "ChartSerializeEx.h"
#include "../../DocxFormat/FileTypes.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CChartFile: public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CChartFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
			}
			CChartFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
			virtual ~CChartFile()
			{
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
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
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
                std::wstring sPath = oPath.GetPath();
                write2(sPath);
				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write(oPath, oDirectory, oContent);
			}
            virtual void write2(const std::wstring& sFilename) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
				toXML(sXml);
                NSFile::CFileBinary::SaveToFile(sFilename, sXml.GetData());
			}
			void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				m_oChartSpace.toXML(L"c:chartSpace", writer);
			}
			virtual const OOX::FileType type() const
			{
				return OOX::FileTypes::Chart;
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

			CT_ChartSpace m_oChartSpace;
		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CChartExFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CChartExFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
			}
			CChartExFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
			virtual ~CChartExFile()
			{
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
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
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
                std::wstring sPath = oPath.GetPath();
                write2(sPath);
				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write(oPath, oDirectory, oContent);
			}
            virtual void write2(const std::wstring& sFilename) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
				toXML(sXml);
                NSFile::CFileBinary::SaveToFile(sFilename, sXml.GetData());
			}
			void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				m_oChartSpace.toXML(writer);
			}
			virtual const OOX::FileType type() const
			{
				return OOX::FileTypes::ChartEx;
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

			ChartEx::CChartSpace m_oChartSpace;
		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CChartStyleFile : public OOX::FileGlobalEnumerated/*, public OOX::IFileContainer*/
		{
		public:
			CChartStyleFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain)/*, OOX::IFileContainer(pMain)*/
			{
				//m_bSpreadsheets = true;
			}
			CChartStyleFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain)/*, OOX::IFileContainer(pMain)*/
			{
				//m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
			virtual ~CChartStyleFile()
			{
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
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
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
                std::wstring sPath = oPath.GetPath();
                write2(sPath);
				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				//IFileContainer::Write(oPath, oDirectory, oContent);
			}
            virtual void write2(const std::wstring& sFilename) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
				toXML(sXml);
                NSFile::CFileBinary::SaveToFile(sFilename, sXml.GetData());
			}
			void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				m_oChartStyle.toXML(writer);
			}
			bool isValid() const
			{
				return true;
			}
			virtual const OOX::FileType type() const
			{
				return OOX::FileTypes::ChartStyle;
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

			ChartEx::CChartStyle m_oChartStyle;
		private:
			CPath m_oReadPath;
		};
		class CChartColorsFile: public OOX::FileGlobalEnumerated/*, public OOX::IFileContainer*/
		{
		public:
			CChartColorsFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain)/*, OOX::IFileContainer(pMain)*/
			{
				//m_bSpreadsheets = true;
			}
			CChartColorsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain)/*, OOX::IFileContainer(pMain)*/
			{
				//m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
			virtual ~CChartColorsFile()
			{
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
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
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
                std::wstring sPath = oPath.GetPath();
                write2(sPath);
				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				//IFileContainer::Write(oPath, oDirectory, oContent);
			}
            virtual void write2(const std::wstring& sFilename) const
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n");
				toXML(sXml);
                NSFile::CFileBinary::SaveToFile(sFilename, sXml.GetData());
			}
			void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				m_oColorStyle.toXML(writer);
			}
			bool isValid() const
			{
				return true;
			}
			virtual const OOX::FileType type() const
			{
				return OOX::FileTypes::ChartColors;
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

			ChartEx::CColorStyle m_oColorStyle;

		private:
			CPath m_oReadPath;

		};
	} //Spreadsheet
} // namespace OOX

