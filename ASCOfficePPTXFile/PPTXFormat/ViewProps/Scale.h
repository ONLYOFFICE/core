#pragma once
#ifndef PPTX_VIEWPROPS_SCALE_INCLUDE_H_
#define PPTX_VIEWPROPS_SCALE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Ratio.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Scale : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Scale)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				sx = node.ReadNodeNoNS(_T("sx"));
				sy = node.ReadNodeNoNS(_T("sy"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(sx);
				oValue.Write(sy);

				return XmlUtils::CreateNode(_T("p:scale"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, sx);
				pWriter->WriteRecord1(1, sy);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:scale"));
				pWriter->EndAttributes();

				pWriter->StartNode(_T("a:sx"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("n"), sx.n);
				pWriter->WriteAttribute(_T("d"), sx.d);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:sx"));

				pWriter->StartNode(_T("a:sy"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("n"), sy.n);
				pWriter->WriteAttribute(_T("d"), sy.d);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:sy"));
				
				pWriter->EndNode(_T("p:scale"));
			}

		public:
			nsViewProps::Ratio sx;
			nsViewProps::Ratio sy;
		protected:
			virtual void FillParentPointersForChilds()
			{
				sx.SetParentPointer(this);
				sy.SetParentPointer(this);
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_SCALE_INCLUDE_H_