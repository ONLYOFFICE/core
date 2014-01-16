using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestEngine
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            EditorEngine.CEditorEngineClass oClass = new EditorEngine.CEditorEngineClass();

            // 1st test
            oClass.ImageTest(@"C:\Documents and Settings\All Users\Documents\My Pictures\Sample Pictures\Sunset.jpg");

            // 2nd test
            oClass.PageTest(@"D:\Exchange\1234\RENDERER\page22.pagebin", @"D:\Exchange\1234\RENDERER\dst.png", 210, 297);
        }
    }
}
