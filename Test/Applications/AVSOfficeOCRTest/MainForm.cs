using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AVSOfficeOCRTest
{
    public partial class MainForm : Form
    {
        #region Members

        private object m_oImage = null;

        #endregion

        public MainForm()
        {
            InitializeComponent();

            OpenImage();
        }

        private void m_oButtonSelect_Click(object sender, EventArgs e)
        {
            OpenFileDialog oDlg = new OpenFileDialog();
            oDlg.Multiselect = false;

            oDlg.Filter += "ImageFiles|*.bmp;*.jpg;*.gif;";

            if (oDlg.ShowDialog() == DialogResult.OK)
            {
                m_oTextBFile.Text = oDlg.FileName;
                OpenImage();
            }
        }

        private void OpenImage()
        {
            if (string.IsNullOrEmpty(m_oTextBFile.Text))
            {
                MessageBox.Show("Error File Name", "Error");
                return;
            }

            //LoadImage
            string strXml = LoadImage(m_oTextBFile.Text);

            AVSImageStudio3.ImageTransforms oImageTransform = new AVSImageStudio3.ImageTransforms();
            bool bSuccess = oImageTransform.SetXml(strXml);
            if (false == bSuccess)
            {
                MessageBox.Show("Error Load Image", "Error");
                return;
            }
            bSuccess = oImageTransform.Transform();
            if (false == bSuccess)
            {
                MessageBox.Show("Error Load Image", "Error");
                return;
            }

            if (null != m_oImage)
                System.Runtime.InteropServices.Marshal.ReleaseComObject(m_oImage);
            oImageTransform.GetResult(0, out m_oImage);

            oImageTransform = null;

            m_oImageView.SetMediaData(m_oImage);
            m_oImageView.ZoomToFit();
        }

        private static string LoadImage(string strImagePath)
        {
            return "<transforms><ImageFile-LoadImage sourcepath=\"" + strImagePath + "\"></ImageFile-LoadImage><ImageTransform-TransformFlip  vertical=\"0\" backcolor=\"255\"><timeline type=\"1\" begin=\"0\" end=\"0\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\" /></ImageTransform-TransformFlip><ImageTransform-TransformFlip  vertical=\"0\" backcolor=\"255\"><timeline type=\"1\" begin=\"0\" end=\"0\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\" /></ImageTransform-TransformFlip></transforms>";
        }

        private void MainForm_Paint(object sender, PaintEventArgs e)
        {
            m_oImageView.ZoomToFit();
            m_oImageView.Refresh();
        }

        private void m_oButtonOCR_Click(object sender, EventArgs e)
        {
            if (null == m_oImage)
            {
                MessageBox.Show("No Image", "Error");
                return;
            }

            AVSOfficeUtils.COfficeOCR oOfficeOCR = new AVSOfficeUtils.COfficeOCR();
            if (m_oRadioText.Checked)
                oOfficeOCR.OutputFormat = 1;
            else if (m_oRadioXML.Checked)
                oOfficeOCR.OutputFormat = 0;

            oOfficeOCR.GrayLevel = System.Convert.ToInt32(m_oNumericGrayLevel.Value);
            oOfficeOCR.DustSize = System.Convert.ToInt32(m_oNumericDustSize.Value);
            oOfficeOCR.SpaceWidthDots = System.Convert.ToInt32(m_oNumericSpace.Value);
            oOfficeOCR.Certainty = System.Convert.ToInt32(m_oNumericCertainty.Value);

            string strResult = oOfficeOCR.Recognize(m_oImage);

            OCRView oView = new OCRView(strResult);
            oView.ShowDialog();
        }
    }
}
