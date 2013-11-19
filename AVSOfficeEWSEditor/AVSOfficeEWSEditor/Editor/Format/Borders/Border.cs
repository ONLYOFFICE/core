using System;
using System.Collections.Generic;
using System.Text;
using AVSOfficeEWSEditor.Editor.Walkers;

namespace AVSOfficeEWSEditor.Editor.Format.Borders
{
    /// <summary>
    /// Represents a set of cells border properties.
    /// Has permanent link to the range where it was created from
    /// Corresponds to <left/>, <right/>, etc. tag
    /// </summary>
    public class Border
    {
        internal Border(BorderPosition border_position, RangeWalker range_walker, Workbook workbook)
        {
            this.range_walker = range_walker;
            this.workbook = workbook;
            this.border_position = border_position;
        }

        /// <summary>
        /// Returns Color object that is responsible for getting and setting of border color of cells.
        /// Read-only.
        /// </summary>
        public Colors.Color Color
        {
            get
            {
                Colors.ColorProperty color_property = new Colors.FgColorProperty("color", workbook);
                BorderProperty property = new BorderProperty(border_position, color_property, workbook); // Intermediate level to process <left>-like tags
                StyleCollector collector = new StyleCollector(new BorderEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                StyleSetter style_setter = new StyleSetter(new BorderEntity(workbook), property, workbook);
                if (property.IsNull())
                {
                    color_property.Value = new Colors.FgColor(workbook);
                    return new Colors.Color(null, range_walker, style_setter, color_property, workbook);
                }
                return new Colors.Color(color_property.Value, range_walker, style_setter, color_property, workbook);
                // Terrible code. I know. Don't have time to improve.
            }
            private set { }
        }

        /// <summary>
        /// Get/set the style of a border of the cells
        /// </summary>
        public BorderStyle? Style
        {
            get
            {
                StyleProperty style_property = new StyleProperty(workbook);
                BorderProperty property = new BorderProperty(border_position, style_property, workbook); // Intermediate level to process <left>-like tags
                StyleCollector collector = new StyleCollector(new BorderEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return style_property.Value;
            }
            set
            {
                StyleProperty style_property = new StyleProperty(value.Value, workbook);
                BorderProperty property = new BorderProperty(border_position, style_property, workbook); // Intermediate level to process <left>-like tags
                StyleSetter setter = new StyleSetter(new BorderEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }



        private Workbook workbook;
        private RangeWalker range_walker;
        private BorderPosition border_position;
    }


    /// <summary>
    /// Represents a specific set of cells border properties for diagonal borders.
    /// Has permanent link to the range where it was created from
    /// Corresponds to <diagonal/> tag only
    /// </summary>
    public class DiagonalBorder : Border
    {
        internal DiagonalBorder(BorderPosition border_position, RangeWalker range_walker, Workbook workbook) : base(border_position, range_walker, workbook)
        {
            this.range_walker = range_walker;
            this.border_position = border_position;
            this.workbook = workbook;
        }


        /// <summary>
        /// Get/set the style of a border of the cells
        /// </summary>
        public DiagonalType? DiagonalType
        {
            get
            {
                DiagonalTypeProperty diag_type_property = new DiagonalTypeProperty(workbook);
                StyleCollector collector = new StyleCollector(new BorderEntity(workbook), diag_type_property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return diag_type_property.Value;
            }
            set
            {
                DiagonalTypeProperty diag_type_property = new DiagonalTypeProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new BorderEntity(workbook), diag_type_property, workbook);
                range_walker.SetStyles(setter);
            }
        }



        private Workbook workbook;
        private RangeWalker range_walker;
        private BorderPosition border_position;
    }
    
    /// <summary>
    /// Type of a diagonal border
    /// </summary>
    public enum DiagonalType
    {
        /// <summary>
        /// Top left corner to bottom right
        /// </summary>
        Down,
        /// <summary>
        /// Bottom left to top right
        /// </summary>
        Up,
        /// <summary>
        /// Both up and down diagonals
        /// </summary>
        Both,
        /// <summary>
        /// No diagonal line
        /// </summary>
        none
    }

    public class BorderSimple
    {
        internal BorderSimple()
        {
        }
        public Colors.Color Color;
        public BorderStyle? Style;
    }
}
