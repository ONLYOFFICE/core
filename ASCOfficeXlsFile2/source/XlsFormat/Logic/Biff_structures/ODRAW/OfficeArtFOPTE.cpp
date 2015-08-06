
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
		case 0x0104:
			fopte = OfficeArtFOPTEPtr(new pib);
			break;
		case 0x010B:
			fopte = OfficeArtFOPTEPtr(new pictureId);
			break;
		case 0x011A:
			fopte = OfficeArtFOPTEPtr(new pictureRecolor);
			break;

		case 0x007F:
			fopte = OfficeArtFOPTEPtr(new ProtectionBooleanProperties);
			break;

		case 0x00BF:
			fopte = OfficeArtFOPTEPtr(new TextBooleanProperties);
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
			fopte = OfficeArtFOPTEPtr(new fillBlipName);
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

		case 0x01FF:
			fopte = OfficeArtFOPTEPtr(new LineStyleBooleanProperties);
			break;

		case 0x023F:
			fopte = OfficeArtFOPTEPtr(new ShadowStyleBooleanProperties);
			break;

		case 0x0382:
			fopte = OfficeArtFOPTEPtr(new pihlShape);
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
};

//void TextBooleanProperties::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	bool fFitShapeToText = GETBIT(op, 1);
//	bool fAutoTextMargin = GETBIT(op, 3);
//	bool fSelectText = GETBIT(op, 4);
//	bool fUsefFitShapeToText = GETBIT(op, 17);
//	bool fUsefAutoTextMargin = GETBIT(op, 19);
//	bool fUsefSelectText = GETBIT(op, 20);
//
//	own_tag->setAttribute(L"fFitShapeToText", fFitShapeToText);
//	own_tag->setAttribute(L"fAutoTextMargin", fAutoTextMargin);
//	own_tag->setAttribute(L"fSelectText", fSelectText);
//	own_tag->setAttribute(L"fUsefFitShapeToText", fUsefFitShapeToText);
//	own_tag->setAttribute(L"fUsefAutoTextMargin", fUsefAutoTextMargin);
//	own_tag->setAttribute(L"fUsefSelectText", fUsefSelectText);
//}


//void fillShadeType::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	bool msoshadeNone = GETBIT(op, 0);
//	bool msoshadeGamma = GETBIT(op, 1);
//	bool msoshadeSigma = GETBIT(op, 2);
//	bool msoshadeBand = GETBIT(op, 3);
//	bool msoshadeOneColor = GETBIT(op, 4);
//
//	own_tag->setAttribute(L"fFitShapeToText", msoshadeNone);
//	own_tag->setAttribute(L"fAutoTextMargin", msoshadeGamma);
//	own_tag->setAttribute(L"fSelectText", msoshadeSigma);
//	own_tag->setAttribute(L"fUsefFitShapeToText", msoshadeBand);
//	own_tag->setAttribute(L"fUsefAutoTextMargin", msoshadeOneColor);
//}

//void FillStyleBooleanProperties::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	bool fNoFillHitTest = GETBIT(op, 0);
//	bool fillUseRect = GETBIT(op, 1);
//	bool fillShape = GETBIT(op, 2);
//	bool fHitTestFill = GETBIT(op, 3);
//	bool fFilled = GETBIT(op, 4);
//	bool fUseShapeAnchor = GETBIT(op, 5);
//	bool fRecolorFillAsPicture = GETBIT(op, 6);
//	bool fUsefNoFillHitTest = GETBIT(op, 16);
//	bool fUsefillUseRect = GETBIT(op, 17);
//	bool fUsefillShape = GETBIT(op, 18);
//	bool fUsefHitTestFill = GETBIT(op, 19);
//	bool fUsefFilled = GETBIT(op, 20);
//	bool fUsefUseShapeAnchor = GETBIT(op, 21);
//	bool fUsefRecolorFillAsPicture = GETBIT(op, 22);
//
//	own_tag->setAttribute(L"fNoFillHitTest", fNoFillHitTest);
//	own_tag->setAttribute(L"fillUseRect", fillUseRect);
//	own_tag->setAttribute(L"fillShape", fillShape);
//	own_tag->setAttribute(L"fHitTestFill", fHitTestFill);
//	own_tag->setAttribute(L"fFilled", fFilled);
//	own_tag->setAttribute(L"fUseShapeAnchor", fUseShapeAnchor);
//	own_tag->setAttribute(L"fRecolorFillAsPicture", fRecolorFillAsPicture);
//	own_tag->setAttribute(L"fUsefNoFillHitTest", fUsefNoFillHitTest);
//	own_tag->setAttribute(L"fUsefillUseRect", fUsefillUseRect);
//	own_tag->setAttribute(L"fUsefillShape", fUsefillShape);
//	own_tag->setAttribute(L"fUsefHitTestFill", fUsefHitTestFill);
//	own_tag->setAttribute(L"fUsefFilled", fUsefFilled);
//	own_tag->setAttribute(L"fUsefUseShapeAnchor", fUsefUseShapeAnchor);
//	own_tag->setAttribute(L"fUsefRecolorFillAsPicture", fUsefRecolorFillAsPicture);
//}
//
//
//void fillBlip::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	if(blip)
//	{
//		blip->toXML(xml_tag);
//	}
//}

void fillBlip::ReadComplexData(XLS::CFRecord& record)
{
	blip = OfficeArtBlip::load_blip(record);
}

//void fillBlipName::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	own_tag->setAttribute(L"name", name.c_str());
//}

void fillBlipName::ReadComplexData(XLS::CFRecord& record)
{
	name = std::wstring(record.getCurData<wchar_t>(), op);
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
//
//
//void fillShadeColors::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	if(fComplex && op > 0)
//	{
//		fillShadeColors_complex.toXML(own_tag);
//	}
//}
//
void fillShadeColors::ReadComplexData(XLS::CFRecord& record)
{
	record >> fillShadeColors_complex;
}


//void ProtectionBooleanProperties::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	bool fLockAgainstGrouping = GETBIT(op, 0);
//	bool fLockAdjustHandles = GETBIT(op, 1);
//	bool fLockText = GETBIT(op, 2);
//	bool fLockVertices = GETBIT(op, 3);
//	bool fLockCropping = GETBIT(op, 4);
//	bool fLockAgainstSelect = GETBIT(op, 5);
//	bool fLockPosition = GETBIT(op, 6);
//	bool fLockAspectRatio = GETBIT(op, 7);
//	bool fLockRotation = GETBIT(op, 8);
//	bool fLockAgainstUngrouping = GETBIT(op, 9);
//	bool fUsefLockAgainstGrouping = GETBIT(op, 16);
//	bool fUsefLockAdjustHandles = GETBIT(op, 17);
//	bool fUsefLockText = GETBIT(op, 18);
//	bool fUsefLockVertices = GETBIT(op, 19);
//	bool fUsefLockCropping = GETBIT(op, 20);
//	bool fUsefLockAgainstSelect = GETBIT(op, 21);
//	bool fUsefLockPosition = GETBIT(op, 22);
//	bool fUsefLockAspectRatio = GETBIT(op, 23);
//	bool fUsefLockRotation = GETBIT(op, 24);
//	bool fUsefLockAgainstUngrouping = GETBIT(op, 25);
//
//	own_tag->setAttribute(L"fLockAgainstGrouping", fLockAgainstGrouping);
//	own_tag->setAttribute(L"fLockAdjustHandles", fLockAdjustHandles);
//	own_tag->setAttribute(L"fLockText", fLockText);
//	own_tag->setAttribute(L"fLockVertices", fLockVertices);
//	own_tag->setAttribute(L"fLockCropping", fLockCropping);
//	own_tag->setAttribute(L"fLockAgainstSelect", fLockAgainstSelect);
//	own_tag->setAttribute(L"fLockPosition", fLockPosition);
//	own_tag->setAttribute(L"fLockAspectRatio", fLockAspectRatio);
//	own_tag->setAttribute(L"fLockRotation", fLockRotation);
//	own_tag->setAttribute(L"fLockAgainstUngrouping", fLockAgainstUngrouping);
//	own_tag->setAttribute(L"fUsefLockAgainstGrouping", fUsefLockAgainstGrouping);
//	own_tag->setAttribute(L"fUsefLockAdjustHandles", fUsefLockAdjustHandles);
//	own_tag->setAttribute(L"fUsefLockText", fUsefLockText);
//	own_tag->setAttribute(L"fUsefLockVertices", fUsefLockVertices);
//	own_tag->setAttribute(L"fUsefLockCropping", fUsefLockCropping);
//	own_tag->setAttribute(L"fUsefLockAgainstSelect", fUsefLockAgainstSelect);
//	own_tag->setAttribute(L"fUsefLockPosition", fUsefLockPosition);
//	own_tag->setAttribute(L"fUsefLockAspectRatio", fUsefLockAspectRatio);
//	own_tag->setAttribute(L"fUsefLockRotation", fUsefLockRotation);
//	own_tag->setAttribute(L"fUsefLockAgainstUngrouping", fUsefLockAgainstUngrouping);
//}
//
//void LineStyleBooleanProperties::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	bool fNoLineDrawDash = GETBIT(op, 0);
//	bool fLineFillShape = GETBIT(op, 1);
//	bool fHitTestLine = GETBIT(op, 2);
//	bool fLine = GETBIT(op, 3);
//	bool fArrowheadsOK = GETBIT(op, 4);
//	bool fInsetPenOK = GETBIT(op, 5);
//	bool fInsetPen = GETBIT(op, 6);
//	bool fLineOpaqueBackColor = GETBIT(op, 9);
//	bool fUsefNoLineDrawDash = GETBIT(op, 16);
//	bool fUsefLineFillShape = GETBIT(op, 17);
//	bool fUsefHitTestLine = GETBIT(op, 18);
//	bool fUsefLine = GETBIT(op, 19);
//	bool fUsefArrowheadsOK = GETBIT(op, 20);
//	bool fUsefInsetPenOK = GETBIT(op, 21);
//	bool fUsefInsetPen = GETBIT(op, 22);
//	bool fUsefLineOpaqueBackColor = GETBIT(op, 25);
//
//	own_tag->setAttribute(L"fNoLineDrawDash", fNoLineDrawDash);
//	own_tag->setAttribute(L"fLineFillShape", fLineFillShape);
//	own_tag->setAttribute(L"fHitTestLine", fHitTestLine);
//	own_tag->setAttribute(L"fLine", fLine);
//	own_tag->setAttribute(L"fArrowheadsOK", fArrowheadsOK);
//	own_tag->setAttribute(L"fInsetPenOK", fInsetPenOK);
//	own_tag->setAttribute(L"fInsetPen", fInsetPen);
//	own_tag->setAttribute(L"fLineOpaqueBackColor", fLineOpaqueBackColor);
//	own_tag->setAttribute(L"fUsefNoLineDrawDash", fUsefNoLineDrawDash);
//	own_tag->setAttribute(L"fUsefLineFillShape", fUsefLineFillShape);
//	own_tag->setAttribute(L"fUsefHitTestLine", fUsefHitTestLine);
//	own_tag->setAttribute(L"fUsefLine", fUsefLine);
//	own_tag->setAttribute(L"fUsefArrowheadsOK", fUsefArrowheadsOK);
//	own_tag->setAttribute(L"fUsefInsetPenOK", fUsefInsetPenOK);
//	own_tag->setAttribute(L"fUsefInsetPen", fUsefInsetPen);
//	own_tag->setAttribute(L"fUsefLineOpaqueBackColor", fUsefLineOpaqueBackColor);
//}

//void ShadowStyleBooleanProperties::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	bool fshadowObscured = GETBIT(op, 0);
//	bool fShadow = GETBIT(op, 1);
//	bool fUsefshadowObscured = GETBIT(op, 16);
//	bool fUsefShadow = GETBIT(op, 17);
//
//	own_tag->setAttribute(L"fshadowObscured", fshadowObscured);
//	own_tag->setAttribute(L"fShadow", fShadow);
//	own_tag->setAttribute(L"fUsefshadowObscured", fUsefshadowObscured);
//	own_tag->setAttribute(L"fUsefShadow", fUsefShadow);
//}
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
