#include "HWPRecordCtrlHeader.h"

namespace HWP
{
CHWPRecordCtrlHeader::CHWPRecordCtrlHeader(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize)
{}

CCtrl* CHWPRecordCtrlHeader::Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	STRING sCtrlID;
	oBuffer.ReadString(sCtrlID, 4); //TODO::StandardCharsets.US_ASCII

	CCtrl* pCtrl = nullptr;

	if ("dces" == sCtrlID)
		pCtrl = new CCtrlSectionDef(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if ("dloc" == sCtrlID)
		pCtrl = new CCtrlColumnDef(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if ("daeh" == sCtrlID)
		pCtrl = new CCtrlHeadFoot(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion, true);
	else if ("toof" == sCtrlID)
		pCtrl = new CCtrlHeadFoot(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion, false);
	else if ("  nf" == sCtrlID || "  ne" == sCtrlID)
		pCtrl = new CCtrlNote(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if (" lbt" == sCtrlID)
		pCtrl = new CCtrlTalbe(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if ("onta" == sCtrlID)
		pCtrl = new CCtrlAutoNumber(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if ("onwn" == sCtrlID)
		pCtrl = new CCtrlNewNumber(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if (" osg" == sCtrlID)
		pCtrl = new CCtrlGeneralShape(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if ("deqe" == sCtrlID)
		pCtrl = new CCtrlEqEdit(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if ("dhgp" == sCtrlID)
	{
		int nTempSize = nSize - oBuffer.GetDistanceToLastPos();
		oBuffer.Skip(nTempSize);
	}
	else if ("cot%" == sCtrlID)
	{
		// Когда содержимое считывается как UTF_16LE, следующее содержимое остается тем же самым
		// ¥TableOfContents:set:140:ContentsMake:uint:17 ContentsStyles:wstring:0: ContentsLevel:int:5 ContentsAutoTabRight:int:0 ContentsLeader:int:3 ContentsHyperlink:bool:1

		int nTempSize = nSize - oBuffer.GetDistanceToLastPos();
		oBuffer.Skip(nTempSize);
	}
	else if ("klc%" == sCtrlID)
	{
		pCtrl = new CCtrlClick(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
		pCtrl->SetFullFilled();
	}
	else if ("mrof" == sCtrlID)
	{
		pCtrl = new CCtrlForm(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
		pCtrl->SetFullFilled();
	}
	else if ("pngp" == sCtrlID)
	{
		pCtrl = new CCtrlPageNumPos(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
		pCtrl->SetFullFilled();
	}
	else if ("klh%" == sCtrlID || // hyperlink
	         "frx%" == sCtrlID || // FIELD_CROSSREF
	         "knu%" == sCtrlID || // FIELD_UNKNOWN
	         "etd%" == sCtrlID || // FIELD_DATE
	         "tdd%" == sCtrlID || // FIELD_DOCDATE
	         "tap%" == sCtrlID || // FIELD_PATH
	         "kmb%" == sCtrlID || // FIELD_BOOKMARK
	         "gmm%" == sCtrlID || // FIELD_MAILMERGE
	         "umf%" == sCtrlID || // FIELD_FORMULA
	         "mxdi" == sCtrlID || // ???
	         "mkob" == sCtrlID || // ???
	         "spct" == sCtrlID || // ???
	         "tmct" == sCtrlID || // ???
	         "tcgp" == sCtrlID || // ???
	         "tudt" == sCtrlID)   // ???
	{
		int nTempSize = nSize - oBuffer.GetDistanceToLastPos();
		oBuffer.Skip(nTempSize);
	}

	return pCtrl;
}
}
