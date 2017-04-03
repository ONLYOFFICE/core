/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef PPTX_SLIDES_SLIDEMASTER_INCLUDE_H_
#define PPTX_SLIDES_SLIDEMASTER_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"

#include "Logic/ClrMap.h"
#include "Logic/CSld.h"
#include "Logic/Transitions/Transition.h"
#include "Logic/Timing/Timing.h"
#include "Logic/Hf.h"
#include "Logic/TxStyles.h"
#include "Logic/XmlId.h"

#include "Logic/Shape.h"
#include "Logic/ShapeProperties.h"
#include "Logic/TxBody.h"
#include "Logic/UniColor.h"

#include "Theme.h"
#include "TableStyles.h"

#include "../../Common/DocxFormat/Source/DocxFormat/Media/Image.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/OleObject.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/HyperLink.h"
#include "../../Common/DocxFormat/Source/DocxFormat/VmlDrawing.h"

namespace PPTX
{
	class SlideMaster : public WrapperFile, public FileContainer
	{
	public:
		SlideMaster()
		{
		}
		SlideMaster(const OOX::CPath& filename, FileMap& map)
		{
			read(filename, map);
		}
		virtual ~SlideMaster()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			//FileContainer::read(filename, map);			
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile(filename.m_strFilename);

			oNode.ReadAttributeBase(L"preserve", preserve);
			
			cSld = oNode.ReadNode(_T("p:cSld"));
			cSld.SetParentFilePointer(this);

			clrMap = oNode.ReadNode(_T("p:clrMap"));
			clrMap.SetParentFilePointer(this);

			sldLayoutIdLst.clear();
			XmlUtils::CXmlNode oNodeList;
			if (oNode.GetNode(_T("p:sldLayoutIdLst"), oNodeList))
			{
				oNodeList.LoadArray(_T("p:sldLayoutId"), sldLayoutIdLst);

				size_t count = sldLayoutIdLst.size();
				for (size_t i = 0; i < count; ++i)
					sldLayoutIdLst[i].SetParentFilePointer(this);
			}

			transition = oNode.ReadNode(_T("p:transition"));
			if(transition.is_init())
				transition->SetParentFilePointer(this);

			timing = oNode.ReadNode(_T("p:timing"));
			if(timing.is_init())
				timing->SetParentFilePointer(this);

			hf = oNode.ReadNode(_T("p:hf"));
			if(hf.is_init())
				hf->SetParentFilePointer(this);

			txStyles = oNode.ReadNode(_T("p:txStyles"));
			if(txStyles.is_init())
				txStyles->SetParentFilePointer(this);
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
			oAttr.Write(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			oAttr.Write(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			oAttr.Write(_T("xmlns:m"), PPTX::g_Namespaces.m.m_strLink);
			oAttr.Write(_T("xmlns:w"), PPTX::g_Namespaces.w.m_strLink);
			oAttr.Write(_T("preserve"), preserve);

			XmlUtils::CNodeValue oValue;
			oValue.Write(cSld);
			oValue.Write(clrMap);
			oValue.WriteNullable(transition);
			oValue.WriteNullable(timing);
			oValue.WriteNullable(hf);
			oValue.WriteNullable(txStyles);
			oValue.WriteArray(_T("p:sldLayoutIdLst"), sldLayoutIdLst);

			XmlUtils::SaveToFile(filename.m_strFilename, XmlUtils::CreateNode(_T("p:sldMaster"), oAttr, oValue));
			
			content.Registration(type().OverrideType(), directory, filename);
			m_written = true;
			m_WrittenFileName = filename.GetFilename();
			FileContainer::write(filename, directory, content);
		}

	public:
		virtual const OOX::FileType type() const
		{
			return OOX::Presentation::FileTypes::SlideMaster;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

	public:
		void GetLevelUp(const Logic::Shape& pShape)const
		{
			if(pShape.nvSpPr.nvPr.ph.is_init())
			{
				std::wstring idx = /*pShape.nvSpPr->nvPr->ph.is_init()?*/pShape.nvSpPr.nvPr.ph->idx.get_value_or(_T("0"));//:"";
				std::wstring type = /*pShape.nvSpPr->nvPr->ph.is_init()?*/pShape.nvSpPr.nvPr.ph->type.get_value_or(_T("body"));//:"";
				if(type == _T("ctrTitle"))
					type = _T("title");

				size_t count = cSld.spTree.SpTreeElems.size();
				for (size_t i = 0; i < count; ++i)
				{
					const PPTX::Logic::SpTreeElem* pElem = &cSld.spTree.SpTreeElems[i];
					if(pElem->is<Logic::Shape>())
					{
						const Logic::Shape& MasterShape = pElem->as<Logic::Shape>();
						if(MasterShape.nvSpPr.nvPr.ph.is_init())
						{
							std::wstring lIdx = /*MasterShape->nvSpPr->nvPr->ph.is_init()?*/MasterShape.nvSpPr.nvPr.ph->idx.get_value_or(_T("0"));//:"";
							std::wstring lType = /*MasterShape->nvSpPr->nvPr->ph.is_init()?*/MasterShape.nvSpPr.nvPr.ph->type.get_value_or(_T("body"));//:"";
							if(lType == _T("ctrTitle"))
								lType = _T("title");
							if(type == lType)
							{
								pShape.SetLevelUpElement(MasterShape);
								return;
							}

							//if(idx == MasterShape->nvSpPr->nvPr->idx.get_value_or("0"))
							//{
							//	if((type == MasterShape->nvSpPr->nvPr->type.get_value_or("")) || ((type == "") && (MasterShape->nvSpPr->nvPr->type.get_value_or("") != "")))
							//		pShape->SetLevelUpElement(MasterShape);
							//	return;
							//}
							//if((type == lType) && (type != ""))
							//{
							//	//if(idx == lIdx)
							//	//{
							//		pShape.SetLevelUpElement(MasterShape);
							//		return;
							//	//}
							//	//continue;
							//}
							//if((type == lType) && (type == ""))
							//{
							//	if((idx == lType) && (idx != ""))
							//	{
							//		pShape.SetLevelUpElement(MasterShape);
							//		return;
							//	}
							//}
							//if(type != lType)
							//{
							//	if((idx == lIdx) && (idx != ""))
							//	{
							//		pShape.SetLevelUpElement(MasterShape);
							//		return;
							//	}
							//}
						}
					}
				}
			}
		}
		void FillShapeProperties(Logic::ShapeProperties& props, const std::wstring& type)const
		{
            if((theme.IsInit()) && (theme->presentation.IsInit()))
			{
                PPTX::Presentation* pPres = const_cast<PPTX::Presentation*>(theme->presentation.operator->());

				pPres->SetClrMap(clrMap);
                pPres->SetClrScheme(theme->themeElements.clrScheme);
			}
            if((theme.IsInit()) && (type != _T("")))
                theme->FillShapeProperties(props, type);

			if(txStyles.is_init())
			{
	//			props.FillFromTextListStyle(txStyles->otherStyle, true);

				//if(type == "")
				//{
				//	if(Theme->spDef.is_init())
				//	{
				//		props.FillFromTextListStyle(Theme->spDef->lstStyle, true);
				//		if(Theme->spDef->style.is_init())
				//			props.FillFontRef(Theme->spDef->style->fontRef.get());
				//	}
				//	else
				//		props.FillFromTextListStyle(txStyles->otherStyle, true);
				//	return;
				//}

				if((type == _T("title")) || (type == _T("ctrTitle")))
				{
					props.FillFromTextListStyle(txStyles->titleStyle);
					props.SetTextType(1);
				}
				else if((type == _T("body")) || (type == _T("subTitle")) || (type == _T("obj")))
				{
					props.FillFromTextListStyle(txStyles->bodyStyle);
					props.SetTextType(2);
				}
				else if(type != _T(""))
				{
					props.FillFromTextListStyle(txStyles->otherStyle);
					props.SetTextType(3);
				}
				else
				{
					props.FillFromTextListStyle(txStyles->otherStyle);
					props.SetTextType(3);

                    if(theme.IsInit())
                        theme->FillShapeProperties(props, type);
				}
			}
		}
		void FillShapeTextProperties(Logic::CShapeTextProperties& props, const std::wstring& type)const
		{
            if((theme.IsInit()) && (theme->presentation.IsInit()))
			{
                PPTX::Presentation* pPres = const_cast<PPTX::Presentation*>(theme->presentation.operator->());

				pPres->SetClrMap(clrMap);
                pPres->SetClrScheme(theme->themeElements.clrScheme);
			}

			if (type == _T("table-cell"))
				props.FillMasterFontSize(1800);

			if ((type == _T("title")) || (type == _T("ctrTitle")))
			{
				props.FillTextType(1);
			}
			else if ((type == _T("body")) || (type == _T("subTitle")) || (type == _T("obj")))
			{
				props.FillTextType(2);
			}
			else if (type != _T(""))
			{
				props.FillTextType(3);
			}
			else
			{
				props.FillTextType(0);
			}
		}
		void GetBackground(Logic::BgPr& bg, DWORD& ARGB)const
		{
			if(cSld.bg.is_init())
			{
				if(cSld.bg->bgPr.is_init())
					bg = cSld.bg->bgPr.get();
				else if(cSld.bg->bgRef.is_init())
				{
					ARGB = cSld.bg->bgRef->Color.GetARGB();
                    theme->themeElements.fmtScheme.GetFillStyle(cSld.bg->bgRef->idx.get_value_or(0), bg.Fill);
	//					bg.SetParentFilePointer(this);
				}
			}
		}
		virtual std::wstring GetMediaFullPathNameFromRId(const OOX::RId& rid)const
		{
			smart_ptr<OOX::Image> p = GetImage(rid);
			if (!p.is_init())
				return _T("");
			return p->filename().m_strFilename;
		}
		virtual std::wstring GetFullHyperlinkNameFromRId(const OOX::RId& rid)const
		{
			smart_ptr<OOX::HyperLink> p = GetHyperlink(rid);
			if (!p.is_init())
				return _T("");
			return p->Uri().m_strFilename;
		}
		virtual std::wstring GetOleFromRId(const OOX::RId& rid)const
		{
			smart_ptr<OOX::OleObject> p = GetOleObject(rid);
			if (!p.is_init())
				return _T("");
			return p->filename().m_strFilename;
		}

		//---------------------Colors from map---------------------------------------

		DWORD GetRGBAFromMap(const std::wstring& str)const
		{
            return theme->GetRGBAFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		DWORD GetARGBFromMap(const std::wstring& str)const
		{
            return theme->GetARGBFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		DWORD GetBGRAFromMap(const std::wstring& str)const
		{
            return theme->GetBGRAFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		DWORD GetABGRFromMap(const std::wstring& str)const
		{
            return theme->GetABGRFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		//---------------------------Colors from scheme------------------------------

		DWORD GetRGBAFromScheme(const std::wstring& str)const
		{
            return theme->GetRGBAFromScheme(str);
		}

		DWORD GetARGBFromScheme(const std::wstring& str)const
		{
            return theme->GetARGBFromScheme(str);
		}

		DWORD GetBGRAFromScheme(const std::wstring& str)const
		{
            return theme->GetBGRAFromScheme(str);
		}

		DWORD GetABGRFromScheme(const std::wstring& str)const
		{
            return theme->GetABGRFromScheme(str);
		}

		//void ApplyColors();
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(NSBinPptxRW::NSMainTables::SlideMasters);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, preserve);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, cSld);
			pWriter->WriteRecord1(1, clrMap);
			pWriter->WriteRecordArray(2, 0, sldLayoutIdLst);
			pWriter->WriteRecord2(3, transition);
			pWriter->WriteRecord2(4, timing);
			pWriter->WriteRecord2(5, hf);
			pWriter->WriteRecord2(6, txStyles);

			pWriter->EndRecord();
		}

		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:sldMaster"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:m"), PPTX::g_Namespaces.m.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:w"), PPTX::g_Namespaces.w.m_strLink);
			pWriter->WriteAttribute(_T("preserve"), preserve);
			pWriter->EndAttributes();

			cSld.toXmlWriter(pWriter);
			clrMap.toXmlWriter(pWriter);
			pWriter->Write(transition);
			pWriter->Write(timing);
			pWriter->WriteArray(_T("p:sldLayoutIdLst"), sldLayoutIdLst);
			pWriter->Write(hf);
			pWriter->Write(txStyles);			

			pWriter->EndNode(_T("p:sldMaster"));
		}
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(1); // type
			LONG end = pReader->GetPos() + pReader->GetLong() + 4;

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
						preserve = pReader->GetBool();
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
						clrMap.m_name = _T("p:clrMap");
						clrMap.fromPPTY(pReader);
						break;
					}
					case 2:
					case 3:
					case 4:
					{
						pReader->SkipRecord();
						break;
					}
					case 5:
					{
						hf = new Logic::HF();
						hf->fromPPTY(pReader);
						break;
					}
					case 6:
					{
						txStyles = new Logic::TxStyles();
						txStyles->fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(end);
		}

	public:
		Logic::CSld					cSld;
		Logic::ClrMap				clrMap;
		std::vector<Logic::XmlId>	sldLayoutIdLst;
		nullable<Logic::Transition> transition;
		nullable<Logic::Timing>		timing;
		nullable<Logic::HF>			hf;
		nullable<Logic::TxStyles>	txStyles;
		nullable_bool				preserve;

        smart_ptr<Theme>			theme;
        smart_ptr<TableStyles>		tableStyles;
		smart_ptr<OOX::CVmlDrawing>	Vml;
		
	public:		
		void ApplyRels()
		{
            theme = (FileContainer::Get(OOX::Presentation::FileTypes::ThemePPTX)).smart_dynamic_cast<PPTX::Theme>();

            if (theme.IsInit())
			{
                theme->SetColorMap(clrMap);
				//Theme->Master.reset((PPTX::WrapperFile*)this);
				tableStyles = (theme->presentation->Get(OOX::Presentation::FileTypes::TableStyles)).smart_dynamic_cast<PPTX::TableStyles>();
			}

			if (IsExist(OOX::Presentation::FileTypes::VmlDrawing))
			{
				Vml = FileContainer::Get(OOX::Presentation::FileTypes::VmlDrawing).smart_dynamic_cast<OOX::CVmlDrawing>();//boost::shared_dynamic_cast<PPTX::VmlDrawing, PPTX::File>(FileContainer::get(OOX::Presentation::FileTypes::VmlDrawing));
			}
		}

		std::wstring GetVmlXmlBySpid(const std::wstring& spid)
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
	};
} // namespace PPTX

#endif // PPTX_SLIDES_SLIDEMASTER_INCLUDE_H_
