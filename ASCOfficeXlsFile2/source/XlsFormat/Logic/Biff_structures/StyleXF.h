#pragma once

#include "BiffStructure.h"
#include "BorderFillInfo.h"

namespace XLS
{;

class CFRecord;
class GlobalWorkbookInfo;
typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;

class StyleXF : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(StyleXF)
public:
	StyleXF(size_t& cell_xf_current_id, size_t& style_xf_current_id);
	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	GlobalWorkbookInfoPtr m_GlobalWorkbookInfo;

	static const ElementType	type = typeStyleXF;

	int serialize(std::wostream & stream);

	void RegisterFillBorder();

	unsigned char	alc;
	bool			fWrap;
	unsigned char	alcV;
	bool			fJustLast;
	unsigned char	trot;
	unsigned char	cIndent;
	bool			fShrinkToFit;
	unsigned char	iReadOrder;

	BorderInfo		border;
	FillInfo		fill;

	BiffStructurePtrVector ext_props;

	int font_scheme;

	size_t border_x_id;
	size_t fill_x_id;

	size_t& cell_xf_current_id_;
	size_t& style_xf_current_id_;
};

} // namespace XLS

