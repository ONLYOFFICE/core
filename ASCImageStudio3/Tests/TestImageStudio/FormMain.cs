using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace TestImageStudio
{
    public partial class FormMain : Form
    {
        AVSImageFile3.ImageFile3Class m_oImageFile = new AVSImageFile3.ImageFile3Class();
        AVSImageStudio3.ImageTransformsClass m_oImageTransforms = new AVSImageStudio3.ImageTransformsClass();

        public FormMain()
        {
            InitializeComponent();
        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;

            object oImageSrc = null;

            if (!m_oImageFile.LoadImage2(openFileDialog1.FileName, out oImageSrc))
                textBoxPath.Text = "";
            else
                textBoxPath.Text = openFileDialog1.FileName;

            oImageSrc = null;
        }

        private void buttonRun_Click(object sender, EventArgs e)
        {
            m_oImageTransforms.SetXml(textBoxXml.Text);

            object oImageSrc = null;

            if (!checkBoxAutoReload.Checked)
            {
                oImageSrc = null;
                m_oImageFile.LoadImage2(textBoxPath.Text, out oImageSrc);
            }

            for (int index = 0; index < numericDuration.Value; ++index)
            {
                Application.DoEvents();

                m_oImageTransforms.SetTime(index);

                if (checkBoxAutoReload.Checked)
                {
                    object oImageSrc1 = null;
                    m_oImageFile.LoadImage2(textBoxPath.Text, out oImageSrc1);

                    m_oImageTransforms.SetSource(0, oImageSrc1);

                    if (m_oImageTransforms.Transform())
                    {
                        object oImageDst = null;
                        m_oImageTransforms.GetResult(0, out oImageDst);

                        if (checkBoxUpdatePreview.Checked)
                        {
                            axImageView31.SetAdditionalParam("MediaData", oImageDst);
                            axImageView31.ShrinkToFit();
                        }

                        oImageDst = null;
                    }
                    
                    oImageSrc1 = null;
                }
                else
                {
                    m_oImageTransforms.SetSource(0, oImageSrc);

                    if (m_oImageTransforms.Transform())
                    {
                        object oImageDst = null;
                        m_oImageTransforms.GetResult(0, out oImageDst);

                        if (checkBoxUpdatePreview.Checked)
                        {
                            axImageView31.SetAdditionalParam("MediaData", oImageDst);
                            axImageView31.ShrinkToFit();
                        }

                        oImageDst = null;
                    }
                }
            }
        }

        private void buttonRun3_Click(object sender, EventArgs e)
        {
            m_oImageTransforms.SetXml(textBoxXml.Text);

            object oImageSrc1 = null;
            object oImageSrc2 = null;
            object oImageSrc3 = null;

            m_oImageFile.LoadImage2(textBoxPath.Text, out oImageSrc1);
            m_oImageFile.LoadImage2(textBoxPath.Text, out oImageSrc2);
            m_oImageFile.LoadImage2(textBoxPath.Text, out oImageSrc3);

            m_oImageTransforms.SetSource(2, oImageSrc3);

            for (int index = 0; index < numericDuration.Value; ++index)
            {
                if (checkBoxAutoReload.Checked)
                {
                    oImageSrc1 = null;
                    oImageSrc2 = null;
                    oImageSrc3 = null;

                    m_oImageFile.LoadImage2(textBoxPath.Text, out oImageSrc1);
                    m_oImageFile.LoadImage2(textBoxPath.Text, out oImageSrc2);
                    m_oImageFile.LoadImage2(textBoxPath.Text, out oImageSrc3);

                    m_oImageTransforms.SetSource(0, oImageSrc3);
                }

                m_oImageTransforms.SetTime(index);

                m_oImageTransforms.SetSource(1, oImageSrc1);
                m_oImageTransforms.SetSource(2, oImageSrc2);

                if (m_oImageTransforms.Transform())
                {
                    object oImageDst = null;
                    m_oImageTransforms.GetResult(0, out oImageDst);

                    if (checkBoxUpdatePreview.Checked)
                    {
                        axImageView31.SetAdditionalParam("MediaData", oImageDst);
                        axImageView31.ShrinkToFit();
                    }

                    oImageDst = null;
                }
            }
        }
       
        private void treeViewPresets_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (e.Node != null && e.Node.Tag != null)
                textBoxXml.Text = ((string)e.Node.Tag).Replace("'", "\"");
        }

        private string SplitParams(string strParams)
        {
            return "\r\n " + strParams.Replace(" ", "\r\n ");
        }
        private void AddEffect(string strCaption, string strName, string strParams)
        {
            if (strName.Length < 1)
                treeViewPresets.Nodes.Add(strCaption);
            else
            {
                string strXml = "";

                strXml += "<transforms>\r\n";
                strXml += " <" + strName + " " + SplitParams(strParams) + ">\r\n";
                strXml += "  <timeline type='1' begin='10' end='90' fadein='20' fadeout='20' completeness='1.0' />\r\n";
                strXml += " </" + strName + ">\r\n";
                strXml += "</transforms>\r\n";

                TreeNode oNode = new TreeNode(strCaption);

                oNode.Tag = strXml;

                treeViewPresets.Nodes[treeViewPresets.Nodes.Count - 1].Nodes.Add(oNode);
            }
        }
        private void AddEffect(string strCaption, string strXml)
        {
            TreeNode oNode = new TreeNode(strCaption);

            oNode.Tag = strXml;

            treeViewPresets.Nodes[treeViewPresets.Nodes.Count - 1].Nodes.Add(oNode);
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            AddEffect("Custom", "", "");
            AddEffect("  Applying masks", "<transforms>\r\n<ImageCopy index-from='0' index-to='10'>\r\n<timeline type='1' begin='0' end='50' fadein='20' fadeout='20' completeness='1.0' />\r\n</ImageCopy>\r\n<ImageTransform-AdjustBrightness level='100'>\r\n<timeline type='1' begin='0' end='100' fadein='20' fadeout='20' completeness='1.0' />\r\n</ImageTransform-AdjustBrightness>\r\n<ImageCopy index-from='10' index-to='0' mask-type='3' mask-points='10 10 50 50' metric='1'>\r\n<timeline type='1' begin='0' end='100' fadein='20' fadeout='20' completeness='1.0' />\r\n</ImageCopy>\r\n<ImageClear index='10' />\r\n</transforms>");
            AddEffect("  Advanced rotate", "1. copy image to rotate to additional buffer first\r\n <ImageCopy index-from='0' index-to='1' />\r\n\r\n2. draw saved image\r\n <ImageTransform-TransformRotateAdvanced \r\n index-from='1'\r\n  index-to='0'\r\n  metric='0'\r\n  centerx='200'\r\n  centery='200'\r\n angle='10'\r\n scale='0.1'\r\ninterpolate='1'\r\n/>	               ");
            AddEffect("Low level", "", "");
            AddEffect("  BufferCopy", "BufferCopy", "index-from='0' index-to='10'");
            AddEffect("  BufferClear",      "BufferClear", "index='10'");
            AddEffect("  ImageCopy",        "ImageCopy", "index-from='0' index-to='10' mask-type='3' mask-points='10 10 50 50' metric='1'");
            AddEffect("  ImageClear",       "ImageClear", "index='10'");
            AddEffect("Adjusts", "", "");
            AddEffect("  Brightness",       "ImageTransform-AdjustBrightness", "level='100'");
            AddEffect("  BrightnessEx",     "ImageTransform-AdjustBrightnessEx", "levelred='-255' levelgreen='0' levelblue='0'");
            AddEffect("  Coontrast",        "ImageTransform-AdjustContrast", "level='10'");
            AddEffect("  Gamma",            "ImageTransform-AdjustGamma", "level='50'");
            AddEffect("  Equalize",         "ImageTransform-AdjustEqualize", "");
            AddEffect("  Grayscale",        "ImageTransform-AdjustGrayscale", "desaturate='false'");
            AddEffect("  Invert",           "ImageTransform-AdjustInvert", "");
            AddEffect("  Autocontrast",     "ImageTransform-AdjustAutocontrast", "");
            AddEffect("  Autolevels",       "ImageTransform-AdjustAutolevels", "");
            AddEffect("  Hue",              "ImageTransform-AdjustHue", "angle='120'");
            AddEffect("  Saturation",       "ImageTransform-AdjustSaturation", "level='50'");
            AddEffect("  Posterize",        "ImageTransform-AdjustPosterize", "levels='4'");
            AddEffect("  Temperature",      "ImageTransform-AdjustTemperature", "level='100'");
            AddEffect("  Colorize",         "ImageTransform-AdjustColorize", "levelred='255' levelgreen='0' levelblue='0'");
            AddEffect("  Threshold",        "ImageTransform-AdjustThreshold", "level='100' grayscale='1'");
            AddEffect("Effects", "", "");
            AddEffect("  Blur",             "ImageTransform-EffectBlur", "level='100'");
            AddEffect("  Sharpen",          "ImageTransform-EffectSharpen", "level='100'");
            AddEffect("  Mosaic",           "ImageTransform-EffectMosaic", "size='4' simple='0'");
            AddEffect("  GaussianBlur",     "ImageTransform-EffectGaussianBlur", "size='4'");
            AddEffect("  Noise",            "ImageTransform-EffectNoise", "level='100' monochrome='1'");
            AddEffect("  Diffuse",          "ImageTransform-EffectDiffuse", "distance='20'");
            AddEffect("  MotionBlur",       "ImageTransform-EffectMotionBlur", "angle='30' distance='10'");
            AddEffect("  Emboss",           "ImageTransform-EffectEmboss", "angle='30' distance='3' amount='100'");
            AddEffect("  Minimal",          "ImageTransform-EffectMinimal", "size='5'");
            AddEffect("  Maximal",          "ImageTransform-EffectMaximal", "size='5'");
            AddEffect("  Deinterlace",      "ImageTransform-EffectDeinterlace", "even='1' interpolate='1'");
            AddEffect("  Median",           "ImageTransform-EffectMedian", "type='2'");
            AddEffect("  ChromoKeyRange",   "ImageTransform-EffectChromoKeyRange", "color1='255' color2='0'");
            AddEffect("  ChromoKeyLevel",   "ImageTransform-EffectChromoKeyLevel", "color='255' threshold='30'");
            AddEffect("Transform", "", "");
            AddEffect("  Flip",             "ImageTransform-TransformFlip", "vertical='1' backcolor='255'");
            AddEffect("  Perspective",      "ImageTransform-TransformPerspective", "vertical='1' angle='-45' backcolor='255'");
            AddEffect("  Skew",             "ImageTransform-TransformSkew", "vertical='1' angle='-45' backcolor='255'");
            AddEffect("  Shift",            "ImageTransform-TransformShift", "offsetx='100' offsety='40' backcolor='255' metric='0'");
            AddEffect("  Rotate",           "ImageTransform-TransformRotate", "angle='90' size='1' backcolor='255'");
            AddEffect("  Resample",         "ImageTransform-TransformResample", "times='5'");
            AddEffect("  Zoom",             "ImageTransform-TransformZoom", "zoom='100' backcolor='255'");
            AddEffect("  Mirror",           "ImageTransform-TransformMirror", "type='8100' level='20' backcolor='255'");
            AddEffect("  Resize",           "ImageTransform-TransformResize", "width='32' height='32' type='257' backcolor='255'");
            AddEffect("  Twirl",            "ImageTransform-TransformTwirl", "degree='2' angle='90' backcolor='255'");
            AddEffect("  Sphere",           "ImageTransform-TransformSphere", "centerx='100' centery='50' radiusx='70' radiusy='40' degree='2' backcolor='255' metric='0'");
            AddEffect("  Cylinder",         "ImageTransform-TransformCylinder", "center='70' radius='50' type='1' degree='2' backcolor='255' metric='0'");
            AddEffect("Filter", "", "");
            AddEffect("  Prewitt",          "ImageTransform-FilterPrewitt", "type='0'");
            AddEffect("  Scharr",           "ImageTransform-FilterScharr", "type='0'");
            AddEffect("  Sobel",            "ImageTransform-FilterSobel", "type='1' extended='0'");
            AddEffect("  Roberts",          "ImageTransform-FilterRoberts", "type='0'");
            AddEffect("  Laplace",          "ImageTransform-FilterLaplace", "type='1'");
            AddEffect("  Hipass",           "ImageTransform-FilterHipass", "type='1'");
            AddEffect("  Lowpass",          "ImageTransform-FilterLowpass", "type='1'");
            AddEffect("  Blur",             "ImageTransform-FilterBlur", "type='5'");
            AddEffect("  Canny",            "ImageTransform-FilterCanny", "levellow='10' levelhigh='100'");
            AddEffect("File", "", "");
            AddEffect("  LoadImage",        "ImageFile-LoadImage", "sourcepath='c:\\in.bmp'");
            AddEffect("  LoadScreenshot",   "ImageFile-LoadScreenshot", "");
            AddEffect("  SaveAsBmp",        "ImageFile-SaveAsBmp", "destinationpath='c:\\out.bmp' format='888'");
            AddEffect("  SaveAsGif",        "ImageFile-SaveAsGif", "destinationpath='c:\\out.gif' format='888'");
            AddEffect("  SaveAsJpeg",       "ImageFile-SaveAsJpeg", "destinationpath='c:\\out.jpg' format='888' quality='20'");
            AddEffect("  SaveAsPng",        "ImageFile-SaveAsPng", "destinationpath='c:\\out.png' format='888'");
            AddEffect("  SaveAsTiff",       "ImageFile-SaveAsTiff", "destinationpath='c:\\out.tiff' format='888' compression='1'");
            AddEffect("  SaveAsWmf",        "ImageFile-SaveAsWmf", "destinationpath='c:\\out.wmf' format='888'");
            AddEffect("  SaveAsEmf",        "ImageFile-SaveAsEmf", "destinationpath='c:\\out.emf' format='888'");
            AddEffect("  SaveAsPcx",        "ImageFile-SaveAsPcx", "destinationpath='c:\\out.pcx' format='888'");
            AddEffect("  SaveAsTga",        "ImageFile-SaveAsTga", "destinationpath='c:\\out.tga' format='888'");
            AddEffect("  SaveAsRas",        "ImageFile-SaveAsRas", "destinationpath='c:\\out.ras' format='888'");
            AddEffect("  SaveAsIPod",       "ImageFile-SaveAsIPod", "destinationpath='c:\\out.ipod' format='888'");
            AddEffect("Paint", "", "");
            AddEffect("  SetPen",           "ImagePaint-SetPen", "color='255' alpha='128' size='2'");
            AddEffect("  SetBrush",         "ImagePaint-SetBrush", "type='2001' color1='255' color2='65536' alpha1='128' alpha2='0' texturepath='' texturealpha='100' texturemode='1' rectable='1' rect-left='0' rect-top='0' rect-width='100' rect-height='100'");
            AddEffect("  SetFont",          "ImagePaint-SetFont", "name='Tahoma' size='16' bold='1' italic='1' underline='1' strikeout='1'");
            AddEffect("  SetFormat",        "ImagePaint-SetFormat", "antialiaspen='1' antialiastext='1' stringalignmentvertical='0' stringalignmenthorizontal='0' imagealphmode='1'");
            AddEffect("  DrawLine",         "ImagePaint-DrawLine", "x1='1' y1='1' x2='100' y2='50' metric='0'");
            AddEffect("  DrawRectangle",    "ImagePaint-DrawRectangle", "left='10' top='5' right='100' bottom='50' solid='1' metric='0'");
            AddEffect("  DrawEllipse",      "ImagePaint-DrawEllipse", "left='10' top='5' right='100' bottom='50' solid='1' metric='0'");
            AddEffect("  DrawInvRectangle", "ImagePaint-DrawInvRectangle", "left='10' top='5' right='100' bottom='50' metric='0'");
            AddEffect("  DrawInvEllipse",   "ImagePaint-DrawInvEllipse", "left='10' top='5' right='100' bottom='50' metric='0'");
            AddEffect("  DrawPie",          "ImagePaint-DrawPie", "left='10' top='5' right='100' bottom='50' startangle='45' sweepangle='90' solid='1' metric='0'");
            AddEffect("  DrawSector",       "ImagePaint-DrawSector", "left='10' top='5' right='100' bottom='50' radiusratio='2' startangle='45' sweepangle='90' solid='1' metric='0'");
            AddEffect("  DrawSimplePie",    "ImagePaint-DrawSimplePie", "centerx='60' centery='40' radius='20' startangle='45' sweepangle='90' solid='1' metric='0'");
            AddEffect("  DrawSimpleSector", "ImagePaint-DrawSimpleSector", "centerx='60' centery='40' radius1='20' radius2='40' startangle='45' sweepangle='90' solid='1' metric='0'");
            AddEffect("  DrawPolyline",     "ImagePaint-DrawPolyline", "points='10 5 100 5 10 100' metric='0'");
            AddEffect("  DrawPolygon",      "ImagePaint-DrawPolygon", "points='10 5 100 5 10 100' solid='1' metric='0'");
            AddEffect("  DrawBorder",       "ImagePaint-DrawBorder", "type='10100' size='100' metric='0'");
            AddEffect("  DrawMovObject",    "ImagePaint-DrawMovObject", "type='1100' divisions='5' color='0' alpha='128'");
            AddEffect("  DrawCredits",      "ImagePaint-DrawCredits", "type='5100' text='-= setup font and brush first =-' metric='0'");
            AddEffect("  DrawTextSimple",   "ImagePaint-DrawTextSimple", "left='10' top='50' text='simple text' metric='0'");
            AddEffect("  DrawTextInRect",   "ImagePaint-DrawTextInRect", "left='10' top='50' right='250' bottom='100' scale='1' text='extended text' metric='0'");
            AddEffect("  DrawImageFromFile",   "ImagePaint-DrawImageFromFile", "left='10' top='0' right='90' bottom='100' filepath='c:\\in.bmp' metric='0' backcolor='-1'");
            AddEffect("  DrawImageFromBuffer", "ImagePaint-DrawImageFromBuffer", "left='10' top='0' right='90' bottom='100' buffer='1' metric='0'");
            AddEffect("Paint-TextStatic", "", "");
            AddEffect("  SetBrush",         "ImagePaint-TextStatic-SetBrush", "type='2001' color1='255' color2='65536' alpha1='128' alpha2='0' texturepath='' texturealpha='100' texturemode='1' rectable='1' rect-left='0' rect-top='0' rect-width='100' rect-height='100'");
            AddEffect("  SetFont",          "ImagePaint-TextStatic-SetFont", "name='Tahoma' size='16' bold='1' italic='1' underline='1' strikeout='1'");
            AddEffect("  SetFormat",        "ImagePaint-TextStatic-SetFormat", "stringalignmentvertical='0' stringalignmenthorizontal='0' rotate='30'");
            AddEffect("  SetShadow",        "ImagePaint-TextStatic-SetShadow", "visible='1' distancex='5' distancey='10' blursize='2' color='0' alpha='120'");
            AddEffect("  SetEdge",          "ImagePaint-TextStatic-SetEdge", "visible='1' dist='5' color='65536' alpha='128'");
            AddEffect("  DrawInRect",       "ImagePaint-TextStatic-DrawInRect", "left='10' top='50' right='250' bottom='100' text='DrawInRect' metric='0'");
            AddEffect("  DrawOnEllipse",    "ImagePaint-TextStatic-DrawOnEllipse", "left='10' top='50' right='150' bottom='150' text='DrawOnEllipse' startangle='-30' sweepangle='80' metric='0'");
            AddEffect("  DrawOnPolyline",   "ImagePaint-TextStatic-DrawOnPolyline", "text='DrawOnPolyline' points='10 5 100 140' metric='0'");
            AddEffect("Paint-TextDynamic", "", "");
            AddEffect("  SetBrush",         "ImagePaint-TextDynamic-SetBrush", "type='2001' color1='255' color2='65536' alpha1='128' alpha2='0' texturepath='' texturealpha='100' texturemode='1' rectable='1' rect-left='0' rect-top='0' rect-width='100' rect-height='100'");
            AddEffect("  SetFont",          "ImagePaint-TextDynamic-SetFont", "name='Tahoma' size='16' bold='1' italic='1' underline='1' strikeout='1'");
            AddEffect("  SetShadow",        "ImagePaint-TextDynamic-SetShadow", "visible='1' distancex='5' distancey='10' blursize='2' color='0' alpha='120'");
            AddEffect("  SetEdge",          "ImagePaint-TextDynamic-SetEdge", "visible='1' dist='5' color='65536' alpha='128'");
            AddEffect("  Start",            "ImagePaint-TextDynamic-Start", "text='sample string' effectid='815' x='20' y='40' step='10' metric='0'");
            AddEffect("  Stop",             "ImagePaint-TextDynamic-Stop", "");
            AddEffect("  Render",           "ImagePaint-TextDynamic-Render", "");
        }
    }
}