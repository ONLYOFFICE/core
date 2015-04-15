#pragma once

#include "../RtfDocument.h"
#include "../RtfReader.h"

#include "OOXReader.h"

class ReaderParameter
{
public: 

	OOXReader*			oReader;
	OOX::CDocx*			oDocx;	
	RtfDocument*		oRtf;
	
	RtfTableStylePtr	poTableStyle;
};
