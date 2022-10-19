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

namespace HeaderDepSearcher
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void buttonSearch_Click(object sender, EventArgs e)
        {
            int count = 0;

            StreamReader streamReader = null;
            StreamWriter streamWriter = null;

            string dirPath = textBoxDir.Text;

            if (Directory.Exists(dirPath))
            {
                List<HeaderInfo> fileMap = new List<HeaderInfo>();

                // Headers
                string[] hFiles = Directory.GetFiles(dirPath, "*.h", SearchOption.AllDirectories);
                string[] cppFiles = Directory.GetFiles(dirPath, "*.cpp", SearchOption.AllDirectories);
                string[] proFiles = Directory.GetFiles(dirPath, "*.pro", SearchOption.AllDirectories);

                // Advanced headers list
                foreach (string hFile in hFiles)
                {
                    string hName = Path.GetFileName(hFile);
                    HeaderInfo hInfo = new HeaderInfo(hName, hFile);

                    if (!fileMap.Contains(hInfo))
                    {
                        fileMap.Add(hInfo);
                    }
                }

                // h + cpp
                string[] allFiles = hFiles.Concat(cppFiles).ToArray();

                // Std Lib Headers & Relative header paths
                count = 0;
                progressBar.Visible = true;
                progressBar.Maximum = allFiles.Length;

                foreach (string filePath in allFiles)
                {
                    streamReader = new StreamReader(filePath, Encoding.Default);

                    while (!streamReader.EndOfStream)
                    {
                        string line = streamReader.ReadLine();
                        if (line.Contains("#include "))
                        {
                            // Std
                            Match match = Regex.Match(line, "#include (<)(.*)(>)");
                            if (match.Success)
                            {
                                string includeString = match.Groups[2].Value;

                                HeaderInfo hInfo = new HeaderInfo(Path.GetFileName(includeString), includeString, true);

                                if (!fileMap.Contains(hInfo))
                                {
                                    fileMap.Add(hInfo);
                                }
                            }

                            // Relative
                            match = Regex.Match(line, "#include (\")(.*)(\")");
                            if (match.Success)
                            {
                                try
                                {
                                    string includeString = match.Groups[2].Value;

                                    HeaderInfo hInfo = new HeaderInfo(Path.GetFileName(includeString), includeString);

                                    if (!fileMap.Contains(hInfo))
                                    {
                                        fileMap.Add(hInfo);
                                    }
                                }
                                catch (Exception ex)
                                {
                                    string msg = ex.Message;
                                }
                            }
                        }
                    }

                    streamReader.Close();

                    count++;
                    progressBar.Value = count;
                    Application.DoEvents();
                }

                // Search headers for all project files
                count = 0;
                progressBar.Visible = true;
                progressBar.Maximum = fileMap.Count;

                // Analyze each header                
                foreach (HeaderInfo hInfo in fileMap)
                {
                    string hName = hInfo.m_fileName;                    

                    // Search includes
                    foreach (string filePath in allFiles)
                    {
                        string fileName = Path.GetFileName(filePath);

                        if (fileName != hName)
                        {
                            // !!! TO-D0 : Read line by line
                            streamReader = new StreamReader(filePath, Encoding.Default);
                            string fileContent = streamReader.ReadToEnd();
                            streamReader.Close();

                            // Advanced search by path, not name only
                            string checkName = hInfo.m_fileName;

                            string[] spl = hInfo.m_filePath.Split(new char[] { '/', '\\' });
                            if (spl.Length > 1)
                            {
                                checkName = string.Format("{0}/{1}", spl[spl.Length - 2], spl[spl.Length - 1]);
                            }

                            if (fileContent.Contains(string.Format("/{0}\"", checkName)) ||
                                fileContent.Contains(string.Format("\"{0}\"", checkName)) ||
                                fileContent.Contains(string.Format("<{0}>", checkName)))
                            {
                                if (!hInfo.m_includeList.Contains(filePath))
                                {
                                    hInfo.m_includeList.Add(filePath);

                                    labelFileProcess.Text = string.Format("{0}: {1}", hName, hInfo.m_includeList.Count);
                                    Application.DoEvents();
                                }
                            }
                        }
                    }

                    count++;
                    progressBar.Value = count;
                    Application.DoEvents();
                }

                labelFileProcess.Text = "";

                // Create CSV report
                string reportFile = "_";
                foreach (string proFile in proFiles)
                {
                    reportFile += Path.GetFileNameWithoutExtension(proFile) + "_";
                }
                reportFile += ".csv";

                streamWriter = new StreamWriter(reportFile, false);
                streamWriter.WriteLine("File name;File path;Include count;File size(KB)");

                foreach (HeaderInfo hInfo in fileMap.OrderByDescending(x => x.m_includeList.Count))
                {
                    FileInfo fInfo = new FileInfo(hInfo.m_filePath);
                    long fileSize = fInfo.Exists ? fInfo.Length / 1024 : 0;

                    streamWriter.WriteLine(string.Format("{0};{1};{2};{3}", hInfo.m_fileName, hInfo.m_filePath, hInfo.m_includeList.Count, fileSize));
                }

                streamWriter.Close();

                // Create precompiled includes
                List<string> writeList = new List<string>();

                foreach (string proFile in proFiles)
                {
                    int headerIncludeLimit = Int32.Parse(textBoxIncludeLimit.Text);
                    int headerIncludeLimitStd = Int32.Parse(textBoxIncludeLimitStd.Text);

                    streamReader = new StreamReader(proFile, Encoding.Default);
                    streamWriter = new StreamWriter(string.Format("{0}_pch.txt", Path.GetFileNameWithoutExtension(proFile)), false);

                    // Standart Lib Headers firstly
                    foreach (HeaderInfo hInfo in fileMap.OrderByDescending(x => x.m_includeList.Count))
                    {
                        if (hInfo.m_includeList.Count < headerIncludeLimitStd)
                        {
                            continue;
                        }

                        if (hInfo.m_stdHeader)
                        {
                            streamWriter.WriteLine(string.Format("#include <{0}>", hInfo.m_filePath));
                        }
                    }

                    // Other project includes
                    while (!streamReader.EndOfStream)
                    {
                        string line = streamReader.ReadLine();

                        foreach (HeaderInfo hInfo in fileMap.OrderByDescending(x => x.m_includeList.Count))
                        {
                            if (hInfo.m_includeList.Count < headerIncludeLimit)
                            {
                                continue;
                            }                            

                            // Advanced search by path, not name only
                            string checkName = hInfo.m_fileName;

                            string[] spl = hInfo.m_filePath.Split(new char[] { '/', '\\' });
                            if (spl.Length > 1)
                            {
                                checkName = string.Format("{0}/{1}", spl[spl.Length - 2], spl[spl.Length - 1]);
                            }
                            else
                            {
                                // Fix (example: OMath.h & SimpleTypes_OMath.h)
                                checkName = string.Format("/{0}", checkName);
                            }

                            if (line.Contains(checkName))
                            {
                                string tmp = line.TrimStart(' ').TrimEnd(new Char[] { '\\', ' ' });
                                tmp = string.Format("#include \"{0}\"", tmp);

                                if (!writeList.Contains(tmp))
                                {
                                    streamWriter.WriteLine(tmp);
                                    writeList.Add(tmp);
                                }
                            }
                        }
                    }
                    streamReader.Close();
                    streamWriter.Close();

                    writeList.Clear();
                }

                progressBar.Visible = false;
                MessageBox.Show("Done");
            }
        }

        private void FormMain_Shown(object sender, EventArgs e)
        {
            buttonSearch.Focus();
        }

        private void buttonClassSplitter_Click(object sender, EventArgs e)
        {
            FormSplitter form = new FormSplitter();
            form.Show();
        }

        private void buttonDir_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog();
            DialogResult result = folderBrowserDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                textBoxDir.Text = folderBrowserDialog.SelectedPath;
            }
        }
    }

    public class HeaderInfo
    {
        public string m_filePath = "", m_fileName = "";
        public bool m_stdHeader = false;
        public List<string> m_includeList = new List<string>();

        public HeaderInfo(string fileName, string filePath, bool stdHeader = false)
        {
            m_fileName = fileName;
            m_filePath = filePath;
            m_stdHeader = stdHeader;
        }

        public override bool Equals(object obj)
        {
            HeaderInfo obj_ = obj as HeaderInfo;
            return (obj_ != null) && obj_.m_filePath == this.m_filePath;
        }

        public override int GetHashCode()
        {
            return this.m_filePath.GetHashCode();
        }
    }
}
