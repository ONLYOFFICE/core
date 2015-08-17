#pragma once

#include "OfficeArtRecord.h"
#include "OfficeArtRGFOPTE.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtFOPT : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFOPT)
public:
	OfficeArtFOPT();
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtFOPT;
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	// overriden
	const unsigned short GetInstanceToStore();

	OfficeArtRGFOPTE fopt;

};

typedef boost::shared_ptr<OfficeArtFOPT> OfficeArtFOPTPtr;


} // namespace XLS
