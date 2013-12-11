using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgName : OperandPtg, IHavingSubjectPtg
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="name">Must be contained in DefinedNames.NameItems. Otherwise - exception</param>
        /// <param name="workbook"></param>
        public PtgName(string name, Workbook workbook)
        {
            this.workbook = workbook;
            this.manager = workbook.ValuesUpdateManager;
            this.name = name;
        }

        #region IHavingSubjectPtg Members
        public IObserver UnregisterDependency()
        {
            if (observer != null)
            {
                NameItem name_item;
                if (workbook.DefinedNames.NameItems.TryGetValue(name, out name_item))
                {
                    manager.Unregister(name_item, observer);
                }
                else
                {
                    throw new Events.ReportableException(new Events.InternalError("Attempt to unregister a non-existent NameItem"));
                }
                IObserver former_observer = observer;
                observer = null;
                return former_observer;
            }
            return null;
        }
        
        public void RegisterDependency(IObserver observer) 
        {
            this.observer = observer;
            NameItem name_item;
            if (workbook.DefinedNames.NameItems.TryGetValue(name, out name_item))
            {
                manager.Register(name_item, observer);
            }
            else
            {
                throw new Events.ReportableException(new Events.InternalError("Attempt to register a non-existent NameItem"));
            }
        }
        #endregion
        private static Regex rx_ref = new Regex("\\G(?<name>\\w[\\w\\d.]*)([-+*/^&%<=>: ;),]|$)");
        public static bool Extract(string formula, ref int start_pos, out string operand_str, Workbook workbook)
        {
            Match match = rx_ref.Match(formula, start_pos);
            if (match.Success)
            {
                GroupCollection groups = match.Groups;
                string name = groups["name"].Value;
                if (name.Length != 0 && null != workbook.DefinedNames && workbook.DefinedNames.IsNameExists(name))
                {
                    start_pos += name.Length;
                    operand_str = name;
                    return true;
                }
            }
            operand_str = null;
            return false;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            NameItem name_item;
            if (workbook.DefinedNames.NameItems.TryGetValue(name, out name_item))
            {
                return name_item.Formula.Recalculate(PreferredResultType.as_is).ConvertIfPossible(result_type);
            }
            else
            {
                return new DataObjects.ObjectError(ErrorValue.wrong_name);
            }
        }

        public override string ToString()
        {
            return name;
        }

        private string name; // The referred defined name

        private IObserver observer;
        private ValuesUpdateManager manager;
        private Workbook workbook;
    }
}
