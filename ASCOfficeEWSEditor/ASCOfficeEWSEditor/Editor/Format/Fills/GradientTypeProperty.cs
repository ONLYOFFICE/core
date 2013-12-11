using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Fills
{
    internal class GradientTypeProperty : Property<GradientType>
    {
        public GradientTypeProperty(GradientType init_value)
            : base(init_value)
        {
        }

        public GradientTypeProperty()
        {
        }

        public override GradientType ExtractValueFromEntity(XmlElement gradient_fill_tag)
        {
            if (gradient_fill_tag == null)
            {
                return GradientType.linear;
            }
            string type_val = gradient_fill_tag.GetAttribute("type");
            switch (type_val)
            {
                case "path":
                    return GradientType.path;
                case "linear":
                default:
                    return GradientType.linear;
            }
        }

        public override void SetToEntity(XmlElement alignment_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value == GradientType.path)
                {
                    alignment_tag.SetAttribute("type", "path");
                }
                else
                {
                    alignment_tag.RemoveAttribute("type");
                }
            }
        }
    }

    /// <summary>
    /// Gradient Type
    /// </summary>
    public enum GradientType
    {
        /// <summary>
        /// Linear gradient type
        /// </summary>
        linear,
        /// <summary>
        /// Path gradient type
        /// </summary>
        path
    }

}
