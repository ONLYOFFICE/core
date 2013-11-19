#pragma once
#ifndef PPTX_LOGIC_BUBLIP_INCLUDE_H_
#define PPTX_LOGIC_BUBLIP_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../Fills/Blip.h"

namespace PPTX
{
	namespace Logic
	{
		class BuBlip : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(BuBlip)

			BuBlip& operator=(const BuBlip& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				blip = oSrc.blip;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				blip = node.ReadNodeNoNS(_T("blip"));
			}
			virtual CString toXML() const
			{
				return XmlUtils::CreateNode(_T("a:buBlip"), blip.toXML());
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:buBlip"));
				pWriter->EndAttributes();
				blip.toXmlWriter(pWriter);
				pWriter->EndNode(_T("a:buBlip"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_BULLET_BLIP);
				pWriter->WriteRecord1(0, blip);
				pWriter->EndRecord();
			}

		public:
			Blip blip;
		protected:
			virtual void FillParentPointersForChilds()
			{
				blip.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUBLIP_INCLUDE_H