/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

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
#include "../Paragraph/CtrlField.h"

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
		// TODO:: Return to this later
		int nTempSize = nSize - oBuffer.GetDistanceToLastPos();
		oBuffer.Skip(nTempSize);
	}
	else if (L"cot%" == sCtrlID)
	{
		// TODO:: Return to this later
		// When content is read as UTF_16LE, the following content remains the same
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
	else if (L"klh%" == sCtrlID ||
	         L"kmb%" == sCtrlID)
	{
		pCtrl = new CCtrlField(sCtrlID, nSize - oBuffer.GetDistanceToLastPos(), oBuffer, 0, nVersion);
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
