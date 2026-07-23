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
#include "../Biff_structures/LongRGB.h"

namespace XLS
{


// Logical representation of AreaFormat record in BIFF8
class AreaFormat: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(AreaFormat)
	BASE_OBJECT_DEFINE_CLASS_NAME(AreaFormat)
public:
	AreaFormat();
	~AreaFormat();

	BaseObjectPtr clone();

	
	void readFields(CFRecord& record);
	void writeFields(CFRecord& record);
		
	static const ElementType	type = typeAreaFormat;

	int serialize(std::wostream & _stream);
	void setChecksum(std::vector<BYTE> &ChecksumData);

	LongRGB rgbFore;
	LongRGB rgbBack;
	
	_UINT16 fls = 0;
	
	bool fAuto = true;
	bool fInvertNeg = false;
	
	IcvChart icvFore = 0x004D;
	IcvChart icvBack = 0x004D;
};

} // namespace XLS

