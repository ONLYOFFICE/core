using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;
using System.IO;
using ASCOfficeEWSEditor.Editor.Charts;
using ASCOfficeEWSEditor.Editor.Images;
using FileConverterUtils2;
using ASCOfficeFile;
using ASCImageFile3;
using ASCMediaCore3;
using System.Net;
using System.Text.RegularExpressions;
using ASCOfficeEWSEditor.Editor.Cells;
using ASCOfficeEWSEditor.Editor.Walkers;
using ASCImageStudio3;

namespace ASCOfficeEWSEditor.Editor
{
    /// <summary>
    /// Represents a worksheet.
    /// </summary>
    public class Worksheet : Sheet
    {
        /// <summary>
        /// Create a new worksheet
        /// </summary>
        internal static Worksheet CreateNew(string name, Sheet before, XmlNode xml_workbook_sheets, Sheets container, Workbook workbook)
        {
            OOX.WorksheetFile worksheet_file = new OOX.WorksheetFile(workbook.ContentTypes, workbook.Relationships, workbook);
            worksheet_file.GetFileTagOrCreate();
            return new Worksheet(name, worksheet_file.rId, before, xml_workbook_sheets, container, workbook, worksheet_file);
        }

        /// <summary>
        /// Create a new worksheet
        /// </summary>
        private Worksheet(string name, string rId, Sheet before, XmlNode xml_workbook_sheets, Sheets container, Workbook workbook, OOX.WorksheetFile worksheet_file)
            : base(name, rId, before, xml_workbook_sheets, container, workbook)
        {
            this.worksheet_file = worksheet_file;
            xml_worksheet = worksheet_file.GetWorksheetTagOrCreate();
            CommonInitialization();
        }

        /// <summary>
        /// Create from existent worksheet
        /// </summary>
        internal Worksheet(string sheet_path, XmlNode xml_workbook_sheet, Sheets container, Workbook workbook)
            : base(container, workbook, xml_workbook_sheet)
        {
            worksheet_file = new OOX.WorksheetFile(sheet_path, workbook.ContentTypes, workbook.Relationships, workbook);
            xml_worksheet = worksheet_file.GetRootTag();
            CommonInitialization();
            //BuildFormulasDependancies();

            FindTables();
            FindImages(workbook);
            FindCharts();
        }

        private void CommonInitialization()
        {
            Relationships = new OOX.Relationships(xml_worksheet.ParentNode);
            SubjectsStorage = new Calculation.SubjectsStorage(this);
            Formulas = new Dictionary<Cells.Cell, Calculation.Formulas.CellParsedFormula>();
            xml_sheetData = xml_worksheet.SelectSingleNode("main:sheetData", ns_manager);
            if (null == xml_sheetData)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. Required 'sheetData' tag is absent."));
            }
            xml_sheetView = xml_worksheet.SelectSingleNode("main:sheetViews", ns_manager);
            if (null == xml_sheetView || 0 == xml_sheetView.ChildNodes.Count)
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. Required 'sheetView' tag is absent."));
            else
                xml_sheetView = xml_sheetView.SelectSingleNode("main:sheetView", ns_manager);
            if (null == xml_sheetView)
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. Required 'sheetView' tag is absent."));

            MergeCells = new MergeCells(this, xml_worksheet);
            Subject = new Calculation.WorksheetSubject(this);
            Hyperlinks = new Hyperlinks.Hyperlinks(this); // Must follow Relationships initialization

            Tables = new List<Table>();
            SimpleImages = new List<Image>();
            SimpleCharts = new List<Chart>();

            AutoFilter = new AutoFilter(this, xml_worksheet, SheetData);
        }

        internal void BuildFormulasDependancies()
        {
            Walkers.RangeWalker range_walker = new Walkers.RangeWalker(Helpers.ReferencesFormat.maxRange(), this);

            Walkers.FormulasParser collector = new Walkers.FormulasParser(this);
            range_walker.CollectStatistics(collector);
        }


        /// <summary>
        /// Has sheet data. Read-only.
        /// </summary>
        public override bool HasSheetData { get { return true; } }


        /// <summary>
        /// References the specified range of cells
        /// </summary>
        /// <param name="range_ref">Name of the range in A1 form</param>
        /// <returns></returns>
        public virtual Range Range(string range_ref)
        {
            return new Range(range_ref, this);
        }

        /// <summary>
        /// References the specified single cell
        /// </summary>
        /// <param name="row">Row of the cell (1-based)</param>
        /// <param name="col">Column of the cell (1-based)</param>
        /// <returns></returns>
        public virtual Range Cell(int row, int col)
        {
            return new Range(new Cells.CellRange(row, col, row, col), this);
        }

        internal virtual Range Cell(Cells.Cell cell)
        {
            return new Range(new Cells.CellRange(cell), this);
        }

        /// <summary>
        /// Sets/Gets the standard (default) height of all the rows in the worksheet, in points. 
        /// Note: You should keep in mind that when you set this value this will only affect 
        /// the rows which height was not set manually
        /// </summary>
        public double StandardHeight
        {
            get
            {
                XmlNode height_node = xml_worksheet.SelectSingleNode("main:sheetFormatPr/@defaultRowHeight", ns_manager);
                if (null == height_node)
                {
                    return 15; // TODO: find a way how to calculate this value depending on font type
                }
                double standard_height = XMLDouble.ParseOrDefault(height_node.InnerText, -1);
                if (standard_height == -1 || double.IsNaN(standard_height))
                {
                    Workbook.ReportError(new Events.InternalError("Wrong OOX format. sheetFormatPr/@defaultRowHeight is not double or NaN"));
                    return 15; // TODO: find a way how to calculate this value depending on font type
                }
                return standard_height;
            }
            set
            {
                XmlElement sheetFormatPr_node = (XmlElement)xml_worksheet.SelectSingleNode("main:sheetFormatPr", ns_manager);
                if (null == sheetFormatPr_node)
                {
                    sheetFormatPr_node = xml_worksheet.OwnerDocument.CreateElement("sheetFormatPr", ns_manager.LookupNamespace("main"));
                    xml_worksheet.InsertAfter(sheetFormatPr_node, null);
                }
                sheetFormatPr_node.SetAttribute("defaultRowHeight", XMLDouble.ToString(value));
            }
        }

        /// <summary>
        /// Sets/Gets the standard (default) width of all the columns in the worksheet. 
        /// One unit of the value is equal to the width of '0' (zero) character in the Normal style. 
        /// Note: You should keep in mind that when you set this value this will only affect 
        /// the columns which width was not set manually
        /// </summary>
        public double StandardWidth
        {
            get
            {
                XmlNode width_node = xml_worksheet.SelectSingleNode("main:sheetFormatPr/@defaultColWidth", ns_manager);
                if (null == width_node)
                {
                    return 8.43; // TODO: find a way how to calculate this value depending on font type
                }
                double standard_width = XMLDouble.ParseOrDefault(width_node.InnerText, -1);
                if (standard_width == -1 || double.IsNaN(standard_width))
                {
                    Workbook.ReportError(new Events.InternalError("Wrong OOX format. sheetFormatPr/@defaultColWidth is not double or NaN"));
                    return 8.43; // TODO: find a way how to calculate this value depending on font type
                }
                return standard_width;
            }
            set
            {
                XmlElement sheetFormatPr_node = (XmlElement)xml_worksheet.SelectSingleNode("main:sheetFormatPr", ns_manager);
                if (null == sheetFormatPr_node)
                {
                    sheetFormatPr_node = xml_worksheet.OwnerDocument.CreateElement("sheetFormatPr", ns_manager.LookupNamespace("main"));
                    XMLSectionsController.AppendXmlSection(sheetFormatPr_node);
                }
                sheetFormatPr_node.SetAttribute("defaultColWidth", XMLDouble.ToString(value));
            }
        }

        /// <summary>
        /// Permanently removes the worksheet from the workbook
        /// </summary>
        /// <returns>Whether the operation successful. Return false if the removed sheet is the only sheet in the workbook</returns>
        public override bool Remove()
        {
            if (RemoveSheet())
            {
                worksheet_file.Remove();

                foreach (Calculation.Formulas.CellParsedFormula cell_parsed_formula in Formulas.Values)
                {
                    cell_parsed_formula.UnregisterDependencies();
                }

                foreach (Worksheet ws in Workbook.Worksheets)
                {
                    foreach (Calculation.CellItem cell_item in ws.SubjectsStorage.CellSubjects.Values)
                    {
                        if (cell_item.HasFormula() && cell_item.UpdateDelayed)
                        {
                            cell_item.RecalcFormulaAndUpdateCell();
                            Workbook.ValuesUpdateManager.NotifyUpdatedCell(cell_item.Cell, ws);
                            cell_item.UpdateDelayed = false;
                        }
                    }
                }
                return true;
            }
            return false;
        }

        /// <summary>
        /// Sets the sheet active.
        /// </summary>
        public override void Activate()
        {
            ((XmlElement)this.xml_sheetView).SetAttribute("tabSelected", "1");
            base.Activate();
        }

        /// <summary>
        /// Remove sheet active
        /// </summary>
        public override void RemoveActive()
        {
            ((XmlElement)this.xml_sheetView).RemoveAttribute("tabSelected");
            base.Activate();
        }

        /// <summary>
        /// Gets a Range object that references all the cells on the worksheet 
        /// </summary>
        public virtual Range Cells { get { return new Range(Helpers.ReferencesFormat.maxRange(), this); } }

        /// <summary>
        /// Returns references to the merged ranges on the sheet
        /// </summary>
        /// <returns>List of range references</returns>
        public List<string> GetMergedAreas()
        {
            return MergeCells.GetMergedAreas();
        }

        /// <summary>
        /// Returns references to the merged ranges on the sheet
        /// </summary>
        /// <returns>List of range references</returns>
        public List<Cells.CellRange> GetMergedAreas2()
        {
            return MergeCells.GetMergedAreas2();
        }

        /// <summary>
        /// This just sets the recalculated cell value without any attendant actions
        /// </summary>
        /// <param name="cell"></param>
        /// <param name="value"></param>
        internal void CellSetValue(Cells.Cell cell, Calculation.Formulas.DataObjects.IDataObject value)
        {
            Walkers.CellWalker cell_walker = new Walkers.CellWalker(cell, this);
            Walkers.ValueSimpleSetter setter = new Walkers.ValueSimpleSetter(value, this, 1);
            cell_walker.ChangeValues(setter);

        }

        /// <summary>
        /// This just gets the recalculated cell value without any attendant actions
        /// </summary>
        /// <param name="cell"></param>
        internal Calculation.Formulas.DataObjects.IDataObject CellGetValue(Cells.Cell cell)
        {
            XmlNode value_tag = xml_sheetData.SelectSingleNode("main:row[@r = " + cell.Row + "]/main:c[@r = '" + cell.ToString() + "']/main:v", ns_manager);
            if (value_tag == null)
            {
                return new Calculation.Formulas.DataObjects.ObjectEmpty();
            }
            string value_type = ((XmlElement)value_tag.ParentNode).GetAttribute("t");
            return Walkers.ValueGetter.RecognizeValue(value_tag.InnerText, value_type, Workbook);
        }

        /// <summary>
        /// This just sets the recalculated formula value without any attendant actions
        /// </summary>
        /// <param name="cell"></param>
        /// <param name="formula"></param>
        internal void FormulaSimpleSetValue(Cells.Cell cell, string formula)
        {
            Walkers.CellWalker cell_walker = new Walkers.CellWalker(cell, this);
            Walkers.FormulaSimpleSetter setter = new Walkers.FormulaSimpleSetter(formula, this);
            cell_walker.ChangeValues(setter);

        }

        public string AddNewTable(string strFilterRange)
        {
            OOX.TableFile oTableFile = new ASCOfficeEWSEditor.Editor.OOX.TableFile(Workbook.ContentTypes, Relationships, Workbook);

            Cells.CellRange oFilterRange = new ASCOfficeEWSEditor.Editor.Cells.CellRange(strFilterRange, Workbook);
            Table oNewTable = Table.CreateNew(this, oTableFile, xml_worksheet, oFilterRange);

            Tables.Add(oNewTable);
            return oNewTable.rId;
        }

        public void RemoveTable(string table_rId)
        {
            for (int nIndex = 0; nIndex < Tables.Count; ++nIndex)
            {
                if (Tables[nIndex].rId == table_rId)
                {
                    Tables[nIndex].RemoveFromXml(Tables.Count);
                    Tables.RemoveAt(nIndex);
                    break;
                }
            }
        }

        public Dictionary<int, bool> ChangeAutoFilterTable(string table_rId, AutoFilterElementSendData oChangeObject)
        {
            for (int nIndex = 0; nIndex < Tables.Count; ++nIndex)
            {
                if (Tables[nIndex].rId == table_rId)
                {
                    return Tables[nIndex].ChangeAutoFilter(oChangeObject);
                }
            }

            return null;
        }

        public bool isCellHeader(int row, int col, out int nTableIndex)
        {
            for (nTableIndex = 0; nTableIndex < Tables.Count; ++nTableIndex)
            {
                if (Tables[nTableIndex].isCellHeader(row, col))
                    return true;
            }

            nTableIndex = -1;
            return false;
        }

        public void changeCellHeader(int nTableIndex, int col)
        {
            if (0 > nTableIndex || nTableIndex >= Tables.Count)
                return;

            Tables[nTableIndex].changeCellHeader(col);
        }

        /// <summary>
        /// An event that can be used to be notified whenever the cells containing formulas are recalculated.
        /// </summary>
        public event EventHandler CellsUpdatedEvent;

        /// <summary>
        /// Obtain the set of updated cells
        /// </summary>
        /// <returns>Set of updated cells</returns>
        public Events.CellsUpdatedEventArg GetUpdatedCells()
        {
            Events.CellsUpdatedEventArg cell_refs = new Events.CellsUpdatedEventArg();
            foreach (KeyValuePair<Cells.Cell, Calculation.CellItem> element in SubjectsStorage.CellSubjects)
            {
                if (element.Value.CellOrFormulaUpdated)
                {
                    cell_refs.AddCellRef(element.Key.ToString());
                    element.Value.CellOrFormulaUpdated = false;
                }
            }

            return cell_refs;
        }

        public bool CopyData(Worksheet worksheet)
        {
            if (null == worksheet.xml_sheetData)
                return false;
            //TODO подумать как бы копировать xml целиком, при этом убирая или наоборот дублирую rid в некоторых нодах(например pageSetup).
            //Хорошо бы еще посмотреть есть ли rid в тех нодах, что копируются сейчас.
            XmlNode oNodeSheetData = worksheet.xml_sheetData.Clone();
            xml_worksheet.ReplaceChild(oNodeSheetData, xml_sheetData);
            xml_sheetData = oNodeSheetData;

            XmlNode oCols = worksheet.xml_worksheet.SelectSingleNode("main:cols", worksheet.ns_manager);
            if (null != oCols)
                xml_worksheet.InsertBefore(oCols.Clone(), xml_sheetData);

            XmlNode oPageMargin = xml_worksheet.SelectSingleNode("main:pageMargins", ns_manager);
            if (null != oPageMargin)
            {
                if (null != worksheet.MergeCells.MergeCellsXml)
                {
                    XmlNode oNodeMergeCells = worksheet.MergeCells.MergeCellsXml.Clone();
                    xml_worksheet.InsertBefore(oNodeMergeCells, oPageMargin);
                    MergeCells.MergeCellsXml = oNodeMergeCells;
                }
                if (null != worksheet.Hyperlinks.HyperlinksXml)
                {
                    XmlNode oNodeHyperlinks = worksheet.Hyperlinks.HyperlinksXml.Clone();
                    xml_worksheet.InsertBefore(oNodeHyperlinks, oPageMargin);
                    Hyperlinks.HyperlinksXml = oNodeHyperlinks;
                }
            }
            else
            {
                if (null != worksheet.MergeCells.MergeCellsXml)
                {
                    XmlNode oNodeMergeCells = worksheet.MergeCells.MergeCellsXml.Clone();
                    xml_worksheet.AppendChild(oNodeMergeCells);
                    MergeCells.MergeCellsXml = oNodeMergeCells;
                }
                if (null != worksheet.Hyperlinks.HyperlinksXml)
                {
                    XmlNode oNodeHyperlinks = worksheet.Hyperlinks.HyperlinksXml.Clone();
                    xml_worksheet.AppendChild(oNodeHyperlinks);
                    Hyperlinks.HyperlinksXml = oNodeHyperlinks;
                }
            }
            foreach (Table oTable in worksheet.Tables)
            {
                if (ASCOfficeEWSEditor.Editor.Cells.CellRange.Empty != oTable.TableAutoFilter.m_oRefRange)
                {
                    OOX.TableFile oTableFile = new ASCOfficeEWSEditor.Editor.OOX.TableFile(Workbook.ContentTypes, Relationships, Workbook);
                    Table oNewTable = new Table(this, oTableFile, xml_worksheet, oTable);
                    Tables.Add(oNewTable);
                }
            }
            return true;
        }

        internal void IssueCellsUpdatedEvent()
        {
            if (CellsUpdatedEvent != null)
            {
                Events.CellsUpdatedEventArg cell_refs = GetUpdatedCells();

                if (cell_refs.IsSomethingChanged)
                {
                    CellsUpdatedEvent(this, cell_refs);
                }
            }
        }

        internal Cells.CellRange GetNonEmptyCellsRange()
        {
            Walkers.RangeWalker range_walker = new Walkers.RangeWalker(Helpers.ReferencesFormat.maxRange(), this);
            Walkers.NonEmptyCellsCollector collector = new Walkers.NonEmptyCellsCollector(this);
            range_walker.CollectStatistics(collector);
            return collector.GetRange();
        }

        /// <summary>
        /// The difference from GetNonEmptyCellsRange is that GetNonEmptyCells0Range returns range started in A1 and ended in max used cell
        /// </summary>
        /// <returns></returns>
        internal Cells.CellRange GetNonEmptyCells0Range()
        {
            Walkers.RangeWalker range_walker = new Walkers.RangeWalker(Helpers.ReferencesFormat.maxRange(), this);
            Walkers.NonEmptyCellsCollector collector = new Walkers.NonEmptyCellsCollector(this);
            range_walker.CollectStatistics(collector);
            return collector.Get0Range();
        }

        private void FindCharts()
        {
            XmlElement drawing_node = (XmlElement)xml_worksheet.SelectSingleNode("main:drawing", ns_manager);
            if (drawing_node == null)
            {
                // There is no drawing in the worksheet
                return;
            }
            string drawing_rId = drawing_node.GetAttribute("id", ns_manager.LookupNamespace("mainr"));
            string drawing_path = Relationships.GetTarget(drawing_rId);

            XmlNode drawing_file_xml = Helpers.OOXNavigate.getReferenceByPath(drawing_path, xml_worksheet.ParentNode.ParentNode);
            if (drawing_file_xml == null)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. File '" + drawing_path + "' not found."));
            }

            XmlNodeList anchors = drawing_file_xml.SelectNodes("xdr:wsDr/xdr:twoCellAnchor", ns_manager);
            foreach (XmlNode anchor_xml in anchors)
            {
                XmlElement anchor_chart_xml = (XmlElement)anchor_xml.SelectSingleNode("xdr:graphicFrame/a:graphic/a:graphicData/c:chart", ns_manager);
                if (anchor_chart_xml == null)   // image                   
                    continue;

                string chart_rId = anchor_chart_xml.GetAttribute("id", ns_manager.LookupNamespace("mainr"));

                OOX.Relationships drawing_relationships = new OOX.Relationships(drawing_file_xml);

                string chart_path = drawing_relationships.GetTarget(chart_rId);

                if (string.IsNullOrEmpty(chart_path))   // случай когда ошибочно построили drawing, но файла нет
                    continue;

                XmlNode chart_file_xml = Helpers.OOXNavigate.getReferenceByPath(chart_path, drawing_file_xml.ParentNode);
                if (chart_file_xml == null)
                {
                    //drawing_relationships.RemoveTarget(chart_rId);
                    continue;
                    throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. File '" + chart_path + "' not found."));
                }
                SimpleCharts.Add(new Chart(chart_file_xml, anchor_xml, this));
            }
        }

        private void FindTables()
        {
            XmlElement table_node = (XmlElement)xml_worksheet.SelectSingleNode("main:tableParts", ns_manager);
            if (table_node == null)
            {
                // There is no tables in the worksheet
                return;
            }
            foreach (XmlElement oXmlNode in table_node.ChildNodes)
            {
                string table_rId = oXmlNode.GetAttribute("id", ns_manager.LookupNamespace("mainr"));
                string table_path = Relationships.GetTarget(table_rId);

                OOX.TableFile oTableFile = new OOX.TableFile(table_path, Workbook.ContentTypes, Relationships, Workbook);

                Tables.Add(new Table(this, oTableFile, xml_worksheet, SheetData, table_rId));
            }
        }

        private void FindImages(Workbook workbook)
        {
            if ((workbook.Editor == null) || string.IsNullOrEmpty(workbook.Editor.site_dir) || !Directory.Exists(workbook.Editor.site_dir))
                return;

            string mediaFolder = null;
            SimpleImages = new List<Image>();
            XmlElement drawing_node = (XmlElement)xml_worksheet.SelectSingleNode("main:drawing", ns_manager);
            if (drawing_node == null)
                return;
            else
            {
                mediaFolder = workbook.Editor.site_dir + "\\process\\media\\";
                if (!Directory.Exists(mediaFolder))
                    Directory.CreateDirectory(mediaFolder);
            }

            string drawing_rId = drawing_node.GetAttribute("id", ns_manager.LookupNamespace("mainr"));
            string drawing_path = Relationships.GetTarget(drawing_rId);

            XmlNode drawing_file_xml = Helpers.OOXNavigate.getReferenceByPath(drawing_path, xml_worksheet.ParentNode.ParentNode);
            if (drawing_file_xml == null)
                return;

            // find anchors
            XmlNodeList anchors = drawing_file_xml.SelectNodes("xdr:wsDr/xdr:twoCellAnchor", ns_manager);
            foreach (XmlNode anchor_xml in anchors)
            {
                XmlElement imageId = (XmlElement)anchor_xml.SelectSingleNode("xdr:pic/xdr:blipFill/a:blip", ns_manager);
                if (imageId == null)    // chart
                    continue;

                Image img = new Image(anchor_xml, ns_manager);

                // find image path
                OOX.Relationships drawing_relationships = new OOX.Relationships(drawing_file_xml);
                string image_doc_path = drawing_relationships.GetTarget(img.pic_rid);

                if (string.IsNullOrEmpty(image_doc_path))       // случай когда ошибочно построили drawing, но файла нет
                    continue;

                img.image_doc_path = image_doc_path;

                // extract images
                XmlNode image_file_xml = Helpers.OOXNavigate.getReferenceByPath(image_doc_path, drawing_file_xml.ParentNode);
                if (image_file_xml == null)
                    continue;

                string imageFileName = workbook.Name + "_" + Path.GetFileName(image_doc_path);
                int binId = int.Parse(image_file_xml.Attributes["bin_data_id"].Value);

                string imageUrl = mediaFolder + imageFileName;
                imageUrl = imageUrl.ToLower();
                string imageExt = Path.GetExtension(imageUrl);

                FileStream stream = new FileStream(imageUrl, FileMode.Create);
                stream.Write(workbook.BinaryDataStorage[binId], 0, workbook.BinaryDataStorage[binId].Length);
                stream.Close();

                switch (imageExt)
                {
                    case ".tiff":
                    case ".emf":
                    case ".wmf":
                        {
                            string toFile = imageUrl.Replace(imageExt, ".jpg");

                            ASCOfficeFile.CAVSOfficeFileConverterClass converter = new ASCOfficeFile.CAVSOfficeFileConverterClass();
                            ASCImageStudio3.ImageTransformsClass imageConverter = new ASCImageStudio3.ImageTransformsClass();

                            string strLoadXml = "<transforms>" +
                                                    "<ImageFile-LoadImage sourcepath=\"" + imageUrl + "\"/>" +
                                                    "<ImageFile-SaveAsJpeg destinationpath=\"" + toFile + "\" format=\"888\"/>" +
                                                "</transforms>";
                            bool bResult = imageConverter.SetXml(strLoadXml);
                            bResult = imageConverter.Transform();
                            int reff = System.Runtime.InteropServices.Marshal.ReleaseComObject(imageConverter);

                            if (bResult)
                            {
                                File.Delete(imageUrl);
                                imageUrl = toFile;
                            }
                        }
                        break;

                    default:
                        break;
                }

                int start = imageUrl.IndexOf(@"process\");
                img.image_url = imageUrl.Remove(0, start);
                img.image_url = img.image_url.Replace("\\", "/");

                SimpleImages.Add(img);
            }
        }

        bool ConvertToPng()
        {
            bool bResult = false;
            return bResult;
        }

        void ToLog(Workbook workbook, string what, bool newFile)
        {
            if (!string.IsNullOrEmpty(what))
            {
                FileStream log = new FileStream(@"d:\Work\AVSDocuments\ASCOfficeWeb\wwwroot\process\log.txt", newFile ? FileMode.Create : FileMode.Open);
                log.Seek(log.Length, SeekOrigin.Begin);
                System.Text.UTF8Encoding encoding = new System.Text.UTF8Encoding();
                string str = what + "\n";
                log.Write(encoding.GetBytes(str), 0, str.Length);
                log.Close();
            }
        }

        private string ConvertInterval(string interval)
        {
            string retVal = "";

            string listName, cells;
            string[] spl = interval.Split(new char[] { '!' });

            if (spl.Length > 0)
            {
                listName = spl[0];
                cells = spl[1];

                spl = cells.Split(new char[] { ':' });

                if (spl.Length > 0)
                {
                    retVal = listName + "!";
                    for (int i = 0; i < spl.Length; i++)
                    {
                        string cellNum = Regex.Replace(spl[i], @"\D", "", RegexOptions.Compiled);
                        string cellWord = spl[i].Replace(cellNum, "");

                        retVal += "$" + cellWord + "$" + cellNum + ((i < spl.Length - 1) ? ":" : "");
                    }
                }
            }

            return retVal;
        }

        public void UpdateChartPos(Chart chart, DrawingTransFromJson jsonChart)
        {
            chart.TwoCellAnchor.from_col = jsonChart.from_col;
            chart.TwoCellAnchor.from_colOff = jsonChart.from_colOff;
            chart.TwoCellAnchor.from_row = jsonChart.from_row;
            chart.TwoCellAnchor.from_rowOff = jsonChart.from_rowOff;

            chart.TwoCellAnchor.to_col = jsonChart.to_col;
            chart.TwoCellAnchor.to_colOff = jsonChart.to_colOff;
            chart.TwoCellAnchor.to_row = jsonChart.to_row;
            chart.TwoCellAnchor.to_rowOff = jsonChart.to_rowOff;

            chart.TwoCellAnchor.chart_width = jsonChart.object_width;
            chart.TwoCellAnchor.chart_height = jsonChart.object_height;
            chart.TwoCellAnchor.chart_xOff = jsonChart.object_xOff;
            chart.TwoCellAnchor.chart_yOff = jsonChart.object_yOff;
        }

        public bool UpdateChartData(DrawingTransFromJson jsonChart, Workbook workbook)
        {
            for (int i = 0; i < SimpleCharts.Count; i++)
            {
                if (SimpleCharts[i].TwoCellAnchor.frame_id == jsonChart.object_id)
                {
                    switch (jsonChart.chart.chartType)
                    {
                        case "col":
                        case "bar":
                            SimpleCharts[i].chart_file_xml.InnerXml = Resource1.empty_barChart;
                            break;
                        case "lineChart":
                            SimpleCharts[i].chart_file_xml.InnerXml = Resource1.empty_lineChart;
                            break;
                        case "scatterChart":
                            SimpleCharts[i].chart_file_xml.InnerXml = Resource1.empty_scatterChart;
                            break;
                        case "bubbleChart":
                            SimpleCharts[i].chart_file_xml.InnerXml = Resource1.empty_bubbleChart;
                            break;
                        case "areaChart":
                            SimpleCharts[i].chart_file_xml.InnerXml = Resource1.empty_areaChart;
                            break;
                        case "radarChart":
                            SimpleCharts[i].chart_file_xml.InnerXml = Resource1.empty_radarChart;
                            break;
                        case "doughnutChart":
                            SimpleCharts[i].chart_file_xml.InnerXml = Resource1.empty_doughnutChart;
                            break;
                        case "pieChart":
                            SimpleCharts[i].chart_file_xml.InnerXml = Resource1.empty_pieChart;
                            break;
                        case "stockChart":
                            SimpleCharts[i].chart_file_xml.InnerXml = Resource1.empty_stockChart;
                            break;
                    }

                    SimpleCharts.Clear();
                    FindCharts();

                    for (int j = 0; j < SimpleCharts.Count; j++)
                    {
                        if (SimpleCharts[j].TwoCellAnchor.frame_id == jsonChart.object_id)
                        {
                            InitChartFromJson(SimpleCharts[j], jsonChart, workbook);
                            break;
                        }
                    }
                    break;
                }
            }
            return true;
        }

        public void UpdateChartValues(List<string> cells, Workbook wb)
        {
            for (int i = 0; i < SimpleCharts.Count; i++)
            {
                for (int j = 0; j < SimpleCharts[i].PlotArea.basicChart.seriesList.Count; j++)
                {
                    SimpleCharts[i].PlotArea.basicChart.seriesList[j].ChangeValues(cells, wb);
                }
            }
        }

        public void UpdateChartAutoFilterValues(Dictionary<int, bool> autoFilterRows, Workbook workbook)
        {
            int maxRow, maxCol;
            workbook.ActiveWorksheet.Cells.ExistentCells(out maxRow, out maxCol);
            List<string> deleteList = new List<string>();
            foreach (KeyValuePair<int, bool> pair in autoFilterRows)
            {
                CellRange cellRange = new CellRange(pair.Key, 1, pair.Key, maxCol);
                Range range = new Range(cellRange, workbook.ActiveWorksheet);
                List<string> changeList = range.NonEmptyCells();
                if (pair.Value)
                {
                    for (int i = 0; i < changeList.Count; i++)
                    {
                        deleteList.Add(changeList[i]);
                    }
                }
            }

            // проверяем в каждой диаграмме все серии
            for (int i = 0; i < SimpleCharts.Count; i++)
            {
                for (int j = 0; j < SimpleCharts[i].PlotArea.basicChart.seriesList.Count; j++)
                {
                    SimpleCharts[i].PlotArea.basicChart.seriesList[j].ClearAutoFilterValues(deleteList, workbook);
                }
            }
        }

        private void InitChartFromJson(Chart chart, DrawingTransFromJson jsonChart, Workbook workbook)
        {
            UpdateChartPos(chart, jsonChart);

            chart.PlotArea.basicChart.chartType = jsonChart.chart.chartType;
            chart.PlotArea.basicChart.seriesType = jsonChart.chart.seriesType;

            chart.PlotArea.xShow = jsonChart.chart.xAxis.show;
            chart.PlotArea.xTitle = jsonChart.chart.xAxis.title;
            chart.PlotArea.xGrid = jsonChart.chart.xAxis.grid;

            chart.PlotArea.yShow = jsonChart.chart.yAxis.show;
            chart.PlotArea.yTitle = jsonChart.chart.yAxis.title;
            chart.PlotArea.yGrid = jsonChart.chart.yAxis.grid;

            chart.title = jsonChart.chart.title;
            if (!jsonChart.chart.legend.show)
                chart.DeleteLegend();
            else
            {
                chart.Legend.overlay = jsonChart.chart.legend.overlay;
                chart.Legend.position = jsonChart.chart.legend.legendPos;
            }

            // устанавливаем кол-во серий - 1
            chart.PlotArea.basicChart.AddSeries(jsonChart.chart.seriesList.Count - 1);
            for (int i = 0; i < jsonChart.chart.seriesList.Count; i++)
            {
                chart.PlotArea.basicChart.seriesList[i].InsertValues(jsonChart.chart.seriesList[i].valuesList);
                Cells.CellRange3D location = new Cells.CellRange3D(new Cells.CellRange(jsonChart.chart.seriesList[i].referredrange, workbook), workbook.ActiveWorksheet);
                string formatedInterval = ConvertInterval(location.ToString());
                chart.PlotArea.basicChart.seriesList[i].interval = formatedInterval;
                chart.PlotArea.basicChart.seriesList[i].color = jsonChart.chart.seriesList[i].color;
                chart.PlotArea.basicChart.seriesList[i].borderColor = jsonChart.chart.seriesList[i].borderColor;
                chart.PlotArea.basicChart.seriesList[i].name = jsonChart.chart.seriesList[i].name;
                chart.PlotArea.basicChart.seriesList[i].showVal = jsonChart.chart.seriesList[i].showVal;
            }

            chart.TwoCellAnchor.chart_guid = jsonChart.object_guid;
        }

        // считает все диаграммы по всем листам
        private int GetWorkbookLastRid(Workbook workbook)
        {
            int lastRid = 0;
            for (int i = 0; i < workbook.Worksheets.Count; i++)
            {               
                XmlNode drawing = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/drawing" + (i + 1).ToString() + ".xml", xml_worksheet.OwnerDocument.DocumentElement);
                if (drawing == null)
                    continue;

                OOX.Relationships rels = new OOX.Relationships(drawing);
                int currRid = rels.GetLastRid();
                if (currRid > 0)
                    lastRid += currRid;
            }
            return lastRid;
        }

        public bool AddChart(Workbook workbook, DrawingTransFromJson newObject)
        {
            // вставка            
            XmlElement drawing_node = (XmlElement)xml_worksheet.SelectSingleNode("main:drawing", ns_manager);
            if (drawing_node == null)
            {
                // создаём новый drawing в листе
                XmlElement drawing_tag = xml_worksheet.OwnerDocument.CreateElement("drawing", ns_manager.LookupNamespace("main"));

                // поиск последнего индекса автофильтров
                OOX.Relationships sheet_rel = new OOX.Relationships(Helpers.OOXNavigate.getReferenceByPath("xl/worksheets/sheet" + (workbook.ActiveSheet.Index + 1).ToString() + ".xml",
                                                                    xml_worksheet.OwnerDocument.DocumentElement));

                drawing_tag.SetAttribute("id", ns_manager.LookupNamespace("mainr"), "rId" + (sheet_rel.GetLastRid() + 1).ToString());
                
                
                XmlNode pageMargins = xml_worksheet.SelectSingleNode("main:pageMargins", ns_manager);
                if (pageMargins != null)
                    xml_worksheet.InsertAfter((XmlNode)drawing_tag, pageMargins);
                else
                {
                    XmlNode pageSetup = xml_worksheet.SelectSingleNode("main:pageSetup", ns_manager);
                    if (pageSetup == null)
                        return false;
                    xml_worksheet.InsertAfter((XmlNode)drawing_tag, pageSetup);
                }

                // папки                
                XmlNode chart_dir_xml = Helpers.OOXNavigate.AddDirectory("xl/charts", xml_worksheet.OwnerDocument.DocumentElement, false);
                XmlNode draw_dir_xml = Helpers.OOXNavigate.AddDirectory("xl/drawings", xml_worksheet.OwnerDocument.DocumentElement, false);
                XmlNode draw_rels_dir_xml = Helpers.OOXNavigate.AddDirectory("xl/drawings/_rels", xml_worksheet.OwnerDocument.DocumentElement, false);
                XmlNode sheet_rels_dir_xml = Helpers.OOXNavigate.AddDirectory("xl/worksheets/_rels", xml_worksheet.OwnerDocument.DocumentElement, false);

                // rels для worksheets                
                string sheet_rels_file_name = "sheet" + (workbook.ActiveSheet.Index + 1).ToString() + ".xml.rels";
                workbook.ContentTypes.RegisterFile("/xl/worksheets/_rels/" + sheet_rels_file_name, ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.rels);

                string drawingName = "drawing" + (workbook.ActiveSheet.Index + 1).ToString() + ".xml";
                XmlNode draw_file_xml = Helpers.OOXNavigate.AddFile("xl/drawings/" + drawingName, xml_worksheet.OwnerDocument.DocumentElement, false);

                // register                
                workbook.ContentTypes.RegisterFile(draw_file_xml, ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.drawing);

                // targets
                Relationships = new OOX.Relationships(xml_worksheet.ParentNode);                
                string chartName = "chart" + (GetWorkbookLastRid(workbook) + 1).ToString() + ".xml";
                
                draw_file_xml.InnerXml = Resource1.empty_chartDrawing;
                OOX.Relationships drawing_rel = new OOX.Relationships(draw_file_xml);
                drawing_rel.AddTarget("../charts/" + chartName, ASCOfficeEWSEditor.Editor.OOX.Relationships.FileType.chart, false);

                sheet_rel.AddTarget("../drawings/" + drawingName, ASCOfficeEWSEditor.Editor.OOX.Relationships.FileType.drawing, false);
                workbook.ContentTypes.RegisterFile("/xl/drawings/_rels/" + drawingName + ".rels", ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.rels);

                // заполняем объект
                XmlNode drawing_chart_file_xml = Helpers.OOXNavigate.getReferenceByPath("../charts/" + chartName, xml_worksheet.ParentNode.ParentNode);

                XmlNode chart_file_xml = Helpers.OOXNavigate.AddFile("xl/charts/" + chartName, xml_worksheet.OwnerDocument.DocumentElement, false);
                workbook.ContentTypes.RegisterFile(chart_file_xml, ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.chartsheet);

                switch (newObject.chart.chartType)
                {
                    case "col":
                    case "bar":
                        chart_file_xml.InnerXml = Resource1.empty_barChart;
                        break;
                    case "lineChart":
                        chart_file_xml.InnerXml = Resource1.empty_lineChart;
                        break;
                    case "scatterChart":
                        chart_file_xml.InnerXml = Resource1.empty_scatterChart;
                        break;
                    case "bubbleChart":
                        chart_file_xml.InnerXml = Resource1.empty_bubbleChart;
                        break;
                    case "areaChart":
                        chart_file_xml.InnerXml = Resource1.empty_areaChart;
                        break;
                    case "radarChart":
                        chart_file_xml.InnerXml = Resource1.empty_radarChart;
                        break;
                    case "doughnutChart":
                        chart_file_xml.InnerXml = Resource1.empty_doughnutChart;
                        break;
                    case "pieChart":
                        chart_file_xml.InnerXml = Resource1.empty_pieChart;
                        break;
                    case "stockChart":
                        chart_file_xml.InnerXml = Resource1.empty_stockChart;
                        break;
                }

                Relationships = new OOX.Relationships(xml_worksheet.ParentNode);

                // получаем объект и заполняем поля
                FindCharts();
                if (SimpleCharts.Count > 0)
                    InitChartFromJson(SimpleCharts[0], newObject, workbook);
            }
            else // есть drawing
            {
                string drawing_rId = drawing_node.GetAttribute("id", ns_manager.LookupNamespace("mainr"));
                Relationships = new OOX.Relationships(xml_worksheet.ParentNode);
                string drawing_path = Relationships.GetTarget(drawing_rId);

                string[] spl = drawing_path.Split(new char[] { '/' });
                XmlNode drawing_file_xml = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/" + spl[spl.Length - 1], xml_worksheet.OwnerDocument.DocumentElement);

                if (drawing_file_xml == null)
                    return false;

                XmlNodeList anchors = drawing_file_xml.SelectNodes("xdr:wsDr/xdr:twoCellAnchor", ns_manager);

                // делаем копию и задаём новые параметры
                if (anchors.Count > 0)
                {
                    OOX.Relationships drawing_relationships = new OOX.Relationships(drawing_file_xml);
                    //string chartName = "chart" + (drawing_relationships.GetLastRid() + 1).ToString() + ".xml";
                    string chartName = "chart" + (GetWorkbookLastRid(workbook) + 1).ToString() + ".xml";

                    XmlNode chart_file_xml = Helpers.OOXNavigate.AddFile("xl/charts/" + chartName, xml_worksheet.OwnerDocument.DocumentElement, false);
                    drawing_relationships.AddTarget("../charts/" + chartName, ASCOfficeEWSEditor.Editor.OOX.Relationships.FileType.chart, false);
                    workbook.ContentTypes.RegisterFile(chart_file_xml, ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.chartsheet);

                    // пишем в drawing
                    XmlElement resourceDrawing = xml_worksheet.OwnerDocument.CreateElement("tmp");
                    resourceDrawing.InnerXml = Resource1.empty_chartDrawing;
                    XmlNode template = resourceDrawing.SelectSingleNode("xdr:wsDr/xdr:twoCellAnchor", ns_manager);

                    XmlNode newAnchor = template.Clone();

                    // меняем индексы
                    XmlNode frameNode = (XmlElement)newAnchor.SelectSingleNode("xdr:graphicFrame", ns_manager);
                    XmlElement id = (XmlElement)frameNode.SelectSingleNode("xdr:nvGraphicFramePr/xdr:cNvPr", ns_manager);
                    if (id != null)
                        id.SetAttribute("id", (drawing_relationships.GetLastRid() - 1).ToString());

                    XmlElement rid = (XmlElement)frameNode.SelectSingleNode("a:graphic/a:graphicData/c:chart", ns_manager);
                    if (rid != null)
                        rid.SetAttribute("r:id", "rId" + drawing_relationships.GetLastRid().ToString());

                    XmlNode docRoot = drawing_file_xml.SelectSingleNode("xdr:wsDr", ns_manager);
                    docRoot.AppendChild(newAnchor);

                    // пишем в chart
                    switch (newObject.chart.chartType)
                    {
                        case "col":
                        case "bar":
                            chart_file_xml.InnerXml = Resource1.empty_barChart;
                            break;
                        case "lineChart":
                            chart_file_xml.InnerXml = Resource1.empty_lineChart;
                            break;
                        case "scatterChart":
                            chart_file_xml.InnerXml = Resource1.empty_scatterChart;
                            break;
                        case "bubbleChart":
                            chart_file_xml.InnerXml = Resource1.empty_bubbleChart;
                            break;
                        case "areaChart":
                            chart_file_xml.InnerXml = Resource1.empty_areaChart;
                            break;
                        case "radarChart":
                            chart_file_xml.InnerXml = Resource1.empty_radarChart;
                            break;
                        case "doughnutChart":
                            chart_file_xml.InnerXml = Resource1.empty_doughnutChart;
                            break;
                        case "pieChart":
                            chart_file_xml.InnerXml = Resource1.empty_pieChart;
                            break;
                        case "stockChart":
                            chart_file_xml.InnerXml = Resource1.empty_stockChart;
                            break;
                    }

                    Relationships = new OOX.Relationships(xml_worksheet.ParentNode);

                    // получаем объект и заполняем поля
                    SimpleCharts.Clear();
                    FindCharts();
                    if (SimpleCharts.Count > 0)
                        InitChartFromJson(SimpleCharts[SimpleCharts.Count - 1], newObject, workbook);
                }
            }
            return true;
        }

        public bool DeleteChart(Workbook workbook, DrawingTransFromJson newObject)
        {
            // поиск всех зависимостей
            XmlElement drawing_node = (XmlElement)xml_worksheet.SelectSingleNode("main:drawing", ns_manager);
            if (drawing_node == null)
                return false;

            string drawing_rId = drawing_node.GetAttribute("id", ns_manager.LookupNamespace("mainr"));
            Relationships = new OOX.Relationships(xml_worksheet.ParentNode);
            string drawing_path = Relationships.GetTarget(drawing_rId);

            string[] spl = drawing_path.Split(new char[] { '/' });
            XmlNode drawing_file_xml = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/" + spl[spl.Length - 1], xml_worksheet.OwnerDocument.DocumentElement);

            if (drawing_file_xml == null)
                return false;

            for (int i = 0; i < workbook.ActiveWorksheet.SimpleCharts.Count; i++)
            {
                if (workbook.ActiveWorksheet.SimpleCharts[i].TwoCellAnchor.frame_id == newObject.object_id)
                {
                    workbook.ActiveWorksheet.SimpleCharts.RemoveAt(i);
                    break;
                }
            }

            XmlNodeList anchors = drawing_file_xml.SelectNodes("xdr:wsDr/xdr:twoCellAnchor", ns_manager);
            foreach (XmlNode anchor_xml in anchors)
            {
                XmlElement anchor_chart_xml = (XmlElement)anchor_xml.SelectSingleNode("xdr:graphicFrame/a:graphic/a:graphicData/c:chart", ns_manager);
                if (anchor_chart_xml == null)   // image                   
                    continue;

                string chart_rId = anchor_chart_xml.GetAttribute("id", ns_manager.LookupNamespace("mainr"));

                OOX.Relationships drawing_relationships = new OOX.Relationships(drawing_file_xml);

                string chart_path = drawing_relationships.GetTarget(chart_rId);
                XmlNode chart_file_xml = Helpers.OOXNavigate.getReferenceByPath(chart_path, drawing_file_xml.ParentNode);

                Chart chart = new Chart(chart_file_xml, anchor_xml, this);

                if (chart.TwoCellAnchor.frame_id == newObject.object_id)
                {
                    anchor_xml.ParentNode.RemoveChild(anchor_xml);

                    // find image path
                    drawing_relationships = new OOX.Relationships(drawing_file_xml);
                    string chart_file_path = drawing_relationships.GetTarget(chart.TwoCellAnchor.chart_rid);
                    XmlNode image_file_xml = Helpers.OOXNavigate.getReferenceByPath(chart_file_path, drawing_file_xml.ParentNode);

                    workbook.ContentTypes.DeRegisterFile(Helpers.OOXNavigate.getReferenceByPath("xl/charts/" + Path.GetFileName(chart_file_path), xml_worksheet.OwnerDocument.DocumentElement));
                    drawing_relationships.RemoveTarget(chart.TwoCellAnchor.chart_rid);

                    XmlNode ret_xml = Helpers.OOXNavigate.DeleteBinFile("xl/charts/" + Path.GetFileName(chart_file_path), xml_worksheet.OwnerDocument.DocumentElement);

                    // удаляем drawing в листе
                    if (drawing_relationships.GetLastRid() == 0)
                    {
                        string drawingName = "drawing" + (workbook.ActiveSheet.Index + 1).ToString() + ".xml";
                        string sheetName = "sheet" + (workbook.ActiveSheet.Index + 1).ToString() + ".xml";
                        XmlNode removeElement = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/" + drawingName, xml_worksheet.OwnerDocument.DocumentElement);
                        workbook.ContentTypes.DeRegisterFile(removeElement);

                        removeElement = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/_rels/" + drawingName + ".rels", xml_worksheet.OwnerDocument.DocumentElement);
                        workbook.ContentTypes.DeRegisterFile(removeElement);

                        removeElement = Helpers.OOXNavigate.getReferenceByPath("xl/worksheets/_rels/" + sheetName + ".rels", xml_worksheet.OwnerDocument.DocumentElement);
                        workbook.ContentTypes.DeRegisterFile(removeElement);
                        //removeElement.ParentNode.RemoveChild(removeElement);

                        xml_worksheet.RemoveChild(drawing_node);

                        XmlNode drawing_xml = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/" + drawingName, xml_worksheet.OwnerDocument.DocumentElement);
                        drawing_xml.ParentNode.RemoveChild(drawing_xml);

                        XmlNode drawing_rels_xml = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/_rels/" + drawingName + ".rels", xml_worksheet.OwnerDocument.DocumentElement);
                        drawing_rels_xml.ParentNode.RemoveChild(drawing_rels_xml);
                    }
                    break;
                }
            }

            return true;
        }

        public bool AddImage(Workbook workbook, DrawingTransFromJson newObject, string siteDir)
        {
            string mediaFolder = siteDir + "\\process\\media\\";
            if (!Directory.Exists(mediaFolder))
                Directory.CreateDirectory(mediaFolder);

            string fileName = mediaFolder + workbook.Name + "_" + newObject.object_guid;

            // файл уже загружен
            if (newObject.image_url.IndexOf("process/", 0) == 0)
            {
                string downloadFilePath = siteDir + "//" + newObject.image_url.Replace("/", "\\");
                File.Copy(downloadFilePath, fileName, true);
                //File.Delete(downloadFilePath);
            }
            else // url картинки
            {
                // base64
                if (newObject.image_url.IndexOf("data:image/", 0) == 0)
                {
                    string base64Image = newObject.image_url.Remove(0, newObject.image_url.IndexOf(",") + 1);
                    byte[] base64Buff = Convert.FromBase64String(base64Image);
                    FileStream base64Stream = new FileStream(fileName, FileMode.Create);
                    base64Stream.Write(base64Buff, 0, base64Buff.Length);
                    base64Stream.Close();
                }
                else
                {
                    try
                    {
                        WebClient webClient = new WebClient();
                        webClient.DownloadFile(newObject.image_url, fileName);
                    }
                    catch (Exception ex)
                    {
                        throw new Events.ReportableException(new Events.InternalError("Error downloading file."));
                    }
                }
            }

            // init registration
            ASCOfficeFile.CAVSOfficeFormatChecker imageChecker = new CAVSOfficeFormatChecker();
            ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType contentType;
            int type = imageChecker.GetFileFormat(fileName);
            int reff = System.Runtime.InteropServices.Marshal.ReleaseComObject(imageChecker);
            string ext = "";

            switch (type)
            {
                case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_JPG:
                    contentType = ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.jpeg;
                    ext = ".jpeg";
                    break;
                case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_PNG:
                    contentType = ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.png;
                    ext = ".png";
                    break;
                case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_GIF:
                    contentType = ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.gif;
                    ext = ".gif";
                    break;
                
                // необходима конвертация
                case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_BMP:
                case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_TIFF:
                case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_EMF:
                case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_WMF:
                case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_TGA:
                    {
                        try
                        {
                            contentType = ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.jpeg;
                            ext = ".jpeg";
                            string toFile = fileName + "_jpeg";

                            ASCOfficeFile.CAVSOfficeFileConverterClass converter = new ASCOfficeFile.CAVSOfficeFileConverterClass();
                            ASCImageStudio3.ImageTransformsClass imageConverter = new ASCImageStudio3.ImageTransformsClass();

                            string strLoadXml = "<transforms>" +
                                                    "<ImageFile-LoadImage sourcepath=\"" + fileName + "\"/>" +
                                                    "<ImageFile-SaveAsJpeg destinationpath=\"" + toFile + "\" format=\"888\"/>" +
                                                "</transforms>";
                            bool bResult = imageConverter.SetXml(strLoadXml);
                            bResult = imageConverter.Transform();
                            reff = System.Runtime.InteropServices.Marshal.ReleaseComObject(imageConverter);

                            if (bResult)
                            {
                                File.Delete(fileName);
                                fileName = toFile;
                            }
                        }
                        catch (Exception ex)
                        {
                            throw new Events.ReportableException(new Events.InternalError("Image converter error."));
                        }
                    }
                    break;
                                    
                default:
                    throw new Events.ReportableException(new Events.InternalError("Unsupported file."));
            }

            try
            {
                ASCOfficeFile.CAVSOfficeFileConverterClass converter = new ASCOfficeFile.CAVSOfficeFileConverterClass();
                ASCImageFile3.ImageFile3Class imageFileObj = new ASCImageFile3.ImageFile3Class();

                Object itf = new Object();
                bool bRes = imageFileObj.LoadImage2(fileName, out itf);

                if (itf != null)
                {
                    ASCMediaCore3.IAVSUncompressedVideoFrame metrics = (ASCMediaCore3.IAVSUncompressedVideoFrame)itf;
                    newObject.object_width = metrics.Width;
                    newObject.object_height = metrics.Height;

                    if (itf != null)
                        reff = System.Runtime.InteropServices.Marshal.ReleaseComObject(itf);
                }
                reff = System.Runtime.InteropServices.Marshal.ReleaseComObject(imageFileObj);
            }
            catch (Exception ex)
            {
            }

            string binName = "image" + (workbook.BinaryDataStorage.Count + 1).ToString() + ext;
            File.Copy(fileName, mediaFolder + workbook.Name + "_" + binName, true);
            File.Delete(fileName);
            fileName = mediaFolder + workbook.Name + "_" + binName;

            int start = fileName.IndexOf(@"process\");
            newObject.image_url = fileName.Remove(0, start);
            newObject.image_url = newObject.image_url.Replace("\\", "/");

            // вставка            
            XmlElement drawing_node = (XmlElement)xml_worksheet.SelectSingleNode("main:drawing", ns_manager);
            if (drawing_node == null)
            {
                // создаём новый drawing в листе
                XmlElement drawing_tag = xml_worksheet.OwnerDocument.CreateElement("drawing", ns_manager.LookupNamespace("main"));
                drawing_tag.SetAttribute("id", ns_manager.LookupNamespace("mainr"), "rId1");
                xml_worksheet.AppendChild((XmlNode)drawing_tag);

                // папки
                XmlNode media_dir_xml = Helpers.OOXNavigate.AddDirectory("xl/media", xml_worksheet.OwnerDocument.DocumentElement, false);
                XmlNode draw_dir_xml = Helpers.OOXNavigate.AddDirectory("xl/drawings", xml_worksheet.OwnerDocument.DocumentElement, false);
                XmlNode draw_rels_dir_xml = Helpers.OOXNavigate.AddDirectory("xl/drawings/_rels", xml_worksheet.OwnerDocument.DocumentElement, false);
                XmlNode sheet_rels_dir_xml = Helpers.OOXNavigate.AddDirectory("xl/worksheets/_rels", xml_worksheet.OwnerDocument.DocumentElement, false);

                // rels для worksheets                
                string sheet_rels_file_name = "sheet" + (workbook.ActiveSheet.Index + 1).ToString() + ".xml.rels";
                workbook.ContentTypes.RegisterFile("/xl/worksheets/_rels/" + sheet_rels_file_name, ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.rels);

                string drawingName = "drawing" + (workbook.ActiveSheet.Index + 1).ToString() + ".xml";
                XmlNode draw_file_xml = Helpers.OOXNavigate.AddFile("xl/drawings/" + drawingName, xml_worksheet.OwnerDocument.DocumentElement, false);

                // register                
                workbook.ContentTypes.RegisterFile(draw_file_xml, ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.drawing);

                // targets
                draw_file_xml.InnerXml = Resource1.empty_imageDrawing;
                OOX.Relationships drawing_rel = new OOX.Relationships(draw_file_xml);
                drawing_rel.AddTarget("../media/" + binName, ASCOfficeEWSEditor.Editor.OOX.Relationships.FileType.image, false);

                OOX.Relationships sheet_rel = new OOX.Relationships(Helpers.OOXNavigate.getReferenceByPath("xl/worksheets/sheet" + (workbook.ActiveSheet.Index + 1).ToString() + ".xml",
                                                                    xml_worksheet.OwnerDocument.DocumentElement));
                sheet_rel.AddTarget("../drawings/" + drawingName, ASCOfficeEWSEditor.Editor.OOX.Relationships.FileType.drawing, false);
                workbook.ContentTypes.RegisterFile("/xl/drawings/_rels/" + drawingName + ".rels", ASCOfficeEWSEditor.Editor.OOX.ContentTypes.FileType.rels);

                // заполняем объект
                FileStream stream = new FileStream(fileName, FileMode.Open);
                byte[] buffer = new byte[stream.Length];
                stream.Read(buffer, 0, (int)(stream.Length));
                stream.Close();

                workbook.BinaryDataStorage.Add(buffer);

                XmlNode drawing_bin_file_xml = Helpers.OOXNavigate.getReferenceByPath("../media/" + binName, xml_worksheet.ParentNode.ParentNode);

                XmlNode image_file_xml = Helpers.OOXNavigate.AddBinFile("xl/media/" + binName, xml_worksheet.OwnerDocument.DocumentElement, false, workbook.BinaryDataStorage.Count - 1);
                workbook.ContentTypes.RegisterFile(image_file_xml, contentType);

                XmlNode anchor = draw_file_xml.SelectSingleNode("xdr:wsDr/xdr:twoCellAnchor", ns_manager);
                Image img = new Image(anchor, ns_manager);

                img.image_url = newObject.image_url;
                img.image_guid = newObject.object_guid;

                // from
                img.from_col = newObject.from_col;
                img.from_colOff = newObject.from_colOff;
                img.from_row = newObject.from_row;
                img.from_rowOff = newObject.from_rowOff;

                // to
                img.to_col = newObject.to_col;
                img.to_colOff = newObject.to_colOff;
                img.to_row = newObject.to_row;
                img.to_rowOff = newObject.to_rowOff;

                img.image_width = newObject.object_width;
                img.image_height = newObject.object_height;

                img.image_xOff = newObject.object_xOff;
                img.image_yOff = newObject.object_yOff;

                // id                
                img.pic_id = workbook.BinaryDataStorage.Count - 1;

                // name
                img.pic_name = "Image " + workbook.BinaryDataStorage.Count.ToString();

                // rId                
                img.pic_rid = "rId1";

                SimpleImages.Add(img);
            }
            else // есть drawing
            {
                string drawing_rId = drawing_node.GetAttribute("id", ns_manager.LookupNamespace("mainr"));
                Relationships = new OOX.Relationships(xml_worksheet.ParentNode);
                string drawing_path = Relationships.GetTarget(drawing_rId);

                string[] spl = drawing_path.Split(new char[] { '/' });
                XmlNode drawing_file_xml = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/" + spl[spl.Length - 1], xml_worksheet.OwnerDocument.DocumentElement);

                if (drawing_file_xml == null)
                    return false;

                XmlNodeList anchors = drawing_file_xml.SelectNodes("xdr:wsDr/xdr:twoCellAnchor", ns_manager);

                // делаем копию и задаём новые параметры
                if (anchors.Count > 0)
                {
                    FileStream stream = new FileStream(fileName, FileMode.Open);
                    byte[] buffer = new byte[stream.Length];
                    stream.Read(buffer, 0, (int)(stream.Length));
                    stream.Close();

                    workbook.BinaryDataStorage.Add(buffer);

                    XmlNode image_file_xml = Helpers.OOXNavigate.AddBinFile("xl/media/" + binName, xml_worksheet.OwnerDocument.DocumentElement, false, workbook.BinaryDataStorage.Count - 1);
                    OOX.Relationships drawing_relationships = new OOX.Relationships(drawing_file_xml);
                    drawing_relationships.AddTarget("../media/" + binName, ASCOfficeEWSEditor.Editor.OOX.Relationships.FileType.image, false);
                    workbook.ContentTypes.RegisterFile(image_file_xml, contentType);

                    XmlElement resourceDrawing = xml_worksheet.OwnerDocument.CreateElement("tmp");
                    resourceDrawing.InnerXml = Resource1.empty_imageDrawing;
                    XmlNode template = resourceDrawing.SelectSingleNode("xdr:wsDr/xdr:twoCellAnchor", ns_manager);

                    XmlNode newAnchor = template.Clone();
                    Image img = new Image(newAnchor, ns_manager);

                    img.image_url = newObject.image_url;
                    img.image_guid = newObject.object_guid;

                    // from
                    img.from_col = newObject.from_col;
                    img.from_colOff = newObject.from_colOff;
                    img.from_row = newObject.from_row;
                    img.from_rowOff = newObject.from_rowOff;

                    // to
                    img.to_col = newObject.to_col;
                    img.to_colOff = newObject.to_colOff;
                    img.to_row = newObject.to_row;
                    img.to_rowOff = newObject.to_rowOff;

                    img.image_width = newObject.object_width;
                    img.image_height = newObject.object_height;

                    img.image_xOff = newObject.object_xOff;
                    img.image_yOff = newObject.object_yOff;

                    // id                
                    img.pic_id = workbook.BinaryDataStorage.Count - 1;

                    // name
                    img.pic_name = "Image " + workbook.BinaryDataStorage.Count.ToString();

                    // rId                                   
                    img.pic_rid = "rId" + drawing_relationships.GetLastRid().ToString();

                    SimpleImages.Add(img);

                    XmlNode docRoot = drawing_file_xml.SelectSingleNode("xdr:wsDr", ns_manager);
                    docRoot.AppendChild(newAnchor);
                }
            }
            return true;
        }

        public bool DeleteImage(Workbook workbook, DrawingTransFromJson newObject, string siteDir)
        {
            // поиск всех зависимостей
            XmlElement drawing_node = (XmlElement)xml_worksheet.SelectSingleNode("main:drawing", ns_manager);
            if (drawing_node == null)
                return false;

            string drawing_rId = drawing_node.GetAttribute("id", ns_manager.LookupNamespace("mainr"));
            Relationships = new OOX.Relationships(xml_worksheet.ParentNode);
            string drawing_path = Relationships.GetTarget(drawing_rId);

            string[] spl = drawing_path.Split(new char[] { '/' });
            XmlNode drawing_file_xml = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/" + spl[spl.Length - 1], xml_worksheet.OwnerDocument.DocumentElement);

            //XmlNode drawing_file_xml = Helpers.OOXNavigate.getReferenceByPath(drawing_path, xml_worksheet.ParentNode.ParentNode);
            if (drawing_file_xml == null)
                return false;

            for (int i = 0; i < workbook.ActiveWorksheet.SimpleImages.Count; i++)
            {
                if (workbook.ActiveWorksheet.SimpleImages[i].pic_id == newObject.object_id)
                {
                    workbook.ActiveWorksheet.SimpleImages.RemoveAt(i);
                    break;
                }
            }

            XmlNodeList anchors = drawing_file_xml.SelectNodes("xdr:wsDr/xdr:twoCellAnchor", ns_manager);
            foreach (XmlNode anchor_xml in anchors)
            {
                XmlElement imageId = (XmlElement)anchor_xml.SelectSingleNode("xdr:pic/xdr:blipFill/a:blip", ns_manager);
                if (imageId == null)
                    continue;

                Image img = new Image(anchor_xml, ns_manager);

                if (img.pic_id == newObject.object_id)
                {
                    anchor_xml.ParentNode.RemoveChild(anchor_xml);

                    // find image path
                    OOX.Relationships drawing_relationships = new OOX.Relationships(drawing_file_xml);
                    string image_doc_path = drawing_relationships.GetTarget(img.pic_rid);
                    XmlNode image_file_xml = Helpers.OOXNavigate.getReferenceByPath(image_doc_path, drawing_file_xml.ParentNode);

                    workbook.ContentTypes.DeRegisterFile(Helpers.OOXNavigate.getReferenceByPath("xl/media/" + Path.GetFileName(image_doc_path), xml_worksheet.OwnerDocument.DocumentElement));
                    drawing_relationships.RemoveTarget(img.pic_rid);

                    XmlNode ret_xml = Helpers.OOXNavigate.DeleteBinFile("xl/media/" + Path.GetFileName(image_doc_path), xml_worksheet.OwnerDocument.DocumentElement);

                    // удаляем drawing в листе
                    if (drawing_relationships.GetLastRid() == 0)
                    {
                        string drawingName = "drawing" + (workbook.ActiveSheet.Index + 1).ToString() + ".xml";
                        string sheetName = "sheet" + (workbook.ActiveSheet.Index + 1).ToString() + ".xml";
                        XmlNode removeElement = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/" + drawingName, xml_worksheet.OwnerDocument.DocumentElement);
                        workbook.ContentTypes.DeRegisterFile(removeElement);

                        removeElement = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/_rels/" + drawingName + ".rels", xml_worksheet.OwnerDocument.DocumentElement);
                        workbook.ContentTypes.DeRegisterFile(removeElement);

                        removeElement = Helpers.OOXNavigate.getReferenceByPath("xl/worksheets/_rels/" + sheetName + ".rels", xml_worksheet.OwnerDocument.DocumentElement);
                        workbook.ContentTypes.DeRegisterFile(removeElement);
                        removeElement.ParentNode.RemoveChild(removeElement);

                        xml_worksheet.RemoveChild(drawing_node);

                        XmlNode drawing_xml = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/" + drawingName, xml_worksheet.OwnerDocument.DocumentElement);
                        drawing_xml.ParentNode.RemoveChild(drawing_xml);

                        XmlNode drawing_rels_xml = Helpers.OOXNavigate.getReferenceByPath("xl/drawings/_rels/" + drawingName + ".rels", xml_worksheet.OwnerDocument.DocumentElement);
                        drawing_rels_xml.ParentNode.RemoveChild(drawing_rels_xml);
                    }
                    break;
                }
            }

            return true;
        }

        internal Calculation.SubjectsStorage SubjectsStorage { get; private set; }
        internal Dictionary<Cells.Cell, Calculation.Formulas.CellParsedFormula> Formulas { get; private set; }

        internal Calculation.WorksheetSubject Subject { get; private set; }

        internal MergeCells MergeCells { get; private set; }
        /// <summary>
        /// Collection of hyperlinks contained in the worksheet
        /// </summary>
        public Hyperlinks.Hyperlinks Hyperlinks { get; private set; }

        /// <summary>
        /// AutoFilterAllSheet
        /// </summary>
        public AutoFilter AutoFilter { get; private set; }

        /// <summary>
        /// TableAutoFilters
        /// </summary>
        public List<Table> Tables { get; private set; }

        internal XmlNode SheetData { get { return xml_sheetData; } }

        public List<Chart> SimpleCharts { get; private set; }

        public List<Image> SimpleImages { get; private set; }

        internal OOX.Relationships Relationships { get; private set; }
        internal OOX.IOrderedSections XMLSectionsController { get { return worksheet_file; } }

        private OOX.WorksheetFile worksheet_file;

        #region XML references
        private XmlNode xml_worksheet;
        private XmlNode xml_sheetData;
        private XmlNode xml_sheetView;
        #endregion
        #region tree references
        #endregion
    }
}
