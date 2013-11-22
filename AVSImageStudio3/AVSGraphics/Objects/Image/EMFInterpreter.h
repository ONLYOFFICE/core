#pragma once
#include "..\..\DocumentViewer\DIB.h"
#include "..\..\Interfaces\AVSRenderer.h"
#include "..\..\Interfaces\IAVSFontManager.h"
#include "..\Structures.h"

#define IS_VALID(Renderer)			\
			if (NULL == Renderer)	\
				return FALSE;

#define VTXTALIGN_MASK		(TA_TOP|TA_BASELINE|TA_BOTTOM)
#define HTXTALIGN_MASK		(TA_LEFT|TA_CENTER|TA_RIGHT)

class CEMFInterpreter
{
private:
	IAVSFontManager* m_pFontManager;
	IAVSRenderer* m_pRenderer;

	void*		  m_pRecord;

	///
	HDC				m_hDC;
	UINT			m_dwObjects;
	HANDLETABLE*	m_pHandleTable;

	NSDocumentViewer::CDoubleRect m_oBoundsSrc;
	NSDocumentViewer::CDoubleRect m_oBoundsDst;

	BOOL			m_bIsPath;

	double			m_dCurrentX;
	double			m_dCurrentY;

	COLORREF		m_lTextColor;
	COLORREF		m_lBkColor;

	double			m_dSizeTextKoef;
	
public:
	CEMFInterpreter()
	{
		m_pFontManager = NULL;
		m_pRenderer = NULL;
		m_pRecord	= NULL;

		m_bIsPath	= FALSE;

		m_dCurrentX = 0;
		m_dCurrentY = 0;

		m_dSizeTextKoef = 1.0;
	}
	~CEMFInterpreter()
	{
		RELEASEINTERFACE(m_pFontManager);
		RELEASEINTERFACE(m_pRenderer);
		m_pRecord = NULL;
	}

public:
	inline void SetRenderer(IAVSRenderer* pRenderer)
	{
		RELEASEINTERFACE(m_pRenderer);
		m_pRenderer = pRenderer;
		ADDREFINTERFACE(m_pRenderer);
	}
	inline void SetFontManager(IAVSFontManager* pManager)
	{
		RELEASEINTERFACE(m_pFontManager);
		m_pFontManager = pManager;
		ADDREFINTERFACE(m_pFontManager);
	}
	inline void SetRecord(void* pRecord)
	{
		m_pRecord = pRecord;
	}
	inline void SetTable(HANDLETABLE* pTable)
	{
		m_pHandleTable = pTable;
	}
	inline void Init(HDC hDC, UINT lObjects)
	{
		m_hDC = hDC;
		m_dwObjects = lObjects;
	}
	inline void SetRectDst(const double& x, const double&  y, const double& width, const double& height)
	{
		m_oBoundsDst.left		= x;
		m_oBoundsDst.top		= y;
		m_oBoundsDst.right		= x + width;
		m_oBoundsDst.bottom		= y + height;
	}

	void ProduceRecord()
	{
		DWORD lType = ((ENHMETARECORD*)m_pRecord)->iType;

		switch (lType)
		{
		case EMR_HEADER:					{ OnEmfHEADER();				break; }
		case EMR_POLYBEZIER:				{ OnEmfPOLYBEZIER();			break; }
		case EMR_POLYGON:					{ OnEmfPOLYGON();				break; }
		case EMR_POLYLINE:					{ OnEmfPOLYLINE();				break; }
		case EMR_POLYBEZIERTO:				{ OnEmfPOLYBEZIERTO();			break; }
		case EMR_POLYLINETO:				{ OnEmfPOLYLINETO();			break; }
		case EMR_POLYPOLYLINE:				{ OnEmfPOLYPOLYLINE();			break; }
		case EMR_POLYPOLYGON:				{ OnEmfPOLYPOLYGON();			break; }
		case EMR_SETWINDOWEXTEX:			{ OnEmfSETWINDOWEXTEX();		break; }
		case EMR_SETWINDOWORGEX:			{ OnEmfSETWINDOWORGEX();		break; }
		case EMR_SETVIEWPORTEXTEX:			{ OnEmfSETVIEWPORTEXTEX();		break; }
		case EMR_SETVIEWPORTORGEX:			{ OnEmfSETVIEWPORTORGEX();		break; }
		case EMR_SETBRUSHORGEX:				{ OnEmfSETBRUSHORGEX();			break; }
		case EMR_EOF:						{ OnEmfEOF();					break; }
		case EMR_SETPIXELV:					{ OnEmfSETPIXELV();				break; }
		case EMR_SETMAPPERFLAGS:			{ OnEmfSETMAPPERFLAGS();		break; }
		case EMR_SETMAPMODE:				{ OnEmfSETMAPMODE();			break; }
		case EMR_SETBKMODE:					{ OnEmfSETBKMODE();				break; }
		case EMR_SETPOLYFILLMODE:			{ OnEmfSETPOLYFILLMODE();		break; }
		case EMR_SETROP2:					{ OnEmfSETROP2();				break; }
		case EMR_SETSTRETCHBLTMODE:			{ OnEmfSETSTRETCHBLTMODE();		break; }
		case EMR_SETTEXTALIGN:				{ OnEmfSETTEXTALIGN();			break; }
		case EMR_SETCOLORADJUSTMENT:		{ OnEmfSETCOLORADJUSTMENT();	break; }
		case EMR_SETTEXTCOLOR:				{ OnEmfSETTEXTCOLOR();			break; }
		case EMR_SETBKCOLOR:				{ OnEmfSETBKCOLOR();			break; }
		case EMR_OFFSETCLIPRGN:				{ OnEmfOFFSETCLIPRGN();			break; }
		case EMR_MOVETOEX:					{ OnEmfMOVETOEX();				break; }
		case EMR_SETMETARGN:				{ OnEmfSETMETARGN();			break; }
		case EMR_EXCLUDECLIPRECT:			{ OnEmfEXCLUDECLIPRECT();		break; }
		case EMR_INTERSECTCLIPRECT:			{ OnEmfINTERSECTCLIPRECT();		break; }
		case EMR_SCALEVIEWPORTEXTEX:		{ OnEmfSCALEVIEWPORTEXTEX();	break; }
		case EMR_SCALEWINDOWEXTEX:			{ OnEmfSCALEWINDOWEXTEX();		break; }
		case EMR_SAVEDC:					{ OnEmfSAVEDC();				break; }
		case EMR_RESTOREDC:					{ OnEmfRESTOREDC();				break; }
		case EMR_SETWORLDTRANSFORM:			{ OnEmfSETWORLDTRANSFORM();		break; }
		case EMR_MODIFYWORLDTRANSFORM:		{ OnEmfMODIFYWORLDTRANSFORM();	break; }
		case EMR_SELECTOBJECT:				{ OnEmfSELECTOBJECT();			break; }
		case EMR_CREATEPEN:					{ OnEmfCREATEPEN();				break; }
		case EMR_CREATEBRUSHINDIRECT:		{ OnEmfCREATEBRUSHINDIRECT();	break; }
		case EMR_DELETEOBJECT:				{ OnEmfDELETEOBJECT();			break; }
		case EMR_ANGLEARC:					{ OnEmfANGLEARC();				break; }
		case EMR_ELLIPSE:					{ OnEmfELLIPSE();				break; }
		case EMR_RECTANGLE:					{ OnEmfRECTANGLE();				break; }
		case EMR_ROUNDRECT:					{ OnEmfROUNDRECT();				break; }
		case EMR_ARC:						{ OnEmfARC();					break; }
		case EMR_CHORD:						{ OnEmfCHORD();					break; }
		case EMR_PIE:						{ OnEmfPIE();					break; }
		case EMR_SELECTPALETTE:				{ OnEmfSELECTPALETTE();			break; }
		case EMR_CREATEPALETTE:				{ OnEmfCREATEPALETTE();			break; }
		case EMR_SETPALETTEENTRIES:			{ OnEmfSETPALETTEENTRIES();		break; }
		case EMR_RESIZEPALETTE:				{ OnEmfRESIZEPALETTE();			break; }
		case EMR_REALIZEPALETTE:			{ OnEmfREALIZEPALETTE();		break; }
		case EMR_EXTFLOODFILL:				{ OnEmfEXTFLOODFILL();			break; }
		case EMR_LINETO:					{ OnEmfLINETO();				break; }
		case EMR_ARCTO:						{ OnEmfARCTO();					break; }
		case EMR_POLYDRAW:					{ OnEmfPOLYDRAW();				break; }
		case EMR_SETARCDIRECTION:			{ OnEmfSETARCDIRECTION();		break; }
		case EMR_SETMITERLIMIT:				{ OnEmfSETMITERLIMIT();			break; }
		case EMR_BEGINPATH:					{ OnEmfBEGINPATH();				break; }
		case EMR_ENDPATH:					{ OnEmfENDPATH();				break; }
		case EMR_CLOSEFIGURE:				{ OnEmfCLOSEFIGURE();			break; }
		case EMR_FILLPATH:					{ OnEmfFILLPATH();				break; }
		case EMR_STROKEANDFILLPATH:			{ OnEmfSTROKEANDFILLPATH();		break; }
		case EMR_STROKEPATH:				{ OnEmfSTROKEPATH();			break; }
		case EMR_FLATTENPATH:				{ OnEmfFLATTENPATH();			break; }
		case EMR_WIDENPATH:					{ OnEmfWIDENPATH();				break; }
		case EMR_SELECTCLIPPATH:			{ OnEmfSELECTCLIPPATH();		break; }
		case EMR_ABORTPATH:					{ OnEmfABORTPATH();				break; }

		case EMR_GDICOMMENT:				{ OnEmfGDICOMMENT();			break; }
		case EMR_FILLRGN:					{ OnEmfFILLRGN();				break; }
		case EMR_FRAMERGN:					{ OnEmfFRAMERGN();				break; }
		case EMR_INVERTRGN:					{ OnEmfINVERTRGN();				break; }
		case EMR_PAINTRGN:					{ OnEmfPAINTRGN();				break; }
		case EMR_EXTSELECTCLIPRGN:			{ OnEmfEXTSELECTCLIPRGN();		break; }
		case EMR_BITBLT:					{ OnEmfBITBLT();				break; }
		case EMR_STRETCHBLT:				{ OnEmfSTRETCHBLT();			break; }
		case EMR_MASKBLT:					{ OnEmfMASKBLT();				break; }
		case EMR_PLGBLT:					{ OnEmfPLGBLT();				break; }
		case EMR_SETDIBITSTODEVICE:			{ OnEmfSETDIBITSTODEVICE();		break; }
		case EMR_STRETCHDIBITS:				{ OnEmfSTRETCHDIBITS();			break; }
		case EMR_EXTCREATEFONTINDIRECTW:	{ OnEmfEXTCREATEFONTINDIRECTW();break; }
		case EMR_EXTTEXTOUTA:				{ OnEmfEXTTEXTOUTA();			break; }
		case EMR_EXTTEXTOUTW:				{ OnEmfEXTTEXTOUTW();			break; }
		case EMR_POLYBEZIER16:				{ OnEmfPOLYBEZIER16();			break; }
		case EMR_POLYGON16:					{ OnEmfPOLYGON16();				break; }
		case EMR_POLYLINE16:				{ OnEmfPOLYLINE16();			break; }
		case EMR_POLYBEZIERTO16:			{ OnEmfPOLYBEZIERTO16();		break; }
		case EMR_POLYLINETO16:				{ OnEmfPOLYLINETO16();			break; }
		case EMR_POLYPOLYLINE16:			{ OnEmfPOLYPOLYLINE16();		break; }
		case EMR_POLYPOLYGON16:				{ OnEmfPOLYPOLYGON16();			break; }
		case EMR_POLYDRAW16:				{ OnEmfPOLYDRAW16();			break; }
		case EMR_CREATEMONOBRUSH:			{ OnEmfCREATEMONOBRUSH();		break; }
		case EMR_CREATEDIBPATTERNBRUSHPT:	{ OnEmfCREATEDIBPATTERNBRUSHPT();break;}
		case EMR_EXTCREATEPEN:              { OnEmfEXTCREATEPEN();			break; }
		case EMR_POLYTEXTOUTA:              { OnEmfPOLYTEXTOUTA();			break; }
		case EMR_POLYTEXTOUTW:              { OnEmfPOLYTEXTOUTW();			break; }

		#if(WINVER >= 0x0400)
		case EMR_SETICMMODE:                { OnEmfSETICMMODE();			break; }
		case EMR_CREATECOLORSPACE:          { OnEmfCREATECOLORSPACE();		break; }
		case EMR_SETCOLORSPACE:             { OnEmfSETCOLORSPACE();			break; }
		case EMR_DELETECOLORSPACE:          { OnEmfDELETECOLORSPACE();		break; }
		case EMR_GLSRECORD:                 { OnEmfGLSRECORD();				break; }
		case EMR_GLSBOUNDEDRECORD:          { OnEmfGLSBOUNDEDRECORD();		break; }
		case EMR_PIXELFORMAT:               { OnEmfPIXELFORMAT();			break; }
		#endif /* WINVER >= 0x0400 */

		#if(WINVER >= 0x0500)
		case EMR_RESERVED_105:              { break; }
		case EMR_RESERVED_106:              { break; }
		case EMR_RESERVED_107:              { break; }
		case EMR_RESERVED_108:              { break; }
		case EMR_RESERVED_109:              { break; }
		case EMR_RESERVED_110:              { break; }
		case EMR_COLORCORRECTPALETTE:       { OnEmfCOLORCORRECTPALETTE();	break; }
		case EMR_SETICMPROFILEA:            { OnEmfSETICMPROFILEA();		break; }
		case EMR_SETICMPROFILEW:            { OnEmfSETICMPROFILEW();		break; }
		case EMR_ALPHABLEND:                { OnEmfALPHABLEND();			break; }
		case EMR_SETLAYOUT:                 { OnEmfALPHADIBBLEND();			break; }
		case EMR_TRANSPARENTBLT:            { OnEmfTRANSPARENTBLT();		break; }
		//case EMR_RESERVED_117:              { OnEmfTRANSPARENTDIB();		break; }
		case EMR_GRADIENTFILL:              { OnEmfGRADIENTFILL();			break; }
		case EMR_RESERVED_119:              { OnEmfSETLINKEDUFIS();			break; }
		case EMR_RESERVED_120:              { OnEmfSETTEXTJUSTIFICATION();	break; }
		case EMR_COLORMATCHTOTARGETW:       { OnEmfCOLORMATCHTOTARGETW();	break; }
		case EMR_CREATECOLORSPACEW:         { OnEmfCREATECOLORSPACEW();		break; }
		#endif /* WINVER >= 0x0500 */
		default:
			break;
		}
	}

	void ConvertPoint(double& x, double& y)
	{
		x = x * m_oBoundsDst.GetWidth() / m_oBoundsSrc.GetWidth() + m_oBoundsDst.left;
		y = y * m_oBoundsDst.GetHeight() / m_oBoundsSrc.GetHeight() + m_oBoundsDst.top;
	}
	template<typename T>
	inline void ConvertPoints(double* pPoints, T* points, DWORD lCount)
	{
		double* pMemory = pPoints;
		for (DWORD i = 0; i < lCount; ++i)
		{
			pMemory[0] = (double)points[i].x;
			pMemory[1] = (double)points[i].y;

			ConvertPoint(pMemory[0], pMemory[1]);
			pMemory += 2;
		}
	}
	template<typename T>
	LPSAFEARRAY ConvertPoints(T* points, DWORD lCount)
	{
		SAFEARRAY* pArray = SafeArrayCreateVector(VT_R8, lCount * 2);
		double* pData = (double*)pArray->pvData;
		
		ConvertPoints(pData, points, lCount);
		return pArray;
	}

	double GetMM(double dPix)
	{
		double dDpiX = 0;
		double dDpiY = 0;

		m_pRenderer->get_DpiX(&dDpiX);
		m_pRenderer->get_DpiY(&dDpiY);

		double dKoef = 50.8 / (dDpiX + dDpiY);
		return dPix * dKoef;
	}

public:

	//////////////////////////////////////////////////////////////////////
	// Events methods
	//////////////////////////////////////////////////////////////////////
	void PlayRecord()
	{
		PlayEnhMetaFileRecord(m_hDC, m_pHandleTable, (ENHMETARECORD*)m_pRecord, m_dwObjects);
	}

	BOOL OnEmfHEADER()
	{
		IS_VALID(m_pRenderer)

		ENHMETAHEADER *pRec = (ENHMETAHEADER*)m_pRecord;

		double _d1 = pRec->szlDevice.cx;
		double _d2 = pRec->szlDevice.cy;

		double _d3 = pRec->szlMillimeters.cx;
		double _d4 = pRec->szlMillimeters.cy;

		double dKoefX = _d1 / (_d3 * 100.0);
		double dKoefY = _d2 / (_d4 * 100.0);
		
		m_oBoundsSrc.left		= dKoefX * pRec->rclFrame.left;
		m_oBoundsSrc.top		= dKoefY * pRec->rclFrame.top;
		m_oBoundsSrc.right		= dKoefX * pRec->rclFrame.right;
		m_oBoundsSrc.bottom		= dKoefY * pRec->rclFrame.bottom;

		double dDpiRendX = 0;
		double dDpiRendY = 0;
		m_pRenderer->get_DpiX(&dDpiRendX);
		m_pRenderer->get_DpiY(&dDpiRendY);

		double dRendDpi = (dDpiRendX + dDpiRendY) / 2.0;

		double dDpiDCx = _d1 / (_d3 * 25.4);
		double dDpiDCy = _d2 / (_d4 * 25.4);

		double dDCDpi = (dDpiDCx + dDpiDCy) / 2.0;

		//m_dSizeTextKoef = dDCDpi / dRendDpi;

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfPOLYBEZIER()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYBEZIER *pRec = (EMRPOLYBEZIER*)m_pRecord;

		SAFEARRAY* pArray = ConvertPoints((LPPOINT)pRec->aptl, pRec->cptl);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandCurvesTo(pArray);
		}
		else
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->PathCommandCurvesTo(pArray);
			m_pRenderer->DrawPath(c_nStroke);
			m_pRenderer->PathCommandEnd();
		}

		RELEASEARRAY(pArray);
		
		return TRUE;
	}

	BOOL OnEmfPOLYGON()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYGON *pRec = (EMRPOLYGON*)m_pRecord;
		
		SAFEARRAY* pArray = ConvertPoints((LPPOINT)pRec->aptl, pRec->cptl);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandLinesTo(pArray);
			m_pRenderer->PathCommandClose();
		}
		else
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->PathCommandLinesTo(pArray);
			m_pRenderer->PathCommandClose();
			m_pRenderer->DrawPath(c_nStroke);
			m_pRenderer->PathCommandEnd();
		}

		RELEASEARRAY(pArray);
		
		return TRUE;
	}

	BOOL OnEmfPOLYLINE()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYLINE *pRec = (EMRPOLYLINE*)m_pRecord;
		
		SAFEARRAY* pArray = ConvertPoints((LPPOINT)pRec->aptl, pRec->cptl);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandLinesTo(pArray);
		}
		else
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->PathCommandLinesTo(pArray);
			m_pRenderer->DrawPath(c_nStroke);
			m_pRenderer->PathCommandEnd();
		}

		RELEASEARRAY(pArray);
		
		return TRUE;
	}

	BOOL OnEmfPOLYBEZIERTO()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYBEZIERTO *pRec = (EMRPOLYBEZIERTO*)m_pRecord;
		
		SAFEARRAY* pArray = ConvertPoints((LPPOINT)pRec->aptl, pRec->cptl);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandCurvesTo(pArray);
		}
		else
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->PathCommandCurvesTo(pArray);
			m_pRenderer->DrawPath(c_nStroke);
			m_pRenderer->PathCommandEnd();
		}

		RELEASEARRAY(pArray);
		
		return TRUE;
	}

	BOOL OnEmfPOLYLINETO()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYLINETO *pRec = (EMRPOLYLINETO*)m_pRecord;
		
		SAFEARRAY* pArray = ConvertPoints((LPPOINT)pRec->aptl, pRec->cptl);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandLinesTo(pArray);
		}
		else
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->PathCommandLinesTo(pArray);
			m_pRenderer->DrawPath(c_nStroke);
			m_pRenderer->PathCommandEnd();
		}

		RELEASEARRAY(pArray);
		
		return TRUE;
	}

	BOOL OnEmfPOLYPOLYLINE()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYPOLYLINE *pRec = (EMRPOLYPOLYLINE*)m_pRecord;
		
		//LPPOINT pPts = (LPPOINT)((DWORD*)pRec->aPolyCounts + pRec->nPolys);
		//m_pRenderer->SCDrawPolyPolyline(pPts, pRec->cptl, (DWORD*)pRec->aPolyCounts, pRec->nPolys);
		
		return TRUE;
	}

	BOOL OnEmfPOLYPOLYGON()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYPOLYGON *pRec = (EMRPOLYPOLYGON*)m_pRecord;
		
		//LPPOINT pPts = (LPPOINT)((DWORD*)pRec->aPolyCounts + pRec->nPolys);
		//m_pRenderer->SCDrawPolyPolygon(pPts, pRec->cptl, (DWORD*)pRec->aPolyCounts, pRec->nPolys);
		
		return TRUE;
	}

	BOOL OnEmfSETWINDOWEXTEX()
	{
		IS_VALID(m_pRenderer)

		LONG lx = ((EMRSETWINDOWEXTEX*)m_pRecord)->szlExtent.cx;
		LONG ly = ((EMRSETWINDOWEXTEX*)m_pRecord)->szlExtent.cy;

		m_oBoundsSrc.right 	= m_oBoundsSrc.left + lx;
		m_oBoundsSrc.bottom	= m_oBoundsSrc.top  + ly;
			
		return TRUE;
	}

	BOOL OnEmfSETWINDOWORGEX()
	{
		IS_VALID(m_pRenderer)

		LONG lx = ((EMRSETWINDOWORGEX*)m_pRecord)->ptlOrigin.x;
		LONG ly = ((EMRSETWINDOWORGEX*)m_pRecord)->ptlOrigin.y;

		m_oBoundsSrc.left	= lx;
		m_oBoundsSrc.top	= ly;
			
		return TRUE;
	}

	BOOL OnEmfSETVIEWPORTEXTEX()
	{
		IS_VALID(m_pRenderer)

		LONG lx = ((EMRSETVIEWPORTEXTEX*)m_pRecord)->szlExtent.cx;
		LONG ly = ((EMRSETVIEWPORTEXTEX*)m_pRecord)->szlExtent.cy;

		return TRUE;
	}

	BOOL OnEmfSETVIEWPORTORGEX()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetViewportOrg(((EMRSETVIEWPORTORGEX*)m_pRecord)->ptlOrigin.x,
		//							  ((EMRSETVIEWPORTORGEX*)m_pRecord)->ptlOrigin.y);

		return TRUE;
	}

	BOOL OnEmfSETBRUSHORGEX()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetBrushOrg(((EMRSETBRUSHORGEX*)m_pRecord)->ptlOrigin);

		return TRUE;
	}

	BOOL OnEmfEOF()
	{
		IS_VALID(m_pRenderer)

		// (EMREOF*)m_pRecord;
		//SCPlayRecord();
		//m_pRenderer->SCEndRendering();

		return TRUE;
	}

	BOOL OnEmfSETPIXELV()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCDrawPixel(((EMRSETPIXELV*)m_pRecord)->ptlPixel,
		//						 ((EMRSETPIXELV*)m_pRecord)->crColor);

		return TRUE;
	}

	BOOL OnEmfSETMAPPERFLAGS()
	{
		IS_VALID(m_pRenderer)

		// (EMRSETMAPPERFLAGS*)m_pRecord;

		return TRUE;
	}

	BOOL OnEmfSETMAPMODE()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetMapMode(((EMRSETMAPMODE*)m_pRecord)->iMode);

		return TRUE;
	}

	BOOL OnEmfSETBKMODE()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetBkMode(((EMRSETBKMODE*)m_pRecord)->iMode);

		return TRUE;
	}

	BOOL OnEmfSETPOLYFILLMODE()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetPolyFillMode(((EMRSETPOLYFILLMODE*)m_pRecord)->iMode);
		
		return TRUE;
	}

	BOOL OnEmfSETROP2()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetROP2(((EMRSETROP2*)m_pRecord)->iMode);
		
		return TRUE;
	}

	BOOL OnEmfSETSTRETCHBLTMODE()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetStretchBltMode(((EMRSETSTRETCHBLTMODE*)m_pRecord)->iMode);
		
		return TRUE;
	}

	BOOL OnEmfSETTEXTALIGN()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetTextAlign(((EMRSETTEXTALIGN*)m_pRecord)->iMode);

		return TRUE;
	}

	BOOL OnEmfSETCOLORADJUSTMENT()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetColorAsjustment(&((EMRSETCOLORADJUSTMENT*)m_pRecord)->ColorAdjustment);
		
		return TRUE;
	}

	BOOL OnEmfSETTEXTCOLOR()
	{
		IS_VALID(m_pRenderer)

		m_lTextColor = ((EMRSETTEXTCOLOR*)m_pRecord)->crColor;
				
		return TRUE;
	}

	BOOL OnEmfSETBKCOLOR()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetBkColor(((EMRSETBKCOLOR*)m_pRecord)->crColor);
		
		return TRUE;
	}

	BOOL OnEmfOFFSETCLIPRGN()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCOffsetClipRect(((EMROFFSETCLIPRGN*)m_pRecord)->ptlOffset);
		
		return TRUE;
	}

	BOOL OnEmfMOVETOEX()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCMoveToEx(((EMRMOVETOEX*)m_pRecord)->ptl);

		double x = (double)((EMRMOVETOEX*)m_pRecord)->ptl.x;
		double y = (double)((EMRMOVETOEX*)m_pRecord)->ptl.y;

		ConvertPoint(x, y);

		m_dCurrentX = x;
		m_dCurrentY = y;

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandMoveTo(x, y);
		}

		return TRUE;
	}

	BOOL OnEmfSETMETARGN()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCOnSetMetaRgn();
		
		return TRUE;
	}

	BOOL OnEmfEXCLUDECLIPRECT()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCExcludeClipRect(((EMREXCLUDECLIPRECT*)m_pRecord)->rclClip);

		return TRUE;
	}

	BOOL OnEmfINTERSECTCLIPRECT()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCIntersectClipRect(((EMRINTERSECTCLIPRECT*)m_pRecord)->rclClip);
		
		return TRUE;
	}

	BOOL OnEmfSCALEVIEWPORTEXTEX()
	{
		IS_VALID(m_pRenderer)

		//EMRSCALEVIEWPORTEXTEX *pRec = (EMRSCALEVIEWPORTEXTEX*)m_pRecord;
		//m_pRenderer->SCScaleViewport(pRec->xNum, pRec->xDenom, pRec->yNum, pRec->yDenom);

		return TRUE;
	}

	BOOL OnEmfSCALEWINDOWEXTEX()
	{
		IS_VALID(m_pRenderer)

		//EMRSCALEWINDOWEXTEX *pRec = (EMRSCALEWINDOWEXTEX*)m_pRecord;
		//m_pRenderer->SCScaleWindow(pRec->xNum, pRec->xDenom, pRec->yNum, pRec->yDenom);
		
		return TRUE;
	}

	BOOL OnEmfSAVEDC()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCOnDCSaved();

		return TRUE;
	}

	BOOL OnEmfRESTOREDC()
	{
		IS_VALID(m_pRenderer)
		
		//m_pRenderer->SCOnDCRestored(((EMRRESTOREDC*)m_pRecord)->iRelative);

		return TRUE;
	}

	BOOL OnEmfSETWORLDTRANSFORM()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetWorldTransform(((EMRSETWORLDTRANSFORM*)m_pRecord)->xform);

		return TRUE;
	}

	BOOL OnEmfMODIFYWORLDTRANSFORM()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCModifyWorldTransform(((EMRMODIFYWORLDTRANSFORM*)m_pRecord)->xform,
		//									((EMRMODIFYWORLDTRANSFORM*)m_pRecord)->iMode);

		return TRUE;
	}

	BOOL OnEmfSELECTOBJECT()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();
			
		EMRSELECTOBJECT *pRec = (EMRSELECTOBJECT*)m_pRecord;
		HGDIOBJ hGdiObj;

		if (pRec->ihObject & 0x80000000)
		{
			DWORD dwIndex = pRec->ihObject & 0x7fffffff;
			hGdiObj = GetStockObject( dwIndex );
		} 
		else
		{
			hGdiObj = (HGDIOBJ)m_pHandleTable->objectHandle[pRec->ihObject];
		}

		long lType = GetObjectType(hGdiObj);
		switch(lType)
		{
			case OBJ_EXTPEN:
				{
					DWORD dwSize = ::GetObject((HPEN)hGdiObj, sizeof(EXTLOGPEN), NULL);
					BYTE* pBytes = new BYTE[dwSize];
					if (::GetObject((HPEN)hGdiObj, dwSize, pBytes))
					{
						EXTLOGPEN* pExtLogPen = (EXTLOGPEN*)pBytes;
						if (BS_SOLID == pExtLogPen->elpBrushStyle || BS_HATCHED == pExtLogPen->elpBrushStyle)
						{
							m_pRenderer->put_PenColor(pExtLogPen->elpColor);
						}

						INT iPenStyle = pExtLogPen->elpPenStyle & PS_STYLE_MASK;  // style bits masking is required
						if (PS_NULL==iPenStyle)
							return NULL;

						INT iPenType = pExtLogPen->elpPenStyle & PS_TYPE_MASK;

						// Default pen width and color
						INT iWidth = (PS_GEOMETRIC == iPenType) ? pExtLogPen->elpWidth : 1;
						
						m_pRenderer->put_PenColor(pExtLogPen->elpColor);
						m_pRenderer->put_PenSize(GetMM((double)iWidth));

						// Line style
						switch (iPenStyle)
						{
						case PS_SOLID:
							m_pRenderer->put_PenDashStyle(DashStyleSolid);
							break;

						case PS_DASH:
							m_pRenderer->put_PenDashStyle(DashStyleDash);
							break;

						case PS_DOT:
							m_pRenderer->put_PenDashStyle(DashStyleDot);
							break;

						case PS_DASHDOT:
							m_pRenderer->put_PenDashStyle(DashStyleDashDot);
							break;

						case PS_DASHDOTDOT:
							m_pRenderer->put_PenDashStyle(DashStyleDashDotDot);
							break;

						case PS_USERSTYLE:
							// custom
							m_pRenderer->put_PenDashStyle(DashStyleCustom);
							break;

						case PS_INSIDEFRAME:
							m_pRenderer->put_PenDashStyle(DashStyleSolid);
							break;

						case PS_ALTERNATE:
						default:
							m_pRenderer->put_PenDashStyle(DashStyleSolid);
						}

						if (PS_GEOMETRIC==iPenType)
						{
							// Lines joint (applies only to PS_GEOMETRIC pens in GDI)
							switch (pExtLogPen->elpPenStyle & PS_JOIN_MASK)
							{
							case PS_JOIN_ROUND:
								m_pRenderer->put_PenLineJoin(LineJoinRound);
								break;
								
							case PS_JOIN_MITER:
								m_pRenderer->put_PenLineJoin(LineJoinMiter);
								break;
								
							case PS_JOIN_BEVEL:
								m_pRenderer->put_PenLineJoin(LineJoinBevel);
								break;
								
							default:
								m_pRenderer->put_PenLineJoin(LineJoinRound);
							}

							switch (pExtLogPen->elpPenStyle & PS_ENDCAP_MASK)
							{
							case PS_ENDCAP_ROUND:
								m_pRenderer->put_PenLineEndCap(LineCapRound);
								break;
								
							case PS_ENDCAP_FLAT:
								m_pRenderer->put_PenLineEndCap(LineCapFlat);
								break;
								
							case PS_ENDCAP_SQUARE:
								m_pRenderer->put_PenLineEndCap(LineCapSquare);
								break;
								
							default:
								m_pRenderer->put_PenLineEndCap(LineCapRound);
							}

							if  ((BS_HOLLOW != pExtLogPen->elpBrushStyle) && (BS_SOLID!=pExtLogPen->elpBrushStyle || PS_SOLID!=iPenStyle))
							{
								LOGBRUSH LogBrush;
								LogBrush.lbStyle = pExtLogPen->elpBrushStyle;
								LogBrush.lbColor = pExtLogPen->elpColor;
								LogBrush.lbHatch = pExtLogPen->elpHatch;
							}
						} 
					}
					RELEASEARRAYOBJECTS(pBytes);
					break;
				}

			case OBJ_PEN:
				{
					LOGPEN LogPen;
					if (::GetObject((HPEN)hGdiObj, sizeof(LogPen), &LogPen))
					{
						if (PS_NULL == LogPen.lopnStyle)
							return FALSE;

						m_pRenderer->put_PenColor(LogPen.lopnColor);
						m_pRenderer->put_PenSize(GetMM((double)LogPen.lopnWidth.x));

						// Line style
						switch (LogPen.lopnStyle)
						{
						case PS_SOLID:
							m_pRenderer->put_PenDashStyle(Gdiplus::DashStyleSolid);
							break;

						case PS_DASH:
							m_pRenderer->put_PenDashStyle(Gdiplus::DashStyleDash);
							break;

						case PS_DOT:
							m_pRenderer->put_PenDashStyle(Gdiplus::DashStyleDot);
							break;

						case PS_DASHDOT:
							m_pRenderer->put_PenDashStyle(Gdiplus::DashStyleDashDot);
							break;

						case PS_DASHDOTDOT:
							m_pRenderer->put_PenDashStyle(Gdiplus::DashStyleDashDotDot);
							break;

						case PS_INSIDEFRAME:
						case PS_ALTERNATE:
							m_pRenderer->put_PenDashStyle(Gdiplus::DashStyleSolid);
							break;

						default:
							break;
						}
					}
					break;
				}

			case OBJ_BRUSH:
				{
					LOGBRUSH LogBrush;
					LogBrush.lbHatch = 0;
					if (::GetObject((HBRUSH)hGdiObj, sizeof(LogBrush), &LogBrush))
					{
						switch (LogBrush.lbStyle)
						{
						case BS_SOLID:
							{
								m_pRenderer->put_BrushType(c_BrushTypeSolid);
								m_pRenderer->put_BrushColor1(LogBrush.lbColor);
								break;
							}
							
						case BS_HATCHED:
							{
								// Hatched brush
								switch (LogBrush.lbHatch)
								{
								case HS_BDIAGONAL:
									break;
								case HS_CROSS:
									break;
								case HS_DIAGCROSS:
									break;
								case HS_FDIAGONAL:
									break;
								case HS_HORIZONTAL:
									break;
								case HS_VERTICAL:
									break;
								default:
									break;
								}

								m_pRenderer->put_BrushColor1(LogBrush.lbColor);
								m_pRenderer->put_BrushColor2(0x00);
								break;
							}

						case BS_PATTERN:
						case BS_PATTERN8X8:
							{
								BITMAP bm;
								int iRes = GetObject((HBITMAP)LogBrush.lbHatch, sizeof(BITMAP), &bm);

								// реализовать!!!
								
							}
							break;

						case BS_DIBPATTERNPT:
							{
								BITMAPINFO* pBmi = (BITMAPINFO*)((LPBYTE)LogBrush.lbHatch);
								BYTE* pBits = (BYTE*)pBmi + pBmi->bmiHeader.biSize + pBmi->bmiHeader.biClrUsed;

								// реализовать!!!
							}
							break;

						case BS_DIBPATTERN:
						case BS_DIBPATTERN8X8:
							{
								// get the BITMAPINFO
								BITMAPINFO* pBmi = (BITMAPINFO*)((HGLOBAL)LogBrush.lbHatch); //GlobalLock(hMem);

								// Locate the bits after the color table:
								// Since the DIB is packed, the biClrUsed member must be either zero or
								// the actual size of the color table
								INT iNbColors = 0;
								//INT iNbColors = DIBNumColors(LPBITMAPINFOHEADER(pBmi)); 
								// (LPBITMAPINFOHEADER)pBmi)->biClrUsed
								BYTE* pBits = (BYTE*)pBmi + pBmi->bmiHeader.biSize + iNbColors*sizeof(RGBQUAD);
							}
							break;

						case BS_NULL:
							break;

						case BS_INDEXED:
						case BS_MONOPATTERN:
							break;

						default:
							break;
						}
					}
					break;
				}
				
			case OBJ_FONT:
				{
					LOGFONT LogFont;
					INT iRes = ::GetObject((HFONT)hGdiObj, sizeof(LogFont), &LogFont);

					NSStructures::CFont oFont;

					if (LogFont.lfUnderline)
					{
						oFont.Underline = 1;
					}
					if (LogFont.lfStrikeOut)
					{
						oFont.Strikeout = 1;
					}

					m_pRenderer->put_BrushColor1(0);
					
					_bstr_t bsName = (LPTSTR)(LogFont.lfFaceName);
					m_pRenderer->put_FontName(bsName.GetBSTR());
					m_pRenderer->put_FontSize(m_dSizeTextKoef * LogFont.lfHeight);
					m_pRenderer->put_FontStyle(oFont.GetStyle());
				}
			
			default:
				break;
		}

		return TRUE;
	}

	BOOL OnEmfCREATEPEN()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfCREATEBRUSHINDIRECT()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();
		
		return TRUE;
	}

	BOOL OnEmfDELETEOBJECT()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();
		
		return TRUE;
	}

	BOOL OnEmfANGLEARC()
	{
		IS_VALID(m_pRenderer)

		//EMRANGLEARC *pRec = (EMRANGLEARC*)m_pRecord;
		//m_pRenderer->SCDrawAngleArc((LPPOINT)&pRec->ptlCenter, pRec->nRadius, pRec->eStartAngle, pRec->eSweepAngle);
		
		return TRUE;
	}

	BOOL OnEmfELLIPSE()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCDrawEllipse((LPCRECT)&((EMRELLIPSE*)m_pRecord)->rclBox);
		
		return TRUE;
	}

	BOOL OnEmfRECTANGLE()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCDrawRectangle((LPCRECT)&((EMRRECTANGLE*)m_pRecord)->rclBox);
		
		return TRUE;
	}

	BOOL OnEmfROUNDRECT()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCDrawRoundRectangle((LPCRECT)&((EMRROUNDRECT*)m_pRecord)->rclBox,
		//											((EMRROUNDRECT*)m_pRecord)->szlCorner);

		return TRUE;
	}

	BOOL OnEmfARC()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCDrawArc(&((EMRARC*)m_pRecord)->rclBox,
		//					   &((EMRARC*)m_pRecord)->ptlStart,
		//					   &((EMRARC*)m_pRecord)->ptlEnd);

		return TRUE;
	}

	BOOL OnEmfCHORD()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCDrawChord(&((EMRCHORD*)m_pRecord)->rclBox,
		//						 &((EMRCHORD*)m_pRecord)->ptlStart,
		//						 &((EMRCHORD*)m_pRecord)->ptlEnd);

		return TRUE;
	}

	BOOL OnEmfPIE()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCDrawPie(&((EMRPIE*)m_pRecord)->rclBox,
		//					   &((EMRPIE*)m_pRecord)->ptlStart,
		//					   &((EMRPIE*)m_pRecord)->ptlEnd);

		return TRUE;

	}

	BOOL OnEmfSELECTPALETTE()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();
		
		return TRUE;
	}

	BOOL OnEmfCREATEPALETTE()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();
		
		return TRUE;
	}

	BOOL OnEmfSETPALETTEENTRIES()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfRESIZEPALETTE()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfREALIZEPALETTE()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfEXTFLOODFILL()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCFloodFill(((EMREXTFLOODFILL*)m_pRecord)->ptlStart,
		//						 ((EMREXTFLOODFILL*)m_pRecord)->crColor,
		//						 ((EMREXTFLOODFILL*)m_pRecord)->iMode);

		return TRUE;
	}

	BOOL OnEmfLINETO()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCDrawLinesTo((LPPOINT)&((EMRLINETO*)m_pRecord)->ptl, 1);
		double x = (double)((EMRLINETO*)m_pRecord)->ptl.x;
		double y = (double)((EMRLINETO*)m_pRecord)->ptl.y;

		ConvertPoint(x, y);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandLineTo(x, y);
		}
		else
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->PathCommandMoveTo(m_dCurrentX, m_dCurrentY);
			m_pRenderer->PathCommandLineTo(x, y);
			m_pRenderer->DrawPath(c_nStroke);
			m_pRenderer->PathCommandEnd();
		}
		
		return TRUE;
	}

	BOOL OnEmfARCTO()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCDrawArcTo(&((EMRARCTO*)m_pRecord)->rclBox,
		//						 &((EMRARCTO*)m_pRecord)->ptlStart,
		//						 &((EMRARCTO*)m_pRecord)->ptlEnd);

		return TRUE;
	}

	BOOL OnEmfPOLYDRAW()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYDRAW *pRec = (EMRPOLYDRAW*)m_pRecord;

		//BYTE* pTypes = (BYTE*)((POINTL*)pRec->aptl + pRec->cptl);
		//m_pRenderer->SCDrawPolyDraw((POINT*)pRec->aptl,
		//							pRec->cptl,
		//							pTypes);

		return TRUE;
	}

	BOOL OnEmfSETARCDIRECTION()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetArcDirection(((EMRSETARCDIRECTION*)m_pRecord)->iArcDirection);

		return TRUE;
	}

	BOOL OnEmfSETMITERLIMIT()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSetMiterLimit(((EMRSETMITERLIMIT*)m_pRecord)->eMiterLimit);

		return TRUE;
	}

	BOOL OnEmfBEGINPATH()
	{
		IS_VALID(m_pRenderer)

		m_pRenderer->PathCommandStart();
		m_bIsPath = TRUE;

		return TRUE;
	}

	BOOL OnEmfENDPATH()
	{
		IS_VALID(m_pRenderer)

		//EMRENDPATH *pRec = (EMRENDPATH*)m_pRecord;

		//m_pRenderer->PathCommandEnd();
		
		return TRUE;
	}

	BOOL OnEmfCLOSEFIGURE()
	{
		IS_VALID(m_pRenderer)

		m_pRenderer->PathCommandClose();

		return TRUE;
	}

	BOOL OnEmfFILLPATH()
	{
		IS_VALID(m_pRenderer)

		m_pRenderer->DrawPath(c_nWindingFillMode);

		return TRUE;
	}

	BOOL OnEmfSTROKEANDFILLPATH()
	{
		IS_VALID(m_pRenderer)

		m_pRenderer->DrawPath(c_nStroke | c_nWindingFillMode);

		return TRUE;
	}

	BOOL OnEmfSTROKEPATH()
	{
		IS_VALID(m_pRenderer)

		m_pRenderer->DrawPath(c_nStroke);

		return TRUE;
	}

	BOOL OnEmfFLATTENPATH()
	{
		IS_VALID(m_pRenderer)

		//EMRFLATTENPATH *pRec = (EMRFLATTENPATH*)m_pRecord;

		//m_pRenderer->SCFlattenPath();

		return TRUE;
	}

	BOOL OnEmfWIDENPATH()
	{
		IS_VALID(m_pRenderer)

		//EMRWIDENPATH *pRec = (EMRWIDENPATH*)m_pRecord;

		//m_pRenderer->SCWidenPath();
		
		return TRUE;
	}

	BOOL OnEmfSELECTCLIPPATH()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCApplyPath(SC_PATH_CLIP, ((EMRSELECTCLIPPATH*)m_pRecord)->iMode);
		
		return TRUE;
	}

	BOOL OnEmfABORTPATH()
	{
		IS_VALID(m_pRenderer)

		m_pRenderer->PathCommandEnd();
		m_bIsPath = FALSE;
		
		return TRUE;
	}

	BOOL OnEmfGDICOMMENT()
	{
		IS_VALID(m_pRenderer)

		return TRUE;
	}

	BOOL OnEmfFILLRGN()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCFillRgn((HBRUSH)m_lpEnumHandleTable->objectHandle[((EMRFILLRGN*)m_pRecord)->ihBrush],
		//					   ((EMRFILLRGN*)m_pRecord)->RgnData,
		//					   ((EMRFILLRGN*)m_pRecord)->cbRgnData);

		return TRUE;
	}

	BOOL OnEmfFRAMERGN()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCFrameRgn((HBRUSH)m_lpEnumHandleTable->objectHandle[((EMRFRAMERGN*)m_pRecord)->ihBrush],
		//						((EMRFRAMERGN*)m_pRecord)->szlStroke,
		//						((EMRFRAMERGN*)m_pRecord)->RgnData,
		//						((EMRFRAMERGN*)m_pRecord)->cbRgnData);

		return TRUE;
	}

	BOOL OnEmfINVERTRGN()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCInvertRgn(((EMRINVERTRGN*)m_pRecord)->RgnData,
		//						 ((EMRINVERTRGN*)m_pRecord)->cbRgnData);

		return TRUE;
	}

	BOOL OnEmfPAINTRGN()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCPaintRgn(((EMRPAINTRGN*)m_pRecord)->RgnData,
		//						((EMRPAINTRGN*)m_pRecord)->cbRgnData);

		return TRUE;
	}

	BOOL OnEmfEXTSELECTCLIPRGN()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCSelectClipRgn(((EMREXTSELECTCLIPRGN*)m_pRecord)->RgnData,
		//							 ((EMREXTSELECTCLIPRGN*)m_pRecord)->cbRgnData,
		//							 ((EMREXTSELECTCLIPRGN*)m_pRecord)->iMode);

		return TRUE;
	}

	BOOL OnEmfBITBLT()
	{
		IS_VALID(m_pRenderer)

		EMRBITBLT *pRec = (EMRBITBLT*)m_pRecord;

		//// Destination
		//CRect RcDest(pRec->xDest, pRec->yDest, pRec->xDest + pRec->cxDest, pRec->yDest + pRec->cyDest);
		//// Check if there is a bitmap
		//if (pRec->offBmiSrc && pRec->offBitsSrc)
		//{
		//	BITMAPINFO *pBmi    = (BITMAPINFO *) ((BYTE *)pRec + pRec->offBmiSrc); 
		//	BYTE *pBits     = (BYTE *) ((BYTE *) pRec + pRec->offBitsSrc);
		//	// Source and Destination have same size for BitBlt function
		//	CRect RcSrc(pRec->xSrc, pRec->ySrc, pRec->xSrc + pRec->cxDest, pRec->ySrc + pRec->cyDest);
		//	m_pRenderer->SCDrawImage(&RcDest, &RcSrc, pBits, pBmi, pRec->iUsageSrc, pRec->dwRop, &pRec->xformSrc);
		//} else
		//	m_pRenderer->SCPatBlt(&RcDest, pRec->dwRop, &pRec->xformSrc);

		return TRUE;
	}

	BOOL OnEmfSTRETCHBLT()
	{
		IS_VALID(m_pRenderer)

		EMRSTRETCHBLT *pRec = (EMRSTRETCHBLT*)m_pRecord;

		//// Destination
		//CRect RcDest(pRec->xDest, pRec->yDest, pRec->xDest + pRec->cxDest, pRec->yDest + pRec->cyDest);
		//// Check if there is a bitmap
		//if (pRec->offBmiSrc && pRec->offBitsSrc)
		//{
		//	BITMAPINFO *pBmi    = (BITMAPINFO *) ((BYTE *)pRec + pRec->offBmiSrc); 
		//	BYTE *pBits     = (BYTE *) ((BYTE *) pRec + pRec->offBitsSrc);
		//	// Source
		//	CRect RcSrc(pRec->xSrc, pRec->ySrc, pRec->xSrc + pRec->cxSrc, pRec->ySrc + pRec->cySrc);
		//	m_pRenderer->SCDrawImage(&RcDest, &RcSrc, pBits, pBmi, pRec->iUsageSrc, pRec->dwRop, &pRec->xformSrc);
		//} else
		//	m_pRenderer->SCPatBlt(&RcDest, pRec->dwRop, &pRec->xformSrc);

		return TRUE;
	}

	BOOL OnEmfMASKBLT()
	{
		IS_VALID(m_pRenderer)

		EMRMASKBLT *pRec = (EMRMASKBLT*)m_pRecord;

		//// Destination
		//CRect RcDest(pRec->xDest, pRec->yDest, pRec->xDest + pRec->cxDest, pRec->yDest + pRec->cyDest);
		//// Check if there is a bitmap
		//if (pRec->offBmiSrc && pRec->offBitsSrc)
		//{
		//	// the source bitmap
		//	BITMAPINFO *pBmi    = (BITMAPINFO *) ((BYTE *)pRec + pRec->offBmiSrc); 
		//	BYTE *pBits     = (BYTE *) ((BYTE *) pRec + pRec->offBitsSrc);

		//	// the mask bitmap is optional
		//	BITMAPINFO *pBmiMask = NULL;
		//	BYTE *pBitsMask = NULL;
		//	if (pRec->offBmiMask && pRec->offBitsMask)
		//	{
		//		pBmiMask = (BITMAPINFO *)((BYTE *)pRec + pRec->offBmiMask); 
		//		pBitsMask = (BYTE *)((BYTE *)pRec + pRec->offBitsMask);
		//	}

		//	// Source equals dest in dimensions
		//	CRect RcSrc(pRec->xSrc, pRec->ySrc, pRec->xSrc + pRec->cxDest, pRec->ySrc + pRec->cyDest);

		//	m_pRenderer->SCDrawImageMsk(&RcDest, &RcSrc, pBits, pBmi, pRec->iUsageSrc, pRec->dwRop, &pRec->xformSrc,
		//	pRec->crBkColorSrc, pRec->xMask, pRec->yMask, pBitsMask, pBmiMask, pRec->iUsageMask);
		//} else
		//	m_pRenderer->SCPatBlt(&RcDest, pRec->dwRop, &pRec->xformSrc);

		return TRUE;
	}

	BOOL OnEmfPLGBLT()
	{
		IS_VALID(m_pRenderer)

		EMRPLGBLT *pRec = (EMRPLGBLT*)m_pRecord;

		//ASSERT(pRec->offBmiSrc && pRec->offBitsSrc);
		//if (pRec->offBmiSrc && pRec->offBitsSrc)
		//{
		//	// the source bitmap
		//	BITMAPINFO *pBmi = (BITMAPINFO *)((BYTE *)pRec + pRec->offBmiSrc); 
		//	BYTE *pBits = (BYTE *)((BYTE *)pRec + pRec->offBitsSrc);

		//	// the mask bitmap is optional
		//	BITMAPINFO *pBmiMask = NULL;
		//	BYTE *pBitsMask = NULL;
		//	if (pRec->offBmiMask && pRec->offBitsMask)
		//	{
		//		pBmiMask = (BITMAPINFO *)((BYTE *)pRec + pRec->offBmiMask); 
		//		pBitsMask = (BYTE *)((BYTE *)pRec + pRec->offBitsMask);
		//	}

		//	// Source
		//	CRect RcSrc(pRec->xSrc, pRec->ySrc, pRec->xSrc + pRec->cxSrc, pRec->ySrc + pRec->cySrc);

		//	m_pRenderer->SCDrawImagePlg(pRec->aptlDest,
		//	&RcSrc, pBits, pBmi, pRec->iUsageSrc, &pRec->xformSrc, pRec->crBkColorSrc,
		//	pRec->xMask, pRec->yMask, pBitsMask, pBmiMask, pRec->iUsageMask);
		//}

		return TRUE;
	}

	BOOL OnEmfSETDIBITSTODEVICE()
	{
		IS_VALID(m_pRenderer)

		EMRSETDIBITSTODEVICE *pRec = (EMRSETDIBITSTODEVICE*)m_pRecord;

		//// Destination
		//CRect RcDest(pRec->xDest, pRec->yDest, pRec->xDest + pRec->cxSrc, pRec->yDest + pRec->cySrc);
		//// Check if there is a bitmap
		//if (pRec->offBmiSrc && pRec->offBitsSrc)
		//{
		//	BITMAPINFO *pBmi    = (BITMAPINFO *) ((BYTE *)pRec + pRec->offBmiSrc); 
		//	BYTE *pBits     = (BYTE *) ((BYTE *) pRec + pRec->offBitsSrc);
		//	// Source
		//	CRect RcSrc(pRec->xSrc, pRec->ySrc, pRec->xSrc + pRec->cxSrc, pRec->ySrc + pRec->cySrc);
		//	m_pRenderer->SCDrawImage(&RcDest, &RcSrc, pBits, pBmi, pRec->iUsageSrc, SRCCOPY);
		//} else
		//	m_pRenderer->SCPatBlt(&RcDest, SRCCOPY);

		return TRUE;
	}

	BOOL OnEmfSTRETCHDIBITS()
	{
		IS_VALID(m_pRenderer)

		EMRSTRETCHDIBITS *pRec = (EMRSTRETCHDIBITS*)m_pRecord;

		//// Destination
		//CRect RcDest(pRec->xDest, pRec->yDest, pRec->xDest + pRec->cxDest, pRec->yDest + pRec->cyDest);
		//// Check if there is a bitmap
		//if (pRec->offBmiSrc && pRec->offBitsSrc)
		//{
		//	BITMAPINFO *pBmi    = (BITMAPINFO *) ((BYTE *)pRec + pRec->offBmiSrc); 
		//	BYTE *pBits     = (BYTE *) ((BYTE *) pRec + pRec->offBitsSrc);
		//	// Source
		//	CRect RcSrc(pRec->xSrc, pRec->ySrc, pRec->xSrc + pRec->cxSrc, pRec->ySrc + pRec->cySrc);
		//	m_pRenderer->SCDrawImage(&RcDest, &RcSrc, pBits, pBmi, pRec->iUsageSrc, pRec->dwRop);
		//} else
		//	m_pRenderer->SCPatBlt(&RcDest, pRec->dwRop);

		return TRUE;
	}

	BOOL OnEmfEXTCREATEFONTINDIRECTW()
	{
		IS_VALID(m_pRenderer)

		return TRUE;
	}


	BOOL OnEmfEXTTEXTOUTA()
	{
		IS_VALID(m_pRenderer)

		// the structure EMREXTTEXTOUTA is same as EMREXTTEXTOUTW
		EMREXTTEXTOUTA *pRec = (EMREXTTEXTOUTA*)m_pRecord;

		// get the EMRTEXT
		EMRTEXT *pTextRec = &(pRec->emrtext);

		// number of characters	
		INT iNbChars = pTextRec->nChars;
		if (iNbChars <= 0)
		{
			return FALSE;
		}

		UINT uiOptions = pTextRec->fOptions; // we might have to alter it

		// Build the string
		LPWSTR pwStr = NULL;
		if (pRec->emr.iType == EMR_EXTTEXTOUTW) 
		{
			// The string is in unicode -> we let it as is.
			// Offset is from the beginning of the struct and is in bytes.
			// 	
			// Get the unicode string
			pwStr = (LPWSTR) ((BYTE*)pRec + pTextRec->offString);
		} 
		else
		{
			// The string is single-byte encoded.
			// Get adress of the string	then construct the string.			
			LPSTR pString = (char *)pRec + pTextRec->offString;
			pwStr = new wchar_t[iNbChars+1];
			if (pwStr)
			{
				// We will use UNICODE strings exclusively
				pwStr[iNbChars] = 0;
				int iNum = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pString, iNbChars, pwStr, iNbChars+1);
			}
		}

		// Get intercharacter spacing array.
		// Be careful ! there are cases where OffDx==0.
		DWORD OffDx = pTextRec->offDx;

		INT *lpDx = (INT*)((BYTE*)pRec + OffDx);
		//if (0 == OffDx)
		{
			lpDx = new INT[iNbChars];
			memset(lpDx, 0, iNbChars * sizeof(INT));
		}

		if (GM_COMPATIBLE == pRec->iGraphicsMode)
		{
			SCDrawText(pTextRec->ptlReference.x, pTextRec->ptlReference.y, uiOptions,
			(LPCRECT)&pTextRec->rcl, pwStr, iNbChars, lpDx, pRec->exScale, pRec->eyScale);
		}
		else
		{
			SCDrawText(pTextRec->ptlReference.x, pTextRec->ptlReference.y, uiOptions,
			(LPCRECT)&pTextRec->rcl, pwStr, iNbChars, lpDx, 1, 1);
		}

		// Clean up
		if (pRec->emr.iType == EMR_EXTTEXTOUTA) 
			delete [] pwStr;

		return TRUE;
	}

	void SCDrawText(INT x, INT y, UINT uiOptions, LPCRECT pRect, LPCWSTR pwString,
								UINT uiCount, LPINT pDxWidths, double fScaleX, double fScaleY)
	{
		double dX = (double)x;
		double dY = (double)y;
		double dR = (double)pRect->right;
		double dB = (double)pRect->bottom;
		
		ConvertPoint(dX, dY);
		ConvertPoint(dR, dB);

		LONG lColorOld = 0;
		m_pRenderer->get_BrushColor1(&lColorOld);
		m_pRenderer->put_BrushColor1(m_lTextColor);
		
		m_pRenderer->CommandDrawText((BSTR)pwString, dX, dY, dR - dX, dB - dY, 0);

		m_pRenderer->put_BrushColor1(lColorOld);
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// It is unlikely that SCEmfTextoutA/SCEmfTextoutW are called. So I prefer to let
	// them separated (with duplicated code). See OnEmfTextoutA for documentation
	BOOL SCEmfTextoutA(EMREXTTEXTOUTA* pRec, EMRTEXT* pTextRec)
	{
		INT iNbChars = pTextRec->nChars;
		if (iNbChars<=0)
			return TRUE;

		// Get adress of the string	then construct the widestring.			
		LPWSTR pwStr = new wchar_t[iNbChars + 1];
		if (pwStr)
		{// We will use UNICODE strings exclusively
			pwStr[iNbChars] = 0;
			int iNum = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)((char *)pRec + pTextRec->offString),
				iNbChars, pwStr, iNbChars + 1);
			//ASSERT(iNum==iNbChars);
			
			SCFinishTextout(pRec, pTextRec, pwStr, pTextRec->fOptions);
			
			// Clean up
			delete [] pwStr;
		}
		return TRUE;
	}

	BOOL SCEmfTextoutW(EMREXTTEXTOUTW* pRec, EMRTEXT* pTextRec)
	{
		INT iNbChars = pTextRec->nChars;
		if (iNbChars<=0)
			return TRUE;

		UINT uiOptions = pTextRec->fOptions; // we might have to alter it

		// Get the unicode string
		LPWSTR pwStr = (LPWSTR) ((BYTE *)pRec + pTextRec->offString);

		SCFinishTextout(pRec, pTextRec, pwStr, uiOptions);
		return TRUE;
	}

	void SCFinishTextout(EMREXTTEXTOUTA* pRec, EMRTEXT* pTextRec, LPWSTR pwStr, UINT uiOptions)
	{
		//IS_VALID(m_pRenderer)

		//INT iNbChars = pTextRec->nChars;
		//// Get intercharacter spacing array.
		//DWORD OffDx = pTextRec->offDx;
		//INT *lpDx = (INT*)((BYTE*)pRec + OffDx);
		//if (0==OffDx)
		//{
		//	// Compute intercharacter spacing, based on each character width.
		//	lpDx = new INT[iNbChars];
		//	BOOL bOk = SCGetTextCharWidthsW(m_hPlayDC, pwStr, iNbChars, lpDx);
		//	ASSERT(bOk);
		//}
		//else
		//{ // else use distances between origins of adjacent character cells as passed to ExtTextOut.
		//	// except that, sometimes, the last character is given a spacing of 0, which is not good for us.
		//	if (0==lpDx[iNbChars-1])
		//	{
		//		INT* pInts = new INT[iNbChars];
		//		memmove(pInts, lpDx, iNbChars*sizeof(INT));

		//		// compute only the last character's width
		//		BOOL bOk = SCGetTextCharWidthsW(m_hPlayDC, pwStr+iNbChars-1, 1, pInts+iNbChars-1);
		//		ASSERT(bOk);
		//		lpDx = pInts;
		//		OffDx = 0; // for cleanup
		//		ASSERT(lpDx[iNbChars-1]);
		//	}
		//}

		//if (GM_COMPATIBLE==pRec->iGraphicsMode)
		//	m_pRenderer->SCDrawText(pTextRec->ptlReference.x, pTextRec->ptlReference.y, uiOptions,
		//	(LPCRECT)&pTextRec->rcl, pwStr, iNbChars, lpDx, pRec->exScale, pRec->eyScale);
		//else
		//	m_pRenderer->SCDrawText(pTextRec->ptlReference.x, pTextRec->ptlReference.y, uiOptions,
		//	(LPCRECT)&pTextRec->rcl, pwStr, iNbChars, lpDx, 1, 1);

		//// Clean up
		//if (0==OffDx)
		//	delete [] lpDx;
	}
	///////////////////////////////////////////////////////////////////////////////////////////

	BOOL OnEmfEXTTEXTOUTW()
	{
		return OnEmfEXTTEXTOUTA();
	}

	BOOL OnEmfPOLYBEZIER16()
	{
		IS_VALID(m_pRenderer)

		//m_pRenderer->SCDrawBezierS(((EMRPOLYBEZIER16*)m_pRecord)->apts,
		//						  ((EMRPOLYBEZIER16*)m_pRecord)->cpts);
		
		return TRUE;
	}

	BOOL OnEmfPOLYGON16()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYGON16 *pRec = (EMRPOLYGON16*)m_pRecord;

		SAFEARRAY* pArray = ConvertPoints((LPPOINTS)pRec->apts, pRec->cpts);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandLinesTo(pArray);
			m_pRenderer->PathCommandClose();
		}
		else
		{
			m_pRenderer->PathCommandStart();
			//m_pRenderer->PathCommandMoveTo(m_dCurrentX, m_dCurrentY);
			double* mas = (double*)pArray->pvData;
			m_pRenderer->PathCommandMoveTo(mas[0], mas[1]);
			m_pRenderer->PathCommandLinesTo(pArray);
			m_pRenderer->PathCommandClose();
			m_pRenderer->DrawPath(c_nStroke | c_nWindingFillMode);
			m_pRenderer->PathCommandEnd();
		}

		RELEASEARRAY(pArray);


		return TRUE;
	}

	BOOL OnEmfPOLYLINE16()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYLINE16 *pRec = (EMRPOLYLINE16*)m_pRecord;

		SAFEARRAY* pArray = ConvertPoints((LPPOINTS)pRec->apts, pRec->cpts);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandCurvesTo(pArray);
		}
		else
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->PathCommandMoveTo(m_dCurrentX, m_dCurrentY);
			m_pRenderer->PathCommandCurvesTo(pArray);
			m_pRenderer->DrawPath(c_nStroke);
			m_pRenderer->PathCommandEnd();
		}

		RELEASEARRAY(pArray);

		return TRUE;
	}


	BOOL OnEmfPOLYBEZIERTO16()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYBEZIERTO16 *pRec = (EMRPOLYBEZIERTO16*)m_pRecord;

		SAFEARRAY* pArray = ConvertPoints((LPPOINTS)pRec->apts, pRec->cpts);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandCurvesTo(pArray);
		}
		else
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->PathCommandCurvesTo(pArray);
			m_pRenderer->DrawPath(c_nStroke);
			m_pRenderer->PathCommandEnd();
		}

		RELEASEARRAY(pArray);

		return TRUE;
	}


	BOOL OnEmfPOLYLINETO16()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYLINETO16 *pRec = (EMRPOLYLINETO16*)m_pRecord;

		SAFEARRAY* pArray = ConvertPoints((LPPOINTS)pRec->apts, pRec->cpts);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandLinesTo(pArray);
		}
		else
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->PathCommandLinesTo(pArray);
			m_pRenderer->DrawPath(c_nStroke);
			m_pRenderer->PathCommandEnd();
		}

		RELEASEARRAY(pArray);

		return TRUE;
	}

	BOOL OnEmfPOLYPOLYLINE16()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYPOLYLINE16 *pRec = (EMRPOLYPOLYLINE16*)m_pRecord;

		SAFEARRAY* pArray = ConvertPoints((LPPOINTS)pRec->apts, pRec->cpts);

		if (m_bIsPath)
		{
			m_pRenderer->PathCommandLinesTo(pArray);
		}
		else
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->PathCommandLinesTo(pArray);
			m_pRenderer->DrawPath(c_nStroke);
			m_pRenderer->PathCommandEnd();
		}

		RELEASEARRAY(pArray);

		return TRUE;
	}

	BOOL OnEmfPOLYPOLYGON16()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYPOLYGON16 *pRec = (EMRPOLYPOLYGON16*)m_pRecord;
		//LPPOINTS pPts = (LPPOINTS)((DWORD*)pRec->aPolyCounts + pRec->nPolys);
		//m_pRenderer->SCDrawPolyPolygonS(pPts, pRec->cpts, (DWORD*)pRec->aPolyCounts, pRec->nPolys);

		return TRUE;
	}

	BOOL OnEmfPOLYDRAW16()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYDRAW16 *pRec = (EMRPOLYDRAW16*)m_pRecord;

		//BYTE* pTypes = (BYTE*)((POINTS*)pRec->apts + pRec->cpts);
		//m_pRenderer->SCDrawPolyDrawS(pRec->apts,
		//							pRec->cpts,
		//							pTypes);

		return TRUE;
	}

	BOOL OnEmfCREATEMONOBRUSH()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		EMRCREATEMONOBRUSH *pRec = (EMRCREATEMONOBRUSH*)m_pRecord;

		//// SCPlayRecord() won't work: the brush will reference a deleted bitmap (XP specific bug?).
		//// See comments in the final block.
		//SCPlayRecord();

		//#ifndef DIB_PAL_INDICES
		//	#define DIB_PAL_INDICES	2
		//#endif
		//ASSERT(DIB_PAL_INDICES==pRec->iUsage);

		//// DIB_PAL_INDICES management. Special case: the DIB has no color table.
		//BITMAPINFO *pBmi    = (BITMAPINFO *) ((BYTE *)pRec + pRec->offBmi); 
		//DWORD *pBitsDW     = (DWORD *) ((BYTE *) pRec + pRec->offBits);
		//// Attach a monochrome palette to the DIB specification,
		//// and create a device-dependent bitmap
		//HBITMAP hBm = NULL;
		//DWORD dwSize = pBmi->bmiHeader.biSize + 2*sizeof(RGBQUAD);
		//BITMAPINFO* pBmi2 = (BITMAPINFO*) new BYTE[dwSize];
		//memmove(pBmi2, pBmi, dwSize);
		//
		//HDC hMonoDC = CreateCompatibleDC(m_hPlayDC); // must be monochrome
		//ASSERT(hMonoDC);
		//SCFillMonochromePalette(hMonoDC, (PPALETTEENTRY)pBmi2->bmiColors);

		//// May create resource leak, as we don't call DeleteObject on hBm (see comments in the final block)
		//hBm = CreateDIBitmap(hMonoDC, &pBmi2->bmiHeader, CBM_INIT, pBitsDW, pBmi2, DIB_RGB_COLORS);
		//
		//DeleteDC(hMonoDC);
		//delete [] (BYTE*)pBmi2;
		//ASSERT(hBm);

		//HBRUSH hBrush = CreatePatternBrush(hBm);
		//ASSERT(hBrush);
		//m_lpEnumHandleTable->objectHandle[pRec->ihBrush] = hBrush;

		return TRUE;
	}

	BOOL OnEmfCREATEDIBPATTERNBRUSHPT()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();
		
		//EMRCREATEDIBPATTERNBRUSHPT *pRec = (EMRCREATEDIBPATTERNBRUSHPT*)m_pRecord;
		//ASSERT(DIB_PAL_COLORS==pRec->iUsage || DIB_RGB_COLORS==pRec->iUsage);
		//BITMAPINFO *pBmi    = (BITMAPINFO *) ((BYTE *)pRec + pRec->offBmi);

		//// Note: On Windows 95 and Windows 98, "creating brushes from bitmaps or DIBs
		////	larger than 8x8 pixels is not supported". So we should restrict the pattern to 8x8.
		//// But what if the EMF was created on NT/2K/XP? It seems that GDI doesn't shrink the
		//// pattern in order to render the EMF OS-independently.

		//DWORD dwSize = pRec->cbBmi + pRec->cbBits;
		//// should agree with:
		//// BITMAPINFOHEADER* pBmih = &pBmi->bmiHeader;
		//// DWORD dwSize = pBmih->biSize +
		////						ColorTableSize(pBmih) +
		////						((pBmih->biSizeImage) ? pBmih->biSizeImage :
		////						abs(pBmih->biHeight)*WIDTHBYTES(pBmih->biWidth*pBmih->biBitCount));

		//// May create resource leak, as we don't call GlobalFree on hMem
		//// (in fact, this is the sole purpose of this code)
		//HGLOBAL hMem = GlobalAlloc(GPTR, dwSize);
		//BITMAPINFO* pBmi2 = (BITMAPINFO*)GlobalLock(hMem);
		//memmove(pBmi2, pBmi, dwSize);

		//HBRUSH hBrush = CreateDIBPatternBrushPt(pBmi2, pRec->iUsage);

		//GlobalUnlock(hMem);
		//ASSERT(hBrush);
		//m_lpEnumHandleTable->objectHandle[pRec->ihBrush] = hBrush;

		return TRUE;
	}

	BOOL OnEmfEXTCREATEPEN()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfPOLYTEXTOUTA()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYTEXTOUTA *pRec = (EMRPOLYTEXTOUTA*)m_pRecord;
		//for (int i=0; (i<pRec->cStrings); i++)
		//{
		//	SCEmfTextoutA((EMREXTTEXTOUTA*)pRec, &pRec->aemrtext[i]);
		//}

		return TRUE;
	}

	BOOL OnEmfPOLYTEXTOUTW()
	{
		IS_VALID(m_pRenderer)

		EMRPOLYTEXTOUTW *pRec = (EMRPOLYTEXTOUTW*)m_pRecord;
		for (int i=0; (i<pRec->cStrings); i++)
		{
			SCEmfTextoutW((EMREXTTEXTOUTW*)pRec, &pRec->aemrtext[i]);
		}

		return TRUE;
	}

	BOOL OnEmfSETICMMODE()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfCREATECOLORSPACE()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfCREATECOLORSPACEW()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfSETCOLORSPACE()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfDELETECOLORSPACE()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfGLSRECORD()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfGLSBOUNDEDRECORD()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfPIXELFORMAT()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfDRAWESCAPE()
	{
		IS_VALID(m_pRenderer)

		// EMR *pRec = (EMR*)m_pRecord;

		return TRUE;
	}

	BOOL OnEmfEXTESCAPE()
	{
		IS_VALID(m_pRenderer)

		// EMR *pRec = (EMR*)m_pRecord;

		return TRUE;
	}

	BOOL OnEmfSTARTDOC()
	{
		IS_VALID(m_pRenderer)

		// EMR *pRec = (EMR*)m_pRecord;

		return TRUE;
	}

	BOOL OnEmfSMALLTEXTOUT()
	{
		IS_VALID(m_pRenderer)

		//// WARNING: it's SCEMF_RESERVED_108
		//SCEMRSMALLTEXTOUTA *pRec = (SCEMRSMALLTEXTOUTA*)m_pRecord;
		//// TODO: review this line
		//SCEMRSMALLTEXTOUTA2 *pTextRec2 = (SCEMRSMALLTEXTOUTA2*)m_pRecord;

		//// get the EMRTEXT
		//SCEMRSMALLTEXTOUTA *pTextRec = pRec;

		//// number of characters	
		//INT iNbChars = pTextRec->nChars;
		//if (iNbChars<=0)
		//{
		//	// We don't know the behavior of ExtTextOut:
		//	// - if number of characters is 0
		//	// - if, moreover, TA_UPDATECP is set in the DC
		//	return TRUE;
		//}

		//UINT uiOptions = pRec->fOptions; // we might have to alter it
		//// TODO: review this line
		//BOOL bUnicode = (uiOptions == 0x100);

		//// Build the string
		//LPWSTR pwStr = NULL;
		//if (bUnicode) 
		//{
		//	// The string is in unicode -> we let it as is.
		//	// Offset is from the beginning of the struct and is in bytes.
		//	// 	
		//	// Get the unicode string
		//	pwStr = (LPWSTR)pTextRec->Text;
		//} 
		//else
		//{
		//	// The string is single-byte encoded.
		//	// Get adress of the string	then construct the string.			
		//	LPSTR pString = (LPSTR)pTextRec->Text;
		//	// TODO: review this line
		//	if (uiOptions==0x204)
		//		pString = (char *)pTextRec2->Text;

		//	pwStr = new wchar_t[iNbChars+1];
		//	if (pwStr)
		//	{// We will use UNICODE strings exclusively
		//		pwStr[iNbChars] = 0;
		//		int iNum = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pString, iNbChars, pwStr, iNbChars+1);
		//		ASSERT(iNum==iNbChars);
		//	}
		//}
		//ASSERT(pwStr);

		//// Get intercharacter spacing array.
		//// Be careful ! there are cases where OffDx==0.
		//// TODO: review this line
		//DWORD OffDx = 0;//pTextRec->offDx;
		//INT *lpDx = (INT*)((BYTE*)pRec + OffDx);
		////if (0==OffDx)
		//{
		//	// If the lpDx parameter is NULL, the ExtTextOut function uses the default
		//	// spacing between characters. So we do.
		//	//
		//	// Compute intercharacter spacing, based on each character width.
		//	lpDx = new INT[iNbChars];
		//	BOOL bOk = SCGetTextCharWidthsW(m_hPlayDC, pwStr, iNbChars, lpDx);
		//	ASSERT(bOk);
		//} // else use distances between origins of adjacent character cells as passed to ExtTextOut.

		//CRect rcText;
		//if (uiOptions==0x204)
		//{
		//	rcText.CopyRect((LPRECT)&pTextRec2->rclBoundsSC);
		//} else
		//{
		//	uiOptions &= ~ETO_CLIPPED; // we don't have the rectangle
		//	rcText.SetRectEmpty();
		//}
		//if (GM_COMPATIBLE==pTextRec->iGraphicsMode)
		//	m_pRenderer->SCDrawText(pTextRec->ptlReference.x, pTextRec->ptlReference.y, uiOptions,
		//	(LPRECT)&rcText, pwStr, iNbChars, lpDx, pRec->exScale, pRec->eyScale);
		//else
		//	m_pRenderer->SCDrawText(pTextRec->ptlReference.x, pTextRec->ptlReference.y, uiOptions,
		//	(LPRECT)&rcText, pwStr, iNbChars, lpDx, 1, 1);
		//

		//// Clean up
		//if (!bUnicode)
		//	delete [] pwStr;
		//if (0==OffDx)
		//	delete [] lpDx;

		return TRUE;
	}

	BOOL OnEmfFORCEUFIMAPPING()
	{
		IS_VALID(m_pRenderer)

		// EMR *pRec = (EMR*)m_pRecord;

		return TRUE;
	}

	BOOL OnEmfNAMEDESCAPE()
	{
		IS_VALID(m_pRenderer)

		return TRUE;
	}

	BOOL OnEmfCOLORCORRECTPALETTE()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfSETICMPROFILEA()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfSETICMPROFILEW()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfALPHABLEND()
	{
		IS_VALID(m_pRenderer)

		EMRALPHABLEND *pRec = (EMRALPHABLEND*)m_pRecord;

		//ASSERT(pRec->offBmiSrc && pRec->offBitsSrc);

		//// Check if there is a bitmap
		//if (pRec->offBmiSrc && pRec->offBitsSrc)
		//{
		//	// Source
		//	CRect RcSrc(pRec->xSrc, pRec->ySrc, pRec->xSrc + pRec->cxSrc, pRec->ySrc + pRec->cySrc);
		//	BITMAPINFO *pBmi    = (BITMAPINFO *) ((BYTE *)pRec + pRec->offBmiSrc); 
		//	BYTE *pBits     = (BYTE *) ((BYTE *) pRec + pRec->offBitsSrc);

		//	// Destination
		//	CRect RcDest(pRec->xDest, pRec->yDest, pRec->xDest + pRec->cxDest, pRec->yDest + pRec->cyDest);

		//	m_pRenderer->SCDrawImageAlpha(&RcDest, &RcSrc, pBits, pBmi, pRec->iUsageSrc,
		//		pRec->dwRop, &pRec->xformSrc, pRec->crBkColorSrc);
		//}

		return TRUE;
	}

	BOOL OnEmfALPHADIBBLEND()
	{
		IS_VALID(m_pRenderer)

		// Now it's SCEMF_SETLAYOUT
		//DWORD SetLayout(
		//	HDC hdc,             // handle to DC
		//	DWORD dwLayout,      // layout options
		//	);
		// dwLayout==LAYOUT_BITMAPORIENTATIONPRESERVED Disables any reflection during BitBlt
		//											   and StretchBlt operations. 
		//           LAYOUT_RTL						   Sets the default horizontal layout
		//											   to be right to left. 
		// OSes: Windows 2000 and later, Windows 98 and later

		// TODO_EMF: horizontal reflection when the LAYOUT_RTL flag is selected
		// m_pRenderer->SCSetLayout(((EMRSETLAYOUT*)m_pRecord)->iMode);

		return TRUE;
	}

	BOOL OnEmfTRANSPARENTBLT()
	{
		IS_VALID(m_pRenderer)

		EMRTRANSPARENTBLT *pRec = (EMRTRANSPARENTBLT*)m_pRecord;
		//ASSERT(pRec->offBmiSrc && pRec->offBitsSrc);

		//// Check if there is a bitmap
		//if (pRec->offBmiSrc && pRec->offBitsSrc)
		//{
		//	// Source
		//	CRect RcSrc(pRec->xSrc, pRec->ySrc, pRec->xSrc + pRec->cxSrc, pRec->ySrc + pRec->cySrc);
		//	BITMAPINFO *pBmi    = (BITMAPINFO *) ((BYTE *)pRec + pRec->offBmiSrc); 
		//	BYTE *pBits     = (BYTE *) ((BYTE *) pRec + pRec->offBitsSrc);

		//	// Destination
		//	CRect RcDest(pRec->xDest, pRec->yDest, pRec->xDest + pRec->cxDest, pRec->yDest + pRec->cyDest);

		//	m_pRenderer->SCDrawImageTransparent(&RcDest, &RcSrc, pBits, pBmi, pRec->iUsageSrc,
		//		pRec->dwRop, &pRec->xformSrc, pRec->crBkColorSrc);
		//}

		return TRUE;
	}

	BOOL OnEmfTRANSPARENTDIB()
	{
		IS_VALID(m_pRenderer)

		// EMR *pRec = (EMR*)m_pRecord;

		return TRUE;
	}

	BOOL OnEmfGRADIENTFILL()
	{
		IS_VALID(m_pRenderer)

		EMRGRADIENTFILL *pRec = (EMRGRADIENTFILL*)m_pRecord;

		//LPBYTE pMesh = (LPBYTE)&pRec->Ver[pRec->nVer];
		//m_pRenderer->SCGradientFill(pRec->Ver, pRec->nVer, pMesh, pRec->nTri, pRec->ulMode);
		
		return TRUE;
	}

	BOOL OnEmfSETLINKEDUFIS()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}

	BOOL OnEmfSETTEXTJUSTIFICATION()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();		

		return TRUE;
	}

	BOOL OnEmfCOLORMATCHTOTARGETW()
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}


	BOOL OnBrkUNKRecord(long lMsg)
	{
		IS_VALID(m_pRenderer)

		PlayRecord();

		return TRUE;
	}
};