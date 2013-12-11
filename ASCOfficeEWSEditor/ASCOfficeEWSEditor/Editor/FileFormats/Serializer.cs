using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using ASCOfficeEWSEditor.Editor;
using System.Diagnostics;


namespace ASCOfficeEWSEditor.Editor.FileFormats
{
    internal class Serializer
    {
        static public void Save(Workbook wb, string path)
        {
            Save(wb, path, Format.AUTO);
        }

        static public Workbook Load(EWSEditor editor, string path)
        {
            return Load(editor, path, Format.AUTO);
        }

        static public void Save(Workbook wb, string path, Format format)
        {
            string temp_dir_path = Path.GetDirectoryName(path) + '\\' + Path.GetFileNameWithoutExtension(path);

            if(format == Format.AUTO)
            {
                format = DetectFormatByExtension(path);
            }
            Trace.TraceInformation("Saving to file \"{0}\" started. Format: {1}", new object[] { path, format });
            switch (format)
            {
                case Format.XLSX:
                {
                    XlsxSaver saver = new XlsxSaver(wb.GetXmlDocument(), wb.BinaryDataStorage);
                    ASCOfficeUtils.COfficeUtilsClass ooxArchiver = new ASCOfficeUtils.COfficeUtilsClass();
                    if (!ooxArchiver.CompressFilesFromMemory(path, saver, -1))
                    {
                        throw new Events.ReportableException(new Events.InternalError("Compression to \"" + path + "\" failed for unknown reason"));
                    }
                    break;
                }
                case Format.XLS:
                {
                    XlsxSaver.Save(wb.GetXmlDocument(), temp_dir_path, wb.BinaryDataStorage);
                    ASCOfficeXlsFile.CXlsFileClass converter = new ASCOfficeXlsFile.CXlsFileClass();
                    converter.SaveToFile(path, temp_dir_path, "");
                    break;
                }
                case Format.HTML:
                {
                    XlsxSaver.Save(wb.GetXmlDocument(), temp_dir_path, wb.BinaryDataStorage);

                    ASCOfficeWSHtmlFile.CWSHtmlFileClass converter = new ASCOfficeWSHtmlFile.CWSHtmlFileClass();
                    converter.SaveToFile(path, temp_dir_path, "");
                    break;
                }
                case Format.ODS:
                {
                    XlsxSaver saver = new XlsxSaver(wb.GetXmlDocument(), wb.BinaryDataStorage);
                    ASCOfficeUtils.COfficeUtilsClass ooxArchiver = new ASCOfficeUtils.COfficeUtilsClass();
                    if (!ooxArchiver.CompressFilesFromMemory(temp_dir_path + ".xlsx", saver, -1))
                    {
                        throw new Events.ReportableException(new Events.InternalError("Compression to \"" + path + "\" failed for unknown reason"));
                    }

                    ODSConverter.ODSConverter.ConvertFile(temp_dir_path + ".xlsx", path, CleverAge.OdfConverter.OdfConverterLib.ConversionDirection.XlsxToOds);
                    break;
                }
                default:
                    throw new Events.ReportableException(new Events.UnsupportedFileFormatError(format));
            }
            Trace.TraceInformation("Saving to file successfully finished");
        }

        static public void SaveToCSV(Workbook wb, string path, int code_page, char delimiter)
        {
            Trace.TraceInformation("Saving to CSV file \"{0}\" started. Code page: {1} Delimiter: \"{2}\"",
                                    new object[] { path, code_page, delimiter });
            CsvSaver.Save(wb, path, code_page, delimiter);
            Trace.TraceInformation("Saving to CSV file successfully finished");
        }
        static public void SaveToPrint(Workbook wb, string path, int index )
        {
            Trace.TraceInformation("Saving to Print file \"{0}\" started.");
            string sIndex = index.ToString();//формируем трехзначную индексную строку
            string temp_dir_path = Path.GetDirectoryName(path) + '/' + Path.GetFileNameWithoutExtension(path);
            XlsxSaver.Save(wb.GetXmlDocument(), temp_dir_path, wb.BinaryDataStorage);
            ASCOfficeWSHtmlFile.CWSHtmlFileClass converter = new ASCOfficeWSHtmlFile.CWSHtmlFileClass();
            converter.SaveToFile(path, temp_dir_path, "<Options><HTML><Print val = \"true\" ind=\"" + index.ToString() + "\"/></HTML></Options>");
            Trace.TraceInformation("Saving to Print file successfully finished");
        }

        static public Workbook Load(EWSEditor editor, string path, Format format)
        {
            if (format == Format.AUTO)
            {
                ASCOfficeFile.CAVSOfficeFormatCheckerClass oFormatChecker = new ASCOfficeFile.CAVSOfficeFormatCheckerClass();
                int nCheckerFormat = oFormatChecker.GetFileFormat(path);
                switch (nCheckerFormat)
                {
                    case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX: format = Format.XLSX; break;
                    case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS: format = Format.XLS; break;
                    case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS: format = Format.ODS; break;
                    case FileConverterUtils2.FileFormats.AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV: format = Format.CSV; break;
                    default: format = DetectFormatByExtension(path); break;
                }
            }
            Trace.TraceInformation("Loading from file \"{0}\" started. Format: {1}", new object[] { path, format });

            if(!File.Exists(path))
            {
                throw new Events.ReportableException(new Events.FileNotFoundError(Path.GetFullPath(path)));
            }

            Workbook wb = null;
            switch (format)
            {
                case Format.XLSX:
                {
                    XmlDocument xml_doc = new XmlDocument();
                    xml_doc.LoadXml("<?xml version=\"1.0\" encoding=\"utf-8\" ?> <root/>");

                    ASCOfficeUtils.COfficeUtilsClass ooxArchiver = new ASCOfficeUtils.COfficeUtilsClass();
                    List<byte[]> binary_data_storage = new List<byte[]>();
                    XlsxLoader data_receiver = new XlsxLoader(xml_doc, binary_data_storage);
                    if (!ooxArchiver.ExtractFilesToMemory(path, data_receiver))
                    {
                        throw new Events.ReportableException(new Events.InternalError("Extracting from \"" + path + "\" failed for unknown reason"));
                    }

                    wb = new Workbook(Path.GetFileNameWithoutExtension(path), xml_doc, editor, binary_data_storage);

                    editor.Workbooks.Add(wb.Name, wb);
                    break;
                }
                case Format.XLS:
                {
                    string temp_dir_path = Path.GetDirectoryName(path) + '/' + Path.GetFileNameWithoutExtension(path);

                    XmlDocument xml_doc = new XmlDocument();
                    xml_doc.LoadXml("<?xml version=\"1.0\" encoding=\"utf-8\" ?> <root/>");

                    ASCOfficeXlsFile.CXlsFileClass converter = new ASCOfficeXlsFile.CXlsFileClass();
                    converter.LoadFromFile(path, temp_dir_path, "");
                    List<byte[]> binary_data_storage = new List<byte[]>();
                    XlsxLoader.LoadXML(xml_doc, temp_dir_path, binary_data_storage);
                    wb = new Workbook(Path.GetFileNameWithoutExtension(path), xml_doc, editor, binary_data_storage);
                    editor.Workbooks.Add(wb.Name, wb);
                    break;
                }
                default:
                    throw new Events.ReportableException(new Events.UnsupportedFileFormatError(format));

            }
            Trace.TraceInformation("Loading from file successfully finished");
            wb.Activate();
            return wb;
        }

        static public Workbook LoadFromCSV(EWSEditor editor, string path, int code_page, char delimiter)
        {
            Trace.TraceInformation("Loading from CSV file \"{0}\" started. Code page: {1} Delimiter: \"{2}\"",
                                    new object[] { path, code_page, delimiter });

            if (!File.Exists(path))
            {
                throw new Events.ReportableException(new Events.FileNotFoundError(Path.GetFullPath(path)));
            }

            Workbook wb = editor.CreateWorkbook(Path.GetFileNameWithoutExtension(path), 1);
            CsvLoader.Load(wb, path, code_page, delimiter);
            Trace.TraceInformation("Loading from CSV file successfully finished");
            wb.Activate();
            return wb;
        }

        static private Format DetectFormatByExtension(string path)
        {
            string ext = Path.GetExtension(path).ToLower();
            switch (ext)
            {
                case ".xls":
                    return Format.XLS;
                case ".ods":
                    return Format.ODS;
                case ".csv":
                    return Format.CSV;
                case ".htm":
                case ".html":
                    return Format.HTML;
                case ".txt":
                    return Format.TXT;
                case ".xlsx":
                case "":
                default:
                    return Format.XLSX;
            }
        }

        public const char CSV_default_delimiter = ',';
    }
    /// <summary>
    /// Worksheet file format
    /// </summary>
    public enum Format 
    {
        /// <summary>
        /// *.xlsx Microsoft Excel 2007
        /// </summary>
        XLSX,
        /// <summary>
        /// *.xls Microsoft Excel 97-2003
        /// </summary>
        XLS, 
        /// <summary>
        /// *.ods OpenOffice.org Calc
        /// </summary>
        ODS, 
        /// <summary>
        /// *.csv Tab-delimited data
        /// </summary>
        HTML, 
        /// <summary>
        /// *.html Html files
        /// </summary>
        CSV, 
        /// <summary>
        /// *.txt Text format
        /// </summary>
        TXT, 
        /// <summary>
        /// *.xml Internal AVS format
        /// </summary>
        AVS_INTERNAL, 
        /// <summary>
        /// *.* Automatic format detection
        /// </summary>
        AUTO 
    };

}
