
#include "SvmFile.h"

#include <string>

#include "../Common/MetaFileTypes.h"
#include "../../../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"
#ifdef _DEBUG
	#include <iostream>
#endif

#include "SvmEnums.h"
#include "SvmObjects.h"

#define DEBUG_CSvmParser 0

namespace MetaFile
{

static const struct ActionNames 
{
    int     actionNumber;
    std::wstring actionName;
} actionNames[] = 
{
	{ META_NULL_ACTION,					 L"META_NULL_ACTION" },
    { META_PIXEL_ACTION,                 L"META_PIXEL_ACTION" },
    { META_POINT_ACTION,                 L"META_POINT_ACTION" },
    { META_LINE_ACTION,                  L"META_LINE_ACTION" },
    { META_RECT_ACTION,                  L"META_RECT_ACTION" },
    { META_ROUNDRECT_ACTION,             L"META_ROUNDRECT_ACTION" },
    { META_ELLIPSE_ACTION,               L"META_ELLIPSE_ACTION" },
    { META_ARC_ACTION,                   L"META_ARC_ACTION" },
    { META_PIE_ACTION,                   L"META_PIE_ACTION" },
    { META_CHORD_ACTION,                 L"META_CHORD_ACTION" },
    { META_POLYLINE_ACTION,              L"META_POLYLINE_ACTION" },
    { META_POLYGON_ACTION,               L"META_POLYGON_ACTION" },
    { META_POLYPOLYGON_ACTION,           L"META_POLYPOLYGON_ACTION" },
    { META_TEXT_ACTION,                  L"META_TEXT_ACTION" },
    { META_TEXTARRAY_ACTION,             L"META_TEXTARRAY_ACTION" },
    { META_STRETCHTEXT_ACTION,           L"META_STRETCHTEXT_ACTION" },
    { META_TEXTRECT_ACTION,              L"META_TEXTRECT_ACTION" },
    { META_BMP_ACTION,                   L"META_BMP_ACTION" },
    { META_BMPSCALE_ACTION,              L"META_BMPSCALE_ACTION" },
    { META_BMPSCALEPART_ACTION,          L"META_BMPSCALEPART_ACTION" },
    { META_BMPEX_ACTION,                 L"META_BMPEX_ACTION" },
    { META_BMPEXSCALE_ACTION,            L"META_BMPEXSCALE_ACTION" },
    { META_BMPEXSCALEPART_ACTION,        L"META_BMPEXSCALEPART_ACTION" },
    { META_MASK_ACTION,                  L"META_MASK_ACTION" },
    { META_MASKSCALE_ACTION,             L"META_MASKSCALE_ACTION" },
    { META_MASKSCALEPART_ACTION,         L"META_MASKSCALEPART_ACTION" },
    { META_GRADIENT_ACTION,              L"META_GRADIENT_ACTION" },
    { META_HATCH_ACTION,                 L"META_HATCH_ACTION" },
    { META_WALLPAPER_ACTION,             L"META_WALLPAPER_ACTION" },
    { META_CLIPREGION_ACTION,            L"META_CLIPREGION_ACTION" },
    { META_ISECTRECTCLIPREGION_ACTION,   L"META_ISECTRECTCLIPREGION_ACTION" },
    { META_ISECTREGIONCLIPREGION_ACTION, L"META_ISECTREGIONCLIPREGION_ACTION" },
    { META_MOVECLIPREGION_ACTION,        L"META_MOVECLIPREGION_ACTION" },
    { META_LINECOLOR_ACTION,             L"META_LINECOLOR_ACTION" },
    { META_FILLCOLOR_ACTION,             L"META_FILLCOLOR_ACTION" },
    { META_TEXTCOLOR_ACTION,             L"META_TEXTCOLOR_ACTION" },
    { META_TEXTFILLCOLOR_ACTION,         L"META_TEXTFILLCOLOR_ACTION" },
    { META_TEXTALIGN_ACTION,             L"META_TEXTALIGN_ACTION" },
    { META_MAPMODE_ACTION,               L"META_MAPMODE_ACTION" },
    { META_FONT_ACTION,                  L"META_FONT_ACTION" },
    { META_PUSH_ACTION,                  L"META_PUSH_ACTION" },
    { META_POP_ACTION,                   L"META_POP_ACTION" },
    { META_RASTEROP_ACTION,              L"META_RASTEROP_ACTION" },
    { META_TRANSPARENT_ACTION,           L"META_TRANSPARENT_ACTION" },
    { META_EPS_ACTION,                   L"META_EPS_ACTION" },
    { META_REFPOINT_ACTION,              L"META_REFPOINT_ACTION" },
    { META_TEXTLINECOLOR_ACTION,         L"META_TEXTLINECOLOR_ACTION" },
    { META_TEXTLINE_ACTION,              L"META_TEXTLINE_ACTION" },
    { META_FLOATTRANSPARENT_ACTION,      L"META_FLOATTRANSPARENT_ACTION" },
    { META_GRADIENTEX_ACTION,            L"META_GRADIENTEX_ACTION" },
    { META_LAYOUTMODE_ACTION,            L"META_LAYOUTMODE_ACTION" },
    { META_TEXTLANGUAGE_ACTION,          L"META_TEXTLANGUAGE_ACTION" },
    { META_OVERLINECOLOR_ACTION,         L"META_OVERLINECOLOR_ACTION" },
    { META_RENDERGRAPHIC_ACTION,         L"META_RENDERGRAPHIC_ACTION" },
    { META_COMMENT_ACTION,               L"META_COMMENT_ACTION" }
};

void CSvmFile::PlayMetaFile()
{
	m_oStream.SeekToStart();

    // Check the signature "VCLMTF"
	std::string start = std::string((char*)m_oStream.GetCurPtr(), 6);

    if (start != "VCLMTF")    return;
	m_oStream.Skip(6);
	
	Read_SVM_HEADER();

	if (m_pOutput)
		m_pOutput->Begin();

	for (unsigned int action = 0; action < m_oHeader.actionCount; ++action)
	{
        unsigned short  actionType;

		m_oStream >> actionType;

		m_oStream >> m_currentActionVersion;
		m_oStream >> m_unRecordSize;

        m_unRecordPos = m_oStream.Tell();

        switch (actionType) 
		{
			case META_RECT_ACTION:			Read_META_RECTANGLE();		break;
			case META_POLYLINE_ACTION:		Read_META_POLYLINE();       break;
			case META_POLYGON_ACTION:		Read_META_POLYGON();        break;
			case META_POLYPOLYGON_ACTION:	Read_META_POLYPOLYGON();	break;
			
			case META_TEXT_ACTION:			Read_META_TEXT();			break;
			case META_TEXTARRAY_ACTION:		Read_META_ARRAYTEXT();		break;
			case META_MAPMODE_ACTION:		Read_META_SETMAPMODE();		break;
			case META_FONT_ACTION:			Read_META_FONT();		break;
			case META_BMPSCALE_ACTION:		Read_META_BMPSCALE();		break;

			case META_LINECOLOR_ACTION:		Read_META_SETLINECOLOR();	break;
			case META_FILLCOLOR_ACTION:		Read_META_SETFILLCOLOR();	break;
			case META_TEXTFILLCOLOR_ACTION:
			case META_TEXTCOLOR_ACTION:		Read_META_SETTEXTCOLOR();	break;
			case META_GRADIENT_ACTION:		Read_META_GRADIENT();		break;
			case META_GRADIENTEX_ACTION:	Read_META_GRADIENTEX();		break;
			case META_HATCH_ACTION:
		
			case META_RASTEROP_ACTION:		Read_META_RASTEROP();		break;			
			case META_PUSH_ACTION:			Read_META_PUSH();			break;		
			case META_POP_ACTION:			Read_META_POP();			break;		

			case META_TRANSPARENT_ACTION:	Read_META_TRANSPARENT();			break;	

			case META_ROUNDRECT_ACTION:
			case META_ELLIPSE_ACTION:
			case META_ARC_ACTION:
			case META_PIE_ACTION:
			case META_CHORD_ACTION:
			case META_PIXEL_ACTION:
			case META_POINT_ACTION:
			case META_LINE_ACTION:

			case META_STRETCHTEXT_ACTION:
			case META_TEXTRECT_ACTION:
			case META_BMP_ACTION:
			case META_BMPSCALEPART_ACTION:
			case META_BMPEX_ACTION:
			case META_BMPEXSCALE_ACTION:
			case META_BMPEXSCALEPART_ACTION:
			case META_MASK_ACTION:
			case META_MASKSCALE_ACTION:
			case META_MASKSCALEPART_ACTION:
			case META_WALLPAPER_ACTION:
			case META_CLIPREGION_ACTION:
			case META_ISECTRECTCLIPREGION_ACTION:
			case META_ISECTREGIONCLIPREGION_ACTION:
			case META_MOVECLIPREGION_ACTION:
			case META_TEXTALIGN_ACTION:
			case META_EPS_ACTION:
			case META_REFPOINT_ACTION:
			case META_TEXTLINECOLOR_ACTION:
			case META_TEXTLINE_ACTION:
			case META_FLOATTRANSPARENT_ACTION:
			case META_LAYOUTMODE_ACTION:
			case META_TEXTLANGUAGE_ACTION:
			case META_OVERLINECOLOR_ACTION:
			case META_RENDERGRAPHIC_ACTION:				
			case META_COMMENT_ACTION:

			case META_NULL_ACTION:
			default:
#ifdef _DEBUG
			if (actionType != 512 && m_unRecordSize > 0)
			{
				std::wstring name;
				if (actionType == 0)
					name = actionNames[0].actionName;
				else if (actionType == 512)
					name = L"META_COMMENT_ACTION";            
				else if (100 <= actionType && actionType <= META_LAST_ACTION)
					name = actionNames[actionType - 99].actionName;
				else
					name = L"(out of bounds)";

				std::wcout << name << L"\t(" << actionType << L") " << L"\tversion = " << m_currentActionVersion << L"\t; totalSize = " << m_unRecordSize << L"\n";
			}			
#endif
			break;
        }

		m_ñurrentActionType = actionType;

		int need_skip = m_unRecordSize - (m_oStream.Tell() - m_unRecordPos);
		m_oStream.Skip(need_skip);
    }
	if (m_pOutput)
		m_pOutput->End();
}


void CSvmFile::Read_META_RECTANGLE()
{
	TSvmRect oBox;
	m_oStream >> oBox;
	{
		MoveTo(oBox.l, oBox.t);
		LineTo(oBox.r, oBox.t);
		LineTo(oBox.r, oBox.b);
		LineTo(oBox.l, oBox.b);
	}
	ClosePath();
	DrawPath(true, true);
}
void CSvmFile::Read_SVM_HEADER()
{
	m_oStream >> m_oHeader;

	//m_pDC->SetWindowExt (	m_oHeader.boundRect.nRight	- m_oHeader.boundRect.nLeft,
	//						m_oHeader.boundRect.nBottom - m_oHeader.boundRect.nTop);

	//m_pDC->SetWindowScale(	(double)m_oHeader.mapMode.scaleX.numerator/m_oHeader.mapMode.scaleX.denominator, 
	//						(double)m_oHeader.mapMode.scaleY.numerator/m_oHeader.mapMode.scaleY.denominator);
	//
	//if (m_pOutput)
	//{
	//	TRect oRect = m_oHeader.boundRect;// GetBoundingBox();
	//	m_pDC->SetWindowOff(oRect.nLeft, oRect.nTop);
	//	m_pDC->SetWindowExt(oRect.nRight - oRect.nLeft, oRect.nBottom - oRect.nTop);
	//}
	//else
	{
		m_bFirstPoint = true;
	}
}
void CSvmFile::Read_META_POLYLINE()
{
	TSvmPolygon polygon;
	m_oStream >> polygon;

	if (m_currentActionVersion >= 2)
	{
		TSvmLineInfo line_info;
		m_oStream >> line_info;

		CSvmPen *last_pen = dynamic_cast<CSvmPen *>(m_oPlayer.GetLastObject(SVM_OBJECT_PEN));
		if (last_pen)
		{
			last_pen->Width = line_info.width;

			switch(line_info.style)
			{
				case LINE_SOLID:	last_pen->PenStyle = PS_SOLID ; break;
				case LINE_DASH:		last_pen->PenStyle = PS_DASH ; break;
			}
		}
	}

	if (m_currentActionVersion >= 3)
	{
	}

	if (polygon.count < 1) return;

	MoveTo(polygon.points[0].x, polygon.points[0].y);

	for (int i = 1; i < polygon.count; i++)
	{
		LineTo(polygon.points[i].x, polygon.points[i].y);
	}
	ClosePath();
	DrawPath(true, true);
}

void CSvmFile::Read_META_POLYGON()
{
	TSvmPolygon polygon;

	m_oStream >> polygon;

	if (polygon.count < 1) return;

	MoveTo(polygon.points[0].x, polygon.points[0].y);

	for (int i = 1; i < polygon.count; i++)
	{
		LineTo(polygon.points[i].x, polygon.points[i].y);
	}
	ClosePath();
	DrawPath(true, true);
}

void CSvmFile::Read_META_POLYPOLYGON(std::vector<TSvmPolygon> & polygons)
{
	unsigned short ushNumberOfPolygons;
	m_oStream >> ushNumberOfPolygons;
	if (ushNumberOfPolygons <= 0)
		return;

	for (unsigned short ushIndex = 0; ushIndex < ushNumberOfPolygons; ushIndex++)
	{
		TSvmPolygon poligon;

		m_oStream >> poligon;
		polygons.push_back(poligon);
	}

	std::vector<TSvmPolygon> complexPolygons;

	if (m_currentActionVersion > 1)
	{
		unsigned short complexPolygonCount;
		m_oStream >> complexPolygonCount;

		complexPolygons.resize(complexPolygonCount);

		for (unsigned short i = 0; i < complexPolygonCount; i++)
		{
			unsigned short complexPolygonIndex;
			m_oStream >> complexPolygonIndex;

			m_oStream >> complexPolygons[complexPolygonIndex];
		}
	}
}
void CSvmFile::Read_META_POLYPOLYGON()
{
	std::vector<TSvmPolygon> polygons;
	Read_META_POLYPOLYGON(polygons);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (unsigned short ushPolygonIndex = 0; ushPolygonIndex < polygons.size(); ushPolygonIndex++)
	{
		unsigned short ushPointsCount = polygons[ushPolygonIndex].count;

		if (ushPointsCount <= 0)
			continue;

		MoveTo(polygons[ushPolygonIndex].points[0].x, polygons[ushPolygonIndex].points[0].y);
		for (int i = 1; i < ushPointsCount; i++)
		{
			LineTo(polygons[ushPolygonIndex].points[i].x, polygons[ushPolygonIndex].points[i].y);
		}
		ClosePath();
	}
	//for (unsigned short ushPolygonIndex = 0; ushPolygonIndex < ushNumberOfPolygons; ushPolygonIndex++)
	//{
	//	unsigned short ushPointsCount = complexPolygons[ushPolygonIndex].count;

	//	if (ushPointsCount <= 0)
	//		continue;

	//	MoveTo(complexPolygons[ushPolygonIndex].points[0].x, complexPolygons[ushPolygonIndex].points[0].y);
	//	for (int i = 1; i < ushPointsCount; i++)
	//	{
	//		LineTo(complexPolygons[ushPolygonIndex].points[i].x, complexPolygons[ushPolygonIndex].points[i].y);
	//	}
	//	ClosePath();
	//}
	DrawPath(true, true);
}

void CSvmFile::Read_META_SETMAPMODE()
{
	MapMode mapMode;
	m_oStream >> mapMode;
	
	m_pDC->SetMapMode(mapMode);

	UpdateOutputDC();
}
void CSvmFile::Read_META_TEXT()
{
	std::wstring sText;
	TSvmPoint   startPoint;

	m_oStream >> startPoint;

	parseString(m_oStream, sText, m_currentActionVersion, m_currentCharset);
	
	//unsigned short index, len;
	//	
	//m_oStream	>> index;
	//m_oStream	>> len;

	//if (m_currentActionVersion > 1)
	//{
	//	unsigned short nLen;
	//	m_oStream >> nLen;
	//	
	//	std::wstring buf;
	//	unsigned short nTemp;
	//	while ( nLen-- )
	//	{
	//		m_oStream >> nTemp;
	//		buf += (wchar_t)nTemp;
	//	}
	//	sText = buf;
	//}
	DrawText(sText, sText.length(), startPoint.x, startPoint.y);
}
void CSvmFile::Read_META_ARRAYTEXT()
{
	std::wstring sText;
	TSvmPoint   startPoint;

	m_oStream >> startPoint;

	parseString(m_oStream, sText, m_currentActionVersion, m_currentCharset);
	
	unsigned short nIndex, nLen;
	unsigned int nArrayLen;
		
	m_oStream >> nIndex;
	m_oStream >> nLen;

	m_oStream >> nArrayLen;

	if( nArrayLen > 0 )
	{
        // #i9762#, #106172# Ensure that DX array is at least mnLen entries long
		const unsigned int nIntAryLen = (std::max)(nArrayLen, (unsigned int)nLen) ;
		int *mpDXAry = new int[ nIntAryLen ];
        
        int i=0;
		for( i = 0; i < nArrayLen; i++ )
			m_oStream >> mpDXAry[ i ];

        // #106172# setup remainder
		for( ; i < nIntAryLen; i++ )
            mpDXAry[ i ] = 0;
	}

	if ( m_currentActionVersion >= 2 )							// Version 2
	{
		unsigned short nLen;
		m_oStream >> nLen;

		std::wstring tempBuffer;
		unsigned short nTemp;
		while ( nLen-- )
		{
			m_oStream >> nTemp;
			tempBuffer += (wchar_t)nTemp;
		}
	}
	DrawText(sText, sText.length(), startPoint.x, startPoint.y);
}
void CSvmFile::Read_META_SETTEXTCOLOR()
{
	TSvmColor oColor; 
	m_oStream >> oColor;
	m_pDC->SetTextColor(oColor);
	UpdateOutputDC();
}
void CSvmFile::Read_META_SETFILLCOLOR()
{
	if (m_ñurrentActionType == META_GRADIENT_ACTION ||
		m_ñurrentActionType == META_GRADIENTEX_ACTION)
	{
		return;
	}

	CSvmBrush* pBrush = new CSvmBrush();
	pBrush->BrushStyle = BS_NULL;
	if (!pBrush)
		return SetError();		
	
	bool doSet;
	m_oStream >> pBrush->Color;
	m_oStream >> doSet;

	if (doSet)
	{
		pBrush->BrushStyle = BS_SOLID; 
	}
	m_oPlayer.RegisterObject((CSvmObjectBase*)pBrush);
}

void CSvmFile::Read_META_SETLINECOLOR()
{
	CSvmPen* pPen = new CSvmPen();
	if (!pPen)
		return SetError();
	pPen->PenStyle = PS_NULL;

	bool     doSet;

	m_oStream >> pPen->Color;
	m_oStream >> doSet;
	
	if (doSet)
	{
		pPen->PenStyle = PS_COSMETIC | PS_SOLID;
	}
	m_oPlayer.RegisterObject((CSvmObjectBase*)pPen);
}

void CSvmFile::Read_META_GRADIENTEX()
{
	std::vector<TSvmPolygon> polygons;
	Read_META_POLYPOLYGON(polygons);

	CSvmBrush* pBrush = new CSvmBrush();
	if (!pBrush)
		return SetError();
	pBrush->BrushStyle	= BS_PATHGRADIENT; 

	unsigned short nTmp16;

    m_oStream >> nTmp16; 
	ESvmGradientStyle gradientStyle = (ESvmGradientStyle) nTmp16;

	m_oStream >> pBrush->Color;
	m_oStream >> pBrush->Color2;

	m_oStream >> pBrush->BrushStyleEx;
    m_oStream >> nTmp16; //Border 
    m_oStream >> nTmp16; //OfsX 
    m_oStream >> nTmp16; //OfsY 
	m_oStream >> nTmp16; //IntensityStart 
    m_oStream >> nTmp16; //IntensityEnd 
    m_oStream >> nTmp16; //StepCount;	

	m_oPlayer.RegisterObject((CSvmObjectBase*)pBrush);
}

void CSvmFile::Read_META_TRANSPARENT()
{
	std::vector<TSvmPolygon> polygons;
	Read_META_POLYPOLYGON(polygons);

	unsigned short nPercent;

	 m_oStream >> nPercent;

	 if (m_pOutput)
	 {
		CSvmBrush *last_brush = dynamic_cast<CSvmBrush *>(m_oPlayer.GetLastObject(SVM_OBJECT_BRUSH));
		
		CSvmBrush* pBrush = new CSvmBrush();
		pBrush->BrushStyle = BS_NULL;
		if (!pBrush)
			return SetError();		
		
		if (last_brush)
		{
			pBrush->BrushStyle = last_brush->BrushStyle; 
			pBrush->BrushHatch = last_brush->BrushHatch; 
			pBrush->BrushStyleEx = last_brush->BrushStyleEx; 
			pBrush->BrushBounds = last_brush->BrushBounds; 
			pBrush->Color = last_brush->Color; 
			pBrush->Color2 = last_brush->Color2; 

			pBrush->Color.a = 0xff * nPercent /100;
			pBrush->Color2.a = 0xff * nPercent /100;
		}
		m_oPlayer.RegisterObject((CSvmObjectBase*)pBrush);
	
		m_pOutput->StartPath();
		
		for (unsigned short ushPolygonIndex = 0; ushPolygonIndex < polygons.size(); ushPolygonIndex++)
		{
			unsigned short ushPointsCount = polygons[ushPolygonIndex].count;

			if (ushPointsCount <= 0)
				continue;

			MoveTo(polygons[ushPolygonIndex].points[0].x, polygons[ushPolygonIndex].points[0].y);
			for (int i = 1; i < ushPointsCount; i++)
			{
				LineTo(polygons[ushPolygonIndex].points[i].x, polygons[ushPolygonIndex].points[i].y);
			}

			m_pOutput->ClosePath();
		}
	 }

	DrawPath(false, true);
}

void CSvmFile::Read_META_GRADIENT()
{
	CSvmBrush* pBrush = new CSvmBrush();
	if (!pBrush)
		return SetError();
	pBrush->BrushStyle	= BS_LINEARGRADIENT; 

	m_oStream >> pBrush->BrushBounds;
	
	unsigned short nTmp16;
    m_oStream >> nTmp16; 
	ESvmGradientStyle gradientStyle = (ESvmGradientStyle) nTmp16;

	m_oStream >> pBrush->Color;
	m_oStream >> pBrush->Color2;

	m_oStream >> pBrush->BrushStyleEx;
    m_oStream >> nTmp16; //Border 
    m_oStream >> nTmp16; //OfsX 
    m_oStream >> nTmp16; //OfsY 
	m_oStream >> nTmp16; //IntensityStart 
    m_oStream >> nTmp16; //IntensityEnd 
    m_oStream >> nTmp16; //StepCount;	
	
	m_oPlayer.RegisterObject((CSvmObjectBase*)pBrush);

}
 
void CSvmFile::Read_META_FONT()
{
	CSvmFont* pFont = new CSvmFont();
	if (!pFont)
		return SetError();

	m_oStream >> pFont;
	m_currentCharset = pFont->CharSet;

	m_oPlayer.RegisterObject((CSvmObjectBase*)pFont);
}

#define COMPRESS_OWN				('S'|('D'<<8))
#define COMPRESS_NONE				( 0 )
#define RLE_8						( 1 )
#define RLE_4						( 2 )
#define BITFIELDS					( 3 )
#define ZCOMPRESS					( COMPRESS_OWN | 0x01000000 ) 

inline USHORT discretizeBitcount( int nInputCount )
{
    return ( nInputCount <= 1 ) ? 1 :
           ( nInputCount <= 4 ) ? 4 :
           ( nInputCount <= 8 ) ? 8 : 24;
}
bool CSvmFile::ReadImage(unsigned short ushColorUsage, BYTE** ppBgraBuffer, unsigned int* pulWidth, unsigned int* pulHeight)
{
	unsigned int unRemainBytes = m_unRecordSize - ( m_oStream.Tell() - m_unRecordPos);
	if (unRemainBytes <= 0)
		return false;

	BYTE* pBuffer = m_oStream.GetCurPtr();
	MetaFile::ReadImage(pBuffer, unRemainBytes, ushColorUsage, ppBgraBuffer, pulWidth, pulHeight);
	return true;
}
void CSvmFile::Read_META_POP()
{
	m_oPlayer.Pop();
}
void CSvmFile::Read_META_PUSH()
{
	unsigned short nFlags;
	m_oStream >> nFlags; 
	
	m_oPlayer.Push(nFlags);
}
void CSvmFile::Read_META_RASTEROP()
{
	unsigned short tmp;
	m_oStream >> tmp; 
	
	m_oPlayer.SetRasterOp(tmp);
	
}

void CSvmFile::Read_META_BMPSCALE()
{
	TSvmBitmapSize	size; 
	TSvmBitmapPoint	point; 

	m_oStream >> size;
	m_oStream >> point;

	unsigned int	rOffset = 0;
	
	unsigned int	nTmp32;
	unsigned short	nTmp16 = 0;
	bool			bRet = false;

	m_oStream >> nTmp16;
	m_oStream >> rOffset;

	if ( ( 0x4D42 == nTmp16 ) || ( 0x4142 == nTmp16 ) )
	{
		if ( 0x4142 == nTmp16 )
		{
			m_oStream.Skip( 12 );
			m_oStream >> nTmp16;
			m_oStream.Skip( 8 );
			m_oStream >> nTmp32;
			rOffset = nTmp32 - 28UL;;
			bRet = ( 0x4D42 == nTmp16 );
		}
		else
		{
			m_oStream.Skip( 8L );
			m_oStream >> nTmp32;
			rOffset = nTmp32 - 14UL;
		}
	}

	TSvmBitmap bitmap_info;

	m_oStream >> bitmap_info; //size = 40 
	
	unsigned short	nColors;
	BYTE*			pData = NULL;
	
	int nBitCount = discretizeBitcount(bitmap_info.nBitCount);

	if( nBitCount <= 8 )
	{
		if( bitmap_info.nColsUsed )
			nColors = (USHORT) bitmap_info.nColsUsed;
		else
			nColors = ( 1 << bitmap_info.nBitCount );
	}
	else
		nColors = 0;

	if( ZCOMPRESS == bitmap_info.nCompression && m_pOutput )
	{
		COfficeUtils OfficeUtils(NULL);
		
		ULONG destSize, srcSize;
		m_oStream >> srcSize >> destSize >> bitmap_info.nCompression;

		BYTE*		srcBuf = m_oStream.GetCurPtr();
		BYTE*		destBuf = new BYTE[destSize];

		if (destSize > 0 && m_pOutput && destBuf )
		{
			if (OfficeUtils.Uncompress(destBuf, &destSize, srcBuf, srcSize) == S_OK)
			{
				//memcpy(destBuf,&bitmap_info,40);
				//unsigned int ulWidth=0, ulHeight=0;
				//BYTE*		bufBGRA = NULL;
				//MetaFile::ReadImage(destBuf, destSize, 0, &bufBGRA, &ulWidth, &ulHeight);

				int size_BGRA = bitmap_info.nWidth * bitmap_info.nHeight * 4;

				BYTE*		bufBGRA = new BYTE [size_BGRA];
				if (bufBGRA)
				{
					BYTE* bufBGRA1 = bufBGRA;
					int  nCount = bitmap_info.nHeight;
					for( int j = bitmap_info.nHeight-1; j>=0; j--)
					{
						for( long i=0; i < bitmap_info.nWidth; i+=3 )
						{
							*bufBGRA1 = destBuf[j * bitmap_info.nWidth + i + 0];	bufBGRA1++;
							*bufBGRA1 = destBuf[j * bitmap_info.nWidth + i + 1];	bufBGRA1++;
							*bufBGRA1 = destBuf[j * bitmap_info.nWidth + i + 2];	bufBGRA1++;
							*bufBGRA1 = 0xff;										bufBGRA1++;
						}
					}
					//for ( int i=0, j=0 ; i < destSize, j < size_BGRA; i+=3, j+=4)
					//{
					//	bufBGRA[j+0] = destBuf[i+0];
					//	bufBGRA[j+1] = destBuf[i+1];
					//	bufBGRA[j+2] = destBuf[i+2];
					//	bufBGRA[j+3] = 0xff;
					//}
					double dX = point.x, dY = point.y, dX1 = point.x + size.cx, dY1 =point.y + size.cy;
					TranslatePoint(point.x, point.y, dX, dY);
					TranslatePoint(point.x + size.cx, point.y + size.cy, dX1, dY1);

					dX1 = dX1-dX;
					dY1 = dY1-dY;

					MapMode aMapMode;
					aMapMode.unit = MAP_PIXEL;//MAP_MM;
									  //Fraction( 1000, aHeader.nXPelsPerMeter )
									  //Fraction( 1000, aHeader.nYPelsPerMeter ) 

					m_pDC->SetMapMode(aMapMode);
					UpdateOutputDC();

					m_pOutput->DrawBitmap(dX, dY, dX1, dY1, bufBGRA, bitmap_info.nWidth, bitmap_info.nHeight);
					//m_pOutput->DrawBitmap(dX, dY, dX1, dY1, bufBGRA, ulWidth, ulHeight);
					delete []bufBGRA;
				}
			}
			delete []destBuf;
		}
	}
	UpdateOutputDC();
}
} // namespace MetaFile

