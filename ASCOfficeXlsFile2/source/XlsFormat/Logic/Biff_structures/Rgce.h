#pragma once

#include "BiffStructure.h"
#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class Rgce : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Rgce)
public:
	Rgce(const CellRef& cell_base_ref_init);
	BiffStructurePtr clone();

	static const ElementType	type = typeRgce;

	
	void load(CFRecord& record, const size_t cce_val);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const size_t getCCE() const;
	void setCCE(const size_t cce_val); // mandatory to call before any use of "cce"
	const bool isEmpty() const;
	const PtgVector& getPtgs() const;
	void addPtg(const PtgPtr ptg);
	const CellRef& getLocation() const;

	const CellRef cell_base_ref;

	_CP_OPT(size_t) cce; // Size of rgce variable. Setting explicit is mandatory
	PtgVector sequence;
};

} // namespace XLS

