#pragma once
#ifndef PPTX_SHOWPR_FILE_INCLUDE_H_
#define PPTX_SHOWPR_FILE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "../Logic/UniColor.h"
#include "./Browse.h"
#include "./CustShow.h"
#include "./Kiosk.h"
#include "./Present.h"
#include "./SldAll.h"
#include "./SldRg.h"

namespace PPTX
{
	namespace nsShowPr
	{
		class ShowPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ShowPr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(_T("loop"), loop);
				node.ReadAttributeBase(_T("showAnimation"), showAnimation);
				node.ReadAttributeBase(_T("showNarration"), showNarration);
				node.ReadAttributeBase(_T("useTimings"), useTimings);

				Browse = node.ReadNodeNoNS(_T("browse"));
				CustShow = node.ReadNodeNoNS(_T("custShow"));
				Kiosk = node.ReadNodeNoNS(_T("kiosk"));
				XmlUtils::CXmlNode node1 = node.ReadNodeNoNS(_T("penClr"));
				PenClr.GetColorFrom(node1);
				Present = node.ReadNodeNoNS(_T("present"));
				SldAll = node.ReadNodeNoNS(_T("sldAll"));
				SldRg = node.ReadNodeNoNS(_T("sldRg"));
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("loop"), loop);
				oAttr.Write(_T("showAnimation"), showAnimation);
				oAttr.Write(_T("showNarration"), showNarration);
				oAttr.Write(_T("useTimings"), useTimings);

				XmlUtils::CNodeValue oValue;
                oValue.WriteNullable(Present);
				oValue.WriteNullable(Browse);
                oValue.WriteNullable(Kiosk);
				oValue.WriteNullable(SldAll);
				oValue.WriteNullable(SldRg);
                oValue.WriteNullable(CustShow);
                oValue.Write(PenClr);
                
				return XmlUtils::CreateNode(_T("p:ShowPr"), oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
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
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:showPr"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("loop"), loop);
				pWriter->WriteAttribute(_T("showAnimation"), showAnimation);
				pWriter->WriteAttribute(_T("showNarration"), showNarration);
				pWriter->WriteAttribute(_T("useTimings"), useTimings);

				pWriter->EndAttributes();

                pWriter->Write(Present);
				pWriter->Write(Browse);
                pWriter->Write(Kiosk);
                pWriter->Write(SldAll);
				pWriter->Write(SldRg);
				pWriter->Write(CustShow);
				if(PenClr.is_init())
				{
					pWriter->WriteString(_T("<p:penClr>"));
					PenClr.toXmlWriter(pWriter);
					pWriter->WriteString(_T("</p:penClr>"));
				}
                
				pWriter->EndNode(_T("p:showPr"));
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
							Browse = new nsShowPr::Browse();
							Browse->fromPPTY(pReader);
							break;
						}
						case 1:
						{
							CustShow = new nsShowPr::CustShow();
							CustShow->fromPPTY(pReader);
							break;
						}
						case 2:
						{
							Kiosk = new nsShowPr::Kiosk();
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
							Present = new nsShowPr::Present();
							pReader->SkipRecord();
							break;
						}
						case 5:
						{
							SldAll = new nsShowPr::SldAll();
							pReader->SkipRecord();
							break;
						}
						case 6:
						{
							SldRg = new nsShowPr::SldRg();
							SldRg->fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}
		public:
			nullable<nsShowPr::Browse> Browse;
			nullable<nsShowPr::CustShow> CustShow;
			nullable<nsShowPr::Kiosk> Kiosk;
			Logic::UniColor PenClr;
			nullable<nsShowPr::Present> Present;
			nullable<nsShowPr::SldAll> SldAll;
			nullable<nsShowPr::SldRg> SldRg;

			nullable_bool			loop;
			nullable_bool			showAnimation;
			nullable_bool			showNarration;
			nullable_bool			useTimings;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsShowPr
} // namespace PPTX

#endif // PPTX_SHOWPR_FILE_INCLUDE_H_
