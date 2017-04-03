/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "PROTECTION_COMMON.h"
#include <Logic/Biff_records/Protect.h>
#include <Logic/Biff_records/ScenarioProtect.h>
#include <Logic/Biff_records/ObjProtect.h>
#include <Logic/Biff_records/Password.h>

namespace XLS
{


PROTECTION_COMMON::PROTECTION_COMMON()
{
}


PROTECTION_COMMON::~PROTECTION_COMMON()
{
}


BaseObjectPtr PROTECTION_COMMON::clone()
{
	return BaseObjectPtr(new PROTECTION_COMMON(*this));
}


// PROTECTION_COMMON = [Protect] [ScenarioProtect] [ObjProtect] [Password]
const bool PROTECTION_COMMON::loadContent(BinProcessor& proc)
{
	bool res1 = proc.optional<Protect>();
	bool res2 = proc.optional<ScenarioProtect>();
	bool res3 = proc.optional<ObjProtect>();
	bool res4 = proc.optional<Password>();

	return res1 || res2 || res3 || res4;
}

} // namespace XLS

