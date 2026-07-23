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
#include "ObjFmla.h"
#include "LbsDropData.h"
#include "BiffString.h"
#include "Boolean.h"

namespace XLS
{

class CFRecord;

class FtLbsData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtLbsData)
public:
	FtLbsData() : fmla(false), fExist(false)
	{
	}
	BiffStructurePtr clone();

	static const ElementType type = typeFtLbsData;

	virtual void load(CFRecord& record, const unsigned short ot);
	virtual void save(CFRecord& record, const unsigned short ot);

	virtual void load(CFRecord& record){}

	ObjFmla fmla;
	unsigned short cLines = 0;
	unsigned short iSel = 0;

	bool fUseCB = false;
	bool fValidPlex = false;
	bool fValidIds = false;
	bool fNo3d = false;
	unsigned char wListSelType = 0;
	unsigned char lct = 0;

	unsigned short idEdit = 0;

	LbsDropDataPtr dropData;
	std::vector<XLUnicodeString> rgLines;
	std::vector<Boolean<unsigned char>> bsels;

	bool fExist;
};

} // namespace XLS

