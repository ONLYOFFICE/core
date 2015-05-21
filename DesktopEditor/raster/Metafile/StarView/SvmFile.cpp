
// Own
#include "SvmFile.h"

#include <string>

#include "../Common/MetaFileTypes.h"

#ifdef _DEBUG
	#include <iostream>
#endif
// MetaFile
#include "SvmEnums.h"
#include "SvmObjects.h"

// 0 - No debug
// 1 - Print a lot of debug info
// 2 - Just print all the records instead of parsing them
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
	BYTE* startPosBuffer = m_oStream.GetCurPtr();

    // Check the signature "VCLMTF"
	std::string start = std::string((char*)m_oStream.GetCurPtr(), 6);

    if (start != "VCLMTF")
        return;


    // Start reading from the stream: read past the signature and get the header.
	soakBytes(m_oStream, 6);
	Read_SVM_HEADER();

    //kDebug(31000) << "================ SVM HEADER ================";
    //kDebug(31000) << "version, length:" << header.versionCompat.version << header.versionCompat.length;
    //kDebug(31000) << "compressionMode:" << header.compressionMode;
    //kDebug(31000) << "mapMode:" << "Origin" << header.mapMode.origin
                  //<< "scaleX"
                  //<< header.mapMode.scaleX.numerator << header.mapMode.scaleX.numerator
                  //<< (qreal(header.mapMode.scaleX.numerator) / header.mapMode.scaleX.numerator)
                  //<< "scaleY"
                  //<< header.mapMode.scaleY.numerator << header.mapMode.scaleY.numerator
                  //<< (qreal(header.mapMode.scaleX.numerator) / header.mapMode.scaleX.numerator);
    //kDebug(31000) << "size:" << header.width << header.height;
    //kDebug(31000) << "actionCount:" << header.actionCount;
    //kDebug(31000) << "================ SVM HEADER ================";  

	if (m_pOutput)
		m_pOutput->Begin();

#if DEBUG_CSvmParser
    {
        QPolygon polygon;
        polygon << QPoint(0, 0);
        polygon << QPoint(header.width, header.height);
        mBackend->polyLine(mContext, polygon);
    }
#endif

    // Parse all actions and call the appropriate backend callback for
    // the graphics drawing actions.  The context actions will
    // manipulate the graphics context, which is maintained here.
	for (unsigned int action = 0; action < m_oHeader.actionCount; ++action)
	{
        unsigned short  actionType;
        unsigned short  version;
        unsigned int	totalSize;

		m_oStream >> actionType;

		m_oStream >> version;
		m_oStream >> totalSize;

		m_ulRecordSize = totalSize;// - 8;

        int start_node_pos = m_oStream.GetCurPtr() - startPosBuffer;
        // Debug
#if _DEBUG
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

			std::wcout << name << L"\t(" << actionType << L") " << L"\tversion = " << version << L"\t; totalSize = " << totalSize << L"\n";
        }			
#endif

        // Parse all actions.
        switch (actionType) 
		{
			case META_RECT_ACTION:			Read_META_RECTANGLE();		break;
			case META_POLYLINE_ACTION:
			case META_POLYGON_ACTION:
				{
					unsigned int size = Read_META_POLYGON();		
					// FIXME: Version 2: Lineinfo, Version 3: polyflags
					if (version > 1)
						soakBytes(m_oStream, totalSize - 2 - 4 * 2 * size); //2 points * 4 byte * size polygon
				
				}break;
			case META_POLYPOLYGON_ACTION:	Read_META_POLYPOLYGON(version); break;

			case META_TEXT_ACTION:
			case META_TEXTARRAY_ACTION:		Read_META_TEXT_A();			break;
			case META_LINECOLOR_ACTION:		Read_META_SETLINECOLOR();	break;
			case META_FILLCOLOR_ACTION:		Read_META_SETFILLCOLOR();	break;
			case META_TEXTCOLOR_ACTION:		Read_META_SETTEXTCOLOR();	break;
			case META_MAPMODE_ACTION:		Read_META_SETMAPMODE();		break;
			case META_FONT_ACTION:			Read_META_CREATEFONT();		break;
			case META_PUSH_ACTION:
				{
					////kDebug(31000) << "Push action : " << totalSize;
					unsigned short pushValue;
					m_oStream >> pushValue;
					////kDebug(31000) << "Push value : " << pushValue;
				}break;
			case META_POP_ACTION:
				//{
				//		//kDebug(31000) << "Pop action : " << totalSize;
				//		/*unsigned short pushValue;
				//		m_oStream >> pushValue;
				//		//kDebug(31000) << "Push value : " << pushValue;*/
				//}break;
			case META_STRETCHTEXT_ACTION:
			case META_TEXTRECT_ACTION:
			case META_BMP_ACTION:
			case META_BMPSCALE_ACTION:
			case META_BMPSCALEPART_ACTION:
			case META_BMPEX_ACTION:
			case META_BMPEXSCALE_ACTION:
			case META_BMPEXSCALEPART_ACTION:
			case META_MASK_ACTION:
			case META_MASKSCALE_ACTION:
			case META_MASKSCALEPART_ACTION:
			case META_GRADIENT_ACTION:
			case META_HATCH_ACTION:
			case META_WALLPAPER_ACTION:
			case META_CLIPREGION_ACTION:
			case META_ISECTRECTCLIPREGION_ACTION:
			case META_ISECTREGIONCLIPREGION_ACTION:
			case META_MOVECLIPREGION_ACTION:
			case META_TEXTFILLCOLOR_ACTION:
			case META_TEXTALIGN_ACTION:
			case META_RASTEROP_ACTION:
			case META_TRANSPARENT_ACTION:
			case META_EPS_ACTION:
			case META_REFPOINT_ACTION:
			case META_TEXTLINECOLOR_ACTION:
			case META_TEXTLINE_ACTION:
			case META_FLOATTRANSPARENT_ACTION:
			case META_GRADIENTEX_ACTION:
			case META_LAYOUTMODE_ACTION:
			case META_TEXTLANGUAGE_ACTION:
			case META_OVERLINECOLOR_ACTION:
			case META_RENDERGRAPHIC_ACTION:			//dumpAction(m_oStream, version, totalSize);				
			case META_COMMENT_ACTION:
			case META_ROUNDRECT_ACTION:
			case META_ELLIPSE_ACTION:
			case META_ARC_ACTION:
			case META_PIE_ACTION:
			case META_CHORD_ACTION:
			case META_NULL_ACTION:
			case META_PIXEL_ACTION:
			case META_POINT_ACTION:
			case META_LINE_ACTION:
			default:
				break;
        }

		int cur_pos = m_oStream.GetCurPtr() - startPosBuffer;
		int need_skip = totalSize - (cur_pos - start_node_pos);
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
}
unsigned int CSvmFile::Read_META_POLYGON()
{
	TSvmPolygon polygon;

	m_oStream >> polygon;

	if (polygon.count < 1) return 0;

	MoveTo(polygon.points[0].x, polygon.points[0].y);

	for (int i = 1; i < polygon.count; i++)
	{
		LineTo(polygon.points[i].x, polygon.points[i].y);
	}
	ClosePath();
	DrawPath(true, true);

	return polygon.count;
}

void CSvmFile::Read_META_POLYPOLYGON(int version)
{
	std::vector<TSvmPolygon> polygons;

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
	//todooo ??? тута нуно отрисовывать ???
	
	std::vector<TSvmPolygon> complexPolygons;
	if (version > 1)
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (unsigned short ushPolygonIndex = 0; ushPolygonIndex < ushNumberOfPolygons; ushPolygonIndex++)
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
	////kDebug(31000) << "mapMode:" << "Origin" << mContext.mapMode.origin
	//              << "scaleX"
	//              << mContext.mapMode.scaleX.numerator << mContext.mapMode.scaleX.numerator
	//              << (qreal(mContext.mapMode.scaleX.numerator) / mContext.mapMode.scaleX.numerator)
	//              << "scaleY"
	//              << mContext.mapMode.scaleY.numerator << mContext.mapMode.scaleY.numerator
	//              << (qreal(mContext.mapMode.scaleX.numerator) / mContext.mapMode.scaleX.numerator);
	
	m_pDC->SetMapMode(mapMode);
	UpdateOutputDC();
}
void CSvmFile::Read_META_TEXT_A()
{
	std::wstring sText;
	TSvmPoint   startPoint;

	m_oStream >> startPoint;
	parseString(m_oStream, sText);

	DrawText(sText, sText.length(), startPoint.x, startPoint.y);
}
void CSvmFile::Read_META_SETTEXTCOLOR()
{
	TSvmColor oColor; //??? 3 byte????
	m_oStream >> oColor;
	m_pDC->SetTextColor(oColor);
	UpdateOutputDC();
}
void CSvmFile::Read_META_SETFILLCOLOR()
{
	bool     doSet;
	TSvmColor oColor; //??? 3 byte????
	
	m_oStream >> oColor;
	m_oStream >> doSet;

	CSvmBrush* pBrush = new CSvmBrush();
	if (!pBrush)
		return SetError();
	
	pBrush->BrushStyle = BS_NULL;
	if (doSet)
	{
		pBrush->Color = oColor;
		pBrush->BrushStyle = BS_SOLID; 
	}
	m_oPlayer.RegisterObject((CSvmObjectBase*)pBrush);
}

void CSvmFile::Read_META_SETLINECOLOR()
{
	TSvmColor  colorData;
	bool     doSet;

	m_oStream >> colorData;
	m_oStream >> doSet;

	CSvmPen* pPen = new CSvmPen();
	if (!pPen)
		return SetError();

	pPen->PenStyle = PS_NULL;
	if (doSet)
	{
		pPen->Color = colorData;
		pPen->PenStyle = PS_COSMETIC | PS_SOLID;
	}
	m_oPlayer.RegisterObject((CSvmObjectBase*)pPen);
}
void CSvmFile::Read_META_CREATEFONT()
{
	CSvmFont* pFont = new CSvmFont();
	if (!pFont)
		return SetError();

	m_oStream >> pFont;
	m_oPlayer.RegisterObject((CSvmObjectBase*)pFont);
}

void CSvmFile::dumpAction(CDataStream &stream, unsigned short version, unsigned int totalSize)
{
    //qDebug() << "Version: " << version;
    for (unsigned int i = 0; i < totalSize; ++i) 
	{
        unsigned char  temp;
        stream >> temp;
        //qDebug() << hex << i << temp << dec;
    }
}

} // namespace MetaFile

