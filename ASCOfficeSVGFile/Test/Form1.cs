using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using AVSMediaCore3;
using AVSOfficeSVGFile;

using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.Xml;
using System.Xml.Xsl;
using System.Diagnostics;
using System.Threading;

namespace Test
{
    public partial class Form1 : Form
    {
        private Bitmap m_oBitmap = null;
        private bool m_bYesToDrawImage = false;
        private float deScale = 1.0f;

        private uint c_nPageType = 0x0001;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
           LoadSvgFile();
        }

        private void openSvgFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog oOpenFileDialog = new OpenFileDialog();
            oOpenFileDialog.Title = "Open SVG File";
            oOpenFileDialog.FileName = "";
            oOpenFileDialog.Filter = "SVG (*.svg)|*.svg";

            if (oOpenFileDialog.ShowDialog() == DialogResult.OK)
            {
               // try
                {
                    toolStripTextBox1.Text = oOpenFileDialog.FileName;
                    LoadSvgFile();
                }
              //  catch
                {

                }
            }
        }

        private void LoadSvgFile()
        {
            XmlDocument oDocument = new XmlDocument();
            
           // try
           // {
            //    oDocument.Load(toolStripTextBox1.Text);
           // }
           // catch (Exception e)
           // {
              //  MessageBox.Show(e.Message, "Load XML",MessageBoxButtons.OK,MessageBoxIcon.Error );
                
              //  return;
           // }

            AVSGraphics.IAVSGraphicsRenderer oRender = new AVSGraphics.CAVSGraphicsRenderer();
            CAVSUncompressedVideoFrame oSource = new CAVSUncompressedVideoFrame();

            AVSOfficeSVGFile.ISVGTransformer oTranformer = new AVSOfficeSVGFile.SVGTransformerClass();
            // oTranformer.Load(oDocument.InnerXml);
            try
            {
                //oTranformer.LoadFile(toolStripTextBox1.Text);

                StreamReader oStream = new StreamReader(toolStripTextBox1.Text);
                oTranformer.Load(oStream.ReadToEnd());
                oStream.Close();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "Load XML", MessageBoxButtons.OK, MessageBoxIcon.Error);

                return;
            }

            if (0 == oTranformer.Width || 0 == oTranformer.Height)
            {
                Marshal.FinalReleaseComObject(oRender);
                Marshal.FinalReleaseComObject(oTranformer);
                
                return;
            }

            oSource.AspectRatioX = 0;
            oSource.AspectRatioY = 0;
            oSource.Width = (int)(oTranformer.Width);
            oSource.Height = (int)(oTranformer.Height);
            oSource.ColorSpace = (1 << 6);  // |(1 << 31) ; // NONE FLIP
            oSource.SetDefaultStrides();
            oSource.AllocateBuffer(oSource.Width * oSource.Height * 4);

            m_oBitmap = null;
            IAVSUncompressedVideoFrame image = (IAVSUncompressedVideoFrame)oSource;

            Bitmap oBitmap = new Bitmap(image.Width, image.Height, image.Width * 4, System.Drawing.Imaging.PixelFormat.Format32bppArgb, image.Buffer);
            using (Graphics graphics2 = Graphics.FromImage(oBitmap))
            {
               // graphics2.Clear(Color.FromArgb(255, 0, 0, 255));
                graphics2.Clear(Color.FromArgb(255, 255, 255, 255));
            }

            oRender.Width = oTranformer.Width;
            oRender.Height = oTranformer.Height;
            oRender.SetAdditionalParam("FontManager", null);
            oRender.CreateFromMediaData(oSource, 0.0, 0.0, (double)oSource.Width, (double)oSource.Height);

            oTranformer.Draw(oRender,0,0,800,600);

            Bitmap oBitmap2 = new Bitmap(image.Width, image.Height);
            using (Graphics gr = Graphics.FromImage(oBitmap2))
            {
               // gr.Clear(Color.FromArgb(255, 0, 0, 255));
                gr.Clear(Color.FromArgb(255, 255, 255, 255));
                gr.DrawImage(oBitmap, 0, 0, image.Width, image.Height);
            }

            oBitmap = null;
         
            Width = (int)(oTranformer.Width) + 30;
            Height = (int)(oTranformer.Height) + 80;

            Marshal.FinalReleaseComObject(oRender);
            Marshal.FinalReleaseComObject(oTranformer);
            Marshal.FinalReleaseComObject(image);

            oTranformer = null;
            oRender = null;

            m_oBitmap = oBitmap2;
            m_bYesToDrawImage = true;

            deScale = 1.0f;

            Update();
            Refresh();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            if (null != m_oBitmap)
            {               
                if (m_bYesToDrawImage)
                    e.Graphics.DrawImage(m_oBitmap, 10, 30, m_oBitmap.Width * deScale, m_oBitmap.Height * deScale);
            }
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            LoadSvgFile();
        }

        private void saveImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (null!=m_oBitmap)
                m_oBitmap.Save("c:\\result.png");
        }

        private void toolStripTextBox1_Click(object sender, EventArgs e)
        {

        }

        private void x05ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (null != m_oBitmap)
            {
                if (m_bYesToDrawImage)
                {
                    deScale *= 0.75f;

                    if (deScale <= 0.5f)
                        deScale = 1.0f;
                    

                    Width = (int)(m_oBitmap.Width * deScale) + 30;
                    Height = (int)(m_oBitmap.Height * deScale) + 80;

                    Update();
                    Refresh();
                }
            }
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            /*
            
            AVSGraphics.SVGTransformerClass piSvgTransformer = new AVSGraphics.SVGTransformerClass();
            piSvgTransformer.LoadFile("C:\\triangle.svg");//("C:\\new.pdf_temp.svg");
            int nWidthPx = piSvgTransformer.Width;
            int nHeightPx = piSvgTransformer.Height;
           // double dWidthMm = 2.54 * 100 * nWidthPx / 72; //72dpi
           // double dHeightMm = 2.54 * 100 * nHeightPx / 72; //72dpi
            double dWidthMm = nWidthPx; //72dpi
            double dHeightMm = nHeightPx; //72dpi

            AVSOfficePDFWriter.CPDFWriterClass piPdfWriter = new AVSOfficePDFWriter.CPDFWriterClass();
            piPdfWriter.CreatePDF();
            piPdfWriter.NewPage();
            piPdfWriter.Width = dWidthMm;
            piPdfWriter.Height = dHeightMm;
            piPdfWriter.BeginCommand(c_nPageType);
            piSvgTransformer.Draw(piPdfWriter, 0, 0, dWidthMm, dHeightMm);
            piPdfWriter.EndCommand(c_nPageType);
            piPdfWriter.SaveToFile("C:\\dump.pdf");
            piPdfWriter.DeletePDF();
            
            */
        }
    }
}
