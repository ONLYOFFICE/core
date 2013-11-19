#pragma once
#ifndef PPTX_LOGIC_TIMING_INCLUDE_H_
#define PPTX_LOGIC_TIMING_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "TnLst.h"
#include "BldLst.h"

namespace PPTX
{
	namespace Logic
	{
		class Timing : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Timing)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				tnLst	= node.ReadNode(_T("p:tnLst"));
				bldLst	= node.ReadNode(_T("p:bldLst"));
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(tnLst);
				oValue.WriteNullable(bldLst);

				return XmlUtils::CreateNode(_T("p:timing"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				// TODO:
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				return;
				// TODO:
				pWriter->StartNode(_T("p:timing"));
				pWriter->EndAttributes();
				pWriter->EndNode(_T("p:timing"));
			}

		public:
			nullable<TnLst>		tnLst;
			nullable<BldLst>	bldLst;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(tnLst.IsInit())
					tnLst->SetParentPointer(this);
				if(bldLst.IsInit())
					bldLst->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TIMING_INCLUDE_H_