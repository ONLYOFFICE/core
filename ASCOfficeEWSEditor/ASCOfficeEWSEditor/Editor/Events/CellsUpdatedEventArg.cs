using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// Set of arguments for CellsUpdatedEvent event
    /// </summary>
    public class CellsUpdatedEventArg : EventArgs
    {
        internal CellsUpdatedEventArg()
        {
            cell_refs = new List<string>();
        }

        internal void AddCellRef(string cell_ref)
        {
            cell_refs.Add(cell_ref);
        }

        /// <summary>
        /// Returns List of updated cells references
        /// </summary>
        public List<string> UpdatedCells { get { return cell_refs;  } }
        
        /// <summary>
        /// Whether at least one cell changed its value
        /// </summary>
        public bool IsSomethingChanged { get { return cell_refs.Count != 0; } }

        private List<string> cell_refs;
    }
}
