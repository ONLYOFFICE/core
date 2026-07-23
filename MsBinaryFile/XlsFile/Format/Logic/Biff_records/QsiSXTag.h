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
#include "../Biff_structures/BiffString.h"
#include "../Biff_structures/FrtHeaderOld.h"

namespace XLS
{

class QsiSXTag: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(QsiSXTag)
	BASE_OBJECT_DEFINE_CLASS_NAME(QsiSXTag)
public:
	QsiSXTag();
	~QsiSXTag();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);
	void writeFields(CFRecord& record);

	static const ElementType type = typeQsiSXTag;

	FrtHeaderOld	frtHeaderOld;
	unsigned short	fSx = 1;

	bool			fEnableRefresh = true;
	bool			fInvalid = false;
	bool			fTensorEx = false;
	
	_UINT32			dwQsiFuture = 0;
	unsigned char	verSxLastUpdated = 1;
	unsigned char	verSxUpdatableMin = 0;

	unsigned char	obCchName = 0x10;
	XLUnicodeString	stName;

	_CP_OPT(bool)	fNoStencil;
	_CP_OPT(bool)	fHideTotAnnotation;
	_CP_OPT(bool)	fIncludeEmptyRw;	
	_CP_OPT(bool)	fIncludeEmptyCol;
	_CP_OPT(bool)	fPreserveFmt;
	_CP_OPT(bool)	fAutoFit;
	_CP_OPT(bool)	fExtDataList;
	_CP_OPT(bool)	fCreateQTList;
	_CP_OPT(bool)	fDummyList;
};

} // namespace XLS

