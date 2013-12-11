using System;
using System.Collections.Generic;
using System.Text;
using ASCOfficeEWSEditor.Editor.Calculation.Formulas;

namespace ASCOfficeEWSEditor.Editor.Calculation
{
    internal class CellItem : IObserver, ISubject
    {
        /// <summary>
        /// Create as an Observer
        /// </summary>
        public CellItem(Cells.Cell cell, Worksheet parent_sheet, ValuesUpdateManager manager, CellParsedFormula formula_item)
        {
            this.manager = manager;
            this.formula_item = formula_item; // may be null
            this.parent_sheet = parent_sheet;
            this.cell = cell;
            valid = true;
            CellOrFormulaUpdated = false;
            UpdateDelayed = false;
        }

        /// <summary>
        /// Construct non-observer, just subject
        /// </summary>
        public CellItem(Cells.Cell cell, Worksheet parent_sheet, ValuesUpdateManager manager)
            : this(cell, parent_sheet, manager, null)
        {
        }

        #region IObserver Members

        // Do NOT call if not Observer
        public void Update()
        {
            RecalcFormulaAndUpdateCell();
        }

        public bool RecalcFormulaAndUpdateCell()
        {
            Calculation.Formulas.DataObjects.IDataObject result = formula_item.Recalculate(); // May lead in Update of 'this' 
            CellOrFormulaUpdated = true;
            valid = true;
            if (result != null)
            {
                parent_sheet.CellSetValue(cell, result);
                return true;
            }
            return false;
        }

        public void CellSetErrorValue(Calculation.Formulas.DataObjects.IDataObject error)
        {
            parent_sheet.CellSetValue(cell, error);
        }

        public void ChangeObserverOnRangeRemoved(Cells.CellRange removed_range, Worksheet removed_range_sheet)
        {
            if (formula_item.RemoveReferencesTo(removed_range, removed_range_sheet))
            {
                CellOrFormulaUpdated = true;
            }
            else
            {
                UpdateDelayed = true;
            }
        }

        public void ChangeObserverOnWorksheetRemoved(Worksheet removed_sheet)
        {
            if (formula_item.RemoveReferencesTo(removed_sheet))
            {
                RecalcFormulaAndUpdateCell();
                CellOrFormulaUpdated = true;
            }
            else // This case if the names of the sheets haven't been changed and the value shall be recalculated later (after actual sheet removing)
            {
                UpdateDelayed = true;
            }
            parent_sheet.FormulaSimpleSetValue(cell, formula_item.Assemble());
        }

        public void ChangeObserverOnWorksheetRenamed(Worksheet removed_sheet)
        {
            parent_sheet.FormulaSimpleSetValue(cell, formula_item.Assemble());
            CellOrFormulaUpdated = true;
        }

        public void ChangeObserverOnRangeInserted(Cells.CellRange moved_range, Cells.Offset offset, Worksheet affected_sheet)
        {
            if (formula_item.ResizeInsertionAffected(moved_range, offset, affected_sheet))
            {
                CellOrFormulaUpdated = true;
            }
            else
            {
                UpdateDelayed = true;
            }
        }

        public void ShiftReferencesTo(ISubject subject, Cells.Offset offset, Worksheet affected_sheet)
        {
            if (formula_item.ShiftReferencesTo(subject, offset, affected_sheet))
            {
                CellOrFormulaUpdated = true;
            }
            else
            {
                UpdateDelayed = true;
            }
        }

        public bool Valid { get { return valid; } }
        public void Invalidate()
        {
            valid = false;
            manager.InvalidateUpdatedCell(this, cell, parent_sheet);
        }

        #endregion

        #region ISubject Members

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

        #endregion

        public override string ToString()
        {
            return cell.ToString();
        }

        public void BecomeObserver(CellParsedFormula formula_item)
        {
            this.formula_item = formula_item;
        }

        public void BecomeSubject()
        {
            this.formula_item = null;
        }

        public bool HasFormula()
        {
            return formula_item != null;
        }

        #region IComparable<ISubject> Members

        public int CompareTo(ISubject other)
        {
            CellItem other_subj = other as CellItem;
            if (other_subj == null)
            {
                return this.GetType().GUID.CompareTo(other.GetType().GUID);
            }
            int sheets_cmp = parent_sheet.Index.CompareTo(other_subj.parent_sheet.Index);
            if(sheets_cmp != 0)
            {
                return sheets_cmp;
            }
            return cell.CompareTo(other_subj.cell);
        }

        #endregion

        internal bool CellOrFormulaUpdated { get; set; } // Set if the result value is changed due to a recalculation or linked subject changed
        internal bool UpdateDelayed { get; set; } // Set if the formula can't be recalculated at a moment and must be recalculated later
        internal Cells.Cell Cell { get { return cell; } }
        
        private Cells.Cell cell;
        private Worksheet parent_sheet;
        
        private ValuesUpdateManager manager;
        private CellParsedFormula formula_item;
        private bool valid;

    }

}
