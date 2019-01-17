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
#ifndef _ASC_FONTCONVERTER_STRING_EXT_H_
#define _ASC_FONTCONVERTER_STRING_EXT_H_

#include <stdarg.h>

namespace NSFontConverter
{
    //---------------------------------------------------------------------------------------
    // класс StringExt - аналог CString
    //---------------------------------------------------------------------------------------

    class StringExt
    {
    public:

        // Создаем пустую строку.
        StringExt();

        // Создаем строку из Сишной строки.
        StringExt(const char *sString);

        // Создаем строку из <nLength> символов в <sString>. Данная строка
        // может содержать нулевые символы.
        StringExt(const char *sString, int nLength);

        // Создаем строку из <nLength> символов, начиная с <nIndex>, строки <seString>.
        StringExt(StringExt *seString, int nIndex, int nLength);

        // Копируем строку.
        StringExt(StringExt *seString);
        StringExt *Copy()
        {
            return new StringExt(this);
        }

        // Соединяем две строки.
        StringExt(StringExt *seString1, StringExt *seString2);

        // Переводим целое значение в строку.
        static StringExt *FromInt(int nValue);

        // Создаем форматированную строку. Функция подобна printf, но без проблем
        // с переполнением строки. Формат выглядит следующим образом:
        //     {<nArg>:[<nWidth>][.<precision>]<type>}
        // где:
        // - <nArg> номер аргумента (нумерация начинается с 0).
        // -- PS: сами аргументы должны идти по порядку, а использовать их можно
        //    многократно и в любом порядке.
        // - <nWidth> ширина поля, если она отрицательна, тогда прилежание будет
        //   сменено на противоположное, а пустые места будут заполнены нулями.
        // - <nPrecision> количество знаков после запятой
        // - <nType> тип один из слудющих:
        //     d, x, o, b -- целое(int) в десятичной, шестнадцатиричной, восьмиричной
        //                   и двоичной системах исчисления
        //     ud, ux, uo, ub -- тоже самое, только беззнаковое целое(uint)
        //     ld, lx, lo, lb, uld, ulx, ulo, ulb -- аналогичной long и ulong
        //     f, g -- double
        //     c -- char
        //     s -- string (char *)
        //     t -- StringExt *
        //     w -- Пробелы; значение аргументы означает количество пробелов
        // Для вывода фигурных скобок надо использовать {{ и }}.
        static StringExt *Format (char *sFormat, ...);
        static StringExt *FormatV(char *sFormat, va_list sArgList);

        // Деструктор.
        ~StringExt();

        int GetLength()
        {
            return m_nLength;
        }

        // Возвращаем строку в виде char*.
        char *GetBuffer()
        {
            return m_sData;
        }

        char GetAt(int nIndex)
        {
            return m_sData[ nIndex ];
        }
        void SetAt(int nIndex, char nChar)
        {
            m_sData[ nIndex ] = nChar;
        }

        // Очищаем строку.
        StringExt *Clear();

        // Добавляем символ или строку.
        StringExt *Append(char nChar);
        StringExt *Append(StringExt *seString);
        StringExt *Append(const char *sString);
        StringExt *Append(const char *sString, int nLength);

        // Добавляем форматированную строку.
        StringExt *AppendFormat (char *sFormat, ...);
        StringExt *AppendFormatV(char *sFormat, va_list sArgList);

        // Вставляем символ или строку.
        StringExt *Insert(int nIndex, char nChar);
        StringExt *Insert(int nIndex, StringExt  *seString);
        StringExt *Insert(int nIndex, const char *sString);
        StringExt *Insert(int nIndex, const char *sString, int nLength);

        // Удаляем один символ или массив символов.
        StringExt *Delete(int nIndex, int nCount = 1);

        // Делаем в строке все символы большими/маленькими буквами.
        StringExt *MakeUpper();
        StringExt *MakeLower();

        // Сравнение двух строк:  -1:<  0:=  +1:>
        int Compare (StringExt *seString);
        int CompareN(StringExt *seString, int nCount);
        int Compare (const char *sString);
        int CompareN(const char *sString, int nCount);

    private:

        void Resize(int nLength);

        static void FormatInt   (         long nValue, char *sBuffer, int nBufferSize, bool bZeroFill, int nWidth, int nBase, char **ppData, int *nLen);
        static void FormatUInt  (unsigned long nValue, char *sBuffer, int nBufferSize, bool bZeroFill, int nWidth, int nBase, char **ppData, int *nLen);
        static void FormatDouble(       double nValue, char *sBuffer, int nBufferSize, int nPrecision, bool bTrim,            char **ppData, int *nLen);

    private:

        int   m_nLength;
        char *m_sData;

    };
}

#endif /* _ASC_FONTCONVERTER_STRING_EXT_H_ */
