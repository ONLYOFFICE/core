#pragma once
#ifndef PPTX_LOGIC_PRSTTXWARP_INCLUDE_H_
#define PPTX_LOGIC_PRSTTXWARP_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/TextShapeType.h"
#include "Gd.h"

namespace PPTX
{
	namespace Logic
	{
		class PrstTxWarp : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(PrstTxWarp)

			PrstTxWarp& operator=(const PrstTxWarp& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				avLst.Copy(oSrc.avLst);
				prst = oSrc.prst;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"prst", prst);
				node.ReadNode(_T("a:avLst")).LoadArray(_T("a:gd"), avLst);

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("prst"), prst.get());

				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(_T("a:avLst"), avLst);

				return XmlUtils::CreateNode(_T("a:prstTxWarp"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:prstTxWarp"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("prst"), prst.get());
				pWriter->EndAttributes();

				pWriter->WriteArray(_T("a:avLst"), avLst);

				pWriter->EndNode(_T("a:prstTxWarp"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit1(0, prst);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecordArray(0, 1, avLst);
			}

		public:
			Limit::TextShapeType prst;
			CAtlArray<Gd>		 avLst;
		protected:
			virtual void FillParentPointersForChilds()
			{
				size_t count = avLst.GetCount();
				for (size_t i = 0; i < count; ++i)
					avLst[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PRSTTXWARP_INCLUDE_H