#pragma once

namespace ImageStudio
{
	namespace Serialize
	{
		/* @desc */
		const CString c_strSerializeGroup							= _T("Transforms|transforms|Group|group|VideoTransforms|videotransforms|InitialVideoTransforms|initialvideotransforms");
		const CString c_strSerializeBufferClear						= _T("BufferClear|buffer-clear");
		const CString c_strSerializeBufferCopy						= _T("BufferCopy|buffer-copy");
		const CString c_strSerializeSetParamsUseVideoCard			= _T("SetParamsUseVideoCard|setparamsusevideocard");

		const CString c_strSerializeImageClear						= _T("ImageClear|transform-image-clear");
		const CString c_strSerializeImageCopy						= _T("ImageCopy|transform-image-copy");
		const CString c_strSerializeSetAdditionalParams				= _T("SetAdditionalParams|transform-setadditionalparams");

		const CString c_strSerializeAdjustBrightness				= _T("ImageTransform-AdjustBrightness|transform-adjust-brightness");
		const CString c_strSerializeAdjustBrightnessEx				= _T("ImageTransform-AdjustBrightnessEx|transform-adjust-brightness-2");
		const CString c_strSerializeAdjustContrast					= _T("ImageTransform-AdjustContrast|transform-adjust-contrast");
		const CString c_strSerializeAdjustGamma						= _T("ImageTransform-AdjustGamma|transform-adjust-gamma");
		const CString c_strSerializeAdjustEqualize					= _T("ImageTransform-AdjustEqualize|transform-adjust-equalize");
		const CString c_strSerializeAdjustGrayscale					= _T("ImageTransform-AdjustGrayscale|transform-adjust-grayscale");
		const CString c_strSerializeAdjustInvert					= _T("ImageTransform-AdjustInvert|transform-adjust-invert");
		const CString c_strSerializeAdjustAutocontrast				= _T("ImageTransform-AdjustAutocontrast|transform-adjust-autocontrast");
		const CString c_strSerializeAdjustAutolevels				= _T("ImageTransform-AdjustAutolevels|transform-adjust-autolevels");
		const CString c_strSerializeAdjustHue						= _T("ImageTransform-AdjustHue|transform-adjust-hue");
		const CString c_strSerializeAdjustSaturation				= _T("ImageTransform-AdjustSaturation|transform-adjust-saturation");
		const CString c_strSerializeAdjustPosterize					= _T("ImageTransform-AdjustPosterize|transform-adjust-posterize");
		const CString c_strSerializeAdjustTemperature				= _T("ImageTransform-AdjustTemperature|transform-adjust-temperature");
		const CString c_strSerializeAdjustColorize					= _T("ImageTransform-AdjustColorize|transform-adjust-colorize");
		const CString c_strSerializeAdjustThreshold					= _T("ImageTransform-AdjustThreshold|transform-adjust-threshold");
		const CString c_strSerializeAdjustSepia                     = _T("ImageTransform-AdjustSepia|transform-adjust-sepia");
		const CString c_strSerializeAdjustAutobrightness			= _T("ImageTransform-AdjustAutobrightness|transform-adjust-autobrightness");

		const CString c_strSerializeAdjustRecovery			        = _T("ImageTransform-AdjustRecovery|transform-adjust-recovery");
		const CString c_strSerializeAdjustFillLight			        = _T("ImageTransform-AdjustFillLight|transform-adjust-filllight");
		const CString c_strSerializeAdjustExposure			        = _T("ImageTransform-AdjustExposure|transform-adjust-exposure");
		const CString c_strSerializeAdjustBlacks			        = _T("ImageTransform-AdjustBlacks|transform-adjust-blacks");
		const CString c_strSerializeAdjustHighLight			        = _T("ImageTransform-AdjustHighLight|transform-adjust-highlight");
		const CString c_strSerializeAdjustShadows			        = _T("ImageTransform-AdjustShadows|transform-adjust-shadows");
		const CString c_strSerializeAdjustWhiteBalance              = _T("ImageTransform-AdjustWhiteBalance|transform-adjust-whitebalance");
		const CString c_strSerializeAdjustClarity		            = _T("ImageTransform-AdjustClarity|transform-adjust-clarity");
		const CString c_strSerializeAdjustVibrance		            = _T("ImageTransform-AdjustVibrance|transform-adjust-vibrance");
		const CString c_strSerializeAdjustAutoWhiteBalance          = _T("ImageTransform-AdjustAutoWhiteBalance|transform-adjust-autowhitebalance");

		const CString c_strSerializeEffectBlur						= _T("ImageTransform-EffectBlur|transform-effect-blur");
		const CString c_strSerializeEffectSharpen					= _T("ImageTransform-EffectSharpen|transform-effect-sharpen");
		const CString c_strSerializeEffectMosaic					= _T("ImageTransform-EffectMosaic|transform-effect-mosaic");
		const CString c_strSerializeEffectGaussianBlur				= _T("ImageTransform-EffectGaussianBlur|transform-effect-gaussian-blur");
		const CString c_strSerializeEffectNoise						= _T("ImageTransform-EffectNoise|transform-effect-noise");
		const CString c_strSerializeEffectDiffuse					= _T("ImageTransform-EffectDiffuse|transform-effect-diffuse");
		const CString c_strSerializeEffectMotionBlur				= _T("ImageTransform-EffectMotionBlur|transform-effect-motion-blur");
		const CString c_strSerializeEffectEmboss					= _T("ImageTransform-EffectEmboss|transform-effect-emboss");
		const CString c_strSerializeEffectKenBurns					= _T("ImageTransform-EffectKenBurns|transform-ken-burns");
		const CString c_strSerializeEffectMinimal					= _T("ImageTransform-EffectMinimal|transform-effect-minimal");
		const CString c_strSerializeEffectMaximal					= _T("ImageTransform-EffectMaximal|transform-effect-maximal");
		const CString c_strSerializeEffectDeinterlace				= _T("ImageTransform-EffectDeinterlace|transform-effect-deinterlace");
		const CString c_strSerializeEffectMedian					= _T("ImageTransform-EffectMedian|transform-effect-median");
		const CString c_strSerializeEffectChromoKeyRange			= _T("ImageTransform-EffectChromoKeyRange|transform-effect-chromokey-range");
		const CString c_strSerializeEffectChromoKeyLevel			= _T("ImageTransform-EffectChromoKeyLevel|transform-effect-chromokey-level");
		const CString c_strSerializeEffectCinema					= _T("ImageTransform-EffectCinema|transform-effect-cinema");
		const CString c_strSerializeEffectGlass						= _T("ImageTransform-EffectGlass|transform-effect-glass");
		const CString c_strSerializeEffectGlassMosaic				= _T("ImageTransform-EffectGlassMosaic|transform-effect-glass-mosaic");
		const CString c_strSerializeEffectPencilSketch				= _T("ImageTransform-EffectPencilSketch|transform-effect-pencil-sketch");
		const CString c_strSerializeEffectWaterColor				= _T("ImageTransform-EffectWaterColor|transform-effect-water-color");
		const CString c_strSerializeEffectGlow						= _T("ImageTransform-EffectGlow|transform-effect-glow");
		const CString c_strSerializeEffectTVSimulation				= _T("ImageTransform-EffectTVSimulation|transform-effect-tv-simulation");
		const CString c_strSerializeEffectGrayOut					= _T("ImageTransform-EffectGrayOut|transform-effect-gray-out");
		const CString c_strSerializeEffectWind						= _T("ImageTransform-EffectWind|transform-effect-wind");
		const CString c_strSerializeEffectNewsprint					= _T("ImageTransform-EffectNewsprint|transform-effect-newsprint");
		const CString c_strSerializeEffectSnow   					= _T("ImageTransform-EffectSnow|transform-effect-snow");
		const CString c_strSerializeEffectJpegCompression			= _T("ImageTransform-EffectJpegCompression|transform-effect-jpegcompression");
		const CString c_strSerializeEffectSoftFocus					= _T("ImageTransform-EffectSoftFocus|transform-effect-softfocus");
		const CString c_strSerializeEffectOldPhoto					= _T("ImageTransform-EffectOldPhoto|transform-effect-oldphoto");
		const CString c_strSerializeEffectVignette					= _T("ImageTransform-EffectVignette|transform-effect-vignette");
		const CString c_strSerializeEffectPostCrop					= _T("ImageTransform-EffectPostCrop|transform-effect-postcrop");
		const CString c_strSerializeEffectSetAlphaChannel			= _T("ImageTransform-EffectSetAlphaChannel|transform-effect-setalphachannel");
		const CString c_strSerializeEffectZoomingSphere				= _T("ImageTransform-EffectZoomingSphere|transform-effect-zoomingsphere");
		const CString c_strSerializeEffectWaterMirrow				= _T("ImageTransform-EffectWaterMirrow|transform-effect-watermirrow");
		const CString c_strSerializeEffectShapeMosaic				= _T("ImageTransform-EffectShapeMosaic|transform-effect-shapemosaic");
		const CString c_strSerializeEffectCanvas					= _T("ImageTransform-EffectCanvas|transform-effect-canvas");
		const CString c_strSerializeEffectGlow2						= _T("ImageTransform-EffectGlow2|transform-effect-glow2");
		const CString c_strSerializeEffectColorExchange				= _T("ImageTransform-EffectColorExchange|transform-effect-colorexchange");
		const CString c_strSerializeEffectWritingText				= _T("ImageTransform-EffectWritingText|transform-effect-writingtext");
		const CString c_strSerializeEffectParticles					= _T("ImageTransform-EffectParticles|transform-effect-particles");
		const CString c_strSerializeEffectAnaglyph3d				= _T("ImageTransform-EffectAnaglyph3d|transform-effect-anaglyph3d");
		const CString c_strSerializeEffectFog						= _T("ImageTransform-EffectFog|transform-effect-fog");
		
		const CString c_strSerializeTransformFlip					= _T("ImageTransform-TransformFlip|transform-transform-flip");
		const CString c_strSerializeTransformFlip2					= _T("ImageTransform-TransformFlip2|transform-transform-flip2");
		const CString c_strSerializeTransformPerspective			= _T("ImageTransform-TransformPerspective|transform-transform-perspective");
		const CString c_strSerializeTransformSkew					= _T("ImageTransform-TransformSkew|transform-transform-skew");
		const CString c_strSerializeTransformShift					= _T("ImageTransform-TransformShift|transform-transform-shift");
		const CString c_strSerializeTransformRotate					= _T("ImageTransform-TransformRotate|transform-transform-rotate");
		const CString c_strSerializeTransformRotateAdvanced			= _T("ImageTransform-TransformRotateAdvanced|transform-transform-rotateadvanced");
		const CString c_strSerializeTransformRotateSimple			= _T("ImageTransform-TransformRotateSimple|transform-transform-rotatesimple");
		const CString c_strSerializeTransformResample				= _T("ImageTransform-TransformResample|transform-transform-resample");
		const CString c_strSerializeTransformZoom					= _T("ImageTransform-TransformZoom|transform-transform-zoom");
		const CString c_strSerializeTransformZoomLinear				= _T("ImageTransform-TransformZoomLinear|transform-transform-zoomlinear");
		const CString c_strSerializeTransformMirror					= _T("ImageTransform-TransformMirror|transform-transform-mirror");
		const CString c_strSerializeTransformResize					= _T("ImageTransform-TransformResize|transform-transform-resize");
		const CString c_strSerializeTransformTwirl					= _T("ImageTransform-TransformTwirl|transform-transform-twirl");
		const CString c_strSerializeTransformSphere					= _T("ImageTransform-TransformSphere|transform-transform-sphere");
		const CString c_strSerializeTransformCylinder				= _T("ImageTransform-TransformCylinder|transform-transform-cylinder");
		const CString c_strSerializeTransformWave  					= _T("ImageTransform-TransformWave|transform-effect-wave");
		const CString c_strSerializeTransformRipple                 = _T("ImageTransform-TransformRipple|transform-effect-ripple");
		const CString c_strSerializeTransformRotateSpecial          = _T("ImageTransform-TransformRotateSpecial|transform-transform-rotatespecial");
		const CString c_strSerializeTransformLinear		            = _T("ImageTransform-TransformLinear|transform-transform-linear");
		const CString c_strSerializeTransformQuadro		            = _T("ImageTransform-TransformQuadro|transform-transform-quadro");
		const CString c_strSerializeTransformGlassTile	            = _T("ImageTransform-TransformGlassTile|transform-transform-glasstile");
		const CString c_strSerializeTransformBrokenGlass            = _T("ImageTransform-TransformBrokenGlass|transform-transform-brokenglass");

		const CString c_strSerializeFilterPrewitt					= _T("ImageTransform-FilterPrewitt|transform-transform-filter-prewitt");
		const CString c_strSerializeFilterScharr					= _T("ImageTransform-FilterScharr|transform-transform-filter-scharr");
		const CString c_strSerializeFilterSobel						= _T("ImageTransform-FilterSobel|transform-transform-filter-sobel");
		const CString c_strSerializeFilterRoberts					= _T("ImageTransform-FilterRoberts|transform-transform-filter-roberts");
		const CString c_strSerializeFilterLaplace					= _T("ImageTransform-FilterLaplace|transform-transform-filter-laplace");
		const CString c_strSerializeFilterHipass					= _T("ImageTransform-FilterHipass|transform-transform-filter-hipass");
		const CString c_strSerializeFilterLowpass					= _T("ImageTransform-FilterLowpass|transform-transform-filter-lowpass");
		const CString c_strSerializeFilterBlur						= _T("ImageTransform-FilterBlur|transform-transform-filter-blur");
		const CString c_strSerializeFilterCanny						= _T("ImageTransform-FilterCanny|transform-transform-filter-canny");

		const CString c_strSerializeFileLoadImage					= _T("ImageFile-LoadImage|image-file-load-image");
		const CString c_strSerializeFileLoadScreenshot				= _T("ImageFile-LoadScreenshot|image-file-load-screenshot");
		const CString c_strSerializeFileLoadRaw					    = _T("ImageFile-LoadRaw|image-file-load-raw");
		const CString c_strSerializeFileSaveAsBmp					= _T("ImageFile-SaveAsBmp|image-file-save-bmp");
		const CString c_strSerializeFileSaveAsGif					= _T("ImageFile-SaveAsGif|image-file-save-gif");
		const CString c_strSerializeFileSaveAsJpeg					= _T("ImageFile-SaveAsJpeg|image-file-save-jpeg");
		const CString c_strSerializeFileSaveAsPng					= _T("ImageFile-SaveAsPng|image-file-save-png");
		const CString c_strSerializeFileSaveAsTiff					= _T("ImageFile-SaveAsTiff|image-file-save-tiff");
		const CString c_strSerializeFileSaveAsWmf					= _T("ImageFile-SaveAsWmf|image-file-save-wmf");
		const CString c_strSerializeFileSaveAsEmf					= _T("ImageFile-SaveAsEmf|image-file-save-emf");
		const CString c_strSerializeFileSaveAsPcx					= _T("ImageFile-SaveAsPcx|image-file-save-pcx");
		const CString c_strSerializeFileSaveAsTga					= _T("ImageFile-SaveAsTga|image-file-save-tga");
		const CString c_strSerializeFileSaveAsRas					= _T("ImageFile-SaveAsRas|image-file-save-ras");
		const CString c_strSerializeFileSaveAsIPod					= _T("ImageFile-SaveAsIPod|image-file-save-ipod");
		const CString c_strSerializeFileSaveAsPsd					= _T("ImageFile-SaveAsPsd|image-file-save-psd");
		const CString c_strSerializeFileSaveAsJp2                   = _T("ImageFile-SaveAsJp2|image-file-save-jp2");

		const CString c_strSerializePaintSetPen						= _T("ImagePaint-SetPen|image-paint-set-pen");
		const CString c_strSerializePaintSetBrush					= _T("ImagePaint-SetBrush|image-paint-set-brush");
		const CString c_strSerializePaintSetFont					= _T("ImagePaint-SetFont|image-paint-set-font");
		const CString c_strSerializePaintSetFormat					= _T("ImagePaint-SetFormat|image-paint-set-format");
		const CString c_strSerializePaintDrawLine					= _T("ImagePaint-DrawLine|image-paint-draw-line");
		const CString c_strSerializePaintDrawRectangle				= _T("ImagePaint-DrawRectangle|image-paint-draw-rectangle");
		const CString c_strSerializePaintDrawEllipse				= _T("ImagePaint-DrawEllipse|image-paint-draw-ellipse");
		const CString c_strSerializePaintDrawInvRectangle			= _T("ImagePaint-DrawInvRectangle|image-paint-draw-inv-rectangle");
		const CString c_strSerializePaintDrawInvEllipse				= _T("ImagePaint-DrawInvEllipse|image-paint-draw-inv-ellipse");
		const CString c_strSerializePaintDrawPolyline				= _T("ImagePaint-DrawPolyline|image-paint-draw-polyline");
		const CString c_strSerializePaintDrawPolygon				= _T("ImagePaint-DrawPolygon|image-paint-draw-polygon");
		const CString c_strSerializePaintDrawPie					= _T("ImagePaint-DrawPie|image-paint-draw-pie");
		const CString c_strSerializePaintDrawSector					= _T("ImagePaint-DrawSector|image-paint-draw-sector");
		const CString c_strSerializePaintDrawSimplePie				= _T("ImagePaint-DrawSimplePie|image-paint-draw-simple-pie");
		const CString c_strSerializePaintDrawSimpleSector			= _T("ImagePaint-DrawSimpleSector|image-paint-draw-simple-sector");
		const CString c_strSerializePaintDrawBorder					= _T("ImagePaint-DrawBorder|image-paint-draw-border");
		const CString c_strSerializePaintDrawMovObject				= _T("ImagePaint-DrawMovObject|image-paint-draw-mov-object");
		const CString c_strSerializePaintDrawCredits				= _T("ImagePaint-DrawCredits|image-paint-draw-credits");
		const CString c_strSerializePaintDrawTextSimple				= _T("ImagePaint-DrawTextSimple|image-paint-draw-text-simple");
		const CString c_strSerializePaintDrawTextInRect				= _T("ImagePaint-DrawTextInRect|image-paint-draw-text-inrect");
		const CString c_strSerializePaintDrawImageFromFile			= _T("ImagePaint-DrawImageFromFile|image-paint-draw-image-file");
		const CString c_strSerializePaintDrawImageFromFileEx		= _T("ImagePaint-DrawImageFromFileEx|image-paint-draw-image-file-ex");
		const CString c_strSerializePaintDrawImageFromBuffer		= _T("ImagePaint-DrawImageFromBuffer|image-paint-draw-image-buffer");
		const CString c_strSerializePaintDrawTimer					= _T("ImagePaint-DrawTimer|image-paint-draw-timer");
		const CString c_strSerializePaintDrawImageFromStorage       = _T("ImagePaint-DrawImageFromStorage|image-paint-draw-image-storage");
		const CString c_strSerializePaintDrawFilm                   = _T("ImagePaint-DrawFilm|image-paint-draw-film");
		const CString c_strSerializePaintDrawPuzzle                 = _T("ImagePaint-DrawPuzzle|image-paint-draw-puzzle");
		const CString c_strSerializePaintDrawPath	                = _T("ImagePaint-DrawGraphicPath|image-paint-draw-graphicpath");
		const CString c_strSerializePaintDrawTextEx	                = _T("ImagePaint-DrawTextEx|image-paint-draw-textex");
		const CString c_strSerializePaintDrawAutoShape              = _T("ImagePaint-DrawAutoShape|image-paint-draw-autoshape");
		const CString c_strSerializePaintDrawCountdown				= _T("ImagePaint-DrawCountdown|image-paint-draw-countdown");

		const CString c_strSerializePaintTextStaticSetBrush			= _T("ImagePaint-TextStatic-SetBrush|image-paint-textstatic-setbrush");
		const CString c_strSerializePaintTextStaticSetFont			= _T("ImagePaint-TextStatic-SetFont|image-paint-textstatic-setfont");
		const CString c_strSerializePaintTextStaticSetFormat		= _T("ImagePaint-TextStatic-SetFormat|image-paint-textstatic-setformat");
		const CString c_strSerializePaintTextStaticSetShadow		= _T("ImagePaint-TextStatic-SetShadow|image-paint-textstatic-setshadow");
		const CString c_strSerializePaintTextStaticSetEdge			= _T("ImagePaint-TextStatic-SetEdge|image-paint-textstatic-setedge");
		const CString c_strSerializePaintTextStaticDrawInRect		= _T("ImagePaint-TextStatic-DrawInRect|image-paint-textstatic-draw-inrect");
		const CString c_strSerializePaintTextStaticDrawOnEllipse	= _T("ImagePaint-TextStatic-DrawOnEllipse|image-paint-textstatic-draw-onellipse");
		const CString c_strSerializePaintTextStaticDrawOnPolyline	= _T("ImagePaint-TextStatic-DrawOnPolyline|image-paint-textstatic-draw-onpolyline");

		const CString c_strSerializePaintTextDynamicSetBrush		= _T("ImagePaint-TextDynamic-SetBrush|image-paint-textdynamic-setbrush");
		const CString c_strSerializePaintTextDynamicSetFont			= _T("ImagePaint-TextDynamic-SetFont|image-paint-textdynamic-setfont");
		const CString c_strSerializePaintTextDynamicSetShadow		= _T("ImagePaint-TextDynamic-SetShadow|image-paint-textdynamic-setshadow");
		const CString c_strSerializePaintTextDynamicSetEdge			= _T("ImagePaint-TextDynamic-SetEdge|image-paint-textdynamic-setedge");
		const CString c_strSerializePaintTextDynamicStart			= _T("ImagePaint-TextDynamic-Start|image-paint-textdynamic-start");
		const CString c_strSerializePaintTextDynamicStop			= _T("ImagePaint-TextDynamic-Stop|image-paint-textdynamic-stop");
		const CString c_strSerializePaintTextDynamicRender			= _T("ImagePaint-TextDynamic-Render|image-paint-textdynamic-render");

		const CString c_strSerializeComposeRender					= _T("ImageCompose-Render|image-compose-render|VideoCompose|ImageCompose|");

		const CString c_strSerializePaintAnimatedText				= _T("ImagePaint-DrawAnimatedText");


		const CString c_strSerializePaintDrawImageFromFileAnimate	= _T("ImagePaint-DrawImageFromFileAnimate|image-paint-draw-image-file-animate");
		const CString c_strSerializePaintDrawPathAnimate            = _T("ImagePaint-DrawGraphicPathAnimate|image-paint-draw-graphicpath-animate");

		/* @desc */

		const int c_nSerializeNone							= 100000000;
		const int c_nSerializeGroup							= 100000101;
		const int c_nSerializeBufferClear					= 100000102;
		const int c_nSerializeBufferCopy					= 100000103;
		const int c_nSerializeSetParamsUseVideoCard			= 100000104;

		const int c_nSerializeImageClear					= 100010101;
		const int c_nSerializeImageCopy						= 100010102;
		const int c_nSerializeSetAdditionalParams           = 100010103;

		const int c_nSerializeAdjustBrightness				= 100100101;
		const int c_nSerializeAdjustBrightnessEx			= 100100102;
		const int c_nSerializeAdjustContrast				= 100100103;
		const int c_nSerializeAdjustGamma					= 100100104;
		const int c_nSerializeAdjustEqualize				= 100100105;
		const int c_nSerializeAdjustGrayscale				= 100100106;
		const int c_nSerializeAdjustInvert					= 100100107;
		const int c_nSerializeAdjustAutocontrast			= 100100108;
		const int c_nSerializeAdjustAutolevels				= 100100109;
		const int c_nSerializeAdjustHue						= 100100110;
		const int c_nSerializeAdjustSaturation				= 100100111;
		const int c_nSerializeAdjustPosterize				= 100100112;
		const int c_nSerializeAdjustTemperature 			= 100100113;
		const int c_nSerializeAdjustColorize				= 100100114;
		const int c_nSerializeAdjustThreshold				= 100100115;
		const int c_nSerializeAdjustSepia                   = 100100116;
		const int c_nSerializeAdjustAutobrightness			= 100100117;

		const int c_nSerializeAdjustRecovery			    = 100100118;
		const int c_nSerializeAdjustFillLight			    = 100100119;
		const int c_nSerializeAdjustExposure			    = 100100120;
		const int c_nSerializeAdjustBlacks			        = 100100121;
		const int c_nSerializeAdjustHighLight			    = 100100122;
		const int c_nSerializeAdjustShadows			        = 100100123;
		const int c_nSerializeAdjustWhiteBalance            = 100100124;
		const int c_nSerializeAdjustClarity				    = 100100125;
		const int c_nSerializeAdjustVibrance	            = 100100126;
		const int c_nSerializeAdjustAutoWhiteBalance        = 100100127;

		const int c_nSerializeEffectBlur					= 100100201;
		const int c_nSerializeEffectSharpen 				= 100100202;
		const int c_nSerializeEffectMosaic					= 100100203;
		const int c_nSerializeEffectGaussianBlur			= 100100204;
		const int c_nSerializeEffectNoise					= 100100205;
		const int c_nSerializeEffectDiffuse 				= 100100206;
		const int c_nSerializeEffectMotionBlur				= 100100207;
		const int c_nSerializeEffectEmboss					= 100100208;
		const int c_nSerializeEffectMinimal					= 100100209;
		const int c_nSerializeEffectMaximal					= 100100210;
		const int c_nSerializeEffectDeinterlace 			= 100100211;
		const int c_nSerializeEffectMedian					= 100100212;
		const int c_nSerializeEffectChromoKeyRange			= 100100213;
		const int c_nSerializeEffectChromoKeyLevel			= 100100214;
		const int c_nSerializeEffectCinema					= 100100215;
		const int c_nSerializeEffectGlass					= 100100216;
		const int c_nSerializeEffectGlassMosaic				= 100100217;
		const int c_nSerializeEffectPencilSketch			= 100100218;
		const int c_nSerializeEffectWaterColor				= 100100219;
		const int c_nSerializeEffectGlow					= 100100220;
		const int c_nSerializeEffectTVSimulation			= 100100221;
		const int c_nSerializeEffectGrayOut					= 100100222;
		const int c_nSerializeEffectWind					= 100100223;
		const int c_nSerializeEffectNewsprint				= 100100224;
		const int c_nSerializeEffectSnow     				= 100100225;
		const int c_nSerializeEffectJpegCompression			= 100100226;
		const int c_nSerializeEffectSoftFocus				= 100100227;
		const int c_nSerializeEffectOldPhoto				= 100100228;
		const int c_nSerializeEffectKenBurns				= 100000229;
		const int c_nSerializeEffectVignette				= 100100230;
		const int c_nSerializeEffectPostCrop				= 100100231;
		const int c_nSerializeEffectSetAlphaChannel			= 100100232;
		const int c_nSerializeEffectZoomingSphere			= 100100233;
		const int c_nSerializeEffectWaterMirrow				= 100100234;
		const int c_nSerializeEffectShapeMosaic				= 100100235;
		const int c_nSerializeEffectCanvas					= 100100236;
		const int c_nSerializeEffectGlow2					= 100100237;
		const int c_nSerializeEffectColorExchange			= 100100238;
		const int c_nSerializeEffectWritingText				= 100100239;
		const int c_nSerializeEffectParticles				= 100100240;
		const int c_nSerializeEffectAnaglyph3d				= 100100241;
		const int c_nSerializeEffectFog			            = 100300242;

		const int c_nSerializeTransformFlip					= 100100301;
		const int c_nSerializeTransformPerspective			= 100100302;
		const int c_nSerializeTransformSkew					= 100100303;
		const int c_nSerializeTransformShift				= 100100304;
		const int c_nSerializeTransformRotate				= 100100305;
		const int c_nSerializeTransformResample 			= 100100306;
		const int c_nSerializeTransformZoom					= 100100307;
		const int c_nSerializeTransformMirror				= 100100308;
		const int c_nSerializeTransformResize				= 100100309;
		const int c_nSerializeTransformTwirl				= 100100310;
		const int c_nSerializeTransformSphere				= 100100311;
		const int c_nSerializeTransformCylinder 			= 100100312;
		const int c_nSerializeTransformRotateAdvanced		= 100100313;
		const int c_nSerializeTransformZoomLinear			= 100100314;
		const int c_nSerializeTransformWave					= 100100315;
		const int c_nSerializeTransformRipple				= 100100316;
		const int c_nSerializeTransformRotateSpecial		= 100100317;
		const int c_nSerializeTransformRotateSimple			= 100100318;
		const int c_nSerializeTransformLinear				= 100100319;
		const int c_nSerializeTransformQuadro				= 100100320;
		const int c_nSerializeTransformFlip2				= 100100321;
		const int c_nSerializeTransformGlassTile			= 100100322;
		const int c_nSerializeTransformBrokenGlass			= 100100323;

		const int c_nSerializeFilterPrewitt					= 100100401;
		const int c_nSerializeFilterScharr					= 100100402;
		const int c_nSerializeFilterSobel					= 100100403;
		const int c_nSerializeFilterRoberts					= 100100404;
		const int c_nSerializeFilterLaplace					= 100100405;
		const int c_nSerializeFilterHipass					= 100100406;
		const int c_nSerializeFilterLowpass					= 100100407;
		const int c_nSerializeFilterBlur					= 100100408;
		const int c_nSerializeFilterCanny					= 100100409;

		const int c_nSerializeFileLoadImage					= 100200101;
		const int c_nSerializeFileLoadScreenshot			= 100200102;
		const int c_nSerializeFileLoadRaw					= 100200103;
		const int c_nSerializeFileSaveAsBmp					= 100200201;
		const int c_nSerializeFileSaveAsGif					= 100200202;
		const int c_nSerializeFileSaveAsJpeg				= 100200203;
		const int c_nSerializeFileSaveAsPng					= 100200204;
		const int c_nSerializeFileSaveAsTiff				= 100200205;
		const int c_nSerializeFileSaveAsWmf 				= 100200206;
		const int c_nSerializeFileSaveAsEmf 				= 100200207;
		const int c_nSerializeFileSaveAsPcx 				= 100200208;
		const int c_nSerializeFileSaveAsTga 				= 100200209;
		const int c_nSerializeFileSaveAsRas 				= 100200210;
		const int c_nSerializeFileSaveAsIPod				= 100200211;
		const int c_nSerializeFileSaveAsPsd					= 100200212;
		const int c_nSerializeFileSaveAsJp2                 = 100200213;

		const int c_nSerializePaintSetPen					= 100300201;
		const int c_nSerializePaintSetBrush 				= 100300202;
		const int c_nSerializePaintSetFont					= 100300203;
		const int c_nSerializePaintSetFormat 				= 100300204;
		const int c_nSerializePaintDrawLine	 				= 100300101;
		const int c_nSerializePaintDrawRectangle			= 100300102;
		const int c_nSerializePaintDrawEllipse				= 100300103;
		const int c_nSerializePaintDrawInvRectangle 		= 100300104;
		const int c_nSerializePaintDrawInvEllipse			= 100300105;
		const int c_nSerializePaintDrawPolyline 			= 100300106;
		const int c_nSerializePaintDrawPolygon				= 100300107;
		const int c_nSerializePaintDrawPie					= 100300108;
		const int c_nSerializePaintDrawSector				= 100300109;
		const int c_nSerializePaintDrawSimplePie			= 100300110;
		const int c_nSerializePaintDrawSimpleSector 		= 100300111;
		const int c_nSerializePaintDrawBorder				= 100300112;
		const int c_nSerializePaintDrawMovObject			= 100300113;
		const int c_nSerializePaintDrawCredits				= 100300114;
		const int c_nSerializePaintDrawTextSimple			= 100300115;
		const int c_nSerializePaintDrawTextInRect			= 100300116;
		const int c_nSerializePaintDrawImageFromFile		= 100300117;
		const int c_nSerializePaintDrawImageFromBuffer		= 100300118;
		const int c_nSerializePaintDrawTimer                = 100300119;
		const int c_nSerializePaintDrawImageFromStorage		= 100300120;
		const int c_nSerializePaintDrawFilm                 = 100300121;
		const int c_nSerializePaintDrawPuzzle               = 100300122;
		const int c_nSerializePaintDrawPath		            = 100300123;
		const int c_nSerializePaintDrawTextEx	            = 100300124;
		const int c_nSerializePaintDrawAutoShape            = 100300125;
		const int c_nSerializePaintDrawImageFromFileEx		= 100300126;
		const int c_nSerializePaintDrawCountdown            = 100300127;
		
		const int c_nSerializePaintTextStaticSetBrush		= 100400201;
		const int c_nSerializePaintTextStaticSetFont		= 100400202;
		const int c_nSerializePaintTextStaticSetFormat		= 100400203;
		const int c_nSerializePaintTextStaticSetShadow		= 100400204;
		const int c_nSerializePaintTextStaticSetEdge		= 100400205;
		const int c_nSerializePaintTextStaticDrawInRect		= 100400101;
		const int c_nSerializePaintTextStaticDrawOnEllipse	= 100400102;
		const int c_nSerializePaintTextStaticDrawOnPolyline	= 100400103;
		
		const int c_nSerializePaintTextDynamicSetBrush		= 100500201;
		const int c_nSerializePaintTextDynamicSetFont		= 100500202;
		const int c_nSerializePaintTextDynamicSetShadow		= 100500203;
		const int c_nSerializePaintTextDynamicSetEdge		= 100500204;
		const int c_nSerializePaintTextDynamicStart			= 100500101;
		const int c_nSerializePaintTextDynamicStop			= 100500102;
		const int c_nSerializePaintTextDynamicRender		= 100500103;

		const int c_nSerializeComposeRender					= 100600101;

		const int c_nSerializePaintAnimatedText				= 100700101;

		const int c_nSerializePaintDrawImageFromFileAnimate	= 100800101;
		const int c_nSerializePaintDrawPathAnimate          = 100800102;

		const int c_nMetricPixels = 0;
		const int c_nMetricPercents = 1;
		const int c_nMetricNormalizedCoordinates = 2;
		const int c_nMetricAlternativeNormalizedCoordinates = 3;

		const int c_nMaskNone = 0;
		const int c_nMaskAllPixels = 1;
		const int c_nMaskRectangle = 2;
		const int c_nMaskInverseRectangle = 3;
		const int c_nMaskEllipse = 4;
		const int c_nMaskInverseEllipse = 5;
		const int c_nMaskPolygon = 6;
		const int c_nMaskInversePolygon = 7;
	}
}
