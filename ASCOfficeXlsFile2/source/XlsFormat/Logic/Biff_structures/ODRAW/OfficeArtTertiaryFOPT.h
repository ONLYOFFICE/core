#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtRGFOPTE.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtTertiaryFOPT : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtTertiaryFOPT)
public:
	OfficeArtTertiaryFOPT();
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtTertiaryFOPT;
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	// overriden
	const unsigned short GetInstanceToStore();

	OfficeArtRGFOPTE fopt;

};

typedef boost::shared_ptr<OfficeArtTertiaryFOPT> OfficeArtTertiaryFOPTPtr;


} // namespace XLS
