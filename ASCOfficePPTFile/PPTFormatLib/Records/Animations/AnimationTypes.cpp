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
#include "AnimationTypes.h"

//----------------------------------------------------------------------------------------------------------------------//
//													Animation Types														//
//----------------------------------------------------------------------------------------------------------------------//

/*
static const wchar_t EffectBehaviorBlindsHorizontal		[]	=	L"blinds(horizontal)";
static const wchar_t EffectBehaviorBoxIn				[]	=	L"box(in)";
static const wchar_t EffectBehaviorBoxOut				[]	=	L"box(out)";
static const wchar_t EffectBehaviorCheckerboardAcross	[]	=	L"checkerboard(across)";
static const wchar_t EffectBehaviorCheckerboardDown		[]	=	L"checkerboard(down)";
static const wchar_t EffectBehaviorCircleIn				[]	=	L"circle(in)";
static const wchar_t EffectBehaviorCircleOut			[]	=	L"circle(out)";
static const wchar_t EffectBehaviorDiamondIn			[]	=	L"diamond(in)";
static const wchar_t EffectBehaviorDiamonOut			[]	=	L"diamond(out)";
static const wchar_t EffectBehaviorDissolve				[]	=	L"dissolve";
static const wchar_t EffectBehaviorFade					[]	=	L"fade";
static const wchar_t EffectBehaviorPlusIn				[]	=	L"plus(in)";
static const wchar_t EffectBehaviorPlusOut				[]	=	L"plus(out)";
static const wchar_t EffectBehaviorBarnInVertical		[]	=	L"barn(inVertical)";
static const wchar_t EffectBehaviorBarnInHorizontal		[]	=	L"barn(inHorizontal)";
static const wchar_t EffectBehaviorBarnOutVertical		[]	=	L"barn(outVertical)";
static const wchar_t EffectBehaviorBarnOnHorizontal		[]	=	L"barn(outHorizontal)";
static const wchar_t EffectBehaviorRandomBarHorizontal	[]	=	L"randombar(horizontal)";
static const wchar_t EffectBehaviorRandomBarVertical	[]	=	L"randombar(vertical)";
static const wchar_t EffectBehaviorStripsDownLeft		[]	=	L"strips(downLeft)";
static const wchar_t EffectBehaviorStripsUpLeft			[]	=	L"strips(upLeft)";
static const wchar_t EffectBehaviorStripsDownRight		[]	=	L"strips(downRight)";
static const wchar_t EffectBehaviorStripsUpRight		[]	=	L"strips(upRight)";
static const wchar_t EffectBehaviorWedge				[]	=	L"wedge";
static const wchar_t EffectBehaviorWheel1				[]	=	L"wheel(1)";
static const wchar_t EffectBehaviorWheel2				[]	=	L"wheel(2)";
static const wchar_t EffectBehaviorWheel3				[]	=	L"wheel(3)";
static const wchar_t EffectBehaviorWheel4				[]	=	L"wheel(4)";
static const wchar_t EffectBehaviorWheel8				[]	=	L"wheel(8)";
static const wchar_t EffectBehaviorWipeRight			[]	=	L"wipe(right)";
static const wchar_t EffectBehaviorWipeLeft				[]	=	L"wipe(left)";
static const wchar_t EffectBehaviorWipeUp				[]	=	L"wipe(up)";
static const wchar_t EffectBehaviorWipeDown				[]	=	L"wipe(down)";
*/

#pragma region _MACROSES_

#define GET_ENUM_STRING(NAMEE, VALUEE)\
    case  TL_##NAMEE##VALUEE: 	\
    return std::wstring(_T("TL_")) + std::wstring(_T(#NAMEE)) + std::wstring(_T(#VALUEE)); \
    break;

#if defined(_WIN32) || defined (_WIN64)
    #define GET_EFFECT_NAME_BY_ID(VALUE,DESCRIPTION)							\
    case (##VALUE) : return std::wstring(_T(#DESCRIPTION)); break;
#else
    #define GET_EFFECT_NAME_BY_ID(VALUE,DESCRIPTION)							\
    case ##VALUE : return std::wstring(_T(#DESCRIPTION)); break;
#endif
#pragma endregion

namespace Animations
{
    std::wstring Helpers::GetTimePropertyID4TimeNode ( TimePropertyID4TimeNode Value )
	{
		switch ( Value )
		{
            GET_ENUM_STRING( TPID_, Display );
            GET_ENUM_STRING( TPID_, MasterPos );
            GET_ENUM_STRING( TPID_, SlaveType );
            GET_ENUM_STRING( TPID_, EffectID );
            GET_ENUM_STRING( TPID_, EffectDir );
            GET_ENUM_STRING( TPID_, AfterEffect );
            GET_ENUM_STRING( TPID_, SlideCount );
            GET_ENUM_STRING( TPID_, TimeFilter );
            GET_ENUM_STRING( TPID_, EventFilter );
            GET_ENUM_STRING( TPID_, HideWhenStopped );
            GET_ENUM_STRING( TPID_, GroupID );
            GET_ENUM_STRING( TPID_, EffectNodeType );
            GET_ENUM_STRING( TPID_, PlaceholderNode );
            GET_ENUM_STRING( TPID_, MediaVolume );
            GET_ENUM_STRING( TPID_, MediaMute );
            GET_ENUM_STRING( TPID_, ZoomToFullScreen );

		default: 
            return std::wstring ( _T("TimePropertyID4TimeNode : Unknown Enum") );
		}
	};

    std::wstring Helpers::GetTimeVariantTypeEnum ( TimeVariantTypeEnum Value )
	{
		switch ( Value )
		{
            GET_ENUM_STRING( TVT_, Bool );
            GET_ENUM_STRING( TVT_, Int );
            GET_ENUM_STRING( TVT_, Float );
            GET_ENUM_STRING( TVT_, String );

		default: 
            return std::wstring ( _T("TimeVariantTypeEnum : Unknown Enum") );
		}
	};

    std::wstring Helpers::GetTimeNodeTypeEnum ( TimeNodeTypeEnum Value )
	{
		switch ( Value )
		{
            GET_ENUM_STRING( TNT_, Parallel );
            GET_ENUM_STRING( TNT_, Sequential );
            GET_ENUM_STRING( TNT_, Behavior );
            GET_ENUM_STRING( TNT_, Media );

		default: 
            return std::wstring ( _T("TimeNodeTypeEnum : Unknown Enum") );
		}
	};

    std::wstring Helpers::GetTriggerObjectEnum ( TriggerObjectEnum Value )
	{
		switch ( Value )
		{
            GET_ENUM_STRING( TOT_, None );				//	None
            GET_ENUM_STRING( TOT_, VisualElement );	//	An animatable object.
            GET_ENUM_STRING( TOT_, TimeNode );			//	A time node.
            GET_ENUM_STRING( TOT_, RuntimeNodeRef );	//	Runtime child time nodes.

		default: 
            return std::wstring ( _T("TriggerObjectEnum : Unknown Enum") );
		}
	}

    std::wstring Helpers::GetTimeVisualElementEnum ( TimeVisualElementEnum Value )
	{
		switch ( Value )
		{
            GET_ENUM_STRING( TVET_, Shape );
            GET_ENUM_STRING( TVET_, Page );
            GET_ENUM_STRING( TVET_, TextRange );
            GET_ENUM_STRING( TVET_, Audio );
            GET_ENUM_STRING( TVET_, Video );
            GET_ENUM_STRING( TVET_, ChartElement );
            GET_ENUM_STRING( TVET_, ShapeOnly );
            GET_ENUM_STRING( TVET_, AllTextRange );

		default: 
            return std::wstring ( _T("TimeVisualElementEnum : Unknown Enum") );
		}
	}

    std::wstring Helpers::GetElementTypeEnum ( ElementTypeEnum Value )
	{
		switch ( Value )
		{
			GET_ENUM_STRING( ET_, ShapeType );			
			GET_ENUM_STRING( ET_, SoundType );	

		default: 
            return std::wstring ( _T("ElementTypeEnum : Unknown Enum") );
		}
	}

    std::wstring Helpers::GetTimeAnimateBehaviorValueTypeEnum ( TimeAnimateBehaviorValueTypeEnum Value )
	{
		switch ( Value )
		{
			GET_ENUM_STRING( TABVT_, String );			
			GET_ENUM_STRING( TABVT_, Number );	
			GET_ENUM_STRING( TABVT_, Color );	

		default: 
            return std::wstring ( _T("TimeAnimateBehaviorValueTypeEnum : Unknown Enum") );
		}
	}
    std::wstring Helpers::GetAnimationClassName ( AnimationsClassType Value )
	{
		switch ( Value )
		{
		case Animations::RT_TimeNode:
            return std::wstring (_T("TimeNodeAtom") );

		case Animations::RT_TimePropertyList:	//	Specifies a TimePropertyList4TimeNodeContainer or TimePropertyList4TimeBehavior.  
            return std::wstring (_T("TimePropertyList4TimeNodeContainer"));

		case Animations::RT_TimeExtTimeNodeContainer:
            return std::wstring (_T("ExtTimeNodeContainer"));

		case Animations::RT_SlideTime10Atom	:
            return std::wstring (_T("SlideTime10Atom"));

		case Animations::RT_SlideFlags10Atom:
            return std::wstring (_T("SlideFlags10Atom"));

		case Animations::RT_HashCodeAtom:
            return std::wstring (_T("HashCode10Atom"));

		case Animations::RT_TimeSequenceData:
            return std::wstring (_T("TimeSequenceDataAtom"));

		case Animations::RT_TimeConditionContainer:
            return std::wstring (_T("TimeConditionContainer"));

		case Animations::RT_TimeCondition:
            return std::wstring (_T("TimeConditionAtom"));
		}

        return std::wstring (_T(""));
	}

    std::wstring Helpers::GetEffectTypeOfGroup ( _UINT32 Value )
	{
		if ( 0x00000001	==	Value )
            return std::wstring ( _T("Entrance") );
		if ( 0x00000002	==	Value )
            return std::wstring ( _T("Exit") );
		if ( 0x00000003	==	Value )
            return std::wstring ( _T("Emphasis") );
		if ( 0x00000004	==	Value )
            return std::wstring ( _T("MotionPath") );
		if ( 0x00000005	==	Value )
            return std::wstring ( _T("ActionVerb") );
		if ( 0x00000006	==	Value )
            return std::wstring ( _T("MediaCommand") );

        return std::wstring ( _T("") );
	}
	//	the corresponding effect type is an entrance or an exit effect
    std::wstring Helpers::GetEffectEntranceOrExitNameByID ( _UINT32 EffectID )
	{
		switch ( EffectID )
		{
            GET_EFFECT_NAME_BY_ID ( 0x00000000,  Custom );				//  The corresponding effect direction MUST be ignored.
            GET_EFFECT_NAME_BY_ID ( 0x00000001,  Appear );				//  The corresponding effect direction MUST be ignored.
			GET_EFFECT_NAME_BY_ID ( 0x00000002,  FlyIn );				// 
			GET_EFFECT_NAME_BY_ID ( 0x00000003,  Blinds );				// 
			GET_EFFECT_NAME_BY_ID ( 0x00000004,  Box );					// 
			GET_EFFECT_NAME_BY_ID ( 0x00000005,  CheckBoard );			// 
			GET_EFFECT_NAME_BY_ID ( 0x00000006,  Circle );				//
			GET_EFFECT_NAME_BY_ID ( 0x00000007,  Crawl );				//  
			GET_EFFECT_NAME_BY_ID ( 0x00000008,  Diamond );				//  
			GET_EFFECT_NAME_BY_ID ( 0x00000009,  Dissolve );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000000A,  Fade );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000000B,  FlashOnce );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000000C,  Peek );				//  
			GET_EFFECT_NAME_BY_ID ( 0x0000000D,  Plus );				// 
			GET_EFFECT_NAME_BY_ID ( 0x0000000E,  RandomBars );			//  
			GET_EFFECT_NAME_BY_ID ( 0x0000000F,  Spiral );				//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000010,  Split );				// 
			GET_EFFECT_NAME_BY_ID ( 0x00000011,  Stretch );				//  
			GET_EFFECT_NAME_BY_ID ( 0x00000012,  Strips );				//  
			GET_EFFECT_NAME_BY_ID ( 0x00000013,  Swivel );				//  
			GET_EFFECT_NAME_BY_ID ( 0x00000014,  Wedge );				//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000015,  Wheel );				//  
			GET_EFFECT_NAME_BY_ID ( 0x00000016,  Wipe );				//   
			GET_EFFECT_NAME_BY_ID ( 0x00000017,  Zoom );				//  
			GET_EFFECT_NAME_BY_ID ( 0x00000018,  RandomEffects );		//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000019,  Boomerang );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000001A,  Bounce );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000001B,  ColorReveal );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000001C,  Credits );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000001D,  EaseIn );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000001E,  Float );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000001F,  GrowAndTurn );			//	The corresponding effect direction MUST be ignored. 
			//GET_EFFECT_NAME_BY_ID ( 0x00000020,  Reserved );			// 
			//GET_EFFECT_NAME_BY_ID ( 0x00000021,  Reserved );			//  
			GET_EFFECT_NAME_BY_ID ( 0x00000022,  LightSpeed );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000023,  PinWheel );			//  The corresponding effect direction MUST be ignored. 
			//GET_EFFECT_NAME_BY_ID ( 0x00000024,  Reserved );			//  
			GET_EFFECT_NAME_BY_ID ( 0x00000025,  RiseUp );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000026,  Swish );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000027,  ThinLine );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000028,  Unfold );				//	The corresponding effect direction MUST be ignored. 
            GET_EFFECT_NAME_BY_ID ( 0x00000029,  Whip );				//  The corresponding effect direction MUST be ignored.
			GET_EFFECT_NAME_BY_ID ( 0x0000002A,  Ascend );				//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000002B,  CenterRevolve );		//  The corresponding effect direction MUST be ignored. 
			//GET_EFFECT_NAME_BY_ID ( 0x0000002C,  Reserved );			//  
			GET_EFFECT_NAME_BY_ID ( 0x0000002D,  FadedSwivel );			//  The corresponding effect direction MUST be ignored. 
			//GET_EFFECT_NAME_BY_ID ( 0x0000002E,  Reserved );			//  
			GET_EFFECT_NAME_BY_ID ( 0x0000002F,  Descend );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000030,  Sling );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000031,  Spinner );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000032,  Compress );			//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000033,  Zip );					//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000034,  ArcUp );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000035,  FadedZoom );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000036,  Glide );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000037,  Expand );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000038,  Flip );				//  The corresponding effect direction MUST be ignored. 
			//GET_EFFECT_NAME_BY_ID ( 0x00000039,  Reserved );			//  
			GET_EFFECT_NAME_BY_ID ( 0x0000003A,  Fold );				//  The corresponding effect direction MUST be ignored. 
		default :			  
			break;
		}
        return std::wstring(_T("Unknown EffectID"));
	}

	//	the corresponding effect type is an emphasis effect
    std::wstring Helpers::GetEffectEmphasisNameByID ( _UINT32 EffectID )
	{
		switch ( EffectID )
		{
			GET_EFFECT_NAME_BY_ID ( 0x00000001,  ChangeFillColor );		//  
			GET_EFFECT_NAME_BY_ID ( 0x00000002,  ChangeFont );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000003,  ChangeFillColor );		//  
			GET_EFFECT_NAME_BY_ID ( 0x00000004,  ChangeFontSize );		//
			GET_EFFECT_NAME_BY_ID ( 0x00000005,  ChangeFontStyle );		//
			GET_EFFECT_NAME_BY_ID ( 0x00000006,  GrowAndShrink );		//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000007,  ChangeFillColor );		//  
			GET_EFFECT_NAME_BY_ID ( 0x00000008,  Spin );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000009,  Transparency );		//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000000A,  BoldFlash );			//  The corresponding effect direction MUST be ignored. 
			//GET_EFFECT_NAME_BY_ID ( 0x0000000B,  Reserved );			//  
			//GET_EFFECT_NAME_BY_ID ( 0x0000000C,  Reserved );			//  
			//GET_EFFECT_NAME_BY_ID ( 0x0000000D,  Reserved );			//  
			GET_EFFECT_NAME_BY_ID ( 0x0000000E,  Blast );				//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000000F,  BoldReveal );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000010,  BrushOnColor );		//  The corresponding effect direction MUST be ignored. 
			//GET_EFFECT_NAME_BY_ID ( 0x00000011,  Reserved );			//
			GET_EFFECT_NAME_BY_ID ( 0x00000012,  BrushOnUnderline );	//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000013,  ColorBlend );			//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000014,  ColorWave );			//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000015,  ComplementaryColor );	//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000016,  ComplementaryColor2 );	//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000017,  ContrastingColor );	//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000018,  Darken );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000019,  Desaturate );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000001A,  FlashBulb );			//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000001B,  Flicker );				//  The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000001C,  GrowWithColor );		//  The corresponding effect direction MUST be ignored. 
			//GET_EFFECT_NAME_BY_ID ( 0x0000001D,  Reserved );			//  
			GET_EFFECT_NAME_BY_ID ( 0x0000001E,  Lighten );				//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x0000001F,  StyleEmphasis );		//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000020,  Teeter );				//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000021,  VerticalGrow );		//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000022,  Wave );				//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000023,  Blink );				//	The corresponding effect direction MUST be ignored. 
			GET_EFFECT_NAME_BY_ID ( 0x00000024,  Shimmer );				//	The corresponding effect direction MUST be ignored. 
		default :			  
			break;
		}
        return std::wstring(_T("Unknown EffectID"));
	}
	//	the corresponding effect type is a motion path effect
    std::wstring Helpers::GetEffectMotionPathNameByID ( _UINT32 EffectID )
	{
		switch ( EffectID )
		{
			GET_EFFECT_NAME_BY_ID ( 0x00000000,  Custom ); 
			GET_EFFECT_NAME_BY_ID ( 0x00000001,  Circle );  
			GET_EFFECT_NAME_BY_ID ( 0x00000002,  RightTriangle );   
			GET_EFFECT_NAME_BY_ID ( 0x00000003,  Diamond );   
			GET_EFFECT_NAME_BY_ID ( 0x00000004,  Hexagon );   
			GET_EFFECT_NAME_BY_ID ( 0x00000005,  FivePointStar );   
			GET_EFFECT_NAME_BY_ID ( 0x00000006,  CrescentMoon );   
			GET_EFFECT_NAME_BY_ID ( 0x00000007,  Square );  
			GET_EFFECT_NAME_BY_ID ( 0x00000008,  Trapezoid );   
			GET_EFFECT_NAME_BY_ID ( 0x00000009,  Heart ); 
			GET_EFFECT_NAME_BY_ID ( 0x0000000A,  Octagon );   
			GET_EFFECT_NAME_BY_ID ( 0x0000000B,  SixPointStar );  
			GET_EFFECT_NAME_BY_ID ( 0x0000000C,  Football );  
			GET_EFFECT_NAME_BY_ID ( 0x0000000D,  EqualTriangle );  
			GET_EFFECT_NAME_BY_ID ( 0x0000000E,  Parallelogram );  
			GET_EFFECT_NAME_BY_ID ( 0x0000000F,  Pentagon ); 
			GET_EFFECT_NAME_BY_ID ( 0x00000010,  FourPointStar );   
			GET_EFFECT_NAME_BY_ID ( 0x00000011,  EightPointStar );   
			GET_EFFECT_NAME_BY_ID ( 0x00000012,  Teardrop ); 
			GET_EFFECT_NAME_BY_ID ( 0x00000013,  PointyStar );   
			GET_EFFECT_NAME_BY_ID ( 0x00000014,  CurvedSquare );   
			GET_EFFECT_NAME_BY_ID ( 0x00000015,  CurvedX );  
			GET_EFFECT_NAME_BY_ID ( 0x00000016,  VerticalFigure8 );  
			GET_EFFECT_NAME_BY_ID ( 0x00000017,  CurvyStar );  
			GET_EFFECT_NAME_BY_ID ( 0x00000018,  LoopDeLoop );  
			GET_EFFECT_NAME_BY_ID ( 0x00000019,  BuzzSaw );   
			GET_EFFECT_NAME_BY_ID ( 0x0000001A,  HorizontalFigure8 );   
			GET_EFFECT_NAME_BY_ID ( 0x0000001B,  Peanut );  
			GET_EFFECT_NAME_BY_ID ( 0x0000001C,  Figure8four );   
			GET_EFFECT_NAME_BY_ID ( 0x0000001D,  Neutron ); 
			GET_EFFECT_NAME_BY_ID ( 0x0000001E,  Swoosh );  
			GET_EFFECT_NAME_BY_ID ( 0x0000001F,  Bean );  
			GET_EFFECT_NAME_BY_ID ( 0x00000020,  Plus );  
			GET_EFFECT_NAME_BY_ID ( 0x00000021,  InvertedTriangle ); 
			GET_EFFECT_NAME_BY_ID ( 0x00000022,  InvertedSquare );  
			GET_EFFECT_NAME_BY_ID ( 0x00000023,  Left );   
			GET_EFFECT_NAME_BY_ID ( 0x00000024,  TurnRight );  
			GET_EFFECT_NAME_BY_ID ( 0x00000025,  ArcDown );   
			GET_EFFECT_NAME_BY_ID ( 0x00000026,  Zigzag );   
			GET_EFFECT_NAME_BY_ID ( 0x00000027,  SCurve2 );  
			GET_EFFECT_NAME_BY_ID ( 0x00000028,  SineWave );    
			GET_EFFECT_NAME_BY_ID ( 0x00000029,  BounceLeft );   
			GET_EFFECT_NAME_BY_ID ( 0x0000002A,  Down );   
			GET_EFFECT_NAME_BY_ID ( 0x0000002B,  TurnUp );   
			GET_EFFECT_NAME_BY_ID ( 0x0000002C,  ArcUp );  
			GET_EFFECT_NAME_BY_ID ( 0x0000002D,  HeartBeat );   
			GET_EFFECT_NAME_BY_ID ( 0x0000002E,  SpiralRight );   
			GET_EFFECT_NAME_BY_ID ( 0x0000002F,  Wave );  
			GET_EFFECT_NAME_BY_ID ( 0x00000030,  CurvyLeft );  
			GET_EFFECT_NAME_BY_ID ( 0x00000031,  DiagonalDownRight );  
			GET_EFFECT_NAME_BY_ID ( 0x00000032,  TurnDown );   
			GET_EFFECT_NAME_BY_ID ( 0x00000033,  ArcLeft );  
			GET_EFFECT_NAME_BY_ID ( 0x00000034,  Funnel );  
			GET_EFFECT_NAME_BY_ID ( 0x00000035,  Spring );  
			GET_EFFECT_NAME_BY_ID ( 0x00000036,  BounceRight );   
			GET_EFFECT_NAME_BY_ID ( 0x00000037,  SpiralLeft );  
			GET_EFFECT_NAME_BY_ID ( 0x00000038,  DiagonalUpRight );  
			GET_EFFECT_NAME_BY_ID ( 0x00000039,  TurnUpRight );  
			GET_EFFECT_NAME_BY_ID ( 0x0000003A,  ArcRight );  
			GET_EFFECT_NAME_BY_ID ( 0x0000003B,  Scurve1 );   
			GET_EFFECT_NAME_BY_ID ( 0x0000003C,  DecayingWave );   
			GET_EFFECT_NAME_BY_ID ( 0x0000003D,  CurvyRight );   
			GET_EFFECT_NAME_BY_ID ( 0x0000003E,  StairsDown );   
			GET_EFFECT_NAME_BY_ID ( 0x0000003F,  Right );  
			GET_EFFECT_NAME_BY_ID ( 0x00000040,  Up );  
		default :			  
			break;
		}
        return std::wstring(_T("Unknown EffectID"));
	}

    std::wstring Helpers::GetEffectNameByID ( _UINT32 EffectType, _UINT32 EffectID )
	{
		switch ( EffectType )
		{
		case 0x00000001:	//	Entrance	+
		case 0x00000002:	//	Exit
			return Animations::Helpers::GetEffectEntranceOrExitNameByID ( EffectID );
			break;
		case 0x00000003:	//	Emphasis	+
			return Animations::Helpers::GetEffectEmphasisNameByID ( EffectID );
			break;
		case 0x00000004:	//	Motion path	+
			return Animations::Helpers::GetEffectMotionPathNameByID ( EffectID );
			break;
		case 0x00000005:	//	Action verb
		case 0x00000006:	//	Media command
		default:
			break;
		}

        return std::wstring ( _T("") );
	}
}

namespace Animations
{
	IRecord* CAnimationFactory::BuildAnimationObject ( WORD Type )
	{
		switch ( Type )
		{
		case Animations::RT_SlideTime10Atom	:
			return new Animations::SlideTime10Atom ();

		case Animations::RT_SlideFlags10Atom:
			return new Animations::SlideFlags10Atom ();

		case Animations::RT_HashCodeAtom:
			return new Animations::HashCode10Atom ();

		case Animations::RT_TimeExtTimeNodeContainer:
			return new Animations::ExtTimeNodeContainer ();

		case Animations::RT_TimeNode:
			return new Animations::TimeNodeAtom ();

		case Animations::RT_TimePropertyList:	//	Specifies a TimePropertyList4TimeNodeContainer or TimePropertyList4TimeBehavior.  
			return new Animations::TimePropertyList4TimeNodeContainer ();

		case Animations::RT_TimeSequenceData:
			return new Animations::TimeSequenceDataAtom ();

		case Animations::RT_TimeConditionContainer:
			return new TimeConditionContainer (); 

		case Animations::RT_TimeCondition:
			return new TimeConditionAtom (); 

		case Animations::RT_TimeClientVisualElement:
			return new ClientVisualElementContainer (); 

		case Animations::RT_VisualPageAtom:
			return new VisualPageAtom ();

		case Animations::RT_TimeEffectBehaviorContainer:
			return new TimeEffectBehaviorContainer ();

		case Animations::RT_TimeEffectBehavior:
			return new TimeEffectBehaviorAtom ();

		case Animations::RT_TimeBehavior:
			return new TimeBehaviorAtom ();

		case Animations::RT_TimeBehaviorContainer:
			return new TimeBehaviorContainer ();

		case Animations::RT_TimeMotionBehaviorContainer:
			return new TimeMotionBehaviorContainer ();

		case Animations::RT_TimeSetBehaviorContainer:
			return new TimeSetBehaviorContainer ();

		case Animations::RT_TimeAnimateBehaviorContainer:
			return new TimeAnimateBehaviorContainer ();

		case Animations::RT_TimeScaleBehaviorContainer:
			return new TimeScaleBehaviorContainer ();
		
		case Animations::RT_TimeRotationBehaviorContainer:
			return new TimeRotationBehaviorContainer ();

		case Animations::RT_TimeSlaveContainer:
			return new SlaveContainer ();

		case Animations::RT_TimeModifierAtom:
			return new TimeModifierAtom();
		}

		return NULL;
	}
}
