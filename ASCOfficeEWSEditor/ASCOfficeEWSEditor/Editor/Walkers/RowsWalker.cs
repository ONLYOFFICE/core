using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class RowsWalker
    {
        public RowsWalker(int rowFirst, int rowLast, Worksheet parent_sheet)
        {
            this.rowFirst = rowFirst;
            this.rowLast = rowLast;
            this.sheetData_tag = parent_sheet.SheetData;

            ns_manager = parent_sheet.Workbook.NamespaceManager;
        }


        /// <summary>
        /// Collect statistics from all existent and non-existent rows. 
        /// </summary>
        /// <param name="collector"></param>
        public void CollectStatisticsIncludingEmpty(IRowsCollector collector)
        {
            int last_updated_row = rowFirst - 1;
            foreach (XmlElement row_tag in sheetData_tag.SelectNodes("main:row[@r >= " + rowFirst + " and @r <= " + rowLast + "]", ns_manager))
            {
                int row = XMLInt.ParseOrDefault(row_tag.GetAttribute("r"), 0);
                // If inside the range
                for (int i = last_updated_row + 1; i < row; ++i)
                {
                    if (!collector.CollectIncludingEmpty(null, i))
                    {
                        return;
                    }
                }
                if (!collector.CollectIncludingEmpty(row_tag, row))
                {
                    return;
                }
                last_updated_row = row;
            }

            if (last_updated_row < rowLast)
            {
                XmlNode before = sheetData_tag.SelectSingleNode("main:row[@r > " + rowLast + "]", ns_manager);
                for (int i = last_updated_row + 1; i <= rowLast; ++i)
                {
                    if (!collector.CollectIncludingEmpty(null, i))
                    {
                        return;
                    }
                }
            }
        }

        /// <summary>
        /// Goes through the existent rows among specified ones and update all visited rows according to the passed setter.
        /// </summary>
        /// <param name="setter"></param>
        public void UpdateRows(IRowsSetter setter)
        {
            foreach (XmlElement row_tag in sheetData_tag.SelectNodes("main:row[@r >= " + rowFirst + " and @r <= " + rowLast + "]", ns_manager))
            {
                setter.Set(row_tag, XMLInt.ParseOrDefault(row_tag.GetAttribute("r"), 0));
            }
            setter.PostProcessing();
        }

        /// <summary>
        /// Create absent rows in the range and update all the rows according to the passed setter.
        /// </summary>
        /// <param name="setter"></param>
        public bool SetRows(IRowsSetter setter)
        {
            int last_updated_row = rowFirst - 1;
            foreach (XmlElement row_tag in sheetData_tag.SelectNodes("main:row[@r >= " + rowFirst + " and @r <= " + rowLast + "]", ns_manager))
            {
                int row = XMLInt.ParseOrDefault(row_tag.GetAttribute("r"), 0);
                for (int i = last_updated_row + 1; i < row; ++i)
                {
                    XmlElement row_ins = sheetData_tag.OwnerDocument.CreateElement("row", ns_manager.LookupNamespace("main"));
                    row_ins.SetAttribute("r", i.ToString());
                    sheetData_tag.InsertBefore(row_ins, row_tag);

                    if(!setter.Set(row_ins, i))
                    {
                        return false;
                    }
                }
                if(!setter.Set(row_tag, row))
                {
                    return false;
                }
                last_updated_row = row;
            }

            if(last_updated_row < rowLast)
            {
                XmlNode before = sheetData_tag.SelectSingleNode("main:row[@r > " + rowLast + "]", ns_manager);
                for (int i = last_updated_row + 1; i <= rowLast; ++i)
                {
                    XmlElement row_ins = sheetData_tag.OwnerDocument.CreateElement("row", ns_manager.LookupNamespace("main"));
                    row_ins.SetAttribute("r", i.ToString());
                    sheetData_tag.InsertBefore(row_ins, before);

                    if(!setter.Set(row_ins, i))
                    {
                        return false;
                    }
                }
            }

            setter.PostProcessing();
            return true;
        }

        private int rowFirst;
        private int rowLast;
        private XmlNamespaceManager ns_manager;
        private XmlNode sheetData_tag;
    }
}
