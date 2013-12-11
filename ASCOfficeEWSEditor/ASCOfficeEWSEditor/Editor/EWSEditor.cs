using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Diagnostics;

/*
 * My main idea in this project is to minimize amount of stored in classes information
 * that duplicates the one stored in linked XML. 
 * Since the performance is not sufficient here, we can access all the info from the XML itself to 
 * keep the tree in consistency and avoid the majority of errors.
 * If there is a necessity to duplicate - do it as little as possible.
 */


namespace ASCOfficeEWSEditor.Editor
{
    /// <summary>
    /// Represents the entire application
    /// </summary>
    public class EWSEditor : IDisposable
    {
        /// <summary>
        /// Constructs an EWSEditor instance
        /// </summary>
        public EWSEditor()
        {
            Workbooks = new Dictionary<string, Workbook>();
            ReadDefaultFontPropertiesFromXML();
            FunctionsStorage = new Calculation.Formulas.Ptgs.Functions.FunctionsStorage();
            NumberFormatsStorage = new NumberFormat.NumberFormatsStorage();
            //SetupLog("EWSEditor.log", TraceOptions.Callstack);
        }
        public EWSEditor(string siteDir)
        {
            site_dir = siteDir;
            Workbooks = new Dictionary<string, Workbook>();
            ReadDefaultFontPropertiesFromXML();
            FunctionsStorage = new Calculation.Formulas.Ptgs.Functions.FunctionsStorage();
            NumberFormatsStorage = new NumberFormat.NumberFormatsStorage();
            //SetupLog("EWSEditor.log", TraceOptions.Callstack);            
        } 

        private void ReadDefaultFontPropertiesFromXML()
        {
            XmlDocument xml_doc = new XmlDocument();
            xml_doc.LoadXml(Resource1.empty_book);
            Workbook wb = new Workbook("temp_workbook", xml_doc, this);
            string font_name0 = wb.StyleSheet.Get0StyleFontName();
            int? font_size0 = wb.StyleSheet.Get0StyleFontSize();

            if (font_name0 != null)
            {
                DefaultFontNameOnBooksCreation = font_name0;
            }
            else
            {
                DefaultFontNameOnBooksCreation = "Arial";
            }
            if (font_size0 != null)
            {
                DefaultFontSizeOnBooksCreation = font_size0.Value;
            }
            else
            {
                DefaultFontSizeOnBooksCreation = 11;
            }
        }

        /// <summary>
        /// Creates an empty workbook and appends it to the EWSEditor internal list. The workbook becomes active
        /// </summary>
        /// <param name="name">Name of the created workbook</param>
        /// <returns>Just created workbook reference</returns>
        public Workbook CreateWorkbook(string name)
        {
            return CreateWorkbook(name, 1);
        }

        /// <summary>
        /// Creates an empty workbook and appends it to the EWSEditor internal list. The workbook becomes active
        /// </summary>
        /// <param name="name">Name of the created workbook</param>
        /// <param name="num_sheets">Number of empty sheets in the created workbook</param>
        /// <returns>Just created workbook reference</returns>
        public Workbook CreateWorkbook(string name, int num_sheets)
        {
            Trace.TraceInformation("Workbook creation issued. Name: \"{0}\" Number of sheets: {1}", new object[] {name, num_sheets});

            XmlDocument xml_doc = new XmlDocument();
            xml_doc.LoadXml(Resource1.empty_book);
            Workbook wb;
            try
            {
                wb = new Workbook(name, xml_doc, this);
            }
            catch (Events.ReportableException reportable_exception)
            {
                ReportError(null, reportable_exception.error_description);
                return null;
            }
            // TODO: set default font properties
            Workbooks.Add(name, wb);
            ActiveWorkbook = wb;
            for (int i = 2; i <= num_sheets; ++i)
            {
                wb.Sheets.AddEmptySheet("Sheet" + i, null);
            }
            wb.Sheets[0].Activate();
            return wb;
        }

        /// <summary>
        /// Loads a workbook from the specified path and appends it to the EWSEditor internal list. The workbook becomes active
        /// </summary>
        /// <param name="path">File path to the workbook location.</param>
        /// <returns>Just loaded workbook reference.</returns>
        public Workbook LoadWorkbook(string path)
        {
            Workbook wb;
            try
            {
                wb = FileFormats.Serializer.Load(this, path);
            }
            catch (Events.ReportableException reportable_exception)
            {
                ReportError(null, reportable_exception.error_description);
                return null;
            }
            catch (System.IO.IOException io_exception)
            {
                ReportError(null, new Events.SystemIOError(io_exception));
                return null;
            }
            return wb;
        }

        /// <summary>
        /// Loads a workbook from CSV file at the specified path and appends it to the EWSEditor internal list.
        /// The workbook becomes active
        /// </summary>
        /// <param name="path">File path to the CSV file location.</param>
        /// <param name="code_page">Code page of CSV data.</param>
        /// <param name="delimiter">Cells delimiter.</param>
        /// <returns>Just loaded workbook reference.</returns>
        public Workbook LoadWorkbookFromCSV(string path, int code_page, char delimiter)
        {
            Workbook wb;
            try
            {
                wb = FileFormats.Serializer.LoadFromCSV(this, path, code_page, delimiter);
            }
            catch (Events.ReportableException reportable_exception)
            {
                ReportError(null, reportable_exception.error_description);
                return null;
            }
            catch (System.IO.IOException io_exception)
            {
                ReportError(null, new Events.SystemIOError(io_exception));
                return null;
            }
            return wb;
        }

        /// <summary>
        /// Loads a workbook from a CSV file at the specified path and appends it to the EWSEditor internal list.
        /// The workbook becomes active.
        /// Delimiter set to default (,)
        /// </summary>
        /// <param name="path">File path to the CSV file location.</param>
        /// <param name="code_page">Code page of CSV data.</param>
        /// <returns>Just loaded workbook reference.</returns>
        public Workbook LoadWorkbookFromCSV(string path, int code_page)
        {
            return LoadWorkbookFromCSV(path, code_page, FileFormats.Serializer.CSV_default_delimiter);
        }

        /// <summary>
        /// Active workbook reference. Read-only
        /// </summary>
        public Workbook ActiveWorkbook { get; internal set; }

        /// <summary>
        /// Active sheet in active workbook reference. Read-only
        /// </summary>
        public Sheet ActiveSheet { get { return ActiveWorkbook.ActiveSheet; } private set { } }

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
        /// Setup and start logging
        /// </summary>
        /// <param name="file_name">Name of the log file</param>
        /// <param name="trace_options">Output options</param>
        /// <param name="trace_level">Trace level</param>
        public void SetupLog(string file_name, TraceOptions trace_options, SourceLevels trace_level)
        {
            StopLogging();
            try
            {
                log_file = File.Open(file_name, FileMode.Append, FileAccess.Write, FileShare.Read);
                log_file_listener = new TextWriterTraceListener(log_file, "EWSEditorLog");
                Trace.Listeners.Add(log_file_listener);
                Trace.AutoFlush = true;
                log_file_listener.TraceOutputOptions = trace_options;
                log_file_listener.Filter = new System.Diagnostics.EventTypeFilter(trace_level);
            }
            catch (System.SystemException e)
            {
                ReportError(null, new Events.SystemIOError(e));
            }
        }

        /// <summary>
        /// Setup and start logging
        /// </summary>
        /// <param name="file_name">Name of the log file</param>
        /// <param name="trace_options">Output options</param>
        public void SetupLog(string file_name, TraceOptions trace_options)
        {
            SetupLog(file_name, trace_options, SourceLevels.All);
        }

        /// <summary>
        /// Setup and start logging
        /// </summary>
        /// <param name="file_name">Name of the log file</param>
        /// <param name="trace_level">Trace level</param>
        public void SetupLog(string file_name, SourceLevels trace_level)
        {
            SetupLog(file_name, TraceOptions.None, trace_level);
        }

        /// <summary>
        /// Setup and start logging
        /// </summary>
        /// <param name="file_name">Name of the log file</param>
        public void SetupLog(string file_name)
        {
            SetupLog(file_name, TraceOptions.None, SourceLevels.All);
        }

        /// <summary>
        /// Stop logging
        /// </summary>
        public void StopLogging()
        {
            if (log_file_listener != null)
            {
                log_file_listener.Close();
                Trace.Listeners.Remove(log_file_listener);
                log_file_listener = null;

                if (log_file != null)
                {
                    log_file.Close();
                    log_file = null;
                }
            }
        }
        /// <summary>
        /// Collection of all Workbooks opened in the application. Read-only
        /// Key in the tree is the file name without an extension.
        /// </summary>
        public Dictionary<string, Workbook> Workbooks { get; private set; }

        /// <summary>
        /// Default font size that is applied to the newly created workbooks
        /// </summary>
        public int DefaultFontSizeOnBooksCreation { get; set; }

        /// <summary>
        /// Default font name that is applied to the newly created workbooks
        /// </summary>
        public string DefaultFontNameOnBooksCreation { get; set; }

        /// <summary>
        /// An event that can be used to be notified whenever an error in the workbook happened.
        /// 'sender' of EventHandler is Workbook
        /// </summary>
        public event EventHandler ErrorHappenedEvent;

        /// <summary>
        /// Functions information storage
        /// </summary>
        public Calculation.Formulas.Ptgs.Functions.FunctionsStorage FunctionsStorage { get; private set; }

        /// <summary>
        /// Internal number formats storage
        /// </summary>
        public NumberFormat.NumberFormatsStorage NumberFormatsStorage { get; private set; }

        internal void ReportError(Workbook workbook, Events.ErrorDescription description)
        {
            if (ErrorHappenedEvent != null)
            {
                ErrorHappenedEvent(workbook, description);
            }

            Trace.TraceError(description.Text);
        }

        static TraceOptions EnableCallstackTrace()
        {
            TraceListener ls = Trace.Listeners["EWSEditorLog"];
            if (ls != null)
            {
                TraceOptions callback_cache = ls.TraceOutputOptions;
                ls.TraceOutputOptions |= TraceOptions.Callstack;
                return callback_cache;
            }
            return TraceOptions.None;
        }

        static void RestoreTraceOptions(TraceOptions options)
        {
            TraceListener ls = Trace.Listeners["EWSEditorLog"];
            if (ls != null)
            {
                ls.TraceOutputOptions = options;
            }
        }

        #region IDisposable Members

        /// <summary>
        /// Resources releasing
        /// </summary>
        public void Dispose()
        {
            StopLogging();
        }

        #endregion

        private FileStream log_file;
        private TextWriterTraceListener log_file_listener;

        public string site_dir;
    }
}
