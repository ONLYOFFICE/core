using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation
{
    internal interface IObserver
    {
        void Update();
        void ChangeObserverOnRangeRemoved(Cells.CellRange removed_range, Worksheet removed_range_sheet);
        void ChangeObserverOnRangeInserted(Cells.CellRange moved_rows_range, Cells.Offset offset, Worksheet affected_sheet);
        void ShiftReferencesTo(ISubject subject, Cells.Offset offset, Worksheet affected_sheet);

        void ChangeObserverOnWorksheetRemoved(Worksheet removed_sheet);
        void ChangeObserverOnWorksheetRenamed(Worksheet removed_sheet);

        bool Valid { get; }
        void Invalidate();
    }
}
