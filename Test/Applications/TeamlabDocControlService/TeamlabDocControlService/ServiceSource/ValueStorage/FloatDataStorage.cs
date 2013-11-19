using System;
using System.Collections.Generic;
using System.Text;

namespace TeamlabDocControlService.ServiceSource.ValueStorage
{
    class FloatDataStorage: DataStorage<float>
    {
        public override string ToXML()
        {
            List<IDataItem<float>> items = GetItems();
            String xml = "<storage type='float'>";
            foreach (IDataItem<float> item in items)
            {
                xml += String.Format("<item time='{0}' val='{1}'/>", item.getDate().ToString("o"), item.get());
            }
            xml += "</storage>";
            return xml;
        }
    }
}
