using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;
using AVSOfficeEWSEditor.Editor.Walkers;

namespace AVSOfficeEWSEditor.Editor
{
    /// <summary>
    /// Represents a workbook.
    /// </summary>
    public class Workbook : IDisposable
    {
        internal Workbook(string wb_name, XmlDocument document, EWSEditor editor, List<byte[]> binary_data_storage)
        {
            InitNamespaceManager(document);
            xml_doc = document;
            Editor = editor;
            Name = wb_name;
            BinaryDataStorage = binary_data_storage;
            xml_workbook = Helpers.OOXNavigate.FindWorkbookTag(document);
            ValuesUpdateManager = new Calculation.ValuesUpdateManager();
            if (null == xml_workbook)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong document format. Workbook tag hasn't been found."));
            }
            Relationships = new OOX.Relationships(xml_workbook.ParentNode);
            ContentTypes = new OOX.ContentTypes(document);
            StyleSheet = new StyleSheet(ContentTypes, Relationships, this);
            Theme = new Theme(ContentTypes, Relationships, this);
            Sheets = new Sheets(this);
            Worksheets = new Worksheets(Sheets);
            SharedStrings = new SharedStrings(ContentTypes, Relationships, this);

            RemoveCalculationChain();
            DefinedNames = new DefinedNames.DefinedNames(this);
            DefinedNames.RegisterFormulas();
            foreach (Worksheet ws in Worksheets)
            {
                ws.BuildFormulasDependancies();
            }
            ////если в ячейке есть формула, но нет значения - пересчитывать формулу, с целью получения значения.
            //foreach (Worksheet ws in Worksheets)
            //{
            //    FormulaRecheck formula_updater = new FormulaRecheck(ws);
            //    RangeWalker low_part_walker = new RangeWalker(Helpers.ReferencesFormat.maxRange(), ws);
            //    low_part_walker.UpdateValues(formula_updater);
            //}
            workbook_file = new OOX.WorkbookFile(ContentTypes, Relationships, this);
        }

        internal Workbook(string wb_name, XmlDocument document, EWSEditor editor)
            : this(wb_name, document, editor, new List<byte[]>())
        {
        }

        private void InitNamespaceManager(XmlDocument document)
        {
            NamespaceManager = new XmlNamespaceManager(document.NameTable);
            NamespaceManager.AddNamespace("main", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            NamespaceManager.AddNamespace("r", "http://schemas.openxmlformats.org/package/2006/relationships");
            NamespaceManager.AddNamespace("mainr", "http://schemas.openxmlformats.org/officeDocument/2006/relationships");
            NamespaceManager.AddNamespace("xdr", "http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing");
            NamespaceManager.AddNamespace("a", "http://schemas.openxmlformats.org/drawingml/2006/main");
            NamespaceManager.AddNamespace("c", "http://schemas.openxmlformats.org/drawingml/2006/chart");
        }

        private void RemoveCalculationChain()
        {
            OOX.XmlFile calc_chain_file = new OOX.XmlFile(ContentTypes, Relationships, GetRootTag().ParentNode.ParentNode,
                        Relationships.GetTarget(OOX.Relationships.FileType.calcChain), OOX.Relationships.FileType.calcChain, OOX.ContentTypes.FileType.calcChain);
            calc_chain_file.Remove();
        }

        /// <summary>
        /// Stores workbook to the file. Format is obtained from extension of filename
        /// </summary>
        /// <param name="path">Path for storing</param>
        /// <returns>OK/NOK</returns>
        public bool Save(string path)
        {
            return Save(path, FileFormats.Format.AUTO);
        }

        /// <summary>
        /// Stores workbook to the file of specified format
        /// </summary>
        /// <param name="path">Path for storing</param>
        /// <param name="format">File format</param>
        /// <returns>OK/NOK</returns>
        public bool Save(string path, FileFormats.Format format)
        {
            try
            {
                FileFormats.Serializer.Save(this, path, format);
                return true;
            }
            catch (Events.ReportableException reportable_exception)
            {
                Editor.ReportError(null, reportable_exception.error_description);
                return false;
            }
            catch (System.IO.IOException io_exception)
            {
                Editor.ReportError(null, new Events.SystemIOError(io_exception));
                return false;
            }
        }

        /// <summary>
        /// Stores workbook to the specified CSV file
        /// </summary>
        /// <param name="path">Path for storing</param>
        /// <param name="code_page">Code page</param>
        /// <param name="delimiter">Cells delimiter</param>
        /// <returns>OK/NOK</returns>
        public bool SaveToCSV(string path, int code_page, char delimiter)
        {
            try
            {
                FileFormats.Serializer.SaveToCSV(this, path, code_page, delimiter);
                return true;
            }
            catch (Events.ReportableException reportable_exception)
            {
                Editor.ReportError(null, reportable_exception.error_description);
                return false;
            }
            catch (System.IO.IOException io_exception)
            {
                Editor.ReportError(null, new Events.SystemIOError(io_exception));
                return false;
            }
        }

        /// <summary>
        /// Stores workbook to the specified CSV file with default cells delimiter (,)
        /// </summary>
        /// <param name="path">Path for storing</param>
        /// <param name="code_page">Code page</param>
        /// <returns>OK/NOK</returns>
        public bool SaveToCSV(string path, int code_page)
        {
            return SaveToCSV(path, code_page, FileFormats.Serializer.CSV_default_delimiter);
        }


        public bool SaveToPrint(string path, int nIndex)
        {
            try
            {
                FileFormats.Serializer.SaveToPrint(this, path, nIndex);
                return true;
            }
            catch (Events.ReportableException reportable_exception)
            {
                Editor.ReportError(null, reportable_exception.error_description);
                return false;
            }
            catch (System.IO.IOException io_exception)
            {
                Editor.ReportError(null, new Events.SystemIOError(io_exception));
                return false;
            }
        }
        /// <summary>
        /// Sets the workbook active.
        /// </summary>
        public void Activate()
        {
            Editor.ActiveWorkbook = this;
        }

        /// <summary>
        /// Collection of all sheets in the workbook. Read-only
        /// </summary>
        public Sheets Sheets { get; private set; }

        /// <summary>
        /// Returns collection of all worksheets in the workbook. 
        /// </summary>
        public Worksheets Worksheets { get; private set; }

//         /// <summary>
//         /// Returns collection of all worksheets in the workbook. 
//         /// </summary>
//         public Worksheets Worksheets()
//         {
//             return new Worksheets(Sheets);
//         }
//         public IEnumerable<Worksheet> Worksheets
//         {
//             get
//             {
//                 foreach (Sheet sheet in Sheets)
//                 {
//                     Worksheet ws = sheet as Worksheet;
//                     if (ws != null)
//                     {
//                         yield return ws;
//                     }
//                 }
//             }
//         }

        /// <summary>
        /// Returns collection of worksheets in the specified indices range (inclusive boundaries)
        /// </summary>
        public IEnumerable<Worksheet> WorksheetsN(int from_index, int to_index)
        {
            foreach (Sheet sheet in Sheets)
            {
                int index = sheet.Index;
                if (index >= from_index && index <= to_index)
                {
                    Worksheet ws = sheet as Worksheet;
                    if (ws != null)
                    {
                        yield return ws;
                    }
                }
            }
        }


        /// <summary>
        /// Active sheet. Read-only
        /// </summary>
        public Sheet ActiveSheet 
        {
            get
            {
                XmlNode activeTab = xml_workbook.SelectSingleNode("main:bookViews/main:workbookView/@activeTab", NamespaceManager);
                return Sheets[null != activeTab ? XMLInt.ParseOrDefaultAndReport(activeTab.InnerText, 0, this, "workbookView/@activeTab") : 0];
            }
            private set{}
        }


        /// <summary>
        /// Active worksheet. Read-only
        /// </summary>
        public Worksheet ActiveWorksheet 
        {
            get
            {
                return ActiveSheet as Worksheet;
            }
        }

        /// <summary>
        /// Name of the workbook. Read-only. (use SetName to change the name of the workbook)
        /// </summary>
        public string Name { get; internal set;}

        /// <summary>
        /// Sets the new name for the workbook
        /// </summary>
        /// <param name="name">new name</param>
        /// <returns>Whether the assignment was successful</returns>
        public bool SetName(string name)
        {
            if (name.Length == 0)
            {
                return false;
            }
            if(Name == name)
            {
                return true;
            }
            if (Editor.Workbooks.ContainsKey(name))
            {
                return false;
            }
            Editor.Workbooks.Remove(Name);
            Name = name;
            Editor.Workbooks.Add(name, this);
            return true;            
        }

        /// <summary>
        /// Close the workbook
        /// </summary>
        public void Close()
        {
            Editor.Workbooks.Remove(Name);
        }

        internal void ActivateSheet(int index)
        {
            XmlElement workbookView = (XmlElement)xml_workbook.SelectSingleNode("main:bookViews/main:workbookView", NamespaceManager);
            if(null == workbookView)
            {
                //AddBookView(); // TODO: implement
                workbookView = (XmlElement)xml_workbook.SelectSingleNode("main:bookViews/main:workbookView", NamespaceManager);
            }
            workbookView.SetAttribute("activeTab", index.ToString());
        }

        /// <summary>
        /// Registers a new or already existent shared string in SharedStrings file.
        /// </summary>
        /// <param name="ssi">String to register</param>
        /// <param name="used_times">Number times the string value is to be additionally used in </param>
        /// <returns>Offset reference (0-based)</returns>
        internal int RegisterSharedString(string ssi, int used_times)
        {
            return SharedStrings.SetString(ssi, used_times);
        }

        /// <summary>
        /// Obtain &lt;workbook&gt; tag
        /// </summary>
        /// <returns></returns>
        internal XmlNode GetRootTag()
        {
            return xml_workbook;
        }

        internal XmlDocument GetXmlDocument()
        {
            return xml_doc;
        }

        internal void ReportError(Events.ErrorDescription description)
        {
            Editor.ReportError(this, description);
        }

        /// <summary>
        /// Collection of defined names contained in the workbook
        /// </summary>
        public DefinedNames.DefinedNames DefinedNames { get; private set; }

        /// <summary>
        /// Default font name of the workbook (set as style 0).
        /// </summary>
        public string DefaultFontName
        {
            get
            {
                return StyleSheet.GetZeroStyleOrDefaultFontName();
            }
        }

        /// <summary>
        /// Default font size of the workbook (set as style 0).
        /// </summary>
        public int DefaultFontSize
        {
            get
            {
                return StyleSheet.GetZeroStyleOrDefaultFontSize();
            }
        }

        #region IDisposable Members
        /// <summary>
        /// Simply closes the workbook
        /// </summary>
        public void Dispose()
        {
            Close();
        }

        #endregion

        internal OOX.IOrderedSections XMLSectionsController { get { return workbook_file; } }

        internal OOX.Relationships Relationships { get; private set; }
        internal OOX.ContentTypes ContentTypes { get; private set; }
        internal SharedStrings SharedStrings { get; private set; }
        internal StyleSheet StyleSheet { get; private set; }
        internal Theme Theme { get; private set; }
        internal Calculation.ValuesUpdateManager ValuesUpdateManager { get; private set; }
        internal List<byte[]> BinaryDataStorage { get; private set; }


        private OOX.WorkbookFile workbook_file;
        #region XML references
        private XmlDocument xml_doc;
        private XmlNode xml_workbook;
        #endregion
        #region Tree references
        internal EWSEditor Editor { get; private set; }
        #endregion

        internal XmlNamespaceManager NamespaceManager { get; private set; }
    }
}
