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
#include "XFProp.h"
#include "XFPropColor.h"
#include "XFPropGradient.h"
#include "XFPropGradientStop.h"
#include "XFPropBorder.h"
#include "BiffString.h"
#include "BitMarkedStructs.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{


BiffStructurePtr XFProp::clone()
{
	return BiffStructurePtr(new XFProp(*this));
}

void XFProp::load(CFRecord& record)
{
	record >> xfPropType >> cb;

	//record.skipNunBytes(cb - sizeof(cb) - sizeof(xfPropType));
	switch(xfPropType)
	{
		case 0x0000:
			xfPropDataBlob.reset(new BIFF_BYTE(0, L"value"));
			break;
		case 0x0001:
		case 0x0002:
		case 0x0005:
			xfPropDataBlob.reset(new XFPropColor);
			break;
		case 0x0003:
			xfPropDataBlob.reset(new XFPropGradient);
			break;
		case 0x0004:
			xfPropDataBlob.reset(new XFPropGradientStop);
			break;
		case 0x0006:
		case 0x0007:
		case 0x0008:
		case 0x0009:
		case 0x000A:
		case 0x000B:
		case 0x000C:
			xfPropDataBlob.reset(new XFPropBorder);
			break;
		case 0x000D:
		case 0x000E:
		case 0x000F:
		case 0x0010:
		case 0x0011: // XFPropTextRotation
		case 0x0013: // ReadingOrder
		case 0x0014:
		case 0x0015:
		case 0x0016:
		case 0x0017:
		case 0x001C:
		case 0x001D:
		case 0x001E:
		case 0x001F:
		case 0x0020:
		case 0x0021:
		case 0x0022:
		case 0x0023:
		case 0x0025:
		case 0x002B:
		case 0x002C:
			xfPropDataBlob.reset(new BIFF_BYTE(0, L"value"));
			break;
		case 0x0018:
			{
				boost::shared_ptr<LPWideString> str(new LPWideString);
				str->setName(L"value");
				record >> *str;
				xfPropDataBlob = str;
				return;
			}
		case 0x0019:
		case 0x001A:
		case 0x001B:
		case 0x0029:
		case 0x002A:
			xfPropDataBlob.reset(new BIFF_WORD(0, L"value"));
			break;
		case 0x0024:
			xfPropDataBlob.reset(new BIFF_DWORD(0, L"value"));
			break;
		case 0x0026:
			{
				boost::shared_ptr<LPWideString> str(new LPWideString);
				str->setName(L"value");
				record >> *str;
				xfPropDataBlob = str;
				return;
			}
		default:
			// EXCEPT::RT::WrongBiffRecord("Unsupported type of XFProp.", record.getTypeString());
			break;
	}
	if (xfPropDataBlob)
	{
		record >> *xfPropDataBlob;
	}
}
static void serialize_color_prop(std::wostream & stream, const std::wstring & name, BiffStructurePtr & val)
{
	if (name.empty())	return;
	XFPropColor * color = dynamic_cast<XFPropColor*>(val.get());
	if (!color) return;

	color->serialize(stream, name);
}
static long get_val_prop(BiffStructurePtr & val)
{
	BIFF_DWORD* dword = dynamic_cast<BIFF_DWORD*>(val.get());
	if (dword) return *dword->value();

	BIFF_WORD*	word = dynamic_cast<BIFF_WORD*>(val.get());
	if (word) return *word->value();

	BIFF_BYTE*	byte_ = dynamic_cast<BIFF_BYTE*>(val.get());
	if (byte_) return *byte_->value();

	return -1;
}
static void serialize_val_prop(std::wostream & stream, const std::wstring & name, BiffStructurePtr & val)
{
	if (val == NULL)	return;
	if (name.empty())	return;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(name)
		{
			BIFF_DWORD* dword = dynamic_cast<BIFF_DWORD*>(val.get());
			if (dword)	CP_XML_ATTR(L"val", *dword->value());

			BIFF_WORD*	word = dynamic_cast<BIFF_WORD*>(val.get());
			if (word)	CP_XML_ATTR(L"val", *word->value());

			BIFF_BYTE*	byte_ = dynamic_cast<BIFF_BYTE*>(val.get());
			if (byte_)	CP_XML_ATTR(L"val", *byte_->value());

			LPWideString* str_ = dynamic_cast<LPWideString*>(val.get());
			if (str_)	CP_XML_ATTR(L"val", XmlUtils::EncodeXmlString(str_->value()));
		}
	}
}
static void serialize_val_attr(CP_ATTR_NODE, const std::wstring & name, BiffStructurePtr & val)
{
	if (val == NULL)	return;
	if (name.empty())	return;

	BIFF_DWORD* dword = dynamic_cast<BIFF_DWORD*>(val.get());
	if (dword)	CP_XML_ATTR(name.c_str(), *dword->value());

	BIFF_WORD*	word = dynamic_cast<BIFF_WORD*>(val.get());
	if (word)	CP_XML_ATTR(name.c_str(), *word->value());

	BIFF_BYTE*	byte_ = dynamic_cast<BIFF_BYTE*>(val.get());
	if (byte_)	CP_XML_ATTR(name.c_str(), *byte_->value());

	LPWideString* str_ = dynamic_cast<LPWideString*>(val.get());
	if (str_)	CP_XML_ATTR(name.c_str(), XmlUtils::EncodeXmlString(str_->value()));
}
static void serialize_border_prop(std::wostream & stream, const std::wstring & name, BiffStructurePtr & val)
{
	if (name.empty())	return;
	
	XFPropBorder * border = dynamic_cast<XFPropBorder*>(val.get());
	if (!border)	return;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(name)
		{
			switch(border->dgBorder)
			{		
				case 1: CP_XML_ATTR(L"style", L"thin");				break;
				case 2: CP_XML_ATTR(L"style", L"medium");			break;
				case 3: CP_XML_ATTR(L"style", L"dashed");			break;
				case 4: CP_XML_ATTR(L"style", L"dotted");			break;
				case 5: CP_XML_ATTR(L"style", L"thick");			break;
				case 6: CP_XML_ATTR(L"style", L"double");			break;
				case 7: CP_XML_ATTR(L"style", L"hair");				break;
				case 8: CP_XML_ATTR(L"style", L"mediumDashed");		break;
				case 9: CP_XML_ATTR(L"style", L"dashDot");			break;
				case 10: CP_XML_ATTR(L"style", L"mediumDashDot");	break;
				case 11: CP_XML_ATTR(L"style", L"dashDotDot");		break;
				case 12: CP_XML_ATTR(L"style", L"mediumDashDotDot");break;
				case 13: CP_XML_ATTR(L"style", L"slantDashDot");	break;
			}
			if (border->dgBorder != 0)
			{
				border->color.serialize(CP_XML_STREAM(), L"color");
			}
		}
	}
}

void XFProp::serialize_attr(CP_ATTR_NODE)
{
	switch(xfPropType)
	{
		case 0x0000:
		{
			BIFF_BYTE* byte_ = dynamic_cast<BIFF_BYTE*>(xfPropDataBlob.get());
			if (!byte_)	return;

			switch(*byte_)
			{		
				case 2:		CP_XML_ATTR(L"patternType", L"pct50");					break; //50% gray
				case 3:		CP_XML_ATTR(L"patternType", L"pct75");					break; //75% gray
				case 4:		CP_XML_ATTR(L"patternType", L"pct25");					break; //25% gray
				case 5:		CP_XML_ATTR(L"patternType", L"horzStripe");				break; //Horizontal stripe
				case 6:		CP_XML_ATTR(L"patternType", L"vertStripe");				break; //Vertical stripe
				case 7:		CP_XML_ATTR(L"patternType", L"reverseDiagStripe");		break; //Reverse diagonal stripe
				case 8:		CP_XML_ATTR(L"patternType", L"diagStripe");				break; //Diagonal stripe
				case 9:		CP_XML_ATTR(L"patternType", L"diagCross");				break; //Diagonal crosshatch
				case 10:	CP_XML_ATTR(L"patternType", L"trellis");				break; //Thick Diagonal crosshatch
				case 11:	CP_XML_ATTR(L"patternType", L"thinHorzStripe");			break; //Thin horizontal stripe
				case 12:	CP_XML_ATTR(L"patternType", L"thinVertStripe");			break; //Thin vertical stripe
				case 13:	CP_XML_ATTR(L"patternType", L"thinReverseDiagStripe");	break; //Thin reverse diagonal stripe
				case 14:	CP_XML_ATTR(L"patternType", L"thinDiagStripe");			break; //Thin diagonal stripe
				case 15:	CP_XML_ATTR(L"patternType", L"thinHorzCross");			break; //Thin horizontal crosshatch
				case 16:	CP_XML_ATTR(L"patternType", L"thinDiagCross");			break; //Thin diagonal crosshatch
				case 17:	CP_XML_ATTR(L"patternType", L"gray125");				break; //12.5% gray
				case 18:	CP_XML_ATTR(L"patternType", L"gray0625");				break; //6.25% gray
							CP_XML_ATTR(L"patternType", L"solid");		
			}
		}break;
		case 0x0003:
		{
			XFPropGradient * grad_ = dynamic_cast<XFPropGradient*>(xfPropDataBlob.get());
			if (grad_) grad_->serialize_attr(CP_GET_XML_NODE());
 		}break;
		case 0x0026:
		{
			serialize_val_attr(CP_GET_XML_NODE(), L"formatCode",	xfPropDataBlob);		
		}break;
		case 0x0029:
		{
			serialize_val_attr(CP_GET_XML_NODE(), L"numFmtId",	xfPropDataBlob);	
		}break;	
	}
}
int XFProp::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		switch(xfPropType)
		{
			case 0x0001:	serialize_color_prop(stream, L"fgColor",		xfPropDataBlob);		break;
			case 0x0002:	serialize_color_prop(stream, L"bgColor",		xfPropDataBlob);		break;
			case 0x0005:	serialize_color_prop(stream, L"color",			xfPropDataBlob);		break;
			case 0x0006:	serialize_border_prop(stream, L"top",			xfPropDataBlob);		break;
			case 0x0007:	serialize_border_prop(stream, L"bottom",		xfPropDataBlob);		break;
			case 0x0008:	serialize_border_prop(stream, L"left",			xfPropDataBlob);		break;
			case 0x0009:	serialize_border_prop(stream, L"right",			xfPropDataBlob);		break;
			case 0x000A:	serialize_border_prop(stream, L"diagonal",		xfPropDataBlob);		break;
			case 0x000B:	serialize_border_prop(stream, L"vertical",		xfPropDataBlob);		break;
			case 0x000C:	serialize_border_prop(stream, L"horizontal",	xfPropDataBlob);		break;
			case 0x000D:	serialize_val_prop	(stream, L"diagonalDown",	xfPropDataBlob);		break;
			case 0x000E:	serialize_val_prop	(stream, L"diagonalUp",		xfPropDataBlob);		break;
			case 0x000F:	serialize_val_prop	(stream, L"horizontal",		xfPropDataBlob);		break;
			case 0x0010:	serialize_val_prop	(stream, L"vertical",		xfPropDataBlob);		break;
			case 0x0011:	serialize_val_prop	(stream, L"textRotation",	xfPropDataBlob);		break;
			case 0x0013: 	serialize_val_prop	(stream, L"readingOrder",	xfPropDataBlob);		break;
			case 0x0014:	serialize_val_prop	(stream, L"wrapText",		xfPropDataBlob);		break;
			case 0x0015:	serialize_val_prop	(stream, L"justifyLastLine", xfPropDataBlob);		break;
			case 0x0016:	serialize_val_prop	(stream, L"shrinkToFit",	xfPropDataBlob);		break;
			case 0x0018:	serialize_val_prop	(stream, L"name",			xfPropDataBlob);		break;
			case 0x0019:	serialize_val_prop	(stream, L"b",				xfPropDataBlob);		break;
			case 0x001A:	serialize_val_prop	(stream, L"u",				xfPropDataBlob);		break;
			case 0x001B: 
			{
				int v = get_val_prop(xfPropDataBlob);
				if (v >= 0)
				{
					CP_XML_NODE(L"vertAlign")
					{			
						switch(v)
						{
							case 0:	CP_XML_ATTR(L"val", L"baseline");	break;
							case 1:	CP_XML_ATTR(L"val", L"superscript");break;
							case 2:	CP_XML_ATTR(L"val", L"subscript");	break;
						}
					}
				}
			}break;
			case 0x001C:	serialize_val_prop	(stream, L"i",				xfPropDataBlob);		break;
			case 0x001D:	serialize_val_prop	(stream, L"strike",			xfPropDataBlob);		break;
			case 0x001E:	serialize_val_prop	(stream, L"outline",		xfPropDataBlob);		break;
			case 0x001F:	serialize_val_prop	(stream, L"shadow",			xfPropDataBlob);		break;
			case 0x0020:	serialize_val_prop	(stream, L"condense",		xfPropDataBlob);		break;
			case 0x0021:	serialize_val_prop	(stream, L"extend",			xfPropDataBlob);		break;
			case 0x0022:	serialize_val_prop	(stream, L"charset",		xfPropDataBlob);		break;
			case 0x0023:	serialize_val_prop	(stream, L"family",			xfPropDataBlob);		break;
			case 0x0024:	serialize_val_prop	(stream, L"sz",				xfPropDataBlob);		break;
			//case 0x0025:	serialize_val_prop	(stream, L"scheme",			xfPropDataBlob);		break;
			case 0x0026:	serialize_val_prop	(stream, L"formatCode",		xfPropDataBlob);		break;
			case 0x0029:	serialize_val_prop	(stream, L"numFmtId",		xfPropDataBlob);		break;
			case 0x002A:	serialize_val_prop	(stream, L"relativeIndent", xfPropDataBlob);		break;
			case 0x002B:	serialize_val_prop	(stream, L"locked",			xfPropDataBlob);		break;
			case 0x002C:	serialize_val_prop	(stream, L"hidden",			xfPropDataBlob);		break;
		}
	}
	return 0;
}


} // namespace XLS
