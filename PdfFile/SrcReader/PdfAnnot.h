/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
#include "PdfFont.h"

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

	unsigned int m_unRefNum; // Object reference number
	double m_dx1, m_dy1, m_dx2, m_dy2;
	double m_dCropX, m_dCropY;
	double m_dWScale, m_dHScale;
	double m_dRWScale, m_dRHScale;
	double m_dWTale, m_dHTale;
	double m_dRx1, m_dRy1;
	double m_dx1S, m_dy2S;
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
	double m_dHeight; // Canvas height, for Y transformation
	double m_dX; // X offset for transformation
	double m_pRect[4]; // Coordinates
	std::vector<double> m_arrC; // Special color

private:
	unsigned int m_unAnnotFlag; // Annotation flag - F
	unsigned int m_unRefNum; // Object reference number
	unsigned int m_unPage; // Page
	std::pair<BYTE, double> m_pBE; // Border effects
	std::string m_sContents; // Display text
	std::string m_sNM; // Unique name
	std::string m_sM; // Date of last modification
	std::string m_sOUserID; // OO User ID
	std::string m_sOMetadata; // OO form metadata
	CBorderType* m_pBorder; // Border
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
	bool ChangeFullName(const std::string& sPrefixForm);
	void ClearActions();
	virtual std::string GetType() = 0;
	virtual void ToWASM(NSWasm::CData& oRes) override;

	bool m_bChangeFullName;

protected:
	CAnnotWidget(PDFDoc* pdfDoc, AcroFormField* pField, int nStartRefID);

	std::string FieldLookupString(AcroFormField* pField, const char* sName, int nByte);

	BYTE m_nType; // Type - FT + flags
	unsigned int m_unFieldFlag; // Flag - Ff

private:
	unsigned int m_unR; // Annotation rotation relative to page - R
	unsigned int m_unRefNumParent; // Parent object reference number
	unsigned int m_unFontStyle; // Font style - from DA
	unsigned int m_unMEOptions; // MEOptions
	double m_dFontSize; // Font size - from DA
	std::vector<double> m_arrTC; // Text color - from DA
	std::vector<double> m_arrBC; // Border color - BC
	std::vector<double> m_arrBG; // Background color - BG
	std::vector<CAction*> m_arrAction; // Actions - A&AA
	BYTE m_nQ; // Text alignment - Q
	BYTE m_nH; // Highlight mode - H
	std::string m_sTU; // Alternate field name, used in tooltips and error messages - TU
	std::string m_sDS; // Default style string - DS
	std::string m_sDV; // Default value - DV
	std::string m_sT; // Partial field name - T
	std::string m_sFontKey; // Unique font identifier
	std::string m_sFullName; // Full field name
	std::string m_sFontName; // Font name - from DA
	std::string m_sActualFontName; // Substituted font name
	std::string m_sButtonFontName; // Button font name
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
	std::vector< std::pair<std::string, std::string> > m_arrOpt;
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
	unsigned int m_unRefNumParent; // Parent object reference number
};

//------------------------------------------------------------------------
// PdfReader::CLinkAnnot
//------------------------------------------------------------------------

class CAnnotLink final : public CAnnot
{
public:
	CAnnotLink(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);
	virtual ~CAnnotLink();

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nH; // Highlight mode - H
	double m_pRD[4]{}; // Rect and RD differences
	std::vector<double> m_arrQuadPoints; // Coordinates - QuadPoints
	CAction* m_pAction; // Action - A&Dest
	CAction* m_pPA; // URI action - PA
};

//------------------------------------------------------------------------
// PdfReader::CScreenAnnot
//------------------------------------------------------------------------

class CAnnotScreen final : public CAnnot
{
public:
	CAnnotScreen(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);
	virtual ~CAnnotScreen();

	void ToWASM(NSWasm::CData& oRes) override;

private:
	unsigned int m_unR; // Поворот аннотации относительно страницы - R
	std::string m_sT; // Title
	std::vector<double> m_arrBC; // Цвет границ - BC
	std::vector<double> m_arrBG; // Цвет фона - BG
	std::vector<CAction*> m_arrAction; // Действия - A&AA
};

//------------------------------------------------------------------------
// PdfReader::CAnnotMarkup
//------------------------------------------------------------------------

class CAnnotMarkup : public CAnnot
{
public:
	void SetFont(PDFDoc* pdfDoc, Object* oAnnotRef, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList);

protected:
	CAnnotMarkup(PDFDoc* pdfDoc, Object* oAnnotRef, int nPageIndex, int nStartRefID);
	virtual ~CAnnotMarkup();

	virtual void ToWASM(NSWasm::CData& oRes) override;

	std::vector<CFontData*> m_arrRC; // Formatted text

private:
	BYTE m_nRT; // Reply annotation type
	unsigned int m_unRefNumPopup; // Popup annotation reference number
	unsigned int m_unRefNumIRT; // Reply annotation reference number
	double m_dCA; // Opacity value
	std::string m_sT; // Text label, user who added the annotation
	std::string m_sCreationDate; // Creation date
	std::string m_sSubj; // Brief description
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
	BYTE m_nName; // Icon
	BYTE m_nState; // State
	BYTE m_nStateModel; // State model
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
	std::vector< std::vector<double> > m_arrInkList; // Path
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
	BYTE m_nIT; // Annotation intent
	BYTE m_nCP; // Annotation caption position
	double m_dLL; // Leader line length
	double m_dLLE; // Leader line extension
	double m_dLLO; // Leader line offset
	BYTE m_nLE[2]; // Line ending styles
	double m_pL[4]{}; // Line coordinates
	double m_pCO[2]; // Caption text offset
	std::vector<double> m_arrIC; // Line ending color
	// TODO Measure scale indicators, measurement units
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
	BYTE m_nSubtype; // TextMarkup annotation subtype
	std::vector<double> m_arrQuadPoints; // Coordinates
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
	BYTE m_nSubtype; // Square or Circle annotation subtype
	double m_pRD[4]{}; // Differences between Rect and actual size
	std::vector<double> m_arrIC; // Fill color
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
	BYTE m_nIT; // Annotation intent
	BYTE m_nSubtype; // Polygon or Polyline annotation subtype
	BYTE m_nLE[2]; // Line ending styles
	std::vector<double> m_arrIC; // Fill color
	std::vector<double> m_arrVertices; // Vertex coordinates
	// TODO Measure scale indicators, measurement units
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
	BYTE m_nQ; // Text alignment - Q
	BYTE m_nIT; // Annotation intent
	BYTE m_nLE; // Line ending style
	int m_nRotate;
	std::string m_sDS; // Default style string - DS
	double m_pRD[4]{}; // Differences between Rect and actual size
	std::vector<double> m_arrCFromDA; // Border color
	std::vector<double> m_arrCL; // Callout coordinates
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
	BYTE m_nSy; // Associated symbol - Sy
	double m_pRD[4]{}; // Differences between Rect and actual size
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
	std::string m_sName; // Icon
	std::string m_sFS;   // File system
	std::string m_sDesc; // File description
	std::string m_sF;    // File specification (backward compatibility)
	std::string m_sUF;   // File specification (cross-platform and cross-language compatibility)
	std::string m_sDOS;  // DOS file specification
	std::string m_sMac;  // Mac file specification
	std::string m_sUnix; // Unix file specification
	std::pair<std::string, std::string> m_sID; // File identifier
	CEmbeddedFiles* m_pEF; // EF contains F/UF/DOS/Mac/Unix with references to embedded file streams by corresponding specifications
	// TODO RF contains F/UF/DOS/Mac/Unix with arrays of related files by corresponding specifications
	// TODO Cl collection for creating user interface
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
	std::string m_sName; // Icon
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

	void SetFont(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, Object* oAnnotRef);

	void ToWASM(NSWasm::CData& oRes) override;

private:
	BYTE m_nQ; // Text alignment
	unsigned int m_unFontStyle; // Font style - from DA
	double m_dFontSize; // Font size - from DA
	std::string m_sFontName; // Font name - from DA
	std::string m_sActualFontName; // Substituted font name
	std::string m_sOverlayText; // Overlay text
	std::vector<double> m_arrQuadPoints; // Coordinates
	std::vector<double> m_arrIC; // Fill color
	std::vector<double> m_arrCFromDA; // Text color
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
			bChangeFullName = false;
		}
		~CAnnotParent()
		{
			ClearActions();
		}
		void ClearActions();

		void ToWASM(NSWasm::CData& oRes);

		bool bChangeFullName;
		unsigned int unFlags;
		unsigned int unRefNum; // Object reference number
		unsigned int unMaxLen; // Maximum length limit for text field
		unsigned int unFieldFlag; // Flag Ff
		unsigned int unMEOptions; // MEOptions
		unsigned int unRefNumParent; // Parent object reference number
		std::vector<int> arrI;
		std::vector<std::string> arrV;
		std::vector< std::pair<std::string, std::string> > arrOpt;
		std::vector<CAction*> arrAction; // Actions
		std::string sT;
		std::string sV;
		std::string sDV;
		std::string sTU;
		std::string sFullName;
	};

	void getParents(PDFDoc* pdfDoc, Object* oFieldRef, int nStartRefID);

	std::vector<int> m_arrCO; // Calculation order - CO
	std::vector<CAnnotParent*> m_arrParents; // Parent Fields
	std::vector<CAnnotWidget*> m_arrAnnots;
};

}

#endif // _PDF_READER_ANNOT_H
