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

namespace XLS
{

class StreamCacheReader;

class BOF: public BiffRecord
{
	//BIFF_RECORD_DEFINE_TYPE_INFO(BOF)
	BASE_OBJECT_DEFINE_CLASS_NAME(BOF)
public:
	BOF();
	~BOF();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);
    void writeFields(CFRecord& record);

	static const ElementType type = typeBOF;

	unsigned short getSubstreamType();

	const CFRecordType::TypeId getTypeId() const
	{
		return type_id_;
	}

	typedef enum
	{
		st_Workbook		= 0x0005,
		st_Globals		= st_Workbook,
		st_Dialog		= 0x0010,
		st_Worksheet	= st_Dialog,
		st_Chart		= 0x0020,
		st_Macro		= 0x040
	} SubstreamType;


//-----------------------------
    _UINT16 vers = 0x0600;
	_UINT16 dt = 0;
	_UINT16 rupBuild = 0x4F5A;
	_UINT16 rupYear = 0x07CD;
	
    bool fWin = 1;
    bool fRisc = 0;
    bool fBeta = 0;
    bool fWinAny = 1;
    bool fMacAny = 0;
    bool fBetaAny = 0;
    bool fRiscAny = 0;
    bool fOOM = 0;
    bool fGlJmp = 0;
    bool fFontLimit = 0;
	
	_UINT16 verXLHigh = 0x8;
	unsigned char	verLowestBiff = 0x6;
	unsigned char	verLastXLSaved = 0x8;
	
	_CP_OPT(unsigned int) stream_ptr;

	CFRecordType::TypeId type_id_ = rt_BOF_BIFF8;	
};

} // namespace XLS

