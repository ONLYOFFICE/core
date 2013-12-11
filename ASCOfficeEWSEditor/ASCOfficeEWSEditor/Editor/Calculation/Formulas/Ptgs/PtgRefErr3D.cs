using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgRefErr3D : OperandPtgRef3D
    {
        public PtgRefErr3D(ErrorValue value, Worksheet ws_from, Worksheet ws_to)
            : base(ws_from, ws_to)
        {
            this.value = new DataObjects.ObjectError(value);
        }

        public PtgRefErr3D(string value, Worksheet ws_from, Worksheet ws_to)
            : this(ErrorValue.Parse(value), ws_from, ws_to)
        {
        }
        private static Regex rx_num = new Regex("\\G#REF!", RegexOptions.IgnoreCase); // searched case insensitive
        public static bool Extract(string formula, ref int start_pos, out string operand_str)
        {
            Match match = rx_num.Match(formula, start_pos);
            if (match.Success)
            {
                start_pos += match.Length;
                operand_str = match.Value;
                return true;
            }
            operand_str = null;
            return false;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            return value;
        }

        public override string ToString()
        {
            return ToString3DPart() + value.ToString();
        }

        private DataObjects.ObjectError value;
    }
}
