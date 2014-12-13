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

#include "Theme.h"
#include "TableStyles.h"
#include "VmlDrawing.h"

#include "DocxFormat/Media/Image.h"
#include "DocxFormat/External/HyperLink.h"
#include "Logic/UniColor.h"

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
			oNode.FromXmlFile2(filename.m_strFilename);

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
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, PPTX::ContentTypes::File& content)const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
			oAttr.Write(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			oAttr.Write(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
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
			
			content.registration(type().OverrideType(), directory, filename);
			m_written = true;
			m_WrittenFileName = filename.GetFilename();
			FileContainer::write(filename, directory, content);
		}

	public:
		virtual const PPTX::FileType type() const
		{
			return PPTX::FileTypes::SlideMaster;
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
				CString idx = /*pShape.nvSpPr->nvPr->ph.is_init()?*/pShape.nvSpPr.nvPr.ph->idx.get_value_or(_T("0"));//:"";
				CString type = /*pShape.nvSpPr->nvPr->ph.is_init()?*/pShape.nvSpPr.nvPr.ph->type.get_value_or(_T("body"));//:"";
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
							CString lIdx = /*MasterShape->nvSpPr->nvPr->ph.is_init()?*/MasterShape.nvSpPr.nvPr.ph->idx.get_value_or(_T("0"));//:"";
							CString lType = /*MasterShape->nvSpPr->nvPr->ph.is_init()?*/MasterShape.nvSpPr.nvPr.ph->type.get_value_or(_T("body"));//:"";
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
		void FillShapeProperties(Logic::ShapeProperties& props, const CString& type)const
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
		void FillShapeTextProperties(Logic::CShapeTextProperties& props, const CString& type)const
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
		virtual CString GetMediaFullPathNameFromRId(const PPTX::RId& rid)const
		{
			smart_ptr<PPTX::Image> p = image(rid);
			if (!p.is_init())
				return _T("");
			return p->filename().m_strFilename;
		}
		virtual CString GetFullHyperlinkNameFromRId(const PPTX::RId& rid)const
		{
			smart_ptr<PPTX::HyperLink> p = hyperlink(rid);
			if (!p.is_init())
				return _T("");
			return p->Uri().m_strFilename;
		}


		//---------------------Colors from map---------------------------------------

		DWORD GetRGBAFromMap(const CString& str)const
		{
            return theme->GetRGBAFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		DWORD GetARGBFromMap(const CString& str)const
		{
            return theme->GetARGBFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		DWORD GetBGRAFromMap(const CString& str)const
		{
            return theme->GetBGRAFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		DWORD GetABGRFromMap(const CString& str)const
		{
            return theme->GetABGRFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		//---------------------------Colors from scheme------------------------------

		DWORD GetRGBAFromScheme(const CString& str)const
		{
            return theme->GetRGBAFromScheme(str);
		}

		DWORD GetARGBFromScheme(const CString& str)const
		{
            return theme->GetARGBFromScheme(str);
		}

		DWORD GetBGRAFromScheme(const CString& str)const
		{
            return theme->GetBGRAFromScheme(str);
		}

		DWORD GetABGRFromScheme(const CString& str)const
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
				BYTE _at = pReader->GetUChar();
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
		smart_ptr<VmlDrawing>		Vml;
		
	public:		
		void ApplyRels()
		{
            theme = (FileContainer::get(PPTX::FileTypes::ThemePPTX)).smart_dynamic_cast<PPTX::Theme>();

            if (theme.IsInit())
                theme->SetColorMap(clrMap);
			//Theme->Master.reset((PPTX::WrapperFile*)this);
            tableStyles = (theme->presentation->get(PPTX::FileTypes::TableStyles)).smart_dynamic_cast<PPTX::TableStyles>();
			
			if (exist(PPTX::FileTypes::VmlDrawing))
			{
				Vml = FileContainer::get(PPTX::FileTypes::VmlDrawing).smart_dynamic_cast<PPTX::VmlDrawing>();//boost::shared_dynamic_cast<PPTX::VmlDrawing, PPTX::File>(FileContainer::get(PPTX::FileTypes::VmlDrawing));
				Vml->FillRIds();
			}
		}
		const OOX::CPath GetPathBySpid(const CString& spid)const
		{
            OOX::CPath filename;
			if((Vml.is_init()) && (spid != _T("")))
			{
				std::map<CString, OOX::CPath>::const_iterator pPair = Vml->SpIds.find(spid);
				if (Vml->SpIds.end() != pPair)
					filename = pPair->second;
			}
			return filename;
		}
	};
} // namespace PPTX

#endif // PPTX_SLIDES_SLIDEMASTER_INCLUDE_H_
