#pragma once
#ifndef PPTX_LOGIC_SLIDE_BGPR_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_BGPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "UniFill.h"
#include "EffectProperties.h"

namespace PPTX
{
	namespace Logic
	{

		class BgPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BgPr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"shadeToTitle", shadeToTitle);
				Fill.GetFillFrom(node);
				EffectList.GetEffectListFrom(node);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("shadeToTitle"), shadeToTitle);

				XmlUtils::CNodeValue oValue;
				oValue.Write(Fill);
				oValue.Write(EffectList);

				return XmlUtils::CreateNode(_T("p:bgPr"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:bgPr"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("shadeToTitle"), shadeToTitle);
				pWriter->EndAttributes();

				Fill.toXmlWriter(pWriter);
				EffectList.toXmlWriter(pWriter);
				
				pWriter->EndNode(_T("p:bgPr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, shadeToTitle);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, Fill);
				pWriter->WriteRecord1(1, EffectList);
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
						shadeToTitle = pReader->GetBool();
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
							Fill.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							pReader->SkipRecord();
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			UniFill				Fill;
			EffectProperties	EffectList;
 
			nullable_bool		shadeToTitle;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Fill.SetParentPointer(this);
				EffectList.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_BGPR_INCLUDE_H_