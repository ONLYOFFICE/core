using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal interface IRowsSetter
    {
        bool Set(XmlElement row_tag, int row_num);
        void PostProcessing();
    }
}
