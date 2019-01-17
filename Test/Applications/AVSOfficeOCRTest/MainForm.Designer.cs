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
﻿namespace AVSOfficeOCRTest
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.m_oButtonOCR = new System.Windows.Forms.Button();
            this.m_oTextBFile = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.m_oButtonSelect = new System.Windows.Forms.Button();
            this.m_oRadioText = new System.Windows.Forms.RadioButton();
            this.m_oRadioXML = new System.Windows.Forms.RadioButton();
            this.m_oImageView = new AxAVSImageView5.AxImageView5();
            this.label2 = new System.Windows.Forms.Label();
            this.m_oNumericGrayLevel = new System.Windows.Forms.NumericUpDown();
            this.m_oNumericDustSize = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.m_oNumericSpace = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.m_oNumericCertainty = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.m_oImageView)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_oNumericGrayLevel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_oNumericDustSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_oNumericSpace)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_oNumericCertainty)).BeginInit();
            this.SuspendLayout();
            // 
            // m_oButtonOCR
            // 
            this.m_oButtonOCR.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.m_oButtonOCR.Location = new System.Drawing.Point(397, 329);
            this.m_oButtonOCR.Name = "m_oButtonOCR";
            this.m_oButtonOCR.Size = new System.Drawing.Size(75, 23);
            this.m_oButtonOCR.TabIndex = 0;
            this.m_oButtonOCR.Text = "Convert";
            this.m_oButtonOCR.UseVisualStyleBackColor = true;
            this.m_oButtonOCR.Click += new System.EventHandler(this.m_oButtonOCR_Click);
            // 
            // m_oTextBFile
            // 
            this.m_oTextBFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.m_oTextBFile.Location = new System.Drawing.Point(69, 302);
            this.m_oTextBFile.Name = "m_oTextBFile";
            this.m_oTextBFile.Size = new System.Drawing.Size(322, 20);
            this.m_oTextBFile.TabIndex = 2;
            this.m_oTextBFile.Text = "E:\\GOCR_exe\\Untitled1111.bmp";
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 305);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "ImageFile";
            // 
            // m_oButtonSelect
            // 
            this.m_oButtonSelect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.m_oButtonSelect.Location = new System.Drawing.Point(397, 300);
            this.m_oButtonSelect.Name = "m_oButtonSelect";
            this.m_oButtonSelect.Size = new System.Drawing.Size(75, 23);
            this.m_oButtonSelect.TabIndex = 4;
            this.m_oButtonSelect.Text = "...";
            this.m_oButtonSelect.UseVisualStyleBackColor = true;
            this.m_oButtonSelect.Click += new System.EventHandler(this.m_oButtonSelect_Click);
            // 
            // m_oRadioText
            // 
            this.m_oRadioText.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.m_oRadioText.AutoSize = true;
            this.m_oRadioText.Location = new System.Drawing.Point(241, 332);
            this.m_oRadioText.Name = "m_oRadioText";
            this.m_oRadioText.Size = new System.Drawing.Size(46, 17);
            this.m_oRadioText.TabIndex = 5;
            this.m_oRadioText.TabStop = true;
            this.m_oRadioText.Text = "Text";
            this.m_oRadioText.UseVisualStyleBackColor = true;
            // 
            // m_oRadioXML
            // 
            this.m_oRadioXML.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.m_oRadioXML.AutoSize = true;
            this.m_oRadioXML.Checked = true;
            this.m_oRadioXML.Location = new System.Drawing.Point(293, 332);
            this.m_oRadioXML.Name = "m_oRadioXML";
            this.m_oRadioXML.Size = new System.Drawing.Size(47, 17);
            this.m_oRadioXML.TabIndex = 6;
            this.m_oRadioXML.TabStop = true;
            this.m_oRadioXML.Text = "XML";
            this.m_oRadioXML.UseVisualStyleBackColor = true;
            // 
            // m_oImageView
            // 
            this.m_oImageView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.m_oImageView.Enabled = true;
            this.m_oImageView.Location = new System.Drawing.Point(12, 12);
            this.m_oImageView.Name = "m_oImageView";
            this.m_oImageView.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("m_oImageView.OcxState")));
            this.m_oImageView.Size = new System.Drawing.Size(461, 282);
            this.m_oImageView.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(11, 334);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(58, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Gray Level";
            // 
            // m_oNumericGrayLevel
            // 
            this.m_oNumericGrayLevel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.m_oNumericGrayLevel.Location = new System.Drawing.Point(69, 332);
            this.m_oNumericGrayLevel.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.m_oNumericGrayLevel.Name = "m_oNumericGrayLevel";
            this.m_oNumericGrayLevel.Size = new System.Drawing.Size(46, 20);
            this.m_oNumericGrayLevel.TabIndex = 8;
            this.m_oNumericGrayLevel.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // m_oNumericDustSize
            // 
            this.m_oNumericDustSize.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.m_oNumericDustSize.Location = new System.Drawing.Point(179, 332);
            this.m_oNumericDustSize.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.m_oNumericDustSize.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.m_oNumericDustSize.Name = "m_oNumericDustSize";
            this.m_oNumericDustSize.Size = new System.Drawing.Size(46, 20);
            this.m_oNumericDustSize.TabIndex = 10;
            this.m_oNumericDustSize.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.m_oNumericDustSize.Value = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(121, 334);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(52, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Dust Size";
            // 
            // m_oNumericSpace
            // 
            this.m_oNumericSpace.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.m_oNumericSpace.Location = new System.Drawing.Point(69, 358);
            this.m_oNumericSpace.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.m_oNumericSpace.Name = "m_oNumericSpace";
            this.m_oNumericSpace.Size = new System.Drawing.Size(46, 20);
            this.m_oNumericSpace.TabIndex = 12;
            this.m_oNumericSpace.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(4, 360);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "Space/Dots";
            // 
            // m_oNumericCertainty
            // 
            this.m_oNumericCertainty.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.m_oNumericCertainty.Location = new System.Drawing.Point(179, 358);
            this.m_oNumericCertainty.Name = "m_oNumericCertainty";
            this.m_oNumericCertainty.Size = new System.Drawing.Size(46, 20);
            this.m_oNumericCertainty.TabIndex = 14;
            this.m_oNumericCertainty.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.m_oNumericCertainty.Value = new decimal(new int[] {
            95,
            0,
            0,
            0});
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(121, 360);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(48, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "Certainty";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(485, 386);
            this.Controls.Add(this.m_oNumericCertainty);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.m_oNumericSpace);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.m_oNumericDustSize);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.m_oNumericGrayLevel);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.m_oRadioXML);
            this.Controls.Add(this.m_oRadioText);
            this.Controls.Add(this.m_oButtonSelect);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.m_oTextBFile);
            this.Controls.Add(this.m_oImageView);
            this.Controls.Add(this.m_oButtonOCR);
            this.MinimumSize = new System.Drawing.Size(493, 420);
            this.Name = "MainForm";
            this.Text = "AVSOfficeOCR";
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.MainForm_Paint);
            ((System.ComponentModel.ISupportInitialize)(this.m_oImageView)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_oNumericGrayLevel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_oNumericDustSize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_oNumericSpace)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.m_oNumericCertainty)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button m_oButtonOCR;
        private AxAVSImageView5.AxImageView5 m_oImageView;
        private System.Windows.Forms.TextBox m_oTextBFile;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button m_oButtonSelect;
        private System.Windows.Forms.RadioButton m_oRadioText;
        private System.Windows.Forms.RadioButton m_oRadioXML;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown m_oNumericGrayLevel;
        private System.Windows.Forms.NumericUpDown m_oNumericDustSize;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown m_oNumericSpace;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown m_oNumericCertainty;
        private System.Windows.Forms.Label label5;
    }
}

