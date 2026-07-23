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
#pragma once

#include "BiffRecord.h"
#include "../Biff_structures/Xnum.h"

namespace XLS
{

class Setup: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Setup)
	BASE_OBJECT_DEFINE_CLASS_NAME(Setup)
public:
	Setup(bool isChart = false);
	~Setup();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record) override;
	void writeFields(CFRecord& record) override;

	static const ElementType type = typeSetup;

	_UINT32 iPaperSize = 9;
	_UINT32 iScale = 100;
	_INT32 iPageStart = 1;
	_UINT32 iFitWidth = 1;
	_UINT32 iFitHeight = 1;

	bool fLeftToRight = false;
	bool fPortrait = true;
	bool fNoPls = true;
	bool fNoColor = false;
	bool fDraft = false;
	bool fNotes = false;
	bool fNoOrient = false;
	bool fUsePage = true;
	bool fEndNotes = false;
	unsigned char iErrors = 0;

	_UINT32 iRes = 300;
	_UINT32 iVRes = 300;

	Xnum	numHdr;
	Xnum	numFtr;
	_UINT32 iCopies = 1;

	std::wstring szRelID = L""; // biff12
	bool fLandscape = false; // biff12

	bool _isChart = false;
};

} // namespace XLS

