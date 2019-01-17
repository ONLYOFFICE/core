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
#include "FontPath.h"

//-------------------------------------------------------------------------------------------------------------------------------
// CPath
//-------------------------------------------------------------------------------------------------------------------------------

// Path может быть в одном из трех следующий состояний:
//
// 1. Нет текущей точки -- ни одного или более законченных SubPaths
//    [m_nCurSubpath == m_nPointsCount]
//
// 2. Одна точка в SubPaths
//    [m_nCurSubpath == m_nPointsCount - 1]
//
// 3. Открытый SubPaths c двумя или более точками
//    [m_nCurSubpath < m_nPointsCount - 1]

CFontPath::CFontPath() : NSFonts::IFontPath()
{
	m_pPoints = NULL;
	m_pFlags = NULL;
	m_nPointsCount = m_nSize = 0;
	m_nCurSubpath = 0;

	m_pHints = NULL;
	m_nHintsCount = m_nHintsSize = 0;
}

CFontPath::CFontPath(CFontPath *pPath) : NSFonts::IFontPath()
{
	m_nPointsCount = pPath->m_nPointsCount;
	m_nSize = pPath->m_nSize;
	m_pPoints = (TPathPoint    *)malloc( m_nSize * sizeof(TPathPoint) );
	m_pFlags  = (unsigned char *)malloc( m_nSize * sizeof(unsigned char) );

	memcpy( m_pPoints, pPath->m_pPoints, m_nPointsCount * sizeof(TPathPoint) );
	memcpy( m_pFlags, pPath->m_pFlags, m_nPointsCount * sizeof(unsigned char) );

	m_nCurSubpath = pPath->m_nCurSubpath;

	if ( pPath->m_pHints ) 
	{
		m_nHintsCount = m_nHintsSize = pPath->m_nHintsCount;
		m_pHints = (TPathHint *)malloc( m_nHintsSize * sizeof(TPathHint) );
		memcpy( m_pHints, pPath->m_pHints, m_nHintsCount * sizeof(TPathHint) );
	} 
	else 
	{
		m_pHints = NULL;
	}
}

CFontPath::~CFontPath() 
{
	free( m_pPoints );
	free( m_pFlags );
	free( m_pHints );
}

void CFontPath::Resize(int nPointsCount) 
{
	if ( m_nPointsCount + nPointsCount > m_nSize ) 
	{
		if ( m_nSize == 0 ) 
		{
			m_nSize = 32;
		}
		while ( m_nSize < m_nPointsCount + nPointsCount ) 
		{
			m_nSize *= 2;
		}
		m_pPoints = (TPathPoint    *)realloc( m_pPoints, m_nSize * sizeof(TPathPoint) );
		m_pFlags  = (unsigned char *)realloc( m_pFlags,  m_nSize * sizeof(unsigned char) );
	}
}

void CFontPath::Append(CFontPath *pPath) 
{
	m_nCurSubpath = m_nPointsCount + pPath->m_nCurSubpath;
	Resize( pPath->m_nPointsCount );

	for ( int nIndex = 0; nIndex < pPath->m_nPointsCount; ++nIndex ) 
	{
		m_pPoints[m_nPointsCount] = pPath->m_pPoints[nIndex];
		m_pFlags[m_nPointsCount]  = pPath->m_pFlags[nIndex];
		++m_nPointsCount;
	}
}

int CFontPath::MoveTo(double dX, double dY) 
{
	if ( OnePointSubpath() ) 
	{
		return -1;
	}
	Resize(1);
	m_pPoints[m_nPointsCount].dX = dX;
	m_pPoints[m_nPointsCount].dY = dY;
	m_pFlags[m_nPointsCount] = PathFirst | PathLast;
	m_nCurSubpath = m_nPointsCount++;

	return 0;
}

int CFontPath::LineTo(double dX, double dY) 
{
	if ( NoCurrentPoint() ) 
	{
		return -1;
	}
	m_pFlags[m_nPointsCount - 1] &= ~PathLast;
	Resize(1);

	m_pPoints[m_nPointsCount].dX = dX;
	m_pPoints[m_nPointsCount].dY = dY;
	m_pFlags[m_nPointsCount] = PathLast;
	++m_nPointsCount;

	return 0;
}

int CFontPath::CurveTo(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3) 
{
	if ( NoCurrentPoint() )
	{
		return -1;
	}

	m_pFlags[m_nPointsCount-1] &= ~PathLast;
	Resize(3);

	m_pPoints[m_nPointsCount].dX = dX1;
	m_pPoints[m_nPointsCount].dY = dY1;
	m_pFlags[m_nPointsCount] = PathCurve;
	++m_nPointsCount;

	m_pPoints[m_nPointsCount].dX = dX2;
	m_pPoints[m_nPointsCount].dY = dY2;
	m_pFlags[m_nPointsCount] = PathCurve;
	++m_nPointsCount;

	m_pPoints[m_nPointsCount].dX = dX3;
	m_pPoints[m_nPointsCount].dY = dY3;
	m_pFlags[m_nPointsCount] = PathLast;
	++m_nPointsCount;

	return 0;
}

int CFontPath::Close() 
{
	if ( NoCurrentPoint() ) 
	{
		return -1;
	}
	if ( m_nCurSubpath == m_nPointsCount - 1 || m_pPoints[m_nPointsCount - 1].dX != m_pPoints[m_nCurSubpath].dX || m_pPoints[m_nPointsCount - 1].dY != m_pPoints[m_nCurSubpath].dY ) 
	{
		LineTo( m_pPoints[m_nCurSubpath].dX, m_pPoints[m_nCurSubpath].dY );
	}

	m_pFlags[m_nCurSubpath] |= PathClosed;
	m_pFlags[m_nPointsCount - 1] |= PathClosed;
	m_nCurSubpath = m_nPointsCount;

	return 0;
}

void CFontPath::AddStrokeAdjustHint(int nFirstControl, int nSecondControl, int nFirstPoint, int nLastPoint) 
{
	if ( m_nHintsCount == m_nHintsSize ) 
	{
		m_nHintsSize = m_nHintsCount ? 2 * m_nHintsCount : 8;
		m_pHints = (TPathHint *)realloc( m_pHints, m_nHintsSize * sizeof(TPathHint) );
	}
	m_pHints[m_nHintsCount].nFirstControl  = nFirstControl;
	m_pHints[m_nHintsCount].nSecondControl = nSecondControl;
	m_pHints[m_nHintsCount].nFirstPoint = nFirstPoint;
	m_pHints[m_nHintsCount].nLastPoint  = nLastPoint;
	
	++m_nHintsCount;
}

void CFontPath::Offset(double dDx, double dDy) 
{
	for ( int nIndex = 0; nIndex < m_nPointsCount; ++nIndex ) 
	{
		m_pPoints[nIndex].dX += dDx;
		m_pPoints[nIndex].dY += dDy;
	}
}

void CFontPath::Reverse()
{
	for ( int nIndex = 0; nIndex < m_nPointsCount; ++nIndex ) 
	{
		m_pPoints[nIndex].dY = -m_pPoints[nIndex].dY;
	}
}

void CFontPath::ToMM(double dHorDpi, double dVerDpi) 
{
	for ( int nIndex = 0; nIndex < m_nPointsCount; ++nIndex ) 
	{
		m_pPoints[nIndex].dX *= 25.4 / dHorDpi;
		m_pPoints[nIndex].dY *= 25.4 / dVerDpi;
	}
}

INT CFontPath::GetCurPoint(double *pdX, double *pdY) 
{
	if ( NoCurrentPoint() ) 
	{
		return FALSE;
	}
	*pdX = m_pPoints[m_nPointsCount - 1].dX;
	*pdY = m_pPoints[m_nPointsCount - 1].dY;
	return TRUE;
}

int CFontPath::ToInterface(NSFonts::ISimpleGraphicsPath* pPath)
{
	if ( !pPath  )
		return FALSE;

	for ( int nIndex = 0; nIndex < m_nPointsCount; )
	{
		TPathPoint oPoint = m_pPoints[nIndex];
		unsigned char nFlag = m_pFlags[nIndex];

		if ( nFlag & PathFirst )
		{
			pPath->_MoveTo( oPoint.dX, oPoint.dY );
			nIndex++;
		}
		else if ( nFlag & PathCurve )
		{
			pPath->_CurveTo( m_pPoints[nIndex + 0].dX, m_pPoints[nIndex + 0].dY, m_pPoints[nIndex + 1].dX, m_pPoints[nIndex + 1].dY, m_pPoints[nIndex + 2].dX, m_pPoints[nIndex + 2].dY );
			nIndex += 3;
		}
		else
		{
			pPath->_LineTo( oPoint.dX, oPoint.dY );
			nIndex++;
		}

		if ( nFlag & PathClosed && nFlag & PathLast )
		{
			pPath->_Close();
		}
		
	}

	return TRUE;
}
