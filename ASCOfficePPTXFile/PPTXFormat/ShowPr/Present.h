#pragma once
#ifndef PPTX_SHOWPR_PRESENT_INCLUDE_H_
#define PPTX_SHOWPR_PRESENT_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsShowPr
	{
		class Present : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Present)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual CString toXML() const
			{
				return _T("<p:present/>");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(_T("<p:present/>"));
			}
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsShowPr
} // namespace PPTX

#endif // PPTX_SHOWPR_PRESENT_INCLUDE_H_
