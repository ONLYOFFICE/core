
#include "StyleXF.h"
#include "ExtProp.h"
#include <Binary/CFRecord.h>

#include <simple_xml_writer.h>

namespace XLS
{;


StyleXF::StyleXF(size_t& cell_xf_current_id, size_t& style_xf_current_id)
:	cell_xf_current_id_(cell_xf_current_id), style_xf_current_id_(style_xf_current_id)
{
}


BiffStructurePtr StyleXF::clone()
{
	return BiffStructurePtr(new StyleXF(*this));
}

void StyleXF::store(CFRecord& record)
{
	unsigned int flags = 0;
	SETBITS(flags, 0, 2, alc);
	SETBIT(flags, 3, fWrap);
	SETBITS(flags, 4, 6, alcV);
	SETBIT(flags, 7, fJustLast);
	SETBITS(flags, 8, 15, trot);
	SETBITS(flags, 16, 19, cIndent);
	SETBIT(flags, 20, fShrinkToFit);
	SETBITS(flags, 22, 23, iReadOrder);
	record << flags;

	flags = 0;
	SETBITS(flags, 0, 3, border.dgLeft);
	SETBITS(flags, 4, 7, border.dgRight);
	SETBITS(flags, 8, 11, border.dgTop);
	SETBITS(flags, 12, 15, border.dgBottom);
	SETBITS(flags, 16, 22, border.icvLeft);
	SETBITS(flags, 23, 29, border.icvRight);
	SETBITS(flags, 30, 31, border.grbitDiag);
	record << flags;

	flags = 0;
	SETBITS(flags, 0, 6, border.icvTop);
	SETBITS(flags, 7, 13, border.icvBottom);
	SETBITS(flags, 14, 20, border.icvDiag);
	SETBITS(flags, 21, 24, border.dgDiag);
	SETBITS(flags, 26, 31, fill.fls);
	record << flags;

	unsigned short flags2 = 0;
	SETBITS(flags2, 0, 6, fill.icvFore);
	SETBITS(flags2, 7, 13, fill.icvBack);
	record << flags2;
}


void StyleXF::load(CFRecord& record)
{
	m_GlobalWorkbookInfo = record.getGlobalWorkbookInfo();

	unsigned int flags1;
	unsigned int flags2;
	unsigned int flags3;
	unsigned short flags4;
	record >> flags1 >> flags2 >> flags3 >> flags4;
	alc = static_cast<unsigned char>(GETBITS(flags1, 0, 2));
	fWrap = GETBIT(flags1, 3);
	alcV = static_cast<unsigned char>(GETBITS(flags1, 4, 6));
	fJustLast = GETBIT(flags1, 7);
	trot = static_cast<unsigned char>(GETBITS(flags1, 8, 15));
	cIndent = static_cast<unsigned char>(GETBITS(flags1, 16, 19));
	fShrinkToFit = GETBIT(flags1, 20);
	iReadOrder = static_cast<unsigned char>(GETBITS(flags1, 22, 23));

	border.dgLeft = static_cast<unsigned char>(GETBITS(flags2, 0, 3));
	border.dgRight = static_cast<unsigned char>(GETBITS(flags2, 4, 7));
	border.dgTop = static_cast<unsigned char>(GETBITS(flags2, 8, 11));
	border.dgBottom = static_cast<unsigned char>(GETBITS(flags2, 12, 15));
	border.dgDiag = static_cast<unsigned char>(GETBITS(flags3, 21, 24));
	border.grbitDiag = static_cast<unsigned char>(GETBITS(flags2, 30, 31));

	border.icvLeft = 0 != border.dgLeft ? static_cast<unsigned char>(GETBITS(flags2, 16, 22)) : 0;
	border.icvRight = 0 != border.dgRight ? static_cast<unsigned char>(GETBITS(flags2, 23, 29)) : 0;
	border.icvTop = 0 != border.dgTop ? static_cast<unsigned char>(GETBITS(flags3, 0, 6)) : 0;
	border.icvBottom = 0 != border.dgBottom ? static_cast<unsigned char>(GETBITS(flags3, 7, 13)) : 0;
	border.icvDiag = 0 != border.dgDiag ? static_cast<unsigned char>(GETBITS(flags3, 14, 20)) : 0;
	
	fill.fls = static_cast<unsigned char>(GETBITS(flags3, 26, 31));
	fill.icvFore = GETBITS(flags4, 0, 6);
	fill.icvBack = GETBITS(flags4, 7, 13);

}

void StyleXF::RegisterFillBorder()
{
	for (long i = 0; i < ext_props.size(); i++ )
	{
		ExtProp* ext_prop = dynamic_cast<ExtProp*>(ext_props[i].get());

		switch(ext_prop->extType)
		{
			case 0x0004:
			{
				fill.foreFillInfo_.enabled		= true;
				fill.foreFillInfo_.icv			= ext_prop->extPropData.color.icv;
				fill.foreFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
				fill.foreFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
				fill.foreFillInfo_.xclrValue	= ext_prop->extPropData.color.xclrValue;
			}break;
			case 0x0005:
			{
				fill.backFillInfo_.enabled		= true;
				fill.backFillInfo_.icv			= ext_prop->extPropData.color.icv;
				fill.backFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
				fill.backFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
				fill.backFillInfo_.xclrValue	= ext_prop->extPropData.color.xclrValue;
			}break;
			//case 0x0007:
			//case 0x0008:
			//case 0x0009:
			//case 0x000A:
			//case 0x000B:
			//case 0x000C:
			//case 0x000D:
			//	extPropData.color.toXML(own_tag);
			//	break;
			//case 0x0006:
			//	extPropData.gradient_fill.toXML(own_tag);
			//	break;
			//case 0x000E:
			//	own_tag->Puttext(STR::int2str(extPropData.font_scheme, 10).c_str());
			//	break;
			//case 0x000F:
			//	own_tag->Puttext(STR::int2str(extPropData.indent_level, 10).c_str());
			//	break;
		}
	}	
	
	border_x_id	= m_GlobalWorkbookInfo->RegisterBorderId(border);
	fill_x_id	= m_GlobalWorkbookInfo->RegisterFillId(fill);
	

}
int StyleXF::serialize(std::wostream & stream)
{
	return 0;
}

} // namespace XLS

