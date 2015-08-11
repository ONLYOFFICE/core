#pragma once

#include "BiffStructure.h"
#include "BorderFillInfo.h"

namespace XLS
{;

class CFRecord;
class GlobalWorkbookInfo;
typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;

class CellXF : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CellXF)
public:
	CellXF(size_t& cell_xf_current_id, size_t& style_xf_current_id);

	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	static const ElementType	type = typeCellXF;

	GlobalWorkbookInfoPtr m_GlobalWorkbookInfo;
	
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

	BiffStructurePtrVector ext_props;

	FillInfoExt		font_color;
	size_t			font_id;

	bool fHasXFExt;
	bool fsxButton;

	size_t border_x_id;
	size_t fill_x_id;

	size_t& cell_xf_current_id_;
	size_t& style_xf_current_id_;

	void RegisterFillBorder();
};

} // namespace XLS

