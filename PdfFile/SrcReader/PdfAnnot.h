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

#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"

namespace PdfReader
{

struct CAnnotParent final
{
    CAnnotParent()
    {
        unFlags = 0;
        unRefNum = 0;
        unRefNumParent = 0;
    }

    void ToWASM(NSWasm::CData& oRes);

    unsigned int unFlags;
    unsigned int unRefNum; // Номер ссылки на объект
    unsigned int unRefNumParent; // Номер ссылки на объект родителя
    std::string sT;
    std::string sV;
    std::string sDV;
};

struct CBorderType final
{
    CBorderType()
    {
        nType = 5;
        dWidth = 1;
        dDashesAlternating = 3;
        dGaps = 3;
    }

    void ToWASM(NSWasm::CData& oRes);

    BYTE nType;
    double dWidth;
    double dDashesAlternating;
    double dGaps;
};

//------------------------------------------------------------------------
// PdfReader::CAction
//------------------------------------------------------------------------

struct CAction
{
    virtual void ToWASM(NSWasm::CData& oRes);

    CAction() : pNext(NULL) {}
    virtual ~CAction() { RELEASEOBJECT(pNext); }

    std::string sType;
    CAction* pNext;
};
struct CActionGoTo       final : public CAction
{
    unsigned int unPage;
    unsigned int unKindFlag;
    double pRect[4];
    BYTE nKind;

    void ToWASM(NSWasm::CData& oRes) override;
};
struct CActionURI        final : public CAction
{
    std::string sURI;

    void ToWASM(NSWasm::CData& oRes) override;
};
struct CActionNamed      final : public CAction
{
    std::string sNamed;

    void ToWASM(NSWasm::CData& oRes) override;
};
struct CActionJavaScript final : public CAction
{
    std::string sJavaScript;

    void ToWASM(NSWasm::CData& oRes) override;
};
struct CActionHide       final : public CAction
{
    bool bHideFlag;
    std::vector<std::string> arrAnnotName;

    void ToWASM(NSWasm::CData& oRes) override;
};
struct CActionResetForm  final : public CAction
{
    CActionResetForm()
    {
        unFlags = 0;
    }

    unsigned int unFlags;
    std::vector<std::string> arrAnnotName;

    void ToWASM(NSWasm::CData& oRes) override;
};

//------------------------------------------------------------------------
// PdfReader::CAnnotAP
//------------------------------------------------------------------------

class CAnnotAP final
{

};

//------------------------------------------------------------------------
// PdfReader::CAnnot
//------------------------------------------------------------------------

class CAnnot
{
public:
    CAnnot(PDFDoc* pdfDoc, AcroFormField* pField);
	CAnnot(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);
    virtual ~CAnnot();

    virtual void ToWASM(NSWasm::CData& oRes);

private:
    unsigned int m_unAFlags;
    unsigned int m_unAnnotFlag; // Флаг аннотации - F
    unsigned int m_unRefNum; // Номер ссылки на объект
    unsigned int m_unRefNumParent; // Номер ссылки на объект родителя
    unsigned int m_unPage; // Страница
	double m_pRect[4]; // Координаты
	double m_dBE; // Эффекты границы
	std::vector<double> m_arrC; // Специальный цвет
    CBorderType* m_pBorder; // Граница
};

//------------------------------------------------------------------------
// PdfReader::CWidgetAnnot
//------------------------------------------------------------------------

class CAnnotWidget : public CAnnot
{
public:
    CAnnotWidget(PDFDoc* pdfDoc, AcroFormField* pField);

    virtual void ToWASM(NSWasm::CData& oRes) override;

    unsigned int m_unFlags;
    BYTE m_nType; // Тип - FT + флаги
    unsigned int m_unFieldFlag; // Флаг - Ff

private:
    unsigned int m_unR; // Поворот аннотации относительно страницы - R
	std::vector<double> m_arrTC; // Цвет текста - из DA
    std::vector<double> m_arrBC; // Цвет границ - BC
	std::vector<double> m_arrBG; // Цвет фона - BG
    std::vector<CAction*> m_arrAction; // Действия
    BYTE m_nQ; // Выравнивание текста - Q
    BYTE m_nH; // Режим выделения - H
    std::string m_sTU; // Альтернативное имя поля, используется во всплывающей подсказке и сообщениях об ошибке - TU
    std::string m_sDS; // Строка стиля по умолчанию - DS
    std::string m_sDV; // Значение по-умолчанию - DV
    std::string m_sContents; // Альтернативный текст - Contents
    std::string m_sT; // Частичное имя поля - T
};

class CAnnotWidgetBtn final : public CAnnotWidget
{
public:
    CAnnotWidgetBtn(PDFDoc* pdfDoc, AcroFormField* pField);

    void ToWASM(NSWasm::CData& oRes) override;
private:
    BYTE m_nStyle;
    BYTE m_nTP;
    BYTE m_nSW;
    BYTE m_nS;
    unsigned int m_unIFFlag;
    std::string m_sCA;
    std::string m_sRC;
    std::string m_sAC;
    std::string m_sAP_N_Yes;
    double m_dA1, m_dA2;
};

class CAnnotWidgetTx final : public CAnnotWidget
{
public:
    CAnnotWidgetTx(PDFDoc* pdfDoc, AcroFormField* pField);

    void ToWASM(NSWasm::CData& oRes) override;
private:
    std::string m_sV;
    unsigned int m_unMaxLen;
    std::string m_sRV;
};

class CAnnotWidgetCh final : public CAnnotWidget
{
public:
    CAnnotWidgetCh(PDFDoc* pdfDoc, AcroFormField* pField);

    void ToWASM(NSWasm::CData& oRes) override;
private:
    std::string m_sV;
    std::vector<std::pair<std::string, std::string>> m_arrOpt;
    unsigned int m_unTI;
};

class CAnnotWidgetSig final : public CAnnotWidget
{
public:
    CAnnotWidgetSig(PDFDoc* pdfDoc, AcroFormField* pField);

    void ToWASM(NSWasm::CData& oRes) override;
};

//------------------------------------------------------------------------
// PdfReader::CMarkupAnnot
//------------------------------------------------------------------------

class CMarkupAnnot : public CAnnot
{
public:
	CMarkupAnnot(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	virtual void ToWASM(NSWasm::CData& oRes) override;

	unsigned int m_unFlags;

private:
	unsigned int m_unRefNumPopup; // Номер ссылки на всплывающую аннотацию
	double m_dCA; // Значение непрозрачности
	std::string m_sT; // Текстовая метка, пользователь добавивший аннотацию
	std::string m_sRC; // Форматированный текст для отображения во всплывающем окне
};

//------------------------------------------------------------------------
// PdfReader::CMarkupAnnot
//------------------------------------------------------------------------

class CAnnotText final : public CMarkupAnnot
{
public:
	CAnnotText(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	void ToWASM(NSWasm::CData& oRes) override;
private:
	bool m_bOpen; // Отображаться открытой?
	BYTE m_nName; // Иконка
	BYTE m_nState; // Состояние
	BYTE m_nStateModel; // Модель состояния
};

//------------------------------------------------------------------------
// PdfReader::CAnnots
//------------------------------------------------------------------------

class CAnnots
{
public:
    CAnnots(PDFDoc* pdfDoc);
    ~CAnnots();

    void ToWASM(NSWasm::CData& oRes);

private:
    std::vector<std::string> m_arrCO; // Порядок вычислений - CO
    std::vector<CAnnotParent*> m_arrParents; // Родительские Fields
    std::vector<CAnnot*> m_arrAnnots;
};

}

#endif // _PDF_READER_ANNOT_H
