using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal interface IRowsCollector
    {
        bool CollectIncludingEmpty(XmlElement row_tag, int row_num);
    }
}
