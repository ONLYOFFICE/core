using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncCount : PtgFuncVar
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            int count = 0;
            foreach (DataObjects.IDataObject arg in argv)
            {
                DataObjects.ObjectReference range = arg as DataObjects.ObjectReference;
                if(range != null)
                {
                    foreach (DataObjects.IDataObject arg_referenced in range)
                    {
                        if (arg_referenced is DataObjects.ObjectNumber)
                        {
                            ++count;
                        }
                    }
                }
                else // If single value
                {
                    if(arg.ConvertIfPossible(PreferredResultType.numeric_type) is DataObjects.ObjectNumber)
                    {
                        ++count;
                    }
                }
            }

            return new DataObjects.ObjectNumber(count);
        }

        public override string ToString() { return "COUNT"; }
        public override string ArgumentsTip { get { return "COUNT(value1; value2; ...)"; } }
    }
}
