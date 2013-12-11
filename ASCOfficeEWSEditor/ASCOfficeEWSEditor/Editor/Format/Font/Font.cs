using System;
using System.Collections.Generic;
using System.Text;
using ASCOfficeEWSEditor.Editor.Walkers;

namespace ASCOfficeEWSEditor.Editor.Format.Font
{
    /// <summary>
    /// Represents a set of cells font properties.
    /// Has permanent link to the range where it was created from
    /// </summary>
    public class Font
    {
        internal Font(RangeWalker range_walker, Workbook workbook)
        {
            this.range_walker = range_walker;
            this.workbook = workbook;
        }

        /// <summary>
        /// Get/set the weight property of the font.
        /// </summary>
        public bool? Bold
        {
            get
            {
                BoldProperty property = new BoldProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                BoldProperty property = new BoldProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the charset property of the font.
        /// </summary>
        public int? Charset
        {
            get
            {
                CharsetProperty property = new CharsetProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                CharsetProperty property = new CharsetProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the condese property of the font.
        /// </summary>
        public bool? Condese
        {
            get
            {
                CondenseProperty property = new CondenseProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                CondenseProperty property = new CondenseProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the extend property of the font.
        /// </summary>
        public bool? Extend
        {
            get
            {
                ExtendProperty property = new ExtendProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                ExtendProperty property = new ExtendProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the family property of the font.
        /// </summary>
        public int? Family
        {
            get
            {
                FamilyProperty property = new FamilyProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                FamilyProperty property = new FamilyProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the outline property of the font (used on Mac only).
        /// </summary>
        public bool? Outline
        {
            get
            {
                OutlineProperty property = new OutlineProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                OutlineProperty property = new OutlineProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the scheme property of the font.
        /// </summary>
        public SchemeStyle? Scheme
        {
            get
            {
                SchemeProperty property = new SchemeProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                SchemeProperty property = new SchemeProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the italic property of the font.
        /// </summary>
        public bool? Italic
        {
            get
            {
                ItalicProperty property = new ItalicProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                ItalicProperty property = new ItalicProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the underline property of the font.
        /// </summary>
        public bool? Underline
        {
            get
            {
                UnderlineProperty property = new UnderlineProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                UnderlineProperty property = new UnderlineProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the Strikethrough property of the font.
        /// </summary>
        public bool? Strikethrough
        {
            get
            {
                StrikethroughProperty property = new StrikethroughProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                StrikethroughProperty property = new StrikethroughProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the shadow property of the font.
        /// </summary>
        public bool? Shadow
        {
            get
            {
                ShadowProperty property = new ShadowProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                ShadowProperty property = new ShadowProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the superscript property of the font.
        /// </summary>
        public bool? Superscript
        {
            get
            {
                SuperscriptProperty property = new SuperscriptProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                SuperscriptProperty property = new SuperscriptProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the subscript property of the font.
        /// </summary>
        public bool? Subscript
        {
            get
            {
                SubscriptProperty property = new SubscriptProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                SubscriptProperty property = new SubscriptProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the size of the font.
        /// </summary>
        public int? Size
        {
            get
            {
                FontSizeProperty property = new FontSizeProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                FontSizeProperty property = new FontSizeProperty(value.Value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set the name of the font.
        /// </summary>
        public string Name
        {
            get
            {
                FontNameProperty property = new FontNameProperty(workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                FontNameProperty property = new FontNameProperty(value, workbook);
                StyleSetter setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Returns Color object that is responsible for getting and setting of font color of cells.
        /// Read-only.
        /// </summary>
        public Colors.Color Color
        {
            get
            {
                Colors.ColorProperty property = new Colors.FgColorProperty("color", workbook);
                StyleCollector collector = new StyleCollector(new FontEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                StyleSetter style_setter = new StyleSetter(new FontEntity(workbook), property, workbook);
                if (property.IsNull())
                {
                    property.Value = new Colors.FgColor(workbook);
                    return new Colors.Color(null, range_walker, style_setter, property, workbook);
                }
                return new Colors.Color(property.Value, range_walker, style_setter, property, workbook);
                // Terrible code. I know. Don't have time to improve.
            }
            private set{}
        }

        private Workbook workbook;
        private RangeWalker range_walker;
    }
    public class FontSimple
    {
        internal FontSimple()
        {
        }
        public bool? Bold;
        public bool? Italic;
        public bool? Underline;
        public int? Size;
        public string Name;
        public Colors.Color Color;
    }
}
