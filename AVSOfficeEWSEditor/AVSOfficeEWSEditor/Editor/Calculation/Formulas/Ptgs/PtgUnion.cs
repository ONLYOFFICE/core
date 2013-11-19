using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgUnion : BinOperatorPtg
    {
        public override BinOpCalc.IFullBinaryOpDispatcher Calculator { get { return BinOpCalc.Arithmetic.UnionCalculator; } }

        public override PreferredResultType OperandsPreferredType { get { return PreferredResultType.reference_type; } }

        public override string ToString()
        {
            return ",";
        }

        public override int Priority { get { return 5; } }
    }
}
