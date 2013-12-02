using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Borders
{
    internal class StyleProperty : Property<BorderStyle>
    {
        public StyleProperty(BorderStyle init_value, Workbook workbook)
            : base(init_value)
        {
            this.workbook = workbook;
        }

        public StyleProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override BorderStyle ExtractValueFromEntity(XmlElement border_part_tag)
        {
            if (border_part_tag == null)
            {
                return BorderStyle.none;
            }
            string style_val = border_part_tag.GetAttribute("style");
            switch (style_val)
            {
                case "dashDot":
                    return BorderStyle.dashDot;
                case "dashDotDot":
                    return BorderStyle.dashDotDot;
                case "dashed":
                    return BorderStyle.dashed;
                case "dotted":
                    return BorderStyle.dotted;
                case "double":
                    return BorderStyle.doubleLine;
                case "hair":
                    return BorderStyle.hair;
                case "medium":
                    return BorderStyle.medium;
                case "mediumDashDot":
                    return BorderStyle.mediumDashDot;
                case "mediumDashDotDot":
                    return BorderStyle.mediumDashDotDot;
                case "mediumDashed":
                    return BorderStyle.mediumDashed;
                case "slantDashDot":
                    return BorderStyle.slantDashDot;
                case "thick":
                    return BorderStyle.thick;
                case "thin":
                    return BorderStyle.thin;
                case "none":
                default:
                    return BorderStyle.none;
            }
        }

        public override void SetToEntity(XmlElement alignment_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != BorderStyle.none)
                {
                    if (Value.Value == BorderStyle.doubleLine)
                    {
                        alignment_tag.SetAttribute("style", "double");
                    }
                    else
                    {
                        alignment_tag.SetAttribute("style", Value.Value.ToString());
                    }
                }
                else
                {
                    alignment_tag.RemoveAttribute("style");
                }
            }
        }


        private Workbook workbook;
    }

    /// <summary>
    /// Border Style Type
    /// </summary>
    public enum BorderStyle
    {
        /// <summary>
        /// Dash-dot
        /// </summary>
        dashDot,
        /// <summary>
        /// Dash-dot-dot
        /// </summary>
        dashDotDot,
        /// <summary>
        /// Dashed
        /// </summary>
        dashed,
        /// <summary>
        /// Dotted
        /// </summary>
        dotted,
        /// <summary>
        /// Double line
        /// </summary>
        doubleLine,
        /// <summary>
        /// Hairline
        /// </summary>
        hair,
        /// <summary>
        /// Medium
        /// </summary>
        medium,
        /// <summary>
        /// Medium dash-dot
        /// </summary>
        mediumDashDot,
        /// <summary>
        /// Medium dash-dot-dot
        /// </summary>
        mediumDashDotDot,
        /// <summary>
        /// Medium dashed
        /// </summary>
        mediumDashed,
        /// <summary>
        /// No border visible
        /// </summary>
        none,
        /// <summary>
        /// Slant-dash-dot
        /// </summary>
        slantDashDot,
        /// <summary>
        /// Thick
        /// </summary>
        thick,
        /// <summary>
        /// Thin
        /// </summary>
        thin
    }


}
