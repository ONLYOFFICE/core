#pragma once
#ifndef PPTX_PRESENTATION_SLDSZ_INCLUDE_H_
#define PPTX_PRESENTATION_SLDSZ_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/SlideSize.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class SldSz : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SldSz)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cx = node.ReadAttributeInt(L"cx");
				cy = node.ReadAttributeInt(L"cy");

				node.ReadAttributeBase(L"type", type);

				Normalize();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("cx"), cx);
				oAttr.Write(_T("cy"), cy);
				oAttr.WriteLimitNullable(_T("type"), type);

				return XmlUtils::CreateNode(_T("p:sldSz"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				
				pWriter->WriteInt1(0, cx);
				pWriter->WriteInt1(1, cy);
				pWriter->WriteLimit2(2, type);
			
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:sldSz"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("cx"), cx);
				pWriter->WriteAttribute(_T("cy"), cy);
				pWriter->WriteAttribute(_T("type"), type);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("p:sldSz"));
			}

		public:
			int cx;
			int cy;
			nullable_limit<Limit::SlideSize> type;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
                cx = (std::min)((std::max)(cx, 914400), 51206400);
                cy = (std::min)((std::max)(cy, 914400), 51206400);
			}
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_SLDSZ_INCLUDE_H_
