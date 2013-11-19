using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace WindowsApplication1
{
    public partial class Form1 : Form
    {
        private string m_strFilePath = @"X:\AVS\Sources\AVSOfficeStudio\OfficeWebPowerPoint\document\editor.js";
        private AVSPresentationEditor.CAVSPresentationEditorClass oPPT = new AVSPresentationEditor.CAVSPresentationEditorClass();

        public Form1()
        {
            InitializeComponent();

            textBox1.Text = m_strFilePath;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            oPPT.OpenFile(m_strFilePath);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            oPPT.Save(0, "C:\\xml.xml");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK != openFileDialog1.ShowDialog())
                return;
            m_strFilePath = openFileDialog1.FileName;
            textBox1.Text = m_strFilePath;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            oPPT.OpenXmlFile(textBox2.Text);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            oPPT.TempDirectory = "C:\\PPTMemory\\Test_pptx";
            oPPT.Save(1, "C:\\PPTMemory\\TEST.pptx");
        }

        private void button6_Click(object sender, EventArgs e)
        {
            oPPT.TempDirectory = "C:\\PPTMemory\\Test_pptx";
            oPPT.Save(2, "C:\\PPTMemory\\TEST.pdf");
        }

        private void button7_Click(object sender, EventArgs e)
        {
            oPPT.TempDirectory = "C:\\PPTMemory\\Test_pptx";
            oPPT.Save(3, "C:\\PPTMemory\\TEST_html.html");
        }

        private void button8_Click(object sender, EventArgs e)
        {
            oPPT.TempDirectory = "C:\\PPTMemory\\Test_pptx";
            oPPT.SetFontDir("X:\\AVS\\Sources\\AVSOfficeStudio\\OfficeWebCommon\\TM_Fonts_Generator\\Generations\\Input");
            oPPT.SetMediaDir("C:\\PPTMemory\\media");
            oPPT.SetUseSystemFonts(false);

            if (checkBox1.Checked)
            {
                oPPT.SetAdditionalParam("EmbeddedFontsDirectory", "C:\\PPTMemory\\fonts");
            }

            oPPT.ConvertToPPTY(m_strFilePath, "C:\\PPTMemory\\Test.ppty");
        }

        private void button9_Click(object sender, EventArgs e)
        {
            AVSOfficePPTXFile.CAVSOfficePPTXFileClass oPPTX = new AVSOfficePPTXFile.CAVSOfficePPTXFileClass();
            string str = GetTemporaryDirectory();
            oPPTX.TempDirectory = str;
            oPPTX.ConvertPPTYToPPTX(textBox1.Text, "C:\\PPTMemory\\1.pptx");
        }

        private string GetTemporaryDirectory()
        {
            string tempDirectory = Path.Combine(Path.GetTempPath(), Path.GetRandomFileName());
            while (File.Exists(tempDirectory))
                tempDirectory = Path.Combine(Path.GetTempPath(), Path.GetRandomFileName());
            Directory.CreateDirectory(tempDirectory);
            return tempDirectory;
        }

        private void button10_Click(object sender, EventArgs e)
        {
            AVSOfficePPTXFile.CAVSOfficeDrawingConverterClass oConverter = new AVSOfficePPTXFile.CAVSOfficeDrawingConverterClass();

            StreamReader oReader = new StreamReader(textBox1.Text);
            string sXml = oReader.ReadToEnd();

            string sMainProps = "";
            Array oBinaryData = null;
            oConverter.AddObject(sXml, out sMainProps, out oBinaryData);
        }

        private void button11_Click(object sender, EventArgs e)
        {
            AVSOfficePPTXFile.CAVSOfficeDrawingConverterClass oConverter = new AVSOfficePPTXFile.CAVSOfficeDrawingConverterClass();

            int typeTest = 2;
            switch (typeTest)
            {
                case 0:
                    {                        
                        StreamReader oReader = new StreamReader("C:\\XML\\shapetype.xml");
                        string sXml = oReader.ReadToEnd();

                        oConverter.AddShapeType(sXml);
                        break;
                    }

                case 1:
                    {
                        StreamReader oReader = new StreamReader("C:\\XML\\shapetype.xml");
                        string sXml = oReader.ReadToEnd();

                        oConverter.AddShapeType(sXml);

                        StreamReader oReader1 = new StreamReader("C:\\XML\\shape.xml");
                        string sXml1 = oReader1.ReadToEnd();

                        string sMainProps = "";
                        Array oBinaryData = null;
                        oConverter.AddObject(sXml1, out sMainProps, out oBinaryData);

                        break;
                    }
                case 2:
                    {
                        StreamReader oReader1 = new StreamReader("C:\\XML\\test1.xml");
                        string sXml1 = oReader1.ReadToEnd();

                        string sMainProps = "";
                        Array oBinaryData = null;
                        oConverter.AddObject(sXml1, out sMainProps, out oBinaryData);
                        break;
                    }
                case 3:
                    {
                        StreamReader oReader1 = new StreamReader("C:\\XML\\SA\\word\\document.xml");
                        string sXml1 = oReader1.ReadToEnd();

                        oConverter.SetRelsPath("C:\\XML\\SA\\word\\document.xml");
                        string sMainProps = "";
                        Array oBinaryData = null;
                        oConverter.AddObject(sXml1, out sMainProps, out oBinaryData);
                        break;
                    }
                default:
                    break;
            }            
        }

        private void button12_Click(object sender, EventArgs e)
        {
            StreamReader oReader1 = new StreamReader(textBox1.Text);
            string sXml1 = oReader1.ReadToEnd();

            AVSOfficePPTXFile.CAVSOfficeDrawingConverterClass oConverter = new AVSOfficePPTXFile.CAVSOfficeDrawingConverterClass();
            string sMainProps = "";
            Array oBinaryData = null;
            oConverter.AddObject(sXml1, out sMainProps, out oBinaryData);
        }
    }
}