using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation
{
    internal class WorksheetSubject : Calculation.ISubject
    {
        public WorksheetSubject(Worksheet worksheet)
        {
            Worksheet = worksheet;
            ws_index = worksheet.Index;
        }

        #region IComparable<ISubject> Members

        public int CompareTo(ASCOfficeEWSEditor.Editor.Calculation.ISubject other)
        {
            WorksheetSubject other_subj = other as WorksheetSubject;
            if (other_subj == null)
            {
                return this.GetType().GUID.CompareTo(other.GetType().GUID);
            }
            return ws_index.CompareTo(other_subj.ws_index);
        }

        #endregion

        public Worksheet Worksheet { get; private set; }
        private int ws_index;
    }
}
