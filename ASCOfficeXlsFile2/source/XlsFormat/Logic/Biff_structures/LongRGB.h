#pragma once

#include "BiffStructure.h"

namespace XLS
{

class CFRecord;

class LongRGBA : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LongRGBA)
public:
	LongRGBA();
	LongRGBA(const bool alpha_present);
	BiffStructurePtr clone();

	static const ElementType	type = typeLongRGBA;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
	std::wstring  argb;

	bool is_alpha;
};


class LongRGB : public LongRGBA
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LongRGB)
public:
	LongRGB();
	BiffStructurePtr clone();
};

typedef boost::shared_ptr<LongRGB> LongRGBPtr;

} // namespace XLS
