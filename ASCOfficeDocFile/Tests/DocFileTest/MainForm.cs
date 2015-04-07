using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.IO.Compression;
using System.Runtime.InteropServices;

using ASCDocFile;

namespace DocFileTest
{
    public partial class MainForm : Form
    {
        public const int ERROR_FIRST = (1 << 31) | (4 << 16) | 0x0300;
        public const int ERROR_UNEXPECTED = ERROR_FIRST + 0x0000;
        public const int ERROR_BUSY = ERROR_FIRST + 0x0001;
        public const int ERROR_MEMORY = ERROR_FIRST + 0x0002;
        public const int ERROR_FILEACCESS = ERROR_FIRST + 0x0003;
        public const int ERROR_FILEFORMAT = ERROR_FIRST + 0x0004;
        public const int ERROR_PASSWORD = ERROR_FIRST + 0x0005;
        public const int ERROR_DRM = ERROR_FIRST + 0x0006;

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

                        try
                        {
                            oFile.LoadFromFile(docFile, docxFilePath, "");
                        }
                        catch (COMException ex) 
                        {
                            if (ERROR_BUSY == ex.ErrorCode)
                            {
                                MessageBox.Show("ERROR_BUSY - Source File: " + docFile, "DocTest (doc to docx)", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                            else if (ERROR_MEMORY == ex.ErrorCode)
                            {
                                MessageBox.Show("ERROR_BUSY - Source File: " + docFile, "DocTest (doc to docx)", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                            else if (ERROR_FILEACCESS == ex.ErrorCode)
                            {
                                MessageBox.Show("ERROR_FILEACCESS - Source File: " + docFile, "DocTest (doc to docx)", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                            else if (ERROR_FILEFORMAT == ex.ErrorCode)
                            {
                                MessageBox.Show("ERROR_FILEFORMAT - Source File: " + docFile, "DocTest (doc to docx)", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                            else if (ERROR_PASSWORD == ex.ErrorCode)
                            {
                                MessageBox.Show("ERROR_PASSWORD - Source File: " + docFile, "DocTest (doc to docx)", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }
                            else if (ERROR_DRM == ex.ErrorCode)
                            {
                                MessageBox.Show("ERROR_DRM - Source File: " + docFile, "DocTest (doc to docx)", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            }

                            Directory.Delete(docxFilePath, true);
                            Close();
                        }

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