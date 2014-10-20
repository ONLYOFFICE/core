#pragma once
#ifndef PPTX_LOGIC_TABLE_CELLPROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_TABLE_CELLPROPERTIES_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/TextVerticalType.h"
#include "./../../Limit/TextAnchor.h"
#include "./../../Limit/HorzOverflow.h"
#include "./../Ln.h"
#include "./../Cell3D.h"
#include "./../UniFill.h"

namespace PPTX
{
	namespace Logic
	{
		class TableCellProperties : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TableCellProperties)

			TableCellProperties& operator=(const TableCellProperties& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				LnL				= oSrc.LnL;
				LnR				= oSrc.LnR;
				LnT				= oSrc.LnT;
				LnB				= oSrc.LnB;
				LnTlToBr		= oSrc.LnTlToBr;
				LnBlToTr		= oSrc.LnBlToTr;
                cell3D			= oSrc.cell3D;
				Fill			= oSrc.Fill;
				
				MarL			= oSrc.MarL;
				MarR			= oSrc.MarR;
				MarT			= oSrc.MarT;
				MarB			= oSrc.MarB;
				Vert			= oSrc.Vert;
				Anchor			= oSrc.Anchor;
				AnchorCtr		= oSrc.AnchorCtr;
				HorzOverflow	= oSrc.HorzOverflow;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				LnL				= node.ReadNode(_T("a:lnL"));
				LnR				= node.ReadNode(_T("a:lnR"));
				LnT				= node.ReadNode(_T("a:lnT"));
				LnB				= node.ReadNode(_T("a:lnB"));
				LnTlToBr		= node.ReadNode(_T("a:lnTlToBr"));
				LnBlToTr		= node.ReadNode(_T("a:lnBlToTr"));
                cell3D			= node.ReadNode(_T("a:cell3D"));
				Fill.GetFillFrom(node);
	//			<xsd:element name="headers" type="CT_Headers" minOccurs="0"/> 

				node.ReadAttributeBase(L"marL", MarL);
				node.ReadAttributeBase(L"marR", MarR);
				node.ReadAttributeBase(L"marT", MarT);
				node.ReadAttributeBase(L"marB", MarB);
				node.ReadAttributeBase(L"vert", Vert);
				node.ReadAttributeBase(L"anchor", Anchor);
				node.ReadAttributeBase(L"anchorCtr", AnchorCtr);
				node.ReadAttributeBase(L"horzOverflow", HorzOverflow);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("marL"), MarL);
				oAttr.Write(_T("marR"), MarR);
				oAttr.Write(_T("marT"), MarT);
				oAttr.Write(_T("marB"), MarB);
				oAttr.WriteLimitNullable(_T("vert"), Vert);
				oAttr.WriteLimitNullable(_T("anchor"), Anchor);
				oAttr.Write(_T("anchorCtr"), AnchorCtr);
				oAttr.WriteLimitNullable(_T("horzOverflow"), HorzOverflow);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(LnL);
				oValue.WriteNullable(LnR);
				oValue.WriteNullable(LnT);
				oValue.WriteNullable(LnB);
				oValue.WriteNullable(LnTlToBr);
				oValue.WriteNullable(LnBlToTr);
                oValue.WriteNullable(cell3D);
				oValue.Write(Fill);

				return XmlUtils::CreateNode(_T("a:tcPr"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tcPr"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("marL"), MarL);
				pWriter->WriteAttribute(_T("marR"), MarR);
				pWriter->WriteAttribute(_T("marT"), MarT);
				pWriter->WriteAttribute(_T("marB"), MarB);
				pWriter->WriteAttribute(_T("vert"), Vert);
				pWriter->WriteAttribute(_T("anchor"), Anchor);
				pWriter->WriteAttribute(_T("anchorCtr"), AnchorCtr);
				pWriter->WriteAttribute(_T("horzOverflow"), HorzOverflow);

				pWriter->EndAttributes();

				pWriter->Write(LnL);
				pWriter->Write(LnR);
				pWriter->Write(LnT);				
				pWriter->Write(LnB);
				pWriter->Write(LnTlToBr);
				pWriter->Write(LnBlToTr);
				Fill.toXmlWriter(pWriter);
                pWriter->Write(cell3D);

				pWriter->EndNode(_T("a:tcPr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, MarL);
				pWriter->WriteInt2(1, MarT);
				pWriter->WriteInt2(2, MarR);
				pWriter->WriteInt2(3, MarB);
				pWriter->WriteBool2(4, AnchorCtr);
				pWriter->WriteLimit2(5, Vert);
				pWriter->WriteLimit2(6, Anchor);
				pWriter->WriteLimit2(7, HorzOverflow);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, LnL);
				pWriter->WriteRecord2(1, LnT);
				pWriter->WriteRecord2(2, LnR);
				pWriter->WriteRecord2(3, LnB);
				pWriter->WriteRecord2(4, LnTlToBr);
				pWriter->WriteRecord2(5, LnBlToTr);
				pWriter->WriteRecord1(6, Fill);
                pWriter->WriteRecord2(7, cell3D);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							MarL = pReader->GetLong();
							break;
						}
						case 1:
						{
							MarT = pReader->GetLong();
							break;
						}
						case 2:
						{
							MarR = pReader->GetLong();
							break;
						}
						case 3:
						{
							MarB = pReader->GetLong();
							break;
						}
						case 4:
						{
							AnchorCtr = pReader->GetBool();
							break;
						}
						case 5:
						{
							Vert = new Limit::TextVerticalType();
							Vert->SetBYTECode(pReader->GetUChar());
							break;
						}
						case 6:
						{
							Anchor = new Limit::TextAnchor();
							Anchor->SetBYTECode(pReader->GetUChar());
							break;
						}
						case 7:
						{
							HorzOverflow = new Limit::HorzOverflow();
							HorzOverflow->SetBYTECode(pReader->GetUChar());
							break;
						}
						default:
							break;
					}
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							LnL = new Ln();
							LnL->fromPPTY(pReader);
							LnL->m_name = _T("a:lnL");
							break;
						}
						case 1:
						{
							LnT = new Ln();
							LnT->fromPPTY(pReader);
							LnT->m_name = _T("a:lnT");
							break;
						}
						case 2:
						{
							LnR = new Ln();
							LnR->fromPPTY(pReader);
							LnR->m_name = _T("a:lnR");
							break;
						}
						case 3:
						{
							LnB = new Ln();
							LnB->fromPPTY(pReader);
							LnB->m_name = _T("a:lnB");
							break;
						}
						case 4:
						{
							LnTlToBr = new Ln();
							LnTlToBr->fromPPTY(pReader);
							LnTlToBr->m_name = _T("a:lnTlToBr");
							break;
						}
						case 5:
						{
							LnBlToTr = new Ln();
							LnBlToTr->fromPPTY(pReader);
							LnBlToTr->m_name = _T("a:lnBlToTr");
							break;
						}
						case 6:
						{
							Fill.fromPPTY(pReader);							
							break;
						}
						case 7:
						{
                            cell3D = new Logic::Cell3D();
                            cell3D->fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}

		public:
			nullable<Ln>		LnL;
			nullable<Ln>		LnR;
			nullable<Ln>		LnT;
			nullable<Ln>		LnB;
			nullable<Ln>		LnTlToBr;
			nullable<Ln>		LnBlToTr;
            nullable<Cell3D>	cell3D;
			UniFill				Fill;
//			<xsd:element name="headers" type="CT_Headers" minOccurs="0"/> 

			nullable_int							MarL;//default="91440"
			nullable_int							MarR;//default="91440"
			nullable_int							MarT;//default="45720"
			nullable_int							MarB;//default="45720"
			nullable_limit<Limit::TextVerticalType> Vert;//default="horz"
			nullable_limit<Limit::TextAnchor>		Anchor;//default="t"
			nullable_bool							AnchorCtr;//default="false"
			nullable_limit<Limit::HorzOverflow>		HorzOverflow;//default="clip"
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(LnL.IsInit())
					LnL->SetParentPointer(this);
				if(LnR.IsInit())
					LnR->SetParentPointer(this);
				if(LnT.IsInit())
					LnT->SetParentPointer(this);
				if(LnB.IsInit())
					LnB->SetParentPointer(this);
				if(LnTlToBr.IsInit())
					LnTlToBr->SetParentPointer(this);
				if(LnBlToTr.IsInit())
					LnBlToTr->SetParentPointer(this);

                if(cell3D.IsInit())
                    cell3D->SetParentPointer(this);
				if(Fill.is_init())
					Fill.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLE_CELLPROPERTIES_INCLUDE_H_
