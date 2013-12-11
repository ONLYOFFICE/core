using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;
using System.Diagnostics;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    public class RangeWalker
    {
        public RangeWalker(Cells.CellRange range_coords, Worksheet parent_sheet)
        {
            this.range_coords = range_coords;
            this.parent_sheet = parent_sheet;
            this.sheetData_tag = parent_sheet.SheetData;

            ns_manager = parent_sheet.Workbook.NamespaceManager;

            rows_walker = new RowsWalker(range_coords.RowFirst, range_coords.RowLast, parent_sheet);
            cols_walker = new ColsWalker(range_coords.ColumnFirst, range_coords.ColumnLast, parent_sheet);
        }


        /// <summary>
        /// Collect statistics from all existent cells. 
        /// Note: Don't use if you want to collect after setting. Use ChangeCells with two parameters for that purpose
        /// </summary>
        /// <param name="collector"></param>
        public void CollectStatistics(IStatCollector collector)
        {
            if (range_coords.IsSingleCell)
            {
                new CellWalker(range_coords.TopLeft, parent_sheet).CollectStatistics(collector);
                return;
            }
            int last_updated_row = range_coords.RowFirst - 1;
            List<int[]> empty_row_cells = new List<int[]>();
            int empty_row_cells_row_list = -1;
            int empty_row_cells_row = -1;
            //int collector_issued_times = 0;

            CellSimpleGetter oCellSimple = collector as CellSimpleGetter;
            if (null != oCellSimple)
            {
                XmlNodeList oXmlCollsList = sheetData_tag.ParentNode.SelectNodes("main:cols", ns_manager);
                if (null != oXmlCollsList && 0 < oXmlCollsList.Count)
                {
                    XmlElement oCollTag = (XmlElement)oXmlCollsList[0].FirstChild;
                    while (null != oCollTag)
                    {
                        Format.CCollFormat oCollFormat;

                        oCollFormat.m_nMin = XMLInt.ParseOrDefault(((XmlElement)oCollTag).GetAttribute("min"), 0);
                        oCollFormat.m_nMax = XMLInt.ParseOrDefault(((XmlElement)oCollTag).GetAttribute("max"), 0);
                        int nStyleColl = XMLInt.ParseOrDefault(((XmlElement)oCollTag).GetAttribute("style"), 0);
                        oCollFormat.m_oFormatSimple = oCellSimple.GetFormat(nStyleColl);
                        oCellSimple.m_arrCollsDefaultStyle.Add(oCollFormat);

                        oCollTag = (XmlElement)oCollTag.NextSibling;
                    }
                }
            }

            XmlNodeList oXmlNodeList = sheetData_tag.SelectNodes("main:row", ns_manager);
            try
            {
                XmlElement row_tag = (XmlElement)oXmlNodeList[0];
                while (null != row_tag)
                {
                    int row = XMLInt.ParseOrDefault(((XmlElement)row_tag).GetAttribute("r"), 0);
                    if (row >= range_coords.RowFirst)
                        break;
                    row_tag = (XmlElement)row_tag.NextSibling;
                }

                while (null != row_tag)
                {
                    int row = XMLInt.ParseOrDefault(((XmlElement)row_tag).GetAttribute("r"), 0);
                    if (row > range_coords.RowLast)
                        break;

                    if (last_updated_row + 1 < row && -1 == empty_row_cells_row)
                        empty_row_cells_row = last_updated_row + 1;

                    int last_updated_column = range_coords.ColumnFirst - 1;
                    bool row_has_style = row_tag.GetAttribute("s").Length != 0;

                    if (null != oCellSimple)
                    {
                        int nCustomFormat = XMLInt.ParseOrDefault(((XmlElement)row_tag).GetAttribute("customFormat"), -1);
                        if (-1 != nCustomFormat)
                            oCellSimple.m_arrRowsDefaultStyle.Add(row, oCellSimple.GetFormat(nCustomFormat));
                    }

                    bool bFirst = true;
                    try
                    {
                        XmlNodeList oXmlNodeListCells = row_tag.SelectNodes("main:c", ns_manager);
                        XmlElement cell_tag = (XmlElement)oXmlNodeListCells[0];
                        while (null != cell_tag)
                        {
                            int col = Helpers.ReferencesFormat.str2column(cell_tag.GetAttribute("r"));

                            if (col > range_coords.ColumnLast)
                            {
                                break;
                            }

                            if (col >= range_coords.ColumnFirst)
                            {
                                if (col > last_updated_column + 1)
                                {
                                    if (row_has_style)
                                    {
                                        collector.Collect(row, last_updated_column + 1, parent_sheet);
                                    }
                                    else if (-1 == empty_row_cells_row)
                                    {
                                        empty_row_cells_row_list = row;
                                        if (true == bFirst)
                                        {
                                            bFirst = false;
                                            empty_row_cells.Clear();
                                        }
                                        int[] aPair = new int[2];
                                        aPair[0] = last_updated_column + 1;
                                        aPair[1] = col - 1;
                                        empty_row_cells.Add(aPair);
                                    }
                                }
                                if (!collector.Collect(row, col, cell_tag))
                                {
                                    return; // Stop collecting if 'collector' wishes
                                }
                                last_updated_column = col;
                            }

                            cell_tag = (XmlElement)cell_tag.NextSibling;
                        }
                    }
                    catch { }

                    if (last_updated_column != range_coords.ColumnLast)
                    {
                        if (row_has_style)
                        {
                            collector.Collect(row, last_updated_column + 1, parent_sheet);
                        }
                        else if (-1 == empty_row_cells_row)
                        {
                            empty_row_cells_row_list = row;
                            if (true == bFirst)
                            {
                                bFirst = false;
                                empty_row_cells.Clear();
                            }
                            int[] aPair = new int[2];
                            aPair[0] = last_updated_column + 1;
                            aPair[1] = range_coords.ColumnLast;
                            empty_row_cells.Add(aPair);
                        }
                    }
                    last_updated_row = row;

                    row_tag = (XmlElement)row_tag.NextSibling;
                }
            }
            catch { }

            if (last_updated_row < range_coords.RowLast && -1 == empty_row_cells_row)
                empty_row_cells_row = last_updated_row + 1;

            if (-1 != empty_row_cells_row)
            {
                for (int col = range_coords.ColumnFirst; col <= range_coords.ColumnLast; col++)
                    collector.Collect(empty_row_cells_row, col, parent_sheet);
            }
            else if (-1 != empty_row_cells_row_list)
            {
                for (int i = 0; i < empty_row_cells.Count; i++)
                    for (int col = empty_row_cells[i][0]; col <= empty_row_cells[i][1]; col++)
                        collector.Collect(empty_row_cells_row_list, col, parent_sheet);
            }
        }

        /// <summary>
        /// Collect statistics from all existent cells and all non-existent cells. 
        /// Note: Don't use if you want to collect after setting. Use ChangeCells with two parameters for that purpose
        /// </summary>
        /// <param name="collector"></param>
        public void CollectStatisticsIncludingEmpty(IStatCollector collector)
        {
            rows_walker.CollectStatisticsIncludingEmpty(new RowsCellCollector(range_coords, parent_sheet, collector));
        }

        /// <summary>
        /// Create absent cells in the range and update all the cells according to the passed setter.
        /// Note: Doesn't stop on setter or collector signals
        /// </summary>
        /// <param name="setter">Any setter</param>
        public bool ChangeValues(ICellSetter setter)
        {
            return rows_walker.SetRows(new RowsCellSetter(range_coords, setter, parent_sheet));
        }

        /// <summary>
        /// Goes through the existent cells among specified ones and update all visited cells according to the passed setter.
        /// This means that absent rows and cells are not created
        /// </summary>
        /// <param name="setter"></param>
        /// <returns></returns>
        public void UpdateValues(ICellSetter setter)
        {
            foreach (XmlElement row_tag in sheetData_tag.SelectNodes("main:row", ns_manager))
            {
                int row = XMLInt.ParseOrDefault(((XmlElement)row_tag).GetAttribute("r"), 0);

                if (row >= range_coords.RowFirst && row <= range_coords.RowLast)
                {
                    foreach (XmlElement cell_tag in row_tag.SelectNodes("main:c", ns_manager))
                    {
                        int col = Helpers.ReferencesFormat.str2column(cell_tag.GetAttribute("r"));
                        if (col >= range_coords.ColumnFirst)
                        {
                            if(col <= range_coords.ColumnLast)
                            {
                                if (!setter.Set(row, col, cell_tag))
                                {
                                    return;
                                }
                            }
                            else
                            {
                                break; // doesn't walk through the cells that are not in the range for sure. (Optimizatsiya, blin)
                            }
                        }
                    }
                }
            }
            setter.PostProcessing();
        }

        /// <summary>
        /// Call the passed setter for every existent cell and set styles for rows and columns if necessary.
        /// Note: Doesn't stop on setter or collector signals
        /// </summary>
        /// <param name="setter">Only style setter is acceptable</param>
        public void SetStyles(StyleSetter setter)
        {
            Trace.TraceInformation("SetStyles issued for " + range_coords.ToString());
            if (range_coords.IsSetOfRows)
            {
                rows_walker.SetRows(setter);
                UpdateValues(setter);
            }
            else if (range_coords.IsSetOfColumns)
            {
                cols_walker.SetCols(setter);
                UpdateValues(setter);
            }
            else
            {
                ChangeValues(setter);
            }
        }

        private Cells.CellRange range_coords;
        private XmlNode sheetData_tag;
        private Worksheet parent_sheet;
        private XmlNamespaceManager ns_manager;
        RowsWalker rows_walker;
        ColsWalker cols_walker;
    }
}
