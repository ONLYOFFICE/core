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

#include "SlideMaster.h"

namespace PPTX
{
		SlideMaster::SlideMaster(OOX::Document* pMain) : WrapperFile(pMain), FileContainer(pMain)
		{
		}
		SlideMaster::SlideMaster(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain), FileContainer(pMain)
		{
			read(filename, map);
		}
		SlideMaster::~SlideMaster()
		{
		}

		void SlideMaster::read(const OOX::CPath& filename, FileMap& map)
		{
			m_sOutputFilename = filename.GetFilename();

			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile(filename.m_strFilename);

			XmlMacroReadAttributeBase(oNode, L"preserve", preserve);

			cSld = oNode.ReadNode(_T("p:cSld"));
			cSld.SetParentFilePointer(this);

			clrMap = oNode.ReadNode(_T("p:clrMap"));
			clrMap.SetParentFilePointer(this);

			sldLayoutIdLst.clear();
			XmlUtils::CXmlNode oNodeList;
			if (oNode.GetNode(_T("p:sldLayoutIdLst"), oNodeList))
			{
				XmlMacroLoadArray(oNodeList, _T("p:sldLayoutId"), sldLayoutIdLst, Logic::XmlId);

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
		void SlideMaster::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
		{
			WrapperFile::write(filename, directory, content);
			FileContainer::write(filename, directory, content);
		}
		const OOX::FileType SlideMaster::type() const
		{
			return OOX::Presentation::FileTypes::SlideMaster;
		}
		const OOX::CPath SlideMaster::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const OOX::CPath SlideMaster::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		void SlideMaster::GetLevelUp(WrapperWritingElement* pElem)
		{
			Logic::Shape	*pShape	= dynamic_cast<PPTX::Logic::Shape*>(pElem);
			Logic::Pic		*pPic	= dynamic_cast<PPTX::Logic::Pic*>(pElem);

			if (!pShape && !pPic) return;

			Logic::NvPr *pNvPr = NULL;

			if (pShape)	pNvPr = &pShape->nvSpPr.nvPr;
			if (pPic)	pNvPr = &pPic->nvPicPr.nvPr;

			if (!pNvPr) return;
			if (pNvPr->ph.is_init() == false) return;

			std::wstring idx = pNvPr->ph->idx.get_value_or(_T("0"));
			std::wstring type = pNvPr->ph->type.get_value_or(_T("body"));

			if (type == _T("ctrTitle")) type = _T("title");

			for (size_t i = 0; i < cSld.spTree.SpTreeElems.size(); ++i)
			{
				smart_ptr<Logic::Shape> pMasterShape = cSld.spTree.SpTreeElems[i].GetElem().smart_dynamic_cast<Logic::Shape>();
				if (pMasterShape.IsInit())
				{
					if (pMasterShape->nvSpPr.nvPr.ph.is_init())
					{
						std::wstring lIdx	= pMasterShape->nvSpPr.nvPr.ph->idx.get_value_or(_T("0"));
						std::wstring lType	= pMasterShape->nvSpPr.nvPr.ph->type.get_value_or(_T("body"));

						if (lType == L"ctrTitle")	lType = L"title";
						if (type == lType)
						{
							if (pShape)	pShape->SetLevelUpElement(pMasterShape.GetPointer());
							if (pPic)	pPic->SetLevelUpElement(pMasterShape.GetPointer());

							return;
						}
					}
				}
			}
		}

//---------------------Colors from map---------------------------------------
		DWORD SlideMaster::GetRGBAFromMap(const std::wstring& str)const
		{
			return theme->GetRGBAFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		DWORD SlideMaster::GetARGBFromMap(const std::wstring& str)const
		{
			return theme->GetARGBFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		DWORD SlideMaster::GetBGRAFromMap(const std::wstring& str)const
		{
			return theme->GetBGRAFromScheme(clrMap.GetColorSchemeIndex(str));
		}

		DWORD SlideMaster::GetABGRFromMap(const std::wstring& str)const
		{
			return theme->GetABGRFromScheme(clrMap.GetColorSchemeIndex(str));
		}
//---------------------------Colors from scheme------------------------------
		DWORD SlideMaster::GetRGBAFromScheme(const std::wstring& str)const
		{
			return theme->GetRGBAFromScheme(str);
		}

		DWORD SlideMaster::GetARGBFromScheme(const std::wstring& str)const
		{
			return theme->GetARGBFromScheme(str);
		}

		DWORD SlideMaster::GetBGRAFromScheme(const std::wstring& str)const
		{
			return theme->GetBGRAFromScheme(str);
		}

		DWORD SlideMaster::GetABGRFromScheme(const std::wstring& str)const
		{
			return theme->GetABGRFromScheme(str);
		}

		void SlideMaster::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
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
		void SlideMaster::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
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
		void SlideMaster::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(1); // type
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

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

		void SlideMaster::ApplyRels()
		{
			theme = (FileContainer::Get(OOX::FileTypes::Theme)).smart_dynamic_cast<PPTX::Theme>();

			if (theme.IsInit())
			{
				theme->SetColorMap(clrMap);
				//Theme->Master.reset((PPTX::WrapperFile*)this);
				tableStyles = (theme->presentation->Get(OOX::Presentation::FileTypes::TableStyles)).smart_dynamic_cast<PPTX::TableStyles>();
			}

			if (IsExist(OOX::FileTypes::VmlDrawing))
			{
				Vml = FileContainer::Get(OOX::FileTypes::VmlDrawing).smart_dynamic_cast<OOX::CVmlDrawing>();//boost::shared_dynamic_cast<PPTX::VmlDrawing, PPTX::File>(FileContainer::get(OOX::Presentation::FileTypes::VmlDrawing));
			}
		}
		std::wstring SlideMaster::GetVmlXmlBySpid(const std::wstring& spid)
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
} // namespace PPTX
