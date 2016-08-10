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

void RtfShape::SetDefault()
{
	m_eShapeType = st_none;

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

	//m_bBehindDocument = -1;
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
	DEFAULT_PROPERTY( m_nAdjustValue )
	DEFAULT_PROPERTY( m_nAdjustValue2 )
	DEFAULT_PROPERTY( m_nAdjustValue3 )
	DEFAULT_PROPERTY( m_nAdjustValue4 )
	DEFAULT_PROPERTY( m_nAdjustValue5 )
	DEFAULT_PROPERTY( m_nAdjustValue6 )
	DEFAULT_PROPERTY( m_nAdjustValue7 )
	DEFAULT_PROPERTY( m_nAdjustValue8 )
	DEFAULT_PROPERTY( m_nAdjustValue9 )
	DEFAULT_PROPERTY( m_nAdjustValue10 )

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
	m_sMetroBlob = _T("");
	m_sMetroBlobRels = _T("");
	m_sMetroBlobData = _T("");
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

	m_aTextItems = TextItemContainerPtr();
	m_oPicture = RtfPicturePtr();
	m_bIsOle = false;
}

CString RtfShape::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	if( PROP_DEF != m_nShapeType && 0 != m_nShapeType )
	{
		//запоминаем координаты и если нужно поворачиваем
		int nLeft = m_nLeft;
		int nTop = m_nTop;
		int nRight = m_nRight;
		int nBottom = m_nBottom;
		int nRotate = m_nRotation;

		int nRelLeft = m_nRelLeft;
		int nRelTop = m_nRelTop;
		int nRelRight = m_nRelRight;
		int nRelBottom = m_nRelBottom;
		int nRelRotate = m_nRelRotation;

		if( PROP_DEF != m_nRotation && PROP_DEF != m_nLeft && PROP_DEF != m_nTop &&
			PROP_DEF != m_nRight && PROP_DEF != m_nBottom )
			ToRtfRotation( m_nRotation, m_nLeft, m_nTop, m_nRight, m_nBottom );

		if( PROP_DEF != m_nRelRotation && PROP_DEF != m_nRelLeft && PROP_DEF != m_nRelTop &&
			PROP_DEF != m_nRelRight && PROP_DEF != m_nRelBottom )
			ToRtfRotation( m_nRelRotation, m_nRelLeft, m_nRelTop, m_nRelRight, m_nRelBottom );

		if( st_inline == m_eShapeType )
		{
			if( NULL != m_oPicture && m_nShapeType == 75)
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
					sResult += _T("{\\*\\shppict");
					sResult +=  m_oPicture->RenderToRtf( oRenderParameter );
					sResult += _T("}");
					sResult += _T("{\\nonshppict");
					sResult +=  m_oPicture->GenerateWMF( oRenderParameter );

					sResult += _T("}");
				}
			}
			else
			{
				sResult += _T("{\\shp");
				sResult += _T("{\\*\\shpinst");
				RENDER_RTF_INT( m_nLeft			, sResult, _T("shpleft") );
				RENDER_RTF_INT( m_nTop			, sResult, _T("shptop") );
				RENDER_RTF_INT( m_nBottom		, sResult, _T("shpbottom") );
				RENDER_RTF_INT( m_nRight		, sResult, _T("shpright") );
				RENDER_RTF_INT( m_nID			, sResult, _T("shplid") );
				RENDER_RTF_INT( m_nHeader		, sResult, _T("shpfhdr") );
				RENDER_RTF_INT( m_nWrapType		, sResult, _T("shpwr") );
				RENDER_RTF_INT( m_nWrapSideType	, sResult, _T("shpwrk") );
				RENDER_RTF_BOOL( m_bLockAnchor	, sResult, _T("shplockanchor") );
				
				sResult += _T("\\shpbxignore");
				sResult += _T("\\shpbyignore");

				sResult.AppendFormat( _T("{\\sp{\\sn fUseShapeAnchor}{\\sv %d}}"), false);
				sResult.AppendFormat( _T("{\\sp{\\sn fPseudoInline}{\\sv %d}}"), true);
				
				sResult +=  RenderToRtfShapeProperty( oRenderParameter );
				
				//picture
				if( 0 != m_oPicture && m_nFillType == 1 || m_nFillType == 2 || m_nFillType == 9)
				{
					sResult += _T("{\\sp{\\sn fillBlip}{\\sv ");
					sResult +=  m_oPicture->RenderToRtf( oRenderParameter );
					sResult += _T("}}");
				}
				//textbox
				if( 0 != m_aTextItems )
				{
					sResult += _T("{\\shptxt ");
					sResult +=  m_aTextItems->RenderToRtf( oRenderParameter );
					sResult += _T("}");
				}
				sResult += _T("}");
				//if( 0 != m_oPicture )
				//{
				//	sResult += _T("{\\shprslt\\par\\plain"));
				//	sResult +=  m_oPicture->GenerateWMF( oRenderParameter ) );
				//	sResult += _T("\\par}"));
				//}
				sResult += _T("}");
			}
		}
		else
		{
			sResult += _T("{\\shp");
			sResult += _T("{\\*\\shpinst");

			RENDER_RTF_INT( m_nLeft			, sResult, _T("shpleft") );
			RENDER_RTF_INT( m_nTop			, sResult, _T("shptop") );
			RENDER_RTF_INT( m_nBottom		, sResult, _T("shpbottom") );
			RENDER_RTF_INT( m_nRight		, sResult, _T("shpright") );
			RENDER_RTF_INT( m_nID			, sResult, _T("shplid") );
			RENDER_RTF_INT( m_nZOrder		, sResult, _T("shpz") );
			RENDER_RTF_INT( m_nHeader		, sResult, _T("shpfhdr") );
			RENDER_RTF_INT( m_nWrapType		, sResult, _T("shpwr") );
			RENDER_RTF_INT( m_nWrapSideType	, sResult, _T("shpwrk") );
			RENDER_RTF_BOOL( m_bLockAnchor	, sResult, _T("shplockanchor") );

			switch(m_eXAnchor)
			{
				case ax_page:	sResult += _T("\\shpbxpage");	break;
				case ax_margin: sResult += _T("\\shpbxmargin");	break;
				case ax_column: sResult += _T("\\shpbxcolumn");	break;
			}
			sResult += _T("\\shpbxignore");
			switch(m_eYAnchor)
			{
				case ay_page:	sResult += _T("\\shpbypage");	break;
				case ay_margin: sResult += _T("\\shpbymargin");	break;
				case ay_Para:	sResult += _T("\\shpbypara");	break;
			}
			sResult += _T("\\shpbyignore");
			RENDER_RTF_INT( m_nZOrderRelative, sResult, _T("shpfblwtxt"));

			sResult += RenderToRtfShapeProperty( oRenderParameter );

			//picture
			if( 0 != m_oPicture)
			{
				if (m_nShapeType == 75)
				{
					sResult += _T("{\\sp{\\sn pib}{\\sv ");
					sResult +=  m_oPicture->RenderToRtf( oRenderParameter );
					sResult += _T("}}");
				}
				else if (m_nFillType == 1 || m_nFillType == 2 || m_nFillType == 9)
				{
					sResult += _T("{\\sp{\\sn fillType}{\\sv 2}}");
					sResult += _T("{\\sp{\\sn fillBlip}{\\sv ");
					sResult +=  m_oPicture->RenderToRtf( oRenderParameter );
					sResult += _T("}}");
				}
			}
			//textbox
			if( m_aTextItems )
			{
				sResult += _T("{\\shptxt ");
				sResult += m_aTextItems->RenderToRtf( oRenderParameter );
				sResult += _T("}");
			}
			sResult += _T("}");
			if( m_oPicture && m_nShapeType == 75 )
			{
				sResult += _T("{\\shprslt\\par\\plain");
				sResult +=  m_oPicture->GenerateWMF( oRenderParameter );
				sResult += _T("\\par}");
			}
			sResult += _T("}");
		}
		//восстанавливаем координаты и если нужно поворачиваем
		m_nLeft = nLeft;
		m_nTop = nTop;
		m_nRight = nRight;
		m_nBottom = nBottom;
		m_nRotation = nRotate;

		m_nRelLeft = nRelLeft;
		m_nRelTop = nRelTop;
		m_nRelRight = nRelRight;
		m_nRelBottom = nRelBottom;
		m_nRelRotation = nRelRotate;
	}
	return sResult;
}
CString RtfShape::RenderToRtfShapeProperty(RenderParameter oRenderParameter)
{
	CString sResult;

//Position absolute
	if( PROP_DEF != m_nPositionH )
		sResult.AppendFormat( _T("{\\sp{\\sn posh}{\\sv %d}}"),	m_nPositionH);
	if( PROP_DEF != m_nPositionHRelative )
		sResult.AppendFormat( _T("{\\sp{\\sn posrelh}{\\sv %d}}"),	m_nPositionHRelative);
	if( PROP_DEF != m_nPositionV )
		sResult.AppendFormat( _T("{\\sp{\\sn posv}{\\sv %d}}"), m_nPositionV);
	
	if( PROP_DEF != m_nPositionVRelative )
		sResult.AppendFormat( _T("{\\sp{\\sn posrelv}{\\sv %d}}"), m_nPositionVRelative);
	if(  PROP_DEF != m_bLayoutInCell )
		sResult.AppendFormat( _T("{\\sp{\\sn fLayoutInCell}{\\sv %d}}"), m_bLayoutInCell);
	if(  PROP_DEF != m_bAllowOverlap )
		sResult.AppendFormat( _T("{\\sp{\\sn fAllowOverlap}{\\sv %d}}"),	m_bAllowOverlap);

//Position relative
	if(  PROP_DEF != m_nPositionHPct )
		sResult.AppendFormat( _T("{\\sp{\\sn pctHorizPos}{\\sv %d}}"),	m_nPositionHPct);
	if(  PROP_DEF != m_nPositionVPct )
		sResult.AppendFormat( _T("{\\sp{\\sn pctVertPos}{\\sv %d}}"),	m_nPositionVPct);
	if(  PROP_DEF != m_nPctWidth )
		sResult.AppendFormat( _T("{\\sp{\\sn pctHoriz}{\\sv %d}}"),	m_nPctWidth);
	if(  PROP_DEF != m_nPctHeight )
		sResult.AppendFormat( _T("{\\sp{\\sn pctVert}{\\sv %d}}"),	m_nPctHeight);
	if(  PROP_DEF != m_nPctWidthRelative )
		sResult.AppendFormat( _T("{\\sp{\\sn sizerelh}{\\sv %d}}"),	m_nPctWidthRelative);
	if(  PROP_DEF != m_nPctHeightRelative )
		sResult.AppendFormat( _T("{\\sp{\\sn sizerelv}{\\sv %d}}"),	m_nPctHeightRelative);
	if(  PROP_DEF != m_nColStart )
		sResult.AppendFormat( _T("{\\sp{\\sn colStart}{\\sv %d}}"),	m_nColStart);
	if(  PROP_DEF != m_nColSpan )
		sResult.AppendFormat( _T("{\\sp{\\sn colSpan}{\\sv %d}}"),	m_nColSpan);
//Rehydration
	if(  _T("") != m_sMetroBlob )
        sResult.AppendFormat( _T("{\\sp{\\sn metroBlob}{\\sv %d}}"),	m_sMetroBlob.GetBuffer());

//Object Type
	if(  PROP_DEF != m_bIsBullet )
		sResult.AppendFormat( _T("{\\sp{\\sn fIsBullet}{\\sv %d}}"),	m_bIsBullet);
	if(  PROP_DEF != m_nRotation  )
		sResult.AppendFormat( _T("{\\sp{\\sn rotation}{\\sv %d}}"),	m_nRotation);
	if(  PROP_DEF != m_bFlipV )
		sResult.AppendFormat( _T("{\\sp{\\sn fFlipV}{\\sv %d}}"),	m_bFlipV);
	if(  PROP_DEF != m_bFlipH )
		sResult.AppendFormat( _T("{\\sp{\\sn fFlipH}{\\sv %d}}"),	m_bFlipH);
	if(  PROP_DEF != m_nShapeType )
		sResult.AppendFormat( _T("{\\sp{\\sn shapeType}{\\sv %d}}"),	m_nShapeType);
	if(  PROP_DEF != m_nZOrderRelative )
		sResult.AppendFormat( _T("{\\sp{\\sn fBehindDocument}{\\sv %d}}"),	m_nZOrderRelative);
	if(  PROP_DEF != m_bHidden )
		sResult.AppendFormat( _T("{\\sp{\\sn fHidden}{\\sv %d}}"),	m_bHidden);
//Text
	//sResult += _T("{\\sp{\\sn fLockText}{\\sv 0}}");

	if(  PROP_DEF != m_nTexpLeft)
		sResult.AppendFormat( _T("{\\sp{\\sn dxTextLeft}{\\sv %d}}"),	m_nTexpLeft);
	if(  PROP_DEF != m_nTexpTop)
		sResult.AppendFormat( _T("{\\sp{\\sn dyTextTop}{\\sv %d}}"),	m_nTexpTop);
	if(  PROP_DEF != m_nTexpRight)
		sResult.AppendFormat( _T("{\\sp{\\sn dxTextRight}{\\sv %d}}"),	m_nTexpRight);
	if(  PROP_DEF != m_nTexpBottom)
		sResult.AppendFormat( _T("{\\sp{\\sn dyTextBottom}{\\sv %d}}"),	m_nTexpBottom);

	if(  PROP_DEF != m_nAnchorText )
		sResult.AppendFormat( _T("{\\sp{\\sn anchorText}{\\sv %d}}"),	m_nAnchorText);
	//else
	{
		if(  PROP_DEF != m_nWrapDistLeft )
			sResult.AppendFormat( _T("{\\sp{\\sn dxWrapDistLeft}{\\sv %d}}"), RtfUtility::Twips2Emu( m_nWrapDistLeft ));
		if(  PROP_DEF != m_nWrapDistTop )
			sResult.AppendFormat( _T("{\\sp{\\sn dyWrapDistTop}{\\sv %d}}"), RtfUtility::Twips2Emu( m_nWrapDistTop ));
		if(  PROP_DEF != m_nWrapDistRight ) 
			sResult.AppendFormat( _T("{\\sp{\\sn dxWrapDistRight}{\\sv %d}}"), RtfUtility::Twips2Emu( m_nWrapDistRight ));
		if(  PROP_DEF != m_nWrapDistBottom )
			sResult.AppendFormat( _T("{\\sp{\\sn dyWrapDistBottom}{\\sv %d}}"),RtfUtility::Twips2Emu(  m_nWrapDistBottom ));
	}
	if(  PROP_DEF != m_bFitShapeToText )
		sResult.AppendFormat( _T("{\\sp{\\sn fFitShapeToText}{\\sv %d}}"),	m_bFitShapeToText);
	if(  PROP_DEF != m_bFitTextToShape )
		sResult.AppendFormat( _T("{\\sp{\\sn fFitTextToShape}{\\sv %d}}"),	m_bFitTextToShape);
	if(  PROP_DEF != m_nCcol )
		sResult.AppendFormat( _T("{\\sp{\\sn ccol}{\\sv %d}}"),	m_nCcol);
	if(  PROP_DEF != m_nTxdir )
		sResult.AppendFormat( _T("{\\sp{\\sn txdir}{\\sv %d}}"),	m_nTxdir);
	if(  PROP_DEF != m_nWrapText )
		sResult.AppendFormat( _T("{\\sp{\\sn WrapText}{\\sv %d}}"),	m_nWrapText);
//Geometry
	if( PROP_DEF != m_nAdjustValue )
		sResult.AppendFormat( _T("{\\sp{\\sn adjustValue}{\\sv %d}}"),	m_nAdjustValue );
	if( PROP_DEF != m_nAdjustValue )
		sResult.AppendFormat( _T("{\\sp{\\sn adjust2Value}{\\sv %d}}"),	m_nAdjustValue2 );
	if( PROP_DEF != m_nAdjustValue )
		sResult.AppendFormat( _T("{\\sp{\\sn adjust3Value}{\\sv %d}}"),	m_nAdjustValue3 );
	if( PROP_DEF != m_nAdjustValue )
		sResult.AppendFormat( _T("{\\sp{\\sn adjust4Value}{\\sv %d}}"),	m_nAdjustValue4 );
	if( PROP_DEF != m_nAdjustValue )
		sResult.AppendFormat( _T("{\\sp{\\sn adjust5Value}{\\sv %d}}"),	m_nAdjustValue5 );
	if( PROP_DEF != m_nAdjustValue )
		sResult.AppendFormat( _T("{\\sp{\\sn adjust6Value}{\\sv %d}}"),	m_nAdjustValue6 );
	if( PROP_DEF != m_nAdjustValue )
		sResult.AppendFormat( _T("{\\sp{\\sn adjust7Value}{\\sv %d}}"),	m_nAdjustValue7 );
	if( PROP_DEF != m_nAdjustValue )
		sResult.AppendFormat( _T("{\\sp{\\sn adjust8Value}{\\sv %d}}"),	m_nAdjustValue8 );
	if( PROP_DEF != m_nAdjustValue )
		sResult.AppendFormat( _T("{\\sp{\\sn adjust9Value}{\\sv %d}}"),	m_nAdjustValue9 );
	if( PROP_DEF != m_nAdjustValue )
		sResult.AppendFormat( _T("{\\sp{\\sn adjust10Value}{\\sv %d}}"),	m_nAdjustValue10 );
//Connectors
	if( PROP_DEF != m_nConnectionType )
		sResult.AppendFormat( _T("{\\sp{\\sn cxk}{\\sv %d}}"),	m_nConnectionType );
	if( PROP_DEF != m_nConnectorStyle )
		sResult.AppendFormat( _T("{\\sp{\\sn cxstyle}{\\sv %d}}"),	m_nConnectorStyle );
//Picture Effects
	if( PROP_DEF != m_nCropFromTop )
		sResult.AppendFormat( _T("{\\sp{\\sn cropFromTop}{\\sv %d}}"),	m_nCropFromTop );
	if( PROP_DEF != m_nCropFromBottom )
		sResult.AppendFormat( _T("{\\sp{\\sn cropFromBottom}{\\sv %d}}"),	m_nCropFromBottom );
	if( PROP_DEF != m_nCropFromLeft )
		sResult.AppendFormat( _T("{\\sp{\\sn cropFromLeft}{\\sv %d}}"),	m_nCropFromLeft );
	if( PROP_DEF != m_nCropFromRight )
		sResult.AppendFormat( _T("{\\sp{\\sn cropFromRight}{\\sv %d}}"),	m_nCropFromRight );
//Grouped Shapes
	if( PROP_DEF != m_nGroupBottom )
		sResult.AppendFormat( _T("{\\sp{\\sn groupBottom}{\\sv %d}}"),	m_nCropFromRight );
	if( PROP_DEF != m_nGroupLeft )
		sResult.AppendFormat( _T("{\\sp{\\sn groupLeft}{\\sv %d}}"),	m_nGroupLeft );
	if( PROP_DEF != m_nGroupRight )
		sResult.AppendFormat( _T("{\\sp{\\sn groupRight}{\\sv %d}}"),	m_nGroupRight );
	if( PROP_DEF != m_nGroupTop )
		sResult.AppendFormat( _T("{\\sp{\\sn groupTop}{\\sv %d}}"),	m_nGroupTop );
	if( PROP_DEF != m_nRelBottom )
		sResult.AppendFormat( _T("{\\sp{\\sn relBottom}{\\sv %d}}"),	m_nRelBottom );
	if( PROP_DEF != m_nRelLeft )
		sResult.AppendFormat( _T("{\\sp{\\sn relLeft}{\\sv %d}}"),	m_nRelLeft );
	if( PROP_DEF != m_nRelRight )
		sResult.AppendFormat( _T("{\\sp{\\sn relRight}{\\sv %d}}"),	m_nRelRight );
	if( PROP_DEF != m_nRelTop )
		sResult.AppendFormat( _T("{\\sp{\\sn relTop}{\\sv %d}}"),	m_nRelTop );
	if( PROP_DEF != m_nRelRotation )
		sResult.AppendFormat( _T("{\\sp{\\sn relRotation}{\\sv %d}}"),	m_nRelRotation );
	if( PROP_DEF != m_nRelZOrder )
		sResult.AppendFormat( _T("{\\sp{\\sn dhgt}{\\sv %d}}"),		m_nRelZOrder );
//Fill
	if( 0 == m_bFilled )
		sResult += _T("{\\sp{\\sn fFilled}{\\sv 0}}");
	if( PROP_DEF != m_nFillType )
		sResult.AppendFormat( _T("{\\sp{\\sn fillType}{\\sv %d}}"), m_nFillType );
	if( PROP_DEF != m_nFillColor )
		sResult.AppendFormat( _T("{\\sp{\\sn fillColor}{\\sv %d}}"), m_nFillColor );
	if( PROP_DEF != m_nFillColor2 )
		sResult.AppendFormat( _T("{\\sp{\\sn fillBackColor}{\\sv %d}}"), m_nFillColor2 );
	if( PROP_DEF != m_nFillOpacity )
		sResult.AppendFormat( _T("{\\sp{\\sn fillOpacity}{\\sv %d}}"), ( m_nFillOpacity * 65536 /100 ) );
	if( PROP_DEF != m_nFillFocus )
		sResult.AppendFormat( _T("{\\sp{\\sn fillFocus}{\\sv %d}}"), m_nFillFocus );
	if( PROP_DEF != m_nFillAngle )
		sResult.AppendFormat( _T("{\\sp{\\sn fillAngle}{\\sv %d}}"), m_nFillAngle * 65536 );
//Line
	if( 0 == m_bLine )
		sResult += _T("{\\sp{\\sn fLine}{\\sv 0}}");
	if( PROP_DEF != m_nLineColor )
		sResult.AppendFormat( _T("{\\sp{\\sn lineColor}{\\sv %d}}"), m_nLineColor );
	if( PROP_DEF != m_nLineStartArrow )
		sResult.AppendFormat( _T("{\\sp{\\sn lineStartArrowhead}{\\sv %d}}"),	m_nLineStartArrow );
	if( PROP_DEF != m_nLineEndArrow )
		sResult.AppendFormat( _T("{\\sp{\\sn lineEndArrowhead}{\\sv %d}}"),	m_nLineEndArrow );
	if( PROP_DEF != m_nLineStartArrowWidth )
		sResult.AppendFormat( _T("{\\sp{\\sn lineStartArrowWidth}{\\sv %d}}"),	m_nLineStartArrowWidth );
	if( PROP_DEF != m_nLineStartArrowLength )
		sResult.AppendFormat( _T("{\\sp{\\sn lineStartArrowLength}{\\sv %d}}"),	m_nLineStartArrowLength );
	if( PROP_DEF != m_nLineEndArrowWidth )
		sResult.AppendFormat( _T("{\\sp{\\sn lineEndArrowWidth}{\\sv %d}}"),	m_nLineEndArrowWidth );
	if( PROP_DEF != m_nLineEndArrowLength )
		sResult.AppendFormat( _T("{\\sp{\\sn lineEndArrowLength}{\\sv %d}}"),	m_nLineEndArrowLength );
	if( PROP_DEF != m_nLineWidth )
		sResult.AppendFormat( _T("{\\sp{\\sn lineWidth}{\\sv %d}}"),	m_nLineWidth );
	if( PROP_DEF != m_nLineDashing )
		sResult.AppendFormat( _T("{\\sp{\\sn lineDashing}{\\sv %d}}"),	m_nLineDashing );

//pWrapPolygonVertices	Points of the text wrap polygon.
	int nWrapPointCount = (int)m_aWrapPoints.size();
	if( nWrapPointCount > 0 )
	{
		sResult.AppendFormat( _T("{\\sp{\\sn pWrapPolygonVertices}{\\sv 8;%d"), nWrapPointCount / 2 );
		for( int i = 0; i < nWrapPointCount - 1; i += 2 )
			sResult.AppendFormat( _T(";(%d,%d)"), m_aWrapPoints[i], m_aWrapPoints[ i + 1 ] );
		sResult += _T("}}");
	}
//WordArt
	if( PROP_DEF != m_bGtext )
	{
		sResult.AppendFormat( _T("{\\sp{\\sn fGtext}{\\sv %d}}"),	m_bGtext );
		if( !m_sGtextUNICODE.IsEmpty() )
		{
			sResult += _T("{\\sp{\\sn gtextUNICODE}{\\sv ");
			sResult += m_sGtextUNICODE + _T("}}");
		}
		if( !m_sGtextFont.IsEmpty() )
		{
			sResult += _T("{\\sp{\\sn gtextFont}{\\sv ");
			sResult += m_sGtextFont + _T("}}");
		}
		if( PROP_DEF != m_nGtextSize )
			sResult.AppendFormat( _T("{\\sp{\\sn gtextSize}{\\sv %d}}"), m_nGtextSize );
	}

	return sResult;
}
CString RtfShape::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	RtfDocument* poDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	if( PROP_DEF != m_nShapeType && 0 != m_nShapeType )
	{
		{
			TextItemContainerPtr aTempTextItems;
			if( 75 == m_nShapeType && 0 != m_aTextItems )
			{//Значит это Ole обьект с anchor, для него все также только TextBox надо делать по-другому
				aTempTextItems = m_aTextItems;
				m_aTextItems = TextItemContainerPtr();
				m_bIsOle = true;
			}

			sResult = RenderToOOXBegin(oRenderParameter);
			if( _T("") != sResult )
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
			if( false == sOle.IsEmpty() )
				sResult.Replace( _T("</w:pict>"), sOle + _T("</w:pict>") );
		}
	}
	return sResult;
}
CString RtfShape::RenderToOOXBegin(RenderParameter oRenderParameter)
{
	if( false == IsValid() )
		return _T("");

	RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
	CString sResult;
	
	if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2 == oRenderParameter.nType )
		;
	else if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE == oRenderParameter.nType )
		sResult += _T("<w:pict>");
	else
		sResult += _T("<w:r><w:pict>");//работает по умолчанию
	
	if( _T("") == oRenderParameter.sValue )
		sResult += _T("<v:shape");
	else
        sResult.AppendFormat( _T("<%ls"),oRenderParameter.sValue.GetBuffer() );

	CString strUniqId;
	if (m_sName.IsEmpty())
	{
		m_sName.AppendFormat(L"_x0000_s%d", poDocument->GetShapeId( m_nID ));
	}
	sResult += _T(" id=\"") + m_sName + _T("\"");

	if( PROP_DEF != m_nShapeType )
		sResult.AppendFormat( _T(" type=\"#_x0000_t%d\""),	m_nShapeType );

	if( 0 == m_bFilled) sResult += _T(" filled=\"f\"");
	else				sResult += _T(" filled=\"t\"");

	if( 0 == m_bLine)	sResult += _T(" stroked=\"f\"");
	else				sResult += _T(" stroked=\"t\"");

	if( PROP_DEF != m_nFillColor)
	{
		RtfColor color(m_nFillColor);
		sResult.AppendFormat( _T(" fillcolor=\"#") + color.ToHexColor(true) + _T("\""));
	}
	if( PROP_DEF != m_nLineColor)
	{
		RtfColor color(m_nLineColor);
		sResult.AppendFormat( _T(" strokecolor=\"#") + color.ToHexColor(true) + _T("\""));
	}

	//Connectors
	switch( m_nConnectionType )
	{
		case 0: sResult += _T(" o:connecttype=\"custom\"");		break;
		case 1: sResult += _T(" o:connecttype=\"none\"");		break;
		case 2: sResult += _T(" o:connecttype=\"rect\"");		break;
		case 3: sResult += _T(" o:connecttype=\"segments\"");	break;
	}
	switch( m_nConnectorStyle )
	{
		case 0: sResult += _T(" o:connectortype=\"straight\"");	break;
		case 1: sResult += _T(" o:connectortype=\"elbow\"");	break;
		case 2: sResult += _T(" o:connectortype=\"curved\"");	break;
		case 3: sResult += _T(" o:connectortype=\"none\"");		break;
	}
//-----------------------------------------------------------------------------------------------------------------
	CString sStyle ;
	if( PROP_DEF != m_nLeft &&  PROP_DEF != m_nRight && PROP_DEF != m_nTop && PROP_DEF != m_nBottom   )
	{
		//не пишем если inline
		if( 3 != m_nPositionHRelative || 3 != m_nPositionVRelative )
		{
			sStyle .Append		( _T("position:absolute;") );
			sStyle.AppendFormat	( _T("margin-left:%fpt;")	, RtfUtility::Twip2pt(m_nLeft) );
			sStyle.AppendFormat	( _T("margin-top:%fpt;")	, RtfUtility::Twip2pt(m_nTop));
			sStyle.AppendFormat	( _T("margin-bottom:%fpt;")	, RtfUtility::Twip2pt(m_nBottom) );
			sStyle.AppendFormat	( _T("margin-right:%fpt;")	, RtfUtility::Twip2pt(m_nRight));
		}
		int nWidth = m_nRight - m_nLeft;
		int nHeight = m_nBottom - m_nTop;
		sStyle.AppendFormat(_T("width:%fpt;height:%fpt;"), RtfUtility::Twip2pt(nWidth), RtfUtility::Twip2pt(nHeight));
	}
	else if( PROP_DEF != m_nRelLeft &&  PROP_DEF != m_nRelRight && PROP_DEF != m_nRelTop && PROP_DEF != m_nRelBottom  )
	{
		int nWidth	= m_nRelRight - m_nRelLeft;
		int nHeight = m_nRelBottom - m_nRelTop;
		
		sStyle.Append		( _T("position:absolute;") );
		sStyle.AppendFormat	( _T("left:%d;")			, m_nRelLeft );
		sStyle.AppendFormat	( _T("top:%d;")				, m_nRelTop);
		sStyle.AppendFormat	( _T("bottom:%d;")			, m_nRelBottom );
		sStyle.AppendFormat	( _T("right:%d;")			, m_nRelRight);
		sStyle.AppendFormat	( _T("width:%d;height:%d;")	, nWidth, nHeight);
	}
	else if( 0 != m_oPicture && PROP_DEF != m_oPicture->m_nWidthGoal && PROP_DEF != m_oPicture->m_nHeightGoal && PROP_DEF != (int)m_oPicture->m_dScaleX && PROP_DEF != (int)m_oPicture->m_dScaleY )
	{
		int nWidth = (int)(m_oPicture->m_nWidthGoal * m_oPicture->m_dScaleX / 100);
		if( PROP_DEF != m_oPicture->m_nCropL )
			nWidth -= m_oPicture->m_nCropL;
		if( PROP_DEF != m_oPicture->m_nCropR )
			nWidth -= m_oPicture->m_nCropR;

		int nHeight = (int)(m_oPicture->m_nHeightGoal * m_oPicture->m_dScaleY / 100);
		if( PROP_DEF != m_oPicture->m_nCropT )
			nHeight -= m_oPicture->m_nCropT;
		if( PROP_DEF != m_oPicture->m_nCropB )
			nHeight -= m_oPicture->m_nCropB;

		sStyle.AppendFormat(_T("width:%.2fpt;height:%.2fpt;"), RtfUtility::Twip2pt(nWidth), RtfUtility::Twip2pt(nHeight));
	}

	switch( m_nPositionH )
	{
		case 0: sStyle += _T("mso-position-horizontal:absolute;");	break;
		case 1: sStyle += _T("mso-position-horizontal:left;");		break;
		case 2: sStyle += _T("mso-position-horizontal:center;");	break;
		case 3: sStyle += _T("mso-position-horizontal:right;");		break;
		case 4: sStyle += _T("mso-position-horizontal:inside;");	break;
		case 5: sStyle += _T("mso-position-horizontal:outside;");	break;
	}
	if( PROP_DEF != m_nPositionHPct )//todo
	{
		sStyle.AppendFormat(_T("mso-left-percent:%d;"), m_nPositionHPct);
	}
	if( PROP_DEF != m_nPositionH && PROP_DEF == m_nPositionHRelative )
		m_nPositionHRelative = 2;
	if( PROP_DEF != m_nPositionHRelative )
	{
		switch( m_nPositionHRelative )
		{
			case 0: sStyle += _T("mso-position-horizontal-relative:margin;");				break;
			case 1: sStyle += _T("mso-position-horizontal-relative:page;");					break;
			case 2: sStyle += _T("mso-position-horizontal-relative:text;");					break;
			case 3: sStyle += _T("mso-position-horizontal-relative:char;");					break;
			case 4: sStyle += _T("mso-position-horizontal-relative:left-margin-area;");		break;
			case 5: sStyle += _T("mso-position-horizontal-relative:right-margin-area;");	break;
			case 6: sStyle += _T("mso-position-horizontal-relative:inner-margin-area;");	break;
			case 7: sStyle += _T("mso-position-horizontal-relative:outer-margin-area;");	break;
		}
	}
	else
	{
		switch( m_eXAnchor )
		{
			case ax_page:	sStyle += _T("mso-position-horizontal-relative:page;");		break;
			case ax_margin: sStyle += _T("mso-position-horizontal-relative:margin;");	break;
			//case ax_column: sStyle += _T("mso-position-horizontal-relative:text;"));break;
		}
	}

	switch( m_nPositionV )
	{
		case 0: sStyle += _T("mso-position-vertical:absolute;");	break;
		case 1: sStyle += _T("mso-position-vertical:top;");			break;
		case 2: sStyle += _T("mso-position-vertical:center;");		break;
		case 3: sStyle += _T("mso-position-vertical:bottom;");		break;
		case 4: sStyle += _T("mso-position-vertical:inside;");		break;
		case 5: sStyle += _T("mso-position-vertical:outside;");		break;
	}
	if( PROP_DEF != m_nPositionVPct )
		sStyle.AppendFormat(_T("mso-top-percent:%d;"),	m_nPositionVPct);

	if( PROP_DEF != m_nPositionV && PROP_DEF == m_nPositionVRelative )
		m_nPositionVRelative =2;
	if( PROP_DEF != m_nPositionVRelative )
	{
		switch( m_nPositionVRelative )
		{
			case 0: sStyle += _T("mso-position-vertical-relative:margin;");				break;
			case 1: sStyle += _T("mso-position-vertical-relative:page;");				break;
			case 2: sStyle += _T("mso-position-vertical-relative:text;");				break;
			case 3: sStyle += _T("mso-position-vertical-relative:line;");				break;
			case 4: sStyle += _T("mso-position-vertical-relative:top-margin-area;");	break;
			case 5: sStyle += _T("mso-position-vertical-relative:bottom-margin-area;");	break;
			case 6: sStyle += _T("mso-position-vertical-relative:inner-margin-area;");	break;
			case 7: sStyle += _T("mso-position-vertical-relative:outer-margin-area;");	break;
		}
	}
	else
	{
		switch( m_eYAnchor )
		{
			case ay_page: sStyle += _T("mso-position-vertical-relative:page;");			break;
			case ay_margin: sStyle += _T("mso-position-vertical-relative:margin;");		break;
			//case ay_Para: sStyle += _T("mso-position-vertical-relative:text;");		break;
		}
	}
	if( PROP_DEF != m_nPctWidth )
		sStyle.AppendFormat(_T("mso-width-percent:%d;"),	m_nPctWidth);
	switch( m_nPctWidthRelative )
	{
		case 0:	sStyle += _T("mso-width-relative:margin;");				break;
		case 1:	sStyle += _T("mso-width-relative:page;");				break;
		case 2:	sStyle += _T("mso-width-relative:left-margin-area;");	break;
		case 3:	sStyle += _T("mso-width-relative:right-margin-area;");	break;
		case 4:	sStyle += _T("mso-width-relative:inner-margin-area;");	break;
		case 5:	sStyle += _T("mso-width-relative:outer-margin-area;");	break;
	}
	
	if( PROP_DEF != m_nPctHeight )
		sStyle.AppendFormat(_T("mso-height-percent:%d;"),	m_nPctHeight);
	
	switch( m_nPctHeightRelative )
	{
		case 0:	sStyle += _T("mso-height-relative:margin;");				break;
		case 1:	sStyle += _T("mso-height-relative:page;");					break;
		case 2:	sStyle += _T("mso-height-relative:top-margin-area;");		break;
		case 3:	sStyle += _T("mso-height-relative:bottom-margin-area;");	break;
		case 4:	sStyle += _T("mso-height-relative:inner-margin-area;");		break;
		case 5:	sStyle += _T("mso-height-relative:outer-margin-area;");		break;
	}

	if( PROP_DEF != m_nRotation )
		sStyle.AppendFormat(_T("rotation:%d;"),	m_nRotation / 65536 );
	else if( PROP_DEF != m_nRelRotation )
		sStyle.AppendFormat(_T("rotation:%d;"),	m_nRelRotation / 65536 );

	int nZIndex = PROP_DEF;
	if( PROP_DEF != m_nRelZOrder )
		nZIndex = m_nRelZOrder;
	else if( PROP_DEF != m_nZOrder )
		nZIndex = m_nZOrder;
	else
		nZIndex = 100; //на свое усмотрение ставлю 100
	if( PROP_DEF != m_nZOrderRelative )
	{
		//берем большое значение чтобы сделать строго выше или ниже текста
		if( 0 == m_nZOrderRelative )
			sStyle.AppendFormat( _T("z-index:%d;"), 10000 + nZIndex );//Text is below shape
		else
			sStyle.AppendFormat( _T("z-index:%d;"), -10000 + nZIndex );//Shape is below text
	}
	else
		sStyle.AppendFormat( _T("z-index:%d;"), nZIndex );

	if(  PROP_DEF != m_nWrapDistLeft )
		sStyle.AppendFormat( _T("mso-wrap-distance-left:%fpt;"), RtfUtility::Twip2pt( m_nWrapDistLeft ));
	if(  PROP_DEF != m_nWrapDistTop )
		sStyle.AppendFormat( _T("mso-wrap-distance-top:%fpt;"), RtfUtility::Twip2pt( m_nWrapDistTop ));
	if(  PROP_DEF != m_nWrapDistRight ) 
		sStyle.AppendFormat( _T("mso-wrap-distance-right:%fpt;"), RtfUtility::Twip2pt( m_nWrapDistRight ));
	if(  PROP_DEF != m_nWrapDistBottom )
		sStyle.AppendFormat( _T("mso-wrap-distance-bottom:%fpt;"), RtfUtility::Twip2pt( m_nWrapDistBottom ));

	switch( m_nAnchorText)
	{
		case 0:	sStyle += _T("v-text-anchor:top;");						break;
		case 1:	sStyle += _T("v-text-anchor:middle;");					break;
		case 2:	sStyle += _T("v-text-anchor:bottom;");					break;
		case 3:	sStyle += _T("v-text-anchor:topcenter;");				break;
		case 4:	sStyle += _T("v-text-anchor:middle-center;");			break;
		case 5:	sStyle += _T("v-text-anchor:bottom-center;");			break;
		case 6:	sStyle += _T("v-text-anchor:top-baseline;");			break;
		case 7:	sStyle += _T("v-text-anchor:bottom-baseline;");			break;
		case 8:	sStyle += _T("v-text-anchor:top-center-baseline;");		break;
		case 9:	sStyle += _T("v-text-anchor:bottom-center-baseline;");	break;
	}

//---------------------------------------------------------------------------------------------------------------------------
	if( false == sStyle.IsEmpty() )
	{
		sStyle.Delete( sStyle.GetLength() - 1 );
        sResult += _T(" style=\"") + sStyle + _T("\"");
	}
//----------------------------------------------------------------------------------------------------------------------------
	if( PROP_DEF != m_bLayoutInCell )
	{
		if( 0 == m_bLayoutInCell )
			sResult += _T(" o:allowincell=\"false\"");
		else
			sResult += _T(" o:allowincell=\"true\"");
	}
	if( PROP_DEF != m_bAllowOverlap )
	{
		if( 0 == m_bAllowOverlap )
			sResult += _T(" o:allowoverlap=\"false\"");
		else
			sResult += _T(" o:allowoverlap=\"true\"");
	}
	//Position

	//Geometry
	CString sAdjust;
	if( PROP_DEF != m_nAdjustValue )
	{
		sAdjust.AppendFormat( _T("%d"),	m_nAdjustValue);
		if( PROP_DEF != m_nAdjustValue2 )
		{
			sAdjust.AppendFormat( _T(",%d"),	m_nAdjustValue2);
			if( PROP_DEF != m_nAdjustValue3 )
			{
				sAdjust.AppendFormat( _T(",%d"),	m_nAdjustValue3);
				if( PROP_DEF != m_nAdjustValue4 )
				{
					sAdjust.AppendFormat( _T(",%d"),	m_nAdjustValue4);
					if( PROP_DEF != m_nAdjustValue5 )
					{
						sAdjust.AppendFormat( _T(",%d"),	m_nAdjustValue5);
						if( PROP_DEF != m_nAdjustValue6 )
						{
							sAdjust.AppendFormat( _T(",%d"),	m_nAdjustValue6);
							if( PROP_DEF != m_nAdjustValue7 )
							{
								sAdjust.AppendFormat( _T(",%d"),	m_nAdjustValue7);
								if( PROP_DEF != m_nAdjustValue8 )
								{
									sAdjust.AppendFormat( _T(",%d"),	m_nAdjustValue8);
									if( PROP_DEF != m_nAdjustValue9 )
									{
										sAdjust.AppendFormat( _T(",%d"),	m_nAdjustValue9);
										if( PROP_DEF != m_nAdjustValue10 )
										{
											sAdjust.AppendFormat( _T(",%d"),	m_nAdjustValue10);
										}
									}
								}
							}
						}
					}
				}
			}
		}
    sResult.AppendFormat( _T(" adj=\"%ls\""),sAdjust.GetBuffer() );
	}
	//Geometry
	if( m_aWrapPoints.size() > 0 )
	{
		sResult += _T(" wrapcoords=\"");
		sResult.AppendFormat( _T("%d"), m_aWrapPoints[0]);
		
		for( int i = 1; i < (int)m_aWrapPoints.size(); i++ )
		{
			sResult.AppendFormat( _T(" %d"), m_aWrapPoints[i]);
		}
		
		sResult += _T("\"");
	}
	if( PROP_DEF != m_nGroupLeft && PROP_DEF != m_nGroupTop )
		sResult.AppendFormat( _T(" coordorigin=\"%d,%d\""), m_nGroupLeft, m_nGroupTop);
	if( PROP_DEF != m_nGroupLeft && PROP_DEF != m_nGroupTop && PROP_DEF != m_nGroupRight && PROP_DEF != m_nGroupBottom)
		sResult.AppendFormat( _T(" coordsize=\"%d,%d\""), m_nGroupRight - m_nGroupLeft, m_nGroupBottom - m_nGroupTop );
	
	if( true == m_bIsOle )		sResult += _T(" o:ole=\"\"");
	
	if(PROP_DEF != m_nLineWidth)
		sResult.AppendFormat( _T(" strokeweight=\"%fpt\""), RtfUtility::Emu2Pt(m_nLineWidth) );
	sResult += _T(">");
	
	if( PROP_DEF != m_nWrapType && 3 != m_nWrapType)
	{
		sResult += _T("<w10:wrap");

		switch( m_nWrapType )
		{
			case 1:sResult += _T(" type=\"topAndBottom\"");	break;
			case 2:sResult += _T(" type=\"square\"");		break;
			case 3:sResult += _T(" type=\"none\"");			break;
			case 4:sResult += _T(" type=\"tight\"");		break;
			case 5:sResult += _T(" type=\"through\"");		break;
		}
		switch( m_nWrapSideType )
		{
			case 0:sResult += _T(" side=\"both\"");			break;
			case 1:sResult += _T(" side=\"left\"");			break;
			case 2:sResult += _T(" side=\"right\"");		break;
			case 3:sResult += _T(" side=\"largest\"");		break;
		}
		sResult += _T("/>");
	}
	
//Line
	if( 0 != m_bLine)
	{
		CString sStroke;
		switch( m_nLineDashing )
		{
			case 1: sStroke += _T(" dashstyle=\"shortdash\"");		break;
			case 2: sStroke += _T(" dashstyle=\"shortdot\"");		break;
			case 3: sStroke += _T(" dashstyle=\"shortdashdot\"");	break;
			case 4: sStroke += _T(" dashstyle=\"shortdashdotdot\"");break;
			case 5: sStroke += _T(" dashstyle=\"dot\"");			break;
			case 6: sStroke += _T(" dashstyle=\"dash\"");			break;
			case 7: sStroke += _T(" dashstyle=\"longdash\"");		break;
			case 8: sStroke += _T(" dashstyle=\"dashdot\"");		break;
			case 9: sStroke += _T(" dashstyle=\"longdashdot\"");	break;
			case 10: sStroke += _T(" dashstyle=\"longdashdotdot\"");break;
		}
		switch( m_nLineStartArrow )
		{
			case 0: sStroke += _T(" startarrow=\"none\"");		break;
			case 1: sStroke += _T(" startarrow=\"block\"");		break;
			case 2: sStroke += _T(" startarrow=\"classic\"");	break;
			case 3: sStroke += _T(" startarrow=\"diamond\"");	break;
			case 4: sStroke += _T(" startarrow=\"oval\"");		break;
			case 5: sStroke += _T(" startarrow=\"open\"");		break;
			case 6: sStroke += _T(" startarrow=\"block\"");		break;
			case 7: sStroke += _T(" startarrow=\"block\"");		break;
		}
		switch( m_nLineStartArrowLength )
		{
			case 0: sStroke += _T(" startarrowlength=\"short\"");	break;
			case 1: sStroke += _T(" startarrowlength=\"medium\"");	break;
			case 2: sStroke += _T(" startarrowlength=\"long\"");	break;
		}
		switch( m_nLineStartArrowWidth )
		{
			case 0: sStroke += _T(" startarrowwidth=\"narrow\"");	break;
			case 1: sStroke += _T(" startarrowwidth=\"medium\"");	break;
			case 2: sStroke += _T(" startarrowwidth=\"wide\"");		break;
		}
		switch( m_nLineEndArrow )
		{
			case 0: sStroke += _T(" endarrow=\"none\"");	break;
			case 1: sStroke += _T(" endarrow=\"block\"");	break;
			case 2: sStroke += _T(" endarrow=\"classic\"");	break;
			case 3: sStroke += _T(" endarrow=\"diamond\"");	break;
			case 4: sStroke += _T(" endarrow=\"oval\"");	break;
			case 5: sStroke += _T(" endarrow=\"open\"");	break;
			case 6: sStroke += _T(" endarrow=\"block\"");	break;
			case 7: sStroke += _T(" endarrow=\"block\"");	break;
		}
		switch( m_nLineEndArrowLength )
		{
			case 0: sStroke += _T(" endarrowlength=\"short\"");		break;
			case 1: sStroke += _T(" endarrowlength=\"medium\"");	break;
			case 2: sStroke += _T(" endarrowlength=\"long\"");		break;
		}
		switch( m_nLineEndArrowWidth )
		{
			case 0: sStroke += _T(" endarrowwidth=\"narrow\"");	break;
			case 1: sStroke += _T(" endarrowwidth=\"medium\"");	break;
			case 2: sStroke += _T(" endarrowwidth=\"wide\"");	break;
		}

		if( false == sStroke.IsEmpty())
		{
			sResult += _T("<v:stroke ") + sStroke + _T("/>");
		}
	}

	if( 0 != m_aTextItems )
	{
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

		sResult += _T("<v:textbox");
		if (m_nTexpLeft != PROP_DEF && m_nTexpTop != PROP_DEF && m_nTexpRight != PROP_DEF && m_nTexpBottom != PROP_DEF)
		{
			sResult.AppendFormat( _T("< inset=\"%.2fpt,%.2fpt,%.2fpt,%.2fpt\">"), 
			RtfUtility::Emu2Pt(m_nTexpLeft), RtfUtility::Emu2Pt(m_nTexpTop), RtfUtility::Emu2Pt(m_nTexpRight), RtfUtility::Emu2Pt(m_nTexpBottom) );
		}
		else  
			sResult += _T(">");
		
		sResult += _T("<w:txbxContent>");
		sResult +=  m_aTextItems->RenderToOOX(oNewParam);
		sResult += _T("</w:txbxContent>");
		sResult += _T("</v:textbox>");
	}
	
	CString sPicture;	
	if( m_oPicture )
	{
		sPicture = m_oPicture->RenderToOOX(oRenderParameter);
		
		if (m_nShapeType == PROP_DEF || m_nShapeType == 75)
		{
			if( sPicture.IsEmpty() )//если не сохранилась картинка, то весь shape-picture будет бесполезным
				return _T("");

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

			sResult += _T("<v:imagedata r:id=\"") + sPicture + _T("\"");

			if( PROP_DEF != nCropLeft )
				sResult.AppendFormat( _T(" cropleft=\"%df\""), nCropLeft );
			if( PROP_DEF != nCropTop )
				sResult.AppendFormat( _T(" croptop=\"%df\""), nCropTop );
			if( PROP_DEF != nCropRight )
				sResult.AppendFormat( _T(" cropright=\"%df\""), nCropRight );
			if( PROP_DEF != nCropBottom )
				sResult.AppendFormat( _T(" cropbottom=\"%df\""), nCropBottom );

			sResult += _T(" o:title=\"\"/>");
		}
	}
//-----------------------------------------------------------------------------------------------
	if( 0 != m_bFilled)
	{
		sResult += _T("<v:fill");

		if (!sPicture.IsEmpty() && m_nShapeType != 75)
		{
 			sResult += _T(" r:id=\"") + sPicture + _T("\"");
			
			if( PROP_DEF == m_nFillType) m_nFillType = 2;
		}
		switch(m_nFillType)
		{
		case 1:	sResult += _T(" type=\"pattern\"");		break;
		case 2:	sResult += _T(" type=\"tile\"");		break;
		case 3:	sResult += _T(" type=\"frame\"");		break;
		case 4:	sResult += _T(" type=\"gradient\"");	break;
		case 5:	sResult += _T(" type=\"gradient\"");	break;
		case 6:	sResult += _T(" type=\"gradient\"");	break;
		case 7:	sResult += _T(" type=\"gradient\"");	break;
		case 8:	sResult += _T(" type=\"gradient\"");	break;
		case 9:	sResult += _T(" type=\"gradient\"");	break;
		}//todooo center radial ...

		if ( PROP_DEF != m_nFillOpacity)
		{
			CString sOpacity; sOpacity.Format(L"%d", /*100 - */m_nFillOpacity);
			sResult += _T(" opacity=\"") + sOpacity +_T("%\"");
			//sResult.AppendFormat( _T(" opacity=\"%df\""), m_nFillOpacity );
		}
		if ( PROP_DEF != m_nFillColor2)
		{
			RtfColor color(m_nFillColor2);
			sResult += _T(" color2=\"#") + color.ToHexColor(true) + _T("\"");
		}
		if ( PROP_DEF != m_nFillFocus)
		{
			CString sFocus; sFocus.Format(L"%d", m_nFillFocus );
			sResult += _T(" focus=\"")+ sFocus +_T("%\"");
		}
		
		if ( PROP_DEF != m_nFillAngle)
			sResult.AppendFormat( _T(" angle=\"%d\""), m_nFillAngle );

		sResult += _T("/>");
	}
//---------------------------------------------------------------------------------------------------------------------------
	if( false == m_sGtextUNICODE.IsEmpty())
	{
		sResult += _T("<v:textpath"); 

		CString sTextStyle;

		if ( PROP_DEF != m_sGtextFont )
		{
			sTextStyle += _T("font-family:") + XmlUtils::EncodeXmlString(m_sGtextFont) + _T(";");//todooo значения как в кавычках так и без - проверить как без
		}

		if (!sTextStyle.IsEmpty())
			sResult += _T(" style=\"") + sTextStyle + _T("\"");

		if ( PROP_DEF != m_nGtextSize )
			sTextStyle.AppendFormat( _T("font-size:%dpt;"), m_nGtextSize );

		sResult += _T(" string=\"") + XmlUtils::EncodeXmlString(m_sGtextUNICODE) + _T("\"");
		sResult += _T("/>");
	}
	return sResult;
}
CString RtfShape::RenderToOOXEnd(RenderParameter oRenderParameter)
{
	CString sResult;
	if( _T("") == oRenderParameter.sValue )
		sResult += _T("</v:shape>");
	else
        sResult.AppendFormat( _T("</%ls>"), oRenderParameter.sValue.GetBuffer() );
	if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE2 == oRenderParameter.nType )
		;
	else if( RENDER_TO_OOX_PARAM_SHAPE_WSHAPE == oRenderParameter.nType )
		sResult += _T("</w:pict>");
	else
		sResult += _T("</w:pict></w:r>");//работает по умолчанию
	return sResult;
}
CString RtfShapeGroup::RenderToRtf(RenderParameter oRenderParameter)
{
	return RtfShape::RenderToRtf(  oRenderParameter  );
}
CString RtfShapeGroup::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	RenderParameter oNewParamGroup	= oRenderParameter;
	oNewParamGroup.sValue			= _T("v:group");
	RenderParameter oNewParamShape	= oRenderParameter;
	oNewParamShape.sValue			= _T("v:shape");
	
	sResult = RenderToOOXBegin( oNewParamGroup );

	for( int i = 0; i < (int)m_aArray.size(); i++ )
	{
		sResult +=  m_aArray[i]->RenderToOOX( oNewParamShape );
	}

	sResult +=  RenderToOOXEnd( oNewParamGroup );
	return sResult;
}
