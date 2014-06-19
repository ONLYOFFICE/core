using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.IO.Compression;

using ASCDocFile;

namespace DocFileTest
{
    public partial class MainForm : Form
    {
        public const bool DOC_TO_DOCX = true;
        public const string DOC_FILE_NAME = "test";

        public MainForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string filePath = this.textBox1.Text;
            if (0 == filePath.Length)
            {
                filePath = Path.GetDirectoryName(Application.ExecutablePath) + "\\" + DOC_FILE_NAME;
            }

            string docFile = Path.GetDirectoryName(filePath) + "\\" + DOC_FILE_NAME + ".doc";
            string docxFilePath = Path.GetDirectoryName(filePath) + "\\" + DOC_FILE_NAME;
            string docxFile = Path.GetDirectoryName(filePath) + "\\" + DOC_FILE_NAME + ".docx";

            if (File.Exists(docFile))
            {
                if (Directory.Exists(docxFilePath))
                {
                    Directory.Delete(docxFilePath, true);
                }

                Directory.CreateDirectory(docxFilePath);

                COfficeDocFile oFile = new COfficeDocFile();
                if (null != oFile)
                {
                    if (DOC_TO_DOCX)
                    {
                        if (File.Exists(docxFile))
                        {
                            File.Delete(docxFile);
                        }

                        oFile.LoadFromFile(docFile, docxFilePath, "");

                        using (var fs = new FileStream(docxFile, FileMode.Create))
                        {
                            using (var zip = new ZipArchive(fs, ZipArchiveMode.Create))
                            {
                                string[] fileEntries = Directory.GetFiles(docxFilePath, "*.*", SearchOption.AllDirectories);
                                foreach (string fileName in fileEntries)
                                {
                                    string fileRelative = fileName.Replace(docxFilePath + "\\", "");
                                    zip.CreateEntryFromFile(fileName, fileRelative);
                                }
                            }
                        }

                        Directory.Delete(docxFilePath, true);
                    }
                    else
                    {
                        // oFile.SaveToFile(docFile, docxFile, ""); // docx to doc
                    }
                }
            }

            Close();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            button1_Click(null, null);
        }
    }
}