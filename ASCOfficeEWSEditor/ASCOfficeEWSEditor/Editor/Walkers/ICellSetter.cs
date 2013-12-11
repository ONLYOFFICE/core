using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    public interface ICellSetter
    {
        bool Set(int row, int col, XmlElement cell_tag);
        void PostProcessing();
    }
}
