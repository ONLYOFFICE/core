using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Font
{
    internal class SchemeProperty : Property<SchemeStyle>
    {
        public SchemeProperty(SchemeStyle init_value, Workbook workbook)
            : base(init_value)
        {
            this.workbook = workbook;
        }

        public SchemeProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override SchemeStyle ExtractValueFromEntity(XmlElement font_tag)
        {
            XmlElement scheme_tag = (XmlElement)font_tag.SelectSingleNode("main:scheme", workbook.NamespaceManager);
            if (scheme_tag == null)
            {
                return SchemeStyle.None;
            }
            string scheme_val = scheme_tag.GetAttribute("val");
            switch (scheme_val)
            {
                case "major":
                    return SchemeStyle.Major;
                case "minor":
                    return SchemeStyle.Minor;
                case "none":
                default:
                    return SchemeStyle.None;
            }
        }

        public override void SetToEntity(XmlElement font_tag)
        {
            if (Value.HasValue)
            {
                XmlElement family_tag = (XmlElement)font_tag.SelectSingleNode("main:family", workbook.NamespaceManager);
                if (Value.Value != SchemeStyle.None)
                {
                    if (family_tag == null)
                    {
                        family_tag = font_tag.OwnerDocument.CreateElement("family", workbook.NamespaceManager.LookupNamespace("main"));
                        font_tag.AppendChild(family_tag);
                    }
                    family_tag.SetAttribute("val", Value.Value == SchemeStyle.Major ? "major" : "minor");
                }
                else
                {
                    if (family_tag != null)
                    {
                        font_tag.RemoveChild(family_tag);
                    }
                }
            }
        }

        private Workbook workbook;
    }


    /// <summary>
    /// Font scheme style
    /// </summary>
    public enum SchemeStyle
    {
        /// <summary>
        /// Major font of the scheme
        /// </summary>
        Major,
        /// <summary>
        /// Minor font of the scheme
        /// </summary>
        Minor,
        /// <summary>
        /// The font is not a theme font
        /// </summary>
        None
    }
}
