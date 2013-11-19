using System;
using System.IO;

using OdfConverter.OdfConverterLib;
using CleverAge.OdfConverter.OdfConverterLib;

namespace AVSOfficeEWSEditor.Editor.ODSConverter
{
    public class ConverterFactory
    {
        private static AbstractConverter spreadsheetInstance;

        public static AbstractConverter Instance(ConversionDirection transformDirection)
        {
            if (spreadsheetInstance == null)
            {
                spreadsheetInstance = new CleverAge.OdfConverter.Spreadsheet.Converter();
            }
            return spreadsheetInstance;
        }
    }

    public class ODSConverter
    {
        #region Constants
        
        private const string GENERATOR = "";
        private const int NOT_CONVERTED = 0;
        private const int VALIDATED = 1;
        private const int NOT_VALIDATED = 2;

        #endregion

        #region Methods

        static public void ConvertFile(string InputFileName, string OutputFileName, ConversionDirection oDirection)
        {
            ConversionOptions options = new ConversionOptions();

            options.InputFullName = InputFileName;
            options.OutputFullName = OutputFileName;
            options.Generator = GENERATOR;
            options.InputFullName = Path.GetFullPath(options.InputFullName);
            options.ForceOverwrite = true;
            options.ConversionDirection = oDirection;

            options.InputBaseFolder = Path.GetDirectoryName(options.InputFullName);
            options.OutputBaseFolder = Path.GetDirectoryName(options.OutputFullName);

            proceed(options);
        }
        static private void proceed(ConversionOptions options)
        {

            if (!File.Exists(options.InputFullName))
            {
                return;
            }

            options.DocumentType = DocumentType.Document;

            if (File.Exists(options.OutputFullName) && !options.ForceOverwrite)
            {
                return;
            }

            proceedSingleFile(options);

            FileInfo oFileInfo = new FileInfo(options.OutputFullName);

            System.Security.AccessControl.FileSystemAccessRule oRule = new System.Security.AccessControl.FileSystemAccessRule("Everyone",
                System.Security.AccessControl.FileSystemRights.FullControl,
                System.Security.AccessControl.AccessControlType.Allow);

            System.Security.AccessControl.FileSecurity oFileSecurity = oFileInfo.GetAccessControl();
            oFileSecurity.AddAccessRule(oRule);
            oFileInfo.SetAccessControl(oFileSecurity);
        }

        static private int proceedSingleFile(ConversionOptions options)
        {
            bool converted = false;

            converted = convertFile(options.InputFullName, options.OutputFullName, options);

            if (!converted)
            {
                Environment.ExitCode = 1;
                return NOT_CONVERTED;
            }
            else 
                return VALIDATED;
        }
        static private bool convertFile(string input, string output, ConversionOptions options)
        {
            AbstractConverter converter = ConverterFactory.Instance(options.ConversionDirection);
            converter.DirectTransform = options.ConversionDirection == ConversionDirection.OdsToXlsx;

            converter.Transform(input, output, options);
            return true;
        }

        #endregion
    }
}
