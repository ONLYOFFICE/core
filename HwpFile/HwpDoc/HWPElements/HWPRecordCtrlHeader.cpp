#include "HWPRecordCtrlHeader.h"

#include "../Paragraph/CtrlAutoNumber.h"
#include "../Paragraph/CtrlClick.h"
#include "../Paragraph/CtrlColumnDef.h"
#include "../Paragraph/CtrlEqEdit.h"
#include "../Paragraph/CtrlForm.h"
#include "../Paragraph/CtrlGeneralShape.h"
#include "../Paragraph/CtrlHeadFoot.h"
#include "../Paragraph/CtrlNewNumber.h"
#include "../Paragraph/CtrlNote.h"
#include "../Paragraph/CtrlPageNumPos.h"
#include "../Paragraph/CtrlSectionDef.h"
#include "../Paragraph/CtrlTable.h"
#include "../Paragraph/CtrlEmpty.h"

namespace HWP
{
CHWPRecordCtrlHeader::CHWPRecordCtrlHeader(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize)
{}

CCtrl* CHWPRecordCtrlHeader::Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	HWP_STRING sCtrlID;
	oBuffer.ReadString(sCtrlID, 4, EStringCharacter::ASCII);

	CCtrl* pCtrl = nullptr;

	if (L"dces" == sCtrlID)
		pCtrl = new CCtrlSectionDef(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if (L"dloc" == sCtrlID)
		pCtrl = new CCtrlColumnDef(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if (L"daeh" == sCtrlID)
		pCtrl = new CCtrlHeadFoot(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion, true);
	else if (L"toof" == sCtrlID)
		pCtrl = new CCtrlHeadFoot(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion, false);
	else if (L"  nf" == sCtrlID || L"  ne" == sCtrlID)
		pCtrl = new CCtrlNote(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if (L" lbt" == sCtrlID)
		pCtrl = new CCtrlTable(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if (L"onta" == sCtrlID)
		pCtrl = new CCtrlAutoNumber(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if (L"onwn" == sCtrlID)
		pCtrl = new CCtrlNewNumber(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if (L" osg" == sCtrlID)
		pCtrl = new CCtrlGeneralShape(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if (L"deqe" == sCtrlID)
		pCtrl = new CCtrlEqEdit(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
	else if (L"dhgp" == sCtrlID)
	{
		int nTempSize = nSize - oBuffer.GetDistanceToLastPos();
		oBuffer.Skip(nTempSize);
	}
	else if (L"cot%" == sCtrlID)
	{
		// Когда содержимое считывается как UTF_16LE, следующее содержимое остается тем же самым
		// ¥TableOfContents:set:140:ContentsMake:uint:17 ContentsStyles:wstring:0: ContentsLevel:int:5 ContentsAutoTabRight:int:0 ContentsLeader:int:3 ContentsHyperlink:bool:1

		int nTempSize = nSize - oBuffer.GetDistanceToLastPos();
		oBuffer.Skip(nTempSize);
	}
	else if (L"klc%" == sCtrlID)
	{
		pCtrl = new CCtrlClick(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
		pCtrl->SetFullFilled();
	}
	else if (L"mrof" == sCtrlID)
	{
		pCtrl = new CCtrlForm(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
		pCtrl->SetFullFilled();
	}
	else if (L"pngp" == sCtrlID)
	{
		pCtrl = new CCtrlPageNumPos(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
		pCtrl->SetFullFilled();
	}
	// else if (L"klh%" == sCtrlID || // hyperlink
	//          L"frx%" == sCtrlID || // FIELD_CROSSREF
	//          L"knu%" == sCtrlID || // FIELD_UNKNOWN
	//          L"etd%" == sCtrlID || // FIELD_DATE
	//          L"tdd%" == sCtrlID || // FIELD_DOCDATE
	//          L"tap%" == sCtrlID || // FIELD_PATH
	//          L"kmb%" == sCtrlID || // FIELD_BOOKMARK
	//          L"gmm%" == sCtrlID || // FIELD_MAILMERGE
	//          L"umf%" == sCtrlID || // FIELD_FORMULA
	//          L"mxdi" == sCtrlID || // ???
	//          L"mkob" == sCtrlID || // ???
	//          L"spct" == sCtrlID || // ???
	//          L"tmct" == sCtrlID || // ???
	//          L"tcgp" == sCtrlID || // ???
	//          L"tudt" == sCtrlID)   // ???
	else
	{
		int nTempSize = nSize - oBuffer.GetDistanceToLastPos();
		pCtrl = new CCtrlEmpty(sCtrlID);
		pCtrl->SetFullFilled();
		oBuffer.Skip(nTempSize);
	}

	return pCtrl;
}
}
