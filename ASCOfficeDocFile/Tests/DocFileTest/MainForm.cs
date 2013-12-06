using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

using ASCDocFile;

namespace DocFileTest
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string filePath = this.textBox1.Text;
            if (0 == filePath.Length)
                filePath = Path.GetDirectoryName(Application.ExecutablePath) + "\\TestFile";

            string docFile = Path.GetDirectoryName(filePath) + "\\TestFile.doc";
            string docxFile = Path.GetDirectoryName(filePath) + "\\TestFile";

            COfficeDocFile oFile = new COfficeDocFile();
            oFile.SaveToFile(docFile, docxFile, "");

            Close();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            button1_Click(null, null);
        }
    }
}