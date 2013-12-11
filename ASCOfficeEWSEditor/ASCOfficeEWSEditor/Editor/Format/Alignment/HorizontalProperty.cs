using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Alignment
{
    internal class HorizontalProperty : Property<HorizontalAlignmentType>
    {
        public HorizontalProperty(HorizontalAlignmentType init_value, Workbook workbook)
            : base(init_value)
        {
            this.workbook = workbook;
        }

        public HorizontalProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override HorizontalAlignmentType ExtractValueFromEntity(XmlElement alignment_tag)
        {
            if (alignment_tag == null)
            {
                return HorizontalAlignmentType.general;
            }
            string scheme_val = alignment_tag.GetAttribute("horizontal");
            switch (scheme_val)
            {
                case "center":
                    return HorizontalAlignmentType.center;
                case "centerContinuous":
                    return HorizontalAlignmentType.centerContinuous;
                case "distributed":
                    return HorizontalAlignmentType.distributed;
                case "fill":
                    return HorizontalAlignmentType.fill;
                case "justify":
                    return HorizontalAlignmentType.justify;
                case "left":
                    return HorizontalAlignmentType.left;
                case "right":
                    return HorizontalAlignmentType.right;
                case "general":
                default:
                    return HorizontalAlignmentType.general;
            }
        }

        public override void SetToEntity(XmlElement alignment_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != HorizontalAlignmentType.general)
                {
                    alignment_tag.SetAttribute("horizontal", Value.Value.ToString());
                }
                else
                {
                    alignment_tag.RemoveAttribute("horizontal");
                }
            }
        }


        private Workbook workbook;
    }

    /// <summary>
    /// Horizontal Alignment Type
    /// </summary>
    public enum HorizontalAlignmentType
    {
        /// <summary>
        /// Centered
        /// </summary>
        center,
        /// <summary>
        /// centered across multiple cells
        /// </summary>
        centerContinuous,
        /// <summary>
        /// Distributed across the width of the cell
        /// </summary>
        distributed,
        /// <summary>
        /// Filled across the entire width of the cell
        /// </summary>
        fill,
        /// <summary>
        /// General-aligned
        /// </summary>
        general,
        /// <summary>
        /// Justified
        /// </summary>
        justify,
        /// <summary>
        /// Left-aligned
        /// </summary>
        left,
        /// <summary>
        /// Right-aligned
        /// </summary>
        right

    }


}
