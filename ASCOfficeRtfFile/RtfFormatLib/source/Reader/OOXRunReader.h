#pragma once

#include "OOXrPrReader.h"
#include "OOXShapeReader.h"
#include "OOXPictureReader.h"

#include "../RtfDocument.h"
#include "../RtfProperty.h"
#include "../RtfField.h"
#include "../RtfBookmark.h"
#include "../RtfOle.h"

class OOXRunReader
{
private:
	OOX::Logic::CRun *m_ooxRun;
public:
	OOXRunReader(OOX::Logic::CRun *ooxRun)
	{
		m_ooxRun = ooxRun;
	}

	bool Parse( ReaderParameter oParam , RtfParagraph& oOutputParagraph, RtfStylePtr poStyle );
};