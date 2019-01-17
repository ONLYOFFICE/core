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

#include "BiffString.h"

namespace XLS
{

class CFRecord;

class Feat11XMapEntry2 : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Feat11XMapEntry2)
public:
	Feat11XMapEntry2();
	BiffStructurePtr clone();

	static const ElementType type = typeFeat11XMapEntry2;

	virtual void load(CFRecord& record);

	_UINT32			dwMapId;
	XLUnicodeString	rgbXPath;
};

class Feat11XMapEntry : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Feat11XMapEntry)
public:
	Feat11XMapEntry();
	BiffStructurePtr clone();

	static const ElementType type = typeFeat11XMapEntry;

	virtual void load(CFRecord& record);

	bool	fLoadXMap;
	bool	fCanBeSingle;

	Feat11XMapEntry2 entry;
};

class Feat11XMap : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Feat11XMap)
public:
	Feat11XMap();
	BiffStructurePtr clone();

	static const ElementType type = typeFeat11XMap;

	virtual void load(CFRecord& record);

	std::vector<Feat11XMapEntry> rgXmap;


};

} // namespace XLS

