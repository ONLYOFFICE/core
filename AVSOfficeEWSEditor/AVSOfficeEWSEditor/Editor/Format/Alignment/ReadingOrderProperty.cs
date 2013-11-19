using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Alignment
{
    internal class ReadingOrderProperty : Property<ReadingOrderType>
    {
        public ReadingOrderProperty(ReadingOrderType init_value, Workbook workbook)
            : base(init_value)
        {
            this.workbook = workbook;
        }

        public ReadingOrderProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override ReadingOrderType ExtractValueFromEntity(XmlElement alignment_tag)
        {
            if (alignment_tag == null)
            {
                return ReadingOrderType.ContextDependent;
            }
            string scheme_val = alignment_tag.GetAttribute("readingOrder");
            switch (scheme_val)
            {
                case "1":
                    return ReadingOrderType.LeftToRight;
                case "2":
                    return ReadingOrderType.RightToLeft;
                case "0":
                default:
                    return ReadingOrderType.ContextDependent;
            }
        }

        public override void SetToEntity(XmlElement alignment_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != ReadingOrderType.ContextDependent)
                {
                    alignment_tag.SetAttribute("readingOrder", Value.Value == ReadingOrderType.LeftToRight ? "1" : "2");
                }
                else
                {
                    alignment_tag.RemoveAttribute("readingOrder");
                }
            }
        }


        private Workbook workbook;
    }

    /// <summary>
    /// Reading Order Type
    /// </summary>
    public enum ReadingOrderType
    {
        /// <summary>
        /// reading order is determined by scanning the text for the first non-whitespace character:
        /// if it is a strong right-to-left character, the reading order is right-to-left; otherwise, the reading order left-to-right.
        /// </summary>
        ContextDependent,
        /// <summary>
        /// reading order is left-to-right in the cell, as in English.
        /// </summary>
        LeftToRight,
        /// <summary>
        /// reading order is right-to-left in the cell, as in Hebrew
        /// </summary>
        RightToLeft
    }


}
