using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Xml.Schema;
using System.Xml.Serialization;




namespace AVSImageRawSimpleConverter
{
    
    

    public partial class Form1 : Form
    {

        public int m_PreviewWidth,m_PreviewHeight;

        private bool m_bEnabledThumb = false;


        public const int BrightMul = 50;

        private bool DoFillForm = false;

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

        private void DrawPreview()
        {
            object piUncFrame;
            m_ImageRaw.GetPreviewImage(out piUncFrame);

            AVSMediaCore3.IAVSUncompressedVideoFrame unkFrame = piUncFrame as AVSMediaCore3.IAVSUncompressedVideoFrame;
            if (unkFrame != null)
            {
                axImageView31.BeginInit();
                axImageView31.Visible = false;
                               
                groupBox_Im.Top = 0;
                groupBox_Im.Left = 0;
                axImageView31.Top = 0;
                axImageView31.Left = 0;

                axImageView31.Width = 1;
                axImageView31.Height = 1;


                groupBox_Im.Width = m_PreviewWidth = unkFrame.Width;
                groupBox_Im.Height = m_PreviewHeight = unkFrame.Height;

                axImageView31.Width = unkFrame.Width;
                axImageView31.Height = unkFrame.Height;


                axImageView31.SetMediaData(piUncFrame);
                axImageView31.Width = unkFrame.Width;
                axImageView31.Height = unkFrame.Height;
                axImageView31.viewZoom = 1.0;
                axImageView31.viewCenterX = unkFrame.Width / 2;
                axImageView31.viewCenterY = unkFrame.Height / 2;
                axImageView31.Visible = true;
                axImageView31.EndInit();
            }


            //axImageView31.ShrinkToFit();
            textBox_Zoom.Text = axImageView31.viewZoom.ToString("F2");
        
        }

        private void button_Open_Click(object sender, EventArgs e)
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

            string OptionsXML;
            m_ImageRaw.GetXML(out OptionsXML,0);

            try
            {
                ApplyXMLToForm(OptionsXML);
            }
            catch (System.Exception exc)
            {
                MessageBox.Show(exc.Message, "Error reading XML");
                return;
            }
            catch
            {
                MessageBox.Show("Unexpected error", "Error reading XML");
                return;
            }

            try
            {
                DrawPreview();
            }
            catch (COMException exc)
            {
                MessageBox.Show("Error: 0x" + exc.ErrorCode.ToString("X") + "\n" + exc.Message, "Getting preview error");
                return;
            }
        }


        private void Form1_Resize(object sender, EventArgs e)
        {
            //axImageView31.ShrinkToFit();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            try
            {
                object piUncFrame;
                m_ImageRaw.GetThumbnailImage(out piUncFrame);
                AVSMediaCore3.IAVSUncompressedVideoFrame unkFrame = piUncFrame as AVSMediaCore3.IAVSUncompressedVideoFrame;
                if (unkFrame != null)
                {
                    axImageView31.Visible = false;
                    axImageView31.Top = 0;
                    axImageView31.Left = 0;

                    groupBox_Im.Top = 0;
                    groupBox_Im.Left = 0;
                    groupBox_Im.Width = m_PreviewWidth = unkFrame.Width;
                    groupBox_Im.Height = m_PreviewHeight = unkFrame.Height;

                    axImageView31.Width = unkFrame.Width;
                    axImageView31.Height = unkFrame.Height;

                    axImageView31.SetMediaData(piUncFrame);
                    axImageView31.viewZoom = 1.0;
                    axImageView31.viewCenterX = unkFrame.Width / 2;
                    axImageView31.viewCenterY = unkFrame.Height / 2;
                    axImageView31.Visible = true;
                    m_bEnabledThumb = true;
                }
                textBox_Zoom.Text = axImageView31.viewZoom.ToString("F2");
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
                textBox_Zoom.Text = axImageView31.viewZoom.ToString("F2");
            }
            catch (COMException exc)
            {
                MessageBox.Show("Error: 0x" + exc.ErrorCode.ToString("X") + "\n" + exc.Message, "Generate preview error");
                return;
            }
        }

        private void UpdateRAW()
        {
            Application.UseWaitCursor = true;
            
            if (m_bEnabledThumb)
                DrawPreview();

            
            string xmlstr;
            m_ImageRaw.GetXML(out xmlstr, 0);
            ApplyFormToXML(ref xmlstr);
            m_ImageRaw.ApplyXML(xmlstr,0);

            try
            {
                object piUncFrame;
                m_ImageRaw.GetPreviewImage(out piUncFrame);
                axImageView31.SetMediaData(piUncFrame);
                
                m_ImageRaw.GetXML(out xmlstr, 0);
                ApplyXMLToForm(xmlstr);

            }
            catch (COMException exc)
            {
                MessageBox.Show("Error: 0x" + exc.ErrorCode.ToString("X") + "\n" + exc.Message, "Generate preview error");
                Application.UseWaitCursor = false;
                return;
            }
            Application.UseWaitCursor = false;
        }


        private bool ApplyFormToXML(ref string xmlstr)
        {
            ImageRaw3OptionsWrapper.ImageRaw3 ImageRaw3Options = null;
            XmlSerializer ImageRaw3OptionsSerializer = new XmlSerializer(typeof(ImageRaw3OptionsWrapper.ImageRaw3));


            // deserialize XML
            using (var _StrReader = new System.IO.StringReader(xmlstr))
            {
                try
                {
                    ImageRaw3Options = ImageRaw3OptionsSerializer.Deserialize(_StrReader) as ImageRaw3OptionsWrapper.ImageRaw3;
                }
                catch (System.InvalidOperationException exc)
                {
                    // не получилось разобрать XML
                    return false;
                    ImageRaw3Options = new ImageRaw3OptionsWrapper.ImageRaw3();
                    ImageRaw3Options.Item = new ImageRaw3OptionsWrapper.SimpleModeType();
                }
            }
            
            ImageRaw3OptionsWrapper.SimpleModeType  SimpleModeOptions = null;

            
            if (ImageRaw3Options.Item.GetType() == typeof(ImageRaw3OptionsWrapper.SimpleModeType))
            {
                SimpleModeOptions = ImageRaw3Options.Item as ImageRaw3OptionsWrapper.SimpleModeType;
            }
            else
            {
                return false;
                ImageRaw3Options.Item = new ImageRaw3OptionsWrapper.SimpleModeType();
            }

            ImageRaw3Options.mode = 0;

            SimpleModeOptions.Temperature.use = radioButton_TemperatureWB.Checked;
            SimpleModeOptions.CustomWhiteBalance.use = radioButton_MulWB.Checked;
            SimpleModeOptions.CameraWhiteBalance.use = radioButton_CameraWB.Checked;
            SimpleModeOptions.AutoWhiteBalance.use = radioButton_AutoWB.Checked;
            SimpleModeOptions.PresetWhiteBalance.use = radioButton_PresetWB.Checked;

            SimpleModeOptions.Temperature.TemperatureValue.Value = trackBar_Temperature.Value;
            SimpleModeOptions.Temperature.Green.Value =  (float)(1.0*trackBar_Green.Value/100.0);
            try
            {
                SimpleModeOptions.CustomWhiteBalance.R.Value = (float)(Convert.ToDouble(textBox_RMul.Text));
                SimpleModeOptions.CustomWhiteBalance.B.Value = (float)(Convert.ToDouble(textBox_BMul.Text));
                SimpleModeOptions.CustomWhiteBalance.G1.Value = (float)(Convert.ToDouble(textBox_GMul.Text));
            }
            catch (FormatException fexc)
            {
                if (SimpleModeOptions.CustomWhiteBalance.use)
                    return false;
            }
            catch (OverflowException oexc){
                if (SimpleModeOptions.CustomWhiteBalance.use)
                    return false;
            }

            SimpleModeOptions.AverageGrayBox.use = checkBox_GrayBox.Checked;
            // todo graybox coords

            SimpleModeOptions.DarknessLevel.use = checkBox_UseBlack.Checked;
            SimpleModeOptions.DarknessLevel.Value = (uint)(trackBar_Black.Value);

            SimpleModeOptions.SaturationLevel.use = checkBox_UseSat.Checked;
            SimpleModeOptions.SaturationLevel.Value = (uint)(trackBar_Saturation.Value);

            SimpleModeOptions.DenoisingThreshold.use = checkBox_UseDenoise.Checked;
            SimpleModeOptions.DenoisingThreshold.Value = trackBar_DenoisingThreshold.Value;

            SimpleModeOptions.AdjustBrightness.use = checkBox_UseBright.Checked;

            SimpleModeOptions.AdjustBrightness.Value = (float)EnumToBright(trackBar_Brightness.Value,BrightMul);

            SimpleModeOptions.CorrectChromaticAberration.use = checkBox_CA.Checked;
            SimpleModeOptions.CorrectChromaticAberration.R.Value = (float)EnumToCA(trackBar_CARed.Value);
            SimpleModeOptions.CorrectChromaticAberration.B.Value = (float)EnumToCA(trackBar_CABlue.Value);

            
            if (radioButton_HLClip.Checked)
            {
                SimpleModeOptions.HighlightMode.Value = 0;
            }
            else if (radioButton_HLUnclip.Checked)
            {
                SimpleModeOptions.HighlightMode.Value = 1;
            }
            else if (radioButton_HLBlend.Checked)
            {
                SimpleModeOptions.HighlightMode.Value = 2;
            }
            else if (radioButton_HLReconstruct.Checked)
            {
                SimpleModeOptions.HighlightMode.Value = (uint)(numericUpDown_HLReconstruct.Value);
            }


            
            SimpleModeOptions.MedianFilterPasses.Value =(uint)(numericUpDown_MedianFilterPasses.Value);
            SimpleModeOptions.MedianFilterPasses.use = SimpleModeOptions.MedianFilterPasses.Value >0;

            StringBuilder sb = new StringBuilder();
            var _StrWriter = new System.IO.StringWriter(sb);

            ImageRaw3OptionsSerializer.Serialize(_StrWriter, ImageRaw3Options);
            xmlstr = sb.ToString();
            System.IO.File.WriteAllText("c:/test_FormToXML.xml", xmlstr);
            return true;
        }

        double EnumToBright(Int32 val, Int32 mul)
        {
            
            if (val >= 0)
                return 1.0 + 1.0 * val / mul;
            else
                return 1.0/(1.0 - 1.0 * val / mul);

        }

        Int32 BrightToEnum(double val, Int32 mul)
        {
            if (val >= 1)
                return Convert.ToInt32(1.0*mul * (val - 1.0));
            else
                return Convert.ToInt32(1.0 * mul * (1.0 - 1.0 / val)); ;
        }

        Int32 CAToEnum(double val)
        {
            return Convert.ToInt32((val - 1.0) / 0.0001);
        }

        double EnumToCA(Int32 val)
        {
            return 0.0001 * val + 1.0;
        }

        private void ApplyXMLToForm(string xmlstr)
        {
            ImageRaw3OptionsWrapper.ImageRaw3 ImageRaw3Options = null;
            XmlSerializer ImageRaw3OptionsSerializer = new XmlSerializer(typeof(ImageRaw3OptionsWrapper.ImageRaw3));

            System.IO.File.WriteAllText("c:/test_01.xml", xmlstr);

            // deserialize XML
            using (var _str = new System.IO.StringReader(xmlstr))
            {
                try
                {
                    ImageRaw3Options = ImageRaw3OptionsSerializer.Deserialize(_str) as ImageRaw3OptionsWrapper.ImageRaw3;
                }
                catch (System.InvalidOperationException exc)
                {
                    System.Diagnostics.Trace.TraceError("cannot deserialize xml:\n"+exc.Message);
                    throw new System.Exception("cannot deserialize xml:\n" + exc.Message);
                }
            }

            // simple mode
            if (0 == ImageRaw3Options.mode)
            {
                DoFillForm = true;

                ImageRaw3OptionsWrapper.SimpleModeType SimpleModeOptions = ImageRaw3Options.Item as ImageRaw3OptionsWrapper.SimpleModeType;

                try
                {
                    trackBar_Temperature.Maximum = System.Convert.ToInt32(SimpleModeOptions.Temperature.TemperatureValue.max);
                    trackBar_Temperature.Minimum = System.Convert.ToInt32(SimpleModeOptions.Temperature.TemperatureValue.min);
                    trackBar_Temperature.Value = System.Convert.ToInt32(SimpleModeOptions.Temperature.TemperatureValue.Value);
                    trackBar_Temperature_Scroll(trackBar_Temperature, null);
                }
                catch
                { 
                }

                trackBar_Temperature.SmallChange = 50;
                trackBar_Temperature.LargeChange = 100;
                
                try
                {
                    trackBar_Green.Maximum = System.Convert.ToInt32(SimpleModeOptions.Temperature.Green.max * 100.0);
                    trackBar_Green.Minimum = System.Convert.ToInt32(SimpleModeOptions.Temperature.Green.min * 100.0);
                    trackBar_Green.Value = System.Convert.ToInt32(SimpleModeOptions.Temperature.Green.Value * 100.0);
                    trackBar_Green_Scroll(trackBar_Green,null);
                }
                catch
                { 
                }
                
                trackBar_Green.SmallChange = 1;
                trackBar_Green.LargeChange = 10;

                textBox_RMul.Text = SimpleModeOptions.CustomWhiteBalance.R.Value.ToString();
                textBox_GMul.Text = SimpleModeOptions.CustomWhiteBalance.G1.Value.ToString();
                textBox_BMul.Text = SimpleModeOptions.CustomWhiteBalance.B.Value.ToString();

                if (SimpleModeOptions.Temperature.use)
                {
                    radioButton_TemperatureWB.Checked = true;
                }
                else if (SimpleModeOptions.CustomWhiteBalance.use)
                {
                    radioButton_MulWB.Checked = true;
                }
                else if (SimpleModeOptions.PresetWhiteBalance.use)
                {
                    radioButton_PresetWB.Checked = true;
                }
                else if (SimpleModeOptions.AutoWhiteBalance.use)
                {
                    radioButton_AutoWB.Checked = true;
                    checkBox_GrayBox.Checked = SimpleModeOptions.AverageGrayBox.use;
                }
                else if (SimpleModeOptions.CameraWhiteBalance.use)
                {
                    radioButton_CameraWB.Checked = true;
                }
                else
                {
                    radioButton_Default.Checked = true;
                }


                try
                {
                    trackBar_Black.Maximum = System.Convert.ToInt32(SimpleModeOptions.DarknessLevel.max);
                    trackBar_Black.Minimum = System.Convert.ToInt32(SimpleModeOptions.DarknessLevel.min);
                    trackBar_Black.Value = System.Convert.ToInt32(SimpleModeOptions.DarknessLevel.Value);
                    trackBar_Black_Scroll(trackBar_Black, null);
                }
                catch { 
                }
                
                trackBar_Black.SmallChange = 1;
                trackBar_Black.LargeChange = 5;

                try
                {
                    trackBar_Saturation.Maximum =  System.Convert.ToInt32(SimpleModeOptions.SaturationLevel.max);
                    trackBar_Saturation.Minimum =  System.Convert.ToInt32(SimpleModeOptions.SaturationLevel.min);
                    trackBar_Saturation.Value = System.Convert.ToInt32(SimpleModeOptions.SaturationLevel.Value);
                    trackBar_Saturation_Scroll(trackBar_Saturation, null);
                }
                catch
                { 
                }

                trackBar_Saturation.SmallChange = 5;
                trackBar_Saturation.LargeChange = 50;

                try
                {
                    trackBar_DenoisingThreshold.Maximum = System.Convert.ToInt32(SimpleModeOptions.DenoisingThreshold.max);
                    trackBar_DenoisingThreshold.Minimum = System.Convert.ToInt32(SimpleModeOptions.DenoisingThreshold.min);
                    trackBar_DenoisingThreshold.Value = System.Convert.ToInt32(SimpleModeOptions.DenoisingThreshold.Value);
                    trackBar_DenoisingThreshold_Scroll(trackBar_DenoisingThreshold, null);
                }
                catch { 
                }
                trackBar_DenoisingThreshold.SmallChange = 5;
                trackBar_DenoisingThreshold.LargeChange = 50;

                try
                {
                    trackBar_Brightness.Maximum = BrightToEnum(SimpleModeOptions.AdjustBrightness.max, BrightMul);
                    trackBar_Brightness.Minimum = BrightToEnum(SimpleModeOptions.AdjustBrightness.min, BrightMul);
                    trackBar_Brightness.Value = BrightToEnum(SimpleModeOptions.AdjustBrightness.Value, BrightMul);

                    trackBar_Brightness_Scroll(trackBar_Brightness, null);
                }
                catch { }

                try
                {

                    numericUpDown_MedianFilterPasses.Minimum = SimpleModeOptions.MedianFilterPasses.min;
                    numericUpDown_MedianFilterPasses.Maximum = SimpleModeOptions.MedianFilterPasses.max;
                    numericUpDown_MedianFilterPasses.Value = SimpleModeOptions.MedianFilterPasses.Value;
                }
                catch { }

                try
                {
                    trackBar_CARed.Maximum = CAToEnum(SimpleModeOptions.CorrectChromaticAberration.R.max);
                    trackBar_CARed.Minimum = CAToEnum(SimpleModeOptions.CorrectChromaticAberration.R.min);
                    trackBar_CARed.Value = CAToEnum(SimpleModeOptions.CorrectChromaticAberration.R.Value);
                    
                    trackBar_CABlue.Maximum = CAToEnum(SimpleModeOptions.CorrectChromaticAberration.B.max);
                    trackBar_CABlue.Minimum = CAToEnum(SimpleModeOptions.CorrectChromaticAberration.B.min);
                    trackBar_CABlue.Value = CAToEnum(SimpleModeOptions.CorrectChromaticAberration.B.Value);
                    
                    trackBar_CARed_Scroll(trackBar_CARed, null);
                    trackBar_CABlue_Scroll(trackBar_CABlue, null);
                }
                catch { }

                trackBar_CABlue.LargeChange = 2;
                trackBar_CABlue.SmallChange = 1;

                trackBar_CARed.LargeChange = 2;
                trackBar_CARed.SmallChange = 1;


                numericUpDown_HLReconstruct.Minimum = 3;
                numericUpDown_HLReconstruct.Maximum = SimpleModeOptions.HighlightMode.max;
                if (!SimpleModeOptions.HighlightMode.use)
                {
                    radioButton_HLClip.Checked = true;
                    radioButton_HLUnclip.Enabled = false;
                    radioButton_HLBlend.Enabled = false;
                    radioButton_HLReconstruct.Enabled = false;
                    numericUpDown_HLReconstruct.Enabled = false;
                } else
                switch (SimpleModeOptions.HighlightMode.Value)
                { 
                    case 0:
                        radioButton_HLClip.Checked = true;
                        numericUpDown_HLReconstruct.Enabled = false;
                        break;
                    case 1:
                        radioButton_HLUnclip.Checked = true;
                        numericUpDown_HLReconstruct.Enabled = false;
                        break;
                    case 2:
                        radioButton_HLBlend.Checked = true;
                        numericUpDown_HLReconstruct.Enabled = false;
                        break;
                    default:
                        radioButton_HLReconstruct.Checked = true;
                        numericUpDown_HLReconstruct.Enabled = true;
                        numericUpDown_HLReconstruct.Value = SimpleModeOptions.HighlightMode.Value;
                        break;
                } 

                DoFillForm = false;
                return;
            }

            throw new System.Exception("Unsupported XML format");
        }

        private void RenderPreview()
        { 
               
        }


        private void trackBar_Temperature_Scroll(object sender, EventArgs e)
        {
            textBox_Temperature.Text = trackBar_Temperature.Value.ToString();
            if (DoFillForm)
                return;

            UpdateRAW();
        }

        private void trackBar_Green_Scroll(object sender, EventArgs e)
        {
            textBox_Green.Text = (trackBar_Green.Value/100.0 ).ToString("F2");
            if (DoFillForm)
                return;
            UpdateRAW();
        }

        private void trackBar_Black_Scroll(object sender, EventArgs e)
        {
            textBox_Black.Text = trackBar_Black.Value.ToString();
            if (!DoFillForm && checkBox_UseBlack.Checked)
                UpdateRAW();
        }

        private void checkBox_UseBlack_CheckedChanged(object sender, EventArgs e)
        {
            if (!DoFillForm)
                UpdateRAW();
        }

        private void trackBar_Saturation_Scroll(object sender, EventArgs e)
        {
            textBox_Saturation.Text = trackBar_Saturation.Value.ToString();
            if (!DoFillForm && checkBox_UseSat.Checked)
                UpdateRAW();
        }

        private void checkBox_UseSat_CheckedChanged(object sender, EventArgs e)
        {
            if (!DoFillForm)
                UpdateRAW();
        }


        private void trackBar_DenoisingThreshold_Scroll(object sender, EventArgs e)
        {
            textBox_DenoisingThreshold.Text = trackBar_DenoisingThreshold.Value.ToString();
            if (!DoFillForm && checkBox_UseDenoise.Checked)
                UpdateRAW();
        }

        private void checkBox_UseDenoise_CheckedChanged(object sender, EventArgs e)
        {
            if (!DoFillForm)
                UpdateRAW();
        }


        private void trackBar_Brightness_Scroll(object sender, EventArgs e)
        {
            textBox_Brightness.Text = (trackBar_Brightness.Value / 10.0).ToString("F2");
            if (!DoFillForm && checkBox_UseBright.Checked)
                UpdateRAW();
        }

        private void checkBox_UseBright_CheckedChanged(object sender, EventArgs e)
        {
            if (!DoFillForm)
                UpdateRAW();
        }


        private void radioButton_Default_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_Default.Checked)
                UpdateRAW();
        }

        private void radioButton_CameraWB_CheckedChanged(object sender, EventArgs e)
        {
            if (DoFillForm)
                return;

            if (radioButton_CameraWB.Checked)
                UpdateRAW();
        }

        private void radioButton_AutoWB_CheckedChanged(object sender, EventArgs e)
        {
            if (DoFillForm)
                return;

            if (radioButton_AutoWB.Checked)
                checkBox_GrayBox.Enabled = true;
            else
                checkBox_GrayBox.Enabled = false;
            
            if (radioButton_AutoWB.Checked)
                UpdateRAW();
        }

        private void radioButton_PresetWB_CheckedChanged(object sender, EventArgs e)
        {
            if (DoFillForm)
                return;
        }

        private void numericUpDown_MedianFilterPasses_ValueChanged(object sender, EventArgs e)
        {
            if (!DoFillForm && numericUpDown_MedianFilterPasses.Value > 0)
                UpdateRAW();
        }


        private void axImageView31_Enter(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            axImageView31.Visible = false;
            axImageView31.viewZoom = axImageView31.viewZoom + 0.1;
            groupBox_Im.Width = (int)( Math.Ceiling(1.0*m_PreviewWidth * axImageView31.viewZoom));
            groupBox_Im.Height = (int)(Math.Ceiling(1.0*m_PreviewHeight * axImageView31.viewZoom));
            axImageView31.Visible = true;
            textBox_Zoom.Text = axImageView31.viewZoom.ToString("F2");
        }

        private void button5_Click(object sender, EventArgs e)
        {
            axImageView31.Visible = false;
            axImageView31.viewZoom = axImageView31.viewZoom - 0.1;
            groupBox_Im.Width = (int)(Math.Ceiling(1.0*m_PreviewWidth * axImageView31.viewZoom));
            groupBox_Im.Height = (int)(Math.Ceiling(1.0*m_PreviewHeight * axImageView31.viewZoom));
            axImageView31.Visible = true;
            textBox_Zoom.Text = axImageView31.viewZoom.ToString("F2");
        }


        private void axImageView31_MouseDownEvent(object sender, AxAVSImageView3._IImageView3Events_MouseDownEvent e)
        {

            //MessageBox.Show(m_x_center.ToString("F2") + " " + m_x_center.ToString("F2"));
        }

        private void axImageView31_MouseMoveEvent(object sender, AxAVSImageView3._IImageView3Events_MouseMoveEvent e)
        {

        }

        private void axImageView31_MouseUpEvent(object sender, AxAVSImageView3._IImageView3Events_MouseUpEvent e)
        {

        }

        private void button6_Click(object sender, EventArgs e)
        {
            axImageView31.Visible = false;
            axImageView31.viewZoom = 1.0;
            groupBox_Im.Width = (int)(Math.Ceiling(1.0 * m_PreviewWidth * axImageView31.viewZoom));
            groupBox_Im.Height = (int)(Math.Ceiling(1.0 * m_PreviewHeight * axImageView31.viewZoom));
            axImageView31.Visible = true;
            textBox_Zoom.Text = axImageView31.viewZoom.ToString("F2");
        }

        private void trackBar_CARed_Scroll(object sender, EventArgs e)
        {
            textBox_CARed.Text = trackBar_CARed.Value.ToString();
            if (!DoFillForm && checkBox_CA.Checked)
                UpdateRAW();
        }

        private void trackBar_CABlue_Scroll(object sender, EventArgs e)
        {
            textBox_CABlue.Text = trackBar_CABlue.Value.ToString();
            if (!DoFillForm && checkBox_CA.Checked)
                UpdateRAW();
        }

        private void checkBox_CA_CheckedChanged(object sender, EventArgs e)
        {
            if (!DoFillForm)
                UpdateRAW();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void groupBox_Im_Enter(object sender, EventArgs e)
        {

        }

        private void radioButton_HLClip_CheckedChanged(object sender, EventArgs e)
        {
            if (!DoFillForm)
                UpdateRAW();
        }

        private void radioButton_HLUnclip_CheckedChanged(object sender, EventArgs e)
        {
            if (!DoFillForm && radioButton_HLUnclip.Checked)
                UpdateRAW();
        }

        private void radioButton_HLBlend_CheckedChanged(object sender, EventArgs e)
        {
            if (!DoFillForm && radioButton_HLBlend.Checked)
                UpdateRAW();
        }

        private void radioButton_HLReconstruct_CheckedChanged(object sender, EventArgs e)
        {
            if (!DoFillForm && radioButton_HLReconstruct.Checked)
            {
                numericUpDown_HLReconstruct.Enabled = true;
                UpdateRAW();
            }

            if (!radioButton_HLReconstruct.Checked)
                numericUpDown_HLReconstruct.Enabled = false;
        }

        private void numericUpDown_HLReconstruct_ValueChanged(object sender, EventArgs e)
        {
            if (!DoFillForm)
                UpdateRAW();
        }

        private void radioButton_TemperatureWB_CheckedChanged(object sender, EventArgs e)
        {
            if (DoFillForm)
                return;
        }

        private void radioButton_MulWB_CheckedChanged(object sender, EventArgs e)
        {
            if (DoFillForm || !radioButton_MulWB.Checked)
                return;

            textBox_RMul_TextChanged(null, null);
        }

        private void textBox_RMul_TextChanged(object sender, EventArgs e)
        {
            if (DoFillForm || !radioButton_MulWB.Checked)
                return;

            try
            {
                Convert.ToDouble(textBox_RMul.Text);
                Convert.ToDouble(textBox_GMul.Text);
                Convert.ToDouble(textBox_BMul.Text);
               // radioButton_MulWB.Checked = true;
            }
            catch
            {
                return;
            }
            UpdateRAW();            
        }

        private void button4_Click_1(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            AVSImageRaw3.IImageRaw3Thumbnail thumb = new AVSImageRaw3.CImageRaw3Class();

            try
            {
                object piUncFrame;
                thumb.LoadThumbnail(openFileDialog1.FileName, out piUncFrame);
                AVSMediaCore3.IAVSUncompressedVideoFrame unkFrame = piUncFrame as AVSMediaCore3.IAVSUncompressedVideoFrame;
                if (unkFrame != null)
                {
                    axImageView31.Visible = false;
                    axImageView31.Top = 0;
                    axImageView31.Left = 0;

                    groupBox_Im.Top = 0;
                    groupBox_Im.Left = 0;
                    groupBox_Im.Width = m_PreviewWidth = unkFrame.Width;
                    groupBox_Im.Height = m_PreviewHeight = unkFrame.Height;

                    axImageView31.Width = unkFrame.Width;
                    axImageView31.Height = unkFrame.Height;

                    axImageView31.SetMediaData(piUncFrame);
                    axImageView31.viewZoom = 1.0;
                    axImageView31.viewCenterX = unkFrame.Width / 2;
                    axImageView31.viewCenterY = unkFrame.Height / 2;
                    axImageView31.Visible = true;
                    m_bEnabledThumb = true;
                }
                textBox_Zoom.Text = axImageView31.viewZoom.ToString("F2");
            }
            catch (COMException exc)
            {
                MessageBox.Show("Error: 0x" + exc.ErrorCode.ToString("X") + "\n" + exc.Message, "Open thumbnail error");
                return;
            }

            
        }

    }
}
