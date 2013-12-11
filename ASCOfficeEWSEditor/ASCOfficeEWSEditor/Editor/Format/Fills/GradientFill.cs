using System;
using System.Collections.Generic;
using System.Text;
using ASCOfficeEWSEditor.Editor.Walkers;

namespace ASCOfficeEWSEditor.Editor.Format.Fills
{
    /// <summary>
    /// Represents a set of cells gradient fill properties.
    /// Has permanent link to the range where it was created from
    /// </summary>
    public class GradientFill
    {
        internal GradientFill(RangeWalker range_walker, Workbook workbook)
        {
            this.range_walker = range_walker;
            this.workbook = workbook;
        }

        /// <summary>
        /// Get/set the fill type of the cells
        /// </summary>
        public int? Degree
        {
            get
            {
                DegreeProperty degree_property = new DegreeProperty();
                GradientFillProperty property = new GradientFillProperty(degree_property, workbook); 
                StyleCollector collector = new StyleCollector(new FillEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);
                return degree_property.Value;
            }
            set
            {
                if(value.HasValue)
                {
                    DegreeProperty degree_property = new DegreeProperty(value.Value);
                    GradientFillProperty property = new GradientFillProperty(degree_property, workbook);
                    StyleSetter setter = new StyleSetter(new FillEntity(workbook), property, workbook);
                    range_walker.SetStyles(setter);
                }
            }
        }

        /// <summary>
        /// Get/set the gradient type of the cells
        /// </summary>
        public GradientType? Type
        {
            get
            {
                GradientTypeProperty gradient_type_property = new GradientTypeProperty();
                GradientFillProperty property = new GradientFillProperty(gradient_type_property, workbook);
                StyleCollector collector = new StyleCollector(new FillEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return gradient_type_property.Value;
            }
            set
            {
                GradientTypeProperty gradient_type_property = new GradientTypeProperty(value.Value);
                GradientFillProperty property = new GradientFillProperty(gradient_type_property, workbook);
                StyleSetter setter = new StyleSetter(new FillEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the position of the left edge of the inner rectangle (color 1).
        /// </summary>
        public double? Left
        {
            get
            {
                LeftProperty degree_property = new LeftProperty();
                GradientFillProperty property = new GradientFillProperty(degree_property, workbook);
                StyleCollector collector = new StyleCollector(new FillEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);
                return degree_property.Value;
            }
            set
            {
                LeftProperty degree_property = new LeftProperty(value.Value);
                GradientFillProperty property = new GradientFillProperty(degree_property, workbook);
                StyleSetter setter = new StyleSetter(new FillEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the position of the right edge of the inner rectangle (color 1).
        /// </summary>
        public double? Right
        {
            get
            {
                RightProperty degree_property = new RightProperty();
                GradientFillProperty property = new GradientFillProperty(degree_property, workbook);
                StyleCollector collector = new StyleCollector(new FillEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);
                return degree_property.Value;
            }
            set
            {
                RightProperty degree_property = new RightProperty(value.Value);
                GradientFillProperty property = new GradientFillProperty(degree_property, workbook);
                StyleSetter setter = new StyleSetter(new FillEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the position of the top edge of the inner rectangle (color 1).
        /// </summary>
        public double? Top
        {
            get
            {
                TopProperty degree_property = new TopProperty();
                GradientFillProperty property = new GradientFillProperty(degree_property, workbook);
                StyleCollector collector = new StyleCollector(new FillEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);
                return degree_property.Value;
            }
            set
            {
                TopProperty degree_property = new TopProperty(value.Value);
                GradientFillProperty property = new GradientFillProperty(degree_property, workbook);
                StyleSetter setter = new StyleSetter(new FillEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the position of the bottom edge of the inner rectangle (color 1).
        /// </summary>
        public double? Bottom
        {
            get
            {
                BottomProperty degree_property = new BottomProperty();
                GradientFillProperty property = new GradientFillProperty(degree_property, workbook);
                StyleCollector collector = new StyleCollector(new FillEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);
                return degree_property.Value;
            }
            set
            {
                BottomProperty degree_property = new BottomProperty(value.Value);
                GradientFillProperty property = new GradientFillProperty(degree_property, workbook);
                StyleSetter setter = new StyleSetter(new FillEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Returns Color object that is responsible for getting and setting of gradient stop color at the specified position.
        /// The functionality is limited. The only setting new and getting of existent stops is possible.
        /// No interface to get the whole set of stops was implemented.
        /// </summary>
        /// <param name="position">Position of the color (from 0 to 1 inclusively)</param>
        /// <returns></returns>
        public Colors.Color Color(double position)
        {
            Colors.ColorProperty color_property = new Colors.FgColorProperty("color", workbook);
            StopProperty stop_property = new StopProperty(position, color_property, workbook);
            GradientFillProperty property = new GradientFillProperty(stop_property, workbook);
            StyleCollector collector = new StyleCollector(new FillEntity(workbook), property, workbook);
            range_walker.CollectStatisticsIncludingEmpty(collector);

            StyleSetter style_setter = new StyleSetter(new FillEntity(workbook), property, workbook);
            if (property.IsNull())
            {
                color_property.Value = new Colors.FgColor(workbook);
                return new Colors.Color(null, range_walker, style_setter, color_property, workbook);
            }
            return new Colors.Color(color_property.Value, range_walker, style_setter, color_property, workbook);
        }

        private Workbook workbook;
        private RangeWalker range_walker;
    }
    public class GradientFillSimple
    {
        internal GradientFillSimple()
        {
        }
        public Colors.Color Color;
        public GradientType? Type;
    }

}
