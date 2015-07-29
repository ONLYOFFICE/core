#pragma once

#include "../BiffStructure.h"
#include "OfficeArtFOPTE.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtRGFOPTE : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtRGFOPTE)
public:
	XLS::BiffStructurePtr clone();

	
	
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	void SetOptionsNumber(const unsigned short number);
	const unsigned short GetOptionsNumber();

private:
	std::vector<OfficeArtFOPTEPtr> rgfopte;
	unsigned short options_num;
};

typedef boost::shared_ptr<OfficeArtRGFOPTE> OfficeArtRGFOPTEPtr;


} // namespace XLS
