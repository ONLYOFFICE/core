#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2OfficeDrawing
{
	void Converter::TextParse(const PPTX::Logic::TxBody& pShapeText, CTextAttributesEx& pText)
	{
		pText.m_sText = _T("");
		for(std::list<PPTX::Logic::Paragraph>::const_iterator SpTextPar = pShapeText.Paragrs->begin(); SpTextPar != pShapeText.Paragrs->end(); SpTextPar++)
		{
			STextPFRun paragraph;

			paragraph.lCount = SpTextPar->GetWText().length();

			int level = 0;
			if(SpTextPar->pPr.is_init())
				level = SpTextPar->pPr->lvl.get_value_or(0);

			paragraph.lIndentLevel = level;

			ParagraphPropertiesConvert(SpTextPar->pPr, paragraph);
			pText.m_arPFs.Add(paragraph);

			for(std::list<PPTX::Logic::RunElem>::const_iterator SpTextRun = SpTextPar->RunElems->begin(); SpTextRun != SpTextPar->RunElems->end(); SpTextRun++)
			{
				STextCFRun CFRun;
				CStringW RunText = CStringW(SpTextRun->GetWText().c_str());
				CFRun.lCount = SpTextRun->GetWText().length();

				if(SpTextRun->is<PPTX::Logic::Run>())
				{
					RunPropertiesConvert(SpTextRun->as<PPTX::Logic::Run>().rPr, CFRun);
				}
				else if(SpTextRun->is<PPTX::Logic::Fld>())
				{
					RunPropertiesConvert(SpTextRun->as<PPTX::Logic::Fld>().rPr, CFRun);
				}
				else if(SpTextRun->is<PPTX::Logic::Br>())
				{
					RunPropertiesConvert(SpTextRun->as<PPTX::Logic::Br>().rPr, CFRun);
				}
				if(CFRun.lCount > 0)
				{
					pText.m_sText += RunText;
					pText.m_arCFs.Add(CFRun);
				}
			}

			STextCFRun CFRun;
			CFRun.lCount = 1;
			CFRun.hasSize = true;

			if(pText.m_arCFs.GetCount() > 0)
				CFRun.fontSize = pText.m_arCFs.GetAt(pText.m_arCFs.GetCount() - 1).fontSize;
			else if(SpTextPar->endParaRPr.is_init())
			{
				if(SpTextPar->endParaRPr->sz.is_init())
					CFRun.fontSize = (WORD)(SpTextPar->endParaRPr->sz.get()/100);
			}

			pText.m_arCFs.Add(CFRun);
			pText.m_sText += L"\n";
		}
	}
}