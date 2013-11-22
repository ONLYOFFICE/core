using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace TestImageStudioMdi
{
    public partial class FormTransforms : Form
    {
        const string c_sTimeline = "\r\n  <timeline type='1' begin='0' end='300' fadein='125' fadeout='125' completeness='1.0' />\r\n";

        const string c_sHours = "hours-prefix='' hours-suffix=':' hours-start='0' hours-end='3' hours-digits='0' hours-visible='1'";
        const string c_sMinutes = "minutes-prefix='' minutes-suffix=':' minutes-start='0' minutes-end='33' minutes-digits='2' minutes-visible='1'";
        const string c_sSeconds = "seconds-prefix='' seconds-suffix=':' seconds-start='0' seconds-end='56' seconds-digits='2' seconds-visible='1'";
        const string c_sMilliseconds = "milliseconds-prefix='' milliseconds-suffix='' milliseconds-start='0' milliseconds-end='750' milliseconds-digits='3' milliseconds-visible='1'>";
        const string c_sPaintDrawTimerAttr = "type='1' metric='0' rect-left='0' rect-top='0' rect-right='300' rect-bottom='300'\r\nanimation-fadein='1' animation-state='3' animation-fadeout='1' text-fillmode='0' text-typeeffects='1' token='_'\r\nfont-name='Arial' font-size='36' font-bold='1' font-italic='0' font-underline='0' font-strikeout='1' font-angle='0'\r\nbrush-type='0' brush-color1='255' brush-color2='65535' brush-alpha1='255' brush-alpha2='255' brush-texturepath='c:\\' brush-texturealpha='255' brush-texturemode='0' brush-rectable='0' brush-rect-left='0' brush-rect-top='0' brush-rect-width='0' brush-rect-height='0' pen-antialiasbrush='1'\r\nshadow-visible='1' shadow-distancex='5' shadow-distancey='10' shadow-blursize='3' shadow-color='0' shadow-alpha='120'\r\nedge-visible='1' edge-dist='3' edge-color='65536' edge-alpha='192'";
        const string c_sPaintDrawCountdownAttr = "startnumber='10' finishnumber='0' metric='0' rect-left='0' rect-top='0' rect-right='300' rect-bottom='300'\r\nanimation-fadein='1' animation-state='3' animation-fadeout='1' text-fillmode='0' text-typeeffects='1' token='_'\r\nfont-name='Arial' font-size='36' font-bold='1' font-italic='0' font-underline='0' font-strikeout='1' font-angle='0'\r\nbrush-type='0' brush-color1='255' brush-color2='65535' brush-alpha1='255' brush-alpha2='255' brush-texturepath='c:\\' brush-texturealpha='255' brush-texturemode='0' brush-rectable='0' brush-rect-left='0' brush-rect-top='0' brush-rect-width='0' brush-rect-height='0' pen-antialiasbrush='1'\r\nshadow-visible='1' shadow-distancex='5' shadow-distancey='10' shadow-blursize='3' shadow-color='0' shadow-alpha='120'\r\nedge-visible='1' edge-dist='3' edge-color='65536' edge-alpha='192'>";        
        
        public FormTransforms()
        {
            InitializeComponent();
        }

        private void FormTransforms_Load(object sender, EventArgs e)
        {
            AddEffect("None", "", "");
            AddEffect("Empty Effect", "<none> </none>" );
            AddEffect("VideoCard", "", "");
            AddEffect("Set Params", "<transforms>\r\n  <SetParamsUseVideoCard\r\n  usetype='2'\r\n  psversion='0'\r\n  interpolation='2'/>\r\n</transforms>");
            AddEffect("Custom", "", "");
            AddEffect("  Applying masks", "<transforms>\r\n<ImageCopy index-from='0' index-to='10'>\r\n<timeline type='1' begin='0' end='50' fadein='20' fadeout='20' completeness='1.0' />\r\n</ImageCopy>\r\n<ImageTransform-AdjustBrightness level='100'>\r\n<timeline type='1' begin='0' end='100' fadein='20' fadeout='20' completeness='1.0' />\r\n</ImageTransform-AdjustBrightness>\r\n<ImageCopy index-from='10' index-to='0' mask-type='3' mask-points='10 10 50 50' metric='1'>\r\n<timeline type='1' begin='0' end='100' fadein='20' fadeout='20' completeness='1.0' />\r\n</ImageCopy>\r\n<ImageClear index='10' />\r\n</transforms>");
            AddEffect("  Advanced rotate", "1. copy image to rotate to additional buffer first\r\n <ImageCopy index-from='0' index-to='1' />\r\n\r\n2. draw saved image\r\n <ImageTransform-TransformRotateAdvanced \r\n index-from='1'\r\n  index-to='0'\r\n  metric='2'\r\n  centerx='0.6'\r\n  centery='0.4'\r\n angle='10'\r\n scale='0.2'\r\nalpha='192'\r\ninterpolate='1'\r\n/>	               ");
            AddEffect("  Paint chromo-key image", "<transforms>\r\n <ImageTransform-EffectChromoKeyRange index='1' color1='0' color2='255' composealpha='1' />\r\n <ImagePaint-DrawImageFromBuffer \r\n left='10'\r\n top='10'\r\n right='100'\r\n bottom='100'\r\n buffer='1'\r\n metric='0'>\r\n  <timeline type='1' begin='10' end='90' fadein='20' fadeout='20' completeness='1.0' />\r\n </ImagePaint-DrawImageFromBuffer>\r\n</transforms>");
            AddEffect("Low level", "", "");
            AddEffect("  BufferCopy", "BufferCopy", "index-from='0' index-to='10'");
            AddEffect("  BufferClear", "BufferClear", "index='10'");
            AddEffect("  ImageCopy", "ImageCopy", "index-from='0' index-to='10' mask-type='3' mask-points='10 10 50 50' metric='1'");
            AddEffect("  ImageClear", "ImageClear", "index='10'");
            AddEffect("Adjusts", "", "");
            AddEffect("  Brightness", "ImageTransform-AdjustBrightness", "level='100'");
            AddEffect("  BrightnessEx", "ImageTransform-AdjustBrightnessEx", "levelred='-255' levelgreen='0' levelblue='0'");
            AddEffect("  Coontrast", "ImageTransform-AdjustContrast", "level='10'");
            AddEffect("  Gamma", "ImageTransform-AdjustGamma", "level='50'");
            AddEffect("  Equalize", "ImageTransform-AdjustEqualize", "");
            AddEffect("  Grayscale", "ImageTransform-AdjustGrayscale", "desaturate='false'");
            AddEffect("  Invert", "ImageTransform-AdjustInvert", "");
            AddEffect("  Autocontrast", "ImageTransform-AdjustAutocontrast", "");
            AddEffect("  Autolevels", "ImageTransform-AdjustAutolevels", "");
            AddEffect("  Autobrightness", "ImageTransform-AdjustAutobrightness", "");
            AddEffect("  Hue", "ImageTransform-AdjustHue", "angle='120'");
            AddEffect("  Saturation", "ImageTransform-AdjustSaturation", "level='100' factor='4'");
            AddEffect("  Posterize", "ImageTransform-AdjustPosterize", "levels='4'");
            AddEffect("  Temperature", "ImageTransform-AdjustTemperature", "level='100'");
            AddEffect("  Colorize", "ImageTransform-AdjustColorize", "levelred='255' levelgreen='0' levelblue='0' type='0'");
            AddEffect("  Threshold", "ImageTransform-AdjustThreshold", "level='100' grayscale='1'");
            AddEffect("  Sepia", "ImageTransform-AdjustSepia", "level='100' darkness='20' redlevel='100' greenlevel='90' bluelevel='85'");
            AddEffect("  WhiteBalance", "ImageTransform-AdjustWhiteBalance", "red='255' green='0' blue='0'");
            AddEffect("  Recovery", "ImageTransform-AdjustRecovery", "level='500'");
            AddEffect("  FillLight", "ImageTransform-AdjustFillLight", "level='500'");
            AddEffect("  Exposure", "ImageTransform-AdjustExposure", "level='500'");
            AddEffect("  Blacks", "ImageTransform-AdjustBlacks", "level='500'");
            AddEffect("  HighLight", "ImageTransform-AdjustHighLight", "filllight='500' exposure='100'");
            AddEffect("  Shadows", "ImageTransform-AdjustShadows", "blacks='300' recovery='100'");
            AddEffect("  Clarity", "ImageTransform-AdjustClarity", "level='50' radius='75' threshold='20'");
            AddEffect("  Vibrance", "ImageTransform-AdjustVibrance", "level='100'");
            AddEffect("  AutoWhiteBalance", "ImageTransform-AdjustAutoWhiteBalance", "percent='90'");
            AddEffect("Effects", "", "");
            AddEffect("  Anaglyph3d", "ImageTransform-EffectAnaglyph3d", "depth='36' type='0'");
            AddEffect("  Blur", "ImageTransform-EffectBlur", "level='100' metric='0'");
            AddEffect("  Sharpen", "ImageTransform-EffectSharpen", "level='100' metric='0'");
            AddEffect("  Mosaic", "ImageTransform-EffectMosaic", "size='4' simple='0' metric='0'");
            AddEffect("  GaussianBlur", "ImageTransform-EffectGaussianBlur", "size='4' metric='0'");
            AddEffect("  Noise", "ImageTransform-EffectNoise", "level='100' monochrome='1' metric='0'");
            AddEffect("  Diffuse", "ImageTransform-EffectDiffuse", "distance='20' metric='0'");
            AddEffect("  MotionBlur", "ImageTransform-EffectMotionBlur", "angle='30' distance='10' metric='0'");
            AddEffect("  Emboss", "ImageTransform-EffectEmboss", "angle='30' distance='3' amount='100' metric='0' grayscale='0'");
            AddEffect("  Minimal", "ImageTransform-EffectMinimal", "size='5' metric='0'");
            AddEffect("  Maximal", "ImageTransform-EffectMaximal", "size='5' metric='0'");
            AddEffect("  Deinterlace", "ImageTransform-EffectDeinterlace", "even='1' interpolate='1'");
            AddEffect("  Median", "ImageTransform-EffectMedian", "type='2'");
            AddEffect("  ChromoKeyRange", "ImageTransform-EffectChromoKeyRange", "color1='255' color2='0'");
            AddEffect("  ChromoKeyLevel", "ImageTransform-EffectChromoKeyLevel", "color='255' threshold='30'");
            AddEffect("  Cinema", "ImageTransform-EffectCinema", "lines-minshow='1' lines-maxshow='3' lines-framesforshow='20' artifacts-minshow='1' artifacts-maxshow='3' shift-offset-maxx='5' shift-offset-maxy='5' shift-offset-factorx = '1.0' shift-offset-factory = '1.5' brightness = '90' contrast = '10'");
            AddEffect("  Glass", "ImageTransform-EffectGlass", "radius='10'");
            AddEffect("  GlassMosaic", "ImageTransform-EffectGlassMosaic", "metric='2' angle='30' width='0.4' height='0.3' scale='1' offsetx='0' offsety='0' curvature='10' smooth='15' quality='2' maxfactor='4' ");
            AddEffect("  WaterColor", "ImageTransform-EffectWaterColor", "size='3' coarseness='10'");
            AddEffect("  Glow", "ImageTransform-EffectGlow", "level='80'");
            AddEffect("  TVSimulation", "ImageTransform-EffectTVSimulation", "metric='2' interline='0' synchronize-ver-speed='0' synchronize-ver-offset='0' interframe-size='0.03' synchronize-hor-amplitude='0.05' synchronize-hor-offset='0.05' synchronize-hor-amplfactor='30' synchronize-hor-amploffset='10' synchronize-hor-speed='10' interframe-x-size='0.03' synchronize-ver-speed-factor='1'");
            AddEffect("  PencilSketch", "ImageTransform-EffectPencilSketch", "level='100' brightness = '10' contrast = '8'");
            AddEffect("  GrayOut", "ImageTransform-EffectGrayOut", "color='255' filter='101' levelred='100' levelgreen='10' levelblue='10' type='1' threshold='3'");
            AddEffect("  Wind", "ImageTransform-EffectWind", "metric='0' length='16' density='16'");
            AddEffect("  Newsprint", "ImageTransform-EffectNewsprint", "level='10'");
            AddEffect("  Snow", "ImageTransform-EffectSnow", "type='0' quality='0' count='500' size='10' angle-dir='0' angle-offset='30' droptime-min='120' droptime-max='150' color='16777215' alpha='192' snowdrift = '0' snowdriftspeedfactor = '1'");
            AddEffect("  JpegCompression", "ImageTransform-EffectJpegCompression", "quality='50'");
            AddEffect("  SoftFocus", "ImageTransform-EffectSoftFocus", "metric='0' blursize='5' intensity='50'");
            AddEffect("  SetAlphaChannel", "ImageTransform-EffectSetAlphaChannel", "metric='2' dst-x='0' dst-y='0' dst-width='1' dst-height='1' type='0' mask-path='d:\\1.bmp'");
            AddEffect("  OldPhoto", "ImageTransform-EffectOldPhoto", "mask='0' clarity='100'");
            AddEffect("  PostCrop", "ImageTransform-EffectPostCrop", "amount='100' midpoint='50' roundness='-70' feather='0'");
            AddEffect("  Vignette", "ImageTransform-EffectVignette", "metric='2' centerx='0.5' centery='0.5' radius1x='0.5' radius1y='0.5' radius2x='0.7' radius2y='0.7' angle='30' intensity='-0.5'");
            AddEffect2("  KenBurns", "    <ImageTransform-EffectKenBurns \r\n\tmetric='2' \r\n\tleft1='0.0' \r\n\ttop1='0.0' \r\n\tright1='0.5' \r\n\tbottom1='0.5' \r\n\tleft2='0.0' \r\n\ttop2='0.0' \r\n\tright2='0.65' \r\n\tbottom2='0.65'>\r\n" + " <timeline type='1' begin='0' end='300' fadein='100' fadeout='100' completeness='1.0' />\r\n" + "    </ImageTransform-EffectKenBurns>");
            AddEffect("  ZoomingSphere", "ImageTransform-EffectZoomingSphere", "power='2.5'");
            AddEffect("  ShapeMosaic", "ImageTransform-EffectShapeMosaic", "metric='0' size='20'");
            AddEffect("  Particles", "ImageTransform-EffectParticles", "filepath='d:\\avatar3.bmp' transparency='100' maxcountparticles='10' metric='0' minsize='20' maxsize='30' basedirangle='0' dispdirangle='0' minlifetime='150' maxlifetime='250' rottype='4' minrottime='110' maxrottime='220' minbaseangle='10' maxbaseangle='70' absrot='0' quality='0'");
            AddEffect("  Canvas", "ImageTransform-EffectCanvas", "alpha='100'");
            AddEffect("  Glow2", "ImageTransform-EffectGlow2", "metric='0' size='5'");
            AddEffect("  WaterMirrow", "ImageTransform-EffectWaterMirrow", "alphamin='32' alphamax='100' mask-type='2' mask-points='0.1 0.1 0.5 0.5' metric='2' x1='0' y1='0.5' x2='1' y2='0.5' ");
            AddEffect("  ColorExchange", "ImageTransform-EffectColorExchange", "color1='255' color2='16711680'");
            AddEffect("Transform", "", "");
            AddEffect("  GlassTile", "ImageTransform-TransformGlassTile", "metric='2' size='0.02'");
            AddEffect("  BrokenGlass", "ImageTransform-TransformBrokenGlass", "debris='15'");
            AddEffect("  Linear", "ImageTransform-TransformLinear", "metric='2' angle='30' scalex='1.1' scaley='0.9' shiftx='0.1' shifty='0.05' backcolor='255' backalpha='128'");
            AddEffect("  Flip", "ImageTransform-TransformFlip", "vertical='1' backcolor='255' backalpha='128'");
            AddEffect("  Flip2", "ImageTransform-TransformFlip2", "dir='1' backcolor='255' backalpha='128'");
            AddEffect("  Perspective", "ImageTransform-TransformPerspective", "vertical='1' angle='-45' backcolor='255' backalpha='128' alternative='0' x1='0.1' y1='0.1' x2='0.6' y2='0.2' x3='1.0' y3='0.9' x4='0.2' y4='0.8'");
            AddEffect("  Skew", "ImageTransform-TransformSkew", "vertical='1' angle='-45' backcolor='255' backalpha='128'");
            AddEffect("  Shift", "ImageTransform-TransformShift", "offsetx='100' offsety='40' backcolor='255' backalpha='128' metric='0'");
            AddEffect("  Rotate", "ImageTransform-TransformRotate", "angle='90' size='1' backcolor='255' backalpha='128'");
            AddEffect("  Resample", "ImageTransform-TransformResample", "times='5'");
            AddEffect("  Zoom", "ImageTransform-TransformZoom", "zoom='10' backcolor='255' backalpha='128' dx='0.5' dy='0.5' metric='2' render='0'");
            AddEffect("  ZoomLinear", "ImageTransform-TransformZoomLinear", "zoomX='10' zoomY='10' metric='2' backcolor='255' backalpha='128' dx='-1' dy='1'");
            AddEffect("  Mirror", "ImageTransform-TransformMirror", "type='8100' level='20' backcolor='255' backalpha='128'");
            AddEffect("  Resize", "ImageTransform-TransformResize", "width='200' height='200' type='69632' backcolor='255' backalpha='128' metric='0' cropzoom='0.3' src-x='0' src-y='0' src-width='0' src-height='0' src-metric='0'");
            AddEffect("  Twirl", "ImageTransform-TransformTwirl", "degree='4' angle='500' zoom='0.3' backcolor='255' backalpha='128'");
            AddEffect("  Sphere", "ImageTransform-TransformSphere", "centerx='0.31' centery='0.5' radiusx='0.7' radiusy='0.4' degree='2' backcolor='255' backalpha='128' metric='2'");
            AddEffect("  Cylinder", "ImageTransform-TransformCylinder", "center='0.7' radius='0.5' type='1' degree='2' backcolor='255' backalpha='128' metric='2'");
            AddEffect("  Wave", "ImageTransform-TransformWave", "metric='2' peaks='3' peakoffset='0.3' amplitude='0.1' speed='0.01' vertical='0'");
            AddEffect("  Ripple", "ImageTransform-TransformRipple", "metric='2' omega='7.5' offset='1' out='0' speed='1'");
            AddEffect("  RotateSpecial", "ImageTransform-TransformRotateSpecial", "angle='90'");
            AddEffect("  RotateSimple", "ImageTransform-TransformRotateSimple", "type='1'");
            AddEffect("Filter", "", "");
            AddEffect("  Prewitt", "ImageTransform-FilterPrewitt", "type='0'");
            AddEffect("  Scharr", "ImageTransform-FilterScharr", "type='0'");
            AddEffect("  Sobel", "ImageTransform-FilterSobel", "type='1' extended='0'");
            AddEffect("  Roberts", "ImageTransform-FilterRoberts", "type='0'");
            AddEffect("  Laplace", "ImageTransform-FilterLaplace", "type='1'");
            AddEffect("  Hipass", "ImageTransform-FilterHipass", "type='1'");
            AddEffect("  Lowpass", "ImageTransform-FilterLowpass", "type='1'");
            AddEffect("  Blur", "ImageTransform-FilterBlur", "type='5'");
            AddEffect("  Canny", "ImageTransform-FilterCanny", "levellow='10' levelhigh='100'");
            AddEffect("File", "", "");
            AddEffect("  LoadImage", "ImageFile-LoadImage", "sourcepath='c:\\in.bmp'");
            AddEffect("  LoadScreenshot", "ImageFile-LoadScreenshot", "");
            AddEffect("  LoadRaw", "ImageFile-LoadRaw", "sourcepath='c:\\in.bmp' Version='1' UseCameraWhiteBalance='0' UseAutoWhiteBalance ='0' UseAverageGrayBox='0' GrayBox_x='0' GrayBox_max_x='0' GrayBox_y='0' GrayBox_max_y='0' GrayBox_w='0' GrayBox_max_w='0' GrayBox_h='0' GrayBox_max_h='0' UseCustomWhiteBalance='0' WhiteBalance_b='0' WhiteBalance_max_b='0' WhiteBalance_min_b='0' WhiteBalance_g1='0' WhiteBalance_max_g1='0' WhiteBalance_min_g1='0' WhiteBalance_g2='0' WhiteBalance_max_g2='0' WhiteBalance_min_g2='0' WhiteBalance_r='0' WhiteBalance_max_r='0' WhiteBalance_min_r='0' UsePresetWhiteBalance='0' PresetWhiteBalance='0' max_PresetWhiteBalance='0' UseTemperature='0' Temperature_green='0' Temperature_max_green='0' Temperature_min_green='0' Temperature_temperature='0' Temperature_max_temperature='0' Temperature_min_temperature='0' UseCameraMatrix='0' UseCorrectChromaticAberration='0' ChromaticAberration_b='0' ChromaticAberration_max_b='0' ChromaticAberration_min_b='0' ChromaticAberration_r='0' ChromaticAberration_max_r='0' ChromaticAberration_min_r='0' UseDarknessLevel='0' max_DarknessLevel='0' min_DarknessLevel='0' DarknessLevel='-1' UseSaturationLevel='0' SaturationLevel='-1' max_SaturationLevel='-1' min_SaturationLevel='-1' UseDenoisingThreshold='0' max_DenoisingThreshold='0.0' DenoisingThreshold='0.0' UseHighlightMode='0' max_HighlightMode='0' min_HighlightMode='0' HighlightMode='0' NoAutoBrighten='0' UseAdjustBrightness='0' AdjustBrightness='1.0' max_AdjustBrightness='0.0' min_AdjustBrightness='0.0' UseInterpolationQuality='0' InterpolationQuality='3' max_InterpolationQuality='3' UseMedianFilterPasses='0' MedianFilterPasses='0' max_MedianFilterPasses='0'");
            AddEffect("  SaveAsBmp", "ImageFile-SaveAsBmp", "destinationpath='c:\\out.bmp' format='888'");
            AddEffect("  SaveAsGif", "ImageFile-SaveAsGif", "destinationpath='c:\\out.gif' format='888'");
            AddEffect("  SaveAsJpeg", "ImageFile-SaveAsJpeg", "destinationpath='c:\\out.jpg' format='888' quality='20'");
            AddEffect("  SaveAsPng", "ImageFile-SaveAsPng", "destinationpath='c:\\out.png' format='888'");
            AddEffect("  SaveAsTiff", "ImageFile-SaveAsTiff", "destinationpath='c:\\out.tiff' format='888' compression='1'");
            AddEffect("  SaveAsWmf", "ImageFile-SaveAsWmf", "destinationpath='c:\\out.wmf' format='888'");
            AddEffect("  SaveAsEmf", "ImageFile-SaveAsEmf", "destinationpath='c:\\out.emf' format='888'");
            AddEffect("  SaveAsPcx", "ImageFile-SaveAsPcx", "destinationpath='c:\\out.pcx' format='888'");
            AddEffect("  SaveAsTga", "ImageFile-SaveAsTga", "destinationpath='c:\\out.tga' format='888'");
            AddEffect("  SaveAsRas", "ImageFile-SaveAsRas", "destinationpath='c:\\out.ras' format='888'");
            AddEffect("  SaveAsIPod", "ImageFile-SaveAsIPod", "destinationpath='c:\\out.ipod' format='888'");
            AddEffect("  SaveAsPsd", "ImageFile-SaveAsPsd", "destinationpath='c:\\out.psd' format='8888'");
            AddEffect("  SaveAsJp2", "ImageFile-SaveAsJp2", "destinationpath='c:\\out.j2k' format='888' jp2='0' lossless='0' quality='90' filesize='0'");
            AddEffect("Paint", "", "");
            AddEffect2("  DrawCountdown", " <ImagePaint-DrawCountdown" + SplitParams(c_sPaintDrawCountdownAttr, true) + c_sTimeline + " </ImagePaint-DrawCountdown>");
            AddEffect("  SetPen", "ImagePaint-SetPen", "color='255' alpha='128' size='2'");
            AddEffect("  SetBrush", "ImagePaint-SetBrush", "type='3008' color1='255' color2='65535' alpha1='255' alpha2='255' texturepath='c:\\1.jpg' texturealpha='255' texturemode='0' rectable='1' rect-left='0' rect-top='0' rect-width='100' rect-height='100'");
            AddEffect("  SetFont", "ImagePaint-SetFont", "name='Tahoma' size='16' bold='1' italic='1' underline='1' strikeout='1'");
            AddEffect("  SetFormat", "ImagePaint-SetFormat", "antialiaspen='1' antialiastext='1' stringalignmentvertical='0' stringalignmenthorizontal='0' imagealphmode='1'");
            AddEffect("  DrawLine", "ImagePaint-DrawLine", "x1='1' y1='1' x2='100' y2='50' metric='0'");
            AddEffect("  DrawRectangle", "ImagePaint-DrawRectangle", "left='10' top='5' right='100' bottom='50' solid='1' metric='0'");
            AddEffect("  DrawEllipse", "ImagePaint-DrawEllipse", "left='10' top='5' right='100' bottom='50' solid='1' metric='0'");
            AddEffect("  DrawInvRectangle", "ImagePaint-DrawInvRectangle", "left='10' top='5' right='100' bottom='50' metric='0'");
            AddEffect("  DrawInvEllipse", "ImagePaint-DrawInvEllipse", "left='10' top='5' right='100' bottom='50' metric='0'");
            AddEffect("  DrawPie", "ImagePaint-DrawPie", "left='10' top='5' right='100' bottom='50' startangle='45' sweepangle='90' solid='1' metric='0'");
            AddEffect("  DrawSector", "ImagePaint-DrawSector", "left='10' top='5' right='100' bottom='50' radiusratio='2' startangle='45' sweepangle='90' solid='1' metric='0'");
            AddEffect("  DrawSimplePie", "ImagePaint-DrawSimplePie", "centerx='60' centery='40' radius='20' startangle='45' sweepangle='90' solid='1' metric='0'");
            AddEffect("  DrawSimpleSector", "ImagePaint-DrawSimpleSector", "centerx='60' centery='40' radius1='20' radius2='40' startangle='45' sweepangle='90' solid='1' metric='0'");
            AddEffect("  DrawPolyline", "ImagePaint-DrawPolyline", "points='10 5 100 5 10 100' metric='0'");
            AddEffect("  DrawPolygon", "ImagePaint-DrawPolygon", "points='10 5 100 5 10 100' solid='1' metric='0'");
            AddEffect("  DrawBorder", "ImagePaint-DrawBorder", "type='10100' size='100' metric='0'");
            AddEffect("  DrawMovObject", "ImagePaint-DrawMovObject", "type='1100' divisions='5' color='0' alpha='128'");
            AddEffect("  DrawCredits", "ImagePaint-DrawCredits", "type='5100' text='-= setup font and brush first =-' metric='0'");
            AddEffect("  DrawTextSimple", "ImagePaint-DrawTextSimple", "left='10' top='50' text='simple text' metric='0' aligntype='0' margin='10'");
            AddEffect("  DrawTextInRect", "ImagePaint-DrawTextInRect", "left='10' top='50' right='250' bottom='100' scale='1' text='extended text' metric='0'");
            AddEffect("  DrawImageFromFile", "ImagePaint-DrawImageFromFile", "left='0.3' top='0.3' right='0.9' bottom='0.9' filepath='c:\\1.jpg' metric='2' backcolor='0' alpha='192' scaletype='-1' scalecolor='255' angle='30'");
            AddEffect("  DrawImageFromFileEx", "ImagePaint-DrawImageFromFileEx", "src-metric='2' src-x='0' src-y='0' src-width='0' src-height='0' src-colorkey='-1' filepath='c:\\1.jpg' dst-metric='2' dst-x='0' dst-y='0' dst-width='0' dst-height='0' angle='0' alpha='128' cropscale='1' scaletype='0'");
            AddEffect("  DrawImageFromBuffer", "ImagePaint-DrawImageFromBuffer", "left='10' top='0' right='90' bottom='100' buffer='1' metric='0' scaletype='256' scalecolor='255' alpha='192'");
            AddEffect2("  DrawTimer", " <ImagePaint-DrawTimer" + SplitParams(c_sPaintDrawTimerAttr, true) + SplitParams(c_sHours, false) + SplitParams(c_sMinutes, false) + SplitParams(c_sSeconds, false) + SplitParams(c_sMilliseconds, false) + c_sTimeline + " </ImagePaint-DrawTimer>");
            AddEffect("  DrawFilm", "ImagePaint-DrawFilm", "speed='0' widefactor='24' vertical='0' drawborder = '1' backcolor = '0' filmsmidcolor = '2105376' filmsbordercolor = '6316128' blendcolor = '8421504' zoom = '0' alpha = '100' movedivider = '0' speedfactor = '1'");
            AddEffect("  DrawPuzzle", "ImagePaint-DrawPuzzle", " rows = '10' columns = '10' sidefactor = '1.0' type = '0' metric = '0'");
            AddEffect("  DrawFog", "ImagePaint-DrawFog", "color='60138' alpha='200'");
            AddEffectDrawTextEX("  DrawTextEx");
            AddEffect("Paint-TextStatic", "", "");
            AddEffect("  SetBrush", "ImagePaint-TextStatic-SetBrush", "type='2001' color1='255' color2='65536' alpha1='128' alpha2='0' texturepath='' texturealpha='100' texturemode='1' rectable='1' rect-left='0' rect-top='0' rect-width='100' rect-height='100'");
            AddEffect("  SetFont", "ImagePaint-TextStatic-SetFont", "name='Tahoma' size='16' bold='1' italic='1' underline='1' strikeout='1'");
            AddEffect("  SetFormat", "ImagePaint-TextStatic-SetFormat", "stringalignmentvertical='0' stringalignmenthorizontal='0' rotate='30'");
            AddEffect("  SetShadow", "ImagePaint-TextStatic-SetShadow", "visible='1' distancex='5' distancey='10' blursize='2' color='0' alpha='120'");
            AddEffect("  SetEdge", "ImagePaint-TextStatic-SetEdge", "visible='1' dist='5' color='65536' alpha='128'");
            AddEffect("  DrawInRect", "ImagePaint-TextStatic-DrawInRect", "left='10' top='50' right='250' bottom='100' text='DrawInRect' metric='0'");
            AddEffect("  DrawOnEllipse", "ImagePaint-TextStatic-DrawOnEllipse", "left='10' top='50' right='150' bottom='150' text='DrawOnEllipse' startangle='-30' sweepangle='80' metric='0'");
            AddEffect("  DrawOnPolyline", "ImagePaint-TextStatic-DrawOnPolyline", "text='DrawOnPolyline' points='10 5 100 140' metric='0'");
            AddEffect("Paint-TextDynamic", "", "");
            AddEffect("  SetBrush", "ImagePaint-TextDynamic-SetBrush", "type='0' color1='255' color2='65536' alpha1='255' alpha2='0' texturepath='' texturealpha='100' texturemode='1' rectable='1' rect-left='0' rect-top='0' rect-width='100' rect-height='100'");
            AddEffect("  SetFont", "ImagePaint-TextDynamic-SetFont", "name='Tahoma' size='24' bold='1' italic='1' underline='1' strikeout='1'");
            AddEffect("  SetShadow", "ImagePaint-TextDynamic-SetShadow", "visible='0' distancex='5' distancey='10' blursize='2' color='0' alpha='120'");
            AddEffect("  SetEdge", "ImagePaint-TextDynamic-SetEdge", "visible='0' dist='5' color='65536' alpha='128'");
            AddEffect("  Start", "ImagePaint-TextDynamic-Start", "text='Sample string' effectid='0' x='20' y='40' step='10' metric='0'");
            AddEffect("  Stop", "ImagePaint-TextDynamic-Stop", "");
            AddEffect("  Render", "ImagePaint-TextDynamic-Render", "");
            AddEffect("Composition", "", "");
            AddEffect("  Compose", "ImageCompose-Render", "effectid='3205' tileswidth='600' tilesheight='400'");
            AddEffect("Paint-Text", "", "");
            AddEffect("  AnimatedText (Rotated Rectangle)", "ImagePaint-DrawAnimatedText", "metric='0' rect-left='0' rect-top='0' rect-right='300' rect-bottom='300'\r\nanimation-fadein='1' animation-state='3' animation-fadeout='1' text-dir-x1='0' text-dir-y1='0' text-dir-x2='0' text-dir-y2='0' text-fillmode='0' text-typeeffects='1' text-simpleline='0' token='_' text='Simple string'\r\nfont-name='Arial' font-size='36' font-bold='1' font-italic='0' font-underline='0' font-strikeout='1' font-stringalignmenthorizontal='1' font-stringalignmentvertical='1' font-angle='0'\r\nbrush-type='0' brush-color1='255' brush-color2='65535' brush-alpha1='255' brush-alpha2='255' brush-texturepath='c:\\' brush-texturealpha='255' brush-texturemode='0' brush-rectable='0' brush-rect-left='0' brush-rect-top='0' brush-rect-width='0' brush-rect-height='0' pen-antialiasbrush='1'\r\nshadow-visible='1' shadow-distancex='5' shadow-distancey='10' shadow-blursize='3' shadow-color='0' shadow-alpha='120'\r\nedge-visible='1' edge-dist='3' edge-color='65536' edge-alpha='192'");
            AddEffect("  AnimatedText (Line)", "ImagePaint-DrawAnimatedText", "metric='0' rect-left='10' rect-top='20' rect-right='200' rect-bottom='100'\r\nanimation-fadein='0' animation-state='0' animation-fadeout='0' text='sample string'\r\nfont-name='Tahoma' font-size='16' font-bold='1' font-italic='0' font-underline='0' font-strikeout='0'\r\nbrush-type='2001' brush-color1='255' brush-color2='65536' brush-alpha1='128' brush-alpha2='0'\r\nshadow-visible='1' shadow-distancex='5' shadow-distancey='10' shadow-blursize='2' shadow-color='0' shadow-alpha='120'\r\nedge-visible='1' edge-dist='5' edge-color='65536' edge-alpha='128'");
            AddEffect("  AnimatedText (Ellipse, AlphaBlend)", "ImagePaint-DrawAnimatedText", "metric='0' rect-left='30' rect-top='30' rect-right='180' rect-bottom='160' ellipse-startangle='80' animation-fadein='1' animation-state='1' animation-fadeout='0' text='привет.участникам.соц..соревнования!' font-name='Tahoma' font-size='16' font-bold='1' font-italic='0' font-underline='0' font-strikeout='0' brush-type='2001' brush-color1='255' brush-color2='65536' brush-alpha1='128' brush-alpha2='0' shadow-visible='1' shadow-distancex='5' shadow-distancey='10' shadow-blursize='2' shadow-color='0' shadow-alpha='120' edge-visible='1' edge-dist='5' edge-color='65536' edge-alpha='128'");
            AddEffect("  AnimatedText (Rotated Rectangle & TextureBrush)", "ImagePaint-DrawAnimatedText", "metric='0' rect-left='10' rect-top='20' rect-right='200' rect-bottom='100' animation-fadein='1' animation-state='3' animation-fadeout='21' text='sample\r\nstring' font-name='Tahoma' font-size='36' font-bold='1' font-italic='0' font-underline='0' font-strikeout='0' font-stringalignmenthorizontal='1' font-stringalignmentvertical='1' font-angle='25' brush-type='3008' brush-color1='255' brush-color2='65536' brush-alpha1='128' brush-alpha2='255' brush-texturepath='C:\\Storage\\Work\\Temp\\New Folder\\Redist\\Bmp\\ms.bmp' brush-texturealpha='255' brush-texturemode='1' shadow-visible='1' shadow-distancex='5' shadow-distancey='10' shadow-blursize='2' shadow-color='0' shadow-alpha='120' edge-visible='0' edge-dist='5' edge-color='65536' edge-alpha='128'");
            AddEffect("  WritingText", "ImageTransform-EffectWritingText", "type='1' metric='0' rect-left='0' rect-top='0' rect-right='300' rect-bottom='300'\r\ntoken='_' text='Simple string'\r\nfont-name='Arial' font-size='36' font-bold='1' font-italic='0' font-underline='0' font-strikeout='1' font-stringalignmenthorizontal='1' font-stringalignmentvertical='1' font-angle='0'\r\nedge-visible='1' edge-dist='3' edge-color='65536' edge-alpha='192'");

        }

        private void treeViewPresets_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (e.Node != null && e.Node.Tag != null)
                textBoxXml.Text = ((string)e.Node.Tag).Replace("'", "\"");
        }

        private string SplitParams(string strParams, bool bSpaceShift)
        {
            string res = "\r\n" + (bSpaceShift ? "  " : "") + strParams.Replace("' ", "'\r\n  ");
            return res;
        }
        private void AddEffect(string strCaption, string strName, string strParams)
        {
            if (strName.Length < 1)
                treeViewPresets.Nodes.Add(strCaption);
            else
            {
                string strXml = "";

                strXml += "<transforms>\r\n";
                strXml += " <" + strName + " " + SplitParams(strParams, true) + ">";
                strXml += c_sTimeline;
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
        private void AddEffect2(string strCaption, string strXml)
        {
            string xml = "";

            xml += "<transforms>\r\n";
            xml += strXml;
            xml += "\r\n</transforms>";
            TreeNode oNode = new TreeNode(strCaption);

            oNode.Tag = xml;

            treeViewPresets.Nodes[treeViewPresets.Nodes.Count - 1].Nodes.Add(oNode);
        }
        private void AddEffectDrawTextEX(string strCaption)
        {
            string xml = "";
            xml += "<transforms>\r\n";
            xml += "   <ImagePaint-DrawTextEx\r\n";
            xml += "    text=\"Hi, dear the best rest friend!\r\n\r\n Can I help you?My favorite american film is Green Mile.\"\r\n";
            xml += "    metric=\"0\" widthmm=\"280\" heightmm=\"210\" widthl=\"6000\" heightl=\"4200\" left=\"0\" top=\"0\" right=\"400\" bottom=\"500\" vertical=\"0\" righttoleft=\"0\">\r\n";
            xml += "      <Attributes font-strikeout=\"1\" font-underline=\"1\" font-stringalignmentvertical=\"0\" shadow-visible=\"0\" font-name=\"Times New Roman\" font-size=\"28\"\r\n";
            xml += "       font-stringalignmenthorizontal=\"2\"/>\r\n";
            xml += "      <Paragraph count=\"48\" hasbullet=\"1\" textalignment=\"3\" linespacing=\"0\" spaceafter=\"0\"\r\n";
            xml += "       defaulttabsize=\"4\">\r\n";
            xml += "         <TabStop size=\"10\"/><TabStop size=\"10\"/><bulletchar>•</bulletchar>\r\n";
            xml += "      </Paragraph>\r\n";
            xml += "      <Paragraph count=\"40\" hasbullet=\"0\" textalignment=\"2\" linespacing=\"5\"/>\r\n";
            xml += "      <Character count=\"16\">\r\n";
            xml += "         <Attributes font-name=\"Arial\" font-size=\"36\" font-underline=\"1\" brush-color1=\"100\" brush-color2=\"100\"\r\n";
            xml += "          shadow-distancey=\"4\" shadow-distancex=\"3\" shadow-alpha=\"40\"/>\r\n";
            xml += "      </Character>\r\n";
            xml += "      <Character count=\"32\">\r\n";
            xml += "         <Attributes font-name=\"Times New Roman\" font-size=\"28\" brush-color1=\"10\" brush-color2=\"250\"/>\r\n";
            xml += "      </Character>\r\n";
            xml += "      <Character count=\"40\">\r\n";
            xml += "         <Attributes font-name=\"Times New Roman\" font-size=\"36\"/>\r\n";
            xml += "      </Character>\r\n";
            xml += "      <timeline type=\"1\" begin=\"0\" end=\"300\" fadein=\"125\" fadeout=\"125\" completeness=\"1.0\" />\r\n";
            xml += "   </ImagePaint-DrawTextEx>\r\n";
            xml += "</transforms>";

            TreeNode oNode = new TreeNode(strCaption);
            oNode.Tag = xml;

            treeViewPresets.Nodes[treeViewPresets.Nodes.Count - 1].Nodes.Add(oNode);
        }

        public string TransformXml
        {
            get { return textBoxXml.Text; }
        }

        private void menuFormat_Click(object sender, EventArgs e)
        {
            try
            {
                StringBuilder oStringBuilder = new StringBuilder();
                StringWriter oStringWriter = new StringWriter(oStringBuilder);
                XmlTextWriter oXmlWriter = new XmlTextWriter(oStringWriter);

                oXmlWriter.Namespaces = true;
                oXmlWriter.Formatting = Formatting.Indented;
                oXmlWriter.Indentation = 4;

                XmlDocument oDoc = new XmlDocument();

                oDoc.LoadXml(textBoxXml.Text);

                oDoc.WriteTo(oXmlWriter);

                textBoxXml.Text = oStringBuilder.ToString();
            }
            catch
            { 
            }
        }
    }
}
