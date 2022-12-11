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
#include "Attributes.h"

namespace PPT
{
    enum PointPathType
    {
        etMoveTo		=	0,
        etLineTo		=	1,
        etCurveTo		=	2,
        etCloseLoop		=	3,
        etHorzTo		=	4,
        etVertTo		=	5,
        etEnd			=	6

        // add arc, c3, c2 ????
    };

    enum EffectType
    {
        FadeInType		=	1,
        FadeExitType	=	2,
        EmphasisEffect	=	3,
        MotionEffect	=	4,
        VerbEffect		=	5,
        MediaCallEffect	=	6
    };

    class CMotionPath
    {
    private:

        struct PathPoint
        {
            int Type;

            double X [ 4 ];
            double Y [ 4 ];
        };

    public:

//        inline bool FromStr (const std::wstring& Str)
//        {
//            m_Points.clear();

//            int To		=	0;
//            std::wstring Tokens	=	_T(" mlczevh");

//            double dEndX	=	0.0;
//            double dEndY	=	0.0;

//            PathPoint oPoint;

//            for ( int i = 0; i < Str.length (); ++i )
//            {
//                if ( _T('m') == Str [ i ] )		oPoint.Type =	etMoveTo;
//                else if ( _T('l') == Str [ i ] )	oPoint.Type =	etLineTo;
//                else if ( _T('c') == Str [ i ] )	oPoint.Type =	etCurveTo;
//                else if ( _T('z') == Str [ i ] )	oPoint.Type =	etCloseLoop;		//	This action requires no points.
//                else if ( _T('e') == Str [ i ] )	oPoint.Type =	etEnd;				//	This action requires no points.
//                else if ( _T('h') == Str [ i ] )	oPoint.Type =	etHorzTo;			//
//                else if ( _T('v') == Str [ i ] )	oPoint.Type =	etVertTo;			//
//                else if ( _T('M') == Str [ i ] )	oPoint.Type =	etMoveTo;
//                else if ( _T('L') == Str [ i ] )	oPoint.Type =	etLineTo;
//                else if ( _T('C') == Str [ i ] )	oPoint.Type =	etCurveTo;
//                else if ( _T('Z') == Str [ i ] )	oPoint.Type =	etCloseLoop;
//                else if ( _T('E') == Str [ i ] )	oPoint.Type =	etEnd;
//                else continue;

//                To	=	i + 1;

//                if ( etMoveTo == oPoint.Type )
//                {
//                    oPoint.X [ 0 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;
//                    oPoint.Y [ 0 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;

//                    dEndX			=	oPoint.X [ 0 ];
//                    dEndY			=	oPoint.Y [ 0 ];

//                    continue;
//                }

//                if ( etLineTo == oPoint.Type )
//                {
//                    oPoint.X [ 0 ]	=	dEndX;
//                    oPoint.Y [ 0 ]	=	dEndY;

//                    oPoint.X [ 1 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;
//                    oPoint.Y [ 1 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;

//                    dEndX			=	oPoint.X [ 1 ];
//                    dEndY			=	oPoint.Y [ 1 ];
//                }

//                if ( etHorzTo == oPoint.Type )
//                {
//                    oPoint.Type =	etLineTo;

//                    oPoint.X [ 0 ]	=	dEndX;
//                    oPoint.Y [ 0 ]	=	dEndY;

//                    oPoint.X [ 1 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;
//                    oPoint.Y [ 1 ]	=	oPoint.Y [ 0 ];

//                    dEndX			=	oPoint.X [ 1 ];
//                    dEndY			=	oPoint.Y [ 1 ];
//                }

//                if ( etVertTo == oPoint.Type )
//                {
//                    oPoint.Type =	etLineTo;

//                    oPoint.X [ 0 ]	=	dEndX;
//                    oPoint.Y [ 0 ]	=	dEndY;

//                    oPoint.X [ 1 ]	=	oPoint.X [ 0 ];
//                    oPoint.Y [ 1 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;

//                    dEndX			=	oPoint.X [ 1 ];
//                    dEndY			=	oPoint.Y [ 1 ];
//                }

//                if ( etCurveTo == oPoint.Type )
//                {
//                    oPoint.X [ 0 ]	=	dEndX;
//                    oPoint.Y [ 0 ]	=	dEndY;

//                    oPoint.X [ 1 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;
//                    oPoint.Y [ 1 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;

//                    oPoint.X [ 2 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;
//                    oPoint.Y [ 2 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;

//                    oPoint.X [ 3 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;
//                    oPoint.Y [ 3 ]	=	_tstof ( Str.Tokenize ( Tokens, To ) ); if ( -1 == To ) break;

//                    dEndX			=	oPoint.X [ 3 ];
//                    dEndY			=	oPoint.Y [ 3 ];
//                }

//                m_Points.push_back ( oPoint );
//            }

//            return ( m_Points.size() >= 2 );
//        }

//        inline std::wstring ToXml (double dW, double dH)
//        {
//            std::wstring Xml;

//            for ( long i = 0; i < (long)m_Points.size(); ++i )
//            {
//                std::wstring Fmt;

//                if ( etMoveTo == m_Points [ i ].Type )
//                {
//                    Fmt.Format ( _T("<move x = '%f' y = '%f' />"),
//                                 m_Points [ i ].X [ 0 ] * dW, m_Points [ i ].Y [ 0 ] * dH );
//                }

//                if ( etLineTo == m_Points [ i ].Type )
//                {
//                    Fmt.Format ( _T("<line x0 = '%f' y0 = '%f' x1 = '%f' y1 = '%f' />"),
//                                 m_Points [ i ].X [ 0 ] * dW, m_Points [ i ].Y [ 0 ] * dH,
//                            m_Points [ i ].X [ 1 ] * dW, m_Points [ i ].Y [ 1 ] * dH );
//                }

//                if ( etCurveTo == m_Points [ i ].Type )
//                {
//                    Fmt.Format ( _T("<curve x0 = '%f' y0 = '%f' x1 = '%f' y1 = '%f' x2 = '%f' y2 = '%f' x3 = '%f' y3 = '%f' />"),
//                                 m_Points [ i ].X [ 0 ] * dW, m_Points [ i ].Y [ 0 ] * dH,
//                            m_Points [ i ].X [ 1 ] * dW, m_Points [ i ].Y [ 1 ] * dH,
//                            m_Points [ i ].X [ 2 ] * dW, m_Points [ i ].Y [ 2 ] * dH,
//                            m_Points [ i ].X [ 3 ] * dW, m_Points [ i ].Y [ 3 ] * dH );
//                }

//                Xml	+= Fmt;
//            }

//            return Xml;
//        }


        inline std::wstring ToStr ()
        {
            return _T("");
        }

    private:

        std::vector <PathPoint> m_Points;
    };

    class CAnimationSimple
    {
    public:
        CAnimationSimple()
        {
            m_nRefID			=	-1;

            m_nBeginTime			=	0;
            m_nDuration			=	0;

            m_nEffectID			=	0;
            m_nEffectDir			=	0;
            m_nEffectType			=	0;
            m_nEffectNodeType		=	0;

            m_dSX				=	1.0;
            m_dSY				=	1.0;

            m_dRotateAngle			=	0.0;

            m_dTransparency			=	1.0;

            m_nSchemeColor			=	0;
            m_nColorTo			=	0;


            m_dTimeAccel			=	0.0;
            m_dTimeDecel			=	0.0;

            m_bIgnoreShape			=	false;
            m_nTextSequence			=	-1;
            m_bRemoveEmptyBlocks            =	false;

            m_nMediaCMD			=	-1;
        }

    public:

        unsigned long           m_nRefID;

        double			m_nBeginTime;
        double			m_nDuration;

        unsigned long           m_nEffectID;
        unsigned long           m_nEffectDir;
        unsigned long           m_nEffectType;
        unsigned long           m_nEffectNodeType;

        std::wstring			m_MotionPath;

        double			m_dSX;
        double			m_dSY;

        double			m_dRotateAngle;

        double			m_dTransparency;

        int			m_nSchemeColor;
        unsigned long           m_nColorTo;

        double			m_dTimeAccel;
        double			m_dTimeDecel;

        int			m_nTextSequence;
        bool			m_bIgnoreShape;
        bool			m_bRemoveEmptyBlocks;

        long			m_nMediaCMD;
    };

    class CAnimationInfo
    {
    public:
        std::vector<CAnimationSimple> m_arAnimations;

        double m_dSlideWidth;
        double m_dSlideHeight;

        std::vector <std::wstring> m_oQuery;

        CAnimationInfo() : m_arAnimations()
        {
        }
        CAnimationInfo& operator=(const CAnimationInfo& oSrc)
        {
            m_dSlideWidth	= oSrc.m_dSlideWidth;
            m_dSlideHeight	= oSrc.m_dSlideHeight;

            m_arAnimations.insert(m_arAnimations.end(), oSrc.m_arAnimations.begin(), oSrc.m_arAnimations.end());
            return *this;
        }
        CAnimationInfo(const CAnimationInfo& oSrc)
        {
            *this = oSrc;
        }
        ~CAnimationInfo()
        {
        }


//        std::wstring ToXml(const double& dStartTime, const double& dEndTime, bool bIgnore = false)
//        {
//            std::wstring baseXML;
//            baseXML.Format ( _T("<animations width = '%f' height = '%f'>"), m_dSlideWidth, m_dSlideHeight );

//            std::wstring Source;

//            for ( long i = 0; i < (long)m_arAnimations.size(); ++i )
//            {
//                CAnimationSimple* pEffect = &m_arAnimations[i];
//                if (NULL==pEffect)
//                    continue;

//                if (bIgnore)
//                {
//                    if (pEffect->m_bIgnoreShape)
//                        continue;
//                }

//                double dEffectBegin	=	dStartTime + pEffect->m_nBeginTime;

//                m_oQuery.clear ();

//                m_oQuery.push_back ( _T(" id		=	'") + PPT::ToString (pEffect->m_nEffectID)		+ _T("' "));
//                m_oQuery.push_back ( _T(" type	=	'")		+ PPT::ToString (pEffect->m_nEffectType)	+ _T("' "));
//                m_oQuery.push_back ( _T(" dir		=	'") + PPT::ToString (pEffect->m_nEffectDir)	+ _T("' "));

//                m_oQuery.push_back ( _T(" begin	=	'")		+ PPT::ToString (dEffectBegin)				+ _T("' "));
//                m_oQuery.push_back ( _T(" dur		=	'") + PPT::ToString (pEffect->m_nDuration)		+ _T("' "));
//                m_oQuery.push_back ( _T(" accel	=	'")		+ PPT::ToString (pEffect->m_dTimeAccel)	+ _T("' "));
//                m_oQuery.push_back ( _T(" decel	=	'")		+ PPT::ToString (pEffect->m_dTimeDecel)	+ _T("' "));

//                m_oQuery.push_back ( _T(" sx		=	'")	+ PPT::ToString (pEffect->m_dSX)			+ _T("' "));
//                m_oQuery.push_back ( _T(" sy		=	'") + PPT::ToString (pEffect->m_dSY)			+ _T("' "));
//                m_oQuery.push_back ( _T(" alpha	=	'")		+ PPT::ToString (pEffect->m_dTransparency)	+ _T("' "));

//                m_oQuery.push_back ( _T(" angle	=	'")		+ PPT::ToString (pEffect->m_dRotateAngle)	+ _T("' "));
//                m_oQuery.push_back ( _T(" color	=	'")		+ PPT::ToString (pEffect->m_nColorTo)		+ _T("' "));

//                m_oQuery.push_back ( _T(" block	=	'")		+ PPT::ToString (pEffect->m_nTextSequence)	+ _T("' "));
//                m_oQuery.push_back ( _T(" removeemptyblocks	=	'") + PPT::ToString (pEffect->m_bRemoveEmptyBlocks) + _T("' "));

//                std::wstring Effect;

//                if (MotionEffect == pEffect->m_nEffectType || pEffect->m_MotionPath.length())
//                {
//                    CMotionPath oPath;
//                    if ( oPath.FromStr ( pEffect->m_MotionPath ) )
//                    {
//                        Effect.Format ( _T("<animateMotion %s >%s</animateMotion>"), FormatXml (), oPath.ToXml(1.0,1.0));
//                        Source	+=	Effect;
//                    }
//                }

//                if ( FadeInType == pEffect->m_nEffectType )
//                {
//                    Effect.Format ( _T("<animateFadeIn %s />"), FormatXml ());
//                    Source	+=	Effect;
//                }

//                if ( FadeExitType == pEffect->m_nEffectType )
//                {
//                    Effect.Format ( _T("<animateFadeExit %s />"), FormatXml ());
//                    Source	+=	Effect;
//                }

//                if (EmphasisEffect == pEffect->m_nEffectType )
//                {
//                    if (6 == pEffect->m_nEffectID)	//	GrowAndShrink Effect
//                    {
//                        Effect.Format ( _T("<animateScale %s />"), FormatXml ());
//                    }
//                    else if (8 == pEffect->m_nEffectID || 32 == pEffect->m_nEffectID)	//	Spin Effect || Teeter Effect
//                    {
//                        Effect.Format ( _T("<animateRotate %s />"), FormatXml ());
//                    }
//                    else if (9 == pEffect->m_nEffectID || 35 == pEffect->m_nEffectID)	//	Blink - 36 - мигание
//                    {
//                        Effect.Format ( _T("<animateAlpha %s />"), FormatXml ());
//                    }
//                    else if (26	== pEffect->m_nEffectID)	// FlashBulb Effect ( Пульсация )
//                    {
//                        Effect.Format ( _T("<animateAlpha %s />"), FormatXml ());
//                        Source	+=	Effect;

//                        Effect.Format ( _T("<animateScale %s />"), FormatXml ());
//                        Source	+=	Effect;

//                        continue;
//                    }
//                    else if (27 == pEffect->m_nEffectID		//	Flicker (÷ветовая пульсация)
//                             || 1 ==	pEffect->m_nEffectID		//	ChangeFillColor	-	fill color
//                             || 3 ==	pEffect->m_nEffectID		//	ChangeFillColor	-	font color
//                             || 7 == pEffect->m_nEffectID		//	ChangeFillColor	-	fill lines
//                             || 19 == pEffect->m_nEffectID)		//	ColorBlend		-	object fill color
//                    {
//                        Effect.Format ( _T("<animateColor %s />"), FormatXml ());
//                    }
//                    else
//                    {
//                        Effect.Format ( _T("<animateEmphasis %s />"), FormatXml ());
//                    }

//                    Source	+=	Effect;
//                }

//                if (MediaCallEffect == pEffect->m_nEffectType )
//                {
//                    std::wstring sTime;
//                    sTime.Format(_T(" begin='%f' dur='%f' accel='%f' decel='%f' "), dStartTime + pEffect->m_nBeginTime, pEffect->m_nDuration, pEffect->m_dTimeAccel, pEffect->m_dTimeDecel);

//                    Effect.Format(_T("<mediaCall id='%d' %s event='%d' />"), pEffect->m_nEffectID, sTime, pEffect->m_nMediaCMD);
//                    Source	+=	Effect;
//                }
//            }

//            if (bIgnore)
//            {
//                if (0 == Source.length())
//                    return _T("");
//            }

//            baseXML	+=	Source;
//            baseXML	+=	std::wstring(_T("</animations>"));

//            return baseXML;
//        }

    };
}
