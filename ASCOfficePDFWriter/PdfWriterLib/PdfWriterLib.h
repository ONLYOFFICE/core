// PDFWriter.h : Declaration of the CPDFWriter

#pragma once

#include "../../DesktopEditor/graphics/IRenderer.h"

#include "Document.h"
#include "Image.h"
#include "Font.h"
#include "Streams.h"
#include "Utils.h"
#include "Errors.h"
#include "MemoryManager.h"
#include "List.h"
#include "Streams.h"
#include "Objects.h"
#include "Annotation.h"
#include "Destination.h"
#include "GraphicState.h"
#include "MetaData.h"
#include "Outline.h"
#include "PageOperations.h"
#include "Pages.h"
#include "Catalog.h"
#include "PDF.h"
#include "Encoder.h"
#include "EncryptDictionary.h"
#include "LZWEncoder.h"
#include "ImageExt.h"

#include "Common.h"
#include <vector>

class COfficeFontPicker;
class CFontManager;


class CPdfWriterLib : public IRenderer
{
public:

	CPdfWriterLib();
	virtual ~CPdfWriterLib();

	void SetFontDir(CString& sFontDir);
	void SetThemesPlace(const CString& sThemesPlace);
    void InitializeFonts(bool bIsUseSystem, std::vector<std::wstring> arAdditionalFolders);
//-----------------------------------------------------------------------------------------------------
//
// OfficePDFWriter
//
//-----------------------------------------------------------------------------------------------------
	HRESULT CreatePDF ();
	HRESULT DeletePDF ();
	HRESULT SaveToFile (CString bsPath);
	HRESULT SetPDFPageMode (long nMode);
	HRESULT GetPDFPageMode (long *pnMode);
	HRESULT SetPDFPageLayout (long nLayout);
	HRESULT GetPDFPageLayout (long *pnLayout);
	HRESULT SetPDFViewerPreference (long nValue );
	HRESULT GetPDFViewerPreference (long *pnValue);
	HRESULT SetPDFCompressionMode (long nMode);
	HRESULT SetPDFOpenAction (long nAction);
	HRESULT GetLastErrorCode (long *pnErrorCode);
	HRESULT GetLastErrorString (CString *pbsErrorString);
//-----------------------------------------------------------------------------------------------------
	HRESULT AddNewPage ();
	HRESULT GetPagesCount (long *pnCount );
	HRESULT SetCurrentPage (long nIndex);
	HRESULT GetCurrentPageIndex (long *pnIndex);
	HRESULT SetPageWidth (float fWidth);
	HRESULT SetPageHeight (float fHeight);
	HRESULT GetWidth (float *pfWidth);
	HRESULT GetHeight (float *pfHeight);
	HRESULT SetSize (long nSize, long nDirection);
	HRESULT SetRotate (long nAngle);
	HRESULT AddLabel (long nPageNum, long nNumStyle, long nFirstPage, CString bsPrefix );
	
//-----------------------------------------------------------------------------------------------------
	HRESULT SetLineWidth (float fLineWidth);
	HRESULT GetLineWidth (float *pfLineWidth);
	HRESULT SetLineCap (long nLineCap);
	HRESULT GetLineCap (long *pnLineCap);
	HRESULT SetLineJoin (long nLineJoin);
	HRESULT GetLineJoin (long *pnLineJoin);
	HRESULT SetMiterLimit (float fMiterLimit);
	HRESULT GetMiterLimit (float *pfMiterLimit);
	//HRESULT SetDash (SAFEARRAY **ppnDashPtn, long   nNumParam, float   fPhase)
	//{
	//	double *pArray = new double[nNumParam];
	//	if ( 0 != nNumParam )
	//	{
	//		// check for valid pixel array size
	//		if ( 1 != SafeArrayGetDim(*ppnDashPtn) )
	//			return FALSE;

	//		VARTYPE vType;
	//		// check for valid array element type
    //		if ( S_OK != (SafeArrayGetVartype(*ppnDashPtn, &vType)) || VT_R4 != vType )
	//			return S_OK;

	//		float* pTemp = (float* (*ppnDashPtn)->pvData;
	//		for ( int nIndex = 0 ; nIndex < nNumParam; nIndex++ )
	//		{
	//			pArray[nIndex] = *(pTemp + nIndex);
	//		}
	//	}

	//	unsigned long nRet = PageSetDash( m_pDocument->pCurPage, pArray, nNumParam, fPhase);
	//	delete[] pArray;
	//	if ( OK != nRet )
	//		return nRet;
	//	return S_OK;
	//}

	//HRESULT GetDash (SAFEARRAY **ppnDashPtn, long *pnNumParam, float *pfPhase)
	//{
	//	DashMode oMode = {{0, 0, 0, 0, 0, 0, 0, 0}, 0, 0};		
	//	oMode = PageGetDash( m_pDocument->pCurPage );

	//	// проверяем не появилась ли ошибка
	//	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	//	if ( OK != nRet )
	//		return nRet;

	//	if ( 0 == oMode.nNumPoints )
	//	{
	//		*ppnDashPtn = NULL;
	//		*pnNumParam = 0;
	//		*pfPhase = 0;
	//		return S_OK;
	//	}
	//	float* pTemp = new float[oMode.nNumPoints];
	//	for ( int nIndex = 0; nIndex < oMode.nNumPoints; nIndex++ )
	//		pTemp[nIndex] = (float)oMode.afPtn[nIndex];

	//	SAFEARRAYBOUND bounds[1];

	//	bounds[0].lLbound = 0;
	//	bounds[0].cElements = oMode.nNumPoints;

	//	(*ppnDashPtn) = SafeArrayCreate(VT_R4, 1, bounds);

	//	if ( !(*ppnDashPtn) )
	//	{
	//		delete[] pTemp;
	//		return S_OK;
	//	}

	//	memcpy( (*ppnDashPtn)->pvData, pTemp, oMode.nNumPoints * sizeof(float) );
	//	delete[] pTemp;
	//	*pnNumParam = (long)oMode.nNumPoints;
	//	*pfPhase    = (float)oMode.fPhase;
	//	return S_OK;
	//}
	HRESULT SetFlat (float   fFlatness);
	HRESULT GetFlat (float *pfFlatness);
	HRESULT NewExtGraphicsState ();
	HRESULT ExtGraphicsStateSetAlphaStroke (float fValue);
	HRESULT ExtGraphicsStateSetAlphaFill (float fValue);
	HRESULT ExtGraphicsStateSetBlendMode (long nBlendMode);
	HRESULT ExtGraphicsStateSetStrokeAdjustment (int nFlag);
	HRESULT SetExtGraphicsState ();
	HRESULT GSave ();
	HRESULT GRestore ();
	HRESULT GetGStateDepth (long *pnDepth);
	HRESULT GetGMode (long *pnGMode);
	HRESULT Concat (float fA, float fB, float fC, float fD, float fX, float fY);
	HRESULT MoveTo (float fX, float fY);
	HRESULT GetCurrentPosition (float *pfX, float *pfY);
	HRESULT LineTo (float fX, float fY);
	HRESULT CurveTo (float fX1, float fY1, float fX2, float fY2, float fX3, float fY3);
	HRESULT CurveTo2 (float fX2, float fY2, float fX3, float fY3);
	HRESULT CurveTo3 (float fX1, float fY1, float fX3, float fY3);
	HRESULT ClosePath ();
	HRESULT AppendRectangle (float fX, float fY, float fWidth, float fHeight);
	HRESULT Stroke ();
	HRESULT GetStrokingColorSpace (long *pnColorSpace);
	HRESULT ClosePathStroke ();
	HRESULT Fill ();
	HRESULT GetFillingColorSpace (long *pnColorSpace);
	HRESULT EoFill ();
	HRESULT FillStroke ();
	HRESULT EoFillStroke ();
	HRESULT ClosePathFillStroke ();
	HRESULT ClosePathEoFillStroke ();
	HRESULT EndPath ();
	HRESULT Clip ();
	HRESULT EoClip ();
	HRESULT BeginText ();
	HRESULT EndText ();
	HRESULT SetCharSpace (float fValue);
	HRESULT GetCharSpace (float *pfValue);
	HRESULT SetWordSpace (float fValue);
	HRESULT GetWordSpace (float *pfValue);
	HRESULT SetHorizontalScalling (float fValue);
	HRESULT GetHorizontalScalling (float *pfValue);
	HRESULT SetTextLeading (float fValue);
	HRESULT GetTextLeading (float *pfValue);
	HRESULT SetFontAndSizeToGState (float fSize);
	HRESULT GetCurrentFontFromGState (CString *pbsFontName, CString *pbsEncodingName);
	HRESULT GetCurrentFontSizeFromGState (float *pfSize);
	HRESULT SetTextRenderingMode (long nMode);
	HRESULT GetTextRenderingMode (long *pnMode);
	HRESULT SetTextRise (float fValue);
	HRESULT GetTextRise (float *pfValue);
	HRESULT MoveTextPos (float fX, float fY);
	HRESULT MoveTextPos2 (float fX, float fY);
	HRESULT GetCurrentTextPosition (float *pfX, float *pfY);
	HRESULT SetTextMatrix (float   fA, float   fB, float   fC, float   fD, float   fX, float   fY);
	HRESULT GetTextMatrix (float *pfA, float *pfB, float *pfC, float *pfD, float *pfX, float *pfY);
	HRESULT MoveToNextLine ();
	HRESULT ShowText (CString bsText);
	HRESULT ShowTextNextLine (CString bsText);
	HRESULT ShowTextNextLineEx (float fWordSpace, float fCharSpace, CString bsText);
	HRESULT SetGrayStroke (float fGray);
	HRESULT GetGrayStroke (float *pfGray);
	HRESULT SetGrayFill (float fGray);
	HRESULT GetGrayFill (float *pfGray);
	HRESULT SetRGBStroke (float   fR, float   fG, float   fB);
	HRESULT GetRGBStroke (float *pfR, float *pfG, float *pfB);
	HRESULT SetRGBFill (float fR, float fG, float fB);
	HRESULT GetRGBFill (float *pfR, float *pfG, float *pfB);
	HRESULT SetCMYKStroke (float   fC, float   fM, float   fY, float   fK);
	HRESULT GetCMYKStroke (float *pfC, float *pfM, float *pfY, float *pfK);
	HRESULT SetCMYKFill (float   fC, float   fM, float   fY, float   fK);
	HRESULT GetCMYKFill (float *pfC, float *pfM, float *pfY, float *pfK);
	HRESULT ExecuteXObject ();
	HRESULT SetExtGraphicsState2 (CString bsXmlOptions);
	//-----------------------------------------------------------------------------------------------------
	HRESULT AppendCircle (float fX, float fY, float fRad);
	HRESULT AppendEllipse (float fX, float fY, float fXRad, float fYRad);
	HRESULT AppendArc (float fX, float fY, float fRad, float fAngle1, float fAngle2);
	HRESULT AppendEllipseArc (float fX, float fY, float fXRad, float fYRad, float fAngle1, float fAngle2, BOOL bClockDirection = FALSE);
    HRESULT TextOut_ (float fXPos, float fYPos, std::wstring & bsText);
	HRESULT TextRect (float fLeft, float fTop, float fRight, float fBottom, CString bsText, long nAlign, unsigned int *pnLen);
	HRESULT SetSlideShow (long nType, float fDispTime, float fTransTime);
	HRESULT GetTextWidth (std::wstring bsText, float *pfResult);
	HRESULT MeasureText (CString bsText, float fWidth, BOOL bWordWrap, float *pfRealWidth, long *pnLength);
	//-----------------------------------------------------------------------------------------------------
	HRESULT LoadT1FFromFile ( CString bsAFMFileName, CString bsDataFileName, CString *bsFontName);
	HRESULT LoadTTFFromFile ( CString bsFileName, BOOL bEmbedding, CString bsEncodingName, CString *bsFontName);
	HRESULT LoadTTCFromFile ( CString bsFileName, long nIndex, BOOL bEmbedding, CString *bsFontName);
	HRESULT SetCurrentFont ( CString bsFontName, CString bsEncodingName, BOOL bNameIsUnicodeArray, CString bsToUnicodeName);
	HRESULT GetCurrentFont ( CString *pbsFontName, CString *pbsEncodingName );
	HRESULT GetFontUnicodeWidth (long nCode, long *pnUnicode);
	HRESULT GetFontBBox         (float *pfLeft, float *pfBottom, float *pfRight, float *pfTop);
	HRESULT GetFontAscent       (long *pnAscent);
	HRESULT GetFontDescent      (long *pnDescent);
	HRESULT GetFontXHeight      (long *pnXHeight);
	HRESULT GetFontCapHeight    (long *pnCapHeight);
	//-----------------------------------------------------------------------------------------------------
	HRESULT SetCurrentEncoder ( CString bsEncodingName );
	HRESULT GetCurrentEncoder ( CString *pbsEncodingName );
	HRESULT GetEncoderUnicode (long nCode, long *pnUnicode);
	HRESULT GetEncoderByteType (CString bsText, long nIndex, long *pnByteType);
	HRESULT GetEncoderType (long *pnEncoderType);
	HRESULT GetEncoderWritingMode (long *pnWritingMode);
//-----------------------------------------------------------------------------------------------------
	HRESULT CreateNewDestination ();
	HRESULT DestSetXYZ (float fLeft, float fTop, float fZoom);
	HRESULT DestSetFit ();
	HRESULT DestSetFitH (float fTop);
	HRESULT DestSetFitV (float fLeft);
	HRESULT DestSetFitR (float fLeft, float fBottom, float fRight, float fTop);
	HRESULT DestSetFitB ();
	HRESULT DestSetFitBH (float fTop);
	HRESULT DestSetFitBV (float fLeft);
	HRESULT SetCurrentDest (long nIndex);
	HRESULT GetCurrentDestIndex (long *pnIndex);
	//------------------------------------------------------------------------------------------------
	HRESULT CreateTextAnnot (float fLeft, float fBottom, float fRight, float fTop, CString bsText, CString bsEncoder);
	HRESULT CreateLinkAnnot (float fLeft, float fBottom, float fRight, float fTop);
	HRESULT CreateUriLinkAnnot (float fLeft, float fBottom, float fRight, float fTop, CString bsUrl);
	HRESULT ArbAnnotationSetBorderStyle (long nSubtype, float fWidth, long nDashOn, long nDashOff, long nDashPhase);
	HRESULT LinkAnnotationSetBorderStyle (float fWidth, long nDashOn, long nDashOff );
	HRESULT LinkAnnotationSetHighlightMode (long nMode);
	HRESULT TextAnnotationSetIcon (long nIcon);
	HRESULT TextAnnotationSetOpened (BOOL bOpened);
	HRESULT SetCurrentAnnot (long nIndex);
	HRESULT GetCurrentAnnotIndex (long *pnIndex);
	HRESULT MakeAnnotationFromXml (CString bsXML);
	//------------------------------------------------------------------------------------------------
	HRESULT CreateNewOutline (long nParentIndex, CString bsTitle, CString bsEncoder);
	HRESULT OutlineSetCurrentDestination ();
	HRESULT SetOutlineOpened (BOOL bOpened);
	HRESULT SetCurrentOutline (long nIndex);
	HRESULT GetCurrentOutlineIndex (long *pnIndex);
	HRESULT MakeOutlineFromXml (CString bsXML);
	//------------------------------------------------------------------------------------------------
	HRESULT LoadJpegFromFile (CString bsFileName);
    HRESULT LoadJpxFromFile (CString bsFileName, int nFlag);

//	HRESULT LoadImageFromFile (CString bsFileName,     long nWidth, long nHeight, long nColorSpace)
//	{
//		unsigned long nRet = OK;
//
//		CString sFileName( bsFileName );
//
//		if ( !m_pXObjectList )
//		{
//			m_pXObjectList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
//			if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
//				return nRet;
//		}
//
//		m_pCurrentXObject = LoadRawImageFromFile( m_pDocument, sFileName.GetBuffer(), nWidth, nHeight, (ColorSpace)nColorSpace );
//		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
//			return nRet;
//
//		if ( OK != ( nRet = ListAdd( m_pXObjectList, m_pCurrentXObject) ) ) 
//			return nRet;
//
//		return S_OK;
//	}
//	HRESULT LoadImageFromArray (SAFEARRAY **ppImage, long nWidth, long nHeight, long nColorSpace, long nBitsPerComponent)
//	{
//		unsigned long nRet = OK;
//
//		BYTE *pArray;
//		if ( nHeight && nWidth )
//		{
//			// check for valid pixel array size
//			if ( 1 != SafeArrayGetDim(*ppImage) )
//				return AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE;
//
//			VARTYPE vType;
//			// check for valid array element type
//			if ( S_OK != (SafeArrayGetVartype(*ppImage, &vType)) || VT_UI1 != vType )
//				return AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE;
//
//			pArray = (BYTE* (*ppImage)->pvData;
//		}
//		if ( !m_pXObjectList )
//		{
//			m_pXObjectList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
//			if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
//				return nRet;
//		}
//
//		m_pCurrentXObject = LoadRawImageFromMem( m_pDocument, pArray, nWidth, nHeight, (ColorSpace)nColorSpace, nBitsPerComponent );
//		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
//			return nRet;
//
//		if ( OK != ( nRet = ListAdd( m_pXObjectList, m_pCurrentXObject) ) ) 
//			return nRet;
//
//		return S_OK;
//	}
	HRESULT LoadImageFromInterface (IGrObject *pInterface , BOOL bAlpha = FALSE);
	HRESULT DrawCurrentImage (float fX, float fY, float fWidth, float fHeight);

	HRESULT GetImageBitsPerComponent (long *pnBitsPerComponent);
	HRESULT GetImageColorSpace (long *pnColorSpace);
	HRESULT GetImageHeight (long *pnHeight);
	HRESULT GetImageWidth (long *pnWidth);
	HRESULT SetImageColorMask (long nRMin, long nRMax, long nGMin, long nGMax, long nBMin, long nBMax);
	HRESULT SetImageMaskImage (long nMaskImageIndex);
	HRESULT SetCurrentImage (long nIndex);
	HRESULT GetCurrentImageIndex (long *pnIndex);
	//------------------------------------------------------------------------------------------------
	HRESULT SetEncryptOn ();
	HRESULT SetPDFPassword (CString bsOwnerPassword, CString bsUserPassword);
	HRESULT SetPDFPermission (long nPermission);
	HRESULT SetPDFEncryptionMode (long nMode, long nKeyLen);
	HRESULT SetEncryptOff ();
	//------------------------------------------------------------------------------------------------
	HRESULT GetInfoItem (long nDataType, CString *pbsItem);
	HRESULT SetInfoItem (long nDataType, CString bsData);
	HRESULT SetInfoDateItem (long nType, long nYear, long nMonth, long nDay, long nHour, long nMinutes, long nSeconds, long nInd, long nOffHour, long nOffMinutes);
	//------------------------------------------------------------------------------------------------
	HRESULT SetAdditionalParam (CString ParamName, VARIANT	ParamValue);
	HRESULT GetAdditionalParam (CString ParamName, VARIANT *	ParamValue);
	//-----------------------------------------------------------------------------------------------------
	//
	// Интерфейс IAVSOfficeRendererTemplate (IPDFRenderer) IAVSRenderer
	//
	//-----------------------------------------------------------------------------------------------------

	HRESULT OnlineWordToPdf (CString sPathXml, CString sDstFile, LONG nReg);
    void OnlineWordToPdfInternal (BYTE* dstArray, LONG len, const std::wstring& sHtmlPlace, std::wstring& sHypers, int& nCountPages, CString sTempLogo, LONG nReg);

//IRenderer

// тип рендерера-----------------------------------------------------------------------------
	virtual HRESULT get_Type(LONG* lType)				;
//-------- Функции для работы со страницей --------------------------------------------------
	virtual HRESULT NewPage()							;
	virtual HRESULT get_Height(double* dHeight)			;
	virtual HRESULT put_Height(const double& dHeight)	;
	virtual HRESULT get_Width(double* dWidth)			;
	virtual HRESULT put_Width(const double& dWidth)		;
	virtual HRESULT get_DpiX(double* dDpiX)				;
	virtual HRESULT get_DpiY(double* dDpiY)				;

// pen --------------------------------------------------------------------------------------
	virtual HRESULT get_PenColor(LONG* lColor)					;
	virtual HRESULT put_PenColor(const LONG& lColor)			;
	virtual HRESULT get_PenAlpha(LONG* lAlpha)					;
	virtual HRESULT put_PenAlpha(const LONG& lAlpha)			;
	virtual HRESULT get_PenSize(double* dSize)					;
	virtual HRESULT put_PenSize(const double& dSize)			;
	virtual HRESULT get_PenDashStyle(BYTE* val)					;
	virtual HRESULT put_PenDashStyle(const BYTE& val)			;
	virtual HRESULT get_PenLineStartCap(BYTE* val)				;
	virtual HRESULT put_PenLineStartCap(const BYTE& val)		;
	virtual HRESULT get_PenLineEndCap(BYTE* val)				;
	virtual HRESULT put_PenLineEndCap(const BYTE& val)			;
	virtual HRESULT get_PenLineJoin(BYTE* val)					;
	virtual HRESULT put_PenLineJoin(const BYTE& val)			;
	virtual HRESULT get_PenDashOffset(double* dOffset)			;
	virtual HRESULT put_PenDashOffset(const double& dOffset)	;
	virtual HRESULT get_PenAlign(LONG* lAlign)					;
	virtual HRESULT put_PenAlign(const LONG& lAlign)			;
	virtual HRESULT get_PenMiterLimit(double* dOffset)			;
	virtual HRESULT put_PenMiterLimit(const double& dOffset)	;
	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount);

// brush ------------------------------------------------------------------------------------
	virtual HRESULT get_BrushType(LONG* lType)							;
	virtual HRESULT put_BrushType(const LONG& lType)					;
	virtual HRESULT get_BrushColor1(LONG* lColor)						;
	virtual HRESULT put_BrushColor1(const LONG& lColor)					;
	virtual HRESULT get_BrushAlpha1(LONG* lAlpha)						;
	virtual HRESULT put_BrushAlpha1(const LONG& lAlpha)					;
	virtual HRESULT get_BrushColor2(LONG* lColor)						;
	virtual HRESULT put_BrushColor2(const LONG& lColor)					;
	virtual HRESULT get_BrushAlpha2(LONG* lAlpha)						;
	virtual HRESULT put_BrushAlpha2(const LONG& lAlpha)					;
	virtual HRESULT get_BrushTexturePath(std::wstring* bsPath)			; 
	virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath)	; 
	virtual HRESULT get_BrushTextureMode(LONG* lMode)					;
	virtual HRESULT put_BrushTextureMode(const LONG& lMode)				;
	virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)				;
	virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha)			;
	virtual HRESULT get_BrushLinearAngle(double* dAngle)				;
	virtual HRESULT put_BrushLinearAngle(const double& dAngle)			;
	virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height) ;
	virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height) ;

	virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount) ;

// font -------------------------------------------------------------------------------------
	virtual HRESULT get_FontName(std::wstring* bsName)			;
	virtual HRESULT put_FontName(const std::wstring& bsName)	;
	virtual HRESULT get_FontPath(std::wstring* bsName)			;
	virtual HRESULT put_FontPath(const std::wstring& bsName)	;
	virtual HRESULT get_FontSize(double* dSize)					;
	virtual HRESULT put_FontSize(const double& dSize)			;
	virtual HRESULT get_FontStyle(LONG* lStyle)					;
	virtual HRESULT put_FontStyle(const LONG& lStyle)			;
	virtual HRESULT get_FontStringGID(INT* bGID)				;
	virtual HRESULT put_FontStringGID(const INT& bGID)			;
	virtual HRESULT get_FontCharSpace(double* dSpace)			;
	virtual HRESULT put_FontCharSpace(const double& dSpace)		;
	virtual HRESULT get_FontFaceIndex(int* lFaceIndex)			;
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex)	;

//-------- Функции для вывода текста --------------------------------------------------------
	virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) ;
	virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) ;
	
	virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) ;
	virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) ;

//-------- Маркеры для команд ---------------------------------------------------------------
	virtual HRESULT BeginCommand(const DWORD& lType)	;
	virtual HRESULT EndCommand(const DWORD& lType)		;

//-------- Функции для работы с Graphics Path -----------------------------------------------
	virtual HRESULT PathCommandMoveTo(const double& x, const double& y)			;
	virtual HRESULT PathCommandLineTo(const double& x, const double& y)			;
	virtual HRESULT PathCommandLinesTo(double* points, const int& count)		;
	virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)	 ;
	virtual HRESULT PathCommandCurvesTo(double* points, const int& count)		;
	virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle) ;
	virtual HRESULT PathCommandClose()											;
	virtual HRESULT PathCommandEnd()											;
	virtual HRESULT DrawPath(const LONG& nType)									;
	virtual HRESULT PathCommandStart()											;
	virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y)			;

	virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) ;
	virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) ;
	
	virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) ;
	virtual HRESULT PathCommandTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) ;

//-------- Функции для вывода изображений ---------------------------------------------------
	virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)		;
	virtual HRESULT DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255)	;	

// transform --------------------------------------------------------------------------------
	virtual HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags);
	virtual HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags);
	virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6);
	virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)	;
	virtual HRESULT ResetTransform();

// -----------------------------------------------------------------------------------------
	virtual HRESULT get_ClipMode(LONG* plMode)			;
	virtual HRESULT put_ClipMode(const LONG& lMode)		;

// additiaonal params ----------------------------------------------------------------------
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand)			;
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand)		;
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand)	;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//нету  в IRenderer

	//-------- Функции для работы с Shadow -----------------------------------------------------------
	HRESULT get_ShadowDistanceX (double *pdX);
	HRESULT put_ShadowDistanceX (const double &dX);
	HRESULT get_ShadowDistanceY (double *pdY);
	HRESULT put_ShadowDistanceY (const double &dY);
	HRESULT get_ShadowBlurSize (double *pdSize);
	HRESULT put_ShadowBlurSize (const double &dSize);
	HRESULT get_ShadowColor (LONG *plColor);
	HRESULT put_ShadowColor (const LONG &lColor);
	HRESULT get_ShadowAlpha (LONG *plAlpha);
	HRESULT put_ShadowAlpha (const LONG &lAlpha);
	HRESULT get_ShadowVisible (BOOL *pbVisible);
	HRESULT put_ShadowVisible (const BOOL &bVisible);
	//-------- Функции для работы с Edge Text --------------------------------------------------------
	HRESULT get_EdgeVisible (LONG *plVisible);
	HRESULT put_EdgeVisible (const LONG &lVisible);
	HRESULT get_EdgeColor (LONG *plColor);
	HRESULT put_EdgeColor (const LONG &lColor);
	HRESULT get_EdgeAlpha (LONG *plAlpha);
	HRESULT put_EdgeAlpha (const LONG &lAlpha);
	HRESULT get_EdgeDist (double *pdDist);
	HRESULT put_EdgeDist (const double &dDist);

private:
	Doc								m_pDocument;
	FontDict						m_pCurrentFont;
	ExtGState						m_pCurrentExtGState;

	XObject							m_pCurrentXObject;
	List							m_pXObjectList;

	Annotation						m_pCurrentAnnotation;
	List							m_pAnnotList;

	Destination						m_pCurrentDestination;
	List							m_pDestList;

	Outline							m_pCurrentOutline;
	List							m_pOutlineList;

	List							m_pFontEncodingList;

	PDF::PatternStorage				m_oPatterns;			//	add patterns in commands
	PDF::PaintState					m_oPatternState;		//	current pattern

	PDF::XForm*						m_XForm;
	CString							m_MaskWithFormCommands;
	int								m_BlockCommandsStart;
	BOOL							m_IsStrokeShape;

	// для вырезки, хардкор

	CString							m_strPatternFill;
	CString							m_strPatternStroke;

	// для IPDFRenderer

	NSStructures::CPen				m_oPen;
	NSStructures::CBrush			m_oBrush;
	NSStructures::CFont				m_oFont;
	NSStructures::CShadow			m_oShadow;			
	NSStructures::CEdgeText			m_oEdgeText;		

	Aggplus::CGraphicsPath			m_oCurPath;

	double							m_dCharSpace;

	BOOL							m_bStartSubPath;
	BOOL							m_bGStateChange;

	BOOL							m_bClipPath;
	TMatrix							m_oClipCTM;
	TMatrix							m_oBaseTransform;
	TCommandParams					m_oCommandParams;
	TCommandType*					m_pCurCommand;

	CFindFonts*						m_pFindFonts;
	Aggplus::CGraphicsPath*			m_pCurPath;
	BOOL                            m_bUseTextureRect; // Используем ли рект для текстурной заливки (заданный в m_oBrush)
	BOOL                            m_bUseImageTransform; // Заглушка для функции DrawImage (использовать ли там трансформ)

	CFontLoader						m_oFontLoader;

	CString							m_wsTempDir;
	CString							m_sThemesPlace;

	CFontManager*					m_pFontManager;
	CApplicationFonts				m_oFontsApplication;

	BOOL							m_bIsWhiteBackImage;
	BOOL							m_bUseImageTextureAlpha;

	CMatrix							m_oTransform;   // Текущая матрица преобразования
	CContiniousText					m_oContiniousText;
	CRendererState					m_oRendererState;
	BOOL                            m_bFirstMoveTo;

	//CGdiPlusInit					m_oGdiplusInit;

	BOOL							m_bIsFontsInitialize;

	void FreePDF();

	TFontEncodingPair *FindFontEncodingPair( char* sFontName, char* sEncodingName = NULL );
	
	unsigned long FontFromFileToPdfFont();

	unsigned long GdiFontToPdfFont	(CString bsEncodingName		= L"CP1251", BOOL bNameIsUnicodeArray = FALSE);
	unsigned long GdiFontToPdfFont2	(CString bsToUnicodeName	= _T(""), CString bsEncodingName = _T(""));
	unsigned long GdiDashToPDFDash	( int nDashStyle = 0, double *pDashPattern = NULL, int nDashPatternSize = 0, float fDashOffset = 0 );
	unsigned long ReadOutlineChilds	(CString sXml, int nRootIndex);
	unsigned long UpdateGState();
	unsigned long UpdateCoordSpace();
	unsigned long UpdateCoordSpace2(CMatrix *pTransform);
	bool          FileExist(CString sFilePath );
	// Устанавливаем текущее состояние рендерера
	BOOL SetState ( ERendererState eState );


	// NODE: пока в виде заглушки
	// NOTE: пока градиенты выставим в виде svg-xml

	BOOL ApplyFillGradient();
	BOOL ApplyStrokeGradient();
    BOOL CreateLinearGradientFromSvgXml(const std::wstring& sXml, BOOL fill);
    BOOL CreateRadialGradientFromSvgXml(const std::wstring& sXml, BOOL fill);

	BOOL ApplyTileFill();

	BOOL RebuildResources();
	
	//  
	
	template<typename T> Proxy GenXRef(T* object, int Type);

	PDF::Rect PathBounds();
	PDF::Rect PageBounds();
};
