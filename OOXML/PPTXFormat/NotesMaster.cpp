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

#include "NotesMaster.h"

#include "TableStyles.h"
#include "Logic/Hf.h"
#include "Logic/TextListStyle.h"
#include "Logic/Shape.h"

namespace PPTX
{
	NotesMaster::NotesMaster(OOX::Document* pMain) : WrapperFile(pMain), FileContainer(pMain)
	{
	}
	NotesMaster::NotesMaster(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain), FileContainer(pMain)
	{
		read(filename, map);
	}
	NotesMaster::~NotesMaster()
	{
	}
	void NotesMaster::read(const OOX::CPath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);

		XmlUtils::CXmlNode oNode;
		oNode.FromXmlFile(filename.m_strFilename);

		cSld = oNode.ReadNode(L"p:cSld");
		cSld.SetParentFilePointer(this);

		clrMap = oNode.ReadNode(L"p:clrMap");
		clrMap.SetParentFilePointer(this);

		hf = oNode.ReadNode(L"p:hf");
		if (hf.IsInit())
			hf->SetParentFilePointer(this);

		notesStyle = oNode.ReadNode(L"p:notesStyle");
		if (notesStyle.is_init())
			notesStyle->SetParentFilePointer(this);
	}
	void NotesMaster::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		WrapperFile::write(filename, directory, content);
		FileContainer::write(filename, directory, content);
	}
	const OOX::FileType NotesMaster::type() const
	{
		return OOX::Presentation::FileTypes::NotesMaster;
	}
	const OOX::CPath NotesMaster::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath NotesMaster::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void NotesMaster::ApplyRels()
	{
		theme_= (FileContainer::Get(OOX::FileTypes::Theme)).smart_dynamic_cast<PPTX::Theme>();

		if (theme_.IsInit())
		{
			theme_->SetColorMap(clrMap);

			tableStyles_ = (theme_->presentation->Get(OOX::Presentation::FileTypes::TableStyles)).smart_dynamic_cast<PPTX::TableStyles>();
		}
	}
	void NotesMaster::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(NSBinPptxRW::NSMainTables::NotesMasters);

		pWriter->WriteRecord1(0, cSld);
		pWriter->WriteRecord1(1, clrMap);
		pWriter->WriteRecord2(2, hf);
		pWriter->WriteRecord2(3, notesStyle);

		pWriter->EndRecord();
	}
	void NotesMaster::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"p:notesMaster");

		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"xmlns:a", PPTX::g_Namespaces.a.m_strLink);
		pWriter->WriteAttribute(L"xmlns:r", PPTX::g_Namespaces.r.m_strLink);
		pWriter->WriteAttribute(L"xmlns:p", PPTX::g_Namespaces.p.m_strLink);
		pWriter->EndAttributes();

		cSld.toXmlWriter(pWriter);

		clrMap.toXmlWriter(pWriter);
		pWriter->Write(hf);
		pWriter->Write(notesStyle);

		pWriter->EndNode(L"p:notesMaster");
	}
	void NotesMaster::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				case 3:
				{
					notesStyle = new Logic::TextListStyle(L"p:notesStyle");
					notesStyle->fromPPTY(pReader);
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
	void NotesMaster::GetLevelUp(WrapperWritingElement* pElem)
	{
		Logic::Shape	*pShape	= dynamic_cast<PPTX::Logic::Shape*>(pElem);
		Logic::Pic		*pPic	= dynamic_cast<PPTX::Logic::Pic*>(pElem);

		if (!pShape && !pPic) return;

		Logic::NvPr *pNvPr = NULL;

		if (pShape)	pNvPr = &pShape->nvSpPr.nvPr;
		if (pPic)	pNvPr = &pPic->nvPicPr.nvPr;

		if (!pNvPr) return;
		if (pNvPr->ph.is_init() == false) return;

		std::wstring idx = pNvPr->ph->idx.get_value_or(L"");
		std::wstring type = pNvPr->ph->type.get_value_or(L"body");

		if (type == L"ctrTitle") type = L"title";

		for (size_t i = 0; i < cSld.spTree.SpTreeElems.size(); ++i)
		{
			smart_ptr<Logic::Shape> pMasterShape = cSld.spTree.SpTreeElems[i].GetElem().smart_dynamic_cast<Logic::Shape>();

			if (pMasterShape.IsInit())
			{
				if (pMasterShape->nvSpPr.nvPr.ph.is_init())
				{
					std::wstring lIdx	= pMasterShape->nvSpPr.nvPr.ph->idx.get_value_or(L"");
					std::wstring lType	= pMasterShape->nvSpPr.nvPr.ph->type.get_value_or(L"body");

					if (lType == L"ctrTitle") lType = L"title";

					if ((type == lType) && (idx == lIdx) && !idx.empty())
					{
						if (pShape)	pShape->SetLevelUpElement(pMasterShape.GetPointer());
						if (pPic)	pPic->SetLevelUpElement(pMasterShape.GetPointer());
						return;
					}
					else if ((type == lType) && idx.empty() && lIdx.empty())
					{
						if (pShape)	pShape->SetLevelUpElement(pMasterShape.GetPointer());
						if (pPic)	pPic->SetLevelUpElement(pMasterShape.GetPointer());
						return;
					}
				}
			}
		}
	}
} // namespace PPTX
