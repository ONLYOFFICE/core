using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Font
{
    internal class FontSizeProperty : Property<int>
    {
        public FontSizeProperty(int init_value, Workbook workbook) : base(init_value)
        {
            this.workbook = workbook;
        }

        public FontSizeProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override int ExtractValueFromEntity(XmlElement font_tag)
        {
            XmlElement sz_tag = (XmlElement)font_tag.SelectSingleNode("main:sz", workbook.NamespaceManager);
            int sz;
            if (sz_tag != null && (sz = XMLInt.ParseOrDefault(sz_tag.GetAttribute("val"), -1)) != -1)
            {
                return sz;
            }
            return workbook.StyleSheet.GetZeroStyleOrDefaultFontSize();
        }

        public override void SetToEntity(XmlElement font_tag)
        {
            if (Value.HasValue)
            {
                XmlElement sz_tag = (XmlElement)font_tag.SelectSingleNode("main:sz", workbook.NamespaceManager);
                if (sz_tag == null)
                {
                    sz_tag = font_tag.OwnerDocument.CreateElement("sz", workbook.NamespaceManager.LookupNamespace("main"));
                    font_tag.AppendChild(sz_tag);
                }
                sz_tag.SetAttribute("val", Value.Value.ToString());
            }
        }
        
        private Workbook workbook;
    }
}
