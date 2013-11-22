using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace WindowsApplication1
{
    public partial class Form1 : Form
    {
        private string m_strFileName = "";

        AVSOfficeUniversalConverter.CAVSOfficeFilesCacheClass m_oCache = new AVSOfficeUniversalConverter.CAVSOfficeFilesCacheClass();
        AVSOfficeUniversalConverter.CAVSXMLSingleOfficeSourceClass m_oSource = new AVSOfficeUniversalConverter.CAVSXMLSingleOfficeSourceClass();
        
        private bool m_bIsScrollPositionAttack  = true;
        private bool m_bIsScrollPositionAttack2 = true;

        private int m_lCaclKoef = 30000;

        private double m_dPositionHor = 0;
        private double m_dPositionVer = 0;

        private double m_dPositionHor2 = 0;
        private double m_dPositionVer2 = 0;

        private bool m_bIsComboPageAttack = true;

        private bool m_bIsPDFSource = false;

        private delegate void DelegateType<T>(T type);

        bool m_bTabsEvent = false;
        bool m_bTabsEnabled = false;

        string m_strWatermark = "<Watermark mode='0' text='WATERMARK'>" +
                                	"<Attributes brush-type='1000' brush-color1='0' brush-color2='16777215' brush-alpha1='255' " + 
                                    "brush-alpha2='255' brush-texturepath='' brush-texturealpha='255' brush-texturemode='0' brush-rectable='0' " +
                                    "font-name='Arial' font-size='36' font-bold='0' font-italic='0' font-underline='0' font-strikeout='0' " +
                                    "font-antialiastext='1' font-stringalignmentvertical='1' textalignment='1' font-angle='0' />" +
                                "</Watermark>";

        bool m_bWatermark = false;

        public Form1()
        {
            InitializeComponent();

            m_oSource.OnProgress += new AVSOfficeUniversalConverter._IAVSXMLOfficeSourceEvents_OnProgressEventHandler(m_oSource_OnProgress);
            //m_oSource.OnCompletePage += new AVSOfficeUniversalConverter._IAVSXMLOfficeSourceEvents_OnCompletePageEventHandler(m_oSource_OnCompletePage);
            axCAVSDocumentViewer1.OnDocumentSizeChanged         += new AxAVSGraphics._IAVSDocumentViewerEvents_OnDocumentSizeChangedEventHandler(axCAVSDocumentViewer1_OnDocumentSizeChanged);
            axCAVSDocumentViewer1.OnDocumentPositionChanged     += new AxAVSGraphics._IAVSDocumentViewerEvents_OnDocumentPositionChangedEventHandler(axCAVSDocumentViewer1_OnDocumentPositionChanged);
            axCAVSDocumentViewer1.OnDocumentPageChanged         += new AxAVSGraphics._IAVSDocumentViewerEvents_OnDocumentPageChangedEventHandler(axCAVSDocumentViewer1_OnDocumentPageChanged);
            axCAVSDocumentViewer1.OnDocumentCountPagesChanged   += new AxAVSGraphics._IAVSDocumentViewerEvents_OnDocumentCountPagesChangedEventHandler(axCAVSDocumentViewer1_OnDocumentCountPagesChanged);

            axCAVSThumbnailsViewer1.OnDocumentSizeChanged       += new AxAVSGraphics._IAVSThumbnailsViewerEvents_OnDocumentSizeChangedEventHandler(axCAVSThumbnailsViewer1_OnDocumentSizeChanged);
            axCAVSThumbnailsViewer1.OnDocumentPositionChanged   += new AxAVSGraphics._IAVSThumbnailsViewerEvents_OnDocumentPositionChangedEventHandler(axCAVSThumbnailsViewer1_OnDocumentPositionChanged);

            toolTextCurPage.Enabled = false;
            toolTextPagesCount.Enabled = false;

            m_bIsScrollPositionAttack   = false;

            vScrollBar1.Minimum         = 0;
            vScrollBar1.Maximum         = m_lCaclKoef;
            vScrollBar1.LargeChange     = m_lCaclKoef;
            vScrollBar1.SmallChange     = 1;

            hScrollBar1.Minimum         = 0;
            hScrollBar1.Maximum         = 10000;
            hScrollBar1.LargeChange     = 10000;
            hScrollBar1.SmallChange     = 1;

            vScrollBar2.Minimum         = 0;
            vScrollBar2.Maximum         = m_lCaclKoef;
            vScrollBar2.LargeChange     = m_lCaclKoef;
            vScrollBar2.SmallChange     = 1;

            hScrollBar2.Minimum         = 0;
            hScrollBar2.Maximum         = 10000;
            hScrollBar2.LargeChange     = 10000;
            hScrollBar2.SmallChange     = 1;

            m_bIsScrollPositionAttack   = true;
            m_bIsScrollPositionAttack2  = true;
           
            toolComboViewMode.SelectedIndex = 1;

            axCAVSThumbnailsViewer1.Dock = DockStyle.Fill;

            axCAVSThumbnailsViewer1.DocumentViewer = (object)axCAVSDocumentViewer1.GetOcx();
            axCAVSDocumentViewer1.ThumbnailsViewer = (object)axCAVSThumbnailsViewer1.GetOcx();
            axCAVSThumbnailsViewer1.VisibleViewerBounds = true;

            axCAVSDocumentViewer1.Init();
            axCAVSThumbnailsViewer1.Init();
        }
        
        bool bFlag = false;
        void m_oSource_OnCompletePage()
        {
            if (false == bFlag)
            {
                bFlag = true;
                object punkFrame = null;
                m_oSource.ReadPage(0, ref punkFrame);
            }
        }

        private void axCAVSDocumentViewer1_OnDocumentCountPagesChanged(object sender, AxAVSGraphics._IAVSDocumentViewerEvents_OnDocumentCountPagesChangedEvent e)
        {
            DelegateType<bool> Delegate = delegate(bool value)
            {
                if (e.lPageCount == 0)
                {
                    toolComboPages.Items.Clear();

                    if (toolComboPages.Enabled)
                        toolComboPages.Enabled = false;

                    toolTextPagesCount.Text = "0";
                    toolTextCurPage.Text = "0";
                }
                else
                {
                    if (!toolComboPages.Enabled)
                        toolComboPages.Enabled = true;

                    m_bIsComboPageAttack = false;

                    int nCountOld = toolComboPages.Items.Count;
                    if (nCountOld > e.lPageCount)
                    {
                        toolComboPages.Items.Clear();
                        nCountOld = 0;
                    }

                    while (nCountOld < e.lPageCount)
                    {
                        toolComboPages.Items.Add("Page " + ((int)(nCountOld + 1)).ToString());
                        ++nCountOld;
                    }

                    toolTextPagesCount.Text = ((int)(nCountOld)).ToString();

                    int nCur = axCAVSDocumentViewer1.CurrentPage;
                    toolButtonPrevPage.Enabled = (0 != nCur);
                    toolButtonNextPage.Enabled = (nCur < (e.lPageCount - 1));

                    m_bIsComboPageAttack = true;
                }
            };

            this.BeginInvoke(Delegate, new object[] { true });
        }

        private void axCAVSDocumentViewer1_OnDocumentPageChanged(object sender, AxAVSGraphics._IAVSDocumentViewerEvents_OnDocumentPageChangedEvent e)
        {
            DelegateType<bool> Delegate = delegate(bool value)
            {
                toolTextCurPage.Text = ((int)(e.lPageNumber + 1)).ToString();

                toolButtonPrevPage.Enabled = (0 != e.lPageNumber);
                toolButtonNextPage.Enabled = (e.lPageNumber < (axCAVSDocumentViewer1.CountPages - 1));

                m_bIsComboPageAttack = false;
                toolComboPages.SelectedIndex = e.lPageNumber;
                m_bIsComboPageAttack = true;
            };

            this.BeginInvoke(Delegate, new object[] { true });
        }

        private void axCAVSDocumentViewer1_OnDocumentPositionChanged(object sender, AxAVSGraphics._IAVSDocumentViewerEvents_OnDocumentPositionChangedEvent e)
        {
            DelegateType<bool> Delegate = delegate(bool value)
            {
                m_bIsScrollPositionAttack = false;

                int nMaxV = vScrollBar1.Maximum - vScrollBar1.LargeChange;
                int nMaxH = hScrollBar1.Maximum - hScrollBar1.LargeChange;

                vScrollBar1.Value = Math.Max(vScrollBar1.Minimum, Math.Min(nMaxV, (int)(nMaxV * e.dPositionVer)));
                hScrollBar1.Value = Math.Max(hScrollBar1.Minimum, Math.Min(nMaxH, (int)(nMaxH * e.dPositionHor)));

                vScrollBar1.Update();
                hScrollBar1.Update();

                m_bIsScrollPositionAttack = true;
            };

            this.BeginInvoke(Delegate, new object[] { true });
        }

        private void axCAVSDocumentViewer1_OnDocumentSizeChanged(object sender, AxAVSGraphics._IAVSDocumentViewerEvents_OnDocumentSizeChangedEvent e)
        {
            DelegateType<bool> Delegate = delegate(bool value)
            {
                m_bIsScrollPositionAttack = false;

                // здесь нужно выставить горизонтальный/вертикальный скроллы
                double dDocumentWidth = 1000 * e.dWidth;
                double dDocumentHeight = 1000 * e.dHeight;

                if (dDocumentHeight == 0 || dDocumentWidth == 0)
                {
                    vScrollBar1.LargeChange = 10000;
                    hScrollBar1.LargeChange = 10000;

                    m_dPositionHor = 0;
                    m_dPositionVer = 0;

                    m_bIsScrollPositionAttack = true;
                    return;
                }

                double dControlWidth = (double)axCAVSDocumentViewer1.ControlWidth;
                double dControlHeight = (double)axCAVSDocumentViewer1.ControlHeight;

                int nLargeChangeV = (int)Math.Min(m_lCaclKoef, m_lCaclKoef * dControlHeight / dDocumentHeight);
                int nLargeChangeH = (int)Math.Min(10000.0, 10000.0 * dControlWidth  / dDocumentWidth);

                vScrollBar1.LargeChange = nLargeChangeV;
                hScrollBar1.LargeChange = nLargeChangeH;

                vScrollBar1.Update();
                hScrollBar1.Update();

                m_bIsScrollPositionAttack = true;
            };

            this.BeginInvoke(Delegate, new object[] { true });
        }

        private void axCAVSThumbnailsViewer1_OnDocumentPositionChanged(object sender, AxAVSGraphics._IAVSThumbnailsViewerEvents_OnDocumentPositionChangedEvent e)
        {
            DelegateType<bool> Delegate = delegate(bool value)
            {
                m_bIsScrollPositionAttack2 = false;

                int nMaxV = vScrollBar2.Maximum - vScrollBar2.LargeChange;
                int nMaxH = hScrollBar2.Maximum - hScrollBar2.LargeChange;

                vScrollBar2.Value = Math.Max(vScrollBar2.Minimum, Math.Min(nMaxV, (int)(nMaxV * e.dPositionVer)));
                hScrollBar2.Value = Math.Max(hScrollBar2.Minimum, Math.Min(nMaxH, (int)(nMaxH * e.dPositionHor)));

                vScrollBar2.Update();
                hScrollBar2.Update();

                m_bIsScrollPositionAttack2 = true;
            };

            this.BeginInvoke(Delegate, new object[] { true });
        }

        private void axCAVSThumbnailsViewer1_OnDocumentSizeChanged(object sender, AxAVSGraphics._IAVSThumbnailsViewerEvents_OnDocumentSizeChangedEvent e)
        {
            DelegateType<bool> Delegate = delegate(bool value)
            {
                m_bIsScrollPositionAttack2 = false;

                // здесь нужно выставить горизонтальный/вертикальный скроллы
                double dDocumentWidth = 1000 * e.dWidth;
                double dDocumentHeight = 1000 * e.dHeight;

                if (dDocumentHeight == 0 || dDocumentWidth == 0)
                {
                    vScrollBar2.LargeChange = 10000;
                    hScrollBar2.LargeChange = 10000;

                    m_dPositionHor2 = 0;
                    m_dPositionVer2 = 0;

                    m_bIsScrollPositionAttack2 = true;
                    return;
                }

                double dControlWidth = (double)axCAVSThumbnailsViewer1.ControlWidth;
                double dControlHeight = (double)axCAVSThumbnailsViewer1.ControlHeight;

                int nLargeChangeV = (int)Math.Min(m_lCaclKoef, m_lCaclKoef * dControlHeight / dDocumentHeight);
                int nLargeChangeH = (int)Math.Min(10000.0, 10000 * dControlWidth / dDocumentWidth);

                vScrollBar2.LargeChange = nLargeChangeV;
                hScrollBar2.LargeChange = nLargeChangeH;

                vScrollBar2.Update();
                hScrollBar2.Update();

                m_bIsScrollPositionAttack2 = true;
            };

            this.BeginInvoke(Delegate, new object[] { true });
        }

        private void m_oSource_OnProgress(int nID, int lProgressPage, ref short Cancel)
        {
            DelegateType<bool> Delegate = delegate(bool value)
            {
                progressBar1.Value = (int)(lProgressPage / 10000);
                progressBar1.Update();
            };

            this.BeginInvoke(Delegate, new object[] { true });
        }

        private void toolBottonOpen_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK != openFileDialog1.ShowDialog())
            {
                this.Text = "DocumentViewer - NoDocument";
                return;
            }

            m_strFileName = openFileDialog1.FileName;

            this.Text = "DocumentViewer - " + m_strFileName;
            OpenFile();
        }

        private void OpenFile()
        {
            // открытие файла.
            //string strID = tabControl1.TabCount.ToString();
            //создаем AVSOfficeFile
            AVSOfficeFile.CAVSOfficeFile piOfficeFile = new AVSOfficeFile.CAVSOfficeFile();
            //ставим темповую директорю
            piOfficeFile.TempDirectory = "";
            try
            {
                //открываем файл
                piOfficeFile.OpenFile(m_strFileName, "");
            }
            catch{
                //обработка ошибок OpenFile (например обработка пароля в PDF)
            }
            //добавляем в кеш с индексом, который в дальнейшем будет использовать OfficeUniversalConverter
            m_oCache.AddFile( m_oCache.GetCount(),  piOfficeFile);

            //передаем кеш в Source
            m_oSource.OfficeFilesCache = m_oCache;

            m_bTabsEvent = false;
            tabControl1.TabPages.Add(System.IO.Path.GetFileName(m_strFileName));
            m_bTabsEvent = true;

            m_bTabsEnabled = false;
            tabControl1.SelectedIndex = tabControl1.TabCount - 1;

            if (!m_bTabsEnabled)
            {
                tabControl1_SelectedIndexChanged(null, null);
            }

            if ((m_strFileName.IndexOf(".pdf") != -1) || (-1 != m_strFileName.IndexOf(".xps")))
                m_bIsPDFSource = true;
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            labelZoom.Text = "Zoom = " + trackBar1.Value.ToString() + "%";
            labelZoom.Update();
            axCAVSDocumentViewer1.Zoom = ((double)trackBar1.Value / 100.0);
        }

        private void trackBar2_ValueChanged(object sender, EventArgs e)
        {
            labelZoom2.Text = "Zoom = " + trackBar2.Value.ToString() + "%";
            labelZoom2.Update();
            axCAVSThumbnailsViewer1.Zoom = ((double)trackBar2.Value / 100.0);
        }

        private void toolComboViewMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            int nIndex = toolComboViewMode.SelectedIndex;

            switch (nIndex)
            {
                case 0:
                    {
                        axCAVSDocumentViewer1.ViewMode = 3;
                        axCAVSDocumentViewer1.Zoom = (double)trackBar1.Value / 100.0;
                        trackBar1.Enabled = true;
                        break;
                    }
                case 1:
                    {
                        axCAVSDocumentViewer1.ViewMode = 1;
                        trackBar1.Enabled = false;
                        break;
                    }
                case 2:
                    {
                        axCAVSDocumentViewer1.ViewMode = 2;
                        trackBar1.Enabled = false;
                        break;
                    }
                case 3:
                    {
                        axCAVSDocumentViewer1.ViewMode = 0;
                        trackBar1.Enabled = false;
                        break;
                    }
                default:
                    break;
            }
        }

        private void vScrollBar1_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsScrollPositionAttack)
                return;

            m_dPositionVer = (double)vScrollBar1.Value / (vScrollBar1.Maximum - vScrollBar1.LargeChange);
            m_dPositionVer = Math.Max(0, Math.Min(1, m_dPositionVer));

            axCAVSDocumentViewer1.PositionVer = m_dPositionVer;
        }

        private void hScrollBar1_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsScrollPositionAttack)
                return;

            m_dPositionHor = (double)hScrollBar1.Value / (hScrollBar1.Maximum - hScrollBar1.LargeChange);
            m_dPositionHor = Math.Max(0, Math.Min(1, m_dPositionHor));

            axCAVSDocumentViewer1.PositionHor = m_dPositionHor;
        }

        private void vScrollBar2_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsScrollPositionAttack2)
                return;

            m_dPositionVer2 = (double)vScrollBar2.Value / (vScrollBar2.Maximum - vScrollBar2.LargeChange);
            m_dPositionVer2 = Math.Max(0, Math.Min(1, m_dPositionVer2));

            axCAVSThumbnailsViewer1.PositionVer = m_dPositionVer2;
        }

        private void hScrollBar2_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsScrollPositionAttack2)
                return;

            m_dPositionHor2 = (double)hScrollBar2.Value / (hScrollBar2.Maximum - hScrollBar2.LargeChange);
            m_dPositionHor2 = Math.Max(0, Math.Min(1, m_dPositionHor2));

            axCAVSThumbnailsViewer1.PositionHor = m_dPositionHor2;
        }

        private void toolComboPages_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!m_bIsComboPageAttack)
                return;

            axCAVSDocumentViewer1.CurrentPage = toolComboPages.SelectedIndex;
        }

        private void toolButtonNextPage_Click(object sender, EventArgs e)
        {
            axCAVSDocumentViewer1.CurrentPage = (axCAVSDocumentViewer1.CurrentPage + 1);
        }

        private void toolButtonPrevPage_Click(object sender, EventArgs e)
        {
            axCAVSDocumentViewer1.CurrentPage = (axCAVSDocumentViewer1.CurrentPage - 1);
        }

        private void cont_one_Click(object sender, EventArgs e)
        {
            axCAVSDocumentViewer1.PagesInBlock = 1;
        }

        private void break_one_Click(object sender, EventArgs e)
        {
            axCAVSDocumentViewer1.PagesInBlock = 1;
        }

        private void cont_two_Click(object sender, EventArgs e)
        {
            axCAVSDocumentViewer1.PagesInBlock = 2;
        }

        private void break_two_Click(object sender, EventArgs e)
        {
            axCAVSDocumentViewer1.PagesInBlock = 2;
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!m_bTabsEvent)
                return;

            string strID = tabControl1.SelectedIndex.ToString();
            string strXml = "<SingleSource><Source id='" + strID + "'/></SingleSource>";

            m_oSource.Close();
            progressBar1.Value = 100;
            progressBar1.Update();
            m_oSource.Open(strXml);

            axCAVSDocumentViewer1.CloseFile();
            axCAVSThumbnailsViewer1.CloseFile();

            axCAVSDocumentViewer1.OfficeSource      = m_oSource;
            axCAVSThumbnailsViewer1.OfficeSource    = m_oSource;

            //m_oSource.Start();

            m_bTabsEnabled = true;
        }

        private void Form1_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] aFiles = (string[])e.Data.GetData(DataFormats.FileDrop);

                if (aFiles.Length != 0)
                {
                    m_strFileName = aFiles[0];
                    OpenFile();
                }
            }
        }

        private void Form1_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.Copy;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        private void toolStripWater_Click(object sender, EventArgs e)
        {
            m_bWatermark = !m_bWatermark;

            if (!m_bWatermark)
                m_oSource.Watermark = null;
            else
                m_oSource.Watermark = m_strWatermark;
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            axCAVSDocumentViewer1.CloseFile();
            axCAVSThumbnailsViewer1.CloseFile();

            axCAVSThumbnailsViewer1.DocumentViewer = null;
            axCAVSDocumentViewer1.ThumbnailsViewer = null;
        }

        private void toolStripButtonEpub_Click(object sender, EventArgs e)
        {
            AVSEBOOKWriter.CAVSEBOOKWriterClass oWriter = new AVSEBOOKWriter.CAVSEBOOKWriterClass();

            oWriter.TempDirectory = "C:\\PPTMemory";
            oWriter.SrcFileType = 0;
            if (m_bIsPDFSource)
                oWriter.SrcFileType = 1;

            oWriter.CreateOfficeFile("C:\\PPTMemory\\1.epub", 0);

            int nPagesCount = m_oSource.GetPageCount();
            for (int i = 0; i < nPagesCount; ++i)
            {
                //if (!m_bIsPDFSource)
                {
                    oWriter.NewPage();
                    oWriter.BeginCommand(1);
                }

                AVSGraphics.CAVSMetafile pMeta = (AVSGraphics.CAVSMetafile)m_oSource.ReadMetafile(i);

                int nBreak = 0;
                if (null != pMeta)
                    pMeta.Draw2(oWriter, 96, 96, ref nBreak);

                Marshal.ReleaseComObject(pMeta);
                //if (!m_bIsPDFSource)
                {
                    oWriter.EndCommand(1);
                }
            }
            oWriter.CloseFile();
        }

        private void toolStripButtonFb2_Click(object sender, EventArgs e)
        {
            AVSEBOOKWriter.CAVSEBOOKWriterClass oWriter = new AVSEBOOKWriter.CAVSEBOOKWriterClass();
            
            oWriter.TempDirectory = "C:\\PPTMemory";
            oWriter.SrcFileType = 0;
            if (m_bIsPDFSource)
                oWriter.SrcFileType = 1;

            oWriter.CreateOfficeFile("C:\\PPTMemory\\1.fb2", 1);

            int nPagesCount = m_oSource.GetPageCount();
            for (int i = 0; i < nPagesCount; ++i)
            {
                if (!m_bIsPDFSource)
                {
                    oWriter.NewPage();
                    oWriter.BeginCommand(1);
                }

                AVSGraphics.CAVSMetafile pMeta = (AVSGraphics.CAVSMetafile)m_oSource.ReadMetafile(i);

                int nBreak = 0;
                if (null != pMeta)
                    pMeta.Draw2(oWriter, 96, 96, ref nBreak);

                if (null != pMeta)
                    Marshal.ReleaseComObject(pMeta);
                
				if (!m_bIsPDFSource)
                {
                    oWriter.EndCommand(1);
                }
            }
            oWriter.CloseFile();
        }

        private void toolStripButtonMobi_Click(object sender, EventArgs e)
        {
            AVSEBOOKWriter.CAVSEBOOKWriterClass oWriter = new AVSEBOOKWriter.CAVSEBOOKWriterClass();

            oWriter.TempDirectory = "C:\\PPTMemory";
            oWriter.SrcFileType = 0;
            if (m_bIsPDFSource)
                oWriter.SrcFileType = 1;

            oWriter.CreateOfficeFile("C:\\PPTMemory\\1.mobi", 2);

            int nPagesCount = m_oSource.GetPageCount();
            for (int i = 0; i < nPagesCount; ++i)
            {
                if (!m_bIsPDFSource)
                {
                    oWriter.NewPage();
                    oWriter.BeginCommand(1);
                }

                AVSGraphics.CAVSMetafile pMeta = (AVSGraphics.CAVSMetafile)m_oSource.ReadMetafile(i);

                int nBreak = 0;
                if (null != pMeta)
                    pMeta.Draw2(oWriter, 96, 96, ref nBreak);

                Marshal.ReleaseComObject(pMeta);

                if (!m_bIsPDFSource)
                {
                    oWriter.EndCommand(1);
                }
            }
            oWriter.CloseFile();
        }
    }
}