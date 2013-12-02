using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgRef : OperandPtg, IRefOperandPtg, IHavingSubjectPtg
    {
        private PtgRef(Cells.CellRangeRel cell_ref, Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            this.manager = parent_sheet.Workbook.ValuesUpdateManager;
            alien_cell_ref = cell_ref;

            // Store Subject in CellSubject storage
            alien_cell_subject = parent_sheet.SubjectsStorage.GetCellItemOrCreate(alien_cell_ref.ToCell());
        }

        public static OperandPtg Create(string ref_str, Cells.Offset offset, Worksheet parent_sheet)
        {
            if (parent_sheet == null)
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("It is prohibited to use range references without any certain sheet specified in Defined Names"));
            }

            Cells.CellRangeRel cell_ref = new Cells.CellRangeRel(ref_str, parent_sheet.Workbook) + offset;
            if(cell_ref != Cells.CellRangeRel.Empty)
            {
                return new PtgRef(cell_ref, parent_sheet);
            }
            else
            {
                return new PtgErr(ErrorValue.bad_reference);
            }
        }

        #region IHavingSubjectPtg Members
        public IObserver UnregisterDependency() 
        {
            if (observer != null)
            {
                manager.Unregister(alien_cell_subject, observer);
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
            manager.Register(alien_cell_subject, observer);
        }
        #endregion
        private static Regex rx_ref = new Regex("\\G(\\$?[A-Za-z]+\\$?(\\d+))([-+*/^&%<=>: ;),]|$)");
        public static bool Extract(string formula, ref int start_pos, out string operand_str)
        {
            Match match = rx_ref.Match(formula, start_pos);
            if (match.Success)
            {
                GroupCollection groups = match.Groups;
                if (groups.Count >= 2 && (groups[1].Length < 7 || groups[1].Value.CompareTo("1048576") < 0))
                {
                    start_pos += groups[1].Length;
                    operand_str = groups[1].Value;
                    return true;
                }
            }
            operand_str = null;
            return false;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            return result_type == PreferredResultType.reference_type || result_type == PreferredResultType.as_is ?
                new DataObjects.ObjectReference(alien_cell_ref.ToCellRange(), parent_sheet, observer) :
                alien_cell_subject.GetValue().ConvertIfPossible(result_type);
        }

        #region IRefOperandPtg members
        
        public Cells.CellRange GetReferredRange()
        {
            return alien_cell_ref.ToCellRange();
        }

        public bool ReferencesCellOrRange(ISubject subject)
        {
            return alien_cell_subject.CompareTo(subject) == 0;
        }

        public OperandPtg Shift(Cells.Offset offset, Worksheet affected_sheet)
        {
            if (parent_sheet == affected_sheet)
            {
                IObserver former_observer = UnregisterDependency();

                alien_cell_ref = alien_cell_ref.Shift(offset);
                if (alien_cell_ref == Cells.CellRangeRel.Empty)
                {
                    return new PtgErr(ErrorValue.bad_reference);
                }

                // Store Subject in CellSubject storage
                alien_cell_subject = parent_sheet.SubjectsStorage.GetCellItemOrCreate(alien_cell_ref.ToCell());

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
            if (parent_sheet == removed_range_sheet && removed_range.Contains(alien_cell_ref.ToCell()))
            {
                UnregisterDependency();
                return new PtgErr(ErrorValue.bad_reference);
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
            return alien_cell_ref.ToString();
        }

        private Cells.CellRangeRel alien_cell_ref; // The referred cell
        private CellItem alien_cell_subject;

        private IObserver observer;

        private ValuesUpdateManager manager;
        private Worksheet parent_sheet;
    }
}
