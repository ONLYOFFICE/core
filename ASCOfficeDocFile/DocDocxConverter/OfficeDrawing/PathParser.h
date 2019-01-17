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
#pragma once

#include "PathSegment.h"

namespace DocFileFormat
{
	struct _guides
	{
		unsigned char type;

		unsigned char param_type1;
		unsigned char param_type2;
		unsigned char param_type3;

		WORD param1;
		WORD param2;
		WORD param3;
	};
	class PathParser
	{
	public:

		PathParser (const unsigned char* pSegmentInfo, unsigned int pSegmentInfoSize, const unsigned char* pVertices, unsigned int pVerticesSize, std::vector<_guides> & guides)
		{
			
			if ((pSegmentInfo != NULL) && (pSegmentInfoSize > 0))
			{
				unsigned short nElems		=	FormatUtils::BytesToUInt16(pSegmentInfo, 0, pSegmentInfoSize);
				unsigned short nElemsAlloc	=	FormatUtils::BytesToUInt16(pSegmentInfo, 2, pSegmentInfoSize);
				unsigned short cb			=	FormatUtils::BytesToUInt16(pSegmentInfo, 4, pSegmentInfoSize);
				
				unsigned short cbElement = 4;

				int offset = 6;
				if (cb == 0xfff0)
					cbElement = 2;

				if (nElems == 0)
				{
					nElems = (pSegmentInfoSize - offset) / cbElement;
				}
				for (unsigned short i = 0; i < nElems; ++i)
				{
					PathSegment oSegment = PathSegment(FormatUtils::BytesToInt32(pSegmentInfo + offset, (i * cbElement), pSegmentInfoSize - offset));
					m_arSegments.push_back (oSegment);
				}

				if ((long)pSegmentInfoSize < (long)(cb * nElems))		// Есть несколько файлов с мусором вместо данных
					m_arSegments.clear();
			}

			if ((NULL != pVertices) && (pVerticesSize > 0))
			{
				unsigned short nElems		=	FormatUtils::BytesToUInt16(pVertices, 0, pVerticesSize);
				unsigned short nElemsAlloc	=	FormatUtils::BytesToUInt16(pVertices, 2, pVerticesSize);
				unsigned short cb			=	FormatUtils::BytesToUInt16(pVertices, 4, pVerticesSize);

				unsigned short cbElement = 4;

				if (cb == 0xfff0)
					cbElement = 2;

				int offset = 6;

				for (unsigned short i = 0; i < nElems; ++i)
				{				
					POINT point;	
					if (cbElement == 4)
					{
						point.x	= FormatUtils::BytesToInt32(pVertices + offset, 0, pVerticesSize - offset);
						point.y	= FormatUtils::BytesToInt32(pVertices + offset + cbElement, 0 , pVerticesSize - offset);
					}
					else
					{
						point.x	= FormatUtils::BytesToInt16(pVertices + offset, 0, pVerticesSize - offset);
						point.y	= FormatUtils::BytesToInt16(pVertices + offset + cbElement, 0 , pVerticesSize - offset);
					}
					offset += cbElement * 2;

					LONG lMinF = (LONG)0x80000000;
					if (lMinF <= point.x)
					{
						int index = (DWORD)point.x - 0x80000000;
						if (index >= 0 && index < (int)guides.size())
						{
							point.x = guides[index].param3;
						}
					}
					if (lMinF <= point.y)
					{
						int index = (DWORD)point.y - 0x80000000;
						if (index >= 0 && index < (int)guides.size())
						{
							point.y = guides[index].param3;
						}
					}
					if ((size_t)point.y > 0xffff)
					{
						point.y &= 0xffff;
					}
					if ((size_t)point.x > 0xffff)
					{
						point.x &= 0xffff;
					}
					m_arPoints.push_back(point);
				}
			}
		}

		inline std::wstring GetVmlPath () const
		{
			if ((0 == m_arSegments.size()) && (0 == m_arPoints.size()))
				return std::wstring(L"");

			std::wstring strVmlPath;
			int valuePointer = 0;

			if (0 == m_arSegments.size())
			{
				for (size_t i = 0; i < m_arPoints.size(); ++i)
				{
                    strVmlPath += L"l";
					strVmlPath += FormatUtils::IntToWideString(m_arPoints[i].x);
                    strVmlPath += L",";
					strVmlPath += FormatUtils::IntToWideString(m_arPoints[i].y);
					
					++valuePointer;
				}

                strVmlPath += L"xe";

				return strVmlPath;
			}
			
			int cc = 0;
			std::vector<PathSegment>::const_iterator end = m_arSegments.end();
			for (std::vector<PathSegment>::const_iterator iter = m_arSegments.begin(); iter != end; ++iter, cc++)
			{
				switch (iter->Type)
				{
				case PathSegment::msopathLineTo:
					{
						for (int i = 0; i < iter->Count; ++i)
						{
							if (valuePointer + 1 > (int)m_arPoints.size())
							{
								break;

                                strVmlPath += L"l";
								strVmlPath += FormatUtils::IntToWideString(m_arPoints[0].x);
                                strVmlPath += L",";
								strVmlPath += FormatUtils::IntToWideString(m_arPoints[0].y);
								
								++valuePointer;
								
								//break;
							}
							else
							{
                                strVmlPath += L"l";
								strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].x );
                                strVmlPath += L",";
								strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].y );
								
								++valuePointer;
							}
						}
					}
					break;

				case PathSegment::msopathCurveTo:
					{
						for (int i = 0; i < iter->Count; ++i)
						{
							if (valuePointer + 3 > (int)m_arPoints.size()) 
								break;
                            strVmlPath += L"c";
							strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].x );
                            strVmlPath += L",";
							strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].y );
                            strVmlPath += L",";
							strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer + 1].x );
                            strVmlPath += L",";
							strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer + 1].y );
                            strVmlPath += L",";
							strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer + 2].x );
                            strVmlPath += L",";
							strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer + 2].y );
							valuePointer += 3;
						}
					}
					break;

				case PathSegment::msopathMoveTo:
					{
						if (valuePointer < (int)m_arPoints.size()) 
						{
							strVmlPath += L"m";
							strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].x );
							strVmlPath += L",";
							strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].y );
							
							++valuePointer;
						}
					}
					break;

				case PathSegment::msopathClose:
					{
                        strVmlPath += L"x";
					}
					break;

				case PathSegment::msopathEnd:
					{
                        strVmlPath += L"e";
					}
					break;

				case PathSegment::msopathEscape:
					{
						if (PathSegment::msopathEscapeNoFill == iter->EscapeCode)
                            strVmlPath += L"nf";
						
						if (PathSegment::msopathEscapeNoLine == iter->EscapeCode)
                            strVmlPath += L"ns";
					}
				case PathSegment::msopathClientEscape:
				case PathSegment::msopathInvalid:
					{
						//ignore escape segments and invalid segments
					}
					break;
				}
			}

			// end the path
            if ( !strVmlPath.empty() && ( strVmlPath[strVmlPath.size() - 1] != L'e' ) )
                strVmlPath +=L"e";

			return strVmlPath;
		}

	private:

		std::vector<POINT>			m_arPoints;
		std::vector<PathSegment>	m_arSegments;
	};
}
