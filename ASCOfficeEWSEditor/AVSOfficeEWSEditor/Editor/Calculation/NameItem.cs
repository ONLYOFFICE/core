using System;
using System.Collections.Generic;
using System.Text;
using AVSOfficeEWSEditor.Editor.Calculation.Formulas;

namespace AVSOfficeEWSEditor.Editor.Calculation
{
    internal class NameItem : IObserver, ISubject
    {
        public NameItem(string name, Workbook workbook, ValuesUpdateManager manager, NameParsedFormula formula_item)
        {
            this.manager = manager;
            this.workbook = workbook;
            Formula = formula_item;
            Name = name;
            valid = true;
        }

        #region IObserver Members

        public void Update()
        {
            valid = true;
        }

        public void ChangeObserverOnRangeRemoved(Cells.CellRange removed_range, Worksheet removed_range_sheet)
        {
            Formula.RemoveReferencesTo(removed_range, removed_range_sheet);
            workbook.DefinedNames.AddName(Name, Formula.Assemble());
        }

        public void ChangeObserverOnWorksheetRemoved(Worksheet removed_sheet)
        {
            Formula.RemoveReferencesTo(removed_sheet);
            workbook.DefinedNames.AddName(Name, Formula.Assemble());
        }

        public void ChangeObserverOnWorksheetRenamed(Worksheet removed_sheet)
        {
            workbook.DefinedNames.AddName(Name, Formula.Assemble());
        }

        public void ChangeObserverOnRangeInserted(Cells.CellRange moved_range, Cells.Offset offset, Worksheet affected_sheet)
        {
            Formula.ResizeInsertionAffected(moved_range, offset, affected_sheet);
            workbook.DefinedNames.AddName(Name, Formula.Assemble());
        }

        public void ShiftReferencesTo(ISubject subject, Cells.Offset offset, Worksheet affected_sheet)
        {
            Formula.ShiftReferencesTo(subject, offset, affected_sheet);
            workbook.DefinedNames.AddName(Name, Formula.Assemble());
        }

        public bool Valid { get { return valid; } }
        public void Invalidate()
        {
            valid = false;
            manager.Invalidate(this);
        }

        #endregion

        #region ISubject Members

/*
        public Formulas.DataObjects.IDataObject GetValue()
        {
            if (HasFormula() && !valid)
            {
                if (formula_item.RecalcInProgress) // Cycle found!!!
                {
                    return new Formulas.DataObjects.ObjectError(ErrorValue.bad_reference);
                }
                Formulas.DataObjects.IDataObject result = formula_item.Recalculate(); // May lead in Update of 'this' 
                valid = true;
                CellOrFormulaUpdated = true;
                if (result != null)
                {
                    parent_sheet.CellSetValue(cell, result);
                }
                return result;
            }
            else
            {
                return parent_sheet.CellGetValue(cell);
            }
        }
*/

        #endregion

        public override string ToString()
        {
            return Name;
        }

        #region IComparable<ISubject> Members

        public int CompareTo(ISubject other)
        {
            NameItem other_subj = other as NameItem;
            if (other_subj == null)
            {
                return this.GetType().GUID.CompareTo(other.GetType().GUID);
            }
            return Name.CompareTo(other_subj.Name);
        }

        #endregion

        internal string Name { get; private set; }
        internal NameParsedFormula Formula { get; private set; }

        private bool valid;

        private Workbook workbook;
        private ValuesUpdateManager manager;
    }

}
