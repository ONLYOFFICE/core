/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
            //getFilesConditional();
            //getFilesPivot();
            //getFilesAlternateContent();
            //getFilesOverride();
            //getFiles();
            //getFiles2();
            //getFiles3();
            //getSize();
            //convertFiles();
            //getLegacyDrawingHF();
            //getShapeType();
            //getCustomXml();
            //getKeywords();
            //getMath();
            //getFullCalcOnLoad();
            //getGradientPos();
            //getCAFiles();
            //getSlicerFiles();
            getNamedSheetView();
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
        static void getCustomXml()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\docx";
            string sDirOutput = @"D:\Files\CustomXml";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (-1 != entry.FullName.IndexOf("CustomXml", StringComparison.OrdinalIgnoreCase))
                        {
                            System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                            break;
                        }
                    }
                }
                catch { }

            }
        }
        static void getKeywords()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\docx";
            string sDirOutput = @"D:\Files\keywords";
            string sFindText = "lastPrinted";
            string sFindText2 = "lastPrinted";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            StringBuilder sb = new StringBuilder();
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.EndsWith("core.xml", StringComparison.OrdinalIgnoreCase))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                int index1 = sXml.IndexOf(sFindText);
                                int index2 = sXml.IndexOf(sFindText2);
                                if (-1 != index1 && -1 != index2)
                                {
                                    String sKeywords = sXml.Substring(index1 + sFindText.Length, index2 - index1 - sFindText.Length);
                                    if (sKeywords.Length > 0)
                                    {
                                        sb.AppendLine(sKeywords);
                                        System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                                    }
                                    
                                }
                            }
                            break;
                        }
                    }
                }
                catch { }
            }
        }
        static void getMath()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\docx";
            string sDirOutput = @"D:\Files\Math";
            string sFindText = "m:phant";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.EndsWith("document.xml", StringComparison.OrdinalIgnoreCase))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                if (-1 != sXml.IndexOf(sFindText))
                                {
                                    //System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                                }
                            }
                            break;
                        }
                    }
                }
                catch { }
            }
        }
        static void getMove()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\docx";
            string sDirOutput = @"D:\Files\Move";
            string sFindText = "w:moveFrom";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.EndsWith("document.xml", StringComparison.OrdinalIgnoreCase))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                if (-1 != sXml.IndexOf(sFindText))
                                {
                                    System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                                }
                            }
                            break;
                        }
                    }
                }
                catch { }
            }
        }
        static void getExternal()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\xlsx";
            string sDirOutput = @"D:\Files\ExternalReferences";
            string sFindText = "externalBook";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.Contains("externalLink") && entry.FullName.EndsWith(".xml"))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                if (-1 == sXml.IndexOf(sFindText))
                                {
                                    System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                                }
                            }
                            break;
                        }
                    }
                }
                catch { }
            }
        }
        static void getFullCalcOnLoad()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\xlsx";
            string sDirOutput = @"D:\Files\fullCalcOnLoad";
            string sFindText = "fullCalcOnLoad";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.EndsWith("workbook.xml", StringComparison.OrdinalIgnoreCase))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                if (-1 != sXml.IndexOf(sFindText))
                                {
                                    System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                                }
                            }
                            break;
                        }
                    }
                }
                catch { }

            }
        }
        static void getGradientPos()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\xlsx";
            string sDirOutput = @"D:\Files\GradientPos";
            string sFindText = "stop position=\"";
            string sFindNoText1 = "0\"";
            string sFindNoText2 = "1\"";
            string sFindNoText3 = "0.5\"";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.EndsWith("styles.xml", StringComparison.OrdinalIgnoreCase))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                int indexStart = -1;
                                while(-1 != (indexStart = sXml.IndexOf(sFindText, indexStart + 1)))
                                {
                                    string substring1 = sXml.Substring(indexStart + sFindText.Length, sFindNoText1.Length);
                                    string substring2 = sXml.Substring(indexStart + sFindText.Length, sFindNoText2.Length);
                                    string substring3 = sXml.Substring(indexStart + sFindText.Length, sFindNoText3.Length);
                                    if (sFindNoText1 != substring1 && sFindNoText2 != substring2)
                                    {
                                        if (sFindNoText3 != substring3)
                                        {
                                            System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                                            break;
                                        }
                                    }
                                }
                                
                            }
                            break;
                        }
                    }
                }
                catch { }

            }
        }
        static void getCAFiles()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\xlsx";
            string sDirOutput = @"D:\Files\ca";
            string sFindText = " ca=\"1\"";
            int nThreshold = 100;
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            List<Tuple<int, string>> files = new List<Tuple<int,string>>();
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    int nCounter = 0;
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.StartsWith("xl/worksheets/sheet", StringComparison.OrdinalIgnoreCase) && entry.FullName.EndsWith(".xml", StringComparison.OrdinalIgnoreCase))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8)){
                                string sXml = reader.ReadToEnd();
                                int indexStart = -1;
                                while (-1 != (indexStart = sXml.IndexOf(sFindText, indexStart + 1)))
                                {
                                    nCounter++;
                                }
                            }
                        }
                    }
                    if (nCounter > nThreshold)
                    {
                        files.Add(new Tuple<int, string>(nCounter, file));
                        //if (files.Count >= 3)
                        //{
                        //    break;
                        //}
                    }
                }
                catch (Exception e){ }
            }
            files.Sort();
            for (int i = files.Count - 1; i >= 0; --i)
            {
                Tuple<int, string> tuple = files[i];
                System.IO.File.Copy(tuple.Item2, Path.Combine(sDirOutput, tuple.Item1.ToString("D6") + "_" + Path.GetFileName(tuple.Item2)), true);
            }
        }
        static void getSlicerFiles()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\xlsx";
            string sDirOutput1 = @"D:\Files\Slicers\table";
            string sFindText1 = "tableSlicerCache";
            string sDirOutput2 = @"D:\Files\Slicers\olap";
            string sFindText2 = "olap";
            string sDirOutput3 = @"D:\Files\Slicers\pivot";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.Contains("slicerCache") && entry.FullName.EndsWith(".xml"))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                if (-1 != sXml.IndexOf(sFindText1))
                                {
                                    System.IO.File.Copy(file, Path.Combine(sDirOutput1, Path.GetFileName(file)), true);
                                }
                                else if (-1 != sXml.IndexOf(sFindText2))
                                {
                                    System.IO.File.Copy(file, Path.Combine(sDirOutput2, Path.GetFileName(file)), true);
                                }
                                else
                                {
                                    System.IO.File.Copy(file, Path.Combine(sDirOutput3, Path.GetFileName(file)), true);
                                }
                            }
                        }
                    }
                }
                catch { }
            }
        }
        static void getNamedSheetView()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\xlsx";
            string sDirOutput = @"D:\Files\NamedSheetView";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.StartsWith("namedSheetView"))
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
        static void getFilesOverride()
        {
            string sAlternateContent = "lvlOverride";
            string sDirInput = @"\\192.168.5.3\files\Files\docx";
            string sDirOutput = @"D:\Files\override";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            SortedList<long, string> listUncompressed = new SortedList<long, string>();
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    string sOutputPath = Path.Combine(sDirOutput, Path.GetFileName(file));
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.EndsWith("numbering.xml", StringComparison.OrdinalIgnoreCase))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                int nIndex = -1;
                                if (-1 != (nIndex = sXml.IndexOf(sAlternateContent, nIndex + 1)))
                                {
                                    listUncompressed.Add(entry.Length, file);
                                    //System.IO.File.Copy(file, sOutputPath, true);
                                }
                            }
                            break;
                        }
                    }
                }
                catch (Exception e)
                {

                }
            }
            StringBuilder sb = new StringBuilder();
            sb.Append("size(bytes)\tName\r\n");
            foreach (KeyValuePair<long, string> kvp in listUncompressed.Reverse())
            {
                sb.AppendFormat("{0}\t{1}\r\n", kvp.Key, kvp.Value);
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
        static void getFiles2()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\docx";
            string sDirOutput = @"D:\Files\embed";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (!entry.FullName.EndsWith("/", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".xml", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".rels", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".docx", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".xlsx", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".pptx", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".doc", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".xls", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".ppt", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".bmp", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".jpg", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".jpeg", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".png", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".gif", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".tif", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".tiff", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".wdp", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".emf", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".emz", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".wmf", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".wav", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".wmv", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".mp3", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".mp4", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".mov", StringComparison.OrdinalIgnoreCase) &&
                            (!entry.FullName.EndsWith(".bin", StringComparison.OrdinalIgnoreCase) || (!entry.FullName.Contains("embeding") && !entry.FullName.Contains("oleObject") && !entry.FullName.Contains("printerSettings"))) &&
                            !entry.FullName.EndsWith(".sigs", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".sldx", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".dat", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".vml", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".fntdata", StringComparison.OrdinalIgnoreCase) &&
                            !entry.FullName.EndsWith(".odttf", StringComparison.OrdinalIgnoreCase))
                        {
                            System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                            break;
                        }
                    }
                }
                catch { }

            }
        }
        static void getFiles3()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\xlsx";
            string sDirOutput = @"D:\Files\height";
            string sHeight = " ht=\"";
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    bool bCopy = false;
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.Contains("sheet"))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                int nIndex = -1;
                                while (-1 != (nIndex = sXml.IndexOf(sHeight, nIndex + 1)))
                                {
                                    int endIndex = sXml.IndexOf("\"", nIndex + sHeight.Length + 1);
                                    string height = sXml.Substring(nIndex + sHeight.Length, endIndex - (nIndex + sHeight.Length));
                                    int dotIndex;
                                    if (-1 != (dotIndex = height.IndexOf('.')) && height.Length - dotIndex > 3)
                                    {
                                        bCopy = true;
                                        //System.IO.File.Copy(file, Path.Combine(sDirOutput, Path.GetFileName(file)), true);
                                        break;
                                    }
                                }
                            }
                        }
                        if (bCopy)
                        {
                            break;
                        }
                    }
                }
                catch { }

            }
        }
        static void getSize()
        {
            string sDirInput = @"\\192.168.5.3\files\Files\pptx";
            SortedList<long, string> listUncompressed = new SortedList<long,string>();
            String[] allfiles = System.IO.Directory.GetFiles(sDirInput, "*.*", System.IO.SearchOption.AllDirectories);
            for (var i = 0; i < allfiles.Length; ++i)
            {
                string file = allfiles[i];
                try
                {
                    long uncommressed = 0;
                    ZipArchive zip = ZipFile.OpenRead(file);
                    foreach (ZipArchiveEntry entry in zip.Entries)
                    {
                        if (entry.FullName.EndsWith(".xml"))
                        {
                            uncommressed += entry.Length;
                        }
                    }
                    listUncompressed.Add(uncommressed, file);
                }
                catch { }
            }
            StringBuilder sb = new StringBuilder();
            sb.Append("uncommressed(bytes)\tfilesize(bytes)\tName\r\n");
            foreach (KeyValuePair<long, string> kvp in listUncompressed.Reverse())
            {
                sb.AppendFormat("{0}\t{1}\t{2}\r\n", kvp.Key, new System.IO.FileInfo(kvp.Value).Length, kvp.Value);
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
        static void getLegacyDrawingHF()
        {
            string sAlternateContent = "<legacyDrawingHF ";
            string sDirInput = @"\\192.168.5.3\files\Files\xlsx";
            string sDirOutput = @"D:\Files\legacyDrawingHF";
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
                        if (entry.FullName.StartsWith("xl/worksheets/sheet", StringComparison.OrdinalIgnoreCase) && entry.FullName.EndsWith(".xml", StringComparison.OrdinalIgnoreCase))
                        {
                            using (StreamReader reader = new StreamReader(entry.Open(), Encoding.UTF8))
                            {
                                string sXml = reader.ReadToEnd();
                                if(-1 != sXml.IndexOf(sAlternateContent))
                                {
                                    System.IO.File.Copy(file, sOutputPath, true);
                                    break;
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
        static void getShapeType()
        {
            string sAlternateContent = "v:shapetype ";
            string sAlternateContent2 = "<w:pict";
            string sAlternateContent3 = "<w:object";
            string sDirInput = @"\\192.168.5.3\files\Files\docx";
            string sDirOutput = @"D:\Files\shapetype\docx";
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
                                if (-1 != sXml.IndexOf(sAlternateContent) && -1 == sXml.IndexOf(sAlternateContent2) && -1 == sXml.IndexOf(sAlternateContent3))
                                {
                                    System.IO.File.Copy(file, sOutputPath, true);
                                    break;
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
    }
}
