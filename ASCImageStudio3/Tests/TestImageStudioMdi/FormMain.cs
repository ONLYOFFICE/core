using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace TestImageStudioMdi
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        #region Methods

        private void FormMain_Load(object sender, EventArgs e)
        {
            m_oNavigation.MdiParent = this;
            m_oNavigation.Main = this;
            m_oNavigation.Show();
            m_oNavigation.SetBounds(2, 0, 0, 0, BoundsSpecified.Location);

            m_oPreview.MdiParent = this;
            m_oPreview.Show();
            m_oPreview.SetBounds(m_oNavigation.Left, m_oNavigation.Bottom, m_oNavigation.Width, m_oNavigation.Height, BoundsSpecified.All);

            m_oTransforms.MdiParent = this;
            m_oTransforms.Show();
            m_oTransforms.SetBounds(m_oNavigation.Right + 2, m_oNavigation.Top, 600, m_oPreview.Bottom - m_oNavigation.Top, BoundsSpecified.All);
        }

        private void menuFileExit_Click(object sender, EventArgs e)
        {
            Dispose();
        }

        public void SetPreview(object oImage)
        {
            m_oPreview.SetPreview(oImage);
        }

        #endregion

        #region Members

        private FormTransforms m_oTransforms = new FormTransforms();
        private FormPreview m_oPreview = new FormPreview();
        private FormNavigation m_oNavigation = new FormNavigation();

        #endregion

        #region Properties

        public string TransformXml
        {
            get { return m_oTransforms.TransformXml; }
        }

        #endregion
    }
}