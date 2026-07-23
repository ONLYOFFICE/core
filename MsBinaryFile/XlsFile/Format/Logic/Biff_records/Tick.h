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


// Logical representation of Tick record in BIFF8
class Tick: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(Tick)
	BASE_OBJECT_DEFINE_CLASS_NAME(Tick)
public:
	Tick();
	~Tick();

	BaseObjectPtr clone();
	
	void readFields(CFRecord& record);
	void writeFields(CFRecord& record);

	static const ElementType	type = typeTick;

	int serialize(std::wostream & _stream);

//-----------------------------
	unsigned char	tktMajor = 0;
	unsigned char	tktMinor = 0;
	unsigned char	tlt = 0;
	unsigned char	wBkgMode = 1;
	LongRGB rgb;

	bool fAutoCo = true;
	bool fAutoMode = false;
	unsigned char	rot = 0;
	bool fAutoRot = false;
	unsigned char	iReadingOrder = 0;

	_UINT16 icv = 0;
	_UINT16 trot = 0;

	int _rott = 0;
};

} // namespace XLS

