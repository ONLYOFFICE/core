using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncTrue : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            return new DataObjects.ObjectBoolean(true).ConvertIfPossible(result_type);
        }

        public override string ToString() { return "TRUE"; }
        public override int ParametersNum { get { return 0; } }
        public override int ParametersNumMin { get { return 0; } }
        public override int ParametersNumMax { get { return 0; } }
        public override string ArgumentsTip { get { return "TRUE()"; } }
    }
}
