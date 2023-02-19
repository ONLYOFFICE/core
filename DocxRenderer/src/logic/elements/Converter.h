#pragma once
#include "Table.h"

namespace NSDocxRenderer
{
	class CConverter
	{
	public:
		void BuildLines(std::vector<CTextLine*>& rTextLines);
		void DetermineDominantGraphics(std::vector<CTextLine*>& rTextLines);

		void BuildParagraphes(double dPageWidth, eTextAssociationType eType,
							  CBaseItem::ElemType eBaseType, std::vector<CTextLine*>& rTextLines,
							  std::vector<CBaseItem*> &rOutputObjects);
		void BuildParagraphes(double dPageWidth, eTextAssociationType eType,
							  CBaseItem::ElemType eBaseType, std::vector<CTextLine*>& rTextLines,
							  std::vector<CTable*>& rTables, std::vector<CBaseItem*> &rOutputObjects);

		void CreateSingleLineParagraph(CTextLine *pLine, double dPageWidth, const double *pBeforeSpacing, std::vector<CBaseItem*> &rOutputObjects);
		void CreateSingleLineShape(CTextLine *pLine, std::vector<CBaseItem*>& rOutputObjects);
		void CreateShapeFormParagraphs(CParagraph *pParagraph, bool bIsSameTypeText, double dPageWidth, std::vector<CBaseItem*> &rOutputObjects);
		void CreateShapeFormTable(CTable* pParagraph, std::vector<CBaseItem *> &rOutputObjects);
		void CorrectionObjectesInShapes(std::vector<CBaseItem*>& rOutputObjects, double dPageWidth);

	private:
		CTextLine* GetNextTextLine(size_t& nCurrentIndex, std::vector<CTextLine*>& rTextLines, size_t* pIndexForCheking = nullptr);
		CTextLine* GetPrevTextLine(size_t nCurrentIndex, std::vector<CTextLine*>& rTextLines);
	};

}
