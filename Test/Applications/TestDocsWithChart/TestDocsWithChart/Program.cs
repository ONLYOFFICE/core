/*
 * (c) Copyright Ascensio System SIA 2010-2017
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
﻿using System;
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
            getFilesConditional();
            //getFilesPivot();
            //getFilesAlternateContent();
            //getFiles();
            //convertFiles();
        }
        static void getFilesPivot()
        {
            string sDirInput = @"\\192.168.3.208\allusers\Files\XLSX";
            string sDirOutput = @"D:\Files\Pivot";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (-1 != entry.FullName.IndexOf("pivotTable", StringComparison.OrdinalIgnoreCase))
                        {
                            System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                            break;
                        }
                    }
                }
                catch { }

            }
        }
        static void getFilesConditional()
        {
            //string sFindText = "conditionalFormatting";
            //string sDirInput = @"\\192.168.3.208\allusers\Files\XLSX";
            //string sDirOutput = @"D:\Files\Conditional";
            string sFindText = "type=\"expression\"";
            string sDirInput = @"D:\Files\Conditional";
            string sDirOutput = @"D:\Files\ConditionalFormulaExpression";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);

            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    string sOutputPath = Path.Combine(sDirOutput, Path.GetFileName(file));
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.EndsWith(".xml", StringComparison.OrdinalIgnoreCase))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                if (-1 != sXml.IndexOf(sFindText))
                                {
                                    System.IO.File.Copy(file, sOutputPath, true);
                                }
                            }
                        }
                    }
                }
                catch (Exception e)
                {

                }
            }

        }
        static void getFilesAlternateContent()
        {
            string sAlternateContent = ":Choice ";
            string sDirInput = @"\\192.168.3.208\allusers\Files\PPTX";
            string sDirOutput = @"F:\Files\AlternateContent\pptx";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    string sOutputPath = Path.Combine(sDirOutput, Path.GetFileName(file));
                    bool bCopy = false;
                    string sRes = "";
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.EndsWith(".xml", StringComparison.OrdinalIgnoreCase))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                int nIndex = -1;
                                while (-1 != (nIndex = sXml.IndexOf(sAlternateContent, nIndex + 1)))
                                {
                                    if (!bCopy)
                                    {
                                        bCopy = true;
                                        sRes += sOutputPath;
                                        System.IO.File.Copy(file, sOutputPath, true);
                                    }
                                    sRes += ";";
                                    sRes += entry.ToString();
                                    sRes += "-";
                                    sRes += sXml.Substring(nIndex, sXml.IndexOf(">", nIndex + 1) - nIndex);
                                }
                            }
                        }
                    }
                    Debug.WriteLineIf(bCopy, sRes);
                }
                catch(Exception e) {

                }
            }

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
