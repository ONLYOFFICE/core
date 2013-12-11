using System;
using System.Collections.Generic;
using System.Text;
using ASCOfficeEWSEditor.Editor.Calculation.Formulas;

namespace ASCOfficeEWSEditor.Editor.Calculation
{
    internal class HyperlinkItem : IObserver
    {
        public HyperlinkItem(Cells.Cell anchor, Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            this.anchor = anchor;
            valid = true;
        }

        #region IObserver Members

        public void Update()
        {
            valid = true;
        }

        public void ChangeObserverOnRangeRemoved(Cells.CellRange removed_range, Worksheet removed_range_sheet)
        {
            if(removed_range.Contains(anchor))
            {
                parent_sheet.Hyperlinks.RemoveHyperlink(anchor);
            }
        }

        public void ChangeObserverOnWorksheetRemoved(Worksheet removed_sheet)
        {
            // Nothing to do. All resources will be removed automatically
        }

        public void ChangeObserverOnWorksheetRenamed(Worksheet removed_sheet)
        {
            // Nothing to do.
        }

        public void ChangeObserverOnRangeInserted(Cells.CellRange moved_range, Cells.Offset offset, Worksheet affected_sheet)
        {
            // Nothing to do. It is not a range
        }

        public void ShiftReferencesTo(ISubject subject, Cells.Offset offset, Worksheet affected_sheet)
        {
            if (subject.Equals(parent_sheet.SubjectsStorage.GetCellItemOrCreate(anchor)))
            {
                parent_sheet.Hyperlinks.MoveHyperlink(anchor, anchor + offset);
            }
        }

        public bool Valid { get { return valid; } }
        public void Invalidate()
        {
            valid = false;
        }

        #endregion

        private bool valid;

        internal Cells.Cell anchor;/* { get; private set; }*/
        private Worksheet parent_sheet;
    }

}
