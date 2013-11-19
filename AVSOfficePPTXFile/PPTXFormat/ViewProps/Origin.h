#pragma once
#ifndef PPTX_VIEWPROPS_ORIGIN_INCLUDE_H_
#define PPTX_VIEWPROPS_ORIGIN_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Origin : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Origin)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				x = node.ReadAttributeInt(L"x");
				y = node.ReadAttributeInt(L"y");
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("x"), x);
				oAttr.Write(_T("y"), y);

				return XmlUtils::CreateNode(_T("p:origin"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteINT(x);
				pWriter->WriteINT(y);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:origin"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), x);
				pWriter->WriteAttribute(_T("y"), y);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("p:origin"));
			}

		public:
			int x;
			int y;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_ORIGIN_INCLUDE_H_