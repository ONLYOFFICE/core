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
	CAnnotAP(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, const char* sView, const char* sButtonView, AcroFormField* pField, int nStartRefID);
	CAnnotAP(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, const char* sView, Object* oAnnotRef, int nStartRefID);
	~CAnnotAP();

	void ToWASM(NSWasm::CData& oRes);

private:
	struct CAnnotAPView final
	{
		BYTE nBlendMode;
		std::string sAPName;
		std::string sASName;
		BYTE* pAP;
	};

	void WriteAppearance(unsigned int nColor, CAnnotAPView* pView);
	BYTE GetBlendMode();
	void Init(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList*  pFontList, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex);
	void Init(Object* oAnnot);
	void Draw(PDFDoc* pdfDoc, Object* oAP, int nRasterH, int nBackgroundColor, int nPageIndex, AcroFormField* pField, const char* sView, const char* sButtonView);
	void Draw(PDFDoc* pdfDoc, Object* oAP, int nRasterH, int nBackgroundColor, Object* oAnnotRef, const char* sView);
	void Clear();

	unsigned int m_unRefNum; // Номер ссылки на объект
	double m_dx1, m_dy1, m_dx2, m_dy2;
	double m_dCropX, m_dCropY;
	double m_dWScale, m_dHScale;
	double m_dRWScale, m_dRHScale;
	double m_dWTale, m_dHTale;
	double m_dRx1, m_dRy1;
	int m_nWidth, m_nHeight;
	std::vector<CAnnotAPView*> m_arrAP;
	bool m_bIsStamp;

	Gfx* m_gfx;
	CBgraFrame* m_pFrame;
	RendererOutputDev* m_pRendererOut;
	NSGraphics::IGraphicsRenderer* m_pRenderer;
};

//------------------------------------------------------------------------
// PdfReader::CAnnot
//------------------------------------------------------------------------

class CAnnot
{
public:
	virtual ~CAnnot();

	virtual void ToWASM(NSWasm::CData& oRes);
	void SetPage(unsigned int nPage) { m_unPage = nPage; }

	struct CBorderType final
	{
		CBorderType()
		{
			nType = 0;
			dWidth = 1;
		}

		void ToWASM(NSWasm::CData& oRes);

		BYTE nType;
		double dWidth;
		std::vector<double> arrDash;
	};

protected:
	CAnnot(PDFDoc* pdfDoc, AcroFormField* pField, int nStartRefID);
	CAnnot(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);
	std::string DictLookupString(Object* pObj, const char* sName, int nByte);

	unsigned int m_unAFlags;
	unsigned int m_unFlags;
	double m_dHeight; // Высота холста, для Y трансформации
	double m_dX; // Смещение по X для трансформации
	double m_pRect[4]; // Координаты

private:
	unsigned int m_unAnnotFlag; // Флаг аннотации - F
	unsigned int m_unRefNum; // Номер ссылки на объект
	unsigned int m_unPage; // Страница
	std::pair<BYTE, double> m_pBE; // Эффекты границы
	std::string m_sContents; // Отображаемый текст
	std::string m_sNM; // Уникальное имя
	std::string m_sM; // Дата последнего изменения
	std::string m_sOUserID; // OO User ID
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

	void SetFont(PDFDoc* pdfDoc, AcroFormField* pField, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList);
	void SetButtonFont(PDFDoc* pdfDoc, AcroFormField* pField, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList);
	unsigned int GetRefNumParent() { return m_unRefNumParent; }
	const std::string& GetFullName() { return m_sFullName; }
	void SetFullName(const std::string& sFullName) { m_sFullName = sFullName; }
	void AddFullName(const std::string& sPrefixForm) { m_sFullName += sPrefixForm; }
	bool ChangeFullName(const std::string& sPrefixForm);
	void ClearActions();
	virtual std::string GetType() = 0;
	virtual void ToWASM(NSWasm::CData& oRes) override;

protected:
	CAnnotWidget(PDFDoc* pdfDoc, AcroFormField* pField, int nStartRefID);

	std::string FieldLookupString(AcroFormField* pField, const char* sName, int nByte);

	BYTE m_nType; // Тип - FT + флаги
	unsigned int m_unFieldFlag; // Флаг - Ff

private:
	unsigned int m_unR; // Поворот аннотации относительно страницы - R
	unsigned int m_unRefNumParent; // Номер ссылки на объект родителя
	unsigned int m_unFontStyle; // Стиль шрифта - из DA
	double m_dFontSize; // Размер шрифта - из DA
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
	std::string m_sFontKey; // Уникальный идентификатор шрифта
	std::string m_sFullName; // Полное имя поля
	std::string m_sFontName; // Имя шрифта - из DA
	std::string m_sOMetadata; // OO метаданные формы
	std::string m_sActualFontName; // Имя замененного шрифта
	std::string m_sButtonFontName; // Имя шрифта кнопки
};

class CAnnotWidgetBtn final : public CAnnotWidget
{
public:
	CAnnotWidgetBtn(PDFDoc* pdfDoc, AcroFormField* pField, int nStartRefID);
	virtual std::string GetType() override { return "Btn"; }

	void ToWASM(NSWasm::CData& oRes) override;
private:
	BYTE m_nStyle;
	BYTE m_nTP;
	BYTE m_nSW;
	BYTE m_nS;
	unsigned int m_unIFFlag;
	std::string m_sV;
	std::string m_sCA;
	std::string m_sRC;
	std::string m_sAC;
	std::string m_sAP_N_Yes;
	double m_dA1, m_dA2;
};

class CAnnotWidgetTx final : public CAnnotWidget
{
public:
	CAnnotWidgetTx(PDFDoc* pdfDoc, AcroFormField* pField, int nStartRefID);
	virtual std::string GetType() override { return "Tx"; }

	void ToWASM(NSWasm::CData& oRes) override;
private:
	std::string m_sV;
	unsigned int m_unMaxLen;
	std::string m_sRV;
};

class CAnnotWidgetCh final : public CAnnotWidget
{
public:
	CAnnotWidgetCh(PDFDoc* pdfDoc, AcroFormField* pField, int nStartRefID);
	virtual std::string GetType() override { return "Ch"; }

	void ToWASM(NSWasm::CData& oRes) override;
private:
	std::string m_sV;
	std::vector<std::string> m_arrV;
	std::vector<int> m_arrI;
	std::vector< std::pair<std::string, std::string> > m_arrOpt;
	unsigned int m_unTI;
};

class CAnnotWidgetSig final : public CAnnotWidget
{
public:
	CAnnotWidgetSig(PDFDoc* pdfDoc, AcroFormField* pField, int nStartRefID);
	virtual std::string GetType() override { return "Sig"; }

	void ToWASM(NSWasm::CData& oRes) override;
};

//------------------------------------------------------------------------
// PdfReader::CAnnotPopup
//------------------------------------------------------------------------

class CAnnotPopup final : public CAnnot
{
public:
	CAnnotPopup(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	unsigned int m_unFlags;
	unsigned int m_unRefNumParent; // Номер ссылки на объект родителя
};

//------------------------------------------------------------------------
// PdfReader::CAnnotMarkup
//------------------------------------------------------------------------

class CAnnotMarkup : public CAnnot
{
public:
	struct CFontData final
	{
		bool bFind;
		BYTE nAlign;
		unsigned int unFontFlags; // 0 Bold, 1 Italic, 3 зачеркнутый, 4 подчеркнутый, 5 vertical-align, 6 actual font
		double dFontSise;
		double dVAlign;
		double dColor[3];
		std::string sFontFamily;
		std::string sActualFont;
		std::string sText;

		CFontData() : bFind(false), nAlign(0), unFontFlags(4), dFontSise(10), dVAlign(0), dColor{0, 0, 0} {}
		CFontData(const CFontData& oFont) : bFind(oFont.bFind), nAlign(oFont.nAlign), unFontFlags(oFont.unFontFlags), dFontSise(oFont.dFontSise), dVAlign(oFont.dVAlign),
			dColor{oFont.dColor[0], oFont.dColor[1], oFont.dColor[2]}, sFontFamily(oFont.sFontFamily), sActualFont(oFont.sActualFont), sText(oFont.sText) {}
	};

	void SetFont(PDFDoc* pdfDoc, Object* oAnnotRef, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList);
	static std::vector<CAnnotMarkup::CFontData*> ReadRC(const std::string& sRC);

protected:
	CAnnotMarkup(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);
	virtual ~CAnnotMarkup();

	virtual void ToWASM(NSWasm::CData& oRes) override;

	std::vector<CFontData*> m_arrRC; // Форматированный текст

private:
	BYTE m_nRT; // Тип аннотации-ответа
	unsigned int m_unRefNumPopup; // Номер ссылки на всплывающую аннотацию
	unsigned int m_unRefNumIRT; // Номер ссылки на аннотацию-ответ
	double m_dCA; // Значение непрозрачности
	std::string m_sT; // Текстовая метка, пользователь добавивший аннотацию
	std::string m_sCreationDate; // Дата создания
	std::string m_sSubj; // Краткое описание
};

//------------------------------------------------------------------------
// PdfReader::CAnnotText
//------------------------------------------------------------------------

class CAnnotText final : public CAnnotMarkup
{
public:
	CAnnotText(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

	void ToWASM(NSWasm::CData& oRes) override;
private:
	BYTE m_nName; // Иконка
	BYTE m_nState; // Состояние
	BYTE m_nStateModel; // Модель состояния
};

//------------------------------------------------------------------------
// PdfReader::CAnnotInk
//------------------------------------------------------------------------

class CAnnotInk final : public CAnnotMarkup
{
public:
	CAnnotInk(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	std::vector< std::vector<double> > m_arrInkList; // Путь
};

//------------------------------------------------------------------------
// PdfReader::CAnnotLine
//------------------------------------------------------------------------

class CAnnotLine final : public CAnnotMarkup
{
public:
	CAnnotLine(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

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

class CAnnotTextMarkup final : public CAnnotMarkup
{
public:
	CAnnotTextMarkup(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nSubtype; // Подтип TextMarkup аннотации
	std::vector<double> m_arrQuadPoints; // Координаты
};

//------------------------------------------------------------------------
// PdfReader::CAnnotSquareCircle
//------------------------------------------------------------------------

class CAnnotSquareCircle final : public CAnnotMarkup
{
public:
	CAnnotSquareCircle(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nSubtype; // Подтип Square или Circle аннотации
	double m_pRD[4]{}; // Различия Rect и фактического размера
	std::vector<double> m_arrIC; // Цвет заполнения
};

//------------------------------------------------------------------------
// PdfReader::CAnnotPolygonPolyline
//------------------------------------------------------------------------

class CAnnotPolygonLine final : public CAnnotMarkup
{
public:
	CAnnotPolygonLine(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

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

class CAnnotFreeText final : public CAnnotMarkup
{
public:
	CAnnotFreeText(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nQ; // Выравнивание текста - Q
	BYTE m_nIT; // Назначение аннотации
	BYTE m_nLE; // Стиль окончания линии
	int m_nRotate;
	std::string m_sDS; // Строка стиля по умолчанию - DS
	double m_pRD[4]{}; // Различия Rect и фактического размера
	std::vector<double> m_arrCFromDA; // Цвет границы
	std::vector<double> m_arrCL; // Координаты выноски
};

//------------------------------------------------------------------------
// PdfReader::CAnnotCaret
//------------------------------------------------------------------------

class CAnnotCaret final : public CAnnotMarkup
{
public:
	CAnnotCaret(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nSy; // Связанный символ - Sy
	double m_pRD[4]{}; // Различия Rect и фактического размера
};

//------------------------------------------------------------------------
// PdfReader::CAnnotFileAttachment
//------------------------------------------------------------------------

class CAnnotFileAttachment final : public CAnnotMarkup
{
public:
	CAnnotFileAttachment(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);
	virtual ~CAnnotFileAttachment();

	void ToWASM(NSWasm::CData& oRes) override;

	struct CEmbeddedFile
	{
		BYTE* pFile;
		int nLength;
		bool bFree;

		CEmbeddedFile() : pFile(NULL), nLength(0), bFree(true) {}
		~CEmbeddedFile() { if (bFree) RELEASEARRAYOBJECTS(pFile); }
	};

	struct CEmbeddedFiles
	{
		CEmbeddedFile* m_pF;
		CEmbeddedFile* m_pUF;
		CEmbeddedFile* m_pDOS;
		CEmbeddedFile* m_pMac;
		CEmbeddedFile* m_pUnix;

		CEmbeddedFiles() : m_pF(NULL), m_pUF(NULL), m_pDOS(NULL), m_pMac(NULL), m_pUnix(NULL) {}
		~CEmbeddedFiles()
		{
			RELEASEOBJECT(m_pF);
			RELEASEOBJECT(m_pUF);
			RELEASEOBJECT(m_pDOS);
			RELEASEOBJECT(m_pMac);
			RELEASEOBJECT(m_pUnix);
		}
	};

private:
	std::string m_sName; // Иконка
	std::string m_sFS;   // Файловая система
	std::string m_sDesc; // Описание файла
	std::string m_sF;    // Спецификация файла (обратная совместимость)
	std::string m_sUF;   // Спецификация файла (кросс-платформенная и межъязыковая совместимость)
	std::string m_sDOS;  // Спецификация файла DOS
	std::string m_sMac;  // Спецификация файла Mac
	std::string m_sUnix; // Спецификация файла Unix
	std::pair<std::string, std::string> m_sID; // Идентификатор файла
	CEmbeddedFiles* m_pEF; // EF содержит F/UF/DOS/Mac/Unix со ссылками на встроенные файловые потоки по соответствующим спецификациях
	// TODO RF содержит F/UF/DOS/Mac/Unix с массивами связанных файлов по соответствующим спецификациях
	// TODO Cl коллекция для создания пользовательского интерфейса
};

//------------------------------------------------------------------------
// PdfReader::CAnnotStamp
//------------------------------------------------------------------------

class CAnnotStamp final : public CAnnotMarkup
{
public:
	CAnnotStamp(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	std::string m_sName; // Иконка
	double m_dRotate;
	double m_dX1, m_dY1, m_dX2, m_dY2, m_dX3, m_dY3, m_dX4, m_dY4;
};

//------------------------------------------------------------------------
// PdfReader::CAnnotRedact
//------------------------------------------------------------------------

class CAnnotRedact final : public CAnnotMarkup
{
public:
	CAnnotRedact(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);

	void SetFont(const std::map<std::wstring, std::wstring>& mFont);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nQ; // Выравнивание текста
	unsigned int m_unFontStyle; // Стиль шрифта - из DA
	double m_dFontSize; // Размер шрифта - из DA
	std::string m_sFontName; // Имя шрифта - из DA
	std::string m_sActualFontName; // Имя замененного шрифта
	std::string m_sOverlayText; // Текст наложения
	std::vector<double> m_arrQuadPoints; // Координаты
	std::vector<double> m_arrIC; // Цвет заполнения
	std::vector<double> m_arrCFromDA; // Цвет текста
};

//------------------------------------------------------------------------
// PdfReader::CAnnots
//------------------------------------------------------------------------

class CAnnots
{
public:
	CAnnots(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, int nStartPage, int nStartRefID);
	~CAnnots();

	void ToWASM(NSWasm::CData& oRes);
	bool ChangeFullNameAnnot(int nAnnot, const std::string& sPrefixForm);
	bool ChangeFullNameParent(int nParent, const std::string& sPrefixForm);
	const std::vector<CAnnotWidget*>& GetAnnots() { return m_arrAnnots; }

private:
	struct CAnnotParent final
	{
		CAnnotParent()
		{
			unFlags = 0;
			unRefNum = 0;
			unRefNumParent = 0;
		}
		~CAnnotParent()
		{
			ClearActions();
		}
		void ClearActions();

		void ToWASM(NSWasm::CData& oRes);

		unsigned int unFlags;
		unsigned int unRefNum; // Номер ссылки на объект
		unsigned int unMaxLen; // Ограничение на максимальную длину text field
		unsigned int unFieldFlag; // Флаг Ff
		unsigned int unRefNumParent; // Номер ссылки на объект родителя
		std::vector<int> arrI;
		std::vector<std::string> arrV;
		std::vector< std::pair<std::string, std::string> > arrOpt;
		std::vector<CAction*> arrAction; // Действия
		std::string sT;
		std::string sV;
		std::string sDV;
		std::string sFullName;
	};

	void getParents(PDFDoc* pdfDoc, Object* oFieldRef, int nStartRefID);

	std::vector<int> m_arrCO; // Порядок вычислений - CO
	std::vector<CAnnotParent*> m_arrParents; // Родительские Fields
	std::vector<CAnnotWidget*> m_arrAnnots;
};

//------------------------------------------------------------------------
// PdfReader::CAnnotFonts
//------------------------------------------------------------------------

class CAnnotFonts final
{
public:
	static bool IsBaseFont(const std::wstring& wsName);
	static std::map<std::wstring, std::wstring> GetAllFonts(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, bool bIsNeedCMap);
	static std::wstring GetFontData(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, Object* oFontRef, std::string& sFontName, std::string& sActualFontName, bool& bBold, bool& bItalic, bool bIsNeedCMap = false);
	static bool GetFontFromAP(PDFDoc* pdfDoc, AcroFormField* pField, Object* oFontRef, std::string& sFontKey);
	static std::map<std::wstring, std::wstring> GetAnnotFont(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, Object* oAnnotRef);
	static std::map<std::wstring, std::wstring> GetFreeTextFont(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, Object* oAnnotRef, std::vector<CAnnotMarkup::CFontData*>& arrRC);
private:
	static bool FindFonts(Object* oStream, int nDepth, Object* oResFonts);
};

}

#endif // _PDF_READER_ANNOT_H
