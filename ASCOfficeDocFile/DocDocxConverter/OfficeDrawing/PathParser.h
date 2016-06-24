/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
	class PathParser
	{
	public:

		PathParser (const unsigned char* pSegmentInfo, unsigned int pSegmentInfoSize, const unsigned char* pVertices, unsigned int pVerticesSize)
		{
			if ((pSegmentInfo != NULL) && (pSegmentInfoSize > 0))
			{
				int offset					=	6;

				unsigned short nElems		=	FormatUtils::BytesToUInt16(pSegmentInfo, 0, pSegmentInfoSize);
				unsigned short nElemsAlloc	=	FormatUtils::BytesToUInt16(pSegmentInfo, 2, pSegmentInfoSize);
				unsigned short cb			=	FormatUtils::BytesToUInt16(pSegmentInfo, 4, pSegmentInfoSize);
				
				// видимо без шапки сразу пишутся все элементы
				bool headerIs = ((0xfff0 != cb) && (cb > 8) || nElems > nElemsAlloc);
				if (headerIs)
				{	
					cb		=	2;
					offset	=	0;
					nElems	=	pSegmentInfoSize / 2;

					for (unsigned short i = 0; i < nElems; ++i)
					{
						PathSegment oSegment = PathSegment(FormatUtils::BytesToUInt16(pSegmentInfo + offset, (i * cb), pSegmentInfoSize));
						m_arSegments.push_back (oSegment);
					}
				}
				else
				{
					if ((0xfff0 != cb) && (8 != cb) && (4 != cb) && (2 != cb))
					{
						cb		=	0xfff0;
						offset	=	0;
						nElems	=	pSegmentInfoSize / 2;
					}

					if (0xfff0 == cb)	
					{
						cb = 4;

						for (unsigned short i = 0; i < nElems; ++i)
						{
							PathSegment oSegment = PathSegment(FormatUtils::BytesToUInt16(pSegmentInfo + offset, (i * cb), pSegmentInfoSize));
							m_arSegments.push_back (oSegment);
						}
					}
					else
					{
						if ((2 != cb) && (1 != cb))
							cb = 8;

						for (unsigned short i = 0; i < nElems; ++i)
						{
							PathSegment oSegment = PathSegment(FormatUtils::BytesToInt32(pSegmentInfo + offset, (i * cb), pSegmentInfoSize));
							m_arSegments.push_back (oSegment);
						}

						if ((long)pSegmentInfoSize < (long)(cb*nElems))		// Есть несколько файлов с мусором вместо данных
							m_arSegments.clear();
					}
				}
			}

			if ((NULL != pVertices) && (pVerticesSize > 0))
			{
				unsigned short nElems		=	FormatUtils::BytesToUInt16(pVertices, 0, pVerticesSize);
				unsigned short nElemsAlloc	=	FormatUtils::BytesToUInt16(pVertices, 2, pVerticesSize);
				unsigned short cb			=	FormatUtils::BytesToUInt16(pVertices, 4, pVerticesSize);

				if (0xfff0 == cb)
				{
					cb	=	4;

					for (unsigned short i = 0; i < nElems; ++i)
					{
						POINT point;

						point.x	= FormatUtils::BytesToInt16(pVertices + 6, (i * cb), pVerticesSize);
						point.y	= FormatUtils::BytesToInt16(pVertices + 6, (i * cb) + (cb / 2), pVerticesSize);

						m_arPoints.push_back(point);
					}
				}
				else
				{
					for (unsigned short i = 0; i < nElems; ++i)
					{
						POINT point;

						point.x	= FormatUtils::BytesToInt32(pVertices + 6, (i * cb), pVerticesSize);
						point.y	= FormatUtils::BytesToInt32(pVertices + 6, (i * cb) + (cb / 2), pVerticesSize);

						m_arPoints.push_back(point);
					}
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
					strVmlPath += _T( "l" );
					strVmlPath += FormatUtils::IntToWideString(m_arPoints[i].x);
					strVmlPath += _T( "," );
					strVmlPath += FormatUtils::IntToWideString(m_arPoints[i].y);
					
					++valuePointer;
				}

				strVmlPath += _T( "xe" );

				return strVmlPath;
			}
			
			int cc = 0;
			std::vector<PathSegment>::const_iterator end = m_arSegments.end();
			for (vector<PathSegment>::const_iterator iter = m_arSegments.begin(); iter != end; ++iter, cc++)
			{
				try
				{
					switch (iter->Type)
					{
					case PathSegment::msopathLineTo:
						{
							for (int i = 0; i < iter->Count; ++i)
							{
								if (valuePointer >= (int)m_arPoints.size())
								{
									break;

									strVmlPath += _T( "l" );
									strVmlPath += FormatUtils::IntToWideString(m_arPoints[0].x);
									strVmlPath += _T( "," );
									strVmlPath += FormatUtils::IntToWideString(m_arPoints[0].y);
									
									++valuePointer;
									
									//break;
								}
								else
								{
									strVmlPath += _T( "l" );
									strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].x );
									strVmlPath += _T( "," );
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
								strVmlPath += _T( "c" );
								strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].x );
								strVmlPath += _T( "," );
								strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].y );
								strVmlPath += _T( "," );
								strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer + 1].x );
								strVmlPath += _T( "," );
								strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer + 1].y );
								strVmlPath += _T( "," );
								strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer + 2].x );
								strVmlPath += _T( "," );
								strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer + 2].y );
								valuePointer += 3;
							}
						}
						break;

					case PathSegment::msopathMoveTo:
						{
							strVmlPath += _T( "m" );
							strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].x );
							strVmlPath += _T( "," );
							strVmlPath += FormatUtils::IntToWideString(m_arPoints[valuePointer].y );
							
							++valuePointer;
						}
						break;

					case PathSegment::msopathClose:
						{
							strVmlPath += _T( "x" );
						}
						break;

					case PathSegment::msopathEnd:
						{
							strVmlPath += _T( "e" );
						}
						break;

					case PathSegment::msopathEscape:
						{
							if (PathSegment::msopathEscapeNoFill == iter->EscapeCode)
								strVmlPath += _T( "nf" );
							
							if (PathSegment::msopathEscapeNoLine == iter->EscapeCode)
								strVmlPath += _T( "ns" );
						}
					case PathSegment::msopathClientEscape:
					case PathSegment::msopathInvalid:
						{
							//ignore escape segments and invalid segments
						}
						break;
					}
				}
				catch (...)
				{
					// Sometimes there are more Segments than available m_arPoints.
					// Accordingly to the spec this should never happen :)
					break;
				}
			}

			// end the path
			if ( !strVmlPath.empty() && ( strVmlPath[strVmlPath.size() - 1] != _T( 'e' ) ) )
				strVmlPath += _T("e");

			return strVmlPath;
		}

	private:

		std::vector<POINT>			m_arPoints;
		std::vector<PathSegment>	m_arSegments;
	};
}