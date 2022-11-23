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
#include "XFProps.h"
#include "XFProp.h"
#include "XFPropColor.h"
#include "XFPropGradient.h"
#include "XFPropGradientStop.h"

#include "BitMarkedStructs.h"

namespace XLS
{


BiffStructurePtr XFProps::clone()
{
	return BiffStructurePtr(new XFProps(*this));
}

void XFProps::load(CFRecord& record)
{
	arXFPropBorder.is_present = false;

	record.skipNunBytes(2); // reserved
	record >> cprops;
	
	for(int i = 0; i < cprops; ++i)
	{
		XFPropPtr prop(new XFProp);
		if (!prop) continue;

		prop->load(record);
		rgExt.push_back(prop);

		if	(prop->xfPropType >=  0 && prop->xfPropType <=  3)
		{
			arXFPropFill.push_back(prop);
		}
		else if (prop->xfPropType == 4)		
		{
			arXFPropGradient.push_back(prop);
		}
		else if (prop->xfPropType >=  6 && prop->xfPropType <= 14)
		{
			arXFPropBorder.is_present = true;
			switch(prop->xfPropType)
			{
				case 6: arXFPropBorder.top		= prop; break;
				case 7: arXFPropBorder.bottom	= prop; break;
				case 8: arXFPropBorder.left		= prop; break;
				case 9: arXFPropBorder.right	= prop; break;
				default:
					arXFPropBorder.other.push_back(prop);
			}
		}
		else if (prop->xfPropType >= 15 && prop->xfPropType <= 22 || prop->xfPropType == 42)	
		{
			arXFPropAlignment.push_back(prop);
		}
		else if	(prop->xfPropType >= 24 && prop->xfPropType <= 37 || prop->xfPropType ==  5)	
		{
			arXFPropFont.push_back(prop);
		}
		else if (prop->xfPropType >= 38 && prop->xfPropType <= 41)
		{
			bool skip_codes = false;
			if (prop->xfPropType == 0x0029)
			{
				BIFF_WORD* word = dynamic_cast<BIFF_WORD*>(prop->xfPropDataBlob.get());
				if ((word) && (word->value() == (_UINT16)30))
				{
					skip_codes = true;
				}

			}
			if (!skip_codes)
				arXFPropNumFmt.push_back(prop);	
		}
		else if (prop->xfPropType >= 43 && prop->xfPropType <= 44)	
		{
			arXFPropProtection.push_back(prop);		
		}
		else
		{
		}
	}
}

int XFProps::serialize_fill(std::wostream & stream)
{
	if (arXFPropFill.empty()) return 0;

	XFProp			*pPatternType = NULL;
	XFPropGradient	*pGradient = NULL;
	
	for (size_t i = 0; i < arXFPropFill.size(); i++)
	{
		switch(arXFPropFill[i]->xfPropType)
		{
		case 0: pPatternType	= arXFPropFill[i].get();	break;
		case 3: pGradient		= dynamic_cast<XFPropGradient*>(arXFPropFill[i]->xfPropDataBlob.get());		break;
		}

	}
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"fill")
		{
			if (pGradient || arXFPropGradient.size() > 0) 
			{
				CP_XML_NODE(L"gradientFill")
				{
					if (pGradient) 
						pGradient->serialize_attr(CP_GET_XML_NODE());

					for (size_t i = 0 ; i < arXFPropGradient.size(); i++)
					{
						if (arXFPropGradient[i]->xfPropDataBlob == NULL) continue;
						arXFPropGradient[i]->xfPropDataBlob->serialize(CP_XML_STREAM());
					}
				}
			}
			else
			{
				CP_XML_NODE(L"patternFill")
				{
					if (pPatternType)
					{
						pPatternType->serialize_attr(CP_GET_XML_NODE());					
					}	
					for (size_t i = 0; i < arXFPropFill.size(); i++)
					{
						XFPropColor *pColor = dynamic_cast<XFPropColor*>(arXFPropFill[i]->xfPropDataBlob.get());
						if (!pColor) continue;

						std::wstring sNode = L"color";
						switch(arXFPropFill[i]->xfPropType)
						{
						case 1: sNode = L"fgColor"; break;
						case 2: sNode = L"bgColor"; break;
						}

						pColor->serialize(CP_XML_STREAM(), sNode);
					}
				}
			}
		}
	}
	return 0;
}

int XFProps::serialize(std::wostream & strm, bool dxf)
{
	CP_XML_WRITER(strm)    
    {
		if (arXFPropFont.size() > 0)
		{
			CP_XML_NODE(L"font")
			{	
				for (size_t i = 0; i < arXFPropFont.size(); i++)
				{
					arXFPropFont[i]->serialize(CP_XML_STREAM());
				}
			}
		}
		if (arXFPropNumFmt.size() > 0)
		{
			CP_XML_NODE(L"numFmt")
			{	
				for (size_t i = 0; i < arXFPropNumFmt.size(); i++)
				{
					if (dxf)
					{
						arXFPropNumFmt[i]->serialize_attr(CP_GET_XML_NODE());
					}
					else
					{
						arXFPropNumFmt[i]->serialize(CP_XML_STREAM());
					}
				}
			}
		}
		serialize_fill(strm);

		if (arXFPropAlignment.size() > 0)
		{
			CP_XML_NODE(L"alignment")
			{	
				for (size_t i = 0; i < arXFPropAlignment.size(); i++)
				{
					arXFPropAlignment[i]->serialize(CP_XML_STREAM());
				}
			}
		}
		if (arXFPropBorder.is_present)
		{
			CP_XML_NODE(L"border")
			{	
				//порядок важен - xfPropType - 8, 9, 6, 7 ( - DataVal_Headings.xls
				if (arXFPropBorder.left)	arXFPropBorder.left->serialize(CP_XML_STREAM());
				if (arXFPropBorder.right)	arXFPropBorder.right->serialize(CP_XML_STREAM());
				if (arXFPropBorder.top)		arXFPropBorder.top->serialize(CP_XML_STREAM());
				if (arXFPropBorder.bottom)	arXFPropBorder.bottom->serialize(CP_XML_STREAM());

				//----------------------------------------
				for (size_t i = 0; i < arXFPropBorder.other.size(); i++)
				{
					arXFPropBorder.other[i]->serialize(CP_XML_STREAM());
				}
			}
		}	
	}
	return 0;
}
} // namespace XLS
