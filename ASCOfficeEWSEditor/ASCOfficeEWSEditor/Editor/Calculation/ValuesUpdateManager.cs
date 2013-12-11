using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation
{
    internal class ValuesUpdateManager
    {
        public ValuesUpdateManager()
        {
            map = new Dictionary<ISubject, List<IObserver>>();
            update_candidates = new Queue<IObserver>();
        }

        public void Register(ISubject subj, IObserver observer)
        {
            List<IObserver> list;
            if (!map.TryGetValue(subj, out list))
            {
                list = new List<IObserver>();
                list.Add(observer);
                map[subj] = list;
            }
            else
            {
                if(!list.Contains(observer))
                {
                    list.Add(observer);
                }
            }
        }

        public void Unregister(ISubject subj, IObserver observer)
        {
            List<IObserver> list;
            if(map.TryGetValue(subj, out list))
            {
                list.Remove(observer);
            }
        }

        // Designated for named ranges subject
        internal void NotifyUpdated(ISubject subj)
        {
            Invalidate(subj); // Exclude snowball update
            UpdateMarkedObservers();
        }

        internal void NotifyUpdatedCell(Cells.Cell updated_cell, Worksheet parent_sheet)
        {
            Calculation.CellItem cell_subj = parent_sheet.SubjectsStorage.GetCellItemOrCreate(updated_cell);
            cell_subj.CellOrFormulaUpdated = true;
            // Notify recalculation engine
            InvalidateUpdatedCell(cell_subj, updated_cell, parent_sheet);

            UpdateMarkedObservers();
        }

        internal void NotifyDeleted(Cells.CellRange removed_range, Worksheet removed_range_sheet)
        {
            List<ISubject> deleted_subjects = new List<ISubject>();
            // Search for single cells
            foreach (KeyValuePair<Cells.Cell, CellItem> subj_item in removed_range_sheet.SubjectsStorage.CellSubjects)
            {
                if(removed_range.Contains(subj_item.Key))
                {
                    deleted_subjects.Add(subj_item.Value);
                    InvalidateUpdatedCell(subj_item.Value, subj_item.Key, removed_range_sheet);
                }
            }
            // Search for ranges
            foreach (KeyValuePair<Cells.CellRange, RangeSubject> subj_item in removed_range_sheet.SubjectsStorage.RangeSubjects)
            {
                if (removed_range.IsIntersectedWith(subj_item.Key))
                {
                    deleted_subjects.Add(subj_item.Value);
                    Invalidate(subj_item.Value);
                }
            }
            // Change only first level observers. The others will be updated in UpdateMarkedObservers()
            foreach (ISubject subj in deleted_subjects)
            {
                List<IObserver> observers;
                if (map.TryGetValue(subj, out observers))
                {
                    List<IObserver> delete_candidates = new List<IObserver>();
                    foreach (IObserver observer in observers)
                    {
                        delete_candidates.Add(observer);
                    }
                    foreach (IObserver observer in delete_candidates)
                    {
                        observer.ChangeObserverOnRangeRemoved(removed_range, removed_range_sheet);
                    }
                }
            }
            UpdateMarkedObservers();
        }

        internal void NotifyRemovedSheet(Sheet removed_sheet)
        {
            Worksheet removed_worksheet = removed_sheet as Worksheet;
            if (removed_worksheet != null)
            {
                Invalidate(removed_worksheet.Subject);
                List<IObserver> observers;
                if (map.TryGetValue(removed_worksheet.Subject, out observers))
                {
                    List<IObserver> referenced_observers = new List<IObserver>();
                    foreach (IObserver observer in observers)
                    {
                        referenced_observers.Add(observer);
                    }
                    foreach (IObserver observer in referenced_observers)
                    {
                        update_candidates.Enqueue(observer);
                        observer.ChangeObserverOnWorksheetRemoved(removed_worksheet);
                        observer.Invalidate();
                    }
                }
                UpdateMarkedObservers();
            }
        }

        internal void NotifyRenamedSheet(Sheet renamed_sheet)
        {
            List<IObserver> observers;
            Worksheet renamed_worksheet = renamed_sheet as Worksheet;
            if (renamed_worksheet != null)
            {
                if (map.TryGetValue(renamed_worksheet.Subject, out observers))
                {
                    foreach (IObserver observer in observers)
                    {
                        observer.ChangeObserverOnWorksheetRenamed(renamed_worksheet);
                    }
                }
            }
        }

        internal void NotifyRangesOnInsertedRowsOrColumns(Cells.CellRange moved_range, Cells.Offset offset, Worksheet affected_range)
        {
            List<ISubject> resized_subjects = new List<ISubject>();
            // Search for ranges
            foreach (KeyValuePair<Cells.CellRange, RangeSubject> subj_item in affected_range.SubjectsStorage.RangeSubjects)
            {
                if (subj_item.Key.IsBoundaryIntersectedWith(moved_range))
                {
                    resized_subjects.Add(subj_item.Value);
                    Invalidate(subj_item.Value);
                }
            }
            // Change only first level observers. The others will be updated in UpdateMarkedObservers()
            foreach (ISubject subj in resized_subjects)
            {
                List<IObserver> observers;
                if (map.TryGetValue(subj, out observers))
                {
                    List<IObserver> delete_candidates = new List<IObserver>();
                    foreach (IObserver observer in observers)
                    {
                        delete_candidates.Add(observer);
                    }
                    foreach (IObserver observer in delete_candidates)
                    {
                        observer.ChangeObserverOnRangeInserted(moved_range, offset, affected_range);
                    }
                }
            }
            UpdateMarkedObservers();
        }

        internal void NotifyCellOrRangeMoved(Cells.CellRange moved_range, Cells.Offset offset, Worksheet affected_range)
        {
            LinkedList<ISubject> moved_subjects = new LinkedList<ISubject>();
            // Search for single cells
            foreach (KeyValuePair<Cells.Cell, CellItem> element in affected_range.SubjectsStorage.CellSubjects)
            {
                Cells.Cell cell = element.Key;
                if (moved_range.Contains(cell))
                {
                    if (offset.RowOffset > 0 || offset.ColumnOffset > 0)
                    {
                        moved_subjects.AddFirst(element.Value);
                    }
                    else
                    {
                        moved_subjects.AddLast(element.Value);
                    }
                    InvalidateUpdatedCell(element.Value, element.Key, affected_range);
                }
            }
            // Search for ranges
            foreach (KeyValuePair<Cells.CellRange, RangeSubject> element in affected_range.SubjectsStorage.RangeSubjects)
            {
                Cells.CellRange cell_range = element.Key;
                if (cell_range.IsIntersectedWith(moved_range))
                {
                    if (offset.RowOffset > 0 || offset.ColumnOffset > 0)
                    {
                        moved_subjects.AddFirst(element.Value);
                    }
                    else
                    {
                        moved_subjects.AddLast(element.Value);
                    }
                    Invalidate(element.Value);
                }
            }

            // Change only first level observers. The others will be updated in UpdateMarkedObservers()
            foreach (ISubject subj in moved_subjects)
            {
                List<IObserver> observers;
                if (map.TryGetValue(subj, out observers))
                {
                    List<IObserver> move_candidates = new List<IObserver>();
                    foreach (IObserver observer in observers)
                    {
                        move_candidates.Add(observer);
                    }
                    foreach (IObserver observer in move_candidates)
                    {
                        observer.ShiftReferencesTo(subj, offset, affected_range);
                    }
                }
            }
            UpdateMarkedObservers();
        }

        internal void InvalidateUpdatedCell(ISubject subj, Cells.Cell updated_cell, Worksheet parent_sheet)
        {
            Invalidate(subj);

            // Invalidate range subjects that aren't explicitly linked with the subject
            foreach (KeyValuePair<Cells.CellRange, RangeSubject> subj_item in parent_sheet.SubjectsStorage.RangeSubjects)
            {
                if (subj_item.Key.Contains(updated_cell))
                {
                    Invalidate(subj_item.Value);
                }
            }

        }

        // Note: No ranges subjects are touched here
        internal void Invalidate(ISubject subj)
        {
            List<IObserver> observers;
            if (map.TryGetValue(subj, out observers))
            {
                foreach (IObserver observer in observers)
                {
                    if (observer.Valid)
                    {
                        update_candidates.Enqueue(observer);
                        observer.Invalidate();
                    }
                }
            }
        }

        private void InvalidateObserver(IObserver observer)
        {
            if (observer.Valid)
            {
                update_candidates.Enqueue(observer);
                observer.Invalidate();
            }
        }

        private void UpdateMarkedObservers()
        {
            while (update_candidates.Count != 0)
            {
                IObserver observer = update_candidates.Dequeue();
                if (!observer.Valid)
                {
                    observer.Update();
                }
            }
        }

        internal bool HasObservers(ISubject subject)
        {
            List<IObserver> observers;
            if (map.TryGetValue(subject, out observers))
            {
                return observers.Count != 0;
            }
            return false;
        }

        private Dictionary<ISubject, List<IObserver>> map;
        private Queue<IObserver> update_candidates;
    }
}
