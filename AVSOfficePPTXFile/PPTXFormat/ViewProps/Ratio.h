#pragma once
#ifndef PPTX_VIEWPROPS_RATIO_INCLUDE_H_
#define PPTX_VIEWPROPS_RATIO_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Ratio : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Ratio)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name = XmlUtils::GetNameNoNS(node.GetName());

				n = node.ReadAttributeInt(L"n");
				d = node.ReadAttributeInt(L"d");

				Normalize();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("n"), n);
				oAttr.Write(_T("d"), d);

				return XmlUtils::CreateNode(_T("a:") + name, oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteINT(d);
				pWriter->WriteINT(n);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

		public:
			int d;
			int n;
		public:
			CString name;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				if (d < 0)
					d = 0;
				if (n < 0)
					n = 0;
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_RATIO_INCLUDE_H_