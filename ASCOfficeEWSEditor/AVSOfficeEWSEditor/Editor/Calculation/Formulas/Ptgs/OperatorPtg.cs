using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal abstract class OperatorPtg : Ptg
    {
        /// <summary>
        /// Must be overridden for right associative operators
        /// </summary>
        /// <returns>Associativity of the operator</returns>
        public virtual bool isRightAssociative() 
        {
            return false;
        }

        public abstract int Priority { get; }
    }

    internal abstract class BinOperatorPtg : OperatorPtg
    {
        public override string Assemble(ref LinkedList<Ptg>.Enumerator ptgs_enum)
        {
            if (!ptgs_enum.MoveNext())
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("Second argument of operator \"" + ToString() + "\" was not found"));
            }
            string arg2 = ptgs_enum.Current.Assemble(ref ptgs_enum);
            if (!ptgs_enum.MoveNext())
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("First argument of operator \"" + ToString() + "\" was not found"));
            }
            string arg1 = ptgs_enum.Current.Assemble(ref ptgs_enum);

            return arg1 + ToString() + arg2;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            if (!ptgs_enum.MoveNext())
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("Second argument of operator \"" + ToString() + "\" was not found"));
            }
            DataObjects.IDataObject arg2 = ptgs_enum.Current.Calculate(ref ptgs_enum, OperandsPreferredType);
            if (!ptgs_enum.MoveNext())
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("First argument of operator \"" + ToString() + "\" was not found"));
            }
            DataObjects.IDataObject arg1 = ptgs_enum.Current.Calculate(ref ptgs_enum, OperandsPreferredType);

            return arg1.PerformBinaryOperation(arg2, Calculator).ConvertIfPossible(result_type);
        }

        public abstract BinOpCalc.IFullBinaryOpDispatcher Calculator { get; }
        public abstract PreferredResultType OperandsPreferredType { get; }
    }
}
