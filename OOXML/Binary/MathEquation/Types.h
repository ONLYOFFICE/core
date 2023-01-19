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

#include <map>
#include "../../../DesktopEditor/common/Types.h"
#include "../../Base/Base.h"

namespace MathEquation
{
	struct TMathFont
	{
		std::string sName;
		bool        bItalic;
		bool        bBold;
	};

    typedef std::map<unsigned char, TMathFont> TFontMap;

	enum MBRACKETSTYPE
	{
		bracketsAngle            = 0x00, // < > Угловые скобки
		bracketsRound            = 0x01, // ( ) Круглые скобки
		bracketsCurve            = 0x02, // { } Фигурные скобки
		bracketsSquare           = 0x03, // [ ] Квадратные скобки
		bracketsLine             = 0x04, // | | Скобки в виде линии
		bracketsDLine            = 0x05, // || ||Скобки в виде двойной линии
		bracketsFloor            = 0x06, // Скобки с углом внизу
		bracketsCeil             = 0x07, // Скобки с углом наверху
		bracketsSquareOpenOpen   = 0x08, // [ [
		bracketsSquareCloseClose = 0x09, // ] ]
		bracketsSquareCloseOpen  = 0x0a, // ] [
		bracketsSquareRound      = 0x0b, // [ )
		bracketsRoundSquare      = 0x0c  // ( ]

	};

	enum MFRACTIONTYPES
	{
		fractionRegular = 0, // Обычная дробь
		fractionSlanted = 1  // Наклонная дробь
	};

	enum MSCRIPTALIGN
	{
		scriptalignRight  = 0,
		scriptalignCenter = 1,
		scriptalignLeft   = 2
	};

	enum MBARTYPE
	{
		bartypeLine        = 0, // простая линия
		bartypeDoubleLine  = 1, // двойная линия
		bartypeArrowLeft   = 2, // стрелка налево
		bartypeArrowRight  = 3, // стрелка направо
		bartypeArrowDouble = 4  // двойная стрелка (и налево, и направо)
	};

	enum MARROWTYPE
	{
		arrowtypeLeft   = 0, // стрелка налево
		arrowtypeRight  = 1, // стрелка направо
		arrowtypeDouble = 2  // двойная стрелка (и налево, и направо)
	};

	enum MINTEGRALTYPE
	{
		integraltypeSingle             = 0, // Обычный одинарный интеграл без индексов
		integraltypeSingleRSub         = 1, // Одинарный интеграл с правым нижним индексом
		integraltypeSingleRSubSup      = 2, // Одинарный интеграл с правыми верхним и нижним индексами
		integraltypeSingleOriented     = 3, // Одинарный ориентированный интеграл
		integraltypeSingleOrientedRSub = 4, // Одинарный ориентированный интеграл с правым нижним индексом

		integraltypeDouble             = 5,  // Двойной интеграл без индексов
		integraltypeDoubleRSub         = 6,  // Двойной интеграл с правым нижним индексом
		integraltypeDoubleOriented     = 7,  // Двойной ориентированный интеграл
		integraltypeDoubleOrientedRSub = 8,  // Двойной ориентированный интеграл с правым нижним индексом

		integraltypeTriple             = 9,  // Тройной интеграл без индексов
		integraltypeTripleRSub         = 10, // Тройной интеграл с правым нижним индексом
		integraltypeTripleOriented     = 11, // Тройной ориентированный интеграл
		integraltypeTripleOrientedRSub = 12, // Тройной ориентированный интеграл с правым нижним индексом

		integraltypeSingleCSubSup      = 13, // Одинарный интеграл с центральными верхним и нижним индексами
		integraltypeSingleCSub         = 14, // Одинарный интеграл с центральным нижним индексом
		integraltypeSingleOrientedCSub = 15, // Одинарный ориентированный интеграл с центральным нижним индексом

		integraltypeDoubleOrientedCSub = 16, // Двойной ориентированный интеграл с центральным нижним индексом
		integraltypeDoubleCSub         = 17, // Двойной интеграл с центральным нижним индексом

		integraltypeTripleOrientedCSub = 18, // Тройной ориентированный интеграл с центральным нижним индексом
		integraltypeTripleCSub         = 19  // Тройной интеграл с центральным нижним индексом
	};

	enum MNARRAYTYPE
	{
		narySumCSub          = 0,  // Знак суммы с центральным нижним индексом
		narySumCSubSup       = 1,  // Знак суммы с центральными нижним и верхним индексами
		narySum              = 2,  // Знак суммы без индексов

		narySumRSub          = 3,  // Знак суммы с правым нижним индексом
		narySumRSubSup       = 4,  // Знак суммы с правыми нижним и верхним индексами

		naryProdCSub         = 5,  // Знак произведения с центральным нижним индексом
		naryProdCSubSup      = 6,  // Знак произведения с центральными нижним и верхним индексами
		naryProd             = 7,  // Знак произведения  без индексов

		naryProdRSub         = 8,  // Знак произведения с правым нижним индексом
		naryProdRSubSup      = 9,  // Знак произведения с правыми нижним и верхним индексами

		naryCoProdCSub       = 10, // Знак копроизведения с центральным нижним индексом
		naryCoProdCSubSup    = 11, // Знак копроизведения с центральными нижним и верхним индексами
		naryCoProd           = 12, // Знак копроизведения  без индексов

		naryCoProdRSub       = 13, // Знак копроизведения с правым нижним индексом
		naryCoProdRSubSup    = 14, // Знак копроизведения с правыми нижним и верхним индексами

		naryUnionCSub        = 15, // Знак объединения с центральным нижним индексом
		naryUnionCSubSup     = 16, // Знак объединения с центральными нижним и верхним индексами
		naryUnion            = 17, // Знак объединения  без индексов

		naryUnionRSub        = 18, // Знак объединения с правым нижним индексом
		naryUnionRSubSup     = 19, // Знак объединения с правыми нижним и верхним индексами

		naryIntersectCSub    = 20, // Знак пересечения с центральным нижним индексом
		naryIntersectCSubSup = 21, // Знак пересечения с центральными нижним и верхним индексами
		naryIntersect        = 22, // Знак пересечения  без индексов

		naryIntersectRSub    = 23, // Знак пересечения с правым нижним индексом
		naryIntersectRSubSup = 24  // Знак пересечения с правыми нижним и верхним индексами
	};

	enum MLONGDIVISION
	{
		longdivisionWithResult = 0, // Знак деления столбиком без частного
		longdivisionRegular    = 1  // Знак деления столбиком с частным
	};

	enum MANGLEBRACKETSWITHSEPARATORTYPE
	{
		angbrwithsepBoth  = 0, // < | >
		angbrwithsepLeft  = 1, // < |
		angbrwithsepRight = 2  // | >		
	};

	enum MEMBELTYPE
	{
		embelDot         = 0x02, // точка         .
		embelDDot        = 0x03, // две точки     ..  
		embelDDDot       = 0x04, // три точки     ...
		embelPrime       = 0x05, // штрих         '
		embelDPrime      = 0x06, // два штриха    "
		embelLPrime      = 0x07, // штрих слева   '
		embelTilde       = 0x08, // волна         ~
		embelHat         = 0x09, // крышка(шляпа) ^
		embelSlash       = 0x0a, // косая черта   /
		embelRArrow      = 0x0b, // стрелка вправо <-
		embelLArrow      = 0x0c, // стрелка влево ->
		embelDArrow      = 0x0d, // стрелка в обе стороны <->
		embelRHarpoon    = 0x0e, // гарпун вправо
		embelLHarpoon    = 0x0f, // гарпун влево
		embelStrikeout   = 0x10, // зачеркивание
		embelBar         = 0x11, // линия
		embelTPrime      = 0x12, // три штриха '''
		embelFrown       = 0x13, // дуга вверх
		embelSmile       = 0x14, // дуга вниз

		embelMin         = 0x02,
		embelMax         = 0x14
	};

	enum MMATRIXBORDERTYPE
	{
		matrixborderNone  = 0x00,
		matrixborderSolid = 0x01,
		matrixborderDash  = 0x02,
		matrixborderDot   = 0x03
	};

	enum MMATRIXHORALIGN
	{
		matrixhoralignLeft      = 0x00, // прилегание к левому краю
		matrixhoralignCenter    = 0x01, // прилегание по центру
		matrixhoralignRight     = 0x02, // прилегание к правому краю
		matrixhoralignEqualSign = 0x03, // прилегание по знаку =
		matrixhoralignCommaSign = 0x04  // прилегание по знаку ,
	};

	enum MMATRIXVERALIGN
	{
		matrixveralignTop      = 0x00,
		matrixveralignBaseLine = 0x01,
		matrixveralginBottom   = 0x02
	};

	enum MSPECIALSYMBOLS
	{
		specialsymAlignment  = 0xeb00,
		specialsymZeroSpace  = 0xeb01,
		specialsymThinSpace  = 0xeb02,
		specialsymThickSpace = 0xeb04,
		specialsymLargeSpace = 0xeb05,
		specialsymOnePtSpace = 0xeb08
	};
}
