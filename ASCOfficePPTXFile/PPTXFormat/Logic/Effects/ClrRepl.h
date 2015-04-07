#pragma once
#ifndef PPTX_LOGIC_CLRREPL_INCLUDE_H_
#define PPTX_LOGIC_CLRREPL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class ClrRepl : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(ClrRepl)

			ClrRepl& operator=(const ClrRepl& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Color = oSrc.Color;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Color.GetColorFrom(node);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				return _T("<a:clrRepl>") + Color.toXML() + _T("</a:clrRepl>");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_CLRREPL);

				pWriter->WriteRecord1(0, Color);

				pWriter->EndRecord();
			}

		public:
			UniColor Color;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Color.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CLRREPL_INCLUDE_H_