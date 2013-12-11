using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncT : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            foreach (DataObjects.IDataObject arg in argv)
            {
                DataObjects.IDataObject value_arg = arg.ConvertIfPossible(PreferredResultType.value_type);
                if (value_arg is DataObjects.ObjectString || value_arg is DataObjects.ObjectError)
                {
                    return value_arg;
                }
                return new DataObjects.ObjectString("");
            }
            throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
        }

        public override string ToString() { return "T"; }
        public override int ParametersNum { get { return 1; } }
        public override int ParametersNumMin { get { return 1; } }
        public override int ParametersNumMax { get { return 1; } }
        public override string ArgumentsTip { get { return "T(value)"; } }
    }
}
