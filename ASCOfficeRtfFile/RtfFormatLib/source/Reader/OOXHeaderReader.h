#pragma once
#include "../RtfDocument.h"

#include "OOXReader.h"
#include "OOXReaderBasic.h"

class OOXHeaderReader
{
private:
	ComplexTypes::Word::CHdrFtrRef* m_ref;
public: 
	OOXHeaderReader(ComplexTypes::Word::CHdrFtrRef* ref)
	{
		m_ref = ref;
	}
	bool Parse(ReaderParameter oParam, CString sRID, TextItemContainerPtr& oOutput, bool bHeader );
};
