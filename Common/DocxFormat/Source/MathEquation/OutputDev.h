#ifndef _MATH_EQUATION_OUTPUTDEV_H
#define _MATH_EQUATION_OUTPUTDEV_H

#include "Types.h"

namespace MathEquation
{
	struct TMathFont;
	class MathEquation;
	class IOutputDev
	{
	public:

		// Начало и конец формулы
		virtual void BeginEquation() = 0;
		virtual void EndEquation()   = 0;

		// Начало и конец блока.
		virtual void BeginBlock() = 0;
		virtual void EndBlock()   = 0;

		// Выставляем размер в текущем блоке
		virtual void SetSize(uint16_t nSize) = 0;

		// Добавляем символ. К символу могут быть добавлены разные элементы. Зачеркивание, добавление акцента и т.д.
		virtual void BeginChar(Unicode_t uChar, uint8_t nTypeFace, bool bSpecialSymbol) = 0;
		virtual void AddCharEmbel(MEMBELTYPE eType) = 0;
		virtual void EndChar() = 0;

		// Матрица. Количество блоков здесь равно nRows * nCol, посылаются последовательно в обычном порядке (первая строка слева направо, вторая строка слева направо и т.д.)
		virtual void BeginMatrix(uint8_t nVAlign, MMATRIXHORALIGN eHorAlign, MMATRIXVERALIGN eVerAlign, bool bEqualRows, bool bEqualCols, uint8_t nRows, uint8_t nCols, uint8_t* pVerBorders, uint8_t* pHorBorders) = 0;
		virtual void EndMatrix  () = 0;

		virtual void StartPile(uint8_t nHAlign, uint8_t nVAlign) = 0;
		virtual void EndPile() = 0;

		// Скобки с элементом внутри
		virtual void BeginBrackets(MBRACKETSTYPE eType, bool bOpen, bool bClose) = 0;
		virtual void EndBrackets  (MBRACKETSTYPE eType, bool bOpen, bool bClose) = 0;

		// Корень. Первый блок - основание, второй, если есть, степень.
		virtual void BeginRoot(bool bDegree) = 0;
		virtual void EndRoot  () = 0;

		// Дроби. Первый блок - числитель, второй блок - знаменатель.
		virtual void BeginFraction(MFRACTIONTYPES eType, bool bInline) = 0;
		virtual void EndFraction  () = 0;

		// Если bInline = true, то последовательность блоков Base, Sub, Sup
		// Если bInline = false, то последовательность блоков  Sub, Sup, Base
		virtual void BeginScript(MSCRIPTALIGN eAlign, bool bBase = false, bool bSup = false, bool bSub = false, bool bInline = true) = 0;
		virtual void EndScript  () = 0;

		// Линия свеху или снизу относительно текста
		virtual void BeginBar(MBARTYPE eType, bool bTop) = 0;
		virtual void EndBar  () = 0;

		// Стрелка с надписью сверху или снизу (отличе от BeginBar/EndBar в том, что здесь стрелка основной 
		// объект, а текст - это небольшая надпись, а у тех функций наоборот)
		virtual void BeginArrow(MARROWTYPE eType, bool bTop) = 0;
		virtual void EndArrow  () = 0;

		// Интегралы. Последовательность блоков всегда следующая(если блок присутствует) Base, Sub, Sup
		virtual void BeginIntegral(MINTEGRALTYPE eType) = 0;
		virtual void EndIntegral  () = 0;

		// Фигурная скобка с текстом над/под объектом. Сначала идет основной объект, потом надпись фигурной скобки.
		virtual void BeginVerticalBrace(bool bTop) = 0;
		virtual void EndVerticalBrace  () = 0;

		// Сумма, произведение, копроизведение, объединение, пересечение. Последовательность блоков Base, Sub, Sup
		virtual void BeingNArray(MNARRAYTYPE eType) = 0;
		virtual void EndNArray  () = 0;

		// Деление столбиком. С частным или без. Вначале всегда идет основание, потом если есть частное.
		virtual void BeginLongDivision(MLONGDIVISION eType) = 0;
		virtual void EndLongDivision  () = 0;

		// < | > - такой элемент
		virtual void BeginAngleBracketsWithSeparator(MANGLEBRACKETSWITHSEPARATORTYPE eType) = 0;
		virtual void EndAngleBracketsWithSeparator  () = 0;

		void AddFont(uint8_t nTypeFace, std::string sName, bool bBold, bool bItalic)
		{
			TMathFont aFont;
			aFont.sName   = sName;
			aFont.bBold   = bBold;
			aFont.bItalic = bItalic;
			m_mFonts[nTypeFace] = aFont;
		}

		TMathFont* GetFont(uint8_t nTypeFace)
		{
			TFontMap::iterator itFind = m_mFonts.find(nTypeFace);
			if (itFind != m_mFonts.end())			
				return &(itFind->second);

			return NULL;
		}

	protected:

		TFontMap m_mFonts;
	};

}

#endif