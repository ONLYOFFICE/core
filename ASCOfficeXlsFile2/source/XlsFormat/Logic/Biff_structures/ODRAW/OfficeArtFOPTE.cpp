
#include "OfficeArtFOPTE.h"
#include <Binary/CFRecord.h>
#include "OfficeArtBlip.h"

namespace ODRAW
{;


XLS::BiffStructurePtr OfficeArtFOPTE::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFOPTE(*this));
}


void OfficeArtFOPTE::store(XLS::CFRecord& record)
{
	// We don't support xlsx to xls conversion
}


void OfficeArtFOPTE::load(XLS::CFRecord& record)
{
	unsigned short flags;
	record >> flags >> op;
	opid = GETBITS(flags, 0, 13);
	fBid = GETBIT(flags, 14);
	fComplex = GETBIT(flags, 15);

	// TODO: complex data shall be parsed here
}

OfficeArtFOPTEPtr OfficeArtFOPTE::load_and_create(XLS::CFRecord& record)
{
	const unsigned short* op = record.getCurData<unsigned short>();
	unsigned short opid = GETBITS(*op, 0, 13);

	OfficeArtFOPTEPtr fopte;
	switch(opid)
	{
		case 0x007F:
			fopte = OfficeArtFOPTEPtr(new ProtectionBooleanProperties);
			break;
		case 0x00BF:
			fopte = OfficeArtFOPTEPtr(new TextBooleanProperties);
			break;
		case 0x0100:
			fopte = OfficeArtFOPTEPtr(new cropFromTop);
			break;
		case 0x0101:
			fopte = OfficeArtFOPTEPtr(new cropFromBottom);
			break;
		case 0x0102:
			fopte = OfficeArtFOPTEPtr(new cropFromLeft);
			break;
		case 0x0103:
			fopte = OfficeArtFOPTEPtr(new cropFromRight);
			break;
		case 0x0104:
			fopte = OfficeArtFOPTEPtr(new pib);
			break;
		case 0x0105:
			fopte = OfficeArtFOPTEPtr(new anyString);
			break;
		case 0x010B:
			fopte = OfficeArtFOPTEPtr(new pictureId);
			break;
		case 0x011A:
			fopte = OfficeArtFOPTEPtr(new pictureRecolor);
			break;
		case 0x013F:
			fopte = OfficeArtFOPTEPtr(new BlipBooleanProperties);
			break;
		case 0x0180:
			fopte = OfficeArtFOPTEPtr(new fillType);
			break;
		case 0x0181:
			fopte = OfficeArtFOPTEPtr(new fillColor);
			break;
		case 0x0182:
			fopte = OfficeArtFOPTEPtr(new fillOpacity);
			break;
		case 0x0183:
			fopte = OfficeArtFOPTEPtr(new fillBackColor);
			break;
		case 0x0184:
			fopte = OfficeArtFOPTEPtr(new fillBackOpacity);
			break;
		case 0x0185:
			fopte = OfficeArtFOPTEPtr(new fillCrMod);
			break;
		case 0x0186:
			fopte = OfficeArtFOPTEPtr(new fillBlip);
			break;
		case 0x0187:
			fopte = OfficeArtFOPTEPtr(new anyString);
			break;
		case 0x0188:
			fopte = OfficeArtFOPTEPtr(new fillBlipFlags);
			break;
		case 0x0189:
			fopte = OfficeArtFOPTEPtr(new fillWidth);
			break;
		case 0x018A:
			fopte = OfficeArtFOPTEPtr(new fillHeight);
			break;
		case 0x018B:
			fopte = OfficeArtFOPTEPtr(new fillAngle);
			break;
		case 0x018C:
			fopte = OfficeArtFOPTEPtr(new fillFocus);
			break;
		case 0x018D:
			fopte = OfficeArtFOPTEPtr(new fillToLeft);
			break;
		case 0x018E:
			fopte = OfficeArtFOPTEPtr(new fillToTop);
			break;
		case 0x018F:
			fopte = OfficeArtFOPTEPtr(new fillToRight);
			break;
		case 0x0190:
			fopte = OfficeArtFOPTEPtr(new fillToBottom);
			break;
		case 0x0191:
			fopte = OfficeArtFOPTEPtr(new fillRectLeft);
			break;
		case 0x0192:
			fopte = OfficeArtFOPTEPtr(new fillRectTop);
			break;
		case 0x0193:
			fopte = OfficeArtFOPTEPtr(new fillRectRight);
			break;
		case 0x0194:
			fopte = OfficeArtFOPTEPtr(new fillRectBottom);
			break;
		case 0x0195:
			fopte = OfficeArtFOPTEPtr(new fillDztype);
			break;
		case 0x0196:
			fopte = OfficeArtFOPTEPtr(new fillShadePreset);
			break;
		case 0x0197:
			fopte = OfficeArtFOPTEPtr(new fillShadeColors);
			break;
		case 0x0198:
			fopte = OfficeArtFOPTEPtr(new fillOriginX);
			break;
		case 0x0199:
			fopte = OfficeArtFOPTEPtr(new fillOriginY);
			break;
		case 0x019A:
			fopte = OfficeArtFOPTEPtr(new fillShapeOriginX);
			break;
		case 0x019B:
			fopte = OfficeArtFOPTEPtr(new fillShapeOriginY);
			break;
		case 0x019C:
			fopte = OfficeArtFOPTEPtr(new fillShadeType);
			break;
		case 0x019E:
			fopte = OfficeArtFOPTEPtr(new fillColorExt);
			break;
		case 0x019F:
			fopte = OfficeArtFOPTEPtr(new reserved415);
			break;
		case 0x01A0:
			fopte = OfficeArtFOPTEPtr(new fillColorExtMod);
			break;
		case 0x01A1:
			fopte = OfficeArtFOPTEPtr(new reserved417);
			break;
		case 0x01A2:
			fopte = OfficeArtFOPTEPtr(new fillBackColorExt);
			break;
		case 0x01A3:
			fopte = OfficeArtFOPTEPtr(new reserved419);
			break;
		case 0x01A4:
			fopte = OfficeArtFOPTEPtr(new fillBackColorExtMod);
			break;
		case 0x01A5:
			fopte = OfficeArtFOPTEPtr(new reserved421);
			break;
		case 0x01A6:
			fopte = OfficeArtFOPTEPtr(new reserved422);
			break;
		case 0x01A7:
			fopte = OfficeArtFOPTEPtr(new reserved423);
			break;
		case 0x01BF:
			fopte = OfficeArtFOPTEPtr(new FillStyleBooleanProperties);
			break;
		case 0x01C0:
			fopte = OfficeArtFOPTEPtr(new lineColor);
			break;
		case 0x01CB:
			fopte = OfficeArtFOPTEPtr(new lineWidth);
			break;
		case 0x01CC:
			fopte = OfficeArtFOPTEPtr(new lineMiterLimit);
			break;
		case 0x01CD:
			fopte = OfficeArtFOPTEPtr(new lineStyle);
			break;
		case 0x01CE:
			fopte = OfficeArtFOPTEPtr(new lineDashing);
			break;
/*todo*/case 0x1D0:	//lineStartArrowhead
/*todo*/case 0x1D1:	//lineEndArrowhead
			fopte = OfficeArtFOPTEPtr(new OfficeArtFOPTE);
			break;
		case 0x01FF:
			fopte = OfficeArtFOPTEPtr(new LineStyleBooleanProperties);
			break;
		case 0x023F:
			fopte = OfficeArtFOPTEPtr(new ShadowStyleBooleanProperties);
			break;
/*todo*/case 0x0303: //cxstyle
			fopte = OfficeArtFOPTEPtr(new OfficeArtFOPTE);
			break;
		case 0x033F:
			fopte = OfficeArtFOPTEPtr(new ShapeBooleanProperties);
			break;
		case 0x0380:
		case 0x0381:
			fopte = OfficeArtFOPTEPtr(new anyString);
			break;
		case 0x0382:
			fopte = OfficeArtFOPTEPtr(new pihlShape);
			break;
		case 0x03BF:
			fopte = OfficeArtFOPTEPtr(new GroupShapeBooleanProperties);
			break;
		default:
			fopte = OfficeArtFOPTEPtr(new OfficeArtFOPTE);
			break;
	}

	fopte->load(record);
	return fopte;
}

void OfficeArtFOPTE::ReadComplexData(XLS::CFRecord& record)
{
	record.skipNunBytes(op); // default is to skip complex data
}

void TextBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	fFitShapeToText = GETBIT(op, 1);
	fAutoTextMargin = GETBIT(op, 3);
	fSelectText = GETBIT(op, 4);
	fUsefFitShapeToText = GETBIT(op, 17);
	fUsefAutoTextMargin = GETBIT(op, 19);
	fUsefSelectText = GETBIT(op, 20);
}


//void fillShadeType::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	bool msoshadeNone = GETBIT(op, 0);
//	bool msoshadeGamma = GETBIT(op, 1);
//	bool msoshadeSigma = GETBIT(op, 2);
//	bool msoshadeBand = GETBIT(op, 3);
//	bool msoshadeOneColor = GETBIT(op, 4);
//}

void FillStyleBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	fNoFillHitTest = GETBIT(op, 0);
	fillUseRect = GETBIT(op, 1);
	fillShape = GETBIT(op, 2);
	fHitTestFill = GETBIT(op, 3);
	fFilled = GETBIT(op, 4);
	fUseShapeAnchor = GETBIT(op, 5);
	fRecolorFillAsPicture = GETBIT(op, 6);
	fUsefNoFillHitTest = GETBIT(op, 16);
	fUsefillUseRect = GETBIT(op, 17);
	fUsefillShape = GETBIT(op, 18);
	fUsefHitTestFill = GETBIT(op, 19);
	fUsefFilled = GETBIT(op, 20);
	fUsefUseShapeAnchor = GETBIT(op, 21);
	fUsefRecolorFillAsPicture = GETBIT(op, 22);
}

void fillBlip::ReadComplexData(XLS::CFRecord& record)
{
	blip = OfficeArtBlip::load_blip(record);
}

void anyString::ReadComplexData(XLS::CFRecord& record)
{
	string_ = std::wstring(record.getCurData<wchar_t>(), op);
	record.skipNunBytes(op);
}

//void fillBlipFlags::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	if(op == msoblipflagComment)
//	{
//		own_tag->setAttribute(L"msoblipflagComment", true);
//	}
//	else
//	{
//		if(0 != (op & msoblipflagFile))
//		{
//			own_tag->setAttribute(L"msoblipflagFile", true);
//		}
//		if(0 != (op & msoblipflagURL))
//		{
//			own_tag->setAttribute(L"msoblipflagURL", true);
//		}
//		if(0 != (op & msoblipflagDoNotSave))
//		{
//			own_tag->setAttribute(L"msoblipflagDoNotSave", true);
//		}
//		if(0 != (op & msoblipflagLinkToFile))
//		{
//			own_tag->setAttribute(L"msoblipflagLinkToFile", true);
//		}
//	}
//}

void fillShadeColors::ReadComplexData(XLS::CFRecord& record)
{
	record >> fillShadeColors_complex;
}

void ProtectionBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	fLockAgainstGrouping = GETBIT(op, 0);
	fLockAdjustHandles = GETBIT(op, 1);
	fLockText = GETBIT(op, 2);
	fLockVertices = GETBIT(op, 3);
	fLockCropping = GETBIT(op, 4);
	fLockAgainstSelect = GETBIT(op, 5);
	fLockPosition = GETBIT(op, 6);
	fLockAspectRatio = GETBIT(op, 7);
	fLockRotation = GETBIT(op, 8);
	fLockAgainstUngrouping = GETBIT(op, 9);
	fUsefLockAgainstGrouping = GETBIT(op, 16);
	fUsefLockAdjustHandles = GETBIT(op, 17);
	fUsefLockText = GETBIT(op, 18);
	fUsefLockVertices = GETBIT(op, 19);
	fUsefLockCropping = GETBIT(op, 20);
	fUsefLockAgainstSelect = GETBIT(op, 21);
	fUsefLockPosition = GETBIT(op, 22);
	fUsefLockAspectRatio = GETBIT(op, 23);
	fUsefLockRotation = GETBIT(op, 24);
	fUsefLockAgainstUngrouping = GETBIT(op, 25);
}

void LineStyleBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	fNoLineDrawDash = GETBIT(op, 0);
	fLineFillShape = GETBIT(op, 1);
	fHitTestLine = GETBIT(op, 2);
	fLine = GETBIT(op, 3);
	fArrowheadsOK = GETBIT(op, 4);
	fInsetPenOK = GETBIT(op, 5);
	fInsetPen = GETBIT(op, 6);
	fLineOpaqueBackColor = GETBIT(op, 9);
	fUsefNoLineDrawDash = GETBIT(op, 16);
	fUsefLineFillShape = GETBIT(op, 17);
	fUsefHitTestLine = GETBIT(op, 18);
	fUsefLine = GETBIT(op, 19);
	fUsefArrowheadsOK = GETBIT(op, 20);
	fUsefInsetPenOK = GETBIT(op, 21);
	fUsefInsetPen = GETBIT(op, 22);
	fUsefLineOpaqueBackColor = GETBIT(op, 25);
}

void ShadowStyleBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	fshadowObscured = GETBIT(op, 0);
	fShadow = GETBIT(op, 1);
	fUsefshadowObscured = GETBIT(op, 16);
	fUsefShadow = GETBIT(op, 17);
}
XLS::BiffStructurePtr IHlink::clone()
{
	return XLS::BiffStructurePtr(new IHlink(*this));
}

void IHlink::load(XLS::CFRecord& record)
{
	record >> CLSID_StdHlink;
	record >> hyperlink;
}

void pihlShape::ReadComplexData(XLS::CFRecord& record)
{
	record >> IHlink_complex;
}

} 
