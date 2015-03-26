#include "MathEquation.h"
#include "OutputDev.h"

using namespace MathEquation;

#define CHECK_OPEN_BRACKET(nVaritation)  ((0 == nVariation) || (1 == nVariation))
#define CHECK_CLOSE_BRACKET(nVaritation) ((0 == nVariation) || (2 == nVariation))
#define CHECK_SUP(nVariation) ((0 == nVariation) || (2 == nVariation))
#define CHECK_SUB(nVariation) ((1 == nVariation) || (2 == nVariation))
#define GET_BARLINE_TYPE(nVariation)  (0 == nVariation? bartypeLine : bartypeDoubleLine)
#define GET_BARARROW_TYPE(nVariation) (0 == nVariation? bartypeArrowLeft : (1 == nVariation? bartypeArrowRight : bartypeArrowDouble))

void CEquationReader::InitSizes()
{
	// Выставляем размеры текста по умолчанию (если они изменены, тогда ничего не поделаешь, т.к. это не сохраняется в самом файле)
	aSizeTable[0] = 12;
	aSizeTable[1] = 7;
	aSizeTable[2] = 5;
	aSizeTable[3] = 18;
	aSizeTable[4] = 12;
}

void CEquationReader::InitFonts()
{
	// Стандартные шрифты для MathEquation со стандартными настройками стилей. 
	// (если они изменены, тогда ничего не поделаешь, т.к. это не сохраняется в самом файле)

	if (pOutputDev)
	{
		for(uint8_t i = 1; i <= 8; i++)
		{
			switch(i)
			{
			case 1: pOutputDev->AddFont(i + 128, "Times New Roman", false, false); break; // текст
			case 2: pOutputDev->AddFont(i + 128, "Times New Roman", false, false); break; // функция
			case 3: pOutputDev->AddFont(i + 128, "Times New Roman", false, true);  break; // переменная
			case 4: pOutputDev->AddFont(i + 128, "Symbol",          false, true);  break; // ст. греческие
			case 5: pOutputDev->AddFont(i + 128, "Symbol",          false, false); break; // пр. греческие
			case 6: pOutputDev->AddFont(i + 128, "Symbol",          false, false); break; // символ
			case 7: pOutputDev->AddFont(i + 128, "Times New Roman", true, false);  break; // матрица-вектор
			case 8: pOutputDev->AddFont(i + 128, "Times New Roman", false, false); break; // числа
			}
		}
	}
}

int CEquationReader::Parse()
{
	pS = new CLEStream<Stream>(m_oStorage.stream("Equation Native"));
	if (!pS->IsValid())
		return 0;

	*pS >> oHeader.nCBHdr;     // length of header, sizeof(CEquationHeader) = 28
	*pS >> oHeader.nVersion;   // hiword = 2, loword = 0
	*pS >> oHeader.nCf;        // clipboard format ("MathType EF")
	*pS >> oHeader.nCBObject;  // length of MTEF data following this header
	*pS >> oHeader.nReserved1; // not used
	*pS >> oHeader.nReserved2; // not used
	*pS >> oHeader.nReserved3; // not used
	*pS >> oHeader.nReserved4; // not used

	*pS >> nVersion;
	*pS >> nPlatform;
	*pS >> nProduct;
	*pS >> nProdVersion;
	*pS >> nProdSubVersion;

	if (nVersion > 3)
		return 0;

	pOutputDev->BeginEquation();
	int nRet = HandleRecords();
	pOutputDev->EndEquation();

	return nRet;
}

int CEquationReader::HandleRecords()
{
	uint8_t nTag, nRecord;
	uint16_t nTabOffset;

	int nRet = 1;

	int nCurRow = 0;
	int nCurCol = 0;
	int nPart   = 0;

	do
	{
		*pS >> nTag;
		nRecord = nTag & 0x0F;

		switch(nRecord)
		{
			case LINE:		

				if (xfLMOVE(nTag))
					HandleNudge();
				//if (xfLSPACE(nTag))
				//if (xfRULER(nTag))

				if (!(xfNULL(nTag)))
				{								
					pOutputDev->BeginBlock();
					nRet = HandleRecords();
					pOutputDev->EndBlock();
				}
				break;

			case CHAR:

                if (xfLMOVE(nTag))
                    HandleNudge();
				nRet = HandleChar(nTag);
 				break;

			case TMPL:

                if (xfLMOVE(nTag))
                    HandleNudge();
				nRet = HandleTemplate();
				break;

			case PILE:

                if (xfLMOVE(nTag))
                    HandleNudge();
				nRet = HandlePile();
				break;

			case MATRIX:

                if (xfLMOVE(nTag))
                    HandleNudge();
				nRet = HandleMatrix();
				break;

			case EMBEL:

                if (xfLMOVE(nTag))
                    HandleNudge();
				HandleEmblishments();
				break;

			case RULER:

				HandleRuler();
				break;

			case FONT:

				HandleFont();
				break;

			case SIZE_CUSTOM:
			case SIZE_REGULAR:
			case SIZE_BIGSCRIPT:
			case SIZE_SMALLSCRIT:
			case SIZE_BIGSYMBOL:
			case SIZE_SMALLSYMBOL:

				HandleSetSize((MTOKENS)nRecord);
				break;

			case END:
			default:
				break;
		}
	}
    while (nRecord != END && !pS->IsEof());
	return nRet;
}

void CEquationReader::HandleNudge()
{
    uint8_t nXNudge; *pS >> nXNudge;
	uint8_t nYNudge; *pS >> nYNudge;

	if (128 == nXNudge && 128 == nYNudge)
    {
		uint16_t nXLongNudge; *pS >> nXLongNudge;
		uint16_t nYLongNudge; *pS >> nYLongNudge;
    }
}
int  CEquationReader::HandleChar(uint8_t nTag)
{
	Unicode_t nChar;
	int nRet = 1;

	if (xfAUTO(nTag))
	{
		//This is a candidate for function recognition, whatever
		//that is!
	}

	uint8_t nTypeFace;
	*pS >> nTypeFace;

	if (nVersion < 3)
	{
		uint8_t nChar8;
		*pS >> nChar8;
		nChar = nChar8;
	}
	else
		*pS >> nChar;

    // bad character, old mathtype < 3 has these
    if (nChar < 0x20)
        return nRet;

	pOutputDev->BeginChar(nChar, nTypeFace, IsSpecialChar(nChar));	

	if (xfEMBELL(nTag))
	{
		nRet = HandleRecords();
	}	

	pOutputDev->EndChar();
	return nRet;
}
int CEquationReader::HandleTemplate()
{
	uint8_t nSelector, nVariation, nOption;
	*pS >> nSelector;
	*pS >> nVariation;
	*pS >> nOption;

	HandleStartTemplate(nSelector, nVariation);
    int nRet = HandleRecords();
	HandleEndTemplate(nSelector, nVariation);
	return nRet;
}

void CEquationReader::HandleStartTemplate(uint8_t nSelector, uint8_t nVariation)
{
	switch (nSelector)
	{
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
	case 0x08:
	case 0x09:
	case 0x0a:
	case 0x0b:
	case 0x0c:

		pOutputDev->BeginBrackets((MBRACKETSTYPE)nSelector, CHECK_OPEN_BRACKET(nVariation), CHECK_CLOSE_BRACKET(nVariation));
		break;

	case 0xd:

		pOutputDev->BeginRoot(0 == nVariation ? false : true);
		break;

	case 0xe:

		pOutputDev->BeginFraction(fractionRegular, 1 == nVariation);
		break;

	case 0xf:

		pOutputDev->BeginScript(scriptalignRight, false, 0 == nVariation || 2 == nVariation, 1 == nVariation || 2 == nVariation, true);
		break;

	case 0x10:

		pOutputDev->BeginBar(GET_BARLINE_TYPE(nVariation), false);
		break;

	case 0x11:

		pOutputDev->BeginBar(GET_BARLINE_TYPE(nVariation), true);
		break;

	case 0x12:

		pOutputDev->BeginArrow(arrowtypeLeft, 0 == nVariation ? true : false);
		break;

	case 0x13:
		pOutputDev->BeginArrow(arrowtypeRight, 0 == nVariation ? true : false);
		break;

	case 0x14:

		pOutputDev->BeginArrow(arrowtypeDouble, 0 == nVariation ? true : false);
		break;

	case 0x15:

		pOutputDev->BeginIntegral((MINTEGRALTYPE)(nVariation));
		break;

	case 0x16:

		pOutputDev->BeginIntegral((MINTEGRALTYPE)(nVariation + (BYTE)integraltypeDouble));
		break;

	case 0x17:

		pOutputDev->BeginIntegral((MINTEGRALTYPE)(nVariation + (BYTE)integraltypeTriple));
		break;

	case 0x18:

		pOutputDev->BeginIntegral((MINTEGRALTYPE)(nVariation + (BYTE)integraltypeSingleCSubSup));
		break;

	case 0x19:

		pOutputDev->BeginIntegral((MINTEGRALTYPE)(nVariation + (BYTE)integraltypeDoubleOrientedCSub));
		break;

	case 0x1a:

		pOutputDev->BeginIntegral((MINTEGRALTYPE)(nVariation + (BYTE)integraltypeTripleOrientedCSub));
		break;

	case 0x1b:

		pOutputDev->BeginVerticalBrace(true);
		break;

	case 0x1c:

		pOutputDev->BeginVerticalBrace(false);
		break;

	case 0x1d:

		pOutputDev->BeingNArray((MNARRAYTYPE)nVariation);
		break;

	case 0x1e:

		pOutputDev->BeingNArray((MNARRAYTYPE)(nVariation + narySumRSub));
		break;

	case 0x1f:

		pOutputDev->BeingNArray((MNARRAYTYPE)(nVariation + naryProdCSub));
		break;

	case 0x20:

		pOutputDev->BeingNArray((MNARRAYTYPE)(nVariation + naryProdRSub));
		break;

	case 0x21:

		pOutputDev->BeingNArray((MNARRAYTYPE)(nVariation + naryCoProdCSub));
		break;

	case 0x22:

		pOutputDev->BeingNArray((MNARRAYTYPE)(nVariation + naryCoProdRSub));
		break;

	case 0x23:

		pOutputDev->BeingNArray((MNARRAYTYPE)(nVariation + naryUnionCSub));
		break;

	case 0x24:

		pOutputDev->BeingNArray((MNARRAYTYPE)(nVariation + naryUnionRSub));
		break;

	case 0x25:

		pOutputDev->BeingNArray((MNARRAYTYPE)(nVariation + naryIntersectCSub));
		break;

	case 0x26:

		pOutputDev->BeingNArray((MNARRAYTYPE)(nVariation + naryIntersectRSub));
		break;

	case 0x27:

		pOutputDev->BeginScript(scriptalignCenter, true, CHECK_SUP(nVariation), CHECK_SUB(nVariation), true);
		break;

	case 0x28:

		pOutputDev->BeginLongDivision((MLONGDIVISION)nVariation);		
		break;

	case 0x29:

		pOutputDev->BeginFraction(fractionSlanted, 1 == nVariation);
		break;

	case 0x2a:

		pOutputDev->BeginScript(scriptalignRight, true, CHECK_SUP(nVariation), CHECK_SUB(nVariation), false);
		break;

	case 0x2b:

		pOutputDev->BeginScript(scriptalignCenter, true, CHECK_SUP(nVariation), CHECK_SUB(nVariation), false);
		break;

	case 0x2c:

		pOutputDev->BeginScript(scriptalignLeft, false, 0 == nVariation || 2 == nVariation, 1 == nVariation || 2 == nVariation, true);
		break;

	case 0x2d:

		pOutputDev->BeginAngleBracketsWithSeparator((MANGLEBRACKETSWITHSEPARATORTYPE)nVariation);
		break;

	case 0x2e:

		pOutputDev->BeginBar(GET_BARARROW_TYPE(nVariation), false);
		break;

	case 0x2f:

		pOutputDev->BeginBar(GET_BARARROW_TYPE(nVariation), true);
		break;

	default:
		break;
	}
}
void CEquationReader::HandleEndTemplate(uint8_t nSelector, uint8_t nVariation)
{
	switch (nSelector)
	{
	case 0x0:
	case 0x1:
	case 0x2:
	case 0x3:
	case 0x4:
	case 0x5:
	case 0x6:
	case 0x7:
	case 0x8:
	case 0x9:
	case 0xa:
	case 0xb:
	case 0xc:

		pOutputDev->EndBrackets((MBRACKETSTYPE)nSelector, CHECK_OPEN_BRACKET(nVariation), CHECK_CLOSE_BRACKET(nVariation));
		break;

	case 0xd:

		pOutputDev->EndRoot();
		break;

	case 0xe:

		pOutputDev->EndFraction();
		break;

	case 0xf:

		pOutputDev->EndScript();
		break;

	case 0x2c:

		pOutputDev->EndScript();
		break;

	case 0x2e:
	case 0x2f:

		pOutputDev->EndBar();
		break;

	case 0x10:
	case 0x11:

		pOutputDev->EndBar();
		break;

	case 0x12:
	case 0x13:
	case 0x14:

		pOutputDev->EndArrow();
		break;

	case 0x1b:

		pOutputDev->EndVerticalBrace();
		break;

	case 0x1c:

		pOutputDev->EndVerticalBrace();
		break;

	case 0x27:

		pOutputDev->EndScript();
		break;

	case 0x28:

		pOutputDev->EndLongDivision();
		break;

	case 0x29:

		pOutputDev->EndFraction();
		break;

	case 0x1d:
	case 0x1e:
	case 0x1f:
	case 0x20:
	case 0x21:
	case 0x22:
	case 0x23:
	case 0x24:
	case 0x25:
	case 0x26:

		pOutputDev->EndNArray();
		break;							

	case 0x15:
	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
	case 0x1a:

		pOutputDev->EndIntegral();
		break;

	case 0x2a:
	case 0x2b:

		pOutputDev->EndScript();
		break;

	case 0x2d:

		pOutputDev->EndAngleBracketsWithSeparator();
		break;

	default:

		break;
	}
}
int CEquationReader::HandlePile()
{
	int nSetAlign = 0;
	*pS >> nHAlign;
	*pS >> nVAlign;

	pOutputDev->StartPile(nHAlign, nVAlign);
	int nRet = HandleRecords();
	pOutputDev->EndPile();

	return nRet;
}

int CEquationReader::HandleMatrix()
{
	uint8_t nHorAlign, nVerAlign, nRows, nCols;
	*pS >> nVAlign;
	*pS >> nHorAlign;
	*pS >> nVerAlign;
	*pS >> nRows;
	*pS >> nCols;

	bool bEqualRows = nHorAlign & 0xf0;
	nHorAlign &= 0xf;

	bool bEqualCols = nVerAlign & 0xf0;
	nVerAlign &= 0xf;

	int nBytes = ((nRows + 1) * 2) / 8;
	if (((nRows + 1) * 2) % 8)
		nBytes++;

	uint8_t *pVerBorders = new uint8_t[nRows + 1];
	uint8_t nTempByte;
	for (int nByteIndex = 0, nRowIndex = 0, nBitIndex = 0; nByteIndex < nBytes; nByteIndex++, nBitIndex = 0)
	{
		*pS >> nTempByte;

		while(nRowIndex < nRows + 1 && nBitIndex < 8)
		{
			pVerBorders[nRowIndex] = (nTempByte >> nBitIndex) & 0x03;
			nBitIndex += 2;
			nRowIndex++;
		}
	}

	nBytes = ((nCols + 1) * 2) / 8;
	if (((nCols + 1) * 2) % 8)
		nBytes++;
	
	uint8_t *pHorBorders = new uint8_t[nCols + 1];
	for (int nByteIndex = 0, nColIndex = 0, nBitIndex = 0; nByteIndex < nBytes; nByteIndex++, nBitIndex = 0)
	{
		*pS >> nTempByte;

		while(nColIndex < nCols + 1 && nBitIndex < 8)
		{
			pHorBorders[nColIndex] = (nTempByte >> nBitIndex) & 0x03;
			nBitIndex += 2;
			nColIndex++;
		}
	}

	pOutputDev->BeginMatrix(nVAlign, (MMATRIXHORALIGN)nHorAlign, (MMATRIXVERALIGN)nVerAlign, bEqualRows, bEqualCols, nRows, nCols, pVerBorders, pHorBorders);

	delete []pVerBorders;
	delete []pHorBorders;

	int nRet = HandleRecords();

	pOutputDev->EndMatrix();

	return nRet;
}

void CEquationReader::HandleEmblishments()
{
	uint8_t nEmbel;
	do
	{
		*pS >> nEmbel;

		if (nEmbel >= embelMin && nEmbel <= embelMax)
			pOutputDev->AddCharEmbel((MEMBELTYPE)nEmbel);

        if (nVersion < 3)
            break;
	}while (nEmbel);
}
void CEquationReader::HandleSetSize(MTOKENS eType)
{
	if (SIZE_CUSTOM == eType)
	{
		uint8_t nTemp;
		*pS >> nTemp;
		uint16_t nSize;
		switch (nTemp)
		{
		case 101:
			{
				*pS >> nSize;
				break;
			}
		case 100:
			{
				// TODO: Проверить эту ветку
				*pS >> nTemp;
				nSize = nTemp;

				uint16_t nTempSize;
				*pS >> nTempSize;
				break;
			}
		default:
			{
				// TODO: Проверить эту ветку
				nSize = nTemp;
				*pS >> nTemp;
				uint16_t nTempSize = nTemp - 128;
				break;
			}
		}

		pOutputDev->SetSize(nSize / 32);
	}
	else if (SIZE_BIGSCRIPT == eType || SIZE_BIGSYMBOL == eType || SIZE_REGULAR == eType || SIZE_SMALLSCRIT == eType || SIZE_SMALLSYMBOL == eType)
	{
		int nIndex = (int)eType - (int)SIZE_REGULAR;
		pOutputDev->SetSize(aSizeTable[nIndex]);
	}
}

void CEquationReader::HandleRuler()
{
	uint8_t nTabType, nTabStops;
	uint16_t nTabOffset;
	*pS >> nTabStops;

	for (int nTabIndex = 0; nTabIndex < nTabStops; nTabIndex++)
	{
		*pS >> nTabType;
		*pS >> nTabOffset;
	}
}
void CEquationReader::HandleFont()
{
	uint8_t nTFace, nStyle;
	MathString sName;

	*pS >> nTFace;
	*pS >> nStyle;

	Char_t nChar8;
	do
	{
		*pS >> nChar8;
		sName.Append(nChar8);
	}
	while(nChar8);

	pOutputDev->AddFont(128 - nTFace, sName.GetValue(), nStyle & 0x01, nStyle & 0x02);
}

bool CEquationReader::IsSpecialChar(Unicode_t nChar)
{
	switch(nChar)
	{
	case 0xeb00: // Alignment Symbol
	case 0xeb01: // Zerospace
	case 0xeb02: // thin space
	case 0xeb04: // thick space
	case 0xeb05: // large space
	case 0xeb08: // 1pt space
		return true;
	}

	return false;
}