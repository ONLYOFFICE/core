using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class WidthSetter : IColsSetter
    {
        public WidthSetter(double width)
        {
            this.width = width;
        }

        #region IColsSetter Members

        public bool WouldLeftTheSame(XmlElement col_tag)
        {
            int old_width = XMLInt.ParseOrDefault(col_tag.GetAttribute("width"), 0);
            return width == old_width;
        }

        public void Set(XmlElement col_tag)
        {
            col_tag.SetAttribute("width", XMLDouble.ToString(width));
        }

        #endregion

        private double width;
    }

}
