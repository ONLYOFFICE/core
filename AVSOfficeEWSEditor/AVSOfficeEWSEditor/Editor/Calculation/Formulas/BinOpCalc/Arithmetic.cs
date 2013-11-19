using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.BinOpCalc
{
    internal class Arithmetic
    {
        public static readonly AddCalculator AddCalculator = new AddCalculator();
        public static readonly SubCalculator SubCalculator = new SubCalculator();
        public static readonly MulCalculator MulCalculator = new MulCalculator();
        public static readonly DivCalculator DivCalculator = new DivCalculator();
        public static readonly PowerCalculator PowerCalculator = new PowerCalculator();
        public static readonly EqCalculator EqCalculator = new EqCalculator();
        public static readonly LtCalculator LtCalculator = new LtCalculator();
        public static readonly GtCalculator GtCalculator = new GtCalculator();
        public static readonly ConcatCalculator ConcatCalculator = new ConcatCalculator();
        public static readonly UnionCalculator UnionCalculator = new UnionCalculator();
        public static readonly IsectCalculator IsectCalculator = new IsectCalculator();
        public static readonly RangeCalculator RangeCalculator = new RangeCalculator();
    }
}
