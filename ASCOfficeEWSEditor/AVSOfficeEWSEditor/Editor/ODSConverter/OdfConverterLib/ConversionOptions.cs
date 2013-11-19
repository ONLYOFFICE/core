
using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace CleverAge.OdfConverter.OdfConverterLib
{
    /// <summary>
    /// This enum is used to determine the direction of a conversion.
    /// This is needed e.g. to instantiate the correct converter.
    /// </summary>
    public enum ConversionDirection
    {
        None,
        OdtToDocx,
        DocxToOdt,
        OdsToXlsx,
        XlsxToOds,
        OdpToPptx,
        PptxToOdp
    };

    /// <summary>
    /// This enum is used to determine whether we are in batch mode or not
    /// </summary>
    public enum ConversionMode
    {
        SingleDocument,
        Batch
    }

    /// <summary>
    /// The type of document to be converted.
    /// </summary>
    public enum DocumentType
    {
        Document,
        Template
    }

    /// <summary>
    /// An exception to be thrown when any conversion parameters are not valid. 
    /// </summary>
    [global::System.Serializable]
    public class InvalidConversionOptionsException : Exception
    {
        public InvalidConversionOptionsException() { }
        public InvalidConversionOptionsException(string message) : base(message) { }
        public InvalidConversionOptionsException(string message, Exception inner) : base(message, inner) { }
        protected InvalidConversionOptionsException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context)
            : base(info, context) { }
    }


    public class ConversionOptions
    {
        private string _inputFullName = null;           // input path
        private string _inputBaseFolder = null;         // the root path of the input document
        private string _outputFullName = null;          // output path
        private string _outputBaseFolder = null;        // the root path for the output document
        private string _inputFullNameOriginal = null;   // the full path to the original input file name (we might translate a temp copy)
        private bool _validate = false;                 // validate the result of the transformations
        private bool _recursiveMode = false;            // go in subfolders ?
        private bool _forceOverwrite = false;			// override existing files ?

        private string _reportPath = null;              // file to save report
        private ConversionReport _report = null;        // report to write log messages to
        private LogLevel _logLevel = LogLevel.Info;     // file to save report
        private string _xslPath = null;                 // Path to an external stylesheet
        private List<string> _skippedPostProcessors = new List<string>();  // Post processors to skip (identified by their names)
        private bool _packaging = true;                 // Build the zip archive after conversion
        private ConversionDirection _conversionDirection = ConversionDirection.OdtToDocx; // direction of conversion
        private bool _autoDetectDirection = true;       // if true the conversion direction is detected based on the filename
        private DocumentType _documentType = DocumentType.Document;
        private ConversionMode _conversionMode = ConversionMode.SingleDocument;

        private bool _showUserInterface = false;
        private bool _showProgress = false;

        private string _generator;

        /// <summary>
        /// Determines whether a progress dialog and other dialogs will be shown. Default is <code>false</code>.
        /// </summary>
        public bool ShowUserInterface
        {
            get { return _showUserInterface; }
            set { _showUserInterface = value; }
        }

        /// <summary>
        /// Determines whether a client should display progress information.
        /// </summary>
        public bool ShowProgress
        {
            get { return _showProgress; }
            set { _showProgress = value; }
        }

        /// <summary>
        /// This property contains information about the converter version and environment
        /// It will be written to the document's meta data
        /// </summary>
        public string Generator
        {
            get { return _generator; }
            set { _generator = value; }
        }

        /// <summary>
        /// The file to be converted. 
        /// 
        /// This must be a valid filename of an existing file. 
        /// 
        /// If ConversionMode is set to <code>ConversionMode.Batch</code> this 
        /// property must be set to a valid directory instead.
        /// </summary>
        public string InputFullName
        {
            get { return _inputFullName; }
            set { _inputFullName = value; }
        }

        /// <summary>
        /// The base folder of the document(s) to be converted.
        /// 
        /// In single document mode this is the folder where the input document is in.
        /// In batch mode this is the input folder. (Note: On recursive conversion of whole directory trees this 
        /// property specifies the base folder only).  
        /// </summary>
        public string InputBaseFolder
        {
            get { return _inputBaseFolder != null ? _inputBaseFolder : Path.GetDirectoryName(this.InputFullNameOriginal); }
            set { _inputBaseFolder = value; }
        }

        /// <summary>
        /// The name of the output file to be created.
        /// 
        /// If this property is not initially set, clients should generate a meaningful output file name
        /// themselves and set this property to the generated name.
        /// </summary>
        public string OutputFullName
        {
            get { return _outputFullName; }
            set { _outputFullName = value; }
        }

        /// <summary>
        /// The base folder of the document(s) to be created.
        /// 
        /// In single document mode this is the folder where the output document will be created in.
        /// In batch mode this is the output folder (Note: On recursive conversion of whole directory trees this 
        /// property specifies the base folder only). 
        /// </summary>
        public string OutputBaseFolder
        {
            get { return _outputBaseFolder; }
            set { _outputBaseFolder = value; }
        }

        /// <summary>
        /// The full path to the original file to be converted. 
        /// 
        /// This must be a valid filename of an existing file. 
        /// 
        /// If ConversionMode is set to <code>ConversionMode.Batch</code> this 
        /// property must be set to a valid directory instead.
        /// </summary>
        public string InputFullNameOriginal
        {
            get { return _inputFullNameOriginal != null ? _inputFullNameOriginal : _inputFullName; }
            set { _inputFullNameOriginal = value; }
        }

        /// <summary>
        /// This property determines whetehr output files should be validated against the underlying schema.
        /// </summary>
        public bool Validate
        {
            get { return _validate; }
            set { _validate = value; }
        }

        /// <summary>
        /// If this property is set to true for batch conversions all documents in <code>InputBaseFolder</code> and all subfolders
        /// will be converted recursively.
        /// </summary>
        public bool RecursiveMode
        {
            get { return _recursiveMode; }
            set { _recursiveMode = value; }
        }

        /// <summary>
        /// If this property is set to true, existing output files will be overwritten by the converter, otherwise
        /// a unique output name will be generated.
        /// </summary>
        public bool ForceOverwrite
        {
            get { return _forceOverwrite; }
            set { _forceOverwrite = value; }
        }

        /// <summary>
        /// The path to a log file.
        /// </summary>
        public string ReportPath
        {
            get { return _reportPath; }
            set { _reportPath = value; }
        }

        /// <summary>
        /// A report to write log messages to.
        /// </summary>
        public ConversionReport Report
        {
            get { return _report; }
            set { _report = value; }
        }

        /// <summary>
        /// Defines which messages will be written to the <code>Report</code>
        /// </summary>
        public LogLevel LogLevel
        {
            get { return _logLevel; }
            set { _logLevel = value; }
        }

        public string XslPath
        {
            get { return _xslPath; }
            set { _xslPath = value; }
        }

        public List<string> SkippedPostProcessors
        {
            get { return _skippedPostProcessors; }
            set { _skippedPostProcessors = value; }
        }

        public bool Packaging
        {
            get { return _packaging; }
            set { _packaging = value; }
        }

        /// <summary>
        /// The direction of the converion (i.e. from which input format to which output format)
        /// </summary>
        public ConversionDirection ConversionDirection
        {
            get { return _conversionDirection; }
            set { _conversionDirection = value; }
        }

        public bool IsDirectTransform
        {
            get
            {
                return (_conversionDirection == ConversionDirection.OdtToDocx
                     || _conversionDirection == ConversionDirection.OdsToXlsx
                     || _conversionDirection == ConversionDirection.OdpToPptx);
            }
        }

        /// <summary>
        /// If set to true conversion direction will be detected automatically
        /// based on the extension of the input file.
        /// </summary>
        public bool AutoDetectDirection
        {
            get { return _autoDetectDirection; }
            set { _autoDetectDirection = value; }
        }

        /// <summary>
        /// Determines whether the document to be converted is a regular
        /// document or a document template.
        /// </summary>
        public DocumentType DocumentType
        {
            get { return _documentType; }
            set { _documentType = value; }
        }

        /// <summary>
        /// Tells us whether we are in batch conversion mode or single file conversion mode.
        /// </summary>
        public ConversionMode ConversionMode
        {
            get { return _conversionMode; }
            set { _conversionMode = value; }
        }
    }
}
