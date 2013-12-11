using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation
{
    internal class SubjectsStorage
    {
        public SubjectsStorage(Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            CellSubjects = new Dictionary<Cells.Cell, CellItem>();
            RangeSubjects = new Dictionary<Cells.CellRange, RangeSubject>();
        }

        internal Dictionary<Cells.Cell, CellItem> CellSubjects { get; private set; }
        internal Dictionary<Cells.CellRange, RangeSubject> RangeSubjects { get; private set; }

        internal CellItem GetCellItemOrCreate(Cells.Cell cell)
        {
            CellItem result = GetCellItem(cell);
            if (result == null)
            {
                result = new CellItem(cell, parent_sheet, parent_sheet.Workbook.ValuesUpdateManager); // create as subject
                parent_sheet.SubjectsStorage.CellSubjects[cell] = result;
            }
            return result;
        }

        internal CellItem GetCellItem(Cells.Cell cell)
        {
            CellItem result;
            if (CellSubjects.TryGetValue(cell, out result))
            {
                return result;
            }
            return null;
        }

        internal RangeSubject GetRangeSubjectOrCreate(Cells.CellRange range)
        {
            RangeSubject result = GetRangeSubject(range);
            if (result == null)
            {
                result = new RangeSubject(range, parent_sheet, parent_sheet.Workbook.ValuesUpdateManager);
                parent_sheet.SubjectsStorage.RangeSubjects[range] = result;
            }
            return result;
        }

        internal RangeSubject GetRangeSubject(Cells.CellRange range)
        {
            RangeSubject result;
            if (RangeSubjects.TryGetValue(range, out result))
            {
                return result;
            }
            return null;
        }

        internal void MarkRangeCellsUpdated(Cells.CellRange range)
        {
            foreach (Cells.Cell cell in range)
            {
                GetCellItemOrCreate(cell).CellOrFormulaUpdated = true;
            }
        }

        private Worksheet parent_sheet;
    }
}
