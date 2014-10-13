#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of MSODRAWINGGROUP union of records 
class MSODRAWINGGROUP: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MSODRAWINGGROUP)
public:
	MSODRAWINGGROUP(const bool is_inside_chart_sheet);
	~MSODRAWINGGROUP();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

private:
	bool is_inside_chart_sheet_;
};

} // namespace XLS

