using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace AutoFilesTests
{
    public partial class Form1 : Form
    {
        AutoTester _tests = new AutoTester();

        public Form1()
        {
            InitializeComponent();
        }

        private void Run_Click(object sender, EventArgs e)
        {
            _tests._listInfo = listBox1;
            _tests._filesPath = textBox1.Text;
            _tests._resultPath = textBox2.Text;
            _tests.run();
        }

        private void Stop_Click(object sender, EventArgs e)
        {
            _tests.stop();
        }

        private void listBox1_DrawItem(object sender, DrawItemEventArgs e)
        {
            if (e.Index >= 0)
            {
                e.DrawBackground();
                Brush myBrush = Brushes.Black;

                if (e.Index < _tests._failedLoad.Count)
                {
                    if (false == _tests._failedLoad[e.Index])
                        myBrush = Brushes.Red;
                }

                e.Graphics.DrawString(((ListBox)sender).Items[e.Index].ToString(), e.Font, myBrush, e.Bounds, StringFormat.GenericDefault);
                e.DrawFocusRectangle();
            }
        }
    }
}
