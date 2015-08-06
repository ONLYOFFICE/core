#pragma once

#include "../BiffStructure.h"
#include "OfficeArtCOLORREF.h"
#include "OfficeArtBlip.h"
#include "../FixedPoint.h"
#include "IMsoArray.h"
#include "MSOSHADECOLOR.h"
#include "MSO_enums.h"
#include <Logic/Biff_structures/HyperlinkObject.h>

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{;
class OfficeArtFOPTE;
typedef boost::shared_ptr<OfficeArtFOPTE> OfficeArtFOPTEPtr;

class OfficeArtFOPTE : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFOPTE)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtFOPTE;
	
	virtual void store(XLS::CFRecord& record);

protected:
	virtual void load(XLS::CFRecord& record);

public:
	static OfficeArtFOPTEPtr load_and_create(XLS::CFRecord& record);
	virtual void ReadComplexData(XLS::CFRecord& record);


public:
	unsigned short opid;
	bool fBid;
	bool fComplex;
	long op;
};


class fillColor : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillColor)
public:

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
	//{
	//	OfficeArtCOLORREF(op).toXML(xml_tag);
	//}

};

class TextBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(TextBooleanProperties)
public:


};

class lineColor : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(lineColor)
public:

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
	//{
	//	OfficeArtCOLORREF(op).toXML(xml_tag);
	//}

};

class fillType : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillType)
public:

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	static std::wstring  fills[10] = {L"msofillSolid", L"msofillPattern", L"msofillTexture", L"msofillPicture", L"msofillShade", L"msofillShadeCenter", L"msofillShadeShape", L"msofillShadeScale", L"msofillShadeTitle", L"msofillBackground"};

	//	if(op >= 0 && op < 10)
	//	{
	//		own_tag->setAttribute(L"type", fills[op]);
	//	}
	//	else
	//	{
	//		own_tag->setAttribute(L"type", L"unknown");
	//	}
	//}

};

class pib : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(pib)
public:

};

class pictureId : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(pictureId)
public:

};

class pictureRecolor : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(pictureRecolor)
public:

};


class fillOpacity : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillOpacity)
public:

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"opacity");
	//}

};

class fillBackColor : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillBackColor)
public:

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
	//{
	//	OfficeArtCOLORREF(op).toXML(xml_tag);
	//}

};

class fillBackOpacity : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillBackOpacity)
public:

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"opacity");
	//}

};

class fillCrMod : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillCrMod)
public:

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
	//{
	//	OfficeArtCOLORREF(op).toXML(xml_tag);
	//}

};

class fillWidth : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillWidth)
public:

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	own_tag->setAttribute(L"width", op);

	//}

};

class fillShadeType : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillShadeType)
public:

};

class FillStyleBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillStyleBooleanProperties)
public:

};

class fillBlip : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillBlip)
public:
	
	virtual void ReadComplexData(XLS::CFRecord& record);

private:
	OfficeArtBlipPtr blip;
};

class fillBlipName : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillBlipName)
public:
	virtual void ReadComplexData(XLS::CFRecord& record);

private:
	std::wstring name;
};


class fillBlipFlags : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillBlipFlags)
public:

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
private:
	enum MSOBLIPFLAGS
	{
		msoblipflagComment		= 0x00000000,
		msoblipflagFile			= 0x00000001,
		msoblipflagURL			= 0x00000002,
		msoblipflagDoNotSave	= 0x00000004,
		msoblipflagLinkToFile	= 0x00000008,
	};

};

class fillHeight : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillHeight)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	own_tag->setAttribute(L"height", op);
	//}
};

class fillAngle : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillAngle)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"angle");
	//}
};

class fillFocus : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillFocus)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	own_tag->setAttribute(L"focus", op);
	//}
};

class fillToLeft : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillToLeft)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"left");
	//}
};

class fillToTop : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillToTop)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"top");
	//}
};

class fillToRight : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillToRight)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"right");
	//}
};

class fillToBottom : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillToBottom)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"bottom");
	//}
};

class fillRectLeft : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillRectLeft)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"left");
	//}
};

class fillRectTop : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillRectTop)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"top");
	//}
};

class fillRectRight : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillRectRight)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"right");
	//}
};

class fillRectBottom : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillRectBottom)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"bottom");
	//}
};

class fillDztype : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillDztype)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	own_tag->setAttribute(L"type", op);
	//}
};

class fillShadePreset : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillShadePreset)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	own_tag->setAttribute(L"preset", op);
	//}
};

class fillShadeColors : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillShadeColors)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void ReadComplexData(XLS::CFRecord& record);

	IMsoArray<MSOSHADECOLOR> fillShadeColors_complex;
};


class fillShapeOriginX : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillShapeOriginX)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"origin_x");
	//}
};

class fillShapeOriginY : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillShapeOriginY)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"origin_y");
	//}
};

class fillOriginX : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillOriginX)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"origin_x");
	//}
};

class fillOriginY : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillOriginY)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"origin_y");
	//}
};

class fillColorExt : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillColorExt)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OfficeArtCOLORREF(op).toXML(own_tag);
	//}
};

class reserved415 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(reserved415)
public:
};

class reserved417 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(reserved417)
public:
};

class reserved419 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(reserved419)
public:
};

class reserved421 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(reserved421)
public:
};

class reserved422 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(reserved422)
public:
};

class reserved423 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(reserved423)
public:
};

class fillBackColorExt : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillBackColorExt)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OfficeArtCOLORREF(op).toXML(own_tag);
	//}
};

class fillColorExtMod : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillColorExtMod)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	own_tag->setAttribute(L"mode", op);
	//}
};

class fillBackColorExtMod : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(fillBackColorExtMod)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	own_tag->setAttribute(L"mode", op);
	//}
};

class ProtectionBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ProtectionBooleanProperties)
public:
};

class LineStyleBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineStyleBooleanProperties)
public:
};

class ShadowStyleBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ShadowStyleBooleanProperties)
public:
};

class lineWidth : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(lineWidth)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	own_tag->setAttribute(L"width", op);
	//}
};

class lineMiterLimit : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(lineMiterLimit)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	OSHARED::FixedPoint(op).toXML(own_tag, L"limit");
	//}
};

class lineDashing : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(lineDashing)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	own_tag->setAttribute(L"dashing", MSOLINEDASHING::ToString(op));
	//}
};

class lineStyle : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(lineStyle)
public:
	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
	//{
	//	own_tag->setAttribute(L"style", MSOLINESTYLE::ToString(op));
	//}
};

class IHlink;
typedef boost::shared_ptr<IHlink> IHlinkPtr;

class IHlink : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(IHlink)
public:
	IHlink(){}

	XLS::BiffStructurePtr clone();

	void load(XLS::CFRecord& record);
	void store(XLS::CFRecord& record){}
	
	static const XLS::ElementType	type = XLS::typeIHLink;

	_GUID_							CLSID_StdHlink;
	OSHARED::HyperlinkObject		hyperlink;
};

class pihlShape : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(pihlShape)
public:
	void ReadComplexData(XLS::CFRecord& record);

	IHlink IHlink_complex;
};



}
