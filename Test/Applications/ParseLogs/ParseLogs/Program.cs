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

namespace ParseLogs
{
    class Program
    {
        static bool processFile(string filename, bool start, Dictionary<string, int> unique, StreamWriter writetext)
        {
            string key = "";
            string logMessage = "";
            foreach (var line in File.ReadLines(filename))
            {
                var _line = line.Replace("\\n", "\n");
                if (!start)
                {
                    if (_line.Contains("changesError:"))
                    {
                        key = "";
                        logMessage = "";
                        int indexStart = _line.IndexOf("Error:");
                        if (-1 != indexStart)
                        {
                            key = _line.Substring(indexStart);
                            int indexEnd = key.IndexOf("userAgent:");
                            if (-1 != indexEnd)
                            {
                                key = key.Substring(0, indexEnd);
                            }
                        }
                        if("" == key || !unique.ContainsKey(key))
                        {
                            if("" != key)
                            {
                                unique[key] = 1;
                            }
                            start = true;
                            logMessage += _line;
                            logMessage += '\n';
                        }
                    }
                }
                else
                {
                    if ("" == key)
                    {
                        key = _line;
                        if (!unique.ContainsKey(key))
                        {
                            unique[key] = 1;
                        }
                        else
                        {
                            start = false;
                            continue;
                        }
                    }
                    if (!_line.StartsWith("202") && !line.StartsWith("[202") && !_line.StartsWith("\"{\"\"startTime") && !_line.StartsWith("{\"\"startTime"))
                    {
                        logMessage += _line;
                        logMessage += '\n';
                    }
                    else
                    {
                        writetext.WriteLine(logMessage);
                        start = false;
                    }
                   
                }
            
            }
            return start;
        }
        static void Main(string[] args)
        {
            //string inputDir = @"D:\logs\doc.onlyoffice.eu\7.3.2\log2";
            string inputDir = @"D:\logs\doc.onlyoffice.eu\7.4.0\log";
            string outputFile = @"D:\logs\doc.onlyoffice.eu\7.4.0\changesError.txt";
            using (StreamWriter writetext = new StreamWriter(outputFile))
            {
                Dictionary<string, int> unique = new Dictionary<string, int>();
                bool start = false;
                string[] fileNames = Directory.GetFiles(inputDir);
                Array.Sort(fileNames, StringComparer.InvariantCulture);
                foreach (string file in fileNames)
                {
                        start = processFile(file, start, unique, writetext);
                }
            }
        }
    }
}
