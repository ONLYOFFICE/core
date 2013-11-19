#pragma once
#ifndef PPTX_PRESENTATION_NOTESSZ_INCLUDE_H_
#define PPTX_PRESENTATION_NOTESSZ_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class NotesSz : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(NotesSz)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cx = node.ReadAttributeInt(L"cx");
				cy = node.ReadAttributeInt(L"cy");

				Normalize();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("cx"), cx);
				oAttr.Write(_T("cy"), cy);

				return XmlUtils::CreateNode(_T("p:notesSz"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				
				pWriter->WriteInt1(0, cx);
				pWriter->WriteInt1(1, cy);
				
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:notesSz"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("cx"), cx);
				pWriter->WriteAttribute(_T("cy"), cy);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("p:notesSz"));
			}

		public:
			int cx;
			int cy;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				if (cx < 0)
					cx = 0;
				if (cy < 0)
					cy = 0;
			}
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_NOTESSZ_INCLUDE_H_