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

#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"

#include "RendererOutputDev.h"

namespace PdfReader
{

//------------------------------------------------------------------------
// PdfReader::CAction
//------------------------------------------------------------------------

class CAction
{
public:
	virtual ~CAction() { RELEASEOBJECT(pNext); }

	virtual void ToWASM(NSWasm::CData& oRes);

	std::string sType;
	CAction* pNext;
protected:
	CAction() : pNext(NULL) {}
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
public:
	CAnnotAP(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CFontList*  pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, const char* sView, const char* sButtonView, AcroFormField* pField);
	CAnnotAP(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CFontList*  pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, const char* sView, Object* oAnnotRef);
	~CAnnotAP();

	void ToWASM(NSWasm::CData& oRes);

private:
	struct CAnnotAPView final
	{
		std::string sAPName;
		std::string sASName;
		BYTE* pAP;
		BYTE* pText;
	};
	void WriteAppearance(unsigned int nColor, CAnnotAPView* pView);

	unsigned int m_unRefNum; // Номер ссылки на объект
	double m_dx1, m_dy1, m_dx2, m_dy2;
	double m_dWScale, m_dHScale;
	double m_dWTale;
	double m_dHTale;
	int m_nRx1, m_nRy1, m_nWidth, m_nHeight;
	std::vector<CAnnotAPView*> m_arrAP;

	Gfx* m_gfx;
	CBgraFrame* m_pFrame;
	RendererOutputDev* m_pRendererOut;
	NSGraphics::IGraphicsRenderer* m_pRenderer;

	void Init(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CFontList*  pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex);
	void Init(AcroFormField* pField);
	void Init(Object* oAnnot);
	void Draw(PDFDoc* pdfDoc, Object* oAP, int nRasterH, int nBackgroundColor, int nPageIndex, AcroFormField* pField, const char* sView, const char* sButtonView);
	void Draw(PDFDoc* pdfDoc, Object* oAP, int nRasterH, int nBackgroundColor, Object* oAnnotRef, const char* sView);

	void Clear();
};

//------------------------------------------------------------------------
// PdfReader::CAnnot
//------------------------------------------------------------------------

class CAnnot
{
public:
	virtual ~CAnnot();

	virtual void ToWASM(NSWasm::CData& oRes);

protected:
	CAnnot(PDFDoc* pdfDoc, AcroFormField* pField);
	CAnnot(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	double m_dHeight; // Высота холста, для Y трансормации

private:
	struct CBorderType final
	{
		CBorderType()
		{
			nType = 0;
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
	CBorderType* getBorder(Object* oBorder, bool bBSorBorder);

	unsigned int m_unAFlags;
	unsigned int m_unAnnotFlag; // Флаг аннотации - F
	unsigned int m_unRefNum; // Номер ссылки на объект
	unsigned int m_unPage; // Страница
	double m_pRect[4]; // Координаты
	std::pair<BYTE, double> m_pBE; // Эффекты границы
	std::string m_sContents; // Отображаемый текст
	std::string m_sNM; // Уникальное имя
	std::string m_sM; // Дата последнего изменения
	std::vector<double> m_arrC; // Специальный цвет
	CBorderType* m_pBorder; // Граница
};

//------------------------------------------------------------------------
// PdfReader::CWidgetAnnot
//------------------------------------------------------------------------

class CAnnotWidget : public CAnnot
{
public:
	virtual ~CAnnotWidget();

protected:
	CAnnotWidget(PDFDoc* pdfDoc, AcroFormField* pField);

	virtual void ToWASM(NSWasm::CData& oRes) override;

	unsigned int m_unFlags;
	BYTE m_nType; // Тип - FT + флаги
	unsigned int m_unFieldFlag; // Флаг - Ff

private:
	unsigned int m_unR; // Поворот аннотации относительно страницы - R
	unsigned int m_unRefNumParent; // Номер ссылки на объект родителя
	std::vector<double> m_arrTC; // Цвет текста - из DA
	std::vector<double> m_arrBC; // Цвет границ - BC
	std::vector<double> m_arrBG; // Цвет фона - BG
	std::vector<CAction*> m_arrAction; // Действия
	BYTE m_nQ; // Выравнивание текста - Q
	BYTE m_nH; // Режим выделения - H
	std::string m_sTU; // Альтернативное имя поля, используется во всплывающей подсказке и сообщениях об ошибке - TU
	std::string m_sDS; // Строка стиля по умолчанию - DS
	std::string m_sDV; // Значение по-умолчанию - DV
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
	std::vector< std::pair<std::string, std::string> > m_arrOpt;
	unsigned int m_unTI;
};

class CAnnotWidgetSig final : public CAnnotWidget
{
public:
	CAnnotWidgetSig(PDFDoc* pdfDoc, AcroFormField* pField);

	void ToWASM(NSWasm::CData& oRes) override;
};

//------------------------------------------------------------------------
// PdfReader::CAnnotPopup
//------------------------------------------------------------------------

class CAnnotPopup final : public CAnnot
{
public:
	CAnnotPopup(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	unsigned int m_unFlags;
	unsigned int m_unRefNumParent; // Номер ссылки на объект родителя
};

//------------------------------------------------------------------------
// PdfReader::CMarkupAnnot
//------------------------------------------------------------------------

class CMarkupAnnot : public CAnnot
{
protected:
	CMarkupAnnot(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	virtual void ToWASM(NSWasm::CData& oRes) override;

	unsigned int m_unFlags;

private:
	BYTE m_nRT; // Тип аннотации-ответа
	unsigned int m_unRefNumPopup; // Номер ссылки на всплывающую аннотацию
	unsigned int m_unRefNumIRT; // Номер ссылки на аннотацию-ответ
	double m_dCA; // Значение непрозрачности
	std::string m_sT; // Текстовая метка, пользователь добавивший аннотацию
	std::string m_sRC; // Форматированный текст для отображения во всплывающем окне
	std::string m_sCreationDate; // Дата создания
	std::string m_sSubj; // Краткое описание
};

//------------------------------------------------------------------------
// PdfReader::CAnnotText
//------------------------------------------------------------------------

class CAnnotText final : public CMarkupAnnot
{
public:
	CAnnotText(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	void ToWASM(NSWasm::CData& oRes) override;
private:
	BYTE m_nName; // Иконка
	BYTE m_nState; // Состояние
	BYTE m_nStateModel; // Модель состояния
};

//------------------------------------------------------------------------
// PdfReader::CAnnotInk
//------------------------------------------------------------------------

class CAnnotInk final : public CMarkupAnnot
{
public:
	CAnnotInk(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	std::vector< std::vector<double> > m_arrInkList; // Путь
};

//------------------------------------------------------------------------
// PdfReader::CAnnotLine
//------------------------------------------------------------------------

class CAnnotLine final : public CMarkupAnnot
{
public:
	CAnnotLine(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nIT; // Назначение аннотации
	BYTE m_nCP; // Расположение заголовка аннотации
	double m_dLL; // Длина линий выноски
	double m_dLLE; // Продолжение линий выноски
	double m_dLLO; // Длина смещения выноски
	BYTE m_nLE[2]; // Стили окончания линии
	double m_pL[4]{}; // Координаты линии
	double m_pCO[2]; // Смещение текста подписи
	std::vector<double> m_arrIC; // Цвет окончаний линии
	// TODO Measure Показатели масштаба, единиц измерения
};

//------------------------------------------------------------------------
// PdfReader::CAnnotTextMarkup
//------------------------------------------------------------------------

class CAnnotTextMarkup final : public CMarkupAnnot
{
public:
	CAnnotTextMarkup(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nSubtype; // Подтип TextMarkup аннотации
	std::vector<double> m_arrQuadPoints; // Координаты
};

//------------------------------------------------------------------------
// PdfReader::CAnnotSquareCircle
//------------------------------------------------------------------------

class CAnnotSquareCircle final : public CMarkupAnnot
{
public:
	CAnnotSquareCircle(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nSubtype; // Подтип Square или Circle аннотации
	double m_pRD[4]{}; // Различия Rect и фактического размера
	std::vector<double> m_arrIC; // Цвет заполнения
};

//------------------------------------------------------------------------
// PdfReader::CAnnotPolygonPolyline
//------------------------------------------------------------------------

class CAnnotPolygonLine final : public CMarkupAnnot
{
public:
	CAnnotPolygonLine(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nIT; // Назначение аннотации
	BYTE m_nSubtype; // Подтип Polygon или Polyline аннотации
	BYTE m_nLE[2]; // Стили окончания линии
	std::vector<double> m_arrIC; // Цвет заполнения
	std::vector<double> m_arrVertices; // Координаты вершин
	// TODO Measure Показатели масштаба, единиц измерения
};

//------------------------------------------------------------------------
// PdfReader::CAnnotFreeText
//------------------------------------------------------------------------

class CAnnotFreeText final : public CMarkupAnnot
{
public:
	CAnnotFreeText(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nQ; // Выравнивание текста - Q
	BYTE m_nIT; // Назначение аннотации
	BYTE m_nLE; // Стиль окончания линии
	std::string m_sDS; // Строка стиля по умолчанию - DS
	double m_pRD[4]{}; // Различия Rect и фактического размера
	std::vector<double> m_arrCL; // Координаты выноски
};

//------------------------------------------------------------------------
// PdfReader::CAnnotCaret
//------------------------------------------------------------------------

class CAnnotCaret final : public CMarkupAnnot
{
public:
	CAnnotCaret(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nSy; // Связанный символ - Sy
	double m_pRD[4]{}; // Различия Rect и фактического размера
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

	void getParents(XRef* xref, Object* oFieldRef);

	std::vector<std::string> m_arrCO; // Порядок вычислений - CO
	std::vector<CAnnotParent*> m_arrParents; // Родительские Fields
	std::vector<CAnnot*> m_arrAnnots;
};

}

#endif // _PDF_READER_ANNOT_H
