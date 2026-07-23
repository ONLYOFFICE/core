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

#include "HWPRecordCtrlData.h"
#include "../Paragraph/CtrlField.h"

namespace HWP
{

EParamItemType GetParamItemType(int nValue)
{
	switch(static_cast<EParamItemType>(nValue))
	{
		case EParamItemType::PIT_NULL:
		case EParamItemType::PIT_BSTR:
		case EParamItemType::PIT_I1:
		case EParamItemType::PIT_I2:
		case EParamItemType::PIT_I4:
		case EParamItemType::PIT_I:
		case EParamItemType::PIT_UI1:
		case EParamItemType::PIT_UI2:
		case EParamItemType::PIT_UI4:
		case EParamItemType::PIT_UI:
		case EParamItemType::PIT_SET:
		case EParamItemType::PIT_ARRAY:
		case EParamItemType::PIT_BINDATA:
			return static_cast<EParamItemType>(nValue);
		default:
			return EParamItemType::PIT_NULL;
	}
}

CHWPRecordCtrlData::CHWPRecordCtrlData(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize)
{}

int CHWPRecordCtrlData::ParseCtrl(CCtrl& oCtrl, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	// TODO:: Return to this later
	// The Hancom document content alone does not provide enough information for interpretation.

	oBuffer.SavePosition();

	if (ECtrlObjectType::Field == oCtrl.GetCtrlType())
	{
		short shSetID, shNumberItems;

		oBuffer.ReadShort(shSetID);
		oBuffer.ReadShort(shNumberItems);

		short shItemID, shItemType;
		for (unsigned short ushIndex = 0; ushIndex < shNumberItems; ++ushIndex)
		{
			oBuffer.ReadShort(shItemID);
			short shUnknownValue;
			oBuffer.ReadShort(shUnknownValue); // Unknown Data
			oBuffer.ReadShort(shItemType);

			switch (GetParamItemType(shItemType))
			{
				case EParamItemType::PIT_NULL: oBuffer.Skip(4); break;
				case EParamItemType::PIT_BSTR:
				{
					HWP_STRING sValue;
					oBuffer.ReadString(sValue, EStringCharacter::UTF16);

					if (EFieldType::Bookmark == ((CCtrlField&)oCtrl).GetType())
						((CCtrlField&)oCtrl).AddStringParam(L"bookmarkname", sValue);

					break;
				}
				case EParamItemType::PIT_I1: oBuffer.Skip(1); break;
				case EParamItemType::PIT_I2: oBuffer.Skip(2); break;
				case EParamItemType::PIT_I4: oBuffer.Skip(4); break;
				case EParamItemType::PIT_I:  oBuffer.Skip(4); break;
				case EParamItemType::PIT_UI1: oBuffer.Skip(1); break;
				case EParamItemType::PIT_UI2: oBuffer.Skip(2); break;
				case EParamItemType::PIT_UI4: oBuffer.Skip(4); break;
				case EParamItemType::PIT_UI: oBuffer.Skip(4); break;
				case EParamItemType::PIT_SET:
				case EParamItemType::PIT_ARRAY:
				case EParamItemType::PIT_BINDATA:
					break;
			}
		}
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}
}
