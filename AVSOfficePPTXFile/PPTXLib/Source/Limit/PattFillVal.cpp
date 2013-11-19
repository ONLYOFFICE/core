#include "PattFillVal.h"

namespace PPTX
{
	namespace Limit
	{
		PattFillVal::PattFillVal()
		{
			add("cross"); //(Cross)
			add("dashDnDiag"); //(Dashed Downward Diagonal)
			add("dashHorz"); //(Dashed Horizontal)
			add("dashUpDiag"); //(Dashed Upward DIagonal)
			add("dashVert"); //(Dashed Vertical)
			add("diagBrick"); //(Diagonal Brick)
			add("diagCross"); //(Diagonal Cross)
			add("divot"); //(Divot)
			add("dkDnDiag"); //(Dark Downward Diagonal)
			add("dkHorz"); //(Dark Horizontal)
			add("dkUpDiag"); //(Dark Upward Diagonal)
			add("dkVert"); //(Dark Vertical)
			add("dnDiag"); //(Downward Diagonal)
			add("dotDmnd"); //(Dotted Diamond)
			add("dotGrid"); //(Dotted Grid)
			add("horz"); //(Horizontal)
			add("horzBrick"); //(Horizontal Brick)
			add("lgCheck"); //(Large Checker Board)
			add("lgConfetti"); //(Large Confetti)
			add("lgGrid"); //(Large Grid)
			add("ltDnDiag"); //(Light Downward Diagonal)
			add("ltHorz"); //(Light Horizontal)
			add("ltUpDiag"); //(Light Upward Diagonal)
			add("ltVert"); //(Light Vertical)
			add("narHorz"); //(Narrow Horizontal)
			add("narVert"); //(Narrow Vertical)
			add("openDmnd"); //(Open Diamond)
			add("pct10"); //(10%)
			add("pct20"); //(20%)
			add("pct25"); //(25%)
			add("pct30"); //(30%)
			add("pct40"); //(40%)
			add("pct5"); //(5%)
			add("pct50"); //(50%)
			add("pct60"); //(60%)
			add("pct70"); //(70%)
			add("pct75"); //(75%)
			add("pct80"); //(80%)
			add("pct90"); //(90%)
			add("plaid"); //(Plaid)
			add("shingle"); //(Shingle)
			add("smCheck"); //(Small Checker Board)
			add("smConfetti"); //(Small Confetti)
			add("smGrid"); //(Small Grid)
			add("solidDmnd"); //(Solid Diamond)
			add("sphere"); //(Sphere)
			add("trellis"); //(Trellis)
			add("upDiag"); //(Upward Diagonal)
			add("vert"); //(Vertical)
			add("wave"); //(Wave)
			add("wdDnDiag"); //(Wide Downward Diagonal)
			add("wdUpDiag"); //(Wide Upward Diagonal)
			add("weave"); //(Weave)
			add("zigZag"); //(Zig Zag)
		}

		const std::string PattFillVal::no_find() const
		{
			return "cross";
		}
	} // namespace Limit
} // namespace PPTX