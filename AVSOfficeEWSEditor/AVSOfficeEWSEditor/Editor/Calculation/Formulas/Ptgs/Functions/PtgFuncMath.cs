using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal abstract class PtgFuncMathOneArg : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            foreach (DataObjects.IDataObject arg in argv)
            {
                DataObjects.IDataObject arg_converted = arg.ConvertIfPossible(PreferredResultType.numeric_type);
                DataObjects.ObjectNumber num = arg_converted as DataObjects.ObjectNumber;
                if (num != null) // If numeric
                {
                    return DoMath(num.Value).ConvertIfPossible(result_type);
                }
                else // If non-numeric
                {
                    if(arg_converted is DataObjects.ObjectError)
                    {
                        return arg_converted;
                    }
                    else
                    {
                        return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
                    }
                }
            }
            throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
        }

        public override int ParametersNum { get { return 1; } }
        public override int ParametersNumMin { get { return 1; } }
        public override int ParametersNumMax { get { return 1; } }
        public override string ArgumentsTip { get { return ToString() + "(number)"; } }

        public abstract DataObjects.IDataObject DoMath(double num_value);
    }

    internal abstract class PtgFuncMathTwoArgs : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.IDataObject arg1 = null;
            DataObjects.IDataObject arg2 = null;
            int argument_index = 0;
            foreach (DataObjects.IDataObject arg in argv)
            {
                switch (++argument_index)
                {
                    case 1:
                        arg1 = arg.ConvertIfPossible(PreferredResultType.numeric_type);
                        break;
                    case 2:
                        arg2 = arg.ConvertIfPossible(PreferredResultType.numeric_type);
                        break;
                }
            }
            if (argument_index != ParametersNum)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
            }
            DataObjects.ObjectNumber num_value1 = arg1 as DataObjects.ObjectNumber;
            DataObjects.ObjectNumber num_value2 = arg2 as DataObjects.ObjectNumber;
            if (num_value1 != null && num_value2!= null) // If both numeric
            {
                return DoMath(num_value1.Value, num_value2.Value).ConvertIfPossible(result_type);
            }
            else // If non-numeric
            {
                if (arg1 is DataObjects.ObjectError)
                {
                    return arg1;
                }
                else if (num_value1 != null && arg2 is DataObjects.ObjectError)
                {
                    return arg2;
                }
                return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
            }
        }

        public override int ParametersNum { get { return 2; } }
        public override int ParametersNumMin { get { return 2; } }
        public override int ParametersNumMax { get { return 2; } }

        public abstract DataObjects.IDataObject DoMath(double num_value1, double num_value2);
    }
}
