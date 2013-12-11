using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    public interface IStatCollector
    {
        /// <summary>
        /// Issued when the cell exists
        /// </summary>
        /// <param name="row"></param>
        /// <param name="col"></param>
        /// <param name="cell_tag"></param>
        /// <returns></returns>
        bool Collect(int row, int col, XmlElement cell_tag);
        /// <summary>
        /// Issued when the cell doesn't exist
        /// </summary>
        /// <param name="row"></param>
        /// <param name="col"></param>
        /// <param name="parent_sheet"></param>
        /// <returns></returns>
        bool Collect(int row, int col, Worksheet parent_sheet);
    }
}
