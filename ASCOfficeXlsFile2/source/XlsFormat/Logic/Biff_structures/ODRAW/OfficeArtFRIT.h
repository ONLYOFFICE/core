#pragma once

#include "../BiffStructure.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;

class OfficeArtFRIT : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFRIT)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtFRIT;

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

public:
	unsigned short fridNew;
	unsigned short fridOld;
};

typedef boost::shared_ptr<OfficeArtFRIT> OfficeArtFRITPtr;


} // namespace XLS
