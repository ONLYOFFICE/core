#pragma once   
#include "precompiled_cpodf.h"
#include "svg_parser.h"


namespace cpdoccore 
{ 
namespace svg_path
{
    void skipSpaces(int&          io_rPos, const std::wstring&     rStr, const int     nLen)
    {
        while( io_rPos < nLen && L' ' == rStr[io_rPos] )
        {
            ++io_rPos;
        }
    }

    void skipSpacesAndCommas(int&         io_rPos,const std::wstring&    rStr,const int    nLen)
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

	bool getDoubleChar(double&  o_fRetval,int &  io_rPos, const std::wstring&      rStr)
	{
		wchar_t aChar( rStr[io_rPos] );
		std::wstring  sNumberString;
		bool separator_seen=false;
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

		if(!sNumberString.empty())
		{
			o_fRetval = boost::lexical_cast<double>(sNumberString);
			return true;
		}

		return false;
	}

	bool importDoubleAndSpaces( double&             o_fRetval, int&          io_rPos, const std::wstring&     rStr, const int    nLen )
	{
		if( !getDoubleChar(o_fRetval, io_rPos, rStr) )
			return false;

		skipSpacesAndCommas(io_rPos, rStr, nLen);

		return true;
	}

	bool importFlagAndSpaces(int&         o_nRetval, int&         io_rPos, const std::wstring&    rStr, const int    nLen)
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

	void putNumberChar( std::wstring & rStr, double          fValue )
	{
		const std::wstring sValue = boost::lexical_cast<std::wstring>(fValue) ;
		rStr.append(sValue);
	}

	void putNumberCharWithSpace( std::wstring &    rStr,  double             fValue,
									 double                 fOldValue,  bool                   bUseRelativeCoordinates )
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


	bool parseSvgD(std::vector<_polyline> & Polyline, const std::wstring &  rSvgDStatement, bool bWrongPositionAfterZ)
    {
        Polyline.clear();
        const int nLen(rSvgDStatement.length());
        int nPos(0);
        bool bIsClosed(false);
        
		double nLastX( 0.0 );
        double nLastY( 0.0 );
		
		double nLastControlX( 0.0 );
 		double nLastControlY( 0.0 );
		
		_polyline aCurrPoly;

        // skip initial whitespace
        skipSpaces(nPos, rSvgDStatement, nLen);

        while(nPos < nLen)
        {
            bool bRelative(false);
            bool bMoveTo(false);
            const wchar_t aCurrChar(rSvgDStatement[nPos]);

            switch(aCurrChar)
            {
                case 'z' :
                case 'Z' :
                {
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

                    // remember closed state of current polygon
                    bIsClosed = true;

                    // update current point - we're back at the start
                    if( aCurrPoly.points.size()>0 && !bWrongPositionAfterZ)
                    {
                        const _point aFirst( aCurrPoly.points[0]);
						nLastX = aFirst.x.get();
						nLastY = aFirst.y.get();
                    }
                    break;
                }

                case 'm' :
                case 'M' :
                {
                    bMoveTo = true;
                    // FALLTHROUGH intended
                }
                case 'l' :
                case 'L' :
                {
                    if('m' == aCurrChar || 'l' == aCurrChar)
                    {
                        bRelative = true;
                    }
					aCurrPoly.command=L"a:lnTo";

                    if(bMoveTo)
                    {
 						aCurrPoly.command=L"a:moveTo";
                       // new polygon start, finish old one
                        if(aCurrPoly.points.size()>0)
                        {
                            // add current polygon
                            if(bIsClosed)
                            {
                                //closeWithGeometryChange(aCurrPoly);
                            }
                            Polyline.push_back(aCurrPoly);

                            // reset import values
                            bIsClosed = false;
							aCurrPoly.points.clear();
                        }
                    }

                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

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
                        aCurrPoly.points.push_back(_point(nX, nY));
                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();                   
					}
                    break;
                }

                case 'h' :
                {
                    bRelative = true;
                    // FALLTHROUGH intended
                }
                case 'H' :
                {
					aCurrPoly.command=L"a:lnTo";
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
                        aCurrPoly.points.push_back(_point(nX, nY));
                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();            
					}
                    break;
                }

                case 'v' :
                {
                    bRelative = true;
                    // FALLTHROUGH intended
                }
                case 'V' :
                {
					aCurrPoly.command =L"a:v";
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
						aCurrPoly.points.push_back(_point(nX, nY));
                        Polyline.push_back(aCurrPoly);
						aCurrPoly.points.clear();   
					}
                    break;
                }

                case 's' :
                {
                    bRelative = true;
                    // FALLTHROUGH intended
                }
                case 'S' :
                {
					aCurrPoly.command=L"a:cubicBezTo";
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

                    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                    {
                        double nX, nY;
                        double nX2, nY2;
                        double nX1, nY1;

                        if(!importDoubleAndSpaces(nX2, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nY2, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nX, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nY, nPos, rSvgDStatement, nLen)) return false;

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
                    break;
                }

                case 'c' :
                {
                    bRelative = true;
                    // FALLTHROUGH intended
                }
                case 'C' :
                {
					aCurrPoly.command = L"a:cubicBezTo";
                    nPos++;
                    skipSpaces(nPos, rSvgDStatement, nLen);

                    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                    {
                        double nX, nY;
                        double nX1, nY1;
                        double nX2, nY2;

                        if(!importDoubleAndSpaces(nX1, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nY1, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nX2, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nY2, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nX, nPos, rSvgDStatement, nLen)) return false;
                        if(!importDoubleAndSpaces(nY, nPos, rSvgDStatement, nLen)) return false;

                        if(bRelative)
                        {
                            nX1 += nLastX;
                            nY1 += nLastY;
                            nX2 += nLastX;
                            nY2 += nLastY;
                            nX += nLastX;
                            nY += nLastY;
                        }

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
                    break;
                }

                // #100617# quadratic beziers are imported as cubic ones
                //case 'q' :
                //{
                //    bRelative = true;
                //    // FALLTHROUGH intended
                //}
                //case 'Q' :
                //{
                //    nPos++;
                //    skipSpaces(nPos, rSvgDStatement, nLen);

                //    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                //    {
                //        double nX, nY;
                //        double nX1, nY1;

                //        if(!importDoubleAndSpaces(nX1, nPos, rSvgDStatement, nLen)) return false;
                //        if(!importDoubleAndSpaces(nY1, nPos, rSvgDStatement, nLen)) return false;
                //        if(!importDoubleAndSpaces(nX, nPos, rSvgDStatement, nLen)) return false;
                //        if(!importDoubleAndSpaces(nY, nPos, rSvgDStatement, nLen)) return false;

                //        if(bRelative)
                //        {
                //            nX1 += nLastX;
                //            nY1 += nLastY;
                //            nX += nLastX;
                //            nY += nLastY;
                //        }

                //        // calculate the cubic bezier coefficients from the quadratic ones
                //        const double nX1Prime((nX1 * 2.0 + nLastX) / 3.0);
                //        const double nY1Prime((nY1 * 2.0 + nLastY) / 3.0);
                //        const double nX2Prime((nX1 * 2.0 + nX) / 3.0);
                //        const double nY2Prime((nY1 * 2.0 + nY) / 3.0);

                //        // ensure existance of start point
                //        if(!aCurrPoly.size())
                //        {
                //            aCurrPoly.append(_point(nLastX, nLastY));
                //        }

                //        // append curved edge
                //        aCurrPoly.appendBezierSegment(_point(nX1Prime, nY1Prime), _point(nX2Prime, nY2Prime), _point(nX, nY));

                //        // set last position
                //        nLastX = nX;
                //        nLastY = nY;
                //    }
                //    break;
                //}

                //// #100617# relative quadratic beziers are imported as cubic
                //case 't' :
                //{
                //    bRelative = true;
                //    // FALLTHROUGH intended
                //}
                //case 'T' :
                //{
                //    nPos++;
                //    skipSpaces(nPos, rSvgDStatement, nLen);

                //    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                //    {
                //        double nX, nY;

                //        if(!importDoubleAndSpaces(nX, nPos, rSvgDStatement, nLen)) return false;
                //        if(!importDoubleAndSpaces(nY, nPos, rSvgDStatement, nLen)) return false;

                //        if(bRelative)
                //        {
                //            nX += nLastX;
                //            nY += nLastY;
                //        }

                //        // ensure existance of start point
                //        if(!aCurrPoly.size())
                //        {
                //            aCurrPoly.append(_point(nLastX, nLastY));
                //        }

                //        // get first control point. It's the reflection of the PrevControlPoint
                //        // of the last point. If not existent, use current point (see SVG)
                //        _point aPrevControl(_point(nLastX, nLastY));
                //        const sal_uInt32 nIndex(aCurrPoly.size() - 1);
                //        const _point aPrevPoint(aCurrPoly.getB2DPoint(nIndex));

                //        if(aCurrPoly.areControlPointsUsed() && aCurrPoly.isPrevControlPointUsed(nIndex))
                //        {
                //            const _point aPrevControlPoint(aCurrPoly.getPrevControlPoint(nIndex));

                //            // use mirrored previous control point
                //            aPrevControl.setX((2.0 * aPrevPoint.getX()) - aPrevControlPoint.getX());
                //            aPrevControl.setY((2.0 * aPrevPoint.getY()) - aPrevControlPoint.getY());
                //        }

                //        if(!aPrevControl.equal(aPrevPoint))
                //        {
                //            // there is a prev control point, and we have the already mirrored one
                //            // in aPrevControl. We also need the quadratic control point for this
                //            // new quadratic segment to calculate the 2nd cubic control point
                //            const _point aQuadControlPoint(
                //                ((3.0 * aPrevControl.getX()) - aPrevPoint.getX()) / 2.0,
                //                ((3.0 * aPrevControl.getY()) - aPrevPoint.getY()) / 2.0);

                //            // calculate the cubic bezier coefficients from the quadratic ones.
                //            const double nX2Prime((aQuadControlPoint.getX() * 2.0 + nX) / 3.0);
                //            const double nY2Prime((aQuadControlPoint.getY() * 2.0 + nY) / 3.0);

                //            // append curved edge, use mirrored cubic control point directly
                //            aCurrPoly.appendBezierSegment(aPrevControl, _point(nX2Prime, nY2Prime), _point(nX, nY));
                //        }
                //        else
                //        {
                //            // when no previous control, SVG says to use current point -> straight line.
                //            // Just add end point
                //            aCurrPoly.append(_point(nX, nY));
                //        }

                //        // set last position
                //        nLastX = nX;
                //        nLastY = nY;
                //    }
                //    break;
                //}

                //case 'a' :
                //{
                //    bRelative = true;
                //    // FALLTHROUGH intended
                //}
                //case 'A' :
                //{
                //    nPos++;
                //    skipSpaces(nPos, rSvgDStatement, nLen);

                //    while(nPos < nLen && isOnNumberChar(rSvgDStatement, nPos))
                //    {
                //        double nX, nY;
                //        double fRX, fRY, fPhi;
                //        int bLargeArcFlag, bSweepFlag;

                //        if(!importDoubleAndSpaces(fRX, nPos, rSvgDStatement, nLen)) return false;
                //        if(!importDoubleAndSpaces(fRY, nPos, rSvgDStatement, nLen)) return false;
                //        if(!importDoubleAndSpaces(fPhi, nPos, rSvgDStatement, nLen)) return false;
                //        if(!importFlagAndSpaces(bLargeArcFlag, nPos, rSvgDStatement, nLen)) return false;
                //        if(!importFlagAndSpaces(bSweepFlag, nPos, rSvgDStatement, nLen)) return false;
                //        if(!importDoubleAndSpaces(nX, nPos, rSvgDStatement, nLen)) return false;
                //        if(!importDoubleAndSpaces(nY, nPos, rSvgDStatement, nLen)) return false;

                //        if(bRelative)
                //        {
                //            nX += nLastX;
                //            nY += nLastY;
                //        }

                //        if( nX == nLastX && nY == nLastY )
                //            continue; // start==end -> skip according to SVG spec

                //        if( fRX == 0.0 || fRY == 0.0 )
                //        {
                //            // straight line segment according to SVG spec
                //            aCurrPoly.append(_point(nX, nY));
                //        }
                //        else
                //        {
                //            // normalize according to SVG spec
                //            fRX=fabs(fRX); fRY=fabs(fRY);

                //            // from the SVG spec, appendix F.6.4

                //            // |x1'|   |cos phi   sin phi|  |(x1 - x2)/2|
                //            // |y1'| = |-sin phi  cos phi|  |(y1 - y2)/2|
                //            const _point p1(nLastX, nLastY);
                //            const _point p2(nX, nY);
                //            B2DHomMatrix aTransform(basegfx::tools::createRotateB2DHomMatrix(-fPhi*M_PI/180));

                //            const _point p1_prime( aTransform * _point(((p1-p2)/2.0)) );

                //            //           ______________________________________       rx y1'
                //            // |cx'|  + /  rx^2 ry^2 - rx^2 y1'^2 - ry^2 x1^2           ry
                //            // |cy'| =-/       rx^2y1'^2 + ry^2 x1'^2               - ry x1'
                //            //                                                          rx
                //            // chose + if f_A != f_S
                //            // chose - if f_A  = f_S
                //            _point aCenter_prime;
                //            const double fRadicant(
                //                (fRX*fRX*fRY*fRY - fRX*fRX*p1_prime.getY()*p1_prime.getY() - fRY*fRY*p1_prime.getX()*p1_prime.getX())/
                //                (fRX*fRX*p1_prime.getY()*p1_prime.getY() + fRY*fRY*p1_prime.getX()*p1_prime.getX()));
                //            if( fRadicant < 0.0 )
                //            {
                //                // no solution - according to SVG
                //                // spec, scale up ellipse
                //                // uniformly such that it passes
                //                // through end points (denominator
                //                // of radicant solved for fRY,
                //                // with s=fRX/fRY)
                //                const double fRatio(fRX/fRY);
                //                const double fRadicant2(
                //                    p1_prime.getY()*p1_prime.getY() +
                //                    p1_prime.getX()*p1_prime.getX()/(fRatio*fRatio));
                //                if( fRadicant2 < 0.0 )
                //                {
                //                    // only trivial solution, one
                //                    // of the axes 0 -> straight
                //                    // line segment according to
                //                    // SVG spec
                //                    aCurrPoly.append(_point(nX, nY));
                //                    continue;
                //                }

                //                fRY=sqrt(fRadicant2);
                //                fRX=fRatio*fRY;

                //                // keep center_prime forced to (0,0)
                //            }
                //            else
                //            {
                //                const double fFactor(
                //                    (bLargeArcFlag==bSweepFlag ? -1.0 : 1.0) *
                //                    sqrt(fRadicant));

                //                // actually calculate center_prime
                //                aCenter_prime = _point(
                //                    fFactor*fRX*p1_prime.getY()/fRY,
                //                    -fFactor*fRY*p1_prime.getX()/fRX);
                //            }

                //            //              +           u - v
                //            // angle(u,v) =  arccos( ------------ )     (take the sign of (ux vy - uy vx))
                //            //              -        ||u|| ||v||

                //            //                  1    | (x1' - cx')/rx |
                //            // theta1 = angle((   ), |                | )
                //            //                  0    | (y1' - cy')/ry |
                //            const _point aRadii(fRX,fRY);
                //            double fTheta1(
                //                B2DVector(1.0,0.0).angle(
                //                    (p1_prime-aCenter_prime)/aRadii));

                //            //                 |1|    |  (-x1' - cx')/rx |
                //            // theta2 = angle( | | ,  |                  | )
                //            //                 |0|    |  (-y1' - cy')/ry |
                //            double fTheta2(
                //                B2DVector(1.0,0.0).angle(
                //                    (-p1_prime-aCenter_prime)/aRadii));

                //            // map both angles to [0,2pi)
                //            fTheta1 = fmod(2*M_PI+fTheta1,2*M_PI);
                //            fTheta2 = fmod(2*M_PI+fTheta2,2*M_PI);

                //            // make sure the large arc is taken
                //            // (since
                //            // createPolylineFromEllipseSegment()
                //            // normalizes to e.g. cw arc)
                //            if( !bSweepFlag )
                //                std::swap(fTheta1,fTheta2);

                //            // finally, create bezier polygon from this
                //            B2DPolyline aSegment(
                //                tools::createPolylineFromUnitEllipseSegment(
                //                    fTheta1, fTheta2 ));

                //            // transform ellipse by rotation & move to final center
                //            aTransform = basegfx::tools::createScaleB2DHomMatrix(fRX, fRY);
                //            aTransform.translate(aCenter_prime.getX(),
                //                                 aCenter_prime.getY());
                //            aTransform.rotate(fPhi*M_PI/180);
                //            const _point aOffset((p1+p2)/2.0);
                //            aTransform.translate(aOffset.getX(),
                //                                 aOffset.getY());
                //            aSegment.transform(aTransform);

                //            // createPolylineFromEllipseSegment()
                //            // always creates arcs that are
                //            // positively oriented - flip polygon
                //            // if we swapped angles above
                //            if( !bSweepFlag )
                //                aSegment.flip();
                //            aCurrPoly.append(aSegment);
                //        }

                //        // set last position
                //        nLastX = nX;
                //        nLastY = nY;
                //    }
                //    break;
                //}

                default:
                {
                    //OSL_FAIL("importFromSvgD(): skipping tags in svg:d element (unknown)!");
                    //OSL_TRACE("importFromSvgD(): skipping tags in svg:d element (unknown: \"%c\")!", aCurrChar);
                    ++nPos;
                    break;
                }
            }
        }

        if(aCurrPoly.points.size()>0)
        {
            // end-process last poly
            if(bIsClosed)
            //{
            //    closeWithGeometryChange(aCurrPoly);
            //}

            Polyline.push_back(aCurrPoly);
        }

        return true;
    }

	bool parsePolygon(std::vector<_polyline> & Polyline, const std::wstring &  rPolygonStatement, bool bWrongPositionAfterZ)
    {
        Polyline.clear();

        _polyline aCurrPoly;
		aCurrPoly.command = L"a:moveTo"; //первая точка сдвиг - остальные линия

 		std::vector<std::wstring> points_str;
		
		boost::algorithm::split(points_str,rPolygonStatement, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

		int last_x=0,last_y=0;
		for (long i=0; i< points_str.size();i++)
		{
 			std::vector<std::wstring> point_str;
			boost::algorithm::split(point_str,points_str[i], boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			if (point_str.size()>0)last_x = boost::lexical_cast<int>(point_str[0]);
			if (point_str.size()>1)last_y = boost::lexical_cast<int>(point_str[1]);
			
			aCurrPoly.points.push_back(_point (last_x,last_y));

			Polyline.push_back(aCurrPoly);

			aCurrPoly.points.clear();
			aCurrPoly.command = L"a:lnTo";
		}
		if (Polyline.size()>2)
		{
			//замкнем
			Polyline.push_back(Polyline[0]);
			Polyline.back().command = L"a:lnTo";
		}
		return true;
    }


}
}

