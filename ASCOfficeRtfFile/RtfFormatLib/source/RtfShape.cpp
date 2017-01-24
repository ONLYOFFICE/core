/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#include "RtfShape.h"
#include "Writer/OOXWriter.h"
#include "RtfOle.h"

#include "../../../UnicodeConverter/UnicodeConverter.h"

#include "../../../ASCOfficePPTXFile/Editor/Drawing/Shapes/BaseShape/PPTShape/PPTShape.h"

void RtfShape::SetDefault()
{
	m_eAnchorTypeShape = st_none;

//Common
	DEFAULT_PROPERTY( m_nLeft )
	DEFAULT_PROPERTY( m_nTop )
	DEFAULT_PROPERTY( m_nBottom )
	DEFAULT_PROPERTY( m_nRight )
	DEFAULT_PROPERTY( m_nID )
	DEFAULT_PROPERTY( m_nZOrder )
	DEFAULT_PROPERTY( m_nHeader )
	DEFAULT_PROPERTY( m_nWrapType )
	DEFAULT_PROPERTY( m_nWrapSideType )
	DEFAULT_PROPERTY( m_nZOrderRelative )
	DEFAULT_PROPERTY( m_bLockAnchor )
	DEFAULT_PROPERTY_DEF( m_eXAnchor, ax_column ) //по умолчанию - привязка к тексту
	DEFAULT_PROPERTY_DEF( m_eYAnchor, ay_Para )//по умолчанию - привязка к тексту

//Position absolute
	DEFAULT_PROPERTY( m_nPositionH )
	DEFAULT_PROPERTY( m_nPositionHRelative )
	DEFAULT_PROPERTY( m_nPositionV )
	DEFAULT_PROPERTY( m_nPositionVRelative )
	DEFAULT_PROPERTY( m_bLayoutInCell )
	DEFAULT_PROPERTY( m_bAllowOverlap )

//Position relative
	DEFAULT_PROPERTY( m_nPositionHPct )
	DEFAULT_PROPERTY( m_nPositionVPct )
	DEFAULT_PROPERTY( m_nPctWidth )
	DEFAULT_PROPERTY( m_nPctHeight )
	DEFAULT_PROPERTY( m_nPctWidthRelative )
	DEFAULT_PROPERTY( m_nPctHeightRelative )
	DEFAULT_PROPERTY( m_nColStart )
	DEFAULT_PROPERTY( m_nColSpan )

//Object Type
	DEFAULT_PROPERTY( m_bIsBullet )
	DEFAULT_PROPERTY( m_nRotation )
	DEFAULT_PROPERTY( m_bFlipV )
	DEFAULT_PROPERTY( m_bFlipH )
	DEFAULT_PROPERTY( m_nShapeType )
	DEFAULT_PROPERTY( m_nWrapDistLeft )
	DEFAULT_PROPERTY( m_nWrapDistTop )
	DEFAULT_PROPERTY( m_nWrapDistRight )
	DEFAULT_PROPERTY( m_nWrapDistBottom )

	DEFAULT_PROPERTY( m_bHidden )

//Text box
	DEFAULT_PROPERTY( m_nAnchorText )
	DEFAULT_PROPERTY_DEF( m_nTexpLeft, 91440 )
	DEFAULT_PROPERTY_DEF( m_nTexpTop, 45720 )
	DEFAULT_PROPERTY_DEF( m_nTexpRight, 91440 )
	DEFAULT_PROPERTY_DEF( m_nTexpBottom, 45720 )
	DEFAULT_PROPERTY( m_nTexpLeft )
	DEFAULT_PROPERTY( m_nTexpTop )
	DEFAULT_PROPERTY( m_nTexpRight )
	DEFAULT_PROPERTY( m_nTexpBottom )
	DEFAULT_PROPERTY( m_bFitShapeToText )
	DEFAULT_PROPERTY( m_bFitTextToShape )
	DEFAULT_PROPERTY( m_nCcol )
	DEFAULT_PROPERTY( m_nTxdir )
	DEFAULT_PROPERTY( m_nWrapText )
//Geometry
	for (int i = 0; i < 10; i++)
		DEFAULT_PROPERTY( m_nAdjustValue[i] )
//Custom
	DEFAULT_PROPERTY( m_nGeoLeft)	
	DEFAULT_PROPERTY( m_nGeoTop)	
	DEFAULT_PROPERTY( m_nGeoRight)
	DEFAULT_PROPERTY( m_nGeoBottom)
	DEFAULT_PROPERTY( m_nShapePath)
//Picture Effects
	DEFAULT_PROPERTY( m_nCropFromTop )
	DEFAULT_PROPERTY( m_nCropFromBottom )
	DEFAULT_PROPERTY( m_nCropFromLeft )
	DEFAULT_PROPERTY( m_nCropFromRight )

//Grouped Shapes
	DEFAULT_PROPERTY( m_nGroupBottom )
	DEFAULT_PROPERTY( m_nGroupLeft )
	DEFAULT_PROPERTY( m_nGroupRight )
	DEFAULT_PROPERTY( m_nGroupTop )

	DEFAULT_PROPERTY( m_nRelBottom )
	DEFAULT_PROPERTY( m_nRelLeft )
	DEFAULT_PROPERTY( m_nRelRight )
	DEFAULT_PROPERTY( m_nRelTop )
	DEFAULT_PROPERTY( m_nRelRotation )
	DEFAULT_PROPERTY( m_nRelZOrder )

//Rehydration
	m_sMetroBlob = L"";
	m_sMetroBlobRels = L"";
	m_sMetroBlobData = L"";
//Connectors
	DEFAULT_PROPERTY( m_nConnectionType )
	DEFAULT_PROPERTY( m_nConnectorStyle )

//Fill
	DEFAULT_PROPERTY_DEF( m_bFilled, true )
	DEFAULT_PROPERTY( m_nFillType )
	DEFAULT_PROPERTY( m_nFillColor )
	DEFAULT_PROPERTY( m_nFillColor2 )
	DEFAULT_PROPERTY( m_nFillOpacity )
	DEFAULT_PROPERTY( m_nFillFocus )
	DEFAULT_PROPERTY( m_nFillAngle )
//Line
	DEFAULT_PROPERTY_DEF( m_bLine, true )
	DEFAULT_PROPERTY( m_nLineColor )
	DEFAULT_PROPERTY( m_nLineStartArrow )
	DEFAULT_PROPERTY( m_nLineStartArrowWidth )
	DEFAULT_PROPERTY( m_nLineStartArrowLength )
	DEFAULT_PROPERTY( m_nLineEndArrow )
	DEFAULT_PROPERTY( m_nLineEndArrowWidth )
	DEFAULT_PROPERTY( m_nLineEndArrowLength )
	DEFAULT_PROPERTY( m_nLineWidth )
	DEFAULT_PROPERTY( m_nLineDashing )

	DEFAULT_PROPERTY( m_nGtextSize )
	DEFAULT_PROPERTY( m_bGtext )
	DEFAULT_PROPERTY( m_bGtextFVertical )
	DEFAULT_PROPERTY( m_bGtextFKern )
	DEFAULT_PROPERTY( m_bGtextFStretch )
	DEFAULT_PROPERTY( m_bGtextFShrinkFit )
	DEFAULT_PROPERTY( m_bGtextFBestFit )
	
	m_aTextItems	= TextItemContainerPtr();
	m_oPicture		= RtfPicturePtr();
	
	m_bIsOle		= false;
	m_bInGroup		= false;
	
	m_oCharProperty.SetDefault();
}

std::wstring RtfShape::RenderToRtf(RenderParameter oRenderParameter)
{
	if( PROP_DEF == m_nShapeType) return L"";
	
    std::wstring sResult;
	//запоминаем координаты и если нужно поворачиваем
	int nLeft		= m_nLeft;
	int nTop		= m_nTop;
	int nRight		= m_nRight;
	int nBottom		= m_nBottom;
	int nRotate		= m_nRotation;

	int nRelLeft	= m_nRelLeft;
	int nRelTop		= m_nRelTop;
	int nRelRight	= m_nRelRight;
	int nRelBottom	= m_nRelBottom;
	int nRelRotate	= m_nRelRotation;

	if( PROP_DEF != m_nRotation && PROP_DEF != m_nLeft && PROP_DEF != m_nTop &&
		PROP_DEF != m_nRight	&& PROP_DEF != m_nBottom )
		ToRtfRotation( m_nRotation, m_nLeft, m_nTop, m_nRight, m_nBottom );

	if( PROP_DEF != m_nRelRotation	&& PROP_DEF != m_nRelLeft && PROP_DEF != m_nRelTop &&
		PROP_DEF != m_nRelRight		&& PROP_DEF != m_nRelBottom )
		ToRtfRotation( m_nRelRotation, m_nRelLeft, m_nRelTop, m_nRelRight, m_nRelBottom );

	sResult += m_oCharProperty.RenderToRtf( oRenderParameter );

	if( st_inline == m_eAnchorTypeShape || st_none == m_eAnchorTypeShape)
	{
		if( NULL != m_oPicture && m_nShapeType == NSOfficeDrawing::sptPictureFrame)
		{
			if (m_oPicture->m_nWidth == PROP_DEF)
			{
				m_oPicture->m_nWidth	= m_nRight;
				m_oPicture->m_nHeight	= m_nBottom;
			}
			if (m_oPicture->m_nWidthGoal == PROP_DEF)
			{
				m_oPicture->m_nWidthGoal	= m_oPicture->m_nWidth;
				m_oPicture->m_nHeightGoal	= m_oPicture->m_nHeight;
			}
			if( RtfPicture::dt_wmf == m_oPicture->eDataType )
			{
				sResult +=  m_oPicture->RenderToRtf( oRenderParameter );
			}
			else
			{
				sResult += L"{\\*\\shppict";
				sResult +=  m_oPicture->RenderToRtf( oRenderParameter );
				sResult += L"}";
				sResult += L"{\\nonshppict";
				sResult +=  m_oPicture->GenerateWMF( oRenderParameter );

				sResult += L"}";
			}
		}
		else
		{
			sResult += L"{\\shp";
			sResult += L"{\\*\\shpinst";
			
			RENDER_RTF_INT( m_nID , sResult, L"shplid" );
			
			if (!m_bInGroup)
			{
				m_bAllowOverlap		= 1;
				m_bLayoutInCell		= 1;
				m_nWrapType			= 3;
				m_nWrapSideType		= 0;
				m_bLockAnchor		= 0;
				m_nZOrder			= PROP_DEF;
				m_nZOrderRelative	= PROP_DEF;
				m_nLeft				= m_nTop		= 0;
				m_nRelBottom		= m_nRelRight	= PROP_DEF;
				m_nPositionV		= m_nPositionH	= PROP_DEF;

				RENDER_RTF_INT	( m_nLeft			, sResult, L"shpleft" );
				RENDER_RTF_INT	( m_nTop			, sResult, L"shptop" );
				RENDER_RTF_INT	( m_nRight			, sResult, L"shpright" );
				RENDER_RTF_INT	( m_nBottom			, sResult, L"shpbottom" );
				
				RENDER_RTF_INT	( m_nHeader			, sResult, L"shpfhdr" );
				
				//sResult += L"\\shpbxcolumn";
				sResult += L"\\shpbxignore";
				//sResult += L"\\shpbypara";
				sResult += L"\\shpbyignore";
				
				RENDER_RTF_INT	( m_nWrapType		, sResult, L"shpwr" );
				RENDER_RTF_INT	( m_nWrapSideType	, sResult, L"shpwrk" );
				
				//sResult += L"\\shpfblwtxt0";
				sResult += L"\\shplockanchor";
				
				RENDER_RTF_INT	( m_nZOrder			, sResult, L"shpz" );
				
				sResult += L"{\\sp{\\sn fUseShapeAnchor}{\\sv 0}}";
				sResult += L"{\\sp{\\sn fPseudoInline}{\\sv 1}}";
			}
			sResult +=  RenderToRtfShapeProperty( oRenderParameter );

			sResult += L"{\\sp{\\sn fLockPosition}{\\sv 1}}";
			sResult += L"{\\sp{\\sn fLockRotation}{\\sv 1}}";		
		
			//picture
			if( 0 != m_oPicture && m_nFillType == 1 || m_nFillType == 2 || m_nFillType == 9)
			{
				sResult += L"{\\sp{\\sn fillBlip}{\\sv ";
				sResult +=  m_oPicture->RenderToRtf( oRenderParameter );
				sResult += L"}}";
			}
			//textbox
			if( 0 != m_aTextItems )
			{
				sResult += L"{\\shptxt ";
				sResult +=  m_aTextItems->RenderToRtf( oRenderParameter );
				sResult += L"}";
			}
			sResult += L"}";
			sResult += L"}";
		}
	}
	else	// anchor
	{
		sResult += L"{\\shp";
		sResult += L"{\\*\\shpinst";
		
		RENDER_RTF_INT( m_nID					, sResult, L"shplid" );

		if (!m_bInGroup)
		{		
			RENDER_RTF_INT	( m_nLeft			, sResult, L"shpleft" );
			RENDER_RTF_INT	( m_nTop			, sResult, L"shptop" );
			RENDER_RTF_INT	( m_nBottom			, sResult, L"shpbottom" );
			RENDER_RTF_INT	( m_nRight			, sResult, L"shpright" );
			RENDER_RTF_INT	( m_nZOrder			, sResult, L"shpz" );
			RENDER_RTF_INT	( m_nHeader			, sResult, L"shpfhdr" );
			RENDER_RTF_INT	( m_nWrapType		, sResult, L"shpwr" );
			RENDER_RTF_INT	( m_nWrapSideType	, sResult, L"shpwrk" );
			RENDER_RTF_BOOL	( m_bLockAnchor		, sResult, L"shplockanchor" );

			switch(m_eXAnchor)
			{
				case ax_page:	sResult += L"\\shpbxpage";		break;
				case ax_margin: sResult += L"\\shpbxmargin";	break;
				case ax_column: sResult += L"\\shpbxcolumn";	break;
			}
			sResult += L"\\shpbxignore";
			switch(m_eYAnchor)
			{
				case ay_page:	sResult += L"\\shpbypage";		break;
				case ay_margin: sResult += L"\\shpbymargin";	break;
				case ay_Para:	sResult += L"\\shpbypara";		break;
			}
			sResult += L"\\shpbyignore";
			RENDER_RTF_INT( m_nZOrderRelative, sResult, L"shpfblwtxt");
		}

		sResult += RenderToRtfShapeProperty( oRenderParameter );

		//picture
		if( 0 != m_oPicture)
		{
			if (m_nShapeType == NSOfficeDrawing::sptPictureFrame)
			{
				sResult += L"{\\sp{\\sn pib}{\\sv ";
				sResult +=  m_oPicture->RenderToRtf( oRenderParameter );
				sResult += L"}}";
			}
			else if (m_nFillType == 1 || m_nFillType == 2 || m_nFillType == 9)
			{
				sResult += L"{\\sp{\\sn fillType}{\\sv 2}}";
				sResult += L"{\\sp{\\sn fillBlip}{\\sv ";
				sResult +=  m_oPicture->RenderToRtf( oRenderParameter );
				sResult += L"}}";
			}
		}
		//textbox
		if( m_aTextItems )
		{
			sResult += L"{\\shptxt ";
			sResult += m_aTextItems->RenderToRtf( oRenderParameter );
			sResult += L"}";
		}
		sResult += L"}";
		if( m_oPicture && m_nShapeType == NSOfficeDrawing::sptPictureFrame )
		{
			sResult += L"{\\shprslt\\par\\plain";
			sResult +=  m_oPicture->GenerateWMF( oRenderParameter );
			sResult += L"\\par}";
		}
		sResult += L"}";
	}
	//восстанавливаем координаты и если нужно поворачиваем
	m_nLeft			= nLeft;
	m_nTop			= nTop;
	m_nRight		= nRight;
	m_nBottom		= nBottom;
	m_nRotation		= nRotate;

	m_nRelLeft		= nRelLeft;
	m_nRelTop		= nRelTop;
	m_nRelRight		= nRelRight;
	m_nRelBottom	= nRelBottom;
	m_nRelRotation	= nRelRotate;

	return sResult;
}
std::wstring RtfShape::RenderToRtfShapeProperty(RenderParameter oRenderParameter)
{
    RtfDocument* pDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);

    std::wstring sResult;

//Position absolute
    RENDER_RTF_SHAPE_PROP(L"posh",    sResult,   	m_nPositionH);
    RENDER_RTF_SHAPE_PROP(L"posrelh",    sResult,   	m_nPositionHRelative);
    RENDER_RTF_SHAPE_PROP(L"posv",    sResult,   	m_nPositionV);

    RENDER_RTF_SHAPE_PROP(L"posrelv",    sResult,   	m_nPositionVRelative);
    RENDER_RTF_SHAPE_PROP(L"fLayoutInCell",    sResult,    m_bLayoutInCell);
    RENDER_RTF_SHAPE_PROP(L"fAllowOverlap",    sResult,   m_bAllowOverlap);

//Position relative
    RENDER_RTF_SHAPE_PROP(L"pctHorizPos",    sResult,   m_nPositionHPct);
    RENDER_RTF_SHAPE_PROP(L"pctVertPos",    sResult,   m_nPositionVPct);
    RENDER_RTF_SHAPE_PROP(L"pctHoriz",    sResult,   m_nPctWidth);
    RENDER_RTF_SHAPE_PROP(L"pctVert",    sResult,   	m_nPctHeight);
    RENDER_RTF_SHAPE_PROP(L"sizerelh",    sResult,   m_nPctWidthRelative);
    RENDER_RTF_SHAPE_PROP(L"sizerelv",    sResult,   m_nPctHeightRelative);
    RENDER_RTF_SHAPE_PROP(L"colStart",    sResult,   m_nColStart);
    RENDER_RTF_SHAPE_PROP(L"colSpan",    sResult,   	m_nColSpan);
//Rehydration
    //RENDER_RTF_SHAPE_PROP(L"metroBlob",    sResult,   m_sMetroBlob);

//Object Type
    RENDER_RTF_SHAPE_PROP(L"fIsBullet",    sResult,   m_bIsBullet);
    RENDER_RTF_SHAPE_PROP(L"rotation",    sResult,   m_nRotation);
    RENDER_RTF_SHAPE_PROP(L"fFlipV",    sResult,   	m_bFlipV);
    RENDER_RTF_SHAPE_PROP(L"fFlipH",    sResult,   	m_bFlipH);
    RENDER_RTF_SHAPE_PROP(L"shapeType",    sResult,   m_nShapeType);
    RENDER_RTF_SHAPE_PROP(L"fBehindDocument",    sResult,   m_nZOrderRelative);
    RENDER_RTF_SHAPE_PROP(L"fHidden",    sResult,   	m_bHidden);
    //Text
	//sResult += L"{\\sp{\\sn fLockText}{\\sv 0}}";

    RENDER_RTF_SHAPE_PROP(L"dxTextLeft",    sResult,   m_nTexpLeft);
    RENDER_RTF_SHAPE_PROP(L"dyTextTop",    sResult,   m_nTexpTop);
    RENDER_RTF_SHAPE_PROP(L"dxTextRight",    sResult,   m_nTexpRight);
    RENDER_RTF_SHAPE_PROP(L"dyTextBottom",    sResult,   m_nTexpBottom);

    RENDER_RTF_SHAPE_PROP(L"anchorText",    sResult,   m_nAnchorText);
    //else
	{
        RENDER_RTF_SHAPE_PROP(L"dxWrapDistLeft",    sResult,    RtfUtility::Twips2Emu( m_nWrapDistLeft ));
        RENDER_RTF_SHAPE_PROP(L"dyWrapDistTop",    sResult,    RtfUtility::Twips2Emu( m_nWrapDistTop ));
        RENDER_RTF_SHAPE_PROP(L"dxWrapDistRight",    sResult,    RtfUtility::Twips2Emu( m_nWrapDistRight ));
        RENDER_RTF_SHAPE_PROP(L"dyWrapDistBottom",    sResult,   RtfUtility::Twips2Emu(  m_nWrapDistBottom ));
    }
    RENDER_RTF_SHAPE_PROP(L"fFitShapeToText",   sResult,   m_bFitShapeToText);
    RENDER_RTF_SHAPE_PROP(L"fFitTextToShape",   sResult,   m_bFitTextToShape);
    RENDER_RTF_SHAPE_PROP(L"ccol",              sResult,   		m_nCcol);
    RENDER_RTF_SHAPE_PROP(L"txdir",             sResult,   		m_nTxdir);
    RENDER_RTF_SHAPE_PROP(L"WrapText",          sResult,   	m_nWrapText);
//Geometry
    RENDER_RTF_SHAPE_PROP(L"adjustValue",       sResult,   	m_nAdjustValue[0] );
    RENDER_RTF_SHAPE_PROP(L"adjust2Value",      sResult,   m_nAdjustValue[1] );
    RENDER_RTF_SHAPE_PROP(L"adjust3Value",      sResult,   m_nAdjustValue[2] );
    RENDER_RTF_SHAPE_PROP(L"adjust4Value",      sResult,   m_nAdjustValue[3] );
    RENDER_RTF_SHAPE_PROP(L"adjust5Value",      sResult,   m_nAdjustValue[4] );
    RENDER_RTF_SHAPE_PROP(L"adjust6Value",      sResult,   m_nAdjustValue[5] );
    RENDER_RTF_SHAPE_PROP(L"adjust7Value",      sResult,   m_nAdjustValue[6] );
    RENDER_RTF_SHAPE_PROP(L"adjust8Value",      sResult,   m_nAdjustValue[7] );
    RENDER_RTF_SHAPE_PROP(L"adjust9Value",      sResult,   m_nAdjustValue[8] );
    RENDER_RTF_SHAPE_PROP(L"adjust10Value",     sResult,   m_nAdjustValue[9] );
//custom
        RENDER_RTF_SHAPE_PROP(L"geoLeft",       sResult,   	m_nGeoLeft );
        RENDER_RTF_SHAPE_PROP(L"geoTop",        sResult,   	m_nGeoTop);
        RENDER_RTF_SHAPE_PROP(L"geoRight",      sResult,   m_nGeoRight );
        RENDER_RTF_SHAPE_PROP(L"geoBottom",     sResult,   m_nGeoBottom );
        RENDER_RTF_SHAPE_PROP(L"shapePath",     sResult,   m_nShapePath );
	if( !m_aPVerticles.empty())
	{
        sResult += L"{\\sp{\\sn pVerticies}{\\sv 8;" + boost::lexical_cast<std::wstring>( m_aPVerticles.size() );
		for( int i = 0; i < m_aPVerticles.size(); i ++ )
            sResult += L";(" + boost::lexical_cast<std::wstring>(m_aPVerticles[i].first) + L","+ boost::lexical_cast<std::wstring>(m_aPVerticles[i].second) + L")";
		sResult += L"}}";
	}
    if( !m_aPSegmentInfo.empty())
    {
        sResult += L"{\\sp{\\sn pSegmentInfo}{\\sv ";
        sResult += L"{\\sp{\\sn pSegmentInfo}{\\sv 2;" + boost::lexical_cast<std::wstring>( m_aPSegmentInfo.size() );
        for( int i = 0; i < m_aPSegmentInfo.size(); i ++ )
            sResult += L";" + boost::lexical_cast<std::wstring>( m_aPSegmentInfo[i] );
        sResult += L"}}";
    }
//Connectors
    RENDER_RTF_SHAPE_PROP(L"cxk",    sResult,   			m_nConnectionType );
    RENDER_RTF_SHAPE_PROP(L"cxstyle",    sResult,   		m_nConnectorStyle );
//Picture Effects
    RENDER_RTF_SHAPE_PROP(L"cropFromTop",    sResult,   	m_nCropFromTop );
    RENDER_RTF_SHAPE_PROP(L"cropFromBottom",    sResult,   m_nCropFromBottom );
    RENDER_RTF_SHAPE_PROP(L"cropFromLeft",    sResult,   m_nCropFromLeft );
    RENDER_RTF_SHAPE_PROP(L"cropFromRight",    sResult,   m_nCropFromRight );
//Grouped Shapes
    RENDER_RTF_SHAPE_PROP(L"groupBottom",    sResult,   	m_nGroupBottom );
    RENDER_RTF_SHAPE_PROP(L"groupLeft",    sResult,   	m_nGroupLeft );
    RENDER_RTF_SHAPE_PROP(L"groupRight",    sResult,   	m_nGroupRight );
    RENDER_RTF_SHAPE_PROP(L"groupTop",    sResult,   	m_nGroupTop );
    RENDER_RTF_SHAPE_PROP(L"relBottom",    sResult,   	m_nRelBottom );
    RENDER_RTF_SHAPE_PROP(L"relLeft",    sResult,   		m_nRelLeft );
    RENDER_RTF_SHAPE_PROP(L"relRight",    sResult,   	m_nRelRight );
    RENDER_RTF_SHAPE_PROP(L"relTop",    sResult,   		m_nRelTop );
    RENDER_RTF_SHAPE_PROP(L"relRotation",    sResult,   	m_nRelRotation );
    RENDER_RTF_SHAPE_PROP(L"dhgt",    sResult,   		m_nRelZOrder );
//Fill
	if( 0 == m_bFilled )
		sResult += L"{\\sp{\\sn fFilled}{\\sv 0}}";
    RENDER_RTF_SHAPE_PROP(L"fillType",    sResult,   	m_nFillType );
    RENDER_RTF_SHAPE_PROP(L"fillColor",    sResult,   	m_nFillColor );
    RENDER_RTF_SHAPE_PROP(L"fillBackColor",    sResult,   m_nFillColor2 );
    RENDER_RTF_SHAPE_PROP(L"fillOpacity",    sResult,    ( m_nFillOpacity * 65536 /100 ) );
    RENDER_RTF_SHAPE_PROP(L"fillFocus",    sResult,   	m_nFillFocus );
    RENDER_RTF_SHAPE_PROP(L"fillAngle",    sResult,   	m_nFillAngle * 65536 );
    //Line
	if( 0 == m_bLine )
		sResult += L"{\\sp{\\sn fLine}{\\sv 0}}";
    RENDER_RTF_SHAPE_PROP(L"lineColor",             sResult,   	m_nLineColor );
    RENDER_RTF_SHAPE_PROP(L"lineStartArrowhead",    sResult,    m_nLineStartArrow );
    RENDER_RTF_SHAPE_PROP(L"lineEndArrowhead",      sResult,    m_nLineEndArrow );
    RENDER_RTF_SHAPE_PROP(L"lineStartArrowWidth",   sResult,    m_nLineStartArrowWidth );
    RENDER_RTF_SHAPE_PROP(L"lineStartArrowLength",  sResult,    m_nLineStartArrowLength );
    RENDER_RTF_SHAPE_PROP(L"lineEndArrowWidth",     sResult,    m_nLineEndArrowWidth );
    RENDER_RTF_SHAPE_PROP(L"lineEndArrowLength",    sResult,    m_nLineEndArrowLength );
    RENDER_RTF_SHAPE_PROP(L"lineWidth",             sResult,   	m_nLineWidth );
    RENDER_RTF_SHAPE_PROP(L"lineDashing",           sResult,   	m_nLineDashing );

//pWrapPolygonVertices	Points of the text wrap polygon.
	if( !m_aWrapPoints.empty())
	{
        sResult += L"{\\sp{\\sn pWrapPolygonVertices}{\\sv 8;" + boost::lexical_cast<std::wstring>(m_aWrapPoints.size());
		for( int i = 0; i < m_aWrapPoints.size(); i ++ )
            sResult += L";(" + boost::lexical_cast<std::wstring>(m_aWrapPoints[i].first) + L"," + boost::lexical_cast<std::wstring>(m_aWrapPoints[i].second) + L")";
		sResult += L"}}";
	}
//WordArt
	if( PROP_DEF != m_bGtext )
	{
        RENDER_RTF_SHAPE_PROP(L"fGtext",    sResult,   m_bGtext );
		
		int nCodePage = -1;
		
        if( !m_sGtextFont.empty() )
		{
			sResult += L"{\\sp{\\sn gtextFont}{\\sv ";
			sResult += m_sGtextFont + L"}}";

	//---------------------------------
			RtfFont oFont;
			if( true == pDocument->m_oFontTable.GetFont( m_sGtextFont, oFont ) )
			{
				if( PROP_DEF != oFont.m_nCharset )
					nCodePage = RtfUtility::CharsetToCodepage( oFont.m_nCharset );
				else if( PROP_DEF != oFont.m_nCodePage )
					nCodePage = oFont.m_nCodePage;
			}
		}

        if( !m_sGtextUNICODE.empty() )
		{
			sResult += L"{\\sp{\\sn gtextUNICODE}{\\sv ";
				sResult += RtfChar::renderRtfText(m_sGtextUNICODE, oRenderParameter.poDocument, nCodePage);
			sResult += L"}}";
		}

        RENDER_RTF_SHAPE_PROP(L"gtextSize",         sResult,   	m_nGtextSize );
        RENDER_RTF_SHAPE_PROP(L"gtextFVertical",    sResult,   m_bGtextFVertical);
        RENDER_RTF_SHAPE_PROP(L"gtextFKern",        sResult,   	m_bGtextFKern);
        RENDER_RTF_SHAPE_PROP(L"gtextFStretch",     sResult,   m_bGtextFStretch);
        RENDER_RTF_SHAPE_PROP(L"gtextFShrinkFit",   sResult,   m_bGtextFShrinkFit);
        RENDER_RTF_SHAPE_PROP(L"gtextFBestFit",     sResult,   m_bGtextFBestFit);
	}

	return sResult;
}
std::wstring RtfShape::RenderToOOX(RenderParameter oRenderParameter)
{
	if( PROP_DEF == m_nShapeType ) 
		return L"";

    std::wstring sResult;
	RtfDocument* poDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	
	TextItemContainerPtr aTempTextItems;
	
	if( NSOfficeDrawing::sptPictureFrame == m_nShapeType && 0 != m_aTextItems )
	{//Значит это Ole обьект с anchor, для него все также только TextBox надо делать по-другому
		aTempTextItems	= m_aTextItems;
		m_aTextItems	= TextItemContainerPtr();
		m_bIsOle		= true;
	}

	sResult = RenderToOOXBegin(oRenderParameter);
	
    if( !sResult.empty() )
		sResult +=  RenderToOOXEnd(oRenderParameter);

    std::wstring sOle;
	if( 0 != aTempTextItems )
	{//пишем только Ole обьект
		//ищем первый ole обьект
		RtfOlePtr poFirstOle;
		int nTempTextItemsCount = aTempTextItems->GetCount();
		for( int i = 0; i < nTempTextItemsCount; i++ )
		{
			ITextItemPtr piCurTextItem;
			aTempTextItems->GetItem( piCurTextItem, i );
			if( NULL != piCurTextItem && TYPE_RTF_PARAGRAPH == piCurTextItem->GetType() )
			{
				RtfParagraphPtr poCurParagraph = boost::static_pointer_cast< RtfParagraph, ITextItem >( piCurTextItem );
				if( NULL != poCurParagraph )
				{
					bool bBreak = false;
					for( int j = 0; j < poCurParagraph->GetCount(); j++ )
					{
						IDocumentElementPtr piCurIDocumentElement;
						poCurParagraph->GetItem( piCurIDocumentElement, j );
						if( NULL != piCurIDocumentElement && TYPE_RTF_OLE == piCurIDocumentElement->GetType() )
						{
							//рендерим только Ole часть
							RenderParameter oNewParam = oRenderParameter;
							oNewParam.nType = RENDER_TO_OOX_PARAM_OLE_ONLY;
							oNewParam.nValue = m_nID;

							RtfOlePtr poCurOle = boost::static_pointer_cast< RtfOle, IDocumentElement >( piCurIDocumentElement );
							if( NULL != poCurOle )
							{
								sOle += poCurOle->RenderToOOX( oNewParam );
								bBreak = true;
								break;
							}
						}
					}
					if( true == bBreak )
						break;
				}
			}
		}
		//возвращаем text box на место
		m_aTextItems = aTempTextItems;
	}
	
    if( !sOle.empty() && !sResult.empty())
	{
        XmlUtils::replace_all(sResult, L"</w:pict>", sOle + L"</w:pict>" );//todooo переписать
	}
	
	return sResult;
}
std::wstring RtfShape::GetShapeNodeName(int type)
{
	switch(type)
	{
		case NSOfficeDrawing::sptRectangle:			return L"v:rect";
		case NSOfficeDrawing::sptEllipse:			return L"v:oval";
		case NSOfficeDrawing::sptRoundRectangle:	return L"v:roundrect";
		case NSOfficeDrawing::sptLine:				return L"v:line";
		case NSOfficeDrawing::sptArc:				return L"v:arc";
		default:									return L"v:shape";
	}
}
std::wstring RtfShape::RenderToOOXBegin(RenderParameter oRenderParameter)
{
	if( !IsValid() ) return L"";

    std::wstring			sResult;
	
	RtfDocument*	poRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		poOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);

	m_bInsert = false;
	m_bDelete = false;
	
	if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2 == oRenderParameter.nType )
		;//child shape
	else if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE == oRenderParameter.nType )
	{//pic bullets
		sResult += L"<w:pict>";
	}
	else
	{//работает по умолчанию
		if (m_oCharProperty.m_nRevised != PROP_DEF)
		{
			m_bInsert = true;
			
            std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_oCharProperty.m_nRevauth);
            std::wstring sDate(RtfUtility::convertDateTime(m_oCharProperty.m_nRevdttm).c_str());
			
			sResult += L"<w:ins w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + boost::lexical_cast<std::wstring>(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_oCharProperty.m_nRevised = PROP_DEF;
		}
		if (m_oCharProperty.m_nDeleted != PROP_DEF)
		{
			m_bDelete = true;
			
            std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_oCharProperty.m_nRevauthDel);
            std::wstring sDate(RtfUtility::convertDateTime(m_oCharProperty.m_nRevdttmDel).c_str());
			
			sResult += L"<w:del w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + boost::lexical_cast<std::wstring>(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_oCharProperty.m_nDeleted = PROP_DEF;
		}
        std::wstring sCharProp = m_oCharProperty.RenderToOOX(oRenderParameter);
		sResult += L"<w:r>";
        if (!sCharProp .empty())
		{
			sResult += L"<w:rPr>";
				sResult += sCharProp;
			sResult += L"</w:rPr>";
		}
		sResult += L"<w:pict>";
	}
	
    if (oRenderParameter.sValue.empty())
		oRenderParameter.sValue = GetShapeNodeName(m_nShapeType);

	sResult += L"<" + oRenderParameter.sValue;

    if (m_sName.empty())
	{
		RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
        m_sName += L"_x0000_s " + boost::lexical_cast<std::wstring>(poDocument->GetShapeId( m_nID )) + L"";
	}
	sResult += L" id=\"" + m_sName + L"\"";

	if( PROP_DEF != m_nShapeType && 0 != m_nShapeType)
	{
        sResult += L" type=\"#_x0000_t " + boost::lexical_cast<std::wstring>(m_nShapeType) + L"\"";
        sResult += L" o:spt=\" " + boost::lexical_cast<std::wstring>(m_nShapeType) + L"\"";
	}

	if( 0 == m_bFilled) sResult += L" filled=\"f\"";
	else				sResult += L" filled=\"t\"";

	if( 0 == m_bLine)	sResult += L" stroked=\"f\"";
	else				sResult += L" stroked=\"t\"";

	if( PROP_DEF != m_nFillColor)
	{
		RtfColor color(m_nFillColor);
        sResult += L" fillcolor=\"#" + color.ToHexColor(true) + L"\"";
	}
	if( PROP_DEF != m_nLineColor)
	{
		RtfColor color(m_nLineColor);
        sResult += L" strokecolor=\"#" + color.ToHexColor(true) + L"\"";
	}
	if(PROP_DEF != m_nLineWidth)
        sResult += L" strokeweight=\"" + XmlUtils::DoubleToString(RtfUtility::Emu2Pt(m_nLineWidth), L"%.2f") + L"pt\"";
//path
	switch( m_nConnectionType )
	{
		case 0: sResult += L" o:connecttype=\"custom\"";		break;
		case 1: sResult += L" o:connecttype=\"none\"";		break;
		case 2: sResult += L" o:connecttype=\"rect\"";		break;
		case 3: sResult += L" o:connecttype=\"segments\"";	break;
	}
//Connectors
	switch( m_nConnectorStyle )
	{
		case 0: sResult += L" o:connectortype=\"straight\"";	break;
		case 1: sResult += L" o:connectortype=\"elbow\"";	break;
		case 2: sResult += L" o:connectortype=\"curved\"";	break;
		case 3: sResult += L" o:connectortype=\"none\"";		break;
	}

//-----------------------------------------------------------------------------------------------------------------
    std::wstring sStyle ;
	if( PROP_DEF != m_nLeft &&  PROP_DEF != m_nRight && PROP_DEF != m_nTop && PROP_DEF != m_nBottom   )
	{
		//не пишем если inline
		if( 3 != m_nPositionHRelative || 3 != m_nPositionVRelative )
		{
            sStyle += L"position:absolute;";
			if (oRenderParameter.nType !=  RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2)
			{
                sStyle += L"margin-left:"   + XmlUtils::DoubleToString(RtfUtility::Twip2pt(m_nLeft), L"%.2f") + L"pt;";
                sStyle += L"margin-top:"    + XmlUtils::DoubleToString(RtfUtility::Twip2pt(m_nTop), L"%.2f") + L"pt;";
                sStyle += L"margin-bottom:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt(m_nBottom), L"%.2f") + L"pt;";
                sStyle += L"margin-right:"  + XmlUtils::DoubleToString(RtfUtility::Twip2pt(m_nRight), L"%.2f") + L"pt;";
			}
		}
		int nWidth = m_nRight - m_nLeft;
		int nHeight = m_nBottom - m_nTop;
		
		if (oRenderParameter.nType ==  RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2)
            sStyle += L"width:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt(nWidth), L"%.2f") + L";height:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt(nHeight), L"%.2f");
		else
            sStyle += L"width:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt(nWidth), L"%.2f") + L"pt;height:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt(nHeight), L"%.2f") + L"pt;";
	}
	else if( PROP_DEF != m_nRelLeft &&  PROP_DEF != m_nRelRight && PROP_DEF != m_nRelTop && PROP_DEF != m_nRelBottom  )
	{
		int nWidth	= m_nRelRight - m_nRelLeft;
		int nHeight = m_nRelBottom - m_nRelTop;
		
        sStyle += L"position:absolute;";
        sStyle += L"left: " + boost::lexical_cast<std::wstring>(m_nRelLeft) + L";";
        sStyle += L"top: " + boost::lexical_cast<std::wstring>(m_nRelTop) + L";";
        //sStyle += L"bottom: " + boost::lexical_cast<std::wstring>() + L";"			, m_nRelBottom );
        //sStyle += L"right: " + boost::lexical_cast<std::wstring>() + L";"			, m_nRelRight);
        sStyle += L"width: " + boost::lexical_cast<std::wstring>(nWidth) + L";height: " + boost::lexical_cast<std::wstring>(nHeight) + L";";
	}
	else if( 0 != m_oPicture && PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != (int)m_oPicture->m_dScaleX && PROP_DEF != (int)m_oPicture->m_dScaleY )
	{
		float nWidth = (int)(m_oPicture->m_nWidthGoal * m_oPicture->m_dScaleX / 100.);
		if( PROP_DEF != m_oPicture->m_nCropL )
			nWidth -= m_oPicture->m_nCropL;
		if( PROP_DEF != m_oPicture->m_nCropR )
			nWidth -= m_oPicture->m_nCropR;

		float nHeight = (int)(m_oPicture->m_nHeightGoal * m_oPicture->m_dScaleY / 100.);
		if( PROP_DEF != m_oPicture->m_nCropT )
			nHeight -= m_oPicture->m_nCropT;
		if( PROP_DEF != m_oPicture->m_nCropB )
			nHeight -= m_oPicture->m_nCropB;

		if (oRenderParameter.nType ==  RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2)
            sStyle += L"width:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt(nWidth), L"%.2f") + L";height:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt(nHeight), L"%.2f") + L";";
		else
            sStyle += L"width:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt(nWidth), L"%.2f") + L"pt;height:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt(nHeight), L"%.2f") + L"pt;";
	}

	switch( m_nPositionH )
	{
		case 0: sStyle += L"mso-position-horizontal:absolute;";	break;
		case 1: sStyle += L"mso-position-horizontal:left;";		break;
		case 2: sStyle += L"mso-position-horizontal:center;";	break;
        case 3: sStyle += L"mso-position-horizontal:right;";	break;
		case 4: sStyle += L"mso-position-horizontal:inside;";	break;
		case 5: sStyle += L"mso-position-horizontal:outside;";	break;
	}
	if( PROP_DEF != m_nPositionHPct )//todo
	{
        sStyle += L"mso-left-percent: " + boost::lexical_cast<std::wstring>(m_nPositionHPct) + L";";
	}
	if( PROP_DEF != m_nPositionH && PROP_DEF == m_nPositionHRelative )
		m_nPositionHRelative = 2;
	if( PROP_DEF != m_nPositionHRelative )
	{
		switch( m_nPositionHRelative )
		{
			case 0: sStyle += L"mso-position-horizontal-relative:margin;";				break;
            case 1: sStyle += L"mso-position-horizontal-relative:page;";				break;
            case 2: sStyle += L"mso-position-horizontal-relative:text;";				break;
            case 3: sStyle += L"mso-position-horizontal-relative:char;";				break;
            case 4: sStyle += L"mso-position-horizontal-relative:left-margin-area;";	break;
			case 5: sStyle += L"mso-position-horizontal-relative:right-margin-area;";	break;
			case 6: sStyle += L"mso-position-horizontal-relative:inner-margin-area;";	break;
			case 7: sStyle += L"mso-position-horizontal-relative:outer-margin-area;";	break;
		}
	}
	else
	{
		switch( m_eXAnchor )
		{
            case ax_page:	sStyle += L"mso-position-horizontal-relative:page;";	break;
			case ax_margin: sStyle += L"mso-position-horizontal-relative:margin;";	break;
            //case ax_column: sStyle += L"mso-position-horizontal-relative:text;"); break;
		}
	}

	switch( m_nPositionV )
	{
		case 0: sStyle += L"mso-position-vertical:absolute;";	break;
        case 1: sStyle += L"mso-position-vertical:top;";		break;
		case 2: sStyle += L"mso-position-vertical:center;";		break;
		case 3: sStyle += L"mso-position-vertical:bottom;";		break;
		case 4: sStyle += L"mso-position-vertical:inside;";		break;
        case 5: sStyle += L"mso-position-vertical:outside;";	break;
	}
	if( PROP_DEF != m_nPositionVPct )
        sStyle += L"mso-top-percent: " + boost::lexical_cast<std::wstring>(m_nPositionVPct) + L";";

	if( PROP_DEF != m_nPositionV && PROP_DEF == m_nPositionVRelative )
		m_nPositionVRelative =2;
	if( PROP_DEF != m_nPositionVRelative )
	{
		switch( m_nPositionVRelative )
		{
            case 0: sStyle += L"mso-position-vertical-relative:margin;";			break;
			case 1: sStyle += L"mso-position-vertical-relative:page;";				break;
			case 2: sStyle += L"mso-position-vertical-relative:text;";				break;
			case 3: sStyle += L"mso-position-vertical-relative:line;";				break;
			case 4: sStyle += L"mso-position-vertical-relative:top-margin-area;";	break;
            case 5: sStyle += L"mso-position-vertical-relative:bottom-margin-area;";break;
			case 6: sStyle += L"mso-position-vertical-relative:inner-margin-area;";	break;
			case 7: sStyle += L"mso-position-vertical-relative:outer-margin-area;";	break;
		}
	}
	else
	{
		switch( m_eYAnchor )
		{
			case ay_page: sStyle += L"mso-position-vertical-relative:page;";			break;
			case ay_margin: sStyle += L"mso-position-vertical-relative:margin;";		break;
            //case ay_Para: sStyle += L"mso-position-vertical-relative:text;";          break;
		}
	}
	if( PROP_DEF != m_nPctWidth )
        sStyle += L"mso-width-percent: " + boost::lexical_cast<std::wstring>(m_nPctWidth) + L";";
	switch( m_nPctWidthRelative )
	{
        case 0:	sStyle += L"mso-width-relative:margin;";			break;
		case 1:	sStyle += L"mso-width-relative:page;";				break;
		case 2:	sStyle += L"mso-width-relative:left-margin-area;";	break;
		case 3:	sStyle += L"mso-width-relative:right-margin-area;";	break;
		case 4:	sStyle += L"mso-width-relative:inner-margin-area;";	break;
		case 5:	sStyle += L"mso-width-relative:outer-margin-area;";	break;
	}
	
	if( PROP_DEF != m_nPctHeight )
        sStyle += L"mso-height-percent: " + boost::lexical_cast<std::wstring>(m_nPctHeight) + L";";
	
	switch( m_nPctHeightRelative )
	{
		case 0:	sStyle += L"mso-height-relative:margin;";				break;
		case 1:	sStyle += L"mso-height-relative:page;";					break;
		case 2:	sStyle += L"mso-height-relative:top-margin-area;";		break;
		case 3:	sStyle += L"mso-height-relative:bottom-margin-area;";	break;
        case 4:	sStyle += L"mso-height-relative:inner-margin-area;";	break;
        case 5:	sStyle += L"mso-height-relative:outer-margin-area;";	break;
	}

	if( PROP_DEF != m_nRotation )
        sStyle += L"rotation: " + boost::lexical_cast<std::wstring>(m_nRotation / 65536) + L";";
	else if( PROP_DEF != m_nRelRotation )
        sStyle += L"rotation: " + boost::lexical_cast<std::wstring>(m_nRelRotation / 65536) + L";";

	int nZIndex = PROP_DEF;
	if( PROP_DEF != m_nRelZOrder )
		nZIndex = m_nRelZOrder;
	else if( PROP_DEF != m_nZOrder )
		nZIndex = m_nZOrder;
	else if (oRenderParameter.nType !=  RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2)
		nZIndex = 100; //на свое усмотрение ставлю 100

	if( PROP_DEF != m_nZOrderRelative && PROP_DEF != nZIndex)
	{
		//берем большое значение чтобы сделать строго выше или ниже текста		
		if( 0 == m_nZOrderRelative )	nZIndex += 10000;
		else							nZIndex -= 10000;
	}
	if (PROP_DEF != nZIndex)
        sStyle += L"z-index: " + boost::lexical_cast<std::wstring>(nZIndex) + L";";

	if(  PROP_DEF != m_nWrapDistLeft )
        sStyle += L"mso-wrap-distance-left:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt( m_nWrapDistLeft ), L"%.2f") + L"pt;";
	if(  PROP_DEF != m_nWrapDistTop )
        sStyle += L"mso-wrap-distance-top:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt( m_nWrapDistTop ), L"%.2f") + L"pt;";
	if(  PROP_DEF != m_nWrapDistRight ) 
        sStyle += L"mso-wrap-distance-right:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt( m_nWrapDistRight ), L"%.2f") + L"pt;";
	if(  PROP_DEF != m_nWrapDistBottom )
        sStyle += L"mso-wrap-distance-bottom:" + XmlUtils::DoubleToString(RtfUtility::Twip2pt( m_nWrapDistBottom ), L"%.2f") + L"pt;";

	switch( m_nAnchorText)
	{
        case 0:	sStyle += L"v-text-anchor:top;";					break;
		case 1:	sStyle += L"v-text-anchor:middle;";					break;
		case 2:	sStyle += L"v-text-anchor:bottom;";					break;
		case 3:	sStyle += L"v-text-anchor:topcenter;";				break;
		case 4:	sStyle += L"v-text-anchor:middle-center;";			break;
		case 5:	sStyle += L"v-text-anchor:bottom-center;";			break;
		case 6:	sStyle += L"v-text-anchor:top-baseline;";			break;
        case 7:	sStyle += L"v-text-anchor:bottom-baseline;";		break;
        case 8:	sStyle += L"v-text-anchor:top-center-baseline;";	break;
		case 9:	sStyle += L"v-text-anchor:bottom-center-baseline;";	break;
	}

//---------------------------------------------------------------------------------------------------------------------------
    if( false == sStyle.empty() )
	{
        sStyle.erase( sStyle.length() - 1 );
        sResult += L" style=\"" + sStyle + L"\"";
	}
//----------------------------------------------------------------------------------------------------------------------------
	if( true == m_bIsOle )		sResult += L" o:ole=\"\"";
	
	if( PROP_DEF != m_nGroupLeft && PROP_DEF != m_nGroupTop )
        sResult += L" coordorigin=\" " + boost::lexical_cast<std::wstring>(m_nGroupLeft) + L", " + boost::lexical_cast<std::wstring>(m_nGroupTop) + L"\"";
	
	if( PROP_DEF != m_nGroupLeft && PROP_DEF != m_nGroupTop && PROP_DEF != m_nGroupRight && PROP_DEF != m_nGroupBottom)
        sResult += L" coordsize=\" " + boost::lexical_cast<std::wstring>(m_nGroupRight - m_nGroupLeft) + L", " + boost::lexical_cast<std::wstring>(m_nGroupBottom - m_nGroupTop) + L"\"";
	else if ( PROP_DEF != m_nGeoLeft && PROP_DEF != m_nGeoTop && PROP_DEF != m_nGeoRight && PROP_DEF != m_nGeoBottom)
        sResult += L" coordsize=\" " + boost::lexical_cast<std::wstring>(m_nGeoRight - m_nGeoLeft) + L", " + boost::lexical_cast<std::wstring>(m_nGeoBottom - m_nGeoTop) + L"\"";

	if (oRenderParameter.nType !=  RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2)
	{
		if( PROP_DEF != m_bLayoutInCell )
		{
			if( 0 == m_bLayoutInCell )
				sResult += L" o:allowincell=\"false\"";
			else
				sResult += L" o:allowincell=\"true\"";
		}
		if( PROP_DEF != m_bAllowOverlap )
		{
			if( 0 == m_bAllowOverlap )
				sResult += L" o:allowoverlap=\"false\"";
			else
				sResult += L" o:allowoverlap=\"true\"";
		}
	}
//Geometry
	if( PROP_DEF != m_nAdjustValue[0] )
	{
        std::wstring sAdjust;
        sAdjust += L" " + boost::lexical_cast<std::wstring>(m_nAdjustValue[0]) + L"";
		for (int i = 1 ; i < 10; i++)
		{
			if (PROP_DEF != m_nAdjustValue[i])
                sAdjust += L", " + boost::lexical_cast<std::wstring>(m_nAdjustValue[i]) + L"";
			else
				sAdjust += L",";
		}
		sResult +=L" adj=\"" + sAdjust + L"\"";
	}
//Custom
	if (!m_aPVerticles.empty() || !m_aPSegmentInfo.empty())
	{
		CPPTShape * custom_shape = CPPTShape::CreateByType((PPTShapes::ShapeType)m_nShapeType);
		if (custom_shape)
		{
			custom_shape->m_bCustomShape = true;
			
			custom_shape->m_oPath.SetCoordsize(m_nGeoRight - m_nGeoLeft, m_nGeoBottom - m_nGeoTop);
			
			for (int i = 0 ; i < 10; i++)
			{
				if (PROP_DEF != m_nAdjustValue[i])
					custom_shape->m_oCustomVML.LoadAdjusts(i + 1, m_nAdjustValue[i]);
			}
			
			if (PROP_DEF != m_nShapePath)
				custom_shape->m_oCustomVML.SetPath((NSPresentationEditor::RulesType)m_nShapePath);
			
			custom_shape->m_oCustomVML.LoadVertices(m_aPVerticles);
			custom_shape->m_oCustomVML.LoadSegments(m_aPSegmentInfo);

			custom_shape->m_oCustomVML.ToCustomShape(custom_shape, custom_shape->m_oManager);
			
			sResult +=L" path=\"" + custom_shape->m_strPath + L"\"";
		}
	}
//Wrap Geometry
	if( !m_aWrapPoints.empty())
	{
		sResult += L" wrapcoords=\"";
        sResult += L" " + boost::lexical_cast<std::wstring>(m_aWrapPoints[0].first) + L", " + boost::lexical_cast<std::wstring>(m_aWrapPoints[0].second) + L"";
		
		for( int i = 0; i < (int)m_aWrapPoints.size(); i++ )
		{
            sResult += L", " + boost::lexical_cast<std::wstring>(m_aWrapPoints[i].first) + L", " + boost::lexical_cast<std::wstring>(m_aWrapPoints[i].second) + L"";
		}
		
		sResult += L"\"";
	}
	
	sResult += L">";
//-------------------------------------------------------------------------------------------------------------- nodes	
	if( PROP_DEF != m_nWrapType && 3 != m_nWrapType)
	{
		sResult += L"<w10:wrap";

		switch( m_nWrapType )
		{
            case 1:sResult += L" type=\"topAndBottom\"";break;
			case 2:sResult += L" type=\"square\"";		break;
            case 3:sResult += L" type=\"none\"";		break;
			case 4:sResult += L" type=\"tight\"";		break;
			case 5:sResult += L" type=\"through\"";		break;
		}
		switch( m_nWrapSideType )
		{
            case 0:sResult += L" side=\"both\"";		break;
            case 1:sResult += L" side=\"left\"";		break;
			case 2:sResult += L" side=\"right\"";		break;
			case 3:sResult += L" side=\"largest\"";		break;
		}
		sResult += L"/>";
	}
	
//Line
	if( 0 != m_bLine)
	{
        std::wstring sStroke;
		switch( m_nLineDashing )
		{
			case 1: sStroke += L" dashstyle=\"shortdash\"";		break;
			case 2: sStroke += L" dashstyle=\"shortdot\"";		break;
			case 3: sStroke += L" dashstyle=\"shortdashdot\"";	break;
			case 4: sStroke += L" dashstyle=\"shortdashdotdot\"";break;
			case 5: sStroke += L" dashstyle=\"dot\"";			break;
			case 6: sStroke += L" dashstyle=\"dash\"";			break;
			case 7: sStroke += L" dashstyle=\"longdash\"";		break;
			case 8: sStroke += L" dashstyle=\"dashdot\"";		break;
			case 9: sStroke += L" dashstyle=\"longdashdot\"";	break;
			case 10: sStroke += L" dashstyle=\"longdashdotdot\"";break;
		}
		switch( m_nLineStartArrow )
		{
			case 0: sStroke += L" startarrow=\"none\"";		break;
            case 1: sStroke += L" startarrow=\"block\"";	break;
			case 2: sStroke += L" startarrow=\"classic\"";	break;
			case 3: sStroke += L" startarrow=\"diamond\"";	break;
			case 4: sStroke += L" startarrow=\"oval\"";		break;
			case 5: sStroke += L" startarrow=\"open\"";		break;
            case 6: sStroke += L" startarrow=\"block\"";	break;
            case 7: sStroke += L" startarrow=\"block\"";	break;
		}
		switch( m_nLineStartArrowLength )
		{
			case 0: sStroke += L" startarrowlength=\"short\"";	break;
			case 1: sStroke += L" startarrowlength=\"medium\"";	break;
			case 2: sStroke += L" startarrowlength=\"long\"";	break;
		}
		switch( m_nLineStartArrowWidth )
		{
			case 0: sStroke += L" startarrowwidth=\"narrow\"";	break;
			case 1: sStroke += L" startarrowwidth=\"medium\"";	break;
            case 2: sStroke += L" startarrowwidth=\"wide\"";	break;
		}
		switch( m_nLineEndArrow )
		{
			case 0: sStroke += L" endarrow=\"none\"";	break;
			case 1: sStroke += L" endarrow=\"block\"";	break;
            case 2: sStroke += L" endarrow=\"classic\"";break;
            case 3: sStroke += L" endarrow=\"diamond\"";break;
			case 4: sStroke += L" endarrow=\"oval\"";	break;
			case 5: sStroke += L" endarrow=\"open\"";	break;
			case 6: sStroke += L" endarrow=\"block\"";	break;
			case 7: sStroke += L" endarrow=\"block\"";	break;
		}
		switch( m_nLineEndArrowLength )
		{
            case 0: sStroke += L" endarrowlength=\"short\"";	break;
			case 1: sStroke += L" endarrowlength=\"medium\"";	break;
			case 2: sStroke += L" endarrowlength=\"long\"";		break;
		}
		switch( m_nLineEndArrowWidth )
		{
			case 0: sStroke += L" endarrowwidth=\"narrow\"";	break;
			case 1: sStroke += L" endarrowwidth=\"medium\"";	break;
            case 2: sStroke += L" endarrowwidth=\"wide\"";      break;
		}

        if( false == sStroke.empty())
		{
			sResult += L"<v:stroke " + sStroke + L"/>";
		}
	}

	if( 0 != m_aTextItems )
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

		sResult += L"<v:textbox";
		if (m_nTexpLeft != PROP_DEF && m_nTexpTop != PROP_DEF && m_nTexpRight != PROP_DEF && m_nTexpBottom != PROP_DEF)
		{
            sResult += L" inset=\" " + boost::lexical_cast<std::wstring>((int)RtfUtility::Emu2Pt(m_nTexpLeft)) + L"pt,  "
                                    + boost::lexical_cast<std::wstring>((int)RtfUtility::Emu2Pt(m_nTexpTop)) + L"pt,  "
                                    + boost::lexical_cast<std::wstring>((int)RtfUtility::Emu2Pt(m_nTexpRight)) + L"pt,  "
                                    + boost::lexical_cast<std::wstring>((int)RtfUtility::Emu2Pt(m_nTexpBottom)) + L"pt\">";
		}
		else  
			sResult += L">";
		
		sResult += L"<w:txbxContent>";
		sResult +=  m_aTextItems->RenderToOOX(oNewParam);
		sResult += L"</w:txbxContent>";
		sResult += L"</v:textbox>";
	}
	
    std::wstring sPicture;
	if( m_oPicture )
	{
		sPicture = m_oPicture->RenderToOOX(oRenderParameter);
		
		if (m_nShapeType == PROP_DEF || m_nShapeType == 75)
		{
            if( sPicture.empty() )//если не сохранилась картинка, то весь shape-picture будет бесполезным
				return L"";

 			int nCropLeft	= PROP_DEF;
			int nCropTop	= PROP_DEF;
			int nCropRight	= PROP_DEF;
			int nCropBottom = PROP_DEF;

			if( PROP_DEF != m_nCropFromLeft )
				nCropLeft = m_nCropFromLeft;
			else if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nCropL )
				nCropLeft = (int)( 65536 * ( 1.0 * m_oPicture->m_nCropL / m_oPicture->m_nWidthGoal) ); //This numeric value can also be specified in 1/65536-ths if a trailing "f" is supplied
			if( PROP_DEF != m_nCropFromTop )
				nCropTop = m_nCropFromTop;
			else if( PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != m_oPicture->m_nCropT )
				nCropTop = (int)( 65536 * ( 1.0 * m_oPicture->m_nCropT / m_oPicture->m_nHeightGoal) );
			if( PROP_DEF != m_nCropFromRight )
				nCropRight = m_nCropFromRight;
			else if( PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nCropR )
				nCropRight = (int)( 65536 * ( 1.0 * m_oPicture->m_nCropR / m_oPicture->m_nWidthGoal) );
			if( PROP_DEF != m_nCropFromBottom )
				nCropBottom = m_nCropFromBottom;
			else if( PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != m_oPicture->m_nCropB )
				nCropBottom = (int)( 65536 * ( 1.0 * m_oPicture->m_nCropB / m_oPicture->m_nHeightGoal) );

			sResult += L"<v:imagedata r:id=\"" + sPicture + L"\"";

			if( PROP_DEF != nCropLeft )
                sResult += L" cropleft=\" " + boost::lexical_cast<std::wstring>(nCropLeft) + L"f\"";
			if( PROP_DEF != nCropTop )
                sResult += L" croptop=\" " + boost::lexical_cast<std::wstring>(nCropTop) + L"f\"";
			if( PROP_DEF != nCropRight )
                sResult += L" cropright=\" " + boost::lexical_cast<std::wstring>(nCropRight) + L"f\"";
			if( PROP_DEF != nCropBottom )
                sResult += L" cropbottom=\" " + boost::lexical_cast<std::wstring>(nCropBottom) + L"f\"";

			sResult += L" o:title=\"\"/>";
		}
	}
//-----------------------------------------------------------------------------------------------
	if( 0 != m_bFilled)
	{
		sResult += L"<v:fill";

        if (!sPicture.empty() && m_nShapeType != 75)
		{
 			sResult += L" r:id=\"" + sPicture + L"\"";
			
			if( PROP_DEF == m_nFillType) m_nFillType = 2;
		}
		switch(m_nFillType)
		{
		case 1:	sResult += L" type=\"pattern\"";		break;
		case 2:	sResult += L" type=\"tile\"";		break;
		case 3:	sResult += L" type=\"frame\"";		break;
		case 4:	sResult += L" type=\"gradient\"";	break;
		case 5:	sResult += L" type=\"gradient\"";	break;
		case 6:	sResult += L" type=\"gradient\"";	break;
		case 7:	sResult += L" type=\"gradient\"";	break;
		case 8:	sResult += L" type=\"gradient\"";	break;
		case 9:	sResult += L" type=\"gradient\"";	break;
		}//todooo center radial ...

		if ( PROP_DEF != m_nFillOpacity)
		{
            std::wstring sOpacity = boost::lexical_cast<std::wstring>( /*100 - */m_nFillOpacity);
			sResult += L" opacity=\"" + sOpacity +L"%\"";
            //sResult += L" opacity=\" " + boost::lexical_cast<std::wstring>(m_nFillOpacity) + L"f\"";
		}
		if ( PROP_DEF != m_nFillColor2)
		{
			RtfColor color(m_nFillColor2);
			sResult += L" color2=\"#" + color.ToHexColor(true) + L"\"";
		}
		if ( PROP_DEF != m_nFillFocus)
		{
            sResult += L" focus=\""+ boost::lexical_cast<std::wstring>(m_nFillFocus) + L"%\"";
		}
		
		if ( PROP_DEF != m_nFillAngle)
            sResult += L" angle=\"" + boost::lexical_cast<std::wstring>(m_nFillAngle) + L"\"";

		sResult += L"/>";
	}
//---------------------------------------------------------------------------------------------------------------------------
    if( false == m_sGtextUNICODE.empty())
	{
		sResult += L"<v:textpath"; 

        std::wstring sTextStyle;

        if ( !m_sGtextFont.empty() )
		{
			sTextStyle += L"font-family:" + XmlUtils::EncodeXmlString(m_sGtextFont) + L";";//todooo значения как в кавычках так и без - проверить как без
		}

        if (!sTextStyle.empty())
			sResult += L" style=\"" + sTextStyle + L"\"";

		if ( PROP_DEF != m_nGtextSize )
            sTextStyle += L"font-size: " + boost::lexical_cast<std::wstring>(m_nGtextSize) + L"pt;";

		sResult += L" string=\"" + XmlUtils::EncodeXmlString(m_sGtextUNICODE) + L"\"";
		sResult += L"/>";
	}
	return sResult;
}
std::wstring RtfShape::RenderToOOXEnd(RenderParameter oRenderParameter)
{
    std::wstring sResult;

    if (oRenderParameter.sValue.empty())
		oRenderParameter.sValue = GetShapeNodeName(m_nShapeType);

	sResult += L"</" + oRenderParameter.sValue + L">";

	if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2 == oRenderParameter.nType )
		;
	else if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE == oRenderParameter.nType )
		sResult += L"</w:pict>";
	else
	{
		sResult += L"</w:pict></w:r>";//работает по умолчанию
		
		if (m_bDelete)	sResult += L"</w:del>";
		if (m_bInsert)	sResult += L"</w:ins>";
	}
	return sResult;
}
std::wstring RtfShapeGroup::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	//запоминаем координаты и если нужно поворачиваем
	int nLeft		= m_nLeft;
	int nTop		= m_nTop;
	int nRight		= m_nRight;
	int nBottom		= m_nBottom;
	int nRotate		= m_nRotation;

	int nRelLeft	= m_nRelLeft;
	int nRelTop		= m_nRelTop;
	int nRelRight	= m_nRelRight;
	int nRelBottom	= m_nRelBottom;
	int nRelRotate	= m_nRelRotation;

	if( PROP_DEF != m_nRotation && PROP_DEF != m_nLeft && PROP_DEF != m_nTop &&
		PROP_DEF != m_nRight && PROP_DEF != m_nBottom )
		ToRtfRotation( m_nRotation, m_nLeft, m_nTop, m_nRight, m_nBottom );

	if( PROP_DEF != m_nRelRotation && PROP_DEF != m_nRelLeft && PROP_DEF != m_nRelTop &&
		PROP_DEF != m_nRelRight && PROP_DEF != m_nRelBottom )
		ToRtfRotation( m_nRelRotation, m_nRelLeft, m_nRelTop, m_nRelRight, m_nRelBottom );

	sResult += m_oCharProperty.RenderToRtf( oRenderParameter );

	if( st_inline == m_eAnchorTypeShape )
	{
		sResult += L"{\\shpgrp";
		sResult += L"{\\*\\shpinst";
		
		RENDER_RTF_INT( m_nID				, sResult, L"shplid" );
		
		if (!m_bInGroup)
		{	
			RENDER_RTF_INT( m_nLeft			, sResult, L"shpleft" );
			RENDER_RTF_INT( m_nTop			, sResult, L"shptop" );
			RENDER_RTF_INT( m_nBottom		, sResult, L"shpbottom" );
			RENDER_RTF_INT( m_nRight		, sResult, L"shpright" );
			RENDER_RTF_INT( m_nHeader		, sResult, L"shpfhdr" );
			
			sResult += L"\\shpbxignore";
			sResult += L"\\shpbyignore";

			RENDER_RTF_INT( m_nWrapType		, sResult, L"shpwr" );
			RENDER_RTF_INT( m_nWrapSideType	, sResult, L"shpwrk" );
			RENDER_RTF_BOOL( m_bLockAnchor	, sResult, L"shplockanchor" );
						
			//sResult += L"{\\sp{\\sn fUseShapeAnchor}{\\sv 0}}";
			//sResult += L"{\\sp{\\sn fPseudoInline}{\\sv 1}}";
		}

		
		sResult +=  RenderToRtfShapeProperty( oRenderParameter );
		
		for (int i = 0 ; i < m_aArray.size(); i++)
		{
			sResult +=  m_aArray[i]->RenderToRtf( oRenderParameter );
		}
		sResult += L"}";
		sResult += L"}";
	}
	else
	{
		sResult += L"{\\shpgrp";
		sResult += L"{\\*\\shpinst";
		
		RENDER_RTF_INT( m_nID				, sResult, L"shplid" );

		if (!m_bInGroup)
		{	
			RENDER_RTF_INT( m_nLeft			, sResult, L"shpleft" );
			RENDER_RTF_INT( m_nTop			, sResult, L"shptop" );
			RENDER_RTF_INT( m_nBottom		, sResult, L"shpbottom" );
			RENDER_RTF_INT( m_nRight		, sResult, L"shpright" );
			RENDER_RTF_INT( m_nZOrder		, sResult, L"shpz" );
			RENDER_RTF_INT( m_nHeader		, sResult, L"shpfhdr" );
			RENDER_RTF_INT( m_nWrapType		, sResult, L"shpwr" );
			RENDER_RTF_INT( m_nWrapSideType	, sResult, L"shpwrk" );
			RENDER_RTF_BOOL( m_bLockAnchor	, sResult, L"shplockanchor" );

			switch(m_eXAnchor)
			{
				case ax_page:	sResult += L"\\shpbxpage";	break;
				case ax_margin: sResult += L"\\shpbxmargin";	break;
				case ax_column: sResult += L"\\shpbxcolumn";	break;
			}
			sResult += L"\\shpbxignore";
			switch(m_eYAnchor)
			{
				case ay_page:	sResult += L"\\shpbypage";	break;
				case ay_margin: sResult += L"\\shpbymargin";	break;
				case ay_Para:	sResult += L"\\shpbypara";	break;
			}
			sResult += L"\\shpbyignore";
			RENDER_RTF_INT( m_nZOrderRelative, sResult, L"shpfblwtxt");
		}
        sResult += L"{\\sp{\\sn lidRegroup}{\\sv 0}}";

		sResult += RenderToRtfShapeProperty( oRenderParameter );

		for (int i = 0 ; i < m_aArray.size(); i++)
		{
			sResult +=  m_aArray[i]->RenderToRtf( oRenderParameter );
		}

		sResult += L"}";
		sResult += L"}";
	}
	//восстанавливаем координаты и если нужно поворачиваем
	m_nLeft			= nLeft;
	m_nTop			= nTop;
	m_nRight		= nRight;
	m_nBottom		= nBottom;
	m_nRotation		= nRotate;

	m_nRelLeft		= nRelLeft;
	m_nRelTop		= nRelTop;
	m_nRelRight		= nRelRight;
	m_nRelBottom	= nRelBottom;
	m_nRelRotation	= nRelRotate;

	return sResult;
}
std::wstring RtfShapeGroup::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	RenderParameter oNewParamGroup	= oRenderParameter;
	oNewParamGroup.sValue			= L"v:group";
	
	sResult = RenderToOOXBegin( oNewParamGroup );

	for( int i = 0; i < (int)m_aArray.size(); i++ )
	{
		RenderParameter oNewParamShape	= oRenderParameter;
		oNewParamShape.sValue			= L"";
		oNewParamShape.nType			= RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2; //in group
		
		sResult +=  m_aArray[i]->RenderToOOX( oNewParamShape );
	}

	sResult +=  RenderToOOXEnd( oNewParamGroup );
	return sResult;
}
