using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgRef3D : OperandPtgRef3D, IRefOperandPtg
    {
        private PtgRef3D(Cells.CellRangeRel cell_ref, Worksheet ws_from, Worksheet ws_to)
            : base(ws_from, ws_to)
        {
            Workbook parent_book = ws_from.Workbook;
            this.manager = parent_book.ValuesUpdateManager;
            alien_cell_ref = cell_ref;

            // Store Subject in CellSubject storage
            Cells.Cell alien_cell = alien_cell_ref.ToCell();
            alien_cell_subjects = new List<CellItem>();
            foreach (Worksheet ws_current in parent_book.WorksheetsN(ws_from.Index, ws_to.Index))
            {
                alien_cell_subjects.Add(ws_current.SubjectsStorage.GetCellItemOrCreate(alien_cell));
            }
        }

        public static OperandPtg Create(string ref_str, Cells.Offset offset, Worksheet ws_from, Worksheet ws_to)
        {
            Cells.CellRangeRel cell_ref = new Cells.CellRangeRel(ref_str, ws_from.Workbook) + offset;
            if (cell_ref != Cells.CellRangeRel.Empty)
            {
                return new PtgRef3D(cell_ref, ws_from, ws_to);
            }
            else
            {
                return new PtgRefErr3D(ErrorValue.bad_reference, ws_from, ws_to);
            }
        }

        #region IHavingSubjectPtg Members

        public override IObserver UnregisterDependency() 
        {
            if (observer != null)
            {
                foreach (CellItem alien_cell in alien_cell_subjects)
                {
                    manager.Unregister(alien_cell, observer);
                }
                base.UnregisterDependency();
                IObserver former_observer = observer;
                observer = null;
                return former_observer;
            }
            return null;
            // we do not remove cell_subject from SubjectsStorage because another formula may reference the same cell
        }

        public override void RegisterDependency(IObserver observer) 
        {
            this.observer = observer;
            // Register in DAG manager
            foreach (CellItem alien_cell in alien_cell_subjects)
            {
                manager.Register(alien_cell, observer);
            }
            base.RegisterDependency(observer);
        }

        #endregion

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            Cells.CellRange3D range3D = new Cells.CellRange3D(alien_cell_ref.ToCellRange(), WorksheetFrom, WorksheetTo);
            if (result_type != PreferredResultType.reference_type && result_type != PreferredResultType.as_is)
            {
                if (range3D.IsSingleCell)
                {
                    Cells.Cell cell = range3D.CellRange.TopLeft;
                    CellItem cell_subject = WorksheetFrom.SubjectsStorage.GetCellItemOrCreate(cell);
                    return cell_subject.GetValue().ConvertIfPossible(result_type);
                }
                else
                {
                    return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
                }
            }
            else
            {
                return new DataObjects.ObjectReference(range3D, observer);
            }
        }

        #region IRefOperandPtg Members

        public Cells.CellRange GetReferredRange()
        {
            return alien_cell_ref.ToCellRange();
        }

        public bool ReferencesCellOrRange(ISubject subject)
        {
            foreach (CellItem alien_cell in alien_cell_subjects)
            {
                if (alien_cell.CompareTo(subject) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        public OperandPtg Shift(Cells.Offset offset, Worksheet affected_sheet)
        {
            if (WorksheetFrom == WorksheetTo && WorksheetFrom == affected_sheet)
            {
                IObserver former_observer = UnregisterDependency();

                alien_cell_ref = alien_cell_ref.Shift(offset);
                if (alien_cell_ref == Cells.CellRangeRel.Empty)
                {
                    return new PtgRefErr3D(ErrorValue.bad_reference, WorksheetFrom, WorksheetTo);
                }

                // Store Subject in CellSubject storage
                alien_cell_subjects.Clear();
                alien_cell_subjects.Add(affected_sheet.SubjectsStorage.GetCellItemOrCreate(alien_cell_ref.ToCell()));

                if (former_observer != null)
                {
                    RegisterDependency(former_observer);
                }
                return this;
            }
            return null;
        }

        public OperandPtg RemoveRange(Cells.CellRange removed_range, Worksheet removed_range_sheet)
        {
            if (WorksheetFrom == WorksheetTo && WorksheetFrom == removed_range_sheet)
            {
                if (removed_range.Contains(alien_cell_ref.ToCell()))
                {
                    UnregisterDependency();
                    return new PtgRefErr3D(ErrorValue.bad_reference, WorksheetFrom, WorksheetTo);
                }
            }
            return null;
        }

        public OperandPtg ResizeInsertionAffected(Cells.CellRange moved_rows_range, Cells.Offset offset, Worksheet affected_sheet)
        {
            return null;
        }

        public bool IsRelative
        {
            get { return alien_cell_ref.IsRelative; }
        }

        #endregion
        
        public override string ToString()
        {
            return ToString3DPart() + alien_cell_ref.ToString();
        }

        private Cells.CellRangeRel alien_cell_ref; // The referred cell
        private List<CellItem> alien_cell_subjects;

        private IObserver observer;
        private ValuesUpdateManager manager;
    }
}
