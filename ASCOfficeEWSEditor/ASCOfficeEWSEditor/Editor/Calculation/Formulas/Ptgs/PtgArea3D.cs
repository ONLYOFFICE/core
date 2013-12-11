using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgArea3D : OperandPtgRef3D, IRefOperandPtg
    {
        private PtgArea3D(Cells.CellRangeRel range_ref, Worksheet ws_from, Worksheet ws_to)
            :  base(ws_from, ws_to)
        {
            Workbook parent_book = ws_from.Workbook;
            manager = parent_book.ValuesUpdateManager;
            alien_range_ref = range_ref;
            
            // Store Subjects in CellSubject storages
            Cells.CellRange alien_cell_range = alien_range_ref.ToCellRange();
            alien_range_subjects = new List<RangeSubject>();
            foreach (Worksheet ws_current in parent_book.WorksheetsN(ws_from.Index, ws_to.Index))
            {
                alien_range_subjects.Add(ws_current.SubjectsStorage.GetRangeSubjectOrCreate(alien_cell_range));
            }
        }

        public static OperandPtg Create(string ref_str, Cells.Offset offset, Worksheet ws_from, Worksheet ws_to)
        {
            Cells.CellRangeRel range_ref = new Cells.CellRangeRel(ref_str, ws_from.Workbook) + offset;
            if (range_ref != Cells.CellRangeRel.Empty)
            {
                return new PtgArea3D(range_ref, ws_from, ws_to);
            }
            else
            {
                return new PtgRefErr3D(ErrorValue.bad_reference, ws_from, ws_to);
            }

        }

        #region IHavingSubjectPtg Members

        /// <summary>
        /// 
        /// </summary>
        /// <returns>former observer if it is registered</returns>
        public override IObserver UnregisterDependency()
        {
            if (observer != null)
            {
                foreach (RangeSubject range_subj in alien_range_subjects)
                {
                    manager.Unregister(range_subj, observer);
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
            foreach (RangeSubject range_subj in alien_range_subjects)
            {
                manager.Register(range_subj, observer);
            }
            base.RegisterDependency(observer);
        }

        #endregion

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            Cells.CellRange3D range3D = new Cells.CellRange3D(alien_range_ref.ToCellRange(), WorksheetFrom, WorksheetTo);
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
                return  new DataObjects.ObjectReference(range3D, observer);
            }
        }

        #region IRefOperandPtg Members

        public Cells.CellRange GetReferredRange()
        {
            return alien_range_ref.ToCellRange();
        }

        public bool ReferencesCellOrRange(ISubject subject)
        {
            foreach (RangeSubject range_subj in alien_range_subjects)
            {
                if (range_subj.CompareTo(subject) == 0)
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
                IObserver former_observer = UnregisterDependency(); // Need this because the subject changes its key and the DAG tree become wrong

                alien_range_ref = alien_range_ref.Shift(offset);
                if (alien_range_ref == Cells.CellRangeRel.Empty)
                {
                    return new PtgRefErr3D(ErrorValue.bad_reference, WorksheetFrom, WorksheetTo);
                }

                // Store Subject in CellSubject storage
                alien_range_subjects.Clear();
                alien_range_subjects.Add(affected_sheet.SubjectsStorage.GetRangeSubjectOrCreate(alien_range_ref.ToCellRange()));

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
                IObserver former_observer = UnregisterDependency(); // Need this because the subject changes its key and the DAG tree become wrong

                Cells.CellRangeRel new_range_rel = alien_range_ref.Cut(removed_range);
                if (new_range_rel == Cells.CellRangeRel.Empty)
                {
                    return new PtgRefErr3D(ErrorValue.bad_reference, WorksheetFrom, WorksheetTo);
                }
                alien_range_ref = new_range_rel;

                // Store Subject in CellSubject storage
                alien_range_subjects.Clear();
                alien_range_subjects.Add(removed_range_sheet.SubjectsStorage.GetRangeSubjectOrCreate(alien_range_ref.ToCellRange()));

                if (former_observer != null)
                {
                    RegisterDependency(former_observer);
                }
                return this;
            }
            return null;
        }

        public OperandPtg ResizeInsertionAffected(Cells.CellRange moved_range, Cells.Offset offset, Worksheet affected_sheet)
        {
            if (WorksheetFrom == WorksheetTo && WorksheetFrom == affected_sheet)
            {
                IObserver former_observer = UnregisterDependency(); // Need this because the subject changes its key and the DAG tree become wrong

                Cells.CellRange cell_range = alien_range_ref.ToCellRange();

                if (cell_range.IsBoundaryIntersectedWith(moved_range))
                {
                    alien_range_ref = alien_range_ref.Resize(offset);
                }

                // Store Subject in CellSubject storage
                alien_range_subjects.Clear();
                alien_range_subjects.Add(affected_sheet.SubjectsStorage.GetRangeSubjectOrCreate(alien_range_ref.ToCellRange()));

                if (former_observer != null)
                {
                    RegisterDependency(former_observer);
                }
                return this;
            }
            return null;
        }

        public bool IsRelative
        {
            get { return alien_range_ref.IsRelative; }
        }

        #endregion

        public override string ToString()
        {
            return ToString3DPart() + alien_range_ref.ToString();
        }

        private Cells.CellRangeRel alien_range_ref; // The referred cells
        private List<RangeSubject> alien_range_subjects;

        private IObserver observer;
        private ValuesUpdateManager manager;
    }
}
