using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncFalse : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            return new DataObjects.ObjectBoolean(false).ConvertIfPossible(result_type);
        }

        public override string ToString() { return "FALSE"; }
        public override int ParametersNum { get { return 0; } }
        public override int ParametersNumMin { get { return 0; } }
        public override int ParametersNumMax { get { return 0; } }
        public override string ArgumentsTip { get { return "FALSE()"; } }
    }
}
