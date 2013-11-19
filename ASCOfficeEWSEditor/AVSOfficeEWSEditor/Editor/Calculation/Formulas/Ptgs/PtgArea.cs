using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgArea : OperandPtg, IRefOperandPtg, IHavingSubjectPtg
    {
        private PtgArea(Cells.CellRangeRel range_ref, Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            this.manager = parent_sheet.Workbook.ValuesUpdateManager;
            alien_range_ref = range_ref;

            // Store Subject in CellSubject storage
            alien_range_subject = parent_sheet.SubjectsStorage.GetRangeSubjectOrCreate(alien_range_ref.ToCellRange());
        }

        public static OperandPtg Create(string ref_str, Cells.Offset offset, Worksheet parent_sheet)
        {
            if (parent_sheet == null)
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("It is prohibited to use range references without any certain sheet specified in Defined Names"));
            }

            Cells.CellRangeRel range_ref = new Cells.CellRangeRel(ref_str, parent_sheet.Workbook) + offset;
            if (range_ref != Cells.CellRangeRel.Empty)
            {
                return new PtgArea(range_ref, parent_sheet);
            }
            else
            {
                return new PtgErr(ErrorValue.bad_reference);
            }
        }

        #region IHavingSubjectPtg Members

        /// <summary>
        /// 
        /// </summary>
        /// <returns>former observer if it is registered</returns>
        public IObserver UnregisterDependency()
        {
            if (observer != null)
            {
                manager.Unregister(alien_range_subject, observer);
                IObserver former_observer = observer;
                observer = null;
                return former_observer;
            }
            return null;
            // we do not remove cell_subject from SubjectsStorage because another formula may reference the same cell
        }
        
        public void RegisterDependency(IObserver observer) 
        {
            this.observer = observer;
            // Register in DAG manager
            manager.Register(alien_range_subject, observer);
        }

        #endregion
        private static Regex rx_range = new Regex("\\G\\$?[A-Za-z]+\\$?\\d+:\\$?[A-Za-z]+\\$?\\d+");
        private static Regex rx_cols = new Regex("\\G\\$?[A-Za-z]+:\\$?[A-Za-z]+");
        private static Regex rx_rows = new Regex("\\G\\$?\\d+:\\$?\\d+");
        public static bool Extract(string formula, ref int start_pos, out string operand_str)
        {
            Match match = rx_range.Match(formula, start_pos);
            if ((match = rx_range.Match(formula, start_pos)).Success || 
                (match = rx_cols.Match(formula, start_pos)).Success || 
                (match = rx_rows.Match(formula, start_pos)).Success)
            {
                start_pos += match.Length;
                operand_str = match.Value;
                return true;
            }
            operand_str = null;
            return false;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            Cells.CellRange range = alien_range_ref.ToCellRange();
            if (result_type != PreferredResultType.reference_type && result_type != PreferredResultType.as_is)
            {
                if(range.IsSingleCell)
                {
                    Cells.Cell cell = range.TopLeft;
                    CellItem cell_subject = parent_sheet.SubjectsStorage.GetCellItemOrCreate(cell);
                    return cell_subject.GetValue().ConvertIfPossible(result_type);
                }
                else 
                {
                    return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
                }
            }
            else
            {
                return  new DataObjects.ObjectReference(range, parent_sheet, observer);
            }
        }

        #region IRefOperandPtg Members

        public Cells.CellRange GetReferredRange()
        {
            return alien_range_ref.ToCellRange();
        }

        public bool ReferencesCellOrRange(ISubject subject)
        {
            return alien_range_subject.CompareTo(subject) == 0;
        }

        public OperandPtg Shift(Cells.Offset offset, Worksheet affected_sheet)
        {
            if (parent_sheet == affected_sheet)
            {
                IObserver former_observer = UnregisterDependency(); // Need this because the subject changes its key and the DAG tree become wrong

                alien_range_ref = alien_range_ref.Shift(offset);
                if (alien_range_ref == Cells.CellRangeRel.Empty)
                {
                    return new PtgErr(ErrorValue.bad_reference);
                }

                // Store Subject in CellSubject storage
                alien_range_subject = parent_sheet.SubjectsStorage.GetRangeSubjectOrCreate(alien_range_ref.ToCellRange());

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
            if(parent_sheet == removed_range_sheet)
            {
                IObserver former_observer = UnregisterDependency(); // Need this because the subject changes its key and the DAG tree become wrong

                Cells.CellRangeRel new_range_rel = alien_range_ref.Cut(removed_range);
                if (new_range_rel == Cells.CellRangeRel.Empty)
                {
                    return new PtgErr(ErrorValue.bad_reference);
                }
                alien_range_ref = new_range_rel;

                // Store Subject in CellSubject storage
                alien_range_subject = parent_sheet.SubjectsStorage.GetRangeSubjectOrCreate(alien_range_ref.ToCellRange());

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
            if (parent_sheet == affected_sheet)
            {
                IObserver former_observer = UnregisterDependency(); // Need this because the subject changes its key and the DAG tree become wrong

                Cells.CellRange cell_range = alien_range_ref.ToCellRange();

                if (cell_range.IsBoundaryIntersectedWith(moved_range))
                {
                    alien_range_ref = alien_range_ref.Resize(offset);
                }

                // Store Subject in CellSubject storage
                alien_range_subject = parent_sheet.SubjectsStorage.GetRangeSubjectOrCreate(alien_range_ref.ToCellRange());

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
            return alien_range_ref.ToString();
        }

        private Cells.CellRangeRel alien_range_ref; // The referred cells
        private RangeSubject alien_range_subject;

        private IObserver observer;

        private ValuesUpdateManager manager;
        private Worksheet parent_sheet;
    }
}
