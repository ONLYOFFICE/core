// Patterns.h : header file

#pragma once

// effects types
const int c_nEffectsSimple = 0;
const int c_nEffectsAdvanced = 1;
const int c_nEffectsAll = 2;

// additional constants
const int c_nPatternNone = 0;
const int c_nPatternSolid = 1;
const int c_nPatternDots = 2;
const int c_nPatternDotsDense = 3;
const int c_nPatternGrid = 4;
const int c_nPatternGridRotated = 5;
const int c_nPatternLineVertical = 6;
const int c_nPatternLineHorizontal = 7;
const int c_nPatternLineDiagonal = 8;
const int c_nPatternLineDiagonalRotated = 9;
const int c_nPatternChess = 10;
const int c_nPatternDashes = 11;
const int c_nPatternSpiner = 12;
const int c_nPatternThatches = 13;
const int c_nPatternHeart = 17;
const int c_nPatternCross = 18;

const int c_nPatternDots2 = 32;
const int c_nPatternDotsDense2 = 33;
const int c_nPatternGrid2 = 34;
const int c_nPatternGridRotated2 = 35;
const int c_nPatternLineVertical2 = 36;
const int c_nPatternLineHorizontal2 = 37;
const int c_nPatternLineDiagonal2 = 38;
const int c_nPatternLineDiagonalRotated2 = 39;
const int c_nPatternChess2 = 40;
const int c_nPatternDashes2 = 41;
const int c_nPatternSpiner2 = 42;
const int c_nPatternThatches2 = 43;
const int c_nPatternHeart2 = 47;
const int c_nPatternCross2 = 48;

const int c_nPatternDots3 = 62;
const int c_nPatternDotsDense3 = 63;
const int c_nPatternGrid3 = 64;
const int c_nPatternGridRotated3 = 65;
const int c_nPatternLineVertical3 = 66;
const int c_nPatternLineHorizontal3 = 67;
const int c_nPatternLineDiagonal3 = 68;
const int c_nPatternLineDiagonalRotated3 = 69;
const int c_nPatternChess3 = 70;
const int c_nPatternDashes3 = 71;
const int c_nPatternSpiner3 = 72;
const int c_nPatternThatches3 = 73;
const int c_nPatternHeart3 = 77;
const int c_nPatternCross3 = 78;

const int c_nFadeZoomIn = 14;
const int c_nFadeZoomOut = 15;
const int c_nFadeZoomBoth = 16;

const int c_nMosaicSpiralIn	= 1;
const int c_nMosaicSpiralOut = 2;
const int c_nMosaicStrips = 3;
const int c_nMosaicDissolve = 4;
const int c_nMosaicClockwise = 5;
const int c_nMosaicCounterClockwise = 6;
const int c_nMosaicRandom = 7;
const int c_nMosaicLeftTopToRightBottom = 8;
const int c_nMosaicRightTopToLeftBottom = 9;
const int c_nMosaicRightBottomToLeftTop = 10;
const int c_nMosaicLeftBottomToRightTop	= 11;
const int c_nMosaicWallLeftToRight = 12;
const int c_nMosaicWallRightToLeft = 13;
const int c_nMosaicWallTopToBottom = 14;
const int c_nMosaicWallBottomToTop = 15;
const int c_nMosaicChessLeftToRight = 16;
const int c_nMosaicChessRightToLeft = 17;
const int c_nMosaicChessTopToBottom = 18;
const int c_nMosaicChessBottomToTop = 19;

const int c_nRotateSingleIn              = 1;
const int c_nRotateTwiceIn               = 2;
const int c_nRotateSingleOut             = 3;
const int c_nRotateTwiceOut              = 4;
const int c_nRotateHalfIn	             = 5;  
const int c_nRotateHalfOut	             = 6;  
const int c_nRotateTransparentSingleIn	 = 7;  
const int c_nRotateTransparentTwiceIn    = 8;  
const int c_nRotateTransparentSingleOut  = 9;  
const int c_nRotateTransparentTwiceOut	 = 10;  
const int c_nRotateTransparentHalfIn	 = 11;  
const int c_nRotateTransparentHalfOut	 = 12;  


const int c_nDoorVerticalIn = 3; //1;
const int c_nDoorVerticalOut = 4; //2;
const int c_nDoorHorizontalIn = 1; //3;
const int c_nDoorHorizontalOut = 2; //4;
const int c_nDoorDiagonal = 5;
const int c_nDoorDiagonalRotated = 6;

const int c_nStripsVertical = 1;
const int c_nStripsHorizontal = 2;

const int c_nSideLeftToRight = 1;
const int c_nSideRightToLeft = 2;
const int c_nSideTopToBottom = 3;
const int c_nSideBottomToTop = 4;
const int c_nSideLeftTopToRightBottom = 5;
const int c_nSideLeftBottomToRightTop = 6;
const int c_nSideRightBottomToLeftTop = 7;
const int c_nSideRightTopToLeftBottom = 8;

const int c_nCenterBoxIn = 1;
const int c_nCenterBoxOut = 2;
const int c_nCenterDiamondIn = 3;
const int c_nCenterDiamondOut = 4;
const int c_nCenterCircleIn = 5;
const int c_nCenterCircleOut = 6;
const int c_nCenterCrossIn = 7;
const int c_nCenterCrossOut = 8;

const int c_nStretchLeftToRightIn = 1;
const int c_nStretchLeftToRightOut = 2;
const int c_nStretchRightToLeftIn = 3;
const int c_nStretchRightToLeftOut = 4;
const int c_nStretchBottomToTopIn = 5;
const int c_nStretchBottomToTopOut = 6;
const int c_nStretchTopToBottomIn = 7;
const int c_nStretchTopToBottomOut = 8;

//const int c_nCircleCornerLeftTop = 1;
//const int c_nCircleCornerLeftBottom = 2;
//const int c_nCircleCornerRightTop = 3;
//const int c_nCircleCornerRightBottom = 4;
const int c_nCircleCornerLeftTopClockwise = 5;
const int c_nCircleCornerLeftBottomClockwise = 6;
const int c_nCircleCornerRightTopClockwise = 7;
const int c_nCircleCornerRightBottomClockwise = 8;

const int c_nBottomToTopClockwise = 1;
const int c_nBottomToTopCounterClockwise = 2;
const int c_nBottomToTopOpposite = 3;
const int c_nTopToBottomClockwise = 4;
const int c_nTopToBottomCounterClockwise = 5;
const int c_nTopToBottomOpposite = 6;
const int c_nLeftToRightClockwise = 7;
const int c_nLeftToRightCounterClockwise = 8;
const int c_nLeftToRightOpposite = 9;
const int c_nRightToLeftClockwise = 10;
const int c_nRightToLeftCounterClockwise = 11;
const int c_nRightToLeftOpposite = 12;
const int c_n2AngleClockwise = 13;
const int c_n2AngleCounterClockwise = 14;
const int c_n3AngleClockwise = 15;
const int c_n3AngleCounterClockwise = 16;
const int c_n4AngleClockwise = 17;
const int c_n4AngleCounterClockwise = 18;
const int c_n6AngleClockwise = 19;
const int c_n6AngleCounterClockwise = 20;
const int c_n8AngleClockwise = 21;
const int c_n8AngleCounterClockwise = 22;
const int c_n12AngleClockwise = 23;
const int c_n12AngleCounterClockwise = 24;

const int c_nSwingLeftIn = 1;
const int c_nSwingLeftOut = 2;
const int c_nSwingRightIn = 3;
const int c_nSwingRightOut = 4;
const int c_nSwingBottomIn = 7; //5;
const int c_nSwingBottomOut = 8; //6;
const int c_nSwingTopIn = 5; //7;
const int c_nSwingTopOut = 6; //8;

const int c_nShatterHigh = 1;
const int c_nShatterLow = 2;

const int c_nFlipVerticalIn = 5; //1;
const int c_nFlipVerticalOut = 6; //2;
const int c_nFlipVerticalInBackward = 7; //3;
const int c_nFlipVerticalOutBackward = 8; //4;
const int c_nFlipHorizontalIn = 1; //5;
const int c_nFlipHorizontalOut = 2; //6;
const int c_nFlipHorizontalInBackward = 3; //7;
const int c_nFlipHorizontalOutBackward = 4; //8;
	
const int c_nTwirlClockwise               = 1;
const int c_nTwirlClockwiseZoomOut        = 2;
const int c_nTwirlClockwiseZoomIn         = 3;
const int c_nTwirlCounterClockwise        = 4;
const int c_nTwirlCounterClockwiseZoomOut = 5;
const int c_nTwirlCounterClockwiseZoomIn  = 6;
const int c_nTwirlDrain					  = 7;
const int c_nTwirlReverseDrain			  = 8;

const int c_nSpiralTopLeft                = 1; 
const int c_nSpiralTopRight               = 2; 
const int c_nSpiralBottomLeft             = 3; 
const int c_nSpiralBottomRight            = 4; 
const int c_nSpiralCenterIn               = 5; 
const int c_nSpiralReverseCenterIn        = 6; 
const int c_nSpiralCenterOut              = 7; 
const int c_nSpiralReverseCenterOut       = 8; 


const int c_nTwirlStepCount = 5;
const double c_dTwirlStepBeginFrame[5] = {0.0,0.3,0.5,0.7,1.0};

const int c_nSphereIn = 1;
const int c_nSphereOut = 1;

const int c_nSphereStepCount = 5;
const double c_dSphereStepBeginFrame[5] = {0.0,0.3,0.5,0.7,1.0};

const int c_nTwistLeftToRight = 1;
const int c_nTwistRightToLeft = 2;
const int c_nTwistTopToBottom = 3;
const int c_nTwistBottomToTop = 4;
const int c_nTwistLeftTopToRightBottom = 5;
const int c_nTwistLeftBottomToRightTop = 6;
const int c_nTwistRightTopToLeftBottom = 7;
const int c_nTwistRightBottomToLeftTop = 8;

const int c_nRippleOut = 1;
const int c_nRippleIn = 2;
const int c_nRippleStepCount = 4;
const double c_dRippleStepBeginFrame[5] = {0.0,0.2,0.8,1.0};

const int c_nWaveHorizontal = 1;
const int c_nWaveVertical = 2;

const int c_nWaveStepCount = 4;
const double c_dWaveStepBeginFrame[5] = {0.0,0.2,0.8,1.0};

const int c_nBurning = 1;
const int c_nBurningFade = 2;
const int c_nBurningFire = 3;

const int c_nGaussianBlurStepCount = 5;
const double c_dGaussianBlurStepBeginFrame[5] = {0.0,0.3,0.5,0.7,1.0};

const int c_nNoiseColored = 1;
const int c_nNoiseGrayscale = 2;
const int c_nFlashLight = 3;
const int c_nFlashDark = 4;
const int c_nNoiseStepCount = 5;
const double c_dNoiseStepBeginFrame[5] = {0.0,0.4,0.5,0.6,1.0};


const int c_nTetris							= 1;
const int c_nTetrisCollapse					= 2;

const int c_nShotChangeGHorizontalRToL		= 1; 
const int c_nShotChangeGHorizontalLToR		= 2; 
const int c_nShotChangeGVerticalUToB		= 3;
const int c_nShotChangeGVerticalBToU		= 4;
const int c_nShotChangeBHorizontalRToL		= 5;
const int c_nShotChangeBHorizontalLToR		= 6;
const int c_nShotChangeBVerticalUToB		= 7;
const int c_nShotChangeBVerticalBToU		= 8;
const int c_nShotChangeGHorizontalRToLNoZ   = 9;
const int c_nShotChangeGHorizontalLToRNoZ   = 10;
const int c_nShotChangeGVerticalUToBNoZ     = 11;
const int c_nShotChangeGVerticalBToUNoZ     = 12;
const int c_nShotChangeBHorizontalRToLNoZ   = 13;
const int c_nShotChangeBHorizontalLToRNoZ   = 14;
const int c_nShotChangeBVerticalUToBNoZ     = 15;
const int c_nShotChangeBVerticalBToUNoZ     = 16;

const int c_nBlackWhiteLines                = 1;
const int c_nBlackWhiteLinesInverse         = 2;

//mask
const int c_nBowTileHorizontal              = 1;
const int c_nBowTileVertical                = 2;
const int c_nDiagonalBoxOut                 = 3;
const int c_nDiagonalCrossOut               = 4;
const int c_nDiagonalDownRight              = 5;
const int c_nDiagonalDownLeft               = 6;
const int c_nDiagonalUpRight                = 7;
const int c_nDiagonalUpLeft                 = 8;
const int c_nFunIn                          = 9;
const int c_nFunOut                         = 10;
const int c_nFunUp                          = 11;
const int c_nFilledVDown                    = 12;
const int c_nFilledVLeft                    = 13;
const int c_nFilledVRight                   = 14;
const int c_nFilledVUp                      = 15;
const int c_nInsertDownLeft                 = 16;
const int c_nInsertDownRight                = 17;
const int c_nInsertUpLeft                   = 18;
const int c_nInsertUpRight                  = 19;
const int c_nRevealDown                     = 20;
const int c_nRevealRight                    = 21;
const int c_nRevealLeft                     = 22;
const int c_nRevealUp                       = 23;
const int c_nSplitHorizontal                = 24;
const int c_nSplitVertical                  = 25;
const int c_nSweepIn                        = 26;
const int c_nSweepOut                       = 27;
const int c_nSweepUp                        = 28;
const int c_nZigZagHorizontal               = 29;
const int c_nZigZagVertical                 = 30;
const int c_nBarsHorizontal                 = 31;
const int c_nBarsVertical                   = 32;
const int c_nBowTileSmoothHorizontal        = 33; 
const int c_nBowTileSmoothVertical          = 34; 
const int c_nDiagonalSmoothBoxOut           = 35; 
const int c_nDiagonalSmoothCrossOut         = 36; 
const int c_nDiagonalSmoothDownRight        = 37; 
const int c_nDiagonalSmoothDownLeft         = 38; 
const int c_nDiagonalSmoothUpRight          = 39; 
const int c_nDiagonalSmoothUpLeft           = 40; 
const int c_nFunSmoothIn                    = 41; 
const int c_nFunSmoothOut                   = 42; 
const int c_nFunSmoothUp                    = 43; 
const int c_nFilledVSmoothDown              = 44; 
const int c_nFilledVSmoothLeft              = 45; 
const int c_nFilledVSmoothRight             = 46; 
const int c_nFilledVSmoothUp                = 47; 
const int c_nInsertSmoothDownLeft           = 48; 
const int c_nInsertSmoothDownRight          = 49; 
const int c_nInsertSmoothUpLeft             = 50; 
const int c_nInsertSmoothUpRight            = 51; 
const int c_nSplitSmoothHorizontal          = 52; 
const int c_nSplitSmoothVertical            = 53; 
const int c_nSweepSmoothIn                  = 54; 
const int c_nSweepSmoothOut                 = 55; 
const int c_nSweepSmoothUp                  = 56; 
const int c_nZigZagSmoothHorizontal         = 57; 
const int c_nZigZagSmoothVertical           = 58; 
const int c_nWipeSmoothCircle               = 59; 
const int c_nWipeSmoothCircles              = 60; 
const int c_nWipeSmoothDiamond              = 61; 
const int c_nWipeSmoothDiamonds             = 62; 
const int c_nWipeSmoothWideStar				= 63; 
const int c_nWipeSmoothWideStars            = 64; 
const int c_nWipeSmoothWideFlower           = 65; 
const int c_nWipeSmoothWideFlowers          = 66; 
const int c_nWipeSmoothWideKeyHole          = 67; 
const int c_nWipeSmoothWideKeyHoles         = 68; 
const int c_nWipeSmoothNarrowStar           = 69; 
const int c_nWipeSmoothNarrowStars          = 70; 
const int c_nWipeSmoothNarrowKeyHole        = 71; 
const int c_nWipeSmoothNarrowKeyHoles       = 72; 
const int c_nWipeSmoothNarrowFlower         = 73; 
const int c_nWipeSmoothNarrowFlowers        = 74; 
const int c_nCircleCornerLeftTop            = 81;
const int c_nCircleCornerLeftBottom         = 82;
const int c_nCircleCornerRightTop           = 83;
const int c_nCircleCornerRightBottom        = 84;
const int c_nCircleCornerSmoothLeftTop      = 85;
const int c_nCircleCornerSmoothLeftBottom   = 86;
const int c_nCircleCornerSmoothRightTop     = 87;
const int c_nCircleCornerSmoothRightBottom  = 88;
const int c_nWipeSmoothCircleIn             = 89;
const int c_nWipeCircles                    = 90;
const int c_nWipeSmoothBoxIn                = 91;
const int c_nWipeSmoothBoxOut               = 92;
const int c_nWipeSmoothDiamondIn            = 93;
const int c_nWipeDiamonds                   = 94;

const int c_nPixelate                       = 1;
const int c_nRollRight                      = 2;
const int c_nRollLeft                       = 3;
const int c_nSlideUp                        = 4;
const int c_nSlideDown                      = 5;
const int c_nSlideRight                     = 6;
const int c_nSlideLeft                      = 7;
const int c_nSlideCenterUp                  = 8;
const int c_nSlideCenterDown                = 9;
const int c_nSlideCenterRight               = 10;
const int c_nSlideCenterLeft                = 11;
const int c_nShrinkInVertical               = 12;
const int c_nShrinkInHorizontal             = 13;
const int c_nEye                            = 14;
const int c_nHeart                          = 15;
const int c_nWave							= 16;
const int c_nSmoothEye                      = 17;
const int c_nSmoothHeart                    = 18;
const int c_nSmoothWave                     = 19;

//shatter
const int c_nWhirlWind                      = 5;

const int c_nPageTurnLeftTop				= 5;
const int c_nPageTurnLeftBottom				= 6;
const int c_nPageTurnRightBottom			= 7;
const int c_nPageTurnRightTop				= 8;
const int c_nPageTurnLeftTop3d				= 9;
const int c_nPageTurnLeftBottom3d			= 10;
const int c_nPageTurnRightBottom3d			= 11;
const int c_nPageTurnRightTop3d				= 12;

const int c_nPageTurnLeftTopWhiteBack3d     = 20;
const int c_nPageTurnLeftBottomWhiteBack3d  = 21;
const int c_nPageTurnRightBottomWhiteBack3d = 22;
const int c_nPageTurnRightTopWhiteBack3d    = 23;

const int c_nPageTurnAlbumLeftTop           = 24; 
const int c_nPageTurnAlbumRightTop          = 25; 
const int c_nPageTurnAlbumRightBottom       = 26; 
const int c_nPageTurnAlbumLeftBottom        = 27; 

const int c_nPageTurnAlbumLeftTopCorners1    = 28; 
const int c_nPageTurnAlbumRightTopCorners1   = 29; 
const int c_nPageTurnAlbumRightBottomCorners1= 30; 
const int c_nPageTurnAlbumLeftBottomCorners1 = 31; 

const int c_nPageTurnAlbumLeftTopCorners2    = 32; 
const int c_nPageTurnAlbumRightTopCorners2   = 33; 
const int c_nPageTurnAlbumRightBottomCorners2= 34; 
const int c_nPageTurnAlbumLeftBottomCorners2 = 35; 

const int c_nPageTurnAlbumLeftTopCorners3    = 36; 
const int c_nPageTurnAlbumRightTopCorners3   = 37; 
const int c_nPageTurnAlbumRightBottomCorners3= 38; 
const int c_nPageTurnAlbumLeftBottomCorners3 = 39; 

const int c_nPageTurnAlbumLeftTopCorners4    = 40; 
const int c_nPageTurnAlbumRightTopCorners4   = 41; 
const int c_nPageTurnAlbumRightBottomCorners4= 42; 
const int c_nPageTurnAlbumLeftBottomCorners4 = 43; 


const int c_nPinwheelCurvedSN5A    		    =  1;	
const int c_nPinwheelCurvedSN7A		        =  2;	
const int c_nPinwheelCurvedSN12A		    =  3;	
const int c_nPinwheelCurvedSW5A		        =  4;	
const int c_nPinwheelCurvedSW7A		        =  5;	
const int c_nPinwheelCurvedSW12A		    =  6;	
const int c_nPinwheelCurvedLN5A		        =  7;	
const int c_nPinwheelCurvedLN7A		        =  8;	
const int c_nPinwheelCurvedLN12A		    =  9;	
const int c_nPinwheelPropeller4		        = 10;	
const int c_nPinwheelPropeller7		        = 11;	
const int c_nPinwheelPropeller12		    = 12;	

const int c_nPinwheelCurvedSN5ASmooth		= 13;	
const int c_nPinwheelCurvedSN7ASmooth		= 14;	
const int c_nPinwheelCurvedSN12ASmooth		= 15;	
const int c_nPinwheelCurvedSW5ASmooth   	= 16;	
const int c_nPinwheelCurvedSW7ASmooth		= 17;	
const int c_nPinwheelCurvedSW12ASmooth		= 18;	
const int c_nPinwheelCurvedLN5ASmooth		= 19;	
const int c_nPinwheelCurvedLN7ASmooth		= 20;	
const int c_nPinwheelCurvedLN12ASmooth		= 21;	
const int c_nPinwheelPropeller4Smooth		= 22;	
const int c_nPinwheelPropeller7Smooth		= 23;	
const int c_nPinwheelPropeller12Smooth		= 24;	

const int c_nPuzzle4x4		                = 1;
const int c_nPuzzle7x7		                = 2;
const int c_nPuzzle10x10		            = 3;
const int c_nPuzzle4x4Collapse		        = 4;
const int c_nPuzzle7x7Collapse		        = 5;
const int c_nPuzzle10x10Collapse		    = 6;
const int c_nTile4x4Cons		            = 7;
const int c_nTile4x4Rand	                = 8;
const int c_nTile7x7Cons		            = 9;
const int c_nTile7x7Rand		            = 10;
const int c_nTile10x10Cons		            = 11;
const int c_nTile10x10Rand		            = 12;
const int c_nTile4x4ConsCollapse		    = 13;
const int c_nTile4x4RandCollapse		    = 14;
const int c_nTile7x7ConsCollapse		    = 15;
const int c_nTile7x7RandCollapse		    = 16;
const int c_nTile10x10ConsCollapse		    = 17;
const int c_nTile10x10RandCollapse          = 18;


const int c_nSmallHexagons                  = 1;
const int c_nLargeHexagons                  = 2;
const int c_nSmallHexagonsCollapse          = 3;
const int c_nLargeHexagonsCollapse          = 4;
const int c_nSmallRhombuses                 = 5;
const int c_nLargeRhombuses                 = 6;
const int c_nSmallRhombusesCollapse         = 7;
const int c_nLargeRhombusesCollapse         = 8;
const int c_nSmallBricks                    = 9;
const int c_nLargeBricks                    = 10;
const int c_nSmallBricks2                   = 11;
const int c_nLargeBricks2                   = 12;
const int c_nSmallBricks3                   = 13;
const int c_nLargeBricks3                   = 14;
const int c_nSmallBricksCollapse            = 15;
const int c_nLargeBricksCollapse            = 16;
const int c_nSmallTriangles                 = 17;
const int c_nLargeTriangles                 = 18;
const int c_nSmallTriangles2                = 19;
const int c_nLargeTriangles2                = 20;
const int c_nSmallTriangles3                = 21;
const int c_nLargeTriangles3                = 22;
const int c_nSmallTrianglesCollapse         = 23;
const int c_nLargeTrianglesCollapse         = 24;
const int c_nSmallInclinedBricks            = 25;
const int c_nLargeInclinedBricks            = 26;
const int c_nSmallInclinedBricks2           = 27;
const int c_nLargeInclinedBricks2           = 28;
const int c_nSmallInclinedBricksCollapse    = 29;
const int c_nLargeInclinedBricksCollapse    = 30;

const int c_nGraduateSideLeftToRight		= 1;
const int c_nGraduateSideRightToLeft		= 2;
const int c_nGraduateSideTopToBottom		= 3;
const int c_nGraduateSideBottomToTop		= 4;
const int c_nGraduateCornerLeftTop 			= 5;
const int c_nGraduateCornerLeftBottom		= 6;
const int c_nGraduateCornerRightBottom		= 7;
const int c_nGraduateCornerRightTop 		= 8;

const int c_nGraduateClockBottomClockwise	= 1;
const int c_nGraduateClockBottomReverse		= 2;
const int c_nGraduateClockBottomOpposite	= 3;
const int c_nGraduateClockTopClockwise		= 4;
const int c_nGraduateClockTopReverse 		= 5;
const int c_nGraduateClockTopOpposite 		= 6;
const int c_nGraduateClockLeftClockwise		= 7;
const int c_nGraduateClockLeftReverse 		= 8;
const int c_nGraduateClockLeftOpposite 		= 9;
const int c_nGraduateClockRightClockwise	= 10;
const int c_nGraduateClockRightReverse		= 11;
const int c_nGraduateClockRightOpposite		= 12;
const int c_nGraduateClock2AnglesClockwise 	= 13;
const int c_nGraduateClock2AnglesReverse	= 14;
const int c_nGraduateClock3AnglesClockwise 	= 15;
const int c_nGraduateClock3AnglesReverse	= 16;
const int c_nGraduateSideLeftToRightSmooth		 = 17;
const int c_nGraduateSideRightToLeftSmooth		 = 18;
const int c_nGraduateSideTopToBottomSmooth		 = 19;
const int c_nGraduateSideBottomToTopSmooth		 = 20;
const int c_nGraduateCornerLeftTopSmooth		 = 21;
const int c_nGraduateCornerLeftBottomSmooth		 = 22;
const int c_nGraduateCornerRightBottomSmooth	 = 23;
const int c_nGraduateCornerRightTopSmooth 		 = 24;
const int c_nGraduateClockBottomClockwiseSmooth	 = 25;
const int c_nGraduateClockBottomReverseSmooth	 = 26;
const int c_nGraduateClockBottomOppositeSmooth	 = 27;
const int c_nGraduateClockTopClockwiseSmooth	 = 28;
const int c_nGraduateClockTopReverseSmooth 		 = 29;
const int c_nGraduateClockTopOppositeSmooth		 = 30;
const int c_nGraduateClockLeftClockwiseSmooth	 = 31;
const int c_nGraduateClockLeftReverseSmooth 	 = 32;
const int c_nGraduateClockLeftOppositeSmooth 	 = 33;
const int c_nGraduateClockRightClockwiseSmooth	 = 34;
const int c_nGraduateClockRightReverseSmooth	 = 35;
const int c_nGraduateClockRightOppositeSmooth	 = 36;
const int c_nGraduateClock2AnglesClockwiseSmooth = 37;
const int c_nGraduateClock2AnglesReverseSmooth	 = 38;
const int c_nGraduateClock3AnglesClockwiseSmooth = 39;
const int c_nGraduateClock3AnglesReverseSmooth	 = 40;


// dots
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

/*
const BYTE c_byMosaicDissolve[8][8] = 
{
	{  ,  ,  ,  ,  ,  ,  ,  },
	{  ,  ,  ,  ,  ,  ,  ,  },
	{  ,  ,  ,  ,  ,  ,  ,  },
	{  ,  ,  ,  ,  ,  ,  ,  },
	{  ,  ,  ,  ,  ,  ,  ,  },
	{  ,  ,  ,  ,  ,  ,  ,  },
	{  ,  ,  ,  ,  ,  ,  ,  },
	{  ,  ,  ,  ,  ,  ,  ,  }
};
*/

class Category
{
public:

	CString Name;
	CSimpleArray<int> Effects;

	Category()
		{ }
	Category(const CString& _name)
		{ Name = _name; }
	Category& operator= (const Category& x)
	{ 
		Name = x.Name; 
		
		Effects.RemoveAll(); 

		for (int index = 0; index < x.Effects.GetSize(); ++index)
			Effects.Add(x.Effects[index]);
		
		return *this;
	}
};

class Effect
{
public:

	int ID;
	CString Name;
	CString Name2;
	int TileX;
	int TileY;
	int Type;

	Effect()
		{ Name = Name2 = ""; ID = -1; TileX = TileY = -1; Type = c_nEffectsAll; }
	Effect(const CString& _name, int _id)
		{ Name = _name; ID = _id; Type = c_nEffectsAll; }
	Effect(const CString& _name, const CString& _name2, int _id)
		{ Name = _name; Name2 = _name2; ID = _id; Type = c_nEffectsAll; }
	Effect& operator= (const Effect& x)
		{ Name = x.Name; Name2 = x.Name2; ID = x.ID; TileX = x.TileX; TileY = x.TileY; Type = x.Type; return *this; }
};

class Graduate
{
	int Size;
	int* Graduation; 

public:

	Graduate()
		{ Size = 0; Graduation = 0; }
	~Graduate()
		{ Clear(); }
	BOOL Clear()
	{
		if (Graduation)
			delete[] Graduation;

		Graduation = 0;
		Size = 0;

		return TRUE;
	}
	BOOL Create(int _size)
	{
		if (_size < 1)
			return Clear();
		if (_size == Size)
			return TRUE;

		Size = _size;
		Graduation = new int[Size];

		if (!Graduation)
		{
			Clear();
			return FALSE;
		}

		for (int index = 0; index < Size; ++index)
			Graduation[index] = index;

		//srand(GetTickCount());
		srand( 1 );

		for (int index = 0; index < Size; ++index)
		{
			int swap_index = rand() % Size;

			int swap_value = Graduation[swap_index];
			Graduation[swap_index] = Graduation[index];
			Graduation[index] = swap_value;
		}

		return TRUE;
	}
	double GetLevel(int _index, double Frame)
	{
		if (_index < 0 || _index >= Size)
			return 0;
		if (Frame <= 0)
			return 0;
		if (Frame >= 1)
			return 1;

		double level = Size*Frame;

		if (level <= Graduation[_index])
			return 0;
		if (level >= Graduation[_index] + 1)
			return 1;
		return level - Graduation[_index];
	}
};

			
class CTetrisFigure
{
public:
	CTetrisFigure()
	{
		m_nFigHeight = 0;
		m_nFigWidth = 0;
		m_nPicHeight = 0;
		m_nPicWidth = 0;
		m_nType = 0;
		m_pBGRASource = NULL;
		m_nXCoorSource = 0;
		m_nYCoorSource = 0;
	}
	CTetrisFigure(int nFigWidth, int nFigHeight, int nType, BYTE* pBGRASource, int nPicWidth, int nPicHeight, int nXCoorSource, int nYCoorSource)
	{
		m_nFigHeight = nFigHeight;
		m_nFigWidth = nFigWidth;
		m_nType = nType;
		m_nPicHeight = nPicHeight;
		m_nPicWidth = nPicWidth;
		m_pBGRASource = pBGRASource;
		m_nXCoorSource = nXCoorSource;
		m_nYCoorSource = nYCoorSource;
	}
	~CTetrisFigure()
	{
	}
	
private:
					
	void DrawAlphaBlendingLine(BYTE* pBGRAResult, BYTE* pBGRASource, int nColor, int nNumLine, double dEffect)
	{
		BYTE *pResult = pBGRAResult;
		BYTE *pSource = pBGRASource;
		int nAlpha = 7;
		for(int k = 0; k < nNumLine; k++)
		{
			nAlpha *= 2;
		}
		if(1 == dEffect)
		{
			*pResult = (nColor*nAlpha + *pSource *(255-nAlpha))/255; pResult++; pSource ++;
			*pResult = (nColor*nAlpha + *pSource *(255-nAlpha))/255; pResult++; pSource ++;
			*pResult = (nColor*nAlpha + *pSource *(255-nAlpha))/255; pResult++; pSource ++;
		}
		else
		{
			*pResult = dEffect*(nColor*nAlpha + *pSource *(255-nAlpha))/255 + *pSource*(1-dEffect); pResult++; pSource ++;
			*pResult = dEffect*(nColor*nAlpha + *pSource *(255-nAlpha))/255 + *pSource*(1-dEffect); pResult++; pSource ++;
			*pResult = dEffect*(nColor*nAlpha + *pSource *(255-nAlpha))/255 + *pSource*(1-dEffect); pResult++; pSource ++;
		}
		pResult++; pSource++;
	}

public:

	void SetValue(int nFigWidth, int nFigHeight, int nType, BYTE* pBGRASource, int nPicWidth, int nPicHeight, int nXCoorSource, int nYCoorSource)
	{
		m_nFigHeight = nFigHeight;
		m_nFigWidth = nFigWidth;
		m_nType = nType;
		m_nPicHeight = nPicHeight;
		m_nPicWidth = nPicWidth;
		m_pBGRASource = pBGRASource;
		m_nXCoorSource = nXCoorSource;
		m_nYCoorSource = nYCoorSource;
	}

	BOOL Cpy(BYTE* pBGRAResult, int nXCoorResult, int nYCoorResult, int nWidth, int nHeight, double dEffect)
	{
		if(!pBGRAResult || nWidth != m_nPicWidth || nHeight != m_nPicHeight)
		return FALSE;
			
		BYTE* pSource = m_pBGRASource;
		BYTE* pResultBuffer = new BYTE[ 4 * nWidth * ( 2 * nHeight)];
		BYTE* pResultBufferCurent = pResultBuffer;
		pResultBufferCurent += 4 * nWidth * nHeight;
		BYTE* pResultBufferMiddle = pResultBufferCurent;
		BYTE* pResult = pBGRAResult;
				
		IppiSize roiSize = { nWidth, nHeight};
				
		ippiCopy_8u_C4R( pResult, 4 * nWidth, pResultBufferMiddle, 4 * nWidth, roiSize);
					
		pSource             += m_nYCoorSource * nWidth * 4;
		pResultBufferCurent += nYCoorResult * nWidth * 4;
		
		if (1 == m_nType)
		{
			for(int nYIndex = nYCoorResult; nYIndex < min(nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
			{
				pSource             += m_nXCoorSource*4;
				pResultBufferCurent += nXCoorResult*4;
				int nXIndex = nXCoorResult;
				for(; nXIndex < min(nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
				{	
					if( nYCoorResult + 5 >= nYIndex &&
					  ( nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - nYCoorResult)) && 
					    0 != dEffect)
					{
						DrawAlphaBlendingLine(pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
					}
					else if( min( nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
					   ( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex)) &&
					     0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
					}
					else if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
						   ( nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex <= min(nWidth, nXCoorResult + m_nFigWidth) - (min(nHeight, nYCoorResult + m_nFigHeight) - nYIndex)) &&
						     0 != dEffect) 
					{
						DrawAlphaBlendingLine(pResultBufferCurent, pSource, 255, nYIndex - (min(nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
					}
					else if( nXCoorResult + 5 >= nXIndex &&
						   ( nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - nXCoorResult)) &&
						    0 != dEffect)
					{
						DrawAlphaBlendingLine(pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy(pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4; 
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4; 
					pSource             += 4;
				}
			
			}
		}
		else if(2 == m_nType)
		{
			int nYIndex = nYCoorResult;
					
			for(; nYIndex < min(nHeight, nYCoorResult + m_nFigHeight/2); nYIndex++)
			{
				pSource             += m_nXCoorSource * 4;
				pResultBufferCurent += nXCoorResult   * 4;
				int nXIndex = nXCoorResult;

				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
				{	
					if( nYCoorResult + 5 >= nYIndex &&
					  ( nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - nYCoorResult)) &&
					    0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
					}
					else if( min(nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
						   ( nYIndex >= nYCoorResult + ( min(nWidth, nXCoorResult + m_nFigWidth) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 2) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex)) &&
						     0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
					}
					else if( min( nHeight, nYCoorResult + m_nFigHeight / 2) - 5 <= nYIndex &&
						   ( ( nXIndex <= nXCoorResult +     m_nFigWidth / 3 - ( min( nHeight, nYCoorResult + m_nFigHeight / 2) - nYIndex) && nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight / 2) - nYIndex)) ||
						     ( nXIndex >= nXCoorResult + 2 * m_nFigWidth / 3 + ( min( nHeight, nYCoorResult + m_nFigHeight / 2) - nYIndex) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( min( nHeight, nYCoorResult + m_nFigHeight / 2) - nYIndex))) &&
						     0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - (min(nHeight, nYCoorResult + m_nFigHeight/2) - 5), dEffect);
					}
					else if( nXCoorResult + 5 >= nXIndex &&
						   ( nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 2) - ( nXIndex - nXCoorResult)) &&
						     0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy(pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4; 
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4; 
					pSource             += 4;
				}
				
			}
			for(; nYIndex < min(nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
			{
				pSource             += 4 * ( m_nXCoorSource + m_nFigWidth / 3);
				pResultBufferCurent += 4 * ( nXCoorResult   + m_nFigWidth / 3);
				int nXIndex          =       nXCoorResult   + m_nFigWidth / 3;

				for(; nXIndex < min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3); nXIndex++)
				{	
					if
						( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - 5 <= nXIndex 
						&& ( nYIndex >= nYCoorResult + m_nFigHeight / 2 + ( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - nXIndex) 
						&&   nYIndex <= min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - ( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - nXIndex)) 
						&& 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - (min(nWidth, nXCoorResult + 2*m_nFigWidth/3)- 5), dEffect);
					}
					else if
						( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex 
						&& ( nXIndex >= nXCoorResult + m_nFigWidth / 3 + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex)
						&&   nXIndex <= min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex)) 
						&& 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - (min(nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
					}
					else if
						( nXCoorResult + m_nFigWidth / 3 + 5 >= nXIndex 
						&& ( nYIndex >= nYCoorResult + m_nFigHeight / 2 + ( nXIndex - ( nXCoorResult + m_nFigWidth / 3)) 
						&&   nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - ( nXCoorResult + m_nFigWidth / 3)))
						&& 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + m_nFigWidth/3 + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4; 
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4;
					pSource             += 4;
				}
						
			}
		}
		else if(3 == m_nType)
		{
			int nYIndex = nYCoorResult;
			for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight / 3); nYIndex++)
			{
				pSource             += m_nXCoorSource * 4;
				pResultBufferCurent += nXCoorResult   * 4;
				int nXIndex = nXCoorResult;

				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth / 2); nXIndex++)
				{	
					if	( nYCoorResult + 5 >= nYIndex 
						&& ( nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth / 2) - ( nYIndex - nYCoorResult))
						&&   0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
					}
					else if
						( min(nWidth, nXCoorResult + m_nFigWidth / 2) - 5 <= nXIndex &&
						( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - nXIndex))
						&& 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - 5), dEffect);
					}
					else if
						( nXCoorResult + 5 >= nXIndex &&
						( nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 3) - ( nXIndex - nXCoorResult))
						&& 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4; 
					pSource             += 4;
				}
				
			}
			for(; nYIndex < min(nHeight, nYCoorResult + 2 * m_nFigHeight / 3); nYIndex++)
			{
				pSource             += 4 * m_nXCoorSource;
				pResultBufferCurent += 4 * nXCoorResult;
				int nXIndex = nXCoorResult;
				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
				{	
					if ( nYCoorResult + m_nFigHeight / 3 + 5 >= nYIndex &&
					   ( nXIndex <= nXCoorResult + m_nFigWidth  - ( nYIndex - nYCoorResult - m_nFigHeight / 3) && nXIndex >= nXCoorResult + m_nFigWidth / 2 + ( nYIndex - nYCoorResult - m_nFigHeight / 3))
					   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + m_nFigHeight / 3 + 5 - nYIndex, dEffect);
					}
					else if
						( min( nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
						( nYIndex >= nYCoorResult + m_nFigHeight / 3 + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex) &&  nYIndex <= min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex))
						&& 0 != dEffect)
					{
						DrawAlphaBlendingLine(pResultBufferCurent, pSource, 0, nXIndex - (min(nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
					}
					else if
						( ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - 5 <= nYIndex) &&
						( nXIndex <= nXCoorResult + m_nFigWidth - ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - nYIndex) && nXIndex >= nXCoorResult + m_nFigWidth / 2 + ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - nYIndex)) 
						&& 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - (min(nHeight, nYCoorResult + 2*m_nFigHeight/3) - 5), dEffect);
					}
					else if( nXCoorResult + 5 >= nXIndex && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4; 
					pSource             += 4;
				}
				
			}
			for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
			{
				pSource             += 4 * m_nXCoorSource;
				pResultBufferCurent += 4 * nXCoorResult;
				int nXIndex = nXCoorResult;

				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth / 2); nXIndex++)
				{	
					if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
					  ( nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex <= nXCoorResult + m_nFigWidth / 2 - ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex))
						&& 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - (min(nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
					}
					else if( min(nWidth, nXCoorResult + m_nFigWidth / 2) - 5 <= nXIndex &&
						   ( nYIndex >= nYCoorResult + 2 * m_nFigHeight / 3 + ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - nXIndex) && nYIndex <= nYCoorResult + m_nFigHeight - ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - nXIndex))
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth/2) - 5), dEffect);
					}
					else if( nXCoorResult + 5 >= nXIndex &&
						     nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - nXCoorResult)
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				
			}

		}
		else if(4 == m_nType)
		{
			int nYIndex = nYCoorResult;
			for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight / 3); nYIndex++)
			{
				pSource             += 4 * ( m_nXCoorSource + m_nFigWidth / 2);
				pResultBufferCurent += 4 * ( nXCoorResult   + m_nFigWidth / 2);

				int nXIndex = nXCoorResult + m_nFigWidth / 2;
								
				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
				{	
					if	( nYCoorResult + 5 >= nYIndex &&
						( nXIndex >= nXCoorResult + m_nFigWidth/2 + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - nYCoorResult))
						&& 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
					}
					else if
						( min( nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
						( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex))
						&& 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
					}
					else if
						( nXCoorResult + m_nFigWidth / 2 + 5 >= nXIndex &&
						( nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult - m_nFigWidth / 2) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 3) - ( nXIndex - nXCoorResult - m_nFigWidth / 2))
						&& 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + m_nFigWidth/2 + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4;
					pSource             += 4;
				}
					
			}
			for(; nYIndex < min(nHeight, nYCoorResult + 2 * m_nFigHeight / 3); nYIndex++)
			{
				pSource             += 4 * m_nXCoorSource;
				pResultBufferCurent += 4 * nXCoorResult;
				int nXIndex = nXCoorResult;

				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
				{	
					if( nYCoorResult + m_nFigHeight / 3 + 5 >= nYIndex &&
					  ( nXIndex <= nXCoorResult + m_nFigWidth / 2  - ( nYIndex - nYCoorResult - m_nFigHeight / 3) && nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult - m_nFigHeight / 3))
						&& 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + m_nFigHeight / 3 + 5 - nYIndex, dEffect);
					}
					else if( min(nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - (min(nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
					}
					else if( ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - 5 <= nYIndex) &&
						   ( nXIndex <= nXCoorResult + m_nFigWidth / 2 - ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - nYIndex) && nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - nYIndex)) 
						   && 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - 5), dEffect);
					}
					else if( nXCoorResult + 5 >= nXIndex &&
						   ( nYIndex >= nYCoorResult + m_nFigHeight / 3 + ( nXIndex - nXCoorResult) &&  nYIndex <= min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - ( nXIndex - nXCoorResult))
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				
			}
			for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
			{
				pSource             += 4 * ( m_nXCoorSource + m_nFigWidth / 2);
				pResultBufferCurent += 4 * ( nXCoorResult   + m_nFigWidth / 2);
				int nXIndex = nXCoorResult + m_nFigWidth / 2;

				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
				{	
					if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
					  ( nXIndex >= nXCoorResult + m_nFigWidth / 2 + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex <= nXCoorResult + m_nFigWidth - ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex))
					  && 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( nYCoorResult + m_nFigHeight - 5), dEffect);
					}
					else if( min(nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
						     nYIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex)
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
					}
					else if( nXCoorResult + m_nFigWidth / 2 + 5 >= nXIndex &&
						   ( nYIndex >= nYCoorResult + 2 * m_nFigHeight / 3 + ( nXIndex - nXCoorResult - m_nFigWidth / 2) && nYIndex <= nYCoorResult + m_nFigHeight - ( nXIndex - nXCoorResult - m_nFigWidth / 2))
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine(pResultBufferCurent, pSource, 255, nXCoorResult + m_nFigWidth/2 + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4; 
					pSource             += 4;
				}
				
			}
		}
		else if(5 == m_nType)
		{
			int nYIndex = nYCoorResult;
			for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight / 2); nYIndex++)
			{
				pSource             += 4 * ( m_nXCoorSource + m_nFigWidth / 3);
				pResultBufferCurent += 4 * ( nXCoorResult   + m_nFigWidth / 3);
				int nXIndex = nXCoorResult + m_nFigWidth / 3;

				for(; nXIndex < min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3); nXIndex++)
				{	
					if( nYCoorResult + 5 >= nYIndex &&
					  ( nXIndex >= nXCoorResult + m_nFigWidth / 3 + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - ( nYIndex - nYCoorResult))
						&& 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
					}
					else if( min(nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - 5 <= nXIndex &&
						   ( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 2) - ( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - nXIndex)) 
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + 2 * m_nFigWidth / 3) - 5), dEffect);
					}
					else if( nXCoorResult + m_nFigWidth / 3 + 5 >= nXIndex &&
						   ( nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult - m_nFigWidth / 3) && nYIndex <= min( nHeight, nYCoorResult + 2 * m_nFigHeight / 3) - ( nXIndex - nXCoorResult - m_nFigWidth / 3))
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + m_nFigWidth / 3 + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4;
					pSource += 4;
				}
				
			}
			for(; nYIndex < min(nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
			{
				pSource             += 4 * m_nXCoorSource;
				pResultBufferCurent += 4 * nXCoorResult;

				int nXIndex = nXCoorResult;

				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
				{	
					if( nYCoorResult + m_nFigHeight / 2 + 5 >= nYIndex &&
					  ( ( nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult - m_nFigHeight / 2) && nXIndex <= nXCoorResult + m_nFigWidth / 3 - ( nYIndex - nYCoorResult - m_nFigHeight / 2)) 
					  ||  nXIndex >= nXCoorResult + 2 * m_nFigWidth / 3 + ( nYIndex - nYCoorResult - m_nFigHeight / 2) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - nYCoorResult - m_nFigHeight / 2))
						&& 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + m_nFigHeight / 2 + 5 - nYIndex, dEffect);
					}
					else if( min( nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
						   ( nYIndex >= nYCoorResult + m_nFigHeight / 2 + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex)) 
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
					}
					else if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
						   ( nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex))
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( min( nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
					}
					else if( nXCoorResult + 5 >= nXIndex &&
						   ( nYIndex >= nYCoorResult + m_nFigHeight / 2 + ( nXIndex - nXCoorResult) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - nXCoorResult))
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4;
					pSource             += 4;
				}
			}
				
		}

		else if(6 == m_nType)
		{
			int nYIndex = nYCoorResult;
			for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight / 3); nYIndex++)
			{
				pSource             += 4 * m_nXCoorSource;
				pResultBufferCurent += 4 * nXCoorResult;
				int nXIndex = nXCoorResult;
				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
				{	
					if( nYCoorResult + 5 >= nYIndex &&
					  (nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - nYCoorResult))
					  && 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
					}
					else if( min(nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
						   ( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 3) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex))
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
					}
					else if( min( nHeight, nYCoorResult + m_nFigHeight / 3) - 5 <= nYIndex &&
						   ( nXIndex >= nXCoorResult + m_nFigWidth / 2 + ( min( nHeight, nYCoorResult + m_nFigHeight / 3) - nYIndex) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( min( nHeight, nYCoorResult + m_nFigHeight / 3) - nYIndex))
						   && 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( min( nHeight, nYCoorResult + m_nFigHeight / 3) - 5), dEffect);
					}
					else if( nXCoorResult + 5 >= nXIndex && 
						     nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult)
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4; 
					pSource             += 4;
				}
			
			}
			for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
			{
				pSource             += 4 * m_nXCoorSource;
				pResultBufferCurent += 4 * nXCoorResult;
				int nXIndex = nXCoorResult;

				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth / 2); nXIndex++)
				{	
					if( min( nWidth, nXCoorResult + m_nFigWidth / 2) - 5 <= nXIndex &&
					  ( nYIndex >= nYCoorResult + m_nFigHeight / 3 + ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - nXIndex))
					  && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth / 2) - 5), dEffect);
					}
					else if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
						   ( nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth / 2) - ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex))
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( min( nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
					}
					else if( nXCoorResult + 5 >= nXIndex && 
						     nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - nXCoorResult)
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4;
					pSource             += 4;
				}
			}
					
		}

		else if(7 == m_nType)
		{
			int nYIndex = nYCoorResult;
			for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight / 2); nYIndex++)
			{
				pSource             += 4 * m_nXCoorSource;
				pResultBufferCurent += 4 * nXCoorResult;

				int nXIndex = nXCoorResult;
				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth / 3); nXIndex++)
				{	
					if( nYCoorResult + 5 >= nYIndex &&
					  ( nXIndex >= nXCoorResult + ( nYIndex - nYCoorResult) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth / 3) - ( nYIndex - nYCoorResult))
					  && 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + 5 - nYIndex, dEffect);
					}
					else if( min( nWidth, nXCoorResult + m_nFigWidth / 3) - 5 <= nXIndex &&
						   ( nYIndex >= nYCoorResult + ( min( nWidth, nXCoorResult + m_nFigWidth / 3) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight / 2) - ( min( nWidth, nXCoorResult + m_nFigWidth / 3) - nXIndex))
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth / 3) - 5), dEffect);
					}
					else if( nXCoorResult + 5 >= nXIndex && 
						     nYIndex >= nYCoorResult + ( nXIndex - nXCoorResult)
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4;
					pSource             += 4;
				}
			
			}
			for(; nYIndex < min( nHeight, nYCoorResult + m_nFigHeight); nYIndex++)
			{
				pSource             += 4 * m_nXCoorSource;
				pResultBufferCurent += 4 * nXCoorResult;
				int nXIndex = nXCoorResult;

				for(; nXIndex < min( nWidth, nXCoorResult + m_nFigWidth); nXIndex++)
				{	
					if( min( nWidth, nXCoorResult + m_nFigWidth) - 5 <= nXIndex &&
					  ( nYIndex >= nYCoorResult + m_nFigHeight / 2 + ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex) && nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( min( nWidth, nXCoorResult + m_nFigWidth) - nXIndex))
					  && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nXIndex - ( min( nWidth, nXCoorResult + m_nFigWidth) - 5), dEffect);
					}
					else if( min( nHeight, nYCoorResult + m_nFigHeight) - 5 <= nYIndex &&
						   ( nXIndex >= nXCoorResult + ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex) && nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( min( nHeight, nYCoorResult + m_nFigHeight) - nYIndex))
						   && 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nYIndex - ( min( nHeight, nYCoorResult + m_nFigHeight) - 5), dEffect);
					}
					else if( nYCoorResult + m_nFigHeight / 2 + 5 >= nYIndex &&
						   ( nXIndex <= min( nWidth, nXCoorResult + m_nFigWidth) - ( nYIndex - ( nYCoorResult + m_nFigHeight / 2)) && nXIndex >= nXCoorResult + m_nFigWidth / 3 + ( nYIndex - ( nYCoorResult + m_nFigHeight / 2)))
						   && 0 != dEffect) 
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 0, nYCoorResult + m_nFigHeight / 2 + 5 - nYIndex, dEffect);
					}
					else if( nXCoorResult + 5 >= nXIndex && 
						     nYIndex <= min( nHeight, nYCoorResult + m_nFigHeight) - ( nXIndex - nXCoorResult)
						   && 0 != dEffect)
					{
						DrawAlphaBlendingLine( pResultBufferCurent, pSource, 255, nXCoorResult + 5 - nXIndex, dEffect);
					}
					else
					{
						memcpy( pResultBufferCurent, pSource, 3);
					}
					pResultBufferCurent += 4;
					pSource             += 4;
				}
				for(; nXIndex < nWidth; nXIndex++)
				{
					pResultBufferCurent += 4;
					pSource             += 4;
				}
			}
						
		}

		else 
		{
			return FALSE;
		}
		ippiCopy_8u_C4R(pResultBufferMiddle, 4 * nWidth, pResult, 4 * nWidth, roiSize);

		delete pResultBuffer;

		return TRUE;
	}




public:
	
	int m_nFigHeight;
	int m_nFigWidth;
	
	int m_nPicHeight;
	int m_nPicWidth;
	
	int m_nType;

	BYTE* m_pBGRASource;

	int m_nXCoorSource;
	int m_nYCoorSource;
				
};


class Utility
{
protected:

	BOOL DrawFilmsPerf(BYTE* pBGRA, int nWidth, int nHeight, int nBlendColor, int nBkColor, float dX, float dY, float dWidth, float dHeight, float dRadius)
	{
		BYTE* pBuffer = pBGRA; 
		pBuffer += 4 * nWidth * (nHeight - 1);
		
		Gdiplus::Bitmap oImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBuffer );
		if( Gdiplus::Ok == oImage.GetLastStatus() )
		{
			Gdiplus::Graphics oRender( &oImage );
			
			oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
			oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
			
			Gdiplus::GraphicsPath oPath;

			oPath.AddLine(dX + dRadius, dY, dX + dWidth - (dRadius * 2), dY);
			oPath.AddArc(dX + dWidth - (dRadius * 2), dY, dRadius * 2, dRadius * 2, 270, 90);
			oPath.AddLine(dX + dWidth, dY + dRadius, dX + dWidth, dY + dHeight - (dRadius * 2));
			oPath.AddArc(dX + dWidth - (dRadius * 2), dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2,0,90);
			oPath.AddLine(dX + dWidth - (dRadius * 2), dY + dHeight, dX + dRadius, dY + dHeight);
			oPath.AddArc(dX, dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2, 90, 90);
			oPath.AddLine(dX, dY + dHeight - (dRadius * 2), dX, dY + dRadius);
			oPath.AddArc(dX, dY, dRadius * 2, dRadius * 2, 180, 90);
			oPath.CloseFigure();

			Gdiplus::PathGradientBrush *oBrush = new Gdiplus::PathGradientBrush(&oPath);
			oBrush->SetCenterPoint(Gdiplus::PointF(dX + dWidth / 2, dY + dHeight / 2));
			oBrush->SetCenterColor(Color(nBkColor, nBkColor, nBkColor));
			Color *oColor = new Color( nBkColor, nBkColor, nBkColor);
			int nColorCount = 1;
			oBrush->SetSurroundColors(oColor, &nColorCount);

			oRender.FillPath(oBrush, &oPath);

			Gdiplus::Pen *oPen = new Gdiplus::Pen(Color( nBlendColor, nBlendColor, nBlendColor), 1);
			oRender.DrawArc(  oPen, dX + dWidth - (dRadius * 2), dY, dRadius * 2, dRadius * 2, 315, 45);
			oRender.DrawLine( oPen, dX + dWidth, dY + dRadius - 1, dX + dWidth, dY + dHeight - dRadius + 1);
			oRender.DrawArc(  oPen, dX + dWidth - (dRadius * 2), dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2,0,90);
			oRender.DrawLine( oPen, dX + dWidth - dRadius + 1, dY + dHeight, dX + dRadius - 1, dY + dHeight);
			oRender.DrawArc(  oPen, dX, dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2, 90, 45);

			delete oColor;
			delete oPen;
			delete oBrush;
		}
		return TRUE;
	}

	BOOL DrawFilmsBorder(BYTE* pBGRA, int nWidth, int nHeight, int nFilmColorMid, int nFilmColorBorder, float dBorderWidth, float dBorderHeight, BOOL bVertical)
	{
		BYTE* pBuffer = pBGRA; 
		pBuffer += 4 * nWidth * (nHeight - 1);
		
		Gdiplus::Bitmap oImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBuffer );
		if( Gdiplus::Ok == oImage.GetLastStatus() )
		{
			Gdiplus::Graphics oRender( &oImage );
			
			oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
			oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);

			Gdiplus::Color clMid(nFilmColorMid, nFilmColorMid, nFilmColorMid);
			Gdiplus::Color clBorder(nFilmColorBorder, nFilmColorBorder, nFilmColorBorder);
			
			if ( bVertical )
			{
				Gdiplus::SolidBrush *oSolidBrush = new Gdiplus::SolidBrush(clMid);
				oRender.FillRectangle(oSolidBrush, -1.0, -1.0, float(nWidth + 1.0), float(nHeight + 1.0));

				Gdiplus::LinearGradientBrush* oGradBrush1 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( -1.0, -1.0), Gdiplus::PointF( dBorderWidth + 1, -1.0), clBorder, clMid);									
				oRender.FillRectangle(oGradBrush1, -1.0, -1.0, dBorderWidth + 1, float(nHeight) + 1);
				
				Gdiplus::LinearGradientBrush* oGradBrush2 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( nWidth + 1, -1.0), Gdiplus::PointF( nWidth - dBorderWidth -2.0, -1.0), clBorder, clMid);									
				oRender.FillRectangle(oGradBrush2, nWidth - dBorderWidth - 1, -1.0, dBorderWidth + 1, float(nHeight) + 1);

				delete oGradBrush1;
				delete oGradBrush2;
				delete oSolidBrush;
			}
			else
			{
				Gdiplus::SolidBrush *oSolidBrush = new Gdiplus::SolidBrush(clMid);
				oRender.FillRectangle(oSolidBrush, -1.0, -1.0, float(nWidth + 1.0), float(nHeight + 1.0));

				Gdiplus::LinearGradientBrush* oGradBrush1 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( -1.0, -1.0), Gdiplus::PointF( -1.0, dBorderHeight + 1), clBorder, clMid);									
				oRender.FillRectangle(oGradBrush1, -1.0, -1.0, float(nWidth) + 2.0, dBorderHeight + 1);
				
				Gdiplus::LinearGradientBrush* oGradBrush2 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( -1.0, float(nHeight) + 1.0), Gdiplus::PointF( -1.0, nHeight - dBorderHeight - 1.0), clBorder, clMid);									
				oRender.FillRectangle(oGradBrush2, float(-1.0), float(nHeight - dBorderHeight - 1.0), float(nWidth + 2.0), float(dBorderHeight + 2.0));

				delete oGradBrush1;
				delete oGradBrush2;
				delete oSolidBrush;
			}
		}
		return TRUE;
	}



public:
	
	Utility()
	{
		g_fSin = 0;
		g_fCos = 0;

		g_sBurningPalette = 0;
		g_pBurningMask = 0;
		
		g_nBurningMaskWidth = -1;
		g_nBurningMaskHeight = -1;
		
		g_nRand = -1;
			
		InitSinCos();		
		InitBurningPalette();
						
		g_pShotSource1 = 0;
		g_pShotSource2 = 0;
		g_nShotType = -1;

		//g_pMask = 0;
		//g_nMaskType = -1;
		//g_nMaskWidth = -1;
		//g_nMaskHeight = -1;

		g_ppWipeRandomStrips = 0;
		g_nWipeStripsCount = -1;

	}
	
	~Utility()	
	{			
		if (g_fSin)	
			delete[] g_fSin;
		if (g_fCos)
			delete[] g_fCos;
		if (g_sBurningPalette)
			delete[] g_sBurningPalette;
		if (g_pBurningMask)
			delete[] g_pBurningMask;
		if (g_pShotSource1)
			delete[] g_pShotSource1;
		if (g_pShotSource2)
			delete[] g_pShotSource2;
		for(int nIndex = 0; nIndex < g_nWipeStripsCount; nIndex++ )
		{
			if(g_ppWipeRandomStrips[nIndex])
				delete[] g_ppWipeRandomStrips[nIndex];
		}
		if (g_ppWipeRandomStrips)
			delete[] g_ppWipeRandomStrips;
		}
	void InitSinCos()
	{
		if (!g_fSin)
			g_fSin = new float[720];
		if (!g_fCos)
			g_fCos = new float[720];

		for (int i = 0; i < 720; i++)
		{
			g_fCos[i] = (float)cos(M_PI*i/360);
			g_fSin[i] = (float)sin(M_PI*i/360);
		}

		g_fSin[0] = 0.000001f;
		g_fSin[360] = 0.000001f;
	}
	void InitBurningPalette()
	{
		if (!g_sBurningPalette)
			g_sBurningPalette = new Ipp32s[768];

		int r,g,b;
		r = 256+256+255;
		g = 256+255;
		b = 255;
		for(int i = 255; i > 239; i--)
		{
			g_sBurningPalette[i]	= 255;
			g_sBurningPalette[i+256]	= 255;
			g_sBurningPalette[i+512]	= 255;
		}
		for(int i = 239; i > 15; i--)
		{
			g_sBurningPalette[i]	= (r > 255) ? 255 : r;
			g_sBurningPalette[i+256]	= (g > 255) ? 255 : g;
			g_sBurningPalette[i+512]	= (b > 255) ? 255 : b;
			r = (r > 3) ? (r - 4) : 0;
			g = (g > 3) ? (g - 4) : 0;
			b = (b > 3) ? (b - 4) : 0;
		}
		for(int i = 15; i >= 0; i--)
		{
			g_sBurningPalette[i]	= 0;
			g_sBurningPalette[i+256]	= 0;
			g_sBurningPalette[i+512]	= 0;
		}
	}
	void GenerateBurningMask(int nWidth, int nHeight)
	{
		if ((nWidth == g_nBurningMaskWidth) && (nHeight == g_nBurningMaskHeight) && (g_pBurningMask))
			return;

		if (g_pBurningMask)
			delete []g_pBurningMask;

		if (0 == nWidth*nHeight)
			return;

		srand(GetTickCount());
		g_nBurningMaskWidth = nWidth;
		g_nBurningMaskHeight = nHeight;
		g_pBurningMask = new BYTE[nWidth*nHeight];
		Ipp16s *pBuffer = new Ipp16s[nWidth*nHeight];
		IppiSize roiSize = {nWidth,nHeight};
		memset(g_pBurningMask,0,nWidth*nHeight);
		memset(pBuffer,0,nWidth*nHeight*sizeof(Ipp16s));

		float fH = 512.0f;
		float fR = (float)nWidth;
		int k = 0;
		for (k=2;k<5;k++)
		{
			fH /= 2;
			fR /= 3;
			int nSign = 1;
			if (0==rand()%2)
				nSign = -1;
			for(int p=0;p<20*k;p++)
			{
				int nCenterX = rand()%nWidth;
				int nCenterY = rand()%nHeight;
				for (int i=0;i<nWidth;i++)
				{
					for (int j=0;j<nHeight;j++)
					{
						float fTemp = (float)sqrt((double)(i-nCenterX)*(i-nCenterX)+(j-nCenterY)*(j-nCenterY));
						if (fTemp<fR)
						{
							pBuffer[i + j*g_nBurningMaskWidth] += fH*nSign*(1.0-fTemp/fR)*(1.0-fTemp/fR);
						}
					}
				}
			}
		}

		fH = 2;
		fR /= 3;
		int nSign = 1;
		if (0==rand()%2)
			nSign = -1;
		for(int p=0;p<20*k;p++)
		{
			int nCenterX = rand()%nWidth;
			int nCenterY = rand()%nHeight;
			for (int i=0;i<nWidth;i++)
			{
				for (int j=0;j<nHeight;j++)
				{
					float fTemp = (float)sqrt((double)(i-nCenterX)*(i-nCenterX)+(j-nCenterY)*(j-nCenterY));
					if (fTemp<fR)
					{
						pBuffer[i + j*g_nBurningMaskWidth] += fH*nSign*(1.0-fTemp/fR)*(1.0-fTemp/fR);
					}
				}
			}
		}

		Ipp16s nMin, nMax;
		ippiMin_16s_C1R(pBuffer, 2*nWidth, roiSize, &nMin);
		ippiMax_16s_C1R(pBuffer, 2*nWidth, roiSize, &nMax);
		nMax = max(nMax,nMin+1);
		for (int i=0;i<nWidth;i++)
		{
			for (int j=0;j<nHeight;j++)
			{
				pBuffer[i + j*g_nBurningMaskWidth] -= nMin;
				g_pBurningMask[i + j*g_nBurningMaskWidth] = 255.0f/(float)(nMax-nMin)*(pBuffer[i + j*g_nBurningMaskWidth]);
			}
		}

		delete []pBuffer;
	}												
	void GenerateRand(int nRandMax)
	{
		if (-1 != g_nRand)
			return;
		g_nRand = 0;
		::srand(::GetTickCount());
		g_nRand = rand()% ( nRandMax + 1 );
	}
	BOOL RunTetris(CTetrisFigure *pList, int nFiguresCount, float dFrame, BYTE *pBGRAResult, int nWidth, int nHeight, int nStep, int *pnBottomLineFigures)
	{
		if(!pList)
			return FALSE;
		BYTE* pr = pBGRAResult;
		int nSt = nStep;
		if(dFrame < 1./nFiguresCount)
			pList->Cpy(pr, pList->m_nXCoorSource, nHeight - *pnBottomLineFigures + pList->m_nYCoorSource, nWidth, nHeight, 1);		
		else
		{			
			nSt++;
			if(nSt == nFiguresCount - 1)
				for(int i = 0; i < nFiguresCount - 1; i++)
					(pList+i)->Cpy(pr, (pList+i)->m_nXCoorSource, (pList+i)->m_nYCoorSource, nWidth, nHeight, 1 - (dFrame - (nFiguresCount - 1.)/nFiguresCount)*nFiguresCount);	
			else								
			{				
				(pList+nSt - 1)->Cpy(pr, (pList+nSt - 1)->m_nXCoorSource, (pList+nSt - 1)->m_nYCoorSource, nWidth, nHeight, 1);					
				if(dFrame < (nSt + 1.)/nFiguresCount)				
					(pList+nSt)->Cpy(pr, (pList+nSt)->m_nXCoorSource, nHeight - *(pnBottomLineFigures + nSt) + (pList+nSt)->m_nYCoorSource, nWidth, nHeight, 1);								
				else
					if(!RunTetris(pList, nFiguresCount, dFrame, pr, nWidth, nHeight, nSt, pnBottomLineFigures))
							return FALSE;
			}
		}
						return TRUE;
	}


	BOOL CollapseTetris(CTetrisFigure *pList, int nFiguresCount, float dFrame, BYTE *pBGRAResult, int nWidth, int nHeight, int nStep, int *pnBottomLineFigures)
	{
		if(!pList)
			return FALSE;
		BYTE* pr = pBGRAResult;
		int nSt = nStep;
		if(dFrame < 1./nFiguresCount)
			for(int i = 0; i < nFiguresCount - 1; i++)
				(pList + i)->Cpy(pr, (pList + i)->m_nXCoorSource, (pList + i)->m_nYCoorSource, nWidth, nHeight, dFrame*nFiguresCount);
		else
		{
			nSt++;
			if(nSt > nFiguresCount)
				return TRUE;
			else if(dFrame < (nSt + 1.)/nFiguresCount)
			{
				(pList + nSt - 1)->Cpy(pr, (pList + nSt - 1)->m_nXCoorSource, (pList + nSt - 1)->m_nYCoorSource*(1-(dFrame*nFiguresCount - nSt)) + (-(pList + nSt - 1)->m_nFigHeight)*(dFrame*nFiguresCount - nSt), nWidth, nHeight, 1);							
				for(int i = nSt; i < nFiguresCount - 1; i++)
					(pList+i)->Cpy(pr, (pList+i)->m_nXCoorSource, (pList+i)->m_nYCoorSource, nWidth, nHeight, 1);							
			}
			else
			{
				if(!CollapseTetris(pList, nFiguresCount, dFrame, pr, nWidth, nHeight, nSt, pnBottomLineFigures))
					return FALSE;
			}
		}
		return TRUE;
	}

	void GenerateShot(int nWidth, int nHeight, int nType, double dPercent, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor)
	{
		//if(g_pShotSource1 && g_pShotSource2 && nType == g_nShotType)
		//	return;

		if(g_pShotSource1)
			delete[] g_pShotSource1;
		if(g_pShotSource2)
			delete[] g_pShotSource2;

		BOOL bInverse;
		BOOL bRapprochement;
		int nDirection;

		if (c_nShotChangeGHorizontalRToL == nType || c_nShotChangeBHorizontalRToL == nType)
		{
			bInverse = TRUE;
			bRapprochement = TRUE;
			nDirection = 0;
		}
		else if(c_nShotChangeGHorizontalLToR == nType || c_nShotChangeBHorizontalLToR == nType )
		{
			bInverse = FALSE;
			bRapprochement = TRUE;
			nDirection = 0;
		}
		else if(c_nShotChangeGVerticalUToB == nType  || c_nShotChangeBVerticalUToB == nType )
		{
			bInverse = FALSE;
			bRapprochement = TRUE;
			nDirection = 1;
		}
		else if(c_nShotChangeGVerticalBToU == nType  || c_nShotChangeBVerticalBToU == nType )
		{
			bInverse = TRUE;
			bRapprochement = TRUE;
			nDirection = 1;
		}
		else if(c_nShotChangeGHorizontalRToLNoZ == nType  || c_nShotChangeBHorizontalRToLNoZ == nType )
		{
			bInverse = TRUE;
			bRapprochement = FALSE;
			nDirection = 0;
		}
		else if(c_nShotChangeGHorizontalLToRNoZ == nType  || c_nShotChangeBHorizontalLToRNoZ == nType )
		{
			bInverse = FALSE;
			bRapprochement = FALSE;
			nDirection = 0;
		}
		else if(c_nShotChangeGVerticalUToBNoZ == nType  || c_nShotChangeBVerticalUToBNoZ == nType )
		{
			bInverse = FALSE;
			bRapprochement = FALSE;
			nDirection = 1;
		}
		else if(c_nShotChangeGVerticalBToUNoZ == nType  || c_nShotChangeBVerticalBToUNoZ == nType )
		{
			bInverse = TRUE;
			bRapprochement = FALSE;
			nDirection = 1;
		}

		g_pShotSource1 = new BYTE[nWidth*nHeight*4];
		g_pShotSource2 = new BYTE[nWidth*nHeight*4];

		g_nShotType = nType;

		//BOOL bVertical = (1 == nDirection? TRUE : FALSE);

		//if( 1 == nDirection )
		//{
		//	double dUnit                  = nWidth / 35.;
		//	const double dWidthPerf       = 2.8 * dUnit;
		//	const double dHeightPerf      = 1.98 * dUnit;
		//	const double dRadiusPerf      = 0.5 * dUnit;
		//	const double dStepPerf        = (4.75 * dUnit < 10? 10 : 4.75 * dUnit);
		//	const double dBorderWidth     = 1.01 * dUnit;
		//	const double dMidLineWidth    = 0.4 * dUnit;
		//	const double dRadiusFactor    = 1.3 * sqrt(2.) / 2 * dRadiusPerf;
		//	
		//	double dStartY1 = 0;
		//	double dStartY2 = 0;

		//	double dAdditional = nHeight;
		//	if ( bRapprochement )
		//		dAdditional += -nHeight * dPercent / 2;

		//	if ( !bInverse )
		//		dStartY2 = dAdditional;
		//	else
		//		dStartY1 = dAdditional;

		//	const double dFullBorderWidth = dBorderWidth + dWidthPerf + dMidLineWidth;

		//	DrawFilmsBorder(g_pShotSource1, nWidth, nHeight, nFilmColorMid, nFilmColorBorder, dFullBorderWidth, 0.1 * dFullBorderWidth, bVertical);
		//	DrawFilmsBorder(g_pShotSource2, nWidth, nHeight, nFilmColorMid, nFilmColorBorder, dFullBorderWidth, 0.1 * dFullBorderWidth, bVertical);

		//	for ( double dCurY = - 2 * nHeight - dStartY1; dCurY < nHeight + dStepPerf; dCurY += dStepPerf)
		//	{
		//		DrawFilmsPerf(g_pShotSource1, nWidth, nHeight, nBlendColor, nBkColor, dBorderWidth, dCurY,                       dWidthPerf, dHeightPerf, dRadiusPerf);
		//		DrawFilmsPerf(g_pShotSource1, nWidth, nHeight, nBlendColor, nBkColor, nWidth - dBorderWidth - dWidthPerf, dCurY, dWidthPerf, dHeightPerf, dRadiusPerf);
		//	}
		//	for ( double dCurY = - 2 * nHeight - dStartY2; dCurY < nHeight + dStepPerf; dCurY += dStepPerf)
		//	{
		//		DrawFilmsPerf(g_pShotSource2, nWidth, nHeight, nBlendColor, nBkColor, dBorderWidth, dCurY,                       dWidthPerf, dHeightPerf, dRadiusPerf);
		//		DrawFilmsPerf(g_pShotSource2, nWidth, nHeight, nBlendColor, nBkColor, nWidth - dBorderWidth - dWidthPerf, dCurY, dWidthPerf, dHeightPerf, dRadiusPerf);
		//	}
		//	
		//}
		//else
		//{
		//	double dUnit                  = nHeight / 35.;
		//	const double dWidthPerf       = 2.8 * dUnit;
		//	const double dHeightPerf      = 1.98 * dUnit;
		//	const double dRadiusPerf      = 0.5 * dUnit;
		//	const double dStepPerf        = (4.75 * dUnit < 10? 10 : 4.75 * dUnit);
		//	const double dBorderWidth     = 1.01 * dUnit;
		//	const double dMidLineWidth    = 0.4 * dUnit;
		//	const double dRadiusFactor    = 1.3 * sqrt(2.) / 2 * dRadiusPerf;

		//	double dStartX1 = 0;
		//	double dStartX2 = 0;

		//	double dAdditional = nWidth;
		//	if ( bRapprochement )
		//		dAdditional += -nWidth * dPercent / 2;

		//	if ( !bInverse )
		//		dStartX2 = dAdditional;
		//	else
		//		dStartX1 = dAdditional;

		//	const double dFullBorderWidth = dBorderWidth + dWidthPerf + dMidLineWidth;

		//	DrawFilmsBorder(g_pShotSource1, nWidth, nHeight, nFilmColorMid, nFilmColorBorder, 0.1 * dFullBorderWidth, dFullBorderWidth, bVertical);
		//	DrawFilmsBorder(g_pShotSource2, nWidth, nHeight, nFilmColorMid, nFilmColorBorder, 0.1 * dFullBorderWidth, dFullBorderWidth, bVertical);

		//	for ( double dCurX = - 2*nWidth - dStartX1; dCurX < nWidth + dStepPerf; dCurX += dStepPerf)
		//	{
		//		DrawFilmsPerf(g_pShotSource1, nWidth, nHeight, nBlendColor, nBkColor, dCurX, dBorderWidth,                        dHeightPerf, dWidthPerf, dRadiusPerf);
		//		DrawFilmsPerf(g_pShotSource1, nWidth, nHeight, nBlendColor, nBkColor, dCurX, nHeight - dBorderWidth - dWidthPerf, dHeightPerf, dWidthPerf, dRadiusPerf);
		//	}
		//	for ( double dCurX = - 2*nWidth - dStartX2; dCurX < nWidth + dStepPerf; dCurX += dStepPerf)
		//	{
		//		DrawFilmsPerf(g_pShotSource2, nWidth, nHeight, nBlendColor, nBkColor, dCurX, dBorderWidth,                        dHeightPerf, dWidthPerf, dRadiusPerf);
		//		DrawFilmsPerf(g_pShotSource2, nWidth, nHeight, nBlendColor, nBkColor, dCurX, nHeight - dBorderWidth - dWidthPerf, dHeightPerf, dWidthPerf, dRadiusPerf);
		//	}
		//}
		//return;
		if( 1 == nDirection)
		{

			const double dUnit            = nWidth / 35.;
			const double dWidthPerf       = 2.8 * dUnit;
			const double dHeightPerf      = 1.98 * dUnit;
			const double dRadiusPerf      = 0.5 * dUnit;
			const double dStepPerf        = 4.75 * dUnit;
			const double dBorderWidth     = 1.01 * dUnit;
			const double dRadiusFactor    = 1.3 * sqrt(2.) / 2 * dRadiusPerf;
			
			BYTE *pBuffer = g_pShotSource1;

			for (int nIndex = 0; nIndex < 2; nIndex++ )
			{
				if ( 0 == nIndex )
					pBuffer = g_pShotSource1;
				else
					pBuffer = g_pShotSource2;

				for(int nYIndex = 0; nYIndex < nHeight; nYIndex++)
				{
					int nAdditional = nHeight;
					if ( bRapprochement )
					{
						nAdditional += - nHeight * dPercent / 2;
					}
					int nYIndexS1 = nYIndex;
					int nYIndexS2 = nYIndex + nAdditional;
					if(!bInverse)
					{
						nYIndexS1 += nAdditional;
						nYIndexS2 -= nAdditional;
					}
					int nIndexS;
					if ( 0 == nIndex )
						nIndexS = nYIndexS1;
					else
						nIndexS = nYIndexS2;

					for(int nXIndex = 0; nXIndex < nWidth; nXIndex++)
					{
						//постоянные для расчетов

						const int nYStepCount = nIndexS % int(dStepPerf); // ордината нашей точки в пределах одного шага(шаг = ширина дырки + расстояние между дырками)
						const int nFirstFocus = dBorderWidth + dRadiusFactor; // ордината центра первого закругления
						const int nSecondFocus = dBorderWidth + dWidthPerf - dRadiusFactor; // ордината центра второго закругления
						const int nFullBorderWidth = dBorderWidth + dWidthPerf; // суммарная ширина края (ширина края + ширина дырок)
						const double dSqrRadiusPerf = dRadiusPerf * dRadiusPerf;
						const double dSqrDecRadiusPerf = (dRadiusPerf - 1) * (dRadiusPerf - 1);
						const double dSqrIncRadiusPerf = (dRadiusPerf + 1) * (dRadiusPerf + 1);


						if( nXIndex <= dBorderWidth || nXIndex > nWidth - dBorderWidth || 
							((( nXIndex > dBorderWidth && nXIndex <= nFullBorderWidth) || ( nXIndex >= nWidth - nFullBorderWidth && nXIndex < nWidth - dBorderWidth)) && nYStepCount > int(dHeightPerf) ) ||
							/*скругление углов*/ /*слева*/
							(nXIndex <= nFirstFocus                                                   && (nYStepCount) <= dRadiusFactor               &&  (nXIndex - nFirstFocus)  * (nXIndex - nFirstFocus)  + (nYStepCount - dRadiusFactor)               * (nYStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
							(nXIndex <= nFirstFocus                                                   && (nYStepCount) >= dHeightPerf - dRadiusFactor &&  (nXIndex - nFirstFocus)  * (nXIndex - nFirstFocus)  + (nYStepCount - dHeightPerf + dRadiusFactor) * (nYStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
							(nXIndex >= nSecondFocus && nXIndex <= nFullBorderWidth                   && (nYStepCount) >= dHeightPerf - dRadiusFactor &&  (nXIndex - nSecondFocus) * (nXIndex - nSecondFocus) + (nYStepCount - dHeightPerf + dRadiusFactor) * (nYStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
							(nXIndex >= nSecondFocus && nXIndex <= nFullBorderWidth                   && (nYStepCount) <= dRadiusFactor               &&  (nXIndex - nSecondFocus) * (nXIndex - nSecondFocus) + (nYStepCount - dRadiusFactor)               * (nYStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
							/*справа*/
							(nXIndex <= nWidth - nSecondFocus && nXIndex >= nWidth - nFullBorderWidth && (nYStepCount) <= dRadiusFactor               &&  (nXIndex - (nWidth - nSecondFocus)) * (nXIndex - (nWidth - nSecondFocus)) + (nYStepCount - dRadiusFactor)               * (nYStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
							(nXIndex <= nWidth - nSecondFocus && nXIndex >= nWidth - nFullBorderWidth && (nYStepCount) >= dHeightPerf - dRadiusFactor &&  (nXIndex - (nWidth - nSecondFocus)) * (nXIndex - (nWidth - nSecondFocus)) + (nYStepCount - dHeightPerf + dRadiusFactor) * (nYStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
							(nXIndex >= nWidth - nFirstFocus                                          && (nYStepCount) >= dHeightPerf - dRadiusFactor &&  (nXIndex - (nWidth - nFirstFocus))  * (nXIndex - (nWidth - nFirstFocus))  + (nYStepCount - dHeightPerf + dRadiusFactor) * (nYStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
							(nXIndex >= nWidth - nFirstFocus                                          && (nYStepCount) <= dRadiusFactor               &&  (nXIndex - (nWidth - nFirstFocus))  * (nXIndex - (nWidth - nFirstFocus))  + (nYStepCount - dRadiusFactor)               * (nYStepCount - dRadiusFactor)               > dSqrRadiusPerf))
						{
							for(int i = 0; i < 3; i++)
							{
								if(nXIndex <= nFullBorderWidth)
								{
									*pBuffer = nFilmColorBorder * (1 - double(nXIndex) / double (nFullBorderWidth) ) + nFilmColorMid * (double(nXIndex) / double (nFullBorderWidth));
								}
								else
								{
									*pBuffer = nFilmColorBorder * double(nXIndex - (nWidth - nFullBorderWidth))/double(nFullBorderWidth) + nFilmColorMid * (1 - double(nXIndex - (nWidth - nFullBorderWidth))/double(nFullBorderWidth));
								}
								pBuffer++;
							}
							*pBuffer = 255;
							pBuffer++;
						}
						else if( nXIndex > nFullBorderWidth && nXIndex < nWidth - nFullBorderWidth)
						{
							for(int i = 0; i < 3; i++)
							{
								*pBuffer = nFilmColorMid; 
								pBuffer++;
							}
							*pBuffer = 255;
							pBuffer++;
						}
						else
						{
							const double dLeftFirstSqrRad = ( nXIndex - nFirstFocus) * ( nXIndex - nFirstFocus) + (nYStepCount - dRadiusFactor) * (nYStepCount - dRadiusFactor);
							const double dLeftSecondSqrRad = ( nXIndex - nSecondFocus) * ( nXIndex - nSecondFocus) + (nYStepCount - dRadiusFactor) * (nYStepCount - dRadiusFactor);
							const double dRightFirstSqrRad = ( nXIndex - ( nWidth - nSecondFocus)) * ( nXIndex - ( nWidth - nSecondFocus)) + (nYStepCount - dRadiusFactor) * (nYStepCount - dRadiusFactor);
							const double dRightSecondSqrRad = ( nXIndex - ( nWidth - nFirstFocus)) * ( nXIndex - ( nWidth - nFirstFocus)) + (nYStepCount - dRadiusFactor) * (nYStepCount - dRadiusFactor);
							
							if	//слева
								( nXIndex >= dBorderWidth &&
									(( nYStepCount == 0  && nXIndex <= nSecondFocus && nXIndex >= nFirstFocus) 
									 ||
									( dLeftFirstSqrRad > dSqrDecRadiusPerf && dLeftFirstSqrRad < dSqrIncRadiusPerf 
									  && nXIndex <= nFirstFocus  && nYStepCount <= dRadiusFactor) 
									 ||
									( dLeftSecondSqrRad > dSqrDecRadiusPerf && dLeftSecondSqrRad < dSqrIncRadiusPerf 
									  && nXIndex >= nSecondFocus && nXIndex <= nFullBorderWidth  && nYStepCount <= dRadiusFactor) 
									 ||
									  (nXIndex > nFullBorderWidth - 1 && nXIndex < nFullBorderWidth + 1 ))											   ||
								//справа
								  nXIndex <= nWidth - dBorderWidth &&
									(( nYStepCount == 0  && nXIndex >= nWidth - nSecondFocus && nXIndex <= nWidth - nFirstFocus ) 
									 ||
									( dRightFirstSqrRad > dSqrDecRadiusPerf && dRightFirstSqrRad < dSqrIncRadiusPerf 
									  && nXIndex >= nWidth - nFullBorderWidth && nXIndex <= nWidth - nSecondFocus && nYStepCount <= dRadiusFactor) 
									 ||
									( dRightSecondSqrRad > dSqrDecRadiusPerf && dRightSecondSqrRad < dSqrIncRadiusPerf 
									 &&  nXIndex >= nWidth - nFirstFocus && nYStepCount <= dRadiusFactor) 
									 ||
									  (nXIndex > nWidth - dBorderWidth - 1 && nXIndex < nWidth - dBorderWidth + 1 ))
								)
							{
								for(int i = 0; i < 3; i++)
								{
									*pBuffer = nBlendColor;
									*pBuffer++;
								}
								*pBuffer = 255;
								pBuffer++;
							}
							else
							{
								for(int i = 0; i < 3; i++)
								{
									*pBuffer = nBkColor;
									*pBuffer++;
								}
								*pBuffer = 255;
								pBuffer++;
							}
						}
					}
				}
			}
		}
		else
		{

			const double dUnit            = nHeight / 35.;
			const double dWidthPerf       = 2.8 * dUnit;
			const double dHeightPerf      = 1.98 * dUnit;
			const double dRadiusPerf      = 0.5 * dUnit;
			const double dStepPerf        = 4.75 * dUnit;
			const double dBorderWidth     = 1. * dUnit;
			const double dRadiusFactor    = 1.3 * sqrt(2.) / 2 * dRadiusPerf;

			BYTE *pBuffer = g_pShotSource1;

			for (int nIndex = 0; nIndex < 2; nIndex++ )
			{
				if ( 0 == nIndex )
					pBuffer = g_pShotSource1;
				else
					pBuffer = g_pShotSource2;

				for(int nYIndex = 0; nYIndex < nHeight; nYIndex++)
				{
					for(int nXIndex = 0; nXIndex < nWidth; nXIndex++)
					{
						int nAdditional = nWidth;
						if ( bRapprochement )
						{
							nAdditional -= nWidth * dPercent / 2;
						}
						int nXIndexS2 = nXIndex;
						int nXIndexS1 = nXIndex + nAdditional;
						if(!bInverse)
						{
							nXIndexS2 += nAdditional;
							nXIndexS1 -= nAdditional;
						}
						int nIndexS = 0;
						if ( 0 == nIndex)
							nIndexS = nXIndexS1;
						else
							nIndexS = nXIndexS2;


						//постоянные для расчетов

						const int nXStepCount = nIndexS % int(dStepPerf); // ордината нашей точки в пределах одного шага(шаг = ширина дырки + расстояние между дырками)
						const int nFirstFocus = dBorderWidth + dRadiusFactor; // ордината центра первого закругления
						const int nSecondFocus = dBorderWidth + dWidthPerf - dRadiusFactor; // ордината центра второго закругления
						const int nFullBorderWidth = dBorderWidth + dWidthPerf; // суммарная ширина края (ширина края + ширина дырок)
						const double dSqrRadiusPerf = dRadiusPerf * dRadiusPerf;
						const double dSqrDecRadiusPerf = (dRadiusPerf - 1) * (dRadiusPerf - 1);
						const double dSqrIncRadiusPerf = (dRadiusPerf + 1) * (dRadiusPerf + 1);


						if( nYIndex <= dBorderWidth || nYIndex >= nHeight - dBorderWidth || 
							((( nYIndex > dBorderWidth && nYIndex <= nFullBorderWidth) || ( nYIndex >= nHeight - nFullBorderWidth && nYIndex < nHeight - dBorderWidth)) && nXStepCount > int(dHeightPerf) ) ||
							/*скругление углов*/ /*сверху*/
							(nYIndex <= nFirstFocus                                                              && nXStepCount <= dRadiusFactor               &&  (nYIndex - nFirstFocus)  * (nYIndex - nFirstFocus)  + (nXStepCount - dRadiusFactor)               * (nXStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
							(nYIndex <= nFirstFocus                                                              && nXStepCount >= dHeightPerf - dRadiusFactor &&  (nYIndex - nFirstFocus)  * (nYIndex - nFirstFocus)  + (nXStepCount - dHeightPerf + dRadiusFactor) * (nXStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
							(nYIndex >= nSecondFocus && nYIndex <= dBorderWidth + dWidthPerf                     && nXStepCount >= dHeightPerf - dRadiusFactor &&  (nYIndex - nSecondFocus) * (nYIndex - nSecondFocus) + (nXStepCount - dHeightPerf + dRadiusFactor) * (nXStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
							(nYIndex >= nSecondFocus && nYIndex <= dBorderWidth + dWidthPerf                     && nXStepCount <= dRadiusFactor               &&  (nYIndex - nSecondFocus) * (nYIndex - nSecondFocus) + (nXStepCount - dRadiusFactor)               * (nXStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
							/*снизу*/
							(nYIndex <= nHeight - nSecondFocus && nYIndex >= nHeight - dBorderWidth - dWidthPerf && nXStepCount <= dRadiusFactor               &&  (nYIndex - (nHeight - nSecondFocus)) * (nYIndex - (nHeight - nSecondFocus)) + (nXStepCount - dRadiusFactor)               * (nXStepCount - dRadiusFactor)               > dSqrRadiusPerf) ||
							(nYIndex <= nHeight - nSecondFocus && nYIndex >= nHeight - dBorderWidth - dWidthPerf && nXStepCount >= dHeightPerf - dRadiusFactor &&  (nYIndex - (nHeight - nSecondFocus)) * (nYIndex - (nHeight - nSecondFocus)) + (nXStepCount - dHeightPerf + dRadiusFactor) * (nXStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf) ||
							(nYIndex >= nHeight - nFirstFocus                                                    && nXStepCount >= dHeightPerf - dRadiusFactor &&  (nYIndex - (nHeight - nFirstFocus))  * (nYIndex - (nHeight - nFirstFocus))  + (nXStepCount - dHeightPerf + dRadiusFactor) * (nXStepCount - dHeightPerf + dRadiusFactor) > dSqrRadiusPerf ) ||
							(nYIndex >= nHeight - nFirstFocus                                                    && nXStepCount <= dRadiusFactor               &&  (nYIndex - (nHeight - nFirstFocus))  * (nYIndex - (nHeight - nFirstFocus))  + (nXStepCount - dRadiusFactor)               * (nXStepCount- dRadiusFactor)                > dSqrRadiusPerf))
						{
							for(int i = 0; i < 3; i++)
							{
								if( nYIndex <= nFullBorderWidth)
								{
									*pBuffer = nFilmColorBorder * (1 - nYIndex / (dBorderWidth + dWidthPerf)) + nFilmColorMid * (nYIndex / (dBorderWidth + dWidthPerf));
									//double dKoef = double(nYIndex) / double(nFullBorderWidth);
									//*pBuffer = nFilmColorBorder * (1 - dKoef ) + nFilmColorMid * dKoef;
								}
								else
								{
									*pBuffer = nFilmColorBorder * (nYIndex - (nHeight - (dBorderWidth + dWidthPerf))) / (dBorderWidth + dWidthPerf) + nFilmColorMid * (1 - (nYIndex - (nHeight - (dBorderWidth + dWidthPerf)))/ (dBorderWidth + dWidthPerf));
									//double dKoef = double(nYIndex - (nHeight - nFullBorderWidth)) / double(nFullBorderWidth);
									//*pBuffer = nFilmColorBorder * dKoef + nFilmColorMid * (1 - dKoef);
								}
								pBuffer++;
							}
							*pBuffer = 255;
							pBuffer++;
						}
						else if(nYIndex > nFullBorderWidth && nYIndex < nHeight - nFullBorderWidth)
						{
							for(int i = 0; i < 3; i++)
							{
								*pBuffer = nFilmColorMid; 
								pBuffer++;
							}
							*pBuffer = 255;
							pBuffer++;
						}
						else
						{
							const double dTopFirstSqrRad =     ( nYIndex - nFirstFocus) * ( nYIndex - nFirstFocus) + (nXStepCount - dHeightPerf + dRadiusFactor ) * ( nXStepCount - dHeightPerf + dRadiusFactor);
							const double dTopSecondSqrRad =    ( nYIndex - nFirstFocus) * ( nYIndex - nFirstFocus) + (nXStepCount - dRadiusFactor)                * ( nXStepCount - dRadiusFactor);
							const double dBottomFirstSqrRad =  ( nYIndex - ( nHeight - nSecondFocus)) * ( nYIndex - ( nHeight - nSecondFocus)) + (nXStepCount - dHeightPerf + dRadiusFactor) * ( nXStepCount - dHeightPerf + dRadiusFactor);
							const double dBottomSecondSqrRad = ( nYIndex - ( nHeight - nSecondFocus)) * ( nYIndex - ( nHeight - nSecondFocus)) + (nXStepCount - dRadiusFactor)               * ( nXStepCount - dRadiusFactor);

							if( 
								nYIndex >= dBorderWidth &&nYIndex <= nHeight - dBorderWidth &&										   
							    ((( nXStepCount > dHeightPerf - 1 && nXStepCount < dHeightPerf + 1  && nYIndex > nFirstFocus - 10/*заглушка*/) 
							    ||
								( dTopFirstSqrRad > dSqrDecRadiusPerf && dTopFirstSqrRad < dSqrIncRadiusPerf 
								  && nYIndex < nFirstFocus && nXStepCount >= dHeightPerf - dRadiusFactor) 
								||
								( dTopSecondSqrRad > dSqrDecRadiusPerf && dTopSecondSqrRad < dSqrIncRadiusPerf 
								  && nYIndex < nFirstFocus && nXStepCount <= dRadiusFactor) 
								||
								  (nYIndex < dBorderWidth + 1 && nYIndex > dBorderWidth - 1) ) 
								||
								(( nXStepCount > dHeightPerf - 1 && nXStepCount < dHeightPerf + 1  && nYIndex >= nHeight - nSecondFocus ) 
								||
								( dBottomFirstSqrRad > dSqrDecRadiusPerf && dBottomFirstSqrRad < dSqrIncRadiusPerf 
								  && nYIndex >= nHeight - nFullBorderWidth && nYIndex <= nHeight - nSecondFocus && nXStepCount >= dHeightPerf - dRadiusFactor) ||
								( dBottomSecondSqrRad > dSqrDecRadiusPerf && dBottomSecondSqrRad < dSqrIncRadiusPerf 
								  && nYIndex >= nHeight - nFullBorderWidth && nYIndex <= nHeight - nSecondFocus && nXStepCount <= dRadiusFactor) 
								||
								  (nYIndex < nHeight - nFullBorderWidth + 1 && nYIndex > nHeight - nFullBorderWidth - 1 ) )))
							{
								for(int i = 0; i < 3; i++)
								{
									*pBuffer = nBlendColor;
									pBuffer++;
								}
							}
							else
							{
								for(int i = 0; i < 3; i++)
								{
									*pBuffer = nBkColor;
									pBuffer++;
								}
							}
							*pBuffer = 255;
							pBuffer++;
						}
					}
				}
			}
		}
		return;

	}




	void GenerateWipeStrips(int nCount)
	{
		if ( g_ppWipeRandomStrips && g_nWipeStripsCount == nCount )
			return;
				
		if ( g_ppWipeRandomStrips )
		{
			for(int nIndex = 0; nIndex < g_nWipeStripsCount; nIndex++ )
			{
				if(g_ppWipeRandomStrips[nIndex])
					delete[] g_ppWipeRandomStrips[nIndex];
			}
			
			if (g_ppWipeRandomStrips)
				delete[] g_ppWipeRandomStrips;				
		}
		g_nWipeStripsCount = nCount;

		::srand(::GetTickCount());
		g_ppWipeRandomStrips = new int*[nCount];

		for( int nIndex = 0; nIndex < nCount; nIndex++ )
		{							
			g_ppWipeRandomStrips[nIndex] = new int[256];
		}
		for(int nCountIndex = 0; nCountIndex < nCount; nCountIndex++ )
		{
			for(int nTime = 0; nTime < 256; nTime++)
			{
				g_ppWipeRandomStrips[nCountIndex][nTime] = rand() % 100;
			}
		}			
		return;					
	}
				
public:
										
	float* g_fCos;					
	float* g_fSin;					
					
	int	g_nBurningMaskWidth;					
	int	g_nBurningMaskHeight;					
					
	Ipp32s* g_sBurningPalette;					
	Ipp8u* g_pBurningMask;	
	int g_nRand;

	BYTE* g_pShotSource1;
	BYTE* g_pShotSource2;
	int g_nShotType;
	
	int** g_ppWipeRandomStrips;
	int g_nWipeStripsCount;				
};
	
class MaskPinwheelCurved
{
public:

	MaskPinwheelCurved()
	{
		g_nMaskWidth = -1;
		g_nMaskHeight = -1;
		g_nMaskType = -1;

		g_pMask = 0;
	}
	~MaskPinwheelCurved()
	{
		if ( g_pMask )
			delete []g_pMask;
	}

	void GenerateMask(int nWidth, int nHeight, int nType)
	{
		if ((nWidth == g_nMaskWidth) && (nHeight == g_nMaskHeight) && (g_pMask) && (nType == g_nMaskType))
			return ;

		if (g_pMask)
			delete []g_pMask;

		int nSize = nWidth * nHeight;
		if( !nSize )
			return;

		g_nMaskType = nType;
		g_nMaskWidth  = nWidth;
		g_nMaskHeight = nHeight;

		g_pMask = new BYTE[nSize];
		if( !g_pMask )
			return;

		
		ZeroMemory( g_pMask, nSize );

		if ( c_nPinwheelCurvedSN5A <= nType && c_nPinwheelPropeller12Smooth >= nType )
			PinwheelCurved();

		return;
	}



protected:

	void PinwheelCurved()
	{
		double dParts = 1;
		double dAmplitFactor = 0.1;
		double dPhaseFactor = 2.0;

		switch( g_nMaskType )
		{
		case c_nPinwheelCurvedSN5A:        dParts =  5; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
		case c_nPinwheelCurvedSN7A:	       dParts =  7; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
		case c_nPinwheelCurvedSN12A:       dParts = 12; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
		case c_nPinwheelCurvedSW5A:        dParts =  5; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
		case c_nPinwheelCurvedSW7A:        dParts =  7; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
		case c_nPinwheelCurvedSW12A:       dParts = 12; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
		case c_nPinwheelCurvedLN5A:	       dParts =  5; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
		case c_nPinwheelCurvedLN7A:        dParts =  7; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
		case c_nPinwheelCurvedLN12A:       dParts = 12; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
		case c_nPinwheelPropeller4:        dParts =  4; dAmplitFactor = 0.4; dPhaseFactor =  3.0; break;
		case c_nPinwheelPropeller7:	       dParts =  7; dAmplitFactor = 0.4; dPhaseFactor =  3.0; break;
		case c_nPinwheelPropeller12:       dParts = 12; dAmplitFactor = 0.4; dPhaseFactor =  3.0; break;

		case c_nPinwheelCurvedSN5ASmooth:  dParts =  5; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
		case c_nPinwheelCurvedSN7ASmooth:  dParts =  7; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
		case c_nPinwheelCurvedSN12ASmooth: dParts = 12; dAmplitFactor = 0.1; dPhaseFactor = 10.0; break;
		case c_nPinwheelCurvedSW5ASmooth:  dParts =  5; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
		case c_nPinwheelCurvedSW7ASmooth:  dParts =  7; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
		case c_nPinwheelCurvedSW12ASmooth: dParts = 12; dAmplitFactor = 0.1; dPhaseFactor =  2.0; break;
		case c_nPinwheelCurvedLN5ASmooth:  dParts =  5; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
		case c_nPinwheelCurvedLN7ASmooth:  dParts =  7; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
		case c_nPinwheelCurvedLN12ASmooth: dParts = 12; dAmplitFactor = 0.8; dPhaseFactor =  2.0; break;
		case c_nPinwheelPropeller4Smooth:  dParts =  4; dAmplitFactor = 0.4; dPhaseFactor =  3.0; break;
		case c_nPinwheelPropeller7Smooth:  dParts =  7; dAmplitFactor = 0.4; dPhaseFactor =  3.0; break;
		case c_nPinwheelPropeller12Smooth: dParts = 12; dAmplitFactor = 0.4; dPhaseFactor =  3.0; break;
		}

		// создаём таблицу для ускорения расчётов
		int nSizeTable = int(g_nMaskWidth + g_nMaskHeight);
		double* pTable = new double[(nSizeTable + 1)];
		if( !pTable )
		{
			memset( g_pMask, 255, g_nMaskWidth * g_nMaskHeight );
			return;
		}

		double dMaxRadius = (g_nMaskWidth * g_nMaskWidth + g_nMaskHeight * g_nMaskHeight) / 4.0;
		double dFactor = dMaxRadius / nSizeTable;

		double dAmplit = sqrt(dMaxRadius) * dAmplitFactor; // * 4;
		if ( ( g_nMaskType >= c_nPinwheelPropeller4 && g_nMaskType <= c_nPinwheelPropeller12 ) || 
			 ( g_nMaskType >= c_nPinwheelPropeller4Smooth && g_nMaskType <= c_nPinwheelPropeller12Smooth ))
			 dAmplit = dMaxRadius * dAmplitFactor;
		double dPhase  = IPP_2PI / g_nMaskWidth * dPhaseFactor;

		// заполняем таблицу
		pTable[0] = 0;
		for( int i = 1; i <= nSizeTable; i++ )
		{
			double dr = sqrt(i * dFactor);
			pTable[i] = sin( dr * dPhase ) * dAmplit / dr;
		}
		
		// ещё одна вспомагательная таблица
		int nSizeHelp = max(g_nMaskWidth, g_nMaskHeight);
		double* pHelp = new double[nSizeHelp * 2];
		if( !pHelp )
		{
			delete [] pTable;
			memset( g_pMask, 255, g_nMaskWidth * g_nMaskHeight );
			return;
		}

		int nMinSize = nSizeHelp / 2;
		int nMaxSize = nSizeHelp - nMinSize;

		dFactor = nSizeTable / dMaxRadius;

		double* pItem = pHelp;
		for( int i = -nMinSize; i < nMaxSize; ++i, pItem += 2 )
		{
			pItem[0] = i;
			pItem[1] = i * i * dFactor;
		}
		
		Ipp8u* pBuffer = g_pMask;
		int nWidth  = g_nMaskWidth;
		int nHeight = g_nMaskHeight;

		double* pItemY = pHelp + (nMinSize - nHeight / 2) * 2;
		double* pItemX = pHelp + (nMinSize - nWidth  / 2) * 2;

		dParts *= 255 / IPP_2PI;

		for( int nY = nHeight; nY > 0; --nY, pItemY += 2, pItemX -= nWidth * 2 )
		{
			for ( int nX = nWidth; nX > 0; --nX, pItemX += 2 )
			{
				double dot = pItemY[1] + pItemX[1];
				
				int index = int(dot);
				double dr = pTable[index];

				// интерполяция
				if( index < nSizeTable )
				{
					dot -= index;
					dr += (pTable[index + 1] - dr) * dot;
				}

				double dPos = pItemX[0] - dr * pItemY[0];
				double dRow = pItemY[0] + dr * pItemX[0];
				
				double dAngle = atan2( dRow, dPos );

				*pBuffer++ = int( dAngle * dParts /*+ 128*/);
			}
		}

		//int nHalfWidth  = -g_nMaskWidth / 2;
		//int nHalfHeight = -g_nMaskHeight / 2;
		//int nMaxWidth   = g_nMaskWidth  + nHalfWidth;
		//int nMaxHeight  = g_nMaskHeight + nHalfHeight;

		//for ( int nY = nHalfHeight; nY < nMaxHeight; ++nY )
		//{
		//	for ( int nX = nHalfWidth; nX < nMaxWidth; ++nX )
		//	{
		//		double dot = (nX * nX + nY * nY) * dFactor;
		//		
		//		int index = int(dot);
		//		double dr = pTable[index];

		//		// интерполяция
		//		if( index < nSizeTable )
		//		{
		//			dot -= index;
		//			dr += (pTable[index + 1] - dr) * dot;
		//		}

		//		double dPos = nX - dr * nY;
		//		double dRow = nY + dr * nX;
		//		
		//		double dAngle = atan2( dRow, dPos );

		//		*pBuffer++ = int( dAngle * dParts + 128);
		//	}
		//}

		delete [] pHelp;
		delete [] pTable;
	}

public:

	int	g_nMaskWidth;
	int	g_nMaskHeight;
	
	Ipp8u* g_pMask;
	int g_nMaskType;

};




class MaskGraduate
{

public:

	MaskGraduate()
	{
		g_nMaskWidth = -1;
		g_nMaskHeight = -1;
		g_nMaskType = -1;
		g_nTilesX = -1;
		g_nTilesY = -1;

		g_pMask = 0;

		g_nSize = 0;
		g_pGraduation = NULL;
	}

	~MaskGraduate()
	{
		if ( g_pMask )
			delete []g_pMask;

		if ( g_pGraduation )
			delete []g_pGraduation;
	}

	void GenerateMask(int nWidth, int nHeight, int nType, BOOL bWipeSide)
	{
		if ((nWidth == g_nMaskWidth) && (nHeight == g_nMaskHeight) && (g_pMask) && (nType == g_nMaskType))
			return ;

		if (g_pMask)
			delete []g_pMask;

		int nSize = nWidth * nHeight;
		if( !nSize )
			return;

		g_nMaskType = nType;
		g_nMaskWidth  = nWidth;
		g_nMaskHeight = nHeight;

		if ( c_nGraduateSideLeftToRight <= nType && c_nGraduateCornerRightTop >= nType && bWipeSide)
		{
			g_nTilesX = 5;
			g_nTilesY = 5;
		}
		else if ( c_nGraduateClockBottomClockwise <= nType && c_nGraduateClock3AnglesReverse >= nType  && !bWipeSide)
		{
			g_nTilesX = 2;
			g_nTilesY = 2;
		}
		g_nTileWidth = nWidth / g_nTilesX + 2;
		g_nTileHeight = nHeight / g_nTilesY + 2;

		g_pMask = new BYTE[nSize];
		if( !g_pMask )
			return;
		
		ZeroMemory( g_pMask, nSize );
		RandomShuffle(g_nTilesX * g_nTilesY);

		if ( c_nGraduateSideLeftToRight <= nType && c_nGraduateCornerRightTop >= nType && bWipeSide)
			WipeSide();
		else if ( c_nGraduateClockBottomClockwise <= nType && c_nGraduateClock3AnglesReverse >= nType && !bWipeSide)
			Clock();
		return;
	}


protected:

	void WipeSide()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{								
				int nCurTileX = nXIndex / g_nTileWidth;
				int nCurTileY = nYIndex / g_nTileHeight;

				int nCTX = nCurTileX; 
				int nCTY = nCurTileY; 

				nCurTileX = g_pGraduation[nCTX  + nCTY * g_nTilesX]  / g_nTilesX;
				nCurTileY = g_pGraduation[nCTX  + nCTY * g_nTilesX]  % g_nTilesX;
				double dMaxValue = 255 * (nCurTileX  + nCurTileY * g_nTilesX + 1) / (g_nTilesX * g_nTilesY);
				double dMinValue = 255 * (nCurTileX  + nCurTileY * g_nTilesX) / (g_nTilesX * g_nTilesY);
				double dCur = 1;
				if( c_nGraduateSideLeftToRight == g_nMaskType ) 
					dCur = double(nXIndex % g_nTileWidth) / g_nTileWidth;
				else if ( c_nGraduateSideRightToLeft == g_nMaskType )
					dCur = 1 - double(nXIndex % g_nTileWidth) / g_nTileWidth;
				else if ( c_nGraduateSideTopToBottom == g_nMaskType )
					dCur = 1 - double(nYIndex % g_nTileHeight) / g_nTileHeight;
				else if ( c_nGraduateSideBottomToTop == g_nMaskType )
					dCur = double(nYIndex % g_nTileHeight) / g_nTileHeight;
				else if ( c_nGraduateCornerLeftTop == g_nMaskType )
					dCur = max(double(nXIndex % g_nTileWidth) / g_nTileWidth, 1 - double(nYIndex % g_nTileHeight) / g_nTileHeight);
				else if ( c_nGraduateCornerLeftBottom == g_nMaskType )
					dCur = max(double(nXIndex % g_nTileWidth) / g_nTileWidth, double(nYIndex % g_nTileHeight) / g_nTileHeight);
				else if ( c_nGraduateCornerRightTop == g_nMaskType )
					dCur = max(1 - double(nXIndex % g_nTileWidth) / g_nTileWidth, 1 - double(nYIndex % g_nTileHeight) / g_nTileHeight);
				else if ( c_nGraduateCornerRightBottom == g_nMaskType )
					dCur = max(1 - double(nXIndex % g_nTileWidth) / g_nTileWidth, double(nYIndex % g_nTileHeight) / g_nTileHeight);
				*pBuffer = dMaxValue * dCur + ( 1 - dCur) * dMinValue;
				pBuffer++;
			}
		}

	}

	void Clock()
	{
		Ipp8u* pBuffer = g_pMask;

		int nAnglesCount = 1;
		int nStartAngle = 0;
		if ( c_nGraduateClockLeftOpposite == g_nMaskType || c_nGraduateClockLeftClockwise == g_nMaskType || c_nGraduateClockLeftReverse == g_nMaskType )
		{
			nStartAngle = 180;
		}
		else if ( c_nGraduateClockTopOpposite == g_nMaskType || c_nGraduateClockTopClockwise == g_nMaskType || c_nGraduateClockTopReverse == g_nMaskType )
		{
			nStartAngle = 270;
		}
		else if ( c_nGraduateClockBottomOpposite == g_nMaskType || c_nGraduateClockBottomClockwise == g_nMaskType || c_nGraduateClockBottomReverse == g_nMaskType )
		{
			nStartAngle = 90;
		}

		if ( c_nGraduateClock2AnglesClockwise == g_nMaskType || c_nGraduateClock2AnglesReverse == g_nMaskType )
		{
			nAnglesCount = 2;
			nStartAngle = 270;
		}
		else if ( c_nGraduateClock3AnglesClockwise == g_nMaskType || c_nGraduateClock3AnglesReverse == g_nMaskType )
		{
			nStartAngle = 270;
			nAnglesCount = 3;
		}
		BOOL bReverse = FALSE;

		double dRadianFactor = 1 / IPP_PI180; 


		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{								
				int nCurTileX = nXIndex / g_nTileWidth;
				int nCurTileY = nYIndex / g_nTileHeight;

				int nCTX = nCurTileX; 
				int nCTY = nCurTileY; 

				nCurTileX = g_pGraduation[nCTX  + nCTY * g_nTilesX]  / g_nTilesX;
				nCurTileY = g_pGraduation[nCTX  + nCTY * g_nTilesX]  % g_nTilesX;
				double dMaxValue = 255 * (nCurTileX  + nCurTileY * g_nTilesX + 1) / (g_nTilesX * g_nTilesY);
				double dMinValue = 255 * (nCurTileX  + nCurTileY * g_nTilesX) / (g_nTilesX * g_nTilesY);
				double dCur = 1;
				double dX = nXIndex - (nCTX + 0.5) * g_nTileWidth;
				double dY = nYIndex - (nCTY + 0.5) * g_nTileHeight;

				double dRadius = sqrt(dX * dX + dY * dY);
				double dSecY = (dY >= 0 ? 1.0 : -1.0);
				double dAngle = (dRadius ? acos( dX / dRadius) * dSecY : 0) * dRadianFactor;
				dAngle = (dAngle < 0 ? dAngle + 360 : dAngle );
				dAngle = int(dAngle + nStartAngle) % 360;

				dAngle = int(dAngle * nAnglesCount) % 360;

				if( c_nGraduateClockRightOpposite == g_nMaskType || c_nGraduateClockLeftOpposite   == g_nMaskType ||
					c_nGraduateClockTopOpposite   == g_nMaskType || c_nGraduateClockBottomOpposite == g_nMaskType) 
				{
					if ( 180 >= dAngle ) 
						dCur = dAngle / 180;
					else
						dCur = (360 - dAngle) / 180;
				}
				else if( c_nGraduateClockRightClockwise == g_nMaskType || c_nGraduateClockLeftClockwise   == g_nMaskType ||
					     c_nGraduateClockTopClockwise   == g_nMaskType || c_nGraduateClockBottomClockwise == g_nMaskType ||
						 c_nGraduateClock2AnglesClockwise == g_nMaskType || c_nGraduateClock3AnglesClockwise == g_nMaskType ) 
				{
						dCur = 1 - dAngle / 360;
				}
				else if( c_nGraduateClockRightReverse == g_nMaskType || c_nGraduateClockLeftReverse   == g_nMaskType ||
					     c_nGraduateClockTopReverse   == g_nMaskType || c_nGraduateClockBottomReverse == g_nMaskType ||
						 c_nGraduateClock2AnglesReverse == g_nMaskType || c_nGraduateClock3AnglesReverse == g_nMaskType )
				{
						dCur = dAngle / 360;
				}
				*pBuffer = dMaxValue * dCur + ( 1 - dCur) * dMinValue;
				pBuffer++;
			}
		}

	}


	BOOL RandomShuffle(int nSize)
	{
		
		if (nSize < 1)
			return FALSE;
		if (nSize == g_nSize)
			return TRUE;

		g_nSize = nSize;
		g_pGraduation = new int[g_nSize];

		if (NULL == g_pGraduation)
			return FALSE;

		for (int index = 0; index < g_nSize; ++index)
			g_pGraduation[index] = index;

		//srand(GetTickCount());
		srand( 1 );

		for (int index = 0; index < g_nSize; ++index)
		{
			int nSwapIndex = rand() % g_nSize;
			int nSwapValue = g_pGraduation[nSwapIndex];

			g_pGraduation[nSwapIndex] = g_pGraduation[index];
			g_pGraduation[index] = nSwapValue;
		}

		return TRUE;
	}


public:

	Ipp8u* g_pMask;

protected:

	int	 g_nMaskWidth;
	int	 g_nMaskHeight;

	int  g_nSize;
	int* g_pGraduation; 
	int  g_nMaskType;
	int  g_nTilesX;
	int  g_nTilesY;
	int  g_nTileWidth;
	int  g_nTileHeight;

	
};


class Mask
{
public:

	Mask()
	{
		g_nMaskWidth = -1;
		g_nMaskHeight = -1;
		g_nMaskType = -1;

		g_pMask = 0;
	}
	~Mask()
	{
		if ( g_pMask )
			delete []g_pMask;
	}

	void GenerateMask(int nWidth, int nHeight, int nType)
	{
		if ((nWidth == g_nMaskWidth) && (nHeight == g_nMaskHeight) && (g_pMask) && (nType == g_nMaskType))
			return ;

		if (g_pMask)
			delete []g_pMask;

		int nSize = nWidth * nHeight;
		if( !nSize )
			return;

		g_nMaskType = nType;
		g_nMaskWidth  = nWidth;
		g_nMaskHeight = nHeight;

		g_pMask = new BYTE[nSize];
		if( !g_pMask )
			return;

		
		ZeroMemory( g_pMask, nSize );

		if ( c_nBowTileHorizontal == nType || c_nBowTileSmoothHorizontal == nType ||
			 c_nBowTileVertical == nType   || c_nBowTileSmoothVertical == nType)
			BowTile();
		else 
		if ( c_nDiagonalBoxOut == nType || c_nDiagonalSmoothBoxOut == nType)
			DiagonalBoxOut();
		else 
		if ( c_nDiagonalCrossOut == nType || c_nDiagonalSmoothCrossOut == nType)
			DiagonalCrossOut();
		else 
		if ( c_nDiagonalDownRight == nType || c_nDiagonalSmoothDownRight == nType ||
		  	 c_nDiagonalDownLeft == nType || c_nDiagonalSmoothDownLeft == nType ||
			 c_nDiagonalUpRight == nType || c_nDiagonalSmoothUpRight == nType ||
			 c_nDiagonalUpLeft == nType || c_nDiagonalSmoothUpLeft == nType)
			Diagonal();
		else
		if ( c_nFunIn == nType || c_nFunSmoothIn == nType)
			FunIn();
		else 
		if ( c_nFunOut == nType || c_nFunSmoothOut == nType)
			FunOut();
		else
		if ( c_nFunUp == nType || c_nFunSmoothUp == nType)
			FunUp();
		else 
		if ( c_nFilledVDown == nType || c_nFilledVSmoothDown == nType ||
			 c_nFilledVRight == nType || c_nFilledVSmoothRight == nType || 
			 c_nFilledVLeft == nType || c_nFilledVSmoothLeft == nType ||
			 c_nFilledVUp == nType || c_nFilledVSmoothUp == nType )
			FilledV();

		else 
		if ( c_nInsertDownLeft == nType || c_nInsertSmoothDownLeft == nType ||
			 c_nInsertDownRight == nType || c_nInsertSmoothDownRight == nType ||
			 c_nInsertUpLeft == nType || c_nInsertSmoothUpLeft == nType ||
			 c_nInsertUpRight == nType || c_nInsertSmoothUpRight == nType )
			Insert();
		else
		if ( c_nRevealUp == nType || c_nRevealDown == nType || 
			 c_nRevealLeft == nType || c_nRevealRight == nType)
			Reveal();
		else 
		if ( c_nSplitHorizontal == nType || c_nSplitSmoothHorizontal == nType ||
			 c_nSplitVertical == nType || c_nSplitSmoothVertical == nType )
			Split();
		else 
		if ( c_nSweepIn == nType || c_nSweepSmoothIn == nType )
			SweepIn();
		else
		if ( c_nSweepOut == nType || c_nSweepSmoothOut == nType )
			SweepOut();
		else
		if ( c_nSweepUp == nType || c_nSweepSmoothUp == nType )
			SweepUp();
		else
		if ( c_nZigZagHorizontal == nType       || c_nZigZagVertical == nType
		  || c_nZigZagSmoothHorizontal == nType || c_nZigZagSmoothVertical == nType)
			ZigZag();
		else if ( c_nWipeSmoothDiamond == nType || c_nWipeSmoothDiamondIn == nType )
			Diamond();
		else if ( c_nWipeSmoothDiamonds == nType || c_nWipeDiamonds == nType )
			Diamonds();
		else if ( c_nWipeSmoothCircle == nType || c_nWipeSmoothCircleIn == nType )
			Circle();
		else if ( c_nWipeSmoothCircles == nType || c_nWipeCircles == nType )
			Circles();
		else if ( c_nWipeSmoothWideStar   == nType || c_nWipeSmoothNarrowStar   == nType ||
			      c_nWipeSmoothWideFlower == nType || c_nWipeSmoothNarrowFlower == nType)
			StarFlower();
		else if ( c_nWipeSmoothWideStars   == nType || c_nWipeSmoothNarrowStars   == nType ||
			      c_nWipeSmoothWideFlowers == nType || c_nWipeSmoothNarrowFlowers == nType)
			StarsFlowers();
		else if ( c_nWipeSmoothWideKeyHole == nType || c_nWipeSmoothNarrowKeyHole == nType)
			KeyHole();
		else if ( c_nWipeSmoothWideKeyHoles == nType || c_nWipeSmoothNarrowKeyHoles == nType)
			KeyHoles();
		else if ( c_nCircleCornerLeftBottom == nType || c_nCircleCornerSmoothLeftBottom == nType ||
				  c_nCircleCornerLeftTop == nType || c_nCircleCornerSmoothLeftTop == nType ||
				  c_nCircleCornerRightBottom == nType || c_nCircleCornerSmoothRightBottom == nType ||
				  c_nCircleCornerRightTop == nType || c_nCircleCornerSmoothRightTop == nType )
			CircleCorner();

		else if( c_nBarsHorizontal == nType)
			BarsHorizontal();
		else if( c_nBarsVertical == nType)
			BarsVertical();
		else if( c_nWipeSmoothBoxIn == nType || c_nWipeSmoothBoxOut == nType)
			Box();

		return;
	}


protected:

	void BowTile()
	{
		int nType = g_nMaskType;
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				if( c_nBowTileHorizontal == nType || c_nBowTileSmoothHorizontal == nType)
				{
					*pBuffer = 128 - abs( nXIndex - g_nMaskWidth / 2 ) * 255. / g_nMaskWidth + abs( nYIndex - g_nMaskHeight / 2 ) * 256 / g_nMaskHeight;
				}
				else
				{
					*pBuffer = 128 + abs( nXIndex - g_nMaskWidth / 2 ) * 255. / g_nMaskWidth - abs( nYIndex - g_nMaskHeight / 2 ) * 255 / g_nMaskHeight;
				}
				pBuffer++;
			}
		}
	}

	void DiagonalBoxOut()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				int nValue = abs( nXIndex - g_nMaskWidth / 2 ) * 511. / g_nMaskWidth + abs( nYIndex - g_nMaskHeight / 2 ) * 511. / g_nMaskHeight;
				*pBuffer = (nValue > 255? nValue : 255 - nValue);
				pBuffer++;
			}
		}
	}
	void DiagonalCrossOut()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				*pBuffer = abs( abs( nXIndex - g_nMaskWidth / 2 ) * 511. / g_nMaskWidth - abs( nYIndex - g_nMaskHeight / 2 ) * 511 / g_nMaskHeight );	
				pBuffer++;
			}
		}
	}
						
	void Diagonal()
	{
		int nType = g_nMaskType;
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				if( c_nDiagonalDownRight == nType || c_nDiagonalSmoothDownRight == nType)
					*pBuffer = ( nXIndex + g_nMaskHeight - nYIndex ) * 255. / ( g_nMaskHeight + g_nMaskWidth );	
				else if( c_nDiagonalDownLeft == nType || c_nDiagonalSmoothDownLeft == nType)
					*pBuffer = ( g_nMaskWidth - nXIndex + g_nMaskHeight - nYIndex ) * 255. / ( g_nMaskHeight + g_nMaskWidth );	
				else if( c_nDiagonalUpRight == nType || c_nDiagonalSmoothUpRight == nType)
					*pBuffer = ( nXIndex + nYIndex ) * 255. / ( g_nMaskHeight + g_nMaskWidth );	
				else if( c_nDiagonalUpLeft == nType || c_nDiagonalSmoothUpLeft == nType)
					*pBuffer = ( g_nMaskWidth - nXIndex + nYIndex ) * 255. / ( g_nMaskHeight + g_nMaskWidth );	
				pBuffer++;
			}
		}
	}

	void FunIn()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				if(nYIndex > g_nMaskHeight / 2)
				{
					double dY = nXIndex - g_nMaskWidth / 2;
					double dX = nYIndex - 3 * g_nMaskHeight / 4;			
					double dRadius = _hypot(dX, dY);
					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					double dAngle = abs((dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265);
					*pBuffer = int(255 * ( dAngle)/180 );	
				}
				else
				{
					double dY = nXIndex - g_nMaskWidth / 2;
					double dX = nYIndex - g_nMaskHeight / 4;			
					double dRadius = _hypot(dX, dY);

					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					double dAngle = 180 - abs((dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265);
					*pBuffer = int(255 * ( dAngle)/180 );	
				}
				pBuffer++;
			}
		}
	}

	void FunOut()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				double dY = nYIndex - g_nMaskHeight / 2;			
				if(nXIndex > g_nMaskWidth / 2)
				{
					double dX = nXIndex - 3 * g_nMaskWidth / 4;
					double dRadius = _hypot(dX, dY);
					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					double dAngle = -(dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265 + 180;
					*pBuffer = int(255 * ( dAngle)/360 );	

				}
				else
				{
					double dX = nXIndex - g_nMaskWidth / 4;
					double dRadius = _hypot(dX, dY);

					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					double dAngle = (dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265;
					dAngle += (dAngle < 0 ? 360 : 0);
					*pBuffer = int(255 * ( dAngle)/360 );	
				}
				pBuffer++;
			}
		}
	}

	void FunUp()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				double dX = nYIndex - g_nMaskHeight;			
				double dY = nXIndex - g_nMaskWidth / 2;
				double dRadius = _hypot(dX, dY);
				double dSecY = (dY >= 0 ? 1.0 : -1.0);
				double dAngle = 180 - abs((dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265);
				*pBuffer = int(255 * ( dAngle)/90 );	
				pBuffer++;
			}
		}
	}
	
	void FilledV()
	{
		int nType = g_nMaskType;
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				if( c_nFilledVDown == nType || c_nFilledVSmoothDown == nType )
				{
					*pBuffer = abs( nXIndex - g_nMaskWidth / 2 ) * 128. / ( g_nMaskWidth / 2) + ( g_nMaskHeight - nYIndex) * 128./ g_nMaskHeight;	
				}
				else if( c_nFilledVRight == nType || c_nFilledVSmoothRight == nType )
				{
					*pBuffer = ( nXIndex) * 128. / g_nMaskWidth + abs( nYIndex - g_nMaskHeight / 2 ) * 128./ ( g_nMaskHeight / 2 );	
				}
				else if( c_nFilledVLeft == nType || c_nFilledVSmoothLeft == nType )
				{
					*pBuffer = ( g_nMaskWidth - nXIndex) * 128. / g_nMaskWidth + abs( nYIndex - g_nMaskHeight / 2 ) * 128./ ( g_nMaskHeight / 2 );	
				}
				else if( c_nFilledVUp == nType || c_nFilledVSmoothUp == nType )
				{
					*pBuffer = abs( nXIndex - g_nMaskWidth / 2 ) * 128. / ( g_nMaskWidth / 2) + ( nYIndex) * 128./ g_nMaskHeight;	
				}
				pBuffer++;
			}
		}
	}
	void Insert()
	{
		int nType = g_nMaskType;
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				if( c_nInsertDownLeft == nType || c_nInsertSmoothDownLeft == nType )
				{
					*pBuffer = max( (g_nMaskWidth - nXIndex) * 255. / g_nMaskWidth, (g_nMaskHeight - nYIndex) * 255. / g_nMaskHeight);	
				}
				else if( c_nInsertDownRight == nType || c_nInsertSmoothDownRight == nType )
				{
					*pBuffer = max( (nXIndex) * 255. / g_nMaskWidth, (g_nMaskHeight - nYIndex) * 255. / g_nMaskHeight);	
				}
				else if( c_nInsertUpLeft == nType || c_nInsertSmoothUpLeft == nType )
				{
					*pBuffer = max( (g_nMaskWidth - nXIndex) * 255. / g_nMaskWidth, nYIndex * 255. / g_nMaskHeight);	
				}
				else if( c_nInsertUpRight == nType || c_nInsertSmoothUpRight == nType )
				{
					*pBuffer = max( nXIndex * 255. / g_nMaskWidth, nYIndex * 255. / g_nMaskHeight);	
				}
				pBuffer++;
			}
		}
	}

	void Reveal()
	{
		int nType = g_nMaskType;
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				if( c_nRevealUp == nType)
				{
					*pBuffer = nYIndex * 255. / g_nMaskHeight;	
				}
				else if( c_nRevealDown == nType)
				{
					*pBuffer = ( g_nMaskHeight - nYIndex ) * 255. / g_nMaskHeight;	
				}
				else if( c_nRevealLeft == nType)
				{
					*pBuffer = ( g_nMaskWidth - nXIndex ) * 255. / g_nMaskWidth;	
				}
				else if( c_nRevealRight == nType)
				{
					*pBuffer = nXIndex * 255. / g_nMaskWidth;	
				}
				pBuffer++;
			}
		}
	}
	void Split()
	{
		int nType = g_nMaskType;
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				if( c_nSplitHorizontal == nType || c_nSplitSmoothHorizontal == nType )
				{
					*pBuffer = abs( nYIndex - g_nMaskHeight / 2) * 511. / g_nMaskWidth;	
				}
				else if( c_nSplitVertical == nType || c_nSplitSmoothVertical == nType )
				{
					*pBuffer = abs( nXIndex - g_nMaskWidth / 2) * 511. / g_nMaskWidth;	
				}
				pBuffer++;
			}
		}
	}
	void SweepIn()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				if ( nYIndex > int( g_nMaskHeight * nXIndex / ( 2. * g_nMaskWidth ) + g_nMaskHeight / 4. ))
				{
					double dY = nXIndex;
					double dX = nYIndex - g_nMaskHeight / 4;			
					double dRadius = _hypot(dX, dY);
					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					double dAngle = (dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265;
					*pBuffer = int(255 * ( dAngle)/(90 - atan( g_nMaskHeight / (2. * g_nMaskWidth ) ) * 180 / 3.14159265));	
				}
				else if ( nYIndex < int( g_nMaskHeight * nXIndex / ( 2. * g_nMaskWidth ) + g_nMaskHeight / 4. ))
				{
					double dY = nXIndex - g_nMaskWidth;
					double dX = nYIndex - 3 * g_nMaskHeight / 4;			
					double dRadius = _hypot(dX, dY);

					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					double dAngle = 180 + (dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265;
					*pBuffer = int(255 * ( dAngle)/( 90 - atan( g_nMaskHeight / (2. * g_nMaskWidth ) ) * 180 / 3.14159265));	
				}
				else 
				{
					*pBuffer = 255;
				}
				pBuffer++;
			}
		}
	}
	void SweepOut()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				if(nXIndex > g_nMaskWidth / 2)
				{
					double dX = nYIndex - g_nMaskHeight / 2;
					double dY = nXIndex - g_nMaskWidth;			
					double dRadius = _hypot(dX, dY);
					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					double dAngle = -(dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265;
					*pBuffer = int(255 * dAngle / 180);	
				}
				else
				{
					double dY = nXIndex;
					double dX = nYIndex - g_nMaskHeight / 2;			
					double dRadius = _hypot(dX, dY);

					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					double dAngle = (dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265;
					*pBuffer = int(255 * dAngle / 180);	
				}
				pBuffer++;
			}
		}
	}
	void SweepUp()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				if(nXIndex > g_nMaskWidth / 2)
				{
					double dY = nYIndex;
					double dX = nXIndex - g_nMaskWidth;			
					double dRadius = _hypot(dX, dY);
					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					double dAngle = 180 - (dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265;
					*pBuffer = int(255 * dAngle / 90);	
				}
				else
				{
					double dY = nYIndex;
					double dX = nXIndex;			
					double dRadius = _hypot(dX, dY);
					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					double dAngle = (dRadius ? acos( dX / dRadius) * dSecY : 0) * 180 / 3.14159265;
					*pBuffer = int(255 * dAngle / 90);	
				}
				pBuffer++;
			}
		}
	}				
	void ZigZag()
	{
		int nType = g_nMaskType;
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				int g_nMaskHeightBuf = g_nMaskHeight;
				int g_nMaskWidthBuf = g_nMaskWidth;
				int nXIndexBuf = nXIndex;
				int nYIndexBuf = nYIndex;

				if( c_nZigZagVertical == nType || c_nZigZagSmoothVertical == nType)
				{
					int nBuf = g_nMaskHeightBuf;
					g_nMaskHeightBuf = g_nMaskWidthBuf;
					g_nMaskWidthBuf = nBuf;
					nBuf = nXIndexBuf;
					nXIndexBuf = nYIndexBuf;
					nYIndexBuf = nBuf;
				}
				//::srand( ::GetTickCount() );
				//GenerateRand(15);
				//const int nZigZagCount = g_nRand + 1;
				const int nZigZagCount = 4;
				const double dZigZagWidth = g_nMaskWidthBuf / ( nZigZagCount * 2 + 1 );
				const double dZigZagHeight = g_nMaskHeightBuf / 5;
				const double dTimeUnit = 255. / ( g_nMaskHeightBuf / 2 + dZigZagHeight / 2 );

				int nCurValue = 0;

				if( nXIndexBuf % int( 2 * dZigZagWidth ) < dZigZagWidth )
				{
					if(nYIndexBuf > - (nXIndexBuf % int( 2 * dZigZagWidth )) * ( dZigZagHeight / dZigZagWidth ) + g_nMaskHeightBuf / 2 + dZigZagHeight / 2)
					{
						nCurValue += int(( nYIndexBuf - ( g_nMaskHeightBuf / 2 - dZigZagHeight / 2 )) * dTimeUnit);
						nCurValue -= int( ( dZigZagWidth - nXIndexBuf % int( 2 * dZigZagWidth ) ) * ( dZigZagHeight * dTimeUnit ) / dZigZagWidth );
						*pBuffer = nCurValue;
					}
					else
					{
						nCurValue += int(-( nYIndexBuf - ( g_nMaskHeightBuf / 2 - dZigZagHeight / 2 )) * dTimeUnit);
						nCurValue += int( ( dZigZagWidth - nXIndexBuf % int( 2 * dZigZagWidth ) ) * ( dZigZagHeight * dTimeUnit ) / dZigZagWidth );
					}
					*pBuffer = nCurValue;
					
				}
				else
				{
					if(nYIndexBuf > (nXIndexBuf % int( 2 * dZigZagWidth ) - dZigZagWidth) * ( dZigZagHeight / dZigZagWidth ) + g_nMaskHeightBuf / 2 - dZigZagHeight / 2)
					{
						nCurValue += int(( nYIndexBuf - ( g_nMaskHeightBuf / 2 + dZigZagHeight / 2 )) * dTimeUnit);
						nCurValue += int( ( 2 * dZigZagWidth -  nXIndexBuf % int( 2 * dZigZagWidth ) ) * ( dZigZagHeight * dTimeUnit ) / dZigZagWidth );
						*pBuffer = nCurValue;
					}
					else
					{
						nCurValue += int(-( nYIndexBuf - ( g_nMaskHeightBuf / 2 + dZigZagHeight / 2 )) * dTimeUnit);
						nCurValue -= int( ( 2 * dZigZagWidth - nXIndexBuf % int( 2 * dZigZagWidth ) ) * ( dZigZagHeight * dTimeUnit ) / dZigZagWidth );
					}
					*pBuffer = nCurValue;

				}
				pBuffer++;
			}
		}
	}

	void Diamond()
	{
		Ipp8u* pBuffer = g_pMask;
		int nIn = 1;
		if ( c_nWipeSmoothDiamondIn == g_nMaskType )
			nIn = 0;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				int nValue = abs( nXIndex - g_nMaskWidth / 2 ) * 255. / g_nMaskWidth + abs( nYIndex - g_nMaskHeight / 2 ) * 256 / g_nMaskHeight;
				*pBuffer = 255 * (1 - nIn) + (-1 + 2 * nIn) * nValue;
				pBuffer++;
			}
		}
	}
	void Diamonds()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				const int nHorDiamonds = 5;
				const int nVerDiamonds = 5;
				int nPartWidth = g_nMaskWidth / nHorDiamonds; 
				int nPartHeight = g_nMaskHeight / nVerDiamonds; 
			
				int nHorIndex = nXIndex / nPartWidth;
				int nVerIndex = nYIndex / nPartHeight;
				*pBuffer = abs( nXIndex - (nHorIndex + 0.5) * (nPartWidth) ) * 255. / nPartWidth + abs( nYIndex - (nVerIndex + 0.5) * (nPartHeight) ) * 256 / nPartHeight;
				pBuffer++;
			}
		}
	}
	void Circle()
	{
		Ipp8u* pBuffer = g_pMask;
		int nIn = 1;
		if ( c_nWipeSmoothCircleIn == g_nMaskType )
			nIn = 0;
		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				int nValue = sqrt(double(( nXIndex - g_nMaskWidth / 2 ) * ( nXIndex - g_nMaskWidth / 2 ) + ( nYIndex - g_nMaskHeight / 2 ) * ( nYIndex - g_nMaskHeight / 2 ))) * 255. * 2/ sqrt( double(g_nMaskHeight * g_nMaskHeight + g_nMaskWidth* g_nMaskWidth));
				*pBuffer = 255 * (1 - nIn) + (-1 + 2 * nIn) * nValue;
				pBuffer++;
			}
		}
	}

	void Circles()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				const int nHorCircles = 5;
				const int nVerCircles = 5;
				int nPartWidth = g_nMaskWidth / nHorCircles; 
				int nPartHeight = g_nMaskHeight / nVerCircles; 
				
				int nHorIndex = nXIndex / nPartWidth;
				int nVerIndex = nYIndex / nPartHeight;
				*pBuffer = sqrt(double(( nXIndex - (nHorIndex + 0.5) * nPartWidth ) * ( nXIndex - (nHorIndex + 0.5) * nPartWidth ) + ( nYIndex - (nVerIndex + 0.5) * nPartHeight ) * ( nYIndex - (nVerIndex + 0.5) * nPartHeight ))) * 255. * 2/ sqrt( double(nPartHeight * nPartHeight + nPartWidth * nPartWidth));
				pBuffer++;
			}
		}
	}


	void StarFlower()
	{
		int nType = g_nMaskType;
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				const int nStarsPoints = 5;
				const int nCountSectors = nStarsPoints * 2;
				const int nSector = 360 / nCountSectors;
				double dRad = 3.14159265 / 180;
				double dFactor = sin(dRad *  ( 90 - nSector)) / sin(dRad * ( 90 - 2 * nSector)); // отношение большого радиуса звузды к малому

				double dY = nYIndex - g_nMaskHeight / 2;
				double dX = nXIndex - g_nMaskWidth / 2;			
				double dCurRadius = _hypot(dX, dY);
				double dSecY = (dY >= 0 ? 1.0 : -1.0);
				int nAngle = (dCurRadius ? acos( dX / dCurRadius) * dSecY : 0) /dRad;
				nAngle = ( nAngle < 0? 360 + nAngle : nAngle);
				
				nAngle -= 90;
				nAngle = ( nAngle < 0? 360 + nAngle : nAngle);

				int nCurSector = nAngle / nSector;
				int nCurAngle = nAngle % nSector;
				
				double dSmallRadius;  // малый радиус звезды
				if ( nCurSector % 2 )
				{
					if(c_nWipeSmoothWideFlower == nType || c_nWipeSmoothNarrowFlower == nType)
						dSmallRadius = dCurRadius * nSector / (nSector + nCurAngle * (dFactor - 1)); 
					else
					{
						double dValue = 1 / tan(asin( ( dFactor * sin( dRad * (nSector - nCurAngle)) + sin(dRad * nCurAngle ) )/sqrt( dFactor * dFactor + 1 - 2 * dFactor * cos(dRad * nSector)) ));
						dSmallRadius = dCurRadius / (sin(dRad * nCurAngle) * dValue + cos(dRad * nCurAngle)); 
					}
				}
				else
				{
					if(c_nWipeSmoothWideFlower == nType || c_nWipeSmoothNarrowFlower == nType)
						dSmallRadius = dCurRadius * nSector / (dFactor * nSector + nCurAngle * (1 - dFactor)); 
					else
					{
						double dValue = 1 / tan(asin( ( dFactor * sin( dRad * (nCurAngle)) + sin(dRad * (nSector - nCurAngle) ) )/sqrt( dFactor * dFactor + 1 - 2 * dFactor * cos(dRad * nSector)) ));
						dSmallRadius = dCurRadius / (sin(dRad * (nSector - nCurAngle)) * dValue + cos(dRad * (nSector - nCurAngle))); 
					}
				}
				double dCurTime = 255. * dSmallRadius * 2 / sqrt( double (g_nMaskHeight * g_nMaskHeight + g_nMaskWidth * g_nMaskWidth));

				*pBuffer = int(dCurTime) % 255;
				pBuffer++;
			}
		}
	}

	void StarsFlowers()
	{
		int nType = g_nMaskType;
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				const int nHorStars = 5;
				const int nVerStars = 5;
				int nPartWidth = g_nMaskWidth / nHorStars; 
				int nPartHeight = g_nMaskHeight / nVerStars; 
				
				int nHorIndex = nXIndex / nPartWidth;
				int nVerIndex = nYIndex / nPartHeight;
				
				const int nStarsPoints = 5;
				const int nCountSectors = nStarsPoints * 2;
				const int nSector = 360 / nCountSectors;
				double dRad = 3.14159265 / 180;
				double dFactor = sin(dRad *  ( 90 - nSector)) / sin(dRad * ( 90 - 2 * nSector)); // отношение большого радиуса звузды к малому
				double dValue = 300;

				for(int nIndex = 0; nIndex < 2; nIndex++ )
				{
					double dY = nYIndex - (nVerIndex + 0.5) * nPartHeight;
					double dX = nXIndex - (nHorIndex + 0.5) * nPartWidth;			
					double dCurRadius = _hypot(dX, dY);
					double dSecY = (dY >= 0 ? 1.0 : -1.0);
					int nAngle = (dCurRadius ? acos( dX / dCurRadius) * dSecY : 0) /dRad;
					nAngle = ( nAngle < 0? 360 + nAngle : nAngle);
				
					nAngle -= 90;
					nAngle = ( nAngle < 0? 360 + nAngle : nAngle);

					int nCurSector = nAngle / nSector;
					int nCurAngle = nAngle % nSector;
					
					double dSmallRadius;  // малый радиус звезды
					if ( nCurSector % 2 )
					{
						if (c_nWipeSmoothWideFlowers == nType || c_nWipeSmoothNarrowFlowers == nType)
							dSmallRadius = dCurRadius * nSector / (nSector + nCurAngle * (dFactor - 1)); 
						else
						{
							double dValue = 1 / tan(asin( ( dFactor * sin( dRad * (nSector - nCurAngle)) + sin(dRad * nCurAngle ) )/sqrt( dFactor * dFactor + 1 - 2 * dFactor * cos(dRad * nSector)) ));
							dSmallRadius = dCurRadius / (sin(dRad * nCurAngle) * dValue + cos(dRad * nCurAngle)); 
						}
					}
					else
					{
						if (c_nWipeSmoothWideFlowers == nType || c_nWipeSmoothNarrowFlowers == nType)
							dSmallRadius = dCurRadius * nSector / (dFactor * nSector + nCurAngle * (1 - dFactor)); 
						else
						{
							double dValue = 1 / tan(asin( ( dFactor * sin( dRad * (nCurAngle)) + sin(dRad * (nSector - nCurAngle) ) )/sqrt( dFactor * dFactor + 1 - 2 * dFactor * cos(dRad * nSector)) ));
							dSmallRadius = dCurRadius / (sin(dRad * (nSector - nCurAngle)) * dValue + cos(dRad * (nSector - nCurAngle))); 
						}
					}
					double dCurTime = 255. * dSmallRadius * 2 / sqrt( double (nPartHeight * nPartHeight + nPartWidth * nPartWidth));
					
					dValue = (dCurTime < dValue? dCurTime : dValue);
					
					if (nIndex == 0)
					{
						if ( nYIndex % nPartHeight > nPartHeight / 2)
							nVerIndex++;
						else
							nVerIndex--;
						if ( nVerIndex < 0 || nVerIndex >= nVerStars )
							nIndex = 2;
					}
				}
				*pBuffer = int(dValue) % 255;
				pBuffer++;
			}
		}
	}

	void KeyHoles()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				const int nHorHoles = 5;
				const int nVerHoles = 5;
				int nPartWidth = g_nMaskWidth / nHorHoles; 
				int nPartHeight = g_nMaskHeight / nVerHoles; 
				
				int nHorIndex = nXIndex / nPartWidth;
				int nVerIndex = nYIndex / nPartHeight;
				
				double dResValue = 300;

				for(int nIndex =0; nIndex < 2; nIndex++)
				{
					int nX = nXIndex - nHorIndex * nPartWidth;
					int nY = nYIndex - nVerIndex * nPartHeight;
					int nValue;
					int nValue1;
					double dUnit = 255. * 8. / (nPartWidth * 3. );
					if ( nY - 2. * nPartHeight / 3. <= - abs(8. * ( nX - nPartWidth / 2. ) / 3.))
					{
						nValue = (2. * nPartHeight / 3. - nY) * dUnit / 6.;
					}
					else
					{
						int nX1 = nX - nPartWidth / 2.;
						int nY1 = nY - 2. * nPartHeight / 3.;
						int nCurY = nY1 + 12. * abs(nX1) / 3.;
						nValue = nCurY * dUnit / 3.;
					}
					nValue1 = sqrt(double(( nX - nPartWidth / 2. ) * ( nX - nPartWidth / 2. ) + ( nY - 2 * nPartHeight / 3. ) * ( nY - 2. * nPartHeight / 3. ))) * dUnit / 2.;
					if ( nValue1 < nValue || 2. * nPartHeight / 3. < nY)
						nValue = nValue1;

					double dValue = sqrt( nValue / 255.) * 255;
					
					dResValue = (dValue < dResValue? dValue : dResValue);
					
					if (nIndex == 0)
					{
						if ( nYIndex % nPartHeight > nPartHeight / 2)
							nVerIndex++;
						else
							nVerIndex--;
						if ( nVerIndex < 0 || nVerIndex >= nVerHoles )
							nIndex = 2;
					}

				}
				*pBuffer = dResValue ;
				pBuffer++;
			}
		}
	}

	void KeyHole()
	{
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				int nValue;
				int nValue1;
				double dUnit = 255. * 8 / (g_nMaskWidth * 3 );
				if ( nYIndex - 2 * g_nMaskHeight / 3 <= - abs(8. * ( nXIndex - g_nMaskWidth / 2 ) / 3.))
				{
					nValue = (2 * g_nMaskHeight / 3 - nYIndex) * dUnit / 6;
				}
				else
				{
					int nX = nXIndex - g_nMaskWidth / 2;
					int nY = nYIndex - 2 * g_nMaskHeight / 3;
					int nCurY = nY + 12 * abs(nX) / 3;
					nValue = nCurY * dUnit / 3;
				}
				nValue1 = sqrt(double(( nXIndex - g_nMaskWidth / 2 ) * ( nXIndex - g_nMaskWidth / 2 ) + ( nYIndex - 2 * g_nMaskHeight / 3 ) * ( nYIndex - 2 * g_nMaskHeight / 3 ))) * dUnit / 2;
				if ( nValue1 < nValue || 2 * g_nMaskHeight / 3 < nYIndex)
					nValue = nValue1;

				double dValue = sqrt(nValue / 255.);
				*pBuffer = dValue * 255;
				pBuffer++;
			}
		}
	}

	void CircleCorner()
	{
		int nType = g_nMaskType;
		Ipp8u* pBuffer = g_pMask;

		for ( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				double dMax = sqrt(double( g_nMaskWidth * g_nMaskWidth + g_nMaskHeight * g_nMaskHeight ));
				if( c_nCircleCornerLeftBottom == nType || c_nCircleCornerSmoothLeftBottom == nType )
					*pBuffer = 255. * sqrt (double( nXIndex * nXIndex + nYIndex * nYIndex )) / dMax ;
				else if( c_nCircleCornerLeftTop == nType || c_nCircleCornerSmoothLeftTop == nType )
					*pBuffer = 255. * sqrt (double( nXIndex * nXIndex + (nYIndex - g_nMaskHeight) * (nYIndex - g_nMaskHeight) )) / dMax;
				else if( c_nCircleCornerRightBottom == nType || c_nCircleCornerSmoothRightBottom == nType )
					*pBuffer = 255. * sqrt (double( (nXIndex - g_nMaskWidth) * (nXIndex - g_nMaskWidth) + nYIndex * nYIndex )) / dMax;
				else if( c_nCircleCornerRightTop == nType || c_nCircleCornerSmoothRightTop == nType )
					*pBuffer = 255. * sqrt (double( (nXIndex - g_nMaskWidth) * (nXIndex - g_nMaskWidth) + (nYIndex - g_nMaskHeight) * (nYIndex - g_nMaskHeight)) ) / dMax;
				pBuffer++;
			}
		}
	}


	void BarsHorizontal()
	{
		Ipp8u* pBuffer = g_pMask;
		::srand( ::GetTickCount() );
		int* nRow = new int[g_nMaskHeight];
		for (int nIndex = 0; nIndex < g_nMaskHeight; nIndex++ )
		{
			nRow[nIndex] = rand()%255;
		}
		for( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				*pBuffer = nRow[nYIndex];
				pBuffer++;
			}
		}
		delete nRow;
	}

	void BarsVertical()
	{
		Ipp8u* pBuffer = g_pMask;
		::srand( ::GetTickCount() );
		int* nCol = new int[g_nMaskWidth];
		for (int nIndex = 0; nIndex < g_nMaskWidth; nIndex++ )
		{
			nCol[nIndex] = rand()%255;
		}
		for( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				*pBuffer = nCol[nXIndex];
				pBuffer++;
			}
		}
		delete nCol;
	}


	void Box()
	{
		Ipp8u* pBuffer = g_pMask;

		int nIn = 1;
		if ( c_nWipeSmoothBoxIn == g_nMaskType )
			nIn = 0;
		
		double dXFactor = 511. / g_nMaskWidth ;
		double dYFactor = 511. / g_nMaskHeight;

		for( int nYIndex = 0; nYIndex < g_nMaskHeight; nYIndex++ )
		{
			for( int nXIndex = 0; nXIndex < g_nMaskWidth; nXIndex++ )
			{
				int nValue = max( dXFactor * abs(double(nXIndex - g_nMaskWidth / 2)), dYFactor * abs(double(nYIndex - g_nMaskHeight / 2)));
				*pBuffer = 255 * (1 - nIn) + (-1 + 2 * nIn) * nValue;
				pBuffer++;
			}
		}
	}

public:

	int	g_nMaskWidth;
	int	g_nMaskHeight;
	
	Ipp8u* g_pMask;
	int g_nMaskType;

};

class PuzzleFigure
{
public:

	PuzzleFigure()
	{
		m_nFigHeight = 0;
		m_nFigWidth  = 0;
		
		m_nPicHeight = 0;
		m_nPicWidth  = 0;

	    m_nLeftFactor   = 0; 
	    m_nRightFactor  = 0;					
		m_nTopFactor    = 0;					
		m_nBottomFactor = 0;

		m_nXCoorSource = 0;
		m_nYCoorSource = 0;
	}
	PuzzleFigure(int nFigWidth, int nFigHeight, int nTopFactor, int nRightFactor, int nBottomFactor, int nLeftFactor, BYTE* pBGRASource, int nPicWidth, int nPicHeight, int nXCoorSource, int nYCoorSource)
	{
		m_nFigHeight = nFigHeight;
		m_nFigWidth  = nFigWidth;

	    m_nLeftFactor   = nLeftFactor;
	    m_nRightFactor  = nRightFactor;
	    m_nTopFactor    = nTopFactor;
		m_nBottomFactor = nBottomFactor;
		
		m_nPicHeight = nPicHeight;
		m_nPicWidth  = nPicWidth;
		
		m_pBGRASource = pBGRASource;
		
		m_nXCoorSource = nXCoorSource;
		m_nYCoorSource = nYCoorSource;
	}
	~PuzzleFigure()
	{
	}

private:

	void DrawAlphaBlendingLine(BYTE* pBGRAResult, BYTE* pBGRASource, int nColor, int nNumLine, double dEffect)
	{
		BYTE *pResult = pBGRAResult;
		BYTE *pSource = pBGRASource;
		int nAlpha = 7;
		for(int k = 0; k < nNumLine; k++)
		{
			nAlpha *= 2;
		}
		if(1 == dEffect)
		{
			*pResult = (nColor*nAlpha + *pSource *(255-nAlpha))/255; pResult++; pSource ++;
			*pResult = (nColor*nAlpha + *pSource *(255-nAlpha))/255; pResult++; pSource ++;
			*pResult = (nColor*nAlpha + *pSource *(255-nAlpha))/255; pResult++; pSource ++;
		}
		else
		{
			*pResult = dEffect*(nColor*nAlpha + *pSource *(255-nAlpha))/255 + *pSource*(1-dEffect); pResult++; pSource ++;
			*pResult = dEffect*(nColor*nAlpha + *pSource *(255-nAlpha))/255 + *pSource*(1-dEffect); pResult++; pSource ++;
			*pResult = dEffect*(nColor*nAlpha + *pSource *(255-nAlpha))/255 + *pSource*(1-dEffect); pResult++; pSource ++;

		}
		*pResult = 255;
		pResult++; pSource++;
	}


public:

	void SetValue(int nFigWidth, int nFigHeight, int nTopFactor, int nRightFactor, int nBottomFactor, int nLeftFactor, BYTE* pBGRASource, int nPicWidth, int nPicHeight, int nXCoorSource, int nYCoorSource)
	{
		m_nFigHeight = nFigHeight;
		m_nFigWidth  = nFigWidth;

		m_nLeftFactor   = nLeftFactor;
	    m_nRightFactor  = nRightFactor;
	    m_nTopFactor    = nTopFactor;
		m_nBottomFactor = nBottomFactor;

		m_nPicHeight = nPicHeight;
		m_nPicWidth  = nPicWidth;

		m_pBGRASource = pBGRASource;

		m_nXCoorSource = nXCoorSource;
		m_nYCoorSource = nYCoorSource;

	}

	BOOL Draw(Gdiplus::Graphics *oMainRender, Gdiplus::TextureBrush *oBrush, double dXCoorResult, double dYCoorResult, int nWidth, int nHeight, double dEffect)
	{
		if(!oMainRender || !oBrush || nWidth != m_nPicWidth || nHeight != m_nPicHeight )
			return FALSE;
		
		int nSize = 4 * nHeight * nWidth;

		
		double dDiagonal = min(m_nFigHeight, m_nFigWidth);
		
		double dUnit = 1 / (72.0) * dDiagonal;
		double dCircleWidth = 26 * dUnit;                  // ширина кружочка, т.е. 2 * радиус - (зазор со средней частью)
		double dRadius      = 15 * dUnit;                  // радиус кружочка
		double dSqrRadius   = dRadius * dRadius; 
		double dDiff = dCircleWidth - dRadius; 

		double dInStep      = 37 * dUnit;

		double dStep = 11 * dUnit;
		double dSrez  = sqrt(dSqrRadius - dStep * dStep);
		double dAngle = acos(11 * dUnit / dRadius) * 180 / IPP_PI;

		double dXSur = m_nXCoorSource;
		double dYSur = m_nYCoorSource;

		double dFullHeight = m_nFigHeight + 2 * dCircleWidth; // полная высота кусочка, с учетом кружков
		double dFullWidth  = m_nFigWidth  + 2 * dCircleWidth; // полная ширина кусочка

		double dXResult = dXCoorResult - dCircleWidth;
		double dYResult = dYCoorResult - dCircleWidth;


		Gdiplus::GraphicsPath oPathAddEllipse;
		if ( 1 == m_nLeftFactor )
			oPathAddEllipse.AddEllipse( float(dXSur - dCircleWidth),                             nHeight - float(dYSur + m_nFigHeight / 2 + dRadius),  float(2 * dRadius), float(2 * dRadius)); // left
		if ( 1 == m_nRightFactor )
			oPathAddEllipse.AddEllipse( float(dXSur + dCircleWidth + m_nFigWidth - 2 * dRadius), nHeight - float(dYSur + m_nFigHeight / 2 + dRadius),  float(2 * dRadius), float(2 * dRadius)); //right
		if ( 1 == m_nBottomFactor )
			oPathAddEllipse.AddEllipse( float(dXSur + m_nFigWidth / 2 - dRadius),                nHeight - float(dYSur + m_nFigHeight + dCircleWidth), float(2 * dRadius), float(2 * dRadius)); //bottom
		if ( 1 == m_nTopFactor )
			oPathAddEllipse.AddEllipse( float(dXSur + m_nFigWidth / 2 - dRadius),                nHeight - float(dYSur - dCircleWidth + 2 * dRadius),  float(2 * dRadius), float(2 * dRadius)); //top
		oPathAddEllipse.CloseFigure();

		Gdiplus::GraphicsPath oPathExclEllipse;
		if ( -1 == m_nLeftFactor )
			oPathExclEllipse.AddEllipse( float(dXSur - dCircleWidth + 2 * dDiff),                             nHeight - float(dYSur + m_nFigHeight / 2 + dRadius),             float(2 * dRadius), float(2 * dRadius)); // left
		if ( -1 == m_nRightFactor )
			oPathExclEllipse.AddEllipse( float(dXSur + dCircleWidth + m_nFigWidth - 2 * dRadius - 2 * dDiff), nHeight - float(dYSur + m_nFigHeight / 2 + dRadius),             float(2 * dRadius), float(2 * dRadius)); //right
		if ( -1 == m_nBottomFactor )
			oPathExclEllipse.AddEllipse( float(dXSur + m_nFigWidth / 2 - dRadius),                            nHeight - float(dYSur + m_nFigHeight + dCircleWidth - 2 * dDiff),float(2 * dRadius), float(2 * dRadius)); //bottom
		if ( -1 == m_nTopFactor )
			oPathExclEllipse.AddEllipse( float(dXSur + m_nFigWidth / 2 - dRadius),                            nHeight - float(dYSur - dCircleWidth + 2 * dRadius + 2 * dDiff), float(2 * dRadius), float(2 * dRadius)); //top
		oPathExclEllipse.CloseFigure();

		Gdiplus::Region oRegion(&oPathAddEllipse);

		oRegion.Union(Gdiplus::RectF(dXSur, nHeight - (dYSur + m_nFigHeight), m_nFigWidth, m_nFigHeight));
		oRegion.Exclude(&oPathExclEllipse);	
		// сдвигаем регион и кисть, так чтобы они двигались вместе
		oBrush->TranslateTransform(-float(m_nXCoorSource - dXCoorResult), float(m_nYCoorSource - dYCoorResult), MatrixOrderAppend);
		oRegion.Translate(-float(m_nXCoorSource - dXCoorResult), float(m_nYCoorSource - dYCoorResult));
		
		oMainRender->FillRegion(oBrush, &oRegion);
		

		
		int nAlphaBlack = 448 * dEffect;
		int nAlphaWhite = 448 * dEffect;
		
		for (int nIndex = 0; nIndex < 2; nIndex++ )
		{
			nAlphaBlack /= 2; 
			//if (nIndex == 1)
			//{
			//	nAlphaBlack = 224 * dEffect;
			//	nAlphaWhite = 224 * dEffect;
			//}
			Gdiplus::Pen oPen( Color( nAlphaBlack, 0, 0, 0) , 1);
			if ( 0 == m_nRightFactor )
				oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
			else
			{
				if ( 1 == m_nRightFactor)
					oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth - 2 * dRadius + nIndex), nHeight - float(dYResult + dFullHeight / 2 + dRadius - nIndex), float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), 180 + dAngle, 360 - 2 * dAngle);
				else if ( -1 == m_nRightFactor)
					oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth - 2 * dRadius - nIndex - 2 * dStep), nHeight - float(dYResult + dFullHeight / 2 + dRadius + nIndex), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
				oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight / 2 + dSrez +nIndex));
				oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight / 2 - dSrez - nIndex),    float(dXResult + dFullWidth  - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
			}

			if ( 0 == m_nTopFactor )
				oMainRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
			else
			{
				if ( 1 == m_nTopFactor)
					oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius + nIndex), nHeight - float(dYResult + 2 * dRadius - nIndex), float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), 270 + dAngle, 360 - 2 * dAngle);
				else if ( -1 == m_nTopFactor)
					oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius - nIndex), nHeight - float(dYResult + 2 * dRadius + nIndex + 2 * dStep), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), 90 + dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
				oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dFullWidth / 2 + dSrez + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex));
				oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth / 2 - dSrez - nIndex),    nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dCircleWidth + nIndex),           nHeight - float(dYResult + dCircleWidth + nIndex));
			}

			nAlphaWhite /= 2;
			oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));

			if ( 0 == m_nLeftFactor )
				oMainRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex), float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
			else
			{
				if ( 1 == m_nLeftFactor)
					oMainRender->DrawArc(&oPen, float(dXResult + nIndex), nHeight - float(dYResult + dFullHeight / 2 + dRadius - nIndex), float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), dAngle, 360 - 2 * dAngle);
				else if ( -1 == m_nLeftFactor)
					oMainRender->DrawArc(&oPen, float(dXResult - nIndex + 2 * dStep), nHeight - float(dYResult + dFullHeight / 2 + dRadius + nIndex), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), 180 + dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
				oMainRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dCircleWidth + nIndex),            float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight / 2 - dSrez - nIndex));
				oMainRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight / 2 + dSrez + nIndex), float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
			}

			if ( 0 == m_nBottomFactor )
				oMainRender->DrawLine(&oPen, float(dXResult + dCircleWidth + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
			else
			{
				if ( 1 == m_nBottomFactor)
					oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius + nIndex), nHeight - float(dYResult + dFullHeight - nIndex), float(2 * (dRadius - nIndex)), float(2 * (dRadius - nIndex)), 90 + dAngle, 360 - 2 * dAngle);
				else if ( -1 == m_nBottomFactor)
					oMainRender->DrawArc(&oPen, float(dXResult + dFullWidth / 2 - dRadius - nIndex), nHeight - float(dYResult + dFullHeight + nIndex - 2 * dStep), float(2 * (dRadius + nIndex)), float(2 * (dRadius + nIndex)), 270 + dAngle + nIndex, 360 - 2 * dAngle - 2* nIndex);
				oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth - dCircleWidth - nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dFullWidth / 2 + dSrez + nIndex), nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
				oMainRender->DrawLine(&oPen, float(dXResult + dFullWidth / 2 - dSrez - nIndex),    nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex), float(dXResult + dCircleWidth + nIndex),           nHeight - float(dYResult + dFullHeight - dCircleWidth - nIndex));
			}


		}

		// сдвигаем регион и кисть в исходное положение
		oRegion.Translate(float(m_nXCoorSource - dXCoorResult),  - float(m_nYCoorSource - dYCoorResult));
		oBrush->TranslateTransform(float(m_nXCoorSource - dXCoorResult),  - float(m_nYCoorSource - dYCoorResult), MatrixOrderAppend);

		return TRUE;
	}



public:

	int m_nFigHeight;
	int m_nFigWidth;
	int m_nPicHeight;
	int m_nPicWidth;

	int m_nLeftFactor;   // -1, 0, +1 
	int m_nRightFactor;	 // -1 : вогнутая граница
	int m_nTopFactor;    //  0 : ровная граница
	int m_nBottomFactor; // +1 : выпуклая граница

	BYTE* m_pBGRASource;
	int m_nXCoorSource;		// координаты кусочка, который мы вырезаем из картинки
	int m_nYCoorSource;		//

	
};


class PuzzleUtility
{
public:
	
	PuzzleUtility()
	{
		g_pShuffle = 0;
		g_nShuffleCount = -1;

		g_pCorners = 0;
		g_nCornersCount = -1;

	    g_pHorizontalFactor = 0;
	    g_pVerticalFactor   = 0;
	    g_nRow = -1;
	    g_nCol = -1;
		g_nTypeShuffle = -1;
		g_nTypeCorners = -1;
		g_nTypeGenerate = -1;
	}
	~PuzzleUtility()
	{
		if ( g_pShuffle )
			delete []g_pShuffle;

		if ( g_pCorners )
			delete []g_pCorners;

		if ( g_pHorizontalFactor )
			delete []g_pHorizontalFactor;
		if ( g_pVerticalFactor )
			delete []g_pVerticalFactor;
	}
	
	
	void GenerateRandomShuffle(int nXCount, int nYCount, int nShuffle, int nPuzzleType)
	{
		int nCount = nXCount * nYCount;
		if ( g_pShuffle && nCount == g_nShuffleCount && nPuzzleType == g_nTypeShuffle)
			return;

		if ( g_pShuffle )
			delete []g_pShuffle;

		g_nTypeShuffle = nPuzzleType;
		g_nShuffleCount = nCount;
		g_pShuffle = new int[nCount];

		for(int nIndex = 0; nIndex < nCount; nIndex++)
			g_pShuffle[nIndex] = nIndex;

		if ( 0 == nShuffle )
			return;
		else if ( 1 == nShuffle )
		{
			::srand(::GetTickCount());
			std::random_shuffle(g_pShuffle, g_pShuffle + nCount);
			return;
		}
		else if ( 2 == nShuffle )
		{
			for (int nIndex = 0; nIndex < nCount; nIndex++ )
			{
				g_pShuffle[nIndex] = nCount - 1 - nIndex;
			}
			return;
		}
		else if ( 3 == nShuffle )
		{
			int nYLevel = 0;
			for (int nYIndex = 0; nYIndex < nYCount; nYIndex++, nYLevel = (nYLevel + 1) % 2 )
			{					
				for (int nXIndex = 0; nXIndex < nXCount; nXIndex++ )
				{
					int nIndex = nYIndex * nXCount + nXIndex;
					if ( 0 == nYLevel )
						g_pShuffle[nIndex] = nCount - 1 - nIndex;
					else
						g_pShuffle[nIndex] = nCount - 1 - (nYIndex * nXCount + nXCount - 1 - nXIndex);
				}
			}
		}
		else if ( 4 == nShuffle )
		{
			for (int nYIndex = 0; nYIndex < nYCount; nYIndex++ )
			{					
				for (int nXIndex = 0; nXIndex < nXCount; nXIndex++ )
				{
					int nIndex = nYIndex * nXCount + nXIndex;
					if ( nXIndex % 2 == 0 )
					{
						int nCurX = nXIndex / 2;
						g_pShuffle[nIndex] = nCount - 1 - nYIndex * nXCount - nCurX;
					}
					else
					{
						int nCurX = (nXIndex + 1) / 2;
						g_pShuffle[nIndex] = nCount - 1 - (nYIndex * nXCount  + nXCount - 1 - nCurX);
					}
				}
			}
		}
		return;

	}
	void GenerateCorners(int nWidth, int nHeight, int nCount, int nType, int nPuzzleType)
	{
		if ( g_pCorners && nCount == g_nCornersCount && nPuzzleType == g_nTypeCorners && nHeight == g_nPicHeight && nWidth == g_nPicWidth )
			return;

		if ( g_pCorners )
			delete []g_pCorners;

		g_nPicWidth = nWidth;
		g_nPicHeight = nHeight;

		g_nTypeCorners = nPuzzleType;
		g_nCornersCount = nCount;

		g_pCorners = new int[nCount];
		if ( (0 <= nType  && nType <= 3) || 5 == nType  || 6 == nType || 7 == nType)
		{
			::srand(::GetTickCount());
			for ( int nIndex = 0; nIndex < nCount; nIndex++)
				g_pCorners[nIndex] = nType;
		}
		else if ( 4 == nType )
		{
			::srand(::GetTickCount());
			for ( int nIndex = 0; nIndex < nCount; nIndex++)
				g_pCorners[nIndex] = rand() % 4;
		}
		return;

	}
	void GeneratePuzzle(int nRow, int nCol, int nPuzzleType)
	{
		if ( g_pHorizontalFactor && g_pVerticalFactor && nRow == g_nRow && nCol == g_nCol && nPuzzleType == g_nTypeGenerate)
			return;

		if ( g_pHorizontalFactor )
			delete []g_pHorizontalFactor;
		if ( g_pVerticalFactor )
			delete []g_pVerticalFactor;

		int nSize = nCol * nRow * 2;

		g_nTypeGenerate = nPuzzleType;
		g_nRow = nRow;
		g_nCol = nCol;

		g_pHorizontalFactor = new int[nSize];
		g_pVerticalFactor = new int[nSize];

		::srand(GetTickCount());
		for ( int nYIndex = 0, nIndex = 0; nYIndex < nRow; nYIndex++ )
		{
			for ( int nXIndex = 0; nXIndex < nCol; nXIndex++, nIndex += 2 )
			{
				if ( 1 == nRow )
				{
					g_pHorizontalFactor[nIndex + 0] = 0;
					g_pHorizontalFactor[nIndex + 1] = 0;
					continue;
				}
				if ( 0 == nYIndex )
				{
					g_pHorizontalFactor[nIndex + 0] = 0;
					g_pHorizontalFactor[nIndex + 1] = -1 + 2 * (rand() % 2);
				}
				else if ( nRow - 1 == nYIndex )
				{
					g_pHorizontalFactor[nIndex + 1] = 0;
					g_pHorizontalFactor[nIndex + 0] = - g_pHorizontalFactor[nIndex - nCol * 2 + 1];
				}
				else
				{
					g_pHorizontalFactor[nIndex + 1] = -1 + 2 * (rand() % 2);
					g_pHorizontalFactor[nIndex + 0] = - g_pHorizontalFactor[nIndex - nCol * 2 + 1];
				}
				
				if ( 1 == nCol )
				{
					g_pVerticalFactor[nIndex + 0] = 0;
					g_pVerticalFactor[nIndex + 1] = 0;
					continue;
				}
				if ( 0 == nXIndex )
				{
					g_pVerticalFactor[nIndex + 0] = 0;
					g_pVerticalFactor[nIndex + 1] = -1 + 2 * (rand() % 2);
				}
				else if ( nCol - 1 == nXIndex )
				{
					g_pVerticalFactor[nIndex + 1] = 0;
					g_pVerticalFactor[nIndex + 0] = - g_pVerticalFactor[nIndex - 1];
				}
				else
				{
					g_pVerticalFactor[nIndex + 1] = -1 + 2 * (rand() % 2);
					g_pVerticalFactor[nIndex + 0] = - g_pVerticalFactor[nIndex - 1];
				}

			}
		}
		return;
	}



	int GetStartX(int nFigWidth, int nFigHeight, int nXResult, int nYResult, int nIndex, int nCurX)
	{
		int nXStart = 0;

		switch(nIndex)
		{
		case 0: nXStart =             - nFigWidth; break;
		case 1: nXStart = g_nPicWidth + nFigWidth; break; 
		case 2: nXStart = g_nPicWidth + nFigWidth; break; 
		case 3: nXStart =             - nFigWidth; break; 
		case 5: nXStart = nXResult;                break;
		case 6: nXStart = nXResult;                break;
		case 7: 
			{
				if (nCurX % 2)
					nXStart = -nFigWidth;
				else
					nXStart = g_nPicWidth + nFigWidth;
			}
		}

		return nXStart;
	}

	int GetStartY(int nFigWidth, int nFigHeight, int nXResult, int nYResult, int nIndex)
	{
		int nYStart = 0;

		switch(nIndex)
		{
		case 0: nYStart =              - nFigHeight; break;
		case 1: nYStart =              - nFigHeight; break; 
		case 2: nYStart = g_nPicHeight + nFigHeight; break; 
		case 3: nYStart = g_nPicHeight + nFigHeight; break; 
		case 5: nYStart =              - nFigHeight; break;
		case 6: nYStart = g_nPicHeight + nFigHeight; break;
		case 7: nYStart =              - nFigHeight; break;
		}

		return nYStart;
	}
public:
					
	//для перемешивания кусочков пазла
	int *g_pShuffle;
	int g_nShuffleCount;

	//список углов из которых вылетают кусочки
	int *g_pCorners;
	int g_nCornersCount;

	//списки, определяющие как сами кусочки выглядят
	int *g_pHorizontalFactor;
	int *g_pVerticalFactor;
	int g_nRow;
	int g_nCol;
	int g_nTypeShuffle;
	int g_nTypeCorners;
	int g_nTypeGenerate;

protected:

	int g_nPicWidth;
	int g_nPicHeight;


};
class Figure
{
public:

	Figure()
	{
		m_dBigRadius   = -1;

		m_nPicHeight = -1;
		m_nPicWidth  = -1;

		m_pBGRASource = 0;

		m_dXCoorSource = -1;
		m_dYCoorSource = -1;
		m_arrPoints = 0;

		m_nFigureType = -1;
		m_nPointsCount = -1;
	}
	~Figure()
	{
		if ( m_arrPoints )
			delete[] m_arrPoints;
	}

public:

	void SetValue(int nFigureType, double dSide, BYTE* pBGRASource, int nPicWidth, int nPicHeight, double dXCoorSource, double dYCoorSource)
	{

		m_nPicHeight = nPicHeight;
		m_nPicWidth  = nPicWidth;

		m_pBGRASource = pBGRASource;

		m_dXCoorSource = dXCoorSource;
		m_dYCoorSource = dYCoorSource;

		m_arrPoints = 0;
		m_nPointsCount = -1;

		m_nFigureType = nFigureType;

		// далее заполняем дополнительные данные
		if ( 6 == m_nFigureType )
		{
			m_dBigRadius   = dSide;
			m_dShiftX = m_dBigRadius / 2;
			m_dShiftY = sqrt(3.0) * m_dBigRadius / 2;
		}
		else if ( 30 == m_nFigureType || 31 == m_nFigureType)
		{
			m_dBigRadius = sqrt(3.0) * dSide / 3; // радиус описанной окружности
			m_dShiftY = m_dBigRadius / 2;         // радиус вписанной окружности
			m_dShiftX = dSide / 2;
		}
		else if ( 41 == m_nFigureType )
		{
			m_dBigRadius = dSide / sqrt(2.0);
		}
		else if ( 42 == m_nFigureType )
		{
			m_dShiftX = dSide / 2;
			m_dShiftY = dSide * ( 12.0 / 25.0 ) / 2;
		}
		else if ( 43 == m_nFigureType )
		{
			m_dShiftX = dSide / 2;
			m_dShiftY = dSide * ( 12.0 / 25.0 ) / 2;
		}
	}

	BOOL Draw(Gdiplus::Graphics *oMainRender, Gdiplus::TextureBrush *oBrush, double dXCoorResult, double dYCoorResult, int nWidth, int nHeight, double dEffect)
	{
		if(!oMainRender || !oBrush || nWidth != m_nPicWidth || nHeight != m_nPicHeight)
			return FALSE;
		
		int nSize = 4 * nHeight * nWidth;

		CreatePolygon(m_nFigureType, m_dXCoorSource, m_dYCoorSource);
			
		Gdiplus::GraphicsPath oPath;
		oPath.AddPolygon(m_arrPoints, m_nPointsCount);
		oPath.CloseFigure();

		Gdiplus::Region oRegion(&oPath);
		

		// сдвигаем регион и кисть, так чтобы они двигались вместе
		oBrush->TranslateTransform(-float(m_dXCoorSource - dXCoorResult), -float(m_dYCoorSource - dYCoorResult), MatrixOrderAppend);
		oRegion.Translate(-float(m_dXCoorSource - dXCoorResult), -float(m_dYCoorSource - dYCoorResult));
		
		oMainRender->FillRegion(oBrush, &oRegion);

		if (6 == m_nFigureType)
			DrawHexagonBorder(oMainRender, dXCoorResult, dYCoorResult, dEffect);
		else if ( 30 == m_nFigureType || 31 == m_nFigureType )
			DrawTriangleBorder(oMainRender, dXCoorResult, dYCoorResult, dEffect);
		else if ( 41 == m_nFigureType )
			DrawRhombusBorder(oMainRender, dXCoorResult, dYCoorResult, dEffect);
		else if ( 42 == m_nFigureType )
			DrawBrickBorder(oMainRender, dXCoorResult, dYCoorResult, dEffect);
		else if ( 43 == m_nFigureType )
			DrawInclinedBrickBorder(oMainRender, dXCoorResult, dYCoorResult, dEffect);

		// сдвигаем регион и кисть в исходное положение
		oRegion.Translate(float(m_dXCoorSource - dXCoorResult), float(m_dYCoorSource - dYCoorResult));
		oBrush->TranslateTransform(float(m_dXCoorSource - dXCoorResult), float(m_dYCoorSource - dYCoorResult), MatrixOrderAppend);

		return TRUE;
	}

protected:

	BOOL CreatePolygon(int nNumber, double dX, double dY)
	{
		if (m_arrPoints)
			delete []m_arrPoints;
		if (6 == nNumber)
		{
			m_nPointsCount = 6;
			m_arrPoints = new Gdiplus::PointF[6];
			int nRadius = m_dBigRadius;
			double dXShift = m_dShiftX;
			double dYShift = m_dShiftY;

			m_arrPoints[0].X = dX - dXShift;
			m_arrPoints[0].Y = dY + dYShift;
			m_arrPoints[1].X = dX + dXShift;
			m_arrPoints[1].Y = dY + dYShift;
			m_arrPoints[2].X = dX + nRadius;
			m_arrPoints[2].Y = dY;
			m_arrPoints[3].X = dX + dXShift;
			m_arrPoints[3].Y = dY - dYShift;

			m_arrPoints[4].X = dX - dXShift;
			m_arrPoints[4].Y = dY - dYShift;
			m_arrPoints[5].X = dX - nRadius;
			m_arrPoints[5].Y = dY;

		}

		else if ( 30 == nNumber )
		{
			m_nPointsCount = 3;
			m_arrPoints = new Gdiplus::PointF[3];
			m_arrPoints[0].X = dX - m_dShiftX;
			m_arrPoints[0].Y = dY + m_dShiftY;

			m_arrPoints[1].X = dX;
			m_arrPoints[1].Y = dY - m_dBigRadius;
			
			m_arrPoints[2].X = dX + m_dShiftX;
			m_arrPoints[2].Y = dY + m_dShiftY;
		}
		else if ( 31 == nNumber )
		{
			m_nPointsCount = 3;
			m_arrPoints = new Gdiplus::PointF[3];
			m_arrPoints[0].X = dX;
			m_arrPoints[0].Y = dY + m_dBigRadius;

			m_arrPoints[1].X = dX - m_dShiftX;
			m_arrPoints[1].Y = dY - m_dShiftY;
			
			m_arrPoints[2].X = dX + m_dShiftX;
			m_arrPoints[2].Y = dY - m_dShiftY;
		}
		else if ( 41 == m_nFigureType )
		{
			m_nPointsCount = 4;
			m_arrPoints = new Gdiplus::PointF[4];

			m_arrPoints[0].X = dX - m_dBigRadius;
			m_arrPoints[0].Y = dY;

			m_arrPoints[1].X = dX;
			m_arrPoints[1].Y = dY + m_dBigRadius;
			
			m_arrPoints[2].X = dX + m_dBigRadius;
			m_arrPoints[2].Y = dY;
			
			m_arrPoints[3].X = dX;
			m_arrPoints[3].Y = dY - m_dBigRadius;
		}
		else if ( 42 == m_nFigureType )
		{
			m_nPointsCount = 4;
			m_arrPoints = new Gdiplus::PointF[4];

			m_arrPoints[0].X = dX - m_dShiftX;
			m_arrPoints[0].Y = dY - m_dShiftY;

			m_arrPoints[1].X = dX + m_dShiftX;
			m_arrPoints[1].Y = dY - m_dShiftY;
			
			m_arrPoints[2].X = dX + m_dShiftX;
			m_arrPoints[2].Y = dY + m_dShiftY;
			
			m_arrPoints[3].X = dX - m_dShiftX;
			m_arrPoints[3].Y = dY + m_dShiftY;
		}
		else if ( 43 == m_nFigureType )
		{
			m_nPointsCount = 4;
			m_arrPoints = new Gdiplus::PointF[4];

			double dCos = cos(-IPP_PI4);
			double dSin = sin(-IPP_PI4);
			
			m_arrPoints[3].X = - m_dShiftX * dCos - (- m_dShiftY) * dSin + dX;
			m_arrPoints[3].Y = - m_dShiftX * dSin + (- m_dShiftY) * dCos + dY;

			m_arrPoints[2].X =   m_dShiftX * dCos - (- m_dShiftY) * dSin + dX;
			m_arrPoints[2].Y =   m_dShiftX * dSin + (- m_dShiftY) * dCos + dY;
			
			m_arrPoints[1].X =   m_dShiftX * dCos - (  m_dShiftY) * dSin + dX;
			m_arrPoints[1].Y =   m_dShiftX * dSin + (  m_dShiftY) * dCos + dY;
			
			m_arrPoints[0].X = - m_dShiftX * dCos - (  m_dShiftY) * dSin + dX;
			m_arrPoints[0].Y = - m_dShiftX * dSin + (  m_dShiftY) * dCos + dY;
		}
		return TRUE;
	}



	BOOL DrawHexagonBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
	{
		Gdiplus::Pen oPen(Color(0, 0, 0), 1);
		CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

		int nAlphaBlack = 448 * dEffect;
		int nAlphaWhite = 448 * dEffect;
		
		for ( int nIndex = 0; nIndex < 3; nIndex++ )
		{
			nAlphaBlack /= 2; 
			if (nIndex == 1)
			{
				nAlphaBlack = 224 * dEffect;
				nAlphaWhite = 224 * dEffect;
			}
			nAlphaWhite /= 2;
			//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
			oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
			oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
			oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
			oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
			//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
			oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
			oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[4]);
			oMainRender->DrawLine(&oPen, m_arrPoints[4], m_arrPoints[5]);
			oMainRender->DrawLine(&oPen, m_arrPoints[5], m_arrPoints[0]);

			m_arrPoints[0].X++;
			m_arrPoints[0].Y--;

			m_arrPoints[1].X--;
			m_arrPoints[1].Y--;

			m_arrPoints[2].X--;

			m_arrPoints[3].X--;
			m_arrPoints[3].Y++;

			m_arrPoints[4].X++;
			m_arrPoints[4].Y++;

			m_arrPoints[5].X++;

		}
		return TRUE;
	}
	BOOL DrawTriangleBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
	{
		if (30 == m_nFigureType )
		{
			Gdiplus::Pen oPen(Color(0, 0, 0), 1);
			CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

			int nAlphaBlack = 448 * dEffect;
			int nAlphaWhite = 448 * dEffect;
			
			for ( int nIndex = 0; nIndex < 3; nIndex++ )
			{
				nAlphaBlack /= 2; 
				if (nIndex == 1 || nIndex == 2)
				{
					nAlphaBlack = 224 * dEffect;
					nAlphaWhite = 224 * dEffect;
				}
				nAlphaWhite /= 2;
				if(nIndex == 0)
				{
					nAlphaWhite = 128;
					nAlphaBlack = 128;
				}
				oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
				oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
				oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
				oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
				oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[0]);

				m_arrPoints[0].X ++;
				m_arrPoints[0].Y -= float(0.4);

				m_arrPoints[1].Y++;

				m_arrPoints[2].X --;
				m_arrPoints[2].Y -= float(0.4);


			}
		}
		else if ( 31 == m_nFigureType)
		{
			Gdiplus::Pen oPen(Color(0, 0, 0), 1);
			CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

			int nAlphaBlack = 448 * dEffect;
			int nAlphaWhite = 448 * dEffect;
			
			for ( int nIndex = 0; nIndex < 3; nIndex++ )
			{
				nAlphaBlack /= 2; 
				if (nIndex == 1 || nIndex == 2)
				{
					nAlphaBlack = 224 * dEffect;
					nAlphaWhite = 224 * dEffect;
				}
				nAlphaWhite /= 2;
				if(nIndex == 0)
				{
					nAlphaWhite = 128;
					nAlphaBlack = 128;
				}
				oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
				oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
				oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
				oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
				oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[0]);

				m_arrPoints[0].Y--;

				m_arrPoints[1].X ++;
				m_arrPoints[1].Y += float(0.4);

				m_arrPoints[2].X --;
				m_arrPoints[2].Y += float(0.4);


			}
		}
		return TRUE;
	}

	BOOL DrawRhombusBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
	{
		Gdiplus::Pen oPen(Color(0, 0, 0), 1);
		CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

		int nAlphaBlack = 448 * dEffect;
		int nAlphaWhite = 448 * dEffect;
		
		for ( int nIndex = 0; nIndex < 3; nIndex++ )
		{
			nAlphaBlack /= 2; 
			if (nIndex == 1)
			{
				nAlphaBlack = 224 * dEffect;
				nAlphaWhite = 224 * dEffect;
			}
			nAlphaWhite /= 2;
			//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
			oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
			oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
			oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
			//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
			oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
			oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
			oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[0]);

			m_arrPoints[0].X++;
			m_arrPoints[1].Y--;
			m_arrPoints[2].X--;
			m_arrPoints[3].Y++;

		}
		return TRUE;
	}

	BOOL DrawBrickBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
	{
		Gdiplus::Pen oPen(Color(0, 0, 0), 1);
		CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

		int nAlphaBlack = 448 * dEffect;
		int nAlphaWhite = 448 * dEffect;
		
		for ( int nIndex = 0; nIndex < 3; nIndex++ )
		{
			nAlphaBlack /= 2; 
			if (nIndex == 1)
			{
				nAlphaBlack = 224 * dEffect;
				nAlphaWhite = 224 * dEffect;
			}
			nAlphaWhite /= 2;
			//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
			oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
			oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
			oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
			//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
			oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
			oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
			oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[0]);

			m_arrPoints[0].X++;
			m_arrPoints[0].Y++;

			m_arrPoints[1].X--;
			m_arrPoints[1].Y++;
			
			m_arrPoints[2].X--;
			m_arrPoints[2].Y--;
			
			m_arrPoints[3].X++;
			m_arrPoints[3].Y--;

		}
		return TRUE;
	}
	BOOL DrawInclinedBrickBorder(Gdiplus::Graphics *oMainRender, double dXCoorResult, double dYCoorResult, double dEffect)
	{
		Gdiplus::Pen oPen(Color(0, 0, 0), 1);
		CreatePolygon(m_nFigureType, dXCoorResult, dYCoorResult);

		int nAlphaBlack = 448 * dEffect;
		int nAlphaWhite = 448 * dEffect;
		
		for ( int nIndex = 0; nIndex < 3; nIndex++ )
		{
			nAlphaBlack /= 2; 
			if (nIndex == 1)
			{
				nAlphaBlack = 224 * dEffect;
				nAlphaWhite = 224 * dEffect;
			}
			nAlphaWhite /= 2;
			//oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
			oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
			oMainRender->DrawLine(&oPen, m_arrPoints[0], m_arrPoints[1]);
			oMainRender->DrawLine(&oPen, m_arrPoints[1], m_arrPoints[2]);
			//oPen.SetColor(Color(nAlphaBlack, 0, 0, 0));
			oPen.SetColor(Color(nAlphaWhite, 255, 255, 255));
			oMainRender->DrawLine(&oPen, m_arrPoints[2], m_arrPoints[3]);
			oMainRender->DrawLine(&oPen, m_arrPoints[3], m_arrPoints[0]);

			m_arrPoints[1].X--;
			m_arrPoints[2].Y++;
			m_arrPoints[3].X++;
			m_arrPoints[0].Y--;

		}
		return TRUE;
	}
	
public:
	double m_dBigRadius;

	int	m_nPicHeight;
	int	m_nPicWidth;

	BYTE* m_pBGRASource;

	double m_dXCoorSource;
	double m_dYCoorSource;

	int m_nFigureType; 
	// 30, 31 - правильный треугольник (смотрящий вниз или вверх соответсвенно)
	// 6 - правильный шестиугольник
	// 41 - квадрат, повернутый на 45 градусов
	// 42 - прямоугольник в форме кирпича(соотношение сторон 25:12)
	// 43 - крипич под углом 45 градусов

protected:

	double m_dShiftY;  // вспомогательные параметры, обычно используются для нахождения
	double m_dShiftX;  // координат вершин, относительно координат центра

	Gdiplus::PointF* m_arrPoints;
	int              m_nPointsCount;

};



