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
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Windows;
using System.Diagnostics;

namespace TeamlabDocControlTest
{
    public partial class Form1 : Form
    {

        BackgroundWorker worker;
        private Socket server = null;
        volatile string _receivedText = "";
        volatile string _sendText = "";

        public Form1()
        {
            InitializeComponent();
            worker = new BackgroundWorker();
            worker.WorkerReportsProgress = true;
            worker.DoWork += new DoWorkEventHandler(worker_DoWork);
            worker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(worker_RunWorkerCompleted);
            worker.ProgressChanged += new ProgressChangedEventHandler(worker_ProgressChanged);

            comboBox1.SelectedIndex = 0;
        }

        private void worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        { 
        }
        private void worker_RunWorkerCompleted (object sender, RunWorkerCompletedEventArgs e)
        {
            textBox4.Text = _receivedText;
        }
        void worker_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;
            worker.ReportProgress (0);

            if (_sendText.Length > 0)
            {                
                send(_sendText);
                _sendText = "";
                worker.ReportProgress(50);
            }
            _receivedText = receive(server);
            Trace.TraceInformation(_receivedText);
            worker.ReportProgress(100);
        }

        // disconnect
        private void button1_Click(object sender, EventArgs e)
        {
            if (null != server && server.Connected)
            {
                server.Shutdown (SocketShutdown.Both);
                server.Close();
            }
        }

        // connect
        private void button2_Click(object sender, EventArgs e)
        {
            Application.DoEvents();
            try
            {
                IPEndPoint ipep = new IPEndPoint(IPAddress.Parse(textBox2.Text), Int32.Parse(textBox1.Text));
                server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                server.Connect(ipep);
            }
            catch (SocketException e1)
            {
                Console.WriteLine();
                MessageBox.Show(e1.Message, "error");
                return;
            }
            catch (Exception e2)
            {
                MessageBox.Show(e2.Message, "error");
                return;
            }

            Application.DoEvents();
            worker.RunWorkerAsync();
            Application.DoEvents();
        }

        private string receive (Socket server)
        {
            string recvStr = "";
            byte[] data = new byte [1024];
            try
            {
                while (true)
                {
                    int recv = server.Receive(data);
                    if (recv > 0)
                    {
                        recvStr += Encoding.UTF8.GetString(data, 0, recv);
                    }
                    else 
                        throw new Exception("Receive: available bytes more than was read");
                    
                    if (0 == server.Available)
                        break;
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "error");
            }
            return recvStr;
        }

        bool send (string msg)
        {
            bool res = true;
            try
            {
                server.Send(Encoding.UTF8.GetBytes(msg));
            }
            catch (Exception err)
            {
                res = false;
                MessageBox.Show(err.Message, "error");
            }
            return res;
        }

        // send/receive
        private void button3_Click(object sender, EventArgs e)
        {
            if (null == server)
                return;

            if (worker.IsBusy)
                return;

            Application.DoEvents();
            _sendText = textBox3.Text;

            Application.DoEvents();
            

            worker.RunWorkerAsync();
            Application.DoEvents();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (null != server && server.Connected)
            {
                server.Shutdown(SocketShutdown.Both);
                server.Close();
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            int index = comboBox1.SelectedIndex;
            switch (index)
            {
                case 0:
                    textBox3.Text = "<tdcs><type>status</type><id>id12345</id><params></params></tdcs>";
                    break;
                case 1:
                    textBox3.Text = "<tdcs><type>memory-monitor</type><id>id23456</id><params><start-time/></params></tdcs>";
                    break;
                case 2:
                    textBox3.Text = "<tdcs><type>processor-monitor</type><id>id34567</id><params></params></tdcs>";
                    break;
                case 3:
                    textBox3.Text = "<tdcs><type>apppool-monitor</type><id>id34567</id><params><url>http://www.avs4you.com</url></params></tdcs>";
                    break;
                case 4:
                    textBox3.Text = "<tdcs><type>clearfolder</type><id>id34567</id><params><folder>c:\\1</folder></params></tdcs>";
                    break;
            }
        }
    }
}