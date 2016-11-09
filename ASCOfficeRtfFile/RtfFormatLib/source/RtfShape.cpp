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
#include "RtfShape.h"
#include "Writer/OOXWriter.h"
#include "RtfOle.h"

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

CString RtfShape::RenderToRtf(RenderParameter oRenderParameter)
{
	if( PROP_DEF == m_nShapeType) return L"";
	
	CString sResult;
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

	if( st_inline == m_eAnchorTypeShape )
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
			RENDER_RTF_INT( m_nID			, sResult, L"shplid" );
			
			if (!m_bInGroup)
			{
				RENDER_RTF_INT( m_nLeft			, sResult, L"shpleft" );
				RENDER_RTF_INT( m_nTop			, sResult, L"shptop" );
				RENDER_RTF_INT( m_nBottom		, sResult, L"shpbottom" );
				RENDER_RTF_INT( m_nRight		, sResult, L"shpright" );
				RENDER_RTF_INT( m_nHeader		, sResult, L"shpfhdr" );
				RENDER_RTF_INT( m_nWrapType		, sResult, L"shpwr" );
				RENDER_RTF_INT( m_nWrapSideType	, sResult, L"shpwrk" );
				RENDER_RTF_BOOL( m_bLockAnchor	, sResult, L"shplockanchor" );
				
				sResult += L"\\shpbxignore";
				sResult += L"\\shpbyignore";
				
				sResult.AppendFormat( L"{\\sp{\\sn fUseShapeAnchor}{\\sv %d}}", false);
				sResult.AppendFormat( L"{\\sp{\\sn fPseudoInline}{\\sv %d}}", true);
			}
			
			sResult +=  RenderToRtfShapeProperty( oRenderParameter );
			
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
			//if( 0 != m_oPicture )
			//{
			//	sResult += L"{\\shprslt\\par\\plain";
			//	sResult +=  m_oPicture->GenerateWMF( oRenderParameter ) );
			//	sResult += L"\\par}";
			//}
			sResult += L"}";
		}
	}
	else
	{
		sResult += L"{\\shp";
		sResult += L"{\\*\\shpinst";
		
		RENDER_RTF_INT( m_nID			, sResult, L"shplid" );

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
CString RtfShape::RenderToRtfShapeProperty(RenderParameter oRenderParameter)
{
	CString sResult;

//Position absolute
	if( PROP_DEF != m_nPositionH )
		sResult.AppendFormat( L"{\\sp{\\sn posh}{\\sv %d}}",			m_nPositionH);
	if( PROP_DEF != m_nPositionHRelative )
		sResult.AppendFormat( L"{\\sp{\\sn posrelh}{\\sv %d}}",		m_nPositionHRelative);
	if( PROP_DEF != m_nPositionV )
		sResult.AppendFormat( L"{\\sp{\\sn posv}{\\sv %d}}",			m_nPositionV);
	
	if( PROP_DEF != m_nPositionVRelative )
		sResult.AppendFormat( L"{\\sp{\\sn posrelv}{\\sv %d}}",		m_nPositionVRelative);
	if(  PROP_DEF != m_bLayoutInCell )
		sResult.AppendFormat( L"{\\sp{\\sn fLayoutInCell}{\\sv %d}}", m_bLayoutInCell);
	if(  PROP_DEF != m_bAllowOverlap )
		sResult.AppendFormat( L"{\\sp{\\sn fAllowOverlap}{\\sv %d}}",m_bAllowOverlap);

//Position relative
	if(  PROP_DEF != m_nPositionHPct )
		sResult.AppendFormat( L"{\\sp{\\sn pctHorizPos}{\\sv %d}}",	m_nPositionHPct);
	if(  PROP_DEF != m_nPositionVPct )
		sResult.AppendFormat( L"{\\sp{\\sn pctVertPos}{\\sv %d}}",	m_nPositionVPct);
	if(  PROP_DEF != m_nPctWidth )
		sResult.AppendFormat( L"{\\sp{\\sn pctHoriz}{\\sv %d}}",		m_nPctWidth);
	if(  PROP_DEF != m_nPctHeight )
		sResult.AppendFormat( L"{\\sp{\\sn pctVert}{\\sv %d}}",		m_nPctHeight);
	if(  PROP_DEF != m_nPctWidthRelative )
		sResult.AppendFormat( L"{\\sp{\\sn sizerelh}{\\sv %d}}",		m_nPctWidthRelative);
	if(  PROP_DEF != m_nPctHeightRelative )
		sResult.AppendFormat( L"{\\sp{\\sn sizerelv}{\\sv %d}}",		m_nPctHeightRelative);
	if(  PROP_DEF != m_nColStart )
		sResult.AppendFormat( L"{\\sp{\\sn colStart}{\\sv %d}}",		m_nColStart);
	if(  PROP_DEF != m_nColSpan )
		sResult.AppendFormat( L"{\\sp{\\sn colSpan}{\\sv %d}}",		m_nColSpan);
//Rehydration
	if(  L"" != m_sMetroBlob )
        sResult.AppendFormat( L"{\\sp{\\sn metroBlob}{\\sv %d}}",	m_sMetroBlob.GetBuffer());

//Object Type
	if(  PROP_DEF != m_bIsBullet )
		sResult.AppendFormat( L"{\\sp{\\sn fIsBullet}{\\sv %d}}",	m_bIsBullet);
	if(  PROP_DEF != m_nRotation )
		sResult.AppendFormat( L"{\\sp{\\sn rotation}{\\sv %d}}",		m_nRotation);
	if(  PROP_DEF != m_bFlipV )
		sResult.AppendFormat( L"{\\sp{\\sn fFlipV}{\\sv %d}}",		m_bFlipV);
	if(  PROP_DEF != m_bFlipH )
		sResult.AppendFormat( L"{\\sp{\\sn fFlipH}{\\sv %d}}",		m_bFlipH);
	if(  PROP_DEF != m_nShapeType )
		sResult.AppendFormat( L"{\\sp{\\sn shapeType}{\\sv %d}}",	m_nShapeType);
	if(  PROP_DEF != m_nZOrderRelative )
		sResult.AppendFormat( L"{\\sp{\\sn fBehindDocument}{\\sv %d}}",	m_nZOrderRelative);
	if(  PROP_DEF != m_bHidden )
		sResult.AppendFormat( L"{\\sp{\\sn fHidden}{\\sv %d}}",		m_bHidden);
//Text
	//sResult += L"{\\sp{\\sn fLockText}{\\sv 0}}";

	if(  PROP_DEF != m_nTexpLeft)
		sResult.AppendFormat( L"{\\sp{\\sn dxTextLeft}{\\sv %d}}",	m_nTexpLeft);
	if(  PROP_DEF != m_nTexpTop)
		sResult.AppendFormat( L"{\\sp{\\sn dyTextTop}{\\sv %d}}",	m_nTexpTop);
	if(  PROP_DEF != m_nTexpRight)
		sResult.AppendFormat( L"{\\sp{\\sn dxTextRight}{\\sv %d}}",	m_nTexpRight);
	if(  PROP_DEF != m_nTexpBottom)
		sResult.AppendFormat( L"{\\sp{\\sn dyTextBottom}{\\sv %d}}",	m_nTexpBottom);

	if(  PROP_DEF != m_nAnchorText )
		sResult.AppendFormat( L"{\\sp{\\sn anchorText}{\\sv %d}}",	m_nAnchorText);
	//else
	{
		if(  PROP_DEF != m_nWrapDistLeft )
			sResult.AppendFormat( L"{\\sp{\\sn dxWrapDistLeft}{\\sv %d}}", RtfUtility::Twips2Emu( m_nWrapDistLeft ));
		if(  PROP_DEF != m_nWrapDistTop )
			sResult.AppendFormat( L"{\\sp{\\sn dyWrapDistTop}{\\sv %d}}", RtfUtility::Twips2Emu( m_nWrapDistTop ));
		if(  PROP_DEF != m_nWrapDistRight ) 
			sResult.AppendFormat( L"{\\sp{\\sn dxWrapDistRight}{\\sv %d}}", RtfUtility::Twips2Emu( m_nWrapDistRight ));
		if(  PROP_DEF != m_nWrapDistBottom )
			sResult.AppendFormat( L"{\\sp{\\sn dyWrapDistBottom}{\\sv %d}}",RtfUtility::Twips2Emu(  m_nWrapDistBottom ));
	}
	if(  PROP_DEF != m_bFitShapeToText )
		sResult.AppendFormat( L"{\\sp{\\sn fFitShapeToText}{\\sv %d}}",	m_bFitShapeToText);
	if(  PROP_DEF != m_bFitTextToShape )
		sResult.AppendFormat( L"{\\sp{\\sn fFitTextToShape}{\\sv %d}}",	m_bFitTextToShape);
	if(  PROP_DEF != m_nCcol )
		sResult.AppendFormat( L"{\\sp{\\sn ccol}{\\sv %d}}",				m_nCcol);
	if(  PROP_DEF != m_nTxdir )
		sResult.AppendFormat( L"{\\sp{\\sn txdir}{\\sv %d}}",			m_nTxdir);
	if(  PROP_DEF != m_nWrapText )
		sResult.AppendFormat( L"{\\sp{\\sn WrapText}{\\sv %d}}",			m_nWrapText);
//Geometry
	if( PROP_DEF != m_nAdjustValue[0] )
		sResult.AppendFormat( L"{\\sp{\\sn adjustValue}{\\sv %d}}",	m_nAdjustValue[0] );
	if( PROP_DEF != m_nAdjustValue[1] )
		sResult.AppendFormat( L"{\\sp{\\sn adjust2Value}{\\sv %d}}",	m_nAdjustValue[1] );
	if( PROP_DEF != m_nAdjustValue[2] )
		sResult.AppendFormat( L"{\\sp{\\sn adjust3Value}{\\sv %d}}",	m_nAdjustValue[2] );
	if( PROP_DEF != m_nAdjustValue[3] )
		sResult.AppendFormat( L"{\\sp{\\sn adjust4Value}{\\sv %d}}",	m_nAdjustValue[3] );
	if( PROP_DEF != m_nAdjustValue[4] )
		sResult.AppendFormat( L"{\\sp{\\sn adjust5Value}{\\sv %d}}",	m_nAdjustValue[4] );
	if( PROP_DEF != m_nAdjustValue[5] )
		sResult.AppendFormat( L"{\\sp{\\sn adjust6Value}{\\sv %d}}",	m_nAdjustValue[5] );
	if( PROP_DEF != m_nAdjustValue[6] )
		sResult.AppendFormat( L"{\\sp{\\sn adjust7Value}{\\sv %d}}",	m_nAdjustValue[6] );
	if( PROP_DEF != m_nAdjustValue[7] )
		sResult.AppendFormat( L"{\\sp{\\sn adjust8Value}{\\sv %d}}",	m_nAdjustValue[7] );
	if( PROP_DEF != m_nAdjustValue[8] )
		sResult.AppendFormat( L"{\\sp{\\sn adjust9Value}{\\sv %d}}",	m_nAdjustValue[8] );
	if( PROP_DEF != m_nAdjustValue[9] )
		sResult.AppendFormat( L"{\\sp{\\sn adjust10Value}{\\sv %d}}", m_nAdjustValue[9] );
//custom
	if( PROP_DEF != m_nGeoLeft)	
		sResult.AppendFormat( L"{\\sp{\\sn geoLeft}{\\sv %d}}",		m_nGeoLeft );
	if( PROP_DEF != m_nGeoTop)	
		sResult.AppendFormat( L"{\\sp{\\sn geoTop}{\\sv %d}}",		m_nGeoTop);
	if( PROP_DEF != m_nGeoRight)
		sResult.AppendFormat( L"{\\sp{\\sn geoRight}{\\sv %d}}",		m_nGeoRight );
	if( PROP_DEF != m_nGeoBottom)
		sResult.AppendFormat( L"{\\sp{\\sn geoBottom}{\\sv %d}}",	m_nGeoBottom );
	if( PROP_DEF != m_nShapePath)
		sResult.AppendFormat( L"{\\sp{\\sn shapePath}{\\sv %d}}",	m_nShapePath );
	if( !m_aPVerticles.empty())
	{
		sResult.AppendFormat( L"{\\sp{\\sn pVerticies}{\\sv 8;%d",	m_aPVerticles.size() );
		for( int i = 0; i < m_aPVerticles.size(); i ++ )
			sResult.AppendFormat( L";(%d,%d)", m_aPVerticles[i].first, m_aPVerticles[i].second );
		sResult += L"}}";
	}
	if( !m_aPSegmentInfo.empty())
	{
		sResult += L"{\\sp{\\sn pSegmentInfo}{\\sv ";
		sResult.AppendFormat( L"{\\sp{\\sn pSegmentInfo}{\\sv 2;%d", m_aPSegmentInfo.size() );
		for( int i = 0; i < m_aPSegmentInfo.size(); i ++ )
			sResult.AppendFormat( L";%d", m_aPSegmentInfo[i] );
		sResult += L"}}";
	}
//Connectors
	if( PROP_DEF != m_nConnectionType )
		sResult.AppendFormat( L"{\\sp{\\sn cxk}{\\sv %d}}",				m_nConnectionType );
	if( PROP_DEF != m_nConnectorStyle )
		sResult.AppendFormat( L"{\\sp{\\sn cxstyle}{\\sv %d}}",			m_nConnectorStyle );
//Picture Effects
	if( PROP_DEF != m_nCropFromTop )
		sResult.AppendFormat( L"{\\sp{\\sn cropFromTop}{\\sv %d}}",		m_nCropFromTop );
	if( PROP_DEF != m_nCropFromBottom )
		sResult.AppendFormat( L"{\\sp{\\sn cropFromBottom}{\\sv %d}}",	m_nCropFromBottom );
	if( PROP_DEF != m_nCropFromLeft )
		sResult.AppendFormat( L"{\\sp{\\sn cropFromLeft}{\\sv %d}}",		m_nCropFromLeft );
	if( PROP_DEF != m_nCropFromRight )
		sResult.AppendFormat( L"{\\sp{\\sn cropFromRight}{\\sv %d}}",	m_nCropFromRight );
//Grouped Shapes
	if( PROP_DEF != m_nGroupBottom )
		sResult.AppendFormat( L"{\\sp{\\sn groupBottom}{\\sv %d}}",		m_nGroupBottom );
	if( PROP_DEF != m_nGroupLeft )
		sResult.AppendFormat( L"{\\sp{\\sn groupLeft}{\\sv %d}}",		m_nGroupLeft );
	if( PROP_DEF != m_nGroupRight )
		sResult.AppendFormat( L"{\\sp{\\sn groupRight}{\\sv %d}}",		m_nGroupRight );
	if( PROP_DEF != m_nGroupTop )
		sResult.AppendFormat( L"{\\sp{\\sn groupTop}{\\sv %d}}",			m_nGroupTop );
	if( PROP_DEF != m_nRelBottom )
		sResult.AppendFormat( L"{\\sp{\\sn relBottom}{\\sv %d}}",		m_nRelBottom );
	if( PROP_DEF != m_nRelLeft )
		sResult.AppendFormat( L"{\\sp{\\sn relLeft}{\\sv %d}}",			m_nRelLeft );
	if( PROP_DEF != m_nRelRight )
		sResult.AppendFormat( L"{\\sp{\\sn relRight}{\\sv %d}}",			m_nRelRight );
	if( PROP_DEF != m_nRelTop )
		sResult.AppendFormat( L"{\\sp{\\sn relTop}{\\sv %d}}",			m_nRelTop );
	if( PROP_DEF != m_nRelRotation)
		sResult.AppendFormat( L"{\\sp{\\sn relRotation}{\\sv %d}}",		m_nRelRotation );
	if( PROP_DEF != m_nRelZOrder )
		sResult.AppendFormat( L"{\\sp{\\sn dhgt}{\\sv %d}}",				m_nRelZOrder );
//Fill
	if( 0 == m_bFilled )
		sResult += L"{\\sp{\\sn fFilled}{\\sv 0}}";
	if( PROP_DEF != m_nFillType )
		sResult.AppendFormat( L"{\\sp{\\sn fillType}{\\sv %d}}",			m_nFillType );
	if( PROP_DEF != m_nFillColor )
		sResult.AppendFormat( L"{\\sp{\\sn fillColor}{\\sv %d}}",		m_nFillColor );
	if( PROP_DEF != m_nFillColor2 )
		sResult.AppendFormat( L"{\\sp{\\sn fillBackColor}{\\sv %d}}",	m_nFillColor2 );
	if( PROP_DEF != m_nFillOpacity )
		sResult.AppendFormat( L"{\\sp{\\sn fillOpacity}{\\sv %d}}", ( m_nFillOpacity * 65536 /100 ) );
	if( PROP_DEF != m_nFillFocus )
		sResult.AppendFormat( L"{\\sp{\\sn fillFocus}{\\sv %d}}",		 m_nFillFocus );
	if( PROP_DEF != m_nFillAngle )
		sResult.AppendFormat( L"{\\sp{\\sn fillAngle}{\\sv %d}}",		m_nFillAngle * 65536 );
//Line
	if( 0 == m_bLine )
		sResult += L"{\\sp{\\sn fLine}{\\sv 0}}";
	if( PROP_DEF != m_nLineColor )
		sResult.AppendFormat( L"{\\sp{\\sn lineColor}{\\sv %d}}",			m_nLineColor );
	if( PROP_DEF != m_nLineStartArrow )
		sResult.AppendFormat( L"{\\sp{\\sn lineStartArrowhead}{\\sv %d}}",	m_nLineStartArrow );
	if( PROP_DEF != m_nLineEndArrow )
		sResult.AppendFormat( L"{\\sp{\\sn lineEndArrowhead}{\\sv %d}}",		m_nLineEndArrow );
	if( PROP_DEF != m_nLineStartArrowWidth )
		sResult.AppendFormat( L"{\\sp{\\sn lineStartArrowWidth}{\\sv %d}}",	m_nLineStartArrowWidth );
	if( PROP_DEF != m_nLineStartArrowLength )
		sResult.AppendFormat( L"{\\sp{\\sn lineStartArrowLength}{\\sv %d}}",	m_nLineStartArrowLength );
	if( PROP_DEF != m_nLineEndArrowWidth )
		sResult.AppendFormat( L"{\\sp{\\sn lineEndArrowWidth}{\\sv %d}}",	m_nLineEndArrowWidth );
	if( PROP_DEF != m_nLineEndArrowLength )
		sResult.AppendFormat( L"{\\sp{\\sn lineEndArrowLength}{\\sv %d}}",	m_nLineEndArrowLength );
	if( PROP_DEF != m_nLineWidth )
		sResult.AppendFormat( L"{\\sp{\\sn lineWidth}{\\sv %d}}",			m_nLineWidth );
	if( PROP_DEF != m_nLineDashing )
		sResult.AppendFormat( L"{\\sp{\\sn lineDashing}{\\sv %d}}",			m_nLineDashing );

//pWrapPolygonVertices	Points of the text wrap polygon.
	if( !m_aWrapPoints.empty())
	{
		sResult.AppendFormat( L"{\\sp{\\sn pWrapPolygonVertices}{\\sv 8;%d", m_aWrapPoints.size() );
		for( int i = 0; i < m_aWrapPoints.size(); i ++ )
			sResult.AppendFormat( L";(%d,%d)", m_aWrapPoints[i].first, m_aWrapPoints[i].second );
		sResult += L"}}";
	}
//WordArt
	if( PROP_DEF != m_bGtext )
	{
		sResult.AppendFormat( L"{\\sp{\\sn fGtext}{\\sv %d}}",	m_bGtext );
		
		if( !m_sGtextUNICODE.IsEmpty() )
		{
			sResult += L"{\\sp{\\sn gtextUNICODE}{\\sv ";
			sResult += m_sGtextUNICODE + L"}}";
		}
		if( !m_sGtextFont.IsEmpty() )
		{
			sResult += L"{\\sp{\\sn gtextFont}{\\sv ";
			sResult += m_sGtextFont + L"}}";
		}
		if( PROP_DEF != m_nGtextSize )			sResult.AppendFormat( L"{\\sp{\\sn gtextSize}{\\sv %d}}",		m_nGtextSize );
		if( PROP_DEF != m_bGtextFVertical )		sResult.AppendFormat( L"{\\sp{\\sn gtextFVertical}{\\sv %d}}",	m_bGtextFVertical);
		if( PROP_DEF != m_bGtextFKern )			sResult.AppendFormat( L"{\\sp{\\sn gtextFKern}{\\sv %d}}",		m_bGtextFKern);
		if( PROP_DEF != m_bGtextFStretch )		sResult.AppendFormat( L"{\\sp{\\sn gtextFStretch}{\\sv %d}}",	m_bGtextFStretch);
		if( PROP_DEF != m_bGtextFShrinkFit )	sResult.AppendFormat( L"{\\sp{\\sn gtextFShrinkFit}{\\sv %d}}",	m_bGtextFShrinkFit);
		if( PROP_DEF != m_bGtextFBestFit )		sResult.AppendFormat( L"{\\sp{\\sn gtextFBestFit}{\\sv %d}}",	m_bGtextFBestFit);
	}

	return sResult;
}
CString RtfShape::RenderToOOX(RenderParameter oRenderParameter)
{
	if( PROP_DEF == m_nShapeType ) 
		return L"";

	CString sResult;
	RtfDocument* poDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	
	TextItemContainerPtr aTempTextItems;
	
	if( NSOfficeDrawing::sptPictureFrame == m_nShapeType && 0 != m_aTextItems )
	{//Значит это Ole обьект с anchor, для него все также только TextBox надо делать по-другому
		aTempTextItems	= m_aTextItems;
		m_aTextItems	= TextItemContainerPtr();
		m_bIsOle		= true;
	}

	sResult = RenderToOOXBegin(oRenderParameter);
	
	if( !sResult.IsEmpty() )
		sResult +=  RenderToOOXEnd(oRenderParameter);

	CString sOle;
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
	
	if( !sOle.IsEmpty() && !sResult.IsEmpty())
	{
		sResult.Replace( L"</w:pict>", sOle + L"</w:pict>" );//todooo переписать
	}
	
	return sResult;
}
CString RtfShape::GetShapeNodeName(int type)
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
CString RtfShape::RenderToOOXBegin(RenderParameter oRenderParameter)
{
	if( !IsValid() ) return L"";

	CString			sResult;
	
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
		if (m_oCharProperty.m_nDeleted != PROP_DEF)
		{
			m_bDelete = true;
			
			CString sAuthor = m_oCharProperty.m_nRevauthDel != PROP_DEF ? poRtfDocument->m_oRevisionTable[ m_oCharProperty.m_nRevauthDel ] : L"";
			CString sDate(RtfUtility::convertDateTime(m_oCharProperty.m_nRevdttmDel).c_str());
			
			sResult += L"<w:del w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_oCharProperty.m_nDeleted = PROP_DEF;
		}
		else if (m_oCharProperty.m_nRevised != PROP_DEF)
		{
			m_bInsert = true;
			
			CString sAuthor = m_oCharProperty.m_nRevauth != PROP_DEF ? poRtfDocument->m_oRevisionTable[ m_oCharProperty.m_nRevauth] : L"";
			CString sDate(RtfUtility::convertDateTime(m_oCharProperty.m_nRevdttm).c_str());
			
			sResult += L"<w:ins w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_oCharProperty.m_nRevised = PROP_DEF;
		}
		CString sCharProp = m_oCharProperty.RenderToOOX(oRenderParameter);
		sResult += L"<w:r>";
		if (!sCharProp .IsEmpty())
		{
			sResult += _T("<w:rPr>");
				sResult += sCharProp;
			sResult += _T("</w:rPr>");
		}
		sResult += L"<w:pict>";
	}
	
	if (oRenderParameter.sValue.IsEmpty()) 
		oRenderParameter.sValue = GetShapeNodeName(m_nShapeType);

	sResult += L"<" + oRenderParameter.sValue;

	if (m_sName.IsEmpty())
	{
		RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
		m_sName.AppendFormat(L"_x0000_s%d", poDocument->GetShapeId( m_nID ));
	}
	sResult += L" id=\"" + m_sName + L"\"";

	if( PROP_DEF != m_nShapeType && 0 != m_nShapeType)
	{
		sResult.AppendFormat( L" type=\"#_x0000_t%d\"",	m_nShapeType );
		sResult.AppendFormat( L" o:spt=\"%d\""		 ,	m_nShapeType );
	}

	if( 0 == m_bFilled) sResult += L" filled=\"f\"";
	else				sResult += L" filled=\"t\"";

	if( 0 == m_bLine)	sResult += L" stroked=\"f\"";
	else				sResult += L" stroked=\"t\"";

	if( PROP_DEF != m_nFillColor)
	{
		RtfColor color(m_nFillColor);
		sResult.AppendFormat( L" fillcolor=\"#" + color.ToHexColor(true) + L"\"");
	}
	if( PROP_DEF != m_nLineColor)
	{
		RtfColor color(m_nLineColor);
		sResult.AppendFormat( L" strokecolor=\"#" + color.ToHexColor(true) + L"\"");
	}
	if(PROP_DEF != m_nLineWidth)
		sResult.AppendFormat( L" strokeweight=\"%.2fpt\"", RtfUtility::Emu2Pt(m_nLineWidth) );
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
	CString sStyle ;
	if( PROP_DEF != m_nLeft &&  PROP_DEF != m_nRight && PROP_DEF != m_nTop && PROP_DEF != m_nBottom   )
	{
		//не пишем если inline
		if( 3 != m_nPositionHRelative || 3 != m_nPositionVRelative )
		{
			sStyle .Append		( L"position:absolute;" );
			if (oRenderParameter.nType !=  RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2)
			{
				sStyle.AppendFormat	( L"margin-left:%.2fpt;"	, RtfUtility::Twip2pt(m_nLeft) );
				sStyle.AppendFormat	( L"margin-top:%.2fpt;"	, RtfUtility::Twip2pt(m_nTop) );
				sStyle.AppendFormat	( L"margin-bottom:%.2fpt;"	, RtfUtility::Twip2pt(m_nBottom) );
				sStyle.AppendFormat	( L"margin-right:%.2fpt;"	, RtfUtility::Twip2pt(m_nRight) );
			}
		}
		int nWidth = m_nRight - m_nLeft;
		int nHeight = m_nBottom - m_nTop;
		
		if (oRenderParameter.nType ==  RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2)
			sStyle.AppendFormat(L"width:%.2f;height:%.2f;", RtfUtility::Twip2pt(nWidth), RtfUtility::Twip2pt(nHeight));
		else
			sStyle.AppendFormat(L"width:%.2fpt;height:%.2fpt;", RtfUtility::Twip2pt(nWidth), RtfUtility::Twip2pt(nHeight));
	}
	else if( PROP_DEF != m_nRelLeft &&  PROP_DEF != m_nRelRight && PROP_DEF != m_nRelTop && PROP_DEF != m_nRelBottom  )
	{
		int nWidth	= m_nRelRight - m_nRelLeft;
		int nHeight = m_nRelBottom - m_nRelTop;
		
		sStyle.Append		( L"position:absolute;" );
		sStyle.AppendFormat	( L"left:%d;"			, m_nRelLeft );
		sStyle.AppendFormat	( L"top:%d;"				, m_nRelTop);
		//sStyle.AppendFormat	( L"bottom:%d;"			, m_nRelBottom );
		//sStyle.AppendFormat	( L"right:%d;"			, m_nRelRight);
		sStyle.AppendFormat	( L"width:%d;height:%d;"	, nWidth, nHeight);
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
			sStyle.AppendFormat(L"width:%.2f;height:%.2f;", RtfUtility::Twip2pt(nWidth), RtfUtility::Twip2pt(nHeight));
		else
			sStyle.AppendFormat(L"width:%.2fpt;height:%.2fpt;", RtfUtility::Twip2pt(nWidth), RtfUtility::Twip2pt(nHeight));
	}

	switch( m_nPositionH )
	{
		case 0: sStyle += L"mso-position-horizontal:absolute;";	break;
		case 1: sStyle += L"mso-position-horizontal:left;";		break;
		case 2: sStyle += L"mso-position-horizontal:center;";	break;
		case 3: sStyle += L"mso-position-horizontal:right;";		break;
		case 4: sStyle += L"mso-position-horizontal:inside;";	break;
		case 5: sStyle += L"mso-position-horizontal:outside;";	break;
	}
	if( PROP_DEF != m_nPositionHPct )//todo
	{
		sStyle.AppendFormat(L"mso-left-percent:%d;", m_nPositionHPct);
	}
	if( PROP_DEF != m_nPositionH && PROP_DEF == m_nPositionHRelative )
		m_nPositionHRelative = 2;
	if( PROP_DEF != m_nPositionHRelative )
	{
		switch( m_nPositionHRelative )
		{
			case 0: sStyle += L"mso-position-horizontal-relative:margin;";				break;
			case 1: sStyle += L"mso-position-horizontal-relative:page;";					break;
			case 2: sStyle += L"mso-position-horizontal-relative:text;";					break;
			case 3: sStyle += L"mso-position-horizontal-relative:char;";					break;
			case 4: sStyle += L"mso-position-horizontal-relative:left-margin-area;";		break;
			case 5: sStyle += L"mso-position-horizontal-relative:right-margin-area;";	break;
			case 6: sStyle += L"mso-position-horizontal-relative:inner-margin-area;";	break;
			case 7: sStyle += L"mso-position-horizontal-relative:outer-margin-area;";	break;
		}
	}
	else
	{
		switch( m_eXAnchor )
		{
			case ax_page:	sStyle += L"mso-position-horizontal-relative:page;";		break;
			case ax_margin: sStyle += L"mso-position-horizontal-relative:margin;";	break;
			//case ax_column: sStyle += L"mso-position-horizontal-relative:text;");break;
		}
	}

	switch( m_nPositionV )
	{
		case 0: sStyle += L"mso-position-vertical:absolute;";	break;
		case 1: sStyle += L"mso-position-vertical:top;";			break;
		case 2: sStyle += L"mso-position-vertical:center;";		break;
		case 3: sStyle += L"mso-position-vertical:bottom;";		break;
		case 4: sStyle += L"mso-position-vertical:inside;";		break;
		case 5: sStyle += L"mso-position-vertical:outside;";		break;
	}
	if( PROP_DEF != m_nPositionVPct )
		sStyle.AppendFormat(L"mso-top-percent:%d;",	m_nPositionVPct);

	if( PROP_DEF != m_nPositionV && PROP_DEF == m_nPositionVRelative )
		m_nPositionVRelative =2;
	if( PROP_DEF != m_nPositionVRelative )
	{
		switch( m_nPositionVRelative )
		{
			case 0: sStyle += L"mso-position-vertical-relative:margin;";				break;
			case 1: sStyle += L"mso-position-vertical-relative:page;";				break;
			case 2: sStyle += L"mso-position-vertical-relative:text;";				break;
			case 3: sStyle += L"mso-position-vertical-relative:line;";				break;
			case 4: sStyle += L"mso-position-vertical-relative:top-margin-area;";	break;
			case 5: sStyle += L"mso-position-vertical-relative:bottom-margin-area;";	break;
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
			//case ay_Para: sStyle += L"mso-position-vertical-relative:text;";		break;
		}
	}
	if( PROP_DEF != m_nPctWidth )
		sStyle.AppendFormat(L"mso-width-percent:%d;",	m_nPctWidth);
	switch( m_nPctWidthRelative )
	{
		case 0:	sStyle += L"mso-width-relative:margin;";				break;
		case 1:	sStyle += L"mso-width-relative:page;";				break;
		case 2:	sStyle += L"mso-width-relative:left-margin-area;";	break;
		case 3:	sStyle += L"mso-width-relative:right-margin-area;";	break;
		case 4:	sStyle += L"mso-width-relative:inner-margin-area;";	break;
		case 5:	sStyle += L"mso-width-relative:outer-margin-area;";	break;
	}
	
	if( PROP_DEF != m_nPctHeight )
		sStyle.AppendFormat(L"mso-height-percent:%d;",	m_nPctHeight);
	
	switch( m_nPctHeightRelative )
	{
		case 0:	sStyle += L"mso-height-relative:margin;";				break;
		case 1:	sStyle += L"mso-height-relative:page;";					break;
		case 2:	sStyle += L"mso-height-relative:top-margin-area;";		break;
		case 3:	sStyle += L"mso-height-relative:bottom-margin-area;";	break;
		case 4:	sStyle += L"mso-height-relative:inner-margin-area;";		break;
		case 5:	sStyle += L"mso-height-relative:outer-margin-area;";		break;
	}

	if( PROP_DEF != m_nRotation )
		sStyle.AppendFormat(L"rotation:%d;",	m_nRotation / 65536 );
	else if( PROP_DEF != m_nRelRotation )
		sStyle.AppendFormat(L"rotation:%d;",	m_nRelRotation / 65536 );

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
		sStyle.AppendFormat( L"z-index:%d;", nZIndex );

	if(  PROP_DEF != m_nWrapDistLeft )
		sStyle.AppendFormat( L"mso-wrap-distance-left:%.2fpt;", RtfUtility::Twip2pt( m_nWrapDistLeft ));
	if(  PROP_DEF != m_nWrapDistTop )
		sStyle.AppendFormat( L"mso-wrap-distance-top:%.2fpt;", RtfUtility::Twip2pt( m_nWrapDistTop ));
	if(  PROP_DEF != m_nWrapDistRight ) 
		sStyle.AppendFormat( L"mso-wrap-distance-right:%.2fpt;", RtfUtility::Twip2pt( m_nWrapDistRight ));
	if(  PROP_DEF != m_nWrapDistBottom )
		sStyle.AppendFormat( L"mso-wrap-distance-bottom:%.2fpt;", RtfUtility::Twip2pt( m_nWrapDistBottom ));

	switch( m_nAnchorText)
	{
		case 0:	sStyle += L"v-text-anchor:top;";						break;
		case 1:	sStyle += L"v-text-anchor:middle;";					break;
		case 2:	sStyle += L"v-text-anchor:bottom;";					break;
		case 3:	sStyle += L"v-text-anchor:topcenter;";				break;
		case 4:	sStyle += L"v-text-anchor:middle-center;";			break;
		case 5:	sStyle += L"v-text-anchor:bottom-center;";			break;
		case 6:	sStyle += L"v-text-anchor:top-baseline;";			break;
		case 7:	sStyle += L"v-text-anchor:bottom-baseline;";			break;
		case 8:	sStyle += L"v-text-anchor:top-center-baseline;";		break;
		case 9:	sStyle += L"v-text-anchor:bottom-center-baseline;";	break;
	}

//---------------------------------------------------------------------------------------------------------------------------
	if( false == sStyle.IsEmpty() )
	{
		sStyle.Delete( sStyle.GetLength() - 1 );
        sResult += L" style=\"" + sStyle + L"\"";
	}
//----------------------------------------------------------------------------------------------------------------------------
	if( true == m_bIsOle )		sResult += L" o:ole=\"\"";
	
	if( PROP_DEF != m_nGroupLeft && PROP_DEF != m_nGroupTop )
		sResult.AppendFormat( L" coordorigin=\"%d,%d\"", m_nGroupLeft, m_nGroupTop);
	
	if( PROP_DEF != m_nGroupLeft && PROP_DEF != m_nGroupTop && PROP_DEF != m_nGroupRight && PROP_DEF != m_nGroupBottom)
		sResult.AppendFormat( L" coordsize=\"%d,%d\"", m_nGroupRight - m_nGroupLeft, m_nGroupBottom - m_nGroupTop );
	else if ( PROP_DEF != m_nGeoLeft && PROP_DEF != m_nGeoTop && PROP_DEF != m_nGeoRight && PROP_DEF != m_nGeoBottom)
		sResult.AppendFormat( L" coordsize=\"%d,%d\"", m_nGeoRight - m_nGeoLeft, m_nGeoBottom - m_nGeoTop );

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
		CString sAdjust;
		sAdjust.AppendFormat( L"%d",	m_nAdjustValue[0]);
		for (int i = 1 ; i < 10; i++)
		{
			if (PROP_DEF != m_nAdjustValue[i])
				sAdjust.AppendFormat( L",%d", m_nAdjustValue[i]);
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
		sResult.AppendFormat( L"%d,%d", m_aWrapPoints[0].first, m_aWrapPoints[0].second);
		
		for( int i = 0; i < (int)m_aWrapPoints.size(); i++ )
		{
			sResult.AppendFormat( L",%d,%d", m_aWrapPoints[i].first, m_aWrapPoints[i].second);
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
			case 1:sResult += L" type=\"topAndBottom\"";	break;
			case 2:sResult += L" type=\"square\"";		break;
			case 3:sResult += L" type=\"none\"";			break;
			case 4:sResult += L" type=\"tight\"";		break;
			case 5:sResult += L" type=\"through\"";		break;
		}
		switch( m_nWrapSideType )
		{
			case 0:sResult += L" side=\"both\"";			break;
			case 1:sResult += L" side=\"left\"";			break;
			case 2:sResult += L" side=\"right\"";		break;
			case 3:sResult += L" side=\"largest\"";		break;
		}
		sResult += L"/>";
	}
	
//Line
	if( 0 != m_bLine)
	{
		CString sStroke;
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
			case 1: sStroke += L" startarrow=\"block\"";		break;
			case 2: sStroke += L" startarrow=\"classic\"";	break;
			case 3: sStroke += L" startarrow=\"diamond\"";	break;
			case 4: sStroke += L" startarrow=\"oval\"";		break;
			case 5: sStroke += L" startarrow=\"open\"";		break;
			case 6: sStroke += L" startarrow=\"block\"";		break;
			case 7: sStroke += L" startarrow=\"block\"";		break;
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
			case 2: sStroke += L" startarrowwidth=\"wide\"";		break;
		}
		switch( m_nLineEndArrow )
		{
			case 0: sStroke += L" endarrow=\"none\"";	break;
			case 1: sStroke += L" endarrow=\"block\"";	break;
			case 2: sStroke += L" endarrow=\"classic\"";	break;
			case 3: sStroke += L" endarrow=\"diamond\"";	break;
			case 4: sStroke += L" endarrow=\"oval\"";	break;
			case 5: sStroke += L" endarrow=\"open\"";	break;
			case 6: sStroke += L" endarrow=\"block\"";	break;
			case 7: sStroke += L" endarrow=\"block\"";	break;
		}
		switch( m_nLineEndArrowLength )
		{
			case 0: sStroke += L" endarrowlength=\"short\"";		break;
			case 1: sStroke += L" endarrowlength=\"medium\"";	break;
			case 2: sStroke += L" endarrowlength=\"long\"";		break;
		}
		switch( m_nLineEndArrowWidth )
		{
			case 0: sStroke += L" endarrowwidth=\"narrow\"";	break;
			case 1: sStroke += L" endarrowwidth=\"medium\"";	break;
			case 2: sStroke += L" endarrowwidth=\"wide\"";	break;
		}

		if( false == sStroke.IsEmpty())
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
			sResult.AppendFormat( L" inset=\"%dpt, %dpt, %dpt, %dpt\">", 
			(int)RtfUtility::Emu2Pt(m_nTexpLeft), (int)RtfUtility::Emu2Pt(m_nTexpTop), (int)RtfUtility::Emu2Pt(m_nTexpRight), (int)RtfUtility::Emu2Pt(m_nTexpBottom) );
		}
		else  
			sResult += L">";
		
		sResult += L"<w:txbxContent>";
		sResult +=  m_aTextItems->RenderToOOX(oNewParam);
		sResult += L"</w:txbxContent>";
		sResult += L"</v:textbox>";
	}
	
	CString sPicture;	
	if( m_oPicture )
	{
		sPicture = m_oPicture->RenderToOOX(oRenderParameter);
		
		if (m_nShapeType == PROP_DEF || m_nShapeType == 75)
		{
			if( sPicture.IsEmpty() )//если не сохранилась картинка, то весь shape-picture будет бесполезным
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
				sResult.AppendFormat( L" cropleft=\"%df\"", nCropLeft );
			if( PROP_DEF != nCropTop )
				sResult.AppendFormat( L" croptop=\"%df\"", nCropTop );
			if( PROP_DEF != nCropRight )
				sResult.AppendFormat( L" cropright=\"%df\"", nCropRight );
			if( PROP_DEF != nCropBottom )
				sResult.AppendFormat( L" cropbottom=\"%df\"", nCropBottom );

			sResult += L" o:title=\"\"/>";
		}
	}
//-----------------------------------------------------------------------------------------------
	if( 0 != m_bFilled)
	{
		sResult += L"<v:fill";

		if (!sPicture.IsEmpty() && m_nShapeType != 75)
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
			CString sOpacity; sOpacity.Format(L"%d", /*100 - */m_nFillOpacity);
			sResult += L" opacity=\"" + sOpacity +L"%\"";
			//sResult.AppendFormat( L" opacity=\"%df\"", m_nFillOpacity );
		}
		if ( PROP_DEF != m_nFillColor2)
		{
			RtfColor color(m_nFillColor2);
			sResult += L" color2=\"#" + color.ToHexColor(true) + L"\"";
		}
		if ( PROP_DEF != m_nFillFocus)
		{
			CString sFocus; sFocus.Format(L"%d", m_nFillFocus );
			sResult += L" focus=\""+ sFocus +L"%\"";
		}
		
		if ( PROP_DEF != m_nFillAngle)
			sResult.AppendFormat( L" angle=\"%d\"", m_nFillAngle );

		sResult += L"/>";
	}
//---------------------------------------------------------------------------------------------------------------------------
	if( false == m_sGtextUNICODE.IsEmpty())
	{
		sResult += L"<v:textpath"; 

		CString sTextStyle;

        if ( !m_sGtextFont.IsEmpty() )
		{
			sTextStyle += L"font-family:" + XmlUtils::EncodeXmlString(m_sGtextFont) + L";";//todooo значения как в кавычках так и без - проверить как без
		}

		if (!sTextStyle.IsEmpty())
			sResult += L" style=\"" + sTextStyle + L"\"";

		if ( PROP_DEF != m_nGtextSize )
			sTextStyle.AppendFormat( L"font-size:%dpt;", m_nGtextSize );

		sResult += L" string=\"" + XmlUtils::EncodeXmlString(m_sGtextUNICODE) + L"\"";
		sResult += L"/>";
	}
	return sResult;
}
CString RtfShape::RenderToOOXEnd(RenderParameter oRenderParameter)
{
	CString sResult;

	if (oRenderParameter.sValue.IsEmpty())
		oRenderParameter.sValue = GetShapeNodeName(m_nShapeType);

	sResult += L"</" + oRenderParameter.sValue + L">";

	if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2 == oRenderParameter.nType )
		;
	else if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE == oRenderParameter.nType )
		sResult += L"</w:pict>";
	else
	{
		sResult += L"</w:pict></w:r>";//работает по умолчанию
		
		if (m_bInsert)	sResult += L"</w:ins>";
		if (m_bDelete)	sResult += L"</w:del>";
	}
	return sResult;
}
CString RtfShapeGroup::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
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

	if( st_inline == m_eAnchorTypeShape )
	{
		sResult += L"{\\shpgrp";
		sResult += L"{\\*\\shpinst";
		
		RENDER_RTF_INT( m_nID			, sResult, L"shplid" );
		
		if (!m_bInGroup)
		{	
			RENDER_RTF_INT( m_nLeft			, sResult, L"shpleft" );
			RENDER_RTF_INT( m_nTop			, sResult, L"shptop" );
			RENDER_RTF_INT( m_nBottom		, sResult, L"shpbottom" );
			RENDER_RTF_INT( m_nRight		, sResult, L"shpright" );
			RENDER_RTF_INT( m_nHeader		, sResult, L"shpfhdr" );
			RENDER_RTF_INT( m_nWrapType		, sResult, L"shpwr" );
			RENDER_RTF_INT( m_nWrapSideType	, sResult, L"shpwrk" );
			RENDER_RTF_BOOL( m_bLockAnchor	, sResult, L"shplockanchor" );
			
			sResult += L"\\shpbxignore";
			sResult += L"\\shpbyignore";
			
			sResult.AppendFormat( L"{\\sp{\\sn fUseShapeAnchor}{\\sv %d}}", false);
			sResult.AppendFormat( L"{\\sp{\\sn fPseudoInline}{\\sv %d}}", true);
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
		
		RENDER_RTF_INT( m_nID			, sResult, L"shplid" );

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
CString RtfShapeGroup::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
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
