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
#include "UniFill.h"
#include "../../SystemUtility/File.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/AreaFormat.h"

namespace PPTX
{
	namespace Logic
	{
		UniFill::UniFill()
		{
			m_type = notInit;
			Fill.reset();
		}
		OOX::EElementType UniFill::getType() const
		{
			if (Fill.IsInit())
				return Fill->getType();
			return OOX::et_Unknown;
		}
		void UniFill::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring name = XmlUtils::GetNameNoNS(oReader.GetName());
			
			if (name == _T("blipFill"))
			{
				m_type = blipFill;
				Fill.reset(CreatePtrXmlContent<Logic::BlipFill>(oReader));
			}
			else if(name == _T("noFill"))
			{
				m_type = noFill;
				Fill.reset(CreatePtrXmlContent<Logic::NoFill>(oReader));
			}
			else if(name == _T("solidFill"))
			{
				m_type = solidFill;
				Fill.reset(CreatePtrXmlContent<Logic::SolidFill>(oReader));
			}
			else if(name == _T("gradFill"))
			{
				m_type = gradFill;
				Fill.reset(CreatePtrXmlContent<Logic::GradFill>(oReader));
			}
			else if(name == _T("pattFill"))
			{
				m_type = pattFill;
				Fill.reset(CreatePtrXmlContent<Logic::PattFill>(oReader));
			}
			else if(name == _T("grpFill"))
			{
				m_type = grpFill;
				Fill.reset(CreatePtrXmlContent<Logic::GrpFill>(oReader));
			}
			else
			{
				m_type = notInit;
				Fill.reset();
			}	
		}
		void UniFill::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("blipFill"))
			{
				m_type = blipFill;
				Fill.reset(CreatePtrXmlContent<Logic::BlipFill>(node));
			}
			else if(name == _T("noFill"))
			{
				m_type = noFill;
				Fill.reset(CreatePtrXmlContent<Logic::NoFill>(node));
			}
			else if(name == _T("solidFill"))
			{
				m_type = solidFill;
				Fill.reset(CreatePtrXmlContent<Logic::SolidFill>(node));
			}
			else if(name == _T("gradFill"))
			{
				m_type = gradFill;
				Fill.reset(CreatePtrXmlContent<Logic::GradFill>(node));
			}
			else if(name == _T("pattFill"))
			{
				m_type = pattFill;
				Fill.reset(CreatePtrXmlContent<Logic::PattFill>(node));
			}
			else if(name == _T("grpFill"))
			{
				m_type = grpFill;
				Fill.reset(CreatePtrXmlContent<Logic::GrpFill>(node));
			}
			else
			{
				m_type = notInit;
				Fill.reset();
			}
		}
		void UniFill::GetFillFrom(XmlUtils::CXmlNode& element)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (element.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("blipFill") == strName)
					{
						m_type = blipFill;
						Fill.reset(CreatePtrXmlContent<Logic::BlipFill>(oNode));
						return;
					}
					if (_T("noFill") == strName)
					{
						m_type = noFill;
						Fill.reset(CreatePtrXmlContent<Logic::NoFill>(oNode));
						return;
					}
					if (_T("solidFill") == strName)
					{
						m_type = solidFill;
						Fill.reset(CreatePtrXmlContent<Logic::SolidFill>(oNode));
						return;
					}
					if (_T("gradFill") == strName)
					{
						m_type = gradFill;
						Fill.reset(CreatePtrXmlContent<Logic::GradFill>(oNode));
						return;
					}
					if (_T("pattFill") == strName)
					{
						m_type = pattFill;
						Fill.reset(CreatePtrXmlContent<Logic::PattFill>(oNode));
						return;
					}
					if (_T("grpFill") == strName)
					{
						m_type = grpFill;
						Fill.reset(CreatePtrXmlContent<Logic::GrpFill>(oNode));
						return;
					}
				}
			}

			m_type = notInit;
			Fill.reset();
		}
		void UniFill::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG read_start = pReader->GetPos();
			LONG read_end = read_start + pReader->GetRecordSize() + 4;

			m_type = notInit;

			if (pReader->GetPos() < read_end)
			{
				BYTE _type = pReader->GetUChar();
				pReader->Seek(read_start);

				switch (_type)
				{
					case FILL_TYPE_BLIP:
					{
						PPTX::Logic::BlipFill* pFill = new PPTX::Logic::BlipFill();
						pFill->m_namespace = _T("a");
						pFill->fromPPTY(pReader);

						m_type = blipFill;
						Fill = pFill;						
					}break;
					case FILL_TYPE_GRAD:
					{
						PPTX::Logic::GradFill* pFill = new PPTX::Logic::GradFill();
						pFill->m_namespace = _T("a");
						pFill->fromPPTY(pReader);

						m_type = gradFill;
						Fill = pFill;						
					}break;
					case FILL_TYPE_PATT:
					{
						PPTX::Logic::PattFill* pFill = new PPTX::Logic::PattFill();
						pFill->m_namespace = _T("a");
						pFill->fromPPTY(pReader);

						m_type = pattFill;
						Fill = pFill;						
					}break;
					case FILL_TYPE_SOLID:
					{
						PPTX::Logic::SolidFill* pFill = new PPTX::Logic::SolidFill();
						pFill->m_namespace = _T("a");
						pFill->fromPPTY(pReader);

						m_type = solidFill;
						Fill = pFill;						
					}break;
					case FILL_TYPE_NOFILL:
					{
						m_type = noFill;
						Fill = new PPTX::Logic::NoFill();		
					}break;
					case FILL_TYPE_GRP:
					{
						m_type = grpFill;
						Fill = new PPTX::Logic::GrpFill();						
					}break;
				}
			}
			pReader->Seek(read_end);
		}
		std::wstring UniFill::toXML() const
		{
			if (Fill.IsInit())
				return Fill->toXML();
			return _T("");
		}
		void UniFill::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (Fill.is_init())
				Fill->toPPTY(pWriter);
		}
		void UniFill::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (Fill.is_init())
			{
				if (false == m_namespace.empty())
				{
					PPTX::Logic::SolidFill* solid = dynamic_cast<PPTX::Logic::SolidFill*>(Fill.GetPointer());
					if (solid) solid->m_namespace = m_namespace;
					else
					{
						PPTX::Logic::GradFill* grad = dynamic_cast<PPTX::Logic::GradFill*>(Fill.GetPointer());
						if (grad) grad->m_namespace = m_namespace;
						else
						{
							PPTX::Logic::BlipFill* blip = dynamic_cast<PPTX::Logic::BlipFill*>(Fill.GetPointer());
							if (blip) blip->m_namespace = m_namespace;
						}
					}
				}
				Fill->toXmlWriter(pWriter);
			}
		}
		XLS::BaseObjectPtr UniFill::toXLS() const
		{
			auto ptr = new XLS::AreaFormat;
			if (Fill.is_init())
			{
				if(m_type == UniFill::Type::noFill)
					ptr->fls = 0;
				else
				{
					if(m_type == UniFill::Type::solidFill)
					{
						ptr->fls = 1;
						auto solid = dynamic_cast<PPTX::Logic::SolidFill*>(Fill.GetPointer());
						auto rgba = solid->Color.Color->GetRGBA(0);
						ptr->rgbFore.red = GETBITS(rgba, 0, 1);
						ptr->rgbFore.green = GETBITS(rgba, 2, 3);
						ptr->rgbFore.blue = GETBITS(rgba, 4, 5);
					}
					else if(m_type == UniFill::Type::pattFill)
					{
						auto patt = dynamic_cast<PPTX::Logic::PattFill*>(Fill.GetPointer());

						auto rgba = patt->fgClr.Color->GetRGBA(0);
						ptr->rgbFore.red = GETBITS(rgba, 0, 1);
						ptr->rgbFore.green = GETBITS(rgba, 2, 3);
						ptr->rgbFore.blue = GETBITS(rgba, 4, 5);

						rgba = patt->bgClr.Color->GetRGBA(0);
						ptr->rgbBack.red = GETBITS(rgba, 0, 1);
						ptr->rgbBack.green = GETBITS(rgba, 2, 3);
						ptr->rgbBack.blue = GETBITS(rgba, 4, 5);
						if(patt->prst->get() == L"pct50")
							ptr->fls = 2;
						else if(patt->prst->get() == L"pct25")
							ptr->fls = 4;
						else if(patt->prst->get() == L"dkHorz")
							ptr->fls = 5;
						else if(patt->prst->get() == L"dkVert")
							ptr->fls = 6;
						else if(patt->prst->get() == L"dnDiag")
							ptr->fls = 7;
						else if(patt->prst->get() == L"upDiag")
							ptr->fls = 8;
						else if(patt->prst->get() == L"smGrid")
							ptr->fls = 9;
						else if(patt->prst->get() == L"trellis")
							ptr->fls = 0xA;
						else if(patt->prst->get() == L"narHorz")
							ptr->fls = 0xB;
						else if(patt->prst->get() == L"ltVert")
							ptr->fls = 0xC;
						else if(patt->prst->get() == L"dashDnDiag")
							ptr->fls = 0xD;
						else if(patt->prst->get() == L"dashUpDiag")
							ptr->fls = 0xE;
						else if(patt->prst->get() == L"horzBrick")
							ptr->fls = 0xF;
						else if(patt->prst->get() == L"pct30")
							ptr->fls = 0x10;
						else if(patt->prst->get() == L"dotDmnd")
							ptr->fls = 0x11;
						else if(patt->prst->get() == L"pct5")
							ptr->fls = 0x12;
					}
				}
			}
			return XLS::BaseObjectPtr(ptr);
		}
		bool UniFill::is_init() const
		{
			return (Fill.IsInit());
		}
		const UniFill& UniFill::operator+=(const UniFill& fill)
		{
			if(!fill.is_init())
				return (*this);
			*this = fill;
			return (*this);
		}
		void UniFill::Merge(UniFill& fill)const
		{
			if(!fill.is_init())
			{
				fill = *this;
				return;
			}

			if(m_type != fill.m_type)
			{
				fill = *this;
				return;
			}
			if(m_type == solidFill)
			{
				as<SolidFill>().Merge(fill.as<SolidFill>());
				return;
			}
			if(m_type == gradFill)
			{
				as<GradFill>().Merge(fill.as<GradFill>());
				return;
			}
		}
		void UniFill::FillParentPointersForChilds()
		{
			if(is_init())
				Fill->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
