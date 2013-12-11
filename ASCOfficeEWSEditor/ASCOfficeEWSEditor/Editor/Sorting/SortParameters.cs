using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Sorting
{
    /// <summary>
    /// Sort orientation
    /// </summary>
    public enum SortOrientation
    {
        /// <summary>
        /// Sort by rows
        /// </summary>
        by_rows,
        /// <summary>
        /// Sort by columns
        /// </summary>
        by_columns
    }

    /// <summary>
    /// Sort order
    /// </summary>
    public enum SortOrder
    {
        /// <summary>
        /// Sort ascending
        /// </summary>
        ascending,
        /// <summary>
        /// Sort descending
        /// </summary>
        descending
    }

    internal struct ValueRowColumn : IComparable<ValueRowColumn>
    {
        public ValueRowColumn(Calculation.Formulas.DataObjects.IDataObject element, int row_column)
        {
            this.element = element;
            this.row_column = row_column;
        }
        public Calculation.Formulas.DataObjects.IDataObject element;
        public int row_column;

        #region IComparable<ValueRowColumn> Members

        public int CompareTo(ValueRowColumn other)
        {
            return element.CompareTo(other.element);
        }

        #endregion

        public override string ToString()
        {
            return element.ToString() + ", " + row_column;
        }
    }

    internal class DescendingComparer : IComparer<ValueRowColumn>
    {
        #region IComparer<ValueRowColumn> Members

        public int Compare(ValueRowColumn x, ValueRowColumn y)
        {
            if (x.element is Calculation.Formulas.DataObjects.ObjectEmpty || y.element is Calculation.Formulas.DataObjects.ObjectEmpty)
            {
                return x.CompareTo(y); // This places PtgEmty to the end in every case
            }
            return -x.CompareTo(y);
        }

        #endregion
    }

}
