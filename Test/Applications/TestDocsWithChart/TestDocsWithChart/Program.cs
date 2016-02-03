using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Compression;
using System.IO;

using System.Diagnostics;

namespace TestDocsWithChart
{
    class Program
    {
        static void Main(string[] args)
        {
            //getFiles();
            convertFiles();
        }
        static void getFiles()
        {
            string sDirInput = @"\\192.168.3.208\allusers\Files\PPTX";
            string sDirOutput = @"F:\Files\embedchart";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.EndsWith(".xlsx", StringComparison.OrdinalIgnoreCase))
                        {
                            System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                        }
                    }
                }
                catch { }

            }
        }
        static void convertFiles()
        {
            string sPattern = "*.docx";
            int outputformat = 0x41;

            string sDirInput = @"F:\Files\embedchart";
            string sDirOutput = @"F:\Files\embedchartRes";
            string sDirTemp = @"F:\Files\Temp";
            if (Directory.Exists(sDirTemp))
                Directory.Delete(sDirTemp, true);
            string paramsXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><m_sKey>192_168_3_169_0067df00e034b38d42be2cd7b4927430_dae13aaba3b34344836364699c0e2e71_4592</m_sKey><m_sFileFrom>{0}</m_sFileFrom><m_sFileTo>{1}</m_sFileTo><m_nFormatTo>{2}</m_nFormatTo><m_nCsvTxtEncoding xsi:nil=\"true\" /><m_nCsvDelimiter xsi:nil=\"true\" /><m_bPaid xsi:nil=\"true\" /><m_bEmbeddedFonts xsi:nil=\"true\" /><m_bFromChanges>false</m_bFromChanges><m_sFontDir>F:\\Subversion\\trunk\\OfficeWeb\\Fonts\\native</m_sFontDir><m_sThemeDir>F:\\Subversion\\trunk\\OfficeWeb\\PowerPoint\\themes</m_sThemeDir><m_oTimestamp>2016-01-29T17:08:34.472Z</m_oTimestamp></TaskQueueDataConvert>";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, sPattern, System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                Directory.CreateDirectory(sDirTemp);
                string file = allfiles[i];
                try
                {
                    string paramsPath = Path.Combine(sDirTemp, "params.xml");
                    string sTempFile = Path.Combine(sDirTemp, "Editor.bin");
                    File.WriteAllText(paramsPath, string.Format(paramsXml, file, sTempFile, 0x2000));
                    Process process = new Process();
                    process.StartInfo.FileName = @"F:\Subversion\trunk\nodeJSProjects\FileConverter\Bin\x2t32.exe";
                    process.StartInfo.Arguments = paramsPath;
                    process.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                    process.Start();
                    process.WaitForExit();
                    File.WriteAllText(paramsPath, string.Format(paramsXml, sTempFile, Path.Combine(sDirOutput, Path.GetFileName(file)), outputformat));
                    process = new Process();
                    process.StartInfo.FileName = @"F:\Subversion\trunk\nodeJSProjects\FileConverter\Bin\x2t32.exe";
                    process.StartInfo.Arguments = paramsPath;
                    process.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                    process.Start();
                    process.WaitForExit();
                }
                catch
                {
                }
                Directory.Delete(sDirTemp, true);
            }
        }
    }
}
