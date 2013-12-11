using System;
using System.Collections.Generic;
using System.Text;
using ASCOfficeEWSEditor.Editor.Walkers;

namespace ASCOfficeEWSEditor.Editor.Format.Alignment
{
    /// <summary>
    /// Represents a set of cells alignment properties.
    /// Has permanent link to the range where it was created from
    /// </summary>
    public class Alignment
    {
        internal Alignment(RangeWalker range_walker, Workbook workbook)
        {
            this.range_walker = range_walker;
            this.workbook = workbook;
        }


        /// <summary>
        /// Get/set the horizontal alignment in cells.
        /// </summary>
        public HorizontalAlignmentType? Horizontal
        {
            get
            {
                HorizontalProperty property = new HorizontalProperty(workbook);
                StyleCollector collector = new StyleCollector(new AlignmentEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                HorizontalProperty property = new HorizontalProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new AlignmentEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the indentation for text in cells.
        /// </summary>
        public int? Indent
        {
            get
            {
                IndentProperty property = new IndentProperty(workbook);
                StyleCollector collector = new StyleCollector(new AlignmentEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                IndentProperty property = new IndentProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new AlignmentEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set if the cells justified or distributed alignment should be used on the last line of text.
        /// </summary>
        public bool? JustifyLastLine
        {
            get
            {
                JustifyLastLineProperty property = new JustifyLastLineProperty();
                StyleCollector collector = new StyleCollector(new AlignmentEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                JustifyLastLineProperty property = new JustifyLastLineProperty(value.Value);
                StyleSetter setter = new StyleSetter(new AlignmentEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the reading order of the cells.
        /// </summary>
        public ReadingOrderType? ReadingOrder
        {
            get
            {
                ReadingOrderProperty property = new ReadingOrderProperty(workbook);
                StyleCollector collector = new StyleCollector(new AlignmentEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                ReadingOrderProperty property = new ReadingOrderProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new AlignmentEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the rotation of text in cells.
        /// </summary>
        public int? TextRotation
        {
            get
            {
                TextRotationProperty property = new TextRotationProperty(workbook);
                StyleCollector collector = new StyleCollector(new AlignmentEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                TextRotationProperty property = new TextRotationProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new AlignmentEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the horizontal alignment in cells.
        /// </summary>
        public VerticalAlignmentType? Vertical
        {
            get
            {
                VerticalProperty property = new VerticalProperty(workbook);
                StyleCollector collector = new StyleCollector(new AlignmentEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                VerticalProperty property = new VerticalProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new AlignmentEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set if the text in a cell should be line-wrapped within the cell.
        /// </summary>
        public bool? WrapText
        {
            get
            {
                WrapTextProperty property = new WrapTextProperty();
                StyleCollector collector = new StyleCollector(new AlignmentEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                WrapTextProperty property = new WrapTextProperty(value.Value);
                StyleSetter setter = new StyleSetter(new AlignmentEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        private Workbook workbook;
        private RangeWalker range_walker;

    }

    public class AlignmentSimple
    {
        //для нужд быстрой загрузки
        internal AlignmentSimple()
        {
        }
        public HorizontalAlignmentType? Horizontal = HorizontalAlignmentType.right;
        public VerticalAlignmentType? Vertical = VerticalAlignmentType.center;
        public bool? WrapText = false;

        public static string HorizontalAlignmentToString(ref HorizontalAlignmentType? oHorizontal)
        {
            switch (oHorizontal)
            {
                case HorizontalAlignmentType.center:
                    return "center";
                case HorizontalAlignmentType.centerContinuous:
                    return "centerContinuous";
                case HorizontalAlignmentType.distributed:
                    return "distributed";
                case HorizontalAlignmentType.fill:
                    return "fill";
                case HorizontalAlignmentType.general:
                    return "general";
                case HorizontalAlignmentType.justify:
                    return "justify";
                case HorizontalAlignmentType.left:
                    return "left";
                case HorizontalAlignmentType.right:
                default:
                    return "right";
            }
        }
        public static string VerticalAlignmentToString(ref VerticalAlignmentType? oVertical)
        {
            switch (oVertical)
            {
                case VerticalAlignmentType.bottom:
                    return "bottom";
                case VerticalAlignmentType.distributed:
                    return "distributed";
                case VerticalAlignmentType.justify:
                    return "justify";
                case VerticalAlignmentType.top:
                    return "top";
                case VerticalAlignmentType.center:
                default:
                    return "middle";
            }
        }
    }
}
