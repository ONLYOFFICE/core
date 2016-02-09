#pragma once

#include "BiffStructure.h"
#include "Boolean.h"

#include <simple_xml_writer.h>

namespace XLS
{

class CFRecord;

class XFPropGradient : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XFPropGradient)
public:
	BiffStructurePtr clone();
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeXFPropGradient;

	void serialize_attr(CP_ATTR_NODE);

	bool	type1;

	double numDegree;
	double numFillToLeft;
	double numFillToRight;
	double numFillToTop;
	double numFillToBottom;
};

} // namespace XLS
