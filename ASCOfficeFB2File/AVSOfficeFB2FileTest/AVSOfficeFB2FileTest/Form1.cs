using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AVSOfficeFB2FileTest
{
    public partial class Form1 : Form
    {
        private AVSOfficeFB2File.CFB2File m_fb2File = new AVSOfficeFB2File.CFB2File();
        
        public Form1()
        {
            InitializeComponent();            
        }

        private void click_input(object sender, EventArgs e)
        {
            if (DialogResult.OK != openFileDialog1.ShowDialog())
                return;

            string filename = openFileDialog1.FileName;
            input_textbox.Text = filename;

            if (filename.IndexOf(".fb2") != -1)
            {
                output_textbox.Text = filename.Replace(".fb2", ".docx");
            }
        }

        private void click_output(object sender, EventArgs e)
        {
            if (DialogResult.OK != saveFileDialog1.ShowDialog())
                return;

            output_textbox.Text = saveFileDialog1.FileName;
        }

        private void click_convert(object sender, EventArgs e)
        {
            if (input_textbox.Text.Length == 0)
            {
                MessageBox.Show("Choose input file.");
                return;
            }
            if (output_textbox.Text.Length == 0 || input_textbox.Text.Length == 0)
            {
                MessageBox.Show("Choose output file.");
                return;
            }
            m_fb2File.LoadFromFile(input_textbox.Text, output_textbox.Text, "");
        }
    }
}
