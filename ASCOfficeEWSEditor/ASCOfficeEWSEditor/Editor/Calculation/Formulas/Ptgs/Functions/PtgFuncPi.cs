using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncPi : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            return new DataObjects.ObjectNumber(3.14159265358979).ConvertIfPossible(result_type);
        }

        public override string ToString() { return "PI"; }
        public override int ParametersNum { get { return 0; } }
        public override int ParametersNumMin { get { return 0; } }
        public override int ParametersNumMax { get { return 0; } }
        public override string ArgumentsTip { get { return "PI()"; } }
    }
}
