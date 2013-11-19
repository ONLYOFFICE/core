using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncIf : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.IDataObject condition = null;
            DataObjects.IDataObject if_true = new DataObjects.ObjectBoolean (true);
            DataObjects.IDataObject if_false = new DataObjects.ObjectBoolean(false);
            int argument_index = 0;
            foreach (DataObjects.IDataObject arg in argv)
            {
                switch(++argument_index)
                {
                    case 1:
                        condition = arg.ConvertIfPossible(PreferredResultType.boolean_type);
                        break;
                    case 2:
                        if_true = arg;
                        break;
                    case 3:
                        if_false = arg;
                        break;
                }
            }
            if (argument_index != ParametersNum && (ParametersNumMin > argument_index || argument_index > ParametersNumMax))
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
            }

            DataObjects.ObjectBoolean sure_condition = condition as DataObjects.ObjectBoolean;
            if (null == condition)
            {
                sure_condition = new DataObjects.ObjectBoolean(false);
            }
            if(sure_condition == null)
            {
                if (condition is DataObjects.ObjectError)
                {
                    return condition.ConvertIfPossible(result_type);
                }
                return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
            }

            DataObjects.IDataObject result = (sure_condition.Value ? if_true : if_false).ConvertIfPossible(result_type);
            if (result.Type == new DataObjects.ObjectEmpty().Type)
                result = new DataObjects.ObjectNumber(0);
            return result;
        }

        public override string ToString() { return "IF"; }
        public override int ParametersNum { get { return 3; } }
        public override int ParametersNumMin { get { return 0; } }
        public override int ParametersNumMax { get { return 3; } }
        public override string ArgumentsTip { get { return "IF(logical_test; value_if_true; value_if_false)"; } }
    }
}
