using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PDFTest
{
    public partial class ConvertingForm : Form
    {

        public bool m_bCancelConverting = false;

        public ConvertingForm()
        {
            InitializeComponent();
            this.progressBar.Minimum = 0;
            this.progressBar.Maximum = 100;
            this.progressBar.Step = 1;
            this.btFinish.Text = "ОК";
            m_bCancelConverting = false;
            this.progressBar.Style = ProgressBarStyle.Marquee;
        }

        private void ConvertingForm_Load(object sender, EventArgs e)
        {
            this.progressBar.Minimum = 0;
            this.progressBar.Maximum = 100;
            this.progressBar.Step = 1;
            m_bCancelConverting = false;
            this.btFinish.Text = "ОК";
            this.progressBar.Style = ProgressBarStyle.Marquee;
            
        }

        private void btCancel_Click(object sender, EventArgs e)
        {
            m_bCancelConverting = true;
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            //progressBar.Value += 1;
            //if (progressBar.Value >= progressBar.Maximum)
            //{
            //    progressBar.Value = progressBar.Minimum;
            //    return;
            //}
        }

        private void btFinish_Click(object sender, EventArgs e)
        {
            this.Hide();
        }
    }
}
