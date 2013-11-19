#pragma once

#include "BiffStructTagged.h"
#include "Ptg.h"

namespace XLS
{;

class CFRecord;

class RgbExtra : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RgbExtra)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	void load(CFRecord& record, const PtgVector& records_from_rgce, bool is_part_of_a_revision);
	void load(CFRecord& record, WORD rgce_record_type, bool is_part_of_a_revision);
	virtual void store(CFRecord& record);

	const PtgQueue& getPtgs() const;
	const bool isEmpty() const;
	void addPtg(const PtgPtr ptg);

private:
	// Shall not be used. This is just a stub for the virtual 'load' realization
	virtual void load(CFRecord& record);

private:
	PtgQueue ptg_records;
};

} // namespace XLS

