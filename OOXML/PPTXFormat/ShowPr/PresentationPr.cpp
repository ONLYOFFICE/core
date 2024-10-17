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

#include "PresentationPr.h"

#include "./Browse.h"
#include "./CustShow.h"
#include "./Kiosk.h"
#include "./Present.h"
#include "./SldAll.h"
#include "./SldRg.h"

namespace PPTX
{
	namespace nsPresentationPr
	{
		void ShowPr::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"loop", loop);
			XmlMacroReadAttributeBase(node, L"showAnimation", showAnimation);
			XmlMacroReadAttributeBase(node, L"showNarration", showNarration);
			XmlMacroReadAttributeBase(node, L"useTimings", useTimings);

			Browse = node.ReadNodeNoNS(L"browse");
			CustShow = node.ReadNodeNoNS(L"custShow");
			Kiosk = node.ReadNodeNoNS(L"kiosk");
			XmlUtils::CXmlNode node1 = node.ReadNodeNoNS(L"penClr");
			PenClr.GetColorFrom(node1);
			Present = node.ReadNodeNoNS(L"present");
			SldAll = node.ReadNodeNoNS(L"sldAll");
			SldRg = node.ReadNodeNoNS(L"sldRg");
		}
		std::wstring ShowPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"loop", loop);
			oAttr.Write(L"showAnimation", showAnimation);
			oAttr.Write(L"showNarration", showNarration);
			oAttr.Write(L"useTimings", useTimings);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(Present);
			oValue.WriteNullable(Browse);
			oValue.WriteNullable(Kiosk);
			oValue.WriteNullable(SldAll);
			oValue.WriteNullable(SldRg);
			oValue.WriteNullable(CustShow);
			oValue.Write(PenClr);

			return XmlUtils::CreateNode(L"p:ShowPr", oAttr, oValue);
		}
		void ShowPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, loop);
			pWriter->WriteBool2(1, showAnimation);
			pWriter->WriteBool2(2, showNarration);
			pWriter->WriteBool2(3, useTimings);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, Browse);
			pWriter->WriteRecord2(1, CustShow);
			pWriter->WriteRecord2(2, Kiosk);
			pWriter->WriteRecord1(3, PenClr);
			pWriter->WriteRecord2(4, Present);
			pWriter->WriteRecord2(5, SldAll);
			pWriter->WriteRecord2(6, SldRg);
		}
		void ShowPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:showPr");

			pWriter->StartAttributes();

			pWriter->WriteAttribute(L"loop", loop);
			pWriter->WriteAttribute(L"showAnimation", showAnimation);
			pWriter->WriteAttribute(L"showNarration", showNarration);
			pWriter->WriteAttribute(L"useTimings", useTimings);

			pWriter->EndAttributes();

			pWriter->Write(Present);
			pWriter->Write(Browse);
			pWriter->Write(Kiosk);
			pWriter->Write(SldAll);
			pWriter->Write(SldRg);
			pWriter->Write(CustShow);
			if(PenClr.is_init())
			{
				pWriter->WriteString(L"<p:penClr>");
				PenClr.toXmlWriter(pWriter);
				pWriter->WriteString(L"</p:penClr>");
			}

			pWriter->EndNode(L"p:showPr");
		}
		void ShowPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					loop = pReader->GetBool();
				else if (1 == _at)
					showAnimation = pReader->GetBool();
				else if (2 == _at)
					showNarration = pReader->GetBool();
				else if (3 == _at)
					useTimings = pReader->GetBool();
				else
					break;
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						Browse = new nsPresentationPr::Browse();
						Browse->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						CustShow = new nsPresentationPr::CustShow();
						CustShow->fromPPTY(pReader);
						break;
					}
					case 2:
					{
						Kiosk = new nsPresentationPr::Kiosk();
						Kiosk->fromPPTY(pReader);
						break;
					}
					case 3:
					{
						PenClr.fromPPTY(pReader);
						break;
					}
					case 4:
					{
						Present = new nsPresentationPr::Present();
						pReader->SkipRecord();
						break;
					}
					case 5:
					{
						SldAll = new nsPresentationPr::SldAll();
						pReader->SkipRecord();
						break;
					}
					case 6:
					{
						SldRg = new nsPresentationPr::SldRg();
						SldRg->fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void ShowPr::FillParentPointersForChilds()
		{
		}
//--------------------------------------------------------------------------------------------------------
		void PrintPr::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"clrMode", clrMode);
			XmlMacroReadAttributeBase(node, L"prnWhat", prnWhat);
			XmlMacroReadAttributeBase(node, L"frameSlides", frameSlides);
			XmlMacroReadAttributeBase(node, L"hiddenSlides", hiddenSlides);
			XmlMacroReadAttributeBase(node, L"scaleToFitPaper", scaleToFitPaper);

			XmlUtils::CXmlNode list = node.ReadNodeNoNS(L"extLst");
			if (list.IsValid())
			{
				std::vector<XmlUtils::CXmlNode> oNodes;
				if (list.GetNodes(L"*", oNodes))
				{
					size_t nCount = oNodes.size();
					for (size_t i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode& oNode = oNodes[i];

						Logic::Ext element;
						element.fromXML(oNode);

						ExtLst.push_back(element);
					}
				}
			}
		}
		std::wstring PrintPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"frameSlides", frameSlides);
			oAttr.Write(L"hiddenSlides", hiddenSlides);
			oAttr.Write(L"scaleToFitPaper", scaleToFitPaper);
			oAttr.WriteLimitNullable(L"clrMode", clrMode);
			oAttr.WriteLimitNullable(L"prnWhat", prnWhat);

			XmlUtils::CNodeValue oValue;

			return XmlUtils::CreateNode(L"p:PrintPr", oAttr, oValue);
		}
		void PrintPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, frameSlides);
				pWriter->WriteBool2(1, hiddenSlides);
				pWriter->WriteBool2(2, scaleToFitPaper);
				pWriter->WriteLimit2(3, clrMode);
				pWriter->WriteLimit2(4, prnWhat);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void PrintPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void PrintPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
						frameSlides = pReader->GetBool(); break;
					case 1:
						hiddenSlides = pReader->GetBool(); break;
					case 2:
						scaleToFitPaper = pReader->GetBool(); break;
					case 3:
						clrMode = pReader->GetUChar(); break;
					case 4:
						prnWhat = pReader->GetUChar(); break;
					default:
							break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void PrintPr::FillParentPointersForChilds()
		{
		}
	} // namespace nsPresentationPr
} // namespace PPTX
