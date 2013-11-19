#pragma once
#ifndef PPTX_LOGIC_FILLEFFECT_INCLUDE_H_
#define PPTX_LOGIC_FILLEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniFill.h"

namespace PPTX
{
	namespace Logic
	{
		class FillEffect : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(FillEffect)
			
			FillEffect& operator=(const FillEffect& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Fill = oSrc.Fill;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Fill.GetFillFrom(node);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				return _T("<a:fill>") + Fill.toXML() + _T("</a:fill>");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_FILL);

				pWriter->WriteRecord1(0, Fill);

				pWriter->EndRecord();
			}

		public:
			UniFill Fill;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Fill.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FILLEFFECT_INCLUDE_H_