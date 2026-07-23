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

#ifndef HWPRECORDCTRLDATA_H
#define HWPRECORDCTRLDATA_H

#include "../HWPStream.h"
#include "HWPRecord.h"
#include "../Paragraph/Ctrl.h"

namespace HWP
{
enum class EParamItemType
{
	PIT_NULL,
	PIT_BSTR,
	PIT_I1,
	PIT_I2,
	PIT_I4,
	PIT_I,
	PIT_UI1,
	PIT_UI2,
	PIT_UI4,
	PIT_UI,
	PIT_SET = 0x8000,
	PIT_ARRAY = 0x8001,
	PIT_BINDATA = 0x8002
};

struct TParameterItem
{
	short m_shItemId;
	EParamItemType m_eItemType;
	CHWPStream m_oItemData;
};

struct TParameterSet
{
	short m_shParamSetId;
	short m_shNItems;
	LIST<TParameterItem> m_arItems;
};

class CHWPRecordCtrlData : public CHWPRecord
{
	LIST<TParameterSet> m_arParamSets;
public:
	CHWPRecordCtrlData(int nTagNum, int nLevel, int nSize);

	static int ParseCtrl(CCtrl& oCtrl, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDCTRLDATA_H
