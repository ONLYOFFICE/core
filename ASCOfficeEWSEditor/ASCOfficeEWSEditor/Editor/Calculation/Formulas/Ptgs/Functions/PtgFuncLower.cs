using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncLower : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            foreach (DataObjects.IDataObject arg in argv)
            {
                DataObjects.IDataObject arg_converted = arg.ConvertIfPossible(PreferredResultType.string_type);

                DataObjects.ObjectString str = arg_converted as DataObjects.ObjectString;
                if (str != null) // If string
                {
                    return new DataObjects.ObjectString(str.Value.ToLower()).ConvertIfPossible(result_type);
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

        public override string ToString() { return "LOWER"; }
        public override int ParametersNum { get { return 1; } }
        public override int ParametersNumMin { get { return 1; } }
        public override int ParametersNumMax { get { return 1; } }
        public override string ArgumentsTip { get { return "LOWER(text)"; } }
    }
}
