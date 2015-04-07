#ifndef PPTX_LOGIC_PH_INCLUDE_H_
#define PPTX_LOGIC_PH_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/Orient.h"
#include "./../Limit/PlaceholderSize.h"
#include "./../Limit/PlaceholderType.h"

namespace PPTX
{
	namespace Logic
	{
		class Ph : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Ph)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"hasCustomPrompt", hasCustomPrompt);
				node.ReadAttributeBase(L"idx", idx);
				node.ReadAttributeBase(L"orient", orient);
				node.ReadAttributeBase(L"sz", sz);
				node.ReadAttributeBase(L"type", type);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("type"), type);
				oAttr.WriteLimitNullable(_T("orient"), orient);
				oAttr.WriteLimitNullable(_T("sz"), sz);
				oAttr.Write(_T("idx"), idx);
				oAttr.Write(_T("hasCustomPrompt"), hasCustomPrompt);

				return XmlUtils::CreateNode(_T("p:ph"), oAttr);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:ph"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("type"), type);
				pWriter->WriteAttribute(_T("orient"), orient);
				pWriter->WriteAttribute(_T("sz"), sz);
				pWriter->WriteAttribute(_T("idx"), idx);
				pWriter->WriteAttribute(_T("hasCustomPrompt"), hasCustomPrompt);
				pWriter->EndAttributes();
				
				pWriter->EndNode(_T("p:ph"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, hasCustomPrompt);
				pWriter->WriteString2(1, idx);
				pWriter->WriteLimit2(2, orient);
				pWriter->WriteLimit2(3, sz);
				pWriter->WriteLimit2(4, type);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
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
							hasCustomPrompt = pReader->GetBool();
							break;
						}
						case 1:
						{
							idx = pReader->GetString2();
							break;
						}
						case 2:
						{
							orient = new Limit::Orient();
							orient->SetBYTECode(pReader->GetUChar());
							break;
						}
						case 3:
						{
							sz = new Limit::PlaceholderSize();
							sz->SetBYTECode(pReader->GetUChar());
							break;
						}
						case 4:
						{
							type = new Limit::PlaceholderType();
							type->SetBYTECode(pReader->GetUChar());
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			nullable_bool							hasCustomPrompt;
			nullable_string							idx;
			nullable_limit<Limit::Orient>			orient;
			nullable_limit<Limit::PlaceholderSize>	sz;
			nullable_limit<Limit::PlaceholderType>	type;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PH_INCLUDE_H