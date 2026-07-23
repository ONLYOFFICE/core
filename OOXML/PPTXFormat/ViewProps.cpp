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

#include "ViewProps.h"

#include "ViewProps/GridSpacing.h"
#include "ViewProps/NormalViewPr.h"
#include "ViewProps/NotesTextViewPr.h"
#include "ViewProps/NotesViewPr.h"
#include "ViewProps/OutlineViewPr.h"
#include "ViewProps/SlideViewPr.h"
#include "ViewProps/SorterViewPr.h"

namespace PPTX
{
	ViewProps::ViewProps(OOX::Document* pMain) : WrapperFile(pMain), PPTX::FileContainer(pMain)
	{
	}
	ViewProps::ViewProps(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain), PPTX::FileContainer(pMain)
	{
		read(filename, map);
	}
	ViewProps::~ViewProps()
	{
	}
	void ViewProps::read(const OOX::CPath& filename, FileMap& map)
	{
		XmlUtils::CXmlNode oNode;
		oNode.FromXmlFile(filename.m_strFilename);

		XmlMacroReadAttributeBase(oNode, L"lastView", attrLastView);
		XmlMacroReadAttributeBase(oNode, L"showComments", attrShowComments);

		std::vector<XmlUtils::CXmlNode> oNodes;
		if (oNode.GetNodes(_T("*"), oNodes))
		{
			size_t nCount = oNodes.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode & oNodeChild = oNodes[i];

				std::wstring strName = XmlUtils::GetNameNoNS(oNodeChild.GetName());
				if (L"gridSpacing" == strName)
				{
					GridSpacing = oNodeChild;
					if (GridSpacing.is_init())
						GridSpacing->SetParentFilePointer(this);
				}
				else if (L"normalViewPr" == strName)
				{
					NormalViewPr = oNodeChild;
					if (NormalViewPr.is_init())
						NormalViewPr->SetParentFilePointer(this);
				}
				else if (L"notesTextViewPr" == strName)
				{
					NotesTextViewPr = oNodeChild;
					if (NotesTextViewPr.is_init())
						NotesTextViewPr->SetParentFilePointer(this);
				}
				else if (L"notesViewPr" == strName)
				{
					NotesViewPr = oNodeChild;
					if (NotesViewPr.is_init())
						NotesViewPr->SetParentFilePointer(this);
				}
				else if (L"outlineViewPr" == strName)
				{
					OutlineViewPr = oNodeChild;
					if (OutlineViewPr.is_init())
						OutlineViewPr->SetParentFilePointer(this);
				}
				else if (L"slideViewPr" == strName)
				{
					SlideViewPr = oNodeChild;
					if (SlideViewPr.is_init())
						SlideViewPr->SetParentFilePointer(this);
				}
				else if (L"sorterViewPr" == strName)
				{
					SorterViewPr = oNodeChild;
					if (SorterViewPr.is_init())
						SorterViewPr->SetParentFilePointer(this);
				}
			}
		}
	}
	void ViewProps::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		WrapperFile::write(filename, directory, content);
		FileContainer::write(filename, directory, content);
	}
	const OOX::FileType ViewProps::type() const
	{
		return OOX::Presentation::FileTypes::ViewProps;
	}
	const OOX::CPath ViewProps::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath ViewProps::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void ViewProps::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(NSBinPptxRW::NSMainTables::ViewProps);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

		pWriter->WriteLimit2(0, attrLastView);
		pWriter->WriteBool2(1, attrShowComments);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, GridSpacing);
		pWriter->WriteRecord2(1, NormalViewPr);
		pWriter->WriteRecord2(2, NotesTextViewPr);
		pWriter->WriteRecord2(3, NotesViewPr);
		pWriter->WriteRecord2(4, OutlineViewPr);
		pWriter->WriteRecord2(5, SlideViewPr);
		pWriter->WriteRecord2(6, SorterViewPr);

		pWriter->EndRecord();
	}
	void ViewProps::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		pReader->Skip(1);

		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes

		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			switch (_at)
			{
				case 0:
				{
					attrLastView = pReader->GetUChar();
				}break;
				case 1:
				{
					attrShowComments = pReader->GetBool();
				}break;
				default:
					break;
			}
		}

		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					GridSpacing = new nsViewProps::GridSpacing();
					GridSpacing->fromPPTY(pReader);
				}break;
				case 1:
				{
					NormalViewPr = new nsViewProps::NormalViewPr();
					NormalViewPr->fromPPTY(pReader);
				}break;
				case 2:
				{
					NotesTextViewPr = new nsViewProps::NotesTextViewPr();
					NotesTextViewPr->fromPPTY(pReader);
				}break;
				case 3:
				{
					NotesViewPr = new nsViewProps::NotesViewPr();
					NotesViewPr->fromPPTY(pReader);
				}break;
				case 4:
				{
					OutlineViewPr = new nsViewProps::OutlineViewPr();
					OutlineViewPr->fromPPTY(pReader);
				}break;
				case 5:
				{
					SlideViewPr = new nsViewProps::SlideViewPr();
					SlideViewPr->fromPPTY(pReader);
				}break;
				case 6:
				{
					SorterViewPr = new nsViewProps::SorterViewPr();
					SorterViewPr->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void ViewProps::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"p:viewPr");

		pWriter->StartAttributes();

		pWriter->WriteAttribute(L"xmlns:a", PPTX::g_Namespaces.a.m_strLink);
		pWriter->WriteAttribute(L"xmlns:r", PPTX::g_Namespaces.r.m_strLink);
		pWriter->WriteAttribute(L"xmlns:p", PPTX::g_Namespaces.p.m_strLink);
		pWriter->WriteAttribute(L"lastView", attrLastView);
		pWriter->WriteAttribute(L"showComments", attrShowComments);

		pWriter->EndAttributes();

		pWriter->Write(NormalViewPr);
		pWriter->Write(SlideViewPr);
		pWriter->Write(OutlineViewPr);
		pWriter->Write(NotesTextViewPr);
		pWriter->Write(SorterViewPr);
		pWriter->Write(NotesViewPr);
		pWriter->Write(GridSpacing);

		pWriter->EndNode(L"p:viewPr");
	}
} // namespace PPTX

