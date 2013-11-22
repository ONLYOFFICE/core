using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Jpeg2000Test
{
    public struct TOpenSettings
    {
        public int nLayersCount;
        public int nResolutionsCount;

        public void DoDefault()
        {
            nLayersCount = 0;
            nResolutionsCount = 0;
        }
        public void Copy(TOpenSettings oOther)
        {
            nLayersCount = oOther.nLayersCount;
            nResolutionsCount = oOther.nResolutionsCount;
        }
    };
    public struct TSaveSettings
    {
        public int nFormat; // 0 - Jpeg2000, 1 - Jp2

        public int nTileWidth;
        public int nTileHeight;
        public int nTileOffsetX;
        public int nTileOffsetY;

        public int nResolutionsCount;

        public int nCodeBlockWidth;
        public int nCodeBlockHeight;

        public string sPrecinctValues;

        public int nProgressionOrder;

        public int nSubsamplingX;
        public int nSubsamplingY;

        public bool bSOPmarker;
        public bool bEPHmarker;

        public string sComment;

        public bool bLossLess;

        public bool bRateDisto;
        public bool bFixedQuality;
        public bool bFixedLayer;
        public int nLayersCount;
        public string sCompressionValues;

        public void DoDefault()
        {
            nFormat = 0;

            nTileWidth = 0;
            nTileHeight = 0;
            nTileOffsetX = 0;
            nTileOffsetY = 0;

            nResolutionsCount = 6;

            nCodeBlockHeight = 64;
            nCodeBlockWidth = 64;

            sPrecinctValues = "[32768,32768]";

            nProgressionOrder = 0;

            nSubsamplingX = 1;
            nSubsamplingY = 1;

            bSOPmarker = false;
            bEPHmarker = false;

            sComment = "Manufactured by Online Media Technologies Ltd.";

            bLossLess = false;

            bRateDisto = true;
            bFixedLayer = false;
            bFixedQuality = false;
            nLayersCount = 1;
            sCompressionValues = "";
        }
        public void Copy(TSaveSettings oOther)
        {
            nFormat = oOther.nFormat;

            nTileWidth = oOther.nTileWidth;
            nTileHeight = oOther.nTileHeight;
            nTileOffsetX = oOther.nTileOffsetX;
            nTileOffsetY = oOther.nTileOffsetY;

            nResolutionsCount = oOther.nResolutionsCount;

            nCodeBlockHeight = oOther.nCodeBlockHeight;
            nCodeBlockWidth = oOther.nCodeBlockWidth;

            sPrecinctValues = oOther.sPrecinctValues;

            nProgressionOrder = oOther.nProgressionOrder;

            nSubsamplingX = oOther.nSubsamplingX;
            nSubsamplingY = oOther.nSubsamplingY;

            bSOPmarker = oOther.bSOPmarker;
            bEPHmarker = oOther.bEPHmarker;

            sComment = oOther.sComment;

            bLossLess = oOther.bLossLess;

            bRateDisto = oOther.bRateDisto;
            bFixedQuality = oOther.bFixedQuality;
            bFixedLayer = oOther.bFixedLayer;
            nLayersCount = oOther.nLayersCount;
            sCompressionValues = oOther.sCompressionValues;
        }
    };

    public partial class OptionsForm : Form
    {
        public TOpenSettings m_oOpenSettings = new TOpenSettings();
        public TSaveSettings m_oSaveSettings = new TSaveSettings();

        bool m_bShowOpenDefault = true;
        bool m_bShowSaveDefault = true;

        public OptionsForm()
        {
            InitializeComponent();
            m_oOpenSettings.DoDefault();
            m_oSaveSettings.DoDefault();
        }

        private void radioBtOpenDefault_CheckedChanged(object sender, EventArgs e)
        {
            radioBtOpenCustom.Checked = !radioBtOpenDefault.Checked;
            groupBoxOpenSettings.Enabled = !radioBtOpenDefault.Checked;
        }

        private void radioBtOpenCustom_CheckedChanged(object sender, EventArgs e)
        {
            radioBtOpenDefault.Checked = !radioBtOpenCustom.Checked;
        }

        private void btOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void btCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowOpenDefault)
                m_oOpenSettings.nResolutionsCount = (int)numericOSResolutionsCount.Value;
        }

        private void radioBtSaveDefault_CheckedChanged(object sender, EventArgs e)
        {
            radioBtSaveCustom.Checked = !radioBtSaveDefault.Checked;
            groupBoxSaveSettings.Enabled = !radioBtSaveDefault.Checked;
        }

        private void radioBtSaveCustom_CheckedChanged(object sender, EventArgs e)
        {
            radioBtSaveDefault.Checked = !radioBtSaveCustom.Checked;
            groupBoxSaveSettings.Enabled = radioBtSaveCustom.Checked;
        }

        private void numericOSLayersCount_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowOpenDefault)
                m_oOpenSettings.nLayersCount = (int)numericOSLayersCount.Value;
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            // Open Settings
            if (groupBoxOpenSettings.Enabled)
            {
                m_bShowOpenDefault = false;
                numericOSResolutionsCount.Value = m_oOpenSettings.nResolutionsCount;
                numericOSLayersCount.Value = m_oOpenSettings.nLayersCount;
            }
            else
            {
                m_bShowOpenDefault = true;
                TOpenSettings oDefault = new TOpenSettings();
                oDefault.DoDefault();
                numericOSResolutionsCount.Value = oDefault.nResolutionsCount;
                numericOSLayersCount.Value = oDefault.nLayersCount;
            }
            // Save Settings
            if (groupBoxSaveSettings.Enabled)
            {
                m_bShowSaveDefault = false;
                radioBtFormatJp2.Checked = (m_oSaveSettings.nFormat == 1);

                numericTileHeight.Value = m_oSaveSettings.nTileHeight;
                numericTileOffsetX.Value = m_oSaveSettings.nTileOffsetX;
                numericTileOffsetY.Value = m_oSaveSettings.nTileOffsetY;
                numericTileWidth.Value = m_oSaveSettings.nTileWidth;

                numericResolutionsCount.Value = m_oSaveSettings.nResolutionsCount;

                numericCodeBlockHeight.Value = m_oSaveSettings.nCodeBlockHeight;
                numericCodeBlockWidth.Value = m_oSaveSettings.nCodeBlockWidth;

                textBoxPrecinct.Text = m_oSaveSettings.sPrecinctValues;

                domainProgressionOrder.SelectedIndex = m_oSaveSettings.nProgressionOrder;

                numericSubsamplingX.Value = m_oSaveSettings.nSubsamplingX;
                numericSubsamplingY.Value = m_oSaveSettings.nSubsamplingY;

                checkBoxEPHmarker.Checked = m_oSaveSettings.bEPHmarker;
                checkBoxSOPmarker.Checked = m_oSaveSettings.bSOPmarker;

                textBoxComment.Text = m_oSaveSettings.sComment;

                checkBoxLossLess.Checked = m_oSaveSettings.bLossLess;

                radioBtRateDisto.Checked = m_oSaveSettings.bRateDisto;
                radioBtFixedQuality.Checked = m_oSaveSettings.bFixedQuality;
                radioBtFixedLayer.Checked = m_oSaveSettings.bFixedLayer;

                numericLayersCount.Enabled = radioBtFixedLayer.Checked;
                if (!numericLayersCount.Enabled)
                    numericLayersCount.Value = 1;
                else
                    numericLayersCount.Value = m_oSaveSettings.nLayersCount;
                textBoxCompression.Text = m_oSaveSettings.sCompressionValues;
            }
            else
            {
                m_bShowSaveDefault = true;
                TSaveSettings oDefault = new TSaveSettings();
                oDefault.DoDefault();

                radioBtFormatJp2.Checked = (oDefault.nFormat == 1);

                numericTileHeight.Value = oDefault.nTileHeight;
                numericTileOffsetX.Value = oDefault.nTileOffsetX;
                numericTileOffsetY.Value = oDefault.nTileOffsetY;
                numericTileWidth.Value = oDefault.nTileWidth;

                numericResolutionsCount.Value = oDefault.nResolutionsCount;

                numericCodeBlockHeight.Value = oDefault.nCodeBlockHeight;
                numericCodeBlockWidth.Value = oDefault.nCodeBlockWidth;

                textBoxPrecinct.Text = oDefault.sPrecinctValues;

                domainProgressionOrder.SelectedIndex = oDefault.nProgressionOrder;

                numericSubsamplingX.Value = oDefault.nSubsamplingX;
                numericSubsamplingY.Value = oDefault.nSubsamplingY;

                checkBoxSOPmarker.Checked = oDefault.bSOPmarker;
                checkBoxEPHmarker.Checked = oDefault.bEPHmarker;

                textBoxComment.Text = oDefault.sComment;

                checkBoxLossLess.Checked = oDefault.bLossLess;

                radioBtFixedLayer.Checked = oDefault.bFixedLayer;
                radioBtFixedQuality.Checked = oDefault.bFixedQuality;
                radioBtRateDisto.Checked = oDefault.bRateDisto;
                numericLayersCount.Value = oDefault.nLayersCount;
                textBoxCompression.Text = oDefault.sCompressionValues;
            }
        }

        private void radioBtFormatJpeg2000_CheckedChanged(object sender, EventArgs e)
        {
            radioBtFormatJp2.Checked = !radioBtFormatJpeg2000.Checked;
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nFormat = radioBtFormatJp2.Checked ? 1 : 0;
        }

        private void radioBtFormatJp2_CheckedChanged(object sender, EventArgs e)
        {
            radioBtFormatJpeg2000.Checked = !radioBtFormatJp2.Checked;
        }

        private void numericTileWidth_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nTileWidth = (int)numericTileWidth.Value;
        }

        private void numericTileHeight_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nTileHeight = (int)numericTileHeight.Value;
        }

        private void numericTileOffsetX_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nTileOffsetX = (int)numericTileOffsetX.Value;
        }

        private void numericTileOffsetY_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nTileOffsetY = (int)numericTileOffsetY.Value;
        }

        private void numericResolutionsCount_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nResolutionsCount = (int)numericResolutionsCount.Value;
        }

        private void numericCodeBlockWidth_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nCodeBlockWidth = (int)numericCodeBlockWidth.Value;
        }

        private void numericCodeBlockHeight_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nCodeBlockHeight = (int)numericCodeBlockHeight.Value;
        }

        private void textBoxPrecinct_TextChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.sPrecinctValues = textBoxPrecinct.Text;
        }

        private void domainProgressionOrder_SelectedItemChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nProgressionOrder = domainProgressionOrder.SelectedIndex;
        }

        private void numericSubsamplingX_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nSubsamplingX = (int)numericSubsamplingX.Value;
        }

        private void numericSubsamplingY_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nSubsamplingY = (int)numericSubsamplingY.Value;
        }

        private void checkBoxSOPmarker_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.bSOPmarker = checkBoxSOPmarker.Checked;
        }

        private void checkBoxEPHmarker_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.bEPHmarker = checkBoxEPHmarker.Checked;
        }

        private void textBoxComment_TextChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.sComment = textBoxComment.Text;
        }

        private void checkBoxLossLess_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.bLossLess = checkBoxLossLess.Checked;
        }

        private void radioBtRateDisto_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtRateDisto.Checked)
            {
                radioBtFixedLayer.Checked = false;
                radioBtFixedQuality.Checked = false;
            }
            if (!m_bShowSaveDefault)
                m_oSaveSettings.bRateDisto = radioBtRateDisto.Checked;
        }

        private void radioBtFixedQuality_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtFixedQuality.Checked)
            {
                radioBtFixedLayer.Checked = false;
                radioBtRateDisto.Checked = false;
            }
            if (!m_bShowSaveDefault)
                m_oSaveSettings.bFixedQuality = radioBtFixedQuality.Checked;
        }

        private void radioBtFixedLayer_CheckedChanged(object sender, EventArgs e)
        {
            if (radioBtFixedLayer.Checked)
            {
                radioBtFixedQuality.Checked = false;
                radioBtRateDisto.Checked = false;
            }
            if (!m_bShowSaveDefault)
                m_oSaveSettings.bFixedLayer = radioBtFixedLayer.Checked;
        }

        private void numericLayersCount_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.nLayersCount = (int)numericLayersCount.Value;
        }

        private void textBoxCompression_TextChanged(object sender, EventArgs e)
        {
            if (!m_bShowSaveDefault)
                m_oSaveSettings.sCompressionValues = textBoxCompression.Text;
        }

    }
}
