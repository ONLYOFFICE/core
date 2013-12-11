using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncIfError : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.IDataObject value = null;
            DataObjects.IDataObject value_if_error = null;
            int argument_index = 0;
            foreach (DataObjects.IDataObject arg in argv)
            {
                switch(++argument_index)
                {
                    case 1:
                        value = arg.ConvertIfPossible(PreferredResultType.value_type);
                        break;
                    case 2:
                        value_if_error = arg;
                        break;
                }
            }
            if (argument_index != ParametersNum && (ParametersNumMin > argument_index || argument_index > ParametersNumMax))
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
            }

            if (null == value || value.Type == new DataObjects.ObjectEmpty().Type)
                value = new DataObjects.ObjectNumber(0);
            if (null == value_if_error || value_if_error.Type == new DataObjects.ObjectEmpty().Type)
                value_if_error = new DataObjects.ObjectNumber(0);

            return (value is DataObjects.ObjectError ? value_if_error : value).ConvertIfPossible(result_type);
        }

        public override string ToString() { return "IFERROR"; }
        public override int ParametersNum { get { return 2; } }
        public override int ParametersNumMin { get { return 0; } }
        public override int ParametersNumMax { get { return 2; } }
        public override string ArgumentsTip { get { return "IFERROR(value; value_if_error)"; } }
    }
}
