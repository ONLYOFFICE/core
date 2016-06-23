#pragma once

#include "../RtfProperty.h"
#include "OOXReaderBasic.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Logic/TableProperty.h"

struct CtblLook
{
	bool bFirstRow;
	bool bLastRow;
	bool bFirstCol;
	bool bLastCol;
	bool bNoVBand;
	bool bNoHBand;

	CtblLook()
	{
		bFirstRow = false;
		bLastRow = false;
		bFirstCol = false;
		bLastCol = false;
		bNoVBand = false;
		bNoHBand = false;
	}
};

class OOXtblLookReader
{
private:
	ComplexTypes::Word::CTblLook *m_ooxTblLook;
public:
	OOXtblLookReader(ComplexTypes::Word::CTblLook *ooxTblLook)
	{
		m_ooxTblLook = ooxTblLook;
	}
	bool Parse( ReaderParameter oParam, CtblLook& oOutput)
	{
		if (m_ooxTblLook == NULL) return false;

		oOutput.bLastRow = m_ooxTblLook->IsFirstRow();
		oOutput.bLastRow = m_ooxTblLook->IsLastRow();
		oOutput.bFirstCol = m_ooxTblLook->IsFirstColumn();
		oOutput.bLastCol = m_ooxTblLook->IsLastColumn();
		oOutput.bNoHBand = m_ooxTblLook->IsNoHBand();
		oOutput.bNoVBand = m_ooxTblLook->IsNoVBand();

		return true;
	}
};
