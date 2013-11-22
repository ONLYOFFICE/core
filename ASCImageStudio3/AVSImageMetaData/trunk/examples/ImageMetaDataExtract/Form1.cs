using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Runtime.InteropServices;

using System.Xml.Schema;
using System.Xml.Serialization;


namespace ImageMetaDataExtract
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private string[] MetaDataXMLList;

        private void button1_Click(object sender, EventArgs e)
        {
            comboBox1.Items.Clear();
            listView1.Items.Clear();


            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            AVSImageMetaData.IImageMetaDataParser DataParser;
            try
            {
                DataParser = new AVSImageMetaData.CImageMetaDataParserClass();
                DataParser.ParseFile(openFileDialog1.FileName, 0);
            }
            catch (COMException exc)
            {
                MessageBox.Show("Error: 0x" + exc.ErrorCode.ToString("X") + "\n" + exc.Message, "Open file error");
                return;
            }

            if (DataParser.AvailDataTypeCount <= 0)
                return;

            MetaDataXMLList = new string[DataParser.AvailDataTypeCount];
            for (uint i = 0; i < DataParser.AvailDataTypeCount; i++)
            {
                try
                {
                    MetaDataXMLList[i] = (DataParser.MetaData(i) as AVSImageMetaData.IImageMetaDataCommon).GetCommonXML(0);
                    string typeName = (DataParser.MetaData(i) as AVSImageMetaData.IImageMetaDataCommon).GetAdditionalParam("TypeName").ToString();
                    comboBox1.Items.Add(typeName);
                    System.IO.File.AppendAllText("c:/MetaDataCommon_" + typeName + ".xml", MetaDataXMLList[i], Encoding.Unicode);
                    System.IO.File.AppendAllText("c:/MetaData_" + typeName + ".xml", (DataParser.MetaData(i) as AVSImageMetaData.IImageMetaDataCommon).GetXML(0), Encoding.Unicode);
                }
                catch (COMException ex)
                {
                    string s1 = ex.Message;
                }
            }
            comboBox1.SelectedIndex = 0;
            return;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            listView1.Items.Clear();
            ImageMetaDataCommonWrapper.AVSImageMetaDataCommon ImageMetaDataCommon = null;
            XmlSerializer ImageMetaDataCommonSerializer = new XmlSerializer(typeof(ImageMetaDataCommonWrapper.AVSImageMetaDataCommon));
            using (var _StrReader = new System.IO.StringReader(MetaDataXMLList[comboBox1.SelectedIndex]))
            {
                try
                {
                    ImageMetaDataCommon = ImageMetaDataCommonSerializer.Deserialize(_StrReader) as ImageMetaDataCommonWrapper.AVSImageMetaDataCommon;
                }
                catch (System.InvalidOperationException ex)
                {
                    // не получилось разобрать XML
                    MessageBox.Show(ex.Message + "\n" + ex.InnerException.Message);
                    return;
                }
                if (ImageMetaDataCommon.CommonTag != null)
                    for (int j = 0; j < ImageMetaDataCommon.CommonTag.Count(); j++)
                    {
                        ListViewItem item = new ListViewItem();
                        item.Text = (ImageMetaDataCommon.CommonTag[j].Name);
                        item.SubItems.Add(ImageMetaDataCommon.CommonTag[j].Value);
                        item.SubItems.Add(ImageMetaDataCommon.CommonTag[j].Description);
                        if (ImageMetaDataCommon.CommonTag[j].Category != null && ImageMetaDataCommon.CommonTag[j].Category.ToString().Length != 0)
                            item.SubItems.Add(ImageMetaDataCommon.CommonTag[j].Category.ToString());
                        else
                            item.SubItems.Add("common");

                        listView1.Items.Add(item);
                    }

            }


        }
    }
}
