using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal abstract class OperandPtgRef3D : OperandPtg, IHavingSubjectPtg
    {
        public OperandPtgRef3D(Worksheet ws_from, Worksheet ws_to)
        {
            WorksheetFrom = ws_from;
            WorksheetTo = ws_to;
            parent_book = ws_from.Workbook;
            this.manager = parent_book.ValuesUpdateManager;
            alien_ws_subjects = new List<WorksheetSubject>();
            foreach (Worksheet ws_current in parent_book.WorksheetsN(ws_from.Index, ws_to.Index))
            {
                alien_ws_subjects.Add(ws_current.Subject);
            }
        }

        public OperandPtg RemoveWorksheet(Worksheet removed_sheet)
        {
            int from_index = WorksheetFrom.Index;
            int to_index = WorksheetTo.Index;
            int removed_sheet_index = removed_sheet.Index;
            if (from_index <= removed_sheet_index && removed_sheet_index <= to_index)
            {
                if(from_index == removed_sheet_index || to_index == removed_sheet_index)
                {
                    IObserver former_observer = UnregisterDependency();

                    if (from_index == to_index)
                    {
                        return new PtgErr(ErrorValue.bad_reference); // Recalculation is necessary and possible right now
                    }
                    if (from_index == removed_sheet_index)
                    {
                        bool next_has_found = false;
                        foreach (Worksheet ws in parent_book.WorksheetsN(from_index + 1, to_index))
                        {
                            WorksheetFrom = ws;
                            next_has_found = true;
                            break;
                        }
                        if (!next_has_found)
                        {
                            return new PtgErr(ErrorValue.bad_reference); // Recalculation is necessary and possible right now
                        }
                    }
                    else if (to_index == removed_sheet_index)
                    {
                        bool last_has_found = false;
                        foreach (Worksheet ws in parent_book.WorksheetsN(from_index, to_index - 1))
                        {
                            WorksheetTo = ws;
                            last_has_found = true;
                        }

                        if (!last_has_found)
                        {
                            return new PtgErr(ErrorValue.bad_reference); // Recalculation is necessary and possible right now
                        }
                    }
                    // Store Subject in CellSubject storage
                    alien_ws_subjects.Remove(removed_sheet.Subject);

                    if (former_observer != null)
                    {
                        RegisterDependency(former_observer);
                    }
                    return this; // Recalculation is necessary and possible right now
                }
                return null; // Recalculation must be delayed (impossible right now)
            }
            return this; // Recalculation is no necessary
        }

        #region IHavingSubjectPtg Members

        public virtual IObserver UnregisterDependency()
        {
            if (observer != null)
            {
                foreach (WorksheetSubject ws_subj in alien_ws_subjects)
                {
                    manager.Unregister(ws_subj, observer);
                }
                IObserver former_observer = observer;
                observer = null;
                return former_observer;
            }
            return null;
        }

        public virtual void RegisterDependency(IObserver observer)
        {
            this.observer = observer;
            // Register in DAG manager
            foreach (WorksheetSubject ws_subj in alien_ws_subjects)
            {
                manager.Register(ws_subj, observer);
            }
        }

        #endregion


        public string ToString3DPart()
        {
            return Cells.CellRange3D.ToString3DPart(WorksheetFrom, WorksheetTo);
        }

        private Workbook parent_book;
        private IObserver observer;
        private ValuesUpdateManager manager;

        private List<WorksheetSubject> alien_ws_subjects;

        public Worksheet WorksheetFrom { get; private set; }
        public Worksheet WorksheetTo { get; private set; }
    }
}
