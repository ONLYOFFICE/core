using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncN : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            foreach (DataObjects.IDataObject arg in argv)
            {
                DataObjects.IDataObject value_arg = arg.ConvertIfPossible(PreferredResultType.value_type);
                DataObjects.ObjectBoolean arg_bool = value_arg as DataObjects.ObjectBoolean;
                if(arg_bool != null)
                {
                    return new DataObjects.ObjectNumber(arg_bool.Value ? 1 : 0);
                }
                else if (value_arg is DataObjects.ObjectNumber || value_arg is DataObjects.ObjectError)
                {
                    return value_arg;
                }
                return new DataObjects.ObjectNumber(0);
            }
            throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
        }

        public override string ToString() { return "N"; }
        public override int ParametersNum { get { return 1; } }
        public override int ParametersNumMin { get { return 1; } }
        public override int ParametersNumMax { get { return 1; } }
        public override string ArgumentsTip { get { return "N(value)"; } }
    }
}
