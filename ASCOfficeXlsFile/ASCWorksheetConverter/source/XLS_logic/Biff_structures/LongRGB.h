#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class CFRecord;

class LongRGBA : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(LongRGBA)
public:
	LongRGBA();
	LongRGBA(const bool alpha_present);
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

protected:
	BYTE red;
	BYTE green;
	BYTE blue;
	BYTE alpha;
	_bstr_t argb;

private:
	bool is_alpha;
};


class LongRGB : public LongRGBA
{
	BASE_OBJECT_DEFINE_CLASS_NAME(LongRGB)
public:
	LongRGB();
	BiffStructurePtr clone();
};

typedef boost::shared_ptr<LongRGB> LongRGBPtr;

} // namespace XLS
