using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncNa : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            return new DataObjects.ObjectError(ErrorValue.not_available);
        }

        public override string ToString() { return "NA"; }
        public override int ParametersNum { get { return 0; } }
        public override int ParametersNumMin { get { return 0; } }
        public override int ParametersNumMax { get { return 0; } }
        public override string ArgumentsTip { get { return "NA()"; } }
    }
}
