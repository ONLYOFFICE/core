/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "Record.h"

namespace DocFileFormat
{
	typedef enum _PropertyId
	{
		//Transform
		PropertyId_left=0,
		PropertyId_top=1,
		PropertyId_right=2,
		PropertyId_bottom=3,
		PropertyId_rotation=4,
		PropertyId_gvPage=5,
		PropertyId_fChangePage=61,
		PropertyId_fFlipV=62,
		PropertyId_fFlipH=63,

		//Protection
		//fLockAgainstUngrouping=118,
		//fLockRotation=119,
		//fLockAspectRatio=120,
		//fLockPosition=121,
		//fLockAgainstSelect=122,
		//fLockCropping=123,
		//fLockVertices=124,
		//fLockText=125,
		//fLockAdjustHandles=126,
		protectionBooleans=127,

		//Text
		lTxid=128,
		dxTextLeft=129,
		dyTextTop=130,
		dxTextRight=131,
		dyTextBottom=132,
		WrapText=133,
		scaleText=134,
		anchorText=135,
		txflTextFlow=136,
		cdirFont=137,
		hspNext=138,
		txdir=139,
		ccol=140,
		dzColMargin=141,

		textBooleanProperties=191,

		//GeoText
		gtextUNICODE=192,
		gtextRTF=193,
		gtextAlign=194,
		gtextSize=195,
		gtextSpacing=196,
		gtextFont=197,
		gtextCSSFont=198,
		//gtextFReverseRows=240,
		//fGtext=241,
		//gtextFVertical=242,
		//gtextFKern=243,
		//gtextFTight=244,
		//gtextFStretch=245,
		//gtextFShrinkFit=246,
		//gtextFBestFit=247,
		//gtextFNormalize=248,
		//gtextFDxMeasure=249,
		//gtextFBold=250,
		//gtextFItalic=251,
		//gtextFUnderline=252,
		//gtextFShadow=253,
		//gtextFSmallcaps=254,
		geometryTextBooleanProperties=255,

		//BLIP
		cropFromTop=256,
		cropFromBottom=257,
		cropFromLeft=258,
		cropFromRight=259,
		Pib=260,
		pibName=261,
		pibFlags=262,
		pictureTransparent=263,
		pictureContrast=264,
		pictureBrightness=265,
		pictureGamma=266,
		pictureId=267,
		pictureDblCrMod=268,
		pictureFillCrMod=269,
		pictureLineCrMod=270,
		pibPrint=271,
		pibPrintName=272,
		pibPrintFlags=273,
		movie=274,
		pictureRecolor=282,
		picturePreserveGrays=313,

		blipBooleanProperties=319,

		//Geometry
		geoLeft=320,
		geoTop=321,
		geoRight=322,
		geoBottom=323,
		shapePath=324,
		pVertices=325,
		pSegmentInfo=326,
		adjustValue=327,
		adjust2Value=328,
		adjust3Value=329,
		adjust4Value=330,
		adjust5Value=331,
		adjust6Value=332,
		adjust7Value=333,
		adjust8Value=334,
		adjust9Value=335,
		adjust10Value=336,
		pConnectionSites=337,
		pConnectionSitesDir=338,
		xLimo=339,
		yLimo=340,
		pAdjustHandles=341,
		pGuides=342,
		pInscribe=343,
		cxk=344,
		pFragments=345,

		geometryBooleans=383,

		//Fill Style
		fillType=384,
		fillColor=385,
		fillOpacity=386,
		fillBackColor=387,
		fillBackOpacity=388,
		fillCrMod=389,
		fillBlip=390,
		fillBlipName=391,
		fillBlipFlags=392,
		fillWidth=393,
		fillHeight=394,
		fillAngle=395,
		fillFocus=396,
		fillToLeft=397,
		fillToTop=398,
		fillToRight=399,
		fillToBottom=400,
		fillRectLeft=401,
		fillRectTop=402,
		fillRectRight=403,
		fillRectBottom=404,
		fillDztype=405,
		fillShadePreset=406,
		fillShadeColors=407,
		fillOriginX=408,
		fillOriginY=409,
		fillShapeOriginX=410,
		fillShapeOriginY=411,
		fillShadeType=412,

		fillColorExt=414,
		fillColorExtMod=416,
		fillBackColorExt=418,
		fillBackColorExtMod=420,

		fillStyleBooleanProperties=447,

		//Line Style
		lineColor=448,
		lineOpacity=449,
		lineBackColor=450,
		lineCrMod=451,
		lineType=452,
		lineFillBlip=453,
		lineFillBlipName=454,
		lineFillBlipFlags=455,
		lineFillWidth=456,
		lineFillHeight=457,
		lineFillDztype=458,
		lineWidth=459,
		lineMiterLimit=460,
		lineStyle=461,
		lineDashing=462,
		lineDashStyle=463,
		lineStartArrowhead=464,
		lineEndArrowhead=465,
		lineStartArrowWidth=466,
		lineStartArrowLength=467,
		lineEndArrowWidth=468,
		lineEndArrowLength=469,
		lineJoinStyle=470,
		lineEndCapStyle=471,

		lineStyleBooleans=511,

		//Shadow Style
		shadowType=512,
		shadowColor=513,
		shadowHighlight=514,
		shadowCrMod=515,
		shadowOpacity=516,
		shadowOffsetX=517,
		shadowOffsetY=518,
		shadowSecondOffsetX=519,
		shadowSecondOffsetY=520,
		shadowScaleXToX=521,
		shadowScaleYToX=522,
		shadowScaleXToY=523,
		shadowScaleYToY=524,
		shadowPerspectiveX=525,
		shadowPerspectiveY=526,
		shadowWeight=527,
		shadowOriginX=528,
		shadowOriginY=529,

		shadowStyleBooleanProperties=575,

		//Perspective Style
		perspectiveType=576,
		perspectiveOffsetX=577,
		perspectiveOffsetY=578,
		perspectiveScaleXToX=579,
		perspectiveScaleYToX=580,
		perspectiveScaleXToY=581,
		perspectiveScaleYToY=582,
		perspectivePerspectiveX=583,
		perspectivePerspectiveY=584,
		perspectiveWeight=585,
		perspectiveOriginX=586,
		perspectiveOriginY=587,

		perspectiveStyleBooleanProperties=639,

		//3D Object
		c3DSpecularAmt=640,
		c3DDiffuseAmt=641,
		c3DShininess=642,
		c3DEdgeThickness=643,
		C3DExtrudeForward=644,
		c3DExtrudeBackward=645,
		c3DExtrudePlane=646,
		c3DExtrusionColor=647,
		c3DCrMod=648,

		threeDObjectBooleanProperties=703,

		//3D Style
		c3DYRotationAngle=704,
		c3DXRotationAngle=705,
		c3DRotationAxisX=706,
		c3DRotationAxisY=707,
		c3DRotationAxisZ=708,
		c3DRotationAngle=709,
		c3DRotationCenterX=710,
		c3DRotationCenterY=711,
		c3DRotationCenterZ=712,
		c3DRenderMode=713,
		c3DTolerance=714,
		c3DXViewpoint=715,
		c3DYViewpoint=716,
		c3DZViewpoint=717,
		c3DOriginX=718,
		c3DOriginY=719,
		c3DSkewAngle=720,
		c3DSkewAmount=721,
		c3DAmbientIntensity=722,
		c3DKeyX=723,
		c3DKeyY=724,
		c3DKeyZ=725,
		c3DKeyIntensity=726,
		c3DFillX=727,
		c3DFillY=728,
		c3DFillZ=729,
		c3DFillIntensity=730,

		threeDStyleBooleanProperties=767,

		//Shape
		hspMaster=769,
		cxstyle=771,
		bWMode=772,
		bWModePureBW=773,
		bWModeBW=774,
		idDiscussAnchor=775,
		dgmLayout=777,
		dgmNodeKind=778,
		dgmLayoutMRU=779,
		wzEquationXML=780,

		shapeBooleans=831,

		//Callout
		spcot=832,
		dxyCalloutGap=833,
		spcoa=834,
		spcod=835,
		dxyCalloutDropSpecified=836,
		dxyCalloutLengthSpecified=837,
		fCallout=889,
		fCalloutAccentBar=890,
		fCalloutTextBorder=891,
		fCalloutMinusX=892,
		fCalloutMinusY=893,
		fCalloutDropAuto=894,
		fCalloutLengthSpecified=895,

		//Groupe Shape
		wzName=896,
		wzDescription=897,
		pihlShape=898,
		pWrapPolygonVertices=899,
		dxWrapDistLeft=900,
		dyWrapDistTop=901,
		dxWrapDistRight=902,
		dyWrapDistBottom=903,
		lidRegroup=904,
		groupLeft=905,
		groupTop=906,
		groupRight=907,
		groupBottom=908,
		wzTooltip=909,
		wzScript=910,
		posh=911,
		posrelh=912,
		posv=913,
		posrelv=914,
		pctHR=915,
		alignHR=916,
		dxHeightHR=917,
		dxWidthHR=918,
		wzScriptExtAttr=919,
		scriptLang=920,
		wzScriptIdAttr=921,
		wzScriptLangAttr=922,
		borderTopColor=923,
		borderLeftColor=924,
		borderBottomColor=925,
		borderRightColor=926,
		tableProperties=927,
		tableRowProperties=928,
		scriptHtmlLocation=929,
		wzApplet=930,
		wzFrameTrgtUnused=932,
		wzWebBot=933,
		wzAppletArg=934,
		wzAccessBlob=936,
		metroBlob=937,
		dhgt=938,

		groupShapeBooleans = 959,
		
		relRotation = 964,

		//Unknown HTML
		wzLineId=1026,
		wzFillId=1027,
		wzPictureId=1028,
		wzPathId=1029,
		wzShadowId=1030,
		wzPerspectiveId=1031,
		wzGtextId=1032,
		wzFormulaeId=1033,
		wzHandlesId=1034,
		wzCalloutId=1035,
		wzLockId=1036,
		wzTextId=1037,
		wzThreeDId=1038,
		FakeShapeType=1039,
		fFakeMaster=1086,

		//Diagramm
		dgmt=1280,
		dgmStyle=1281,
		pRelationTbl=1284,
		dgmScaleX=1285,
		dgmScaleY=1286,
		dgmDefaultFontSize=1287,
		dgmConstrainBounds=1288,
		dgmBaseTextScale=1289,
		fBorderlessCanvas=1338,
		fNonStickyInkCanvas=1339,
		fDoFormat=1340,
		fReverse=1341,
		fDoLayout=1342,
		diagramBooleans=1343,

		//Web Component
		webComponentWzHtml=1664,
		webComponentWzName=1665,
		webComponentWzUrl=1666,
		webComponentWzProperties=1667,
		fIsWebComponent=1727,

		//Clip
		pVerticesClip=1728,
		pSegmentInfoClip=1729,
		shapePathClip=1730,
		fClipToWrap=1790,
		fClippedOK=1791,

		//Ink
		pInkData=1792,
		fInkAnnotation=1852,
		fHitTestInk=1853,
		fRenderShape=1854,
		fRenderInk=1855,

		//Signature
		wzSigSetupId=1921,
		wzSigSetupProvId=192,
		wzSigSetupSuggSigner=1923,
		wzSigSetupSuggSigner2=1924,
		wzSigSetupSuggSignerEmail=1925,
		wzSigSetupSignInst=1926,
		wzSigSetupAddlXml=1927,
		wzSigSetupProvUrl=1928,
		fSigSetupShowSignDate=1980,
		fSigSetupAllowComments=1981,
		fSigSetupSignInstSet=1982,
		fIsSignatureLine=1983,

		//Groupe Shape 2
		pctHoriz=1984,
		pctVert=1985,
		pctHorizPos=1986,
		pctVertPos=1987,
		sizerelh=1988,
		sizerelv=1989,
		colStart=1990,
		colSpan=1991
	} PropertyId;

	//!!!TODO: Реальные значения не соответствуют значениям из спецификации!!!
	typedef enum _PositionHorizontal
	{
		//msophAbs = 0x1,
		msophLeft = 0x1,
		msophCenter = 0x2,
		msophRight = 0x3,
		msophInside = 0x4,
		msophOutside = 0x5
	} PositionHorizontal;

	typedef enum _PositionHorizontalRelative
	{
		msoprhMargin,
		msoprhPage,
		msoprhText,
		msoprhChar
	} PositionHorizontalRelative;

	//!!!TODO: Реальные значения не соответствуют значениям из спецификации!!!
	typedef enum _PositionVertical
	{
		//msopvAbs = 0x1,
		msopvTop = 0x1,
		msopvCenter = 0x2,
		msopvBottom = 0x3,
		msopvInside = 0x4,
		msopvOutside = 0x5
	} PositionVertical;

	typedef enum _PositionVerticalRelative
	{
		msoprvMargin,
		msoprvPage,
		msoprvText,
		msoprvLine
	} PositionVerticalRelative;

	typedef enum _LineEnd
	{
		NoEnd = 0,
		ArrowEnd,
		ArrowStealthEnd,
		ArrowDiamondEnd,
		ArrowOvalEnd,
		ArrowOpenEnd,
		ArrowChevronEnd,
		ArrowDoubleChevronEnd
	} LineEnd;

	typedef enum _LineDashing
	{
		LineDashing_Solid = 0,
		LineDashing_DashSys,
		LineDashing_DotSys,
		LineDashing_DashDotSys,
		LineDashing_DashDotDotSys,
		LineDashing_DotGEL,
		LineDashing_DashGEL,
		LineDashing_LongDashGEL,
		LineDashing_DashDotGEL,
		LineDashing_LongDashDotGEL,
		LineDashing_LongDashDotDotGEL
	} LineDashing;

	struct OptionEntry
	{
		OptionEntry() :	pid(PropertyId_left), fBid(false), fComplex(false), op(0)
		{
		}

		PropertyId		pid;
		bool			fBid;
		bool			fComplex;
		unsigned int	op;
		std::shared_ptr<unsigned char>	opComplex;
	};

	typedef std::shared_ptr<OptionEntry> OptionEntryPtr;

	class ShapeOptions: public Record
	{
	public: 
		static const unsigned short TYPE_CODE_0xF00B = 0xF00B;
		static const unsigned short TYPE_CODE_0xF121 = 0xF121;
		static const unsigned short TYPE_CODE_0xF122 = 0xF122;

		std::vector<OptionEntryPtr>				Options;
		std::map<PropertyId, OptionEntryPtr>	OptionsByID;

		ShapeOptions() : Record()
		{
		}

		virtual ~ShapeOptions()
		{
		}

		ShapeOptions (IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance) : Record (_reader, size, typeCode, version, instance)
		{
			long pos = Reader->GetPosition();

			//parse the flags and the simple values
			for (unsigned int i = 0; i < instance; ++i)
			{
				OptionEntryPtr entry = std::shared_ptr<OptionEntry>(new OptionEntry());
				unsigned short flag	=	Reader->ReadUInt16();

				entry->pid			=	(PropertyId)FormatUtils::BitmaskToInt (flag, 0x3FFF);
				entry->fBid			=	FormatUtils::BitmaskToBool (flag, 0x4000);
				entry->fComplex		=	FormatUtils::BitmaskToBool (flag, 0x8000);
				entry->op			=	Reader->ReadUInt32();

				Options.push_back( entry );
			}

			//parse the complex values & sorted by pid
			for (unsigned int i = 0; i < instance; ++i)
			{
				if (Options[i]->fComplex && Options[i]->op > 0)
				{			
					unsigned int size = Options[i]->op;
					
					if (Options[i]->pid == 0x0145 ||
						Options[i]->pid == 0x0146 ||
						Options[i]->pid == 0x0197 ||
						Options[i]->pid == 0x0156 ||
						Options[i]->pid == 0x0155 ||
						Options[i]->pid == 0x0151 ||
						Options[i]->pid == 0x0152 ||
						Options[i]->pid == 0x0157 ||
						Options[i]->pid == 0x0158)//mso arrays
							size += 6;
					Options[i]->opComplex = std::shared_ptr<unsigned char>(Reader->ReadBytes( size, true ));
				}

				OptionsByID.insert(std::make_pair(Options[i]->pid, Options[i]));
			}

            Reader->Seek(( pos + size ), 0/*STREAM_SEEK_SET*/);
		}

		virtual Record* NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
		{
			return new ShapeOptions( _reader, bodySize, typeCode, version, instance );
		}
	};
}
