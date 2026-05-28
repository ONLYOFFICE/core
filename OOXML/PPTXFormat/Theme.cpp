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

#include "Theme.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/THEME.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Theme.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../DesktopEditor/common/Directory.h"
#include <chrono>

namespace PPTX
{
	Theme::Theme(OOX::Document *pMain) : WrapperFile(pMain), PPTX::FileContainer(pMain)
	{
		isThemeOverride = false;
		m_map = NULL;

		OOX::CDocx* docx = dynamic_cast<OOX::CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pTheme = this;
	}
	Theme::Theme(OOX::Document *pMain, const std::string &contentTheme) : WrapperFile(pMain), PPTX::FileContainer(pMain)
	{
		FileMap map;

		isThemeOverride = false;
		m_map = NULL;

		read(contentTheme, map);

		OOX::CDocx* docx = dynamic_cast<OOX::CDocx*>(File::m_pMainDocument);
		if (docx && !docx->m_pTheme && !isThemeOverride)
		{
			docx->m_pTheme = this;
		}
	}
	Theme::Theme(OOX::Document *pMain, const OOX::CPath& filename) : WrapperFile(pMain), PPTX::FileContainer(pMain)
	{
		IFileContainer::Read( filename, filename );

		FileMap map;

		isThemeOverride = false;
		m_map = NULL;

		read(filename, map);

		OOX::CDocx* docx = dynamic_cast<OOX::CDocx*>(File::m_pMainDocument);
		if (docx && !docx->m_pTheme && !isThemeOverride)
		{
			docx->m_pTheme = this;
		}
	}
	Theme::Theme(OOX::Document *pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain), PPTX::FileContainer(pMain)
	{
		isThemeOverride = false;
		m_map = NULL;

		read(filename, map);

		OOX::CDocx* docx = dynamic_cast<OOX::CDocx*>(File::m_pMainDocument);
		if (docx && !docx->m_pTheme && !isThemeOverride)
		{
			docx->m_pTheme = this;
		}
	}
	Theme::~Theme()
	{
	}
	void Theme::read(const std::string &contentTheme, FileMap& map)
	{
		isThemeOverride = false;
		//FileContainer::read(filename, map);
		m_map = NULL;

		XmlUtils::CXmlNode oNode;
		oNode.FromXmlStringA(contentTheme);

		read(oNode, map);
	}
	void Theme::read(const OOX::CPath& filename, FileMap& map)
	{
		isThemeOverride = false;
		//FileContainer::read(filename, map);
		m_map = NULL;

		XmlUtils::CXmlNode oNode;
		oNode.FromXmlFile(filename.m_strFilename);

		read(oNode, map);
	}
	void Theme::read(XmlUtils::CXmlNode &oNode, FileMap& map)
	{
		extraClrSchemeLst.clear();

		std::wstring strNodeName = XmlUtils::GetNameNoNS(oNode.GetName());
		if (_T("themeOverride") == strNodeName)
		{
			themeElements = oNode;
			themeElements.SetParentFilePointer(this);

			extraClrSchemeLst.clear();
			isThemeOverride = true;
			return;
		}

		XmlMacroReadAttributeBase(oNode, _T("name"), name);

		themeElements = oNode.ReadNode(_T("a:themeElements"));
		themeElements.SetParentFilePointer(this);

		XmlUtils::CXmlNode oDefaults;
		if (oNode.GetNode(_T("a:objectDefaults"), oDefaults))
		{
			spDef = oDefaults.ReadNode(_T("a:spDef"));
			if (spDef.IsInit())
				spDef->SetParentFilePointer(this);

			lnDef = oDefaults.ReadNode(_T("a:lnDef"));
			if (lnDef.IsInit())
				lnDef->SetParentFilePointer(this);

			txDef = oDefaults.ReadNode(_T("a:txDef"));
			if (txDef.IsInit())
				txDef->SetParentFilePointer(this);
		}

		XmlUtils::CXmlNode oNodeList;
		if (oNode.GetNode(_T("a:extraClrSchemeLst"), oNodeList))
		{
			XmlMacroLoadArray(oNodeList, _T("a:extraClrScheme"), extraClrSchemeLst, nsTheme::ExtraClrScheme);
		}

		size_t count = extraClrSchemeLst.size();
		for (size_t i = 0; i < count; ++i)
			extraClrSchemeLst[i].SetParentFilePointer(this);
	}
	void Theme::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		m_WrittenFileName = filename.GetFilename();

		NSBinPptxRW::CXmlWriter oXmlWriter;
		toXmlWriter(&oXmlWriter);

		oXmlWriter.SaveToFile(filename.m_strFilename);

		content.Registration(type().OverrideType(), directory, m_WrittenFileName);

		m_written = true;
		FileContainer::write(filename, directory, content);
	}
	void Theme::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(NSBinPptxRW::NSMainTables::Themes);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, name);

		if (isThemeOverride)
			pWriter->WriteBool1(1, true);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord1(0, themeElements);
		pWriter->WriteRecord2(1, spDef);
		pWriter->WriteRecord2(2, lnDef);
		pWriter->WriteRecord2(3, txDef);
		pWriter->WriteRecordArray(4, 0, extraClrSchemeLst);

		pWriter->EndRecord();
	}
	void Theme::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		if (isThemeOverride)
		{
			pWriter->StartNode(_T("a:themeOverride"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
			pWriter->EndAttributes();

			themeElements.clrScheme.toXmlWriter(pWriter);
			themeElements.fontScheme.toXmlWriter(pWriter);
			themeElements.fmtScheme.toXmlWriter(pWriter);

			pWriter->EndNode(_T("a:themeOverride"));
			return;
		}

		pWriter->StartNode(_T("a:theme"));

		pWriter->StartAttributes();
		pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
		pWriter->WriteAttribute2(_T("name"), name);
		pWriter->EndAttributes();

		themeElements.toXmlWriter(pWriter);
		pWriter->StartNode(_T("a:objectDefaults"));
		pWriter->EndAttributes();
		pWriter->Write(spDef);
		pWriter->Write(lnDef);
		pWriter->Write(txDef);
		pWriter->EndNode(_T("a:objectDefaults"));

		pWriter->WriteArray(_T("a:extraClrSchemeLst"), extraClrSchemeLst);

		pWriter->EndNode(_T("a:theme"));
	}
XLS::BaseObjectPtr Theme::toXLS(const OOX::CPath& oFilePath)
{
	auto combinePath = [&](const std::wstring& sDir, const std::wstring& sName) {
	return sDir + FILE_SEPARATOR_STR + sName;
	};
	// 1. xml theme generation
	NSBinPptxRW::CXmlWriter oXmlWriter;
	oXmlWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
	themeElements.fontScheme.majorFont.Fonts.clear();
	themeElements.fontScheme.minorFont.Fonts.clear();
	toXmlWriter(&oXmlWriter);

	auto themeData = oXmlWriter.GetXmlString();
	std::string themeXml(themeData.begin(), themeData.end());

	// 2. getting temp path
	std::wstring tempBasePath = oFilePath.GetDirectory(true);
	auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	std::wstring tempDir = tempBasePath + L"xls_theme_" + std::to_wstring(now);

	std::wstring themeDir = combinePath(combinePath(tempDir, L"theme"), L"theme");

	// directory creation
	NSDirectory::CreateDirectory(tempDir.c_str());
	NSDirectory::CreateDirectory(combinePath(tempDir, L"theme").c_str());
	NSDirectory::CreateDirectory(themeDir.c_str());
	NSDirectory::CreateDirectory(combinePath(tempDir, L"_rels").c_str());
	NSDirectory::CreateDirectory(combinePath(themeDir, L"_rels").c_str());

	// 3. file writing
	auto write_file = [](const std::wstring& path, const std::string& content) {
	#ifdef _WIN32
			std::ofstream f(path, std::ios::binary);
	#else
			std::string narrowPath(path.begin(), path.end());
			std::ofstream f(narrowPath, std::ios::binary);
	#endif
			if (f.is_open()) {
				f.write(content.data(), content.size());
			}
		};

	write_file(combinePath(tempDir, L"[Content_Types].xml"),
		"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">"
		"<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>"
		"<Default Extension=\"xml\" ContentType=\"application/xml\"/>"
		"<Override PartName=\"/theme/theme/themeManager.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.themeManager+xml\"/>"
		"<Override PartName=\"/theme/theme/theme1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/></Types>");

	write_file(combinePath(themeDir, L"theme1.xml"), themeXml);

	write_file(combinePath(themeDir, L"themeManager.xml"),
		"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><a:themeManager xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"/>");

	write_file(combinePath(combinePath(tempDir, L"_rels"), L".rels"),
		"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
		"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"theme/theme/themeManager.xml\"/></Relationships>");

	write_file(combinePath(combinePath(themeDir, L"_rels"), L"themeManager.xml.rels"),
		"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
		"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme1.xml\"/></Relationships>");

	// 4. compression
	std::wstring zipPath = tempDir + L".zip";
	COfficeUtils OfficeUtils(NULL);
	OfficeUtils.CompressFileOrDirectory(tempDir, zipPath, true, Z_DEFLATED, 6, true);

	// 5. reading zip file
	std::ifstream zip(zipPath, std::ios::binary | std::ios::ate);
	std::streamsize size = zip.tellg();
	zip.seekg(0, std::ios::beg);

	auto buffer = new BYTE[size];
	zip.read((char*)buffer, size);
	zip.close();

	// 6. xls objects building
	auto XlsTheme = new XLS::THEME;
	auto themeHeader = new XLS::Theme;
	themeHeader->dwThemeVersion = 0;
	themeHeader->nThemeDataSize = (size_t)size;
	themeHeader->pThemeData.reset(buffer);
	XlsTheme->m_Theme = XLS::BaseObjectPtr(themeHeader);

	// 7. removing temp file
	NSFile::CFileBinary::Remove(zipPath.c_str());
	NSDirectory::DeleteDirectory(tempDir);

	return XLS::BaseObjectPtr(XlsTheme);
}
	void Theme::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		BYTE type = pReader->GetUChar();

		LONG _rec_start = pReader->GetPos();
		LONG _end_rec = _rec_start + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // start attributes

		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();

			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			if (0 == _at)
				name = pReader->GetString2();
			else if (1 == _at)
				isThemeOverride = pReader->GetBool();
			else
				break;
		}

		while (pReader->GetPos() < _end_rec)
		{
			BYTE _at = pReader->GetUChar();
			switch (_at)
			{
				case 0:
				{
					// themeElements
					themeElements.fromPPTY(pReader);
					break;
				}
				case 1:
				{
					spDef = new Logic::DefaultShapeDefinition();
					spDef->m_name = _T("spDef");
					spDef->fromPPTY(pReader);
					break;
				}
				case 2:
				{
					lnDef = new Logic::DefaultShapeDefinition();
					lnDef->m_name = _T("lnDef");
					lnDef->fromPPTY(pReader);
					break;
				}
				case 3:
				{
					txDef = new Logic::DefaultShapeDefinition();
					txDef->m_name = _T("txDef");
					txDef->fromPPTY(pReader);
					break;
				}
				case 4:
				{
					extraClrSchemeLst.clear();
					pReader->Skip(4); // len
					ULONG _len = pReader->GetULong();
					for (ULONG i = 0; i < _len; ++i)
					{
						pReader->Skip(1); // type
						extraClrSchemeLst.push_back(nsTheme::ExtraClrScheme());
						extraClrSchemeLst[i].fromPPTY(pReader);
					}
				}
			}
		}

		pReader->Seek(_end_rec);
	}
	const OOX::FileType Theme::type() const
	{
		if (isThemeOverride)	return OOX::FileTypes::ThemeOverride;
		else					return OOX::FileTypes::Theme;
	}
	const OOX::CPath Theme::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath Theme::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	DWORD Theme::GetRGBAFromScheme(const std::wstring& str)const
	{
		return themeElements.clrScheme.GetRGBAFromScheme(str);
	}
	DWORD Theme::GetARGBFromScheme(const std::wstring& str)const
	{
		return themeElements.clrScheme.GetARGBFromScheme(str);
	}
	DWORD Theme::GetBGRAFromScheme(const std::wstring& str)const
	{
		return themeElements.clrScheme.GetBGRAFromScheme(str);
	}
	DWORD Theme::GetABGRFromScheme(const std::wstring& str)const
	{
		return themeElements.clrScheme.GetABGRFromScheme(str);
	}

	DWORD Theme::GetRGBAFromMap(const std::wstring& str)const
	{
		return GetRGBAFromScheme(m_map->GetColorSchemeIndex(str));
	}
	DWORD Theme::GetARGBFromMap(const std::wstring& str)const
	{
		if (m_map)	return GetARGBFromScheme(m_map->GetColorSchemeIndex(str));
		else		return 0;
	}
	DWORD Theme::GetBGRAFromMap(const std::wstring& str)const
	{
		if (m_map)	return GetBGRAFromScheme(m_map->GetColorSchemeIndex(str));
		else		return 0;
	}
	DWORD Theme::GetABGRFromMap(const std::wstring& str)const
	{
		if (m_map)	return GetABGRFromScheme(m_map->GetColorSchemeIndex(str));
		else		return 0;
	}

	void Theme::GetLineStyle(int number, Logic::Ln& lnStyle)const
	{
		themeElements.fmtScheme.GetLineStyle(number, lnStyle);
	}
	void Theme::GetFillStyle(int number, Logic::UniFill& fillStyle)const
	{
		themeElements.fmtScheme.GetFillStyle(number, fillStyle);
	}

	void Theme::SetColorMap(const Logic::ClrMap& map)
	{
		m_map = &map;
	};
	bool Theme::isMapPresent()
	{
		return (m_map != NULL);
	}
} // namespace PPTX
