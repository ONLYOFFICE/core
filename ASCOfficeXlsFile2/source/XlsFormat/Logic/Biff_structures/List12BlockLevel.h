/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/DXFN12List.h>

namespace XLS
{

class List12BlockLevel: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(List12BlockLevel)
public:
	BiffStructurePtr clone();

	List12BlockLevel();
	~List12BlockLevel();

	static const ElementType	type = typeList12BlockLevel;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	_UINT32	cbdxfHeader;
	_UINT32	istnHeader;
	_UINT32	cbdxfData;
	_UINT32	istnData;
	_UINT32	cbdxfAgg;
	_UINT32	istnAgg;
	_UINT32	cbdxfBorder;
	_UINT32	cbdxfHeaderBorder;
	_UINT32	cbdxfAggBorder;

	DXFN12List dxfHeader;
	DXFN12List dxfData;
	DXFN12List dxfAgg;
	DXFN12List dxfBorder;
	DXFN12List dxfHeaderBorder;
	DXFN12List dxfAggBorder;
	
	XLUnicodeString stHeader;
	XLUnicodeString stData;
	XLUnicodeString stAgg;
};

typedef boost::shared_ptr<List12BlockLevel> List12BlockLevelPtr;

} // namespace XLS