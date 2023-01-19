/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "OfficeArtRGFOPTE.h"

namespace ODRAW
{


XLS::BiffStructurePtr OfficeArtRGFOPTE::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtRGFOPTE(*this));
}

void OfficeArtRGFOPTE::load(XLS::CFRecord& record)
{
	std::vector<OfficeArtFOPTEPtr> rgfopte; // temp ... for complex load

	for(int i = 0; i < options_count; ++i)
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
		else
		{
			Other_props.push_back(fopte);
		}

		rgfopte.push_back(fopte);
	}
//-----complex load 

	for (size_t i = 0; i < rgfopte.size(); ++i)
	{
		if (!rgfopte[i]) continue;

		if (rgfopte[i]->fComplex)
		{
			if (rgfopte[i]->op == 0)
			{
				continue;
			}
			else if (rgfopte[i]->op < 0)
			{
				break;
			}
			rgfopte[i]->ReadComplexData(record);
		}
	}

	rgfopte.clear();
}

void OfficeArtRGFOPTE::SetOptionsCount(const unsigned short count)
{
	options_count = count;
}


const unsigned short OfficeArtRGFOPTE::GetOptionsCount()
{
	return options_count;
}

} // namespace XLS
