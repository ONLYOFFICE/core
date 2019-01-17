/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Security.Permissions;

namespace FontEngineTester
{
    public partial class Form1 : Form
    {
        private string[] m_files = null;
        private int m_cur_file = 0;
        private AVSOfficeFile.CAVSOfficeFileConverterClass m_oConverter = null;
        private bool m_bIsEvent = true;
        private delegate void DelegateType<T>(T type);
        
        public Form1()
        {
            InitializeComponent();
            
            textBox1.Text = "C:\\Users\\Oleg.Korshul\\Desktop\\src";
            textBox2.Text = "C:\\Users\\Oleg.Korshul\\Desktop\\dst";

            progressBar1.Maximum = 1000000;

            backgroundWorker1.WorkerReportsProgress = true;
            backgroundWorker1.WorkerSupportsCancellation = true;

            backgroundWorker1.DoWork += new DoWorkEventHandler(backgroundWorker1_DoWork);
            backgroundWorker1.RunWorkerCompleted += new RunWorkerCompletedEventHandler(backgroundWorker1_RunWorkerCompleted);
            backgroundWorker1.ProgressChanged += new ProgressChangedEventHandler(backgroundWorker1_ProgressChanged);
        }

        private void m_oConverter_OnProgressEx(int nID, int nPercent, ref short Cancel)
        {
            if (false == m_bIsEvent)
                return;

            DelegateType<int> Delegate = delegate(int value) 
            {
                progressBar1.Value = Math.Max(0, Math.Min(value, 1000000));
                this.Refresh(); 
            };
            this.Invoke(Delegate, new object[] { nPercent });
         }

        private void button1_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK != folderBrowserDialog1.ShowDialog())
                return;

            textBox1.Text = folderBrowserDialog1.SelectedPath;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK != folderBrowserDialog1.ShowDialog())
                return;

            textBox2.Text = folderBrowserDialog1.SelectedPath;
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            m_files = Directory.GetFiles(textBox1.Text, "*.pdf", SearchOption.AllDirectories);
            m_cur_file = 0;

            string strDstFolder = textBox2.Text;

            Directory.CreateDirectory(strDstFolder);

            m_oConverter = new AVSOfficeFile.CAVSOfficeFileConverterClass();
            m_oConverter.OnProgressEx += new AVSOfficeFile._IAVSOfficeFileConverterEvents_OnProgressExEventHandler(m_oConverter_OnProgressEx);

            for (; m_cur_file < m_files.Length; ++m_cur_file)
            {
                try
                {

                    if (worker.CancellationPending == true)
                    {
                        e.Cancel = true;
                        break;
                    }

                    worker.ReportProgress(m_cur_file + 1);
                    //progressBar1.Value = 0;

                    this.m_bIsEvent = false;
                    m_oConverter.LoadFromFile(m_files[m_cur_file], "", "<Options><Display/><DownloadImages>1</DownloadImages></Options>");
                    this.m_bIsEvent = true;
                    m_oConverter.FileType = 520;

                    string strTemp = Path.GetTempPath();
                    strTemp += "PdfSorter";

                    if (Directory.Exists(strTemp))
                    {
                        DirectoryInfo _dInf = new DirectoryInfo(strTemp);
                        System.Security.AccessControl.DirectorySecurity _dsec = _dInf.GetAccessControl();
                        _dInf.Delete(true);
                        _dInf.Create(_dsec);
                    }
                    else
                    {
                        Directory.CreateDirectory(strTemp);
                    }

                    string sFileName = Path.GetFileName(m_files[m_cur_file]);

                    m_oConverter.SaveToFile(strTemp + "\\" + sFileName + ".html", "", "<Options><LoadInCommandRenderer/><HtmlRendererMode>20</HtmlRendererMode></Options>");

                    string[] strdst = Directory.GetDirectories(strTemp);

                    if (1 == strdst.Length)
                    {
                        // находим файл с инфой о шрифтах
                        string sFontsFile = strdst[0] + "\\fonts\\types.txt";
                        StreamReader oReader = new StreamReader(sFontsFile);
                        string sTypes = oReader.ReadToEnd();
                        oReader.Close();

                        int nFonts = 0;
                        int nTT = sTypes.IndexOf(": TrueType,");
                        int nCFF = sTypes.IndexOf(": CFF,");
                        int nType1 = sTypes.IndexOf(": Type 1,");
                        int nType2 = sTypes.IndexOf(": Type 42,");
                        int nType1Cid = sTypes.IndexOf(": CID Type 1,");
                        int nBDF = sTypes.IndexOf(": BDF,");
                        int nPCF = sTypes.IndexOf(": PCF,");
                        int nPFR = sTypes.IndexOf(": PFR,");
                        int nWFNT = sTypes.IndexOf(": Windows FNT,");

                        string sSort = "";
                        // теперь 8 вариантов имени папки
                        if (nType2 != -1 || nType1Cid != -1 || nBDF != -1 || nPCF != -1 || nPFR != -1 || nWFNT != -1)
                        {
                            sSort = "other";
                        }
                        else if (-1 != nTT && -1 != nCFF && -1 != nType1)
                        {
                            sSort = "tt_cff_type1";
                        }
                        else if (-1 != nTT && -1 != nCFF)
                        {
                            sSort = "tt_cff";
                        }
                        else if (-1 != nTT && -1 != nType1)
                        {
                            sSort = "tt_type1";
                        }
                        else if (-1 != nCFF && -1 != nType1)
                        {
                            sSort = "cff_type1";
                        }
                        else if (-1 != nTT)
                        {
                            sSort = "tt";
                        }
                        else if (-1 != nCFF)
                        {
                            sSort = "cff";
                        }
                        else if (-1 != nType1)
                        {
                            sSort = "type1";
                        }
                        else
                        {
                            sSort = "other";
                        }

                        string sFolderN = sFileName;
                        string sFolderD = strDstFolder + "\\" + sSort;

                        if (!Directory.Exists(sFolderD))
                        {
                            Directory.CreateDirectory(sFolderD);
                        }

                        string strFD = sFolderD + "\\" + sFolderN;

                        while (true)
                        {
                            if (!Directory.Exists(strFD))
                                break;
                            strFD += "1";
                        }

                        DirectoryInfo _dInf2 = new DirectoryInfo(strdst[0]);
                        new FileIOPermission(FileIOPermissionAccess.Write | FileIOPermissionAccess.Read, strdst[0]).Demand();
                        _dInf2.MoveTo(strFD);

                        /*
                        FileIOPermission fiopTemp = new FileIOPermission(PermissionState.Unrestricted);
                        fiopTemp.Assert();
                        fiopTemp.AddPathList(FileIOPermissionAccess.AllAccess, "C:\\");
                        fiopTemp.AllFiles = FileIOPermissionAccess.AllAccess;
                         * */
                    }

                    if (Directory.Exists(strTemp))
                    {
                        DirectoryInfo _dInf = new DirectoryInfo(strTemp);
                        System.Security.AccessControl.DirectorySecurity _dsec = _dInf.GetAccessControl();
                        _dInf.Delete(true);
                        _dInf.Create(_dsec);
                    }
                }
                catch 
                {
                    continue;
                }
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (backgroundWorker1.IsBusy != true)
            {
                // Start the asynchronous operation.
                backgroundWorker1.RunWorkerAsync();
            }            
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar2.Maximum = Math.Max(1, m_files.Length);
            progressBar2.Value = m_cur_file + 1;
            label1.Text = m_files[m_cur_file];
            this.Refresh();
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            label1.Text = "Convertation Progress";
            progressBar1.Value = 0;
            progressBar2.Value = 0;
            this.Refresh();
        }
    }
}
