#ifdef _MSC_VER
	#pragma once			// for MSVS
#endif	// #ifdef _MSC_VER

#ifndef _IMAGEPAINTCOMPOSECONSTANTS_H_	// For Borland compiler
#define _IMAGEPAINTCOMPOSECONSTANTS_H_


namespace ImageStudio
{
	namespace Paint
	{
		namespace Compose
		{
			namespace Constants
			{
				// константы для AlphaComposer'а
				const int c_nAlphaComposeWipeDoorVerticalIn					= 1100;
				const int c_nAlphaComposeWipeDoorVerticalOut				= 1101;
				const int c_nAlphaComposeWipeDoorHorizontalIn				= 1102;
				const int c_nAlphaComposeWipeDoorHorizontalOut				= 1103;
				const int c_nAlphaComposeWipeDoorDiagonal					= 1104;
				const int c_nAlphaComposeWipeDoorDiagonalRotated			= 1105;

				const int c_nAlphaComposeWipeSideTypeLeftToRight			= 1200;
				const int c_nAlphaComposeWipeSideTypeRightToLeft			= 1201;
				const int c_nAlphaComposeWipeSideTypeTopToBottom			= 1202;
				const int c_nAlphaComposeWipeSideTypeBottomToTop			= 1203;
				const int c_nAlphaComposeWipeSideTypeLeftTopToRightBottom	= 1204;
				const int c_nAlphaComposeWipeSideTypeLeftBottomToRightTop	= 1205;
				const int c_nAlphaComposeWipeSideTypeRightBottomToLeftTop	= 1206;
				const int c_nAlphaComposeWipeSideTypeRightTopToLeftBottom	= 1207;

				const int c_nAlphaComposeWipeCenterTypeBoxIn				= 1300;
				const int c_nAlphaComposeWipeCenterTypeBoxOut				= 1301;
				const int c_nAlphaComposeWipeCenterTypeDiamondIn			= 1302;
				const int c_nAlphaComposeWipeCenterTypeDiamondOut			= 1303;
				const int c_nAlphaComposeWipeCenterTypeCircleIn				= 1304;
				const int c_nAlphaComposeWipeCenterTypeCircleOut			= 1305;
				const int c_nAlphaComposeWipeCenterTypeCrossIn				= 1306;
				const int c_nAlphaComposeWipeCenterTypeCrossOut				= 1307;

				const int c_nAlphaComposeWipeStripsTypeVertical				= 1400;
				const int c_nAlphaComposeWipeStripsTypeHorizontal			= 1401;

				const int c_nAlphaComposePushDoorVerticalIn					= 2100;
				const int c_nAlphaComposePushDoorVerticalOut				= 2101;
				const int c_nAlphaComposePushDoorHorizontalIn				= 2102;
				const int c_nAlphaComposePushDoorHorizontalOut				= 2103;

				const int c_nAlphaComposePushSideTypeLeftToRight			= 2200;
				const int c_nAlphaComposePushSideTypeRightToLeft			= 2201;
				const int c_nAlphaComposePushSideTypeTopToBottom			= 2202;
				const int c_nAlphaComposePushSideTypeBottomToTop			= 2203;

				const int c_nAlphaComposePushStripsTypeVertical				= 2300;
				const int c_nAlphaComposePushStripsTypeHorizontal			= 2301;

				const int c_nAlphaComposeMosaicTypeSpiralIn					= 3100;
				const int c_nAlphaComposeMosaicTypeSpiralOut				= 3101;
				const int c_nAlphaComposeMosaicTypeStrips					= 3102;
				const int c_nAlphaComposeMosaicTypeDissolve					= 3103;
				const int c_nAlphaComposeMosaicTypeClockwise				= 3104;
				const int c_nAlphaComposeMosaicTypeCounterClockwise			= 3105;
				const int c_nAlphaComposeMosaicTypeRandom					= 3106;
				const int c_nAlphaComposeMosaicTypeLeftTopToRightBottom		= 3107;
				const int c_nAlphaComposeMosaicTypeRightTopToLeftBottom		= 3108;
				const int c_nAlphaComposeMosaicTypeRightBottomToLeftTop		= 3109;
				const int c_nAlphaComposeMosaicTypeLeftBottomToRightTop		= 3110;
				const int c_nAlphaComposeMosaicTypeWallLeftToRight			= 3111;
				const int c_nAlphaComposeMosaicTypeWallRightToLeft			= 3112;
				const int c_nAlphaComposeMosaicTypeWallTopToBottom			= 3113;
				const int c_nAlphaComposeMosaicTypeWallBottomToTop			= 3114;
				const int c_nAlphaComposeMosaicTypeChessLeftToRight			= 3115;
				const int c_nAlphaComposeMosaicTypeChessRightToLeft			= 3116;
				const int c_nAlphaComposeMosaicTypeChessTopToBottom			= 3117;
				const int c_nAlphaComposeMosaicTypeChessBottomToTop			= 3118;

				// константы для ImageComposer'а (код взят из старой компоненты ImageCompose)

				const int c_nFadeSolid						= 1;	//	AddEffect(0, "Fade Solid Small",							"Fade\nSolid Small",							1, -1, -1);
				const int c_nFadePatternDots				= 2;	//	AddEffect(0, "Fade Dots Small",								"Fade\nDots Small",								2, 8, 8);
				const int c_nFadePatternDotsDense			= 3;	//	AddEffect(0, "Fade Dots Dense Small",						"Fade\nDots Dense Small",						3, 8, 8);
				const int c_nFadePatternGrid				= 4;	//	AddEffect(0, "Fade Grid Small",								"Fade\nGrid Small",								4, 8, 8);
				const int c_nFadePatternGridRotated			= 5;	//	AddEffect(0, "Fade Grid Rotated Small",						"Fade Grid\nRotated Small",						5, 8, 8);
				const int c_nFadePatternLineVertical		= 6;	//	AddEffect(0, "Fade Line Vertical Small",					"Fade Line\nVertical Small",					6, 8, 8);
				const int c_nFadePatternLineHorizontal		= 7;	//	AddEffect(0, "Fade Line Horizontal Small",					"Fade Line\nHorizontal Small",					7, 8, 8);
				const int c_nFadePatternLineDiagonal		= 8;	//	AddEffect(0, "Fade Line Diagonal Small",					"Fade Line\nDiagonal Small",					8, 8, 8);
				const int c_nFadePatternLineDiagonalRotated = 9;	//	AddEffect(0, "Fade Line Diagonal Rotated Small",			"Fade Line\nDiagonal Rotated Small",			9, 8, 8);
				const int c_nFadePatternChess				= 10;	//	AddEffect(0, "Fade Chess Small",							"Fade\nChess Small",							10, 8, 8);
				const int c_nFadePatternDashes				= 11;	//	AddEffect(0, "Fade Dashes Small",							"Fade\nDashes Small",							11, 8, 8);
				const int c_nFadePatternSpiner				= 12;	//	AddEffect(0, "Fade Spiner Small",							"Fade\nSpiner Small",							12, 8, 8);
				const int c_nFadePatternThatches			= 13;	//	AddEffect(0, "Fade Thatches Small",							"Fade\nThatches Small",							13, 8, 8);
				const int c_nFadeZoomIn						= 14;	//	AddEffect(1, "Fade Zoom In",								"Fade\nZoom In",								14, -1, -1);
				const int c_nFadeZoomOut					= 15;	//	AddEffect(1, "Fade Zoom Out",								"Fade\nZoom Out",								15, -1, -1);
				const int c_nFadeZoomBoth					= 16;	//	AddEffect(1, "Fade Zoom Both",								"Fade\nZoom Both",								16, -1, -1);
				const int c_nFadePatternHeart				= 17;	//	AddEffect(0, "Fade Thatches Small",							"Fade\nThatches Small",							13, 8, 8);
				const int c_nFadePatternCross				= 18;	//	AddEffect(0, "Fade Thatches Small",							"Fade\nThatches Small",							13, 8, 8);

				const int c_nFadePatternDots2               = 32;	//	AddEffect(0, "Fade Dots",									"Fade\nDots",									2, 8, 8);
				const int c_nFadePatternDotsDense2          = 33;	//	AddEffect(0, "Fade Dots Dense",								"Fade\nDots Dense",								3, 8, 8);
				const int c_nFadePatternGrid2               = 34;	//	AddEffect(0, "Fade Grid",									"Fade\nGrid",									4, 8, 8);
				const int c_nFadePatternGridRotated2        = 35;	//	AddEffect(0, "Fade Grid Rotated",							"Fade Grid\nRotated",							5, 8, 8);
				const int c_nFadePatternLineVertical2       = 36;	//	AddEffect(0, "Fade Line Vertical",							"Fade Line\nVertical",							6, 8, 8);
				const int c_nFadePatternLineHorizontal2     = 37;	//	AddEffect(0, "Fade Line Horizontal",						"Fade Line\nHorizontal",						7, 8, 8);
				const int c_nFadePatternLineDiagonal2       = 38;	//	AddEffect(0, "Fade Line Diagonal",							"Fade Line\nDiagonal",							8, 8, 8);
				const int c_nFadePatternLineDiagonalRotated2 = 39;	//	AddEffect(0, "Fade Line Diagonal Rotated",					"Fade Line\nDiagonal Rotated",					9, 8, 8);
				const int c_nFadePatternChess2              = 40;	//	AddEffect(0, "Fade Chess",									"Fade\nChess",									10, 8, 8);
				const int c_nFadePatternDashes2             = 41;	//	AddEffect(0, "Fade Dashes",									"Fade\nDashes",									11, 8, 8);
				const int c_nFadePatternSpiner2             = 42;	//	AddEffect(0, "Fade Spiner",									"Fade\nSpiner",									12, 8, 8);
				const int c_nFadePatternThatches2           = 43;	//	AddEffect(0, "Fade Thatches",								"Fade\nThatches",								13, 8, 8);
				const int c_nFadePatternHeart2				= 47;	//	AddEffect(0, "Fade Thatches Small",							"Fade\nThatches Small",							13, 8, 8);
				const int c_nFadePatternCross2				= 48;	//	AddEffect(0, "Fade Thatches Small",							"Fade\nThatches Small",							13, 8, 8);

				const int c_nFadePatternDots3		    	= 62;	//	AddEffect(0, "Fade Dots Large",								"Fade\nDots Large",								2, 8, 8);
				const int c_nFadePatternDotsDense3			= 63;	//	AddEffect(0, "Fade Dots Dense Large",						"Fade\nDots Dense Large",						3, 8, 8);
				const int c_nFadePatternGrid3				= 64;	//	AddEffect(0, "Fade Grid Large",								"Fade\nGrid Large",								4, 8, 8);
				const int c_nFadePatternGridRotated3		= 65;	//	AddEffect(0, "Fade Grid Rotated Large",						"Fade Grid\nRotated Large",						5, 8, 8);
				const int c_nFadePatternLineVertical3		= 66;	//	AddEffect(0, "Fade Line Vertical Large",					"Fade Line\nVertical Large",					6, 8, 8);
				const int c_nFadePatternLineHorizontal3		= 67;	//	AddEffect(0, "Fade Line Horizontal Large",					"Fade Line\nHorizontal Large",					7, 8, 8);
				const int c_nFadePatternLineDiagonal3		= 68;	//	AddEffect(0, "Fade Line Diagonal Large",					"Fade Line\nDiagonal Large",					8, 8, 8);
				const int c_nFadePatternLineDiagonalRotated3 = 69;	//	AddEffect(0, "Fade Line Diagonal Rotated Large",			"Fade Line\nDiagonal Rotated Large",			9, 8, 8);
				const int c_nFadePatternChess3				= 70;	//	AddEffect(0, "Fade Chess Large",							"Fade\nChess Large",							10, 8, 8);
				const int c_nFadePatternDashes3				= 71;	//	AddEffect(0, "Fade Dashes Large",							"Fade\nDashes Large",							11, 8, 8);
				const int c_nFadePatternSpiner3				= 72;	//	AddEffect(0, "Fade Spiner Large",							"Fade\nSpiner Large",							12, 8, 8);
				const int c_nFadePatternThatches3			= 73;	//	AddEffect(0, "Fade Thatches Large",							"Fade\nThatches Large",							13, 8, 8);
				const int c_nFadePatternHeart3				= 77;	//	AddEffect(0, "Fade Thatches Small",							"Fade\nThatches Small",							13, 8, 8);
				const int c_nFadePatternCross3				= 78;	//	AddEffect(0, "Fade Thatches Small",							"Fade\nThatches Small",							13, 8, 8);

				const int c_nMosaicSpiralIn					= 101;	//	AddEffect(0, "Mosaic Spiral In",							"Mosaic\nSpiral In",							101, -1, -1);
				const int c_nMosaicSpiralOut				= 102;	//	AddEffect(0, "Mosaic Spiral Out",							"Mosaic\nSpiral Out",							102, -1, -1);
				const int c_nMosaicStrips					= 103;	//	AddEffect(0, "Mosaic Strips",								"Mosaic\nStrips",								103, -1, -1);
				const int c_nMosaicDissolve					= 104;	//	AddEffect(0, "Mosaic Dissolve",								"Mosaic\nDissolve",								104, 8, 8);
				const int c_nMosaicClockwise				= 105;	//	AddEffect(0, "Mosaic Clockwise",							"Mosaic\nClockwise",							105, 8, 8);
				const int c_nMosaicCounterClockwise			= 106;	//	AddEffect(0, "Mosaic Counterclockwise",						"Mosaic Counter \nClockwise",					106, 8, 8);
				const int c_nMosaicRandom					= 107;	//	AddEffect(0, "Mosaic Random",								"Mosaic\nRandom",								107, 8, 8);
				const int c_nMosaicLeftTop					= 108;	//	AddEffect(0, "Mosaic Left Top", 							"Mosaic\nLeft Top",								108, -1, -1);
				const int c_nMosaicRightTop					= 109;	//	AddEffect(0, "Mosaic Right Top", 							"Mosaic\nRight Top", 							109, -1, -1);
				const int c_nMosaicRightBottom				= 110;	//	AddEffect(0, "Mosaic Right Bottom", 						"Mosaic\nRight Bottom", 						110, -1, -1);
				const int c_nMosaicLeftBottom				= 111;	//	AddEffect(0, "Mosaic Left Bottom", 							"Mosaic\nLeft Bottom", 							111, -1, -1);
				const int c_nMosaicWallLeftToRight			= 112;	//	AddEffect(0, "Mosaic Wall Left To Right", 					"Mosaic Wall\nLeft To Right", 					112, -1, -1);
				const int c_nMosaicWallRightToLeft			= 113;	//	AddEffect(0, "Mosaic Wall Right To Left", 					"Mosaic Wall\nRight To Left", 					113, -1, -1);
				const int c_nMosaicWallTopToBottom			= 114;	//	AddEffect(0, "Mosaic Wall Top To Bottom", 					"Mosaic Wall\nTop To Bottom", 					114, -1, -1);
				const int c_nMosaicWallBottomToTop			= 115;	//	AddEffect(0, "Mosaic Wall Bottom To Top", 					"Mosaic Wall\nBottom To Top", 					115, -1, -1);
				const int c_nMosaicChessLeftToRight			= 116;	//	AddEffect(0, "Mosaic Chess Left To Right", 					"Mosaic Chess\nLeft To Right", 					116, -1, -1);
				const int c_nMosaicChessRightToLeft			= 117;	//	AddEffect(0, "Mosaic Chess Right To Left", 					"Mosaic Chess\nRight To Left", 					117, -1, -1);
				const int c_nMosaicChessTopToBottom			= 118;	//	AddEffect(0, "Mosaic Chess Top To Bottom", 					"Mosaic Chess\nTop To Bottom", 					118, -1, -1);
				const int c_nMosaicChessBottomToTop			= 119;	//	AddEffect(0, "Mosaic Chess Bottom To Top", 					"Mosaic Chess\nBottom To Top", 					119, -1, -1);

				const int c_nDiffuseLeftToRight				= 201;	//	AddEffect(0, "Diffuse Left To Right", 						"Diffuse\nLeft To Right", 						201, -1, -1);
				const int c_nDiffuseRightToLeft				= 202;	//	AddEffect(0, "Diffuse Right To Left", 						"Diffuse\nRight To Left", 						202, -1, -1);
				const int c_nDiffuseTopToBottom				= 203;	//	AddEffect(0, "Diffuse Top To Bottom", 						"Diffuse\nTop To Bottom", 						203, -1, -1);
				const int c_nDiffuseBottomToTop				= 204;	//	AddEffect(0, "Diffuse Bottom To Top", 						"Diffuse\nBottom To Top", 						204, -1, -1);

				const int c_nRotateSingleIn					= 301;	//	AddEffect(0, "Rotate Single In",							"Rotate\nSingle In",							301, -1, -1);
				const int c_nRotateTwiceIn					= 302;	//	AddEffect(0, "Rotate Twice In",								"Rotate\nTwice In",								302, -1, -1);
				const int c_nRotateSingleOut				= 303;	//	AddEffect(0, "Rotate Single Out",							"Rotate\nSingle Out",							303, -1, -1);
				const int c_nRotateTwiceOut					= 304;	//	AddEffect(0, "Rotate Twice Out",							"Rotate\nTwice Out",							304, -1, -1);
				
				const int c_nWipeDoorVerticalIn				= 401;	//	AddEffect(0, "Wipe Door Vertical In",						"Wipe Door\nVertical In",						401, -4, -4);
				const int c_nWipeDoorVerticalOut			= 402;	//	AddEffect(0, "Wipe Door Vertical Out",						"Wipe Door\nVertical Out",						402, -4, -4);
				const int c_nWipeDoorHorizontalIn			= 403;	//	AddEffect(0, "Wipe Door Horizontal In",						"Wipe Door\nHorizontal In",						403, -4, -4);
				const int c_nWipeDoorHorizontalOut			= 404;	//	AddEffect(0, "Wipe Door Horizontal Out",					"Wipe Door\nHorizontal Out",					404, -4, -4);
				const int c_nWipeDoorDiagonal				= 405;	//	AddEffect(0, "Wipe Door Diagonal",							"Wipe Door\nDiagonal",							405, -4, -4);
				const int c_nWipeDoorDiagonalRotated		= 406;	//	AddEffect(0, "Wipe Door Diagonal Rotated", 					"Wipe Door\nDiagonal Rotated",					406, -4, -4);

				const int c_nWipeStripsVertical				= 501;	//	AddEffect(0, "Wipe Strips Vertical",						"Wipe Strips\nVertical",						501, -4, -4);
				const int c_nWipeStripsHorizontal			= 502;	//	AddEffect(0, "Wipe Strips Horizontal",						"Wipe Strips\nHorizontal",						502, -4, -4);

				const int c_nWipeSideLeftToRight			= 601;	//	AddEffect(0, "Wipe Side Left To Right", 					"Wipe Side\nLeft To Right", 					601, -4, -4);
				const int c_nWipeSideRightToLeft			= 602;	//	AddEffect(0, "Wipe Side Right To Left", 					"Wipe Side\nRight To Left", 					602, -4, -4);
				const int c_nWipeSideTopToBottom			= 603;	//	AddEffect(0, "Wipe Side Top To Bottom", 					"Wipe Side\nTop To Bottom", 					603, -4, -4);
				const int c_nWipeSideBottomToTop			= 604;	//	AddEffect(0, "Wipe Side Bottom To Top", 					"Wipe Side\nBottom To Top", 					604, -4, -4);
				const int c_nWipeCornerLeftTop				= 605;	//	AddEffect(0, "Wipe Corner Left Top", 						"Wipe Corner\nLeft Top", 						605, -4, -4);
				const int c_nWipeCornerLeftBottom			= 606;	//	AddEffect(0, "Wipe Corner Left Bottom", 					"Wipe Corner\nLeft Bottom", 					606, -4, -4);
				const int c_nWipeCornerRightBottom			= 607;	//	AddEffect(0, "Wipe Corner Right Bottom", 					"Wipe Corner\nRight Bottom", 					607, -4, -4);
				const int c_nWipeCornerRightTop				= 608;	//	AddEffect(0, "Wipe Corner Right Top", 						"Wipe Corner\nRight Top", 						608, -4, -4);

				const int c_nWipeCenterBoxIn				= 701;	//	AddEffect(0, "Wipe Center Box In",							"Wipe Center\nBox In",							701, -1, -1);
				const int c_nWipeCenterBoxOut				= 702;	//	AddEffect(0, "Wipe Center Box Out",							"Wipe Center\nBox Out",							702, -1, -1);
				const int c_nWipeCenterDiamondIn			= 703;	//	AddEffect(0, "Wipe Center Diamond In",						"Wipe Center\nDiamond In",						703, -1, -1);
				const int c_nWipeCenterDiamondOut			= 704;	//	AddEffect(0, "Wipe Center Diamond Out",						"Wipe Center\nDiamond Out", 					704, -1, -1);
				const int c_nWipeCenterCircleIn				= 705;	//	AddEffect(0, "Wipe Center Circle In",						"Wipe Center\nCircle In",						705, -1, -1);
				const int c_nWipeCenterCircleOut			= 706;	//	AddEffect(0, "Wipe Center Circle Out",						"Wipe Center\nCircle Out",						706, -1, -1);
				const int c_nWipeCenterCrossIn				= 707;	//	AddEffect(0, "Wipe Center Cross In",						"Wipe Center\nCross In",						707, -1, -1);
				const int c_nWipeCenterCrossOut				= 708;	//	AddEffect(0, "Wipe Center Cross Out",						"Wipe Center\nCross Out",						708, -1, -1);

				const int c_nSlideDoorVerticalIn			= 801;	//	AddEffect(0, "Slide Door Vertical In",						"Slide Door\nVertical In",						801, -1, -1);
				const int c_nSlideDoorVerticalOut			= 802;	//	AddEffect(0, "Slide Door Vertical Out",						"Slide Door\nVertical Out",						802, -1, -1);
				const int c_nSlideDoorHorizontalIn			= 803;	//	AddEffect(0, "Slide Door Horizontal In",					"Slide Door\nHorizontal In",					803, -1, -1);
				const int c_nSlideDoorHorizontalOut			= 804;	//	AddEffect(0, "Slide Door Horizontal Out",					"Slide Door\nHorizontal Out",					804, -1, -1);
				const int c_nSlideDoorDiagonal				= 805;	//	AddEffect(0, "Slide Door Diagonal",							"Slide Door\nDiagonal",							805, -1, -1);
				const int c_nSlideDoorDiagonalRotated		= 806;	//	AddEffect(0, "Slide Door Diagonal Rotated",					"Slide Door\nDiagonal Rotated", 				806, -1, -1);
					
				const int c_nSlideStripsVertical			= 901;	//	AddEffect(0, "Slide Strips Vertical",						"Slide Strips\nVertical",						901, -2, -2);
				const int c_nSlideStripsHorizontal			= 902;	//	AddEffect(0, "Slide Strips Horizontal",						"Slide Strips\nHorizontal",						902, -2, -2);

				const int c_nSlideSideLeftToRight			= 1001;	//	AddEffect(0, "Slide Side Left To Right", 					"Slide Side\nLeft To Right",					1001, -8, -8);
				const int c_nSlideSideRightToLeft			= 1002;	//	AddEffect(0, "Slide Side Right To Left", 					"Slide Side\nRight To Left",					1002, -8, -8);
				const int c_nSlideSideTopToBottom			= 1003;	//	AddEffect(0, "Slide Side Top To Bottom", 					"Slide Side\nTop To Bottom",					1003, -8, -8);
				const int c_nSlideSideBottomToTop			= 1004;	//	AddEffect(0, "Slide Side Bottom To Top", 					"Slide Side\nBottom To Top",					1004, -8, -8);
				const int c_nSlideCornerLeftTop				= 1005;	//	AddEffect(0, "Slide Corner Left Top", 						"Slide Corner\nLeft Top", 						1005, -8, -8);
				const int c_nSlideCornerLeftBottom			= 1006;	//	AddEffect(0, "Slide Corner Left Bottom", 					"Slide Corner\nLeft Bottom", 					1006, -8, -8);
				const int c_nSlideCornerRightBottom			= 1007;	//	AddEffect(0, "Slide Corner Right Bottom", 					"Slide Corner\nRight Bottom", 					1007, -8, -8);
				const int c_nSlideCornerRightTop			= 1008;	//	AddEffect(0, "Slide Corner Right Top", 						"Slide Corner\nRight Top", 						1008, -8, -8);

				const int c_nSlideCenterCrossIn				= 1107;	//	AddEffect(0, "Slide Center Cross In",						"Slide Center\nCross In",						1107, -1, -1);
				const int c_nSlideCenterCrossOut			= 1108;	//	AddEffect(0, "Slide Center Cross Out",						"Slide Center\nCross Out",						1108, -1, -1);

				const int c_nPushDoorVerticalIn				= 1201;	//	AddEffect(0, "Push Door Vertical In",						"Push Door\nVertical In",						1201, -1, -1);
				const int c_nPushDoorVerticalOut			= 1202;	//	AddEffect(0, "Push Door Vertical Out",						"Push Door\nVertical Out",						1202, -1, -1);
				const int c_nPushDoorHorizontalIn			= 1203;	//	AddEffect(0, "Push Door Horizontal In",						"Push Door\nHorizontal In",						1203, -1, -1);
				const int c_nPushDoorHorizontalOut			= 1204;	//	AddEffect(0, "Push Door Horizontal Out",					"Push Door\nHorizontal Out",					1204, -1, -1);

				const int c_nPushStripsVertical				= 1301;	//	AddEffect(0, "Push Strips Vertical",						"Push Strips\nVertical",						1301, -1, -1);
				const int c_nPushStripsHorizontal			= 1302;	//	AddEffect(0, "Push Strips Horizontal",						"Push Strips\nHorizontal",						1302, -1, -1);

				const int c_nPushSideLeftToRight			= 1401;	//	AddEffect(0, "Push Side Left To Right",						"Push Side\nLeft To Right",						1401, -1, -1);
				const int c_nPushSideRightToLeft			= 1402;	//	AddEffect(0, "Push Side Right To Left",						"Push Side\nRight To Left",						1402, -1, -1);
				const int c_nPushSideTopToBottom			= 1403;	//	AddEffect(0, "Push Side Top To Bottom",						"Push Side\nTop To Bottom",						1403, -1, -1);
				const int c_nPushSideBottomToTop			= 1404;	//	AddEffect(0, "Push Side Bottom To Top",						"Push Side\nBottom To Top",						1404, -1, -1);

				const int c_nStretchDoorVerticalIn			= 1501;	//	AddEffect(0, "Stretch Door Vertical In",					"Stretch Door\nVertical In",					1501, -1, -1);
				const int c_nStretchDoorVerticalOut			= 1502;	//	AddEffect(0, "Stretch Door Vertical Out",					"Stretch Door\nVertical Out",					1502, -1, -1);
				const int c_nStretchDoorHorizontalIn		= 1503;	//	AddEffect(0, "Stretch Door Horizontal In",					"Stretch Door\nHorizontal In",					1503, -1, -1);
				const int c_nStretchDoorHorizontalOut		= 1504;	//	AddEffect(0, "Stretch Door Horizontal Out",					"Stretch Door\nHorizontal Out",					1504, -1, -1);

				const int c_nStretchStripsVertical			= 1601;	//	AddEffect(0, "Stretch Strips Vertical",						"Stretch Strips\nVertical",						1601, -1, -1);
				const int c_nStretchStripsHorizontal		= 1602;	//	AddEffect(0, "Stretch Strips Horizontal",					"Stretch Strips\nHorizontal",					1602, -1, -1);

				const int c_nStretchSideLeftToRight			= 1701;	//	AddEffect(0, "Stretch Side Left To Right",					"Stretch Side\nLeft To Right",					1701, -1, -1);
				const int c_nStretchSideRightToLeft			= 1702;	//	AddEffect(0, "Stretch Side Right To Left",					"Stretch Side\nRight To Left",					1702, -1, -1);
				const int c_nStretchSideTopToBottom			= 1703;	//	AddEffect(0, "Stretch Side Top To Bottom",					"Stretch Side\nTop To Bottom",					1703, -1, -1);
				const int c_nStretchSideBottomToTop			= 1704;	//	AddEffect(0, "Stretch Side Bottom To Top",					"Stretch Side\nBottom To Top",					1704, -1, -1);
				const int c_nStretchCornerLeftTop			= 1705;	//	AddEffect(0, "Stretch Corner Left Top", 					"Stretch Corner\nLeft Top", 					1705, -1, -1);
				const int c_nStretchCornerLeftBottom		= 1706;	//	AddEffect(0, "Stretch Corner Left Bottom", 					"Stretch Corner\nLeft Bottom", 					1706, -1, -1);
				const int c_nStretchCornerRightBottom		= 1707;	//	AddEffect(0, "Stretch Corner Right Bottom", 				"Stretch Corner\nRight Bottom", 				1707, -1, -1);
				const int c_nStretchCornerRightTop			= 1708;	//	AddEffect(0, "Stretch Corner Right Top", 					"Stretch Corner\nRight Top", 					1708, -1, -1);

				const int c_nStretchCenterBoxIn				= 1801;	//	AddEffect(0, "Stretch Center Box In",						"Stretch Center\nBox In",						1801, -1, -1);
				const int c_nStretchCenterBoxOut			= 1802;	//	AddEffect(0, "Stretch Center Box Out",						"Stretch Center\nBox Out",						1802, -1, -1);

				const int c_nStretchWipeSideLeftToRightIn	= 1901;	//	AddEffect(0, "Stretch Wipe Side Left To Right In",			"Stretch Wipe Side\nLeft To Right In",			1901, -1, -1);
				const int c_nStretchWipeSideLeftToRightOut	= 1902;	//	AddEffect(0, "Stretch Wipe Side Left To Right Out",			"Stretch Wipe Side\nLeft To Right Out",			1902, -1, -1);
				const int c_nStretchWipeSideRightToLeftIn	= 1903;	//	AddEffect(0, "Stretch Wipe Side Right To Left In",			"Stretch Wipe Side\nRight To Left In",			1903, -1, -1);
				const int c_nStretchWipeSideRightToLeftOut	= 1904;	//	AddEffect(0, "Stretch Wipe Side Right To Left Out",			"Stretch Wipe Side\nRight To Left Out",			1904, -1, -1);
				const int c_nStretchWipeSideTopToBottomIn	= 1905;	//	AddEffect(0, "Stretch Wipe Side Top To Bottom In",			"Stretch Wipe Side\nTop To Bottom In",			1905, -1, -1);
				const int c_nStretchWipeSideTopToBottomOut	= 1906;	//	AddEffect(0, "Stretch Wipe Side Top To Bottom Out",			"Stretch Wipe Side\nTop To Bottom Out",			1906, -1, -1);
				const int c_nStretchWipeSideBottomToTopIn	= 1907;	//	AddEffect(0, "Stretch Wipe Side Bottom To Top In",			"Stretch Wipe Side\nBottom To Top In",			1907, -1, -1);
				const int c_nStretchWipeSideBottomToTopOut	= 1908;	//	AddEffect(0, "Stretch Wipe Side Bottom To Top Out",			"Stretch Wipe Side\nBottom To Top Out",			1908, -1, -1);

				const int c_nSwingSideLeftIn				= 2001;	//	AddEffect(1, "Swing Side Left In",							"Swing Side\nLeft In",							2001, -1, -1);
				const int c_nSwingSideLeftOut				= 2002;	//	AddEffect(1, "Swing Side Left Out",							"Swing Side\nLeft Out",							2002, -1, -1);
				const int c_nSwingSideRightIn				= 2003;	//	AddEffect(1, "Swing Side Right In",							"Swing Side\nRight In",							2003, -1, -1);
				const int c_nSwingSideRightOut				= 2004;	//	AddEffect(1, "Swing Side Right Out",						"Swing Side\nRight Out",						2004, -1, -1);
				const int c_nSwingSideTopIn					= 2005;	//	AddEffect(1, "Swing Side Top In",							"Swing Side\nTop In",							2007, -1, -1);
				const int c_nSwingSideTopOut				= 2006;	//	AddEffect(1, "Swing Side Top Out",							"Swing Side\nTop Out",							2008, -1, -1);
				const int c_nSwingSideBottomIn				= 2007;	//	AddEffect(1, "Swing Side Bottom In",						"Swing Side\nBottom In",						2005, -1, -1);
				const int c_nSwingSideBottomOut				= 2008;	//	AddEffect(1, "Swing Side Bottom Out",						"Swing Side\nBottom Out",						2006, -1, -1);

				const int c_nSwingSideChangeLeft			= 2101;	//	AddEffect(1, "Swing Side Change Left", 						"Swing Side\nChange Left", 						2101, -1, -1);
				const int c_nSwingSideChangeRight			= 2102;	//	AddEffect(1, "Swing Side Change Right", 					"Swing Side\nChange Right", 					2102, -1, -1);
				const int c_nSwingSideChangeTop				= 2103;	//	AddEffect(1, "Swing Side Change Top", 						"Swing Side\nChange Top", 						2103, -1, -1);
				const int c_nSwingSideChangeBottom			= 2104;	//	AddEffect(1, "Swing Side Change Bottom", 					"Swing Side\nChange Bottom", 					2104, -1, -1);

				const int c_nSwingDoorsVerticalIn			= 2201;	//	AddEffect(1, "Swing Doors Vertical In",						"Swing Doors\nVertical In",						2201, -1, -1);
				const int c_nSwingDoorsVerticalOut			= 2202;	//	AddEffect(1, "Swing Doors Vertical Out",					"Swing Doors\nVertical Out",					2202, -1, -1);
				const int c_nSwingDoorsHorizontalIn			= 2203;	//	AddEffect(1, "Swing Doors Horizontal In",					"Swing Doors\nHorizontal In",					2203, -1, -1);
				const int c_nSwingDoorsHorizontalOut		= 2204;	//	AddEffect(1, "Swing Doors Horizontal Out",					"Swing Doors\nHorizontal Out",					2204, -1, -1);

				const int c_nSwingDoorsChangeVertical		= 2302;	//	AddEffect(1, "Swing Doors Change Vertical",					"Swing Doors\nChange Vertical",					2302, -1, -1);
				const int c_nSwingDoorsChangeHorizontal		= 2304;	//	AddEffect(1, "Swing Doors Change Horizontal",				"Swing Doors\nChange Horizontal",				2304, -1, -1);

				const int c_nPageTurnLeftTop				= 2405;	//	AddEffect(0, "Page Turn Left Top", 							"Page Turn\nLeft Top",							2405, -1, -1);
				const int c_nPageTurnLeftBottom				= 2406;	//	AddEffect(0, "Page Turn Left Bottom", 						"Page Turn\nLeft Bottom",						2406, -1, -1);
				const int c_nPageTurnRightBottom			= 2407;	//	AddEffect(0, "Page Turn Right Bottom", 						"Page Turn\nRight Bottom",						2407, -1, -1);
				const int c_nPageTurnRightTop				= 2408;	//	AddEffect(0, "Page Turn Right Top", 						"Page Turn\nRight Top",							2408, -1, -1);
				const int c_nPageTurnLeftTop3d				= 2409;	//	AddEffect(0, "Page Turn Left Top", 							"Page Turn\nLeft Top",							2405, -1, -1);
				const int c_nPageTurnLeftBottom3d			= 2410;	//	AddEffect(0, "Page Turn Left Bottom", 						"Page Turn\nLeft Bottom",						2406, -1, -1);
				const int c_nPageTurnRightBottom3d			= 2411;	//	AddEffect(0, "Page Turn Right Bottom", 						"Page Turn\nRight Bottom",						2407, -1, -1);
				const int c_nPageTurnRightTop3d				= 2412;	//	AddEffect(0, "Page Turn Right Top", 						"Page Turn\nRight Top",							2408, -1, -1);

				const int c_nFlipOverVertical				= 2501;	//	AddEffect(1, "Flip Over Vertical",							"Flip Over\nVertical",							2501, -1, -1);
				const int c_nFlipOverVerticalBack			= 2503;	//	AddEffect(1, "Flip Over Vertical Back",						"Flip Over\nVertical Back",						2503, -1, -1);
				const int c_nFlipOverHorizontal				= 2505;	//	AddEffect(1, "Flip Over Horizontal",						"Flip Over\nHorizontal",						2505, -1, -1);
				const int c_nFlipOverHorizontalBack			= 2507;	//	AddEffect(1, "Flip Over Horizontal Back",					"Flip Over\nHorizontal Back",					2507, -1, -1);

				const int c_nFlipAwayVerticalInBack			= 2601;	//	AddEffect(1, "Flip Away Vertical In Back",					"Flip Away\nVertical In Back",					2601, -1, -1);
				const int c_nFlipAwayVerticalOutBack		= 2602;	//	AddEffect(1, "Flip Away Vertical Out Back",					"Flip Away\nVertical Out Back", 				2602, -1, -1);
				const int c_nFlipAwayVerticalIn				= 2603;	//	AddEffect(1, "Flip Away Vertical In",						"Flip Away\nVertical In",						2603, -1, -1);
				const int c_nFlipAwayVerticalOut			= 2604;	//	AddEffect(1, "Flip Away Vertical Out",						"Flip Away\nVertical Out",						2604, -1, -1);
				const int c_nFlipAwayHorizontalInBack		= 2605;	//	AddEffect(1, "Flip Away Horizontal In Back",				"Flip Away\nHorizontal In Back",				2605, -1, -1);
				const int c_nFlipAwayHorizontalOutBack		= 2606;	//	AddEffect(1, "Flip Away Horizontal Out Back",				"Flip Away\nHorizontal Out Back",				2606, -1, -1);
				const int c_nFlipAwayHorizontalIn			= 2607;	//	AddEffect(1, "Flip Away Horizontal In",						"Flip Away\nHorizontal In",						2607, -1, -1);
				const int c_nFlipAwayHorizontalOut			= 2608;	//	AddEffect(1, "Flip Away Horizontal Out",					"Flip Away\nHorizontal Out",					2608, -1, -1);

				const int c_nSlideChangeLeftToRight			= 2701;	//	AddEffect(1, "Slide Change Left To Right", 					"Slide Change\nLeft To Right", 					2701, -1, -1);
				const int c_nSlideChangeRightToLeft			= 2702;	//	AddEffect(1, "Slide Change Right To Left", 					"Slide Change\nRight To Left", 					2702, -1, -1);
				const int c_nSlideChangeTopToBottom			= 2703;	//	AddEffect(1, "Slide Change Top To Bottom", 					"Slide Change\nTop To Bottom", 					2703, -1, -1);
				const int c_nSlideChangeBottomToTop			= 2704;	//	AddEffect(1, "Slide Change Bottom To Top",					"Slide Change\nBottom To Top",					2704, -1, -1);

				const int c_nSlideLinesLeftToRight			= 2801;	//	AddEffect(2, "Slide Lines Left To Right",					"Slide Lines\nLeft To Right",					2801, -1, -1);
				const int c_nSlideLinesRightToLeft			= 2802;	//	AddEffect(2, "Slide Lines Right To Left",					"Slide Lines\nRight To Left",					2802, -1, -1);
				const int c_nSlideLinesTopToBottom			= 2803;	//	AddEffect(2, "Slide Lines Top To Bottom",					"Slide Lines\nTop To Bottom",					2803, -1, -1);
				const int c_nSlideLinesBottomToTop			= 2804;	//	AddEffect(2, "Slide Lines Bottom To Top",					"Slide Lines\nBottom To Top",					2804, -1, -1);

				const int c_nWipeCheckerLeftToRight			= 2901;	//	AddEffect(1, "Wipe Checker Left To Right", 					"Wipe Checker\nLeft To Right", 					2901, -4, -4);
				const int c_nWipeCheckerRightToLeft			= 2902;	//	AddEffect(1, "Wipe Checker Right To Left", 					"Wipe Checker\nRight To Left", 					2902, -4, -4);
				const int c_nWipeCheckerTopToBottom			= 2903;	//	AddEffect(1, "Wipe Checker Top To Bottom", 					"Wipe Checker\nTop To Bottom", 					2903, -4, -4);
				const int c_nWipeCheckerBottomToTop			= 2904;	//	AddEffect(1, "Wipe Checker Bottom To Top", 					"Wipe Checker\nBottom To Top", 					2904, -4, -4);

				const int c_nClockBottomClockwise			= 3001;	//	AddEffect(0, "Clock Bottom Clockwise",						"Clock\nBottom Clockwise",						3001, -1, -1);
				const int c_nClockBottomReverse				= 3002;	//	AddEffect(0, "Clock Bottom Reverse",						"Clock\nBottom Reverse",						3002, -1, -1);
				const int c_nClockBottomOpposite			= 3003;	//	AddEffect(0, "Clock Bottom Opposite",						"Clock\nBottom Opposite",						3003, -1, -1);
				const int c_nClockTopClockwise				= 3004;	//	AddEffect(0, "Clock Top Clockwise",							"Clock\nTop Clockwise",							3004, -1, -1);
				const int c_nClockTopReverse				= 3005;	//	AddEffect(0, "Clock Top Reverse",							"Clock\nTop Reverse",							3005, -1, -1);
				const int c_nClockTopOpposite				= 3006;	//	AddEffect(0, "Clock Top Opposite",							"Clock\nTop Opposite",							3006, -1, -1);
				const int c_nClockLeftClockwise				= 3007;	//	AddEffect(0, "Clock Left Clockwise",						"Clock\nLeft Clockwise",						3007, -1, -1);
				const int c_nClockLeftReverse				= 3008;	//	AddEffect(0, "Clock Left Reverse",							"Clock\nLeft Reverse",							3008, -1, -1);
				const int c_nClockLeftOpposite				= 3009;	//	AddEffect(0, "Clock Left Opposite",							"Clock\nLeft Opposite",							3009, -1, -1);
				const int c_nClockRightClockwise			= 3010;	//	AddEffect(0, "Clock Right Clockwise",						"Clock\nRight Clockwise",						3010, -1, -1);
				const int c_nClockRightReverse				= 3011;	//	AddEffect(0, "Clock Right Reverse",							"Clock\nRight Reverse",							3011, -1, -1);
				const int c_nClockRightOpposite				= 3012;	//	AddEffect(0, "Clock Right Opposite",						"Clock\nRight Opposite",						3012, -1, -1);
				const int c_nClock2AnglesClockwise			= 3013;	//	AddEffect(1, "Clock 2 Angles Clockwise",					"Clock 2\nAngles Clockwise",					3013, -1, -1);
				const int c_nClock2AnglesReverse			= 3014;	//	AddEffect(1, "Clock 2 Angles Reverse",						"Clock 2\nAngles Reverse",						3014, -1, -1);
				const int c_nClock3AnglesClockwise			= 3015;	//	AddEffect(1, "Clock 3 Angles Clockwise",					"Clock 3\nAngles Clockwise",					3015, -1, -1);
				const int c_nClock3AnglesReverse			= 3016;	//	AddEffect(1, "Clock 3 Angles Reverse",						"Clock 3\nAngles Reverse",						3016, -1, -1);
				const int c_nClock4AnglesClockwise			= 3017;	//	AddEffect(1, "Clock 4 Angles Clockwise",					"Clock 4\nAngles Clockwise",					3017, -1, -1);
				const int c_nClock4AnglesReverse			= 3018;	//	AddEffect(1, "Clock 4 Angles Reverse",						"Clock 4\nAngles Reverse",						3018, -1, -1);
				const int c_nClock6AnglesClockwise			= 3019;	//	AddEffect(1, "Clock 6 Angles Clockwise",					"Clock 6\nAngles Clockwise",					3019, -1, -1);
				const int c_nClock6AnglesReverse			= 3020;	//	AddEffect(1, "Clock 6 Angles Reverse",						"Clock 6\nAngles Reverse",						3020, -1, -1);
				const int c_nClock8AnglesClockwise			= 3021;	//	AddEffect(1, "Clock 8 Angles Clockwise",					"Clock 8\nAngles Clockwise",					3021, -1, -1);
				const int c_nClock8AnglesReverse			= 3022;	//	AddEffect(1, "Clock 8 Angles Reverse",						"Clock 8\nAngles Reverse",						3022, -1, -1);
				const int c_nClock12AnglesClockwise			= 3023;	//	AddEffect(1, "Clock 12 Angles Clockwise",					"Clock 12\nAngles Clockwise",					3023, -1, -1);
				const int c_nClock12AnglesReverse			= 3024;	//	AddEffect(1, "Clock 12 Angles Reverse",						"Clock 12\nAngles Reverse",						3024, -1, -1);

				const int c_nWipeRandomStripsVertical		= 3101;	//	AddEffect(1, "Wipe Random Strips Vertical",					"Wipe Random\nStrips Vertical",					3101, -1, -1);
				const int c_nWipeRandomStripsHorizontal		= 3102;	//	AddEffect(1, "Wipe Random Strips Horizontal",				"Wipe Random\nStrips Horizontal",				3102, -1, -1);

				const int c_nShatterCornerRightBottom		= 3205;	//	AddEffect(1, "Shatter Corner Right Bottom", 				"Shatter Corner\nRight Bottom", 				3205, -1, -1);
				const int c_nShatterCornerRightTop			= 3206;	//	AddEffect(1, "Shatter Corner Right Top", 					"Shatter Corner\nRight Top", 					3206, -1, -1);
				const int c_nShatterCornerLeftTop			= 3207;	//	AddEffect(1, "Shatter Corner Left Top", 					"Shatter Corner\nLeft Top", 					3207, -1, -1);
				const int c_nShatterCornerLeftBottom		= 3208;	//	AddEffect(1, "Shatter Corner Left Bottom", 					"Shatter Corner\nLeft Bottom", 					3208, -1, -1);

				const int c_nShatterSideLeftToRight			= 3301;	//	AddEffect(1, "Shatter Side Left To Right", 					"Shatter Side\nLeft To Right",					3301, -1, -1);
				const int c_nShatterSideRightToLeft			= 3302;	//	AddEffect(1, "Shatter Side Right To Left", 					"Shatter Side\nRight To Left",					3302, -1, -1);
				const int c_nShatterSideTopToBottom			= 3303;	//	AddEffect(1, "Shatter Side Top To Bottom", 					"Shatter Side\nTop To Bottom",					3303, -1, -1);
				const int c_nShatterSideBottomToTop			= 3304;	//	AddEffect(1, "Shatter Side Bottom To Top", 					"Shatter Side\nBottom To Top",					3304, -1, -1);

				const int c_nShatterCenterInsideHigh		= 3401;	//	AddEffect(2, "Shatter Center Inside High",					"Shatter Center\nInside High",					3401, -1, -1);
				const int c_nShatterCenterInsideLow			= 3402;	//	AddEffect(2, "Shatter Center Inside Low",					"Shatter Center\nInside Low",					3402, -1, -1);

				const int c_nTetris							= 5401; //	AddEffect(1, "Tertis",										"Tertis",										5401, -2, -2);
				const int c_nTetrisCollapse					= 5402; //	AddEffect(1, "Tertis Collapse",								"Tertis Collapse",								5402, -2, -2);


				const int c_nShotChangeGHorizontalRToL		= 5501; // 	AddEffect(1, "Shot change Grey Horizontal Right To Left",	"Shot change Grey\nRight To Left",				5501, -2, -2);
				const int c_nShotChangeGHorizontalLToR		= 5502; // 	AddEffect(1, "Shot change Grey Horizontal Left To Right",	"Shot change Grey\nLeft To Right",				5502, -2, -2);
				const int c_nShotChangeGVerticalUToB		= 5503;	//	AddEffect(1, "Shot change Grey Vertical Up To Bottom",		"Shot change Grey\nUp To Bottom",				5503, -2, -2);
				const int c_nShotChangeGVerticalBToU		= 5504;	//	AddEffect(1, "Shot change Grey Vertical Bottom To Up",		"Shot change Grey\nBottom To Up",				5504, -2, -2);
				const int c_nShotChangeBHorizontalRToL		= 5505; // 	AddEffect(1, "Shot change Black Horizontal Right To Left",	"Shot change Black\nRight To Left",				5505, -2, -2);
				const int c_nShotChangeBHorizontalLToR		= 5506; // 	AddEffect(1, "Shot change Black Horizontal Left To Right",	"Shot change Black\nLeft To Right",				5506, -2, -2);
				const int c_nShotChangeBVerticalUToB		= 5507;	//	AddEffect(1, "Shot change Black Vertical Up To Bottom",		"Shot change Black\nUp To Bottom",				5507, -2, -2);
				const int c_nShotChangeBVerticalBToU		= 5508;	//	AddEffect(1, "Shot change Black Vertical Bottom To Up",		"Shot change Black\nBottom To Up",				5508, -2, -2);

				const int c_nShotChangeGHorizontalRToLNoZ   = 5509; // 	AddEffect(1, "Shot change Grey Horizontal Right To Left no Zoom", "Shot change Grey\nRight To Left",		5509, -2, -2);
				const int c_nShotChangeGHorizontalLToRNoZ   = 5510; // 	AddEffect(1, "Shot change Grey Horizontal Left To Right no Zoom", "Shot change Grey\nLeft To Right",		5510, -2, -2);
				const int c_nShotChangeGVerticalUToBNoZ     = 5511; // 	AddEffect(1, "Shot change Grey Vertical Up To Bottom no Zoom",    "Shot change Grey\nUp To Bottom",			5511, -2, -2);
				const int c_nShotChangeGVerticalBToUNoZ     = 5512; // 	AddEffect(1, "Shot change Grey Vertical Bottom To Up no Zoom",    "Shot change Grey\nBottom To Up",			5512, -2, -2);
				const int c_nShotChangeBHorizontalRToLNoZ   = 5513; // 	AddEffect(1, "Shot change Black Horizontal Right To Left no Zoom","Shot change Black\nRight To Left",		5513, -2, -2);
				const int c_nShotChangeBHorizontalLToRNoZ   = 5514; // 	AddEffect(1, "Shot change Black Horizontal Left To Right no Zoom","Shot change Black\nLeft To Right",		5514, -2, -2);
				const int c_nShotChangeBVerticalUToBNoZ     = 5515; // 	AddEffect(1, "Shot change Black Vertical Up To Bottom no Zoom",   "Shot change Black\nUp To Bottom",		5515, -2, -2);
				const int c_nShotChangeBVerticalBToUNoZ     = 5516; // 	AddEffect(1, "Shot change Black Vertical Bottom To Up no Zoom",   "Shot change Black\nBottom To Up",		5516, -2, -2);

				const int c_nBlackWhiteLines                = 5601; //  AddEffect(1, "Black White Lines",                           "Black White Lines",					        5601, -2, -2);
				const int c_nBlackWhiteLinesInverse         = 5602; //	AddEffect(1, "Black White Lines Inverse",                   "Black White Lines Inverse",					5602, -2, -2);
				
				const int c_nBowTileHorizontal              = 5301; //  AddEffect(1, "Bow Tile, Horizontal",                        "Bow Tile, Horizontal",			   		        5301, -2, -2);
				const int c_nBowTileVertical                = 5302; //  AddEffect(1, "Bow Tile, Vertical",                          "Bow Tile, Vertical",			   		        5302, -2, -2);
				const int c_nDiagonalBoxOut                 = 5303; //  AddEffect(1, "Diagonal, Box Out",                           "Diagonal, Box Out",			   		        5303, -2, -2);
				const int c_nDiagonalCrossOut               = 5304; //  AddEffect(1, "Diagonal, Cross Out",                         "Diagonal, Cross Out",			   		        5304, -2, -2);
				const int c_nDiagonalDownRight              = 5305; //  AddEffect(1, "Diagonal, Down Right",                        "Diagonal, Down Right",			   		        5305, -2, -2);
				const int c_nDiagonalDownLeft               = 5306; //  AddEffect(1, "Diagonal, Down Left",                         "Diagonal, Down Left",			   		        5306, -2, -2);
				const int c_nDiagonalUpRight                = 5307; //  AddEffect(1, "Diagonal, Up Right",                          "Diagonal, Up Right",			   		        5307, -2, -2);
				const int c_nDiagonalUpLeft                 = 5308; //  AddEffect(1, "Diagonal, Up Left",                           "Diagonal, Up Left",			   		        5308, -2, -2);
			    const int c_nFunIn                          = 5309; //  AddEffect(1, "Fun, In",                                     "Fun, In",			   		                    5309, -2, -2);
			    const int c_nFunOut                         = 5310; //  AddEffect(1, "Fun, Out",                                    "Fun, Out",			   		                    5310, -2, -2);
			    const int c_nFunUp                          = 5311; //  AddEffect(1, "Fun, Up",                                     "Fun, Up",			   		                    5311, -2, -2);
				const int c_nFilledVDown                    = 5312; //  AddEffect(1, "Filled V, Down",                              "Filled V, Down",	     		   		        5312, -2, -2);
				const int c_nFilledVLeft                    = 5313; //  AddEffect(1, "Filled V, Left",                              "Filled V, Left",	     		   		        5313, -2, -2);
				const int c_nFilledVRight                   = 5314; //  AddEffect(1, "Filled V, Right",                             "Filled V, Right",	     		   		        5314, -2, -2);
				const int c_nFilledVUp                      = 5315; //  AddEffect(1, "Filled V, Up",                                "Filled V, Up",	     		   	     	        5315, -2, -2);
				const int c_nInsertDownLeft                 = 5316; //  AddEffect(1, "Insert, Down Left",                           "Insert, Down Left",	     		   		    5316, -2, -2);
				const int c_nInsertDownRight                = 5317; //  AddEffect(1, "Insert, Down Right",                          "Insert, Down Right",	     		   		    5317, -2, -2);
				const int c_nInsertUpLeft                   = 5318; //  AddEffect(1, "Insert, Up Left",                             "Insert, Up Left",	     		   		        5318, -2, -2);
				const int c_nInsertUpRight                  = 5319; //	AddEffect(1, "Insert, Up Right",                            "Insert, Up Right",	     		   	     	    5319, -2, -2);
				const int c_nRevealDown                     = 5320; //	AddEffect(1, "Reveal, Down",                                "Reveal, Down",	     		   		            5320, -2, -2);
				const int c_nRevealRight                    = 5321; //	AddEffect(1, "Reveal, Right",                               "Reveal, Right",	     		   		        5321, -2, -2);
				const int c_nRevealLeft                     = 5322; //	AddEffect(1, "Reveal, Left",                                "Reveal, Left",	     		   		            5322, -2, -2);
				const int c_nRevealUp                       = 5323; //	AddEffect(1, "Reveal, Up",                                  "Reveal, Up",	     		   	     	        5323, -2, -2);
				const int c_nSplitHorizontal                = 5324; //	AddEffect(1, "Split, Horizontal",                           "Split, Horizontal",	     		   		    5324, -2, -2);
				const int c_nSplitVertical                  = 5325; //	AddEffect(1, "Split, Vertical",                             "Split, Vertical",	     		   	     	    5325, -2, -2);
				const int c_nSweepIn                        = 5326; //	AddEffect(1, "Sweep, In",                                   "Sweep, In",			   		                5326, -2, -2);
				const int c_nSweepOut                       = 5327; //	AddEffect(1, "Sweep, Out",                                  "Sweep, Out",			   		                5327, -2, -2);
				const int c_nSweepUp                        = 5328; //	AddEffect(1, "Sweep, Up",                                   "Sweep, Up",			   		                5328, -2, -2);
				const int c_nZigZagHorizontal               = 5329; //	AddEffect(1, "ZigZag, Horizontal",                          "ZigZag, Horizontal",			   		        5329, -2, -2);
				const int c_nZigZagVertical                 = 5330; //	AddEffect(1, "ZigZag, Vertical",                            "ZigZag, Vertical",			   		            5330, -2, -2);
				const int c_nBarsHorizontal                 = 5331; //	AddEffect(1, "Bars, Horizontal",						    "Bars, Horizontal",	     			            5331, -2, -2);
				const int c_nBarsVertical                   = 5332; //	AddEffect(1, "Bars, Vertical",						        "Bars, Vertical",	     			            5332, -2, -2);

				const int c_nBowTileSmoothHorizontal        = 5333; //  AddEffect(1, "Bow Tile Smooth, Horizontal",                 "Bow Tile Smooth, Horizontal",			   		5333, -2, -2);
				const int c_nBowTileSmoothVertical          = 5334; //  AddEffect(1, "Bow Tile Smooth, Vertical",                   "Bow Tile Smooth, Vertical",			   		5334, -2, -2);
				const int c_nDiagonalSmoothBoxOut           = 5335; //  AddEffect(1, "Diagonal Smooth, Box Out",                    "Diagonal Smooth, Box Out",			   		    5335, -2, -2);
				const int c_nDiagonalSmoothCrossOut         = 5336; //  AddEffect(1, "Diagonal Smooth, Cross Out",                  "Diagonal Smooth, Cross Out",			   		5336, -2, -2);
				const int c_nDiagonalSmoothDownRight        = 5337; //  AddEffect(1, "Diagonal Smooth, Down Right",                 "Diagonal Smooth, Down Right",			   		5337, -2, -2);
				const int c_nDiagonalSmoothDownLeft         = 5338; //  AddEffect(1, "Diagonal Smooth, Down Left",                  "Diagonal Smooth, Down Left",			   		5338, -2, -2);
				const int c_nDiagonalSmoothUpRight          = 5339; //  AddEffect(1, "Diagonal Smooth, Up Right",                   "Diagonal Smooth, Up Right",			   		5339, -2, -2);
				const int c_nDiagonalSmoothUpLeft           = 5340; //  AddEffect(1, "Diagonal Smooth, Up Left",                    "Diagonal Smooth, Up Left",			   		    5340, -2, -2);
				const int c_nFunSmoothIn                    = 5341; //  AddEffect(1, "Fun Smooth, In",                              "Fun Smooth, In",			   		            5341, -2, -2);
				const int c_nFunSmoothOut                   = 5342; //  AddEffect(1, "Fun Smooth, Out",                             "Fun Smooth, Out",			   		            5342, -2, -2);
				const int c_nFunSmoothUp                    = 5343; //  AddEffect(1, "Fun Smooth, Up",                              "Fun Smooth, Up",			   		            5343, -2, -2);
				const int c_nFilledVSmoothDown              = 5344; //  AddEffect(1, "Filled V Smooth, Down",                       "Filled V Smooth, Down",	     		   		5344, -2, -2);
				const int c_nFilledVSmoothLeft              = 5345; //  AddEffect(1, "Filled V Smooth, Left",                       "Filled V Smooth, Left",	     		   		5345, -2, -2);
				const int c_nFilledVSmoothRight             = 5346; //  AddEffect(1, "Filled V Smooth, Right",                      "Filled V Smooth, Right",	     		   		5346, -2, -2);
				const int c_nFilledVSmoothUp                = 5347; //  AddEffect(1, "Filled V Smooth, Up",                         "Filled V Smooth, Up",	     		   	     	5347, -2, -2);
				const int c_nInsertSmoothDownLeft           = 5348; //  AddEffect(1, "Insert Smooth, Down Left",                    "Insert Smooth, Down Left",	     		   		5348, -2, -2);
				const int c_nInsertSmoothDownRight          = 5349; //  AddEffect(1, "Insert Smooth, Down Right",                   "Insert Smooth, Down Right",	     		   	5349, -2, -2);
				const int c_nInsertSmoothUpLeft             = 5350; //  AddEffect(1, "Insert Smooth, Up Left",                      "Insert Smooth, Up Left",	     		   		5350, -2, -2);
				const int c_nInsertSmoothUpRight            = 5351; //	AddEffect(1, "Insert Smooth, Up Right",                     "Insert Smooth, Up Right",	     		   	    5351, -2, -2);
				const int c_nSplitSmoothHorizontal          = 5352; //	AddEffect(1, "Split Smooth, Horizontal",                    "Split Smooth, Horizontal",	     		   		5352, -2, -2);
				const int c_nSplitSmoothVertical            = 5353; //	AddEffect(1, "Split Smooth, Vertical",                      "Split Smooth, Vertical",	     		   	    5353, -2, -2);
				const int c_nSweepSmoothIn                  = 5354; //	AddEffect(1, "Sweep Smooth, In",                            "Sweep Smooth, In",			   		            5354, -2, -2);
				const int c_nSweepSmoothOut                 = 5355; //	AddEffect(1, "Sweep Smooth, Out",                           "Sweep Smooth, Out",			   		        5355, -2, -2);
				const int c_nSweepSmoothUp                  = 5356; //	AddEffect(1, "Sweep Smooth, Up",                            "Sweep Smooth, Up",			   		            5356, -2, -2);
				const int c_nZigZagSmoothHorizontal         = 5357; //	AddEffect(1, "ZigZag Smooth, Horizontal",                   "ZigZag Smooth, Horizontal",			   		5357, -2, -2);
				const int c_nZigZagSmoothVertical           = 5358; //	AddEffect(1, "ZigZag Smooth, Vertical",                     "ZigZag Smooth, Vertical",			   		    5358, -2, -2);
				
				const int c_nWipeSmoothCircle               = 5359; //  AddEffect(1, "Wipe Smooth Center Circle",					"Wipe Smooth\nCenter Circle",					5359, -1, -1);
				const int c_nWipeSmoothCircles              = 5360; //  AddEffect(1, "Wipe Smooth Circles",							"Wipe Smooth\nCircles",							5360, -5, -5);
				const int c_nWipeSmoothDiamond              = 5361; //  AddEffect(1, "Wipe Smooth Center Diamond",					"Wipe Smooth\nCenter Diamond",					5361, -1, -1);
				const int c_nWipeSmoothDiamonds             = 5362; //  AddEffect(1, "Wipe Smooth Diamonds",						"Wipe Smooth\nDiamonds",						5362, -5, -5);
				const int c_nWipeSmoothWideStar				= 5363; //  AddEffect(1, "Wipe Smooth Wide Center Star",				"Wipe Smooth Wide\nCenter Star",				5363, -1, -1);
				const int c_nWipeSmoothWideStars            = 5364; //  AddEffect(1, "Wipe Smooth Wide Stars",						"Wipe Smooth Wide\nStars",						5364, -5, -5);
				const int c_nWipeSmoothWideFlower           = 5365; //  AddEffect(1, "Wipe Smooth Wide Center Flower",				"Wipe Smooth Wide\nCenter Flower",				5365, -1, -1);
				const int c_nWipeSmoothWideFlowers          = 5366; //  AddEffect(1, "Wipe Smooth Wide Flowers",					"Wipe Smooth Wide\nFlowers",					5366, -5, -5);
				const int c_nWipeSmoothWideKeyHole          = 5367; //  AddEffect(1, "Wipe Smooth Wide Center KeyHole",				"Wipe Smooth Wide\nCenter KeyHole",				5367, -1, -1);
				const int c_nWipeSmoothWideKeyHoles         = 5368; //  AddEffect(1, "Wipe Smooth Wide KeyHoles",					"Wipe Smooth Wide\nKeyHoles",					5368, -5, -5);
				const int c_nWipeSmoothNarrowStar           = 5369; //  AddEffect(1, "Wipe Smooth Narrow Center Star",				"Wipe Smooth Narrow\nCenter Star",				5369, -1, -1);
				const int c_nWipeSmoothNarrowStars          = 5370; //  AddEffect(1, "Wipe Smooth Narrow Stars",					"Wipe Smooth Narrow\nStars",					5370, -5, -5);
				const int c_nWipeSmoothNarrowKeyHole        = 5371; //  AddEffect(1, "Wipe Smooth Narrow Center KeyHole",			"Wipe Smooth Narrow\nCenter KeyHole",			5371, -1, -1);
				const int c_nWipeSmoothNarrowKeyHoles       = 5372; //  AddEffect(1, "Wipe Smooth Narrow KeyHoles",					"Wipe Smooth Narrow\nKeyHoles",					5372, -5, -5);
				const int c_nWipeSmoothNarrowFlower         = 5373; //  AddEffect(1, "Wipe Smooth Narrow Center Flower",			"Wipe Smooth Narrow\nCenter Flower",			5373, -1, -1);
				const int c_nWipeSmoothNarrowFlowers        = 5374; //  AddEffect(1, "Wipe Smooth Narrow Flowers",					"Wipe Smooth Narrow\nFlowers",					5374, -5, -5);

				const int c_nCircleCornerLeftTop            = 5381; //  AddEffect(1, "Corner Circle, Left Top", 					"Corner Circle\nLeft Top", 				        5381, -1, -1);
				const int c_nCircleCornerLeftBottom         = 5382; //  AddEffect(1, "Corner Circle, Left Bottom", 					"Corner Circle\nLeft Bottom", 				    5382, -1, -1);
				const int c_nCircleCornerRightTop           = 5383; //  AddEffect(1, "Corner Circle, Right Top", 					"Corner Circle\nRight Top", 				    5383, -1, -1);
				const int c_nCircleCornerRightBottom        = 5384; //  AddEffect(1, "Corner Circle, Right Bottom", 				"Corner Circle\nRight Bottom", 			        5384, -1, -1);
				const int c_nCircleCornerSmoothLeftTop      = 5385; //  AddEffect(1, "Corner Circle Smooth, Left Top", 				"Corner Circle Smooth\nLeft Top", 				5385, -1, -1);
				const int c_nCircleCornerSmoothLeftBottom   = 5386; //  AddEffect(1, "Corner Circle Smooth, Left Bottom", 			"Corner Circle Smooth\nLeft Bottom", 			5386, -1, -1);
				const int c_nCircleCornerSmoothRightTop     = 5387; //  AddEffect(1, "Corner Circle Smooth, Right Top", 			"Corner Circle Smooth\nRight Top", 				5387, -1, -1);
				const int c_nCircleCornerSmoothRightBottom  = 5388; //  AddEffect(1, "Corner Circle Smooth, Right Bottom", 			"Corner Circle Smooth\nRight Bottom", 			5388, -1, -1);
				const int c_nWipeSmoothCircleIn             = 5389; //  AddEffect(1, "Stencil Smooth Circle In",					"Stencil Smooth\nCircle In",				    5389, -1, -1);
				const int c_nWipeCircles                    = 5390; //  AddEffect(1, "Stencil Circles",						        "Stencil\nCircles",						        5390, -5, -5);
				const int c_nWipeSmoothBoxIn                = 5391; //  AddEffect(0, "Stencil Smooth Box In",						"Stencil Smooth\nBox In",						5391, -1, -1);
				const int c_nWipeSmoothBoxOut               = 5392; //  AddEffect(0, "Stencil Smooth Box Out",						"Stencil Smooth\nBox Out",						5392, -1, -1);
				const int c_nWipeSmoothDiamondIn            = 5393; //  AddEffect(1, "Stencil Smooth Diamond In",				    "Stencil Smooth\nDiamond In",					5393, -1, -1);
				const int c_nWipeDiamonds                   = 5394; //  AddEffect(1, "Stencil Diamonds",						    "Stencil\nDiamonds",						    5394, -5, -5);

				const int c_nPixelate                       = 5701; //	AddEffect(1, "Pixelate",                                    "Pixelate",			   		                    5701, -2, -2);
				const int c_nRollRight                      = 5702; //	AddEffect(1, "Roll, Right",                                 "Roll, Right",			   		                5702, -2, -2);
				const int c_nRollLeft                       = 5703; //	AddEffect(1, "Roll, Left",                                  "Roll, Left",			   		                5703, -2, -2);
				const int c_nSlideUp                        = 5704; //	AddEffect(1, "Slide, Up",                                   "Slide, Up",			   		                5704, -2, -2);
				const int c_nSlideDown                      = 5705; //	AddEffect(1, "Slide, Down",                                 "Slide, Down",			   		                5705, -2, -2);
				const int c_nSlideRight                     = 5706; //	AddEffect(1, "Slide, Right",                                "Slide, Right",			   		                5706, -2, -2);
				const int c_nSlideLeft                      = 5707; //	AddEffect(1, "Slide, Left",                                 "Slide, Left",			   		                5707, -2, -2);
				const int c_nSlideCenterUp                  = 5708; //	AddEffect(1, "Slide, Up Center",                            "Slide, Up Center",			   		            5708, -2, -2);
				const int c_nSlideCenterDown                = 5709; //	AddEffect(1, "Slide, Down Center",                          "Slide, Down Center",			   		        5709, -2, -2);
				const int c_nSlideCenterRight               = 5710; //	AddEffect(1, "Slide, Right Center",                         "Slide, Right Center",			   		        5710, -2, -2);
				const int c_nSlideCenterLeft                = 5711; //	AddEffect(1, "Slide, Left center",                          "Slide, Left Center",			   		        5711, -2, -2);
				const int c_nShrinkInVertical               = 5712; //	AddEffect(1, "Shrink, In Vertical",						    "Shrink, In Vertical",			   			    5712, -2, -2);
				const int c_nShrinkInHorizontal             = 5713; //	AddEffect(1, "Shrink, In Horizontal",						"Shrink, In Horizontal",			   			5713, -2, -2);
			    const int c_nEye                            = 5714; //	AddEffect(1, "Eye",						                    "Eye",			   			                    5714, -2, -2);
			    const int c_nHeart                          = 5715; //	AddEffect(1, "Heart",						                "Heart",	     			                    5715, -2, -2);
				const int c_nWave                           = 5716; //	AddEffect(1, "Wave",						                "Wave",			   			                    5716, -2, -2);
				const int c_nSmoothEye                      = 5717; //	AddEffect(1, "Stencil Smooth Eye",						    "Stencil Smooth\nEye",			   			    5717, -2, -2);
				const int c_nSmoothHeart                    = 5718; //	AddEffect(1, "Stencil Smooth Heart",						"Stencil Smooth\nHeart",	     			    5718, -2, -2);
				const int c_nSmoothWave                     = 5719; //	AddEffect(1, "Stencil Smooth Wave",						    "Stencil Smooth\nWave",			   			    5719, -2, -2);
				
				const int c_nSpiralTopLeft                  = 5801; //  AddEffect(0, "Spiral, Top Left",					        "Spiral\nTop Left",					            5801, -1, -1);
				const int c_nSpiralTopRight                 = 5802; //  AddEffect(0, "Spiral, Top Right",					        "Spiral\nTop Right",					        5802, -1, -1);
				const int c_nSpiralBottomLeft               = 5803; //  AddEffect(0, "Spiral, Bottom Left",					        "Spiral\nBottom Left",					        5803, -1, -1);
				const int c_nSpiralBottomRight              = 5804; //  AddEffect(0, "Spiral, Bottom Right",					    "Spiral\nBottom Right",					        5804, -1, -1);
				const int c_nSpiralCenterIn                 = 5805; //  AddEffect(0, "Spiral, Center In",							"Spiral\nCenter In",		  	                5805, -1, -1);
				const int c_nSpiralReverseCenterIn          = 5806; //  AddEffect(0, "Spiral, Reverse Center In",					"Spiral\nReverse Center In",					5806, -1, -1);
				const int c_nSpiralCenterOut                = 5807; //  AddEffect(0, "Spiral, Center Out",							"Spiral\nCenter Out",		  	                5807, -1, -1);
				const int c_nSpiralReverseCenterOut         = 5808; //  AddEffect(0, "Spiral, Reverse Center Out",					"Spiral\nReverse Center Out",					5808, -1, -1);

				const int c_nPageTurnLeftTopWhiteBack3d     = 2420; //  AddEffect(1, "Page Turn 3D Left Top White Back", 			"Page Turn 3D\nLeft Top White Back",			2420, -1, -1);
				const int c_nPageTurnLeftBottomWhiteBack3d  = 2421; //  AddEffect(1, "Page Turn 3D Left Bottom White Back", 		"Page Turn 3D\nLeft Bottom White Back",			2421, -1, -1);
				const int c_nPageTurnRightBottomWhiteBack3d = 2422; //  AddEffect(1, "Page Turn 3D Right Bottom White Back", 		"Page Turn 3D\nRight Bottom White Back",		2422, -1, -1);
				const int c_nPageTurnRightTopWhiteBack3d    = 2423; //  AddEffect(1, "Page Turn 3D Right Top White Back", 			"Page Turn 3D\nRight Top White Back",			2423, -1, -1);
				
				const int c_nPageTurnAlbumLeftTop           = 2424; //  AddEffect(1, "Page Turn Album Left Top", 					"Page Turn\nAlbum Left Top",					2424, -1, -1);
				const int c_nPageTurnAlbumRightTop          = 2425; //  AddEffect(1, "Page Turn Album Right Top", 					"Page Turn\nAlbum Right Top",					2425, -1, -1);
				const int c_nPageTurnAlbumRightBottom       = 2426; //  AddEffect(1, "Page Turn Album Right Bottom", 				"Page Turn\nAlbum Right Bottom",				2426, -1, -1);
				const int c_nPageTurnAlbumLeftBottom        = 2427; //  AddEffect(1, "Page Turn Album Left Bottom", 				"Page Turn\nAlbum Left Bottom",					2427, -1, -1);

				const int c_nPageTurnAlbumLeftTopCorners1     = 2428; //  AddEffect(1, "Page Turn Album Left Top Corners", 			"Page Turn\nAlbum Left Top Corners",			2428, -1, -1);
				const int c_nPageTurnAlbumRightTopCorners1    = 2429; //  AddEffect(1, "Page Turn Album Right Top Corners", 			"Page Turn\nAlbum Right Top Corners",			2429, -1, -1);
				const int c_nPageTurnAlbumRightBottomCorners1 = 2430; //  AddEffect(1, "Page Turn Album Right Bottom Corners", 		"Page Turn\nAlbum Right Bottom Corners",		2430, -1, -1);
				const int c_nPageTurnAlbumLeftBottomCorners1  = 2431; //  AddEffect(1, "Page Turn Album Left Bottom Corners", 		"Page Turn\nAlbum Left Bottom Corners",			2431, -1, -1);

				const int c_nPageTurnAlbumLeftTopCorners2     = 2432; //  AddEffect(1, "Page Turn Album Left Top Corners", 			"Page Turn\nAlbum Left Top Corners",			2428, -1, -1);
				const int c_nPageTurnAlbumRightTopCorners2    = 2433; //  AddEffect(1, "Page Turn Album Right Top Corners", 			"Page Turn\nAlbum Right Top Corners",			2429, -1, -1);
				const int c_nPageTurnAlbumRightBottomCorners2 = 2434; //  AddEffect(1, "Page Turn Album Right Bottom Corners", 		"Page Turn\nAlbum Right Bottom Corners",		2430, -1, -1);
				const int c_nPageTurnAlbumLeftBottomCorners2  = 2435; //  AddEffect(1, "Page Turn Album Left Bottom Corners", 		"Page Turn\nAlbum Left Bottom Corners",			2431, -1, -1);

				const int c_nPageTurnAlbumLeftTopCorners3     = 2436; //  AddEffect(1, "Page Turn Album Left Top Corners", 			"Page Turn\nAlbum Left Top Corners",			2428, -1, -1);
				const int c_nPageTurnAlbumRightTopCorners3    = 2437; //  AddEffect(1, "Page Turn Album Right Top Corners", 			"Page Turn\nAlbum Right Top Corners",			2429, -1, -1);
				const int c_nPageTurnAlbumRightBottomCorners3 = 2438; //  AddEffect(1, "Page Turn Album Right Bottom Corners", 		"Page Turn\nAlbum Right Bottom Corners",		2430, -1, -1);
				const int c_nPageTurnAlbumLeftBottomCorners3  = 2439; //  AddEffect(1, "Page Turn Album Left Bottom Corners", 		"Page Turn\nAlbum Left Bottom Corners",			2431, -1, -1);

				const int c_nPageTurnAlbumLeftTopCorners4     = 2440; //  AddEffect(1, "Page Turn Album Left Top Corners", 			"Page Turn\nAlbum Left Top Corners",			2428, -1, -1);
				const int c_nPageTurnAlbumRightTopCorners4    = 2441; //  AddEffect(1, "Page Turn Album Right Top Corners", 			"Page Turn\nAlbum Right Top Corners",			2429, -1, -1);
				const int c_nPageTurnAlbumRightBottomCorners4 = 2442; //  AddEffect(1, "Page Turn Album Right Bottom Corners", 		"Page Turn\nAlbum Right Bottom Corners",		2430, -1, -1);
				const int c_nPageTurnAlbumLeftBottomCorners4  = 2443; //  AddEffect(1, "Page Turn Album Left Bottom Corners", 		"Page Turn\nAlbum Left Bottom Corners",			2431, -1, -1);

				const int c_nWhirlWind                      = 3305; //	AddEffect(1, "WhirlWind",						            "WhirlWind",	     			                3305, -2, -2);
			
				const int c_nRotateHalfIn	                = 305;  //	AddEffect(0, "Rotate Half In",				                "Rotate \nHalf In",				                305, -1, -1);
				const int c_nRotateHalfOut	                = 306;  //	AddEffect(0, "Rotate Half Out",				                "Rotate \nHalf Out",				            306, -1, -1);
				const int c_nRotateTransparentSingleIn	    = 307;  //	AddEffect(0, "Rotate Transparent Single In",				"Rotate Transparent\nSingle In",				307, -1, -1);
				const int c_nRotateTransparentTwiceIn       = 308;  //	AddEffect(0, "Rotate Transparent Twice In",					"Rotate Transparent\nTwice In",					308, -1, -1);
				const int c_nRotateTransparentSingleOut     = 309;  //	AddEffect(0, "Rotate Transparent Single Out",				"Rotate Transparent\nSingle Out",				309, -1, -1);
				const int c_nRotateTransparentTwiceOut	    = 310;  //	AddEffect(0, "Rotate Transparent Twice Out",				"Rotate Transparent\nTwice Out",				310, -1, -1);
				const int c_nRotateTransparentHalfIn	    = 311;  //	AddEffect(0, "Rotate Transparent Half In",					"Rotate Transparent\nHalf In",					311, -1, -1);
				const int c_nRotateTransparentHalfOut	    = 312;  //	AddEffect(0, "Rotate Transparent Half Out",				    "Rotate Transparent\nHalf Out",				    312, -1, -1);
				
				const int c_nRotateTwiceOutClockwise		= 313;	//	AddEffect(0, "Rotate Twice Out Clockwise",					"Rotate\nTwice Out Clockwise",					313, -1, -1);

				const int c_nGraduateSideLeftToRight		= 3501;	//	AddEffect(1, "Graduate Side Left To Right", 				"Graduate Side\nLeft To Right",					3501, -4, -4);
				const int c_nGraduateSideRightToLeft		= 3502;	//	AddEffect(1, "Graduate Side Right To Left", 				"Graduate Side\nRight To Left",					3502, -4, -4);
				const int c_nGraduateSideTopToBottom		= 3503;	//	AddEffect(1, "Graduate Side Top To Bottom", 				"Graduate Side\nTop To Bottom",					3503, -4, -4);
				const int c_nGraduateSideBottomToTop		= 3504;	//	AddEffect(1, "Graduate Side Bottom To Top", 				"Graduate Side\nBottom To Top",					3504, -4, -4);
				const int c_nGraduateCornerLeftTop 			= 3505;	//	AddEffect(1, "Graduate Corner Left Top", 					"Graduate Corner\nLeft Top", 					3505, -4, -4);
				const int c_nGraduateCornerLeftBottom		= 3506;	//	AddEffect(1, "Graduate Corner Left Bottom", 				"Graduate Corner\nLeft Bottom", 				3506, -4, -4);
				const int c_nGraduateCornerRightBottom		= 3507;	//	AddEffect(1, "Graduate Corner Right Bottom", 				"Graduate Corner\nRight Bottom", 				3507, -4, -4);
				const int c_nGraduateCornerRightTop 		= 3508;	//	AddEffect(1, "Graduate Corner Right Top", 					"Graduate Corner\nRight Top", 					3508, -4, -4);

				const int c_nGraduateClockBottomClockwise	= 3601;	//	AddEffect(1, "Graduate Clock Bottom Clockwise",				"Graduate Clock\nBottom Clockwise",				3601, -2, -2);
				const int c_nGraduateClockBottomReverse		= 3602;	//	AddEffect(1, "Graduate Clock Bottom Reverse",				"Graduate Clock\nBottom Reverse",				3602, -2, -2);
				const int c_nGraduateClockBottomOpposite	= 3603;	//	AddEffect(1, "Graduate Clock Bottom Opposite",				"Graduate Clock\nBottom Opposite",				3603, -2, -2);
				const int c_nGraduateClockTopClockwise		= 3604;	//	AddEffect(1, "Graduate Clock Top Clockwise",				"Graduate Clock\nTop Clockwise",				3604, -2, -2);
				const int c_nGraduateClockTopReverse 		= 3605;	//	AddEffect(1, "Graduate Clock Top Reverse",					"Graduate Clock\nTop Reverse",					3605, -2, -2);
				const int c_nGraduateClockTopOpposite 		= 3606;	//	AddEffect(1, "Graduate Clock Top Opposite",					"Graduate Clock\nTop Opposite",					3606, -2, -2);
				const int c_nGraduateClockLeftClockwise		= 3607;	//	AddEffect(1, "Graduate Clock Left Clockwise",				"Graduate Clock\nLeft Clockwise",				3607, -2, -2);
				const int c_nGraduateClockLeftReverse 		= 3608;	//	AddEffect(1, "Graduate Clock Left Reverse",					"Graduate Clock\nLeft Reverse",					3608, -2, -2);
				const int c_nGraduateClockLeftOpposite 		= 3609;	//	AddEffect(1, "Graduate Clock Left Opposite",				"Graduate Clock\nLeft Opposite",				3609, -2, -2);
				const int c_nGraduateClockRightClockwise	= 3610;	//	AddEffect(1, "Graduate Clock Right Clockwise",				"Graduate Clock\nRight Clockwise",				3610, -2, -2);
				const int c_nGraduateClockRightReverse		= 3611;	//	AddEffect(1, "Graduate Clock Right Reverse",				"Graduate Clock\nRight Reverse",				3611, -2, -2);
				const int c_nGraduateClockRightOpposite		= 3612;	//	AddEffect(1, "Graduate Clock Right Opposite",				"Graduate Clock\nRight Opposite",				3612, -2, -2);
				const int c_nGraduateClock2AnglesClockwise 	= 3613;	//	AddEffect(1, "Graduate Clock 2 Angles Clockwise",			"Graduate Clock\n2 Angles Clockwise",			3613, -2, -2);
				const int c_nGraduateClock2AnglesReverse	= 3614;	//	AddEffect(1, "Graduate Clock 2 Angles Reverse",				"Graduate Clock\n2 Angles Reverse",				3614, -2, -2);
				const int c_nGraduateClock3AnglesClockwise 	= 3615;	//	AddEffect(1, "Graduate Clock 3 Angles Clockwise",			"Graduate Clock\n3 Angles Clockwise",			3615, -2, -2);
				const int c_nGraduateClock3AnglesReverse	= 3616;	//	AddEffect(1, "Graduate Clock 3 Angles Reverse",				"Graduate Clock\n3 Angles Reverse",				3616, -2, -2);

				const int c_nGraduateSideLeftToRightSmooth		 = 3617; //	AddEffect(1, "Graduate Smooth Side Left To Right", 			"Graduate Smooth Side\nLeft To Right",			3617, -4, -4);
				const int c_nGraduateSideRightToLeftSmooth		 = 3618; //	AddEffect(1, "Graduate Smooth Side Right To Left", 			"Graduate Smooth Side\nRight To Left",			3618, -4, -4);
				const int c_nGraduateSideTopToBottomSmooth		 = 3619; //	AddEffect(1, "Graduate Smooth Side Top To Bottom", 			"Graduate Smooth Side\nTop To Bottom",			3619, -4, -4);
				const int c_nGraduateSideBottomToTopSmooth		 = 3620; //	AddEffect(1, "Graduate Smooth Side Bottom To Top", 			"Graduate Smooth Side\nBottom To Top",			3620, -4, -4);
				const int c_nGraduateCornerLeftTopSmooth		 = 3621; //	AddEffect(1, "Graduate Smooth Corner Left Top", 			"Graduate Smooth Corner\nLeft Top", 			3621, -4, -4);
				const int c_nGraduateCornerLeftBottomSmooth		 = 3622; //	AddEffect(1, "Graduate Smooth Corner Left Bottom", 			"Graduate Smooth Corner\nLeft Bottom", 			3622, -4, -4);
				const int c_nGraduateCornerRightBottomSmooth	 = 3623; //	AddEffect(1, "Graduate Smooth Corner Right Bottom", 		"Graduate Smooth Corner\nRight Bottom", 		3623, -4, -4);
				const int c_nGraduateCornerRightTopSmooth 		 = 3624; //	AddEffect(1, "Graduate Smooth Corner Right Top", 			"Graduate Smooth Corner\nRight Top", 			3624, -4, -4);
				const int c_nGraduateClockBottomClockwiseSmooth	 = 3625; //	AddEffect(1, "Graduate Smooth Clock Bottom Clockwise",		"Graduate Smooth Clock\nBottom Clockwise",		3625, -2, -2);
				const int c_nGraduateClockBottomReverseSmooth	 = 3626; //	AddEffect(1, "Graduate Smooth Clock Bottom Reverse",		"Graduate Smooth Clock\nBottom Reverse",		3626, -2, -2);
				const int c_nGraduateClockBottomOppositeSmooth	 = 3627; //	AddEffect(1, "Graduate Smooth Clock Bottom Opposite",		"Graduate Smooth Clock\nBottom Opposite",		3627, -2, -2);
				const int c_nGraduateClockTopClockwiseSmooth	 = 3628; //	AddEffect(1, "Graduate Smooth Clock Top Clockwise",			"Graduate Smooth Clock\nTop Clockwise",			3628, -2, -2);
				const int c_nGraduateClockTopReverseSmooth 		 = 3629; //	AddEffect(1, "Graduate Smooth Clock Top Reverse",			"Graduate Smooth Clock\nTop Reverse",			3629, -2, -2);
				const int c_nGraduateClockTopOppositeSmooth		 = 3630; //	AddEffect(1, "Graduate Smooth Clock Top Opposite",			"Graduate Smooth Clock\nTop Opposite",			3630, -2, -2);
				const int c_nGraduateClockLeftClockwiseSmooth	 = 3631; //	AddEffect(1, "Graduate Smooth Clock Left Clockwise",		"Graduate Smooth Clock\nLeft Clockwise",		3631, -2, -2);
				const int c_nGraduateClockLeftReverseSmooth 	 = 3632; //	AddEffect(1, "Graduate Smooth Clock Left Reverse",			"Graduate Smooth Clock\nLeft Reverse",			3632, -2, -2);
				const int c_nGraduateClockLeftOppositeSmooth 	 = 3633; //	AddEffect(1, "Graduate Smooth Clock Left Opposite",			"Graduate Smooth Clock\nLeft Opposite",			3633, -2, -2);
				const int c_nGraduateClockRightClockwiseSmooth	 = 3634; //	AddEffect(1, "Graduate Smooth Clock Right Clockwise",		"Graduate Smooth Clock\nRight Clockwise",		3634, -2, -2);
				const int c_nGraduateClockRightReverseSmooth	 = 3635; //	AddEffect(1, "Graduate Smooth Clock Right Reverse",			"Graduate Smooth Clock\nRight Reverse",			3635, -2, -2);
				const int c_nGraduateClockRightOppositeSmooth	 = 3636; //	AddEffect(1, "Graduate Smooth Clock Right Opposite",		"Graduate Smooth Clock\nRight Opposite",		3636, -2, -2);
				const int c_nGraduateClock2AnglesClockwiseSmooth = 3637; //	AddEffect(1, "Graduate Smooth Clock 2 Angles Clockwise",	"Graduate Smooth Clock\n2 Angles Clockwise",	3637, -2, -2);
				const int c_nGraduateClock2AnglesReverseSmooth	 = 3638; //	AddEffect(1, "Graduate Smooth Clock 2 Angles Reverse",		"Graduate Smooth Clock\n2 Angles Reverse",		3638, -2, -2);
				const int c_nGraduateClock3AnglesClockwiseSmooth = 3639; //	AddEffect(1, "Graduate Smooth Clock 3 Angles Clockwise",	"Graduate Smooth Clock\n3 Angles Clockwise",	3639, -2, -2);
				const int c_nGraduateClock3AnglesReverseSmooth	 = 3640; //	AddEffect(1, "Graduate Smooth Clock 3 Angles Reverse",		"Graduate Smooth Clock\n3 Angles Reverse",		3640, -2, -2);

				const int c_nTwirlClockwise					= 3701;	//	AddEffect(1, "Twirl Clockwise",								"Twirl\nClockwise",								3701, -1, -1);
				const int c_nTwirlClockwiseZoomOut			= 3702;	//	AddEffect(1, "Twirl Clockwise Zoom Out",					"Twirl Clockwise\n Zoom Out",					3702, -1, -1);
				const int c_nTwirlClockwiseZoomIn			= 3703;	//	AddEffect(2, "Twirl Clockwise Zoom In",						"Twirl Clockwise\nZoom In",						3703, -1, -1);
				const int c_nTwirlReverse					= 3704;	//	AddEffect(1, "Twirl Reverse",								"Twirl\nReverse",								3704, -1, -1);
				const int c_nTwirlReverseZoomOut			= 3705;	//	AddEffect(1, "Twirl Reverse Zoom Out",						"Twirl Reverse\nZoom Out",						3705, -1, -1);
				const int c_nTwirlReverseZoomIn				= 3706;	//	AddEffect(2, "Twirl Reverse Zoom In",						"Twirl Reverse\nZoom In",						3706, -1, -1);
				const int c_nTwirlDrain						= 3707;	//	AddEffect(2, "Twirl Drain",						            "Twirl \nDrain",						        3707, -1, -1);
			    const int c_nTwirlReverseDrain			    = 3708;	//	AddEffect(2, "Twirl Reverse Drain",						    "Twirl  Reverse\nDrain",						3708, -1, -1);

				const int c_nSphereInside					= 3801;	//	AddEffect(2, "Sphere Inside",								"Sphere\nInside",								3801, -1, -1);
				const int c_nSphereOutside					= 3802;	//	AddEffect(1, "Sphere Outside",								"Sphere\nOutside",								3802, -1, -1);

				const int c_nTwistLeftToRight				= 3901;	//	AddEffect(2, "Twist Left To Right", 						"Twist Left\nTo Right", 						3901, -1, -1);
				const int c_nTwistRightToLeft				= 3902;	//	AddEffect(2, "Twist Right To Left", 						"Twist Right\nTo Left", 						3902, -1, -1);
				const int c_nTwistTopToBottom				= 3903;	//	AddEffect(2, "Twist Top To Bottom", 						"Twist Top\nTo Bottom", 						3903, -1, -1);
				const int c_nTwistBottomToTop				= 3904;	//	AddEffect(2, "Twist Bottom To Top", 						"Twist Bottom\nTo Top", 						3904, -1, -1);
				const int c_nTwistLeftTop	 				= 3905;	//	AddEffect(2, "Twist Left Top",	 							"Twist\nLeft Top", 								3905, -1, -1);
				const int c_nTwistLeftBottom				= 3906;	//	AddEffect(2, "Twist Left Bottom",	 						"Twist\nLeft Bottom", 							3906, -1, -1);
				const int c_nTwistRightTop	 				= 3907;	//	AddEffect(2, "Twist Right Top",	 							"Twist\nRight Top", 							3907, -1, -1);
				const int c_nTwistRightBottom				= 3908;	//	AddEffect(2, "Twist Right Bottom",	 						"Twist\nRight Bottom", 							3908, -1, -1);

				const int c_nRippleOutside					= 4001;	//	AddEffect(2, "Ripple Outside",								"Ripple\nOutside",								4001, -1, -1);
				const int c_nRippleInside					= 4002;	//	AddEffect(2, "Ripple Inside",								"Ripple\nInside",								4002, -1, -1);

				const int c_nWaveHorizontal					= 4101;	//	AddEffect(0, "Wave Horizontal",								"Wave\nHorizontal",								4101, -1, -1);
				const int c_nWaveVertical					= 4102;	//	AddEffect(0, "Wave Vertical",								"Wave\nVertical",								4102, -1, -1);

				const int c_nEnhancedBurningThreshold		= 4201;	//	AddEffect(2, "Burning Threshold",							"Burning\nThreshold",							4201, -1, -1);
				const int c_nEnhancedBurningFade			= 4202;	//	AddEffect(2, "Burning Fade",								"Burning\nFade",								4202, -1, -1);
				const int c_nEnhancedBurningFire			= 4203;	//	AddEffect(2, "Burning Fire",								"Burning\nFire",								4203, -1, -1);

				const int c_nFadeSideLeftToRight 			= 4301;	//	AddEffect(2, "Fade Side Left To Right",						"Fade Side\nLeft To Right",						4301, -1, -1);
				const int c_nFadeSideRightToLeft 			= 4302;	//	AddEffect(2, "Fade Side Right To Left",						"Fade Side\nRight To Left",						4302, -1, -1);
				const int c_nFadeSideTopToBottom 			= 4303;	//	AddEffect(2, "Fade Side Top To Bottom",						"Fade Side\nTop To Bottom",						4303, -1, -1);
				const int c_nFadeSideBottomToTop 			= 4304;	//	AddEffect(2, "Fade Side Bottom To Top",						"Fade Side\nBottom To Top",						4304, -1, -1);

				const int c_nClockCornerLeftTop 		 	= 4501;	//	AddEffect(0, "Clock Corner Left Top", 						"Clock Corner\nLeft Top", 						4501, -1, -1);
				const int c_nClockCornerLeftBottom 		 	= 4502;	//	AddEffect(0, "Clock Corner Left Bottom", 					"Clock Corner\nLeft Bottom", 					4502, -1, -1);
				const int c_nClockCornerRightTop 		 	= 4503;	//	AddEffect(0, "Clock Corner Right Top", 						"Clock Corner\nRight Top", 						4503, -1, -1);
				const int c_nClockCornerRightBottom 	 	= 4504;	//	AddEffect(0, "Clock Corner Right Bottom", 					"Clock Corner\nRight Bottom", 					4504, -1, -1);
				const int c_nClockCornerReverseLeftTop 	 	= 4505;	//	AddEffect(0, "Clock Corner Reverse Left Top", 				"Clock Corner\nReverse Left Top", 				4505, -1, -1);
				const int c_nClockCornerReverseLeftBottom 	= 4506;	//	AddEffect(0, "Clock Corner Reverse Left Bottom", 			"Clock Corner\nReverse Left Bottom", 			4506, -1, -1);
				const int c_nClockCornerReverseRightTop  	= 4507;	//	AddEffect(0, "Clock Corner Reverse Right Top", 				"Clock Corner\nReverse Right Top", 				4507, -1, -1);
				const int c_nClockCornerReverseRightBottom 	= 4508;	//	AddEffect(0, "Clock Corner Reverse Right Bottom", 			"Clock Corner\nReverse Right Bottom", 			4508, -1, -1);

				const int c_nFlipFlyVerticalIn 				= 4601;	//	AddEffect(2, "Flip Fly Vertical In",						"Flip Fly\nVertical In",						4601, -1, -1);
				const int c_nFlipFlyVerticalOut 			= 4603;	//	AddEffect(2, "Flip Fly Vertical Out",						"Flip Fly\nVertical Out",						4603, -1, -1);
				const int c_nFlipFlyHorizontalIn 			= 4605;	//	AddEffect(2, "Flip Fly Horizontal In",						"Flip Fly\nHorizontal In",						4605, -1, -1);
				const int c_nFlipFlyHorizontalOut 			= 4607;	//	AddEffect(2, "Flip Fly Horizontal Out",						"Flip Fly\nHorizontal Out",						4607, -1, -1);

				const int c_nStretchFoldLeftToRight 		= 4701;	//	AddEffect(Stretch Fold Left To Right",						"Stretch Fold\nLeft To Right",					4701, -4, -4);
				const int c_nStretchFoldRightToLeft 		= 4702;	//	AddEffect(Stretch Fold Right To Left",						"Stretch Fold\nRight To Left",					4702, -4, -4);
				const int c_nStretchFoldTopToBottom 		= 4703;	//	AddEffect(Stretch Fold Top To Bottom",						"Stretch Fold\nTop To Bottom",					4703, -4, -4);
				const int c_nStretchFoldBottomToTop 		= 4704;	//	AddEffect(Stretch Fold Bottom To Top",						"Stretch Fold\nBottom To Top",					4704, -4, -4);

				const int c_nEnhancedGaussianBlur			= 4801;	//	AddEffect(1, "Gaussian Blur", 								"Gaussian\nBlur", 								4801, -1, -1);

				const int c_nEnhancedColorNoise				= 4901;	//	AddEffect(0, "Color Noise", 								"Color\nNoise", 								4901, -1, -1);
				const int c_nEnhancedGrayscaleNoise			= 4902;	//	AddEffect(0, "Grayscale Noise", 							"Grayscale\nNoise", 							4902, -1, -1);
				const int c_nEnhancedFlashLight				= 4903;	//	AddEffect(0, "Flash Light", 								"Flash\nLight", 								4903, -1, -1);
				const int c_nEnhancedFlashDark				= 4904;	//	AddEffect(0, "Flash Dark", 									"Flash\nDark", 									4904, -1, -1);
				const int c_nEnhancedAlphaNoise				= 4905;	
				const int c_nEnhancedShutter				= 4906;	

				const int c_nFlipBoxesLeftToRight			= 5001;	//	AddEffect(2, "Flip Boxes Left To Right",					"Flip Boxes\nLeft To Right",					5001, -8, -8);
				const int c_nFlipBoxesRightToLeft			= 5002;	//	AddEffect(2, "Flip Boxes Right To Left",					"Flip Boxes\nRight To Left",					5002, -8, -8);
				const int c_nFlipBoxesTopToBottom			= 5003;	//	AddEffect(2, "Flip Boxes Top To Bottom",					"Flip Boxes\nTop To Bottom",					5003, -8, -8);
				const int c_nFlipBoxesBottomToTop			= 5004;	//	AddEffect(2, "Flip Boxes Bottom To Top",					"Flip Boxes\nBottom To Top",					5004, -8, -8);

				const int c_nFlipOutBoxesVertical			= 5101;	//	AddEffect(1, "Flip Out Boxes Vertical",						"Flip Out Boxes\nVertical",						5101, -4, -4);
				const int c_nFlipOutBoxesVerticalBack		= 5103;	//	AddEffect(1, "Flip Out Boxes Vertical Back",				"Flip Out Boxes\nVertical Back",				5103, -4, -4);
				const int c_nFlipOutBoxesHorizontal			= 5105;	//	AddEffect(1, "Flip Out Boxes Horizontal",					"Flip Out Boxes\nHorizontal",					5105, -4, -4);
				const int c_nFlipOutBoxesHorizontalBack		= 5107;	//	AddEffect(1, "Flip Out Boxes Horizontal Back",				"Flip Out Boxes\nHorizontal Back",				5107, -4, -4);

				const int c_nPinwheelCurvedSN5A    		    = 5901;	//	AddEffect(1, "Pinwheel Small Narrow Curve, 5 Angles",		"Pinwheel Small Narrow Curve, 5 Angles",		5901, -1, -1);
				const int c_nPinwheelCurvedSN7A		        = 5902;	//	AddEffect(1, "Pinwheel Small Narrow Curve, 7 Angles",		"Pinwheel Small Narrow Curve, 7 Angles",		5902, -1, -1);
				const int c_nPinwheelCurvedSN12A		    = 5903;	//	AddEffect(1, "Pinwheel Small Narrow Curve, 12 Angles",		"Pinwheel Small Narrow Curve, 12 Angles",		5903, -1, -1);
				const int c_nPinwheelCurvedSW5A		        = 5904;	//	AddEffect(1, "Pinwheel Small Wide Curve, 5 Angles",		    "Pinwheel Small Wide Curve, 5 Angles",		    5904, -1, -1);
				const int c_nPinwheelCurvedSW7A		        = 5905;	//	AddEffect(1, "Pinwheel Small Wide Curve, 7 Angles",		    "Pinwheel Small Wide Curve, 7 Angles",		    5905, -1, -1);
				const int c_nPinwheelCurvedSW12A		    = 5906;	//	AddEffect(1, "Pinwheel Small Wide Curve, 12 Angles",		"Pinwheel Small Wide Curve, 12 Angles",		    5906, -1, -1);
				const int c_nPinwheelCurvedLN5A		        = 5907;	//	AddEffect(1, "Pinwheel Large Curve, 5 Angles",		        "Pinwheel Large Curve, 5 Angles",		        5907, -1, -1);
				const int c_nPinwheelCurvedLN7A		        = 5908;	//	AddEffect(1, "Pinwheel Large Curve, 7 Angles",		        "Pinwheel Large Curve, 7 Angles",		        5908, -1, -1);
				const int c_nPinwheelCurvedLN12A		    = 5909;	//	AddEffect(1, "Pinwheel Large Curve, 12 Angles",		        "Pinwheel Large Curve, 12 Angles",		        5909, -1, -1);
				const int c_nPinwheelPropeller4		        = 5910;	//	AddEffect(1, "Propeller, 4 Angles",		                    "Propeller, 4 Angles",		                    5910, -1, -1);
				const int c_nPinwheelPropeller7		        = 5911;	//	AddEffect(1, "Propeller, 7 Angles",		                    "Propeller, 7 Angles",		                    5911, -1, -1);
				const int c_nPinwheelPropeller12		    = 5912;	//	AddEffect(1, "Propeller, 12 Angles",		                "Propeller, 12 Angles",		                    5912, -1, -1);

				const int c_nPinwheelCurvedSN5ASmooth		= 5913;	//	AddEffect(1, "Pinwheel Small Narrow Curve Smooth, 5 Angles", "Pinwheel Small Narrow Curve Smooth, 5 Angles", 5913, -1, -1);
				const int c_nPinwheelCurvedSN7ASmooth		= 5914;	//	AddEffect(1, "Pinwheel Small Narrow Curve Smooth, 7 Angles", "Pinwheel Small Narrow Curve Smooth, 7 Angles", 5914, -1, -1);
				const int c_nPinwheelCurvedSN12ASmooth		= 5915;	//	AddEffect(1, "Pinwheel Small Narrow Curve Smooth, 12 Angles","Pinwheel Small Narrow Curve Smooth, 12 Angles",5915, -1, -1);
				const int c_nPinwheelCurvedSW5ASmooth   	= 5916;	//	AddEffect(1, "Pinwheel Small Wide Curve Smooth, 5 Angles",	 "Pinwheel Small Wide Curve Smooth, 5 Angles",	 5916, -1, -1);
				const int c_nPinwheelCurvedSW7ASmooth		= 5917;	//	AddEffect(1, "Pinwheel Small Wide Curve Smooth, 7 Angles",	 "Pinwheel Small Wide Curve Smooth, 7 Angles",	 5917, -1, -1);
				const int c_nPinwheelCurvedSW12ASmooth		= 5918;	//	AddEffect(1, "Pinwheel Small Wide Curve Smooth, 12 Angles",	 "Pinwheel Small Wide Curve Smooth, 12 Angles",	 5918, -1, -1);
				const int c_nPinwheelCurvedLN5ASmooth		= 5919;	//	AddEffect(1, "Pinwheel Large Curve Smooth, 5 Angles",		 "Pinwheel Large Curve Smooth, 5 Angles",		 5919, -1, -1);
				const int c_nPinwheelCurvedLN7ASmooth		= 5920;	//	AddEffect(1, "Pinwheel Large Curve Smooth, 7 Angles",		 "Pinwheel Large Curve Smooth, 7 Angles",		 5920, -1, -1);
				const int c_nPinwheelCurvedLN12ASmooth		= 5921;	//	AddEffect(1, "Pinwheel Large Curve Smooth, 12 Angles",		 "Pinwheel Large Curve Smooth, 12 Angles",		 5921, -1, -1);
				const int c_nPinwheelPropeller4Smooth		= 5922;	//	AddEffect(1, "Propeller Smooth, 4 Angles",		             "Propeller Smooth, 4 Angles",		             5922, -1, -1);
				const int c_nPinwheelPropeller7Smooth		= 5923;	//	AddEffect(1, "Propeller Smooth, 7 Angles",		             "Propeller Smooth, 7 Angles",		             5923, -1, -1);
				const int c_nPinwheelPropeller12Smooth		= 5924;	//	AddEffect(1, "Propeller Smooth, 12 Angles",		             "Propeller Smooth, 12 Angles",		             5924, -1, -1);

				
				const int c_nPuzzle4x4		                = 6001;	//	AddEffect(1, "Game Puzzle   4x4",						    "Game\nPuzzle   4x4",						    6001, -2, -2);
				const int c_nPuzzle7x7		                = 6002;	//	AddEffect(1, "Game Puzzle   7x7",						    "Game\nPuzzle   7x7",						    6002, -2, -2);
				const int c_nPuzzle10x10		            = 6003;	//	AddEffect(1, "Game Puzzle 10x10",						    "Game\nPuzzle 10x10",						    6003, -2, -2);
				const int c_nPuzzle4x4Collapse		        = 6004;	//	AddEffect(1, "Game Puzzle   4x4 Collapse",  			    "Game\nPuzzle   4x4 Collapse",					6004, -2, -2);
				const int c_nPuzzle7x7Collapse		        = 6005;	//	AddEffect(1, "Game Puzzle   7x7 Collapse",					"Game\nPuzzle   7x7 Collapse",					6005, -2, -2);
				const int c_nPuzzle10x10Collapse		    = 6006;	//	AddEffect(1, "Game Puzzle 10x10 Collapse",					"Game\nPuzzle 10x10 Collapse",					6006, -2, -2);
				const int c_nTile4x4Cons		            = 6007;	//	AddEffect(1, "Game Tile 4x4 Consecutive",					"Game\nGame Tile 4x4 Consecutive",				6007, -2, -2);
				const int c_nTile4x4Rand	                = 6008;	//	AddEffect(1, "Game Tile 4x4 Randomly",						"Game\nGame Tile 4x4 Randomly",					6008, -2, -2);
				const int c_nTile7x7Cons		            = 6009;	//	AddEffect(1, "Game Tile 7x7 Consecutive",					"Game\nGame Tile 7x7 Consecutive",				6009, -2, -2);
				const int c_nTile7x7Rand		            = 6010;	//	AddEffect(1, "Game Tile 7x7 Randomly",						"Game\nGame Tile 7x7 Randomly",					6010, -2, -2);
				const int c_nTile10x10Cons		            = 6011;	//	AddEffect(1, "Game Tile 10x10 Consecutive",					"Game\nGame Tile 10x10 Consecutive",			6011, -2, -2);
				const int c_nTile10x10Rand		            = 6012;	//	AddEffect(1, "Game Tile 10x10 Randomly",					"Game\nGame Tile 10x10 Randomly",				6012, -2, -2);
				const int c_nTile4x4ConsCollapse		    = 6013;	//	AddEffect(1, "Game Tile 4x4 Consecutive Collapse",			"Game\nGame Tile 4x4 Consecutive Collapse",		6013, -2, -2);
				const int c_nTile4x4RandCollapse		    = 6014;	//	AddEffect(1, "Game Tile 4x4 Randomly Collapse",				"Game\nGame Tile 4x4 Randomly Collapse",		6014, -2, -2);
				const int c_nTile7x7ConsCollapse		    = 6015;	//	AddEffect(1, "Game Tile 7x7 Consecutive Collapse",			"Game\nGame Tile 7x7 Consecutive Collapse",		6015, -2, -2);
				const int c_nTile7x7RandCollapse		    = 6016;	//	AddEffect(1, "Game Tile 7x7 Randomly Collapse",				"Game\nGame Tile 7x7 Randomly Collapse",		6016, -2, -2);
				const int c_nTile10x10ConsCollapse		    = 6017;	//	AddEffect(1, "Game Tile 10x10 Consecutive Collapse",		"Game\nGame Tile 10x10 Consecutive Collapse",	6017, -2, -2);
				const int c_nTile10x10RandCollapse          = 6018;	//	AddEffect(1, "Game Tile 10x10 Randomly Collapse",			"Game\nGame Tile 10x10 Randomly Collapse",		6018, -2, -2);

				const int c_nSimpleCopyFirst				= 6101;	//	всегда берется первая картинка (вне зависимости от значения параметра анимации)
				const int c_nSimpleCopyLast					= 6102;	//	всегда берется вторая картинка (вне зависимости от значения параметра анимации)

				const int c_nSmallHexagons                  = 6201;	//	AddEffect(2, "Small Hexagons",							    "Small Hexagons",							    6201, -1, -1);
				const int c_nLargeHexagons                  = 6202;	//	AddEffect(2, "Large Hexagons",							    "Large Hexagons",							    6202, -1, -1);
				const int c_nSmallHexagonsCollapse          = 6203;	//	AddEffect(2, "Small Hexagons, Collapse",					"Small Hexagons, Collapse",						6203, -1, -1);
				const int c_nLargeHexagonsCollapse          = 6204;	//	AddEffect(2, "Large Hexagons, Collapse",					"Large Hexagons, Collapse",						6204, -1, -1);
				const int c_nSmallRhombuses                 = 6205;	//	AddEffect(2, "Small Rhombuses",							    "Small Rhombuses",							    6205, -1, -1);
				const int c_nLargeRhombuses                 = 6206;	//	AddEffect(2, "Large Rhombuses",							    "Large Rhombuses",							    6206, -1, -1);
				const int c_nSmallRhombusesCollapse         = 6207;	//	AddEffect(2, "Small Rhombuses, Collapse",					"Small Rhombuses, Collapse",					6207, -1, -1);
				const int c_nLargeRhombusesCollapse         = 6208;	//	AddEffect(2, "Large Rhombuses, Collapse",					"Large Rhombuses, Collapse",					6208, -1, -1);
				const int c_nSmallBricks                    = 6209;	//	AddEffect(2, "Small Bricks",							    "Small Bricks",							        6209, -1, -1);
				const int c_nLargeBricks                    = 6210;	//	AddEffect(2, "Large Bricks",							    "Large Bricks",							        6210, -1, -1);
				const int c_nSmallBricks2                   = 6211;	//	AddEffect(2, "Small Bricks 2",							    "Small Bricks 2",							    6211, -1, -1);
				const int c_nLargeBricks2                   = 6212;	//	AddEffect(2, "Large Bricks 2",							    "Large Bricks 2",							    6212, -1, -1);
				const int c_nSmallBricks3                   = 6213;	//	AAddEffect(2, "Small Bricks 3",							    "Small Bricks 3",							    6213, -1, -1);
				const int c_nLargeBricks3                   = 6214;	//	AAddEffect(2, "Large Bricks 3",							    "Large Bricks 3",							    6214, -1, -1);
				const int c_nSmallBricksCollapse            = 6215;	//	AddEffect(2, "Small Bricks, Collapse",						"Small Bricks, Collapse",						6215, -1, -1);
				const int c_nLargeBricksCollapse            = 6216;	//	AddEffect(2, "Large Bricks, Collapse",						"Large Bricks, Collapse",						6216, -1, -1);
				const int c_nSmallTriangles                 = 6217;	//  AddEffect(2, "Small Triangles",							    "Small Triangles",							    6217, -1, -1);
				const int c_nLargeTriangles                 = 6218; //	AddEffect(2, "Large Triangles",							    "Large Triangles",							    6218, -1, -1);
				const int c_nSmallTriangles2                = 6219;	//  AddEffect(2, "Small Triangles 2",							"Small Triangles 2",							6219, -1, -1);
				const int c_nLargeTriangles2                = 6220;	//  AddEffect(2, "Large Triangles 2",							"Large Triangles 2",							6220, -1, -1);
				const int c_nSmallTriangles3                = 6221;	//  AddEffect(2, "Small Triangles 3",							"Small Triangles 3",							6221, -1, -1);
				const int c_nLargeTriangles3                = 6222;	//  AddEffect(2, "Large Triangles 3",							"Large Triangles 3",							6222, -1, -1);
				const int c_nSmallTrianglesCollapse         = 6223;	//  AddEffect(2, "Small Triangles, Collapse",					"Small Triangles, Collapse",					6223, -1, -1);
				const int c_nLargeTrianglesCollapse         = 6224;	//  AddEffect(2, "Large Triangles, Collapse",					"Large Triangles, Collapse",					6224, -1, -1);
				const int c_nSmallInclinedBricks            = 6225;	//	AddEffect(2, "Small Inclined Bricks",						"Small Inclined Bricks",						6225, -1, -1);
				const int c_nLargeInclinedBricks            = 6226;	//	AddEffect(2, "Large Inclined Bricks",						"Large Inclined Bricks",						6226, -1, -1);
				const int c_nSmallInclinedBricks2           = 6227;	//	AddEffect(2, "Small Inclined Bricks 2",						"Small Inclined Bricks 2",						6227, -1, -1);
				const int c_nLargeInclinedBricks2           = 6228;	//	AddEffect(2, "Large Inclined Bricks 2",						"Large Inclined Bricks 2",						6228, -1, -1);
				const int c_nSmallInclinedBricksCollapse    = 6229;	//	AddEffect(2, "Small Inclined Bricks, Collapse",				"Small Inclined Bricks, Collapse",				6229, -1, -1);
				const int c_nLargeInclinedBricksCollapse    = 6230;	//	AddEffect(2, "Large Inclined Bricks, Collapse",				"Large Inclined Bricks, Collapse",				6230, -1, -1);
				

				// шаблоны для эффекта fade 
				const int c_nPatternSize = 8;

				// dost
				const BYTE c_byPatternDots[8][8] = 
				{
					{1,0,0,0,1,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{1,0,0,0,1,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0}
				};

				// dots dense
				const BYTE c_byPatternDotsDense[8][8] = 
				{
					{1,0,1,0,1,0,1,0},
					{0,0,0,0,0,0,0,0},
					{1,0,1,0,1,0,1,0},
					{0,0,0,0,0,0,0,0},
					{1,0,1,0,1,0,1,0},
					{0,0,0,0,0,0,0,0},
					{1,0,1,0,1,0,1,0},
					{0,0,0,0,0,0,0,0}
				};

				// grid
				const BYTE c_byPatternGrid[8][8] = 
				{
					{1,1,1,1,1,1,1,1},
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0},
					{1,1,1,1,1,1,1,1},
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0}
				};

				// grid rotated
				const BYTE c_byPatternGridRotated[8][8] = 
				{
					{1,0,0,0,0,0,0,1},
					{0,1,0,0,0,0,1,0},
					{0,0,1,0,0,1,0,0},
					{0,0,0,1,1,0,0,0},
					{0,0,0,1,1,0,0,0},
					{0,0,1,0,0,1,0,0},
					{0,1,0,0,0,0,1,0},
					{1,0,0,0,0,0,0,1}
				};

				// line vertical
				const BYTE c_byPatternLineVertical[8][8] = 
				{
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0},
					{1,0,0,0,1,0,0,0}
				};

				// line horizontal
				const BYTE c_byPatternLineHorizontal[8][8] = 
				{
					{1,1,1,1,1,1,1,1},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{1,1,1,1,1,1,1,1},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0}
				};

				// line diagonal
				const BYTE c_byPatternLineDiagonal[8][8] = 
				{
					{0,0,0,1,0,0,0,0},
					{0,0,1,0,0,0,0,0},
					{0,1,0,0,0,0,0,0},
					{1,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,1},
					{0,0,0,0,0,0,1,0},
					{0,0,0,0,0,1,0,0},
					{0,0,0,0,1,0,0,0}
				};

				// line diagonal rotated
				const BYTE c_byPatternLineDiagonalRotated[8][8] =
				{
					{0,0,0,0,1,0,0,0},
					{0,0,0,0,0,1,0,0},
					{0,0,0,0,0,0,1,0},
					{0,0,0,0,0,0,0,1},
					{1,0,0,0,0,0,0,0},
					{0,1,0,0,0,0,0,0},
					{0,0,1,0,0,0,0,0},
					{0,0,0,1,0,0,0,0}
				};

				// chess board
				const BYTE c_byPatternChessBoard[8][8] =
				{
					{1,1,1,1,0,0,0,0},
					{1,1,1,1,0,0,0,0},
					{1,1,1,1,0,0,0,0},
					{1,1,1,1,0,0,0,0},
					{0,0,0,0,1,1,1,1},
					{0,0,0,0,1,1,1,1},
					{0,0,0,0,1,1,1,1},
					{0,0,0,0,1,1,1,1}
				};

				// dashes
				const BYTE c_byPatternDashes[8][8] =
				{
					{1,0,0,0,0,0,0,0},
					{0,1,0,0,0,0,0,0},
					{0,0,1,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,1,0},
					{0,0,0,0,0,1,0,0},
					{0,0,0,0,1,0,0,0},
					{0,0,0,0,0,0,0,0}
				};

				// spiner
				const BYTE c_byPatternSpiner[8][8] =
				{
					{0,0,0,1,0,1,0,0},
					{0,0,0,0,1,1,0,0},
					{1,1,0,0,1,0,0,0},
					{0,1,1,1,1,0,0,1},
					{1,0,0,1,1,1,1,0},
					{0,0,0,1,0,0,1,1},
					{0,0,1,1,0,0,0,0},
					{0,0,1,0,1,0,0,0}
				};

				// heart
				const BYTE c_byPatternHeart[8][8] =
				{
					{0,0,0,0,0,0,0,0},
					{0,0,0,1,0,0,0,0},
					{0,0,1,1,1,0,0,0},
					{0,1,1,1,1,1,0,0},
					{1,1,1,1,1,1,1,0},
					{1,1,1,1,1,1,1,0},
					{0,1,1,0,1,1,0,0},
					{0,0,0,0,0,0,0,0}
				};

				// Cross
				const BYTE c_byPatternCross[8][8] =
				{
					{1,0,0,0,1,0,0,1},
					{0,1,0,1,0,0,1,0},
					{0,0,1,0,1,1,0,0},
					{0,1,0,1,0,1,0,1},
					{1,0,1,0,1,0,1,0},
					{0,0,1,1,0,1,0,0},
					{0,1,0,0,1,0,1,0},
					{1,0,0,1,0,0,0,1}
				};

				// thatches
				const BYTE c_byPatternThatches[8][8] =
				{
					{1,1,1,1,1,0,0,0},
					{0,1,1,1,0,1,0,0},
					{0,0,1,0,0,0,1,0},
					{0,1,0,0,0,1,1,1},
					{1,0,0,0,1,1,1,1},
					{0,0,0,1,0,1,1,1},
					{0,0,1,0,0,0,1,0},
					{0,1,1,1,0,0,0,1}
				};

				// empty
				const BYTE c_byPatternEmpty[8][8] =
				{
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0}
				};

				// шаблоны для эффекта мозайки

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

			}
		}
	}
}

#endif // #ifndef _IMAGEPAINTCOMPOSECONSTANTS_H_	// For Borland compiler