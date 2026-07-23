/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
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
