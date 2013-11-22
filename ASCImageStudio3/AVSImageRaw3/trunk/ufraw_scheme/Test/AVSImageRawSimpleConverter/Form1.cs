using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace AVSImageRawSimpleConverter
{
    public partial class Form1 : Form
    {

        readonly    AVSImageRaw3.CImageRaw3Class m_ImageRaw = new AVSImageRaw3.CImageRaw3Class(); 
        AVSImageRaw3.IImageRaw3 m_IImageRaw = new AVSImageRaw3.CImageRaw3Class();
        
        public Form1()
        {
            InitializeComponent();
        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            try
            {
                m_ImageRaw.OpenFile(openFileDialog1.FileName);
            }
            catch (COMException exc)
            {
                MessageBox.Show("Error: 0x" + exc.ErrorCode.ToString("X") + "\n" + exc.Message, "Open file error");
                return;
            }
            catch (System.OutOfMemoryException exc)
            {
                MessageBox.Show("Out of memory:\n"+exc.Message, "Open file error");
                return;
            }

           /* try
            {
                object piUncFrame;
                m_ImageRaw.RenderImage(out piUncFrame);
                axImageView31.SetMediaData(piUncFrame);
                axImageView31.ShrinkToFit();
 
            }
            catch (COMException exc)
            {
                MessageBox.Show("Error: 0x" + exc.ErrorCode.ToString("X") + "\n" + exc.Message, "Render error");
                return;
            }*/

        }

        private void Form1_ResizeEnd(object sender, EventArgs e)
        {
            axImageView31.ShrinkToFit();
        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            axImageView31.ShrinkToFit();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            try
            {
                object piUncFrame;
                m_ImageRaw.GetThumbnailImage(out piUncFrame);
                axImageView31.SetMediaData(piUncFrame);
                axImageView31.ShrinkToFit();
                
            }
            catch (COMException exc)
            {
                MessageBox.Show("Error: 0x" + exc.ErrorCode.ToString("X") + "\n" + exc.Message, "Open thumbnail error");
                return;
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            try
            {
                object piUncFrame;
                m_ImageRaw.GetPreviewImage(out piUncFrame);
                axImageView31.SetMediaData(piUncFrame);
                axImageView31.ShrinkToFit();
            }
            catch (COMException exc)
            {
                MessageBox.Show("Error: 0x" + exc.ErrorCode.ToString("X") + "\n" + exc.Message, "Generate preview error");
                return;
            }
        }
    }
}
