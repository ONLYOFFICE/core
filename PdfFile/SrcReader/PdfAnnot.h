/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#ifndef _PDF_READER_ANNOT_H
#define _PDF_READER_ANNOT_H

#include <string>
#include <vector>

#include "../lib/xpdf/PDFDoc.h"
#include "../lib/xpdf/AcroForm.h"

namespace PdfReader
{

enum EAnnotType
{
    annot_type_unknown,
    annot_type_widget
};

struct CAnnotParent
{
    CAnnotParent()
    {
        unFlags = 0;
        unRefNum = 0;
        unRefNumParent = 0;
    }

    unsigned int unFlags;
    unsigned int unRefNum; // Номер ссылки на объект
    unsigned int unRefNumParent; // Номер ссылки на объект родителя
    std::string sT;
    std::string sV;
    std::string sDV;
};

struct CBorderType
{
    CBorderType()
    {
        nType = 5;
        dWidth = 1;
        dDashesAlternating = 3;
        dGaps = 3;
    }

    BYTE nType;
    double dWidth;
    double dDashesAlternating;
    double dGaps;
};

struct CAction
{
    CAction()
    {
        nA = 0;
    }

    std::string sType;
    BYTE nA;
};

//------------------------------------------------------------------------
// PdfReader::CAnnotAP
//------------------------------------------------------------------------

class CAnnotAP
{

};

//------------------------------------------------------------------------
// PdfReader::CAnnot
//------------------------------------------------------------------------

class CAnnot
{
public:
    CAnnot(XRef* xref, AcroFormField* pField);
    virtual ~CAnnot();

    virtual EAnnotType GetAnnotType() const
    {
        return annot_type_unknown;
    }

private:
    unsigned int m_unAFlags;
    unsigned int m_unAnnotFlag; // Флаг аннотации - F
    unsigned int m_unRefNum; // Номер ссылки на объект
    unsigned int m_unRefNumParent; // Номер ссылки на объект родителя
    unsigned int m_unPage; // Страница
    double m_pRect[4]; // Координаты - Rect
    CBorderType* m_pBorder; // Граница
};

//------------------------------------------------------------------------
// PdfReader::CWidgetAnnot
//------------------------------------------------------------------------

class CWidgetAnnot : public CAnnot
{
public:
    CWidgetAnnot(XRef* xref, AcroFormField* pField);

    EAnnotType GetAnnotType() const
    {
        return annot_type_widget;
    }
private:
    unsigned int m_unFlags;
    unsigned int m_unFieldFlag; // Флаг - Ff
    unsigned int n_unR; // Поворот аннотации относительно страницы - R
    std::vector<double> m_arrColorSpace; // Цветовое пространство - из DA
    std::vector<double> m_arrBC; // Цвет границ - BC
    std::vector<double> m_arrBG; // Цвет фона - BG
    std::vector<double> m_arrC; // Специальный цвет для аннотации - C
    std::vector<CAction> m_arrAction; // Действия
    BYTE m_nQ; // Выравнивание текста - Q
    BYTE m_nType; // Тип - FT + флаги
    BYTE m_nH; // Режим выделения - H
    std::string m_sTU; // Альтернативное имя поля, используется во всплывающей подсказке и сообщениях об ошибке - TU
    std::string m_sDS; // Строка стиля по умолчанию - DS
    std::string m_sDV; // Значение по-умолчанию - DV
    std::string m_sContents; // Альтернативный текст - Contents
    std::string m_sT; // Частичное имя поля - T
    double m_dBE; // Эффекты границы - BE
};

//------------------------------------------------------------------------
// PdfReader::CAnnots
//------------------------------------------------------------------------

class CAnnots
{
public:
    CAnnots(XRef* xref, AcroForm* pAcroForms);
    ~CAnnots();

private:
    std::vector<std::string> m_arrCO; // Порядок вычислений - CO
    std::vector<CAnnotParent*> m_arrParents; // Родительские Fields
    std::vector<CAnnot*> m_arrAnnots;
};

}

#endif // _PDF_READER_ANNOT_H
