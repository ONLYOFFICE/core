using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;
using HeaderDepSearcher.Properties;

namespace HeaderDepSearcher
{
    public partial class FormSplitter : Form
    {
        string dirOutput = "";
        string patternStart = "//-----";
        string patternStartExt = "// -----";
        string patternNamespace = "namespace ";
        string patternClassStart = "class ";
        string patternClassEnd = "};";

        public FormSplitter()
        {
            InitializeComponent();
        }

        private void buttonSplit_Click(object sender, EventArgs e)
        {
            int count = 0;

            StreamReader streamReader = null;
            StreamWriter streamWriter = null;

            List<string> lineList = new List<string>();
            List<string> classLines = new List<string>();
            List<string> namespaceList = new List<string>();
            List<string> includeList = new List<string>();

            labelFileProcess.Text = "";
            dirOutput = Path.GetFileNameWithoutExtension(textFilePath.Text);

            if (Directory.Exists(dirOutput))
            {
                Directory.Delete(dirOutput, true);
            }
            Directory.CreateDirectory(dirOutput);

            // Read source file
            streamReader = new StreamReader(textFilePath.Text, Encoding.Default);

            while (!streamReader.EndOfStream)
            {
                string line = streamReader.ReadLine();
                lineList.Add(line);
            }
            streamReader.Close();

            // Parsing
            StreamWriter sw = new StreamWriter(string.Format("{0}\\proInclude.txt", dirOutput), false);

            for (int i = 0; i < lineList.Count; i++)
            {
                // Debug trap
                if (i == 0)
                {
                    int x = 0;
                }

                List<string> resultList = new List<string>();

                // Namespace parser
                if (lineList[i].Contains(patternNamespace))
                {
                    string namespaceName = "";

                    Match match = Regex.Match(lineList[i], @"namespace\s(\w*)");
                    if (match.Success)
                    {
                        namespaceName = match.Groups[1].Value;
                    }

                    if (lineList[i].IndexOf(patternNamespace) == 0)
                    {
                        namespaceList = new List<string>();
                    }

                    namespaceList.Add(namespaceName);
                }

                // Class parser
                if ((lineList[i].Contains(patternStart) && lineList[i + 1].Contains("//") && lineList[i + 2].Contains(patternStart)) ||
                    (lineList[i].Contains(patternStartExt) && lineList[i + 1].Contains("//") && lineList[i + 2].Contains(patternStartExt)))
                {
                    bool classExists = false;
                    string className = "";

                    for (int j = i; j < lineList.Count; j++)
                    {
                        resultList.Add(lineList[j]);

                        if (lineList[j].Contains(patternClassStart))
                        {
                            classExists = true;
                            includeList = new List<string>();

                            Match match = Regex.Match(lineList[j], @"class\s(\w*)(\s\:\s)?");
                            if (match.Success)
                            {
                                className = match.Groups[1].Value;

                                if (className != "ComplexType" && className != "String")
                                {
                                    className = className.Substring(1);
                                }

                                count++;
                                labelFileProcess.Text = string.Format("Class: {0}", count);
                            }
                        }

                        // Check end
                        string clearLine = lineList[j].TrimStart('\t', ' ').TrimEnd('\t', ' ');
                        if ((clearLine == patternClassEnd) && classExists)
                        {
                            break;
                        }
                    }

                    // Save
                    sw.WriteLine(string.Format("../Source/Common/{0}/{1}.h \\", dirOutput, className));

                    streamWriter = new StreamWriter(string.Format("{0}\\{1}.h", dirOutput, className), false);

                    streamWriter.Write(Resources.Copyright);
                    streamWriter.WriteLine("");
                    streamWriter.WriteLine("");
                    streamWriter.WriteLine("#pragma once");
                    streamWriter.WriteLine("");

                    // Includes
                    foreach (string line in resultList)
                    {
                        string includePattern = "CSimpleType<";
                        if (line.Contains(includePattern))
                        {
                            if (!includeList.Contains(includePattern))
                            {
                                streamWriter.WriteLine("#include \"../SimpleTypes_Base/SimpleType.h\"");
                                includeList.Add(includePattern);
                            }
                        }

                        includePattern = ": public ComplexType";
                        if (line.Contains(includePattern))
                        {
                            if (!includeList.Contains(includePattern))
                            {
                                streamWriter.WriteLine("#include \"ComplexType.h\"");
                                includeList.Add(includePattern);
                            }
                        }

                        includePattern = "WritingElement_";
                        if (line.Contains(includePattern))
                        {
                            if (!includeList.Contains(includePattern))
                            {
                                streamWriter.WriteLine("#include \"../../DocxFormat/WritingElement.h\"");
                                includeList.Add(includePattern);
                            }
                        }

                        includePattern = "nullable<";
                        if (line.Contains(includePattern))
                        {
                            if (!includeList.Contains(includePattern))
                            {
                                streamWriter.WriteLine("#include \"../../Base/Nullable.h\"");
                                includeList.Add(includePattern);
                            }
                        }
                    }

                    streamWriter.WriteLine("");

                    // Namespace starts
                    if (namespaceList.Count > 0)
                    {
                        streamWriter.WriteLine("namespace " + namespaceList[0]);
                        streamWriter.WriteLine("{");

                        if (namespaceList.Count > 1)
                        {
                            streamWriter.WriteLine("\tnamespace " + namespaceList[namespaceList.Count - 1]);
                            streamWriter.WriteLine("\t{");
                        }
                    }


                    foreach (string line in resultList)
                    {
                        streamWriter.WriteLine(line);
                    }

                    // Namespace ends
                    if (namespaceList.Count > 0)
                    {
                        if (namespaceList.Count > 1)
                        {
                            streamWriter.WriteLine("\t}");
                        }

                        streamWriter.WriteLine("}");
                    }

                    streamWriter.Close();
                }
            }
            sw.Close();

            MessageBox.Show("Done");
        }

        private void buttonDir_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            DialogResult result = openFileDialog.ShowDialog();
        
            if (result == DialogResult.OK)
            {
                textFilePath.Text = openFileDialog.FileName;
            }
        }
    }
}
