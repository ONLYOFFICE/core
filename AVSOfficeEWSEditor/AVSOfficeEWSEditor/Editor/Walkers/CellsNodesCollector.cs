using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class CellsNodesCollector : IStatCollector
    {
        public CellsNodesCollector()
        {
            found_references = new Dictionary<Cells.Cell, XmlElement>();
        }

        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            found_references.Add(new Cells.Cell(row, col), cell_tag);
            return true;
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            found_references.Add(new Cells.Cell(row, col), null);
            return true;
        }

        #endregion

        public Dictionary<Cells.Cell, XmlElement> FoundReferences { get { return found_references; } }

        private Dictionary<Cells.Cell, XmlElement> found_references;

    }
}
