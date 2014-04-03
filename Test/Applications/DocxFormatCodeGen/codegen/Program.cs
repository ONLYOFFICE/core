using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Schema;

namespace codegen
{
    class Program
    {
        static string sDirIn = @"..\..\Resource\";
        static string sDirCppXmlOut = @"D:\Subversion\AVS\Sources\TeamlabOffice\trunk\ServerComponents\Common\DocxFormat\Source\XlsxFormat\Chart\";
        static string sDirCppBinOut = @"D:\Subversion\AVS\Sources\TeamlabOffice\trunk\ServerComponents\XlsxSerializerCom\Reader\";
        static string sDirJsBinOut = @"D:\Subversion\AVS\Sources\TeamlabOffice\trunk\OfficeWeb\Common\";
        static void Main(string[] args)
        {
            (new codegen.CodeGen()).Start(sDirIn, sDirCppXmlOut, sDirCppBinOut, sDirJsBinOut, ValidationCallback);
        }
        static void ValidationCallback(object sender, ValidationEventArgs args)
        {
            if (args.Severity == XmlSeverityType.Warning)
                Console.Write("WARNING: ");
            else if (args.Severity == XmlSeverityType.Error)
                Console.Write("ERROR: ");

            Console.WriteLine(args.Message);
        }
    }
}
