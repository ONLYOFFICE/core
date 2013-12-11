using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncIsOdd : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            if (argv.Count != ParametersNum)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
            }

            DataObjects.IDataObject arg = argv.First.Value.ConvertIfPossible(PreferredResultType.value_type);
            if(arg is DataObjects.ObjectError)
            {
                return arg;
            }
            if (arg is DataObjects.ObjectBoolean)
            {
                return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
            }
            DataObjects.ObjectNumber num_arg = argv.First.Value.ConvertIfPossible(PreferredResultType.numeric_type) as DataObjects.ObjectNumber;
            if (num_arg == null)
            {
                return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
            }
            return new DataObjects.ObjectBoolean((((int)num_arg.Value) & 1) == 1);
        }

        public override string ToString() { return "ISODD"; }
        public override int ParametersNum { get { return 1; } }
        public override int ParametersNumMin { get { return 1; } }
        public override int ParametersNumMax { get { return 1; } }
        public override string ArgumentsTip { get { return "ISODD(number)"; } }
    }
}
