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

#include "SpTree.h"
#include "Shape.h"
#include "Pic.h"
#include "../Theme.h"
#include "ClrMap.h"

namespace PPTX
{
	namespace Logic
	{
		SpTree::SpTree(std::wstring ns) : nvGrpSpPr(ns), grpSpPr(ns)
		{
			m_namespace		= ns;
			m_lGroupIndex	= 0;
		}
		SpTree& SpTree::operator=(const SpTree& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			nvGrpSpPr	= oSrc.nvGrpSpPr;
			grpSpPr		= oSrc.grpSpPr;

			for (size_t i=0; i < oSrc.SpTreeElems.size(); i++)
				SpTreeElems.push_back(oSrc.SpTreeElems[i]);

			m_namespace		= oSrc.m_namespace;
			m_lGroupIndex	= oSrc.m_lGroupIndex;

			return *this;
		}
		OOX::EElementType SpTree::getType () const
		{
			return OOX::et_p_ShapeTree;
		}
		void SpTree::FillParentPointersForChilds()
	{
		nvGrpSpPr.SetParentPointer(this);
		grpSpPr.SetParentPointer(this);

		for (size_t i = 0; i < SpTreeElems.size(); ++i)
			SpTreeElems[i].SetParentPointer(this);
	}
		void SpTree::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			SpTreeElems.clear();

			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (strName == L"nvGrpSpPr")
					nvGrpSpPr.fromXML(oReader);
				else if (strName == L"grpSpPr")
					grpSpPr.fromXML(oReader);
				else if (_T("cNvPr") == strName)
				{
					nvGrpSpPr.cNvPr = oReader;
				}
				else if (_T("cNvGrpSpPr") == strName)
				{
					nvGrpSpPr.cNvGrpSpPr = oReader;
				}
				else
				{
					SpTreeElem elem(oReader);
					if (elem.is_init())
					{
						if (elem.getType() == OOX::et_p_ShapeTree)
						{
							smart_ptr<SpTree> e = elem.GetElem().smart_dynamic_cast<SpTree>();
							e->m_lGroupIndex = m_lGroupIndex + 1;
						}
						SpTreeElems.push_back(elem);
					}
				}
			}

			FillParentPointersForChilds();
		}
		void SpTree::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			nvGrpSpPr = node.ReadNodeNoNS(_T("nvGrpSpPr"));
			grpSpPr = node.ReadNodeNoNS(_T("grpSpPr"));

			SpTreeElems.clear();

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("cNvPr") == strName)
					{
						nvGrpSpPr.cNvPr = oNode;
					}
					else if (_T("cNvGrpSpPr") == strName)
					{
						nvGrpSpPr.cNvGrpSpPr = oNode;
					}
					else
					{
						SpTreeElem elem(oNode);
						if (elem.is_init())
						{
							if (elem.getType() == OOX::et_p_ShapeTree)
							{
								smart_ptr<SpTree> e = elem.GetElem().smart_dynamic_cast<SpTree>();
								e->m_lGroupIndex = m_lGroupIndex + 1;
							}
							SpTreeElems.push_back(elem);
						}
					}
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring SpTree::toXML() const
		{
			std::wstring name_;
			if (m_namespace == L"wp")
			{
				if (m_lGroupIndex == 0)		name_ = L"wpg:wgp";
				else						name_ = L"wpg:grpSp";
			}
			else if (m_namespace == L"xdr")	name_ = L"xdr:grpSp";
			else if (m_namespace == L"dsp")	name_ = L"dsp:grpSp";
			else
			{
				if (m_lGroupIndex == 0)		name_ = L"p:spTree";
				else						name_ = L"p:grpSp";
			}

			XmlUtils::CNodeValue oValue;
			oValue.Write(nvGrpSpPr);
			oValue.Write(grpSpPr);

			oValue.WriteArray(SpTreeElems);

			return XmlUtils::CreateNode(name_, oValue);
		}
		void SpTree::toXmlWriterVML(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::Theme>& oTheme, NSCommon::smart_ptr<PPTX::Logic::ClrMap>& oClrMap, bool in_group)
		{
			pWriter->StartNode(_T("v:group"));
			pWriter->StartAttributes();

			std::wstring strId = L"group " + std::to_wstring(pWriter->m_lObjectIdVML);
			std::wstring strSpid = L"_x0000_s" + XmlUtils::ToString(0xFFFF & (pWriter->m_lObjectIdVML >> 16), L"%04d");

			pWriter->m_lObjectIdVML++;

			if (pWriter->m_strId.empty())
			{
				if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
				{
					pWriter->WriteAttribute(L"id", strSpid); //??
				}
				else
				{
					pWriter->WriteAttribute(L"id", strId);
					pWriter->WriteAttribute(L"o:spid", strSpid);
				}
			}
			else
			{
				pWriter->WriteAttribute(L"id", pWriter->m_strId);
				pWriter->WriteAttribute(L"o:spid", strSpid);
				pWriter->m_strId.clear();
			}
			
			NSBinPptxRW::CXmlWriter oStylesWriter;

			if (pWriter->m_strStyleMain.empty())
			{
				int dL = 0, dT = 0, dW = 0, dH = 0;
				if (grpSpPr.xfrm.is_init())
				{
					if (grpSpPr.xfrm->offX.is_init())	dL = *grpSpPr.xfrm->offX;
					if (grpSpPr.xfrm->offY.is_init())	dT = *grpSpPr.xfrm->offY;
					if (grpSpPr.xfrm->extX.is_init())	dW = *grpSpPr.xfrm->extX;
					if (grpSpPr.xfrm->extY.is_init())	dH = *grpSpPr.xfrm->extY;
				}

				oStylesWriter.WriteAttributeCSS(L"position", L"absolute");
				if (in_group)
				{
					oStylesWriter.WriteAttributeCSS_int(L"left", dL / 100);
					oStylesWriter.WriteAttributeCSS_int(L"top", dT / 100);
					oStylesWriter.WriteAttributeCSS_int(L"width", dW / 100);
					oStylesWriter.WriteAttributeCSS_int(L"height", dH / 100);
				}
				else
				{
					oStylesWriter.WriteAttributeCSS_int_pt(L"left", dL / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"top", dT / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"width", dW / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"height", dH / 12700);
				}
			}
	
			if (grpSpPr.xfrm.is_init())
			{
				if (grpSpPr.xfrm->rot.is_init())
				{
					int nRot = (int)((double)(*(grpSpPr.xfrm->rot)) / 60000.0);
					oStylesWriter.WriteAttributeCSS_int(_T("rotation"), nRot);
				}
				bool bIsFH = grpSpPr.xfrm->flipH.get_value_or(false);
				bool bIsFV = grpSpPr.xfrm->flipV.get_value_or(false);
				if (bIsFH && bIsFV)
				{
					oStylesWriter.WriteAttributeCSS(_T("flip"), _T("xy"));
				}
				else if (bIsFH)
				{
					oStylesWriter.WriteAttributeCSS(_T("flip"), _T("x"));
				}
				else if (bIsFV)
				{
					oStylesWriter.WriteAttributeCSS(_T("flip"), _T("y"));
				}
			}

			pWriter->WriteAttribute(_T("style"), pWriter->m_strStyleMain + pWriter->m_strStyleWrap + oStylesWriter.GetXmlString());

			pWriter->m_strStyleMain.clear();
			pWriter->m_strStyleWrap.clear();
	
			if (false == pWriter->m_strAttributesMain.empty())
			{
				pWriter->WriteString(pWriter->m_strAttributesMain);
				pWriter->m_strAttributesMain.clear();
			}

			int dL = 0, dT = 0, dW = 0, dH = 0;

			if (grpSpPr.xfrm.is_init())
			{
				if (grpSpPr.xfrm->chOffX.is_init())	dL = *grpSpPr.xfrm->chOffX;
				if (grpSpPr.xfrm->chOffY.is_init())	dT = *grpSpPr.xfrm->chOffY;
				if (grpSpPr.xfrm->chExtX.is_init())	dW = *grpSpPr.xfrm->chExtX;
				if (grpSpPr.xfrm->chExtY.is_init())	dH = *grpSpPr.xfrm->chExtY;
			}
			oStylesWriter.ClearNoAttack();
			oStylesWriter.m_oWriter.AddSize(30);
			oStylesWriter.m_oWriter.AddIntNoCheck(dL / 100);
			oStylesWriter.m_oWriter.AddCharNoCheck(WCHAR(','));
			oStylesWriter.m_oWriter.AddIntNoCheck(dT / 100);
			pWriter->WriteAttribute(_T("coordorigin"), oStylesWriter.GetXmlString());

			oStylesWriter.ClearNoAttack();
			oStylesWriter.m_oWriter.AddSize(30);
			oStylesWriter.m_oWriter.AddIntNoCheck(dW / 100);
			oStylesWriter.m_oWriter.AddCharNoCheck(WCHAR(','));
			oStylesWriter.m_oWriter.AddIntNoCheck(dH / 100);
			pWriter->WriteAttribute(_T("coordsize"), oStylesWriter.GetXmlString());

			pWriter->EndAttributes();

			size_t nCount = SpTreeElems.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (SpTreeElems[i].is<PPTX::Logic::Shape>())
				{
					SpTreeElems[i].as<PPTX::Logic::Shape>().toXmlWriterVML(pWriter, oTheme, oClrMap, true);
				}
				else if (SpTreeElems[i].is<PPTX::Logic::Pic>())
				{
					SpTreeElems[i].as<PPTX::Logic::Pic>().toXmlWriterVML(pWriter, oTheme, oClrMap, true);
				}
				else if (SpTreeElems[i].is<PPTX::Logic::SpTree>())
				{
					SpTreeElems[i].as<PPTX::Logic::SpTree>().toXmlWriterVML(pWriter, oTheme, oClrMap, true);
				}				
			}

			pWriter->WriteString(pWriter->m_strNodes);
			pWriter->m_strNodes = _T("");
			pWriter->EndNode(_T("v:group"));
		}
		void SpTree::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring name_;

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)
			{
				if (pWriter->m_lGroupIndex == 0)	name_ = L"wpg:wgp";
				else								name_ = L"wpg:grpSp";
			}
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			name_ = L"xdr:grpSp";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	name_ = L"cdr:grpSp";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			name_ = L"dgm:grpSp";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		name_ = L"a:grpSp";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)
			{
				if (pWriter->m_lGroupIndex == 0)	name_ = L"dsp:spTree";
				else								name_ = L"dsp:grpSp";
			}
			else
			{
				if (pWriter->m_lGroupIndex == 0)	name_ = L"p:spTree";
				else								name_ = L"p:grpSp";
			}
			pWriter->StartNode(name_);

			pWriter->EndAttributes();

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)
			{
				nvGrpSpPr.cNvGrpSpPr.toXmlWriter2(L"wpg", pWriter);
			}
			else
				nvGrpSpPr.toXmlWriter(pWriter);

			grpSpPr.toXmlWriter(pWriter);

			pWriter->m_lGroupIndex++;

			for (size_t i = 0; i < SpTreeElems.size(); ++i)
				SpTreeElems[i].toXmlWriter(pWriter);

			pWriter->m_lGroupIndex--;

			pWriter->EndNode(name_);
		}
		void SpTree::NormalizeRect(Aggplus::RECT& rect)const
		{
			if (grpSpPr.xfrm.IsInit())
			{
				if ((grpSpPr.xfrm->chExtX.get_value_or(0) != 0) && (grpSpPr.xfrm->chExtY.get_value_or(0) != 0))
				{
					double ScaleX = grpSpPr.xfrm->extX.get_value_or(0) / (double(grpSpPr.xfrm->chExtX.get()));
					double ScaleY = grpSpPr.xfrm->extY.get_value_or(0) / (double(grpSpPr.xfrm->chExtY.get()));
					double RectWidth = ScaleX * (rect.right - rect.left);
					double RectHeight = ScaleY * (rect.bottom - rect.top);
					rect.left = (LONG)((rect.left - grpSpPr.xfrm->chOffX.get()) * ScaleX + grpSpPr.xfrm->offX.get());
					rect.top = (LONG)((rect.top - grpSpPr.xfrm->chOffY.get()) * ScaleY + grpSpPr.xfrm->offY.get());
					rect.right = (LONG)(rect.left + RectWidth);
					rect.bottom = (LONG)(rect.top + RectHeight);
				}
			}
			if (parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(rect);
		}
		void SpTree::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (getType() == OOX::et_lc_LockedCanvas)
				pWriter->StartRecord(SPTREE_TYPE_LOCKED_CANVAS);
			else
				pWriter->StartRecord(SPTREE_TYPE_SPTREE);

			pWriter->WriteRecord1(0, nvGrpSpPr);
			pWriter->WriteRecord1(1, grpSpPr);
			pWriter->WriteRecordArray(2, 0, SpTreeElems);

			pWriter->EndRecord();
		}
		void SpTree::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(5); //+ len

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					nvGrpSpPr.fromPPTY(pReader);
					break;
				}
				case 1:
				{
					grpSpPr.fromPPTY(pReader);
					break;
				}
				case 2:
				{
					pReader->Skip(4); // len
					ULONG _c = pReader->GetULong();

					for (ULONG i = 0; i < _c; ++i)
					{
						pReader->Skip(1); // type (0)
						LONG nElemLength = pReader->GetLong(); // len
															   //SpTreeElem::fromPPTY сразу делает GetChar, а toPPTY ничего не пишет если не инициализирован
						if (nElemLength > 0)
						{
							SpTreeElem elm;
							elm.fromPPTY(pReader);

							if (elm.is_init())
							{
								if (elm.getType() == OOX::et_p_ShapeTree)
								{
									smart_ptr<SpTree> e = elm.GetElem().smart_dynamic_cast<SpTree>();
									e->m_lGroupIndex = m_lGroupIndex + 1;
								}
								SpTreeElems.push_back(elm);
							}
						}
					}
				}
				default:
				{
					break;
				}
				}
			}
			pReader->Seek(_end_rec);
		}

		LockedCanvas::LockedCanvas() : SpTree(L"a")
		{
		}
		LockedCanvas& LockedCanvas::operator=(const LockedCanvas& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			nvGrpSpPr	= oSrc.nvGrpSpPr;
			grpSpPr		= oSrc.grpSpPr;

			for (size_t i=0; i < oSrc.SpTreeElems.size(); i++)
				SpTreeElems.push_back(oSrc.SpTreeElems[i]);

			m_lGroupIndex	= oSrc.m_lGroupIndex;

			return *this;
		}
		OOX::EElementType LockedCanvas::getType () const
		{
			return OOX::et_lc_LockedCanvas;
		}
		void LockedCanvas::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			SpTree::fromXML(oReader);
		}
		void LockedCanvas::fromXML(XmlUtils::CXmlNode& node)
		{
			SpTree::fromXML(node);
		}
		std::wstring LockedCanvas::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"xmlns:lc", L"http://schemas.openxmlformats.org/drawingml/2006/lockedCanvas");

			XmlUtils::CNodeValue oValue;
			oValue.Write(nvGrpSpPr);
			oValue.Write(grpSpPr);

			oValue.WriteArray(SpTreeElems);

			return XmlUtils::CreateNode(L"lc:lockedCanvas", oAttr, oValue);
		}
		void LockedCanvas::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			BYTE lDocType = pWriter->m_lDocType;
			pWriter->m_lDocType = XMLWRITER_DOC_TYPE_GRAPHICS;

			pWriter->StartNode(L"lc:lockedCanvas");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"xmlns:lc", L"http://schemas.openxmlformats.org/drawingml/2006/lockedCanvas");

			pWriter->EndAttributes();

			nvGrpSpPr.toXmlWriter(pWriter);

			grpSpPr.toXmlWriter(pWriter);

			pWriter->m_lGroupIndex++;

			for (size_t i = 0; i < SpTreeElems.size(); ++i)
			{
				SpTreeElems[i].toXmlWriter(pWriter);
			}

			pWriter->m_lGroupIndex--;

			pWriter->EndNode(L"lc:lockedCanvas");

			pWriter->m_lDocType = lDocType;
		}
		void LockedCanvas::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			BinDocxRW::CDocxSerializer* docx = pWriter->m_pMainDocument;
			pWriter->m_pMainDocument = NULL;

			pWriter->StartRecord(SPTREE_TYPE_LOCKED_CANVAS);

			pWriter->WriteRecord1(0, nvGrpSpPr);
			pWriter->WriteRecord1(1, grpSpPr);
			pWriter->WriteRecordArray(2, 0, SpTreeElems);

			pWriter->EndRecord();
			pWriter->m_pMainDocument = docx;
		}
		void LockedCanvas::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(5); // type + len

			BinDocxRW::CDocxSerializer* docx = pReader->m_pMainDocument;
			pReader->m_pMainDocument = NULL;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						nvGrpSpPr.fromPPTY(pReader);
						break;
					}
					case 1:
					{
						grpSpPr.fromPPTY(pReader);
						break;
					}
					case 2:
					{
						pReader->Skip(4); // len
						ULONG _c = pReader->GetULong();

						for (ULONG i = 0; i < _c; ++i)
						{
							pReader->Skip(1); // type (0)
							LONG nElemLength = pReader->GetLong(); // len
							//SpTreeElem::fromPPTY сразу делает GetChar, а toPPTY ничего не пишет если не инициализирован
							if(nElemLength > 0)
							{
								SpTreeElem elm;
								elm.fromPPTY(pReader);

								if (elm.is_init())
								{
									if (elm.getType() == OOX::et_p_ShapeTree)
									{
										smart_ptr<SpTree> e = elm.GetElem().smart_dynamic_cast<SpTree>();
										e->m_lGroupIndex = m_lGroupIndex + 1;
									}
									SpTreeElems.push_back(elm);
								}
							}
						}
					}
					default:
					{
						break;
					}
				}
			}
			pReader->Seek(_end_rec);
			pReader->m_pMainDocument = docx;
		}
	}
}
