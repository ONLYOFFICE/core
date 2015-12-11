#pragma once
#ifndef PPTX_SHOWPR_SLDRG_INCLUDE_H_
#define PPTX_SHOWPR_SLDRG_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsShowPr
	{
		class SldRg : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SldRg)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(_T("st"), st);
				node.ReadAttributeBase(_T("end"), end);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("st"), st);
				oAttr.Write(_T("end"), end);

				return XmlUtils::CreateNode(_T("p:sldRg"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, st);
				pWriter->WriteInt2(1, end);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:sldRg"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("st"), st);
				pWriter->WriteAttribute(_T("end"), end);

				pWriter->EndAttributes();

				pWriter->EndNode(_T("p:sldRg"));
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
						st = pReader->GetLong();
					else if (1 == _at)
						end = pReader->GetLong();
					else
						break;
				}

				pReader->Seek(_end_rec);
			}
		public:
			nullable_int			st;
			nullable_int			end;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsShowPr
} // namespace PPTX

#endif // PPTX_SHOWPR_SLDRG_INCLUDE_H_
