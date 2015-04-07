#pragma once
#include "../RtfPicture.h"
#include "OOXPictureAnchorReader.h"
#include "OOXPictureInlineReader.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Drawing/Drawing.h"

class OOXPictureReader
{
private:
	OOX::Logic::CDrawing* m_ooxDrawing;
public: 
	OOXPictureReader(OOX::Logic::CDrawing* ooxDrawing)
	{
		m_ooxDrawing = ooxDrawing;
	}
 
	bool Parse( ReaderParameter oParam , RtfShape& oOutput)
	{
		if (m_ooxDrawing == NULL) return false;
		//if( oParam.oReader->m_nCurItap != 0 )
		//	oOutput.m_bLayoutInCell = 1;

		if (m_ooxDrawing->m_oInline.IsInit())
		{
			OOXPictureInlineReader oPictureInlineReader(m_ooxDrawing->m_oInline.GetPointer());
			return oPictureInlineReader.Parse( oParam, oOutput );
		}

		if (m_ooxDrawing->m_oAnchor.IsInit())
		{
			OOXPictureAnchorReader oPictureAnchorReader(m_ooxDrawing->m_oAnchor.GetPointer());
			return oPictureAnchorReader.Parse( oParam, oOutput );
		}

		return false;
	}
};
