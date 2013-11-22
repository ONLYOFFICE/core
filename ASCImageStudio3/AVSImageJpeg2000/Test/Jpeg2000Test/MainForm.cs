using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

#pragma warning disable 1690

namespace Jpeg2000Test
{
    public partial class MainForm : Form
    {

        readonly AVSImageJpeg2000.CJ2kFile m_oJ2kFile = new AVSImageJpeg2000.CJ2kFile();
        readonly AVSImageFile3.ImageFile3 m_oImageFile = new AVSImageFile3.ImageFile3();
        object m_oImage = null;

        OptionsForm m_oOptionsForm = new OptionsForm();

        public MainForm()
        {
            InitializeComponent();
        }

        private void btOpen_Click(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "Jpeg2000 (*.j2k);(.jp2);(.jpx)|*.j2k;*.jp2;*.jpx|Bitmap (*.bmp)|*.bmp";
            openFileDialog1.FileName = "";

            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            string sOpenFile = openFileDialog1.FileName;

            TOpenSettings oOptions = new TOpenSettings();
            if (!m_oOptionsForm.radioBtOpenCustom.Checked)
                oOptions.DoDefault();
            else
                oOptions.Copy(m_oOptionsForm.m_oOpenSettings);
            // Составляем XML с настроками 
            string sOptions = "<Jpeg2000-Options>";
            sOptions += "<OpenOptions>";
            sOptions += "<ReduceFactor value='" + oOptions.nResolutionsCount.ToString() + "'/>";
            sOptions += "<LayersCount value='" + oOptions.nLayersCount.ToString() + "'/>";
            sOptions += "</OpenOptions>";
            sOptions += "</Jpeg2000-Options>";

            if (1 == openFileDialog1.FilterIndex)
                m_oJ2kFile.J2kToInterface(sOpenFile, out m_oImage, sOptions);
            else
                m_oImageFile.LoadImage2(sOpenFile, out m_oImage);

            axImageView31.BeginInit();
            axImageView31.Visible = false;
            axImageView31.SetMediaData(m_oImage);
            axImageView31.ZoomToFit();
            axImageView31.Visible = true;
            axImageView31.EndInit();

        }

        private void btSave_Click(object sender, EventArgs e)
        {
            if (m_oOptionsForm.radioBtSaveCustom.Checked && m_oOptionsForm.m_oSaveSettings.nFormat == 1)
                saveFileDialog1.Filter = "Jpeg2000 wrapper (*.jp2)|*.jp2";
            else
                saveFileDialog1.Filter = "Jpeg2000 (*.j2k)|*.j2k";
            saveFileDialog1.FileName = "";

            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            string sSaveFile = saveFileDialog1.FileName;

            TSaveSettings oOptions = new TSaveSettings();
            if (!m_oOptionsForm.radioBtSaveCustom.Checked)
                oOptions.DoDefault();
            else
                oOptions.Copy(m_oOptionsForm.m_oSaveSettings);
            // Составляем XML с настроками
            string sOptions = "<Jpeg2000-Options>";
            sOptions += "<SaveOptions>";
            sOptions += "<Format value='" + oOptions.nFormat.ToString() + "'/>";
            sOptions += "<Tile width='" + oOptions.nTileWidth.ToString() + "' height='" + oOptions.nTileHeight.ToString() + "' offsetx='" + oOptions.nTileOffsetX.ToString() + "' offsety='" + oOptions.nTileOffsetY.ToString() + "'/>";
            sOptions += "<Resolution levelscount='" + oOptions.nResolutionsCount.ToString() + "'/>";
            sOptions += "<PrecinctDimension>";
            sOptions += oOptions.sPrecinctValues;
            sOptions += "</PrecinctDimension>";
            sOptions += "<CodeBlockDimension width='" + oOptions.nCodeBlockWidth.ToString() + "' height='" + oOptions.nCodeBlockHeight.ToString() + "'/>";
            sOptions += "<ProgressionOrder value='" + oOptions.nProgressionOrder + "'/>";
            sOptions += "<Subsampling dx='" + oOptions.nSubsamplingX.ToString() + "' dy='" + oOptions.nSubsamplingY.ToString() + "'/>";
            sOptions += (oOptions.bSOPmarker ? "<SOPmarker/>" : "");
            sOptions += (oOptions.bEPHmarker ? "<EPHmarker/>" : "");
            sOptions += "<Comment value='" + oOptions.sComment + "'/>";
            sOptions += "<Compression lossless='" + (oOptions.bLossLess ? "1" : "0") + "'/>";
            if (oOptions.bRateDisto)
            {
                sOptions += "<Rate-Disto>";
                sOptions += oOptions.sCompressionValues;
                sOptions += "</Rate-Disto>";
            }
            else if (oOptions.bFixedQuality)
            {
                sOptions += "<Fixed-Quality>";
                sOptions += oOptions.sCompressionValues;
                sOptions += "</Fixed-Quality>";
            }
            else if (oOptions.bFixedLayer)
            {
                sOptions += "<Fixed-Layer layerscount='" + oOptions.nLayersCount.ToString() + "'>";
                sOptions += oOptions.sCompressionValues;
                sOptions += "</Fixed-Layer>";
            }
            sOptions += "</SaveOptions>";
            sOptions += "</Jpeg2000-Options>";

            m_oJ2kFile.InterfaceToJ2k(ref m_oImage, sSaveFile, sOptions);

        }

        private void btOptions_Click(object sender, EventArgs e)
        {
            // Сохраняем предыдущие настройки
            TOpenSettings oOpenSettings = new TOpenSettings();
            oOpenSettings.Copy(m_oOptionsForm.m_oOpenSettings);
            TSaveSettings oSaveSettings = new TSaveSettings();
            oSaveSettings.Copy(m_oOptionsForm.m_oSaveSettings);

            if (m_oOptionsForm.ShowDialog() == DialogResult.OK)
                return;
            else
            {
                // Восстанавливаем предыдущие настройки
                m_oOptionsForm.m_oSaveSettings.Copy(oSaveSettings);
                m_oOptionsForm.m_oOpenSettings.Copy(oOpenSettings);
            }
        }

        private void btClose_Click(object sender, EventArgs e)
        {
            axImageView31.BeginInit();
            axImageView31.Visible = false;
            axImageView31.SetMediaData(null);
            axImageView31.ZoomToFit();
            axImageView31.Visible = true;
            axImageView31.EndInit();

            m_oImage = null;
        }
    }
}
