#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2EditorSimple
{
	WORD Converter::GetPattFillValFromStr(const CString& str)
	{
		WORD result = 4009;
		if (str == _T(""))
			return result;
		switch((CHAR)str[0])
		{
		case 'c':
			if (str == _T("cross"))		{ result += 4; break;} //(Cross)
			break;
		case 'd':
			if (str == _T("dashDnDiag")){ result += 30; break;} //(Dashed Downward Diagonal)
			if (str == _T("dashHorz"))	{ result += 32; break;} //(Dashed Horizontal)
			if (str == _T("dashUpDiag")){ result += 31; break;} //(Dashed Upward DIagonal)
			if (str == _T("dashVert"))	{ result += 33; break;} //(Dashed Vertical)
			if (str == _T("diagBrick"))	{ result += 38; break;} //(Diagonal Brick)
			if (str == _T("diagCross"))	{ result += 5; break;} //(Diagonal Cross)
			if (str == _T("divot"))		{ result += 42; break;} //(Divot)
			if (str == _T("dkDnDiag"))	{ result += 20; break;} //(Dark Downward Diagonal)
			if (str == _T("dkHorz"))	{ result += 29; break;} //(Dark Horizontal)
			if (str == _T("dkUpDiag"))	{ result += 21; break;} //(Dark Upward Diagonal)
			if (str == _T("dkVert"))	{ result += 28; break;} //(Dark Vertical)
			if (str == _T("dnDiag"))	{ result += 3; break;} //(Downward Diagonal)
			if (str == _T("dotDmnd"))	{ result += 44; break;} //(Dotted Diamond)
			if (str == _T("dotGrid"))	{ result += 43; break;} //(Dotted Grid)
			break;
		case 'h':
			if (str == _T("horz"))		{ result += 0; break;} //(Horizontal)
			if (str == _T("horzBrick"))	{ result += 39; break;} //(Horizontal Brick)
			break;
		case 'l':
			if (str == _T("lgCheck"))	{ result += 50; break;} //(Large Checker Board)
			if (str == _T("lgConfetti")){ result += 35; break;} //(Large Confetti)
			if (str == _T("lgGrid"))	{ result += 4; break;} //(Large Grid)
			if (str == _T("ltDnDiag"))	{ result += 18; break;} //(Light Downward Diagonal)
			if (str == _T("ltHorz"))	{ result += 25; break;} //(Light Horizontal)
			if (str == _T("ltUpDiag"))	{ result += 19; break;} //(Light Upward Diagonal)
			if (str == _T("ltVert"))	{ result += 24; break;} //(Light Vertical)
			break;
		case 'n':
			if (str == _T("narHorz"))	{ result += 27; break;} //(Narrow Horizontal)
			if (str == _T("narVert"))	{ result += 26; break;} //(Narrow Vertical)
			break;
		case 'o':
			if (str == _T("openDmnd"))	{ result += 51; break;} //(Open Diamond)
			break;
		case 'p':
			if (str == _T("pct10"))		{ result += 7; break;} //(10%)
			if (str == _T("pct20"))		{ result += 8; break;} //(20%)
			if (str == _T("pct25"))		{ result += 9; break;} //(25%)
			if (str == _T("pct30"))		{ result += 10; break;} //(30%)
			if (str == _T("pct40"))		{ result += 11; break;} //(40%)
			if (str == _T("pct5"))		{ result += 6; break;} //(5%)
			if (str == _T("pct50"))		{ result += 12; break;} //(50%)
			if (str == _T("pct60"))		{ result += 13; break;} //(60%)
			if (str == _T("pct70"))		{ result += 14; break;} //(70%)
			if (str == _T("pct75"))		{ result += 15; break;} //(75%)
			if (str == _T("pct80"))		{ result += 16; break;} //(80%)
			if (str == _T("pct90"))		{ result += 17; break;} //(90%)
			if (str == _T("plaid"))		{ result += 41; break;} //(Plaid)
			break;
		case 's':
			if (str == _T("shingle"))	{ result += 45; break;} //(Shingle)
			if (str == _T("smCheck"))	{ result += 49; break;} //(Small Checker Board)
			if (str == _T("smConfetti")){ result += 34; break;} //(Small Confetti)
			if (str == _T("smGrid"))	{ result += 48; break;} //(Small Grid)
			if (str == _T("solidDmnd"))	{ result += 52; break;} //(Solid Diamond)
			if (str == _T("sphere"))	{ result += 47; break;} //(Sphere)
			break;
		case 't':
			if (str == _T("trellis"))	{ result += 46; break;} //(Trellis)
			break;
		case 'u':
			if (str == _T("upDiag"))	{ result += 2; break;} //(Upward Diagonal)
			break;
		case 'v':
			if (str == _T("vert"))		{ result += 1; break;} //(Vertical)
			break;
		case 'w':
			if (str == _T("wave"))		{ result += 37; break;} //(Wave)
			if (str == _T("wdDnDiag"))	{ result += 22; break;} //(Wide Downward Diagonal)
			if (str == _T("wdUpDiag"))	{ result += 23; break;} //(Wide Upward Diagonal)
			if (str == _T("weave"))		{ result += 40; break;} //(Weave)
			break;
		case 'z':
			if (str == _T("zigZag"))	{ result += 36; break;} //(Zig Zag)
			break;
		}
		return result;
	}
}