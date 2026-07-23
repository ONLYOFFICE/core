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
#include "../Biff_structures/BIFF12/ColSpan.h"

namespace XLS
{
	class Row : public BiffRecord
	{
		BIFF_RECORD_DEFINE_TYPE_INFO(Row)
		BASE_OBJECT_DEFINE_CLASS_NAME(Row)
	public:
		Row();
		~Row();

		BaseObjectPtr clone();

		void readFields(CFRecord& record) override;
		void writeFields(CFRecord& record) override;

		int serialize(std::wostream & stream);

		static const ElementType type = typeRow;

        UncheckedRw     rw = 0;
        unsigned short	colMic = 0;
        unsigned short	colMac = 0;
		_UINT16			miyRw = 256;

		unsigned char	iOutLevel = 0;
		bool			fCollapsed = false;
		bool			fDyZero = false;
		bool			fUnsynced = false;
		bool			fGhostDirty = false;

		_UINT32			ixfe_val = 0xffffffff;

		bool			fExAsc = false; //fExtraAsc in biff12
		bool			fExDes = false; //fExtraDsc in biff12
		bool			fPhonetic = false; //fPhShow in biff12

		bool			bValid = false;

//xlsb
		_UINT32 ccolspan;
		std::vector<XLSB::ColSpan> rgBrtColspan;
//--------------------------------------------------------------------
		GlobalWorkbookInfoPtr global_info_;
	};
	class Row_BIFF2  : public Row
	{
		BIFF_RECORD_DEFINE_TYPE_INFO(Row_BIFF2)
		BASE_OBJECT_DEFINE_CLASS_NAME(Row_BIFF2)
	public:
		Row_BIFF2();
		~Row_BIFF2();

		BaseObjectPtr clone();
	};
} // namespace XLS

