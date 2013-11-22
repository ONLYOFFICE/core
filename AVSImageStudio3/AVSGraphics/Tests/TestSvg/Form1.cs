using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using AVSMediaCore3;
using AVSGraphics;
using AVSOfficePDFWriter;

using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.Xml;
using System.Xml.Xsl;
using System.Diagnostics;
using System.Threading;

#if ACROBAT
using Acrobat;
#endif

namespace Test
{
    public partial class Form1 : Form
    {
        private Bitmap bitmap = null;
        private bool lockDraw = false;
        private float scaleImageForPaint = 1.0f;
        private volatile bool asyncMode = false;
        private bool asyncSave = false;

        private bool drawSvgFile = true;//false;
        private bool loadFromFile = true;
        private string pdfFileName = "";

        private uint c_nPageType = 0x0001;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // srcFileName.Text = "D:\\svg files\\USA\\Usa-clipSvgTutorial.svg";
            // srcFileName.Text = "D:\\svg files\\Image.svg";
            // srcFileName.Text = "D:\\svg files\\USA\\star.svg";
            // srcFileName.Text = "D:\\svg files\\Good\\circles1.svg";

            // srcFileName.Text = "D:\\svg files\\Good\\paths3.svg";
            // srcFileName.Text = "D:\\svg files\\Clip4x - No Text.svg";
            // srcFileName.Text = "D:\\svg files\\circles.svg";
            // srcFileName.Text = "D:\\svg files\\image1.svg";
            // srcFileName.Text = "D:\\svg files\\wikipedia\\Russian_Federation.svg";
            // srcFileName.Text = "D:\\svg files\\island.svg";
            // srcFileName.Text = "D:\\svg files\\test01\\01.svg";
            // srcFileName.Text = "D:\\svg files\\wikipedia\\files\\0.svg";

            if (drawSvgFile)
                srcFileName.Text = "D:\\svg files\\wikipedia\\files\\Austria_Bundesadler.svg";
            else
                srcFileName.Text = "D:\\svg files\\0.svg";

            //srcFileName.Text = "D:\\svg files\\linear_gradient.svg";

            string[] filePaths = Directory.GetFiles(Path.GetDirectoryName(srcFileName.Text), "*.svg", SearchOption.AllDirectories);

            for (int i = 0; i < filePaths.Length; ++i)
            {
                listBox1.Items.Add(filePaths[i]);
            }

            if (drawSvgFile)
            {
                renderSvgFile();

                Rectangle bounds = Screen.FromControl(this).Bounds;
                this.Location = new Point((int)(bounds.Width * 0.5 - Width * 0.5), (int)(bounds.Height * 0.5 - Height * 0.5));
            }
            else
            {
                SaveToPDF();
               // Close();
            }
        }

        private void openSvgFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Title = "Open SVG File";
            fileDialog.FileName = "";
            fileDialog.Filter = "SVG (*.svg)|*.svg";

            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                srcFileName.Text = fileDialog.FileName;
                renderSvgFile();
            }
        }

        private void renderSvgFile()
        {
            toolStripTextBox1.Text = "";

            lockDraw = true;

            IAVSGraphicsRenderer render = new CAVSGraphicsRenderer();
            CAVSUncompressedVideoFrame frame = new CAVSUncompressedVideoFrame();

            ISVGTransformer svgFile = new SVGTransformerClass();

            if (loadFromFile)   //  load from file
            {
                try
                {
                    svgFile.LoadFile(srcFileName.Text);
                }
                catch (COMException ex)
                {
                    MessageBox.Show(ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                svgFile.SetAdditionalParam("WorkingDirectory", Path.GetDirectoryName(srcFileName.Text));

                StreamReader xmlFile = new StreamReader(srcFileName.Text);
                svgFile.Load(xmlFile.ReadToEnd());
                xmlFile.Close();
            }

            if (0 == svgFile.Width || 0 == svgFile.Height)
            {
                Marshal.FinalReleaseComObject(render);
                Marshal.FinalReleaseComObject(svgFile);

                return;
            }

            frame.AspectRatioX = 0;
            frame.AspectRatioY = 0;
            frame.Width = (int)(svgFile.Width);
            frame.Height = (int)(svgFile.Height);
            frame.ColorSpace = (1 << 6);  // |(1 << 31) ; // NONE FLIP
            frame.SetDefaultStrides();
            frame.AllocateBuffer(frame.Width * frame.Height * 4);

            bitmap = null;
            IAVSUncompressedVideoFrame image = (IAVSUncompressedVideoFrame)frame;

            Bitmap oBitmap = new Bitmap(image.Width, image.Height, image.Width * 4, System.Drawing.Imaging.PixelFormat.Format32bppArgb, image.Buffer);
            using (Graphics g = Graphics.FromImage(oBitmap))
            {
                g.Clear(Color.White);

                Pen p = new Pen(Color.FromArgb(60, Color.Blue));
                int cellSize = 15;
                int numOfCells = Math.Max(image.Height / cellSize, image.Width / cellSize);

                for (int y = 1; y < numOfCells; ++y)
                {
                    g.DrawLine(p, 0, y * cellSize, numOfCells * cellSize, y * cellSize);
                }

                for (int x = 1; x < numOfCells; ++x)
                {
                    g.DrawLine(p, x * cellSize, 0, x * cellSize, numOfCells * cellSize);
                }
            }

            render.Width = svgFile.Width;
            render.Height = svgFile.Height;
            //oRender.SetAdditionalParam("FontManager", null);
            render.CreateFromMediaData(frame, 0.0, 0.0, (double)frame.Width, (double)frame.Height);

            svgFile.Draw(render, 0, 0, 800, 600);

            Bitmap oBitmap2 = new Bitmap(image.Width, image.Height);
            using (Graphics gr = Graphics.FromImage(oBitmap2))
            {
                gr.DrawImage(oBitmap, 0, 0, image.Width, image.Height);
            }

            int windowW = Math.Max((int)(oBitmap2.Width * scaleImageForPaint) + 60, 1200);
            int windowH = Math.Max((int)(oBitmap2.Height * scaleImageForPaint) + 100, 600);

            oBitmap = null;

            Marshal.FinalReleaseComObject(render);
            Marshal.FinalReleaseComObject(svgFile);
            Marshal.FinalReleaseComObject(image);

            svgFile = null;
            render = null;

            bitmap = oBitmap2;
            lockDraw = false;

            Rectangle bounds = Screen.FromControl(this).Bounds;

            if (windowW > bounds.Width || windowH > bounds.Height)
            {
                while (scaleImageForPaint > 0.1f)
                {
                    scaleImageForPaint *= 0.75f;

                    windowW = (int)(oBitmap2.Width * scaleImageForPaint) + 100;
                    windowH = (int)(oBitmap2.Height * scaleImageForPaint) + 100;

                    if (windowW < bounds.Width && windowH < bounds.Height)
                        break;
                }
            }

            Width = windowW;
            Height = windowH;

            Update();
            Refresh();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            if (null != bitmap)
            {
                if (!lockDraw)
                    e.Graphics.DrawImage(bitmap, 20, 40, bitmap.Width * scaleImageForPaint, bitmap.Height * scaleImageForPaint);
            }
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            renderSvgFile();
        }

        private void saveImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (null != bitmap)
                bitmap.Save(Path.GetDirectoryName(srcFileName.Text) + "\\" + "Image.png");
        }

        private void toolStripTextBox1_Click(object sender, EventArgs e)
        {

        }

        private void x05ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (null != bitmap)
            {
                if (!lockDraw)
                {
                    scaleImageForPaint *= 0.75f;

                    if (scaleImageForPaint <= 0.1f)
                        scaleImageForPaint = 1.0f;


                    Width = Math.Max((int)(bitmap.Width * scaleImageForPaint) + 20 * 2, 1200);
                    Height = Math.Max((int)(bitmap.Height * scaleImageForPaint) + 20 * 2, 600);

                    Update();
                    Refresh();
                }
            }
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            if (!asyncSave)
            {
                SaveToPDF();
                return;
            }

            if (asyncMode)
                return;

            Thread thread = new Thread(SaveToPDF);
            thread.Start();
        }

        private void SaveToPDF()
        {
            asyncMode = true;

            pdfFileName = Path.GetDirectoryName(srcFileName.Text) + "\\" + "PDF.pdf";

            var dateOne = DateTime.Now;

            SVGTransformerClass svgFile = new SVGTransformerClass();
            svgFile.LoadFile(srcFileName.Text);

            double mmW = 2.54 * 10 * svgFile.Width / 72.0;       //  72dpi
            double mmH = 2.54 * 10 * svgFile.Height / 72.0;      //  72dpi

            //double mmW = svgFile.Width; //72dpi
            //double mmH = svgFile.Height; //72dpi

            CPDFWriterClass pdfFile = new CPDFWriterClass();

            pdfFile.CreatePDF();

            var drawBegin = DateTime.Now;

            pdfFile.NewPage();
            pdfFile.Width = mmW;
            pdfFile.Height = mmH;
            pdfFile.BeginCommand(c_nPageType);

            svgFile.Draw(pdfFile, 0, 0, mmW, mmH);

            pdfFile.EndCommand(c_nPageType);
            pdfFile.SaveToFile(pdfFileName);

            var drawEnd = DateTime.Now;

            pdfFile.DeletePDF();

            Marshal.FinalReleaseComObject(svgFile);
            Marshal.FinalReleaseComObject(pdfFile);

            var dateTwo = DateTime.Now;
            var diff = dateTwo.Subtract(dateOne);

            var drawTime = drawEnd.Subtract(drawBegin);

            asyncMode = false;

            this.Invoke((MethodInvoker)delegate
            {
                if (drawSvgFile)
                {
                    toolStripTextBox1.Text = String.Format("Draw: {00}:{01}:{02}:{03}, Build : {04}:{05}:{06}:{07}",
                        drawTime.Hours, drawTime.Minutes, drawTime.Seconds, drawTime.Milliseconds,
                        diff.Hours, diff.Minutes, diff.Seconds, diff.Milliseconds);
#if ACROBAT
                    // проверим на валидность созданного файла

                    Form form = new Form();
                    form.Text = "Adobe Acrobat Validator";
                    form.StartPosition = FormStartPosition.CenterScreen;
                    form.ClientSize = new System.Drawing.Size(800, 600);

                    try
                    {
                        System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form));

                        AxAcroPDFLib.AxAcroPDF acrobat = new AxAcroPDFLib.AxAcroPDF();
                        ((System.ComponentModel.ISupportInitialize)(acrobat)).BeginInit();

                        //form.SuspendLayout();

                        acrobat.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top |
                            System.Windows.Forms.AnchorStyles.Bottom) | System.Windows.Forms.AnchorStyles.Left) | System.Windows.Forms.AnchorStyles.Right)));

                        acrobat.Enabled = true;

                        acrobat.Location = new System.Drawing.Point(10, 10);
                        acrobat.Name = "AdobeAcrobatContol";
                        acrobat.Size = new System.Drawing.Size(form.ClientSize.Width - 10 * 2, form.ClientSize.Height - 10 * 2);
                        acrobat.TabIndex = 0;
                        form.Controls.Add(acrobat);

                        ((System.ComponentModel.ISupportInitialize)(acrobat)).EndInit();

                        acrobat.MouseCaptureChanged += null;

                        form.SizeChanged += delegate(object o, EventArgs e)
                        {
                            acrobat.Size = new System.Drawing.Size(form.ClientSize.Width - 10 * 2, form.ClientSize.Height - 10 * 2);
                        };

                        acrobat.LoadFile(pdfFileName);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.ToString(), "Error open pdf file", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }

                    form.ShowDialog();
#endif
                }
            });
        }

        private void Form1_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] filePaths = (string[])(e.Data.GetData(DataFormats.FileDrop));
                foreach (string fileLoc in filePaths)
                {
                    if (File.Exists(fileLoc))
                    {
                        srcFileName.Text = fileLoc;
                        renderSvgFile();
                    }
                }
            }
        }

        private void Form1_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop)) e.Effect = DragDropEffects.Move;
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            srcFileName.Text = (string)listBox1.SelectedItem;

            renderSvgFile();
        }
    }
}
