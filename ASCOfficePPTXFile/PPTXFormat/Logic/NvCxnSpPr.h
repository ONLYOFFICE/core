#pragma once
#ifndef PPTX_LOGIC_NVCXNSPPR_INCLUDE_H_
#define PPTX_LOGIC_NVCXNSPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CNvPr.h"
#include "CNvCxnSpPr.h"
#include "NvPr.h"


namespace PPTX
{
	namespace Logic
	{
		class NvCxnSpPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(NvCxnSpPr)

			NvCxnSpPr& operator=(const NvCxnSpPr& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				cNvPr		= oSrc.cNvPr;
				cNvCxnSpPr	= oSrc.cNvCxnSpPr;
				nvPr		= oSrc.nvPr;

				return *this;
			}

		public:
            virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cNvPr		= node.ReadNode(_T("p:cNvPr"));
				cNvCxnSpPr	= node.ReadNode(_T("p:cNvCxnSpPr"));
				nvPr		= node.ReadNode(_T("p:nvPr"));

				FillParentPointersForChilds();
			}

            virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(cNvPr);
				oValue.Write(cNvCxnSpPr);
				oValue.Write(nvPr);

				return XmlUtils::CreateNode(_T("<p:nvCxnSpPr>"), oValue);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:nvCxnSpPr"));
				pWriter->EndAttributes();

				cNvPr.toXmlWriter(pWriter);
				cNvCxnSpPr.toXmlWriter(pWriter);
				nvPr.toXmlWriter(pWriter);

				pWriter->EndNode(_T("p:nvCxnSpPr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, cNvPr);
				pWriter->WriteRecord1(1, cNvCxnSpPr);
				pWriter->WriteRecord1(2, nvPr);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							cNvPr.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							cNvCxnSpPr.fromPPTY(pReader);
							break;
						}
						case 2:
						{
							nvPr.fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			CNvPr		cNvPr;
			CNvCxnSpPr	cNvCxnSpPr;
			NvPr		nvPr;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cNvPr.SetParentPointer(this);
				cNvCxnSpPr.SetParentPointer(this);
				nvPr.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NVCXNSPPR_INCLUDE_H
