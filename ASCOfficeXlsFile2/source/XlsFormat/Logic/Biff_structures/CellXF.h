#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BorderFillInfo.h>

namespace XLS
{;

class CFRecord;

class CellXF : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CellXF)
public:
	CellXF(size_t& cell_xf_current_id, size_t& style_xf_current_id);

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
	bool fAtrNum;
	bool fAtrFnt;
	bool fAtrAlc;
	bool fAtrBdr;
	bool fAtrPat;
	bool fAtrProt;

	BorderInfo border;
	FillInfo fill;

	bool fHasXFExt;
	bool fsxButton;

	size_t border_x_id;
	size_t fill_x_id;

private:
	size_t& cell_xf_current_id_;
	size_t& style_xf_current_id_;
};

} // namespace XLS

