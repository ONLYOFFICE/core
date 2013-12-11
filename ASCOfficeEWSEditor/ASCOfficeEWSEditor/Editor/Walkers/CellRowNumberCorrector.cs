using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;
using System.Xml.XPath;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class CellRowNumberCorrector : ICellSetter
    {
        public CellRowNumberCorrector(Cells.Offset offset)
        {
            this.offset = offset;
        }

        #region ICellSetter Members

        public bool Set(int row, int col, XmlElement cell_tag)
        {
            cell_tag.SetAttribute("r", (new Cells.Cell(row, col) + offset).ToString());
            return true;
        }

        public void PostProcessing()
        {
        }

        #endregion

        private readonly Cells.Offset offset;
    }
}
