using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.OOX
{
    internal interface IOrderedSections
    {
        void AppendXmlSection(XmlNode section_node);
    }
}
