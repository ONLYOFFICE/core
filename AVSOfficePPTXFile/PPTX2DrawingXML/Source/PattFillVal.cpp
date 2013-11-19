#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2DrawingXML
{
	WORD Converter::GetPattFillValFromStr(const std::string& str)
	{
		WORD result = 4009;
		if(str == "")
			return result;
		switch(str[0])
		{
		case 'c':
			if(str == "cross")		{ result += 4; break;} //(Cross)
			break;
		case 'd':
			if(str == "dashDnDiag")	{ result += 30; break;} //(Dashed Downward Diagonal)
			if(str == "dashHorz")	{ result += 32; break;} //(Dashed Horizontal)
			if(str == "dashUpDiag")	{ result += 31; break;} //(Dashed Upward DIagonal)
			if(str == "dashVert")	{ result += 33; break;} //(Dashed Vertical)
			if(str == "diagBrick")	{ result += 38; break;} //(Diagonal Brick)
			if(str == "diagCross")	{ result += 5; break;} //(Diagonal Cross)
			if(str == "divot")		{ result += 42; break;} //(Divot)
			if(str == "dkDnDiag")	{ result += 20; break;} //(Dark Downward Diagonal)
			if(str == "dkHorz")		{ result += 29; break;} //(Dark Horizontal)
			if(str == "dkUpDiag")	{ result += 21; break;} //(Dark Upward Diagonal)
			if(str == "dkVert")		{ result += 28; break;} //(Dark Vertical)
			if(str == "dnDiag")		{ result += 3; break;} //(Downward Diagonal)
			if(str == "dotDmnd")	{ result += 44; break;} //(Dotted Diamond)
			if(str == "dotGrid")	{ result += 43; break;} //(Dotted Grid)
			break;
		case 'h':
			if(str == "horz")		{ result += 0; break;} //(Horizontal)
			if(str == "horzBrick")	{ result += 39; break;} //(Horizontal Brick)
			break;
		case 'l':
			if(str == "lgCheck")	{ result += 50; break;} //(Large Checker Board)
			if(str == "lgConfetti")	{ result += 35; break;} //(Large Confetti)
			if(str == "lgGrid")		{ result += 4; break;} //(Large Grid)
			if(str == "ltDnDiag")	{ result += 18; break;} //(Light Downward Diagonal)
			if(str == "ltHorz")		{ result += 25; break;} //(Light Horizontal)
			if(str == "ltUpDiag")	{ result += 19; break;} //(Light Upward Diagonal)
			if(str == "ltVert")		{ result += 24; break;} //(Light Vertical)
			break;
		case 'n':
			if(str == "narHorz")	{ result += 27; break;} //(Narrow Horizontal)
			if(str == "narVert")	{ result += 26; break;} //(Narrow Vertical)
			break;
		case 'o':
			if(str == "openDmnd")	{ result += 51; break;} //(Open Diamond)
			break;
		case 'p':
			if(str == "pct10")		{ result += 7; break;} //(10%)
			if(str == "pct20")		{ result += 8; break;} //(20%)
			if(str == "pct25")		{ result += 9; break;} //(25%)
			if(str == "pct30")		{ result += 10; break;} //(30%)
			if(str == "pct40")		{ result += 11; break;} //(40%)
			if(str == "pct5")		{ result += 6; break;} //(5%)
			if(str == "pct50")		{ result += 12; break;} //(50%)
			if(str == "pct60")		{ result += 13; break;} //(60%)
			if(str == "pct70")		{ result += 14; break;} //(70%)
			if(str == "pct75")		{ result += 15; break;} //(75%)
			if(str == "pct80")		{ result += 16; break;} //(80%)
			if(str == "pct90")		{ result += 17; break;} //(90%)
			if(str == "plaid")		{ result += 41; break;} //(Plaid)
			break;
		case 's':
			if(str == "shingle")	{ result += 45; break;} //(Shingle)
			if(str == "smCheck")	{ result += 49; break;} //(Small Checker Board)
			if(str == "smConfetti")	{ result += 34; break;} //(Small Confetti)
			if(str == "smGrid")		{ result += 48; break;} //(Small Grid)
			if(str == "solidDmnd")	{ result += 52; break;} //(Solid Diamond)
			if(str == "sphere")		{ result += 47; break;} //(Sphere)
			break;
		case 't':
			if(str == "trellis")	{ result += 46; break;} //(Trellis)
			break;
		case 'u':
			if(str == "upDiag")		{ result += 2; break;} //(Upward Diagonal)
			break;
		case 'v':
			if(str == "vert")		{ result += 1; break;} //(Vertical)
			break;
		case 'w':
			if(str == "wave")		{ result += 37; break;} //(Wave)
			if(str == "wdDnDiag")	{ result += 22; break;} //(Wide Downward Diagonal)
			if(str == "wdUpDiag")	{ result += 23; break;} //(Wide Upward Diagonal)
			if(str == "weave")		{ result += 40; break;} //(Weave)
			break;
		case 'z':
			if(str == "zigZag")		{ result += 36; break;} //(Zig Zag)
			break;
		}
		return result;
	}
}