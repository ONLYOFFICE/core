#pragma once
#ifndef PPTX_SLIDES_CLRMAPOVR_INCLUDE_H_
#define PPTX_SLIDES_CLRMAPOVR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "ClrMap.h"

namespace PPTX
{
	namespace Logic
	{
		class ClrMapOvr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ClrMapOvr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				overrideClrMapping = node.ReadNodeNoNS(_T("overrideClrMapping"));
				if (overrideClrMapping.is_init())
					overrideClrMapping->m_name = _T("a:overrideClrMapping");
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				if (overrideClrMapping.IsInit())
					return _T("<p:clrMapOvr>") + overrideClrMapping->toXML() + _T("</p:clrMapOvr>");
				return _T("<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, overrideClrMapping);				
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (overrideClrMapping.is_init())
				{
					pWriter->WriteString(_T("<p:clrMapOvr>"));
					overrideClrMapping->m_name = _T("a:overrideClrMapping");
					overrideClrMapping->toXmlWriter(pWriter);
					pWriter->WriteString(_T("</p:clrMapOvr>"));
				}
				else
				{
					pWriter->WriteString(_T("<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>"));
				}
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _e = pReader->GetPos() + pReader->GetLong() + 4;

				if (pReader->GetPos() < _e)
				{
					pReader->Skip(1); // "0"-rectype
					overrideClrMapping = new Logic::ClrMap();
					overrideClrMapping->fromPPTY(pReader);					
				}

				pReader->Seek(_e);
			}

		public:
			nullable<ClrMap> overrideClrMapping;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(overrideClrMapping.IsInit())
					overrideClrMapping->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_CLRMAPOVR_INCLUDE_H_