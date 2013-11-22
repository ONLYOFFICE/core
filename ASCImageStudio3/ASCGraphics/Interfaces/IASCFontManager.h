#pragma once
#include "../stdafx.h"

#include "IAVSGraphicsBase.h"
#include "IAVSGlyphImage.h"

[object, uuid("472691CD-1FC6-424a-A44E-8E7549ECDCFF"), dual, pointer_default(unique)]
__interface IAVSFontManager : IAVSGraphicsBase
{
	//----- Открытие/закрытие шрифта --------------------------------------------------------------------
	[id(001)]  HRESULT Initialize([in] BSTR bsXmlOptions);
	[id(002)]  HRESULT LoadFontFromFile([in]BSTR bsSrcPath, [in] float fEmSize, [in] double dHorDPI, [in] double dVerDPI, [in] long lFaceIndex = 0);
	[id(003)]  HRESULT LoadFontByName([in]BSTR bsFamilyName, [in] float fEmSize, [in] long lStyle, [in] double dHorDPI, [in] double dVerDPI);
	[id(004)]  HRESULT GetFontType([out, retval] BSTR *pbsType);
	[id(005)]  HRESULT SetDefaultFont([in] BSTR bsFamilyName);
	//----- Функции аналогичные методам классов Font и FontFamily в GDI+ --------------------------------

	[id(101)]  HRESULT GetCellAscent([out, retval] unsigned short *pusAscent);
	[id(102)]  HRESULT GetCellDescent([out, retval] unsigned short *pusDescent);
	[id(103)]  HRESULT GetEmHeight([out, retval] unsigned short *pusEmHeight);
	[id(104)]  HRESULT GetFamilyName([out, retval] BSTR *pbsFamilyName);
	[id(105)]  HRESULT GetLineSpacing([out, retval] unsigned short *pusLineSpacing);
	[id(106)]  HRESULT IsStyleAvailable([in] long lStyle, [out, retval] BOOL *pbAvailable);
	[id(107)]  HRESULT GetHeight([in] float fDPI, [out, retval] float *pfHeight);
	[id(108)]  HRESULT GetFamilyNameEx([in] BSTR bsOptions, [out, retval] BSTR *pbsFamilyName);

	//----- Чтение глобальных параметров шрифта ---------------------------------------------------------
	[id(201)]  HRESULT GetFacesCount([out, retval] long *plFacesCount);
	[id(202)]  HRESULT GetFaceIndex([out, retval] long *plFaceIndex);
	[id(203)]  HRESULT GetGlyphsCount([out, retval] long *plGlyphsCount);
	[id(205)]  HRESULT GetStyleName([out, retval] BSTR *pbsStyleName);
	[id(206)]  HRESULT IsCharAvailable([in] long lUnicode, [out, retval] BOOL *pbAvailable);
	[id(207)]  HRESULT GetUnderlinePosition([out, retval] short *pshUnderPos);
	[id(208)]  HRESULT GetUnderlineThickness([out, retval] short *pshUnderThick);
	[id(209)]  HRESULT GetMaxAdvanceWidth([out, retval] short *pshMaxAdvWidth);
	[id(210)]  HRESULT GetMaxAdvanceHeight([out, retval] short *pshMaxAdvHeight);
	[id(211)]  HRESULT GetBBox([out] long *plMinX, [out] long *plMinY, [out] long *plMaxX, [out] long *plMaxY);
	[id(212)]  HRESULT IsUnicodeRangeAvailable([in] unsigned long ulBit, [in] unsigned int unLongIndex, [out ,retval] VARIANT_BOOL *pvbSuccess);
	[id(213)]  HRESULT GetNameIndex([in] BSTR bsName, [out, retval] unsigned short *pushGID);
	[id(214)]  HRESULT GetCodeByGID([in] unsigned short ushGID, [out, retval] unsigned long *pulCharCode);
	[id(215)]  HRESULT GetPanose([out, satype("BYTE")] SAFEARRAY **ppsaPanose );
	[id(216)]  HRESULT IsFixedWidth([out, retval] BOOL *pbFixedWidth);

	//----- Работа с глифами ----------------------------------------------------------------------------

	[id(301)]  HRESULT LoadString([in] BSTR bsString, [in] float fX, [in] float fY);
	[id(302)]  HRESULT LoadString2([in] BSTR bsString, [in] float fX, [in] float fY);
	[id(303)]  HRESULT GetStringPath([out, retval] BSTR *pbsXmlPath);
	[id(304)]  HRESULT GetStringPath2([out, retval] IAVSGraphicsBase **ppPath);
	[id(305)]  HRESULT GetNextChar([out] IUnknown **ppImage, [out] float *pfX, [out] float *pfY, [out, retval] VARIANT_BOOL *pvbSuccess);
	[id(306)]  HRESULT GetNextChar2([out] IAVSGlyphImage **ppImage, [out] float *pfX, [out] float *pfY, [out, retval] VARIANT_BOOL *pvbSuccess);
	[id(307)]  HRESULT GetCharBBox([out] float *pfX, [out] float *pfY, [out] float *pfWidth, [out] float *pfHeight);
	[id(308)]  HRESULT GetUnderline([out] float *pfStartX, [out] float *pfStartY, [out] float *pfEndX, [out] float *pfEndY, [out] float *pfSize);
	[id(309)]  HRESULT MeasureString([out] float *pfX, [out] float *pfY, [out] float *pfWidth, [out] float *pfHeight);
	[id(310)]  HRESULT MeasureString2([out] float *pfX, [out] float *pfY, [out] float *pfWidth, [out] float *pfHeight);
	[id(311)]  HRESULT SetFontMatrix([in] float fA, [in] float fB, [in] float fC, [in] float fD, [in] float fE, [in] float fF);
	[id(312)]  HRESULT SetTextMatrix([in] float fA, [in] float fB, [in] float fC, [in] float fD, [in] float fE, [in] float fF);
	[id(313)]  HRESULT SetStringGID([in] BOOL bGIDString);
	[id(314)]  HRESULT GetStringGID([out, retval] BOOL *pbGIDString);
	[id(315)]  HRESULT SetCharSpacing([in] double dSpacing);
	[id(316)]  HRESULT GetCharSpacing([out, retval]double *pdSpacing);

	//----- Работа со шрифтами Windows ------------------------------------------------------------------

	[id(501)]  HRESULT GetWinFontsCount([out, retval] long *plCount);
	[id(502)]  HRESULT GetWinFontByIndex([in] long nIndex, [out] BSTR *pbsFontName, [out] BSTR *pbsFontStyle, [out] BSTR *pbsFontPath, [out] long *plIndex, [out] BOOL *pbBold, [out] BOOL *pbItalic);
	[id(503)]  HRESULT GetWinFontByName([in] BSTR bsFontName, [out] BSTR *pbsFontPath, [out] long *plIndex);
	[id(504)]  HRESULT GetWinFontByParams([in] BSTR bsFontParams, [out] BSTR *pbsFontPath, [out] long *plIndex);

};

[object, uuid("2A13E1F7-424C-4996-A39B-A1449C9F232C"), dual, pointer_default(unique)]
__interface IAVSFontManager2 : IDispatch
{
	[id(1001)]  HRESULT UseDefaultFont([in] BOOL bUseDefFont);
	[id(1002)]  HRESULT GetFontPath([out]BSTR *pbsFontPath);
	[id(1003)]  HRESULT GetFontIndex([out]LONG *plFontIndex);
	[id(1004)]  HRESULT GetWinFontByParams2([out]BSTR *pbsFontName, [in]BSTR bsAltFontName, [in] LONG lCharset, [in] BSTR bsFamilyClass, [out, in]LONG* plStyle, [in] LONG lFixed, [in] BSTR bsPanose, [in]LONG lUnicodeRange1, [in]LONG lUnicodeRange2, [in]LONG lUnicodeRange3, [in]LONG lUnicodeRange4, [in]LONG lCodePageRange1, [in]LONG lCodePageRange2, [in] LONG lAvgWidth);
	[id(1005)]  HRESULT SetFontsList([in]BSTR bsXml);
	[id(1006)]  HRESULT UseFontsList([in]BOOL bUseList, [in]BSTR bsXmlOptions);
	[id(1007)]  HRESULT GetPanose2([out, retval] BSTR* bsPanose);
};