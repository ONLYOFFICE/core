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

#include "Slide.h"

#include "Logic/ClrMapOvr.h"
#include "Logic/CSld.h"
#include "Logic/Transitions/Transition.h"
#include "Logic/Timing/Timing.h"

#include "Theme.h"
#include "SlideMaster.h"
#include "SlideLayout.h"
#include "NotesSlide.h"
#include "TableStyles.h"

#include "../DocxFormat/VmlDrawing.h"

namespace PPTX
{
	Slide::Slide(OOX::Document* pMain) : WrapperFile(pMain), FileContainer(pMain)
	{
	}
	Slide::Slide(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain), FileContainer(pMain)
	{
		read(filename, map);
	}
	Slide::~Slide()
	{
	}
	void Slide::fromXML(XmlUtils::CXmlNode& node)
	{
		std::vector<XmlUtils::CXmlNode> oNodes;
		if (node.GetNodes(L"*", oNodes))
		{
			size_t nCount = oNodes.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode& oNode = oNodes[i];

				std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());
				if (L"cSld" == strName)
				{
					cSld = oNode;
				}
				else if (L"clrMapOvr" == strName)
				{
					clrMapOvr = oNode;
				}
				else if (L"transition" == strName)
				{
					transition = oNode;
				}
				else if (L"timing" == strName)
				{
					timing = oNode;
				}
				else if (L"AlternateContent" == strName)
				{
					XmlUtils::CXmlNode oNodeChoice;
					if (false == oNode.GetNode(L"mc:Choice", oNodeChoice))
					{
						oNode.GetNode(L"mc:Fallback", oNodeChoice);
					}
					if (oNode.IsValid())
					{
						fromXML(oNodeChoice);
					}
				}
			}
		}
	}
	void Slide::read(const OOX::CPath& filename, FileMap& map)
	{
		XmlUtils::CXmlNode oNode;
		oNode.FromXmlFile(filename.m_strFilename);

		XmlMacroReadAttributeBase(oNode, L"show", show);
		XmlMacroReadAttributeBase(oNode, L"showMasterPhAnim", showMasterPhAnim);
		XmlMacroReadAttributeBase(oNode, L"showMasterSp", showMasterSp);

		fromXML(oNode);

		if (cSld.IsInit())
			cSld->SetParentFilePointer(this);

		if (clrMapOvr.IsInit())
			clrMapOvr->SetParentFilePointer(this);

		if (transition.IsInit())
			transition->SetParentFilePointer(this);

		if (timing.IsInit())
			timing->SetParentFilePointer(this);
	}
	void Slide::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		WrapperFile::write(filename, directory, content);
		FileContainer::write(filename, directory, content);
	}
	const OOX::FileType Slide::type() const
	{
		return OOX::Presentation::FileTypes::Slide;
	}
	const OOX::CPath Slide::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath Slide::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	DWORD Slide::GetRGBAFromMap(const std::wstring& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Layout->GetRGBAFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Layout->GetRGBAFromMap(str);
		return Master->GetRGBAFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}
	DWORD Slide::GetARGBFromMap(const std::wstring& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Layout->GetARGBFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Layout->GetARGBFromMap(str);
		return Master->GetARGBFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}
	DWORD Slide::GetBGRAFromMap(const std::wstring& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Layout->GetBGRAFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Layout->GetBGRAFromMap(str);
		return Master->GetBGRAFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}
	DWORD Slide::GetABGRFromMap(const std::wstring& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Layout->GetABGRFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Layout->GetABGRFromMap(str);
		return Master->GetABGRFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}
	void Slide::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(NSBinPptxRW::NSMainTables::Slides);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteBool2(0, show);
		pWriter->WriteBool2(1, showMasterPhAnim);
		pWriter->WriteBool2(2, showMasterSp);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, cSld);
		pWriter->WriteRecord2(1, clrMapOvr);
		pWriter->WriteRecord2(2, transition);
		pWriter->WriteRecord2(3, timing);

		pWriter->WriteRecord2(4, comments);

		pWriter->EndRecord();
	}
	void Slide::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"p:sld");

		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"xmlns:a", PPTX::g_Namespaces.a.m_strLink);
		pWriter->WriteAttribute(L"xmlns:r", PPTX::g_Namespaces.r.m_strLink);
		pWriter->WriteAttribute(L"xmlns:p", PPTX::g_Namespaces.p.m_strLink);
		pWriter->WriteAttribute(L"xmlns:m", PPTX::g_Namespaces.m.m_strLink);
		pWriter->WriteAttribute(L"xmlns:w", PPTX::g_Namespaces.w.m_strLink);
		pWriter->WriteAttribute(L"xmlns:mc", L"http://schemas.openxmlformats.org/markup-compatibility/2006");

		pWriter->WriteAttribute(L"showMasterPhAnim", showMasterPhAnim);
		pWriter->WriteAttribute(L"showMasterSp", showMasterSp);
		pWriter->WriteAttribute(L"show", show);
		pWriter->EndAttributes();

		pWriter->Write(cSld);

		if (clrMapOvr.is_init())
			pWriter->Write(clrMapOvr);
		else
			pWriter->WriteString(L"<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>");

		pWriter->Write(transition);
		pWriter->Write(timing);

		pWriter->EndNode(L"p:sld");
	}
	void Slide::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
				show = pReader->GetBool();
			else if (1 == _at)
				showMasterPhAnim = pReader->GetBool();
			else if (2 == _at)
				showMasterSp = pReader->GetBool();
		}

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					cSld = new Logic::CSld(WrapperFile::m_pMainDocument);
					cSld->fromPPTY(pReader);
					break;
				}
				case 1:
				{
					clrMapOvr = new Logic::ClrMapOvr();
					clrMapOvr->fromPPTY(pReader);
					break;
				}
				case 2:
				{
					transition = new Logic::Transition();
					transition->fromPPTY(pReader);
					break;
				}
				case 3:
				{
					timing = new Logic::Timing();
					timing->fromPPTY(pReader);
					break;
				}
				case 4:
				{
					comments = new PPTX::Comments(OOX::File::m_pMainDocument);
					comments->fromPPTY(pReader);
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
	void Slide::ApplyRels()
	{
		Layout		= FileContainer::Get(OOX::Presentation::FileTypes::SlideLayout).smart_dynamic_cast<PPTX::SlideLayout>();//boost::shared_dynamic_cast<PPTX::SlideLayout, PPTX::File>(FileContainer::get(OOX::Presentation::FileTypes::SlideLayout));
		Note		= FileContainer::Get(OOX::Presentation::FileTypes::NotesSlide).smart_dynamic_cast<PPTX::NotesSlide>();
		comments	= FileContainer::Get(OOX::Presentation::FileTypes::SlideComments).smart_dynamic_cast<PPTX::Comments>();

		if (Layout.IsInit())
		{
			Master	= Layout->Master;
			theme	= Layout->theme;

			if (theme.IsInit())
			{
				tableStyles_= theme->presentation->Get(OOX::Presentation::FileTypes::TableStyles).smart_dynamic_cast<PPTX::TableStyles>();//boost::shared_dynamic_cast<PPTX::TableStyles, PPTX::File>(Theme->Presentation->get(OOX::Presentation::FileTypes::TableStyles));
			}
			if (IsExist(OOX::FileTypes::VmlDrawing))
			{
				Vml = FileContainer::Get(OOX::FileTypes::VmlDrawing).smart_dynamic_cast<OOX::CVmlDrawing>();
			}
		}
	}
	std::wstring Slide::GetVmlXmlBySpid(const std::wstring& spid)
	{
		if(Vml.is_init() && !spid.empty())
		{
			std::map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator pPair = Vml->m_mapShapes.find(spid);
			if (Vml->m_mapShapes.end() != pPair)
			{
				pPair->second.bUsed = true;
				return pPair->second.sXml;
			}
		}
		return L"";
	}
} // namespace PPTX
