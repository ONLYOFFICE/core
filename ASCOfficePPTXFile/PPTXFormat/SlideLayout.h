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
#ifndef PPTX_SLIDES_SLIDELAYOUT_INCLUDE_H_
#define PPTX_SLIDES_SLIDELAYOUT_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"
#include "FileTypes.h"

#include "Logic/CSld.h"
#include "Logic/ClrMapOvr.h"
#include "Logic/Transitions/Transition.h"
#include "Logic/Timing/Timing.h"
#include "Logic/Hf.h"
#include "Limit/SlideLayoutType.h"

#include "Logic/SpTreeElem.h"
#include "Logic/Shape.h"

#include "Theme.h"
#include "SlideMaster.h"
#include "TableStyles.h"

#include "../../Common/DocxFormat/Source/DocxFormat/Media/Image.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/OleObject.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/HyperLink.h"
#include "../../Common/DocxFormat/Source/DocxFormat/VmlDrawing.h"


namespace PPTX
{
	class SlideLayout : public WrapperFile, public FileContainer
	{
	public:
		SlideLayout(OOX::Document* pMain) : WrapperFile(pMain), FileContainer(pMain)
		{
		}
		SlideLayout(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain), FileContainer(pMain)
		{
			read(filename, map);
		}
		virtual ~SlideLayout()
		{
		}
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			m_sOutputFilename = filename.GetFilename();

			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile(filename.m_strFilename);

			cSld = oNode.ReadNode(_T("p:cSld"));
			cSld.SetParentFilePointer(this);

			clrMapOvr = oNode.ReadNode(_T("p:clrMapOvr"));
			if(clrMapOvr.is_init())
				clrMapOvr->SetParentFilePointer(this);

			transition = oNode.ReadNode(_T("p:transition"));
			if(transition.is_init())
				transition->SetParentFilePointer(this);

			timing = oNode.ReadNode(_T("p:timing"));
			if(timing.is_init())
				timing->SetParentFilePointer(this);

			hf = oNode.ReadNode(_T("p:hf"));
			if(hf.is_init())
				hf->SetParentFilePointer(this);

            XmlMacroReadAttributeBase(oNode, L"matchingName", matchingName);
            XmlMacroReadAttributeBase(oNode, L"preserve", preserve);
            XmlMacroReadAttributeBase(oNode, L"showMasterPhAnim", showMasterPhAnim);
            XmlMacroReadAttributeBase(oNode, L"showMasterSp", showMasterSp);
            XmlMacroReadAttributeBase(oNode, L"type", attrType);
            XmlMacroReadAttributeBase(oNode, L"userDrawn", userDrawn);
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
		{
			WrapperFile::write(filename, directory, content);
			FileContainer::write(filename, directory, content);
		}

		virtual const OOX::FileType type() const
		{
			return OOX::Presentation::FileTypes::SlideLayout;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		virtual void GetLevelUp(WrapperWritingElement* pElem)
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
				smart_ptr<Logic::Shape> pLayoutShape = cSld.spTree.SpTreeElems[i].GetElem().smart_dynamic_cast<Logic::Shape>();

				if (pLayoutShape.IsInit())
				{
					if (pLayoutShape->nvSpPr.nvPr.ph.is_init())
					{
						std::wstring lIdx	= pLayoutShape->nvSpPr.nvPr.ph->idx.get_value_or(_T(""));
						std::wstring lType	= pLayoutShape->nvSpPr.nvPr.ph->type.get_value_or(_T("body"));
						
						if (lType == L"ctrTitle") lType = L"title";

						if ((type == lType) && (idx == lIdx) && !idx.empty())
						{
							if (pShape)	pShape->SetLevelUpElement(pLayoutShape.GetPointer());
							if (pPic)	pPic->SetLevelUpElement(pLayoutShape.GetPointer());
							
							return;
						}
						else if ((type == lType) && idx.empty() && lIdx.empty())
						{
							if (pShape)	pShape->SetLevelUpElement(pLayoutShape.GetPointer());
							if (pPic)	pPic->SetLevelUpElement(pLayoutShape.GetPointer());
							
							return;
						}
					}
				}
			}

			if (pNvPr->ph->idx.IsInit())
			{
				//not found in layout !! 100818_건강보험과_보건의료_김용익_최종.pptx
				bool bShapeMaster = showMasterSp.get_value_or(true);
				if (Master.IsInit() && bShapeMaster)
				{
					Master->GetLevelUp(pElem);
				}
			}
		}
		virtual DWORD GetRGBAFromMap(const std::wstring& str)const
		{
			if(!(clrMapOvr.is_init()))
				return Master->GetRGBAFromMap(str);
			if(!(clrMapOvr->overrideClrMapping.is_init()))
				return Master->GetRGBAFromMap(str);
			return Master->GetRGBAFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
		}

		virtual DWORD GetARGBFromMap(const std::wstring& str)const
		{
			if(!(clrMapOvr.is_init()))
				return Master->GetARGBFromMap(str);
			if(!(clrMapOvr->overrideClrMapping.is_init()))
				return Master->GetARGBFromMap(str);
			return Master->GetARGBFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
		}

		virtual DWORD GetBGRAFromMap(const std::wstring& str)const
		{
			if(!(clrMapOvr.is_init()))
				return Master->GetBGRAFromMap(str);
			if(!(clrMapOvr->overrideClrMapping.is_init()))
				return Master->GetBGRAFromMap(str);
			return Master->GetBGRAFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
		}

		virtual DWORD GetABGRFromMap(const std::wstring& str)const
		{
			if(!(clrMapOvr.is_init()))
				return Master->GetABGRFromMap(str);
			if(!(clrMapOvr->overrideClrMapping.is_init()))
				return Master->GetABGRFromMap(str);
			return Master->GetABGRFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
		}

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(NSBinPptxRW::NSMainTables::SlideLayouts);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
            pWriter->WriteString2(0, matchingName);
            pWriter->WriteBool2(1, preserve);
            pWriter->WriteBool2(2, showMasterPhAnim);
            pWriter->WriteBool2(3, showMasterSp);
            pWriter->WriteBool2(4, userDrawn);
            pWriter->WriteLimit2(5, attrType);
            pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

            pWriter->WriteRecord1(0, cSld);
            pWriter->WriteRecord2(1, clrMapOvr);
            pWriter->WriteRecord2(2, transition);
            pWriter->WriteRecord2(3, timing);
            pWriter->WriteRecord2(4, hf);

			pWriter->EndRecord();
		}
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
            LONG start = pReader->GetPos();

            pReader->Skip(1); // type

            LONG size   = pReader->GetULong();
            LONG end    = start + 1 + size + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						matchingName = pReader->GetString2();
						break;
					}
					case 1:
					{
						preserve = pReader->GetBool();
						break;
					}
					case 2:
					{
						showMasterPhAnim = pReader->GetBool();
						break;
					}
					case 3:
					{
						showMasterSp = pReader->GetBool();
						break;
					}
					case 4:
					{
						userDrawn = pReader->GetBool();
						break;
					}
					case 5:
					{
						attrType = new Limit::SlideLayoutType();
						attrType->SetBYTECode(pReader->GetUChar());
						break;
					}
					default:
						break;
				}
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
					case 4:
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

		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:sldLayout"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:m"), PPTX::g_Namespaces.m.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:w"), PPTX::g_Namespaces.w.m_strLink);

			pWriter->WriteAttribute(_T("matchingName"), matchingName);
			pWriter->WriteAttribute(_T("preserve"), preserve);
			pWriter->WriteAttribute(_T("showMasterPhAnim"), showMasterPhAnim);
			pWriter->WriteAttribute(_T("showMasterSp"), showMasterSp);
			pWriter->WriteAttribute(_T("type"), attrType);
			pWriter->WriteAttribute(_T("userDrawn"), userDrawn);
			pWriter->EndAttributes();

			cSld.toXmlWriter(pWriter);

			if (clrMapOvr.is_init())
				pWriter->Write(clrMapOvr);
			else
				pWriter->WriteString(_T("<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));

			pWriter->Write(transition);
			pWriter->Write(timing);
			pWriter->Write(hf);

			pWriter->EndNode(_T("p:sldLayout"));
		}

	public:
		Logic::CSld						cSld;
		nullable<Logic::ClrMapOvr>		clrMapOvr;
		nullable<Logic::Transition>		transition;
		nullable<Logic::Timing>			timing;
		nullable<Logic::HF>				hf;

		// Attributes
		nullable_string							matchingName;
		nullable_bool							preserve;
		nullable_bool							showMasterPhAnim;
		nullable_bool							showMasterSp;
		nullable_limit<Limit::SlideLayoutType>	attrType;
		nullable_bool							userDrawn;

		smart_ptr<SlideMaster>					Master;
		smart_ptr<Theme>						theme;
        smart_ptr<TableStyles>					tableStyles;
		smart_ptr<OOX::CVmlDrawing>				Vml;
		
	public:
		void ApplyRels()
		{
			smart_ptr<OOX::File> pFile = FileContainer::Get(OOX::Presentation::FileTypes::SlideMaster);

			Master = pFile.smart_dynamic_cast<PPTX::SlideMaster>();

			if (Master.IsInit())
			{
				theme = Master->theme;
			
				if (theme.IsInit())
				{	
					tableStyles = theme->presentation->Get(OOX::Presentation::FileTypes::TableStyles).smart_dynamic_cast<PPTX::TableStyles>();
				}
			}
			if (IsExist(OOX::FileTypes::VmlDrawing))
			{
				Vml = FileContainer::Get(OOX::FileTypes::VmlDrawing).smart_dynamic_cast<OOX::CVmlDrawing>();//boost::shared_dynamic_cast<PPTX::VmlDrawing, PPTX::File>(FileContainer::get(OOX::Presentation::FileTypes::VmlDrawing));
			}
		}

		std::wstring GetVmlXmlBySpid(const std::wstring& spid)
		{
			if(Vml.is_init() && !spid.empty())
			{
                boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator pPair = Vml->m_mapShapes.find(spid);
				if (Vml->m_mapShapes.end() != pPair)
				{
					pPair->second.bUsed = true;
					return pPair->second.sXml;
				}
			}
			return L"";
		}
	};
} // namespace PPTX

#endif // PPTX_SLIDES_SLIDELAYOUT_INCLUDE_H_
