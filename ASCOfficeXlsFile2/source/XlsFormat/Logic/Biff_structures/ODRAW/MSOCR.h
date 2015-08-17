#pragma once

#include "../BiffStructure.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{

class MSOCR : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(MSOCR)
public:
	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeMSOCR;

	unsigned char red;
	unsigned char green;
	unsigned char blue;
	bool fSchemeIndex;
};

typedef boost::shared_ptr<MSOCR> MSOCRPtr;


} // namespace XLS
