// ImageComposeIPP : header file

#pragma once

#include "smartimage.h"
#include "patterns.h"

BOOL IPPResizeCenter(BYTE* pBGRASource, BYTE* pBGRAResult, int nWidth, int nHeight, double dXFactor, double dYFactor, double dXCenter, double dYCenter);

BOOL IPPRotate(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY,	double dFrame, BOOL bIn, int nFirstAngle, BOOL bTransparent);

BOOL IPPStretchDoorVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bIn);

BOOL IPPStretchDoorHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bIn);

BOOL IPPStretchStripsVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame);

BOOL IPPStretchStripsHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame);

BOOL IPPStretchSideHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bLtoR);

BOOL IPPStretchSideVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bTtoB);

BOOL IPPStretchSideLTToRB(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame);

BOOL IPPStretchSideLBToRT(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame);

BOOL IPPStretchSideRBToLT(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame);

BOOL IPPStretchSideRTToLB(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame);

BOOL IPPStretchCenter(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bIn);

BOOL IPPStretchWipeSideHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bIn, BOOL bLtoR);

BOOL IPPStretchWipeSideVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bIn, BOOL bBtoT);

BOOL IPPTwirl(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bClockwise, int nZoom, Utility* g_utility);

BOOL IPPTwirlDrain(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bClockwise, Utility* pUtility);

BOOL IPPSphere(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bIn);

BOOL IPPSwingSideHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bLeft, BOOL bIn);

BOOL IPPSwingSideVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bTop, BOOL bIn);

BOOL IPPSwingSideChangeHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bLeft);

BOOL IPPSwingSideChangeVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bTop);

BOOL IPPSwingDoorsHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bIn);

BOOL IPPSwingDoorsVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY,	double dFrame, BOOL bIn);

BOOL IPPSwingDoorsChangeHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY,	double dFrame);

BOOL IPPSwingDoorsChangeVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame);

BOOL IPPFlipOverVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bIn );

BOOL IPPFlipOverHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY,	double dFrame, BOOL bIn);

BOOL IPPFlipAwayVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bIn, BOOL bForward );

BOOL IPPFlipAwayHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY,	double dFrame, BOOL bIn, BOOL bForward );

BOOL IPPFlipAwayOutInVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bForward );

BOOL IPPFlipAwayOutInHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bForward );

BOOL IPPTwistHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bLtoR, Utility* g_utility);

BOOL IPPTwistVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bTtoB, Utility* g_utility);

/*
nDirection = 0	LTToRB
nDirection = 1	LBToRT
nDirection = 2	RTToLB
nDirection = 3	RBToLT
*/
BOOL IPPTwistDiagonal(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame,int nDirection, Utility* g_utility);

BOOL IPPRipple(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bIsOut);

BOOL IPPWave(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bIsHorizontal);

BOOL IPPFadeZoom(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bZoomIn, BOOL bZoomOut);

BOOL IPPBurning(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, Utility* g_utility);

BOOL IPPBurningFade(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, Utility* g_utility);

BOOL IPPBurningFire(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, Utility* g_utility);

BOOL IPPFoldVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bTop );

BOOL IPPFoldHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bLeft );

BOOL IPPGaussianBlur(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame);

BOOL IPPNoise(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame);

BOOL IPPFlipBoxesVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bTop );

BOOL IPPFlipBoxesHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bLeft );

BOOL IPPFlipOutBoxesVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bIn);

BOOL IPPFlipOutBoxesHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, int nSizeX, int nSizeY, double dFrame, BOOL bIn);

BOOL IPPFlash(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int Color);

BOOL IPPMask(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Mask* g_mask, int nFadeWide);

BOOL IPPTetris(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Utility* g_utility);

BOOL IPPShotChangeHorizontal(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Utility* g_utility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor);

BOOL IPPShotChangeVertical(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Utility* g_utility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor);

BOOL IPPShotChangeHorizontalWithoutZoom(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Utility* g_utility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor);

BOOL IPPShotChangeVerticalWithoutZoom(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, Utility* g_utility, BYTE nBkColor, BYTE nFilmColorMid, BYTE nFilmColorBorder, BYTE nBlendColor);

BOOL IPPBlackWhiteLines(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, BOOL bInverse);

BOOL IPPEye(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nWideFade);

BOOL IPPHeart(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nWideFade);

BOOL IPPPixelate(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame);

BOOL IPPRoll(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection);

BOOL IPPSlide(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection);

BOOL IPPSlideCenter(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection);

BOOL IPPShrink(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection);

BOOL IPPSpiral(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nDirection);

BOOL IPPSimpleWave(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nWideFade);

BOOL IPPDrawAlbumList(BYTE* pResult, int nWidth, int nHeight, double dAngle, double dResizeFactor, double dXShift, double dYShift, int nWide, BOOL bCorners, double dFinalAngle);

BOOL IPPPageTurn(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness);

BOOL IPPPageTurnSimple(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness);

BOOL IPPPageTurnWhiteBack(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness);

BOOL IPPPageTurnAlbum(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness);

BOOL IPPPinwheelCurved(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, MaskPinwheelCurved* pMask, int nFadeWide);

BOOL IPPPuzzle(const SmartImage &first, const SmartImage &last, SmartImage &result, double dFrame, int nType, PuzzleUtility *pUtility, int nXCount, const int nYCount, const int nFlyTime, int nCorners, BOOL bPuzzle, int nShuffleType, BOOL bCollapse);

BOOL IPPTiles(const SmartImage &first, const SmartImage &last, SmartImage &result, int nFigureType, double dFrame, double dSideFactor, int nType, PuzzleUtility* pUtility, int nFlyTime, int nShuffleType, int nStartType, BOOL bCollapse);

BOOL IPPMaskGraduate(const SmartImage &first, const SmartImage &last, SmartImage &result, int nTilesX, int nTilesY, int nType, double dFrame, MaskGraduate* oMaskManager, int nFadeWide, BOOL bWipeSide);

