#pragma once
#ifndef PPTX_VIEWPROPS_SLIDE_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_SLIDE_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CSldViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class SlideViewPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SlideViewPr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CSldViewPr = node.ReadNode(_T("p:cSldViewPr"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(CSldViewPr);

				return XmlUtils::CreateNode(_T("p:slideViewPr"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, CSldViewPr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:slideViewPr"));
				pWriter->EndAttributes();

				CSldViewPr.toXmlWriter(pWriter);				

				pWriter->EndNode(_T("p:slideViewPr"));
			}

		public:
			nsViewProps::CSldViewPr CSldViewPr;
		protected:
			virtual void FillParentPointersForChilds()
			{
				CSldViewPr.SetParentPointer(this);
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_SLIDE_VIEW_PROPERTIES_INCLUDE_H_