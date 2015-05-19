using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using System.IO;



namespace TestMetaData
{
    public partial class MainForm : Form
    {
        readonly AVSImageFile3.ImageFile3Class m_ImageFile = new AVSImageFile3.ImageFile3Class();
        
        FormMetaData formMetaData = new FormMetaData();

        struct TDateTime
        {
            public short nYear;
            public short nMonth;
            public short nDay;
            public short nHour;
            public short nMinute;
            public short nSecond;
            public string sSummary;
        };


        struct TMetaData
        {

            public string m_sAuthor;
            public string m_sTitle;
            public string m_sDescription;
            public TDateTime m_oDateTimeCreation;
            public string m_sCopyright;
            public string m_sDisclaimer;
            public string m_sComment;
            public string m_sEquipmentType;
            public string m_sEquipmentModel;
            public TDateTime m_oJobTime;
            public string m_sJobName;
            public string m_sSoftwareID;
            public short m_nVersionNumber;
            public string m_sVersionLetter;
            public string m_sHostComputer;
            public string m_sWarning;
            public string m_sDocumentName;
            public string m_sPageName;
            public short m_nPageNumber;
            public short m_nPagesCount;

            public string m_sExposureProgram;
            public string m_sExposureTime;
            public string m_sExposureCompensation;
            public string m_sShutterSpeed;
            public string m_sLensAperture;
            public string m_sFocalLength;
            public string m_sFNumber;
            public short  m_nISOSpeed;
            public string m_sMeteringMode;
            public string m_sFlashMode;
            public string m_sColorSpace;

            public short m_nBitDepth;

        };

        TMetaData oMetaData;



        public MainForm()
        {
            InitializeComponent();
        }

        private void btOpen_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            object oImage = null; 
            
            textBox1.Text = openFileDialog1.FileName;
            if ( !m_ImageFile.LoadImage2(textBox1.Text, out oImage))
                return;

            axImageView31.BeginInit();
            axImageView31.Visible = false;
            axImageView31.SetMediaData(oImage);
            axImageView31.ZoomToFit();
            axImageView31.Visible = true;
            axImageView31.EndInit();

            string sXML;

            axImageView31.GetScreen(out formMetaData.arrImage);

            ClearMetaData();
            m_ImageFile.GetMetaData(out sXML);
            richTextBox1.Text = sXML;

            ParseXML(sXML);

        }

        private void btMetaData_Click(object sender, EventArgs e)
        {

            formMetaData.textBoxTitle.Text = oMetaData.m_sTitle;
            formMetaData.textBoxAuthor.Text = oMetaData.m_sAuthor;
            formMetaData.textBoxCopyright.Text = oMetaData.m_sCopyright;
            formMetaData.textBoxDisclaimer.Text = oMetaData.m_sDisclaimer;
            formMetaData.textBoxEquipmentModel.Text = oMetaData.m_sEquipmentModel;
            formMetaData.textBoxEquipmentType.Text = oMetaData.m_sEquipmentType;

            string sDateTime = oMetaData.m_oDateTimeCreation.nYear.ToString() + "\\";
            sDateTime += oMetaData.m_oDateTimeCreation.nMonth.ToString() + "\\";
            sDateTime += oMetaData.m_oDateTimeCreation.nDay.ToString() + " ";
            sDateTime += oMetaData.m_oDateTimeCreation.nHour.ToString() + ":";
            sDateTime += oMetaData.m_oDateTimeCreation.nMinute.ToString() + ":";
            sDateTime += oMetaData.m_oDateTimeCreation.nSecond.ToString() + " ";
            sDateTime += oMetaData.m_oDateTimeCreation.sSummary;
            formMetaData.textBoxCreationTime.Text = sDateTime;

            string sJobTime = oMetaData.m_oJobTime.nHour.ToString() + ":";
            sJobTime += oMetaData.m_oJobTime.nMinute.ToString() + ":";
            sJobTime += oMetaData.m_oJobTime.nSecond.ToString();
            formMetaData.textBoxJobTime.Text = sJobTime;

            formMetaData.textBoxJobName.Text = oMetaData.m_sJobName;
            formMetaData.textBoxSoftwareID.Text = oMetaData.m_sSoftwareID;
            formMetaData.textBoxVersionLetter.Text = oMetaData.m_sVersionLetter;
            formMetaData.numericSoftwareVersion.Value = oMetaData.m_nVersionNumber;
            formMetaData.richTextBoxHostComputer.Text = oMetaData.m_sHostComputer;
            formMetaData.richTextBoxWarning.Text = oMetaData.m_sWarning;

            formMetaData.richTextBoxComments.Text = oMetaData.m_sComment;
            formMetaData.richTextBoxDescription.Text = oMetaData.m_sDescription;

            formMetaData.textBoxDocumentName.Text = oMetaData.m_sDocumentName;
            formMetaData.textBoxPageName.Text = oMetaData.m_sPageName;
            formMetaData.numericPageNumber.Value = oMetaData.m_nPageNumber;
            formMetaData.numericPagesCount.Value = oMetaData.m_nPagesCount;

            formMetaData.textBoxExposureCompenstion.Text = oMetaData.m_sExposureCompensation;
            formMetaData.textBoxExposureProgram.Text = oMetaData.m_sExposureProgram;
            formMetaData.textBoxExposureTime.Text = oMetaData.m_sExposureTime;

            formMetaData.textBoxShutterSpeed.Text = oMetaData.m_sShutterSpeed;
            formMetaData.textBoxLensAperture.Text = oMetaData.m_sLensAperture;
            formMetaData.textBoxFocalLength.Text = oMetaData.m_sFocalLength;
            formMetaData.textBoxFNumber.Text = oMetaData.m_sFNumber;
            formMetaData.textBoxISOSpeed.Text = oMetaData.m_nISOSpeed.ToString();
            formMetaData.textBoxMeteringMode.Text = oMetaData.m_sMeteringMode;
            formMetaData.textBoxFlashMode.Text = oMetaData.m_sFlashMode;
            formMetaData.textBoxColorSpace.Text = oMetaData.m_sColorSpace;
            formMetaData.textBoxBitDepth.Text = oMetaData.m_nBitDepth.ToString();

            formMetaData.ShowDialog();
        }

        private bool ParseXML(string sXML)
        {
            XmlDocument oDocument = new XmlDocument();
            oDocument.LoadXml(sXML);

            XmlNode oMainNode = oDocument.ChildNodes[0];
            ReadNode(out oMetaData.m_sTitle, oMainNode, 0);
            ReadNode(out oMetaData.m_sAuthor, oMainNode, 1);
            ReadNode(out oMetaData.m_sDescription, oMainNode, 2);

            ReadNode(out oMetaData.m_oDateTimeCreation.nYear,    oMainNode.ChildNodes[3], 0);
            ReadNode(out oMetaData.m_oDateTimeCreation.nMonth,   oMainNode.ChildNodes[3], 1);
            ReadNode(out oMetaData.m_oDateTimeCreation.nDay,     oMainNode.ChildNodes[3], 2);
            ReadNode(out oMetaData.m_oDateTimeCreation.nHour,    oMainNode.ChildNodes[3], 3);
            ReadNode(out oMetaData.m_oDateTimeCreation.nMinute,  oMainNode.ChildNodes[3], 4);
            ReadNode(out oMetaData.m_oDateTimeCreation.nSecond,  oMainNode.ChildNodes[3], 5);
            ReadNode(out oMetaData.m_oDateTimeCreation.sSummary, oMainNode.ChildNodes[3], 6);

            ReadNode(out oMetaData.m_sCopyright,      oMainNode, 4);
            ReadNode(out oMetaData.m_sDisclaimer,     oMainNode, 5);
            ReadNode(out oMetaData.m_sComment,        oMainNode, 6);
            ReadNode(out oMetaData.m_sEquipmentType,  oMainNode, 7);
            ReadNode(out oMetaData.m_sEquipmentModel, oMainNode, 8);
            ReadNode(out oMetaData.m_oJobTime.nHour,  oMainNode.ChildNodes[9], 0);
            ReadNode(out oMetaData.m_oJobTime.nMinute,oMainNode.ChildNodes[9], 1);
            ReadNode(out oMetaData.m_oJobTime.nSecond,oMainNode.ChildNodes[9], 2);
            ReadNode(out oMetaData.m_sJobName,        oMainNode, 10);
            ReadNode(out oMetaData.m_sSoftwareID,     oMainNode, 11);
            ReadNode(out oMetaData.m_nVersionNumber,  oMainNode, 12);
            ReadNode(out oMetaData.m_sVersionLetter,  oMainNode, 13);
            ReadNode(out oMetaData.m_sHostComputer,   oMainNode, 14);
            ReadNode(out oMetaData.m_sWarning,        oMainNode, 15);
            ReadNode(out oMetaData.m_sDocumentName,   oMainNode, 16);
            ReadNode(out oMetaData.m_sPageName,       oMainNode, 17);
            ReadNode(out oMetaData.m_nPageNumber,     oMainNode, 18);
            ReadNode(out oMetaData.m_nPagesCount,     oMainNode, 19);

            ReadNode(out oMetaData.m_sExposureTime,         oMainNode, 20);
            ReadNode(out oMetaData.m_sExposureCompensation, oMainNode, 21);
            ReadNode(out oMetaData.m_sExposureProgram,      oMainNode, 22);
            ReadNode(out oMetaData.m_sShutterSpeed,         oMainNode, 23);
            ReadNode(out oMetaData.m_sLensAperture,         oMainNode, 24);
            ReadNode(out oMetaData.m_sFocalLength,          oMainNode, 25);
            ReadNode(out oMetaData.m_sFNumber,              oMainNode, 26);
            ReadNode(out oMetaData.m_nISOSpeed,             oMainNode, 27);
            ReadNode(out oMetaData.m_sMeteringMode,         oMainNode, 28);
            ReadNode(out oMetaData.m_sFlashMode,            oMainNode, 29);
            ReadNode(out oMetaData.m_sColorSpace,           oMainNode, 30);
            ReadNode(out oMetaData.m_nBitDepth,             oMainNode, 31);

            return true;
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

        private void ReadNode(out double dResult, XmlNode oNode, int nNodeIndex)
        {
            string sRes = oNode.ChildNodes[nNodeIndex].InnerText;
            if (null == sRes)
                dResult = 0;
            else
                dResult = double.Parse(sRes);
        }

        private void ClearMetaData()
        {
            oMetaData.m_sAuthor = "";
            oMetaData.m_sTitle  = "";
            oMetaData.m_sDescription = "";
            oMetaData.m_oDateTimeCreation.nYear = 0;
            oMetaData.m_oDateTimeCreation.nMonth = 0;
            oMetaData.m_oDateTimeCreation.nDay = 0;
            oMetaData.m_oDateTimeCreation.nHour = 0;
            oMetaData.m_oDateTimeCreation.nMinute = 0;
            oMetaData.m_oDateTimeCreation.nSecond = 0;
            oMetaData.m_oDateTimeCreation.sSummary = "";
            oMetaData.m_sCopyright = "";
            oMetaData.m_sDisclaimer = "";
            oMetaData.m_sComment = "";
            oMetaData.m_sEquipmentType = "";
            oMetaData.m_sEquipmentModel = "";
            oMetaData.m_oJobTime.nHour = 0 ;
            oMetaData.m_oJobTime.nMinute = 0;
            oMetaData.m_oJobTime.nSecond = 0;
            oMetaData.m_sJobName = "";
            oMetaData.m_sSoftwareID = "";
            oMetaData.m_nVersionNumber = 0;
            oMetaData.m_sVersionLetter = "";
            oMetaData.m_sHostComputer = "";
            oMetaData.m_sWarning = "";
            oMetaData.m_sDocumentName = "";
            oMetaData.m_sPageName = "";
            oMetaData.m_nPageNumber = 0;
            oMetaData.m_nPagesCount = 0;

            oMetaData.m_sExposureTime = "";
            oMetaData.m_sExposureCompensation = "";
            oMetaData.m_sExposureProgram = "";
            oMetaData.m_sShutterSpeed = "";
            oMetaData.m_sLensAperture = "";
            oMetaData.m_sFocalLength = "";
            oMetaData.m_sFNumber = "";
            oMetaData.m_nISOSpeed = 0;
            oMetaData.m_sMeteringMode = "";
            oMetaData.m_sFlashMode = "";
            oMetaData.m_sColorSpace = "";

            oMetaData.m_nBitDepth = 1;
        }
    }
}
