#pragma once
#ifndef PPTX_LOGIC_BR_INCLUDE_H_
#define PPTX_LOGIC_BR_INCLUDE_H_

#include "RunBase.h"
#include "./../RunProperties.h"

namespace PPTX
{
	namespace Logic
	{
		class Br : public RunBase
		{
		public:
			PPTX_LOGIC_BASE(Br)

			Br& operator=(const Br& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				rPr = oSrc.rPr;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				rPr = node.ReadNode(_T("a:rPr"));
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(rPr);

				return XmlUtils::CreateNode(_T("a:br"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:br"));
				pWriter->EndAttributes();

				pWriter->Write(rPr);
				
				pWriter->EndNode(_T("a:br"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(PARRUN_TYPE_BR);

				pWriter->WriteRecord2(0, rPr);

				pWriter->EndRecord();
			}

			virtual CString GetText()const{return _T("\n");};
		public:
			nullable<RunProperties> rPr;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(rPr.IsInit())
					rPr->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BR_INCLUDE_H