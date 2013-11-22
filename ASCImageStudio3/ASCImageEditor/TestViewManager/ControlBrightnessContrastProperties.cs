using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TestViewManager
{
    public partial class ControlBrightnessContrastProperties : UserControl
    {
        private bool m_bIsUpdate = true;
        private AVSImageEditor.ICommand m_oCommand = null;
        public ControlBrightnessContrastProperties()
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

                if (curParam.GetName() == "Brightness")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackBrightness.SetRange(minV, maxV);
                        trackBrightness.Value = Val;
                        textBrightness.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Contrast")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackContrast.SetRange(minV, maxV);
                        trackContrast.Value = Val;
                        textContrast.Text = Val.ToString();
                    }
                }
            }
            m_bIsUpdate = true;
        }

        private void trackBrightness_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(0))).Value = trackBrightness.Value;
            textBrightness.Text = trackBrightness.Value.ToString();
        }

        private void trackContrast_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(1))).Value = trackContrast.Value;
            textContrast.Text = trackContrast.Value.ToString();
        }

    }
}
