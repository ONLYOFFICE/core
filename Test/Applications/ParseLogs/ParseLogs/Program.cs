using System;
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
