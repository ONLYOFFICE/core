using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TestViewManager
{
    public partial class ControlXMLProperties : UserControl
    {
        private bool m_bIsUpdate = true;
        private AVSImageEditor.ICommand m_oCommand = null;
        public ControlXMLProperties()
        {
            InitializeComponent();
        }

        public void FromCommand(AVSImageEditor.ICommand oCommand)
        {
            m_bIsUpdate = false;
            m_oCommand = oCommand;
            
            m_bIsUpdate = true;
        }

        private void Effect1Radio_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            if (!Effect1Radio.Checked)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(0))).Value =
                "<ImageTransform-AdjustInvert></ImageTransform-AdjustInvert>";
        }

        private void Effect2Radio_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            if (!Effect2Radio.Checked)
                return;
            ((AVSImageEditor.ICommandParam)(m_oCommand.GetParam(0))).Value = 
                "<ImageTransform-EffectNoise level=\"100\" monochrome=\"1\" metric=\"0\"></ImageTransform-EffectNoise>";
        }

        private void Effect3Radio_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            if (!Effect3Radio.Checked)
                return;
        }

        private void Effect4Radio_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            if (!Effect4Radio.Checked)
                return;
        }

        private void Effect5Radio_CheckedChanged(object sender, EventArgs e)
        {
            if (!m_bIsUpdate)
                return;
            if (!Effect5Radio.Checked)
                return;
        }
    }
}
