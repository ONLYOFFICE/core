using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Font
{
    internal class CharsetProperty : Property<int>
    {
        public CharsetProperty(int init_value, Workbook workbook) : base(init_value)
        {
            this.workbook = workbook;
        }

        public CharsetProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override int ExtractValueFromEntity(XmlElement font_tag)
        {
            XmlElement charset_tag = (XmlElement)font_tag.SelectSingleNode("main:charset", workbook.NamespaceManager);
            if (charset_tag == null)
            {
                return 0;
            }
            return XMLInt.ParseOrDefault(charset_tag.GetAttribute("val"), 0);
        }

        public override void SetToEntity(XmlElement font_tag)
        {
            if (Value.HasValue)
            {
                XmlElement charset_tag = (XmlElement)font_tag.SelectSingleNode("main:charset", workbook.NamespaceManager);
                if (charset_tag == null)
                {
                    charset_tag = font_tag.OwnerDocument.CreateElement("charset", workbook.NamespaceManager.LookupNamespace("main"));
                    font_tag.AppendChild(charset_tag);
                }
                charset_tag.SetAttribute("val", Value.Value.ToString());
            }
        }

        private Workbook workbook;

    }
}
