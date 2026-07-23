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
#ifndef _PDF_WRITER_SRC_UTILS_H
#define _PDF_WRITER_SRC_UTILS_H

#include "Types.h"

#define NEEDS_ESCAPE(c)    (c < 0x21 || \
                            c > 0x7e || \
                            c == '\\' || \
                            c == '%' || \
                            c == '#' || \
							c == '/' || \
                            c == '(' || \
                            c == ')' || \
                            c == '<' || \
                            c == '>' || \
                            c == '[' || \
                            c == ']' || \
                            c == '{' || \
                            c == '}')  \

#define NEEDS_ESCAPE_STR(c) (c < 0x20 || \
							c > 0x7e || \
							c == '\\' || \
							c == '(' || \
							c == ')')  \

#define NEEDS_ESCAPE_DICTVALUE(c) (c != 0x9 && \
                            c != 0xA && \
                           (c < 0x20 || \
                            c > 0x7e || \
                            c == '\\' || \
                            c == '%' || \
                            c == '#' || \
							c == '/' ))  \

#define IS_WHITE_SPACE(c)   (c == 0x00 || \
                             c == 0x09 || \
                             c == 0x0A || \
                             c == 0x0C || \
                             c == 0x0D || \
                             c == 0x20 ) \


//-----------------------------------------------------------------------------------------------------
//  CRC 32
//-----------------------------------------------------------------------------------------------------
class CRC32
{

public:   

	CRC32()
	{
		const unsigned CRC_POLY = 0xEDB88320;

		for ( unsigned int i = 0; i < 256; i++ )
		{
			unsigned int r, j;
			for ( r = i, j = 8; j; j--)
				r = r & 1? (r >> 1) ^ CRC_POLY: r >> 1;

			m_pTable[i] = r;
		}

		m_nCRC32 = 0;
	}
	void Init(unsigned int nCRC = 0)
	{
		m_nCRC32 = nCRC;
	}
	void ProcessCRC(void* pData, int nLen)
	{
		const unsigned CRC_MASK = 0xD202EF8D;
		register unsigned char *sData = reinterpret_cast<unsigned char*>(pData);
		register unsigned int nCRC = m_nCRC32;

		while ( nLen-- )
		{
			nCRC = m_pTable[static_cast<unsigned char>(nCRC) ^ *sData++] ^ nCRC >> 8;
			nCRC ^= CRC_MASK;
		}

		m_nCRC32 = nCRC;
	}

protected:

	unsigned int m_pTable[256];

public:

	unsigned int m_nCRC32;   

};

namespace PdfWriter
{
	BYTE* MemCpy(BYTE* pDst, const BYTE *pSrc, unsigned int unLen);
	void  MemSet(void* pBuf, BYTE nChar, unsigned int unLen);

	BYTE* StrCpy(char* sDst, const char* sSrc, char* pEnd);
	int   StrLen(const char* sString, int nMaxLen);
	char* ItoA  (char* sDst, int nVal, char* pEnd);
	char* ItoA2 (char* sDst, unsigned int unVal, unsigned int unLen);
	int   StrCmp(const char* s1, const char* s2);
	char* FtoA  (char* sDst, double dVal, char* pEnd);

	void  UIntChangeBit(unsigned int& nValue, short nBit);
	void  UIntChangeBit2(unsigned int& nValue, short nBit);

	std::string DateNow();
	std::wstring NormalizeWhitespace(const std::wstring& s);

	// Polygon intersection using separating axis theorem
	bool SAT(const std::vector<CPoint>& poly1, const std::vector<CPoint>& poly2);
	// Check that all points of inner polygon are inside outer polygon
	bool isPolygonInsidePolygon(const std::vector<CPoint>& inner, const std::vector<CPoint>& outer);
	// Check if point belongs to convex quadrilateral
	bool isPointInQuad(double px, double py,
					   double x1, double y1, double x2, double y2,
					   double x3, double y3, double x4, double y4);

	class RectangleIntersection
	{
	private:
		// Check for intersection of two line segments
		static bool segmentsIntersect(const CPoint& a, const CPoint& b, const CPoint& c, const CPoint& d, CPoint& intersection);
		// Check if point is inside rectangle
		static bool pointInRectangle(const CPoint& p, const std::vector<CPoint>& rect);
		// Calculate distance from point to segment start along direction
		static double distanceAlongLine(const CPoint& start, const CPoint& end, const CPoint& point);

	public:
		// Main function for finding segments outside all rectangles
		static std::vector<CSegment> findSegmentsOutsideRectangles(const CSegment& line, const std::vector<std::vector<CPoint>>& rectangles);
		// Alternative approach: sequential subtraction of rectangles
		static std::vector<CSegment> findSegmentsOutsideRectanglesSequential(const CSegment& line, const std::vector<std::vector<CPoint>>& rectangles);
	};

}

#endif // _PDF_WRITER_SRC_UTILS_H

