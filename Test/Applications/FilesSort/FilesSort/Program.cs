/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
using System.IO;

namespace FilesSort
{
    class Program
    {
        static void Copy(string sourceDir, string targetDir)
        {
            Directory.CreateDirectory(targetDir);

            foreach (var file in Directory.GetFiles(sourceDir))
                File.Copy(file, Path.Combine(targetDir, Path.GetFileName(file)), true);

            foreach (var directory in Directory.GetDirectories(sourceDir))
                Copy(directory, Path.Combine(targetDir, Path.GetFileName(directory)));
        }
        static void initCache(Dictionary<long, long> hash, string dir)
        {
            foreach (string dir1 in Directory.GetDirectories(dir))
            {
                foreach (string dir2 in Directory.GetDirectories(dir1))
                {
                    string source = Path.Combine(dir2, "source");
                    if (Directory.Exists(source))
                    {
                        string[] files = Directory.GetFiles(source);
                        if (files.Length > 0)
                        {
                            long size = new System.IO.FileInfo(files[0]).Length;
                            hash[size] = 1;
                        }
                    }
                }
            }
        }
        static void Main(string[] args)
        {
            string input = @"D:\logs\doc.onlyoffice.com\7.3.2\files";
            string output = @"D:\logs\doc.onlyoffice.com\7.3.2\files-sorted";

            DateTime start = DateTime.Now;
            Dictionary<long, long> hash = new Dictionary<long, long>();
            if(Directory.Exists(output))
            {
                initCache(hash, output);
            }
            
            
            Directory.CreateDirectory(output);
            string[] dirs = Directory.GetDirectories(input);
            for (int i = 0; i < dirs.Length; ++i)
            {
                string[] dirs2 = Directory.GetDirectories(dirs[i]);
                for (int j = 0; j < dirs2.Length; ++j)
                {
                    string[] dirs3 = Directory.GetDirectories(dirs2[j]);
                    for (int k = 0; k < dirs3.Length; ++k)
                    {
                        string curDir = dirs3[k];
                        if (!curDir.EndsWith("browser"))
                        {
                            string source = Path.Combine(curDir, "source");
                            bool bError = true;
                            if (Directory.Exists(source))
                            {
                                string[] files = Directory.GetFiles(source);
                                if (files.Length > 0)
                                {
                                    bError = false;
                                    string file = files[0];
                                    long size = new System.IO.FileInfo(file).Length;
                                    long outVal;
                                    if (!hash.TryGetValue(size, out outVal))
                                    {
                                        hash[size] = 1;

                                        int format = FormatChecker.GetFileFormat(file);
                                        string formatStr = FormatChecker.FileFormats.ToString(format);
                                        if (string.IsNullOrEmpty(formatStr))
                                            formatStr = "unknown";
                                        string formatDir = Path.Combine(output, formatStr);
                                        Directory.CreateDirectory(formatDir);
                                        Copy(curDir, Path.Combine(formatDir, Path.GetFileName(curDir)));
                                    }
                                }
                            }
                            if (bError)
                            {
                                string error = Path.Combine(output, "error");
                                Directory.CreateDirectory(error);
                                Copy(curDir, Path.Combine(error, Path.GetFileName(curDir)));
                            }
                        }
                    }
                }
            }

            TimeSpan time = DateTime.Now - start;

        }
    }
}
