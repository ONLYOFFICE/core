#pragma once

#include "BiffStructure.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace XLS
{;

#pragma pack(1)
class Ftab_Cetab : public BiffStructure_NoVtbl
{
public:
	Ftab_Cetab();
	Ftab_Cetab(const std::wstring& word);
	Ftab_Cetab(const unsigned short func_index_init);

	const std::wstring getFuncName() const;
	const long getParamsNum() const;
	const unsigned short getIndex() const;
	const bool isMacro() const;

	//void toXML(BiffStructurePtr & parent);

public:
	static const unsigned short recognizeFunction(const std::wstring& word);

private:
	class ValuesDetermination
	{
	public:
		static const std::wstring getName(const unsigned short iftab);
		static const long getParamsNum(const unsigned short iftab);
		static const unsigned short getIndex(const std::wstring& func_name);
		static const long getParamsNum(const std::wstring& func_name);

	private:
		ValuesDetermination();
		static ValuesDetermination& getInst()
		{
			static ValuesDetermination inst;
			return inst;
		};
		struct ParamsFixed{
			ParamsFixed(const unsigned short iftab_, const long params_num_, const std::wstring& name_): name(name_), iftab(iftab_), params_num(params_num_) {};
			std::wstring name;
			unsigned short iftab;
			long params_num;
		};
		struct sort_by_name {};
		struct sort_by_iftab {};
		typedef boost::multi_index_container<
			ParamsFixed,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<boost::multi_index::tag<sort_by_iftab>, BOOST_MULTI_INDEX_MEMBER(ParamsFixed, unsigned short, iftab)>,
				boost::multi_index::ordered_unique<boost::multi_index::tag<sort_by_name>, BOOST_MULTI_INDEX_MEMBER(ParamsFixed, std::wstring, name)> >
		> ParamsFixedSet;


	private:
		ParamsFixedSet params_fixed;
	};

private:
	unsigned short func_index;
};
#pragma pack() // restore

} // namespace XLS

