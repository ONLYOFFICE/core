using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace AVSOfficeOCRTest
{
    public partial class OCRView : Form
    {
        public OCRView(string strText)
        {
            InitializeComponent();

            ShowXmlInWebBrowser(webBrowser1, strText);
        }

        public static void ShowXmlInWebBrowser(WebBrowser webBrowser, string TextDocument)
        {
            try
            {
                // generate a unique file name
                string FileNameTemp = Environment.GetFolderPath(Environment.SpecialFolder.InternetCache) + "\\"
                                      + "TextDocument_".GetHashCode().ToString();
                // clean up just incase
                if (File.Exists(FileNameTemp))
                    File.Delete(FileNameTemp);

                // write wsdl to a temporary file
                using (StreamWriter oTempFile = File.CreateText(FileNameTemp))
                {
                    //TextDocument = TextDocument.Replace("utf-16", "utf-8");

                    oTempFile.Write(TextDocument);
                }

                webBrowser.Navigate(FileNameTemp, false);
            }
            catch
            {

            }
        }
    }
}
