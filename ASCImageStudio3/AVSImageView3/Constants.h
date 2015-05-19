
#pragma once
#include "math.h"

// const double M_PI = 3.14159265358979323846;

#define sgn(x)		( ((x)>0) ? 1 : 0 )
#define rad(x)		( (x)*(M_PI/180) )
#define grad(x)		( (x)*(180/M_PI) )
#define sqr(x)		( (x)*(x) )

const int c_MousePointerDefault = 0;
const int c_MousePointerArrow = 1;
const int c_MousePointerCross = 2;
const int c_MousePointerIBeam = 3;
const int c_MousePointerIcon = 4;
const int c_MousePointerSize = 5;
const int c_MousePointerSizeNESW = 6;
const int c_MousePointerSizeNS = 7;
const int c_MousePointerSizeNWSE = 8;
const int c_MousePointerSizeWE = 9;
const int c_MousePointerUpArrow = 10;
const int c_MousePointerHourglass = 11;
const int c_MousePointerNoDrop = 12;
const int c_MousePointerArrowHourglass = 13;
const int c_MousePointerArrowQuestion = 14;
const int c_MousePointerSizeAll = 15;
const int c_MousePointerHand = 16;
const int c_MousePointerRotation = 17;
const int c_MousePointerSizeForRotation = 18;
	
const int c_WindowStyleNormal = 0;
const int c_WindowStyleModal = 1;
const int c_WindowStyleClient = 2;
const int c_WindowStyleStatic = 3;
	
const int c_MaskStyleHidden = 0;
const int c_MaskStyleSolid = 1;
const int c_MaskStyleBorder = 2;
const int c_MaskStyleDots = 3;
const int c_MaskStyleDotsDense = 4;
const int c_MaskStyleGrid = 5;
const int c_MaskStyleGridRotated = 6;
const int c_MaskStyleLineVertical = 7;
const int c_MaskStyleLineHorizontal = 8;
const int c_MaskStyleLineDiagonal = 9;
const int c_MaskStyleLineDiagonalRotated = 10;
const int c_MaskStyleChessBoard = 11;
const int c_MaskStyleDashes = 12;
const int c_MaskStyleSpiner = 13;
const int c_MaskStyleThatches = 14;
	
const int c_BackStyleNone = 0;
const int c_BackStyleSolid = 1;
const int c_BackStyleDots = 2;
const int c_BackStyleDotsDense = 3;
const int c_BackStyleGrid = 4;
const int c_BackStyleGridRotated = 5;
const int c_BackStyleLineVertical = 6;
const int c_BackStyleLineHorizontal = 7;
const int c_BackStyleLineDiagonal = 8;
const int c_BackStyleLineDiagonalRotated = 9;
const int c_BackStyleChessBoard = 10;
const int c_BackStyleDashes = 11;
const int c_BackStyleSpiner = 12;
const int c_BackStyleThatches = 13;

const int c_TrackColorSchemeInverse = 0;
const int c_TrackColorSchemeBlackWhite = 1;
const int c_TrackColorSchemeWhiteBlack = 2;
const int c_TrackColorSchemeWhiteOrange = 3;
const int c_TrackColorSchemeBlueYellow = 4;
const int c_TrackColorSchemeBlackGreen = 5;
	
const int c_SenderDraw = 0;
const int c_SenderTrack = 1;
const int c_SenderRefresh = 2;

const double c_dImageFactorZoomIn = 1.5;
const double c_dImageFactorZoomOut = 1.0/c_dImageFactorZoomIn;

const BYTE c_Patterns[13][8][8] = 
{
	// c_PatternSolid
	{
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1}
	},
	// c_PatternDots
	{
		{1,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{1,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0}
	},
	// c_PatternDotsDense
	{
		{1,0,1,0,1,0,1,0},
		{0,0,0,0,0,0,0,0},
		{1,0,1,0,1,0,1,0},
		{0,0,0,0,0,0,0,0},
		{1,0,1,0,1,0,1,0},
		{0,0,0,0,0,0,0,0},
		{1,0,1,0,1,0,1,0},
		{0,0,0,0,0,0,0,0}
	},
	// c_PatternGrid
	{
		{1,1,1,1,1,1,1,1},
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0},
		{1,1,1,1,1,1,1,1},
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0}
	},
	// c_PatternGridRotated
	{
		{1,0,0,0,0,0,0,1},
		{0,1,0,0,0,0,1,0},
		{0,0,1,0,0,1,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,1,0,0,1,0,0},
		{0,1,0,0,0,0,1,0},
		{1,0,0,0,0,0,0,1}
	},
	// c_PatternLineVertical
	{
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0},
		{1,0,0,0,1,0,0,0}
	},
	// c_PatternLineHorizontal
	{
		{1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0}
	},
	// c_PatternLineDiagonal 
	{
		{0,0,0,1,0,0,0,0},
		{0,0,1,0,0,0,0,0},
		{0,1,0,0,0,0,0,0},
		{1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1},
		{0,0,0,0,0,0,1,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,1,0,0,0}
	},
	// c_PatternLineDiagonalRotated
	{
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0},
		{0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0},
		{0,1,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0},
		{0,0,0,1,0,0,0,0}
	},
	// c_PatternChessBoard
	{
		{1,1,1,1,0,0,0,0},
		{1,1,1,1,0,0,0,0},
		{1,1,1,1,0,0,0,0},
		{1,1,1,1,0,0,0,0},
		{0,0,0,0,1,1,1,1},
		{0,0,0,0,1,1,1,1},
		{0,0,0,0,1,1,1,1},
		{0,0,0,0,1,1,1,1}
	},
	// c_PatternDashes
	{
		{1,0,0,0,0,0,0,0},
		{0,1,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0},
		{0,0,0,0,0,1,0,0},
		{0,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,0,0}
	},
	// c_PatternSpiner
	{
		{0,0,0,1,0,1,0,0},
		{0,0,0,0,1,1,0,0},
		{1,1,0,0,1,0,0,0},
		{0,1,1,1,1,0,0,1},
		{1,0,0,1,1,1,1,0},
		{0,0,0,1,0,0,1,1},
		{0,0,1,1,0,0,0,0},
		{0,0,1,0,1,0,0,0}
	},
	// c_PatternThatches
	{
		{1,1,1,1,1,0,0,0},
		{0,1,1,1,0,1,0,0},
		{0,0,1,0,0,0,1,0},
		{0,1,0,0,0,1,1,1},
		{1,0,0,0,1,1,1,1},
		{0,0,0,1,0,1,1,1},
		{0,0,1,0,0,0,1,0},
		{0,1,1,1,0,0,0,1}
	}
};

class DPointEx
{
public:
	
	double x;
	double y;
	int X;
	int Y;
	
public:
	
	DPointEx()
	{
		x = y = X = Y = 0;
	}
	void Create(double dX, double dY)
	{
		x = dX;
		y = dY;
	}
};

class DRectEx
{
public:
	
	double left;
	double top;
	double right;
	double bottom;
		
	int Left;
	int Top;
	int Right;
	int Bottom;
	
public:
	
	DRectEx()
	{
		left = top = right = bottom = Left = Top = Right = Bottom = 0;
	}
	void Create(double l, double t, double r, double b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	void Create(double x, double y)
	{
		left = x - 2;
		top = y - 2;
		right = x + 2;
		bottom = y + 2;
	}
};