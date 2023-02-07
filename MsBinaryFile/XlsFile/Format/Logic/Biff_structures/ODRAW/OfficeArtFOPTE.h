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
#pragma once

#include "../BiffStructure.h"
#include "../HyperlinkObject.h"
#include "../FixedPoint.h"

#include "OfficeArtCOLORREF.h"
#include "OfficeArtBlip.h"
#include "IMsoArray.h"
#include "MSOSHADECOLOR.h"
#include "MSO_enums.h"

#include "../../../../../Common/Vml/PPTShape/Enums.h"
#include "../../../../../Common/ODraw/GraphicsPath.h"
#include "../../../../../Common/ODraw/FormulaShape.h"

namespace XLS
{
	class CFRecord;
}

namespace ODRAW
{

class OfficeArtFOPTE;
typedef boost::shared_ptr<OfficeArtFOPTE> OfficeArtFOPTEPtr;

class OfficeArtFOPTE : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFOPTE)
protected:
	virtual void load(XLS::CFRecord& record);
	void load(IBinaryReader* reader);
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType type = XLS::typeOfficeArtFOPTE;

	static OfficeArtFOPTEPtr load_and_create(XLS::CFRecord& record);
	static OfficeArtFOPTEPtr load_and_create(IBinaryReader* reader);
	
	static OfficeArtFOPTEPtr create(unsigned short opid);
	
	virtual void ReadComplexData(XLS::CFRecord& record);	
	virtual void ReadComplexData(IBinaryReader* reader);

	unsigned short opid;
	bool fBid;
	bool fComplex;
    _INT32 op;
};

class FillColor : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillColor)
};

class FixedPoint : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FixedPoint)
	
	FixedPoint();
	FixedPoint(unsigned short cbElement_); //fixed always!!
	
	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);
	
	double dVal;
};

class TextBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(TextBooleanProperties)
	
	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	bool fFitShapeToText;
	bool fAutoTextMargin;
	bool fSelectText;

	bool fUsefFitShapeToText;
	bool fUsefAutoTextMargin;
	bool fUsefSelectText;
private:
	void set();
};
class ThreeDStyleBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ThreeDStyleBooleanProperties)
	
	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	bool fUsefc3DConstrainRotation;
	bool fUsefc3DRotationCenterAuto;
	bool fUsefc3DParallel;
	bool fUsefc3DKeyHarsh;
	bool fUsefc3DFillHarsh;

	bool fc3DConstrainRotation;
	bool fc3DRotationCenterAuto;
	bool fc3DParallel;
	bool fc3DKeyHarsh;
	bool fc3DFillHarsh;
private:
	void set();
};
class ThreeDObjectBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ThreeDObjectBooleanProperties)
	
	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);
	
	bool fUsef3D;
	bool fUsefc3DMetallic;
	bool fUsefc3DUseExtrusionColor;
	bool fUsefc3DLightFace;

	bool f3D;
	bool fc3DMetallic;
	bool fc3DUseExtrusionColor;
	bool fc3DLightFace;
private:
	void set();
};
class GeometryTextBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(TextBooleanProperties)
	
	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);
	
	bool gFReverseRows;
	bool fGtext;
	bool fVertical;
	bool fKern;
	bool fStretch;
	bool fTight;
	bool fBestFit;
	bool fShrinkFit;
	bool fNormalize;
	bool fDxMeasure;
	bool fBold;
	bool fItalic;
	bool fUnderline;
	bool fShadow;
	bool fSmallcaps;
	bool fStrikethrough;

	bool fUsegFReverseRows;
	bool fUsefGtext	;
	bool fUsegFVertical	;
	bool fUsegFKern	;
	bool fUsegTight	;
	bool fUsegFStretch	;
	bool fUsegFShrinkFit;
	bool fUsegFBestFit	;
	bool fUsegFNormalize;
	bool fUsegFDxMeasure;
	bool fUsegFBold		;
	bool fUsegFItalic	;
	bool fUsegFUnderline	;
	bool fUsegFShadow	;
	bool fUsegFSmallcaps	;
	bool fUsegFStrikethrough;
private:
	void set();
};

class GroupShapeBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(GroupShapeBooleanProperties)
	
	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	bool fUsefLayoutInCell;
	bool fUsefIsBullet;
	bool fUsefStandardHR;
	bool fUsefNoshadeHR;
	bool fUsefHorizRule;
	bool fUsefUserDrawn;
	bool fUsefAllowOverlap;
	bool fUsefReallyHidden;
	bool fUsefScriptAnchor;
	bool fUsefEditedWrap;
	bool fUsefBehindDocument;
	bool fUsefOnDblClickNotify;
	bool fUsefIsButton;
	bool fUsefOneD;
	bool fUsefHidden;
	bool fUsefPrint;
	
	bool fLayoutInCell;
	bool fIsBullet;
	bool fStandardHR;
	bool fNoshadeHR;
	bool fHorizRule;
	bool fUserDrawn;
	bool fAllowOverlap;
	bool fReallyHidden;
	bool fScriptAnchor;
	bool fEditedWrap;
	bool fBehindDocument;
	bool fOnDblClickNotify;
	bool fIsButton;
	bool fOneD;
	bool fHidden;
	bool fPrint;
private:
	void set();
};

class LineColor : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineColor)
};

class FillType : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillType)
};

class Pib : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Pib)
};

class PictureId : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PictureId)
};

class PictureRecolor : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PictureRecolor)
};


class FillOpacity : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillOpacity)
};

class FillBackColor : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillBackColor)
};

class FillBackOpacity : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillBackOpacity)
};

class FillCrMod : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillCrMod)
};

class FillWidth : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillWidth)
};

class FillShadeType : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillShadeType)
	
	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);
		
	bool msoshadeNone;
	bool msoshadeGamma;
	bool msoshadeSigma;
	bool msoshadeBand;
	bool msoshadeOneColor;
};

class FillStyleBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillStyleBooleanProperties)
	
	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	bool fNoFillHitTest;
	bool fillUseRect;
	bool fillShape;
	bool fHitTestFill;
	bool fFilled ;
	bool fUseShapeAnchor;
	bool fRecolorFillAsPicture;
	bool fUsefNoFillHitTest;
	bool fUsefillUseRect;
	bool fUsefillShape;
	bool fUsefHitTestFill;
	bool fUsefFilled;
	bool fUsefUseShapeAnchor;
	bool fUsefRecolorFillAsPicture;
private:
	void set();
};

class FillBlip : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillBlip)

	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	OfficeArtBlipPtr blip;
};
class BlipBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(BlipBooleanProperties)

	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	bool fUsefPicturePreserveGrays;
	bool fUsefRewind;
	bool fUsefLooping;
	bool fUsefNoHitTestPicture;
	bool fUsefPictureGray;
	bool fUsefPictureBiLevel;
	bool fUsefPictureActive;

	bool fPicturePreserveGrays;
	bool fRewind;
	bool fLooping;
	bool fNoHitTestPicture;
	bool fPictureGray;
	bool fPictureBiLevel;
	bool fPictureActive;
private:
	void set();
};
class ShapeBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ShapeBooleanProperties)
};

class AnyString : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AnyString)

	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	std::wstring string_;
};


class FillBlipFlags : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillBlipFlags)
	//enum MSOBLIPFLAGS
	//{
	//	msoblipflagComment		= 0x00000000,
	//	msoblipflagFile			= 0x00000001,
	//	msoblipflagURL			= 0x00000002,
	//	msoblipflagDoNotSave	= 0x00000004,
	//	msoblipflagLinkToFile	= 0x00000008,
	//};
};

class FillHeight : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillHeight)
};

class FillAngle : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillAngle)

};

class FillFocus : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillFocus)
};

class FillToLeft : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillToLeft)
};

class FillToTop : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillToTop)
};

class FillToRight : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillToRight)
};

class FillToBottom : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillToBottom)
};

class FillRectLeft : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillRectLeft)
};

class FillRectTop : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillRectTop)
};

class FillRectRight : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillRectRight)
};

class FillRectBottom : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillRectBottom)
};

class FillDztype : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillDztype)
};

class FillShadePreset : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillShadePreset)
};

class FillShadeColors : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillShadeColors)

	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	IMsoArray<MSOSHADECOLOR> complex;
};


class FillShapeOriginX : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillShapeOriginX)
};

class FillShapeOriginY : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillShapeOriginY)
};

class FillOriginX : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillOriginX)
};

class FillOriginY : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillOriginY)
};

class FillColorExt : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillColorExt)
};

class Reserved415 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Reserved415)
};

class Reserved417 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Reserved417)
};

class Reserved419 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Reserved419)
};

class Reserved421 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Reserved421)
};

class Reserved422 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Reserved422)
};

class Reserved423 : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Reserved423)
};

class FillBackColorExt : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillBackColorExt)
};

class FillColorExtMod : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillColorExtMod)
};

class FillBackColorExtMod : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FillBackColorExtMod)
};

class ProtectionBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ProtectionBooleanProperties)

	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	bool fLockAgainstGrouping ;
	bool fLockAdjustHandles ;
	bool fLockText ;
	bool fLockVertices ;
	bool fLockCropping ;
	bool fLockAgainstSelect ;
	bool fLockPosition ;
	bool fLockAspectRatio ;
	bool fLockRotation ;
	bool fLockAgainstUngrouping ;
	bool fUsefLockAgainstGrouping ;
	bool fUsefLockAdjustHandles ;
	bool fUsefLockText ;
	bool fUsefLockVertices ;
	bool fUsefLockCropping ;
	bool fUsefLockAgainstSelect;
	bool fUsefLockPosition ;
	bool fUsefLockAspectRatio;
	bool fUsefLockRotation ;
	bool fUsefLockAgainstUngrouping;
private:
	void set();
};

class LineStyleBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineStyleBooleanProperties)

	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	bool fNoLineDrawDash;
	bool fLineFillShape;
	bool fHitTestLine ;
	bool fLine ;
	bool fArrowheadsOK;
	bool fInsetPenOK ;
	bool fInsetPen ;
	bool fLineOpaqueBackColor;
	bool fUsefNoLineDrawDash;
	bool fUsefLineFillShape ;
	bool fUsefHitTestLine ;
	bool fUsefLine;
	bool fUsefArrowheadsOK ;
	bool fUsefInsetPenOK;
	bool fUsefInsetPen;
	bool fUsefLineOpaqueBackColor ;
private:
	void set();
};

class ShadowStyleBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ShadowStyleBooleanProperties)
	
	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	bool fshadowObscured ;
	bool fShadow ;
	bool fUsefshadowObscured ;
	bool fUsefShadow ;
private:
	void set();
};

class GeometryBooleanProperties : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(GeometryBooleanProperties)
	
	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	bool fUsefShadowOK;
	bool fUsef3DOK ;
	bool fUsefLineOK ;
	bool fUsefGtextOK ;
	bool fUsefFillShadeShapeOK ;
	bool fUsefFillOK ;

	bool fShadowOK;
	bool f3DOK;
	bool fLineOK;
	bool fGtextOK;
	bool fFillShadeShapeOK;
	bool fFillOK;
private:
	void set();
};

class LineWidth : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineWidth)
};

class LineMiterLimit : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineMiterLimit)
};

class LineDashing : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineDashing)
};

class LineStyle : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineStyle)
};

class CropFromBottom : public FixedPoint 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CropFromBottom)
};

class CropFromTop : public FixedPoint 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CropFromTop)
};

class CropFromLeft : public FixedPoint 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CropFromLeft)
};
class CropFromRight : public FixedPoint 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CropFromRight)
};

class Rotation : public FixedPoint 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Rotation)
};
class OfficeArtShapeRectCoord : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtShapeRectCoord)
};

class ShadowOpacity : public FixedPoint
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ShadowOpacity)
};

class MSOPOINT : public XLS::BiffStructure  
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(MSOPOINT)
	
	MSOPOINT(unsigned short cbElement_);
	MSOPOINT();

	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	static const XLS::ElementType	type = XLS::typeOfficeArtRecord;

    _INT32 x;
    _INT32 y;

	int cbElement;
};

class MSORECT : public XLS::BiffStructure  
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(MSORECT)
	
	MSORECT(unsigned short cbElement_);
	MSORECT();

	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	static const XLS::ElementType	type = XLS::typeOfficeArtRecord;

    _INT32 l;
    _INT32 t;
    _INT32 r;
    _INT32 b;

	int cbElement;
};


class MSOPATHINFO : public XLS::BiffStructure 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(MSOPATHINFO)

	MSOPATHINFO();
	MSOPATHINFO(unsigned short cbElement_);

	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	static const XLS::ElementType	type = XLS::typeOfficeArtRecord;

	NSCustomShapesConvert::RulesType	m_eRuler;
	_UINT16					m_nCount;
	
	int cbElement;
};
class MSOSG : public XLS::BiffStructure 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(MSOSG)

	MSOSG();
	MSOSG(unsigned short cbElement_);

	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	static const XLS::ElementType	type = XLS::typeOfficeArtRecord;

	NSCustomShapesConvert::FormulaType	m_eType;

	unsigned char				m_param_type1;
	unsigned char				m_param_type2;
	unsigned char				m_param_type3;

	_UINT16						m_param_value1;
	_UINT16						m_param_value2;
	_UINT16						m_param_value3;
	
	int cbElement;
};
class ADJH : public XLS::BiffStructure 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ADJH)

	ADJH();
	ADJH(unsigned short cbElement_);

	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void load(IBinaryReader* reader);

	static const XLS::ElementType	type = XLS::typeOfficeArtRecord;

	int cbElement;

	_UINT32 apX;
	_UINT32 apY;
	_INT32	xRange;
	_INT32	yRange;
	_INT32	xMin;
	_INT32	xMax;
	_INT32	yMin;
	_INT32	yMax;

//-------------------------
	bool fahInverseX;
	bool fahInverseY;
	bool fahSwitchPosition;
	bool fahPolar;
	bool fahPin;
	bool fahUnused;
	bool fahxMin;
	bool fahxMax;
	bool fahyMin;
	bool fahyMax;
	bool fahxRange;
	bool fahyRange;
	bool fahPolarPin;
};
class ShapePath : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ShapePath)

	enum msoShapePathType
	{
		msoshapeLines  =0, 
		msoshapeLinesClosed,
		msoshapeCurves,
		msoshapeCurvesClosed,
		msoshapeComplex
	};
};

class PVertices : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PVertices)

	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	IMsoArray<MSOPOINT> complex;
};

class PSegmentInfo : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PSegmentInfo)

	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	IMsoArray<MSOPATHINFO> complex;
};
class PGuides : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PGuides)

	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	IMsoArray<MSOSG> complex;
};

class AdjustValue : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(AdjustValue)
};

class PAdjustHandles : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PAdjustHandles)
	
	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	IMsoArray<ADJH> complex;
};

class PConnectionSites : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PConnectionSites)
	
	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	IMsoArray<MSOPOINT> complex;
};

class PConnectionSitesDir : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PConnectionSitesDir)
	
	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	IMsoArray<OSHARED::FixedPoint> complex;
};

class PInscribe : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PInscribe)
	
	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	IMsoArray<MSORECT> complex;
};

class PWrapPolygonVertices : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PWrapPolygonVertices)
	
	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	IMsoArray<MSOPOINT> complex;
}; 
//class cxk : public OfficeArtFOPTE 
//{
//	BASE_STRUCTURE_DEFINE_CLASS_NAME(Cxk)
//	
//	virtual void ReadComplexData(XLS::CFRecord& record);
//
//	IMsoArray<MSOCXK> complex;
//};
class LineOpacity : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineOpacity)
};

class LineBackColor : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineBackColor)
};

class LineCrMod : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineCrMod)
};
class LineType : public OfficeArtFOPTE 
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(LineType)
};

class IHlink;
typedef boost::shared_ptr<IHlink> IHlinkPtr;

class IHlink : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(IHlink)

	IHlink(){}

	XLS::BiffStructurePtr clone();

	void load(XLS::CFRecord& record);
	void load(IBinaryReader* reader);
	
	static const XLS::ElementType	type = XLS::typeIHLink;

	_GUID_							CLSID_StdHlink;
	OSHARED::HyperlinkObject		hyperlink;
};

class PihlShape : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PihlShape)

	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	IHlink complex;
};

class MetroBlob : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(MetroBlob)

	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	std::pair<boost::shared_array<unsigned char>, _INT32> data;

	std::wstring xmlString;
};
class XmlString : public OfficeArtFOPTE
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(XmlString)

	virtual void ReadComplexData(XLS::CFRecord& record);
	virtual void ReadComplexData(IBinaryReader* reader);

	std::pair<boost::shared_array<char>, size_t> data; 
};
class PathParser
{
public:

	PathParser (std::vector<MSOPATHINFO> &arSegments, std::vector<MSOPOINT>& arPoints, std::vector<MSOSG> & arGuides, _CP_OPT(size_t) cx, _CP_OPT(size_t) cy)
		: m_arSegments(arSegments)
	{
		if (!cx) cx = 0xffff;
		if (!cy) cy = 0xffff;

		LONG lMinF = (_INT32)0x80000000;
		POINT point;
		for (size_t i = 0; i < arPoints.size(); i++)
		{
			point.x = arPoints[i].x;
			point.y = arPoints[i].y;

			if (lMinF <= point.x)
			{
				int index = (_UINT32)point.x - 0x80000000;
				if (index >= 0 && index < (int)arGuides.size())
				{
					point.x = arGuides[index].m_param_value3;
				}
			}
			if (lMinF <= point.y)
			{
				int index = (_UINT32)point.y - 0x80000000;
				if (index >= 0 && index < (int)arGuides.size())
				{
					point.y = arGuides[index].m_param_value3;
				}
			}
			if ((size_t)point.y > *cy)	point.y &= *cy;
			if ((size_t)point.x > *cx)	point.x &= *cx;

			m_arPoints.push_back(point);
		}
	}

	inline std::wstring GetVmlPath () const
	{
		if (m_arSegments.empty() && m_arPoints.empty())
			return std::wstring(L"");

		std::wstring strVmlPath;
		size_t valuePointer = 0;

		if (m_arSegments.empty())
		{
			for (size_t i = 0; i < m_arPoints.size(); ++i)
			{
                strVmlPath += L"l";
				strVmlPath += std::to_wstring(m_arPoints[i].x);
                strVmlPath += L",";
				strVmlPath += std::to_wstring(m_arPoints[i].y);
				
				++valuePointer;
			}

            strVmlPath += L"xe";

			return strVmlPath;
		}
		
		for (size_t i = 0; i < m_arSegments.size(); i++)
		{
			switch (m_arSegments[i].m_eRuler)
			{
				case NSCustomShapesConvert::rtLineTo:
				{
					for (_UINT16 j = 0; j < m_arSegments[i].m_nCount; ++j)
					{
						if (valuePointer + 1 > m_arPoints.size())
						{
							break;

							strVmlPath += L"l";
							strVmlPath += std::to_wstring(m_arPoints[0].x);
							strVmlPath += L",";
							strVmlPath += std::to_wstring(m_arPoints[0].y);
							
							++valuePointer;
						}
						else
						{
							strVmlPath += L"l";
							strVmlPath += std::to_wstring(m_arPoints[valuePointer].x );
							strVmlPath += L",";
							strVmlPath += std::to_wstring(m_arPoints[valuePointer].y );
							
							++valuePointer;
						}
					}
				}break;
				case NSCustomShapesConvert::rtCurveTo:
				{
					for (_UINT16 j = 0; j < m_arSegments[i].m_nCount; ++j)
					{
						if (valuePointer + 3 > m_arPoints.size()) 
							break;
						strVmlPath += L"c";
						strVmlPath += std::to_wstring(m_arPoints[valuePointer].x );
						strVmlPath += L",";
						strVmlPath += std::to_wstring(m_arPoints[valuePointer].y );
						strVmlPath += L",";
						strVmlPath += std::to_wstring(m_arPoints[valuePointer + 1].x );
						strVmlPath += L",";
						strVmlPath += std::to_wstring(m_arPoints[valuePointer + 1].y );
						strVmlPath += L",";
						strVmlPath += std::to_wstring(m_arPoints[valuePointer + 2].x );
						strVmlPath += L",";
						strVmlPath += std::to_wstring(m_arPoints[valuePointer + 2].y );
						valuePointer += 3;
					}
				}break;
				case NSCustomShapesConvert::rtMoveTo:
				{
					if (valuePointer < m_arPoints.size()) 
					{
						strVmlPath += L"m";
						strVmlPath += std::to_wstring(m_arPoints[valuePointer].x );
						strVmlPath += L",";
						strVmlPath += std::to_wstring(m_arPoints[valuePointer].y );
						
						++valuePointer;
					}
				}
				break;
				case NSCustomShapesConvert::rtClose:
				{
					strVmlPath += L"x";
				}
				break;
				case NSCustomShapesConvert::rtEnd:
				{
					strVmlPath += L"e";
				}break;	
				default:
					break;
			}
		}

        if ( !strVmlPath.empty() && ( strVmlPath[strVmlPath.size() - 1] != L'e' ) )
            strVmlPath +=L"e";

		return strVmlPath;
	}

private:
	std::vector<MSOPATHINFO> &m_arSegments;
	std::vector<POINT> m_arPoints;
};


}
