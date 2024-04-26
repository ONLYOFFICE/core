/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "../../../../../DesktopEditor/xml/include/xmlutils.h"


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
			xfPropDataBlob.reset(new BIFF_BYTE(0));
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
			xfPropDataBlob.reset(new BIFF_BYTE(0));
			break;
		case 0x0018:
			{
				boost::shared_ptr<LPWideString> str(new LPWideString);
				//str->setName(L"value");
				record >> *str;
				xfPropDataBlob = str;
				return;
			}
		case 0x0012: // indent
		case 0x0019:
		case 0x001A:
		case 0x001B:
		case 0x0029:
		case 0x002A:
			xfPropDataBlob.reset(new BIFF_WORD(0));
			break;
		case 0x0024:
			xfPropDataBlob.reset(new BIFF_DWORD(0));
			break;
		case 0x0026:
			{
				boost::shared_ptr<LPWideString> str(new LPWideString);
				//str->setName(L"value");
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
void XFProp::save(CFRecord& record)
{
    LPWideString* strData;
	if (xfPropDataBlob)
	{
        cb = sizeof(cb) + sizeof(xfPropType);
		auto blobSize = 0;
        switch(xfPropType)
        {
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
		case 0x0000:
            blobSize = 1;
			break;
		case 0x0001:
		case 0x0002:
		case 0x0005:
			blobSize = 8;
			break;
		case 0x0003:
			blobSize = 44;
			break;
		case 0x0004:
			blobSize = 18;
			break;
		case 0x0006:
		case 0x0007:
		case 0x0008:
		case 0x0009:
		case 0x000A:
		case 0x000B:
		case 0x000C:
			blobSize = 10;
			break;

		case 0x0026:
		case 0x0018:
		{
			strData = dynamic_cast<LPWideString*>(xfPropDataBlob.get());
			if(strData)
				blobSize = (strData->getSize() * 2) + 2;
		}
				break;
		case 0x0012: // indent
		case 0x0019:
		case 0x001A:
		case 0x001B:
		case 0x0029:
		case 0x002A:
			blobSize = 2;
			break;
		case 0x0024:
			blobSize = 4;
			break;
		default:
			// EXCEPT::RT::WrongBiffRecord("Unsupported type of XFProp.", record.getTypeString());
			break;
        }
		cb += blobSize;
		record << xfPropType << cb;

		if (xfPropType == 0x0026 || xfPropType == 0x0018)
		{
			if (strData)
				record << *strData;
		}
		else
			 record << *xfPropDataBlob;
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
static void deserialize_val_prop(XmlUtils::CXmlLiteReader& oReader, const std::wstring & typeName, BiffStructurePtr & val)
{
	if (oReader.GetAttributesCount() > 0 && oReader.MoveToFirstAttribute() == true)
	{
		std::wstring wsPropName = oReader.GetName();

		if (!wsPropName.empty() && wsPropName == L"val")
		{
			auto value = oReader.GetText();

			if (typeName == L"BIFF_BYTE")
			{
				BYTE byte = 0;

				if (L"true" == value) byte = 1;
				else if (L"True" == value) byte = 1;
				else if (L"1" == value) byte = 1;
				else if (L"t" == value) byte = 1;
				else if (L"on" == value) byte = 1;
				else if (L"f" == value) byte = 0;
				else if (L"0" == value) byte = 0;
				else if (L"false" == value) byte = 0;
				else if (L"False" == value) byte = 0;
				else if (L"off" == value) byte = 0;

				else if (L"none" == value) byte = 0;
				else if (L"major" == value) byte = 1;
				else if (L"minor" == value) byte = 2;

				else byte = XmlUtils::GetInteger(value);

				val.reset(new BIFF_BYTE(byte));
			}
			else if (typeName == L"BIFF_WORD")
			{
				unsigned short word = 0;

				if (L"baseline" == value) word = 0;
				else if (L"superscript" == value) word = 1;
				else if (L"subscript" == value) word = 2;
				else if (L"none" == value) word = 0;
				else if (L"single" == value) word = 1;
				else if (L"double" == value) word = 2;
				else if (L"singleAccounting" == value) word = 33;
				else if (L"doubleAccounting" == value) word = 34;

				else if (L"true" == value) word = 0x02BC;
				else if (L"True" == value) word = 0x02BC;
				else if (L"1" == value) word = 0x02BC;
				else if (L"t" == value) word = 0x02BC;
				else if (L"on" == value) word = 0x02BC;
				else if (L"f" == value) word = 0x0190;
				else if (L"0" == value) word = 0x0190;
				else if (L"false" == value) word = 0x0190;
				else if (L"False" == value) word = 0x0190;
				else if (L"off" == value) word = 0x0190;

				else word = XmlUtils::GetInteger(value);

				val.reset(new BIFF_WORD(word));
			}
			else if (typeName == L"BIFF_DWORD")
			{
				val.reset(new BIFF_DWORD(XmlUtils::GetInteger(value) * 20.));
			}
			else if (typeName == L"LPWideString")
			{
				val.reset(new LPWideString(value));
			}
		}

		oReader.MoveToElement();
	}

}
static void deserialize_default_val(XmlUtils::CXmlLiteReader& oReader, const std::wstring & typeName, const _UINT32& value, BiffStructurePtr & val)
{
	BiffStructurePtr biffref;
	deserialize_val_prop(oReader, typeName, biffref);
	if (typeName == L"BIFF_WORD")
	{
		auto word = new BIFF_WORD;
		if(biffref.get())
		{
			*word = *(reinterpret_cast<BIFF_WORD*>(biffref.get()));
		}
		else
		{
			*word = value;
		}
		val.reset(word);
	}
	else if(typeName == L"BIFF_BYTE")
	{
		auto word = new BIFF_BYTE;
		if(biffref.get())
		{
			*word = *(reinterpret_cast<BIFF_BYTE*>(biffref.get()));
		}
		else
		{
			*word = value;
		}
		val.reset(word);
	}
}
static void deserialize_prop(XmlUtils::CXmlLiteReader& oReader, const std::wstring & typeName, BiffStructurePtr & val)
{
	auto value = oReader.GetText();

	if (typeName == L"BIFF_BYTE")
	{
		BYTE byte = 0;

		if (L"true" == value) byte = 1;
		else if (L"True" == value) byte = 1;
		else if (L"1" == value) byte = 1;
		else if (L"t" == value) byte = 1;
		else if (L"on" == value) byte = 1;
		else if (L"f" == value) byte = 0;
		else if (L"0" == value) byte = 0;
		else if (L"false" == value) byte = 0;
		else if (L"False" == value) byte = 0;
		else if (L"off" == value) byte = 0;

		else byte = XmlUtils::GetInteger(value);

		val.reset(new BIFF_BYTE(byte));
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

static XFPropBorder* deserialize_border_prop(XmlUtils::CXmlLiteReader& oReader)
{
	XFPropBorder * border = new XFPropBorder;
	if (!border)	return nullptr;

	border->dgBorder = 0;

	if (oReader.GetAttributesCount() > 0 && oReader.MoveToFirstAttribute() == true)
	{
		std::wstring wsPropName = oReader.GetName();

		if (!wsPropName.empty() && wsPropName == L"style")
		{
			auto value = oReader.GetTextA();

			if (value == "none") border->dgBorder = 0;
			else if (value == "thin") border->dgBorder = 1;
			else if (value == "medium") border->dgBorder = 2;
			else if (value == "dashed") border->dgBorder = 3;
			else if (value == "dotted") border->dgBorder = 4;
			else if (value == "thick") border->dgBorder = 5;
			else if (value == "double") border->dgBorder = 6;
			else if (value == "hair") border->dgBorder = 7;
			else if (value == "mediumDashed") border->dgBorder = 8;
			else if (value == "dashDot") border->dgBorder = 9;
			else if (value == "mediumDashDot") border->dgBorder = 10;
			else if (value == "dashDotDot") border->dgBorder = 11;
			else if (value == "mediumDashDotDot") border->dgBorder = 12;
			else if (value == "slantDashDot") border->dgBorder = 13;

			//oReader.MoveToNextAttribute();
		}

		oReader.MoveToElement();
	}

	if (!oReader.IsEmptyNode())
	{
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring wsPropName = oReader.GetName();

			if (wsPropName == L"color")			
				border->color.deserialize(oReader);			
		}
	}
    else
    {
        border->color.xclrType = 0;
        border->color.fValidRGBA = false;
        border->color.nTintShade = 0;
    }

	return border;
}
	static void deserialize_alignment(XmlUtils::CXmlLiteReader& oReader, const unsigned short & typeName, BiffStructurePtr & val)
	{
			BIFF_BYTE* byte_ = new BIFF_BYTE;
            if (!byte_)	return;

            auto value = oReader.GetText();
			if(typeName == 0x000F)
			{
				if (value == L"general") *byte_ = 0;
				else if (value == L"left") *byte_ = 1;
				else if (value == L"center") *byte_ = 2;
				else if (value == L"right") *byte_ = 3;
				else if (value == L"fill") *byte_ = 4;
				else if (value == L"justify") *byte_ = 5;
				else if (value == L"center-across-selection") *byte_ = 6;
				else if (value == L"distributed") *byte_ = 7;
				else *byte_ = 0xFF;
			}
			else if(typeName == 0x0010)
			{
				if (value == L"top") *byte_ = 0;
				else if (value == L"center") *byte_ = 1;
				else if (value == L"bottom") *byte_ = 2;
				else if (value == L"justify") *byte_ = 3;
				else if (value == L"distributed") *byte_ = 4;
				else *byte_ = 0;
			}
			else 
				*byte_ = 0;
            val.reset(byte_);
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
							//CP_XML_ATTR(L"patternType", L"solid");		
			}
		}break;
		case 0x0003:
		{
			XFPropGradient * grad_ = dynamic_cast<XFPropGradient*>(xfPropDataBlob.get());
			if (grad_) grad_->serialize_attr(CP_GET_XML_NODE());
 		}break;
		case 0x000D:	serialize_val_attr(CP_GET_XML_NODE(), L"diagonalDown", xfPropDataBlob);		break;
		case 0x000E:	serialize_val_attr(CP_GET_XML_NODE(), L"diagonalUp", xfPropDataBlob);		break;
		case 0x000F:	serialize_val_attr(CP_GET_XML_NODE(), L"horizontal", xfPropDataBlob);		break;
		case 0x0010:	serialize_val_attr(CP_GET_XML_NODE(), L"vertical", xfPropDataBlob);		break;
		case 0x0011:	serialize_val_attr(CP_GET_XML_NODE(), L"textRotation", xfPropDataBlob);		break;
		case 0x0012:	serialize_val_attr(CP_GET_XML_NODE(), L"indent", xfPropDataBlob);		break;
		case 0x0013: 	serialize_val_attr(CP_GET_XML_NODE(), L"readingOrder", xfPropDataBlob);		break;
		case 0x0014:	serialize_val_attr(CP_GET_XML_NODE(), L"wrapText", xfPropDataBlob);		break;
		case 0x0015:	serialize_val_attr(CP_GET_XML_NODE(), L"justifyLastLine", xfPropDataBlob);		break;
		case 0x0016:	serialize_val_attr(CP_GET_XML_NODE(), L"shrinkToFit", xfPropDataBlob);		break;
		case 0x0026:	serialize_val_attr(CP_GET_XML_NODE(), L"formatCode",	xfPropDataBlob);		break;
		case 0x0029:	serialize_val_attr(CP_GET_XML_NODE(), L"numFmtId",	xfPropDataBlob);		break;	
	}
}
void XFProp::deserialize_attr(XmlUtils::CXmlLiteReader& oReader)
{
	switch (xfPropType)
	{
		case 0x0000:
		{
			BIFF_BYTE* byte_ = new BIFF_BYTE;
			if (!byte_)	return;

			auto value = oReader.GetText();

			if (value == L"none") *byte_ = 0;
			else if (value == L"solid") *byte_ = 1;
			else if (value == L"pct50") *byte_ = 2;
            else if (value == L"pct75" || value == L"darkGray")
                *byte_ = 3;
			else if (value == L"pct25") *byte_ = 4;
			else if (value == L"horzStripe") *byte_ = 5;
			else if (value == L"vertStripe") *byte_ = 6;
			else if (value == L"reverseDiagStripe") *byte_ = 7;
			else if (value == L"diagStripe") *byte_ = 8;
			else if (value == L"diagCross") *byte_ = 9;
			else if (value == L"trellis") *byte_ = 10;
			else if (value == L"thinHorzStripe") *byte_ = 11;
			else if (value == L"thinVertStripe") *byte_ = 12;
			else if (value == L"thinReverseDiagStripe") *byte_ = 13;
			else if (value == L"thinDiagStripe") *byte_ = 14;
			else if (value == L"thinHorzCross") *byte_ = 15;
			else if (value == L"thinDiagCross") *byte_ = 16;
			else if (value == L"gray125") *byte_ = 17;
			else if (value == L"gray0625") *byte_ = 18;
            else *byte_ = 1;

			xfPropDataBlob.reset(byte_);
		}break;

		case 0x0001:
		case 0x0002:
		case 0x0005:
		{
			auto pColor = new XFPropColor;
			pColor->deserialize(oReader);
			xfPropDataBlob.reset(pColor);
		}break;
		case 0x0003:
		{
			auto pGradient = new XFPropGradient;
			pGradient->deserialize(oReader);
			xfPropDataBlob.reset(pGradient);
		}break;
		case 0x0004:
		{
			auto pGradientStop = new XFPropGradientStop;
			pGradientStop->deserialize(oReader);
			xfPropDataBlob.reset(pGradientStop);
		}break;
		case 0x0006:
		case 0x0007:
		case 0x0008:
		case 0x0009:
		case 0x000A:
		case 0x000B:
		case 0x000C:
			xfPropDataBlob.reset(deserialize_border_prop(oReader));
			break;
        case 0x000F:
		case 0x0010:
			deserialize_alignment(oReader, xfPropType, xfPropDataBlob);
        	break;
		case 0x000D:
		case 0x000E:
		case 0x0011: // XFPropTextRotation
		case 0x0013: // ReadingOrder
		case 0x0014:
		case 0x0015:
		case 0x0016:
		//case 0x0017:
            xfPropDataBlob.reset(new BIFF_BYTE(XmlUtils::GetInteger(oReader.GetText())));
            break;//
		case 0x0018:
			deserialize_val_prop(oReader, L"LPWideString", xfPropDataBlob);
			break;
		case 0x0019:
			deserialize_default_val(oReader, L"BIFF_WORD", 0x02BC, xfPropDataBlob);
			break;
		case 0x0020:
		case 0x0021:		
		case 0x001C:
        case 0x001D:
        case 0x001E:
		case 0x001F:
			deserialize_default_val(oReader, L"BIFF_BYTE", 0x01, xfPropDataBlob);
			break;    
		case 0x0022:
		case 0x0023:
			xfPropDataBlob.reset(new BIFF_BYTE(1));
		case 0x0025:
			deserialize_val_prop(oReader, L"BIFF_BYTE", xfPropDataBlob);
			break;
		case 0x002B:
		case 0x002C:
			deserialize_prop(oReader, L"BIFF_BYTE", xfPropDataBlob);
			break;
		case 0x0012: // indent
		case 0x0029:
        case 0x002A:
			xfPropDataBlob.reset(new BIFF_WORD(XmlUtils::GetInteger(oReader.GetText())));
			break;
		case 0x001A:
            //xfPropDataBlob.reset(new BIFF_WORD(1));
		case 0x001B:
			deserialize_val_prop(oReader, L"BIFF_WORD", xfPropDataBlob);			
			break;
		case 0x0024:
			deserialize_val_prop(oReader, L"BIFF_DWORD", xfPropDataBlob);
			break;
		case 0x0026:
			xfPropDataBlob.reset(new LPWideString(oReader.GetText()));
			break;		
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
			case 0x0012:	serialize_val_prop	(stream, L"indent",		xfPropDataBlob);		break;
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
			case 0x0025:	
			{
				int v = get_val_prop(xfPropDataBlob);
				if (v >= 0)
				{
					CP_XML_NODE(L"scheme")
					{
						switch (v)
						{
						case 0:	CP_XML_ATTR(L"val", L"none");	break;
						case 1:	CP_XML_ATTR(L"val", L"major"); break;
						case 2:	CP_XML_ATTR(L"val", L"minor");	break;
						}
					}
				}
			}break;
			case 0x0026:	serialize_val_prop	(stream, L"formatCode",		xfPropDataBlob);		break;
			case 0x0029:	serialize_val_prop	(stream, L"numFmtId",		xfPropDataBlob);		break;
			case 0x002A:	serialize_val_prop	(stream, L"relativeIndent", xfPropDataBlob);		break;
			case 0x002B:	serialize_val_prop	(stream, L"locked",			xfPropDataBlob);		break;
			case 0x002C:	serialize_val_prop	(stream, L"hidden",			xfPropDataBlob);		break;
		}
	}
	return 0;
}

int XFProp::deserialize(std::wstring& nodeName, std::wstring& attrName, XmlUtils::CXmlLiteReader& oReader)
{
	if (nodeName == L"alignment")
	{
		if (attrName == L"horizontal")
			xfPropType = 0x000F;			
		else if (attrName == L"vertical")		
			xfPropType = 0x0010;
		else if (attrName == L"textRotation")		
			xfPropType = 0x0011;
		else if (attrName == L"indent")
			xfPropType = 0x0012;
		else if (attrName == L"readingOrder")
			xfPropType = 0x0013;
		else if (attrName == L"wrapText")
			xfPropType = 0x0014;
		else if (attrName == L"justifyLastLine")
			xfPropType = 0x0015;
		else if (attrName == L"shrinkToFit")
			xfPropType = 0x0016;
		else if (attrName == L"relativeIndent")
			xfPropType = 0x002A;
	}

	else if (nodeName == L"border")
	{
		if (attrName == L"diagonalDown")
			xfPropType = 0x000D;
		else if (attrName == L"diagonalUp")
			xfPropType = 0x000E;
		else if (attrName == L"top")
			xfPropType = 0x0006;
		else if (attrName == L"bottom")
			xfPropType = 0x0007;
		else if (attrName == L"left" || attrName == L"start")
			xfPropType = 0x0008;
		else if (attrName == L"right" || attrName == L"end")
			xfPropType = 0x0009;
		else if (attrName == L"diagonal")
			xfPropType = 0x000A;
		else if (attrName == L"vertical")
			xfPropType = 0x000B;
		else if (attrName == L"horizontal")
			xfPropType = 0x000C;
	}

	else if (nodeName == L"fill")
	{
		if (attrName == L"patternType")
			xfPropType = 0x0000;
		else if (attrName == L"fgColor")		
			xfPropType = 0x0001;
		else if (attrName == L"bgColor")
			xfPropType = 0x0002;
		else if (attrName == L"gradientFill")
			xfPropType = 0x0003;
		else if (attrName == L"stop")
			xfPropType = 0x0004;
	}

	else if (nodeName == L"font")
	{		
		if (attrName == L"color")
			xfPropType = 0x0005;
		else if (attrName == L"name")
			xfPropType = 0x0018;
		else if (attrName == L"b")
			xfPropType = 0x0019;
		else if (attrName == L"u")
			xfPropType = 0x001A;
		else if (attrName == L"vertAlign")
			xfPropType = 0x001B;
		else if (attrName == L"i")
			xfPropType = 0x001C;
		else if (attrName == L"strike")
			xfPropType = 0x001D;
		else if (attrName == L"outline")
			xfPropType = 0x001E;
		else if (attrName == L"shadow")
			xfPropType = 0x001F;
		else if (attrName == L"condense")
			xfPropType = 0x0020;
		else if (attrName == L"extend")
			xfPropType = 0x0021;
		else if (attrName == L"charset")
			xfPropType = 0x0022;
		else if (attrName == L"family")
			xfPropType = 0x0023;
		else if (attrName == L"sz")
			xfPropType = 0x0024;
		else if (attrName == L"scheme")
			xfPropType = 0x0025;
	}

	else if (nodeName == L"numFmt")
	{
		if (attrName == L"formatCode")
			xfPropType = 0x0026;
		else if (attrName == L"numFmtId")
			xfPropType = 0x0029;
	}

	else if (nodeName == L"protection")
	{
		if (attrName == L"locked")
			xfPropType = 0x002B;
		else if (attrName == L"hidden")
			xfPropType = 0x002C;
	}
	
	deserialize_attr(oReader);		

	return 0;
}


} // namespace XLS
