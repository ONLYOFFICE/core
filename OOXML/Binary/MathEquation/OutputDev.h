/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

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
        virtual void SetSize(_UINT16 nSize) = 0;

		// Добавляем символ. К символу могут быть добавлены разные элементы. Зачеркивание, добавление акцента и т.д.
        virtual void BeginChar(unsigned short uChar, unsigned char nTypeFace, bool bSpecialSymbol) = 0;
		virtual void AddCharEmbel(MEMBELTYPE eType) = 0;
		virtual void EndChar() = 0;

		// Матрица. Количество блоков здесь равно nRows * nCol, посылаются последовательно в обычном порядке (первая строка слева направо, вторая строка слева направо и т.д.)
        virtual void BeginMatrix(unsigned char nVAlign, MMATRIXHORALIGN eHorAlign, MMATRIXVERALIGN eVerAlign, bool bEqualRows, bool bEqualCols, unsigned char nRows, unsigned char nCols, unsigned char* pVerBorders, unsigned char* pHorBorders) = 0;
		virtual void EndMatrix  () = 0;

        virtual void StartPile(unsigned char nHAlign, unsigned char nVAlign) = 0;
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

        void AddFont(unsigned char nTypeFace, std::string sName, bool bBold, bool bItalic)
		{
			TMathFont aFont;
			aFont.sName   = sName;
			aFont.bBold   = bBold;
			aFont.bItalic = bItalic;
			m_mFonts[nTypeFace] = aFont;
		}

        TMathFont* GetFont(unsigned char nTypeFace)
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
