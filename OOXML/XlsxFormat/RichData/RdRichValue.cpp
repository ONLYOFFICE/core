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
#include "RdRichValue.h"

#include "../FileTypes_Spreadsheet.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

#include "../../DocxFormat/Drawing/DrawingExt.h"
#include "../../../DesktopEditor/common/File.h"

#include "../../Binary/Presentation/XmlWriter.h"
#include "../../Binary/Presentation/BinaryFileReaderWriter.h"

namespace OOX
{
namespace Spreadsheet
{
	CRdRichValueFile::CRdRichValueFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CRdRichValueFile::CRdRichValueFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CRdRichValueFile::~CRdRichValueFile()
	{
	}
	void CRdRichValueFile::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	const OOX::FileType CRdRichValueFile::type() const
	{
		return OOX::Spreadsheet::FileTypes::RdRichValue;
	}
	const CPath CRdRichValueFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CRdRichValueFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath& CRdRichValueFile::GetReadPath()
	{
		return m_oReadPath;
	}
	void CRdRichValueFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oPath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		//m_oMetadata = oReader;
	}
	void CRdRichValueFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		//if (false == m_oMetadata.IsInit()) return;

		NSStringUtils::CStringBuilder sXml;

		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		//m_oMetadata->toXML(sXml);

		std::wstring sPath = oPath.GetPath();
		NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

		oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
	}
//---------------------------------------------------------------------------------------------------------------------------
	CRdRichValueStructureFile::CRdRichValueStructureFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CRdRichValueStructureFile::CRdRichValueStructureFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CRdRichValueStructureFile::~CRdRichValueStructureFile()
	{
	}
	void CRdRichValueStructureFile::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	const OOX::FileType CRdRichValueStructureFile::type() const
	{
		return OOX::Spreadsheet::FileTypes::RdRichValueStructure;
	}
	const CPath CRdRichValueStructureFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CRdRichValueStructureFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath& CRdRichValueStructureFile::GetReadPath()
	{
		return m_oReadPath;
	}
	void CRdRichValueStructureFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oPath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		//m_oMetadata = oReader;
	}
	void CRdRichValueStructureFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		//if (false == m_oMetadata.IsInit()) return;

		NSStringUtils::CStringBuilder sXml;

		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		//m_oMetadata->toXML(sXml);

		std::wstring sPath = oPath.GetPath();
		NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

		oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
	}
//---------------------------------------------------------------------------------------------------------------------------
	CRdRichValueTypesFile::CRdRichValueTypesFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CRdRichValueTypesFile::CRdRichValueTypesFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CRdRichValueTypesFile::~CRdRichValueTypesFile()
	{
	}
	void CRdRichValueTypesFile::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	const OOX::FileType CRdRichValueTypesFile::type() const
	{
		return OOX::Spreadsheet::FileTypes::RdRichValueTypes;
	}
	const CPath CRdRichValueTypesFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CRdRichValueTypesFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	const CPath& CRdRichValueTypesFile::GetReadPath()
	{
		return m_oReadPath;
	}
	void CRdRichValueTypesFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;

		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oPath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		//m_oMetadata = oReader;
	}
	void CRdRichValueTypesFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		//if (false == m_oMetadata.IsInit()) return;

		NSStringUtils::CStringBuilder sXml;

		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		//m_oMetadata->toXML(sXml);

		std::wstring sPath = oPath.GetPath();
		NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

		oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
	}
}
}

