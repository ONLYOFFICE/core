using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using System.IO;


namespace TestMetaData
{
    public partial class FormMetaData : Form
    {

       readonly AVSImageFile3.ImageFile3Class m_ImageFile = new AVSImageFile3.ImageFile3Class();
        
        public FormMetaData()
        {
            InitializeComponent();
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void btCancel_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void btSave_Click(object sender, EventArgs e)
        {

            //string sExtension = ".bmp";
            //switch (comboBoxFormat.SelectedIndex)
            //{
            //    case 0: sExtension = "*.bmp"; break;
            //    case 1: sExtension = "*.gif"; break;
            //    case 2: sExtension = "*.jpg"; break;
            //    case 3: sExtension = "*.png"; break;
            //    case 4: sExtension = "*.tif"; break;
            //    case 5: sExtension = "*.pcx"; break;
            //    case 6: sExtension = "*.tga"; break;
            //    case 7: sExtension = "*.ras"; break;

            //};

            //saveFileDialog1.DefaultExt = sExtension;
            //saveFileDialog1.Filter = "(" + sExtension + ")";
            //saveFileDialog1.AddExtension = true;
            
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            string sPath = saveFileDialog1.FileName;
            string sXML = UpdateXML();
            m_ImageFile.SetMetaData(sXML);
            switch (comboBoxFormat.SelectedIndex)
            {
                case 0: m_ImageFile.SaveAsBmp(ref arrImage, sPath); break;
                case 1: m_ImageFile.SaveAsGif(ref arrImage, sPath); break;
                case 2: m_ImageFile.SaveAsJpeg(ref arrImage, sPath); break;
                case 3: m_ImageFile.SaveAsPng(ref arrImage, sPath); break;
                case 4: m_ImageFile.SaveAsTiff(ref arrImage, sPath); break;
                case 5: m_ImageFile.SaveAsPcx(ref arrImage, sPath); break;
                case 6: m_ImageFile.SaveAsTga(ref arrImage, sPath); break;
                case 7: m_ImageFile.SaveAsRas(ref arrImage, sPath); break;
                case 8: m_ImageFile.SaveAsEmf(ref arrImage, sPath); break;
                case 9: m_ImageFile.SaveAsWmf(ref arrImage, sPath); break;

            };


        }

        private string UpdateXML()
        {
            StringWriter oStringWriter = new StringWriter(new StringBuilder());
            XmlTextWriter oWriter = new XmlTextWriter(oStringWriter);

            oWriter.WriteStartElement("MetaData");

            
            XMLWriteNode(oWriter, "Title",       textBoxTitle.Text);
            XMLWriteNode(oWriter, "Author",      textBoxAuthor.Text);
            XMLWriteNode(oWriter, "Description", richTextBoxDescription.Text);

            oWriter.WriteStartElement("DateTime");

            char[] sDateTime = textBoxCreationTime.Text.ToCharArray();

            int nPosition = 0;

            string sYear = "";
            while ('\\' != sDateTime[nPosition])
            {
                sYear += sDateTime[nPosition];
                nPosition++;
            }
            nPosition++;

            string sMonth = "";
            while ('\\' != sDateTime[nPosition])
            {
                sMonth += sDateTime[nPosition];
                nPosition++;
            }
            nPosition++;

            string sDay = "";
            while (' ' != sDateTime[nPosition])
            {
                sDay += sDateTime[nPosition];
                nPosition++;
            }
            nPosition++;

            string sHour = "";
            while (':' != sDateTime[nPosition])
            {
                sHour += sDateTime[nPosition];
                nPosition++;
            }
            nPosition++;
            string sMinute = "";
            while (':' != sDateTime[nPosition])
            {
                sMinute += sDateTime[nPosition];
                nPosition++;
            }
            nPosition++;
            string sSecond = "";
            while (' ' != sDateTime[nPosition] && nPosition < 20 && nPosition + 1 < sDateTime.Length)
            {
                sSecond += sDateTime[nPosition];
                nPosition++;
            }
            nPosition++;
            string sSummary = "";
            for ( ;nPosition < textBoxCreationTime.Text.Length; nPosition++)
                sSummary += sDateTime[nPosition];

            XMLWriteNode(oWriter, "Year",    sYear);
            XMLWriteNode(oWriter, "Month",   sMonth);
            XMLWriteNode(oWriter, "Day",     sDay);
            XMLWriteNode(oWriter, "Hour",    sHour);
            XMLWriteNode(oWriter, "Minute",  sMinute);
            XMLWriteNode(oWriter, "Second",  sSecond);
            XMLWriteNode(oWriter, "Summary", sSummary);

            oWriter.WriteEndElement(); //DateTime

            XMLWriteNode(oWriter, "Copyright",      textBoxCopyright.Text);
            XMLWriteNode(oWriter, "Disclaimer",     textBoxDisclaimer.Text);
            XMLWriteNode(oWriter, "Comment",        richTextBoxComments.Text);
            XMLWriteNode(oWriter, "EquipmentType",  textBoxEquipmentType.Text);
            XMLWriteNode(oWriter, "EquipmentModel", textBoxEquipmentModel.Text);

            oWriter.WriteStartElement("JobTime");

            char[] sJobTime   = textBoxJobTime.Text.ToCharArray();

            nPosition = 0;

            string sJobHour = "";
            while ( ':' != sJobTime[nPosition] && nPosition < sJobTime.Length)
            {
                sJobHour += sJobTime[nPosition];
                nPosition++;
            }
            nPosition++;
            string sJobMinute = "";
            while (':' != sJobTime[nPosition] && nPosition < sJobTime.Length)
            {
                sJobMinute += sJobTime[nPosition];
                nPosition++;
            }
            nPosition++;
            string sJobSecond = "";
            while (nPosition < sJobTime.Length)
            {
                sJobSecond += sJobTime[nPosition];
                nPosition++;
            }

            XMLWriteNode(oWriter, "Hour",   sJobHour);
            XMLWriteNode(oWriter, "Minute", sJobMinute);
            XMLWriteNode(oWriter, "Second", sJobSecond);

            oWriter.WriteEndElement(); // JobTime

            XMLWriteNode(oWriter, "JobName",       textBoxJobName.Text);
            XMLWriteNode(oWriter, "SoftwareID",    textBoxSoftwareID.Text);
            XMLWriteNode(oWriter, "VersionNumber", Convert.ToString(numericSoftwareVersion.Value) );
            XMLWriteNode(oWriter, "VersionLetter", textBoxVersionLetter.Text);
            XMLWriteNode(oWriter, "HostComputer",  richTextBoxHostComputer.Text);
            XMLWriteNode(oWriter, "Warning",       richTextBoxWarning.Text);
            XMLWriteNode(oWriter, "DocumentName",  textBoxDocumentName.Text);
            XMLWriteNode(oWriter, "PageName",      textBoxPageName.Text);
            XMLWriteNode(oWriter, "PageNumber",    Convert.ToString(numericPageNumber.Value) );
            XMLWriteNode(oWriter, "PagesCount",    Convert.ToString(numericPagesCount.Value) );

            oWriter.WriteEndElement(); // MetaData

            oWriter.Close();
            oStringWriter.Close();
            return oStringWriter.GetStringBuilder().ToString();
        }

        private void XMLWriteNode(XmlTextWriter oWriter, string sName, string sValue)
        {
            oWriter.WriteStartElement(sName);
            oWriter.WriteString(sValue);
            oWriter.WriteEndElement();
        }

        private void btLoadFromXML_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            XmlDocument oDocument = new XmlDocument();
            oDocument.Load(openFileDialog1.FileName);

            XmlNode oMainNode = oDocument.ChildNodes[0];

            string sTemp = "";
            
            ReadNode(out sTemp, oMainNode, 0);
            textBoxTitle.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 1);
            textBoxAuthor.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 2);
            richTextBoxDescription.Text = sTemp;

            string sDateTime = "";
            ReadNode(out sTemp,  oMainNode.ChildNodes[3], 0);
            sDateTime += sTemp + "\\";
            ReadNode(out sTemp,  oMainNode.ChildNodes[3], 1);
            sDateTime += sTemp + "\\";
            ReadNode(out sTemp, oMainNode.ChildNodes[3], 2);
            sDateTime += sTemp + " ";
            ReadNode(out sTemp, oMainNode.ChildNodes[3], 3);
            sDateTime += sTemp + ":";
            ReadNode(out sTemp, oMainNode.ChildNodes[3], 4);
            sDateTime += sTemp + ":";
            ReadNode(out sTemp,  oMainNode.ChildNodes[3], 5);
            sDateTime += sTemp + " ";
            ReadNode(out sTemp, oMainNode.ChildNodes[3], 6);
            sDateTime += sTemp;

            textBoxCreationTime.Text = sDateTime;

            ReadNode(out sTemp, oMainNode, 4);
            textBoxCopyright.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 5);
            textBoxDisclaimer.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 6);
            richTextBoxComments.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 7);
            textBoxEquipmentType.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 8);
            textBoxEquipmentModel.Text = sTemp;

            string sJobTime = "";

            ReadNode(out sTemp,  oMainNode.ChildNodes[9], 0);
            sJobTime += sTemp + ":";
            ReadNode(out sTemp, oMainNode.ChildNodes[9], 1);
            sJobTime += sTemp + ":";
            ReadNode(out sTemp, oMainNode.ChildNodes[9], 2);
            sJobTime += sTemp;

            textBoxJobTime.Text = sJobTime;

            ReadNode(out sTemp, oMainNode, 10);
            textBoxJobName.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 11);
            textBoxSoftwareID.Text = sTemp;

            ReadNode(out sTemp,  oMainNode, 12);
            numericSoftwareVersion.Value = Convert.ToInt32(sTemp);

            ReadNode(out sTemp, oMainNode, 13);
            textBoxVersionLetter.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 14);
            richTextBoxHostComputer.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 15);
            richTextBoxWarning.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 16);
            textBoxDocumentName.Text = sTemp;
            ReadNode(out sTemp, oMainNode, 17);
            textBoxPageName.Text = sTemp;

            ReadNode(out sTemp,     oMainNode, 18);
            numericPageNumber.Value = Convert.ToInt32(sTemp);

            ReadNode(out sTemp,     oMainNode, 19);
            numericPagesCount.Value = Convert.ToInt32(sTemp);

        }

        private void ReadNode(out string sResult, XmlNode oNode, int nNodeIndex)
        {
            XmlNode oCurNode = oNode.ChildNodes[nNodeIndex];
            string sRes = oNode.ChildNodes[nNodeIndex].InnerText;
            if (null == sRes)
                sResult = "";
            else
                sResult = sRes;
        }
        private void ReadNode(out short nResult, XmlNode oNode, int nNodeIndex)
        {
            string sRes = oNode.ChildNodes[nNodeIndex].InnerText;
            if (null == sRes)
                nResult = 0;
            else
                nResult = short.Parse(sRes);
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label22_Click(object sender, EventArgs e)
        {

        }

        private void textBoxISOSpeed_TextChanged(object sender, EventArgs e)
        {

        }

        private void label28_Click(object sender, EventArgs e)
        {

        }
            
    }
}
