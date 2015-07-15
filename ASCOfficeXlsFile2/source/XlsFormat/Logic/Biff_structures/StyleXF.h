#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BorderFillInfo.h>

namespace XLS
{;

class CFRecord;

class StyleXF : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(StyleXF)
public:
	StyleXF(size_t& cell_xf_current_id, size_t& style_xf_current_id);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:

	unsigned char alc;
	bool fWrap;
	unsigned char alcV;
	bool fJustLast;
	unsigned char trot;
	unsigned char cIndent;
	bool fShrinkToFit;
	unsigned char iReadOrder;

	BorderInfo border;
	FillInfo fill;

	size_t border_x_id;
	size_t fill_x_id;

private:
	size_t& cell_xf_current_id_;
	size_t& style_xf_current_id_;
};

} // namespace XLS

