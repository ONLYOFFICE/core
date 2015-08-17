#pragma once

#include "BiffStructure.h"
#include "CellRef.h"
#include <stack>
#include <queue>
#include <common.h>

namespace XLS
{

class CFRecord;

class Ptg;
typedef	boost::shared_ptr<Ptg> PtgPtr;
typedef std::stack<std::wstring> AssemblerStack;
typedef std::vector<PtgPtr> PtgVector;
typedef std::stack<PtgPtr> PtgStack;
typedef std::vector<PtgPtr>::const_iterator PtgVectorIterator;

typedef std::queue<PtgPtr> PtgQueue;


class Ptg : public BiffStructure // still abstract
{
public:
	Ptg();
	Ptg(const unsigned short ptg_id_init);

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	virtual void loadFields(CFRecord& record) {};
	virtual void storeFields(CFRecord& record) {};

	static const ElementType	type = typePtg;

	virtual const unsigned short getPtgId() const;// = 0;
	const size_t getOffsetInRecord() const;
	const size_t getSizeOfStruct() const;

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data);

	_CP_OPT(unsigned short)	ptg_id;
	_CP_OPT(size_t)			offset_in_record;
	_CP_OPT(size_t)			size_of_struct;

protected:
	void addFuncWrapper(AssemblerStack& ptg_stack, const std::wstring& func_name);

};


} // namespace XLS

