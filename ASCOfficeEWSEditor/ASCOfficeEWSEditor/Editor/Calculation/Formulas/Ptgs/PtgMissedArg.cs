using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgMissedArg : OperandPtg
    {
        public PtgMissedArg()
        {
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            return new DataObjects.ObjectEmpty().ConvertIfPossible(result_type);
        }

        public override string Assemble(ref LinkedList<Ptg>.Enumerator ptgs_enum)
        {
            return "";
        }

        #region IComparable<DataObjects.IDataObject> Members

        public int CompareTo(DataObjects.IDataObject other)
        {
            if (other is DataObjects.ObjectEmpty)
            {
                return 0;
            }
            return 1;
        }

        #endregion

        public override string ToString()
        {
            return "missed argument";
        }

        public object UserValue { get { return null;} }
        public string Type { get { return null; } }

    }
}
