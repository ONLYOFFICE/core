#pragma once
#ifndef PPTX_LOGIC_SLIDE_PIC_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_PIC_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "NvPicPr.h"
#include "Fills/BlipFill.h"
#include "SpPr.h"
#include "ShapeStyle.h"

namespace PPTX
{
	namespace Logic
	{

		class Pic : public WrapperWritingElement
		{
		public:
			Pic();
			virtual ~Pic();
			explicit Pic(XmlUtils::CXmlNode& node);
			const Pic& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual CString toXML() const;
			virtual void GetRect(RECT& pRect)const;
			virtual CString GetFullPicName()const;
			virtual CString GetVideoLink()const;
			virtual CString GetAudioLink()const;
			DWORD GetFill(UniFill& fill)const;
			DWORD GetLine(Ln& line)const;
			double GetStTrim () const;
			double GetEndTrim () const;
			long GetRefId() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(SPTREE_TYPE_PIC);

				pWriter->WriteRecord1(0, nvPicPr);
				pWriter->WriteRecord1(1, blipFill);
				pWriter->WriteRecord1(2, spPr);
				pWriter->WriteRecord2(3, style);

				pWriter->EndRecord();
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->StartNode(_T("xdr:pic"));
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
				{
					pWriter->StartNode(_T("pic:pic"));
					pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("xmlns:pic"), (CString)_T("http://schemas.openxmlformats.org/drawingml/2006/picture"));
				}
				else
					pWriter->StartNode(_T("p:pic"));

				pWriter->EndAttributes();

				nvPicPr.toXmlWriter(pWriter);

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					blipFill.m_namespace = _T("xdr");
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
					blipFill.m_namespace = _T("pic");
				else 
					blipFill.m_namespace = _T("p");

				if (blipFill.blip.is_init())
					blipFill.blip->m_namespace = _T("a");
				blipFill.toXmlWriter(pWriter);

				pWriter->m_lFlag = 1;
				spPr.toXmlWriter(pWriter);
				pWriter->m_lFlag = 0;
				
				pWriter->Write(style);

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->EndNode(_T("xdr:pic"));
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
					pWriter->EndNode(_T("pic:pic"));
				else
					pWriter->EndNode(_T("p:pic"));

			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				nvPicPr.cNvPr.id = -1;
				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							nvPicPr.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							blipFill.fromPPTY(pReader);
							break;
						}
						case 2:
						{
							spPr.fromPPTY(pReader);
							break;
						}
						case 3:
						{
							style = new ShapeStyle();
							style->m_ns = _T("p");
							style->fromPPTY(pReader);
							break;
						}
						default:
						{
							break;
						}
					}
				}

				pReader->Seek(_end_rec);
			}

			void toXmlWriterVML(NSBinPptxRW::CXmlWriter* pWriter, smart_ptr<PPTX::WrapperFile>& oTheme, smart_ptr<PPTX::WrapperWritingElement>& oClrMap);

		public:

			NvPicPr					nvPicPr;
			BlipFill				blipFill;
			SpPr					spPr;
			nullable<ShapeStyle>	style;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_PIC_INCLUDE_H_
