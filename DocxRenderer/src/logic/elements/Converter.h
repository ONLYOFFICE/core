#pragma once
#include "Table.h"
#include "../managers/ParagraphStyleManager.h"

namespace NSDocxRenderer
{
	class CConverter
	{
	public:
		void BuildLines(std::vector<CTextLine*>& rTextLines);
		void DetermineDominantGraphics(std::vector<CTextLine*>& rTextLines);

		void BuildParagraphes(double dPageWidth, TextAssociationType eType,
							  COutputObject::eOutputType eBaseType, std::vector<CTextLine*>& rTextLines,
							  std::vector<COutputObject*>& rOutputObjects, CParagraphStyleManager* pParagraphStyleManager);
//		void BuildParagraphes(double dPageWidth, TextAssociationType eType,
//							  COutputObject::eOutputType eBaseType, std::vector<CTextLine*>& rTextLines,
//							  std::vector<CTable*>& rTables, std::vector<COutputObject*>& rOutputObjects, CParagraphStyleManager* pParagraphStyleManager);

		void CreateSingleLineParagraph(CTextLine *pLine, double dPageWidth, const double *pBeforeSpacing, std::vector<COutputObject*>& rOutputObjects);
		void CreateSingleLineShape(CTextLine *pLine, std::vector<COutputObject*>& rOutputObjects);
		void CreateShapeFormParagraphs(CParagraph *pParagraph, bool bIsSameTypeText, double dPageWidth, std::vector<COutputObject*>& rOutputObjects);
//		void CreateShapeFormTable(CTable* pParagraph, std::vector<COutputObject*>& rOutputObjects);
		void CorrectionObjectesInShapes(std::vector<COutputObject*>& rOutputObjects, double dPageWidth);

	private:
		CTextLine* GetNextTextLine(size_t& nCurrentIndex, std::vector<CTextLine*>& rTextLines, size_t* pIndexForCheking = nullptr);
		CTextLine* GetPrevTextLine(size_t nCurrentIndex, std::vector<CTextLine*>& rTextLines);
	};

}
