using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TestViewManager
{
    public partial class ControlLevelsProperties : UserControl
    {
        private bool m_bIsUpdate = true;
        private AVSImageEditor.ICommand m_oCommand = null;
        public ControlLevelsProperties()
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

                if (curParam.GetName() == "Red")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackRed.SetRange(minV, maxV);
                        trackRed.Value = Val;
                        textRed.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Green")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackGreen.SetRange(minV, maxV);
                        trackGreen.Value = Val;
                        textGreen.Text = Val.ToString();
                    }
                }

                if (curParam.GetName() == "Blue")
                {
                    if (curParam.GetMinValue() is Int32 &&
                        curParam.GetMaxValue() is Int32)
                    {
                        Int32 minV = (Int32)curParam.GetMinValue();
                        Int32 maxV = (Int32)curParam.GetMaxValue();
                        Int32 Val = (Int32)curParam.Value;
                        trackBlue.SetRange(minV, maxV);
                        trackBlue.Value = Val;
                        textBlue.Text = Val.ToString();
                    }
                }

            }
            m_bIsUpdate = true;
        }

        private void trackRed_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(0))).Value = trackRed.Value;
            textRed.Text = trackRed.Value.ToString();
        }

        private void trackGreen_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(1))).Value = trackGreen.Value;
            textGreen.Text = trackGreen.Value.ToString();
        }

        private void trackBlue_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(2))).Value = trackBlue.Value;
            textBlue.Text = trackBlue.Value.ToString();
        }
    }
}
