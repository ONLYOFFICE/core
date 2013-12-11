using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncMin : PtgFuncVar
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.ObjectNumber min = new DataObjects.ObjectNumber(double.MaxValue);
            foreach (DataObjects.IDataObject arg in argv)
            {
                DataObjects.ObjectReference range = arg as DataObjects.ObjectReference;
                if(range != null)
                {
                    foreach (DataObjects.IDataObject arg_referenced in range)
                    {
                        DataObjects.ObjectNumber arg_referenced_num = arg_referenced as DataObjects.ObjectNumber;
                        if (arg_referenced_num != null)
                        {
                            if(arg_referenced_num.Value < min.Value)
                            {
                                min = arg_referenced_num;
                            }
                        }
                        else if(arg_referenced is DataObjects.ObjectError)
                        {
                           return arg_referenced.ConvertIfPossible(result_type);;
                        }
                    }
                }
                else // If non-reference
                {
                    DataObjects.IDataObject arg_converted = arg.ConvertIfPossible(PreferredResultType.numeric_type);
                    if(arg_converted is DataObjects.ObjectError)
                    {
                       return arg.ConvertIfPossible(result_type);;
                    }
                    else if(arg_converted is DataObjects.ObjectString)
                    {
                       return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
                    }
                    DataObjects.ObjectNumber arg_numeric = arg_converted as DataObjects.ObjectNumber;
                    if(arg_numeric != null)
                    {
                        if (arg_numeric.Value < min.Value)
                        {
                            min = arg_numeric;
                        }
                    }
                }
            }
            if (min.Value == double.MaxValue)
            {
                min = new DataObjects.ObjectNumber(0);
            }
            return min.ConvertIfPossible(result_type);
        }

        public override string ToString() { return "MIN"; }
        public override string ArgumentsTip { get { return "MIN(number1; number2; ...)"; } }
    }
}
