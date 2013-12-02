using System;
using System.Collections.Generic;
using System.Text;
using AVSOfficeEWSEditor.Editor.Walkers;

namespace AVSOfficeEWSEditor.Editor.Format.Fills
{
    /// <summary>
    /// Represents a set of cells fill properties.
    /// Has permanent link to the range where it was created from
    /// Corresponds to <border/> tag
    /// </summary>
    public class Fills
    {
        internal Fills(RangeWalker range_walker, Workbook workbook)
        {
            PatternFill = new PatternFill(range_walker, workbook);
            GradientFill = new GradientFill(range_walker, workbook);
        }

        /// <summary>
        /// Returns Border object that is responsible for getting and setting of properties of left borders of the cells.
        /// Read-only.
        /// </summary>
        public PatternFill PatternFill { get; private set; }

        /// <summary>
        /// Returns Border object that is responsible for getting and setting of properties of right borders of the cells.
        /// Read-only.
        /// </summary>
        public GradientFill GradientFill { get; private set; }

    }
    public class FillsSimple
    {
        //для нужд быстрой загрузки
        internal FillsSimple()
        {
            PatternFill = new PatternFillSimple();
            GradientFill = new GradientFillSimple();
        }
        public PatternFillSimple PatternFill { get; set; }
        public GradientFillSimple GradientFill { get; set; }
    }
}
