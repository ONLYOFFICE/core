#pragma once

#include "../BiffStructure.h"
#include "OfficeArtFOPTE.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{

class OfficeArtRGFOPTE : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtRGFOPTE)
public:
	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeOfficeArtRGFOPTE;

	void SetOptionsNumber(const unsigned short number);
	const unsigned short GetOptionsNumber();

	std::vector<OfficeArtFOPTEPtr> rgfopte;

	std::vector<OfficeArtFOPTEPtr> GroupShape_props;
	std::vector<OfficeArtFOPTEPtr> Shape_props;
	std::vector<OfficeArtFOPTEPtr> Blip_props;
	std::vector<OfficeArtFOPTEPtr> Text_props;
	std::vector<OfficeArtFOPTEPtr> Geometry_props;
	std::vector<OfficeArtFOPTEPtr> GeometryText_props;
	std::vector<OfficeArtFOPTEPtr> FillStyle_props;
	std::vector<OfficeArtFOPTEPtr> LineStyle_props;
	std::vector<OfficeArtFOPTEPtr> Shadow_props;
	std::vector<OfficeArtFOPTEPtr> Transform_props;
	unsigned short options_num;
};

typedef boost::shared_ptr<OfficeArtRGFOPTE> OfficeArtRGFOPTEPtr;


} // namespace XLS
