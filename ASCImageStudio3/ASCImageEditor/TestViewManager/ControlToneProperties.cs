using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TestViewManager
{
    public partial class ControlToneProperties : UserControl
    {
        private bool m_bIsUpdate = true;
        private AVSImageEditor.ICommand m_oCommand = null;
        public ControlToneProperties()
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

                if (curParam.GetName() == "Hue")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackHue.SetRange(minV, maxV);
                        trackHue.Value = Val;
                        textHue.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Saturation")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackSaturation.SetRange(minV, maxV);
                        trackSaturation.Value = Val;
                        textSaturation.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Posterize")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackPosterize.SetRange(minV, maxV);
                        trackPosterize.Value = Val;
                        textPosterize.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Temperature")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackTemperature.SetRange(minV, maxV);
                        trackTemperature.Value = Val;
                        textTemperature.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Black")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackBlack.SetRange(minV, maxV);
                        trackBlack.Value = Val;
                        trackBlack.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Fill Light")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackFillLight.SetRange(minV, maxV);
                        trackFillLight.Value = Val;
                        trackFillLight.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Recovery")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackRecovery.SetRange(minV, maxV);
                        trackRecovery.Value = Val;
                        trackRecovery.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Explosure")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackExplosure.SetRange(minV, maxV);
                        trackExplosure.Value = Val;
                        trackExplosure.Text = Val.ToString();
                    }
                }

            }
            m_bIsUpdate = true;
        }

        private void trackHue_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(0))).Value = trackHue.Value;
            textHue.Text = trackHue.Value.ToString();
        }

        private void trackSaturation_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(1))).Value = trackSaturation.Value;
            textSaturation.Text = trackSaturation.Value.ToString();
        }

        private void trackPosterize_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(2))).Value = trackPosterize.Value;
            textPosterize.Text = trackPosterize.Value.ToString();
        }

        private void trackTemperature_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(3))).Value = trackTemperature.Value;
            textTemperature.Text = trackTemperature.Value.ToString();
        }

        private void trackBlack_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(4))).Value = trackBlack.Value;
            textBlack.Text = trackBlack.Value.ToString();
        }
        private void trackFillLight_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(5))).Value = trackFillLight.Value;
            textFillLight.Text = trackFillLight.Value.ToString();
        }
        private void trackRecovery_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(6))).Value = trackRecovery.Value;
            textRecovery.Text = trackRecovery.Value.ToString();
        }
        private void trackExplosure_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(7))).Value = trackExplosure.Value;
            textExplosure.Text = trackExplosure.Value.ToString();
        }
    }
}
