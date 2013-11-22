using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TestViewManager
{
    public partial class ControlBrushProperties : UserControl
    {
        private bool m_bIsUpdate = true;
        private AVSImageEditor.ICommand m_oCommand = null;

        public ControlBrushProperties()
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

                if (curParam.GetName() == "Type")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 Val = (Int32)curParam.Value;
                        comboType.SelectedIndex = Val;
                    }
                }

                if (curParam.GetName() == "Size")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackSize.SetRange(minV, maxV);
                        trackSize.Value = Val;
                        textSize.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Level")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackLevel.SetRange(minV, maxV);
                        trackLevel.Value = Val;
                        textLevel.Text = Val.ToString();
                    }
                }

                /*
                if (curParam.GetName() == "Brightness")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackLevel.SetRange(minV, maxV);
                        trackLevel.Value = Val;
                        textLevel.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "LimitBrightness")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackLimitBrightness.SetRange(minV, maxV);
                        trackLimitBrightness.Value = Val;
                        textLimitBrightness.Text = Val.ToString();
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
                */
            }
            m_bIsUpdate = true;
        }

        private void trackSize_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(1))).Value = trackSize.Value;
            textSize.Text = trackSize.Value.ToString();
        }

        private void trackBrightness_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(2))).Value = trackLevel.Value;
            textLevel.Text = trackLevel.Value.ToString();
        }

        private void trackLimitBrightness_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(3))).Value = trackLimitBrightness.Value;
            textLimitBrightness.Text = trackLimitBrightness.Value.ToString();
        }

        private void trackContrast_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(4))).Value = trackContrast.Value;
            textContrast.Text = trackContrast.Value.ToString();
        }

        private void comboType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(0))).Value = comboType.SelectedIndex;
        }
    }
}
