#include "stdafx.h"
#include "ufrawdata_core.h"

#define __L(STR)	OLESTR(STR)

__conf_data::~__conf_data()
{
		if (darkframe)
		{
			darkframe->_close();
			delete darkframe;
			darkframe = NULL;
		}
}

/*!
 *	\todo	Надо сделать нормальное заполнение свойств, иначе это источник ошибок
 */
const conf_data conf_default = {
    /* Internal data */
    sizeof(conf_data), 7, /* confSize, version */

    /* Image manipulation settings */
    camera_wb, 0, /* wb, WBTuning */
    6500, 1.0, /* temperature, green */
    { -1.0, -1.0, -1.0, -1.0 }, /* chanMul[] */
    0.0, /* wavelet denoising threshold */
#ifdef UFRAW_CONTRAST
    1.0, /* global contrast */
#endif
    0.0, 1.0, 0.0, /* exposure, saturation, black */
    0, /* ExposureNorm */
    restore_lch_details, /* restoreDetails */
    digital_highlights, /* clipHighlights */
    disabled_state, /* autoExposure */
    disabled_state, /* autoBlack */
    camera_curve, camera_curve+1, /* BaseCurveIndex, BaseCurveCount */
    /* BaseCurve data defaults */
    { { N_("Manual curve"), TONE_CURVE, 0.0, 1.0, 0.0, 1.0, 1.0,
	  2 , { { 0.0, 0.0 }, { 1.0, 1.0 } } },
      { N_("Linear curve"), TONE_CURVE, 0.0, 1.0, 0.0, 1.0, 1.0,
	  2 , { { 0.0, 0.0 }, { 1.0, 1.0 } } },
      { N_("Custom curve"), TONE_CURVE, 0.0, 1.0, 0.0, 1.0, 1.0,
	  2 , { { 0.0, 0.0 }, { 1.0, 1.0 } } },
      { N_("Camera curve"), TONE_CURVE, 0.0, 1.0, 0.0, 1.0, 1.0,
	  2 , { { 0.0, 0.0 }, { 1.0, 1.0 } } }
    },
    linear_curve, linear_curve+1, /* curveIndex, curveCount */
    /* Curve data defaults */
    { { N_("Manual curve"), TONE_CURVE, 0.0, 1.0, 0.0, 1.0, 1.0,
	  2 , { { 0.0, 0.0 }, { 1.0, 1.0 } } },
      { N_("Linear curve"), TONE_CURVE, 0.0, 1.0, 0.0, 1.0, 1.0,
	  2 , { { 0.0, 0.0 }, { 1.0, 1.0 } } }
    },
    { 1, 0, 0 } , { 2, 1, 2 }, /* profileIndex[], profileCount[] */
    /* Profile data defaults */
    { { { N_("No profile"), __L(""), "", 0.45, 0.1, 0 },
	{ N_("Color matrix"), __L(""), "", 0.45, 0.1, 0 },
	{ ("Some ICC Profile"), __L(""), "", 0.45, 0.0, 0 } },
      { { N_("sRGB"), __L(""), "", 0.0, 0.0, 8 },
	{ ("Some ICC Profile"), __L(""), "", 0.0, 0.0, 8 } },
      { { N_("System default"), __L(""), "", 0.0, 0.0, 0 },
	{ N_("sRGB"), __L(""), "", 0.0, 0.0, 0 },
	{ "Some ICC Profile", __L(""), "", 0.0, 0.0, 0 } } },
    { (Intent)0, (Intent)0, (Intent)0 }, /* intent */
    ahd_interpolation, 0, /* interpolation, smoothing */
    "", NULL, /* darkframeFile, darkframe */
    -1, -1, -1, -1, /* Crop X1,Y1,X2,Y2 */
    0, /* rotationAngle */
    grayscale_none, /* grayscale mode */
    { 1.0, 1.0, 1.0 },/* grayscale mixer */
//#ifndef _CONF_NO_SAVE	
	/* Save options */
	"", "", "", /* inputFilename, outputFilename, outputPath */
    "", "", /* inputURI, inputModTime */
    ppm_type, 85, no_id, /* type, compression, createID */
    TRUE, /* embedExif */
    FALSE, /* progressiveJPEG */
//#endif
    1, 0, /* shrink, size */
    FALSE, /* overwrite existing files without asking */
    FALSE, /* losslessCompress */
    FALSE, /* load embedded preview image */
    TRUE, /* rotate to camera's setting */


    /* GUI settings */
#ifndef _CONF_NO_GUI    
	25.0, 4, TRUE, /* Zoom, Scale, LockAspect */
    enabled_state, /* saveConfiguration */
    rgb_histogram, /* histogram */
    linear_histogram, /* liveHistogramScale */
    linear_histogram, /* rawHistogramScale */
    { TRUE, TRUE }, /* expander[] */
    FALSE, /* overExp indicator */
    FALSE, /* underExp indicator */
    TRUE, /* blinkOverUnder indicators */
    FALSE, /* RememberOutputPath */
    FALSE, /* WindowMaximized */
    "", "", /* curvePath, profilePath */
    FALSE, /* silent */
#ifdef WIN32
    "gimp-win-remote gimp-2.4.exe", /* remoteGimpCommand */
#elif HAVE_GIMP_2_6
    "gimp", /* remoteGimpCommand */
#else
    "gimp-remote", /* remoteGimpCommand */
#endif
#endif

    /* EXIF data */
    -1, /* orientation */
    0.0, 0.0, 0.0, 0.0, 1.0, /* iso_speed, shutter, aperture, focal_len, subject_dist */
    "", "", "", "", /* exifSource, isoText, shutterText, apertureText */
    "", "", "", "", /* focalLenText, focalLen35Text, lensText, flashText */
    "", /* whiteBalanceText */
    "", "", "", /* timestamp, make, model */
    0, /* timestamp */
    "", "", /* real_make, real_model */

#ifdef HAVE_LENSFUN
    NULL,                         /* mount/camera/lens database */
    NULL,                         /* camera description */
    NULL,                         /* lens description */
    { LF_DIST_MODEL_NONE, 0, { 0, 0, 0 } }, /* lens distortion parameters */
    { LF_TCA_MODEL_NONE, 0, { 0, 0 } }, /* lens tca parameters */
    /* lens vignetting parameters */ 
    { LF_VIGNETTING_MODEL_NONE, 0, 0, 0, { 0, 0, 0 } },
    LF_UNKNOWN,                   /* lens type */
    0,                            /* lens postprocessing scale power-of-two */ 
#endif
};


static const char *interpolationNames[] =
    { "ahd", "vng", "four-color", "ppg", "bilinear", "none", "half", "eahd", NULL };
static const char *restoreDetailsNames[] =
    { "clip", "lch", "hsv", NULL };
static const char *clipHighlightsNames[] =
    { "digital", "film", NULL };
static const char *intentNames[] =
    { "perceptual", "relative", "saturation", "absolute", "disable", NULL };
static const char *grayscaleModeNames[] =
    { "none", "lightness", "luminance", "value", "mixer", NULL };


void conf_init (conf_data *c)
{
#ifdef HAVE_LENSFUN
    static lfDatabase *lensdb = NULL;

    if (!lensdb)
    {
        /* Load lens database */
        lensdb = lf_db_new ();
        lf_db_load (lensdb);
    }
#endif

    *c = conf_default;

#ifdef HAVE_LENSFUN
    c->lensdb = lensdb;
#endif
}

HRESULT conf_set_cmd(conf_data *conf, const conf_data *cmd)
{
    if (cmd->overwrite!=-1)
		conf->overwrite = cmd->overwrite;

    if (cmd->restoreDetails!=-1)
		conf->restoreDetails = cmd->restoreDetails;

    if (cmd->clipHighlights!=-1)
		conf->clipHighlights = cmd->clipHighlights;

    if (cmd->losslessCompress!=-1)
		conf->losslessCompress = cmd->losslessCompress;

    if (cmd->embedExif!=-1)
		conf->embedExif = cmd->embedExif;

    if (cmd->embeddedImage!=-1) conf->embeddedImage = cmd->embeddedImage;
    if (cmd->rotate!=-1) conf->rotate = cmd->rotate;
    if (cmd->rotationAngle!=NULLF) conf->rotationAngle = cmd->rotationAngle;
    
	if (cmd->CropX1 !=-1) conf->CropX1 = cmd->CropX1;
    
	if (cmd->CropY1 !=-1) conf->CropY1 = cmd->CropY1;
    
	if (cmd->CropX2 !=-1) conf->CropX2 = cmd->CropX2;
    
	if (cmd->CropY2 !=-1) conf->CropY2 = cmd->CropY2;

#ifndef _CONF_NO_GUI
	if (cmd->silent!=-1)
		conf->silent = cmd->silent;
#endif
    
	if (cmd->compression!=NULLF)
		conf->compression = cmd->compression;
    
	if (cmd->autoExposure) {
		conf->autoExposure = cmd->autoExposure;
    }
    if (cmd->threshold!=NULLF) conf->threshold = cmd->threshold;

#ifdef UFRAW_CONTRAST
    if (cmd->contrast!=NULLF) conf->contrast = cmd->contrast;
#endif
    
	if (cmd->exposure!=NULLF) {
		conf->exposure = cmd->exposure;
		conf->autoExposure = disabled_state;
    }
    if (strlen(cmd->wb)>0) g_strlcpy(conf->wb, cmd->wb, max_name);
    
	if (cmd->temperature!=NULLF)
		conf->temperature = cmd->temperature;

    if (cmd->green!=NULLF) conf->green = cmd->green;

    if (cmd->temperature!=NULLF || cmd->green!=NULLF)
		g_strlcpy(conf->wb, manual_wb, max_name);
    
	if (cmd->profile[0][0].gamma!=NULLF)
		conf->profile[0][conf->profileIndex[0]].gamma =	cmd->profile[0][0].gamma;
    
	if (cmd->profile[0][0].linear!=NULLF)
		conf->profile[0][conf->profileIndex[0]].linear = cmd->profile[0][0].linear;
    
	if (cmd->profile[1][0].BitDepth!=-1)
		conf->profile[1][conf->profileIndex[1]].BitDepth =	cmd->profile[1][0].BitDepth;
    
	if (cmd->saturation!=NULLF)
		conf->saturation=cmd->saturation;
    
	if (cmd->grayscaleMode!=NULLF)
		conf->grayscaleMode=cmd->grayscaleMode;
    
	if (cmd->BaseCurveIndex>=0) conf->BaseCurveIndex = cmd->BaseCurveIndex;
    
	if (cmd->curveIndex>=0) conf->curveIndex = cmd->curveIndex;
    
	if (cmd->autoBlack)
	{
		conf->autoBlack = cmd->autoBlack;
    }
    
	if (cmd->black!=NULLF)
	{
		CurveDataSetPoint(&conf->curve[conf->curveIndex],0, cmd->black, 0);
		conf->autoBlack = disabled_state;
    }

    if (cmd->interpolation>=0) conf->interpolation = cmd->interpolation;
    
	if (cmd->interpolation==obsolete_eahd_interpolation)
	{
		conf->interpolation = ahd_interpolation;
		conf->smoothing = 3;
    }

    if (cmd->shrink!=NULLF)
	{
		conf->shrink = cmd->shrink;
		conf->size = 0;
	
		if (conf->interpolation==half_interpolation)
			conf->interpolation = ahd_interpolation;
    }

    if (cmd->size!=NULLF)
	{
		conf->size = cmd->size;
		conf->shrink = 1;
		if (conf->interpolation==half_interpolation)
			conf->interpolation = ahd_interpolation;
    }
    
	if (cmd->type>=0) conf->type = cmd->type;

    if (cmd->createID>=0) conf->createID = cmd->createID;
    if (strlen(cmd->darkframeFile)>0)
		g_strlcpy(conf->darkframeFile, cmd->darkframeFile, max_path);
    
	if (cmd->darkframe!=NULL)
		conf->darkframe = cmd->darkframe;

    if (strlen(cmd->outputPath)>0)
		g_strlcpy(conf->outputPath, cmd->outputPath, max_path);

    if (strlen(cmd->outputFilename)>0)
	{
		if ( conf->createID!=no_id && !strcmp(cmd->outputFilename,"-") && !cmd->embeddedImage )
		{
			// todo	
			//ufraw_message(UFRAW_ERROR, _("cannot --create-id with stdout"));
			return E_FAIL;
		}
		g_strlcpy(conf->outputFilename, cmd->outputFilename, max_path);
    }

    return S_OK;
}

HRESULT conf_copy_save(conf_data *dst, const conf_data *src)
{
    /* Filenames get special treatment and are not simply copied
    g_strlcpy(dst->inputFilename, src->inputFilename, max_path);
    g_strlcpy(dst->outputFilename, src->outputFilename, max_path);
    g_strlcpy(dst->outputPath, src->outputPath, max_path);
    */
    g_strlcpy(dst->inputURI, src->inputURI, max_path);
    g_strlcpy(dst->inputModTime, src->inputModTime, max_name);
    dst->type = src->type;
    dst->compression = src->compression;
    dst->createID = src->createID;
    dst->embedExif = src->embedExif;
    dst->shrink = src->shrink;
    dst->size = src->size;
    dst->overwrite = src->overwrite;

#ifndef _CONF_NO_GUI    
	dst->RememberOutputPath = src->RememberOutputPath;
#endif
   
	dst->progressiveJPEG = src->progressiveJPEG;
    dst->losslessCompress = src->losslessCompress;
    dst->embeddedImage = src->embeddedImage;

	return S_OK;
}

HRESULT conf_copy_transform(conf_data *dst, const conf_data *src)
{
    dst->orientation = src->orientation;
    dst->CropX1 = src->CropX1;
    dst->CropY1 = src->CropY1;
    dst->CropX2 = src->CropX2;
    dst->CropY2 = src->CropY2;
    dst->rotationAngle = src->rotationAngle;
	return S_OK;
}

HRESULT conf_copy_image(conf_data *dst, const conf_data *src)
{
    int i, j;

    dst->interpolation = src->interpolation;
    dst->smoothing = src->smoothing;
    g_strlcpy(dst->wb, src->wb, max_name);
    dst->WBTuning = src->WBTuning;
    dst->temperature = src->temperature;
    dst->green = src->green;
    for (i=0; i<4; i++)
		dst->chanMul[i] = src->chanMul[i];
    // make and model are 'part of' ChanMul,
    // since on different make and model ChanMul are meaningless
    g_strlcpy(dst->make, src->make, max_name);
    g_strlcpy(dst->model, src->model, max_name);
    dst->threshold = src->threshold;
    dst->exposure = src->exposure;
#ifdef UFRAW_CONTRAST
    dst->contrast = src->contrast;
#endif
    dst->ExposureNorm = src->ExposureNorm;
    dst->saturation = src->saturation;
    dst->black = src->black;
    dst->autoExposure = src->autoExposure;
    dst->autoBlack = src->autoBlack;
    dst->restoreDetails = src->restoreDetails;
    dst->clipHighlights = src->clipHighlights;
    dst->grayscaleMode = src->grayscaleMode;
    memcpy(dst->grayscaleMixer, src->grayscaleMixer,  sizeof dst->grayscaleMixer);
    g_strlcpy(dst->darkframeFile, src->darkframeFile, max_path);
    
	// We only copy the current BaseCurve 
    if (src->BaseCurveIndex<=camera_curve)
	{
		dst->BaseCurveIndex = src->BaseCurveIndex;
		if (src->BaseCurveIndex==manual_curve)
			dst->BaseCurve[manual_curve] = src->BaseCurve[manual_curve];
    }
	else
	{
		// For non-standard curves we look for a curve with the same name
		// and override it, assuming it is the same curve. 
		for (i=camera_curve+1; i<dst->BaseCurveCount; i++)
		{
			if (!strcmp(dst->BaseCurve[i].name,	src->BaseCurve[src->BaseCurveIndex].name))
			{
				dst->BaseCurve[i] = src->BaseCurve[src->BaseCurveIndex];
				dst->BaseCurveIndex = i;
				break;
			}
		}
		// If the curve was not found we add it. 
		if (i==dst->BaseCurveCount)
		{
			// If there is no more room we throw away the last curve. 
			if (dst->BaseCurveCount==max_curves)
				dst->BaseCurveCount--;
			dst->BaseCurve[dst->BaseCurveCount] = src->BaseCurve[src->BaseCurveIndex];
			dst->BaseCurveIndex = dst->BaseCurveCount;
			dst->BaseCurveCount++;
		}
	}
    // We only copy the current curve
    if (src->curveIndex<=linear_curve)
	{
		dst->curveIndex = src->curveIndex;
		if (src->curveIndex==manual_curve)
			dst->curve[manual_curve] = src->curve[manual_curve];
    }
	else
	{
		// For non-standard curves we look for a curve with the same name
		// and override it, assuming it is the same curve.
		for (i=camera_curve+1; i<dst->curveCount; i++)
		{
			if (!strcmp(dst->curve[i].name, src->curve[src->curveIndex].name))
			{
				dst->curve[i] = src->curve[src->curveIndex];
				dst->curveIndex = i;
				break;
			}
		}
		// If the curve was not found we add it.
		if (i==dst->curveCount)
		{
			// If there is no more room we throw away the last curve.
			if (dst->curveCount==max_curves)
				dst->curveCount--;
		    
			dst->curve[dst->curveCount] = src->curve[src->curveIndex];

			dst->curveIndex = dst->curveCount;

			dst->curveCount++;
		}
	}
    // We only copy the current input/output profile
    for (j=0; j<profile_types; j++)
	{
		// Ignore the display profile
		if ( j==display_profile)
			continue;
		if (src->profileIndex[j]==0)
		{
			dst->profileIndex[j] = src->profileIndex[j];
			dst->profile[j][0] = src->profile[j][0];
		}
		else
		{
		    /* For non-standard profiles we look for a profile with the same
		     * name and override it, assuming it is the same profile. */
		    for (i=1; i<dst->profileCount[j]; i++)
			{
				if (!strcmp(dst->profile[j][i].name, src->profile[j][src->profileIndex[j]].name))
				{
					dst->profile[j][i] = src->profile[j][src->profileIndex[j]];
					dst->profileIndex[j] = i;
					break;
				}
			}

			// If the profile was not found we add it.
			if (i==dst->profileCount[j])
			{
				// If there is no more room we throw away the last profile.
				if (dst->profileCount[j]==max_profiles)
					dst->profileCount[j]--;
				dst->profile[j][dst->profileCount[j]] = src->profile[j][src->profileIndex[j]];
				dst->profileIndex[j] = dst->profileCount[j];
				dst->profileCount[j]++;
			}
		}
    }
    dst->intent[out_profile] = src->intent[out_profile];
    dst->intent[display_profile] = src->intent[display_profile];

#ifdef HAVE_LENSFUN
    dst->lensdb = src->lensdb;
    if (src->camera)
    {
        dst->camera = lf_camera_new ();
        lf_camera_copy (dst->camera, src->camera);
    }
    if (src->lens)
    {
        dst->lens = lf_lens_new ();
        lf_lens_copy (dst->lens, src->lens);
    }
    dst->lens_distortion = src->lens_distortion;
    dst->lens_tca = src->lens_tca;
    dst->lens_vignetting = src->lens_vignetting;
#endif /* HAVE_LENSFUN */
	return S_OK;
}
