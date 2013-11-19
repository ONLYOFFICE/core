using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgRange : BinOperatorPtg, IHavingSubjectPtg
    {
        private static Regex rx_gt = new Regex("\\G *: *");
        public static bool Extract(string formula, ref int start_pos)
        {
            Match match = rx_gt.Match(formula, start_pos);
            if (match.Success)
            {
                start_pos += match.Length;
                return true;
            }
            return false;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            UnregisterDependency();
            DataObjects.IDataObject result = base.Calculate(ref ptgs_enum, result_type);
            vir_ref = result as DataObjects.ObjectReference;
            RegisterDependency();
            return result;
        }

        public override BinOpCalc.IFullBinaryOpDispatcher Calculator { get { return BinOpCalc.Arithmetic.RangeCalculator; } }

        public override PreferredResultType OperandsPreferredType { get { return PreferredResultType.reference_type; } }

        public override string ToString()
        {
            return ":";
        }

        public override int Priority { get { return 4; } }

        #region IHavingSubjectPtg Members

        public IObserver UnregisterDependency()
        {
            IObserver ret_observer = null;
            if (vir_ref != null)
            {
                ret_observer = vir_ref.Unregister();
            }
            vir_ref = null;
            return ret_observer;
        }

        public void RegisterDependency(IObserver observer)
        {
            RegisterDependency();
        }

        #endregion

        private void RegisterDependency()
        {
            if (vir_ref != null)
            {
                vir_ref.Register();
            }
        }

        private DataObjects.ObjectReference vir_ref;
    }
}
