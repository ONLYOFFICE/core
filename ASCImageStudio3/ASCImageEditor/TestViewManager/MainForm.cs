using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace TestViewManager
{
    public partial class MainForm : Form
    {
        #region constants

        private const long nRullerTypePixels = 0;
        private const long nRullerTypeCentimeters = 1;
        private const long nRullerTypeInches = 2;

        #endregion

        #region members

        private bool m_bUpdateZoomToWindow = false;
        private bool m_bUpdateCenterX = false;
        private int m_nHScrollNewValue = 0;

        private double m_dVSCrollMin = 0.0;
        private double m_dVSCrollMax = 0.0;
        private double m_dVLargeChange = 0.0;

        private double m_dHSCrollMin = 0.0;
        private double m_dHSCrollMax = 0.0;
        private double m_dHLargeChange = 0.0;

        AVSImageEditor.ICommandParam m_size_param;
        AVSImageEditor.ICommandParam m_type_param;

        #endregion

        #region MainForm

        public MainForm()
        {
            InitializeComponent();

            SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint, true);

            m_oViewSettings = (AVSImageEditor.IViewSettings)m_oImageEditor.GetViewSettings();
            m_oViewSettings.SetProperty("BackClearColor", (Int64)0xFF0000FF);
            m_oUndoStack = (AVSImageEditor.CHistoryStack)m_oImageEditor.GetUndoStack();
            m_oRedoStack = (AVSImageEditor.CHistoryStack)m_oImageEditor.GetRedoStack();

            m_oUndoStack.Increase += new AVSImageEditor._IHistoryStackEvents_IncreaseEventHandler(IncreaseUndoStack);
            m_oUndoStack.Decrease += new AVSImageEditor._IHistoryStackEvents_DecreaseEventHandler(DecreaseUndoStack);

            m_oRedoStack.Increase += new AVSImageEditor._IHistoryStackEvents_IncreaseEventHandler(IncreaseRedoStack);
            m_oRedoStack.Decrease += new AVSImageEditor._IHistoryStackEvents_DecreaseEventHandler(DecreaseRedoStack);

            int undDpth = m_oUndoStack.GetDepth();

            UndoListBox.BeginUpdate();
            for (int i = 0; i < undDpth; i++)
                UndoListBox.Items.Add(m_oUndoStack.GetElementDescription(i));
            UndoListBox.EndUpdate();


            int redoDpth = m_oRedoStack.GetDepth();

            RedoListBox.BeginUpdate();
            for (int i = 0; i < redoDpth; i++)
                RedoListBox.Items.Add(m_oRedoStack.GetElementDescription(i));
            RedoListBox.EndUpdate();

            CommandChoise_comboBox.SelectedIndex = 0;
            controlXMLProperties1.Visible = false;
            controlToneProperties1.Visible = false;
            controlLevelsProperties1.Visible = false;
            controlCropProperties1.Visible = false;
            controlBrushProperties1.Visible = false;
            controlBrightnessContrastProperties1.Visible = false;
        }

        private void IncreaseUndoStack(int s)
        {
            //  This is very stupid way - dont use this
            UndoListBox.BeginUpdate();
            UndoListBox.Items.Clear();
            int undDpth = m_oUndoStack.GetDepth();
            for (int i = 0; i < undDpth; i++)
                UndoListBox.Items.Add(m_oUndoStack.GetElementDescription(i));
            UndoListBox.EndUpdate();

        }

        private void DecreaseUndoStack(int s)
        {
            //  This is very stupid way - dont use this
            UndoListBox.BeginUpdate();
            UndoListBox.Items.Clear();
            int undDpth = m_oUndoStack.GetDepth();
            for (int i = 0; i < undDpth; i++)
                UndoListBox.Items.Add(m_oUndoStack.GetElementDescription(i));
            UndoListBox.EndUpdate();

        }

        private void IncreaseRedoStack(int s)
        {
            //  This is very stupid way - dont use this
            RedoListBox.BeginUpdate();
            RedoListBox.Items.Clear();
            int redDpth = m_oRedoStack.GetDepth();
            for (int i = 0; i < redDpth; i++)
                RedoListBox.Items.Add(m_oRedoStack.GetElementDescription(i));
            RedoListBox.EndUpdate();

        }

        private void DecreaseRedoStack(int s)
        {
            //  This is very stupid way - dont use this
            RedoListBox.BeginUpdate();
            RedoListBox.Items.Clear();
            int redDpth = m_oRedoStack.GetDepth();
            for (int i = 0; i < redDpth; i++)
                RedoListBox.Items.Add(m_oRedoStack.GetElementDescription(i));
            RedoListBox.EndUpdate();
        }

        private void MainForm_Shown(object sender, EventArgs e)
        {
            m_oImageEditor.Open("C:\\ImageTestFiles\\1920_1280.bmp");

            // m_oImageEditor.Open("C:\\12.bmp");

            // m_oImageEditor.Refresh();

            m_oZoomItems.SelectedItem = "100%";
            m_oComboBoxRullersType.SelectedItem = "Pixels";

            //m_oViewSettings.SetProperty("Zoom", (double)40);
            //m_oViewSettings.SetProperty("CenterX", (double)0);
            //m_oViewSettings.SetProperty("CenterY", (double)0);

            RullersButton_Click(null, null);

            EnabledScrollBars();

            double CenterX = (double)m_oViewSettings.GetProperty("CenterX");
            double CenterY = (double)m_oViewSettings.GetProperty("CenterY");

            if (m_hScrollBar.Enabled)
                if (m_hScrollBar.Maximum > (int)CenterX)
                    m_hScrollBar.Value = (int)CenterX;

            if (m_vScrollBar.Enabled)
                if (m_vScrollBar.Maximum > (int)CenterY)
                    m_vScrollBar.Value = (int)CenterY;

            //  m_oViewSettings.Refresh();

            m_oControlSize.Text = String.Format("ImageEditorControlSize : [ {0} : {1} ]", m_oImageEditor.Width, m_oImageEditor.Height);
        }
        private void MainForm_SizeChanged(object sender, EventArgs e)
        {
            if (null != m_oZoomItems.SelectedItem)
            {
                if ("Window" == (string)m_oZoomItems.SelectedItem)
                {
                    return;
                }
            }
            EnabledScrollBars();

            m_oControlSize.Text = String.Format("ImageEditorControlSize : [ {0} : {1} ]", m_oImageEditor.Width, m_oImageEditor.Height);
        }
        private void MainForm_Resize(object sender, EventArgs e)
        {
            if (m_oViewSettings.ZoomToWindow)
            {
                double Zoom = (double)m_oViewSettings.GetProperty("Zoom");
                if (Zoom > 0.0)
                {
                    m_oZoomTackBar.Value = (int)(Zoom * 500.0);
                    m_oZoomStatusLabel.Text = "Zoom : " + Zoom * 100.0 + "%";
                    m_oZoomItems.SelectedItem = "Window";
                }

                return;
            }

            if (null != m_oZoomItems.SelectedItem)
            {
                if ("Window" == (string)m_oZoomItems.SelectedItem)
                {
                    return;
                    // if (false == m_oViewSettings.ZoomToWindow)
                    //    ZoomToWindow_Click(null, null);
                }
            }
            m_oViewSettings.Refresh();
            m_oImageEditor.Refresh();
            EnabledScrollBars();

            m_oControlSize.Text = String.Format("ImageEditorControlSize : [ {0} : {1} ]", m_oImageEditor.Width, m_oImageEditor.Height);

            // Trace.WriteLine(String.Format("Размеры окна W : {0}, H : {1}", Width, Height));
        }

        #endregion

        #region ZoomOperations

        private void ZoomTackBar_Scroll(object sender, EventArgs e)
        {
            if (m_bUpdateZoomToWindow)
                return;

            double Zoom = (double)m_oZoomTackBar.Value / 500.0;
            //if (Zoom > 1.0)
            //    Zoom = Math.Round(Zoom, 0);

            m_oViewSettings.SetProperty("Zoom", Zoom);

            m_oZoomStatusLabel.Text = "Zoom : " + (double)m_oViewSettings.GetProperty("Zoom") * 100.0 + "%";

            EnabledScrollBars();

            m_oViewSettings.Refresh();
        }
        private void ZoomToWindow_Click(object sender, EventArgs e)
        {
            if (m_oZoomToWindow.Checked)
            {
                m_oZoomToWindow.Checked = false;
                m_oViewSettings.ZoomToWindow = false;
                m_oZoomTackBar.Enabled = true;
            }
            else
            {
                m_oZoomToWindow.Checked = true;
                m_oViewSettings.ZoomToWindow = true;
                m_oZoomTackBar.Enabled = false;
            }

            EnabledScrollBars();

            m_oZoomTackBar.Value = (int)((double)m_oViewSettings.GetProperty("Zoom") * 500.0);
            m_oZoomStatusLabel.Text = "Zoom : " + (double)m_oViewSettings.GetProperty("Zoom") * 100.0 + "%";
            m_oZoomItems.SelectedItem = "Window";

            m_oViewSettings.Refresh();
        }

        private void ZoomButtonOut_Click(object sender, EventArgs e)
        {
            if (m_oZoomItems.SelectedIndex == 24)
                return;

            if (m_oZoomItems.SelectedIndex + 1 < m_oZoomItems.Items.Count)
                m_oZoomItems.SelectedIndex++;
        }
        private void ZoomButtonIn_Click(object sender, EventArgs e)
        {
            if (m_oZoomItems.SelectedIndex == 0)
                return;

            m_oZoomItems.SelectedIndex--;
        }
        private void ZoomItems_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (null != m_oZoomItems.SelectedItem)
            {
                if ("Window" == (string)m_oZoomItems.SelectedItem)
                {
                    return;
                }

                try
                {
                    string Zoom = (string)m_oZoomItems.SelectedItem;
                    int nZoom = Convert.ToInt32(Zoom.Remove(Zoom.IndexOf('%')));

                    m_bUpdateZoomToWindow = true;

                    m_oViewSettings.SetProperty("Zoom", (double)nZoom / 100.0);
                    m_oZoomTackBar.Value = (int)((double)m_oViewSettings.GetProperty("Zoom") * 500.0);
                    m_oViewSettings.Refresh();

                    double Zm = (double)m_oViewSettings.GetProperty("Zoom");

                    m_oZoomStatusLabel.Text = "Zoom : " +  Zm * 100.0 + "%";

                    m_bUpdateZoomToWindow = false;

                    EnabledScrollBars();
                }
                catch
                {

                }
            }

        }

        #endregion

        #region ViewOperations

        private void RullersButton_Click(object sender, EventArgs e)
        {
            if (m_oRullersButton.Checked)
            {
                m_oRullersButton.Checked = false;

                m_oViewSettings.SetProperty("EnableRullerUp", false);
                m_oViewSettings.SetProperty("EnableRullerDown", false);
                m_oViewSettings.SetProperty("EnableRullerLeft", false);
                m_oViewSettings.SetProperty("EnableRullerRight", false);
            }
            else
            {
                m_oRullersButton.Checked = true;
               
                m_oViewSettings.SetProperty("EnableRullerUp", true);
                m_oViewSettings.SetProperty("EnableRullerDown", true);
                m_oViewSettings.SetProperty("EnableRullerLeft", true);
                m_oViewSettings.SetProperty("EnableRullerRight", true);
            }

            m_oImageEditor.Refresh();

        }
        private void GridEnableButton_Click(object sender, EventArgs e)
        {
            if (m_oGridEnableButton.Checked)
            {
                m_oGridEnableButton.Checked = false;
                m_oViewSettings.SetProperty("EnableGid", false);
            }
            else
            {
                m_oGridEnableButton.Checked = true;
                m_oViewSettings.SetProperty("EnableGid", true);
            }

            m_oViewSettings.Refresh();
           // m_oImageEditor.Refresh();
        }
        private void ComboBoxRullersType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (nRullerTypePixels == m_oComboBoxRullersType.SelectedIndex)
            {
                m_oViewSettings.SetProperty("RullersType", nRullerTypePixels);
            }
            if (nRullerTypeCentimeters == m_oComboBoxRullersType.SelectedIndex)
            {
                m_oViewSettings.SetProperty("RullersType", nRullerTypeCentimeters);
            }

            if (nRullerTypeInches == m_oComboBoxRullersType.SelectedIndex)
            {
                m_oViewSettings.SetProperty("RullersType", nRullerTypeInches);
            }

            m_oViewSettings.Refresh();
        }

        #endregion

        #region ScrollBars

        private void HScrollBar_Scroll(object sender, ScrollEventArgs e)
        {
            // m_bUpdateCenterX = true;
            // m_nHScrollNewValue = e.NewValue;
            // return;

            double SCrollPos = (double)e.NewValue;
           // if (m_dHSCrollMin > SCrollPos)
           //     SCrollPos = m_dHSCrollMin;
           
           // if (m_dHSCrollMax < SCrollPos)
           //     SCrollPos = m_dHSCrollMax;

            m_oViewSettings.SetProperty("CenterX", SCrollPos);
            m_oScrollCenterLabel.Text = String.Format("Center : [ {0}: {1} ]", m_oViewSettings.GetProperty("CenterX"), m_oViewSettings.GetProperty("CenterY"));

            int ImageWidth = (int)m_oViewSettings.GetProperty("ImageWidth");
            int ImageHeight = (int)m_oViewSettings.GetProperty("ImageHeight");
            int PreviewLeft = (int)m_oViewSettings.GetProperty("PreviewLeft");
            int PreviewRight = (int)m_oViewSettings.GetProperty("PreviewRight");
            int PreviewTop = (int)m_oViewSettings.GetProperty("PreviewTop");
            int PreviewBottom = (int)m_oViewSettings.GetProperty("PreviewBottom");

            m_oImageSizesLabel.Text = String.Format("ImageSize : [ {0}: {1} ]", ImageWidth, ImageHeight);
            m_oPreviewSizesLabel.Text = String.Format("Preview : [ {0}: {1}: {2}: {3} ]", PreviewLeft, PreviewTop, PreviewRight, PreviewBottom);

            m_oViewSettings.Refresh();
        }
        private void VScrollBar_Scroll(object sender, ScrollEventArgs e)
        {
            double SCrollPos = (double)e.NewValue;
          //  if (m_dVSCrollMin > SCrollPos)
           //     SCrollPos = m_dVSCrollMin;
         
           // if (m_dVSCrollMax < SCrollPos)
           //     SCrollPos = m_dVSCrollMax;

            m_oViewSettings.SetProperty("CenterY", SCrollPos);
            m_oScrollCenterLabel.Text = String.Format("Center : [ {0}: {1} ]", m_oViewSettings.GetProperty("CenterX"), m_oViewSettings.GetProperty("CenterY"));

            int ImageWidth = (int)m_oViewSettings.GetProperty("ImageWidth");
            int ImageHeight = (int)m_oViewSettings.GetProperty("ImageHeight");
            int PreviewLeft = (int)m_oViewSettings.GetProperty("PreviewLeft");
            int PreviewRight = (int)m_oViewSettings.GetProperty("PreviewRight");
            int PreviewTop = (int)m_oViewSettings.GetProperty("PreviewTop");
            int PreviewBottom = (int)m_oViewSettings.GetProperty("PreviewBottom");

            m_oImageSizesLabel.Text = String.Format("ImageSize : [ {0}: {1} ]", ImageWidth, ImageHeight);
            m_oPreviewSizesLabel.Text = String.Format("Preview : [ {0}: {1}: {2}: {3} ]", PreviewLeft, PreviewTop, PreviewRight, PreviewBottom);

            m_oViewSettings.Refresh();
        }
        private void EnabledScrollBars()
        {
            try
            {
                double CurZoom = (double)m_oViewSettings.GetProperty("Zoom");

                int ImageWidth = (int)m_oViewSettings.GetProperty("ImageWidth");
                int ImageHeight = (int)m_oViewSettings.GetProperty("ImageHeight");
                int PreviewLeft = (int)m_oViewSettings.GetProperty("PreviewLeft");
                int PreviewRight = (int)m_oViewSettings.GetProperty("PreviewRight");
                int PreviewTop = (int)m_oViewSettings.GetProperty("PreviewTop");
                int PreviewBottom = (int)m_oViewSettings.GetProperty("PreviewBottom");

                m_oImageSizesLabel.Text = String.Format("ImageSize : [ {0}: {1} ]", ImageWidth, ImageHeight);
                m_oPreviewSizesLabel.Text = String.Format("Preview : [ {0}: {1}: {2}: {3} ]", PreviewLeft, PreviewTop, PreviewRight, PreviewBottom);

                double ZoomToWindow = Math.Min((double)m_oImageEditor.Size.Width / (double)ImageWidth,
                                               (double)m_oImageEditor.Size.Height / (double)ImageHeight);

                if (ZoomToWindow >= CurZoom || m_oViewSettings.ZoomToWindow)
                {
                    m_hScrollBar.Enabled = false;
                    m_vScrollBar.Enabled = false;
                }
                else
                {
                    if ((double)ImageWidth * CurZoom > (double)m_oImageEditor.Size.Width)
                    {
                        m_dHSCrollMin   = (double)(m_oImageEditor.Size.Width - 20 ) * 0.5 / CurZoom;
                        m_dHSCrollMax   = (double)ImageWidth - m_dHSCrollMin;
                        m_dHLargeChange = (double)ImageWidth - (m_dHSCrollMax - m_dHSCrollMin);

                        m_hScrollBar.Enabled = true;
                        m_hScrollBar.Minimum = (int)(m_oImageEditor.Size.Width * 0.5 / CurZoom );
                        m_hScrollBar.Maximum = ImageWidth - m_hScrollBar.Minimum;
                        m_hScrollBar.LargeChange = ImageWidth - (m_hScrollBar.Maximum - m_hScrollBar.Minimum);
                        m_hScrollBar.Maximum += m_hScrollBar.LargeChange;
                    
                        //m_hScrollBar.Minimum = 0;
                        //m_hScrollBar.Maximum = ImageWidth;
                        //m_hScrollBar.LargeChange = 1;
                    }
                    else
                    {
                        m_hScrollBar.Enabled = false;
                    }

                    if ((double)ImageHeight * CurZoom > (double)m_oImageEditor.Size.Height)
                    {
                        m_dVSCrollMin = (double)(m_oImageEditor.Size.Height - 20) * 0.5 / CurZoom;
                        m_dVSCrollMax   = (double)ImageHeight - m_dVSCrollMin;
                        m_dVLargeChange = (double)ImageHeight - (m_dVSCrollMax - m_dVSCrollMin);

                        m_vScrollBar.Enabled = true;
                        m_vScrollBar.Minimum = (int)(m_oImageEditor.Size.Height * 0.5 / CurZoom ) - 1;
                        m_vScrollBar.Maximum = ImageHeight - m_vScrollBar.Minimum + 1;
                        m_vScrollBar.LargeChange = ImageHeight - (m_vScrollBar.Maximum - m_vScrollBar.Minimum);
                        m_vScrollBar.Maximum += m_vScrollBar.LargeChange;
                        
                        //m_vScrollBar.Minimum = 0;
                        //m_vScrollBar.Maximum = ImageHeight;
                        //m_vScrollBar.LargeChange = 1;
                    }
                    else
                    {
                        m_hScrollBar.Enabled = false;
                    }
                }
            }
            catch
            {

            }
        }

        #endregion

        #region FileOperations

        private void OpenImageMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialog.ShowDialog() != DialogResult.OK)
                return;

            if (m_oImageEditor.Open(openFileDialog.FileName))
            {
                //MessageBox("Ok","Ok");
            }
        }
        private void OpenImageFile_Click(object sender, EventArgs e)
        {
            if (openFileDialog.ShowDialog() != DialogResult.OK)
                return;

            if (m_oImageEditor.Open(openFileDialog.FileName))
            {
                //MessageBox("Ok","Ok");
            }
        }

        #endregion

        private void OnTimerSliderHTick(object sender, EventArgs e)
        {
            if (m_bUpdateCenterX)
            {
                m_oViewSettings.SetProperty("CenterX", (double)m_nHScrollNewValue);
                m_oScrollCenterLabel.Text = String.Format("Center : [ {0}: {1} ]", m_oViewSettings.GetProperty("CenterX"), m_oViewSettings.GetProperty("CenterY"));

                m_oImageEditor.Refresh();
                
                m_bUpdateCenterX = false;
            }
        }

        private void StartCommandBtn_Click(object sender, EventArgs e)
        {
            int nID = CommandChoise_comboBox.SelectedIndex;
            if (nID>0)
            {
                nID = 99+nID;
            }
            
            controlBrushProperties1.Visible = (104 == nID);
            controlCropProperties1.Visible = (102 == nID);
            controlBrightnessContrastProperties1.Visible = (106 == nID);
            controlLevelsProperties1.Visible = (107 == nID);
            controlToneProperties1.Visible = (108 == nID);
            controlXMLProperties1.Visible = (109 == nID);
            
            if (m_oImageEditor.StartCommand(nID))
            {
                AVSImageEditor.ICommand commandDescriptor = (AVSImageEditor.ICommand)m_oImageEditor.GetCommand();
                if (102 == nID)
                {
                    controlCropProperties1.FromCommand(commandDescriptor);
                }
                else if (104 == nID)
                {
                    controlBrushProperties1.FromCommand(commandDescriptor);
                }
                else if (106 == nID)
                {
                    controlBrightnessContrastProperties1.FromCommand(commandDescriptor);
                }
                else if (107 == nID)
                {
                    controlLevelsProperties1.FromCommand(commandDescriptor);
                }
                else if (108 == nID)
                {
                    controlToneProperties1.FromCommand(commandDescriptor);
                }
                else if (109 == nID)
                {
                    controlXMLProperties1.FromCommand(commandDescriptor);
                }

                else
                {
                    int paramsCnt = commandDescriptor.GetParamsCount();
                    String msgString = String.Format("Start Command Success: \n     Parameters Count = {0} \n     All parameters:\n", paramsCnt);
                    for (int i = 0; i < paramsCnt; i++)
                    {
                        AVSImageEditor.ICommandParam curParam = (AVSImageEditor.ICommandParam)commandDescriptor.GetParam(i);
                        String tmpString = String.Format("     Parameter {0}:\n                   name - {1},\n                   description - {2},\n                   type - {3},\n                   defaultValue = {4},\n                   minValue = {5},\n                   maxValue = {6} \n", i, curParam.GetName(),
                                                                                                                                                        curParam.GetDescription(),
                                                                                                                                                        curParam.Value.GetType().ToString(),
                                                                                                                                                        curParam.Value,
                                                                                                                                                        curParam.GetMinValue(),
                                                                                                                                                        curParam.GetMaxValue());

                        msgString += tmpString;

                        if (curParam.GetName() == "Size")
                        {
                            Size_label.Show();
                            Size_trackBar.Show();
                            if (curParam.GetMinValue() is Int32 &&
                                curParam.GetMaxValue() is Int32)
                            {
                                Int32 minV = (Int32)curParam.GetMinValue();
                                Int32 maxV = (Int32)curParam.GetMaxValue();
                                Int32 Val = (Int32)curParam.Value;
                                Size_trackBar.SetRange(minV, maxV);
                                Size_trackBar.Value = Val;
                            }
                            m_size_param = curParam;
                        }

                        if (curParam.GetName() == "Type")
                        {
                            Type_label.Show();
                            Type_comboBox.Show();
                            m_type_param = curParam;
                            if (curParam.Value is Int32)
                            {
                                Int32 Val = (Int32)curParam.Value;
                                Type_comboBox.SelectedIndex = Val;
                            }

                        }
                    }
                    //MessageBox.Show(msgString, "TestMessage", MessageBoxButtons.OK);
                }
            }
            else
            {
                MessageBox.Show("Not implemented yet", "Debug Message", MessageBoxButtons.OK);
            }
        }

        private void StopCommandBtn_Click(object sender, EventArgs e)
        {
            m_oImageEditor.StopCommand(true);

            Size_label.Hide();
            Size_trackBar.Hide();

            Type_label.Hide();
            Type_comboBox.Hide();

            controlXMLProperties1.Visible = false;
            controlToneProperties1.Visible = false;
            controlLevelsProperties1.Visible = false;
            controlBrushProperties1.Visible = false;
            controlCropProperties1.Visible = false;
            controlBrightnessContrastProperties1.Visible = false;
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialog.ShowDialog() != DialogResult.OK)
                return;

            if (m_oImageEditor.Open(openFileDialog.FileName))
            {
                //MessageBox("Ok","Ok");
            }
        }

        private void UndoButton_Click(object sender, EventArgs e)
        {
            m_oUndoStack.Pop(1);
        }

        private void RedoButton_Click(object sender, EventArgs e)
        {
            m_oRedoStack.Pop(1);
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (saveFileDialog.ShowDialog() != DialogResult.OK)
                return;

            if (m_oImageEditor.Save(saveFileDialog.FileName))
            {
                //MessageBox("Ok","Ok");
            }
        }

        private void Size_trackBar_Scroll(object sender, EventArgs e)
        {
            m_size_param.Value = Size_trackBar.Value;
              
        }

        private void Type_comboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_type_param.Value = (Int32)Type_comboBox.SelectedIndex;
        }

        private void UndoReset_Click(object sender, EventArgs e)
        {
            m_oUndoStack.Pop(m_oUndoStack.GetDepth());
        }

        private void openEmptyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_oImageEditor.Open("");
        }

        private void openSpaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_oImageEditor.Open(" ");
        }

        private void openBadFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_oImageEditor.Open("asdasdasd");
        }

        private void FullChess_Click(object sender, EventArgs e)
        {

        }

        private void ColorBackground_Click(object sender, EventArgs e)
        {

        }

        private void ChessOnlyImage_Click(object sender, EventArgs e)
        {

        }

        private void OnFastRenderWindow_Click(object sender, EventArgs e)
        {
            if (m_oFastWindow.Checked)
            {
                m_oFastWindow.Checked = false;

                m_oViewSettings.SetProperty("EnableFastWindow", 0);
            }
            else
            {
                m_oFastWindow.Checked = true;

                m_oViewSettings.SetProperty("EnableFastWindow", 1);
            }

            m_oImageEditor.Refresh();
        }

        private void bBeginUpdate_Click(object sender, EventArgs e)
        {
            m_oViewSettings.SetProperty("BeginUpdate", 1);
        }

        private void bEndpdate_Click(object sender, EventArgs e)
        {
            m_oViewSettings.SetProperty("EndUpdate", 1);
        }
    }
}
