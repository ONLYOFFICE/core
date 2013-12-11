using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class RemovedStringsCollector : IStatCollector
    {
        public RemovedStringsCollector(Workbook workbook)
        {
            this.workbook = workbook;
            removed_strings = 0;
        }


        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            if (cell_tag.GetAttribute("t") == "s")
            {
                --removed_strings;
            }
            return true;
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            return true; // no cell - no string
        }

        #endregion

        public void DecrementCounters()
        {
            if (removed_strings != 0)
            {
                workbook.SharedStrings.UpdateUsageCounter(removed_strings);
            }
        }


        private int removed_strings;
        private Workbook workbook;

    }
}
