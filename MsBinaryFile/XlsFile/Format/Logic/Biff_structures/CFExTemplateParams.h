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

namespace XLS
{

#pragma pack(1)

struct CFExFilterParams
{
	bool fTop : 1;
	bool fPercent : 1;
	unsigned short iParam;

private:
	unsigned char reserved2_1;
	_UINT32 reserved2_2;
	_UINT32 reserved2_3;
	_UINT32 reserved2_4;
};


struct CFExTextTemplateParams
{
	unsigned short ctp;

private:
	_UINT16 reserved_1 = 0;
	_UINT32 reserved_2 = 0;
	_UINT32 reserved_3 = 0;
	_UINT32 reserved_4 = 0;
};


struct CFExDateTemplateParams
{
	unsigned short dateOp;

private:
	_UINT16 reserved_1 = 0;
	_UINT32 reserved_2 = 0;
	_UINT32 reserved_3 = 0;
	_UINT32 reserved_4 = 0;
};


struct CFExAveragesTemplateParams
{
	unsigned short iParam = 0;

private:
	unsigned short reserved_1 = 0;
	_UINT32 reserved_2 = 0;
	_UINT32 reserved_3 = 0;
	_UINT32 reserved_4 = 0;
};


struct CFExDefaultTemplateParams
{
private:
	_UINT32 unused1 = 0;
	_UINT32 unused2 = 0;
	_UINT32 unused3 = 0;
	_UINT32 unused4 = 0;
};


class CFExTemplateParams : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CFExTemplateParams)

public:
	CFExTemplateParams();
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
    virtual void save(CFRecord& record);


	static const ElementType	type = typeCFExTemplateParams;

	union
	{
		CFExFilterParams filter;
		CFExTextTemplateParams text;
		CFExDateTemplateParams date;
		CFExAveragesTemplateParams averages;
		CFExDefaultTemplateParams defaults;
	} data;
};
#pragma pack()

} // namespace XLS

