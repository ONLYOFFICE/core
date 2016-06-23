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

#include "METADATA.h"
#include <Logic/Biff_unions/MDTINFO.h>
#include <Logic/Biff_unions/MDXSTR.h>
#include <Logic/Biff_unions/MDBLOCK.h>
#include <Logic/Biff_unions/MDXTUPLESET.h>
#include <Logic/Biff_records/MDXProp.h>
#include <Logic/Biff_records/MDXKPI.h>

namespace XLS
{


METADATA::METADATA()
{
}


METADATA::~METADATA()
{
}


class Parenthesis_METADATA_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_METADATA_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_METADATA_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.optional<MDXTUPLESET>())
		{
			if(!proc.optional<MDXProp>())
			{
				if(!proc.optional<MDXKPI>())
				{
					return false;
				}
			}
		}
		return true;
	};
};

BaseObjectPtr METADATA::clone()
{
	return BaseObjectPtr(new METADATA(*this));
}


// METADATA = *MDTINFO *MDXSTR *(MDXTUPLESET / MDXProp / MDXKPI) *MDBLOCK
const bool METADATA::loadContent(BinProcessor& proc)
{
	if(
		proc.repeated<MDTINFO>(0, 0) ||
		proc.repeated<MDXSTR>(0, 0) ||
		proc.repeated<Parenthesis_METADATA_1>(0, 0) ||
		proc.repeated<MDBLOCK>(0, 0))
	{
		return true;
	}
	return false;
}

} // namespace XLS

