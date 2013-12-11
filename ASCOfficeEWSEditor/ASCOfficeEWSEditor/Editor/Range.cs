using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;
using System.Xml.Xsl;
using System.IO;
using ASCOfficeEWSEditor.XMLDataSpecific;
using ASCOfficeEWSEditor.Editor.Walkers;

namespace ASCOfficeEWSEditor.Editor
{
    /// <summary>
    /// Represents a range of cells
    /// </summary>
    public class Range
    {
        internal Range(string ref_str, Worksheet parent_sheet)
            : this(new Cells.CellRange(ref_str, parent_sheet.Workbook), parent_sheet)
        {
        }

        internal Range(Cells.CellRange coords, Worksheet parent_sheet)
        {
            this.xml_sheetData = parent_sheet.SheetData;
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;
            this.coords = coords;
            range_walker = new RangeWalker(coords, parent_sheet);
            Format = new Format.Format(range_walker, coords, parent_sheet);
        }



        /// <summary>
        ///   Returns or sets a 'System.Object' value that represents the value of the specified range.
        ///   Returns null if the value differs for different cells.
        ///   Sets the passed value to all cells of the range if the range references more than one cell.
        ///   Note: Recalculation is possible.
        /// </summary>
        public object Value
        {
            get
            {
                ValueGetter collector = new ValueGetter(parent_sheet.Workbook);
                range_walker.CollectStatistics(collector);
                return collector.Value;
            }
            set
            {
                ValueSetter setter = null;
                if (string.IsNullOrEmpty(ValueType))
                    setter = new ValueSetter(value, parent_sheet, coords.NumCells);
                else
                    setter = new ValueSetter(value, ValueType, parent_sheet, coords.NumCells, updateHeaders);
                range_walker.ChangeValues(setter);
                parent_sheet.IssueCellsUpdatedEvent();
            }
        }

        internal Calculation.Formulas.DataObjects.IDataObject InternalValue
        {
            get
            {
                ValueGetter collector = new ValueGetter(parent_sheet.Workbook);
                range_walker.CollectStatistics(collector);
                return collector.InternalValue;
            }
            set
            {
                ValueSetter setter = new ValueSetter(value, parent_sheet, coords.NumCells);
                range_walker.ChangeValues(setter);
                parent_sheet.IssueCellsUpdatedEvent();
            }
        }

        /// <summary>
        ///   Returns a 'System.Object' value that represents the displayed value of the specified range using the number format set to this value or range.
        ///   Returns null if the value differs for different cells.
        /// </summary>
        public object DispValue
        {
            get
            {
                Calculation.Formulas.DataObjects.IDataObject value = InternalValue;
                if (value is Calculation.Formulas.DataObjects.ObjectEmpty || value == null)
                {
                    return "";
                }

                int? number_format_id = Format.NumberFormatId;
                if (!number_format_id.HasValue)
                {
                    return "";
                }

                string number_format_string = parent_sheet.Workbook.StyleSheet.GetNumberFormatString(number_format_id.Value);

                return NumberFormat.NumberFormatParser.MakeDisplayValue(value, number_format_string);
            }
        }

        /// <summary>
        ///   Returns or sets a String value that represents the formula of the specified range.
        ///   Returns null if the value differs for different cells.
        ///   Sets the passed formula to all cells of the range if the range references more than one cell.
        ///   Recalculates the value after set formula.
        ///   Note: Recalculation is possible.
        /// </summary>
        public string Formula
        {
            get
            {
                FormulaGetter collector = new FormulaGetter(parent_sheet.Workbook);
                range_walker.CollectStatistics(collector);
                return collector.Value;
            }
            set
            {
                m_oLastSetFormulaError = null;
                if(value != null && value.Length != 0)
                {
                    FormulaSetter setter = new FormulaSetter(value != null ? value.Trim() : null, coords.TopLeft, parent_sheet);
                    if (false == range_walker.ChangeValues(setter))
                        m_oLastSetFormulaError = new Events.FormulaFormatError("");
                    parent_sheet.IssueCellsUpdatedEvent();
                }
                else
                {
                    Value = null;
                }
            }
        }

        /// <summary>
        /// </summary>
        public string Hyperlink
        {
            get
            {
                HyperlinkGetter collector = new HyperlinkGetter(parent_sheet);
                range_walker.CollectStatistics(collector);
                return collector.Value;
            }
            set
            {
                HyperlinkSetter setter = new HyperlinkSetter(parent_sheet, value);
                range_walker.ChangeValues(setter);
            }
        }

        /// <summary>
        /// Sets/gets the height of all the rows in the range specified, measured in points. 
        /// Note: if the obtained values of height of rows in the range differ from each other, 
        /// this property returns 'null'. If all rows have the same height, this height is returned
        /// You'll have no problems setting a value for several rows
        /// </summary>
        public double? RowHeight 
        {
            get
            {
                XmlNode manual_set_height_node = xml_sheetData.SelectSingleNode("main:row[@r = " + coords.RowFirst + "]/@ht", ns_manager);
                double standard_height = parent_sheet.StandardHeight;
                double first_row_height = null == manual_set_height_node ? standard_height : XMLDouble.ParseOrDefault(manual_set_height_node.InnerText, -1);
                if(first_row_height == -1)
                {
                    parent_sheet.Workbook.ReportError(new Events.InternalError("Wrong OOX format. row[@r = " + coords.RowFirst + "]/@ht is not double"));
                    return null;
                }

                for (int another_row = coords.RowFirst + 1; another_row <= coords.RowLast; ++another_row)
                {
                    XmlNode another_height_node = xml_sheetData.SelectSingleNode("main:row[@r = " + another_row + "]/@ht", ns_manager);
                    double another_row_height = null == another_height_node ? standard_height : XMLDouble.ParseOrDefault(another_height_node.InnerText, -1);
                    if (another_row_height == -1)
                    {
                        parent_sheet.Workbook.ReportError(new Events.InternalError("Wrong OOX format. row[@r = " + another_row + "]/@ht is not double"));
                        return null;
                    }
                    if (another_row_height != first_row_height)
                    {
                        return null;
                    }
                }
                return first_row_height;
            }
            set
            {
                RowsWalker walker = new RowsWalker(coords.RowFirst, coords.RowLast, parent_sheet);
                RowsHeightSetter setter = new RowsHeightSetter(value.Value);
                walker.SetRows(setter);
            }
        }
        public double[] GetHeights()
        {
            if (coords.RowFirst <= coords.RowLast)
            {
                int nCount = coords.RowLast - coords.RowFirst + 1;
                double[] aResult = new double[nCount];
                double dStandartHeight = parent_sheet.StandardHeight;
                for (int i = 0; i < nCount; i++)
                    aResult[i] = dStandartHeight;
                XmlNodeList oRowNodes = this.xml_sheetData.ChildNodes;
                try
                {
                    XmlNode oCurRowNode = oRowNodes[0];
                    while (null != oCurRowNode)
                    {
                        XmlElement oCurRowElem = (XmlElement)oCurRowNode;
                        int row = XMLInt.ParseOrDefault(oCurRowElem.GetAttribute("r"), 0);
                        double height = XMLDouble.ParseOrDefault(oCurRowElem.GetAttribute("ht"), -1);
                        if (coords.RowFirst <= row && row <= coords.RowLast)
                            aResult[row - 1] = height;

                        oCurRowNode = oCurRowNode.NextSibling;
                    }
                }
                catch { }
                return aResult;
            }
            return null;
        }
        public double[] GetWidths()
        {
            if (coords.ColumnFirst <= coords.ColumnLast)
            {
                int nCount = coords.ColumnLast - coords.ColumnFirst + 1;
                double[] aResult = new double[nCount];
                double dStandartWidth = parent_sheet.StandardWidth;
                for (int i = 0; i < nCount; i++)
                    aResult[i] = dStandartWidth;
                XmlNodeList manual_set_width_node = xml_sheetData.SelectNodes("../main:cols/main:col[@min <= " + coords.ColumnLast +
                    " and @max >= " + coords.ColumnFirst + "]", ns_manager);
                List<ColWidth> oCols = new List<ColWidth>();

                int nCountElements = manual_set_width_node.Count;
                for (int nIndex = 0; nIndex < nCountElements; ++nIndex)
                {
                    try
                    {
                        XmlNode oCurNode = manual_set_width_node.Item(nIndex);
                        ColWidth oCurWidth = new ColWidth();

                        oCurWidth.nMin = Convert.ToInt32(oCurNode.Attributes["min"].Value);
                        oCurWidth.nMax = Convert.ToInt32(oCurNode.Attributes["max"].Value);
                        oCurWidth.dWidth = XMLDouble.ParseOrDefault(oCurNode.Attributes["width"].Value, -1);
                        oCols.Add(oCurWidth);
                    }
                    catch { }
                }
                
                if (oCols.Count > 0)
                {
                    //сортируем по min
                    oCols.Sort(ColWidth.Compare);
                    int nCurColIndex = 0;
                    int nCurMin = oCols[nCurColIndex].nMin;
                    int nCurMax = oCols[nCurColIndex].nMax;
                    double nCurWidth = oCols[nCurColIndex].dWidth;
                    for (int i = 0; i < nCount; i++)
                    {
                        if (i + 1 > nCurMax)
                        {
                            nCurColIndex++;
                            if (nCurColIndex < oCols.Count)
                            {
                                nCurMin = oCols[nCurColIndex].nMin;
                                nCurMax = oCols[nCurColIndex].nMax;
                                nCurWidth = oCols[nCurColIndex].dWidth;
                            }
                            else
                                break;
                        }
                        if (nCurMin <= i + 1 && i + 1 <= nCurMax)
                            aResult[i] = nCurWidth;
                    }
                }
                return aResult;
            }
            return null;
        }
        /// <summary>
        /// Returns the height, in points, of the range. Read-only.
        /// </summary>
        public double Height
        {
            get
            {
                double total_height = 0;
                double standard_height = parent_sheet.StandardHeight;
                for (int row = coords.RowFirst; row <= coords.RowLast; ++row)
                {
                    XmlNode row_ht_node = xml_sheetData.SelectSingleNode("main:row[@r = " + row + "]/@ht", ns_manager);
                    double row_height = null == row_ht_node ? standard_height : XMLDouble.ParseOrDefault(row_ht_node.InnerText, -1);
                    if (row_height == -1)
                    {
                        parent_sheet.Workbook.ReportError(new Events.InternalError("Wrong OOX format. row[@r = " + row + "]/@ht is not double"));
                        row_height =  standard_height;
                    }
                    total_height += row_height;
                }
                return total_height;
            }
        }
        
        /// <summary>
        /// Sets/gets the width of all columns in the range specified. Read/write. 
        /// One unit of the value is equal to the width of '0' (zero) character in the Normal style. 
        /// Note: if the obtained values of widths of columns in the range differ from each other, 
        /// this property returns 'null'. If all columns have the same width, this width is returned
        /// You'll have no problems setting a value for several columns
        /// </summary>
        public double? ColumnWidth
        {
            get
            {
                string columnFirst_num_str = coords.ColumnFirst.ToString();
                XmlNode manual_set_width_node = xml_sheetData.SelectSingleNode("../main:cols/main:col[@min <= " + columnFirst_num_str +
                    " and @max >= " + columnFirst_num_str + "]/@width", ns_manager);
                double standard_width = parent_sheet.StandardWidth;
                double first_col_width = null == manual_set_width_node ? standard_width : XMLDouble.ParseOrDefault(manual_set_width_node.InnerText, -1);
                if (first_col_width == -1)
                {
                    parent_sheet.Workbook.ReportError(new Events.InternalError("Wrong OOX format. col/@width is not double"));
                    return null;
                }

                for (int another_col = coords.ColumnFirst; another_col <= coords.ColumnLast; ++another_col)
                {
                    string another_col_str = another_col.ToString();
                    XmlNode another_width_node = xml_sheetData.SelectSingleNode("../main:cols/main:col[@min <= " + another_col_str +
                                                " and @max >= " + another_col_str + "]/@width", ns_manager);
                    double another_column_width = null == another_width_node ? standard_width : XMLDouble.ParseOrDefault(another_width_node.InnerText, -1);
                    if (another_column_width == -1)
                    {
                        parent_sheet.Workbook.ReportError(new Events.InternalError("Wrong OOX format. col/@width is not double"));
                        return null;
                    }
                    if (another_column_width != first_col_width)
                    {
                        return null;
                    }
                }
                return first_col_width;
            }
            set
            {
                if(value.HasValue)
                {
                    ColsWalker cols_walker = new ColsWalker(coords.ColumnFirst, coords.ColumnLast, parent_sheet);
                    cols_walker.SetCols(new WidthSetter(value.Value));
                }
            }
        }

        /// <summary>
        /// Returns the width of the range. Read-only.
        /// One unit of the value is equal to the width of '0' (zero) character in the Normal style. 
        /// </summary>
        public double Width
        {
            get
            {
                double total_width = 0;
                double standard_width = parent_sheet.StandardWidth;
                for (int col = coords.ColumnFirst; col <= coords.ColumnLast; ++col)
                {
                    string col_str_num = col.ToString();
                    XmlNode col_width_node = xml_sheetData.SelectSingleNode("../main:cols/main:col[" +
                                "@min <= " + col_str_num + " and @max >= " + col_str_num + "]/@width", ns_manager);

                    double col_width = null == col_width_node ? standard_width : XMLDouble.ParseOrDefault(col_width_node.InnerText, -1);
                    if (col_width == -1)
                    {
                        parent_sheet.Workbook.ReportError(new Events.InternalError("Wrong OOX format. col/@width is not double"));
                        col_width = standard_width;
                    }
                    total_width += col_width;
                }
                return total_width;
            }
        }

        /// <summary>
        ///   Examine if the created Range object references the only single cell
        /// </summary>
        public bool IsSingleCell
        {
            get {return coords.IsSingleCell;}
        }

        /// <summary>
        ///   Returns number of cells in the range. Read-only.
        /// </summary>
        public int NumCells
        {
            get { return coords.NumCells; }
        }

        /// <summary>
        ///   Returns string representation of the Range
        /// </summary>
        /// <returns>Range reference in A1 form</returns>
        override public string ToString()
        {
            return coords.ToString();
        }

        /// <summary>
        /// Inserts a new row above the range
        /// Note: Recalculation is possible.
        /// Note: Merged cells may be changed
        /// </summary>
        public void InsertRow()
        {
            InsertRows(1);
        }

        /// <summary>
        /// Inserts a number of new rows above the range
        /// Note: Recalculation is possible.
        /// Note: Merged cells may be changed
        /// </summary>
        public void InsertRows(int num_rows)
        {
            Cells.CellRange need_to_move_range = new Cells.CellRange(coords.RowFirst, 1, Helpers.ReferencesFormat.maxRow, Helpers.ReferencesFormat.maxCol);
            Cells.Offset offset = new Cells.Offset(num_rows, 0);

            InsertRange(need_to_move_range, offset);
        }

        private void InsertRange(Cells.CellRange moved_range, Cells.Offset offset)
        {
            parent_sheet.Workbook.ValuesUpdateManager.NotifyRangesOnInsertedRowsOrColumns(moved_range, offset, parent_sheet);

            RowsWalker moved_walker = new RowsWalker(moved_range.RowFirst, moved_range.RowLast, parent_sheet);
            RowsCellMover mover = new RowsCellMover(moved_range.ColumnFirst, moved_range.ColumnLast, offset, parent_sheet);
            moved_walker.UpdateRows(mover);

            // The following notification is made outside of any setter because the notification procedure
            // could change some dependencies and lead in incorrect further work
            parent_sheet.Workbook.ValuesUpdateManager.NotifyCellOrRangeMoved(moved_range, offset, parent_sheet);

            foreach (Worksheet ws in parent_sheet.Workbook.Worksheets)
            {
                FormulaTextAndValueUpdater formula_updater = new FormulaTextAndValueUpdater(ws);
                RangeWalker low_part_walker = new RangeWalker(Helpers.ReferencesFormat.maxRange(), ws);
                low_part_walker.UpdateValues(formula_updater);
            }

            parent_sheet.MergeCells.InsertRange(moved_range, offset);

            parent_sheet.IssueCellsUpdatedEvent();
        }

        /// <summary>
        /// Deletes all rows that are pointed to by the range
        /// Note: Recalculation is possible.
        /// Note: Merged cells may be changed
        /// </summary>
        public void DeleteRows()
        {
            Cells.CellRange deleted_range = new Cells.CellRange(coords.RowFirst, 1, coords.RowLast, Helpers.ReferencesFormat.maxCol);
            Cells.CellRange moved_range = new Cells.CellRange(coords.RowLast + 1, 1, Helpers.ReferencesFormat.maxRow, Helpers.ReferencesFormat.maxCol);
            Cells.Offset offset = new Cells.Offset(-deleted_range.Height, 0);

            DeleteRange(deleted_range, moved_range, offset);
        }

        /// <summary>
        /// Inserts a new column on the left of the range
        /// Note: Merged cells may be changed
        /// </summary>
        public void InsertColumn()
        {
            InsertColumns(1);
        }

        /// <summary>
        /// Inserts a number of new columns on the left of the range
        /// Note: Merged cells may be changed
        /// </summary>
        public void InsertColumns(int num_columns)
        {
            Cells.CellRange need_to_move_range = new Cells.CellRange(1, coords.ColumnFirst, Helpers.ReferencesFormat.maxRow, Helpers.ReferencesFormat.maxCol);
            Cells.Offset offset = new Cells.Offset(0, num_columns);

            InsertRange(need_to_move_range, offset);

            #region update cols
            foreach (XmlNode col_item in xml_sheetData.SelectNodes("../main:cols/main:col[@max >= " + coords.ColumnFirst + "]", ns_manager))
            {
                XmlElement col_tag = (XmlElement)col_item;
                int min = XMLInt.ParseOrDefault(col_tag.GetAttribute("min"), 1);
                if(min >= coords.ColumnFirst)
                {
                    col_tag.SetAttribute("min", (min + num_columns).ToString());
                }
                col_tag.SetAttribute("max", (XMLInt.ParseOrDefault(col_tag.GetAttribute("max"), Helpers.ReferencesFormat.maxCol) + num_columns).ToString());
            }
            #endregion
}

        /// <summary>
        /// Deletes all columns that are pointed to by the range
        /// Note: Merged cells may be changed
        /// </summary>
        public void DeleteColumns()
        {
            Cells.CellRange deleted_range = new Cells.CellRange(1, coords.ColumnFirst, Helpers.ReferencesFormat.maxRow, coords.ColumnLast);
            Cells.CellRange moved_range = new Cells.CellRange(1, coords.ColumnLast + 1, Helpers.ReferencesFormat.maxRow, Helpers.ReferencesFormat.maxCol);
            Cells.Offset offset = new Cells.Offset(0, -deleted_range.Width);

            DeleteRange(deleted_range, moved_range, offset);

            #region update cols
            int num_cols_to_delete = coords.Width;
            XmlNode cols_tag = xml_sheetData.SelectSingleNode("../main:cols", ns_manager);
            foreach (XmlNode col_item in xml_sheetData.SelectNodes("../main:cols/main:col[@max >= " + coords.ColumnFirst + "]", ns_manager))
            {
                XmlElement col_tag = (XmlElement)col_item;
                int min = XMLInt.ParseOrDefaultAndReport(col_tag.GetAttribute("min"), 1, parent_sheet.Workbook, "col/@min");
                int max = XMLInt.ParseOrDefault(col_tag.GetAttribute("max"), Helpers.ReferencesFormat.maxCol);

                if (min > coords.ColumnLast) // the whole range on the right of the deleted region
                {
                    min -= num_cols_to_delete;
                    max -= num_cols_to_delete;
                }
                else if (max <= coords.ColumnLast) // max is inside of the deleted region
                {
                    if(min < coords.ColumnFirst) // cut off the right part of the range
                    {
                        max = coords.ColumnFirst - 1;
                    }
                    else // the whole range is inside of the deleted region
                    {
                        cols_tag.RemoveChild(col_item); // Delete nah
                    }
                }
                else // max is on the right of the deleted region
                {
                    if (min >= coords.ColumnFirst) // cut off the left part of the range
                    {
                        min = coords.ColumnFirst;
                    }
                    max -= num_cols_to_delete;
                }
                col_tag.SetAttribute("min", min.ToString());
                col_tag.SetAttribute("max", max.ToString());
            }
            #endregion
        }

        private void DeleteRange(Cells.CellRange deleted_range, Cells.CellRange moved_range, Cells.Offset offset)
        {
            RowsWalker deleted_walker = new RowsWalker(deleted_range.RowFirst, deleted_range.RowLast, parent_sheet);
            RowsCellDeleter deleter = new RowsCellDeleter(deleted_range.ColumnFirst, deleted_range.ColumnLast, parent_sheet);
            deleted_walker.UpdateRows(deleter);

            parent_sheet.Workbook.ValuesUpdateManager.NotifyDeleted(deleted_range, parent_sheet);

            RowsWalker moved_walker = new RowsWalker(moved_range.RowFirst, moved_range.RowLast, parent_sheet);
            RowsCellMover mover = new RowsCellMover(moved_range.ColumnFirst, moved_range.ColumnLast, offset, parent_sheet);
            moved_walker.UpdateRows(mover);

            // The following notification is made outside of any setter because the notification procedure
            // could change some dependencies and lead in incorrect further work
            parent_sheet.Workbook.ValuesUpdateManager.NotifyCellOrRangeMoved(moved_range, offset, parent_sheet);

            foreach (Worksheet ws in parent_sheet.Workbook.Worksheets)
            {
                FormulaTextAndValueUpdater formula_updater = new FormulaTextAndValueUpdater(ws);
                RangeWalker low_part_walker = new RangeWalker(Helpers.ReferencesFormat.maxRange(), ws);
                low_part_walker.UpdateValues(formula_updater);
            }

            parent_sheet.MergeCells.RemoveRange(deleted_range, moved_range, offset);

            parent_sheet.IssueCellsUpdatedEvent();
        }

        /// <summary>
        /// Obtains a list of all existent cells references in the range.
        /// </summary>
        /// <returns>List of string references</returns>
        public List<string> ExistentCells()
        {
            ExistentCellsCollector collector = new ExistentCellsCollector(parent_sheet);
            range_walker.CollectStatistics(collector);
            return collector.GetFoundReferences();
        }
        public List<string> ExistentCells(out int nMaxRow, out int nMaxCol)
        {
            //ExistentCellsCollector collector = new ExistentCellsCollector(parent_sheet);
            //range_walker.CollectStatistics(collector);
            //int nRMin;
            //int nCMin;
            //collector.GetFoundBounds(out nRMin, out nMaxRow, out nCMin, out nMaxCol);
            ////collector.
            //return collector.GetFoundReferences();

            nMaxRow = 0;
            nMaxCol = 0;
            XmlNodeList oRowNodes = this.xml_sheetData.ChildNodes;
            try
            {
                XmlNode oCurRowNode = oRowNodes[0];
                while (null != oCurRowNode)
                {
                    XmlElement oCurRowElem = (XmlElement)oCurRowNode;
                    int row = XMLInt.ParseOrDefault(oCurRowElem.GetAttribute("r"), 0);
                    if (row > nMaxRow)
                        nMaxRow = row;
                    string sSpans = oCurRowElem.GetAttribute("spans");
                    if (string.Empty != sSpans)
                    {
                        int nIndex = sSpans.IndexOf(':');
                        if (-1 != nIndex)
                        {
                            int nLastCol = XMLInt.ParseOrDefault(sSpans.Substring(nIndex + 1), 0);
                            if (nLastCol > nMaxCol)
                                nMaxCol = nLastCol;
                        }
                    }
                    else
                    {
                        XmlNodeList oColNodes = oCurRowNode.ChildNodes;
                        try
                        {
                            XmlNode oCurColNode = oColNodes[0];
                            while (null != oCurColNode)
                            {
                                string sCellId = ((XmlElement)oCurColNode).GetAttribute("r");
                                Cells.Cell oCurCell = new ASCOfficeEWSEditor.Editor.Cells.Cell(sCellId);
                                if (oCurCell.Column > nMaxCol)
                                    nMaxCol = oCurCell.Column;

                                oCurColNode = oCurColNode.NextSibling;
                            }
                        }
                        catch { }
                    }

                    oCurRowNode = oCurRowNode.NextSibling;
                }
            }
            catch { }
            //Cells.Cell oLastCell = new ASCOfficeEWSEditor.Editor.Cells.Cell(nMaxRow, nMaxCol);
            //List<string> aResult = new List<string>();
            //aResult.Add("A1:" + oLastCell.ToString());
            //return aResult;
            return new List<string>();
        }

        /// <summary>
        /// Obtains a list of all non-empty cells references in the range
        /// </summary>
        /// <returns>List of string references</returns>
        public List<string> NonEmptyCells()
        {
            NonEmptyCellsCollector collector = new NonEmptyCellsCollector(parent_sheet);
            range_walker.CollectStatistics(collector);
            return collector.GetFoundReferences();
        }

        /// <summary>
        /// Returns Format object that is responsible for getting and setting of all format and style properties of the cells.
        /// Read-only.
        /// </summary>
        public Format.Format Format { get; private set; }

        /// <summary>
        /// Returns the merged range reference that the Cell is included to (Must be called for single-cell ranges only. Otherwise an empty string is returned).
        /// If no merged range contains the cell, the same cell reference is returned.
        /// </summary>
        public string MergedArea 
        {
            get
            {
                if(coords.IsSingleCell)
                {
                    Cells.CellRange area = parent_sheet.MergeCells.MergedArea(coords.TopLeft);
                    return area.ToString();
                }
                else
                {
                    return "";
                }
            }
        }

        /// <summary>
        ///   Returns Last Formula Error
        /// </summary>
        public Events.ErrorDescription LastFormulaError
        {
            get { return m_oLastSetFormulaError; }
        }

        /// <summary>
        /// Determines whether one of cells of the range is included in a merged range
        /// </summary>
        /// <returns></returns>
        public MergeInfo HasMergedCells()
        {
            return parent_sheet.MergeCells.HasMergedCells(coords);
        }

        /// <summary>
        /// Unmerge all ranges that intersect the specified one
        /// </summary>
        public void UnMerge()
        {
            parent_sheet.MergeCells.UnMerge(coords);
        }

        /// <summary>
        /// Merge the range.
        /// Note: Recalculation is possible.
        /// </summary>
        public void Merge()
        {
            Cells.CellRange result_range = parent_sheet.MergeCells.Merge(coords);
            Cells.Cell top_left = result_range.TopLeft;

            RangeWalker range_walker_result = new RangeWalker(result_range, parent_sheet);
            CellWalker cell_walker_top_left = new CellWalker(top_left, parent_sheet);

            // Determine top left cell value and formula
            FirstNonEmptyFormulaValueCollector formula_value_collector = new FirstNonEmptyFormulaValueCollector(parent_sheet.Workbook);
            range_walker_result.CollectStatistics(formula_value_collector);
            
            // Set removed cells to null (except the top left cell)
            ExceptCellDeleter setter = new ExceptCellDeleter(top_left.Row, top_left.Column, parent_sheet);
            range_walker_result.ChangeValues(setter);

            // Move the first non-empty value to the top left cell
            if(formula_value_collector.Formula != null && formula_value_collector.Formula != "")
            {
                FormulaSetter setter_one_cell = new FormulaSetter(formula_value_collector.Formula, top_left /* Excel do the same */, parent_sheet);
                cell_walker_top_left.ChangeValues(setter_one_cell);
            }
            else if (formula_value_collector.Value != null && !(formula_value_collector.Value is Calculation.Formulas.DataObjects.ObjectEmpty))
            {
                ValueSetter setter_one_cell = new ValueSetter(formula_value_collector.Value, parent_sheet, 1);
                cell_walker_top_left.ChangeValues(setter_one_cell);
            }

            parent_sheet.IssueCellsUpdatedEvent();
        }

        /// <summary>
        /// Sort cells of the range
        /// Note: Recalculation is possible.
        /// </summary>
        /// <param name="orientation">Sort orientation</param>
        /// <param name="key">Key range. Specifies the column (for row orientation) or the row (column orientation) that the sort is performed by</param>
        /// <param name="order">Sort order</param>
        public void Sort(Sorting.SortOrientation orientation, Range key, Sorting.SortOrder order)
        {

            ExistentCellsCollector collector = new ExistentCellsCollector(parent_sheet);
            range_walker.CollectStatistics(collector);
            Cells.CellRange min_coords = collector.GetRange();
            Cells.CellRange sort_coords = new Cells.CellRange(coords.RowFirst, coords.ColumnFirst, min_coords.RowLast, min_coords.ColumnLast);
            RangeWalker sort_range_walker = new RangeWalker(sort_coords, parent_sheet);

            
            Cells.Cell key_cell = key.coords.TopLeft;
            Cells.CellRange key_range = orientation == Sorting.SortOrientation.by_rows ? new Cells.CellRange(sort_coords.RowFirst, key_cell.Column, sort_coords.RowLast, key_cell.Column) :
                                                                                    new Cells.CellRange(key_cell.Row, sort_coords.ColumnFirst, key_cell.Row, sort_coords.ColumnLast);
            #region Sort key values
            RangeWalker key_range_walker = new RangeWalker(key_range, parent_sheet);
            SortValuesCollector key_values = new SortValuesCollector(parent_sheet.Workbook, orientation);
            key_range_walker.CollectStatisticsIncludingEmpty(key_values);
            if(order == Sorting.SortOrder.ascending)
            {
                key_values.Values.Sort();
            }
            else
            {
                key_values.Values.Sort(new Sorting.DescendingComparer());
            }
            #endregion
            #region Collect references to existent nodes
            CellsNodesCollector nodes_collection = new CellsNodesCollector();
            sort_range_walker.CollectStatisticsIncludingEmpty(nodes_collection);
            #endregion

            // Replace all cells
            CellsSubstitutionSetter substitutor = new CellsSubstitutionSetter(nodes_collection.FoundReferences, key_values.Values,
                                                                                orientation, sort_coords.TopLeft, parent_sheet);

            // ToDo (переделать, работает достаточно долго)
            Dictionary<int, Format.FormatSimple> arrRowsDefaultStyle = null;
            List<Format.CCollFormat> arrCoolsDefaultStyle = null;
            List<CellSimple> oCellsOld = GetCellSimple(null, out arrRowsDefaultStyle, out arrCoolsDefaultStyle);
            sort_range_walker.ChangeValues(substitutor);
            for (int nIndex = 0; nIndex < oCellsOld.Count; ++nIndex)
            {
                CellSimple oCell = oCellsOld[nIndex];
                Range oNeighborCel = parent_sheet.Range(oCell.ID);
                oNeighborCel.Format.Borders.Bottom.Style = oCell.Format.Brds.Bottom.Style;
                oNeighborCel.Format.Borders.Left.Style = oCell.Format.Brds.Left.Style;
                oNeighborCel.Format.Borders.Right.Style = oCell.Format.Brds.Right.Style;
                oNeighborCel.Format.Borders.Top.Style = oCell.Format.Brds.Top.Style;
            }

            #region Move Hyperlinks
            {
                List<Hyperlinks.Hyperlink> hyps = new List<Hyperlinks.Hyperlink>();
                foreach (Cells.Cell cell in sort_coords)
                {
                    Hyperlinks.Hyperlink hyp = parent_sheet.Hyperlinks[cell.ToString()];
                    if (hyp != null)
                    {
                        hyps.Add(hyp);
                        parent_sheet.Hyperlinks.RemoveHyperlink(cell);
                    }
                }
                Cells.Cell top_left = sort_coords.TopLeft;
                foreach (Hyperlinks.Hyperlink hyp in hyps)
                {
                    Cells.Cell other_cell = hyp.CellAnchor;
                    Cells.Cell cell = orientation == Sorting.SortOrientation.by_rows ? new Cells.Cell(key_values.Values.FindIndex(delegate(Sorting.ValueRowColumn vrc) { return vrc.row_column == other_cell.Row; }) + top_left.Row, other_cell.Column) :
                                                                                     new Cells.Cell(other_cell.Row, key_values.Values.FindIndex(delegate(Sorting.ValueRowColumn vrc) { return vrc.row_column == other_cell.Column; }) + top_left.Column);
                    // Move hyperlink if it exists in former cell
                    parent_sheet.Hyperlinks[cell.ToString()] = hyp;
                }
            }
            #endregion

            parent_sheet.IssueCellsUpdatedEvent();
        }

        /// <summary>
        /// Copies the content, formulas and styles of all of the cells in the range to the specified destination range.
        /// Note: Recalculation is possible.
        /// Note: Styles are not copied when copy is made between different workbooks.
        /// </summary>
        /// <param name="destination_range">Destination range</param>
        public void CopyTo(Range destination_range)
        {
            if(coords.TopLeft == destination_range.coords.TopLeft && parent_sheet == destination_range.parent_sheet)
            {
                return;
            }
            // Collect references to source nodes
            CellsNodesCollector nodes_collection = new CellsNodesCollector();
            range_walker.CollectStatisticsIncludingEmpty(nodes_collection);

            Cells.Offset offset = destination_range.coords.TopLeft - coords.TopLeft;
            Cells.CellRange dest_coords = coords + offset;
            RangeWalker destination_walker = new RangeWalker(dest_coords, destination_range.parent_sheet);

            // Replace all cells
            CellsCopySetter copier = new CellsCopySetter(nodes_collection.FoundReferences, parent_sheet, 
                                                            destination_range.parent_sheet, offset);
            destination_walker.ChangeValues(copier);

            destination_range.parent_sheet.IssueCellsUpdatedEvent();
        }

        #region XML references
            private XmlNode xml_sheetData;
        #endregion
        #region Tree references
            private Worksheet parent_sheet;
        #endregion
        private Events.FormulaFormatError m_oLastSetFormulaError = null;
        private XmlNamespaceManager ns_manager;
        private Cells.CellRange coords;
        private RangeWalker range_walker;
        private struct ColWidth
        {
            public int nMin;
            public int nMax;
            public double dWidth;
            public static int Compare(ColWidth x, ColWidth y)
            {
                return x.nMin - y.nMin;
            }
        };
        public List<CellSimple> GetCellSimple(StyleCollectorCache oCache, out Dictionary<int, Format.FormatSimple> arrRowsDefaultStyle, out List<Format.CCollFormat> arrCollsDefaultStyle)
        {
            CellSimpleGetter collector = new CellSimpleGetter(parent_sheet.Workbook, oCache);
            range_walker.CollectStatistics(collector);

            arrRowsDefaultStyle = collector.m_arrRowsDefaultStyle;
            arrCollsDefaultStyle = collector.m_arrCollsDefaultStyle;
            return collector.Value;
        }

        public string ValueType { set; get; }
        public bool updateHeaders { set; get; }
    }

    public enum MergeInfo
    {
        none = 0,
        merge = 1,
        firstmerge = 2
    }

    public class CellSimple
    {
        internal CellSimple()
        {
        }
        public string ID;
        public string DispValue;
        public string InternalValue;
        public string InternalDate;
        public string Formula;
        public string Hyperlink;
        public string ValueType;
        public Format.FormatSimple Format;
        public MergeInfo HasMergedCells;
    };

}
