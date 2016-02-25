
#include "OfficeArtFOPTE.h"
#include <Binary/CFRecord.h>
#include "OfficeArtBlip.h"
#include <bitset>
#include <utils.h>

namespace ODRAW
{
	static int GetCountPoints2(NSCustomShapesConvert::RulesType eRuler, int lRepeatCount)
	{
		switch (eRuler)
		{
		case NSCustomShapesConvert::rtMoveTo:				
		case NSCustomShapesConvert::rtRMoveTo:
			{ return 1; }
		
		case NSCustomShapesConvert::rtLineTo:		
		case NSCustomShapesConvert::rtRLineTo:
			{ return lRepeatCount; }
		
		case NSCustomShapesConvert::rtCurveTo:		
		case NSCustomShapesConvert::rtRCurveTo:
			{ return 3 * lRepeatCount; }
		
		case NSCustomShapesConvert::rtNoFill:
		case NSCustomShapesConvert::rtNoStroke:
		case NSCustomShapesConvert::rtClose:
		case NSCustomShapesConvert::rtEnd:	
			{ return 0; }
		
		case NSCustomShapesConvert::rtAngleEllipseTo:
		case NSCustomShapesConvert::rtAngleEllipse:
			{ return lRepeatCount; }
		
		case NSCustomShapesConvert::rtArc:
		case NSCustomShapesConvert::rtArcTo:

		case NSCustomShapesConvert::rtClockwiseArcTo:
		case NSCustomShapesConvert::rtClockwiseArc:
			{ return lRepeatCount; }

		case NSCustomShapesConvert::rtEllipticalQuadrX:
		case NSCustomShapesConvert::rtEllipticalQuadrY:
			{ return 1 * lRepeatCount; }

		case NSCustomShapesConvert::rtQuadrBesier:			
			{ return /*2 * */lRepeatCount; }
		case NSCustomShapesConvert::rtFillColor:
		case NSCustomShapesConvert::rtLineColor:
			{
				return 1;
			}
		default: return 3 * lRepeatCount;
		};

		return 0;
	}
	static int BitmaskToInt( int value, int mask )
	{
		int ret = value & mask;

		//shift for all trailing zeros
		std::bitset<sizeof(int)*8> bits( mask );

		for ( unsigned int i = 0; i < bits.size(); i++ )
		{
			if ( !bits[i] )
			{
				ret >>= 1;
			}
			else
			{
				break;
			}
		}

		return ret;
	}

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

	opid		= GETBITS(flags, 0, 13);
	fBid		= GETBIT(flags, 14);
	fComplex	= GETBIT(flags, 15);

	// TODO: complex data shall be parsed here
}

OfficeArtFOPTEPtr OfficeArtFOPTE::load_and_create(XLS::CFRecord& record)
{
	const unsigned short* op = record.getCurData<unsigned short>();
	unsigned short opid = GETBITS(*op, 0, 13);

	OfficeArtFOPTEPtr fopte;
	switch(opid)
	{
		case 0x0004:
			fopte = OfficeArtFOPTEPtr(new Rotation);
			break;
		case 0x007F:
			fopte = OfficeArtFOPTEPtr(new ProtectionBooleanProperties);
			break;
		case NSOfficeDrawing::lTxid:
		case NSOfficeDrawing::dxTextLeft:
		case NSOfficeDrawing::dyTextTop:
		case NSOfficeDrawing::dxTextRight:
		case NSOfficeDrawing::dyTextBottom:
		case NSOfficeDrawing::WrapText:
		case NSOfficeDrawing::anchorText:
		case NSOfficeDrawing::txflTextFlow:
		case NSOfficeDrawing::cdirFont:
		case NSOfficeDrawing::hspNext:
		case NSOfficeDrawing::txdir:
		case NSOfficeDrawing::gtextRTF:
		case NSOfficeDrawing::gtextAlign:
		case NSOfficeDrawing::gtextSize:
		case NSOfficeDrawing::gtextCSSFont:
			fopte = OfficeArtFOPTEPtr(new OfficeArtFOPTE);
			break;
		case NSOfficeDrawing::gtextSpacing:
			fopte = OfficeArtFOPTEPtr(new FixedPoint);
			break;
		case NSOfficeDrawing::fFitTextToShape:
			fopte = OfficeArtFOPTEPtr(new TextBooleanProperties);
			break;
		case NSOfficeDrawing::gtextUNICODE:
		case NSOfficeDrawing::gtextFont:
			fopte = OfficeArtFOPTEPtr(new anyString);
			break;
		case 0x00ff:
			fopte = OfficeArtFOPTEPtr(new GeometryTextBooleanProperties);
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
		case 0x0140: 
		case 0x0141:
		case 0x0142: 
		case 0x0143:
			fopte = OfficeArtFOPTEPtr(new OfficeArtShapeRectCoord);
			break;
		case 0x0144: 
			fopte = OfficeArtFOPTEPtr(new ShapePath);
			break;
		case 0x0145:
			fopte = OfficeArtFOPTEPtr(new PVertices);
			break;
		case 0x0146: 
			fopte = OfficeArtFOPTEPtr(new PSegmentInfo);
			break;
		case 0x0147: //adjustValue .... //adjust8Value
		case 0x0148:
		case 0x0149:
		case 0x014A:
		case 0x014B:
		case 0x014C:
		case 0x014D:
		case 0x014E:
			fopte = OfficeArtFOPTEPtr(new AdjustValue);
			break;
		case 0x0151:
			fopte = OfficeArtFOPTEPtr(new pAdjustHandles);
			break;
		case 0x0156:
			fopte = OfficeArtFOPTEPtr(new pGuides);
			break;
		case 0x0152:
		case 0x0153:
		case 0x0154:
		case 0x0155:
		case 0x0157:
		case 0x0158:
			fopte = OfficeArtFOPTEPtr(new OfficeArtFOPTE);
			break;
		case 0x017f:
			fopte = OfficeArtFOPTEPtr(new GeometryBooleanProperties);
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
		case 0x01C1:
			fopte = OfficeArtFOPTEPtr(new lineOpacity);
			break;
		case 0x01C2:
			fopte = OfficeArtFOPTEPtr(new lineBackColor);
			break;
		case 0x01C3:
			fopte = OfficeArtFOPTEPtr(new lineCrMod);
			break;
		case 0x01C4:
			fopte = OfficeArtFOPTEPtr(new lineType);
			break;
		case 0x01C5:
			fopte = OfficeArtFOPTEPtr(new fillBlip);
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
		case 0x0204:
			fopte = OfficeArtFOPTEPtr(new shadowOpacity);
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
void FixedPoint::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	short			Integral	= op >> 16;
	unsigned short Fractional	= op - (Integral << 16);

	dVal = Integral + (Fractional / 65536.0);
}
void OfficeArtFOPTE::ReadComplexData(XLS::CFRecord& record)
{
	record.skipNunBytes(op); // default is to skip complex data
}

void TextBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	
	fUsefFitShapeToText = GETBIT(op, 17);
	fUsefAutoTextMargin = GETBIT(op, 19);
	fUsefSelectText		= GETBIT(op, 20);
	
	fFitShapeToText		= GETBIT(op, 1);
	fAutoTextMargin		= GETBIT(op, 3);
	fSelectText			= GETBIT(op, 4);
}
void GeometryTextBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	fUsegFReverseRows	= GETBIT(op, 31);
	fUsefGtext			= GETBIT(op, 30);
	fUsegFVertical		= GETBIT(op, 29);
	fUsegtextFKern		= GETBIT(op, 28);
	fUsegTight			= GETBIT(op, 27);
	fUsegFStretch		= GETBIT(op, 26);
	fUsegFShrinkFit		= GETBIT(op, 25);
	fUsegFBestFit		= GETBIT(op, 24);
	fUsegFNormalize		= GETBIT(op, 23);
	fUsegFDxMeasure		= GETBIT(op, 22);
	fUsegFBold			= GETBIT(op, 21);
	fUsegFItalic		= GETBIT(op, 20);
	fUsegFUnderline		= GETBIT(op, 19);
	fUsegFShadow		= GETBIT(op, 18);
	fUsegFSmallcaps		= GETBIT(op, 17);
	fUsegFStrikethrough	= GETBIT(op, 16);
	
	gFReverseRows	= GETBIT(op, 15);
	fGtext			= GETBIT(op, 14);
	fVertical		= GETBIT(op, 13);
	fKern			= GETBIT(op, 12);
	fTight			= GETBIT(op, 11);
	fStretch		= GETBIT(op, 10);
	fShrinkFit		= GETBIT(op,  9);
	fBestFit		= GETBIT(op,  8);
	fNormalize		= GETBIT(op,  7);
	fDxMeasure		= GETBIT(op,  6);
	fBold			= GETBIT(op,  5);
	fItalic			= GETBIT(op,  4);
	fUnderline		= GETBIT(op,  3);
	fShadow			= GETBIT(op,  2);
	fSmallcaps		= GETBIT(op,  1);
	fStrikethrough	= GETBIT(op,  0);
}

void FillStyleBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	fNoFillHitTest				= GETBIT(op, 0);
	fillUseRect					= GETBIT(op, 1);
	fillShape					= GETBIT(op, 2);
	fHitTestFill				= GETBIT(op, 3);
	fFilled						= GETBIT(op, 4);
	fUseShapeAnchor				= GETBIT(op, 5);
	fRecolorFillAsPicture		= GETBIT(op, 6);
	fUsefNoFillHitTest			= GETBIT(op, 16);
	fUsefillUseRect				= GETBIT(op, 17);
	fUsefillShape				= GETBIT(op, 18);
	fUsefHitTestFill			= GETBIT(op, 19);
	fUsefFilled					= GETBIT(op, 20);
	fUsefUseShapeAnchor			= GETBIT(op, 21);
	fUsefRecolorFillAsPicture	= GETBIT(op, 22);
}

void fillBlip::ReadComplexData(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rh_child;
	record >> rh_child;
	size_t child_beginning_ptr = record.getRdPtr();
	record.RollRdPtrBack(rh_child.size());

	blip = OfficeArtBlipPtr(new OfficeArtBlip(rh_child.recType));
	blip->loadFields(record);
}

void anyString::ReadComplexData(XLS::CFRecord& record)
{	
#if defined(_WIN32) || defined(_WIN64)
        string_ = std::wstring(record.getCurData<wchar_t>(), op);
#else
        string_ = XLS::convertUtf16ToWString(record.getCurData<UTF16>(), op);
#endif
	if (!string_.empty())
	{
        int i, length = (std::min)(op, (_INT32)string_.length());

		for (i = 0; i < length; i++)
		{
			if (string_.at(i) < 14 ) break;
		}
		string_ = string_.substr(0, i);
	}
	record.skipNunBytes(op);
}

void fillShadeColors::ReadComplexData(XLS::CFRecord& record)
{
	record >> fillShadeColors_complex;
}

void ProtectionBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	fLockAgainstGrouping	= GETBIT(op, 0);
	fLockAdjustHandles		= GETBIT(op, 1);
	fLockText				= GETBIT(op, 2);
	fLockVertices			= GETBIT(op, 3);
	fLockCropping			= GETBIT(op, 4);
	fLockAgainstSelect		= GETBIT(op, 5);
	fLockPosition			= GETBIT(op, 6);
	fLockAspectRatio		= GETBIT(op, 7);
	fLockRotation			= GETBIT(op, 8);
	fLockAgainstUngrouping	= GETBIT(op, 9);
	
	fUsefLockAgainstGrouping = GETBIT(op, 16);
	fUsefLockAdjustHandles	= GETBIT(op, 17);
	fUsefLockText			= GETBIT(op, 18);
	fUsefLockVertices		= GETBIT(op, 19);
	fUsefLockCropping		= GETBIT(op, 20);
	fUsefLockAgainstSelect	= GETBIT(op, 21);
	fUsefLockPosition		= GETBIT(op, 22);
	fUsefLockAspectRatio	= GETBIT(op, 23);
	fUsefLockRotation		= GETBIT(op, 24);
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

	fshadowObscured		= GETBIT(op, 0);
	fShadow				= GETBIT(op, 1);
	fUsefshadowObscured = GETBIT(op, 16);
	fUsefShadow			= GETBIT(op, 17);
}

void GeometryBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	fUsefShadowOK			= GETBIT(op, 8);
	fUsef3DOK				= GETBIT(op, 9);
	fUsefLineOK				= GETBIT(op, 10);
	fUsefGtextOK			= GETBIT(op, 11);
	fUsefFillShadeShapeOK	= GETBIT(op, 12);
	fUsefFillOK				= GETBIT(op, 13);
	
	fShadowOK				= GETBIT(op, 22);
	f3DOK					= GETBIT(op, 23);
	fLineOK					= GETBIT(op, 24);
	fGtextOK				= GETBIT(op, 25);
	fFillShadeShapeOK		= GETBIT(op, 26);
	fFillOK					= GETBIT(op, 27);
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
//---------------------------------------------------------------------------------------------
MSOPOINT::MSOPOINT()
{
	cbElement = 4;
}

MSOPOINT::MSOPOINT(unsigned short cbElement_)
{
	cbElement = 4;

	if (cbElement_ == 0xfff0)
	{
		cbElement = 2;
	}
}

XLS::BiffStructurePtr MSOPOINT::clone() 
{
	return XLS::BiffStructurePtr(new MSOPOINT(*this));
}

void MSOPOINT::load(XLS::CFRecord& record)
{
	if (cbElement == 4)
	{
		record >> x >> y;
	}
	else
	{
		unsigned short x_, y_;
		record >> x_ >> y_;
		
		x = x_;
		y = y_;
	}
}
//---------------------------------------------------------------------------------------------
MSOPATHINFO::MSOPATHINFO()
{
	cbElement = 4;
}
MSOPATHINFO::MSOPATHINFO(unsigned short cbElement_)
{
	cbElement = 4;

	if (cbElement_ == 0xfff0)
		cbElement = 2;
}

XLS::BiffStructurePtr MSOPATHINFO::clone()
{
	return XLS::BiffStructurePtr(new MSOPATHINFO(*this));
}

void MSOPATHINFO::load(XLS::CFRecord& record)
{
	_UINT16 mem = 0;
	
	record >> mem;

	unsigned char type = (mem >> 13 & 0x07);
	
	if (type <= 4)
	{
		m_eRuler	= (NSCustomShapesConvert::RulesType)type;
		m_nCount	= (mem & 0x1FFF);
		m_nCount	= (_UINT16)GetCountPoints2(m_eRuler, m_nCount);
		return;
	}

	type = (mem >> 8) & 0x1F;
	mem = mem & 0xFF;

	switch (type)
	{
	case 0x00:
		{
			m_eRuler = NSCustomShapesConvert::rtLineTo;
			break;
		}
	case 0x01:
		{
			m_eRuler = NSCustomShapesConvert::rtAngleEllipseTo;
			break;
		}
	case 0x02:
		{
			m_eRuler = NSCustomShapesConvert::rtAngleEllipse;
			break;
		}
	case 0x03:
		{
			m_eRuler = NSCustomShapesConvert::rtArcTo;
			break;
		}
	case 0x04:
		{
			m_eRuler = NSCustomShapesConvert::rtArc;
			break;
		}
	case 0x05:
		{
			m_eRuler = NSCustomShapesConvert::rtClockwiseArcTo;
			break;
		}
	case 0x06:
		{
			m_eRuler = NSCustomShapesConvert::rtClockwiseArc;
			break;
		}
	case 0x07:
		{
			m_eRuler = NSCustomShapesConvert::rtEllipticalQuadrX;
			break;
		}
	case 0x08:
		{
			m_eRuler = NSCustomShapesConvert::rtEllipticalQuadrY;
			break;
		}
	case 0x09:
		{
			m_eRuler = NSCustomShapesConvert::rtQuadrBesier;
			break;
		}
	case 0x0A:
		{
			m_eRuler = NSCustomShapesConvert::rtNoFill;
			break;
		}
	case 0x0B:
		{
			m_eRuler = NSCustomShapesConvert::rtNoStroke;
			break;
		}
	case 0x0C:
	case 0x10:
		{
			m_eRuler = NSCustomShapesConvert::rtLineTo;
			break;
		}
	case 0x0D:
	case 0x0E:
	case 0x0F:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x14:
		{
			m_eRuler = NSCustomShapesConvert::rtCurveTo;
			break;
		}
	case 0x15:
		{
			m_eRuler = NSCustomShapesConvert::rtFillColor;
			break;
		}
	case 0x16:
		{
			m_eRuler = NSCustomShapesConvert::rtLineColor;
			break;
		}
	default:
		{
			m_eRuler = NSCustomShapesConvert::rtCurveTo;
		}
	};

	m_nCount = (_UINT16)mem;
	m_nCount = (_UINT16)GetCountPoints2(m_eRuler, m_nCount);
}
//---------------------------------------------------------------------------------------------
MSOSG::MSOSG()
{
	cbElement = 4;
}
MSOSG::MSOSG(unsigned short cbElement_)
{
	cbElement = 4;

	if (cbElement_ == 0xfff0)
		cbElement = 2;
}

XLS::BiffStructurePtr MSOSG::clone()
{
	return XLS::BiffStructurePtr(new MSOSG(*this));
}

void MSOSG::load(XLS::CFRecord& record)
{
	_UINT16 ftType;
	record >> ftType;

	m_eType = NSCustomShapesConvert::FormulaType(ftType & 0x1FFF);

	m_param_type1 = (unsigned char)(ftType & 0x04);
	m_param_type2 = (unsigned char)(ftType & 0x02);
	m_param_type3 = (unsigned char)(ftType & 0x01);

	record >> m_param_value1 >> m_param_value2 >> m_param_value3;
}
//---------------------------------------------------------------------------------------------
ADJH::ADJH()
{
	cbElement = 4;
}

ADJH::ADJH(unsigned short cbElement_)
{
	cbElement = 4;

	if (cbElement_ == 0xfff0)
	{
		cbElement = 2;
	}
}

XLS::BiffStructurePtr ADJH::clone() 
{
	return XLS::BiffStructurePtr(new ADJH(*this));
}

void ADJH::load(XLS::CFRecord& record)
{
	_UINT32 flag;
	record >> flag;

	fahInverseX			= GETBIT(flag, 31);
	fahInverseY			= GETBIT(flag, 30);
	fahSwitchPosition	= GETBIT(flag, 29);
	fahPolar			= GETBIT(flag, 28);
	fahPin				= GETBIT(flag, 27);
	fahUnused			= GETBIT(flag, 26);
	fahxMin				= GETBIT(flag, 25);
	fahxMax				= GETBIT(flag, 24);
	fahyMin				= GETBIT(flag, 23);
	fahyMax				= GETBIT(flag, 22);
	fahxRange			= GETBIT(flag, 21);
	fahyRange			= GETBIT(flag, 20);
	fahPolarPin			= GETBIT(flag, 19);
	
	record >> apX >> apY >> xRange >> yRange;
	record >> xMin >> xMax >> yMin >> yMax;


}
//---------------------------------------------------------------------------------------------
void PVertices::ReadComplexData(XLS::CFRecord& record)
{
	record >> complex;
}

void PSegmentInfo::ReadComplexData(XLS::CFRecord& record)
{
	record >> complex;
}

void pGuides::ReadComplexData(XLS::CFRecord& record)
{
	record >> complex;
}
void pAdjustHandles::ReadComplexData(XLS::CFRecord& record)
{
	record >> complex;
}
} 
