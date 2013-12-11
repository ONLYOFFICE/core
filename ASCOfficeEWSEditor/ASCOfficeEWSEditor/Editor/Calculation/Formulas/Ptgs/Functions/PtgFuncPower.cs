using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncPower : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.IDataObject number = null;
            DataObjects.IDataObject power = null;
            int argument_index = 0;
            foreach (DataObjects.IDataObject arg in argv)
            {
                switch (++argument_index)
                {
                    case 1:
                        number = arg.ConvertIfPossible(PreferredResultType.numeric_type);
                        break;
                    case 2:
                        power = arg.ConvertIfPossible(PreferredResultType.numeric_type);
                        break;
                }
            }
            if (argument_index != ParametersNum)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
            }
            
            return number.PerformBinaryOperation(power, BinOpCalc.Arithmetic.PowerCalculator).ConvertIfPossible(result_type);
        }

        public override string ToString() { return "POWER"; }
        public override int ParametersNum { get { return 2; } }
        public override int ParametersNumMin { get { return 2; } }
        public override int ParametersNumMax { get { return 2; } }
        public override string ArgumentsTip { get { return "POWER(number; power)"; } }
    }
}
