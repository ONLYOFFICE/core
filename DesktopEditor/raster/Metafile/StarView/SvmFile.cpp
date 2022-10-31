/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#include <string>

#include "../../../graphics/pro/Graphics.h"
#include "../Common/MetaFileTypes.h"
#include "../Common/MetaFileRenderer.h"

#include "../../../../OfficeUtils/src/OfficeUtils.h"
#ifdef _DEBUG
	#include <iostream>
#endif

#include "SvmFile.h"
#include "SvmEnums.h"
#include "SvmObjects.h"

#define DEBUG_CSvmParser 0

namespace MetaFile
{

static const struct ActionNamesSmv
{
    int     actionNumber;
    std::wstring actionName;
} actionNamesSmv[] =
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

    if (start != "VCLMTF")    
	{
		SetError();
		return;
	}
	m_oStream.Skip(6);
	
	Read_SVM_HEADER();

	if (m_pOutput)
	{
		m_pOutput->Begin();
	}

	for (unsigned int action = 0; action < m_oHeader.actionCount; ++action)
	{
        unsigned short  actionType;

		m_oStream >> actionType;

		m_oStream >> m_currentActionVersion;
		m_oStream >> m_unRecordSize;

        m_unRecordPos = m_oStream.Tell();

        switch (actionType) 
		{
			case META_RECT_ACTION:				Read_META_RECTANGLE();			break;
			case META_POLYLINE_ACTION:			Read_META_POLYLINE();			break;
			case META_POLYGON_ACTION:			Read_META_POLYGON();			break;
			case META_POLYPOLYGON_ACTION:		Read_META_POLYPOLYGON();		break;
			case META_LINE_ACTION:				Read_META_LINE();				break;
			
			case META_TEXT_ACTION:				Read_META_TEXT();				break;
			case META_TEXTARRAY_ACTION:			Read_META_ARRAYTEXT();			break;
			case META_TEXTALIGN_ACTION:			Read_META_TEXTALIGN();			break;
			case META_TEXTRECT_ACTION:			Read_META_TEXTRECT();			break;
			case META_TEXTFILLCOLOR_ACTION:		Read_META_SETTEXTFILLCOLOR();	break;
			case META_TEXTCOLOR_ACTION:			Read_META_SETTEXTCOLOR();		break;
			case META_STRETCHTEXT_ACTION:		Read_META_STRETCHTEXT();		break;
			case META_TEXTLANGUAGE_ACTION:		Read_META_TEXTLANGUAGE();		break;
			case META_TEXTLINECOLOR_ACTION:
			case META_TEXTLINE_ACTION:
				break;
			
			case META_MAPMODE_ACTION:			Read_META_SETMAPMODE();			break;
			case META_FONT_ACTION:				Read_META_FONT();				break;
			case META_BMPSCALE_ACTION:			Read_META_BMPSCALE();			break;
			case META_BMP_ACTION:				Read_META_BMP();				break;

			case META_LINECOLOR_ACTION:			Read_META_SETLINECOLOR();		break;
			case META_FILLCOLOR_ACTION:			Read_META_SETFILLCOLOR();		break;
			case META_GRADIENT_ACTION:			Read_META_GRADIENT();			break;
			case META_GRADIENTEX_ACTION:		Read_META_GRADIENTEX();			break;
			case META_HATCH_ACTION:
		
			case META_RASTEROP_ACTION:			Read_META_RASTEROP();			break;			
			case META_PUSH_ACTION:				Read_META_PUSH();				break;		
			case META_POP_ACTION:				Read_META_POP();				break;		

			case META_TRANSPARENT_ACTION:		Read_META_TRANSPARENT();		break;	
			case META_FLOATTRANSPARENT_ACTION:	Read_META_FLOATTRANSPARENT();	break;

			case META_ISECTRECTCLIPREGION_ACTION:	Read_META_SECTRECTCLIPREGION(); break;
			case META_ISECTREGIONCLIPREGION_ACTION:	Read_META_SECTREGIONCLIPREGION(); break;

                        case META_BMPEX_ACTION:                 Read_META_BMPEX(); break;
                        case META_BMPEXSCALE_ACTION:		Read_META_BMPEXSCALE(); break;

			case META_ROUNDRECT_ACTION:
			case META_ELLIPSE_ACTION:
			case META_ARC_ACTION:
			case META_PIE_ACTION:
			case META_CHORD_ACTION:
			case META_PIXEL_ACTION:
			case META_POINT_ACTION:

			case META_BMPSCALEPART_ACTION:
			case META_BMPEXSCALEPART_ACTION:
			case META_MASK_ACTION:
			case META_MASKSCALE_ACTION:
			case META_MASKSCALEPART_ACTION:
			case META_WALLPAPER_ACTION:
			case META_CLIPREGION_ACTION:
			case META_MOVECLIPREGION_ACTION:
			case META_EPS_ACTION:
			case META_REFPOINT_ACTION:
			case META_LAYOUTMODE_ACTION:
			case META_OVERLINECOLOR_ACTION:
			case META_RENDERGRAPHIC_ACTION:				
			case META_COMMENT_ACTION:

			case META_NULL_ACTION:
			default:
			break;
        }

		m_currentActionType = actionType;

		int need_skip = m_unRecordSize - (m_oStream.Tell() - m_unRecordPos);
		m_oStream.Skip(need_skip);

#ifdef _DEBUG
		if (100 <= actionType && actionType <= META_LAST_ACTION && need_skip > 0 && !m_pOutput)
		{
                        std::wstring name = actionNamesSmv[actionType - 99].actionName;

			std::wcout << name << L"\t\t" << actionType << L"\t(version = " << m_currentActionVersion << L")\t; skiped = " << need_skip << L"\n";
		}			
#endif   
 }
	if (m_pOutput)
		m_pOutput->End();

	ClearFile();
}

void CSvmFile::Read_META_LAYOUTMODE()
{
    unsigned int    textLayputMode;
    
    m_oStream >> textLayputMode;

    UpdateOutputDC();
}

void CSvmFile::Read_META_LINE()
{
	TSvmPoint start, end;

	m_oStream >> start;
	m_oStream >> end;

	if (m_currentActionVersion >= 2)
	{
		TSvmLineInfo line_info;
		m_oStream >> line_info;

		CSvmPen *last_pen = dynamic_cast<CSvmPen *>(m_oPlayer.GetLastObject(SVM_OBJECT_PEN));
		if (last_pen)
		{
			last_pen->Width = static_cast<int>(line_info.width / (m_oPlayer.GetDC()->m_dPixelWidth * 2));

			switch(line_info.style)
			{
				case LINE_SOLID:	last_pen->PenStyle = PS_SOLID ; break;
				case LINE_DASH:		last_pen->PenStyle = PS_DASH ; break;
			}
		}
	}
	MoveTo(start.x + m_oCurrnetOffset.x, start.y + m_oCurrnetOffset.y);
	LineTo(end.x + m_oCurrnetOffset.x, end.y + m_oCurrnetOffset.y);
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

	m_pDC->SetMapMode(m_oHeader.mapMode, true);
	
	if (m_bMainStream) //из-за 2 идет увеличение самой картинки в 2 раза (содержимое имеет исходный размер, т.е. в 2 раза меньше нужного)
	{	
		unsigned int unCoef = 1;
		m_oBoundingBox			= m_oHeader.boundRect;
		m_oBoundingBox.nRight	= static_cast<int>(m_pDC->m_dPixelWidthPrefered		* unCoef * m_oBoundingBox.nRight);
		m_oBoundingBox.nBottom	= static_cast<int>(m_pDC->m_dPixelHeightPrefered	* unCoef * m_oBoundingBox.nBottom);
			
		m_oBoundingBox.nLeft	= static_cast<int>(m_pDC->m_dPixelWidthPrefered		* unCoef * m_oBoundingBox.nLeft);
		m_oBoundingBox.nTop		= static_cast<int>(m_pDC->m_dPixelHeightPrefered	* unCoef * m_oBoundingBox.nTop);
	}// *2 ради повышения качества картинки (если в векторе насамом деле растр - сментся на растровые размеры ниже
		
	m_bFirstPoint = true;
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
			last_pen->Width = line_info.width / (m_oPlayer.GetDC()->m_dPixelWidth * 2);

			switch(line_info.style)
			{
				case LINE_SOLID:	last_pen->PenStyle = PS_SOLID ; break;
				case LINE_DASH:		last_pen->PenStyle = PS_DASH ; break;
			}

			if (last_pen->Width < 1)
				last_pen->Width = 20;
		}
	}

	if (m_currentActionVersion >= 3)
	{
		unsigned char bHasPolyFlags = 0;
		m_oStream >> bHasPolyFlags;
		if ( bHasPolyFlags )
		{
		}
		//read flags
//enum PolyFlags
//{
//    POLY_NORMAL,
//    POLY_SMOOTH,
//    POLY_CONTROL,
//    POLY_SYMMTR
//};
	}

	if (polygon.points.size() < 1) return;

	MoveTo(polygon.points[0].x + m_oCurrnetOffset.x, polygon.points[0].y + m_oCurrnetOffset.y);

	for (size_t i = 1; i < polygon.points.size(); i++)
	{
		LineTo(polygon.points[i].x + m_oCurrnetOffset.x, polygon.points[i].y + m_oCurrnetOffset.y);
	}
	ClosePath();
	DrawPath(true, false);
}

void CSvmFile::Read_META_POLYGON()
{
	TSvmPolygon polygon;

	m_oStream >> polygon;

	if (polygon.points.size() < 1) return;

	MoveTo(polygon.points[0].x + m_oCurrnetOffset.x, polygon.points[0].y + m_oCurrnetOffset.y);

	for (size_t i = 1; i < polygon.points.size(); i++)
	{
		LineTo(polygon.points[i].x + m_oCurrnetOffset.x, polygon.points[i].y + m_oCurrnetOffset.y);
	}
	ClosePath();
	DrawPath(true, true);
}

void CSvmFile::Read_META_POLYPOLYGON(std::vector<TSvmPolygon> & polygons, std::vector<TSvmPolygon> & complexPolygons)
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

	if (m_currentActionVersion > 1)
	{
		unsigned short complexPolygonCount;
		m_oStream >> complexPolygonCount;

		complexPolygons.resize(complexPolygonCount);

		for (unsigned short i = 0; i < complexPolygonCount; i++)
		{
			unsigned short complexPolygonIndex;
			m_oStream >> complexPolygonIndex;

			if (complexPolygonIndex >= complexPolygons.size())
			{
				complexPolygons.resize(complexPolygonIndex+1);
			}

			m_oStream >> complexPolygons[complexPolygonIndex];
		}
	}
}
void CSvmFile::Read_META_POLYPOLYGON()
{
	std::vector<TSvmPolygon> polygons;
	std::vector<TSvmPolygon> complexPolygons;

	Read_META_POLYPOLYGON(polygons, complexPolygons);

	if (polygons.size() < 1) return;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_pOutput)
	{
		m_pOutput->StartPath();
		for (unsigned short ushPolygonIndex = 0; ushPolygonIndex < polygons.size(); ushPolygonIndex++)
		{
			unsigned short ushPointsCount = polygons[ushPolygonIndex].points.size();

			if (ushPointsCount <= 0)
				continue;

			MoveTo(polygons[ushPolygonIndex].points[0].x + m_oCurrnetOffset.x, polygons[ushPolygonIndex].points[0].y + m_oCurrnetOffset.y);
			for (int i = 1; i < ushPointsCount; i++)
			{
				LineTo(polygons[ushPolygonIndex].points[i].x + m_oCurrnetOffset.x, polygons[ushPolygonIndex].points[i].y + m_oCurrnetOffset.y);
			}
			ClosePath();
		}
		//for (unsigned short ushPolygonIndex = 0; ushPolygonIndex < complexPolygons.size(); ushPolygonIndex++)
		//{
		//	unsigned short ushPointsCount = complexPolygons[ushPolygonIndex].points.size();

		//	if (ushPointsCount <= 0)
		//		continue;

		//	MoveTo(complexPolygons[ushPolygonIndex].points[0].x + m_oCurrnetOffset.x, complexPolygons[ushPolygonIndex].points[0].y);
		//	for (int i = 1; i < ushPointsCount; i++)
		//	{
		//		LineTo(complexPolygons[ushPolygonIndex].points[i].x + m_oCurrnetOffset.x, complexPolygons[ushPolygonIndex].points[i].y);
		//	}
		//	ClosePath();
		//}
		DrawPath(true, true);
	}
}

void CSvmFile::Read_META_SETMAPMODE()
{
	TSvmMapMode mapMode;
	m_oStream >> mapMode;
	
	m_pDC->SetMapMode(mapMode);

	m_oCurrnetOffset = mapMode.origin;

	UpdateOutputDC();
}

void CSvmFile::Read_META_TEXTLANGUAGE()
{
	m_oStream >> m_currentLanguage;
}

void CSvmFile::Read_META_STRETCHTEXT()
{
	std::wstring sText;
	TSvmPoint   startPoint;

	m_oStream >> startPoint;

	parseString(m_oStream, sText, m_currentActionVersion, m_currentCharset);

	unsigned short	Index;
	unsigned short	Len;
	unsigned int	Width;
	
	m_oStream >> Width;
	m_oStream >> Index;
	m_oStream >> Len;

	if (m_currentActionVersion > 1)
	{
		unsigned short nLen;
		m_oStream >> nLen;
		
		std::wstring buf;
		unsigned short nTemp;
		while ( nLen-- )
		{
			m_oStream >> nTemp;
			buf += (wchar_t)nTemp;
		}
		sText = buf;
	}
	DrawText(sText, sText.length(), startPoint.x + m_oCurrnetOffset.x, startPoint.y + m_oCurrnetOffset.y);
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
	DrawText(sText, sText.length(), startPoint.x + m_oCurrnetOffset.x, startPoint.y + m_oCurrnetOffset.y);
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

	int *mpDXAry = NULL;
	if( nArrayLen > 0 )
	{
        // #i9762#, #106172# Ensure that DX array is at least mnLen entries long
		const unsigned int nIntAryLen = (std::max)(nArrayLen, (unsigned int)nLen) ;
		mpDXAry = new int[ nIntAryLen ];
        
        unsigned int i=0;
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
	DrawText(sText, sText.length(), startPoint.x + m_oCurrnetOffset.x, startPoint.y + m_oCurrnetOffset.y, mpDXAry, nArrayLen);

	if (mpDXAry)
		delete []mpDXAry;
}
void CSvmFile::Read_META_TEXTRECT()
{
	TSvmRect	rect;
	unsigned short style;
	std::wstring sText;
	
	m_oStream >> rect;
	parseString(m_oStream, sText, m_currentActionVersion, m_currentCharset);

	m_oStream >> style;

	if ( m_currentActionVersion >= 2 )
	{

	}
	DrawText(sText, sText.length(), rect);
}
void CSvmFile::Read_META_TEXTALIGN()
{
	unsigned short nTmp16;
	m_oStream >> nTmp16; 

	unsigned int align = 0;
	switch(nTmp16)
	{
	case 0:	align = TA_TOP;		break;
	case 1:	align = TA_BASELINE;break;
	case 2:	align = TA_BOTTOM;	break;
	}

	m_pDC->SetTextAlign(align);
}

void CSvmFile::Read_META_SETTEXTCOLOR()
{
	TSvmColor oColor; 
	m_oStream >> oColor;
	m_pDC->SetTextColor(oColor);
	UpdateOutputDC();
}

void CSvmFile::Read_META_SETTEXTFILLCOLOR()
{
	TSvmColor oColor; 
	m_oStream >> oColor;
	m_pDC->SetTextBgColor(oColor);
	UpdateOutputDC();
}

void CSvmFile::Read_META_SETFILLCOLOR()
{
	if (m_currentActionType == META_GRADIENT_ACTION ||
		m_currentActionType == META_GRADIENTEX_ACTION)
	{
		//поменять основной цвет??
		m_oStream.Skip(m_unRecordSize);
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
	std::vector<TSvmPolygon> complexPolygons;

	Read_META_POLYPOLYGON(polygons, complexPolygons);

	CSvmBrush* pBrush = new CSvmBrush();
	if (!pBrush)
		return SetError();

	TSvmGradient gradient;
	m_oStream >> gradient;

	switch((ESvmGradientStyle)gradient.style)
	{
	case GRADIENT_LINEAR:		pBrush->BrushStyle	= BS_LINEARGRADIENT;	break;
	case GRADIENT_AXIAL:		pBrush->BrushStyle	= BS_AXIALGRADIENT;		break;
	case GRADIENT_RADIAL:		pBrush->BrushStyle	= BS_RADIALGRADIENT;	break;
	case GRADIENT_ELLIPTICAL:	pBrush->BrushStyle	= BS_RADIALGRADIENT;	break;
	case GRADIENT_SQUARE:		pBrush->BrushStyle	= BS_LINEARGRADIENT;	break;
	case GRADIENT_RECT:			pBrush->BrushStyle	= BS_RECTGRADIENT;		break;
	}
	
	pBrush->Color.Set(gradient.color1.r>>8, gradient.color1.g>>8, gradient.color1.b>>8);
	pBrush->Color2.Set(gradient.color2.r>>8, gradient.color2.g>>8, gradient.color2.b>>8);

	pBrush->BrushStyleEx = 90 + gradient.angle % 3600; //проверить на разных

	m_oPlayer.RegisterObject((CSvmObjectBase*)pBrush);
}

void CSvmFile::Read_META_TRANSPARENT()
{
	std::vector<TSvmPolygon> polygons;
	std::vector<TSvmPolygon> complexPolygons;

	Read_META_POLYPOLYGON(polygons, complexPolygons);

	unsigned short nPercent;

	 m_oStream >> nPercent;

	 if (m_pOutput)
	 {
		CSvmBrush *last_brush = dynamic_cast<CSvmBrush *>(m_oPlayer.GetLastObject(SVM_OBJECT_BRUSH));
		
		CSvmBrush* pBrush = new CSvmBrush();
		pBrush->BrushStyle = BS_NULL;
		if (!pBrush)
			return SetError();		
		
		if ((last_brush) /*&& (last_brush->BrushStyle > 0)*/) //skip Nofill
		{
			pBrush->BrushStyle = last_brush->BrushStyle; 
			pBrush->BrushHatch = last_brush->BrushHatch; 
			pBrush->BrushStyleEx = last_brush->BrushStyleEx; 
			pBrush->BrushBounds = last_brush->BrushBounds; 
			pBrush->Color = last_brush->Color; 
			pBrush->Color2 = last_brush->Color2; 

			pBrush->Color.a		= 0xff * nPercent /100;
			pBrush->Color2.a	= 0xff * nPercent /100;
		}
		m_oPlayer.RegisterObject((CSvmObjectBase*)pBrush);
	
		m_pOutput->StartPath();
		
		for (unsigned short ushPolygonIndex = 0; ushPolygonIndex < polygons.size(); ushPolygonIndex++)
		{
			unsigned short ushPointsCount = polygons[ushPolygonIndex].points.size();

			if (ushPointsCount <= 0)
				continue;

			MoveTo(polygons[ushPolygonIndex].points[0].x + m_oCurrnetOffset.x, polygons[ushPolygonIndex].points[0].y + m_oCurrnetOffset.y);
			for (int i = 1; i < ushPointsCount; i++)
			{
				LineTo(polygons[ushPolygonIndex].points[i].x + m_oCurrnetOffset.x, polygons[ushPolygonIndex].points[i].y + m_oCurrnetOffset.y);
			}

			m_pOutput->ClosePath();
		}
	 }

	DrawPath(false, true);
}

void CSvmFile::Read_META_FLOATTRANSPARENT()
{
	return;
	// тут возможен также вариант svg
	CSvmFile subMetaFile(m_oStream.GetCurPtr() , m_unRecordSize);
	
	subMetaFile.m_bMainStream = false;
    subMetaFile.SetFontManager(GetFontManager());

	subMetaFile.PlayMetaFile();
	int skip_size = subMetaFile.m_oStream.Tell();

	m_oStream.Skip( skip_size);

	TSvmPoint	point;
	TSvmSize	size;

	m_oStream >> point;
	m_oStream >> size;

///////////////////////////////////////////////////
	CSvmBrush* pBrush = new CSvmBrush();
	if (!pBrush)
		return SetError();

	TSvmGradient gradient;
	m_oStream >> gradient;

	switch((ESvmGradientStyle)gradient.style)
	{
		case GRADIENT_LINEAR:		pBrush->BrushStyle	= BS_LINEARGRADIENT;	break;
		case GRADIENT_AXIAL:		pBrush->BrushStyle	= BS_AXIALGRADIENT;		break;
		case GRADIENT_RADIAL:		pBrush->BrushStyle	= BS_RADIALGRADIENT;	break;
		case GRADIENT_ELLIPTICAL:	pBrush->BrushStyle	= BS_RADIALGRADIENT;	break;
		case GRADIENT_SQUARE:		pBrush->BrushStyle	= BS_LINEARGRADIENT;	break;
		case GRADIENT_RECT:			pBrush->BrushStyle	= BS_RECTGRADIENT;		break;
	}
	
	pBrush->Color.Set(gradient.color1.r>>8, gradient.color1.g>>8, gradient.color1.b>>8);
	pBrush->Color2.Set(gradient.color2.r>>8, gradient.color2.g>>8, gradient.color2.b>>8);

	pBrush->BrushStyleEx = 90 + gradient.angle % 3600; //проверить на разных

	m_oPlayer.RegisterObject((CSvmObjectBase*)pBrush);

//---------------------------------------------------------
	CSvmBrush* pBrushCopy = new CSvmBrush(*pBrush);
	subMetaFile.m_oPlayer.RegisterObject((CSvmObjectBase*)pBrushCopy);
	
//---------------------------------------------------------
	subMetaFile.m_oStream.SeekToStart();
	
	//CGraphicsRenderer oRenderer;	

	//TRect *rect = subMetaFile.GetBounds();
	//
	//int nW = rect->nRight - rect->nLeft;
	//int nH = rect->nBottom - rect->nTop;
	//
	//double dWidth  = nW;
	//double dHeight = nH;

	//BYTE* pBgraData = new BYTE[nW * nH * 4];
	//if (!pBgraData)
	//	return;

 //   _UINT32 alfa = 0xffffff;
	////дефолтный тон должен быть прозрачным, а не белым 
	////memset(pBgraData, 0xff, nWidth * nHeight * 4);
	//for (int i = 0; i < nW * nH; i++)
	//{
 //       ((_UINT32*)pBgraData)[i] = alfa;
	//}
	//CBgraFrame oFrame;
	//oFrame.put_Data(pBgraData);
	//oFrame.put_Width(nW);
	//oFrame.put_Height(nH);
	//oFrame.put_Stride(-4 * nW);

	//oRenderer.CreateFromBgraFrame(&oFrame);
	//oRenderer.SetSwapRGB(false);
	//oRenderer.put_Width(dWidth);
	//oRenderer.put_Height(dHeight);
	//
	//CMetaFileRenderer oSvmOut(&subMetaFile, &oRenderer, 0, 0, dWidth, dHeight);
	subMetaFile.SetOutputDevice(m_pOutput);//&oSvmOut);
	
	subMetaFile.PlayMetaFile();
	
	double dX = point.x, dY = point.y, dR = point.x + size.cx, dB = point.y + size.cy;
	
	TranslatePoint(point.x, point.y, dX, dY);
	TranslatePoint(point.x + size.cx, point.y + size.cy, dR, dB);
	
	//if (m_pOutput)
	//{
	//	LONG width, height;
	//	BYTE* bytes = oRenderer.GetPixels(width, height);
	//	m_pOutput->DrawBitmap(dX, dY, dR - dX, dB - dY, bytes, width, height);
	//	oFrame.SaveFile(NSFile::CFileBinary::CreateTempFileWithUniqueName(L"d:\\test\\_svm", L"svm"), 1);

	//}

}

void CSvmFile::Read_META_GRADIENT()
{
	CSvmBrush* pBrush = new CSvmBrush();
	if (!pBrush)
		return SetError(); 

	m_oStream >> pBrush->BrushBounds;

	TSvmGradient gradient;
	m_oStream >> gradient;

	switch((ESvmGradientStyle)gradient.style)
	{
	case GRADIENT_LINEAR:		pBrush->BrushStyle	= BS_LINEARGRADIENT;	break;
	case GRADIENT_AXIAL:		pBrush->BrushStyle	= BS_AXIALGRADIENT;		break;
	case GRADIENT_RADIAL:		pBrush->BrushStyle	= BS_RADIALGRADIENT;	break;
	case GRADIENT_ELLIPTICAL:	pBrush->BrushStyle	= BS_RADIALGRADIENT;	break;
	case GRADIENT_SQUARE:		pBrush->BrushStyle	= BS_LINEARGRADIENT;	break;
	case GRADIENT_RECT:			pBrush->BrushStyle	= BS_RECTGRADIENT;		break;
	}
	
	pBrush->Color.Set(gradient.color1.r>>8, gradient.color1.g>>8, gradient.color1.b>>8);
	pBrush->Color2.Set(gradient.color2.r>>8, gradient.color2.g>>8, gradient.color2.b>>8);

	pBrush->BrushStyleEx = 90 + gradient.angle % 3600; //проверить на разных

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

	//ROP_OVERPAINT, ROP_XOR, ROP_0, ROP_1, ROP_INVERT 

	//switch(tmp)
	//{
	//case 0: if ( m_pOutput)	m_pOutput->EndClipPath(0);			break;
	//case 1: if ( m_pOutput)	m_pOutput->StartClipPath(RGN_AND);	break;
	//case 2: if ( m_pOutput)	m_pOutput->StartClipPath(RGN_OR);	break;
	//case 3: if ( m_pOutput)	m_pOutput->StartClipPath(RGN_DIFF);	break;
	//}

	
}
void CSvmFile::Read_META_BMP()
{
    TSvmBitmap bitmap_info;

    BYTE *pBgraBuffer = NULL;
    unsigned int ulWidth = 0, ulHeight = 0;

    Read_META_BMP(bitmap_info, &pBgraBuffer, &ulWidth, &ulHeight );

    if (bitmap_info.nHeight >  (unsigned int)m_oBoundingBox.nBottom &&
            bitmap_info.nWidth > (unsigned int)m_oBoundingBox.nRight && !m_pOutput)
    {
        m_oBoundingBox.nRight = bitmap_info.nWidth;
        m_oBoundingBox.nBottom = bitmap_info.nHeight;
    }

    if (pBgraBuffer)
    {
        if ( m_pOutput)
        {
            m_pOutput->DrawBitmap( m_oCurrnetOffset.x, m_oCurrnetOffset.y, bitmap_info.nWidth, bitmap_info.nHeight, pBgraBuffer, bitmap_info.nWidth, bitmap_info.nHeight);
        }

        delete []pBgraBuffer;

        UpdateOutputDC();
    }
}
void CSvmFile::Read_META_BMP(TSvmBitmap & bitmap_info, BYTE** ppDstBuffer, unsigned int* pulWidth, unsigned int* pulHeight)
{
    unsigned int	nHeaderSize = 0;

    unsigned int	nTmp32;
    unsigned short	nTmp16 = 0;
    bool                bRet = false;

    m_oStream >> nTmp16;
    m_oStream >> nHeaderSize;

    if ( ( 0x4D42 == nTmp16 ) || ( 0x4142 == nTmp16 ) )
    {
        if ( 0x4142 == nTmp16 )
        {
            m_oStream.Skip( 12 );//не то !!!
            m_oStream >> nTmp16;
            m_oStream.Skip( 8 );
            m_oStream >> nTmp32;
            nHeaderSize = nTmp32 - 28UL;;
            bRet = ( 0x4D42 == nTmp16 );
        }
        else
        {
            m_oStream.Skip( 4 );
            m_oStream >> nTmp32;
            nHeaderSize = nTmp32 - 14;
        }
    }

    unsigned short	nColors;
    BYTE*		pData = NULL;

    m_oStream >> bitmap_info; //size = 40

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

    if( ZCOMPRESS == bitmap_info.nCompression )
    {
        COfficeUtils OfficeUtils(NULL);

        ULONG destSize, srcSize;
        m_oStream >> srcSize >> destSize >> bitmap_info.nCompression;

        BYTE*	srcBuf	= m_oStream.GetCurPtr();
        BYTE*	destBuf = new BYTE[destSize];

        if (destSize > 0 && m_pOutput && destBuf )
        {
            if (OfficeUtils.Uncompress(destBuf, &destSize, srcBuf, srcSize) != S_OK)
            {
                delete []destBuf;
                return;
            }
        }
        m_oStream.Skip(srcSize);

        MetaFile::ReadImage((BYTE*)&bitmap_info, bitmap_info.nSize, destBuf, destSize, ppDstBuffer, pulWidth, pulHeight);
        delete []destBuf;
    }
    else
    {
        BYTE *Header = new BYTE [ nHeaderSize];
        memcpy(Header, &bitmap_info, bitmap_info.nSize);

        m_oStream.ReadBytes(Header + bitmap_info.nSize, nHeaderSize - bitmap_info.nSize);

        MetaFile::ReadImage(Header , nHeaderSize,  m_oStream.GetCurPtr(), bitmap_info.nSizeImage, ppDstBuffer, pulWidth, pulHeight);
        m_oStream.Skip(bitmap_info.nSizeImage);
        delete[] Header;
    }
}

void CSvmFile::Read_META_BMPEX()
{
    TSvmBitmap bitmap_info;

    BYTE *pBgraBuffer = NULL;
    unsigned int ulWidth = 0, ulHeight = 0;

    Read_META_BMP(bitmap_info, &pBgraBuffer, &ulWidth, &ulHeight);

   //     if (ulWidth !=  bitmap_info.nWidth && bitmap_info.nWidth % 2 != 0)
	{
		bitmap_info.nWidth = ulWidth;
	}
	//иногда наверху неверно вычисляется оригинальный размер - если внутри одиночная картинка

    if (bitmap_info.nHeight >  (unsigned int)m_oBoundingBox.nBottom &&
            bitmap_info.nWidth > (unsigned int)m_oBoundingBox.nRight && !m_pOutput)
    {
        m_oBoundingBox.nRight = bitmap_info.nWidth;
        m_oBoundingBox.nBottom = bitmap_info.nHeight;
    }
    unsigned int	nMagic1;
    unsigned int	nMagic2;

    m_oStream >> nMagic1 >> nMagic2;

    TSvmBitmap bitmap_info_mask;
    BYTE *pBgraBufferMask = NULL;
    unsigned int ulWidthMask = 0, ulHeightMask = 0;

    if ( (0x25091962 == nMagic1) && (0xACB20201 == nMagic2))
    {
        BYTE transparentType;
        m_oStream >> transparentType;

        switch(transparentType)
        {
            case 2:// bitmap mask
                Read_META_BMP(bitmap_info_mask, &pBgraBufferMask, &ulWidthMask, &ulHeightMask);
            break;
            case 1:// transparent color
                TSvmColor oColor;
                m_oStream >> oColor;
                break;
        }
    }

    if (pBgraBufferMask)
    {
        delete []pBgraBufferMask;
    }

    if (pBgraBuffer)
    {
        DrawImage(m_oCurrnetOffset.x, m_oCurrnetOffset.y, bitmap_info.nWidth, bitmap_info.nHeight, pBgraBuffer, bitmap_info.nWidth, bitmap_info.nHeight);

        delete []pBgraBuffer;

        UpdateOutputDC();
    }
}

void CSvmFile::Read_META_SECTRECTCLIPREGION()
{
    TSvmRect	rect;

    m_oStream >> rect;
}
void CSvmFile::Read_META_SECTREGIONCLIPREGION()
{
}
void CSvmFile::Read_META_BMPEXSCALE()
{ 
    TSvmBitmap bitmap_info;

    BYTE *pBgraBuffer = NULL;
    unsigned int ulWidth = 0, ulHeight = 0;

    Read_META_BMP(bitmap_info, &pBgraBuffer, &ulWidth, &ulHeight);

     //иногда наверху неверно вычисляется оригинальный размер - если внутри одиночная картинка

   //     if (ulWidth !=  bitmap_info.nWidth && bitmap_info.nWidth % 2 != 0)
	{
		bitmap_info.nWidth = ulWidth;
	}

    if (bitmap_info.nHeight >  (unsigned int)m_oBoundingBox.nBottom &&
            bitmap_info.nWidth > (unsigned int)m_oBoundingBox.nRight && !m_pOutput)
    {
        m_oBoundingBox.nRight = bitmap_info.nWidth;
        m_oBoundingBox.nBottom = bitmap_info.nHeight;
    }
    unsigned int	nMagic1;
    unsigned int	nMagic2;

    m_oStream >> nMagic1 >> nMagic2;

    TSvmBitmap bitmap_info_mask;

    BYTE *pBgraBufferMask = NULL;
    unsigned int ulWidthMask = 0, ulHeightMask = 0;

    if ( (0x25091962 == nMagic1) && (0xACB20201 == nMagic2))
    {
        BYTE transparentType;
        m_oStream >> transparentType;

        switch(transparentType)
        {
            case 2:// bitmap mask
                Read_META_BMP(bitmap_info_mask, &pBgraBufferMask, &ulWidthMask, &ulHeightMask);
            break;
            case 1:// transparent color
                TSvmColor oColor;
                m_oStream >> oColor;
                break;
        }
    }

    if (pBgraBufferMask)
    {
        delete []pBgraBufferMask;
    }

    TSvmSize	size;
    TSvmPoint	point;

    m_oStream >> point;
    m_oStream >> size.cx;
    m_oStream >> size.cy;

    if (pBgraBuffer)
    {
        DrawImage(point.x + m_oCurrnetOffset.x, point.y + m_oCurrnetOffset.y, size.cx, size.cy, pBgraBuffer, ulWidth, ulHeight);

        delete []pBgraBuffer;

        UpdateOutputDC();
    }

}
void CSvmFile::Read_META_BMPSCALE()
{
    TSvmBitmap bitmap_info;

    BYTE *pBgraBuffer = NULL;

    unsigned int ulWidth = 0, ulHeight = 0;

    Read_META_BMP(bitmap_info, &pBgraBuffer, &ulWidth, &ulHeight );

    //иногда наверху неверно вычисляется оригинальный размер - если внутри одиночная картинка
    //if (ulWidth !=  bitmap_info.nWidth && bitmap_info.nWidth % 2 != 0)
    {
        bitmap_info.nWidth = ulWidth;
    }

    if (bitmap_info.nHeight >  (unsigned int)m_oBoundingBox.nBottom &&
            bitmap_info.nWidth > (unsigned int)m_oBoundingBox.nRight && !m_pOutput)
    {
        m_oBoundingBox.nRight = bitmap_info.nWidth;
        m_oBoundingBox.nBottom = bitmap_info.nHeight;
    }

    TSvmSize	size;
    TSvmPoint	point;

    m_oStream >> point;
    m_oStream >> size.cx;
    m_oStream >> size.cy;

    if (pBgraBuffer)
    {
        DrawImage(point.x + m_oCurrnetOffset.x, point.y + m_oCurrnetOffset.y, size.cx, size.cy, pBgraBuffer, ulWidth, ulHeight);

        delete []pBgraBuffer;

        UpdateOutputDC();
    }

}
} // namespace MetaFile

