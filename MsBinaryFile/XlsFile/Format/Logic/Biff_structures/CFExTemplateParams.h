/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "BiffStructure.h"

namespace XLS
{

#pragma pack(1)

struct CFExFilterParams : public BiffStructure_NoVtbl
{
private:
	bool fTop : 1;
	bool fPercent : 1;
	unsigned short iParam;

private:
	unsigned char reserved2_1;
	_UINT32 reserved2_2;
	_UINT32 reserved2_3;
	_UINT32 reserved2_4;
};


struct CFExTextTemplateParams : public BiffStructure_NoVtbl
{
private:
	unsigned short ctp;

private:
	_UINT16 reserved_1;
	_UINT32 reserved_2;
	_UINT32 reserved_3;
	_UINT32 reserved_4;
};


struct CFExDateTemplateParams : public BiffStructure_NoVtbl
{
private:
	unsigned short dateOp;

private:
	_UINT16 reserved_1;
	_UINT32 reserved_2;
	_UINT32 reserved_3;
	_UINT32 reserved_4;
};


struct CFExAveragesTemplateParams : public BiffStructure_NoVtbl
{
private:
	unsigned short iParam;

private:
	unsigned short reserved_1;
	_UINT32 reserved_2;
	_UINT32 reserved_3;
	_UINT32 reserved_4;
};


struct CFExDefaultTemplateParams : public BiffStructure_NoVtbl
{
private:
	_UINT32 unused1;
	_UINT32 unused2;
	_UINT32 unused3;
	_UINT32 unused4;
};


class CFExTemplateParams : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CFExTemplateParams)

public:
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);


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

