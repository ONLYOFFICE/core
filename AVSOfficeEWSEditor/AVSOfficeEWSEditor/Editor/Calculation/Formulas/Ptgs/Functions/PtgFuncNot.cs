using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncNot : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            foreach (DataObjects.IDataObject arg in argv)
            {
                DataObjects.IDataObject arg_converted = arg.ConvertIfPossible(PreferredResultType.boolean_type);
                DataObjects.ObjectBoolean bool_arg = arg_converted as DataObjects.ObjectBoolean;
                if (bool_arg != null) // If boolean
                {
                    return new DataObjects.ObjectBoolean(!bool_arg.Value).ConvertIfPossible(result_type);
                }
                else // If non-numeric
                {
                    if(arg_converted is DataObjects.ObjectError)
                    {
                        return arg_converted.ConvertIfPossible(result_type);
                    }
                    else
                    {
                       return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
                    }
                }
            }
            throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
        }

        public override string ToString() { return "NOT"; }
        public override int ParametersNum { get { return 1; } }
        public override int ParametersNumMin { get { return 1; } }
        public override int ParametersNumMax { get { return 1; } }
        public override string ArgumentsTip { get { return "NOT(logical)"; } }
    }
}
