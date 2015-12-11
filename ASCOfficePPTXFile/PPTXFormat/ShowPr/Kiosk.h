#pragma once
#ifndef PPTX_SHOWPR_KIOSK_INCLUDE_H_
#define PPTX_SHOWPR_KIOSK_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsShowPr
	{
		class Kiosk : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Kiosk)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(_T("restart"), restart);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("restart"), restart);

				return XmlUtils::CreateNode(_T("p:kiosk"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, restart);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:kiosk"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("restart"), restart);

				pWriter->EndAttributes();	

				pWriter->EndNode(_T("p:kiosk"));
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
						restart = pReader->GetLong();
					else
						break;
				}

				pReader->Seek(_end_rec);
			}
		public:
			nullable_int			restart;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsShowPr
} // namespace PPTX

#endif // PPTX_SHOWPR_KIOSK_INCLUDE_H_
