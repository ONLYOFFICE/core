#pragma once
#ifndef PPTX_LOGIC_NVGRAPHICFRAMEPR_INCLUDE_H_
#define PPTX_LOGIC_NVGRAPHICFRAMEPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CNvPr.h"
#include "CNvGraphicFramePr.h"
#include "NvPr.h"


namespace PPTX
{
	namespace Logic
	{
		class NvGraphicFramePr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(NvGraphicFramePr)

			NvGraphicFramePr& operator=(const NvGraphicFramePr& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				cNvPr				= oSrc.cNvPr;
				cNvGraphicFramePr	= oSrc.cNvGraphicFramePr;
				nvPr				= oSrc.nvPr;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cNvPr				= node.ReadNode(_T("p:cNvPr"));
				cNvGraphicFramePr	= node.ReadNode(_T("p:cNvGraphicFramePr"));
				nvPr				= node.ReadNode(_T("p:nvPr"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(cNvPr);
				oValue.Write(cNvGraphicFramePr);
				oValue.Write(nvPr);

				return XmlUtils::CreateNode(_T("p:nvGraphicFramePr"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX && pWriter->m_lGroupIndex >= 0)
				{
					cNvPr.toXmlWriter2(_T("wpg"), pWriter);
					pWriter->WriteString(_T("<wpg:cNvFrPr/>"));
					return;
				}

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX && pWriter->m_lGroupIndex >= 0)
				{
					pWriter->StartNode(_T("xdr:nvGraphicFramePr"));
					pWriter->EndAttributes();

					cNvPr.toXmlWriter(pWriter);
					cNvGraphicFramePr.toXmlWriter(pWriter);
					
					pWriter->EndNode(_T("xdr:nvGraphicFramePr"));
					return;
				}

				pWriter->StartNode(_T("p:nvGraphicFramePr"));
				pWriter->EndAttributes();

				cNvPr.toXmlWriter(pWriter);
				cNvGraphicFramePr.toXmlWriter(pWriter);
				nvPr.toXmlWriter(pWriter);
				
				pWriter->EndNode(_T("p:nvGraphicFramePr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, cNvPr);
				pWriter->WriteRecord1(1, cNvGraphicFramePr);
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
							cNvGraphicFramePr.fromPPTY(pReader);
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
			CNvPr				cNvPr;
			CNvGraphicFramePr	cNvGraphicFramePr;
			NvPr				nvPr;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cNvPr.SetParentPointer(this);
				cNvGraphicFramePr.SetParentPointer(this);
				nvPr.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NVGRAPHICFRAMEPR_INCLUDE_H