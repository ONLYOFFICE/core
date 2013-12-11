using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Alignment
{
    internal class VerticalProperty : Property<VerticalAlignmentType>
    {
        public VerticalProperty(VerticalAlignmentType init_value, Workbook workbook)
            : base(init_value)
        {
            this.workbook = workbook;
        }

        public VerticalProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override VerticalAlignmentType ExtractValueFromEntity(XmlElement alignment_tag)
        {
            if (alignment_tag == null)
            {
                return VerticalAlignmentType.bottom;
            }
            string vert_val = alignment_tag.GetAttribute("vertical");
            switch (vert_val)
            {
                case "center":
                    return VerticalAlignmentType.center;
                case "distributed":
                    return VerticalAlignmentType.distributed;
                case "justify":
                    return VerticalAlignmentType.justify;
                case "top":
                    return VerticalAlignmentType.top;
                case "bottom":
                default:
                    return VerticalAlignmentType.bottom;
            }
        }

        public override void SetToEntity(XmlElement alignment_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != VerticalAlignmentType.bottom)
                {
                    alignment_tag.SetAttribute("vertical", Value.Value.ToString());
                }
                else
                {
                    alignment_tag.RemoveAttribute("vertical");
                }
            }
        }


        private Workbook workbook;
    }

    /// <summary>
    /// Horizontal Alignment Type
    /// </summary>
    public enum VerticalAlignmentType
    {
        /// <summary>
        /// Top-aligned
        /// </summary>
        top,
        /// <summary>
        /// Centered
        /// </summary>
        center,
        /// <summary>
        /// Bottom-aligned
        /// </summary>
        bottom,
        /// <summary>
        /// Justified
        /// </summary>
        justify,
        /// <summary>
        /// Distributed
        /// </summary>
        distributed
    }


}
