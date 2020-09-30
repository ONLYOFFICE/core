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
                if (!start)
                {
                    if (line.Contains("[ERROR] nodeJS - changesError"))
                    {
                        key = "";
                        logMessage = "";
                        int indexStart = line.IndexOf(" Error: ");
                        if (-1 != indexStart)
                        {
                            key = line.Substring(indexStart);
                            int indexEnd = key.IndexOf(" userAgent: ");
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
                            logMessage += line;
                            logMessage += '\n';
                        }
                    }
                }
                else
                {
                    if ("" == key)
                    {
                        key = line;
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
                    if (!line.StartsWith("["))
                    {
                        logMessage += line;
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
            string inputDir = @"D:\logs\doc.onlyoffice.com\sync\logs\docservice";
            string outputFile = @"D:\logs\doc.onlyoffice.com\sync\changesError.txt";
            string startDate = "out.log-20200601";
            using (StreamWriter writetext = new StreamWriter(outputFile))
            {
                Dictionary<string, int> unique = new Dictionary<string, int>();
                bool start = false;
                string[] fileNames = Directory.GetFiles(inputDir);
                Array.Sort(fileNames, StringComparer.InvariantCulture);
                foreach (string file in fileNames)
                {
                    if (String.Compare(Path.GetFileName(file), startDate) >= 0)
                    {
                        start = processFile(file, start, unique, writetext);
                    }
                }
                start = processFile(Path.Combine(inputDir, "out.log"), start, unique, writetext);
            }
        }
    }
}
