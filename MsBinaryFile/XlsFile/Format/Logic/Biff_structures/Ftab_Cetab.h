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
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace XLS
{

#pragma pack(1)
class Ftab_Cetab : public BiffStructure_NoVtbl
{
public:
	Ftab_Cetab();
	Ftab_Cetab(const std::wstring& word);
	Ftab_Cetab(const unsigned short func_index_init);

	const std::wstring getFuncName() const;
    const int getParamsNum() const;
	const unsigned short getIndex() const;
	const bool isMacro() const;

	static const unsigned short recognizeFunction(const std::wstring& word);

	class ValuesDetermination
	{
	public:
		static const std::wstring getName(const unsigned short iftab);
        static const int getParamsNum(const unsigned short iftab);
		static const unsigned short getIndex(const std::wstring& func_name);
        static const int getParamsNum(const std::wstring& func_name);

		ValuesDetermination();
		static ValuesDetermination& getInst()
		{
			static ValuesDetermination inst;
			return inst;
        }
		struct ParamsFixed{
            ParamsFixed(const unsigned short iftab_, const int params_num_, const std::wstring& name_):
                name(name_), iftab(iftab_), params_num(params_num_) {}
			std::wstring name;
			unsigned short iftab;
            int params_num;
		};
		struct sort_by_name {};
		struct sort_by_iftab {};
		typedef boost::multi_index_container<
			ParamsFixed,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<boost::multi_index::tag<sort_by_iftab>, BOOST_MULTI_INDEX_MEMBER(ParamsFixed, unsigned short, iftab)>,
				boost::multi_index::ordered_unique<boost::multi_index::tag<sort_by_name>, BOOST_MULTI_INDEX_MEMBER(ParamsFixed, std::wstring, name)> >
		> ParamsFixedSet;


		ParamsFixedSet params_fixed;
	};

	unsigned short func_index;
};
#pragma pack() // restore

} // namespace XLS

