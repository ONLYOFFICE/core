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

#include "ViewProps.h"

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

		XmlUtils::CXmlNodes oNodes;
		if (oNode.GetNodes(_T("*"), oNodes))
		{
			int nCount = oNodes.GetCount();
			for (int i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode oNodeChild;
				oNodes.GetAt(i, oNodeChild);

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

