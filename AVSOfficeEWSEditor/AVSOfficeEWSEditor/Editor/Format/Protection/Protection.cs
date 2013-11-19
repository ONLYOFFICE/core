using System;
using System.Collections.Generic;
using System.Text;
using AVSOfficeEWSEditor.Editor.Walkers;

namespace AVSOfficeEWSEditor.Editor.Format.Protection
{
    /// <summary>
    /// Represents a set of cells protection properties.
    /// Has permanent link to the range where it was created from
    /// </summary>
    public class Protection
    {
        internal Protection(RangeWalker range_walker, Workbook workbook)
        {
            this.range_walker = range_walker;
            this.workbook = workbook;
        }

        /// <summary>
        /// Get/set if the cell is hidden.
        /// </summary>
        public bool? Hidden
        {
            get
            {
                HiddenProperty property = new HiddenProperty();
                StyleCollector collector = new StyleCollector(new ProtectionEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                HiddenProperty property = new HiddenProperty(value.Value);
                StyleSetter setter = new StyleSetter(new ProtectionEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        /// <summary>
        /// Get/set if the cell is locked.
        /// </summary>
        public bool? Locked
        {
            get
            {
                LockedProperty property = new LockedProperty();
                StyleCollector collector = new StyleCollector(new ProtectionEntity(workbook), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                LockedProperty property = new LockedProperty(value.Value);
                StyleSetter setter = new StyleSetter(new ProtectionEntity(workbook), property, workbook);
                range_walker.SetStyles(setter);
            }
        }

        private Workbook workbook;
        private RangeWalker range_walker;

    }
}
