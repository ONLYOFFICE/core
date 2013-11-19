using System;
using System.Collections.Generic;
using System.Text;
using AVSOfficeEWSEditor.Editor.Walkers;

namespace AVSOfficeEWSEditor.Editor.Format.Fills
{
    /// <summary>
    /// Represents a set of cells pattern fill properties.
    /// Has permanent link to the range where it was created from
    /// </summary>
    public class PatternFill
    {
        internal PatternFill(RangeWalker range_walker, Workbook workbook)
        {
            this.range_walker = range_walker;
            this.workbook = workbook;
        }

        /// <summary>
        /// Returns Color object that is responsible for getting and setting of background color of the fill.
        /// Read-only.
        /// </summary>
        public Colors.Color BgColor
        {
            get
            {
                Colors.ColorProperty color_property = new Colors.BgColorProperty("bgColor", workbook);
                PatternFillProperty property = new PatternFillProperty(color_property, workbook); 
                StyleCollector collector = new StyleCollector(new FillEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                StyleSetter style_setter = new StyleSetter(new FillEntity(workbook), property, workbook);
                if (property.IsNull())
                {
                    color_property.Value = new Colors.BgColor(workbook);
                    return new Colors.Color(null, range_walker, style_setter, color_property, workbook);
                }
                return new Colors.Color(color_property.Value, range_walker, style_setter, color_property, workbook);
                // Terrible code. I know. Don't have time to improve.
            }
            private set { }
        }

        /// <summary>
        /// Returns Color object that is responsible for getting and setting of foreground color of the fill.
        /// Read-only.
        /// </summary>
        public Colors.Color FgColor
        {
            get
            {
                Colors.ColorProperty color_property = new Colors.FgColorProperty("fgColor", workbook);
                PatternFillProperty property = new PatternFillProperty(color_property, workbook); 
                StyleCollector collector = new StyleCollector(new FillEntity(workbook), property, workbook);
                range_walker.CollectStatistics(collector);

                StyleSetter style_setter = new StyleSetter(new FillEntity(workbook), property, workbook);
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
        /// Get/set the fill type of the cells
        /// </summary>
        public PatternType? Pattern
        {
            get
            {
                PatternProperty pattern_property = new PatternProperty();
                PatternFillProperty property = new PatternFillProperty(pattern_property, workbook);
                StyleCollector collector = new StyleCollector(new FillEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return pattern_property.Value;
            }
            set
            {
                PatternProperty pattern_property = new PatternProperty(value.Value);
                PatternFillProperty property = new PatternFillProperty(pattern_property, workbook);
                StyleSetter setter = new StyleSetter(new FillEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        

        private Workbook workbook;
        private RangeWalker range_walker;
    }
    public class PatternFillSimple
    {
        internal PatternFillSimple()
        {
        }
        public PatternType? Pattern;
        public Colors.Color FgColor;
        public Colors.Color BgColor;
    }
}
