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

#include "OfficeArtFOPTE.h"
#include "OfficeArtBlip.h"
#include <bitset>
#include "../../../../../../OOXML/Base/Unit.h"

#include "../../../../../../OfficeUtils/src/OfficeUtils.h"

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

		for ( size_t i = 0; i < bits.size(); i++ )
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

void OfficeArtFOPTE::load(IBinaryReader* _reader)
{
	unsigned short flags = _reader->ReadUInt16();
	op			= _reader->ReadUInt32();
	
	opid		= GETBITS(flags, 0, 13);
	fBid		= GETBIT(flags, 14);
	fComplex	= GETBIT(flags, 15);
}
void OfficeArtFOPTE::load(XLS::CFRecord& record)
{
	unsigned short flags;
	record >> flags >> op;

	opid		= GETBITS(flags, 0, 13);
	fBid		= GETBIT(flags, 14);
	fComplex	= GETBIT(flags, 15);
}
OfficeArtFOPTEPtr OfficeArtFOPTE::create(unsigned short opid)
{
	OfficeArtFOPTEPtr fopte;
	switch(opid)
	{
		case 0x0004:
			fopte = OfficeArtFOPTEPtr(new Rotation);
			break;
		case 0x007F:
			fopte = OfficeArtFOPTEPtr(new ProtectionBooleanProperties);
			break;
		case ODRAW::shadowOriginX:
		case ODRAW::shadowOriginY:
		case ODRAW::shadowScaleXToX:
		case ODRAW::shadowScaleXToY:
		case ODRAW::shadowScaleYToX:
		case ODRAW::shadowScaleYToY:
		case ODRAW::gtextSpacing:
			fopte = OfficeArtFOPTEPtr(new FixedPoint);
			break;
		case ODRAW::textBooleanProperties:
			fopte = OfficeArtFOPTEPtr(new TextBooleanProperties);
			break;
		case ODRAW::gtextUNICODE:
		case ODRAW::gtextFont:
			fopte = OfficeArtFOPTEPtr(new AnyString);
			break;
		case 0x00ff:
			fopte = OfficeArtFOPTEPtr(new GeometryTextBooleanProperties);
			break;
		case 0x0100:
			fopte = OfficeArtFOPTEPtr(new CropFromTop);
			break;
		case 0x0101:
			fopte = OfficeArtFOPTEPtr(new CropFromBottom);
			break;
		case 0x0102:
			fopte = OfficeArtFOPTEPtr(new CropFromLeft);
			break;
		case 0x0103:
			fopte = OfficeArtFOPTEPtr(new CropFromRight);
			break;
		case 0x0104:
			fopte = OfficeArtFOPTEPtr(new Pib);
			break;
		case 0x0105:
			fopte = OfficeArtFOPTEPtr(new AnyString);
			break;
		case 0x010B:
			fopte = OfficeArtFOPTEPtr(new PictureId);
			break;
		case 0x011A:
			fopte = OfficeArtFOPTEPtr(new PictureRecolor);
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
			fopte = OfficeArtFOPTEPtr(new PConnectionSites);
			break;
		case 0x0152:
			fopte = OfficeArtFOPTEPtr(new PConnectionSitesDir);
			break;
		case 0x0153:
		case 0x0154:
			fopte = OfficeArtFOPTEPtr(new OfficeArtFOPTE);
			break;
		case 0x0155:
			fopte = OfficeArtFOPTEPtr(new PAdjustHandles);
			break;
		case 0x0156:
			fopte = OfficeArtFOPTEPtr(new PGuides);
			break;
		case 0x0157:
			fopte = OfficeArtFOPTEPtr(new PInscribe);
			break;
		case 0x0158:
			fopte = OfficeArtFOPTEPtr(new OfficeArtFOPTE);//cxk
			break;
		case 0x017f:
			fopte = OfficeArtFOPTEPtr(new GeometryBooleanProperties);
			break;
		case 0x0180:
			fopte = OfficeArtFOPTEPtr(new FillType);
			break;
		case 0x0181:
			fopte = OfficeArtFOPTEPtr(new FillColor);
			break;
		case 0x0182:
			fopte = OfficeArtFOPTEPtr(new FillOpacity);
			break;
		case 0x0183:
			fopte = OfficeArtFOPTEPtr(new FillBackColor);
			break;
		case 0x0184:
			fopte = OfficeArtFOPTEPtr(new FillBackOpacity);
			break;
		case 0x0185:
			fopte = OfficeArtFOPTEPtr(new FillCrMod);
			break;
		case 0x0186:
			fopte = OfficeArtFOPTEPtr(new FillBlip);
			break;
		case 0x0187:
			fopte = OfficeArtFOPTEPtr(new AnyString);
			break;
		case 0x0188:
			fopte = OfficeArtFOPTEPtr(new FillBlipFlags);
			break;
		case 0x0189:
			fopte = OfficeArtFOPTEPtr(new FillWidth);
			break;
		case 0x018A:
			fopte = OfficeArtFOPTEPtr(new FillHeight);
			break;
		case 0x018B:
			fopte = OfficeArtFOPTEPtr(new FillAngle);
			break;
		case 0x018C:
			fopte = OfficeArtFOPTEPtr(new FillFocus);
			break;
		case 0x018D:
			fopte = OfficeArtFOPTEPtr(new FillToLeft);
			break;
		case 0x018E:
			fopte = OfficeArtFOPTEPtr(new FillToTop);
			break;
		case 0x018F:
			fopte = OfficeArtFOPTEPtr(new FillToRight);
			break;
		case 0x0190:
			fopte = OfficeArtFOPTEPtr(new FillToBottom);
			break;
		case 0x0191:
			fopte = OfficeArtFOPTEPtr(new FillRectLeft);
			break;
		case 0x0192:
			fopte = OfficeArtFOPTEPtr(new FillRectTop);
			break;
		case 0x0193:
			fopte = OfficeArtFOPTEPtr(new FillRectRight);
			break;
		case 0x0194:
			fopte = OfficeArtFOPTEPtr(new FillRectBottom);
			break;
		case 0x0195:
			fopte = OfficeArtFOPTEPtr(new FillDztype);
			break;
		case 0x0196:
			fopte = OfficeArtFOPTEPtr(new FillShadePreset);
			break;
		case 0x0197:
			fopte = OfficeArtFOPTEPtr(new FillShadeColors);
			break;
		case 0x0198:
			fopte = OfficeArtFOPTEPtr(new FillOriginX);
			break;
		case 0x0199:
			fopte = OfficeArtFOPTEPtr(new FillOriginY);
			break;
		case 0x019A:
			fopte = OfficeArtFOPTEPtr(new FillShapeOriginX);
			break;
		case 0x019B:
			fopte = OfficeArtFOPTEPtr(new FillShapeOriginY);
			break;
		case 0x019C:
			fopte = OfficeArtFOPTEPtr(new FillShadeType);
			break;
		case 0x019E:
			fopte = OfficeArtFOPTEPtr(new FillColorExt);
			break;
		case 0x019F:
			fopte = OfficeArtFOPTEPtr(new Reserved415);
			break;
		case 0x01A0:
			fopte = OfficeArtFOPTEPtr(new FillColorExtMod);
			break;
		case 0x01A1:
			fopte = OfficeArtFOPTEPtr(new Reserved417);
			break;
		case 0x01A2:
			fopte = OfficeArtFOPTEPtr(new FillBackColorExt);
			break;
		case 0x01A3:
			fopte = OfficeArtFOPTEPtr(new Reserved419);
			break;
		case 0x01A4:
			fopte = OfficeArtFOPTEPtr(new FillBackColorExtMod);
			break;
		case 0x01A5:
			fopte = OfficeArtFOPTEPtr(new Reserved421);
			break;
		case 0x01A6:
			fopte = OfficeArtFOPTEPtr(new Reserved422);
			break;
		case 0x01A7:
			fopte = OfficeArtFOPTEPtr(new Reserved423);
			break;
		case 0x01BF:
			fopte = OfficeArtFOPTEPtr(new FillStyleBooleanProperties);
			break;
		case 0x01C0:
			fopte = OfficeArtFOPTEPtr(new LineColor);
			break;
		case 0x01C1:
			fopte = OfficeArtFOPTEPtr(new LineOpacity);
			break;
		case 0x01C2:
			fopte = OfficeArtFOPTEPtr(new LineBackColor);
			break;
		case 0x01C3:
			fopte = OfficeArtFOPTEPtr(new LineCrMod);
			break;
		case 0x01C4:
			fopte = OfficeArtFOPTEPtr(new LineType);
			break;
		case 0x01C5:
			fopte = OfficeArtFOPTEPtr(new FillBlip);
			break;
		case 0x01CB:
			fopte = OfficeArtFOPTEPtr(new LineWidth);
			break;
		case 0x01CC:
			fopte = OfficeArtFOPTEPtr(new LineMiterLimit);
			break;
		case 0x01CD:
			fopte = OfficeArtFOPTEPtr(new LineStyle);
			break;
		case 0x01CE:
			fopte = OfficeArtFOPTEPtr(new LineDashing);
			break;
/*todo*/case 0x1D0:	//lineStartArrowhead
/*todo*/case 0x1D1:	//lineEndArrowhead
			fopte = OfficeArtFOPTEPtr(new OfficeArtFOPTE);
			break;
		case 0x01FF:
			fopte = OfficeArtFOPTEPtr(new LineStyleBooleanProperties);
			break;
		case ODRAW::shadowOpacity:
			fopte = OfficeArtFOPTEPtr(new ShadowOpacity);
			break;
		case ODRAW::shadowStyleBooleanProperties:
			fopte = OfficeArtFOPTEPtr(new ShadowStyleBooleanProperties);
			break;
		case 0x02bf:
			fopte = OfficeArtFOPTEPtr(new ThreeDObjectBooleanProperties);
			break;
/*todo*/case 0x0303: //cxstyle
			fopte = OfficeArtFOPTEPtr(new OfficeArtFOPTE);
			break;
		case 0x030C:
			fopte = OfficeArtFOPTEPtr(new XmlString);
			break;
		case 0x033F:
			fopte = OfficeArtFOPTEPtr(new ShapeBooleanProperties);
			break;
		case 0x0380:
		case 0x0381:
			fopte = OfficeArtFOPTEPtr(new AnyString);
			break;
		case 0x0382:
			fopte = OfficeArtFOPTEPtr(new PihlShape);
			break;
		case 0x0383:
			fopte = OfficeArtFOPTEPtr(new PWrapPolygonVertices);
			break;
		case 0x03A9:
			fopte = OfficeArtFOPTEPtr(new MetroBlob);
			break;
		case 0x03BF:
			fopte = OfficeArtFOPTEPtr(new GroupShapeBooleanProperties);
			break;
		case ODRAW::lTxid:
		case ODRAW::dxTextLeft:
		case ODRAW::dyTextTop:
		case ODRAW::dxTextRight:
		case ODRAW::dyTextBottom:
		case ODRAW::WrapText:
		case ODRAW::anchorText:
		case ODRAW::txflTextFlow:
		case ODRAW::cdirFont:
		case ODRAW::hspNext:
		case ODRAW::txdir:
		case ODRAW::gtextRTF:
		case ODRAW::gtextAlign:
		case ODRAW::gtextSize:
		case ODRAW::gtextCSSFont:
		case ODRAW::pictureContrast:
		case ODRAW::pictureBrightness:
		default:
			fopte = OfficeArtFOPTEPtr(new OfficeArtFOPTE);
			break;
	}
	return fopte;
}

OfficeArtFOPTEPtr OfficeArtFOPTE::load_and_create(IBinaryReader* _reader)
{
	if (!_reader) return OfficeArtFOPTEPtr();
	
	long pos = _reader->GetPosition();
	unsigned short opid = _reader->ReadUInt16();
	
	opid = GETBITS(opid, 0, 13);
	
	_reader->Seek(pos, 0);

	OfficeArtFOPTEPtr fopte = create(opid);

	if (fopte) fopte->load(_reader);
	return fopte;
}

OfficeArtFOPTEPtr OfficeArtFOPTE::load_and_create(XLS::CFRecord& record)
{
	const unsigned short* op = record.getCurData<unsigned short>();
	unsigned short opid = GETBITS(*op, 0, 13);

	OfficeArtFOPTEPtr fopte = create(opid);

	if (fopte) fopte->load(record);

	return fopte;
}
FixedPoint::FixedPoint()
{
	dVal = 0;
}
FixedPoint::FixedPoint(unsigned short cbElement_)
{
	dVal = 0;
}
void FixedPoint::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);

	short			Integral	= op >> 16;
	unsigned short Fractional	= op - (Integral << 16);

	dVal = Integral + (Fractional / 65536.0);
}
void FixedPoint::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);

	short			Integral	= op >> 16;
	unsigned short Fractional	= op - (Integral << 16);

	dVal = Integral + (Fractional / 65536.0);
}
void OfficeArtFOPTE::ReadComplexData(XLS::CFRecord& record)
{
	record.skipNunBytes(op); // default is to skip complex data
}
void OfficeArtFOPTE::ReadComplexData(IBinaryReader* reader)
{
	reader->ReadBytes( op, false ); // default is to skip complex data
}
void TextBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	set();
}
void TextBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void TextBooleanProperties::set()
{
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
	set();
}
void GeometryTextBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void GeometryTextBooleanProperties::set()
{
	fUsegFReverseRows	= GETBIT(op, 31);
	fUsefGtext			= GETBIT(op, 30);
	fUsegFVertical		= GETBIT(op, 29);
	fUsegFKern			= GETBIT(op, 28);
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
void GroupShapeBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void GroupShapeBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	set();
}
void GroupShapeBooleanProperties::set()
{
	fUsefLayoutInCell	= GETBIT(op, 31);
	fUsefIsBullet		= GETBIT(op, 30);
	fUsefStandardHR		= GETBIT(op, 29);
	fUsefNoshadeHR		= GETBIT(op, 28);
	fUsefHorizRule		= GETBIT(op, 27);
	fUsefUserDrawn		= GETBIT(op, 26);
	fUsefAllowOverlap	= GETBIT(op, 25);
	fUsefReallyHidden	= GETBIT(op, 24);
	fUsefScriptAnchor	= GETBIT(op, 23);
	fUsefEditedWrap		= GETBIT(op, 22);
	fUsefBehindDocument	= GETBIT(op, 21);
	fUsefOnDblClickNotify	= GETBIT(op, 20);
	fUsefIsButton		= GETBIT(op, 19);
	fUsefOneD			= GETBIT(op, 18);
	fUsefHidden			= GETBIT(op, 17);
	fUsefPrint			= GETBIT(op, 16);
	
	fLayoutInCell		= GETBIT(op, 15);
	fIsBullet			= GETBIT(op, 14);
	fStandardHR			= GETBIT(op, 13);
	fNoshadeHR			= GETBIT(op, 12);
	fHorizRule			= GETBIT(op, 11);
	fUserDrawn			= GETBIT(op, 10);
	fAllowOverlap		= GETBIT(op,  9);
	fReallyHidden		= GETBIT(op,  8);
	fScriptAnchor		= GETBIT(op,  7);
	fEditedWrap			= GETBIT(op,  6);
	fBehindDocument		= GETBIT(op,  5);
	fOnDblClickNotify	= GETBIT(op,  4);
	fIsButton			= GETBIT(op,  3);
	fOneD				= GETBIT(op,  2);
	fHidden				= GETBIT(op,  1);
	fPrint				= GETBIT(op,  0);
}
void FillShadeType::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	
	msoshadeNone		= GETBIT(op, 0);
	msoshadeGamma		= GETBIT(op, 1);
	msoshadeSigma		= GETBIT(op, 2);
	msoshadeBand		= GETBIT(op, 3);
	msoshadeOneColor	= GETBIT(op, 4);
}
void FillShadeType::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);

	msoshadeNone		= GETBIT(op, 0);
	msoshadeGamma		= GETBIT(op, 1);
	msoshadeSigma		= GETBIT(op, 2);
	msoshadeBand		= GETBIT(op, 3);
	msoshadeOneColor	= GETBIT(op, 4);
}
void FillStyleBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	set();
}
void FillStyleBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void FillStyleBooleanProperties::set()
{
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
void BlipBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	set();
}
void BlipBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void BlipBooleanProperties::set()
{
	fUsefPicturePreserveGrays	= GETBIT(op,22);
	fUsefRewind					= GETBIT(op,21);
	fUsefLooping				= GETBIT(op,20);
	fUsefNoHitTestPicture		= GETBIT(op,19);
	fUsefPictureGray			= GETBIT(op,18);
	fUsefPictureBiLevel			= GETBIT(op,17);
	fUsefPictureActive			= GETBIT(op,16);

	fPicturePreserveGrays	= GETBIT(op, 6);
	fRewind					= GETBIT(op, 5);
	fLooping				= GETBIT(op, 4);
	fNoHitTestPicture		= GETBIT(op, 3);
	fPictureGray			= GETBIT(op, 2);
	fPictureBiLevel			= GETBIT(op, 1);
	fPictureActive			= GETBIT(op, 0);
}
void FillBlip::ReadComplexData(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rh_child;
	record >> rh_child;
	size_t child_beginning_ptr = record.getRdPtr();
	record.RollRdPtrBack(rh_child.size());

	blip = OfficeArtBlipPtr(new OfficeArtBlip(rh_child.recType));
	blip->loadFields(record);
}
void FillBlip::ReadComplexData(IBinaryReader* reader)
{
	long pos = reader->GetPosition();
	
	OfficeArtRecordHeader rh_child;
	rh_child.load(reader);
	
	reader->Seek(pos, 0);

	blip = OfficeArtBlipPtr(new OfficeArtBlip(rh_child.recType));
	blip->loadFields(reader);
}
void AnyString::ReadComplexData(IBinaryReader* reader)
{
	std::wstring tmp;
	unsigned char* pData = reader->ReadBytes(op, true);
#if defined(_WIN32) || defined(_WIN64)
		tmp = std::wstring((wchar_t*)pData, op / 2);
#else
        tmp = convertUtf16ToWString((UTF16*)pData, op / 2);
#endif
	delete []pData;

	if (false == tmp.empty())
	{
        _INT32 i, length = (std::min)(op / 2, (_INT32)tmp.length());

		for (i = 0; i < length; i++)
		{
			wchar_t val = tmp.at(i);

			if (val == 0) break;

			if (val < 14 )
			{
				std::wstring hex = STR::int2hex_wstr(val);
				if (false == hex.empty())
				{
					string_ += L"&#x" + hex.substr(hex.length() - 1) + L";";
				}
			}
			else
			{
                switch(val)
                {
					case '&':  string_.append(L"&amp;");      break;
                    case '\"': string_.append(L"&quot;");     break;
                    case '\'': string_.append(L"&apos;");     break;
                    case '<':  string_.append(L"&lt;");       break;
                    case '>':  string_.append(L"&gt;");       break;
                    default:
						string_ += val;
				}
			}
		}
	}
}
void AnyString::ReadComplexData(XLS::CFRecord& record)
{	
	std::wstring tmp;
#if defined(_WIN32) || defined(_WIN64)
        tmp = std::wstring(record.getCurData<wchar_t>(), op / 2);
#else
        tmp = convertUtf16ToWString(record.getCurData<UTF16>(), op / 2);
#endif
	record.skipNunBytes(op);
	
	if (false == tmp.empty())
	{
        _INT32 i, length = (std::min)(op / 2, (_INT32)tmp.length());

		for (i = 0; i < length; i++)
		{
			wchar_t val = tmp.at(i);

			if (val == 0) break;

			if (val < 14 )
			{
				std::wstring hex = STR::int2hex_wstr(val);
				if (false == hex.empty())
				{
					string_ += L"&#x" + hex.substr(hex.length() - 1) + L";";
				}
			}
			else
			{
                switch(val)
                {
					case '&':  string_.append(L"&amp;");      break;
                    case '\"': string_.append(L"&quot;");     break;
                    case '\'': string_.append(L"&apos;");     break;
                    case '<':  string_.append(L"&lt;");       break;
                    case '>':  string_.append(L"&gt;");       break;
                    default:
						string_ += val;
				}
			}
		}
	}
}

void FillShadeColors::ReadComplexData(XLS::CFRecord& record)
{
	complex.op = op;
	record >> complex;
}
void FillShadeColors::ReadComplexData(IBinaryReader* reader)
{
	complex.op = op;
	complex.load(reader);
}
void ProtectionBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void ProtectionBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	set();
}
void ProtectionBooleanProperties::set()
{
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

void LineStyleBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void LineStyleBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	set();
}
void LineStyleBooleanProperties::set()
{
	fNoLineDrawDash			= GETBIT(op, 0);
	fLineFillShape			= GETBIT(op, 1);
	fHitTestLine			= GETBIT(op, 2);
	fLine					= GETBIT(op, 3);
	fArrowheadsOK			= GETBIT(op, 4);
	fInsetPenOK				= GETBIT(op, 5);
	fInsetPen				= GETBIT(op, 6);
	fLineOpaqueBackColor	= GETBIT(op, 9);
	fUsefNoLineDrawDash		= GETBIT(op, 16);
	fUsefLineFillShape		= GETBIT(op, 17);
	fUsefHitTestLine		= GETBIT(op, 18);
	fUsefLine				= GETBIT(op, 19);
	fUsefArrowheadsOK		= GETBIT(op, 20);
	fUsefInsetPenOK			= GETBIT(op, 21);
	fUsefInsetPen			= GETBIT(op, 22);
	fUsefLineOpaqueBackColor= GETBIT(op, 25);
}
void ShadowStyleBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	set();
}
void ShadowStyleBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void ShadowStyleBooleanProperties::set()
{
	fshadowObscured		= GETBIT(op, 0);
	fShadow				= GETBIT(op, 1);
	fUsefshadowObscured = GETBIT(op, 16);
	fUsefShadow			= GETBIT(op, 17);
}
void GeometryBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void GeometryBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	set();
}
void GeometryBooleanProperties::set()
{
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
void ThreeDObjectBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void ThreeDObjectBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	set();
}
void ThreeDObjectBooleanProperties::set()
{
	fUsef3D						=	GETBIT(op, 0);
	fUsefc3DMetallic			=	GETBIT(op, 1);
	fUsefc3DUseExtrusionColor	=	GETBIT(op, 2);
	fUsefc3DLightFace			=	GETBIT(op, 3);

	// 12 unused

	f3D							=	GETBIT(op, 16);
	fc3DMetallic				=	GETBIT(op, 17);
	fc3DUseExtrusionColor		=	GETBIT(op, 18);
	fc3DLightFace				=	GETBIT(op, 19);
}
void ThreeDStyleBooleanProperties::load(IBinaryReader* reader)
{
	OfficeArtFOPTE::load(reader);
	set();
}
void ThreeDStyleBooleanProperties::load(XLS::CFRecord& record)
{
	OfficeArtFOPTE::load(record);
	set();
}
void ThreeDStyleBooleanProperties::set()
{
	fUsefc3DConstrainRotation	=	GETBIT(op, 0);
	fUsefc3DRotationCenterAuto	=	GETBIT(op, 1);
	fUsefc3DParallel			=	GETBIT(op, 2);
	fUsefc3DKeyHarsh			=	GETBIT(op, 3);
	fUsefc3DFillHarsh			=	GETBIT(op, 4);

	fc3DConstrainRotation		=	GETBIT(op, 16);
	fc3DRotationCenterAuto		=	GETBIT(op, 17);
	fc3DParallel				=	GETBIT(op, 18);
	fc3DKeyHarsh				=	GETBIT(op, 19);
	fc3DFillHarsh				=	GETBIT(op, 20);
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
void IHlink::load(IBinaryReader* reader)
{
	CLSID_StdHlink.Data1 = reader->ReadUInt32();
	CLSID_StdHlink.Data2 = reader->ReadUInt16();
	CLSID_StdHlink.Data3 = reader->ReadUInt16();
    
	unsigned char* pData = reader->ReadBytes(8, true);
	memcpy(CLSID_StdHlink.getData4(), pData, 8) ;
	delete pData;

	hyperlink.load(reader);
}
void PihlShape::ReadComplexData(XLS::CFRecord& record)
{
	int pos = record.getRdPtr();

	record >> complex;
}
void PihlShape::ReadComplexData(IBinaryReader* reader)
{
	complex.load(reader);
}
void XmlString::ReadComplexData(IBinaryReader* reader)
{
	unsigned char* pData = reader->ReadBytes(op, true);
	
	data = std::string((char*)pData, op);

	delete []pData;
}
void XmlString::ReadComplexData(XLS::CFRecord& record)
{
	data = std::string(record.getCurData<char>(), op);

	record.skipNunBytes(op);
}
void MetroBlob::ReadComplexData(IBinaryReader* reader)
{
	unsigned char* pData = reader->ReadBytes(op, true);
	
	data = std::make_pair(boost::shared_array<unsigned char>(pData), op);
}
void MetroBlob::ReadComplexData(XLS::CFRecord& record)
{
	int pos = record.getRdPtr();

	data = std::make_pair(boost::shared_array<unsigned char>(new unsigned char[op]), op);

	memcpy(data.first.get(), record.getCurData<unsigned char>(), op);

	record.skipNunBytes(op);
//------------------------------
	XLS::GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();

	NSFile::CFileBinary file;

	std::wstring tempFileName = global_info->tempDirectory + FILE_SEPARATOR_STR + L"tempMetroBlob.zip";

    if (file.CreateFileW(tempFileName))
	{
		file.WriteFile(data.first.get(), data.second);
		file.CloseFile();
	}
	COfficeUtils officeUtils(NULL);

	BYTE *utf8Data = NULL; 
	ULONG utf8DataSize = 0;
	if (S_OK != officeUtils.LoadFileFromArchive(tempFileName, L"drs/shapexml.xml", &utf8Data, utf8DataSize))
	{
		if (S_OK != officeUtils.LoadFileFromArchive(tempFileName, L"drs/diagrams/drawing1.xml", &utf8Data, utf8DataSize))
		{
			if (S_OK != officeUtils.LoadFileFromArchive(tempFileName, L"drs/connectorxml.xml", &utf8Data, utf8DataSize))
			{
				officeUtils.LoadFileFromArchive(tempFileName, L"drs/groupshapexml.xml", &utf8Data, utf8DataSize);
			}
		}
	}

	if (utf8Data && utf8DataSize > 0)
	{
		xmlString = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(utf8Data, utf8DataSize);

		delete []utf8Data;
	}
	NSFile::CFileBinary::Remove(tempFileName);
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
void MSOPOINT::load(IBinaryReader* reader)
{
	if (cbElement == 4)
	{
		x = reader->ReadInt32();
		y = reader->ReadInt32();
	}
	else
	{
		unsigned short x_, y_;
		x_ = reader->ReadInt16();
		y_ = reader->ReadInt16();
		
		x = x_;
		y = y_;
	}
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
MSORECT::MSORECT()
{
	cbElement = 8;
}

MSORECT::MSORECT(unsigned short cbElement_)
{
	cbElement = 8;

	if (cbElement_ == 0xfff0)
	{
		cbElement = 4;
	}
}

XLS::BiffStructurePtr MSORECT::clone() 
{
	return XLS::BiffStructurePtr(new MSORECT(*this));
}
void MSORECT::load(IBinaryReader* reader)
{
	if (cbElement == 8)
	{
		l = reader->ReadInt32();
		t = reader->ReadInt32();
		r = reader->ReadInt32();
		b = reader->ReadInt32();
	}
	else
	{
		unsigned short l_, t_, r_, b_;
		
		l_ = reader->ReadInt16();
		t_ = reader->ReadInt16();
		r_ = reader->ReadInt16();
		b_ = reader->ReadInt16();
		
		l = l_;
		t = t_;
		r = r_;
		b = b_;
	}
}
void MSORECT::load(XLS::CFRecord& record)
{
	if (cbElement == 8)
	{
		record >> l >> t >> r >> b;
	}
	else
	{
		unsigned short l_, t_, r_, b_;
		record >> l_ >> t_ >> r_ >> b_;
		
		l = l_;
		t = t_;
		r = r_;
		b = b_;
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
void MSOPATHINFO::load(IBinaryReader* reader)
{
	_UINT16 mem = reader->ReadInt16();
	
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

void MSOSG::load(IBinaryReader* reader)
{
	_UINT16 ftType = reader->ReadUInt16();
	
	m_eType = NSCustomShapesConvert::FormulaType(ftType & 0x1FFF);

	m_param_type1 = (unsigned char)(ftType & 0x04);
	m_param_type2 = (unsigned char)(ftType & 0x02);
	m_param_type3 = (unsigned char)(ftType & 0x01);

	m_param_value1 = reader->ReadUInt16();
	m_param_value2 = reader->ReadUInt16();
	m_param_value3 = reader->ReadUInt16();
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
	cbElement = cbElement_;
}

XLS::BiffStructurePtr ADJH::clone() 
{
	return XLS::BiffStructurePtr(new ADJH(*this));
}

void ADJH::load(IBinaryReader* reader)
{
	_UINT32 flag = reader->ReadUInt32();
	
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

	cbElement -= 4;

	if (cbElement == 4)
	{
		_UINT16 x, y;
		x = reader->ReadUInt16();
		y = reader->ReadUInt16();
		
		apX	= x;
		apY = y;

		cbElement -= 4;
	}
	else 
	{	
		apX = reader->ReadUInt32();
		apY = reader->ReadUInt32();
	
		cbElement -= 8;
	}
	
	if (cbElement < 1) return;
	
	if (fahxRange)	xRange =  reader->ReadInt16(); 
	if (fahyRange)	yRange =  reader->ReadInt16(); 
	if (fahxMin)	xMin =  reader->ReadInt16(); 
	if (fahxMax)	xMax =  reader->ReadInt16(); 
	if (fahyMin)	yMin =  reader->ReadInt16(); 
	if (fahyMax)	yMax =  reader->ReadInt16(); 
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

	cbElement -= 4;

	if (cbElement == 4)
	{
		_UINT16 x, y;
		record >> x >> y;
		
		apX	= x;
		apY = y;

		cbElement -= 4;
	}
	else 
	{	
		record >> apX >> apY;
	
		cbElement -= 8;
	}
	
	if (cbElement < 1) return;
	
	if (fahxRange)	record >> xRange; 
	if (fahyRange)	record >> yRange;
	if (fahxMin)	record >> xMin;
	if (fahxMax)	record >> xMax;
	if (fahyMin)	record >> yMin;
	if (fahyMax)	record >> yMax;
}
//---------------------------------------------------------------------------------------------
void PVertices::ReadComplexData(XLS::CFRecord& record)
{
	complex.op = op;
	int pos = record.getRdPtr();

	record >> complex;
}
void PVertices::ReadComplexData(IBinaryReader* reader)
{
	complex.op = op;
	complex.load(reader);
}
void PSegmentInfo::ReadComplexData(XLS::CFRecord& record)
{
	complex.op = op;
	int pos = record.getRdPtr();

	record >> complex;
}
void PSegmentInfo::ReadComplexData(IBinaryReader* reader)
{
	complex.op = op;
	complex.load(reader);
}
void PGuides::ReadComplexData(XLS::CFRecord& record)
{
	complex.op = op;
	int pos = record.getRdPtr();

	record >> complex;
}
void PGuides::ReadComplexData(IBinaryReader* reader)
{
	complex.op = op;
	complex.load(reader);
}
void PAdjustHandles::ReadComplexData(XLS::CFRecord& record)
{
	complex.op = op;

	int pos = record.getRdPtr();

	record >> complex;
}
void PAdjustHandles::ReadComplexData(IBinaryReader* reader)
{
	complex.op = op;
	complex.load(reader);
}
void PConnectionSites::ReadComplexData(XLS::CFRecord& record)
{
	complex.op = op;
	int pos = record.getRdPtr();

	record >> complex;
}
void PConnectionSites::ReadComplexData(IBinaryReader* reader)
{
	complex.op = op;
	complex.load(reader);
}
void PConnectionSitesDir::ReadComplexData(XLS::CFRecord& record)
{
	complex.op = op;
	int pos = record.getRdPtr();

	record >> complex;
}
void PConnectionSitesDir::ReadComplexData(IBinaryReader* reader)
{
	complex.op = op;
	complex.load(reader);
}
void PInscribe::ReadComplexData(XLS::CFRecord& record)
{
	complex.op = op;
	int pos = record.getRdPtr();

	record >> complex;
}
void PInscribe::ReadComplexData(IBinaryReader* reader)
{
	complex.op = op;
	complex.load(reader);
}
void PWrapPolygonVertices::ReadComplexData(XLS::CFRecord& record)
{
	complex.op = op;
	int pos = record.getRdPtr();

	record >> complex;
}
void PWrapPolygonVertices::ReadComplexData(IBinaryReader* reader)
{
	complex.op = op;
	complex.load(reader);
}

} 
