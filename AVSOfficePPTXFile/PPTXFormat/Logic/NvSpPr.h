#pragma once
#ifndef PPTX_LOGIC_NVSPPR_INCLUDE_H_
#define PPTX_LOGIC_NVSPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CNvPr.h"
#include "CNvSpPr.h"
#include "NvPr.h"


namespace PPTX
{
	namespace Logic
	{
		class NvSpPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(NvSpPr)

			NvSpPr& operator=(const NvSpPr& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				cNvPr	= oSrc.cNvPr;
				cNvSpPr = oSrc.cNvSpPr;
				nvPr	= oSrc.nvPr;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("cNvPr") == strName)
							cNvPr = oNode;
						else if (_T("cNvSpPr") == strName)
							cNvSpPr = oNode;
						else if (_T("nvPr") == strName)
							nvPr = oNode;
					}
				}

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(cNvPr);
				oValue.Write(cNvSpPr);
				oValue.Write(nvPr);

				return XmlUtils::CreateNode(_T("p:nvSpPr"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->StartNode(_T("xdr:nvSpPr"));
				else
					pWriter->StartNode(_T("p:nvSpPr"));

				pWriter->EndAttributes();

				cNvPr.toXmlWriter(pWriter);
				cNvSpPr.toXmlWriter(pWriter);
				
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_PPTX)
					nvPr.toXmlWriter(pWriter);
				
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->EndNode(_T("xdr:nvSpPr"));
				else
					pWriter->EndNode(_T("p:nvSpPr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, cNvPr);
				pWriter->WriteRecord1(1, cNvSpPr);
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
							cNvSpPr.fromPPTY(pReader);
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
			CNvPr	cNvPr;
			CNvSpPr cNvSpPr;
			NvPr	nvPr;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cNvPr.SetParentPointer(this);
				cNvSpPr.SetParentPointer(this);
				nvPr.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NVSPPR_INCLUDE_H