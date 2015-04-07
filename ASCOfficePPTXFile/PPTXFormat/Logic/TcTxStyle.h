#pragma once
#ifndef PPTX_LOGIC_TCTXSTYLE_INCLUDE_H_
#define PPTX_LOGIC_TCTXSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/OnOff.h"
#include "UniColor.h"
#include "FontRef.h"

namespace PPTX
{
	namespace Logic
	{
		class TcTxStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TcTxStyle)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"i", i);
				node.ReadAttributeBase(L"b", b);

                fontRef = node.ReadNodeNoNS(_T("fontRef"));
				Color.GetColorFrom(node);

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("i"), i);
				oAttr.WriteLimitNullable(_T("b"), b);

				XmlUtils::CNodeValue oValue;
                oValue.WriteNullable(fontRef);
				oValue.Write(Color);

				return XmlUtils::CreateNode(_T("a:tcTxStyle"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tcTxStyle"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("i"), i);
				pWriter->WriteAttribute(_T("b"), b);
				pWriter->EndAttributes();

                pWriter->Write(fontRef);
				Color.toXmlWriter(pWriter);

				pWriter->EndNode(_T("a:tcTxStyle"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, i);
				pWriter->WriteLimit2(1, b);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

                pWriter->WriteRecord2(0, fontRef);
				pWriter->WriteRecord1(1, Color);
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

					switch (_at)
					{
						case 0:
						{
							i = new Limit::OnOff();
							i->SetBYTECode(pReader->GetUChar());
							break;
						}
						case 1:
						{
							b = new Limit::OnOff();
							b->SetBYTECode(pReader->GetUChar());
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
                            fontRef = new Logic::FontRef();
                            fontRef->fromPPTY(pReader);
                            fontRef->m_name = _T("a:fontRef");
							break;
						}
						case 1:
						{
							Color.fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}

		public:
			nullable_limit<Limit::OnOff> i;
			nullable_limit<Limit::OnOff> b;
/*
font (Font)  §20.1.4.2.13 
//Здесь имеется ввиду fontCollection (см. ниже), это еще одна альтернатива для Font
//Данный комментарий оставлен для напоминания
*/
            nullable<FontRef> fontRef;
			UniColor Color;
		protected:
			virtual void FillParentPointersForChilds()
			{
                if(fontRef.IsInit())
                    fontRef->SetParentPointer(this);
				Color.SetParentPointer(this);
			}
		};

//		class FontCollection: public WrapperWritingElement
//		{
//		}

	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TCTXSTYLE_INCLUDE_H
