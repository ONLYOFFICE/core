#pragma once

#include "../../../../Common/DocxFormat/Source/DocxFormat/Logic/Table.h"
#include "../RtfTable.h"

class OOXTableReader
{
private:
	OOX::Logic::CTbl *m_ooxTable;
public:
	OOXTableReader(OOX::Logic::CTbl *ooxTable)
	{
		m_ooxTable = ooxTable;
	}
	bool Parse(  ReaderParameter oParam ,RtfTable & oOutputTable  );
	void ApplyParagraphProperty( RtfTable& oOutputTable );
};