using System;
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
            string input = @"D:\logs\doc.onlyoffice.com\sync\files";
            string output = @"D:\logs\doc.onlyoffice.com\6.4.1\errorsorted";

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
                    string curDir = dirs2[j];
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

            TimeSpan time = DateTime.Now - start;

        }
    }
}
