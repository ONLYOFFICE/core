using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace TestImageStudioMdi
{
    public partial class FormPreview : Form
    {
        public FormPreview()
        {
            InitializeComponent();
        }

        public void SetPreview(object oImage)
        {
            axImageView31.SetAdditionalParam("MediaData", oImage);
            axImageView31.ShrinkToFit();
        }
    }
}