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

#include "NotesSlide.h"

namespace PPTX
{
	NotesSlide::NotesSlide(OOX::Document* pMain) : WrapperFile(pMain), FileContainer(pMain)
	{
	}
	NotesSlide::NotesSlide(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain), FileContainer(pMain)
	{
		read(filename, map);
	}
	NotesSlide::~NotesSlide()
	{
	}
	void NotesSlide::read(const OOX::CPath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);

		XmlUtils::CXmlNode oNode;
		oNode.FromXmlFile(filename.m_strFilename);

		XmlMacroReadAttributeBase(oNode, L"showMasterPhAnim", showMasterPhAnim);
		XmlMacroReadAttributeBase(oNode, L"showMasterSp", showMasterSp);

		cSld = oNode.ReadNode(_T("p:cSld"));
		cSld.SetParentFilePointer(this);

		clrMapOvr = oNode.ReadNode(_T("p:clrMapOvr"));
		if (clrMapOvr.IsInit())
			clrMapOvr->SetParentFilePointer(this);
	}
	void NotesSlide::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		WrapperFile::write(filename, directory, content);
		FileContainer::write(filename, directory, content);
	}
	const OOX::FileType NotesSlide::type() const
	{
		return OOX::Presentation::FileTypes::NotesSlide;
	}
	const OOX::CPath NotesSlide::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath NotesSlide::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void NotesSlide::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(NSBinPptxRW::NSMainTables::NotesSlides);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteBool2(0, showMasterPhAnim);
		pWriter->WriteBool2(1, showMasterSp);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord1(0, cSld);
		pWriter->WriteRecord2(1, clrMapOvr);

		pWriter->EndRecord();
	}
	void NotesSlide::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(_T("p:notes"));

		pWriter->StartAttributes();
		pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:m"), PPTX::g_Namespaces.m.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:w"), PPTX::g_Namespaces.w.m_strLink);
		pWriter->WriteAttribute(_T("showMasterPhAnim"), showMasterPhAnim);
		pWriter->WriteAttribute(_T("showMasterSp"), showMasterSp);
		pWriter->EndAttributes();

		cSld.toXmlWriter(pWriter);
		pWriter->Write(clrMapOvr);

		pWriter->EndNode(_T("p:notes"));
	}
	void NotesSlide::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		pReader->Skip(1); // type
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // attribute start
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			if (0 == _at)
				showMasterPhAnim = pReader->GetBool();
			else if (1 == _at)
				showMasterSp = pReader->GetBool();
		}
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
					clrMapOvr = new Logic::ClrMapOvr();
					clrMapOvr->fromPPTY(pReader);
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
	void NotesSlide::ApplyRels()
	{
		smart_ptr<OOX::File> pFile = FileContainer::Get(OOX::Presentation::FileTypes::NotesMaster);

		master_ = pFile.smart_dynamic_cast<PPTX::NotesMaster>();

		if (master_.IsInit())
		{
			theme_ = master_->theme_;
		}
	}
} // namespace PPTX
