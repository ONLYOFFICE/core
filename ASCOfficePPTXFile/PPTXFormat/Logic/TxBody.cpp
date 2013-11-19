#pragma once
#include "stdafx.h"
#include "./TxBody.h"
#include "../Theme.h"
#include "./ClrMap.h"

namespace PPTX
{
	namespace Logic
	{
		CString TxBody::GetDocxTxBoxContent(NSBinPptxRW::CBinaryFileWriter* pWriter)
		{
			CString strXml = _T("<w:txbxContent ");

			strXml += _T("\
	xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
	xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
	xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
	xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
	xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
	xmlns:v=\"urn:schemas-microsoft-com:vml\" \
	xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
	xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
	xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
	xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
	xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
	xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
	xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
	xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
	xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
	xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
	xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
	xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
	xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" \
	xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"");

			strXml += _T(">");
			
			smart_ptr<PPTX::Theme> pTheme			= pWriter->ThemeDoc.smart_dynamic_cast<PPTX::Theme>();
			smart_ptr<PPTX::Logic::ClrMap> pClrMap	= pWriter->ClrMapDoc.smart_dynamic_cast<PPTX::Logic::ClrMap>();

			NSBinPptxRW::CXmlWriter oWriter;

			oWriter.WriteString(strXml);

			double dKoef = 635; // emu -> 20pt

			size_t nCount = Paragrs.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				PPTX::Logic::Paragraph& oPar = Paragrs[i];

				oWriter.StartNode(_T("w:p"));
				oWriter.StartAttributes();
				oWriter.EndAttributes();

				nullable<PPTX::Logic::RunProperties> pRunProps;
				pRunProps = new PPTX::Logic::RunProperties();

				if (oPar.pPr.is_init())
				{
					nullable<PPTX::Logic::TextParagraphPr> pPr;
					pPr = new PPTX::Logic::TextParagraphPr();
					
					if (oPar.pPr->lvl.is_init() && lstStyle.is_init())
					{
						int nLvl = *oPar.pPr->lvl;
						if (nLvl >= 0 && nLvl < 10)
						{
							if (lstStyle->levels[nLvl].is_init())
							{
								lstStyle->levels[nLvl]->Merge(pPr);

								if (lstStyle->levels[nLvl]->defRPr.is_init())
									lstStyle->levels[nLvl]->defRPr->Merge(pRunProps);
							}
						}
					}
					oPar.pPr->Merge(pPr);

					if (pPr->indent.is_init() || pPr->marL.is_init() || pPr->marR.is_init())
					{
						oWriter.StartNode(_T("w:ind"));
						oWriter.StartAttributes();
						if (oPar.pPr->marL.is_init())
							oWriter.WriteAttribute(_T("w:left"), (int)((double)(*pPr->marL) / 635));
						if (oPar.pPr->marR.is_init())
							oWriter.WriteAttribute(_T("w:right"), (int)((double)(*pPr->marR) / 635));
						if (oPar.pPr->indent.is_init())
							oWriter.WriteAttribute(_T("w:firstLine"), (int)((double)(*pPr->indent) / 635));
						oWriter.EndAttributes();
						oWriter.EndNode(_T("w:ind"));
					}

					if (pPr->algn.is_init())
					{
						LONG _code = pPr->algn->GetBYTECode();

						oWriter.StartNode(_T("w:jc"));
						switch (_code)
						{
						case 0:
							// center
							oWriter.WriteAttribute(_T("w:val"), (CString)_T("center"));
							break;
						case 2:
						case 3:
							// both
							oWriter.WriteAttribute(_T("w:val"), (CString)_T("both"));
							break;
						case 5:
							// right
							oWriter.WriteAttribute(_T("w:val"), (CString)_T("right"));
							break;
						default:
							oWriter.WriteAttribute(_T("w:val"), (CString)_T("left"));
							break;
						}

						oWriter.EndAttributes();
						oWriter.EndNode(_T("w:jc"));
					}

					if (pPr->fontAlgn.is_init())
					{
						BYTE nFA = pPr->fontAlgn->GetBYTECode();

						oWriter.StartNode(_T("w:textAlignment"));
						switch (nFA)
						{
						case 0: 
							oWriter.WriteAttribute(_T("w:val"), (CString)_T("auto")); 
							break;
						case 1: 
							oWriter.WriteAttribute(_T("w:val"), (CString)_T("bottom")); 
							break;
						case 2: 
							oWriter.WriteAttribute(_T("w:val"), (CString)_T("baseline")); 
							break;
						case 3: 
							oWriter.WriteAttribute(_T("w:val"), (CString)_T("center")); 
							break;
						case 4: 
							oWriter.WriteAttribute(_T("w:val"), (CString)_T("top")); 
							break;
						default:
							oWriter.WriteAttribute(_T("w:val"), (CString)_T("baseline")); 
							break;
						}

						oWriter.EndAttributes();
						oWriter.EndNode(_T("w:textAlignment"));
					}
				}

				size_t nCountRuns = oPar.RunElems.GetCount();
				for (size_t j = 0; j < nCountRuns; ++j)
				{
					if (oPar.RunElems[j].is<PPTX::Logic::Run>())
					{
						PPTX::Logic::Run& oRun = oPar.RunElems[j].as<PPTX::Logic::Run>();

						nullable<PPTX::Logic::RunProperties> pRPr;
						pRPr = new PPTX::Logic::RunProperties();
						pRunProps->Merge(pRPr);

						if (oRun.rPr.is_init())
							oRun.rPr->Merge(pRPr);

						oWriter.StartNode(_T("w:r"));
						oWriter.EndAttributes();

						// run props
						oWriter.StartNode(_T("w:rPr"));
						oWriter.EndAttributes();

						if (pRPr->b.get_value_or(false))
							oWriter.WriteString(_T("<w:b/>"));

						if (pRPr->i.get_value_or(false))
							oWriter.WriteString(_T("<w:i/>"));

						if (pRPr->strike.is_init())
						{
							BYTE lType = pRPr->strike->GetBYTECode();
							if (0 == lType)
								oWriter.WriteString(_T("<w:dstrike/>"));
							else if (2 == lType)
								oWriter.WriteString(_T("<w:strike/>"));
						}

						/*
						if (pRPr->sz.is_init())
						{
							double dSize = (double)pRPr->sz.get();
							dSize /= 100;
							int nSize = (int)(dSize * 2);
							
							CString strFS = _T("");
							strFS.Format(_T("<w:sz w:val=\"%d\"/><w:szCs w:val=\"%d\"/>"), nSize, nSize);
							oWriter.WriteString(strFS);
						}
						*/

						if (pRPr->u.is_init())
						{
							BYTE lType = pRPr->u->GetBYTECode();
							if (12 != lType)
								oWriter.WriteString(_T("<w:u w:val=\"single\"/>"));							
						}

						oWriter.EndNode(_T("w:rPr"));

						oWriter.StartNode(_T("w:t"));
						oWriter.StartAttributes();
						oWriter.WriteAttribute(_T("xml:space"), (CString)_T("preserve"));
						oWriter.EndAttributes();
						oWriter.WriteString(oRun.GetText());
						oWriter.EndNode(_T("w:t"));

						oWriter.EndNode(_T("w:r"));
					}
				}

				oWriter.EndNode(_T("w:p"));
			}

			oWriter.WriteString(_T("</w:txbxContent>"));

			return oWriter.GetXmlString();
		}
	} // namespace Logic
} // namespace PPTX

