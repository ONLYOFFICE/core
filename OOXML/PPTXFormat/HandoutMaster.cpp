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

#include "HandoutMaster.h"
#include "Logic/Hf.h"

namespace PPTX
{
	HandoutMaster::HandoutMaster(OOX::Document* pMain) : WrapperFile(pMain), PPTX::FileContainer(pMain)
	{
	}
	HandoutMaster::HandoutMaster(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain), PPTX::FileContainer(pMain)
	{
		read(filename, map);
	}
	HandoutMaster::~HandoutMaster()
	{
	}
	void HandoutMaster::read(const OOX::CPath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);
		XmlUtils::CXmlNode oNode;
		oNode.FromXmlFile(filename.m_strFilename);

		cSld = oNode.ReadNode(L"p:cSld");
		cSld.SetParentFilePointer(this);

		clrMap = oNode.ReadNode(L"p:clrMap");
		clrMap.SetParentFilePointer(this);

		hf = oNode.ReadNode(L"p:hf");

		if (hf.is_init())
			hf->SetParentFilePointer(this);
	}
	void HandoutMaster::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(NSBinPptxRW::NSMainTables::HandoutMasters);

		pWriter->WriteRecord1(0, cSld);
		pWriter->WriteRecord1(1, clrMap);
		pWriter->WriteRecord2(2, hf);

		pWriter->EndRecord();
	}
	void HandoutMaster::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"p:handoutMaster");

		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"xmlns:a", PPTX::g_Namespaces.a.m_strLink);
		pWriter->WriteAttribute(L"xmlns:r", PPTX::g_Namespaces.r.m_strLink);
		pWriter->WriteAttribute(L"xmlns:p", PPTX::g_Namespaces.p.m_strLink);
		pWriter->EndAttributes();

		cSld.toXmlWriter(pWriter);

		clrMap.toXmlWriter(pWriter);
		pWriter->Write(hf);

		pWriter->EndNode(L"p:handoutMaster");
	}
	void HandoutMaster::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		pReader->Skip(1); // type
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				cSld.fromPPTY(pReader);
				break;
			}
			case 1:
			{
				clrMap.fromPPTY(pReader);
				break;
			}
			case 2:
			{
				hf = new Logic::HF();
				hf->fromPPTY(pReader);
				break;
			}
			default:
			{
				pReader->SkipRecord();
				break;
			}
			}
		}

		pReader->Seek(end);
	}
	void HandoutMaster::ApplyRels()
	{
		theme_ = (FileContainer::Get(OOX::FileTypes::Theme)).smart_dynamic_cast<PPTX::Theme>();
		
		if (theme_.IsInit())
		{
			theme_->SetColorMap(clrMap);
		}
	}
	void HandoutMaster::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		WrapperFile::write(filename, directory, content);
		FileContainer::write(filename, directory, content);
	}
	const OOX::FileType HandoutMaster::type() const
	{
		return OOX::Presentation::FileTypes::HandoutMaster;
	}
	const OOX::CPath HandoutMaster::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath HandoutMaster::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
} // namespace PPTX
