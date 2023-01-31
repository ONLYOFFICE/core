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

#include "CustomXmlWriter.h"
#include "../../../DocxFormat/CustomXml.h"
#include "../../../PPTXFormat/DrawingConverter/ASCOfficeDrawingConverter.h"

namespace Writers
{
	CustomXmlWriter::CustomXmlWriter(std::wstring sDir, NSBinPptxRW::CDrawingConverter* pDrawingConverter) : m_sDir(sDir), m_pDrawingConverter(pDrawingConverter)
	{
		m_nCount = 0;
	}
	void CustomXmlWriter::WriteCustom(const std::wstring& sCustomXmlPropertiesContent, const std::wstring& sCustomXmlContent, bool bGlossaryMode)
	{
		m_nCount++;

		std::wstring sCustomXmlDir = m_sDir + FILE_SEPARATOR_STR;
		sCustomXmlDir += OOX::FileTypes::CustomXml.DefaultDirectory().GetPath();
		
		std::wstring sCustomXmlRelsDir = sCustomXmlDir + FILE_SEPARATOR_STR + L"_rels";
		
		std::wstring  sCustomXMLPropsFilename = OOX::FileTypes::CustomXmlProps.DefaultFileName().GetBasename();
		sCustomXMLPropsFilename += std::to_wstring(m_nCount) + OOX::FileTypes::CustomXmlProps.DefaultFileName().GetExtention();

		NSFile::CFileBinary::SaveToFile(sCustomXmlDir + FILE_SEPARATOR_STR + sCustomXMLPropsFilename, sCustomXmlPropertiesContent);
		OOX::CContentTypes& oContentTypes = *m_pDrawingConverter->GetContentTypes();
		oContentTypes.Registration( OOX::FileTypes::CustomXmlProps.OverrideType(), OOX::FileTypes::CustomXml.DefaultDirectory(), sCustomXMLPropsFilename );

		std::wstring sCustomXmlFilename;
		sCustomXmlFilename = OOX::FileTypes::CustomXml.DefaultFileName().GetBasename() + std::to_wstring(m_nCount);
		sCustomXmlFilename += OOX::FileTypes::CustomXml.DefaultFileName().GetExtention();

		NSFile::CFileBinary::SaveToFile(sCustomXmlDir + FILE_SEPARATOR_STR + sCustomXmlFilename, sCustomXmlContent);
		
		m_pDrawingConverter->SetDstContentRels();
		unsigned int lId;
		m_pDrawingConverter->WriteRels(OOX::FileTypes::CustomXmlProps.RelationType(), sCustomXMLPropsFilename, L"", &lId);
		m_pDrawingConverter->SaveDstContentRels(sCustomXmlRelsDir + FILE_SEPARATOR_STR + sCustomXmlFilename + L".rels");

		arItems.push_back(std::make_pair(sCustomXmlFilename, bGlossaryMode));
	}
	void CustomXmlWriter::WriteCustomA(const std::wstring& sCustomXmlPropertiesContent, const std::string& sCustomXmlContent, bool bGlossaryMode)
	{
		m_nCount++;

		std::wstring sCustomXmlDir = m_sDir + FILE_SEPARATOR_STR;
		sCustomXmlDir += OOX::FileTypes::CustomXml.DefaultDirectory().GetPath();

		std::wstring sCustomXmlRelsDir = sCustomXmlDir + FILE_SEPARATOR_STR + L"_rels";

		std::wstring  sCustomXMLPropsFilename = OOX::FileTypes::CustomXmlProps.DefaultFileName().GetBasename();
		sCustomXMLPropsFilename += std::to_wstring(m_nCount) + OOX::FileTypes::CustomXmlProps.DefaultFileName().GetExtention();

		NSFile::CFileBinary::SaveToFile(sCustomXmlDir + FILE_SEPARATOR_STR + sCustomXMLPropsFilename, sCustomXmlPropertiesContent);
		OOX::CContentTypes& oContentTypes = *m_pDrawingConverter->GetContentTypes();
		oContentTypes.Registration(OOX::FileTypes::CustomXmlProps.OverrideType(), OOX::FileTypes::CustomXml.DefaultDirectory(), sCustomXMLPropsFilename);

		std::wstring sCustomXmlFilename;
		sCustomXmlFilename = OOX::FileTypes::CustomXml.DefaultFileName().GetBasename() + std::to_wstring(m_nCount);
		sCustomXmlFilename += OOX::FileTypes::CustomXml.DefaultFileName().GetExtention();

		NSFile::CFileBinary oFile;
		if (true == oFile.CreateFileW(sCustomXmlDir + FILE_SEPARATOR_STR + sCustomXmlFilename))
		{
			if (false == sCustomXmlContent.empty())
				oFile.WriteFile((BYTE*)sCustomXmlContent.c_str(), sCustomXmlContent.length());
			oFile.CloseFile();
		}

		m_pDrawingConverter->SetDstContentRels();
		unsigned int lId;
		m_pDrawingConverter->WriteRels(OOX::FileTypes::CustomXmlProps.RelationType(), sCustomXMLPropsFilename, L"", &lId);
		m_pDrawingConverter->SaveDstContentRels(sCustomXmlRelsDir + FILE_SEPARATOR_STR + sCustomXmlFilename + L".rels");

		arItems.push_back(std::make_pair(sCustomXmlFilename, bGlossaryMode));
	}
	void CustomXmlWriter::WriteCustomSettings(const std::wstring& sUrl, const std::wstring& sXml, bool bGlossaryMode)
	{
		m_nCount++;
		OOX::CCustomXMLProps oCustomXMLProps(NULL);
		OOX::CCustomXMLProps::CShemaRef* pShemaRef = new OOX::CCustomXMLProps::CShemaRef();
		pShemaRef->m_sUri = sUrl;
	//todo guid
		oCustomXMLProps.m_oItemID.FromString(L"{5D0AEA6B-E499-4EEF-98A3-AFBB261C493E}");
		oCustomXMLProps.m_oShemaRefs.Init();
		oCustomXMLProps.m_oShemaRefs->m_arrItems.push_back(pShemaRef);

		std::wstring sCustomXmlPropsDir = m_sDir + FILE_SEPARATOR_STR;
		sCustomXmlPropsDir += OOX::FileTypes::CustomXml.DefaultDirectory().GetPath();
		NSDirectory::CreateDirectories(sCustomXmlPropsDir);
		std::wstring  sCustomXMLPropsFilename = OOX::FileTypes::CustomXmlProps.DefaultFileName().GetBasename();
		sCustomXMLPropsFilename += std::to_wstring(m_nCount) + OOX::FileTypes::CustomXmlProps.DefaultFileName().GetExtention();
		oCustomXMLProps.write(OOX::CPath(sCustomXmlPropsDir + FILE_SEPARATOR_STR + sCustomXMLPropsFilename), OOX::CPath(sCustomXmlPropsDir), *m_pDrawingConverter->GetContentTypes());

		std::wstring sCustomXmlFilename;
		sCustomXmlFilename = OOX::FileTypes::CustomXml.DefaultFileName().GetBasename() + std::to_wstring(m_nCount);
		sCustomXmlFilename += OOX::FileTypes::CustomXml.DefaultFileName().GetExtention();
		std::wstring sCustomXmlDir = m_sDir + FILE_SEPARATOR_STR + OOX::FileTypes::CustomXml.DefaultDirectory().GetPath();
		std::wstring sCustomXmlRelsDir = sCustomXmlDir + FILE_SEPARATOR_STR + L"_rels";
		NSDirectory::CreateDirectories(sCustomXmlRelsDir);

		m_pDrawingConverter->SetDstContentRels();
		unsigned int lId;
		m_pDrawingConverter->WriteRels(OOX::FileTypes::CustomXmlProps.RelationType(), sCustomXMLPropsFilename, L"", &lId);
		m_pDrawingConverter->SaveDstContentRels(sCustomXmlRelsDir + FILE_SEPARATOR_STR + sCustomXmlFilename + L".rels");

		NSFile::CFileBinary::SaveToFile(sCustomXmlDir + FILE_SEPARATOR_STR + sCustomXmlFilename, sXml);
		
		arItems.push_back(std::make_pair(sCustomXmlFilename, bGlossaryMode));
	}
}
