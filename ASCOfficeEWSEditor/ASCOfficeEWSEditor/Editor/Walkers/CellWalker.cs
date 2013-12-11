using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class CellWalker
    {
        public CellWalker(Cells.Cell cell_coords, Worksheet parent_sheet)
        {
            this.cell_coords = cell_coords;
            this.parent_sheet = parent_sheet;
            this.sheetData_tag = parent_sheet.SheetData;

            ns_manager = parent_sheet.Workbook.NamespaceManager;
        }


        /// <summary>
        /// Collect statistics from all existent cells. 
        /// Note: Don't use if you want to collect after setting. Use ChangeCells with two parameters for that purpose
        /// </summary>
        /// <param name="collector"></param>
        public void CollectStatistics(IStatCollector collector)
        {
            XmlElement cell_tag_found = null; //(XmlElement)sheetData_tag.SelectSingleNode("main:row[@r = " + cell_coords.Row + "]/main:c[@r = '" + cell_coords.ToString() + "']", ns_manager);

            string row_str = cell_coords.Row.ToString();
            string cell_str = cell_coords.ToString();
            XmlNodeList oXmlNodesRowList = sheetData_tag.SelectNodes("main:row", ns_manager);
            try
            {
                XmlElement row_tag = (XmlElement)oXmlNodesRowList[0];
                while (null != row_tag)
                {
                    if (row_tag.GetAttribute("r") == row_str)
                    {
                        XmlNodeList oXmlNodesC = row_tag.SelectNodes("main:c", ns_manager);
                        try
                        {
                            XmlElement cell_tag = (XmlElement)oXmlNodesC[0];
                            while (null != cell_tag)
                            {
                                if (cell_tag.GetAttribute("r") == cell_str)
                                {
                                    cell_tag_found = cell_tag;
                                    break;
                                }
                                cell_tag = (XmlElement)cell_tag.NextSibling;
                            }
                        }
                        catch { }
                        if (null != cell_tag_found)
                            break;
                    }
                    row_tag = (XmlElement)row_tag.NextSibling;
                }
            }
            catch { }

            if(cell_tag_found != null)
            {
                collector.Collect(cell_coords.Row, cell_coords.Column, cell_tag_found);
            }
            else
            {
                collector.Collect(cell_coords.Row, cell_coords.Column, parent_sheet);
            }
        }

        /// <summary>
        /// Create the cell if absent and update cells according to the passed setter.
        /// </summary>
        /// <param name="setter">Any setter</param>
        public void ChangeValues(ICellSetter setter)
        {
            XmlElement row_tag = (XmlElement)sheetData_tag.SelectSingleNode("main:row[@r = " + cell_coords.Row + "]", ns_manager);
            if(row_tag == null)
            {
                row_tag = sheetData_tag.OwnerDocument.CreateElement("row", ns_manager.LookupNamespace("main"));
                row_tag.SetAttribute("r", cell_coords.Row.ToString());
                sheetData_tag.InsertBefore(row_tag, sheetData_tag.SelectSingleNode("main:row[@r > " + cell_coords.Row + "]", ns_manager));
            }
            XmlElement cell_tag = (XmlElement)row_tag.SelectSingleNode("main:c[@r = '" + cell_coords.ToString() + "']", ns_manager);
            if (cell_tag == null)
            {
                cell_tag = sheetData_tag.OwnerDocument.CreateElement("c", ns_manager.LookupNamespace("main"));
                cell_tag.SetAttribute("r", cell_coords.ToString());

                try
                {
                    XmlNodeList oXmlNodeListC = row_tag.SelectNodes("main:c", ns_manager);
                    XmlElement insertion_place = (XmlElement)oXmlNodeListC[0];
                    while (null != insertion_place)
                    {
                        int col = Helpers.ReferencesFormat.str2column(cell_tag.GetAttribute("r"));
                        if (col > cell_coords.Column)
                        {
                            row_tag.InsertBefore(cell_tag, insertion_place);
                            break;
                        }
                        insertion_place = (XmlElement)insertion_place.NextSibling;
                    }
                }
                catch { }

                if(cell_tag.ParentNode == null)
                {
                    row_tag.AppendChild(cell_tag);
                }
            }
            setter.Set(cell_coords.Row, cell_coords.Column, cell_tag);
            setter.PostProcessing();
        }

/*
        /// <summary>
        /// Collect statistics from all existent cells and all non-existent cells. 
        /// Note: Don't use if you want to collect after setting. Use ChangeCells with two parameters for that purpose
        /// </summary>
        /// <param name="collector"></param>
        public void CollectStatisticsIncludingEmpty(IStatCollector collector)
        {
            rows_walker.CollectStatisticsIncludingEmpty(new RowsCellCollector(range_coords, sheetData_tag, collector));
        }

        /// <summary>
        /// Goes through the existent cells among specified ones and update all visited cells according to the passed setter.
        /// This means that absent rows and cells are not created
        /// </summary>
        /// <param name="setter"></param>
        /// <param name="collector">Any collector. May be null if you don't want to collect</param>
        /// <returns></returns>
        public bool UpdateValues(ICellSetter setter, IStatCollector collector)
        {
            bool collector_issued = false;

            foreach (XmlElement row_tag in sheetData_tag.SelectNodes("main:row[@r >= " + range_coords.RowFirst + " and @r <= " + range_coords.RowLast + "]", ns_manager))
            {
                int row = XMLInt.Parse(((XmlElement)row_tag).GetAttribute("r"), "@r");

                foreach (XmlElement cell_tag in row_tag.SelectNodes("main:c", ns_manager))
                {
                    int col = Helpers.ReferencesFormat.str2column(cell_tag.GetAttribute("r"));
                    if (col >= range_coords.ColumnFirst && col <= range_coords.ColumnLast)
                    {
                        if(!setter.Set(row, col, cell_tag))
                        {
                            return collector_issued;
                        }
                        if (collector != null)
                        {
                            collector.Collect(row, col, cell_tag);
                        }
                    }
                }
            }
            setter.PostProcessing();
            return collector_issued;
        }
*/

        private Cells.Cell cell_coords;
        private XmlNode sheetData_tag;
        private XmlNamespaceManager ns_manager;
        private Worksheet parent_sheet;
    }
}
