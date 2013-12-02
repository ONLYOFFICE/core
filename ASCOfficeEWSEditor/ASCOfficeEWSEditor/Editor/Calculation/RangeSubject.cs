using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation
{
    internal class RangeSubject : ISubject
    {
        /// <summary>
        /// Construct non-observer, just subject
        /// </summary>
        public RangeSubject(Cells.CellRange range, Worksheet parent_sheet, ValuesUpdateManager manager)
        {
            this.range = range;
            this.parent_sheet = parent_sheet;
            this.manager = manager;
        }

        #region ISubject Members

        #endregion

        public override string ToString()
        {
            return range.ToString();
        }

        #region IComparable<ISubject> Members

        public int CompareTo(ISubject other)
        {
            if (!(other is RangeSubject))
            {
                return this.GetType().GUID.CompareTo(other.GetType().GUID);
            }
            RangeSubject other_subj = (RangeSubject)other;
            return range.CompareTo(other_subj.range);
        }

        #endregion

        internal Cells.CellRange CellRange { get { return range; } }

        private Cells.CellRange range;
        private Worksheet parent_sheet;
        
        private ValuesUpdateManager manager;
    }

}
