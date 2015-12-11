#pragma once
#ifndef PPTX_SHOWPR_CUSTSHOW_INCLUDE_H_
#define PPTX_SHOWPR_CUSTSHOW_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsShowPr
	{
		class CustShow : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CustShow)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(_T("id"), id);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("id"), id);

				return XmlUtils::CreateNode(_T("p:custShow"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, id);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:custShow"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("id"), id);

				pWriter->EndAttributes();	

				pWriter->EndNode(_T("p:custShow"));
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
						id = pReader->GetLong();
					else
						break;
				}

				pReader->Seek(_end_rec);
			}
		public:
			nullable_int			id;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsShowPr
} // namespace PPTX

#endif // PPTX_SHOWPR_CUSTSHOW_INCLUDE_H_
