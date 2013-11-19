using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Images
{
    public class Image
    {
        public Image(XmlNode anchorXml, XmlNamespaceManager manager)
        {
            if (anchorXml != null)
            {
                anchor = (XmlElement)anchorXml;
                from = (XmlElement)anchorXml.SelectSingleNode("xdr:from", manager);
                to = (XmlElement)anchorXml.SelectSingleNode("xdr:to", manager);
                pic = (XmlElement)anchorXml.SelectSingleNode("xdr:pic", manager);
                clientData = (XmlElement)anchorXml.SelectSingleNode("xdr:clientData", manager);

            }
            image_doc_path = null;
            image_url = null;
            image_guid = Guid.NewGuid().ToString();
            sheet_id = 0;

            xmlManager = manager;
        }

        private XmlNamespaceManager xmlManager;
        private XmlDocument doc;
        public XmlElement anchor, from, to, pic, clientData;
        public string image_doc_path;
        public string image_url;
        public string image_guid;
        public int sheet_id;

        #region xdr:from
        public int from_col
        {
            get
            {
                int retVal = -1;
                if (from != null)
                {
                    XmlElement col = (XmlElement)from.SelectSingleNode("xdr:col", xmlManager);
                    if (col != null)
                    {
                        retVal = int.Parse(col.InnerText);
                    }
                }

                return retVal;
            }
            set
            {
                if (from != null)
                {
                    XmlElement col = (XmlElement)from.SelectSingleNode("xdr:col", xmlManager);
                    if (col != null)
                    {
                        col.InnerText = value.ToString();
                    }
                }
            }
        }

        public int from_colOff
        {
            get
            {
                int retVal = -1;
                if (from != null)
                {
                    XmlElement colOff = (XmlElement)from.SelectSingleNode("xdr:colOff", xmlManager);
                    if (colOff != null)
                    {
                        retVal = int.Parse(colOff.InnerText);
                    }
                }

                return retVal;
            }
            set
            {
                if (from != null)
                {
                    XmlElement colOff = (XmlElement)from.SelectSingleNode("xdr:colOff", xmlManager);
                    if (colOff != null)
                    {
                        colOff.InnerText = value.ToString();
                    }
                }
            }
        }

        public int from_row
        {
            get
            {
                int retVal = -1;
                if (from != null)
                {
                    XmlElement row = (XmlElement)from.SelectSingleNode("xdr:row", xmlManager);
                    if (row != null)
                    {
                        retVal = int.Parse(row.InnerText);
                    }
                }

                return retVal;
            }
            set
            {
                if (from != null)
                {
                    XmlElement row = (XmlElement)from.SelectSingleNode("xdr:row", xmlManager);
                    if (row != null)
                    {
                        row.InnerText = value.ToString();
                    }
                }
            }
        }

        public int from_rowOff
        {
            get
            {
                int retVal = -1;
                if (from != null)
                {
                    XmlElement rowOff = (XmlElement)from.SelectSingleNode("xdr:rowOff", xmlManager);
                    if (rowOff != null)
                    {
                        retVal = int.Parse(rowOff.InnerText);
                    }
                }

                return retVal;
            }
            set
            {
                if (from != null)
                {
                    XmlElement rowOff = (XmlElement)from.SelectSingleNode("xdr:rowOff", xmlManager);
                    if (rowOff != null)
                    {
                        rowOff.InnerText = value.ToString();
                    }
                }
            }
        }
        #endregion

        #region xdr:to
        public int to_col
        {
            get
            {
                int retVal = -1;
                if (to != null)
                {
                    XmlElement col = (XmlElement)to.SelectSingleNode("xdr:col", xmlManager);
                    if (col != null)
                    {
                        retVal = int.Parse(col.InnerText);
                    }
                }

                return retVal;
            }
            set
            {
                if (to != null)
                {
                    XmlElement col = (XmlElement)to.SelectSingleNode("xdr:col", xmlManager);
                    if (col != null)
                    {
                        col.InnerText = value.ToString();
                    }
                }
            }
        }

        public int to_colOff
        {
            get
            {
                int retVal = -1;
                if (to != null)
                {
                    XmlElement colOff = (XmlElement)to.SelectSingleNode("xdr:colOff", xmlManager);
                    if (colOff != null)
                    {
                        retVal = int.Parse(colOff.InnerText);
                    }
                }

                return retVal;
            }
            set
            {
                if (to != null)
                {
                    XmlElement colOff = (XmlElement)to.SelectSingleNode("xdr:colOff", xmlManager);
                    if (colOff != null)
                    {
                        colOff.InnerText = value.ToString();
                    }
                }
            }
        }

        public int to_row
        {
            get
            {
                int retVal = -1;
                if (to != null)
                {
                    XmlElement row = (XmlElement)to.SelectSingleNode("xdr:row", xmlManager);
                    if (row != null)
                    {
                        retVal = int.Parse(row.InnerText);
                    }
                }

                return retVal;
            }
            set
            {
                if (to != null)
                {
                    XmlElement row = (XmlElement)to.SelectSingleNode("xdr:row", xmlManager);
                    if (row != null)
                    {
                        row.InnerText = value.ToString();
                    }
                }
            }
        }

        public int to_rowOff
        {
            get
            {
                int retVal = -1;
                if (to != null)
                {
                    XmlElement rowOff = (XmlElement)to.SelectSingleNode("xdr:rowOff", xmlManager);
                    if (rowOff != null)
                    {
                        retVal = int.Parse(rowOff.InnerText);
                    }
                }

                return retVal;
            }
            set
            {
                if (to != null)
                {
                    XmlElement rowOff = (XmlElement)to.SelectSingleNode("xdr:rowOff", xmlManager);
                    if (rowOff != null)
                    {
                        rowOff.InnerText = value.ToString();
                    }
                }
            }
        }
        #endregion

        #region xdr:pic
        public int pic_id
        {
            get
            {
                int retVal = -1;
                if (pic != null)
                {
                    XmlElement id = (XmlElement)pic.SelectSingleNode("xdr:nvPicPr/xdr:cNvPr", xmlManager);
                    if (id != null)
                        retVal = int.Parse(id.GetAttribute("id"));
                }

                return retVal;
            }
            set
            {
                if (pic != null)
                {
                    XmlElement id = (XmlElement)pic.SelectSingleNode("xdr:nvPicPr/xdr:cNvPr", xmlManager);
                    if (id != null)
                        id.SetAttribute("id", value.ToString());
                }
            }
        }

        public string pic_name
        {
            get
            {
                string retVal = "";
                if (pic != null)
                {
                    XmlElement name = (XmlElement)pic.SelectSingleNode("xdr:nvPicPr/xdr:cNvPr", xmlManager);
                    if (name != null)
                        retVal = name.GetAttribute("name");
                }

                return retVal;
            }
            set
            {
                if (pic != null)
                {
                    XmlElement name = (XmlElement)pic.SelectSingleNode("xdr:nvPicPr/xdr:cNvPr", xmlManager);
                    if (name != null)
                        name.SetAttribute("name", value.ToString());
                }
            }
        }

        public string pic_rid
        {
            get
            {
                string retVal = "";
                if (pic != null)
                {
                    XmlElement rid = (XmlElement)pic.SelectSingleNode("xdr:blipFill/a:blip", xmlManager);
                    if (rid != null)
                        retVal = rid.GetAttribute("embed", xmlManager.LookupNamespace("mainr"));
                }

                return retVal;
            }
            set
            {
                if (pic != null)
                {
                    XmlElement rid = (XmlElement)pic.SelectSingleNode("xdr:blipFill/a:blip", xmlManager);
                    if (rid != null)
                        rid.SetAttribute("r:embed", value);
                }
            }
        }
        #endregion

        #region image_xOff
        public int image_xOff
        {
            get
            {
                int retVal = -1;
                if (pic != null)
                {
                    XmlElement cw = (XmlElement)pic.SelectSingleNode("xdr:spPr/a:xfrm/a:off", xmlManager);
                    if (cw != null)
                    {
                        retVal = int.Parse(cw.GetAttribute("x"));
                    }
                }
                return retVal;
            }

            set
            {
                if (pic != null)
                {
                    XmlElement cw = (XmlElement)pic.SelectSingleNode("xdr:spPr/a:xfrm/a:off", xmlManager);
                    if (cw != null)
                    {
                        cw.SetAttribute("x", value.ToString());
                    }
                }
            }
        }
        #endregion

        #region image_yOff
        public int image_yOff
        {
            get
            {
                int retVal = -1;
                if (pic != null)
                {
                    XmlElement ch = (XmlElement)pic.SelectSingleNode("xdr:spPr/a:xfrm/a:off", xmlManager);
                    if (ch != null)
                    {
                        retVal = int.Parse(ch.GetAttribute("y"));
                    }
                }
                return retVal;
            }

            set
            {
                if (pic != null)
                {
                    XmlElement ch = (XmlElement)pic.SelectSingleNode("xdr:spPr/a:xfrm/a:off", xmlManager);
                    if (ch != null)
                    {
                        ch.SetAttribute("y", value.ToString());
                    }
                }
            }
        }
        #endregion

        #region image_width
        public int image_width
        {
            get
            {
                int retVal = -1;
                if (pic != null)
                {
                    XmlElement cw = (XmlElement)pic.SelectSingleNode("xdr:spPr/a:xfrm/a:ext", xmlManager);
                    if (cw != null)
                    {
                        retVal = int.Parse(cw.GetAttribute("cx"));
                    }
                }
                return retVal;
            }

            set
            {
                if (pic != null)
                {
                    XmlElement cw = (XmlElement)pic.SelectSingleNode("xdr:spPr/a:xfrm/a:ext", xmlManager);
                    if (cw != null)
                    {
                        cw.SetAttribute("cx", value.ToString());
                    }
                }
            }
        }
        #endregion

        #region image_height
        public int image_height
        {
            get
            {
                int retVal = -1;
                if (pic != null)
                {
                    XmlElement ch = (XmlElement)pic.SelectSingleNode("xdr:spPr/a:xfrm/a:ext", xmlManager);
                    if (ch != null)
                    {
                        retVal = int.Parse(ch.GetAttribute("cy"));
                    }
                }
                return retVal;
            }

            set
            {
                if (pic != null)
                {
                    XmlElement ch = (XmlElement)pic.SelectSingleNode("xdr:spPr/a:xfrm/a:ext", xmlManager);
                    if (ch != null)
                    {
                        ch.SetAttribute("cy", value.ToString());
                    }
                }
            }
        }
        #endregion
    }

    public class DrawingTransFromJson
    {
        public DrawingTransFromJson()
        {
            chart = new ChartTransFromJson();
        }

        public int object_id;
        public string image_url;
        public string object_guid;
        public int sheet_id;

        public int object_xOff;
        public int object_yOff;

        public int object_width;
        public int object_height;

        public int from_col;
        public int from_colOff;
        public int from_row;
        public int from_rowOff;

        public int to_col;
        public int to_colOff;
        public int to_row;
        public int to_rowOff;

        public ChartTransFromJson chart { get; set; }
    }

    public class ChartAxisFromJson
    {
        public bool show;
        public bool grid;
        public string title;        
    }

    public class ChartX : ChartAxisFromJson { }
    public class ChartY : ChartAxisFromJson { }

    public class ChartLegendFromJson
    {
        public bool show;
        public string legendPos;
        public bool overlay;
    }

    public class ChartTransFromJson
    {
        public ChartTransFromJson()
        {
            seriesList = new List<ChartSeriesTransFromJson>();
        }

        public string chartType;
        public string seriesType;
        public string title;
        public ChartX xAxis;
        public ChartY yAxis;
        public ChartLegendFromJson legend;
        public List<ChartSeriesTransFromJson> seriesList;
    }

    public class ChartSeriesTransFromJson
    {
        public ChartSeriesTransFromJson()
        {
            valuesList = new List<double>();
            catList = new List<string>();            
        }

        public bool showVal;
        public string referredrange;
        public string name;
        public string markerType;
        public int markerSize;
        public string color;
        public string borderColor;
        public List<double> valuesList;
        public List<string> catList;
    }
}
