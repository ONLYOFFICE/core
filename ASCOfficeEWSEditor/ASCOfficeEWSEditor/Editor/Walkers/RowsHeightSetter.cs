using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class RowsHeightSetter : IRowsSetter
    {
        public RowsHeightSetter(double height)
        {
            height_str = XMLDouble.ToString(height);
        }
        #region IRowsSetter Members

        public bool Set(XmlElement row_tag, int row_num)
        {
            row_tag.SetAttribute("ht", height_str);
            row_tag.SetAttribute("customHeight", "true");
            return true;
        }

        public void PostProcessing()
        {
        }

        #endregion

        private string height_str;
    }
}
