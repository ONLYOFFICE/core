#pragma once
#ifndef PPTX_VIEWPROPS_GRIDSPACING_INCLUDE_H_
#define PPTX_VIEWPROPS_GRIDSPACING_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class GridSpacing : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(GridSpacing)

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

				return XmlUtils::CreateNode(_T("p:gridSpacing"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteINT(cx);
				pWriter->WriteINT(cy);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:gridSpacing"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("cx"), cx);
				pWriter->WriteAttribute(_T("cy"), cy);

				pWriter->EndAttributes();

				pWriter->EndNode(_T("p:gridSpacing"));
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
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_GRIDSPACING_INCLUDE_H_