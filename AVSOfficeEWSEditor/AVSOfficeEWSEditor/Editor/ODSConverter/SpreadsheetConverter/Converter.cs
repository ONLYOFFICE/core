using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;
using System.Xml;
using CleverAge.OdfConverter.OdfConverterLib;
using System.IO;


namespace CleverAge.OdfConverter.Spreadsheet
{
    public class Converter : AbstractConverter
    {

        private const string ODF_MIME_TYPE_SPREADSHEET = "application/vnd.oasis.opendocument.spreadsheet";
        private const string ODF_MIME_TYPE_SPREADSHEET_TEMPLATE = "application/vnd.oasis.opendocument.spreadsheet-template";
        private const string OOX_MIME_TYPE_SPREADSHEET = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml";
        private const string OOX_MIME_TYPE_SPREADSHEET_TEMPLATE = "application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml";
        private const string OOX_MIME_TYPE_SPREADSHEET_MACRO = "application/vnd.ms-excel.sheet.macroEnabled.main+xml";
        private const string OOX_MIME_TYPE_SPREADSHEET_TEMPLATE_MACRO = "application/vnd.ms-excel.template.macroEnabled.main+xml";


        public Converter()
            : base(Assembly.GetExecutingAssembly())
        { }
        protected override Type LoadPrecompiledXslt()
        {
            Type stylesheet = null;
            try
            {
                if (this.DirectTransform)
                {
                    stylesheet = Assembly.Load("SpreadsheetConverter2Oox")
                                            .GetType("SpreadsheetConverter2Oox");
                }
                else
                {
                    stylesheet = Assembly.Load("SpreadsheetConverter2Odf")
                                            .GetType("SpreadsheetConverter2Odf");
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.ToString());
                return null;
            }
            return stylesheet;
        }
        protected override string[] DirectPostProcessorsChain
        {
            get
            {
                string fullname = Assembly.GetExecutingAssembly().FullName;
                return new string[]  {
                   "CleverAge.OdfConverter.Spreadsheet.OdfConditionalPostProcessor,"+fullname,
                   "CleverAge.OdfConverter.Spreadsheet.OoxCommentsPostProcessor,"+fullname,
                   "CleverAge.OdfConverter.Spreadsheet.OoxDrawingsPostProcessor,"+fullname,
                   "CleverAge.OdfConverter.Spreadsheet.OoxHeaderFooterPostProcessor,"+fullname,
				   "CleverAge.OdfConverter.Spreadsheet.OOXStyleCellPostProcessor,"+fullname,
                   "CleverAge.OdfConverter.Spreadsheet.OoxMaximumCellTextPostProcessor,"+fullname,
                   "CleverAge.OdfConverter.Spreadsheet.OoxPhysicalPathPostProcessor,"+fullname,
                   "CleverAge.OdfConverter.Spreadsheet.OoxPivotCrashHandlingPostProcessor,"+fullname,
                   "CleverAge.OdfConverter.Spreadsheet.OoxPivotCachePostProcessor,"+fullname
                };
            }
        }
        protected override string[] ReversePostProcessorsChain
        {
            get
            {
                string fullname = Assembly.GetExecutingAssembly().FullName;
                return new string[]  {
                    "CleverAge.OdfConverter.Spreadsheet.OdfSharedStringsPostProcessor,"+fullname,
                    "CleverAge.OdfConverter.Spreadsheet.OOXGroupsPostProcessor,"+fullname
                };
            }
        }

        protected override void CheckOdfFile(string fileName)
        {
            // Test for encryption
            XmlDocument doc, docContent;
            try
            {
                XmlReaderSettings settings = new XmlReaderSettings();
                settings.XmlResolver = new ZipResolver(fileName);
                settings.ProhibitDtd = false;
                doc = new XmlDocument();
                XmlReader reader = XmlReader.Create("META-INF/manifest.xml", settings);
                doc.Load(reader);

                XmlNodeList nodes = doc.GetElementsByTagName("encryption-data", "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0");
                if (nodes.Count > 0)
                {
                    throw new EncryptedDocumentException(fileName + " is an encrypted document");
                }

                /*Code Changed By:Vijayeta 
				    Defect ID      :1740412
					Date           :20th Dec '07
                    Description    :On an attempt to load content.xml, if content.xml is absent, an invalid file message is thrown */
                docContent = new XmlDocument();
                XmlReader readerContent = XmlReader.Create("content.xml", settings);
                docContent.Load(readerContent);
                //End Of code change, for the defect 1740412
            }
            catch (XmlException ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.ToString());
                throw new NotAnOdfDocumentException(ex.Message, ex);
            }


            // Check the document mime-type.
            XmlNamespaceManager nsmgr = new XmlNamespaceManager(doc.NameTable);
            nsmgr.AddNamespace("manifest", "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0");

            XmlNode node = doc.SelectSingleNode("/manifest:manifest/manifest:file-entry["
                                                    + "@manifest:media-type='" + ODF_MIME_TYPE_SPREADSHEET + "'"
                                                    + "or @manifest:media-type='" + ODF_MIME_TYPE_SPREADSHEET_TEMPLATE + "']", nsmgr);

            if (node == null)
            {
                throw new NotAnOdfDocumentException("Could not convert " + fileName
                                                    + ". Invalid OASIS OpenDocument file");
            }
        }

        // Code to fix the bug# 1698280 

        protected override void CheckOoxFile(string fileName)
        {

            XmlDocument doc;
            try
            {
                XmlReaderSettings settings = new XmlReaderSettings();
                settings.XmlResolver = new ZipResolver(fileName);
                settings.ProhibitDtd = false;
                doc = new XmlDocument();
                XmlReader reader = XmlReader.Create("[Content_Types].xml", settings);
                doc.Load(reader);
            }
            catch (XmlException ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.ToString());
                throw new NotAnOoxDocumentException(ex.Message, ex);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Trace.WriteLine(ex.ToString());
                throw;
            }
            XmlNodeList nodelist = doc.SelectNodes("//node()[@ContentType='" + OOX_MIME_TYPE_SPREADSHEET + "'"
                                                    + " or @ContentType='" + OOX_MIME_TYPE_SPREADSHEET_TEMPLATE + "'"
                                                    + " or @ContentType='" + OOX_MIME_TYPE_SPREADSHEET_MACRO + "'"
                                                    + " or @ContentType='" + OOX_MIME_TYPE_SPREADSHEET_TEMPLATE_MACRO + "']");
            if (nodelist.Count == 0)
            {
                throw new NotAnOoxDocumentException("not a valid oox file");
            }

        }

        /// <summary>
        /// Pull the input xml document to the xsl transformation
        /// </summary>
        protected override XmlReader Source(string inputFile)
        {
            if (this.DirectTransform)
            {
                // ODS -> XLSX
                return base.Source(inputFile);
            }
            else
            {
                // use performance improved method for
                // DOCX -> ODT conversion
                XmlReaderSettings xrs = new XmlReaderSettings();
                // do not look for DTD
                xrs.ProhibitDtd = false;

                XlsxDocument doc = new XlsxDocument(inputFile);

                // uncomment for testing
                for (int i = 0; i < Environment.GetCommandLineArgs().Length; i++)
                {
                    if (Environment.GetCommandLineArgs()[i].ToString().ToUpper() == "/DUMP")
                    {
                        StreamReader package = new StreamReader(doc.OpenXML);
                        FileInfo fi = new FileInfo(Environment.GetCommandLineArgs()[i + 1]);

                        XmlWriterSettings settings = new XmlWriterSettings();
                        settings.Indent = true;
                        settings.IndentChars = ("    ");
                        settings.OmitXmlDeclaration = true;
                        using (XmlWriter writer = XmlWriter.Create(fi.FullName, settings))
                        {
                            writer.WriteRaw(package.ReadToEnd());
                            writer.Flush();
                        }



                        //using (StreamWriter sw = new XmlTextWriter File.Create(fi.FullName, (int)package.Length))
                        //{
                        //    char[] buffer = new char[package.Length];
                        //    package.Read(buffer, 0, (int)package.Length);
                        //    sw.Write(buffer, 0, (int)package.Length);
                        //    sw.Close();
                        //}
                    }
                }

                return XmlReader.Create(doc.OpenXML, xrs);
            }
        }

    }
}
