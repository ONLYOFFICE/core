using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal interface IColsSetter
    {
        bool WouldLeftTheSame(XmlElement col_tag);
        void Set(XmlElement col_tag);
    }
}
