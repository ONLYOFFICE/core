using System.Xml;
using System.Collections;
using System;
using System.IO;
using System.Globalization;
using System.Text.RegularExpressions;
using CSharpEval;



namespace CleverAge.OdfConverter.OdfConverterLib
{

    /// <summary>
    /// An <c>XmlWriter</c> implementation for characters post processings
    public class OoxCharactersPostProcessor : AbstractPostProcessor
    {

        private const string NAMESPACE = "http://schemas.openxmlformats.org/wordprocessingml/2006/main";
        private string[] RUN_PROPERTIES = { "ins", "del", "moveFrom", "moveTo", "rStyle", "rFonts", "b", "bCs", "i", "iCs", "caps", "smallCaps", "strike", "dstrike", "outline", "shadow", "emboss", "imprint", "noProof", "snapToGrid", "vanish", "webHidden", "color", "spacing", "w", "kern", "position", "sz", "szCs", "highlight", "u", "effect", "bdr", "shd", "fitText", "vertAlign", "rtl", "cs", "em", "lang", "eastAsianLayout", "specVanish", "oMath", "rPrChange" };

        private static char[] PONCTUATION = { '\u002E', '\u0021', '\u003F', '\u002C', '\u003A', '\u003B', '\u0028', '\u0029', '\u002D' };

        // series of symbol that do not use reverse direction
        private static char[] HEBREW_DIRECT_SYMBOLS = 
            { '\u05B0', '\u05B1', '\u05B2', '\u05B3', '\u05B4', '\u05B5', '\u05B6', '\u05B7', '\u05B8' };

        private static char[] HEBREW_SYMBOLS = 
            { '\u05B9', '\u05BA', '\u05BB', '\u05BC', '\u05BD', '\u05BE', '\u05BF',
			'\u05C0', '\u05C1', '\u05C2', '\u05C3',
			'\u05D0', '\u05D1', '\u05D2', '\u05D3', '\u05D4', '\u05D5', '\u05D6', '\u05D7', '\u05D8', '\u05D9', '\u05DA', '\u05DB', '\u05DC', '\u05DD', '\u05DE', '\u05DF',
			'\u05E0', '\u05E1', '\u05E2', '\u05E3', '\u05E4', '\u05E5', '\u05E6', '\u05E7', '\u05E8', '\u05E9', '\u05EA',
			'\u05F0', '\u05F1', '\u05F2', '\u05F3', '\u05F4' };

        private static char[] ARABIC_BASIC_SYMBOLS = 
            { '\u060C', '\u061B', '\u061F',
			'\u0621', '\u0622', '\u0623', '\u0624', '\u0625', '\u0626', '\u0627', '\u0628', '\u0629', '\u062A', '\u062B', '\u062C', '\u062D', '\u062E', '\u062F',
			'\u0630', '\u0631', '\u0632', '\u0633', '\u0634', '\u0635', '\u0636', '\u0637', '\u0638', '\u0639', '\u063A',
			'\u0640', '\u0641', '\u0642', '\u0643', '\u0644', '\u0645', '\u0646', '\u0647', '\u0648', '\u0649', '\u064A', '\u064B', '\u064C', '\u064D', '\u064E', '\u064F',
			'\u0650', '\u0651', '\u0652', '\u0653', '\u0654', '\u0655' };

        private static char[] ARABIC_EXTENDED_SYMBOLS = 
            { '\u0660', '\u0661', '\u0662', '\u0663', '\u0664', '\u0625', '\u0666', '\u0667', '\u0668', '\u0669', '\u066A', '\u066B', '\u066C', '\u066D', '\u066E', '\u066F',
			'\u0670', '\u0641', '\u0672', '\u0673', '\u0674', '\u0675', '\u0676', '\u0677', '\u0678', '\u0679', '\u067A', '\u067B', '\u067C', '\u067D', '\u067E', '\u067F',
			'\u0680', '\u0681', '\u0682', '\u0683', '\u0684', '\u0685', '\u0686', '\u0687', '\u0688', '\u0689', '\u068A', '\u068B', '\u068C', '\u068D', '\u068E', '\u068F',
			'\u0690', '\u0691', '\u0692', '\u0693', '\u0694', '\u0695', '\u0696', '\u0697', '\u0698', '\u0699', '\u069A', '\u069B', '\u069C', '\u069D', '\u069E', '\u069F',
			'\u06A0', '\u06A1', '\u06A2', '\u06A3', '\u06A4', '\u06A5', '\u06A6', '\u06A7', '\u06A8', '\u06A9', '\u06AA', '\u06AB', '\u06AC', '\u06AD', '\u06AE', '\u06AF',
			'\u06B0', '\u06B1', '\u06B2', '\u06B3', '\u06B4', '\u06B5', '\u06B6', '\u06B7', '\u06B8', '\u06B9', '\u06BA', '\u06BB', '\u06BC', '\u06BD', '\u06BE', '\u06BF',
			'\u06C0', '\u06C1', '\u06C2', '\u06C3', '\u06C4', '\u06C5', '\u06C6', '\u06C7', '\u06C8', '\u06C9', '\u06CA', '\u06CB', '\u06CC', '\u06CD', '\u06CE', '\u06CF',
			'\u06D0', '\u06D1', '\u06D2', '\u06D3', '\u06D4', '\u06D5', '\u06D6', '\u06D7', '\u06D8', '\u06D9', '\u06DA', '\u06DB', '\u06DC', '\u06DD', '\u06DE', '\u06DF',
			'\u06E0', '\u06E1', '\u06E2', '\u06E3', '\u06E4', '\u06E5', '\u06E6', '\u06E7', '\u06E8', '\u06E9', '\u06EA', '\u06EB', '\u06EC', '\u06ED',
			'\u06F0', '\u06F1', '\u06F2', '\u06F3', '\u06F4', '\u06F5', '\u06F6', '\u06F7', '\u06F8', '\u06F9', '\u06FA', '\u06FB', '\u06FC', '\u06FD', '\u06FE' };

        private Stack currentNode;
        private Stack store;
        private bool isRightToLeft;


        public OoxCharactersPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.currentNode = new Stack();
            this.store = new Stack();
        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {
            Element e = null;
            if (NAMESPACE.Equals(ns) && "r".Equals(localName))
            {
                e = new Run(prefix, localName, ns);
            }
            else
            {
                e = new Element(prefix, localName, ns);
            }

            this.currentNode.Push(e);

            if (InRun())
            {
                StartStoreElement();
            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
        }


        public override void WriteEndElement()
        {
            if (IsRun())
            {
                WriteStoredRun();
            }
            if (InRun())
            {
                EndStoreElement();
            }
            else
            {
                this.nextWriter.WriteEndElement();
            }
            this.currentNode.Pop();
        }


        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            this.currentNode.Push(new Attribute(prefix, localName, ns));

            if (InRun())
            {
                StartStoreAttribute();
            }
            else
            {
                this.nextWriter.WriteStartAttribute(prefix, localName, ns);
            }
        }


        public override void WriteEndAttribute()
        {
            if (InRun())
            {
                EndStoreAttribute();
            }
            else
            {
                this.nextWriter.WriteEndAttribute();
            }
            this.currentNode.Pop();
        }


        public override void WriteString(string text)
        {
            if (InRun())
            {
                StoreString(text);
            }
            else if (text.Contains("cxnSp"))
            {
                this.nextWriter.WriteString(EvalExpression(text));
            }
            // added by vipul for Shape Rotation
            //Start
            else if (text.Contains("draw-transform"))
            {
                this.nextWriter.WriteString(EvalRotationExpression(text));
            }
            else if (text.Contains("group-svgXYWidthHeight"))
            {
                this.nextWriter.WriteString(EvalGroupExpression(text));
            }
            //End 
            //Shadow calculation
            else if (text.Contains("a-outerShdw-dist") || text.Contains("a-outerShdw-dir"))
            {
                this.nextWriter.WriteString(EvalShadowExpression(text));
            }
            // This condition is to check for hyperlink relative path 
            else if (text.Contains("hyperlink-path"))
            {
                this.nextWriter.WriteString(EvalHyperlinkPath(text));
            }
            //Image Cropping Calculation Added by Sonata-15/11/2007
            else if (text.Contains("image-properties"))
            {
                EvalImageCropping(text);
            }
            //Callout Adjustments Calculation Added by Sonata
            else if (text.Contains("Callout-DirectAdj") || text.Contains("Callout-DirectLine"))
            {
                this.nextWriter.WriteString(EvalCalloutAdjsExpn(text));
            }
            else if (IsDate())
            {
                try
                {
                    DateTime dateTime = DateTime.Parse(text, CultureInfo.InvariantCulture, DateTimeStyles.AssumeLocal);
                    this.nextWriter.WriteString(dateTime.ToUniversalTime().ToString("s") + "Z");
                }
                catch (FormatException)
                {
                }
            }
            else
            {
                this.nextWriter.WriteString(text);
            }
        }


        /*
         * General methods
         * Private method to evaluate an expression with trignametric functions.
         */
        private string EvalExpression(string text)
        {
            string[] arrVal = text.Split(':');

            string attVal = "";
            if (arrVal.Length >= 6)
            {
                double x1 = Double.Parse(arrVal[2], System.Globalization.CultureInfo.InvariantCulture);
                double x2 = Double.Parse(arrVal[3], System.Globalization.CultureInfo.InvariantCulture);
                double y1 = Double.Parse(arrVal[4], System.Globalization.CultureInfo.InvariantCulture);
                double y2 = Double.Parse(arrVal[5], System.Globalization.CultureInfo.InvariantCulture);

                double xCenter = (x1 + x2) * 360000 / 2;
                double yCenter = (y1 + y2) * 360000 / 2;

                double angle, x, y;
                int flipH = 0;
                int flipV = 0;

                angle = Math.Atan2((y2 - y1), (x2 - x1));

                double angleRd = angle / Math.PI * 180;

                if (angleRd < 0)
                {
                    angleRd += 360;
                }

                int sector = (int)(angleRd / 45) + 1;

                if ((sector == 1) || (sector == 8))
                {
                    angle = 0;
                }
                else if ((sector == 2) || (sector == 6))
                {
                    angle = 270;
                }
                else if ((sector == 3) || (sector == 7))
                {
                    angle = 90;
                }
                else if ((sector == 4) || (sector == 5))
                {
                    angle = 180;
                }

                if (arrVal[1] == "rot")
                {

                    attVal = (angle * 60000).ToString();
                }

                if ((sector == 2) || (sector == 7))
                {
                    flipH = 1;
                }

                if ((sector == 4) || (sector == 6) || (sector == 7) || (sector == 8))
                {
                    flipV = 1;
                }

                if (arrVal[1] == "flipH")
                {
                    attVal = flipH.ToString();
                }
                if (arrVal[1] == "flipV")
                {
                    attVal = flipV.ToString();
                }

                angleRd = angle / 180 * Math.PI;

                double cxby2 = (Math.Cos(angleRd) * (x2 - x1) + Math.Sin(angleRd) * (y2 - y1)) / 2 * 360000;
                double cyby2 = (Math.Sin(angleRd) * (x1 - x2) + Math.Cos(angleRd) * (y2 - y1)) / 2 * 360000;
                double cx = 2 * Math.Round(cxby2);
                double cy = 2 * Math.Round(cyby2);
                if (flipH == 1)
                {
                    cx = -1 * cx;
                }
                if (flipV == 1)
                {
                    cy = -1 * cy;
                }
                if (arrVal[1] == "cx")
                {
                    if (text.Contains("true"))
                        cx = Math.Round(cx / 1588);
                    attVal = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", cx);
                }
                if (arrVal[1] == "cy")
                {
                    if (text.Contains("true"))
                        cy = Math.Round(cy / 1588);
                    attVal = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", cy);
                }
                if (arrVal[1] == "x")
                {
                    x = Math.Round(xCenter - cx / 2);
                    if (text.Contains("true"))
                        x = Math.Round(x / 1588);
                    attVal = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", x);
                }
                if (arrVal[1] == "y")
                {
                    y = Math.Round(yCenter - cy / 2);
                    if (text.Contains("true"))
                        y = Math.Round(y / 1588);
                    attVal = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", y);

                }
            }
            return attVal;
        }
        // added by vipul for Shape Rotation
        //Start
        private string EvalRotationExpression(string text)
        {
            string[] arrVal = new string[7];

            string strReturn = "";
            arrVal = text.Split(':');
            string strXY = arrVal[1];
            Double dblRadius = 0.0;
            Double dblXc = 0.0;
            Double dblYc = 0.0;
            Double dblalpha = 0.0;
            Double dblbeta = 0.0;
            Double dblgammaDegree = 0.0;
            Double dblgammaR = 0.0;
            Double dblX2 = 0.0;
            Double dblY2 = 0.0;
            Double centreX = 0.0;
            Double centreY = 0.0;
            Double dblRotation = 0.0;

            if (arrVal.Length == 7)
            {
                double arrValueWidth = Double.Parse(arrVal[2], System.Globalization.CultureInfo.InvariantCulture);
                double arrValueHeight = Double.Parse(arrVal[3], System.Globalization.CultureInfo.InvariantCulture);
                double arrValueX2 = Double.Parse(arrVal[4], System.Globalization.CultureInfo.InvariantCulture);
                double arrValueY2 = Double.Parse(arrVal[5], System.Globalization.CultureInfo.InvariantCulture);
                double arrValueAngle = Double.Parse(arrVal[6], System.Globalization.CultureInfo.InvariantCulture);

                if (arrVal[0].Contains("draw-transform"))
                {

                    centreX = 360000.00 * arrValueWidth;
                    centreY = 360000.00 * arrValueHeight;

                    dblRadius = Math.Sqrt(centreX * centreX + centreY * centreY) / 2.0;

                    if (Math.Abs(centreY / 2) > 0)
                    {
                        dblbeta = Math.Atan(Math.Abs(centreX / 2) / Math.Abs(centreY / 2)) * 180.00 / Math.PI;
                    }
                    dblalpha = -180.00 * arrValueAngle / Math.PI;

                    if (Math.Abs(dblbeta - dblalpha) > 0)
                    {
                        dblgammaDegree = (dblbeta - dblalpha) % ((int)((dblbeta - dblalpha) / Math.Abs(dblbeta - dblalpha)) * 360) + 90;
                    }
                    dblgammaR = (360.00 - dblgammaDegree) / 180.00 * Math.PI;
                    dblXc = (arrValueX2 * 360036.00) - (dblRadius * Math.Cos(dblgammaR));
                    dblYc = (arrValueY2 * 360036.00) - (dblRadius * Math.Sin(dblgammaR));

                    dblX2 = dblXc - centreX / 2.0;
                    dblY2 = dblYc - centreY / 2.0;
                    dblRotation = (int)Math.Round(-1 * (arrValueAngle * 180.00 / Math.PI) * 60000.00);

                }

            }
            if (strXY.Contains("XGroup"))
                dblX2 = dblX2 / 1588;

            if (strXY.Contains("YGroup"))
                dblY2 = dblY2 / 1588;

            if (strXY.Contains("X"))
            {

                strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", (int)Math.Round(dblX2));

            }
            if (strXY.Contains("Y"))
            {
                strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", (int)Math.Round(dblY2));

            }
            if (strXY.Contains("ROT"))
            {
                strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", dblRotation);

            }
            return strReturn;
        }
        private string EvalGroupExpression(string text)
        {

            int intCount;
            int intNextCnt;
            double tempVal;
            double dblX;
            double dblY;
            double dblWidth;
            double dblHeight;
            double dblRot;

            double dblValueNext;

            ArrayList arrListX = new ArrayList();
            ArrayList arrListY = new ArrayList();

            string strReturn = "";
            string[] arrVal = text.Split('@');

            if (arrVal[1] == "")
                return "0";

            for (intCount = 1; intCount < arrVal.Length - 1; intCount++)
            {
                string[] arrCords = arrVal[intCount].Split(':');

                if (arrCords[0] == "")
                    arrCords[0] = "0";
                if (arrCords[1] == "")
                    arrCords[1] = "0";
                if (arrCords[2] == "")
                    arrCords[2] = "0";
                if (arrCords[3] == "")
                    arrCords[3] = "0";
                if (arrCords[4] == "")
                    arrCords[4] = "0";

                dblX = Double.Parse(arrCords[0], System.Globalization.CultureInfo.InvariantCulture);
                dblY = Double.Parse(arrCords[1], System.Globalization.CultureInfo.InvariantCulture);
                dblWidth = Double.Parse(arrCords[2], System.Globalization.CultureInfo.InvariantCulture);
                dblHeight = Double.Parse(arrCords[3], System.Globalization.CultureInfo.InvariantCulture);
                dblRot = Double.Parse(arrCords[4], System.Globalization.CultureInfo.InvariantCulture);

                tempVal = dblX;

                arrListX.Add(tempVal);

                tempVal = tempVal + Math.Cos(dblRot) * dblWidth;
                arrListX.Add(tempVal);

                tempVal = tempVal + Math.Sin(dblRot) * dblHeight;
                arrListX.Add(tempVal);

                tempVal = dblX + Math.Sin(dblRot) * dblHeight;
                arrListX.Add(tempVal);


                tempVal = dblY;
                arrListY.Add(tempVal);

                tempVal = tempVal - Math.Sin(dblRot) * dblWidth;
                arrListY.Add(tempVal);

                tempVal = tempVal + Math.Cos(dblRot) * dblHeight;
                arrListY.Add(tempVal);

                tempVal = dblY + Math.Cos(dblRot) * dblHeight;
                arrListY.Add(tempVal);

            }

            string strXY = arrVal[0];

            double maxX = 0;
            double maxY = 0;

            double minX = 0;
            double minY = 0;

            long ChOffX;
            long ChOffY;
            long ChExtX;
            long ChExtY;
            long OffX;
            long OffY;
            long ExtX;
            long ExtY;


            minX = (double)arrListX[0];
            maxX = (double)arrListX[0];

            maxY = (double)arrListY[0];
            minY = (double)arrListY[0];
            for (intNextCnt = 1; intNextCnt < arrListX.Count - 1; intNextCnt++)
            {
                dblValueNext = (double)arrListX[intNextCnt];

                if (dblValueNext <= minX)
                {
                    minX = dblValueNext;
                }
                if (dblValueNext >= maxX)
                {
                    maxX = dblValueNext;
                }
            }


            for (intNextCnt = 1; intNextCnt < arrListY.Count - 1; intNextCnt++)
            {
                dblValueNext = (double)arrListY[intNextCnt];

                if (dblValueNext <= minY)
                {
                    minY = dblValueNext;
                }
                if (dblValueNext >= maxY)
                {
                    maxY = dblValueNext;
                }

            }
            //Bounding Box
            OffX = (long)Math.Round(minX * 360000);
            OffY = (long)Math.Round(minY * 360000);
            ExtX = (long)Math.Round((Math.Abs(maxX - minX)) * 360000);
            ExtY = (long)Math.Round(Math.Abs((maxY - minY)) * 360000);

            //Child Bounding Box
            ChOffX = OffX / 1588;
            ChOffY = OffY / 1588;
            ChExtX = ExtX / 1588;
            ChExtY = ExtY / 1588;

            if (strXY.Contains("InnerGroup"))
            {
                if (strXY.Contains("onlyX"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChOffX);
                else if (strXY.Contains("onlyY"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChOffY);
                else if (strXY.Contains("onlyCX"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChExtX);
                else if (strXY.Contains("onlyCY"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChExtY);
                else if (strXY.Contains("onlyChX"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChOffX);
                else if (strXY.Contains("onlyChY"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChOffY);
                else if (strXY.Contains("onlyChCX"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChExtX);

                else if (strXY.Contains("onlyChCY"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChExtY);



            }
            else
            {

                if (strXY.Contains("onlyX"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", OffX);
                else if (strXY.Contains("onlyY"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", OffY);
                else if (strXY.Contains("onlyCX"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ExtX);
                else if (strXY.Contains("onlyCY"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ExtY);
                else if (strXY.Contains("onlyChX"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChOffX);
                else if (strXY.Contains("onlyChY"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChOffY);
                else if (strXY.Contains("onlyChCX"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChExtX);
                else if (strXY.Contains("onlyChCY"))
                    strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", ChExtY);

            }


            return strReturn;
        }

        //End 

        // added for Shadow calculation
        private string EvalShadowExpression(string text)
        {
            string[] arrVal = new string[2];
            arrVal = text.Split(':');
            Double x = 0;
            if (arrVal.Length == 3)
            {
                double arrValue1 = Double.Parse(arrVal[1].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                double arrValue2 = Double.Parse(arrVal[2].ToString(), System.Globalization.CultureInfo.InvariantCulture);

                if (arrVal[0].Contains("a-outerShdw-dist"))
                {
                    x = Math.Sqrt(arrValue1 * arrValue1 + arrValue2 * arrValue2);
                    x = Math.Round(x * 360000);
                    //OpenXML Validation Error-fix
                    //added by yeswanth for conformance ST_PositiveCoordinate
                    if (x > 27273042316900)
                        x = 27273042316900;
                }
                if (arrVal[0].Contains("a-outerShdw-dir"))
                {
                    x = Math.Atan(arrValue2 / arrValue1);
                    x = x * (180.0 / Math.PI);
                    x = Math.Abs(Math.Round(x * 60000));
                    //0 - 90 degrees
                    //if (arrValue1 > 0 && arrValue2 > 0)
                    //{
                    //    x = Math.Atan(arrValue2 / arrValue1);
                    //    x = x * (180.0 / Math.PI);
                    //    x = Math.Abs(Math.Round(x * 60000));
                    //}
                    //181 - 270 degrees
                    if (arrValue1 < 0 && arrValue2 < 0)
                    {
                        x = Math.Abs(10800000 + x);
                    }
                    //271 - 359 degrees
                    if (arrValue1 > 0 && arrValue2 < 0)
                    {
                        x = Math.Abs(21600000 - x);
                    }
                    //91 - 180 degrees
                    if (arrValue1 < 0 && arrValue2 > 0)
                    {
                        x = Math.Abs(10800000 - x);
                    }
                }
            }
            string strRet = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", x);


            return strRet;
        }

        //Resolve relative path to absolute path
        private string EvalHyperlinkPath(string text)
        {
            string[] arrVal = new string[1];
            arrVal = text.Split(':');
            string source = arrVal[1].ToString();
            string address = "";


            if (arrVal.Length == 2)
            {
                string returnInputFilePath = "";

                // for Commandline tool
                for (int i = 0; i < Environment.GetCommandLineArgs().Length; i++)
                {
                    if (Environment.GetCommandLineArgs()[i].ToString().ToUpper() == "/I")
                        returnInputFilePath = Path.GetDirectoryName(Environment.GetCommandLineArgs()[i + 1]);
                }

                //for addins
                if (returnInputFilePath == "")
                {
                    returnInputFilePath = Environment.CurrentDirectory;
                }
                if (source != "")
                {
                    string linkPathLocation = Path.GetFullPath(Path.Combine(returnInputFilePath, source.Remove(0, 3))).Replace("/", "//").Replace(" ", "%20");
                    address = "file:///" + linkPathLocation;
                }
            }
            return address.ToString();
        }
        //end


        //Image Cropping Added by Sonata-15/11/2007
        private void EvalImageCropping(string text)
        {
            string[] arrVal = new string[6];
            arrVal = text.Split(':');
            string source = arrVal[1].ToString();
            double top = Double.Parse(arrVal[2].ToString(), System.Globalization.CultureInfo.InvariantCulture);
            double right = Double.Parse(arrVal[3].ToString(), System.Globalization.CultureInfo.InvariantCulture);
            double bottom = Double.Parse(arrVal[4].ToString(), System.Globalization.CultureInfo.InvariantCulture);
            double left = Double.Parse(arrVal[5].ToString(), System.Globalization.CultureInfo.InvariantCulture);


            string tempFileName = AbstractConverter.inputTempFileName.ToString();
            ZipResolver zipResolverObj = new ZipResolver(tempFileName);
            OdfZipUtils.ZipArchiveWriter zipobj = new OdfZipUtils.ZipArchiveWriter(zipResolverObj);
            string widht_height_res = zipobj.ImageCopyBinary(source);
            zipResolverObj.Dispose();
            zipobj.Close();


            string[] arrValues = new string[3];
            arrValues = widht_height_res.Split(':');
            double width = Double.Parse(arrValues[0].ToString(), System.Globalization.CultureInfo.InvariantCulture);
            double height = Double.Parse(arrValues[1].ToString(), System.Globalization.CultureInfo.InvariantCulture);
            double res = Double.Parse(arrValues[2].ToString(), System.Globalization.CultureInfo.InvariantCulture);

            double cx = width * 2.54 / res;
            double cy = height * 2.54 / res;

            int pptLeft = (int)(left * 100000 / cx);
            int pptRight = (int)(right * 100000 / cx);
            int pptTop = (int)(top * 100000 / cy);
            int pptBottom = (int)(bottom * 100000 / cy);


            WriteStartAttribute("l");
            this.WriteString(string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", pptLeft));
            WriteEndAttribute();
            WriteStartAttribute("r");
            this.WriteString(string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", pptRight));
            WriteEndAttribute();
            WriteStartAttribute("t");
            this.WriteString(string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", pptTop));
            WriteEndAttribute();
            WriteStartAttribute("b");
            this.WriteString(string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", pptBottom));
            WriteEndAttribute();
        }
        //end

        //Callout Adjustments Calculation Added by Sonata
        private string EvalCalloutAdjsExpn(string text)
        {
            string[] arrVal = text.Split('@');
            string[] arrValCallStr = arrVal[0].Split(':');
            string[] arrCallModfr = arrVal[1].Split(' ');

            int val1 = 0;

            string callAdj = (arrValCallStr[0].ToString());

            double CX = Double.Parse(arrValCallStr[1], System.Globalization.CultureInfo.InvariantCulture);
            double CY = Double.Parse(arrValCallStr[2], System.Globalization.CultureInfo.InvariantCulture);
            double X = Double.Parse(arrValCallStr[3], System.Globalization.CultureInfo.InvariantCulture);
            double Y = Double.Parse(arrValCallStr[4], System.Globalization.CultureInfo.InvariantCulture);
            string mirrorH = (arrValCallStr[5]);
            string mirrorV = (arrValCallStr[6]);
            double angle = Double.Parse(arrValCallStr[7], System.Globalization.CultureInfo.InvariantCulture);

            CX = (360000.00 * CX);
            CY = (360000.00 * CY);

            X = (360000.00 * X);
            Y = (360000.00 * Y);

            int flipH;
            if (arrValCallStr[5].ToString().Trim() == "true")
            {
                flipH = 1;
            }
            else
            {
                flipH = 0;
            }

            int flipV;
            if (arrValCallStr[6].ToString().Trim() == "true")
            {
                flipV = 1;
            }
            else
            {
                flipV = 0;
            }

            double xCenter = (X + CX / 2);
            double yCenter = (Y + CY / 2);

            double xCtrBy2;
            if (flipH == 1)
            {
                xCtrBy2 = ((-1) * (CX / 2));
            }
            else
            {
                xCtrBy2 = (CX / 2);
            }

            double yCtrBy2;
            if (flipV == 1)
            {
                yCtrBy2 = ((-1) * (CY / 2));
            }
            else
            {
                yCtrBy2 = (CY / 2);
            }

            double ang;
            if (angle < 0)
            {
                ang = (-1 * angle);
            }
            else
            {
                ang = angle;
            }

            double X1;
            X1 = (xCenter - Math.Cos(ang) * xCtrBy2 + Math.Sin(ang) * yCtrBy2);

            double X2;
            X2 = (xCenter + Math.Cos(ang) * xCtrBy2 - Math.Sin(ang) * yCtrBy2);

            X1 = Math.Round((X1 / 360000), 3);

            X2 = Math.Round((X2 / 360000), 3);


            double Y1;
            Y1 = (yCenter - Math.Sin(ang) * xCtrBy2 - Math.Cos(ang) * yCtrBy2);

            double Y2;
            Y2 = (yCenter + Math.Sin(ang) * xCtrBy2 + Math.Cos(ang) * yCtrBy2);

            Y1 = Math.Round((Y1 / 360000), 3);

            Y2 = Math.Round((Y2 / 360000), 3);

            double width;
            width = (X1 - X2);

            double height;
            height = (Y1 - Y2);


            if ((callAdj.ToString().Trim() == "Callout-DirectAdj1Notlinefmla1") || (callAdj.ToString().Trim() == "Callout-DirectAdj2Notlinefmla2"))
            {
                double viewDX = 0.0;
                double viewDY = 0.0;

                if (arrCallModfr[0] != "")
                {
                    viewDX = Double.Parse(arrCallModfr[0], System.Globalization.CultureInfo.InvariantCulture);
                }
                if (arrCallModfr[1] != "")
                {
                    viewDY = Double.Parse(arrCallModfr[1], System.Globalization.CultureInfo.InvariantCulture);
                }

                if (callAdj.ToString().Trim() == "Callout-DirectAdj1Notlinefmla1")
                {
                    double viewWidth = 21600;

                    double dxFinal;
                    dxFinal = (width * (viewDX / viewWidth));

                    double dxPos;
                    dxPos = (dxFinal - width / 2);

                    double fml1;
                    fml1 = (dxPos / width * 100000);

                    int FML1 = (int)fml1;

                    val1 = FML1;
                }

                if (callAdj.ToString().Trim() == "Callout-DirectAdj2Notlinefmla2")
                {
                    double viewHeight = 21600;

                    double dyFinal;
                    dyFinal = (height * (viewDY / viewHeight));

                    double dyPos;
                    dyPos = (dyFinal - height / 2);

                    double fml2;
                    fml2 = (dyPos / height * 100000);

                    int FML2 = (int)fml2;

                    val1 = FML2;
                }
            }

            if (callAdj.ToString().Trim().Contains("Callout-DirectLine"))
            {
                double viewDX = 0.0;
                double viewDY = 0.0;
                double viewDX1 = 0.0;
                double viewDY1 = 0.0;
                double viewDX2 = 0.0;
                double viewDY2 = 0.0;
                double viewDX3 = 0.0;
                double viewDY3 = 0.0;

                if ((callAdj.ToString().Trim() == "Callout-DirectLine1Adj1fmla1") || (callAdj.ToString().Trim() == "Callout-DirectLine1Adj2fmla2") || (callAdj.ToString().Trim() == "Callout-DirectLine1Adj3fmla3") || (callAdj.ToString().Trim() == "Callout-DirectLine1Adj4fmla4"))
                {
                    if (arrCallModfr[0] != "")
                    {
                        viewDY1 = Double.Parse(arrCallModfr[0], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[1] != "")
                    {
                        viewDX1 = Double.Parse(arrCallModfr[1], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[2] != "")
                    {
                        viewDY = Double.Parse(arrCallModfr[2], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[3] != "")
                    {
                        viewDX = Double.Parse(arrCallModfr[3], System.Globalization.CultureInfo.InvariantCulture);
                    }
                }

                if ((callAdj.ToString().Trim() == "Callout-DirectLine2Adj1fmla1") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj2fmla2") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj3fmla3") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj4fmla4") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj5fmla5") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj6fmla6"))
                {
                    if (arrCallModfr[0] != "")
                    {
                        viewDY2 = Double.Parse(arrCallModfr[0], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[1] != "")
                    {
                        viewDX2 = Double.Parse(arrCallModfr[1], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[2] != "")
                    {
                        viewDY1 = Double.Parse(arrCallModfr[2], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[3] != "")
                    {
                        viewDX1 = Double.Parse(arrCallModfr[3], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[4] != "")
                    {
                        viewDY = Double.Parse(arrCallModfr[4], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[5] != "")
                    {
                        viewDX = Double.Parse(arrCallModfr[5], System.Globalization.CultureInfo.InvariantCulture);
                    }
                }

                if ((callAdj.ToString().Trim() == "Callout-DirectLine3Adj1fmla1") || (callAdj.ToString().Trim() == "Callout-DirectLine3Adj2fmla2") || (callAdj.ToString().Trim() == "Callout-DirectLine3Adj3fmla3") || (callAdj.ToString().Trim() == "Callout-DirectLine3Adj4fmla4") || (callAdj.ToString().Trim() == "Callout-DirectLine3Adj5fmla5") || (callAdj.ToString().Trim() == "Callout-DirectLine3Adj6fmla6") || (callAdj.ToString().Trim() == "Callout-DirectLine3Adj7fmla7") || (callAdj.ToString().Trim() == "Callout-DirectLine3Adj8fmla8"))
                {
                    if (arrCallModfr[0] != "")
                    {
                        viewDY3 = Double.Parse(arrCallModfr[0], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[1] != "")
                    {
                        viewDX3 = Double.Parse(arrCallModfr[1], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[2] != "")
                    {
                        viewDY2 = Double.Parse(arrCallModfr[2], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[3] != "")
                    {
                        viewDX2 = Double.Parse(arrCallModfr[3], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[4] != "")
                    {
                        viewDY1 = Double.Parse(arrCallModfr[4], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[5] != "")
                    {
                        viewDX1 = Double.Parse(arrCallModfr[5], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[6] != "")
                    {
                        viewDY = Double.Parse(arrCallModfr[6], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrCallModfr[7] != "")
                    {
                        viewDX = Double.Parse(arrCallModfr[7], System.Globalization.CultureInfo.InvariantCulture);
                    }
                }

                double viewWidth = 21600;
                double viewHeight = 21600;

                double vdyFinal3 = viewDY3 * 1000;
                double dyFinal3;
                dyFinal3 = (height * (vdyFinal3 / viewHeight));
                double dyPos3;
                dyPos3 = (dyFinal3 - height / 2);
                double fml8;
                fml8 = (dyPos3 / height * 100);
                int FML8 = (int)fml8;

                double vdxFinal3 = viewDX3 * 1000;
                double dxFinal3;
                dxFinal3 = (width * (vdxFinal3 / viewWidth));
                double dxPos3;
                dxPos3 = (dxFinal3 - width / 2);
                double fml7;
                fml7 = (dxPos3 / width * 100);
                int FML7 = (int)fml7;

                double vdyFinal2 = viewDY2 * 1000;
                double dyFinal2;
                dyFinal2 = (height * (vdyFinal2 / viewHeight));
                double dyPos2;
                dyPos2 = (dyFinal2 - height / 2);
                double fml6;
                fml6 = (dyPos2 / height * 100);
                int FML6 = (int)fml6;

                double vdxFinal2 = viewDX2 * 1000;
                double dxFinal2;
                dxFinal2 = (width * (vdxFinal2 / viewWidth));
                double dxPos2;
                dxPos2 = (dxFinal2 - width / 2);
                double fml5;
                fml5 = (dxPos2 / width * 100);
                int FML5 = (int)fml5;

                double vdyFinal1 = viewDY1 * 1000;
                double dyFinal1;
                dyFinal1 = (height * (vdyFinal1 / viewHeight));
                double dyPos1;
                dyPos1 = (dyFinal1 - height / 2);
                double fml4;
                fml4 = (dyPos1 / height * 100);
                int FML4 = (int)fml4;

                double vdxFinal1 = viewDX1 * 1000;
                double dxFinal1;
                dxFinal1 = (width * (vdxFinal1 / viewWidth));
                double dxPos1;
                dxPos1 = (dxFinal1 - width / 2);
                double fml3;
                fml3 = (dxPos1 / width * 100);
                int FML3 = (int)fml3;

                double vdyFinal = viewDY * 1000;
                double dyFinal;
                dyFinal = (height * (vdyFinal / viewHeight));
                double dyPos;
                dyPos = (dyFinal - height / 2);
                double fml2;
                fml2 = (dyPos / height * 100);
                int FML2 = (int)fml2;

                double vdxFinal = viewDX * 1000;
                double dxFinal;
                dxFinal = (width * (vdxFinal / viewWidth));
                double dxPos;
                dxPos = (dxFinal - width / 2);
                double fml1;
                fml1 = (dxPos / width * 100);
                int FML1 = (int)fml1;

                if (callAdj.ToString().Trim() == "Callout-DirectLine3Adj8fmla8")
                {
                    val1 = FML8;
                }

                if (callAdj.ToString().Trim() == "Callout-DirectLine3Adj7fmla7")
                {
                    val1 = FML7;
                }

                if ((callAdj.ToString().Trim() == "Callout-DirectLine3Adj6fmla6") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj6fmla6"))
                {
                    val1 = FML6;
                }

                if ((callAdj.ToString().Trim() == "Callout-DirectLine3Adj5fmla5") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj5fmla5"))
                {
                    val1 = FML5;
                }

                if ((callAdj.ToString().Trim() == "Callout-DirectLine3Adj4fmla4") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj4fmla4") || (callAdj.ToString().Trim() == "Callout-DirectLine1Adj4fmla4"))
                {
                    val1 = FML4;
                }

                if ((callAdj.ToString().Trim() == "Callout-DirectLine3Adj3fmla3") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj3fmla3") || (callAdj.ToString().Trim() == "Callout-DirectLine1Adj3fmla3"))
                {
                    val1 = FML3;
                }

                if ((callAdj.ToString().Trim() == "Callout-DirectLine3Adj2fmla2") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj2fmla2") || (callAdj.ToString().Trim() == "Callout-DirectLine1Adj2fmla2"))
                {
                    val1 = FML2;
                }

                if ((callAdj.ToString().Trim() == "Callout-DirectLine3Adj1fmla1") || (callAdj.ToString().Trim() == "Callout-DirectLine2Adj1fmla1") || (callAdj.ToString().Trim() == "Callout-DirectLine1Adj1fmla1"))
                {
                    val1 = FML1;
                }
            }

            return "val " + string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", val1);
        }
        //End

        public void WriteStoredRun()
        {
            Element e = (Element)this.store.Peek();

            if (e is Run)
            {
                Run r = (Run)e;
                if (r.HasReversedText)
                {
                    isRightToLeft = true;
                    SplitRun(r);
                    isRightToLeft = false;
                }
                else
                {
                    if (this.store.Count < 2)
                    {
                        if (r.HasText)
                        {
                            if (r.GetChild("t", NAMESPACE) != null)
                                r.ReplaceFirstTextChild(this.ReplaceSoftHyphens(r.GetChild("t", NAMESPACE).GetTextChild(), r));
                        }
                        r.Write(nextWriter);
                    }
                }
            }
            else
            {
                if (this.store.Count < 2)
                {
                    e.Write(nextWriter);
                }
            }
        }

        private void StartStoreElement()
        {
            Element element = (Element)this.currentNode.Peek();

            if (this.store.Count > 0)
            {
                Element parent = (Element)this.store.Peek();
                parent.AddChild(element);
            }

            this.store.Push(element);
        }


        private void EndStoreElement()
        {
            Element e = (Element)this.store.Pop();
        }


        private void StartStoreAttribute()
        {
            Element parent = (Element)store.Peek();
            Attribute attr = (Attribute)this.currentNode.Peek();
            parent.AddAttribute(attr);
            this.store.Push(attr);
        }


        private void StoreString(string text)
        {
            Node node = (Node)this.currentNode.Peek();

            if (node is Element)
            {
                Element element = (Element)this.store.Peek();
                element.AddChild(text);
            }
            //Sona: Code for Rotation
            else
            {
                Attribute attr = (Attribute)store.Peek();
                if (text.Contains("draw-transform"))
                {
                    char[] separator = new char[1] { ';' };
                    string[] splitStr = text.Split(separator);
                    string rotStr = "";
                    string xStr = "";
                    string yStr = "";
                    Double angle;
                    Double x = 0.0;
                    Double y = 0.0;
                    //Sona: Different culture defect fix
                    string strReturn = "";
                    foreach (string str in splitStr)
                    {
                        if (str.StartsWith("rotation"))
                        {
                            rotStr = str.Replace("rotation:", "");
                            rotStr.Trim();
                            //Convert emu to degrees
                            angle = Convert.ToDouble(EvalRotationExpression(rotStr)) / 60000;
                            //Sona: Different culture defect fix
                            strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", angle);
                            text = text.Replace(rotStr, strReturn);

                        }
                        if (str.StartsWith("margin-left"))
                        {
                            xStr = str.Replace("margin-left:", "");
                            xStr.Trim();
                            //Sona: Defect #1830350
                            if (xStr != "" && xStr != "0")
                            {
                                //convert emu to points
                                x = (Convert.ToInt32(EvalRotationExpression(xStr)) * 72) / (360000 * 2.54);
                                //Sona: Different culture defect fix
                                strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", (int)Math.Round(x));
                                text = text.Replace(xStr, strReturn + "pt");
                            }

                        }
                        if (str.StartsWith("margin-top"))
                        {
                            yStr = str.Replace("margin-top:", "");
                            yStr.Trim();
                            //Sona: Defect #1830350
                            if (yStr != "" && yStr != "0")
                            {
                                //convert emu to points
                                y = (Convert.ToInt32(EvalRotationExpression(yStr)) * 72) / (360000 * 2.54);
                                //Sona: Different culture defect fix
                                strReturn = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0}", (int)Math.Round(y));
                                text = text.Replace(yStr, strReturn + "pt");
                            }

                        }
                    }
                }
                //Start of RefNo-1
                else if (text.Contains("CustShpWrdFreFrm"))
                {
                    text = getSahpePath(text.Substring(16));
                }
                //End of RefNo-1
                attr.Value += text;
            }
        }


        private void EndStoreAttribute()
        {
            this.store.Pop();
        }


        private void SplitRun(Run r)
        {
            Run extractedRun = new Run("w", "r", NAMESPACE);
            Element rPr = r.GetChild("rPr", NAMESPACE);
            Element rPr0 = null;

            if (rPr != null)
            {
                rPr0 = rPr.Clone();
            }
            else
            {
                rPr0 = new Element("w", "rPr", NAMESPACE);
            }

            // get first substring of run of a unique type, and retrieve it from run
            TextProperties extractedText = ExtractText(r);
            if (extractedText.IsReverse)
            {
                rPr0.AddChild(new Element("w", "rtl", NAMESPACE));
                if (rPr0.GetChild("b", NAMESPACE) != null && rPr0.GetChild("b", NAMESPACE).GetAttribute("val", NAMESPACE).Value.Equals("on"))
                {
                    Element elt = new Element("w", "bCs", NAMESPACE);
                    elt.AddAttribute(new Attribute("w", "val", "on", NAMESPACE));
                    rPr0.AddChild(elt);
                }
                if (rPr0.GetChild("i", NAMESPACE) != null && rPr0.GetChild("i", NAMESPACE).GetAttribute("val", NAMESPACE).Value.Equals("on"))
                {
                    Element elt = new Element("w", "iCs", NAMESPACE);
                    elt.AddAttribute(new Attribute("w", "val", "on", NAMESPACE));
                    rPr0.AddChild(elt);
                }
                string fontSize = null;
                if (rPr0.GetChild("sz", NAMESPACE) != null)
                    fontSize = rPr0.GetChild("sz", NAMESPACE).GetAttribute("val", NAMESPACE).Value;
                if (fontSize != null)
                {
                    Element elt = new Element("w", "szCs", NAMESPACE);
                    elt.AddAttribute(new Attribute("w", "val", fontSize, NAMESPACE));
                    rPr0.AddChild(elt);
                }
                rPr0 = this.GetOrderedRunProperties(rPr0);
            }

            extractedRun.AddChild(rPr0);
            Element t = new Element("w", "t", NAMESPACE);
            t.AddAttribute(new Attribute("xml", "space", "preserve", null));
            t.AddChild(extractedText.Content);
            extractedRun.AddChild(t);

            if (this.store.Count < 2)
            {
                extractedRun.Write(nextWriter);
            }
            else
            {
                Element parent = GetParent(r, this.store);
                if (parent != null)
                {
                    parent.AddChild(extractedRun);
                }
            }

            // Check the rest of the content : Run r now contains the remaining text.
            if (r.HasReversedText || r.HasPonctuation)
            {
                SplitRun(r);
            }
            else
            {
                if (this.store.Count < 2)
                {
                    if (r.HasNotEmptyText)
                    {
                        r.Write(nextWriter);
                    }
                    else
                    {
                        bool hasRelevantChild = false;
                        //try to avoid empty runs
                        foreach (Element runChild in r.GetChildElements())
                        {
                            if ("t".Equals(runChild.Name) && NAMESPACE.Equals(runChild.Ns)) { }
                            else if ("rPr".Equals(runChild.Name) && NAMESPACE.Equals(runChild.Ns)) { }
                            else
                            {
                                hasRelevantChild = true;
                                break;
                            }
                        }
                        if (hasRelevantChild)
                            //do not write run, just pop it.
                            this.currentNode.Pop();
                    }
                }
            }

        }

        // Extract the first part of run text that is of one particular type
        private TextProperties ExtractText(Run r)
        {
            TextProperties extractedText = new TextProperties();
            // teake the very first string of run.
            string text = r.GetChild("t", NAMESPACE).GetTextChild();
            int startChar = -1;
            int endChar = -1;
            int startHebrew = -1;
            int startArabic = -1;
            int startExtendedArabic = -1;
            char[] charTable = null;

            // get start of special symbol substring
            if ((startHebrew = text.IndexOfAny(HEBREW_SYMBOLS)) > -1)
            {
                startChar = startHebrew;
                charTable = HEBREW_SYMBOLS;
            }
            if ((startArabic = text.IndexOfAny(ARABIC_BASIC_SYMBOLS)) > -1)
            {
                if (startChar > -1)
                    startChar = System.Math.Min(startChar, startArabic);
                else startChar = startArabic;
                if (startChar.Equals(startArabic))
                    charTable = ARABIC_BASIC_SYMBOLS;
            }
            if ((startExtendedArabic = text.IndexOfAny(ARABIC_EXTENDED_SYMBOLS)) > -1)
            {
                if (startChar > -1)
                    startChar = System.Math.Min(startChar, startExtendedArabic);
                else startChar = startExtendedArabic;
                if (startChar.Equals(startExtendedArabic))
                    charTable = ARABIC_EXTENDED_SYMBOLS;
            }

            if (startChar.Equals(0)) // isolate right to left words
            {
                // get end of special symbol substring
                endChar = this.GetLastIndexOfCharSetInString(text, startChar, charTable);
                // retrieve substring from run
                r.ReplaceFirstTextChild(text.Substring(endChar + 1, text.Length - endChar - 1));
                // return substring. Do not apply character processing to special characters.
                extractedText.Content = text.Substring(0, endChar + 1);
                extractedText.IsReverse = true;
            }
            else if (startChar > 0) // all the rest : left to right words, spaces and ponctuation.
            {
                string remaining = text.Substring(startChar, text.Length - startChar);
                if (text.IndexOf('\u0020').Equals(0) || text.IndexOfAny(PONCTUATION).Equals(0))
                {
                    extractedText.Content = text.Substring(0, 1);
                    r.ReplaceFirstTextChild(text.Substring(1, text.Length - 1));
                    extractedText.IsReverse = isRightToLeft;
                }
                else
                {
                    string begining = text.Substring(0, startChar);
                    // remove trailing spaces and ponctuation from the begining string.

                    // text ending with ponctuation of space
                    if (begining.IndexOf('\u0020').Equals(begining.Length - 1) || begining.IndexOfAny(PONCTUATION).Equals(begining.Length - 1))
                    {
                        begining = TrimPonctuation(begining);
                        r.ReplaceFirstTextChild(text.Substring(begining.Length, text.Length - begining.Length));
                        extractedText.Content = this.ReplaceSoftHyphens(begining, r);
                        extractedText.IsReverse = false;
                    }
                    else
                    {
                        // retrieve substring from run
                        r.ReplaceFirstTextChild(remaining);
                        // return first substring
                        extractedText.Content = this.ReplaceSoftHyphens(begining, r);
                        extractedText.IsReverse = false;
                    }
                }

            }
            else // text that does not have right to left text
            {
                // begining with space or ponctuation => must be cut in two runs
                if (text.IndexOf('\u0020') == 0 || text.IndexOfAny(PONCTUATION) == 0)
                {
                    extractedText.Content = text.Substring(0, 1);
                    r.ReplaceFirstTextChild(text.Substring(1, text.Length - 1));
                    extractedText.IsReverse = isRightToLeft;
                }
                // ending with space or ponctuation => must be cut in two runs
                else if (text.Length > 0 && (text.IndexOf('\u0020').Equals(text.Length - 1) ||
                    text.Substring(text.Length - 1, 1).IndexOfAny(PONCTUATION) == 0))
                {
                    string begining = TrimPonctuation(text);
                    r.ReplaceFirstTextChild(text.Substring(begining.Length, text.Length - begining.Length));
                    extractedText.Content = this.ReplaceSoftHyphens(begining, r);
                    extractedText.IsReverse = false;
                }
                else
                {
                    // text that does not begin or end with ponctiuation or space
                    extractedText.Content = this.ReplaceSoftHyphens(text, r);
                    extractedText.IsReverse = false;
                }
            }

            return extractedText;
        }


        // Find the last character of a certain char-set in a string.
        private int GetLastIndexOfCharSetInString(string text, int startIndex, char[] charTable)
        {
            int i = startIndex;
            while (i < text.Length && this.IsContainedInCharTable((text.Substring(i, 1)), charTable))
            {
                i++;
            }
            return i - 1;
        }

        public string TrimPonctuation(string text)
        {
            string res = text;
            string lastChar = res.Substring(res.Length - 1, 1);
            while (res.Length > 0 && (lastChar.IndexOf('\u0020') == 0 || lastChar.IndexOfAny(PONCTUATION) == 0))
            {
                res = res.Substring(0, res.Length - 1);
                lastChar = res.Substring(res.Length - 1, 1);
            }
            return res;
        }


        // Check if a character is contained in a char-set table
        private bool IsContainedInCharTable(string character, char[] charTable)
        {
            return character.IndexOfAny(charTable) >= 0;
        }


        private string ReplaceSoftHyphens(string text, Run r)
        {
            string substring = "";
            int i = 0;
            if ((i = text.IndexOf('\u00AD')) >= 0)
            {
                substring = this.ReplaceNonBreakingHyphens(text.Substring(0, i), r);
                r.AddChild(new Element("w", "softHyphen", NAMESPACE));
                if (i < text.Length - 1)
                {
                    Element newT = new Element("w", "t", NAMESPACE);
                    newT.AddChild(this.ReplaceSoftHyphens(text.Substring(i + 1, text.Length - i - 1), r));
                    r.AddChild(newT);
                }
            }
            else
            {
                substring = this.ReplaceNonBreakingHyphens(text, r);
            }
            return substring;
        }


        private string ReplaceNonBreakingHyphens(string text, Run r)
        {
            string substring = "";
            int i = 0;
            if ((i = text.IndexOf('\u2011')) >= 0)
            {
                substring = text.Substring(0, i);
                r.AddChild(new Element("w", "noBreakHyphen", NAMESPACE));
                if (i < text.Length - 1)
                {
                    Element newT = new Element("w", "t", NAMESPACE);
                    newT.AddChild(this.ReplaceNonBreakingHyphens(text.Substring(i + 1, text.Length - i - 1), r));
                    r.AddChild(newT);
                }
            }
            else
            {
                substring = text;
            }
            return substring;
        }

        /// <summary>
        /// Returns true if the current node contains a date.
        /// 
        /// This is needed to undo the special post-processing done for XSD times in DOCX->ODT conversion
        /// (ODF only accepts the format YYYY-MM-DDThh:mm:ss without trailing TZD, see OdfCharactersPostProcessor).
        /// 
        /// Additionaly, OOo interpretes times as local times -> we convert from UTC to local time 
        /// (Of course this only gives the correct value if the document is converted in the same time zone 
        /// for which the time value has been written, but it is better than nothing). This allows to keep e.g. the 
        /// correct value for CREATIONDATE fields when not moving within time zones
        /// </summary>
        /// <returns></returns>
        private bool IsDate()
        {
            if (this.currentNode.Count > 0)
            {
                Element element = this.currentNode.Peek() as Element;

                if (element != null
                    && (element.Prefix.Equals("dcterms") && (element.Name.Equals("created") || element.Name.Equals("modified"))
                        || element.Prefix.Equals("cp") && element.Name.Equals("lastPrinted")))
                {
                    return true;
                }
            }
            return false;
        }

        private bool IsRun()
        {
            Node node = (Node)this.currentNode.Peek();
            if (node is Element)
            {
                Element element = (Element)node;
                if ("r".Equals(element.Name) && NAMESPACE.Equals(element.Ns))
                {
                    return true;
                }
            }
            return false;
        }


        private bool InRun()
        {
            return IsRun() || this.store.Count > 0;
        }

        private Element GetParent(Element e, Stack stack)
        {
            IEnumerator objEnum = stack.GetEnumerator();
            while (objEnum.MoveNext())
            {
                Element parent = objEnum.Current as Element;
                if (parent != null)
                {
                    foreach (object child in parent.Children)
                    {
                        if (child == e) // object identity
                        {
                            return parent;
                        }
                    }
                }
            }
            return null;
        }

        protected class Run : Element
        {
            private TextProperties TextProperties;

            public TextProperties TextPr
            {
                get { return TextProperties; }
                set { TextProperties = value; }
            }

            public Run(Element e) : base(e.Prefix, e.Name, e.Ns) { }

            public Run(string prefix, string localName, string ns)
                :
                base(prefix, localName, ns) { }

            public bool HasReversedText
            {
                get
                {
                    return HasReversedTextNode(this);
                }
            }

            public bool HasPonctuation
            {
                get
                {
                    return HasPonctuationNode(this);
                }
            }

            private bool HasReversedTextNode(Element e)
            {
                bool b = false;
                foreach (object node in e.Children)
                {
                    if (node is Element)
                    {
                        Element element = (Element)node;
                        if (element.GetTextChild() != null)
                        {
                            b = b || isReversed(element.GetTextChild());
                        }
                    }
                }
                return b;
            }

            private bool HasPonctuationNode(Element e)
            {
                bool b = false;
                foreach (object node in e.Children)
                {
                    if (node is Element)
                    {
                        Element element = (Element)node;
                        if (element.GetTextChild() != null)
                        {
                            b = b || hasPonctuation(element.GetTextChild());
                        }
                    }
                }
                return b;
            }

            private bool hasPonctuation(string text)
            {
                if (text.IndexOf('\u0020') == 0)
                    return true;
                else if (text.IndexOfAny(PONCTUATION) == 0)
                    return true;
                else if (text.Length > 0)
                {
                    string lastChar = text.Substring(text.Length - 1, 1);
                    if (lastChar.LastIndexOf('\u0020') == 0)
                        return true;
                    else if (lastChar.IndexOfAny(PONCTUATION) == 0)
                        return true;
                    else return false;
                }
                else return false;
            }

            private bool isReversed(string text)
            {
                if (text.IndexOfAny(HEBREW_SYMBOLS) >= 0)
                    return true;
                else if (text.IndexOfAny(ARABIC_BASIC_SYMBOLS) >= 0)
                    return true;
                else if (text.IndexOfAny(ARABIC_EXTENDED_SYMBOLS) >= 0)
                    return true;
                else return false;
            }

            public void ReplaceFirstTextChild(string newText)
            {
                if (this.GetChild("t", NAMESPACE) != null)
                {
                    string oldText = this.GetChild("t", NAMESPACE).GetTextChild();
                    if (!oldText.Equals(newText))
                    {
                        this.GetChild("t", NAMESPACE).Replace(oldText, newText);
                    }
                }
            }

            public bool HasNotEmptyText
            {
                get
                {
                    return HasNotEmptyTextNode(this);
                }
            }


            private static bool HasNotEmptyTextNode(Element e)
            {
                bool b = false;
                foreach (object node in e.Children)
                {
                    if (node is Element)
                    {
                        Element element = (Element)node;
                        if (element.GetTextChild() != null && element.GetTextChild().Length > 0)
                        {
                            b = true;
                        }
                    }
                }
                return b;
            }

            public bool HasText
            {
                get
                {
                    return HasTextNode(this);
                }
            }


            private static bool HasTextNode(Element e)
            {
                bool b = false;
                if ("t".Equals(e.Name) && NAMESPACE.Equals(e.Ns))
                {
                    b = true;
                }
                else
                {

                    foreach (object node in e.Children)
                    {
                        if (node is Element)
                        {
                            b = b || HasTextNode((Element)node);
                        }
                        else
                        {
                            b = true;
                        }
                    }
                }
                return b;
            }

        }

        protected class TextProperties
        {
            private bool isReverse;
            private string content;

            public bool IsReverse
            {
                get { return isReverse; }
                set { isReverse = value; }
            }

            public string Content
            {
                get { return content; }
                set { content = value; }
            }
        }

        private Element GetOrderedRunProperties(Element rPr)
        {
            Element newRPr = new Element(rPr);
            foreach (string propName in RUN_PROPERTIES)
            {
                Element prop = rPr.GetChild(propName, NAMESPACE);
                if (prop != null)
                {
                    newRPr.AddChild(prop);
                }
            }
            return newRPr;
        }

        //Start of RefNo-1
        private string getSahpePath(string strOdfEnhancedPath)
        {
            //adj$modifiers,'###eqn',$drawEqn,'###box',$viewBox,'###path',$enhPath

            string[] stringSeparators = new string[] { "###" };
            string[] strArrShpPth = strOdfEnhancedPath.Split(stringSeparators, StringSplitOptions.None);
            char[] arrOperators = { '*', '/', '+', '-', '(', ')', ',' };

            string strModifiers = strArrShpPth[0].Trim();
            string strDrawEqn = strArrShpPth[1].Trim();
            string strViewBox = strArrShpPth[2].Trim();
            string strEnhPath = strArrShpPth[3].Trim();

            string[] strArrAdj = strModifiers.Split(' ');

            string[] strArrBox = strViewBox.Split(' ');

            if (strArrBox.Length == 4)
            {
                //replace L T R B. W H
                strDrawEqn = strDrawEqn.Replace("left", strArrBox[0].Trim());
                strDrawEqn = strDrawEqn.Replace("top", strArrBox[1].Trim());
                strDrawEqn = strDrawEqn.Replace("right", strArrBox[2].Trim());
                strDrawEqn = strDrawEqn.Replace("bottom", strArrBox[3].Trim());
                strDrawEqn = strDrawEqn.Replace("width", strArrBox[2].Trim());
                strDrawEqn = strDrawEqn.Replace("height", strArrBox[3].Trim());
                //TO DO : replace 'xstretch'|'ystretch'|'hasstroke'|'hasfill'|'logwidth'|'logheight'
            }

            string[] strArrEqn = strDrawEqn.Split('|');
            string[] strEqnSlvd = { "" };
            string strFinalPath = "";

            strEnhPath = Regex.Replace(strEnhPath, @"[M]\b", "m");
            strEnhPath = Regex.Replace(strEnhPath, @"[L]\b", "l");
            strEnhPath = Regex.Replace(strEnhPath, @"[C]\b", "c");
            strEnhPath = Regex.Replace(strEnhPath, @"[Z]\b", "x");
            strEnhPath = Regex.Replace(strEnhPath, @"[N]\b", "e");
            // Vipul:
            strEnhPath = Regex.Replace(strEnhPath, @"[F]\b", "");
            strEnhPath = Regex.Replace(strEnhPath, @"[S]\b", "ns");
            strEnhPath = Regex.Replace(strEnhPath, @"[A]\b", "at");
            strEnhPath = Regex.Replace(strEnhPath, @"[T]\b", "ae");
            strEnhPath = Regex.Replace(strEnhPath, @"[U]\b", "al");
            strEnhPath = Regex.Replace(strEnhPath, @"[B]\b", "ar");
            strEnhPath = Regex.Replace(strEnhPath, @"[W]\b", "wa");
            strEnhPath = Regex.Replace(strEnhPath, @"[V]\b", "wr");
            strEnhPath = Regex.Replace(strEnhPath, @"[Q]\b", "qb");
            strEnhPath = Regex.Replace(strEnhPath, @"[X]\b", "qx");
            strEnhPath = Regex.Replace(strEnhPath, @"[Y]\b", "qy");

            strEnhPath = strEnhPath.Replace("  ", " ");
            string[] strArrPath = strEnhPath.Split(' ');

            if (strEnhPath != "" && (strEnhPath.Contains("$") || strEnhPath.Contains("?")))
            {
                strEqnSlvd = getEquationVals(strArrAdj, strArrEqn);
            }
            for (int intPathCnt = 0; intPathCnt < strArrPath.Length; intPathCnt++)
            {
                string strElement = strArrPath[intPathCnt].ToString().Trim();
                if ((Regex.IsMatch(strArrPath[intPathCnt].ToString().Trim(), @"[^0-9.]")))
                {
                    if (strElement.StartsWith("$"))
                    {
                        strFinalPath = strFinalPath +
                            Convert.ToInt64(
                                Convert.ToDouble(
                                    strArrAdj[
                                        Convert.ToInt16(
                                            strElement.Remove(0, 1), System.Globalization.CultureInfo.InvariantCulture)].Trim(),
                                            System.Globalization.CultureInfo.InvariantCulture)
                                        , System.Globalization.CultureInfo.InvariantCulture).ToString(System.Globalization.CultureInfo.InvariantCulture) + ",";
                    }
                    else if (strElement.StartsWith("?"))
                    {
                        while (strElement.Contains("?"))
                        {
                            strElement = strEqnSlvd[Convert.ToInt16(strElement.Remove(0, 2), System.Globalization.CultureInfo.InvariantCulture)];

                            if (strElement.Contains("if"))
                            {
                                strElement = GetIfCmdVal(strElement, strEqnSlvd);
                            }
                            else if (strElement.Contains("?") && strElement.IndexOfAny(arrOperators) >= 0)
                            {
                                strElement = GetEqnVal(strElement, strEqnSlvd);
                            }
                            if (strElement.Trim().StartsWith("-"))
                            {
                                strElement = "(" + strElement + ")";
                            }
                        }
                        strElement = strElement.Replace("(", "").Replace(")", "").Trim();
                        strElement = Convert.ToInt64(Convert.ToDouble(strElement, System.Globalization.CultureInfo.InvariantCulture), System.Globalization.CultureInfo.InvariantCulture).ToString(System.Globalization.CultureInfo.InvariantCulture);
                        strFinalPath = strFinalPath + strElement + ",";
                    }
                    else
                    {
                        if (strFinalPath.EndsWith(","))
                        {
                            strFinalPath = strFinalPath.Substring(0, strFinalPath.Length - 1) + strElement;
                        }
                        else
                        {
                            strFinalPath = strFinalPath + strElement;
                        }
                    }
                }
                else
                {
                    strFinalPath = strFinalPath + strElement + ",";
                }
            }
            return strFinalPath;
        }

        private string[] getEquationVals(string[] strArrAdj, string[] strArrEqn)
        {
            char[] arrOperators = { '*', '/', '+', '-', '(', ')', ',' };
            string[] strArrEqnResult = new string[strArrEqn.Length];

            for (int intCnt = 0; intCnt < strArrEqn.Length; intCnt++)
            {
                ArrayList arlEqnPrts = new ArrayList();
                ArrayList arlEqnRplPrts = new ArrayList();
                string strCurEqn = strArrEqn[intCnt].Trim();
                string[] strArrExpElements = strCurEqn.Split(arrOperators, StringSplitOptions.RemoveEmptyEntries);

                for (int i = 0; i < strArrExpElements.Length; i++)
                {
                    if (Regex.IsMatch(strArrExpElements[i].ToString().Trim(), @"[^0-9.]"))
                    {
                        string strElement = strArrExpElements[i].ToString().Trim();
                        if (strElement.StartsWith("$"))
                        {
                            if (!arlEqnPrts.Contains(strElement))
                            {
                                if (Convert.ToInt16(strElement.Remove(0, 1), System.Globalization.CultureInfo.InvariantCulture) < strArrAdj.Length)
                                {
                                    string strEqnRpl = strArrAdj[Convert.ToInt16(strElement.Remove(0, 1), System.Globalization.CultureInfo.InvariantCulture)].Trim();
                                    if (strEqnRpl.Contains("-"))
                                    {
                                        arlEqnRplPrts.Add("(" + strEqnRpl + ")");
                                    }
                                    else
                                    {
                                        arlEqnRplPrts.Add(strEqnRpl);
                                    }
                                }
                                else
                                {
                                    arlEqnRplPrts.Add("0");
                                }
                                arlEqnPrts.Add(strElement);
                            }
                        }
                        else if (strElement.StartsWith("?"))
                        {
                            if (!arlEqnPrts.Contains(strElement))
                            {
                                if (Convert.ToInt16(strElement.Remove(0, 2), System.Globalization.CultureInfo.InvariantCulture) < strArrEqnResult.Length)
                                {
                                    if (Convert.ToInt16(strElement.Remove(0, 2), System.Globalization.CultureInfo.InvariantCulture) < intCnt)
                                    {
                                        string strEqnRpl = strArrEqnResult[Convert.ToInt16(strElement.Remove(0, 2), System.Globalization.CultureInfo.InvariantCulture)];
                                        if (strEqnRpl.Contains("-"))
                                        {
                                            arlEqnRplPrts.Add("(" + strEqnRpl + ")");
                                        }
                                        else
                                        {
                                            arlEqnRplPrts.Add(strEqnRpl);
                                        }
                                    }
                                    else
                                    {
                                        arlEqnRplPrts.Add(strElement);
                                    }
                                }
                                else
                                {
                                    arlEqnRplPrts.Add("0");
                                }
                                arlEqnPrts.Add(strElement);
                            }
                        }
                    }
                }

                for (int intReplace = 0; intReplace < arlEqnPrts.Count; intReplace++)
                {
                    strCurEqn = Regex.Replace(strCurEqn, arlEqnPrts[intReplace].ToString().Replace("$", "\\$").Replace("?", "\\?") + "\\b", arlEqnRplPrts[intReplace].ToString());
                }
                if (strCurEqn.Contains("if"))
                {
                    strArrEqnResult[intCnt] = resolveIfCmd(strCurEqn);
                }
                else
                {
                    if (strCurEqn.Contains("?"))
                    {
                        strArrEqnResult[intCnt] = strCurEqn;
                    }
                    else
                    {
                        if (strCurEqn.StartsWith("*"))
                        {
                            strArrEqnResult[intCnt] = Evaluate(strCurEqn.Substring(1));
                        }
                        else
                        {
                            strArrEqnResult[intCnt] = Evaluate(strCurEqn);
                        }
                    }

                }
            }
            return strArrEqnResult;
        }

        private string resolveIfCmd(string strIfCmd)
        {
            string[] strIf = strIfCmd.Substring(3, strIfCmd.Length - 4).Split(',');
            string strIfChk = strIf[0].Replace("(", "").Replace(")", "").Trim();
            string strResult = "";

            if (strIfChk.Contains("?"))
            {
                strResult = strIfCmd;
            }
            else
            {
                if (Convert.ToDouble(strIfChk, System.Globalization.CultureInfo.InvariantCulture) > 0)
                {
                    strResult = strIf[1].Trim();
                }
                else
                {
                    strResult = strIf[2].Trim();
                }
            }
            return strResult;
        }

        private string GetIfCmdVal(string strIfCmd, string[] strEqnSlvd)
        {
            string[] strIf = strIfCmd.Substring(3, strIfCmd.Length - 4).Split(',');
            string strIfChk = strIf[0].Replace("(", "").Replace(")", "").Trim();
            string strResult = strIfChk;

            if (strIfChk.Contains("?"))
            {
                do
                {
                    strResult = strEqnSlvd[Convert.ToInt16(strResult.Remove(0, 2), System.Globalization.CultureInfo.InvariantCulture)];

                    while (strResult.Contains("if"))
                    {
                        strResult = GetIfCmdVal(strResult, strEqnSlvd);

                        while (strResult.Contains("?"))
                        {
                            strResult = strEqnSlvd[Convert.ToInt16(strResult.Remove(0, 2), System.Globalization.CultureInfo.InvariantCulture)];
                        }
                    }
                }
                while (strResult.Contains("?"));
            }
            if (Convert.ToDouble(strResult.Trim().Replace("(", "").Replace(")", ""), System.Globalization.CultureInfo.InvariantCulture) > 0)
            {
                strResult = strIf[1].Trim();
            }
            else
            {
                strResult = strIf[2].Trim();
            }
            return strResult.Trim();
        }

        private string GetEqnVal(string strEqn, string[] strEqnSlvd)
        {
            char[] arrOperators = { '*', '/', '+', '-', '(', ')', ',' };
            ArrayList arlEqnPrts = new ArrayList();
            ArrayList arlEqnRplPrts = new ArrayList();
            string strCurEqn = strEqn;
            string strResult = "";
            string[] strArrExpElements = strCurEqn.Split(arrOperators, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < strArrExpElements.Length; i++)
            {
                if (Regex.IsMatch(strArrExpElements[i].ToString().Trim(), @"[^0-9.]"))
                {
                    string strElement = strArrExpElements[i].ToString().Trim();
                    if (strElement.StartsWith("?"))
                    {
                        if (!arlEqnPrts.Contains(strElement))
                        {
                            if (Convert.ToInt16(strElement.Remove(0, 2), System.Globalization.CultureInfo.InvariantCulture) < strEqnSlvd.Length)
                            {
                                string strEqnRpl = strEqnSlvd[Convert.ToInt16(strElement.Remove(0, 2), System.Globalization.CultureInfo.InvariantCulture)];
                                if (strEqnRpl.Contains("-"))
                                {
                                    arlEqnRplPrts.Add("(" + strEqnRpl + ")");
                                }
                                else
                                {
                                    arlEqnRplPrts.Add(strEqnRpl);
                                }
                            }
                            else
                            {
                                arlEqnRplPrts.Add("0");
                            }
                            arlEqnPrts.Add(strElement);
                        }
                    }
                }
            }

            for (int intReplace = 0; intReplace < arlEqnPrts.Count; intReplace++)
            {
                strCurEqn = Regex.Replace(strCurEqn, arlEqnPrts[intReplace].ToString().Replace("$", "\\$").Replace("?", "\\?") + "\\b", arlEqnRplPrts[intReplace].ToString());
            }
            if (strCurEqn.Contains("if"))
            {
                strResult = resolveIfCmd(strCurEqn);
            }
            else
            {
                if (strCurEqn.Contains("?"))
                {
                    strResult = strCurEqn;
                }
                else
                {
                    strResult = Evaluate(strCurEqn);
                }
            }
            return strResult;
        }

        private string Evaluate(string strEquation)
        {
            string strResult = "";
            Eval evl = new Eval();
            evl.ParseEquation(strEquation);
            evl.Convert2PostFix();
            evl.EvaluatePostfix();
            if (evl.Error)
            {
                strResult = "0";
            }
            strResult = evl.Result.ToString(System.Globalization.CultureInfo.InvariantCulture);
            return strResult;
        }
        //End of RefNo-1
    }
}
