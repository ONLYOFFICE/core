using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace TestImageStudioMdi
{
    public partial class FormNavigation : Form
    {
        #region Events

        public FormNavigation()
        {
            InitializeComponent();

            m_oImageTransforms.SetAdditionalParam("OldPhotoMasksFolder", "X:\\AVSImageStudio3\\AVSImageStudio3\\Resources\\");
        }

        private void buttonBrowse_Click_1(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            if (!LoadImage(openFileDialog1.FileName, 0))
                textBoxPath1.Text = "";
            else
                textBoxPath1.Text = openFileDialog1.FileName;
        }

        private void buttonBrowse2_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            if (!LoadImage(openFileDialog1.FileName, 1))
                textBoxPath2.Text = "";
            else
                textBoxPath2.Text = openFileDialog1.FileName;
        }

        private void buttonBrowse3_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            if (!LoadImage(openFileDialog1.FileName, 2))
                textBoxPath3.Text = "";
            else
                textBoxPath3.Text = openFileDialog1.FileName;
        }

        private void buttonRun_Click(object sender, EventArgs e)
        {
            if (SourceImage0 == null)
                return;

            if (buttonRun.Text == "Run")
            {
                numericCurrent.Value = numericStartTime.Value;

                buttonRun.Text = "Stop";
            }
            else
                buttonRun.Text = "Run";
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (buttonRun.Text == "Run")
                return;

            m_oImageTransforms.SetXml(Main.TransformXml);

            numericCurrent.Value = numericCurrent.Value += numericStep.Value;

            if (numericCurrent.Value > numericEndTime.Value)
                buttonRun.Text = "Run";
        }

        private void numericCurrent_ValueChanged(object sender, EventArgs e)
        {
            m_oImageTransforms.SetTime((int)numericCurrent.Value);

            if (SourceImage0 != null) m_oImageTransforms.SetSource(0, SourceImage0);
            if (SourceImage1 != null) m_oImageTransforms.SetSource(1, SourceImage1);
            if (SourceImage2 != null) m_oImageTransforms.SetSource(2, SourceImage2);

            if (m_oImageTransforms.Transform())
            {
                object oImageResult = null;

                m_oImageTransforms.GetResult(0, out oImageResult);

                if (checkBoxUpdatePreview.Checked)
                    Main.SetPreview(oImageResult);

                DestroyImage(oImageResult);
            }
        }

        #endregion 

        #region Methods

        private bool LoadImage(string strPath, int nIndex)
        {
            object oImage = null;

            string sXml = "";

            sXml += "<transforms>\r\n";
            sXml += "  <ImageFile-LoadImage sourcepath='" + strPath + "'/>\r\n";
            sXml += "</transforms>\r\n";

            if (!m_oImageTransforms.SetXml(sXml))
                return false;

            if( m_oImageTransforms.Transform() )
                m_oImageTransforms.GetResult(0, out oImage);
                        
            if (oImage == null)
                return false;

            if (nIndex == 0) SourceImage0 = oImage;
            if (nIndex == 1) SourceImage1 = oImage;
            if (nIndex == 2) SourceImage2 = oImage;

            return true;
        }

        private void DestroyImage(object oImage)
        {
            if (null != oImage)
                System.Runtime.InteropServices.Marshal.ReleaseComObject(oImage);
        }

        #endregion

        #region Members

        private AVSImageStudio3.ImageTransformsClass m_oImageTransforms = new AVSImageStudio3.ImageTransformsClass();

        private FormMain m_oParent = null;
        private object m_oImageSource0 = null;
        private object m_oImageSource1 = null;
        private object m_oImageSource2 = null;

        #endregion

        #region Properties

        public FormMain Main
        {
            get { return (m_oParent as FormMain); }
            set { m_oParent = value; }
        }

        public object SourceImage0
        {
            get { return m_oImageSource0; }
            set
            {
                DestroyImage(m_oImageSource0);

                m_oImageSource0 = value;
            }
        }
        public object SourceImage1
        {
            get { return m_oImageSource1; }
            set
            {
                DestroyImage(m_oImageSource1);

                m_oImageSource1 = value;
            }
        }
        public object SourceImage2
        {
            get { return m_oImageSource2; }
            set
            {
                DestroyImage(m_oImageSource2);

                m_oImageSource2 = value;
            }
        }

        #endregion

        private void numericEndTime_ValueChanged(object sender, EventArgs e)
        {

        }
    }
}