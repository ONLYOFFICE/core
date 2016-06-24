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

#include "SXADDLCACHEFIELD.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLCACHEITEM.h>

namespace XLS
{


SXADDLCACHEFIELD::SXADDLCACHEFIELD()
{
}


SXADDLCACHEFIELD::~SXADDLCACHEFIELD()
{
}


class Parenthesis_SXADDLCACHEFIELD_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEFIELD_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEFIELD_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheField_SXDCaption>())
		{
			return false;
		}
		proc.repeated<Continue_SxaddlSxString>(0, 0);
		return true;
	};
};



class Parenthesis_SXADDLCACHEFIELD_3: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEFIELD_3)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEFIELD_3(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheField_SXDPropName>())
		{
			return false;
		}
		proc.repeated<Continue_SxaddlSxString>(0, 0);
		return true;
	};
};


class Parenthesis_SXADDLCACHEFIELD_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEFIELD_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEFIELD_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheField_SXDProperty>())
		{
			return false;
		}
		proc.optional<Parenthesis_SXADDLCACHEFIELD_3>();
		return true;
	};
};


class Parenthesis_SXADDLCACHEFIELD_4: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEFIELD_4)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEFIELD_4(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheField_SXDIfdbMpMapCount>())
		{
			return false;
		}
		proc.mandatory<SXAddl_SXCCacheField_SXDIfdbMempropMap>();
		return true;
	};
};


class Parenthesis_SXADDLCACHEFIELD_5: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXADDLCACHEFIELD_5)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXADDLCACHEFIELD_5(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SXAddl_SXCCacheField_SXDSxrmitmCount>())
		{
			return false;
		}
		proc.repeated<SXADDLCACHEITEM>(0, 0);
		proc.mandatory<SXAddl_SXCCacheItem_SXDEnd>();
		return true;
	};
};




// SXADDLCACHEFIELD = SXAddl_SXCCacheField_SXDId *Continue_SxaddlSxString 
//                    [SXAddl_SXCCacheField_SXDCaption *Continue_SxaddlSxString] 
//                    [SXAddl_SXCCacheField_SXDProperty [SXAddl_SXCCacheField_SXDPropName *Continue_SxaddlSxString]] 
//                    [SXAddl_SXCCacheField_SXDIfdbMpMapCount SXAddl_SXCCacheField_SXDIfdbMempropMap] 
//                    [SXAddl_SXCCacheField_SXDSxrmitmCount *SXADDLCACHEITEM SXAddl_SXCCacheItem_SXDEnd] 
BaseObjectPtr SXADDLCACHEFIELD::clone()
{
	return BaseObjectPtr(new SXADDLCACHEFIELD(*this));
}


//                    SXAddl_SXCCacheField_SXDEnd
const bool SXADDLCACHEFIELD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCCacheField_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.optional<Parenthesis_SXADDLCACHEFIELD_1>();
	proc.optional<Parenthesis_SXADDLCACHEFIELD_2>();
	proc.optional<Parenthesis_SXADDLCACHEFIELD_4>();
	proc.optional<Parenthesis_SXADDLCACHEFIELD_5>();
	proc.mandatory<SXAddl_SXCCacheField_SXDEnd>();
	return true;
}

} // namespace XLS

