using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncErrorType : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            if (argv.Count != ParametersNum)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
            }

            DataObjects.ObjectError error_arg = argv.First.Value.ConvertIfPossible(PreferredResultType.value_type) as DataObjects.ObjectError;

            if (error_arg == null)
            {
                return new DataObjects.ObjectError(ErrorValue.not_available);
            }

            return new DataObjects.ObjectNumber(error_arg.Value.Code);
        }

        public override string ToString() { return "ERROR.TYPE"; }
        public override int ParametersNum { get { return 1; } }
        public override int ParametersNumMin { get { return 1; } }
        public override int ParametersNumMax { get { return 1; } }
        public override string ArgumentsTip { get { return "ERROR.TYPE(value)"; } }
    }
}
