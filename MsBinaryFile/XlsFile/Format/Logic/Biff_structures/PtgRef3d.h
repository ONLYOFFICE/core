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

#include "OperandPtg.h"
#include "../GlobalWorkbookInfo.h"
#include "BIFF12/CellRef.h"

namespace XLS
{

class CFRecord;

class PtgRef3d: public OperandPtg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PtgRef3d)
public:
	PtgRef3d(const unsigned short full_ptg_id, const CellRef& cell_base_ref_init);
	PtgRef3d(const unsigned short ixti, const std::wstring& word, const PtgDataType data_type, const CellRef& cell_base_ref_init);

	void set_base_ref(const CellRef& cell_base_ref);

	BiffStructurePtr clone();

	void fromString(const std::wstring& str);

	void loadFields(CFRecord& record) override;

	void writeFields(CFRecord& record) override;
	
	void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref = false) override;

	static const unsigned short fixed_id = 0x1A;
private:
//biff8
	unsigned short			ixti;
	RgceLocRel				rgce_loc_rel;
	RgceLoc					rgce_loc;
	CellRef					cell_base_ref;
//biff5
	_UINT16					ixals;
	_UINT16					itabFirst;
	_UINT16					itabLast;
//biff12
    XLSB::RgceLoc           rgce_loc_xlsb;

	GlobalWorkbookInfoPtr	global_info;

};

} // namespace XLS

