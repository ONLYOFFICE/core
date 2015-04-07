#pragma once
#ifndef PPTX_LOGIC_FILLOVERLAY_INCLUDE_H_
#define PPTX_LOGIC_FILLOVERLAY_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniFill.h"
#include "./../../Limit/BlendMode.h"

namespace PPTX
{
	namespace Logic
	{

		class FillOverlay : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(FillOverlay)

			FillOverlay& operator=(const FillOverlay& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Fill = oSrc.Fill;
				blend = oSrc.blend;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Fill.GetFillFrom(node);
				blend = node.GetAttribute(_T("blend"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				CString str = _T("<a:fillOverlay blend=\"") + blend.get() + _T("\">");
				str += Fill.toXML();
				str += _T("</a:fillOverlay>");

				return str;
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_FILLOVERLAY);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit1(0, blend);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, Fill);

				pWriter->EndRecord();
			}

		public:
			UniFill				Fill;
			Limit::BlendMode	blend;
		protected:
            virtual void FillParentPointersForChilds()
			{
				Fill.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FILLOVERLAY_INCLUDE_H_
