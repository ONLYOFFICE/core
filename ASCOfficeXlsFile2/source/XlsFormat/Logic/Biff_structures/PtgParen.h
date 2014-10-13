#pragma once

#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class PtgParen : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgParen)
public:
	PtgParen();
	BiffStructurePtr clone();

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);
	static const unsigned __int16 fixed_id = 0x15;

	void incrementParametersNum();
	void decrementParametersNum();
	const size_t getParametersNum() const;

private:
	size_t num_parameters; // used to transfer the value to PtgFuncVar
};

typedef boost::shared_ptr<PtgParen> PtgParenPtr;

} // namespace XLS

