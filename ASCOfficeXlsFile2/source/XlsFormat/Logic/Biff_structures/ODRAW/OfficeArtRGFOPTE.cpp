
#include "OfficeArtRGFOPTE.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


XLS::BiffStructurePtr OfficeArtRGFOPTE::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtRGFOPTE(*this));
}


void OfficeArtRGFOPTE::store(XLS::CFRecord& record)
{
	// We don't support xlsx to xls conversion
}


void OfficeArtRGFOPTE::load(XLS::CFRecord& record)
{
	for(int i = 0; i < options_num; ++i)
	{
		OfficeArtFOPTEPtr fopte = OfficeArtFOPTE::load_and_create(record);
		if (!fopte)continue;

		int type_prop = fopte->opid;

		if (type_prop		>= 0x0000	&&	type_prop < 0x0080)//2.3.18 Transform
		{
			Transform_props.push_back(fopte);
		}		
		else if	(type_prop	>= 0x0080	&&	type_prop < 0x00C0)	//2.3.21 Text
		{
			Text_props.push_back(fopte);
		}
		else if	(type_prop	>= 0x00C0 && type_prop < 0x0100) //2.3.22 Geometry Text - word art
		{
			GeometryText_props.push_back(fopte);
		}
		else if	(type_prop	>= 0x0100 && type_prop < 0x0140) //2.3.23 Blip
		{
			Blip_props.push_back(fopte);
		}
		else if (type_prop	>= 0x0140	&&	type_prop < 0x0180) //2.3.6 Geometry
		{
			Geometry_props.push_back(fopte);
		}
		else if (type_prop	>= 0x0180	&&	type_prop < 0x01C0) //2.3.7 Fill Style
		{
			FillStyle_props.push_back(fopte);
		}
		else if (type_prop	>= 0x01C0	&&	type_prop < 0x0200) //2.3.8 Line Style
		{
			LineStyle_props.push_back(fopte);
		}
		//2.3.9 Left line style
		//2.3.10 Top Line Style
		//2.3.11 Right Line Style
		//2.3.12 Bottom Line Style
		else if (type_prop	>= 0x0200	&&	type_prop < 0x0240)
		{
			Shadow_props.push_back(fopte);
		}
		//2.3.3 Callout
		//2.3.14 Perspective Style
		//2.3.15 3D Object
		//2.3.16 3D Style
		//2.3.17 Diagram		
		//2.3.19 Relative Transform
		//2.3.20 Protection
		//2.3.24 Unknown HTML
		//2.3.25 Web Component
		//2.3.26 Ink
		//2.3.27 Signature Line
		else if (type_prop	>= 0x0300	&&	type_prop < 0x0340) // 2.3.2 Shape
		{
			Shape_props.push_back(fopte);
		}
		else if (type_prop	>= 0x0380	&&	type_prop < 0x07C0) //2.3.4 Group Shape 
		{
			GroupShape_props.push_back(fopte);
		}
		else if (type_prop	>= 0x07C0	&&	type_prop < 0x07C6) //2.3.5 Group Shape 2
		{
			GroupShape_props.push_back(fopte);
		}

		rgfopte.push_back(fopte);
	}

	for(std::vector<OfficeArtFOPTEPtr>::iterator it = rgfopte.begin(), itEnd = rgfopte.end(); it != itEnd; ++it)
	{
		if((*it)->fComplex && (*it)->op > 0)
		{
			(*it)->ReadComplexData(record);
		}
	}

	rgfopte.clear();
}

void OfficeArtRGFOPTE::SetOptionsNumber(const unsigned short number)
{
	options_num = number;
}


const unsigned short OfficeArtRGFOPTE::GetOptionsNumber()
{
	return options_num;
}

} // namespace XLS
