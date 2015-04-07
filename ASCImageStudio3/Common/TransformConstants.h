#ifdef _MSC_VER
	#pragma once			// for MSVS
#endif	// #ifdef _MSC_VER

#ifndef _TRANSFORMCONSTANTS_H_	// For Borland compiler
#define _TRANSFORMCONSTANTS_H_
/////////////////////////////////////////////////////////////////
// Constants

const int c_nWipeDoorVerticalIn					= 1100;
const int c_nWipeDoorVerticalOut				= 1101;
const int c_nWipeDoorHorizontalIn				= 1102;
const int c_nWipeDoorHorizontalOut				= 1103;
const int c_nWipeDoorDiagonal					= 1104;
const int c_nWipeDoorDiagonalRotated			= 1105;

const int c_nWipeSideTypeLeftToRight			= 1200;
const int c_nWipeSideTypeRightToLeft			= 1201;
const int c_nWipeSideTypeTopToBottom			= 1202;
const int c_nWipeSideTypeBottomToTop			= 1203;
const int c_nWipeSideTypeLeftTopToRightBottom	= 1204;
const int c_nWipeSideTypeLeftBottomToRightTop	= 1205;
const int c_nWipeSideTypeRightBottomToLeftTop	= 1206;
const int c_nWipeSideTypeRightTopToLeftBottom	= 1207;

const int c_nWipeCenterTypeBoxIn				= 1300;
const int c_nWipeCenterTypeBoxOut				= 1301;
const int c_nWipeCenterTypeDiamondIn			= 1302;
const int c_nWipeCenterTypeDiamondOut			= 1303;
const int c_nWipeCenterTypeCircleIn				= 1304;
const int c_nWipeCenterTypeCircleOut			= 1305;
const int c_nWipeCenterTypeCrossIn				= 1306;
const int c_nWipeCenterTypeCrossOut				= 1307;

const int c_nWipeStripsTypeVertical				= 1400;
const int c_nWipeStripsTypeHorizontal			= 1401;

const int c_nPushDoorVerticalIn					= 2100;
const int c_nPushDoorVerticalOut				= 2101;
const int c_nPushDoorHorizontalIn				= 2102;
const int c_nPushDoorHorizontalOut				= 2103;

const int c_nPushSideTypeLeftToRight			= 2200;
const int c_nPushSideTypeRightToLeft			= 2201;
const int c_nPushSideTypeTopToBottom			= 2202;
const int c_nPushSideTypeBottomToTop			= 2203;

const int c_nPushStripsTypeVertical				= 2300;
const int c_nPushStripsTypeHorizontal			= 2301;

const int c_nMosaicTypeSpiralIn					= 3100;
const int c_nMosaicTypeSpiralOut				= 3101;
const int c_nMosaicTypeStrips					= 3102;
const int c_nMosaicTypeDissolve					= 3103;
const int c_nMosaicTypeClockwise				= 3104;
const int c_nMosaicTypeCounterClockwise			= 3105;
const int c_nMosaicTypeRandom					= 3106;
const int c_nMosaicTypeLeftTopToRightBottom		= 3107;
const int c_nMosaicTypeRightTopToLeftBottom		= 3108;
const int c_nMosaicTypeRightBottomToLeftTop		= 3109;
const int c_nMosaicTypeLeftBottomToRightTop		= 3110;
const int c_nMosaicTypeWallLeftToRight			= 3111;
const int c_nMosaicTypeWallRightToLeft			= 3112;
const int c_nMosaicTypeWallTopToBottom			= 3113;
const int c_nMosaicTypeWallBottomToTop			= 3114;
const int c_nMosaicTypeChessLeftToRight			= 3115;
const int c_nMosaicTypeChessRightToLeft			= 3116;
const int c_nMosaicTypeChessTopToBottom			= 3117;
const int c_nMosaicTypeChessBottomToTop			= 3118;

const int c_nCreditsHMMBottom					= 5100;
const int c_nCreditsHMMCenter					= 5101;
const int c_nCreditsHMMTop						= 5102;
const int c_nCreditsHMWBottom					= 5200;
const int c_nCreditsHMWCenter					= 5201;
const int c_nCreditsHMWTop						= 5202;
const int c_nCreditsHMSBottom					= 5300;
const int c_nCreditsHMSCenter					= 5301;
const int c_nCreditsHMSTop						= 5302;
const int c_nCreditsHMHBottom					= 5400;
const int c_nCreditsHMHCenter					= 5401;
const int c_nCreditsHMHTop						= 5402;
const int c_nCreditsHHMBottom					= 5500;
const int c_nCreditsHHMCenter					= 5501;
const int c_nCreditsHHMTop						= 5502;

const int c_nCreditsVMMLeft						= 6100;
const int c_nCreditsVMMCenter					= 6101;
const int c_nCreditsVMMRight					= 6102;
const int c_nCreditsVMWLeft						= 6200;
const int c_nCreditsVMWCenter					= 6201;
const int c_nCreditsVMWRight					= 6202;
const int c_nCreditsVMSLeft						= 6300;
const int c_nCreditsVMSCenter					= 6301;
const int c_nCreditsVMSRight					= 6302;
const int c_nCreditsVMHLeft						= 6400;
const int c_nCreditsVMHCenter					= 6401;
const int c_nCreditsVMHRight					= 6402;
const int c_nCreditsVHMLeft						= 6500;
const int c_nCreditsVHMCenter					= 6501;
const int c_nCreditsVHMRight					= 6502;

const int c_nMirrorVerticalBottom				= 8100;
const int c_nMirrorVerticalTop					= 8101;
const int c_nMirrorHorizontalLeft				= 8102;
const int c_nMirrorHorizontalRight				= 8103;

const int c_nBorderSolidAll						= 10100;
const int c_nBorderSolidLeft					= 10101;
const int c_nBorderSolidTop						= 10102;
const int c_nBorderSolidRight					= 10103;
const int c_nBorderSolidBottom					= 10104;
const int c_nBorderSolidLeftRight				= 10105;
const int c_nBorderSolidTopBottom				= 10106;

const int c_nBorderTransparentAll				= 10200;
const int c_nBorderTransparentLeft				= 10201;
const int c_nBorderTransparentTop				= 10202;
const int c_nBorderTransparentRight				= 10203;
const int c_nBorderTransparentBottom			= 10204;
const int c_nBorderTransparentLeftRight			= 10205;
const int c_nBorderTransparentTopBottom			= 10206;

const int c_nBorderGradientLightAll				= 10300;
const int c_nBorderGradientLightLeft			= 10301;
const int c_nBorderGradientLightTop				= 10302;
const int c_nBorderGradientLightRight			= 10303;
const int c_nBorderGradientLightBottom			= 10304;
const int c_nBorderGradientLightLeftRight		= 10305;
const int c_nBorderGradientLightTopBottom		= 10306;

const int c_nBorderGradientDarkAll				= 10400;
const int c_nBorderGradientDarkLeft				= 10401;
const int c_nBorderGradientDarkTop				= 10402;
const int c_nBorderGradientDarkRight			= 10403;
const int c_nBorderGradientDarkBottom			= 10404;
const int c_nBorderGradientDarkLeftRight		= 10405;
const int c_nBorderGradientDarkTopBottom		= 10406;

const int c_nMedianHorizontal3					= -5;
const int c_nMedianHorizontal5					= -4;
const int c_nMedianVertical3					= -3;
const int c_nMedianVertical5					= -2;
const int c_nMedianCross3						= -1;
const int c_nMedianCross5						= 0;

/*
const int c_nResizeNearest						= 0x00000000
const int c_nResizeBilinear						= 0x00000001
const int c_nResizeBicubic						= 0x00000010

const int c_nResizeStretch						= 0x00000000
const int c_nResizeShrink						= 0x00000100
const int c_nResizeCrop							= 0x00001100
*/

const int c_nResizeStretchNearest				= 0x00000000;
const int c_nResizeStretchBillinear				= 0x00000001;
const int c_nResizeStretchBicubic				= 0x00000010;
const int c_nResizeStretchSuper                 = 0x00010000;
const int c_nResizeShrinkNearest				= 0x00000100;
const int c_nResizeShrinkBillinear				= 0x00000101;
const int c_nResizeShrinkBicubic				= 0x00000110;
const int c_nResizeShrinkSuper                  = 0x00010100;
const int c_nResizeCropNearest					= 0x00001100;
const int c_nResizeCropBillinear				= 0x00001101;
const int c_nResizeCropBicubic					= 0x00001110;
const int c_nResizeCropSuper					= 0x00011000;



const int c_nFilterPrewittHoriz					= 0;
const int c_nFilterPrewittVertical				= 1;

const int c_nFilterScharrHoriz					= 0;
const int c_nFilterScharrVertical				= 1;

const int c_nFilterSobelExtHoriz				= 0;
const int c_nFilterSobelExtVertical				= 1;

const int c_nFilterSobelHoriz3x3				= 0;
const int c_nFilterSobelHoriz5x5				= 1;
const int c_nFilterSobelVertical3x3				= 2;
const int c_nFilterSobelVertical5x5				= 3;
const int c_nFilterSobelCross3x3				= 4;
const int c_nFilterSobelCross5x5				= 5;
const int c_nFilterSobelSecondHoriz3x3			= 6;
const int c_nFilterSobelSecondHoriz5x5			= 7;
const int c_nFilterSobelSecondVertical3x3		= 8;
const int c_nFilterSobelSecondVertical5x5		= 9;

const int c_nFilterRobertsUp					= 0;
const int c_nFilterRobertsDown					= 1;

const int c_nFilterLaplace3x3					= 0;
const int c_nFilterLaplace5x5					= 1;

const int c_nFilterHipass3x3					= 0;
const int c_nFilterHipass5x5					= 1;

const int c_nFilterLowpass3x3					= 0;
const int c_nFilterLowpass5x5					= 1;

const int c_nTransformCylinderHoriz				= 0;
const int c_nTransformCylinderVert				= 1;

const double c_dFrameMin = 0.001;
const double c_dFrameMax = 0.999;

/////////////////////////////////////////////////////////
// Mosaic patterns

// spiral in
const BYTE c_byMosaicSpiralIn[8][8] = 
{
	{ 0, 1, 2, 3, 4, 5, 6, 7},
	{27,28,29,30,31,32,33, 8},
	{26,47,48,49,50,51,34, 9},
	{25,45,59,60,61,52,35,10},
	{24,45,58,63,62,53,36,11},
	{23,44,57,56,55,54,37,12},
	{22,43,42,41,40,39,38,13},
	{21,20,19,18,17,16,15,14}
};

// spiral out
const BYTE c_byMosaicSpiralOut[8][8] = 
{
	{42,43,44,45,46,47,48,49},
	{41,20,21,22,23,24,25,50},
	{40,19, 6, 7, 8, 9,26,51},
	{39,18, 5, 0, 1,10,27,52},
	{38,17, 4, 3, 2,11,28,53},
	{37,16,15,14,13,12,29,54},
	{36,35,34,33,32,31,30,55},
	{63,62,61,60,59,58,57,56}
};

// strips
const BYTE c_byMosaicStrips[8][8] = 
{
	{ 7,15,23,31,39,47,55,63},
	{63,55,47,39,31,23,15, 7},
	{ 7,15,23,31,39,47,55,63},
	{63,55,47,39,31,23,15, 7},
	{ 7,15,23,31,39,47,55,63},
	{63,55,47,39,31,23,15, 7},
	{ 7,15,23,31,39,47,55,63},
	{63,55,47,39,31,23,15, 7}
};

// dissolve
const BYTE c_byMosaicDissolve[8][8] = 
{
	{35,55,19,59,35,55,19,59},
	{47,23,63,31,47,23,63,31},
	{27,51,39,11,27,51,39,11},
	{43,15, 7, 3,43,15, 7, 3},
	{35,55,19,59,35,55,19,59},
	{47,23,63,31,47,23,63,31},
	{27,51,39,11,27,51,39,11},
	{43,15, 7, 3,43,15, 7, 3}
};

// random
const BYTE c_byMosaicRandom[8][8] = 
{
	{ 0,54,48,13,61, 5,51,45},
	{24,29, 4,33,44,25, 9,20},
	{43,63,37,55,17,30,40,56},
	{21,14,23,10,27,12, 2,32},
	{38,53,34,47, 1,62,46, 8},
	{59, 7,57,22,39,18,36,41},
	{15,42,16,60,11,50,31,19},
	{49,28, 3,35,58,26, 6,52}
};

// diagonal left-bottom
const BYTE c_byMosaicDiagonalLeftBottom[8][8] = 
{
	{ 0, 4, 8,13,17,21,26,30},
	{ 4, 8,13,17,21,26,30,34},
	{ 8,13,17,21,26,30,34,39},
	{13,17,21,26,30,34,39,43},
	{17,21,26,30,34,39,43,48},
	{21,26,30,34,39,43,48,52},
	{26,30,34,39,43,48,52,57},
	{30,34,39,43,48,52,57,63}
};

// diagonal left-top
const BYTE c_byMosaicDiagonalLeftTop[8][8] = 
{
	{31,35,39,44,49,53,58,63},
	{27,31,35,39,44,49,53,58},
	{21,27,31,35,39,44,49,53},
	{17,21,27,31,35,39,44,49},
	{13,17,21,27,31,35,39,44},
	{ 9,13,17,21,27,31,35,39},
	{ 4, 9,13,17,21,27,31,35},
	{ 0, 4, 9,13,17,21,27,31}
};

// diagonal right-bottom
const BYTE c_byMosaicDiagonalRightBottom[8][8] = 
{
	{31,27,21,17,13, 9, 4, 0},
	{35,31,27,21,17,13, 9, 4},
	{39,35,31,27,21,17,13, 9},
	{44,39,35,31,27,21,17,13},
	{49,44,39,35,31,27,21,17},
	{53,49,44,39,35,31,27,21},
	{59,53,49,44,39,35,31,27},
	{63,59,53,49,44,39,35,31}
};

// diagonal right-top
const BYTE c_byMosaicDiagonalRightTop[8][8] = 
{
	{63,59,53,49,44,39,35,31},
	{59,53,49,44,39,35,31,27},
	{53,49,44,39,35,31,27,21},
	{49,44,39,35,31,27,21,17},
	{44,39,35,31,27,21,17,13},
	{39,35,31,27,21,17,13, 9},
	{35,31,27,21,17,13, 9, 4},
	{31,27,21,17,13, 9, 4, 0}
};

// wall left
const BYTE c_byMosaicWallLeft[8][8] = 
{
	{ 0,15,16,31,32,47,48,63},
	{ 1,14,17,30,33,46,49,62},
	{ 2,13,18,29,34,45,50,61},
	{ 3,12,19,28,35,44,51,60},
	{ 4,11,20,27,36,43,52,59},
	{ 5,10,21,26,37,42,53,58},
	{ 6, 9,22,25,38,41,54,57},
	{ 7, 8,23,24,39,40,55,56}
};

// wall right
const BYTE c_byMosaicWallRight[8][8] = 
{
	{63,48,47,32,31,16,15, 0},
	{62,49,46,33,30,17,14, 1},
	{61,50,45,34,29,18,13, 2},
	{60,51,44,35,28,19,12, 3},
	{59,52,43,36,27,20,11, 4},
	{58,53,42,37,26,21,10, 5},
	{57,54,41,38,25,22, 9, 6},
	{56,55,40,39,24,23, 8, 7}
};

// wall bottom
const BYTE c_byMosaicWallBottom[8][8] = 
{
	{ 0, 1, 2, 3, 4, 5, 6, 7},
	{15,14,13,12,11,10, 9, 8},
	{16,17,18,19,20,21,22,23},
	{31,30,29,28,27,26,25,24},
	{32,33,34,35,36,37,38,39},
	{47,46,45,44,43,42,41,40},
	{48,49,50,51,52,53,54,55},
	{63,62,61,60,59,58,57,56}
};

// wall top
const BYTE c_byMosaicWallTop[8][8] = 
{
	{63,62,61,60,59,58,57,56},
	{48,49,50,51,52,53,54,55},
	{47,46,45,44,43,42,41,40},
	{32,33,34,35,36,37,38,39},
	{31,30,29,28,27,26,25,24},
	{16,17,18,19,20,21,22,23},
	{15,14,13,12,11,10, 9, 8},
	{ 0, 1, 2, 3, 4, 5, 6, 7}
};

// chess left
const BYTE c_byMosaicChessLeft[8][8] = 
{
	{ 0,36, 8,44,16,52,24,60},
	{32, 4,40,12,48,20,56,28},
	{ 1,37, 9,45,17,53,25,61},
	{33, 5,41,13,49,21,57,29},
	{ 2,38,10,46,18,54,26,62},
	{34, 6,42,14,50,22,59,30},
	{ 3,39,11,47,19,55,27,63},
	{35, 7,43,15,51,23,59,31}
};

// chess right
const BYTE c_byMosaicChessRight[8][8] = 
{
	{60,24,52,16,44, 8,36, 0},
	{28,56,20,48,12,40, 4,32},
	{61,25,53,17,45, 9,37, 1},
	{29,57,21,49,13,41, 5,33},
	{62,26,54,18,46,10,38, 2},
	{30,59,22,50,14,42, 6,34},
	{63,27,55,19,47,11,39, 3},
	{31,59,23,51,15,43, 7,35}
};

// chess top
const BYTE c_byMosaicChessTop[8][8] = 
{
	{60,28,61,29,62,30,63,31},
	{24,56,25,57,26,58,27,59},
	{52,20,53,21,54,22,55,23},
	{16,48,17,49,18,50,19,51},
	{44,12,45,13,46,14,47,15},
	{ 8,40, 9,41,10,42,11,43},
	{36, 4,37, 5,38, 6,39, 7},
	{ 0,32, 1,33, 2,34, 3,35}
};

// chess bottom
const BYTE c_byMosaicChessBottom[8][8] = 
{
	{ 0,32, 1,33, 2,34, 3,35},
	{36, 4,37, 5,38, 6,39, 7},
	{ 8,40, 9,41,10,42,11,43},
	{44,12,45,13,46,14,47,15},
	{16,48,17,49,18,50,19,51},
	{52,20,53,21,54,22,55,23},
	{24,56,25,57,26,58,27,59},
	{60,28,61,29,62,30,63,31}
};

// counterclockwise
const BYTE c_byMosaicCounterClockwise[8][8] = 
{
	{60,61,63, 3, 5, 6, 9,12},
	{57,59,62, 2, 4, 8,11,13},
	{55,56,58, 1, 7,10,15,16},
	{54,53,52, 0,14,17,18,19},
	{51,50,48,34,24,22,20,21},
	{49,46,43,37,31,27,25,23},
	{47,44,41,38,35,32,28,26},
	{45,42,40,39,36,33,30,29}
};

// clockwise
const BYTE c_byMosaicClockwise[8][8] = 
{
	{ 9, 6, 5, 3,64,63,60,58},
	{10, 8, 4, 2,62,59,57,55},
	{13,12, 7, 1,61,56,54,53},
	{15,14,11, 0,48,49,50,51},
	{16,17,19,26,36,45,46,47},
	{18,20,22,29,34,39,42,44},
	{21,23,27,30,32,37,40,43},
	{24,25,28,31,33,35,38,41}
};

/////////////////////////////////////////////////////////////////

#endif // #ifndef _TRANSFORMCONSTANTS_H_	// For Borland compiler