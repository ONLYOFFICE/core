using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TestViewManager.CommandParams
{
    public partial class ControlCropProperties : UserControl
    {
        private bool m_bIsUpdate = true;
        private AVSImageEditor.ICommand m_oCommand = null;

        public ControlCropProperties()
        {
            InitializeComponent();
        }

        public void FromCommand(AVSImageEditor.ICommand oCommand)
        {
            m_bIsUpdate = false;
            m_oCommand = oCommand;
            int nParamsCount = m_oCommand.GetParamsCount();
            for (int i = 0; i < nParamsCount; ++i)
            {
                AVSImageEditor.ICommandParam curParam = (AVSImageEditor.ICommandParam)m_oCommand.GetParam(i);

                if (curParam.GetName() == "Width")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackWidthCrop.SetRange(minV, maxV);
                        trackWidthCrop.Value = Val;
                        textWidthCrop.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Height")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackHeightCrop.SetRange(minV, maxV);
                        trackHeightCrop.Value = Val;
                        textHeightCrop.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Angle")
                {
                    if (curParam.GetMinValue() is Double &&
                        curParam.GetMaxValue() is Double)
                    {
                        Int32 minV = (Int32)(Double)curParam.GetMinValue();
                        Int32 maxV = (Int32)(Double)curParam.GetMaxValue();
                        Int32 Val = (Int32)(Double)curParam.Value;
                        trackAngle.SetRange(minV, maxV);
                        trackAngle.Value = Val;
                        textAngle.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "OffsetX")
                {
                    if (curParam.GetMinValue() is Double &&
                        curParam.GetMaxValue() is Double)
                    {
                        Int32 minV = (Int32)(Double)curParam.GetMinValue();
                        Int32 maxV = (Int32)(Double)curParam.GetMaxValue();
                        Int32 Val = (Int32)(Double)curParam.Value;
                        trackOffsetX.SetRange(minV, maxV);
                        trackOffsetX.Value = Val;
                        textOffsetX.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "OffsetY")
                {
                    if (curParam.GetMinValue() is Double &&
                        curParam.GetMaxValue() is Double)
                    {
                        Int32 minV = (Int32)(Double)curParam.GetMinValue();
                        Int32 maxV = (Int32)(Double)curParam.GetMaxValue();
                        Int32 Val = (Int32)(Double)curParam.Value;
                        trackOffsetY.SetRange(minV, maxV);
                        trackOffsetY.Value = Val;
                        textOffsetY.Text = Val.ToString();
                    }
                }
            }
            m_bIsUpdate = true;
        }

        private void trackWidthCrop_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(0))).Value = trackWidthCrop.Value;
            textWidthCrop.Text = trackWidthCrop.Value.ToString();
        }

        private void trackHeightCrop_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(1))).Value = trackHeightCrop.Value;
            textHeightCrop.Text = trackHeightCrop.Value.ToString();
        }

        private void trackAngle_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(2))).Value = trackAngle.Value;
            textAngle.Text = trackAngle.Value.ToString();
        }

        private void trackOffsetX_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(3))).Value = trackOffsetX.Value;
            textOffsetX.Text = trackOffsetX.Value.ToString();
        }

        private void trackOffsetY_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(4))).Value = trackOffsetY.Value;
            textOffsetY.Text = trackOffsetY.Value.ToString();
        }
    }
}
