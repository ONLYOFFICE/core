using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Font
{
    internal class FontNameProperty : PropertyString
    {
        public FontNameProperty(string init_value, Workbook workbook) : base(init_value)
        {
            this.workbook = workbook;
        }

        public FontNameProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override string ExtractValueFromEntity(XmlElement font_tag)
        {
            XmlElement name_tag = (XmlElement)font_tag.SelectSingleNode("main:name", workbook.NamespaceManager);
            string name;
            if (name_tag != null && (name = name_tag.GetAttribute("val")) != "")
            {
                return name;
            }
            return workbook.StyleSheet.GetZeroStyleOrDefaultFontName();
        }

        public override void SetToEntity(XmlElement font_tag)
        {
            if (Value != null)
            {
                XmlElement name_tag = (XmlElement)font_tag.SelectSingleNode("main:name", workbook.NamespaceManager);
                if (name_tag == null)
                {
                    name_tag = font_tag.OwnerDocument.CreateElement("name", workbook.NamespaceManager.LookupNamespace("main"));
                    font_tag.AppendChild(name_tag);
                }
                name_tag.SetAttribute("val", Value);
            }
        }

        private Workbook workbook;
    }
}
