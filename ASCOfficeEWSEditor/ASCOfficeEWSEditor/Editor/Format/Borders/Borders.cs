using System;
using System.Collections.Generic;
using System.Text;
using ASCOfficeEWSEditor.Editor.Walkers;

namespace ASCOfficeEWSEditor.Editor.Format.Borders
{
    /// <summary>
    /// Represents a set of cells border properties.
    /// Has permanent link to the range where it was created from
    /// Corresponds to <border/> tag
    /// </summary>
    public class Borders
    {
        internal Borders(RangeWalker range_walker, Workbook workbook)
        {
            this.range_walker = range_walker;
            this.workbook = workbook;
            Left = new Border(BorderPosition.left, range_walker, workbook);
            Right = new Border(BorderPosition.right, range_walker, workbook);
            Top = new Border(BorderPosition.top, range_walker, workbook);
            Bottom = new Border(BorderPosition.bottom, range_walker, workbook);
            Diagonal = new DiagonalBorder(BorderPosition.diagonal, range_walker, workbook);
        }

        /// <summary>
        /// Returns Border object that is responsible for getting and setting of properties of left borders of the cells.
        /// Read-only.
        /// </summary>
        public Border Left { get; private set; }

        /// <summary>
        /// Returns Border object that is responsible for getting and setting of properties of right borders of the cells.
        /// Read-only.
        /// </summary>
        public Border Right { get; private set; }

        /// <summary>
        /// Returns Border object that is responsible for getting and setting of properties of top borders of the cells.
        /// Read-only.
        /// </summary>
        public Border Top { get; private set; }

        /// <summary>
        /// Returns Border object that is responsible for getting and setting of properties of bottom borders of the cells.
        /// Read-only.
        /// </summary>
        public Border Bottom { get; private set; }

        /// <summary>
        /// Returns Border object that is responsible for getting and setting of properties of diagonal borders of the cells.
        /// Read-only.
        /// </summary>
        public DiagonalBorder Diagonal { get; private set; }

        /// <summary>
        /// Get/set if left, right, top, and bottom borders should be applied only to outside borders of a cell range.
        /// </summary>
        public bool? Outline
        {
            get
            {
                OutlineProperty diag_type_property = new OutlineProperty();
                StyleCollector collector = new StyleCollector(new BorderEntity(workbook), diag_type_property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return diag_type_property.Value;
            }
            set
            {
                OutlineProperty diag_type_property = new OutlineProperty(value.Value);
                StyleSetter setter = new StyleSetter(new BorderEntity(workbook), diag_type_property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        private Workbook workbook;
        private RangeWalker range_walker;
    }
    public class BordersSimple
    {
        internal BordersSimple()
        {
        }
        public BorderSimple Left = new BorderSimple();
        public BorderSimple Right = new BorderSimple();
        public BorderSimple Top = new BorderSimple();
        public BorderSimple Bottom = new BorderSimple();
    }
}
