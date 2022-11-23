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

#include "svg_parser.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace svg_path
{
    void skipSpaces(int& io_rPos, const std::wstring& rStr, const int nLen)
    {
        while( io_rPos < nLen && L' ' == rStr[io_rPos] )
        {
            ++io_rPos;
        }
    }
	static void replace_empty_commas(std::wstring& subject)
	{
		const std::wstring& search = L",,";
		const std::wstring& replace = L",0,";
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::wstring::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += 2;
		}
	}
    void skipSpacesAndCommas(int& io_rPos,const std::wstring& rStr,const int nLen)
    {
        while(io_rPos < nLen && (wchar_t(' ') == rStr[io_rPos] || wchar_t(',') == rStr[io_rPos]))
        {
            ++io_rPos;
        }
    }
	inline bool isOnNumberChar(const wchar_t aChar, bool bSignAllowed = true)
	{
		const bool bPredicate	((L'0' <= aChar && L'9' >= aChar)
								|| (bSignAllowed && L'+' == aChar)
							    || (bSignAllowed && L'-' == aChar)
							    || (L'.' == aChar) );

		return bPredicate;
	}

	inline bool isOnNumberChar(const std::wstring& rStr, const int nPos, bool bSignAllowed = true)
	{
		return isOnNumberChar(rStr[nPos],bSignAllowed);
	}

	bool getDoubleChar(double&  o_fRetval,int &  io_rPos, const std::wstring& rStr)
	{
		wchar_t aChar( rStr[io_rPos] );
		std::wstring  sNumberString;
		bool separator_seen = false;
		std::wstring  tmpString;

		if(wchar_t(L'+') == aChar || wchar_t(L'-') == aChar)
		{
			tmpString = rStr[io_rPos];
			sNumberString.append(tmpString);
			aChar = rStr[++io_rPos];
		}

		while((wchar_t(L'0') <= aChar && wchar_t(L'9') >= aChar)
			  || (!separator_seen && wchar_t(L'.') == aChar))
		{
			if (wchar_t(L'.') == aChar) separator_seen = true;
			tmpString = rStr[io_rPos];
			sNumberString.append(tmpString);
			aChar = rStr[++io_rPos];
		}

		if(wchar_t(L'e') == aChar || wchar_t(L'E') == aChar)
		{
			sNumberString.append(&rStr[io_rPos],1);
			aChar = rStr[++io_rPos];

			if(wchar_t(L'+') == aChar || wchar_t(L'-') == aChar)
			{
				tmpString = rStr[io_rPos];
				sNumberString.append(tmpString);
				aChar = rStr[++io_rPos];
			}

			while(wchar_t(L'0') <= aChar && wchar_t(L'9') >= aChar)
			{
				tmpString = rStr[io_rPos];
				sNumberString.append(tmpString);
				aChar = rStr[++io_rPos];
			}
		}
		if (sNumberString.empty()) 
			return true;

		bool result = false;
		try
		{
			o_fRetval = boost::lexical_cast<double>(sNumberString);
			result = true;
		}
		catch (...)
		{
		}

		return result;
	}

	bool importDoubleAndSpaces( double& o_fRetval, int& io_rPos, const std::wstring& rStr, const int nLen )
	{
		o_fRetval = 0;

		if( !getDoubleChar(o_fRetval, io_rPos, rStr) )
			return false;

		skipSpacesAndCommas(io_rPos, rStr, nLen);

		return true;
	}
	bool importDoubleAndSpacesVml(double& o_fRetval, int& io_rPos, const std::wstring& rStr, const int nLen)
	{
		o_fRetval = 0;
		
		if (rStr[io_rPos] == L',') // for first after letter
		{
			io_rPos++;
		}
		else
		{
			if (!getDoubleChar(o_fRetval, io_rPos, rStr))
				return false;
		}
		skipSpacesAndCommas(io_rPos, rStr, nLen);
		return true;
	}

	bool importFlagAndSpaces(int& o_nRetval, int& io_rPos, const std::wstring& rStr, const int nLen)
	{
		wchar_t aChar( rStr[io_rPos] );

		if(wchar_t(L'0') == aChar)
		{
			o_nRetval = 0;
			++io_rPos;
		}
		else if (wchar_t(L'1') == aChar)
		{
			o_nRetval = 1;
			++io_rPos;
		}
		else
			return false;

		skipSpacesAndCommas(io_rPos, rStr, nLen);

		return true;
	}

	void putNumberChar( std::wstring & rStr, double fValue )
	{
		const std::wstring sValue = boost::lexical_cast<std::wstring>(fValue) ;
		rStr.append(sValue);
	}

	void putNumberCharWithSpace( std::wstring & rStr, double fValue, double fOldValue, bool bUseRelativeCoordinates )
	{
		if( bUseRelativeCoordinates )
			fValue -= fOldValue;

		const int aLen( rStr.length() );
		if(aLen > 0)
		{
			if( isOnNumberChar(rStr[aLen - 1], false) && fValue >= 0.0 )
			{
				rStr.append(L" ");
			}
		}

		putNumberChar(rStr, fValue);
	}

	inline wchar_t getCommand( wchar_t cUpperCaseCommand, wchar_t  cLowerCaseCommand,  bool     bUseRelativeCoordinates )
	{
		return bUseRelativeCoordinates ? cLowerCaseCommand : cUpperCaseCommand;
	}

	bool parseSvgD(std::vector<_polyline> & Polyline, const std::wstring & rSvgDStatement, bool bWrongPositionAfterZ, bool & bIsClosed, bool & bStroked)
    {
        Polyline.clear();
        const int nLen(rSvgDStatement.length());
        int nPos(0);
        
		double nLastX( 0.0 );
        double nLastY( 0.0 );
		
		double nLastControlX( 0.0 );
 		double nLastControlY( 0.0 );
		
		_polyline aCurrPoly;

		bIsClosed = false;
		bStroked = true;

        // skip initial whitespace
        skipSpaces(nPos, rSvgDStatement, nLen);

        while(nPos < nLen)
        {
            bool bRelative	(false);
            bool bMoveTo	(false);
            const wchar_t aCurrChar(rSvgDStatement[nPos]);

			aCurrPoly.command.clear();

            switch(aCurrChar)
            {
                case 'z' :
                case 'Z' :
                {
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

                    bIsClosed = true;

                    // update current point - we're back at the start
                    if( aCurrPoly.points.size() > 0 && !bWrongPositionAfterZ)
                    {
                        const _point aFirst( aCurrPoly.points[0]);
						nLastX = aFirst.x.get();
						nLastY = aFirst.y.get();
                    }
 					
					aCurrPoly.command=L"a:close";   
					Polyline.push_back(aCurrPoly);

                } break;
                case 'm' :
                case 'M' :
                {
                    bMoveTo = true;
                }
                case 'l' :
                case 'L' :
                {
                    if('m' == aCurrChar || 'l' == aCurrChar)
                    {
                        bRelative = true;
                    }
					
					// new polygon start, finish old one
                    if(aCurrPoly.points.size() > 0)
                    {
                        // add current polygon
                        if(bIsClosed)
                        {
                            //closeWithGeometryChange(aCurrPoly);
                        }
                        Polyline.push_back(aCurrPoly);

                        // reset import values
                        bIsClosed = false;
						if(bMoveTo)	aCurrPoly.command = L"a:moveTo";
						else		aCurrPoly.command = L"a:lnTo";
						aCurrPoly.points.clear();
                    }
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);
					aCurrPoly.command.clear();

                    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                    {
                        double nX, nY;

                        if(!importDoubleAndSpaces(nX, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nY, nPos, rSvgDStatement, nLen)) return false;

                        if(bRelative)
                        {
                            nX += nLastX;
                            nY += nLastY;
                        }

                        // set last position
                        nLastX = nX;
                        nLastY = nY;

                        // add point
 						if(bMoveTo)	aCurrPoly.command = L"a:moveTo";
						else		aCurrPoly.command = L"a:lnTo";
						
						aCurrPoly.points.push_back(_point(nX, nY));
                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();   
					}    
                }break;
                case 'h' :
                {
                    bRelative = true;
                }
                case 'H' ://горизонт линия
                {
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

                    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                    {
                        double nX, nY(nLastY);

                        if(!importDoubleAndSpaces(nX, nPos, rSvgDStatement, nLen)) return false;

                        if(bRelative)
                        {
                            nX += nLastX;
                        }

                        // set last position
                        nLastX = nX;

                        // add point
						aCurrPoly.command = L"a:lnTo";
                        aCurrPoly.points.push_back(_point(nX, nY));
                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();            
					}
                    break;
                }

                case 'v' :
                {
                    bRelative = true;
                }
                case 'V' ://вертикальная линия
                {
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

                    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                    {
                        double nX(nLastX), nY;

                        if(!importDoubleAndSpaces(nY, nPos, rSvgDStatement, nLen)) return false;

                        if(bRelative)
                        {
                            nY += nLastY;
                        }

                        // set last position
                        nLastY = nY;

                        // add point
						aCurrPoly.command = L"a:lnTo";
						aCurrPoly.points.push_back(_point(nX, nY));
                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();   
					}
                    break;
                }

                case 's' :
                {
                    bRelative = true;
                }
                case 'S' :
                {
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

					bool bPresentNumber = false;
                    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                    {
						bPresentNumber = true;
                        double nX, nY;
                        double nX2, nY2;
                        double nX1, nY1;

                        if(!importDoubleAndSpaces(nX2, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nY2, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nX, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpaces(nY, nPos, rSvgDStatement, nLen))	return false;

                        if(bRelative)
                        {
                            nX2 += nLastX;
                            nY2 += nLastY;
                            nX += nLastX;
                            nY += nLastY;
                        }

                        // use mirrored previous control point
                        nX1 =((2.0 * nLastX) - nLastControlX);
                        nY1 =((2.0 * nLastY) - nLastControlY);
						
						aCurrPoly.command = L"a:cubicBezTo";
						// append curved edge
						aCurrPoly.points.push_back(_point(nX1, nY1));
						aCurrPoly.points.push_back(_point(nX2, nY2));
						aCurrPoly.points.push_back(_point(nX, nY));
						
                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();   

                        // set last position
                        nLastX = nX;
                        nLastY = nY;
 						
						//keep control point
						nLastControlX = nX2;
						nLastControlY = nY2;            
					} 
					if (false == bPresentNumber)
					{
						bStroked = false;
					}
                }break;

                case 'c' :
                {
                    bRelative = true;
                }
                case 'C' :
                {
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

                    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                    {
                        double nX, nY;
                        double nX1, nY1;
                        double nX2, nY2;

						if(!importDoubleAndSpaces(nX1, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpaces(nY1, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpaces(nX2, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpaces(nY2, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpaces(nX, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpaces(nY, nPos, rSvgDStatement, nLen))	return false;

                        if(bRelative)
                        {
                            nX1 += nLastX;
                            nY1 += nLastY;
                            nX2 += nLastX;
                            nY2 += nLastY;
                            nX += nLastX;
                            nY += nLastY;
                        }

 						aCurrPoly.command = L"a:cubicBezTo";
                       // append curved edge
						aCurrPoly.points.push_back(_point(nX1, nY1));
						aCurrPoly.points.push_back(_point(nX2, nY2));
						aCurrPoly.points.push_back(_point(nX, nY));

                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();   

                        // set last position
                        nLastX = nX;
                        nLastY = nY;

						//keep control point
						nLastControlX = nX2;
						nLastControlY = nY2;
                    }                    
                }break;

				case 'G':
				{
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

                    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                    {
                        double nX, nY;
                        double A1, A2;

						if(!importDoubleAndSpaces(nX, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpaces(nY, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpaces(A1, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpaces(A2, nPos, rSvgDStatement, nLen))	return false;

                        if(bRelative)
                        {
                            nX += nLastX;
                            nY += nLastY;
                        }

 						aCurrPoly.command = L"a:ArcTo";
                       // append curved edge
						aCurrPoly.points.push_back(_point(nX, nY));
						aCurrPoly.points.push_back(_point(A1, A2));

                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();   

                        // set last position
                        nLastX = nX;
                        nLastY = nY;

						//keep control point
						nLastControlX = nX;
						nLastControlY = nY;
                    }                    
				}break;
                default:
                {
                     ++nPos;
                    break;
                }
            }
        }

        if ((aCurrPoly.points.size() > 0/* || !bIsClosed*/) && !aCurrPoly.command.empty() && aCurrPoly.command != L"a:cubicBezTo")
        {
            // end-process last poly

            Polyline.push_back(aCurrPoly);
        }

        return true;
    }

	bool parsePolygon(std::vector<_polyline> & Polyline, const std::wstring &  rPolygonStatement, bool bWrongPositionAfterZ, bool closed)
    {
        Polyline.clear();

        _polyline aCurrPoly;
		aCurrPoly.command = L"a:moveTo"; //первая точка сдвиг - остальные линия

 		std::vector<std::wstring> points_str;
		
		boost::algorithm::split(points_str,rPolygonStatement, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

		int last_x = 0, last_y = 0;
		for (size_t i = 0; i < points_str.size(); i++)
		{
 			std::vector<std::wstring> point_str;
			boost::algorithm::split(point_str, points_str[i], boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			if (point_str.size()>0)last_x = boost::lexical_cast<int>(point_str[0]);
			if (point_str.size()>1)last_y = boost::lexical_cast<int>(point_str[1]);
			
			aCurrPoly.points.push_back(_point (last_x,last_y));

			Polyline.push_back(aCurrPoly);

			aCurrPoly.points.clear();
			aCurrPoly.command = L"a:lnTo";
		}
		if (Polyline.size() > 2 && closed)
		{
			//замкнем
			Polyline.push_back(Polyline[0]);
			Polyline.back().command = L"a:lnTo";
		}
		if (closed) 
		{
			aCurrPoly.points.clear();
			aCurrPoly.command = L"a:close";
			
			Polyline.push_back(aCurrPoly);
		}
		return true;
    }


	bool parseVml(std::vector<_polyline> & Polyline, const std::wstring & VmlPath)
	{
        Polyline.clear();
		
		std::wstring rSvgDStatement = VmlPath;
		
		replace_empty_commas(rSvgDStatement);
		
		int nLen(rSvgDStatement.length());
        int nPos(0);
        bool bIsClosed(false);

		double nLastX( 0.0 );
        double nLastY( 0.0 );
		
		double nLastControlX( 0.0 );
 		double nLastControlY( 0.0 );
		
		_polyline aCurrPoly;

        skipSpaces(nPos, rSvgDStatement, nLen);

        while(nPos < nLen)
        {
            bool bRelative	(false);
            bool bMoveTo	(false);
            const wchar_t aCurrChar(rSvgDStatement[nPos]);

            switch(aCurrChar)
            {
                case 'x' :
                {
                    nPos++;
                    bIsClosed = true;

					if (aCurrPoly.points.size() > 0)
					{
						Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();
					}              
				} break;
                case 'm' :
                case 't' :
                {
                    bMoveTo = true;
                }
                case 'l' :
                case 'r' :
                {
                    if('t' == aCurrChar || 'r' == aCurrChar)
                    {
                        bRelative = true;
                    }
					if (bMoveTo)	aCurrPoly.command = L"m";
					else			aCurrPoly.command = L"l";

                    if(aCurrPoly.points.size() > 0)
                    {
                        if(bIsClosed)
                        {
                        }
                        Polyline.push_back(aCurrPoly);

                        bIsClosed = false;						
						
						aCurrPoly.points.clear();
                    }
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);
					
					while ((nPos < nLen) && (isOnNumberChar(rSvgDStatement, nPos) || rSvgDStatement[nPos] == L','))
                    {
                        double nX, nY;

						if (!importDoubleAndSpacesVml(nX, nPos, rSvgDStatement, nLen))	return false;
						if (!importDoubleAndSpacesVml(nY, nPos, rSvgDStatement, nLen))	return false;

                        if(bRelative)
                        {
                            nX += nLastX;
                            nY += nLastY;
                        }

                        nLastX = nX;
                        nLastY = nY;

						aCurrPoly.points.push_back(_point(nX, nY));
                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();   
					}    
                }break;

				case 'v' :
					bRelative = true;
				case 'c' :
                {
					nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

                    while ((nPos < nLen) && (isOnNumberChar(rSvgDStatement, nPos) || rSvgDStatement[nPos] == L','))
                    {
                        double nX, nY;
                        double nX1, nY1;
                        double nX2, nY2;

						if(!importDoubleAndSpacesVml(nX1, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpacesVml(nY1, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpacesVml(nX2, nPos, rSvgDStatement, nLen))	return false;
                        
						if(!importDoubleAndSpacesVml(nY2, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpacesVml(nX, nPos, rSvgDStatement, nLen))	return false;
                        if(!importDoubleAndSpacesVml(nY, nPos, rSvgDStatement, nLen))	return false;

                        if(bRelative)
                        {
                            nX1 += nLastX;
                            nY1 += nLastY;
                            nX2 += nLastX;
                            nY2 += nLastY;
                            nX += nLastX;
                            nY += nLastY;
                        }
						aCurrPoly.command = L"c";

						aCurrPoly.points.push_back(_point(nX1, nY1));
						aCurrPoly.points.push_back(_point(nX2, nY2));
						aCurrPoly.points.push_back(_point(nX, nY));

                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();   

                        nLastX = nX;
                        nLastY = nY;

						nLastControlX = nX2;
						nLastControlY = nY2;
                    }                    
                }break;

                 default:
                {
					 aCurrPoly.command.clear();
					 ++nPos;
                    break;
                }
            }
        }
        return true;
    }



}

