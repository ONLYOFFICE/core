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

#include "BiffStructure.h"
#include "CellRef.h"
#include <stack>
#include <queue>
#include "../../../../Common/Utils/OptPtr.h"

namespace XLS
{

class CFRecord;

class Ptg;
typedef	boost::shared_ptr<Ptg> PtgPtr;
typedef std::stack<std::wstring> AssemblerStack;
typedef std::vector<PtgPtr> PtgVector;
typedef std::stack<PtgPtr> PtgStack;
typedef std::vector<PtgPtr>::const_iterator PtgVectorIterator;

typedef std::queue<PtgPtr> PtgQueue;


class Ptg : public BiffStructure // still abstract
{
public:
	Ptg();
	Ptg(const unsigned short ptg_id_init);

	void load(CFRecord& record) override;

	virtual void loadFields(CFRecord& record) {};

	virtual void writeFields(CFRecord& record) {};

	void save(CFRecord& record) override;

	static const ElementType	type = typePtg;

	virtual const unsigned short getPtgId() const;// = 0;
	const size_t getOffsetInRecord() const;
	const size_t getSizeOfStruct() const;

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref = false);

	_CP_OPT(unsigned short)	ptg_id;
	_CP_OPT(size_t)			offset_in_record;
	_CP_OPT(size_t)			size_of_struct;

protected:
	void addFuncWrapper(AssemblerStack& ptg_stack, const std::wstring& func_name);

};


} // namespace XLS

