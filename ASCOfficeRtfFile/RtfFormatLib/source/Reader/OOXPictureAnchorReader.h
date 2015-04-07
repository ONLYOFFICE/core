#pragma once
#include "OOXPictureGraphicReader.h"
#include "OOXReaderBasic.h"

class OOXPictureAnchorReader
{
private:
	OOX::Drawing::CAnchor *m_ooxAnchor;
public: 
	OOXPictureAnchorReader(OOX::Drawing::CAnchor *ooxAnchor)
	{
		m_ooxAnchor = ooxAnchor;
	}
	
	bool Parse( ReaderParameter oParam , RtfShape& oOutput)
	{
		if (m_ooxAnchor == NULL) return false;

		oOutput.m_oPicture = RtfPicturePtr( new RtfPicture() );
		oOutput.m_eShapeType = RtfShape::st_anchor;
		oOutput.m_nShapeType = 75;

		oOutput.m_bAllowOverlap		= m_ooxAnchor->m_oAllowOverlap.IsInit() ? m_ooxAnchor->m_oAllowOverlap->ToBool() : false;
		oOutput.m_nZOrderRelative	= m_ooxAnchor->m_oBehindDoc.IsInit() ? 	m_ooxAnchor->m_oBehindDoc->ToBool() : false;
		oOutput.m_bHidden			= m_ooxAnchor->m_oHidden.IsInit() ? m_ooxAnchor->m_oHidden->ToBool() : false;
		oOutput.m_bLayoutInCell		= m_ooxAnchor->m_oLayoutInCell.IsInit() ? m_ooxAnchor->m_oLayoutInCell->ToBool() : false;
		oOutput.m_bLockAnchor		= m_ooxAnchor->m_oLocked.IsInit() ? m_ooxAnchor->m_oLocked->ToBool() : false;
		oOutput.m_nZOrder			= m_ooxAnchor->m_oRelativeHeight.IsInit() ? true : false;
			
		int nDistLeft	= m_ooxAnchor->m_oDistL.IsInit() ? m_ooxAnchor->m_oDistL->ToTwips() : PROP_DEF;
		int nDistTop	= m_ooxAnchor->m_oDistT.IsInit() ? m_ooxAnchor->m_oDistT->ToTwips() : PROP_DEF;
		int nDistRight	= m_ooxAnchor->m_oDistR.IsInit() ? m_ooxAnchor->m_oDistR->ToTwips() : PROP_DEF;
		int nDistBottom = m_ooxAnchor->m_oDistB.IsInit() ? m_ooxAnchor->m_oDistB->ToTwips() : PROP_DEF;

		BOOL bSimplePos = m_ooxAnchor->m_oSimplePos.IsInit() ? true : false;

		int nWidth = PROP_DEF;
		int nHeight = PROP_DEF;

		if( m_ooxAnchor->m_oSimplePos.IsInit())
		{
			int nLeft = m_ooxAnchor->m_oSimplePos->m_oX.ToTwips() ;
			int nTop = m_ooxAnchor->m_oSimplePos->m_oY.ToTwips();

		}
		
		if( m_ooxAnchor->m_oExtent.IsInit() )
		{
			nWidth	= m_ooxAnchor->m_oExtent->m_oCx.ToTwips();
			nHeight = m_ooxAnchor->m_oExtent->m_oCy.ToTwips();

			if( PROP_DEF != oOutput.m_nLeft && PROP_DEF != oOutput.m_nTop )//всегда !!
			{
				oOutput.m_nRight = oOutput.m_nLeft + nWidth;
				oOutput.m_nBottom = oOutput.m_nTop + nHeight;
			}
		}
		if( m_ooxAnchor->m_oPositionH.IsInit() )
		{
			if (m_ooxAnchor->m_oPositionH->m_oRelativeFrom.IsInit())
			{
				switch(m_ooxAnchor->m_oPositionH->m_oRelativeFrom->GetValue())
				{
				case SimpleTypes::relfromhCharacter     : 
					{
				oOutput.m_nPositionHRelative = 3;
				oOutput.m_eXAnchor = RtfShape::ax_margin;
					}break;
				case SimpleTypes::relfromhColumn        : 					
					{
				oOutput.m_nPositionHRelative = 2;
				oOutput.m_eXAnchor = RtfShape::ax_column;
					}break;
				case SimpleTypes::relfromhInsideMargin  : 					
					{
				oOutput.m_nPositionHRelative = 6;
				oOutput.m_eXAnchor = RtfShape::ax_margin;
					}break;
				case SimpleTypes::relfromhLeftMargin    : 					
					{
				oOutput.m_nPositionHRelative = 4;
				oOutput.m_eXAnchor = RtfShape::ax_margin;
					}break;
				case SimpleTypes::relfromhMargin        : 					
					{
				oOutput.m_nPositionHRelative = 0;
				oOutput.m_eXAnchor = RtfShape::ax_margin;
					}break;
				case SimpleTypes::relfromhOutsideMargin : 					
					{
				oOutput.m_nPositionHRelative = 7;
				oOutput.m_eXAnchor = RtfShape::ax_margin;
					}break;
				case SimpleTypes::relfromhPage          : 					
					{
				oOutput.m_nPositionHRelative = 1;
				oOutput.m_eXAnchor = RtfShape::ax_page;
					}break;
				case SimpleTypes::relfromhRightMargin   : 					
					{
				oOutput.m_nPositionHRelative = 5;
				oOutput.m_eXAnchor = RtfShape::ax_margin;
					}break;
				}
			}
			if (m_ooxAnchor->m_oPositionH->m_oAlign.IsInit())
			{
				switch(m_ooxAnchor->m_oPositionH->m_oAlign->GetValue())
				{
				case SimpleTypes::alignhCenter  : oOutput.m_nPositionH = 2; break;
				case SimpleTypes::alignhInside  : oOutput.m_nPositionH = 4; break;
				case SimpleTypes::alignhLeft    : oOutput.m_nPositionH = 1; break;
				case SimpleTypes::alignhOutside : oOutput.m_nPositionH = 5; break;
				case SimpleTypes::alignhRight   : oOutput.m_nPositionH = 3; break;
				}
			}
			if (m_ooxAnchor->m_oPositionH->m_oPosOffset.IsInit())
				oOutput.m_nLeft = m_ooxAnchor->m_oPositionH->m_oPosOffset->ToTwips();
		}
		if(m_ooxAnchor->m_oPositionV.IsInit())
		{
			if (m_ooxAnchor->m_oPositionV->m_oRelativeFrom.IsInit())
			{
				switch(m_ooxAnchor->m_oPositionV->m_oRelativeFrom->GetValue())
				{
					case SimpleTypes::relfromvBottomMargin  : 
						{
				oOutput.m_nPositionVRelative = 5;
				oOutput.m_eYAnchor = RtfShape::ay_margin;
						}break;
					case SimpleTypes::relfromvInsideMargin  :
						{
				oOutput.m_nPositionVRelative = 6;
				oOutput.m_eYAnchor = RtfShape::ay_margin;
						}break;
					case SimpleTypes::relfromvLine          :
						{
				oOutput.m_nPositionVRelative = 3;
				oOutput.m_eYAnchor = RtfShape::ay_Para;
						}break;
					case SimpleTypes::relfromvMargin        :
						{
				oOutput.m_nPositionVRelative = 0;
				oOutput.m_eYAnchor = RtfShape::ay_margin;
						}break;
					case SimpleTypes::relfromvOutsideMargin : 
						{
				oOutput.m_nPositionVRelative = 7;
				oOutput.m_eYAnchor = RtfShape::ay_margin;
						}break;
					case SimpleTypes::relfromvPage          :
						{
				oOutput.m_nPositionVRelative = 1;
				oOutput.m_eYAnchor = RtfShape::ay_page;
						}break;
					case SimpleTypes::relfromvParagraph     :
						{
				oOutput.m_nPositionVRelative = 2;
				oOutput.m_eYAnchor = RtfShape::ay_Para;
						}break;
					case SimpleTypes::relfromvTopMargin     :
						{
				oOutput.m_nPositionVRelative = 4;
				oOutput.m_eYAnchor = RtfShape::ay_margin;
						}break;
				}
			}
			if(m_ooxAnchor->m_oPositionV->m_oAlign.IsInit())
			{
				switch(m_ooxAnchor->m_oPositionV->m_oAlign->GetValue())
				{
					case SimpleTypes::alignvBottom  : oOutput.m_nPositionV = 3; break;
					case SimpleTypes::alignvCenter  : oOutput.m_nPositionV = 2; break;
					case SimpleTypes::alignvInside  : oOutput.m_nPositionV = 4; break;
					case SimpleTypes::alignvOutside : oOutput.m_nPositionV = 5; break;
					case SimpleTypes::alignvTop     : oOutput.m_nPositionV = 1; break;
				}
			}
			if(m_ooxAnchor->m_oPositionV->m_oPosOffset.IsInit())
				oOutput.m_nTop = m_ooxAnchor->m_oPositionV->m_oPosOffset->ToTwips();
		}
		if(m_ooxAnchor->m_oWrapNone.IsInit())
			oOutput.m_nWrapType = 3;
		if(m_ooxAnchor->m_oWrapSquare.IsInit())
		{
			oOutput.m_nWrapType = 2;
			if (m_ooxAnchor->m_oWrapSquare->m_oWrapText.IsInit())
			{
				switch(m_ooxAnchor->m_oWrapSquare->m_oWrapText->GetValue())
				{
				case SimpleTypes::wraptextBothSides : oOutput.m_nWrapSideType = 0; break;
				case SimpleTypes::wraptextLargest   : oOutput.m_nWrapSideType = 3; break;
				case SimpleTypes::wraptextLeft      : oOutput.m_nWrapSideType = 1; break;
				case SimpleTypes::wraptextRight     : oOutput.m_nWrapSideType = 2; break;
				}
			}
		}
		if(m_ooxAnchor->m_oWrapThrough.IsInit())
		{
			oOutput.m_nWrapType = 5;
			if (m_ooxAnchor->m_oWrapThrough->m_oWrapText.IsInit())
			{
				switch(m_ooxAnchor->m_oWrapThrough->m_oWrapText->GetValue())
				{
				case SimpleTypes::wraptextBothSides : oOutput.m_nWrapSideType = 0; break;
				case SimpleTypes::wraptextLargest   : oOutput.m_nWrapSideType = 3; break;
				case SimpleTypes::wraptextLeft      : oOutput.m_nWrapSideType = 1; break;
				case SimpleTypes::wraptextRight     : oOutput.m_nWrapSideType = 2; break;
				}
			}
			if (m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon.IsInit())///??? todooo twips ? pt?
			{
				int nValue;
				if (m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_oStart.IsInit())
				{
					nValue = m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_oStart->m_oX.ToTwips();
					oOutput.m_aWrapPoints.push_back( nValue );
					
					nValue =m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_oStart->m_oY.ToTwips();
					oOutput.m_aWrapPoints.push_back( nValue );
				}
				for( int i = 0; i < m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_arrLineTo.size(); i++ )
				{
					nValue = m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_arrLineTo[i]->m_oX.ToTwips();
					oOutput.m_aWrapPoints.push_back( nValue );
					
					nValue =m_ooxAnchor->m_oWrapThrough->m_oWrapPolygon->m_arrLineTo[i]->m_oY.ToTwips();
					oOutput.m_aWrapPoints.push_back( nValue );
				}
			}
		}
		if(m_ooxAnchor->m_oWrapTight.IsInit())
		{
			oOutput.m_nWrapType = 4;
			if (m_ooxAnchor->m_oWrapTight->m_oWrapText.IsInit())
			{
				switch(m_ooxAnchor->m_oWrapTight->m_oWrapText->GetValue())
				{
				case SimpleTypes::wraptextBothSides : oOutput.m_nWrapSideType = 0; break;
				case SimpleTypes::wraptextLargest   : oOutput.m_nWrapSideType = 3; break;
				case SimpleTypes::wraptextLeft      : oOutput.m_nWrapSideType = 1; break;
				case SimpleTypes::wraptextRight     : oOutput.m_nWrapSideType = 2; break;
				}
			}
			if (m_ooxAnchor->m_oWrapTight->m_oWrapPolygon.IsInit())
			{
				int nValue;

				if (m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_oStart.IsInit())
				{
					nValue = m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_oStart->m_oX.ToTwips();
					oOutput.m_aWrapPoints.push_back( nValue );
					
					nValue =m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_oStart->m_oY.ToTwips();
					oOutput.m_aWrapPoints.push_back( nValue );
				}

				for( int i = 0; i < m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_arrLineTo.size(); i++ )
				{
					nValue = m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_arrLineTo[i]->m_oX.ToTwips();
					oOutput.m_aWrapPoints.push_back( nValue );
					
					nValue =m_ooxAnchor->m_oWrapTight->m_oWrapPolygon->m_arrLineTo[i]->m_oY.ToTwips();
					oOutput.m_aWrapPoints.push_back( nValue );
				}
			}
		}
		if(m_ooxAnchor->m_oWrapTopAndBottom.IsInit())
			oOutput.m_nWrapType = 1;
		
		if( m_ooxAnchor->m_oGraphic.IsInit() )
		{
			OOXPictureGraphicReader oGraphicReader(m_ooxAnchor->m_oGraphic.GetPointer());
			oGraphicReader.Parse( oParam, *oOutput.m_oPicture );
		}
		//изменяем scale в соответсявии с выходным размером
		if( PROP_DEF != nWidth && PROP_DEF != oOutput.m_oPicture->m_nWidthGoal )
		{
			int nNewScale = (int)(100 * ( 1.0 * nWidth / oOutput.m_oPicture->m_nWidthGoal ));
			oOutput.m_oPicture->m_dScaleX = nNewScale;
		}
		if( PROP_DEF != nHeight && PROP_DEF != oOutput.m_oPicture->m_nHeightGoal )
		{
			int nNewScale = (int)(100 * ( 1.0 * nHeight / oOutput.m_oPicture->m_nHeightGoal ));
			oOutput.m_oPicture->m_dScaleY = nNewScale;
		}

		return true;
	}
};
