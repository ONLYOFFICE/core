using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;

using AVSOfficePPTToPPTX;
using TestPPTtoPPTX.Properties;

namespace TestPPTtoPPTX
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            m_oSrcFilePPT.Text = Settings.Default.SourceFile;

            DoTransform(null, null);
        }

        private void OpenSourceFile(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Title = "Open PPT File";
            fileDialog.FileName = "";
            fileDialog.Filter = "Microsoft PowerPoint File (*.ppt)|*.ppt";

            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    m_oSrcFilePPT.Text = fileDialog.FileName;
                    Settings.Default.SourceFile = fileDialog.FileName;
                    Settings.Default.Save();
                }
                catch
                {

                }
            }
        }

        private void DoTransform(object sender, EventArgs e)
        {
            if ((0 != m_oDoTransform.Text.Length) && (0 != m_oSrcFilePPT.Text.Length))
            {
                IOfficePPTToPPTX oTransformer = new COfficePPTToPPTXClass();
                if (null != oTransformer)
                {
                    string filePath = Path.GetDirectoryName(m_oSrcFilePPT.Text);
                    string fileName = Path.GetFileNameWithoutExtension(m_oSrcFilePPT.Text);

                    oTransformer.TransformFile(m_oSrcFilePPT.Text, filePath + "\\" + "fileName_0001" + ".pptx", "");
                    Marshal.FinalReleaseComObject(oTransformer);
                }
            }
        }
    }
}
