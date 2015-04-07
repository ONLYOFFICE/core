#pragma once
#include "OOXPictureGraphicReader.h"

class OOXPictureInlineReader
{
private:
	OOX::Drawing::CInline *m_ooxInline;
public: 
	OOXPictureInlineReader(OOX::Drawing::CInline *ooxInline)
	{
		m_ooxInline = ooxInline;
	}
	bool Parse( ReaderParameter oParam, RtfShape& oOutput)
	{
		if (m_ooxInline == NULL) return false;

		oOutput.m_oPicture = RtfPicturePtr( new RtfPicture() );
		oOutput.m_eShapeType = RtfShape::st_inline;
		oOutput.m_nShapeType = 75;
		oOutput.m_nLeft = 0;
		oOutput.m_nTop = 0;
		oOutput.m_nPositionHRelative = 3;
		oOutput.m_nPositionVRelative = 3;
		
		int nDistLeft	= m_ooxInline->m_oDistL.IsInit() ? m_ooxInline->m_oDistL->ToTwips() : PROP_DEF;
		int nDistTop	= m_ooxInline->m_oDistT.IsInit() ? m_ooxInline->m_oDistT->ToTwips() : PROP_DEF;
		int nDistRight	= m_ooxInline->m_oDistR.IsInit() ? m_ooxInline->m_oDistR->ToTwips() : PROP_DEF;
		int nDistBottom = m_ooxInline->m_oDistB.IsInit() ? m_ooxInline->m_oDistB->ToTwips() : PROP_DEF;
		
		int nWidth = PROP_DEF;
		int nHeight = PROP_DEF;

		if( m_ooxInline->m_oExtent.IsInit() )
		{
			nWidth = m_ooxInline->m_oExtent->m_oCx.GetValue();
			nHeight = m_ooxInline->m_oExtent->m_oCy.GetValue();

			if( PROP_DEF != nWidth && PROP_DEF != nHeight )
			{
				nWidth = RtfUtility::Emu2Twips( nWidth );
				nHeight = RtfUtility::Emu2Twips( nHeight );
				if( PROP_DEF != oOutput.m_nLeft && PROP_DEF != oOutput.m_nTop )
				{
					oOutput.m_nRight = oOutput.m_nLeft + nWidth;
					oOutput.m_nBottom = oOutput.m_nTop + nHeight;
				}
				oOutput.m_oPicture->m_nWidthGoal = nWidth;
				oOutput.m_oPicture->m_nHeightGoal = nHeight;
			}
		}
		
		if(m_ooxInline->m_oGraphic.IsInit())
		{
			OOXPictureGraphicReader oGraphicReader(m_ooxInline->m_oGraphic.GetPointer());
			oGraphicReader.Parse( oParam, *oOutput.m_oPicture );
		}
		//изменяем scale в соответсявии с выходным размером
		if( PROP_DEF != nWidth && PROP_DEF != oOutput.m_oPicture->m_nWidthGoal )
		{
			double dNewScale = 100 * ( 1.0 * nWidth / oOutput.m_oPicture->m_nWidthGoal );
			oOutput.m_oPicture->m_dScaleX = dNewScale;
		}
		if( PROP_DEF != nHeight && PROP_DEF != oOutput.m_oPicture->m_nHeightGoal )
		{
			double dNewScale = 100 * ( 1.0 * nHeight / oOutput.m_oPicture->m_nHeightGoal );
			oOutput.m_oPicture->m_dScaleY = dNewScale;
		}
		return true;
	}
};
