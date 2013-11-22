/*!	\file	ufrawdeveloper_core.cpp
 *	\brief	Functions for developing images or more exactly pixels.
 *			Адаптирован для COM-компоненты AVSImageRaw3: функции обернуты в класс,
 *			используется wchar_t для имен файлов
 */

#include "stdafx.h"
#include "wchar.h"
#include "ufrawdata_core.h"
#include ".\dcraw\nikon_curve.h"
#include ".\lcmslib\include\lcms.h"

//#ifdef _DEBUG
//#	pragma comment(lib,"./lcmslib/bin/lcmslibD.lib")
//#else
//#	pragma comment(lib,"./lcmslib/bin/lcmslib.lib")
//#endif

#define _HR HRESULT hr
#define _CHECK_RET_(HR) 	if (FAILED(hr = (HR))) { return hr;}

/*!
 *	\brief	Перевод строки wchar_t * в char * в соответствии с заданной кодовой страницей
 *	
 *	\param[in]	wstr		исходная строка
 *	\param[in]	codepage	Кодовая страница, по умолчанию используется CP_ACP — текущая кодовая страница Windows
 *
 *	\return	строка char *, под неё выделяется память, клиент обязан после использования вызвать для неё функцию free
 */
static char* __WtoS(const wchar_t* wstr,int codepage = CP_ACP)
{
     if(!wstr)
		 return NULL;
     char* str;
     int strlrn=0;
     strlrn = WideCharToMultiByte(codepage,0,wstr,-1,NULL,0,0,0); 
     if(!strlrn)
		 return NULL;
     
	 str = (char*)malloc(sizeof(char)*strlrn);//new char[strlrn]; 
     
	 int res = (int)WideCharToMultiByte(codepage,0,wstr,(int)wcslen(wstr)+1,str,(int)strlrn,0,0);
     return str;
}

HRESULT	c_developer_data::_init()
{
	int i;
	developer_data *d = &(this->_m_developer_data);
	d->mode			= (DeveloperMode)(-1);
	d->gamma		= -1;
	d->linear		= -1;
	d->saturation	= -1;
#ifdef UFRAW_CONTRAST
	d->contrast = -1;
#endif
	for (i=0; i<profile_types; i++)
	{
		d->profile[i] = NULL;
		//strcpy(d->profileFile[i],"no such file");
		wcscpy(d->profileFile[i],OLESTR("no such file"));
	}
	memset(&d->baseCurveData, 0, sizeof(d->baseCurveData));
	d->baseCurveData.m_gamma = -1.0;
	memset(&d->luminosityCurveData, 0, sizeof(d->luminosityCurveData));
	d->luminosityCurveData.m_gamma = -1.0;
	d->luminosityProfile = NULL;
	LPGAMMATABLE *TransferFunction = (LPGAMMATABLE *)d->TransferFunction;
	TransferFunction[0] = cmsAllocGamma(0x100);
	TransferFunction[1] = TransferFunction[2] = cmsBuildGamma(0x100, 1.0);
	d->saturationProfile = NULL;
	d->intent[out_profile] = (Intent)(-1);
	d->intent[display_profile] = (Intent)(-1);
	d->updateTransform = TRUE;
	d->colorTransform = NULL;
	d->grayscaleMode = (GrayscaleMode)(-1);
	d->grayscaleMixer[0] = d->grayscaleMixer[1] = d->grayscaleMixer[2] = -1;
	cmsSetErrorHandler(lcms_message);
	return S_OK;
}

HRESULT	c_developer_data::_destroy()
{
	int i;
	developer_data *d = &(this->_m_developer_data);

	for (i=0; i<profile_types; i++)
		if (d->profile[i]!=NULL)
		{
			cmsCloseProfile((cmsHPROFILE)(d->profile[i]));
			d->profile[i] = NULL;
		}

	cmsCloseProfile((cmsHPROFILE)(d->luminosityProfile));
	d->luminosityProfile = NULL;
	
	cmsFreeGamma((LPGAMMATABLE)(d->TransferFunction[0]));
	d->TransferFunction[0] = NULL;

	cmsFreeGamma((LPGAMMATABLE)(d->TransferFunction[1]));
	d->TransferFunction[1] = NULL;

	cmsCloseProfile(d->saturationProfile);
	if (d->colorTransform!=NULL)
	{
		cmsDeleteTransform(d->colorTransform);
		d->colorTransform = NULL;
	}
	return S_OK;
}

//static char *embedded_display_profile = "embedded display profile";
static wchar_t *embedded_display_profile = OLESTR("embedded display profile");

/* Update the profile in the developer
* and init values in the profile if needed */
HRESULT	c_developer_data::_profile(int type, profile_data *p)
{
	developer_data *d = &(this->_m_developer_data);
	// embedded_display_profile were handled by developer_display_profile()

	//if ( strcmp(d->profileFile[type],embedded_display_profile)==0 )
	if ( wcscmp(d->profileFile[type],embedded_display_profile)==0 )
		return S_FALSE;

	//if (strcmp(p->file, d->profileFile[type]))
	if (wcscmp(p->file, d->profileFile[type]))
	{
		//g_strlcpy(d->profileFile[type], p->file, max_path);
		wcscpy_s(d->profileFile[type],max_path,p->file);
		
		if (d->profile[type]!=NULL)
			cmsCloseProfile(d->profile[type]);

		//if (!strcmp(d->profileFile[type],""))
		if (!wcscmp(d->profileFile[type],OLESTR("")))
			d->profile[type] = cmsCreate_sRGBProfile();
		else
		{

			//char *filename = uf_win32_locale_filename_from_utf8(d->profileFile[type]);
			
			char *filename = __WtoS(d->profileFile[type]);
			d->profile[type] = cmsOpenProfileFromFile(filename, "r");
			free(filename);
			//uf_win32_locale_filename_free(filename);
			if (d->profile[type]==NULL)
				d->profile[type] = cmsCreate_sRGBProfile();
		}
		
		d->updateTransform = TRUE;
	}

	if (d->updateTransform)
	{
		if (d->profile[type]!=NULL)
			g_strlcpy(p->productName, cmsTakeProductName(d->profile[type]),	max_name);
		else
			strcpy(p->productName, "");
	}
	return S_OK;
}

HRESULT	c_developer_data::_display_profile(unsigned char *profile, int size, char productName[])
{
	developer_data *d = &(this->_m_developer_data);
	int type = display_profile;
	if ( profile!=NULL )
	{
		if (d->profile[type]!=NULL)
			cmsCloseProfile(d->profile[type]);
		d->profile[type] = cmsOpenProfileFromMem(profile, size);
		g_free(profile);

		if (d->profile[type]==NULL)
			d->profile[type] = cmsCreate_sRGBProfile();

		//if ( strcmp(d->profileFile[type], embedded_display_profile)!=0 )
		if ( wcscmp(d->profileFile[type], embedded_display_profile)!=0 )
		{
			// start using embedded profile
			//g_strlcpy(d->profileFile[type], embedded_display_profile, max_path);
			wcscpy_s(d->profileFile[type],max_path,embedded_display_profile);
			d->updateTransform = TRUE;
		}
	} else
	{
		//if ( strcmp(d->profileFile[type], embedded_display_profile)==0 )
		if ( wcscmp(d->profileFile[type], embedded_display_profile)==0 )
		{
			// embedded profile is no longer used
			if (d->profile[type]!=NULL)
				cmsCloseProfile(d->profile[type]);
			d->profile[type] = cmsCreate_sRGBProfile();
			//strcpy(d->profileFile[type], "");
			wcscpy(d->profileFile[type],OLESTR(""));
			d->updateTransform = TRUE;
		}
	}

	if ( d->updateTransform )
	{
		if ( d->profile[type]!=NULL )
			g_strlcpy(productName, cmsTakeProductName(d->profile[type]),max_name);
		else
			strcpy(productName, "");
	}
	return S_OK;
}

static double clamp(double in, double min, double max)
{
	return (in < min) ? min : (in > max) ? max : in;
}

struct contrast_saturation
{
	double contrast;
	double saturation;
};

/* Scale in along a curve from min to max by scale */
static double scale_curve(double in, double min, double max, double scale)
{
	double halfrange = (max - min) / 2.0;
	/* Normalize in to [ -1, 1 ] */
	double value = clamp((in - min) / halfrange - 1.0, -1.0, 1.0);
	/* Linear scaling makes more visual sense for low contrast values. */
	if (scale > 1.0) {
		double n = fabs(value);
		if (n > 1.0)
			n = 1.0;
		scale = n <= 0.0 ? 0.0 : ( 1.0 - pow( 1.0 - n, scale ) ) / n;
	}
	return clamp((value * scale + 1.0) * halfrange + min, min, max);
}

static const double max_luminance = 100.0;
static const double max_colorfulness = 181.019336; /* sqrt(128*128+128*128) */

static int contrast_saturation_sampler(WORD In[], WORD Out[], LPVOID Cargo)
{
	cmsCIELab Lab;
	cmsCIELCh LCh;
	const struct contrast_saturation* cs = (contrast_saturation*)Cargo;

	cmsLabEncoded2Float(&Lab, In);
	cmsLab2LCh(&LCh, &Lab);
	LCh.L = scale_curve(LCh.L, 0.0, max_luminance, cs->contrast);
	LCh.C = scale_curve(LCh.C, -max_colorfulness, max_colorfulness,cs->saturation);
	cmsLCh2Lab(&Lab, &LCh);
	cmsFloat2LabEncoded(Out, &Lab);

	return TRUE;
}

/* Based on lcms' cmsCreateBCHSWabstractProfile() */
static cmsHPROFILE create_contrast_saturation_profile(double contrast, double saturation)
{
	cmsHPROFILE hICC;
	LPLUT Lut;
	struct contrast_saturation cs = { contrast, saturation };

	hICC = _cmsCreateProfilePlaceholder();
	if (hICC==NULL) return NULL;// can't allocate

	cmsSetDeviceClass(hICC, icSigAbstractClass);
	cmsSetColorSpace(hICC, icSigLabData);
	cmsSetPCS(hICC, icSigLabData);
	cmsSetRenderingIntent(hICC, INTENT_PERCEPTUAL);

	// Creates a LUT with 3D grid only
	Lut = cmsAllocLUT();
	cmsAlloc3DGrid(Lut, 11, 3, 3);
	if (!cmsSample3DGrid(Lut, contrast_saturation_sampler, &cs , 0)) {
		// Shouldn't reach here
		cmsFreeLUT(Lut);
		cmsCloseProfile(hICC);
		return NULL;
	}
	// Create tags
	cmsAddTag(hICC, icSigMediaWhitePointTag, (LPVOID) cmsD50_XYZ());
	cmsAddTag(hICC, icSigAToB0Tag, (LPVOID) Lut);
	// LUT is already on virtual profile
	cmsFreeLUT(Lut);
	return hICC;
}

/* Find a for which (1-exp(-a x)/(1-exp(-a)) has derivative b at x=0 */
/* In other words, solve a/(1-exp(-a))==b */
static double findExpCoeff(double b)
{
	double a, bg;
	int trycount;
	if (b<=1)
		return 0;

	if (b<2)
		a=(b-1)/2;
	else
		a=b;
	bg = a/(1-exp(-a));
	/* The limit on try is just to be sure there is no infinite loop. */
	for (trycount=0; abs(bg-b)>0.001 || trycount<100; trycount++) {
		a = a + (b-bg);
		bg = a/(1-exp(-a));
	}
	return a;
}

static HRESULT developer_create_transform(developer_data *d, DeveloperMode mode)
{

	if (!d)
		return E_POINTER;

	if ( !d->updateTransform )
		return S_FALSE;

	d->updateTransform = FALSE;
	if (d->colorTransform!=NULL)
		cmsDeleteTransform(d->colorTransform);

	int targetProfile;
	if ( mode==file_developer || mode==auto_developer )
	{
		targetProfile = out_profile;
	}
	else
	{ /* mode==display_developer */
		targetProfile = display_profile;
	}

	/* When softproofing is disabled, use the out_profile intent. */
	if ( mode==file_developer || mode==auto_developer || d->intent[display_profile]==disable_intent )
	{
		/* No need for proofing transformation. */
		/*if ( strcmp(d->profileFile[in_profile],"")==0 &&
			strcmp(d->profileFile[targetProfile],"")==0 &&
			d->luminosityProfile==NULL && d->saturationProfile==NULL )*/
		if ( wcscmp(d->profileFile[in_profile],OLESTR(""))==0 &&
			wcscmp(d->profileFile[targetProfile],OLESTR(""))==0 &&
			d->luminosityProfile==NULL && d->saturationProfile==NULL )
		{
			/* No transformation at all. */
			d->colorTransform = NULL;
#if defined(LCMS_VERSION) && LCMS_VERSION <= 113 /* Bypass a lcms 1.13 bug. */
		} else if ( d->luminosityProfile==NULL && d->saturationProfile==NULL ) {
			d->colorTransform = cmsCreateTransform(
				d->profile[in_profile], TYPE_RGB_16,
				d->profile[targetProfile], TYPE_RGB_16,
				d->intent[out_profile], 0);
#endif
		}
		else
		{
			cmsHPROFILE prof[4];
			int i = 0;
			prof[i++] = d->profile[in_profile];
			if ( d->luminosityProfile!=NULL )
				prof[i++] = d->luminosityProfile;
			if ( d->saturationProfile!=NULL )
				prof[i++] = d->saturationProfile;
			prof[i++] = d->profile[targetProfile];
			d->colorTransform =
				cmsCreateMultiprofileTransform(prof, i,	TYPE_RGB_16, TYPE_RGB_16, d->intent[out_profile], 0);
		}
	} else
	{
		/* Create a proofing profile */
		if ( d->luminosityProfile==NULL && d->saturationProfile==NULL )
		{
			/* No intermediate profiles, we can use lcms proofing directly. */
			d->colorTransform = cmsCreateProofingTransform(
				d->profile[in_profile], TYPE_RGB_16,
				d->profile[display_profile], TYPE_RGB_16,
				d->profile[out_profile],
				d->intent[out_profile], d->intent[display_profile],
				cmsFLAGS_SOFTPROOFING);
		} else
		{
			/* Following code imitates the function
			* cmsCreateMultiprofileProofingTransform(),
			* which does not exist in lcms. */
			cmsHPROFILE prof[3];
			int i = 0;
			prof[i++] = d->profile[in_profile];
			if ( d->luminosityProfile!=NULL )
				prof[i++] = d->luminosityProfile;
			if ( d->saturationProfile!=NULL )
				prof[i++] = d->saturationProfile;
			d->colorTransform = cmsCreateMultiprofileTransform(prof, i,
				TYPE_RGB_16, NOCOLORSPACECHECK(TYPE_RGB_16),
				d->intent[out_profile], cmsFLAGS_SOFTPROOFING);

			prof[0] = cmsTransform2DeviceLink(d->colorTransform,cmsFLAGS_GUESSDEVICECLASS);
			cmsDeleteTransform(d->colorTransform);
			d->colorTransform = cmsCreateProofingTransform(
				prof[0], TYPE_RGB_16,
				d->profile[display_profile], TYPE_RGB_16,
				d->profile[out_profile],
				d->intent[out_profile], d->intent[display_profile],
				cmsFLAGS_SOFTPROOFING);
		}
	}
}

HRESULT c_developer_data::_prepare(conf_data *conf,
								   int rgbMax, float rgb_cam[3][4], int colors, int useMatrix,
								   DeveloperMode mode)
{
	_HR;
	developer_data * d = &_m_developer_data;
	unsigned c, i;
	profile_data *in, *out, *display;
	CurveData *baseCurve, *curve;
	double total;

	if ( mode!=d->mode )
	{
		d->mode = mode;
		d->updateTransform = TRUE;
	}
	in = &conf->profile[in_profile][conf->profileIndex[in_profile]];
	/* For auto-tools we create an sRGB output. */
	if ( mode==auto_developer )
		out = &conf->profile[out_profile][0];
	else
		out = &conf->profile[out_profile][conf->profileIndex[out_profile]];
	display = &conf->profile[display_profile]
	[conf->profileIndex[display_profile]];
	baseCurve = &conf->BaseCurve[conf->BaseCurveIndex];
	curve = &conf->curve[conf->curveIndex];

	d->rgbMax = rgbMax;
	d->colors = colors;
	d->useMatrix = useMatrix;

	double max = 0;
	/* We assume that min(conf->chanMul[c])==1.0 */
	for (c=0; c<d->colors; c++)
		max = MAX(max, conf->chanMul[c]);

	d->max = 0x10000 / max;
	/* rgbWB is used in dcraw_finalized_interpolation() before the Bayer
	* Interpolation. It is normalized to guaranty that values do not
	* exceed 0xFFFF */
	for (c=0; c<d->colors; c++)
		d->rgbWB[c] = conf->chanMul[c] * d->max	* 0xFFFF / d->rgbMax;

	if (d->useMatrix)
	{
		for (i=0; i<3; i++)
		{
			for (c=0; c<d->colors; c++)
				d->colorMatrix[i][c] = rgb_cam[i][c]*0x10000;
		}
	}

	switch (conf->grayscaleMode)
	{
	case grayscale_mixer:
		d->grayscaleMode = grayscale_mixer;
		for (c = 0, total = 0.0; c < 3; ++c)
			total += fabs(conf->grayscaleMixer[c]);
		total = total == 0.0 ? 1.0 : total;
		for (c = 0; c < 3; ++c)
			d->grayscaleMixer[c] = conf->grayscaleMixer[c] / total;
		break;

	case grayscale_lightness:
	case grayscale_value:
		d->grayscaleMode = (GrayscaleMode)conf->grayscaleMode;
		break;

	default:
		d->grayscaleMode = grayscale_none;
	}

	d->restoreDetails = conf->restoreDetails;
	int clipHighlights = conf->clipHighlights;
	unsigned exposure = pow(2, conf->exposure) * 0x10000;

	/* Handle the exposure normalization for Canon EOS cameras. */

	if ( conf->ExposureNorm>0 )
		exposure = (guint64)exposure * d->rgbMax / conf->ExposureNorm;

	if ( exposure>=0x10000 )
		d->restoreDetails = clip_details;

	if ( exposure<=0x10000 ) clipHighlights = digital_highlights;

	/* Check if gamma curve data has changed. */
	if ( in->gamma!=d->gamma || in->linear!=d->linear ||
		exposure!=d->exposure || clipHighlights!=d->clipHighlights ||
		memcmp(baseCurve, &d->baseCurveData, sizeof(CurveData))!=0 )
	{
		d->baseCurveData = *baseCurve;
		guint16 BaseCurve[0x10000];
		CurveSample *cs = CurveSampleInit(0x10000, 0x10000);
		//ufraw_message(UFRAW_RESET, NULL);

		if (CurveDataSample(baseCurve, cs)!=NC_SUCCESS)
		{
			//ufraw_message(UFRAW_REPORT, NULL);
			for (i=0; i<0x10000; i++)
				cs->m_Samples[i] = i;
		}

		for (i=0; i<0x10000; i++)
			BaseCurve[i] = cs->m_Samples[i];

		CurveSampleFree(cs);

		d->gamma = in->gamma;
		d->linear = in->linear;
		d->exposure = exposure;
		d->clipHighlights = clipHighlights;
		guint16 FilmCurve[0x10000];
		if ( d->clipHighlights==film_highlights )
		{
			/* Exposure is set by FilmCurve[].
			* Set initial slope to d->exposuse/0x10000 */
			double a = findExpCoeff((double)d->exposure/0x10000);
			for (i=0; i<0x10000; i++) FilmCurve[i] =
				(1-exp(-a*i/0x10000)) / (1-exp(-a)) * 0xFFFF;
		} else
		{ /* digital highlights */
			for (i=0; i<0x10000; i++)
				FilmCurve[i] = i;
		}
		double a, b, c, g;
		/* The parameters of the linearized gamma curve are set in a way that
		* keeps the curve continuous and smooth at the connecting point.
		* d->linear also changes the real gamma used for the curve (g) in
		* a way that keeps the derivative at i=0x10000 constant.
		* This way changing the linearity changes the curve behaviour in
		* the shadows, but has a minimal effect on the rest of the range. */
		if (d->linear<1.0)
		{
			g = d->gamma*(1.0-d->linear)/(1.0-d->gamma*d->linear);
			a = 1.0/(1.0+d->linear*(g-1));
			b = d->linear*(g-1)*a;
			c = pow(a*d->linear+b, g)/d->linear;
		}
		else
		{
			a = b = g = 0.0;
			c = 1.0;
		}
		for (i=0; i<0x10000; i++)
		{
			if (BaseCurve[FilmCurve[i]]<0x10000*d->linear)
				d->gammaCurve[i] = MIN(c*BaseCurve[FilmCurve[i]], 0xFFFF);
			else
				d->gammaCurve[i] = MIN(pow(a*BaseCurve[FilmCurve[i]]/0x10000+b,g)*0x10000, 0xFFFF);
		}
	}

	this->_profile(in_profile, in);
	this->_profile(out_profile, out);

	if ( conf->intent[out_profile]!=d->intent[out_profile] )
	{
		d->intent[out_profile] = conf->intent[out_profile];
		d->updateTransform = TRUE;
	}
	/* For auto-tools we ignore all the output settings:
	* luminosity, saturation, output profile and proofing. */
	if ( mode==auto_developer )
	{
		_CHECK_RET_(developer_create_transform(d, mode));
		return	S_OK;
	}

	_CHECK_RET_(this-> _profile(display_profile, display));

	if ( conf->intent[display_profile]!=d->intent[display_profile] )
	{
		d->intent[display_profile] = conf->intent[display_profile];
		d->updateTransform = TRUE;
	}

	/* Check if curve data has changed. */
	if (memcmp(curve, &d->luminosityCurveData, sizeof(CurveData)))
	{
		d->luminosityCurveData = *curve;
		/* Trivial curve does not require a profile */
		if ( CurveDataIsTrivial(curve) )
		{
			d->luminosityProfile = NULL;
		} else
		{
			cmsCloseProfile(d->luminosityProfile);
			CurveSample *cs = CurveSampleInit(0x100, 0x10000);
			//ufraw_message(UFRAW_RESET, NULL);
			if (CurveDataSample(curve, cs)!=NC_SUCCESS)
			{
				//	ufraw_message(UFRAW_REPORT, NULL);
				d->luminosityProfile = NULL;
			}
			else
			{
				LPGAMMATABLE *TransferFunction =
					(LPGAMMATABLE *)d->TransferFunction;
				for (i=0; i<0x100; i++)
					TransferFunction[0]->GammaTable[i] = cs->m_Samples[i];
				d->luminosityProfile = cmsCreateLinearizationDeviceLink(
					icSigLabData, TransferFunction);
				cmsSetDeviceClass(d->luminosityProfile, icSigAbstractClass);
			}
			CurveSampleFree(cs);
		}
		d->updateTransform = TRUE;
	}

	if ( conf->saturation!=d->saturation
#ifdef UFRAW_CONTRAST
		|| conf->contrast!=d->contrast
#endif
		|| conf->grayscaleMode == grayscale_luminance )
	{
#ifdef UFRAW_CONTRAST
		d->contrast = conf->contrast;
#endif
		d->saturation = (conf->grayscaleMode == grayscale_luminance) ? 0 : conf->saturation;
		cmsCloseProfile(d->saturationProfile);

		if (d->saturation==1.0
#ifdef UFRAW_CONTRAST
			&& d->contrast==1.0
#endif
			)
			d->saturationProfile = NULL;
		else
			d->saturationProfile = create_contrast_saturation_profile(
#ifdef UFRAW_CONTRAST
			d->contrast,
#else
			1.0,
#endif
			d->saturation);
		d->updateTransform = TRUE;
	}
	_CHECK_RET_(developer_create_transform(d, mode));
	return S_OK;
}

extern const double xyz_rgb[3][3];

/* RGB from XYZ */
static const double rgb_xyz[3][3] = {{ 3.24048, -1.53715, -0.498536 },{ -0.969255, 1.87599, 0.0415559 },
{ 0.0556466, -0.204041, 1.05731 } };

static void rgb_to_cielch(gint64 rgb[3], float lch[3])
{
	int c, cc, i;
	float r, xyz[3], lab[3];
	// The use of static varibles here should be thread safe.
	// In the worst case cbrt[] will be calculated more than once.
	static gboolean firstRun = TRUE;
	static float cbrt[0x10000];

	if (firstRun) {
		for (i=0; i < 0x10000; i++) {
			r = i / 65535.0;
			cbrt[i] = r > 0.008856 ? pow((float)r,(float)(1./3.0)) : 7.787*r + 16/116.0;
		}
		firstRun = FALSE;
	}
	xyz[0] = xyz[1] = xyz[2] = 0.5;
	for (c=0; c<3; c++)
		for (cc=0; cc<3; cc++)
			xyz[cc] += xyz_rgb[cc][c] * rgb[c];
	for (c=0; c<3; c++)
		xyz[c] = cbrt[MAX(MIN((int)xyz[c], 0xFFFF), 0)];
	lab[0] = 116 * xyz[1] - 16;
	lab[1] = 500 * (xyz[0] - xyz[1]);
	lab[2] = 200 * (xyz[1] - xyz[2]);

	lch[0] = lab[0];
	lch[1] = sqrt(lab[1]*lab[1]+lab[2]*lab[2]);
	lch[2] = atan2(lab[2], lab[1]);
}

static void cielch_to_rgb(float lch[3], gint64 rgb[3])
{
	int c, cc;
	float xyz[3], fx, fy, fz, xr, yr, zr, kappa, epsilon, tmpf, lab[3];
	epsilon = 0.008856; kappa = 903.3;
	lab[0] = lch[0];
	lab[1] = lch[1] * cos(lch[2]);
	lab[2] = lch[1] * sin(lch[2]);
	yr = (lab[0]<=kappa*epsilon) ?
		(lab[0]/kappa) : (pow((lab[0]+16.0)/116.0, 3.0));
	fy = (yr<=epsilon) ? ((kappa*yr+16.0)/116.0) : ((lab[0]+16.0)/116.0);
	fz = fy - lab[2]/200.0;
	fx = lab[1]/500.0 + fy;
	zr = (pow((float)fz, (float)3.0)<=epsilon) ? ((116.0*fz-16.0)/kappa) : (pow((float)fz, (float)3.0));
	xr = (pow((float)fx, (float)3.0)<=epsilon) ? ((116.0*fx-16.0)/kappa) : (pow((float)fx, (float)3.0));

	xyz[0] = xr*65535.0 - 0.5;
	xyz[1] = yr*65535.0 - 0.5;
	xyz[2] = zr*65535.0 - 0.5;

	for (c=0; c<3; c++) {
		tmpf = 0;
		for (cc=0; cc<3; cc++)
			tmpf += rgb_xyz[c][cc] * xyz[cc];
		rgb[c] = MAX(tmpf, 0);
	}
}

static void MaxMidMin(gint64 p[3], int *maxc, int *midc, int *minc)
{
	if (p[0] > p[1] && p[0] > p[2]) {
		*maxc = 0;
		if (p[1] > p[2]) { *midc = 1; *minc = 2; }
		else { *midc = 2; *minc = 1; }
	} else if (p[1] > p[2]) {
		*maxc = 1;
		if (p[0] > p[2]) { *midc = 0; *minc = 2; }
		else { *midc = 2; *minc = 0; }
	} else {
		*maxc = 2;
		if (p[0] > p[1]) { *midc = 0; *minc = 1; }
		else { *midc = 1; *minc = 0; }
	}
}

HRESULT c_developer_data::develope(void *po, guint16 pix[4], int mode,guint16 *buf, int count)
{
	developer_data * d = &(this->_m_developer_data);
	guint8 *p8 = (guint8 *)po;
	guint16 *p16 = (guint16 *)po, c, tmppix[3];
	int i;
	for (i=0; i<count; i++)
	{
		this->develop_linear(pix+i*4, tmppix);
		for (c=0; c<3; c++)
			buf[i*3+c] = d->gammaCurve[tmppix[c]];
	}

	if (d->colorTransform!=NULL)
		cmsDoTransform(d->colorTransform, buf, buf, count);

	if (mode==16)
	{
		for (i=0; i<3*count; i++)
			p16[i] = buf[i];
	}
	else
	{
		for (i=0; i<3*count; i++)
			p8[i] = buf[i] >> 8;
	}
	return S_OK;
}

static void develop_grayscale(guint16 *pixel, const developer_data *d)
{
	gint32 spot;
	guint16 min;
	guint16 max;

	switch (d->grayscaleMode)
	{
	case grayscale_mixer:
		spot = pixel[0] * d->grayscaleMixer[0]
		+ pixel[1] * d->grayscaleMixer[1]
		+ pixel[2] * d->grayscaleMixer[2];
		if (spot > 65535) spot = 65535;
		else if (spot < 0) spot = 0;
		break;

	case grayscale_lightness:
		min = max = pixel[0];
		if (pixel[1] > max) max = pixel[1];
		if (pixel[2] > max) max = pixel[2];
		if (pixel[1] < min) min = pixel[1];
		if (pixel[2] < min) min = pixel[2];
		spot = ((int)min + (int)max) / 2;
		break;

	case grayscale_value:
		max = pixel[0];
		if (pixel[1] > max) max = pixel[1];
		if (pixel[2] > max) max = pixel[2];
		spot = max;
		break;

	default:
		return;
	}
	pixel[0] = pixel[1] = pixel[2] = spot;
	return;
}

HRESULT c_developer_data::develop_linear(guint16 in[4], guint16 out[3])
{
	developer_data *d = &(this->_m_developer_data);
	unsigned c, cc;
	gint64 tmppix[4], tmp;
	gboolean clipped = FALSE;
	for (c=0; c<d->colors; c++)
	{
		/* Set WB, normalizing tmppix[c]<0x10000 */
		tmppix[c] = (guint64)in[c] * d->rgbWB[c] / 0x10000;
		if ( d->restoreDetails!=clip_details &&	tmppix[c] > d->max )
		{
			clipped = TRUE;
		} else 
		{
			tmppix[c] = MIN(tmppix[c], d->max);
		}

		/* We are counting on the fact that film_highlights
		* and !clip_highlights cannot be set simultaneously. */

		if ( d->clipHighlights==film_highlights )
			tmppix[c] = tmppix[c] * 0x10000 / d->max;
		else
			tmppix[c] = tmppix[c] * d->exposure / d->max;
	}

	if ( clipped )
	{
		/* At this point a value of d->exposure in tmppix[c] corresponds
		* to "1.0" (full exposure). Still the maximal value can be
		* d->exposure * 0x10000 / d->max */
		gint64 unclippedPix[3], clippedPix[3];
		if ( d->useMatrix )
		{
			for (cc=0; cc<3; cc++)
			{
				for (c=0, tmp=0; c<d->colors; c++)
					tmp += tmppix[c] * d->colorMatrix[cc][c];
				unclippedPix[cc] = MAX(tmp/0x10000, 0);
			}
		} else
		{
			for (c=0; c<3; c++) unclippedPix[c] = tmppix[c];
		}

		for (c=0; c<3; c++)
			tmppix[c] = MIN(tmppix[c], d->exposure);

		if ( d->useMatrix )
		{
			for (cc=0; cc<3; cc++)
			{
				for (c=0, tmp=0; c<d->colors; c++)
					tmp += tmppix[c] * d->colorMatrix[cc][c];
				clippedPix[cc] = MAX(tmp/0x10000, 0);
			}
		}
		else
		{
			for (c=0; c<3; c++) clippedPix[c] = tmppix[c];
		}

		if ( d->restoreDetails==restore_lch_details )
		{
			float lch[3], clippedLch[3], unclippedLch[3];
			rgb_to_cielch(unclippedPix, unclippedLch);
			rgb_to_cielch(clippedPix, clippedLch);
			//lch[0] = clippedLch[0] + (unclippedLch[0]-clippedLch[0]) * x;
			lch[0] = unclippedLch[0];
			lch[1] = clippedLch[1];
			lch[2] = clippedLch[2];
			cielch_to_rgb(lch, tmppix);
		} else
		{ /* restore_hsv_details */
			int maxc, midc, minc;
			MaxMidMin(unclippedPix, &maxc, &midc, &minc);
			gint64 unclippedLum = unclippedPix[maxc];
			gint64 clippedLum = clippedPix[maxc];
			/*gint64 unclippedSat;
			if ( unclippedPix[maxc]==0 )
			unclippedSat = 0;
			else
			unclippedSat = 0x10000 -
			unclippedPix[minc] * 0x10000 / unclippedPix[maxc];*/
			gint64 clippedSat;

			if ( clippedPix[maxc]<clippedPix[minc] || clippedPix[maxc]==0 )
				clippedSat = 0;
			else
				clippedSat = 0x10000 -	clippedPix[minc] * 0x10000 / clippedPix[maxc];

			gint64 clippedHue;

			if ( clippedPix[maxc]==clippedPix[minc] )
				clippedHue = 0;
			else
				clippedHue = (clippedPix[midc]-clippedPix[minc])*0x10000 /(clippedPix[maxc]-clippedPix[minc]);

			gint64 unclippedHue;

			if ( unclippedPix[maxc]==unclippedPix[minc] )
				unclippedHue = clippedHue;
			else
				unclippedHue =
				(unclippedPix[midc]-unclippedPix[minc])*0x10000 /(unclippedPix[maxc]-unclippedPix[minc]);

			/* Here we decide how to mix the clipped and unclipped values.
			* The general equation is clipped + (unclipped - clipped) * x,
			* where x is between 0 and 1. */
			/* For lum we set x=1/2. Thus hightlights are not too bright. */
			gint64 lum = clippedLum + (unclippedLum - clippedLum) * 1/2;
			/* For sat we should set x=0 to prevent color artifacts. */
			//gint64 sat = clippedSat + (unclippedSat - clippedSat) * 0/1 ;
			gint64 sat = clippedSat;
			/* For hue we set x=1. This doesn't seem to have much effect. */
			gint64 hue = unclippedHue;

			tmppix[maxc] = lum;
			tmppix[minc] = lum * (0x10000-sat) / 0x10000;
			tmppix[midc] = lum * (0x10000-sat + sat*hue/0x10000) / 0x10000;
		}
	} else
	{ /* !clipped */
		if (d->useMatrix)
		{
			gint64 tmp[3];
			for (cc=0; cc<3; cc++)
			{
				for (c=0, tmp[cc]=0; c<d->colors; c++)
					tmp[cc] += tmppix[c] * d->colorMatrix[cc][c];
			}
			for (c=0; c<3; c++)
				tmppix[c] = MAX(tmp[c]/0x10000, 0);
		}

		gint64 max = tmppix[0];
		for (c=1; c<3; c++)
			max = MAX(tmppix[c], max);

		if (max > 0xFFFF)
		{
			gint64 unclippedLum = max;
			gint64 clippedLum = 0xFFFF;
			gint64 lum = clippedLum + (unclippedLum - clippedLum) * 1/4;
			for (c=0; c<3; c++)
				tmppix[c] = tmppix[c] * lum / max;
		}
	}

	for (c=0; c<3; c++)
		out[c] = MIN(MAX(tmppix[c], 0), 0xFFFF);
	develop_grayscale(out, d);

	return S_OK;
}

