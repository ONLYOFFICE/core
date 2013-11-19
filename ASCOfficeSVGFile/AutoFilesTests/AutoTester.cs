using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using System.Windows.Forms;

using AVSMediaCore3;
using AVSOfficeSVGFile;

using System.Reflection;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.Xml;
using System.Xml.Xsl;
using System.Diagnostics;
using System.Threading;
using System.Drawing;

namespace AutoFilesTests
{
    class AutoTester
    {
        delegate void delgateAddItemToList(ListBox list, string item);

        public string _filesPath = "";
        public ListBox _listInfo;
        public string _resultPath = "";
        private Thread _thread;

        private List<string> _fileHolder = new List<string>();
        public List<bool> _failedLoad = new List<bool>();

        public void run()
        {
            _listInfo.Items.Clear();

            _fileHolder.Clear();
            _failedLoad.Clear();

            _thread = new Thread(threadFunc);
            _thread.Priority = ThreadPriority.Normal;
            _thread.SetApartmentState(ApartmentState.STA);
            _thread.IsBackground = true;
            _thread.Start();
        }

        public void stop()
        {
            if ( null != _thread )
            {
                _thread.Abort();
            }
        }

        private void threadFunc()
        {
            dirSearch(_filesPath);

            delgateAddItemToList funcAddItem = delegate(ListBox list, string item) { list.Items.Add(item); list.SelectedIndex = list.Items.Count-1; };

            Stopwatch stopWatch = new Stopwatch();

            //Code

            for (int i =0; i < _fileHolder.Count; ++i)
            {
                stopWatch.Start();
                if (loadFileSVG(_fileHolder[i]))
                {
                    stopWatch.Stop();
                    TimeSpan ts = stopWatch.Elapsed;

                    _failedLoad.Add( true );

                    // Format and display the TimeSpan value.
                    string elapsedTime = String.Format("{0:00}:{1:00}.{2:00}",
                       ts.Minutes, ts.Seconds,
                        ts.Milliseconds / 10);

                    _listInfo.Invoke(funcAddItem, new object[] { _listInfo, String.Format("{0}% - {1}, time - {2}", (float)((double)(i + 1) / (double)_fileHolder.Count * 100.0), _fileHolder[i], elapsedTime) });
                }
                else
                {
                    stopWatch.Stop();
                    TimeSpan ts = stopWatch.Elapsed;
                   
                    _failedLoad.Add(false);

                    // Format and display the TimeSpan value.
                    string elapsedTime = String.Format("{0:00}:{1:00}.{2:00}",
                       ts.Minutes, ts.Seconds,
                        ts.Milliseconds / 10);

                    _listInfo.Invoke(funcAddItem, new object[] { _listInfo, String.Format("{0}% - {1}, time - {2} - failed", (float)((double)(i + 1) / (double)_fileHolder.Count * 100.0), _fileHolder[i], elapsedTime) });
                }
            }
        }

        private void dirSearch(string sDir)
        {
            try
            {
                foreach (string f in Directory.GetFiles(sDir, "*.svg"))
                    _fileHolder.Add(f);

                foreach (string d in Directory.GetDirectories(sDir))
                    dirSearch(d);
            }
            catch (System.Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        private bool loadFileSVG(string fileName)
        {
            XmlDocument oDocument = new XmlDocument();

            AVSOfficeSVGFile.ISVGTransformer oTranformer = new AVSOfficeSVGFile.SVGTransformerClass();
           
            try
            {
                oTranformer.LoadFile(fileName);
            }
            catch 
            {
                Marshal.FinalReleaseComObject(oTranformer);
                return false;
            }

            if (0 == oTranformer.Width || 0 == oTranformer.Height)
            {
                Marshal.FinalReleaseComObject(oTranformer);
                return false;
            }
          
            AVSGraphics.IAVSGraphicsRenderer oRender = new AVSGraphics.CAVSGraphicsRenderer();
            CAVSUncompressedVideoFrame oSource = new CAVSUncompressedVideoFrame();

            oSource.AspectRatioX = 0;
            oSource.AspectRatioY = 0;
            oSource.Width = (int)(oTranformer.Width);
            oSource.Height = (int)(oTranformer.Height);
            oSource.ColorSpace = (1 << 6);  // |(1 << 31) ; // NONE FLIP
            oSource.SetDefaultStrides();
            oSource.AllocateBuffer(oSource.Width * oSource.Height * 4);

            IAVSUncompressedVideoFrame image = (IAVSUncompressedVideoFrame)oSource;
            Bitmap oBitmap = null;
           
            try
            {
                oBitmap = new Bitmap(image.Width, image.Height, image.Width * 4, System.Drawing.Imaging.PixelFormat.Format32bppArgb, image.Buffer);
                using (Graphics graphics2 = Graphics.FromImage(oBitmap))
                {
                    // graphics2.Clear(Color.FromArgb(255, 255, 255, 255));
                    graphics2.Clear(Color.FromArgb(0, 0, 0, 0));
                }
            }
            catch (Exception e)
            {
                Marshal.FinalReleaseComObject(oTranformer);
                return false;
            }

            oRender.Width = oTranformer.Width;
            oRender.Height = oTranformer.Height;
            oRender.SetAdditionalParam("FontManager", null);
            oRender.CreateFromMediaData(oSource, 0.0, 0.0, (double)oSource.Width, (double)oSource.Height);

            oTranformer.Draw(oRender, 0, 0, (double)oSource.Width, (double)oSource.Height);

            Bitmap oBitmap2 = new Bitmap(image.Width, image.Height);
            using (Graphics gr = Graphics.FromImage(oBitmap2))
            {
                gr.Clear(Color.FromArgb(0, 0, 0, 0));
                gr.DrawImage(oBitmap, 0, 0, image.Width, image.Height);
            }

            oBitmap.Save(_resultPath +"\\" + Path.GetFileNameWithoutExtension(fileName) + ".png");

            Marshal.FinalReleaseComObject(oRender);
            Marshal.FinalReleaseComObject(oTranformer);
            Marshal.FinalReleaseComObject(oSource);

            oTranformer = null;
            oRender = null;
            oSource = null;

            return true;
        }
    }
}
