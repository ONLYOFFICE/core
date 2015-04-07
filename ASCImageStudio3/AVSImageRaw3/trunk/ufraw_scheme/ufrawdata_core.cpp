/*!	\file	ufrawdata_core.h
 *	\brief	Работа с ufraw_data
 */

#include "stdafx.h"
#include ".\dcraw\dcrawcommon.h"
#include "dcrawdata_core.h"
#include "ufrawdata_core.h"
#include "atlbase.h"

#define _HR HRESULT hr
#define _CHECK_RET_(HR) 	if (FAILED(hr = (HR))) { return hr;}

#define _CHECK_TRACE_ASSERT_(HR)	do{if (FAILED(hr = (HR)))\
				{\
					AtlTrace(OLESTR("[c_ufraw_data]: failed hr (0x%08lX) at file %s line %d"),hr,__FILE__,__LINE__);\
					_ASSERT(false);\
				}}while(0);

static const double XYZ_to_RGB[3][3] = {{ 3.24071,	-0.969258,  0.0556352 },{-1.53726,	1.87599,    -0.203996 },{-0.498571,	0.0415557,  1.05707 } };

static void ufraw_build_raw_histogram(ufraw_data *uf)
{
	int i, c;
	dcraw_data *raw = (dcraw_data *)uf->raw;
	gboolean updateHistogram = FALSE;

	if (NULL == uf->RawHistogram)
	{
		uf->RawHistogram = g_new(int, uf->rgbMax+1);
		updateHistogram = TRUE;
	}

	double maxChan = 0;
	for (c=0; c<uf->colors; c++)
		maxChan = DCRAW_MAX(uf->conf->chanMul[c], maxChan);

	for (c=0; c<uf->colors; c++)
	{
		int tmp = floor(uf->conf->chanMul[c]/maxChan*0x10000);
		if (uf->RawChanMul[c] != tmp)
		{
			updateHistogram = TRUE;
			uf->RawChanMul[c] = tmp;
		}
	}

	if (!updateHistogram)
		return;

	if (uf->colors==3)
		uf->RawChanMul[3] = uf->RawChanMul[1];

	memset(uf->RawHistogram, 0, (uf->rgbMax+1)*sizeof(int));

	int count = raw->raw.height*raw->raw.width;
	
	for (i=0; i<count; i++)
	{
		for (c=0; c<raw->raw.colors; c++)
		{
			uf->RawHistogram[DCRAW_MIN(
				(gint64)DCRAW_MAX(raw->raw.image[i][c] - raw->black, 0) * uf->RawChanMul[c] / 0x10000
				, uf->rgbMax)]++;
		}
	}

	uf->RawCount = count * raw->raw.colors;
}

void Temperature_to_RGB(double T, double RGB[3])
{
	int c;
	double xD, yD, X, Y, Z, max;
	// Fit for CIE Daylight illuminant
	if (T<= 4000) {
		xD = 0.27475e9/(T*T*T) - 0.98598e6/(T*T) + 1.17444e3/T + 0.145986;
	} else if (T<= 7000) {
		xD = -4.6070e9/(T*T*T) + 2.9678e6/(T*T) + 0.09911e3/T + 0.244063;
	} else {
		xD = -2.0064e9/(T*T*T) + 1.9018e6/(T*T) + 0.24748e3/T + 0.237040;
	}
	yD = -3*xD*xD + 2.87*xD - 0.275;

	// Fit for Blackbody using CIE standard observer function at 2 degrees
	//xD = -1.8596e9/(T*T*T) + 1.37686e6/(T*T) + 0.360496e3/T + 0.232632;
	//yD = -2.6046*xD*xD + 2.6106*xD - 0.239156;

	// Fit for Blackbody using CIE standard observer function at 10 degrees
	//xD = -1.98883e9/(T*T*T) + 1.45155e6/(T*T) + 0.364774e3/T + 0.231136;
	//yD = -2.35563*xD*xD + 2.39688*xD - 0.196035;

	X = xD/yD;
	Y = 1;
	Z = (1-xD-yD)/yD;
	max = 0;
	for (c=0; c<3; c++) {
		RGB[c] = X*XYZ_to_RGB[0][c] + Y*XYZ_to_RGB[1][c] + Z*XYZ_to_RGB[2][c];
		if (RGB[c]>max) max = RGB[c];
	}
	for (c=0; c<3; c++) RGB[c] = RGB[c]/max;
}

void RGB_to_Temperature(double RGB[3], double *T, double *Green)
{
	double Tmax, Tmin, testRGB[3];
	Tmin = 2000;
	Tmax = 15000;
	for (*T=(Tmax+Tmin)/2; Tmax-Tmin>10; *T=(Tmax+Tmin)/2) {
		Temperature_to_RGB(*T, testRGB);
		if (testRGB[2]/testRGB[0] > RGB[2]/RGB[0])
			Tmax = *T;
		else
			Tmin = *T;
	}
	*Green = (testRGB[1]/testRGB[0]) / (RGB[1]/RGB[0]);
}

HRESULT ufraw_img_get_subarea_coord (ufraw_image_data *img, unsigned saidx, int *x, int *y, int *w, int *h)
{
	if (!img || !x || !y || !w || !h)
		return E_POINTER;

	int saw = (img->width + 3) / 4;
	int sah = (img->height + 7) / 8;
	int sax = saidx % 4;
	int say = saidx / 4;
	*x = saw * sax;
	*y = sah * say;
	*w = (sax < 3) ? saw : (img->width - saw * 3);
	*h = (say < 7) ? sah : (img->height - sah * 7);
	return S_OK;
}

HRESULT ufraw_get_image_dimensions(c_dcraw_data *raw, c_ufraw_data *uf)
{

	if (!raw || !uf )
		return E_POINTER;

	if (!(uf->GetUFRawData()->conf))
		return E_POINTER;

	raw->_image_dimensions(uf->GetUFRawData()->conf->orientation,&uf->GetUFRawData()->initialHeight, &uf->GetUFRawData()->initialWidth);
	uf->_update_rotated_dimensions();

	if (uf->GetUFRawData()->conf->CropX1 < 0)
		uf->GetUFRawData()->conf->CropX1 = 0;

	if (uf->GetUFRawData()->conf->CropY1 < 0)
		uf->GetUFRawData()->conf->CropY1 = 0;
	
	if (uf->GetUFRawData()->conf->CropX2 < 0)
		uf->GetUFRawData()->conf->CropX2 = uf->GetUFRawData()->rotatedWidth;
	
	if (uf->GetUFRawData()->conf->CropY2 < 0)
		uf->GetUFRawData()->conf->CropY2 = uf->GetUFRawData()->rotatedHeight;

	return S_OK;
}


unsigned ufraw_img_get_subarea_idx (ufraw_image_data *img, int x, int y)
{
	int saw = (img->width + 3) / 4;
	int sah = (img->height + 7) / 8;
	return (x / saw) + (y / sah) * 4;
}

/*	Implementation of c_ufraw_data methods */

HRESULT	c_ufraw_data::_open(BSTR filename)
{

	int status;
	_HR;

	ufraw_data	*uf = &_m_ufraw_data;
	c_dcraw_data	*raw;
	conf_data	*conf = NULL;

	//raw = g_new(dcraw_data, 1);
	
	raw = new c_dcraw_data;
	if (!raw)
		return E_OUTOFMEMORY;

	if (FAILED(hr = raw->_open(filename)))
	{
		delete raw;
		return hr;
	}
	
	uf->unzippedBuf = NULL;
	uf->unzippedBufLen = NULL;
	uf->conf = NULL;

	wcscpy_s(uf->filename,max_path,_bstr_t(filename));
	uf->image.image = NULL;

	int i;
	for (i=ufraw_first_phase; i<ufraw_phases_num; i++)
		uf->Images[i].buffer = NULL;

	uf->thumb.buffer = NULL;
	uf->raw = raw;
	uf->colors = raw->GetDCRawData()->colors;
	uf->raw_color = raw->GetDCRawData()->raw_color;
	uf->developer = NULL;
	uf->AutoDeveloper = NULL;
	uf->widget = NULL;
	uf->RawHistogram = NULL;
	uf->HaveFilters = (raw->GetDCRawData()->filters!=0);
	
	//ufraw_message(UFRAW_SET_LOG, "ufraw_open: w:%d h:%d curvesize:%d\n",raw->width, raw->height, raw->toneCurveSize);

	return S_OK;
};

HRESULT	c_ufraw_data::_update_rotated_dimensions()
{
	_HR;
	ufraw_data *uf = &_m_ufraw_data;
	double rotationRadians = (uf->conf->rotationAngle * 2 * M_PI) / 360;
	uf->rotatedWidth = ceil((uf->initialHeight * sin(rotationRadians))
		+ (uf->initialWidth * cos(rotationRadians)));
	uf->rotatedHeight = ceil((uf->initialWidth * sin(rotationRadians))
		+ (uf->initialHeight * cos(rotationRadians)));
	return S_OK;
}



HRESULT	c_ufraw_data::_config(conf_data *rc, conf_data *conf,conf_data *cmd)
{
	_HR;
	ufraw_data * uf = &_m_ufraw_data;
	int status;

	if (!rc)
		return E_POINTER;

	if (strcmp(rc->wb, spot_wb))
		rc->chanMul[0] = -1.0;

	if (rc->autoExposure == enabled_state)
		rc->autoExposure = apply_state;

	if (rc->autoBlack == enabled_state)
		rc->autoBlack = apply_state;

	uf->LoadingID = FALSE;
	uf->conf = g_new(conf_data, 1);
	*uf->conf = *rc;
	rc = uf->conf;

	if (conf!=NULL)
	{
		if (conf->version!=0)
		{
			conf_copy_image(rc, conf);
			conf_copy_save(rc, conf);
			if (strcmp(rc->wb, spot_wb))
				rc->chanMul[0] = -1.0;

			if (rc->autoExposure==enabled_state)
				rc->autoExposure = apply_state;
			
			if (rc->autoBlack==enabled_state)
				rc->autoBlack = apply_state;
		}
	}

	if (cmd!=NULL)
	{
		_CHECK_RET_(conf_set_cmd(rc, cmd));
	}

	dcraw_data *raw = uf->raw->GetDCRawData();

	/*
	char *absname = uf_file_set_absolute(uf->filename);
	g_strlcpy(uf->conf->inputFilename, absname, max_path);
	g_free(absname);
	if (!uf->LoadingID) {
		g_snprintf(uf->conf->inputURI, max_path, "file://%s",uf->conf->inputFilename);
		struct stat s;
		fstat(fileno(raw->ifp), &s);
		g_snprintf(uf->conf->inputModTime, max_name, "%d", (int)s.st_mtime);
	}
	if (strlen(uf->conf->outputFilename)==0) {
		// If output filename wasn't specified use input filename 
		char *filename = uf_file_set_type(uf->filename,
			file_type[uf->conf->type]);
		if (strlen(uf->conf->outputPath)>0) {
			char *cp = g_path_get_basename(filename);
			g_free(filename);
			filename = g_build_filename(uf->conf->outputPath, cp , NULL);
			g_free(cp);
		}
		g_strlcpy(uf->conf->outputFilename, filename, max_path);
		g_free(filename);
	}*/
	/*Reset EXIF data text fields to avoid spill over between images.*/
	strcpy(uf->conf->isoText, "");
	strcpy(uf->conf->shutterText, "");
	strcpy(uf->conf->apertureText, "");
	strcpy(uf->conf->focalLenText, "");
	strcpy(uf->conf->focalLen35Text, "");
	strcpy(uf->conf->lensText, "");
	strcpy(uf->conf->flashText, "");

	if (!raw)
		return E_POINTER;
	/* If we switched cameras, ignore channel multipliers and
	* change spot_wb to manual_wb */
	if ( !uf->LoadingID &&
		( strcmp(uf->conf->make, raw->make)!=0 ||
		strcmp(uf->conf->model, raw->model)!=0 ) ) {
			uf->conf->chanMul[0] = -1.0;
			if (strcmp(uf->conf->wb, spot_wb)==0)
				g_strlcpy(uf->conf->wb, manual_wb, max_name);
	}

	/* Set the EXIF data */
#ifdef __MINGW32__
	/* MinG32 does not have ctime_r(). */
	g_strlcpy(uf->conf->timestampText, ctime(&raw->timestamp), max_name);
#elif defined(__sun) /* Solaris */
	/*
	* Some (all?) versions of Solaris followed a draft POSIX.1c standard
	* where ctime_r took a third length argument.
	*/
	ctime_r(&raw->timestamp, uf->conf->timestampText,
		sizeof(uf->conf->timestampText));
#else
	/* POSIX.1c version of ctime_r() */
	ctime_r(&raw->timestamp,sizeof(uf->conf->timestampText),uf->conf->timestampText);
#endif
	if (uf->conf->timestampText[strlen(uf->conf->timestampText)-1]=='\n')
		uf->conf->timestampText[strlen(uf->conf->timestampText)-1] = '\0';
	g_strlcpy(uf->conf->make, raw->make, max_name);
	g_strlcpy(uf->conf->model, raw->model, max_name);

	uf->conf->timestamp = raw->timestamp;

	if ( !uf->conf->rotate )
	{
		uf->conf->orientation = 1;
		uf->conf->rotationAngle = 0;
	} else
	{
		if ( !uf->LoadingID || uf->conf->orientation<0 )
			uf->conf->orientation = raw->flip;

		// Normalise rotations to a flip, then rotation of 0 < a < 90 degrees.
		uf->conf->rotationAngle = fmod(uf->conf->rotationAngle, 360.0);
		int angle, flip = 0;
		angle = floor(uf->conf->rotationAngle/90)*90;
		switch (angle)
		{
		case  90:
			flip = 6;
			break;
		case 180:
			flip = 3;
			break;
		case 270:
			flip = 5; 
			break;
		}
		
		_CHECK_TRACE_ASSERT_(_flip_orientation(flip));
		uf->conf->rotationAngle -= angle;
	}

	if (uf->inputExifBuf==NULL)
	{
		g_strlcpy(uf->conf->exifSource, "DCRaw", max_name);
		uf->conf->iso_speed = raw->iso_speed;
		g_snprintf(uf->conf->isoText, max_name, "%d", (int)(uf->conf->iso_speed));
		uf->conf->shutter = raw->shutter;
		
		// todo: что в случае выдержки больше секунды?
		if (uf->conf->shutter>0 && uf->conf->shutter<1)
			g_snprintf(uf->conf->shutterText, max_name, "1/%0.1f s",1/uf->conf->shutter);
		else
			g_snprintf(uf->conf->shutterText, max_name, "%0.1f s",uf->conf->shutter);

		uf->conf->aperture = raw->aperture;
		g_snprintf(uf->conf->apertureText, max_name, "F/%0.1f",	uf->conf->aperture);
		uf->conf->focal_len = raw->focal_len;
		g_snprintf(uf->conf->focalLenText, max_name, "%0.1f mm",uf->conf->focal_len);
	}

	/* If there is an embeded curve we "turn on" the custom/camera curve
	* mechanism */
	

	do{
		if (raw->toneCurveSize != 0)
		{
			CurveData nc;
			long pos = ftell(raw->ifp);

			if (pos == -1L)
			{
				int error_;
				_get_errno(&error_);
				// todo
				break;
			}

			if (RipNikonNEFCurve(raw->ifp, raw->toneCurveOffset, &nc, NULL)	!= NC_SUCCESS)
			{
				// todo
				break;
				//return E_IMAGERAW(IMAGERAW_READING_NEFCURVE_ERROR);
			}
			if (fseek(raw->ifp, pos, SEEK_SET))
			{
				// todo			
				break;
			}

			if (nc.m_numAnchors<2)
				nc = conf_default.BaseCurve[0];

			g_strlcpy(nc.name, uf->conf->BaseCurve[custom_curve].name, max_name);
			uf->conf->BaseCurve[custom_curve] = nc;

			int use_custom_curve = 0;
			if (raw->toneModeSize)
			{
				// "AUTO    " "HIGH    " "CS      " "MID.L   " "MID.H   "NORMAL  " "LOW     "
				long pos = ftell(raw->ifp);
				if (pos == -1L)
				{
					int error_;
					_get_errno(&error_);
					// todo
					break;
				}
				char buf[9];
				if (fseek(raw->ifp, raw->toneModeOffset, SEEK_SET))
				{
					//todo
					break;
				}
				// read it in.
				fread(&buf, 9, 1, raw->ifp);
				if (fseek(raw->ifp, pos, SEEK_SET))
				{
					//todo
					break;
				}


				if (!strncmp(buf, "CS      ", sizeof(buf)))
					use_custom_curve=1;

				// down the line, we need to translate the other values into
				// tone curves!
			}

			if (use_custom_curve)
			{
				uf->conf->BaseCurve[camera_curve] =	uf->conf->BaseCurve[custom_curve];
				g_strlcpy(uf->conf->BaseCurve[camera_curve].name,conf_default.BaseCurve[camera_curve].name, max_name);
			}
			else
			{
				uf->conf->BaseCurve[camera_curve] =	conf_default.BaseCurve[camera_curve];
			}
		}
	}while(0);

	if (raw->toneCurveSize == 0)
	{
		/* If there is no embeded curve we "turn off" the custom/camera curve
		* mechanism */
		uf->conf->BaseCurve[camera_curve].m_numAnchors = 0;
		uf->conf->BaseCurve[custom_curve].m_numAnchors = 0;
		if (uf->conf->BaseCurveIndex==custom_curve ||
			uf->conf->BaseCurveIndex==camera_curve)
				uf->conf->BaseCurveIndex = linear_curve;
	}

	// todo
	//ufraw_load_darkframe(uf);

	_CHECK_RET_(ufraw_get_image_dimensions(GetUFRawData()->raw, this));

	return S_OK;
};

HRESULT	c_ufraw_data::_load_raw()
{
	_HR;
	int status;
	ufraw_data * uf = &_m_ufraw_data;
	
	c_dcraw_data *raw = (uf->raw);

	/*
	// todo
	if (uf->conf->embeddedImage)
	{
		dcraw_image_data thumb;
		_CHECK_RET_(raw->_load_thumb(&thumb));
		uf->thumb.height = thumb.height;
		uf->thumb.width = thumb.width;
		this->_read_embedded();
	}
	*/

	_CHECK_RET_(raw->_load_raw());

	// Canon EOS cameras require special exposure normalization
	if ( strcmp(uf->conf->make, "Canon")==0 && strncmp(uf->conf->model, "EOS", 3)==0 )
	{
			int c, max = raw->GetDCRawData()->cam_mul[0];
			for (c=1; c<raw->GetDCRawData()->colors; c++)
				max = DCRAW_MAX(raw->GetDCRawData()->cam_mul[c], max);
			// Camera multipliers in DNG file are normalized to 1.
			// Therefore, they can not be used to normalize exposure.
			// Also, for some Canon DSLR cameras dcraw cannot read the
			// camera multipliers (1D for example). 
			if ( max < 100 )
			{
				// todo
				uf->conf->ExposureNorm = 0;
//				ufraw_message(UFRAW_SET_LOG, "Failed to normalizing exposure\n");
				
			} else
			{
				// Convert exposure value from old ID files from before	ExposureNorm
				if (uf->LoadingID && uf->conf->ExposureNorm==0)
					uf->conf->exposure -= log((double)1.0*raw->GetDCRawData()->rgbMax/max)/log((double)2);
				
				uf->conf->ExposureNorm = max * raw->GetDCRawData()->rgbMax / 4095;
				
				//ufraw_message(UFRAW_SET_LOG,
				//	"Exposure Normalization set to %d (%.2f EV)\n",
				//	uf->conf->ExposureNorm,
				//	log((float)1.0*raw->rgbMax/uf->conf->ExposureNorm)/log((float)2));
			}
	} else // Canon EOS
	{
		uf->conf->ExposureNorm = 0;
	}

	uf->rgbMax = raw->GetDCRawData()->rgbMax - raw->GetDCRawData()->black;
	memcpy(uf->rgb_cam, raw->GetDCRawData()->rgb_cam, sizeof uf->rgb_cam);

	// Foveon image dimensions are knows only after load_raw()
	
	ufraw_get_image_dimensions(GetUFRawData()->raw, this);

	if (uf->conf->CropX2 > uf->rotatedWidth)
		uf->conf->CropX2 = uf->rotatedWidth;

	if (uf->conf->CropY2 > uf->rotatedHeight)
		uf->conf->CropY2 = uf->rotatedHeight;

	// chanMul[0]<0 signals that we need to recalculate the WB
	if (uf->conf->chanMul[0]<0)
	{
		_CHECK_TRACE_ASSERT_(this->_set_wb());
	}
	else
	{
		// Otherwise we just normalize the channels and recalculate the temperature/green
		int WBTuning = uf->conf->WBTuning;
		char wb[max_name];
		g_strlcpy(wb, uf->conf->wb, max_name);
		g_strlcpy(uf->conf->wb, spot_wb, max_name);
		_CHECK_TRACE_ASSERT_(this->_set_wb());
		g_strlcpy(uf->conf->wb, wb, max_name);
		uf->conf->WBTuning = WBTuning;
	}
	_CHECK_TRACE_ASSERT_(this->_auto_expose());
	_CHECK_TRACE_ASSERT_(this->_auto_black());
	return S_OK;
};


HRESULT c_ufraw_data::_load_darkframe()
{
	return E_NOTIMPL;
};

HRESULT	c_ufraw_data::_developer_prepare(DeveloperMode mode)
{
	_HR;
	ufraw_data * uf = &_m_ufraw_data;

	int useMatrix = uf->conf->profileIndex[0] == 1 || uf->colors==4;

	if ( mode == auto_developer )
	{
		if ( uf->AutoDeveloper == NULL )
		{
			uf->AutoDeveloper = new c_developer_data();
			if (!(uf->AutoDeveloper))
				return E_OUTOFMEMORY;
			_CHECK_TRACE_ASSERT_(uf->AutoDeveloper->_init());
		}
		_CHECK_TRACE_ASSERT_(uf->AutoDeveloper->_prepare( uf->conf,	uf->rgbMax, uf->rgb_cam, uf->colors, useMatrix, mode));
	} else
	{
		if ( uf->developer==NULL )
		{
			uf->developer = new c_developer_data();
			if (!(uf->developer))
				return E_OUTOFMEMORY;
			_CHECK_TRACE_ASSERT_(uf->developer->_init());
		}
		_CHECK_TRACE_ASSERT_(uf->developer->_prepare(uf->conf,uf->rgbMax, uf->rgb_cam, uf->colors, useMatrix, mode));
	}
	return S_OK;
};

HRESULT c_ufraw_data::_convert_image()
{
	_HR;
	ufraw_data * uf = &_m_ufraw_data;
	
	_CHECK_RET_(this->_developer_prepare(file_developer));
	//_CHECK_RET_(this->_developer_prepare(auto_developer));
	_CHECK_RET_(this->_convert_image_init());
	_CHECK_RET_(this->_convert_image_first_phase(TRUE));
	
	if (uf->ConvertShrink>1)
	{
		dcraw_image_data final;
		final.height = uf->image.height;
		final.width = uf->image.width;
		final.image = uf->image.image;

		// Scale threshold according to shrink factor, as the effect of
		// neighbouring pixels decays about exponentially with distance.
		float threshold = uf->conf->threshold * exp((float)(-(uf->ConvertShrink/2.0-1)));
		_CHECK_TRACE_ASSERT_(uf->raw->_wavelet_denoise_shrinked(&final,threshold));
	}
	return S_OK;
};

HRESULT	c_ufraw_data::_convert_image_init()
{
	ufraw_data * uf = &_m_ufraw_data;
	dcraw_data *raw = _m_ufraw_data.raw->GetDCRawData();

	int temp_height, temp_width;
	// This code is copied from dcraw_image_dimensions().
	if (raw->pixel_aspect < 1)
		temp_height = (int)(raw->height / raw->pixel_aspect + 0.5);
	else
		temp_height = raw->height;

	if (raw->pixel_aspect > 1)
		temp_width = (int)(raw->width * raw->pixel_aspect + 0.5);
	else
		temp_width = raw->width;

	// We can do a simple interpolation in the following cases:
	// We shrink by an integer value.
	// If pixel_aspect<1 (e.g. NIKON D1X) shrink must be at least 4.
	// Wanted size is smaller than raw size (size is after a raw->shrink).
	// There are no filters (Foveon).
	uf->ConvertShrink = 1;
	if ( uf->conf->interpolation == half_interpolation ||
		( uf->conf->size == 0 && uf->conf->shrink>1 ) ||
		( uf->conf->size>0 && uf->conf->size<=DCRAW_MAX(temp_height, temp_width) ) ||
		!uf->HaveFilters )
	{
			if (uf->conf->size==0 && uf->conf->shrink>1 &&(int)(uf->conf->shrink*raw->pixel_aspect)%2==0)
				uf->ConvertShrink = uf->conf->shrink * raw->pixel_aspect;
			else if (uf->conf->interpolation==half_interpolation)
				uf->ConvertShrink = 2;
			else if ( uf->HaveFilters)
				uf->ConvertShrink = 2;
	}
	return S_OK;
};


HRESULT	c_ufraw_data::_convert_image_first_phase(gboolean lensfix)
{
	_HR;
	int status, c;
	ufraw_data * uf = &_m_ufraw_data;
	dcraw_data *raw = uf->raw->GetDCRawData();
	
	// final->image memory will be realloc'd as needed
	dcraw_image_data final;
	final.image = uf->image.image;
	dcraw_data *dark = (uf->conf->darkframe ? ((uf->conf->darkframe->GetUFRawData()->raw->GetDCRawData())) : (dcraw_data *)NULL);

	if ( uf->ConvertShrink>1 || !uf->HaveFilters )
	{
		uf->raw->_finalize_shrink(&final, dark, uf->ConvertShrink);
	}
	else 
	{
		_CHECK_RET_(uf->raw->_wavelet_denoise(uf->conf->threshold));

		uf->raw->_finalize_interpolate(&final, dark, uf->conf->interpolation,uf->conf->smoothing, uf->developer->GetDeveloperData()->rgbWB);

		uf->developer->GetDeveloperData()->rgbMax = uf->developer->GetDeveloperData()->max;
		for (c=0; c<4; c++)
			uf->developer->GetDeveloperData()->rgbWB[c] = 0x10000;
	}

	dcraw_image_stretch(&final, raw->pixel_aspect);
	if (uf->conf->size==0 && uf->conf->shrink>1)
	{
		dcraw_image_resize(&final,	uf->ConvertShrink*DCRAW_MAX(final.height, final.width)/uf->conf->shrink);
		uf->ConvertShrink = uf->conf->shrink;
	}

	if (uf->conf->size>0)
	{
		int cropHeight = uf->conf->CropY2 - uf->conf->CropY1;
		int cropWidth = uf->conf->CropX2 - uf->conf->CropX1;
		int cropSize = DCRAW_MAX(cropHeight, cropWidth);
		if ( uf->conf->size > cropSize )
		{
			// todo
		//	ufraw_message(UFRAW_ERROR, _("Can not downsize from %d to %d."),cropSize, uf->conf->size);
		} 
		else
		{
			/* uf->conf->size holds the size of the cropped image.
			* We need to calculate from it the desired size of
			* th uncropped image. */
			int finalSize = uf->ConvertShrink * DCRAW_MAX(final.height, final.width);
			uf->ConvertShrink = cropSize / uf->conf->size;
			dcraw_image_resize(&final, uf->conf->size * finalSize / cropSize);
		}
	}

	dcraw_flip_image(&final, uf->conf->orientation);

#ifdef HAVE_LENSFUN
	if (lensfix && uf->conf->camera && uf->conf->lens)
	{
		lfModifier *modifier = lensfix ? lf_modifier_new (
			uf->conf->lens, uf->conf->camera->CropFactor, final.width, final.height) : NULL;

		if (modifier)
		{
			float real_scale = pow (2.0, uf->conf->lens_scale);
			const int modflags = LF_MODIFY_TCA | LF_MODIFY_VIGNETTING |
				LF_MODIFY_DISTORTION | LF_MODIFY_GEOMETRY | LF_MODIFY_SCALE;

			int finmodflags = lf_modifier_initialize (
				modifier, uf->conf->lens, LF_PF_U16,
				uf->conf->focal_len, uf->conf->aperture,
				uf->conf->subject_distance, real_scale,
				uf->conf->cur_lens_type, modflags, FALSE);
			if (finmodflags & modflags)
				ufraw_lensfun_modify (&final, modifier, finmodflags);

			lf_modifier_destroy (modifier);
		}
	}
#else
	(void)lensfix;
#endif // HAVE_LENSFUN

	uf->image.image = final.image;
	uf->image.height = final.height;
	uf->image.width = final.width;

	ufraw_image_data *FirstImage = &uf->Images[ufraw_first_phase];
	FirstImage->height = uf->image.height;
	FirstImage->width = uf->image.width;
	FirstImage->depth = sizeof(dcraw_image_type);
	FirstImage->rowstride = FirstImage->width * FirstImage->depth;
	FirstImage->buffer = (guint8 *)uf->image.image;
	FirstImage->valid = 0xffffffff;

	return S_OK;
};

HRESULT c_ufraw_data::_convert_image_init_phase()
{
	ufraw_data * uf = &_m_ufraw_data;
	ufraw_image_data *FirstImage = &uf->Images[ufraw_first_phase];
	ufraw_image_data *img;

	/* Denoise image layer */
	img = &uf->Images[ufraw_denoise_phase];
	if (uf->conf->threshold > 0)
	{
		/* Mark layer invalid if we're resizing */
		if (img->height != FirstImage->height || img->width != FirstImage->width ||	!img->buffer)
			img->valid = 0;

		img->height = FirstImage->height;
		img->width = FirstImage->width;
		img->depth = sizeof(dcraw_image_type);
		img->rowstride = img->width * img->depth;
		img->buffer = (guint8*) g_realloc(img->buffer, img->height * img->rowstride);
	}
	else
		img->valid = 0;

	/* Development image layer */
	img = &uf->Images[ufraw_develop_phase];
	if (img->height != FirstImage->height || img->width != FirstImage->width ||	!img->buffer)
		img->valid = 0;

	img->height = FirstImage->height;
	img->width = FirstImage->width;
	img->depth = 3;
	img->rowstride = img->width * img->depth;
	img->buffer = (guint8*) g_realloc(img->buffer, img->height * img->rowstride);

#ifdef HAVE_LENSFUN
	/* Postprocessing image layer */
	img = &uf->Images[ufraw_lensfun_phase];
	if (img->height != FirstImage->height ||
		img->width != FirstImage->width ||
		!img->buffer || !uf->modifier)
		img->valid = 0;

	img->height = FirstImage->height;
	img->width = FirstImage->width;
	img->depth = 3;
	img->rowstride = img->width * img->depth;

	if (uf->modifier)
		lf_modifier_destroy (uf->modifier);

	uf->modifier = lf_modifier_new (
		uf->conf->lens, uf->conf->camera->CropFactor, img->width, img->height);

	float real_scale = pow (2.0, uf->conf->lens_scale);
	uf->postproc_ops = lf_modifier_initialize (
		uf->modifier, uf->conf->lens, LF_PF_U8,
		uf->conf->focal_len, uf->conf->aperture,
		uf->conf->subject_distance, real_scale,
		uf->conf->cur_lens_type, LF_MODIFY_TCA | LF_MODIFY_VIGNETTING |
		LF_MODIFY_DISTORTION | LF_MODIFY_GEOMETRY | LF_MODIFY_SCALE, FALSE);

	if (uf->postproc_ops & (LF_MODIFY_TCA | LF_MODIFY_VIGNETTING |
		LF_MODIFY_DISTORTION | LF_MODIFY_GEOMETRY |
		LF_MODIFY_SCALE))
		img->buffer = g_realloc(img->buffer, img->height * img->rowstride);
	else
	{
		lf_modifier_destroy (uf->modifier);
		uf->modifier = NULL;
		img->valid = 0;
	}
#endif

	return S_OK;
};


ufraw_image_data * c_ufraw_data::_convert_image_develop()
{
	_HR;
	ufraw_data * uf = &_m_ufraw_data;
	ufraw_image_data *in = &(uf->Images [ufraw_first_phase]);
	
	int x = 0, y = 0, w = in->width , h = in->height;
	
	ufraw_image_data *out = &uf->Images [ufraw_develop_phase];

	guint16 *pixtmp = g_new (guint16, w * 3);

int yy;
	for (yy = y; yy < y + h; yy++)
	{
		guint8 *dest = out->buffer + (yy * out->width + x) * out->depth;
		uf->developer->develope(dest, (guint16*)( (void *)(in->buffer + (yy * in->width + x) * in->depth)),8, pixtmp, w);
	}

	g_free (pixtmp);
	return out;

}

ufraw_image_data * c_ufraw_data::_convert_image_area (int saidx, UFRawPhase phase)
{
	_HR;
	ufraw_data * uf = &_m_ufraw_data;
	int x = 0, y = 0, w = 0, h = 0;
	ufraw_image_data *out = &uf->Images [phase];

	if (saidx >= 0)
	{
		if (out->valid & (1 << saidx))
			return out; // the subarea has been already computed

		// Get subarea coordinates
		ufraw_img_get_subarea_coord (out, saidx, &x, &y, &w, &h);
	}

	// Get the subarea image for previous phase
	ufraw_image_data *in = (phase > ufraw_first_phase) ? this->_convert_image_area (saidx, (UFRawPhase)(phase - 1)) : &uf->Images [ufraw_first_phase];

	switch (phase)
	{
	case ufraw_denoise_phase:
		{
			if (uf->conf->threshold == 0)
				// No denoise phase, return the image from previous phase
				return in;

			if (saidx < 0)
				return out;

			dcraw_image_data tmp;
			/* With shrink==2 the border should be 16 pixels */
			int border = 16 * 2 / uf->ConvertShrink;
			int bx = DCRAW_MAX (x - border, 0);
			int by = DCRAW_MAX (y - border, 0);
			tmp.width = DCRAW_MIN ((x - bx) + w + border, in->width - bx);
			if (tmp.width < 16)
			{
				bx = bx + in->width - 16; // We assume in->width>16
				tmp.width = 16;
			}
			tmp.height = DCRAW_MIN ((y - by) + h + border, in->height - by);
			if (tmp.height < 16)
			{
				by = by + in->height - 16; // We assume in->height>16
				tmp.height = 16;
			}
			tmp.image = g_new (dcraw_image_type, tmp.height * tmp.width);
			int yy;
			for (yy = 0; yy < tmp.height; yy++)
				memcpy (tmp.image [yy * tmp.width],
						in->buffer + ((by + yy) * in->width + bx) * in->depth,
						tmp.width * in->depth);
			// Scale threshold according to shrink factor, as the effect of
			// neighbouring pixels decays about exponentially with distance.
			float threshold = uf->conf->threshold * exp((float)(1.0 - uf->ConvertShrink / 2.0));
			_CHECK_TRACE_ASSERT_(uf->raw->_wavelet_denoise_shrinked(&tmp,threshold));
			//dcraw_wavelet_denoise_shrinked (&tmp,(dcraw_data *) uf->raw, threshold);
			for (yy = 0; yy < h; yy++)
				memcpy (out->buffer + ((y + yy) * out->width + x) * out->depth,
				tmp.image [(yy + y - by) * tmp.width + x - bx], w * out->depth);
			g_free (tmp.image);
		}
		break;

	case ufraw_develop_phase:
		{
			if (saidx < 0)
				return out;

			guint16 *pixtmp = g_new (guint16, w * 3);
			int yy;
			for (yy = y; yy < y + h; yy++)
			{
				guint8 *dest = out->buffer + (yy * out->width + x) * out->depth;
				
				uf->developer->develope(dest, (guint16*)( (void *)(in->buffer + (yy * in->width + x) * in->depth)),8, pixtmp, w);

#ifdef HAVE_LENSFUN
				if (uf->modifier &&	(uf->postproc_ops & LF_MODIFY_VIGNETTING))
				{
					/* Apply de-vignetting filter here, to avoid
					* creating a separate pass for it which will
					* take a lot of memory ... */
					lf_modifier_apply_color_modification (
						uf->modifier, dest, x, yy, w, 1,
						LF_CR_3 (RED, GREEN, BLUE),
						out->width * out->depth);
				}
#endif
			}
			g_free (pixtmp);
		}
		break;

	case ufraw_lensfun_phase:
#ifdef HAVE_LENSFUN
		{
			if (!uf->modifier ||
				!(uf->postproc_ops & (LF_MODIFY_TCA | LF_MODIFY_DISTORTION |
				LF_MODIFY_GEOMETRY | LF_MODIFY_SCALE)))
				return in;

			if (saidx < 0)
				return out;

			int yy;
			float *buff = g_new (float, (w < 8) ? 8 * 2 * 3 : w * 2 * 3);

			// Compute the previous stage subareas, if needed
			lf_modifier_apply_subpixel_geometry_distortion (
				uf->modifier, x, y, 1, 1, buff);
			lf_modifier_apply_subpixel_geometry_distortion (
				uf->modifier, x + w/2, y, 1, 1, buff + 2 * 3);
			lf_modifier_apply_subpixel_geometry_distortion (
				uf->modifier, x + w-1, y, 1, 1, buff + 4 * 3);
			lf_modifier_apply_subpixel_geometry_distortion (
				uf->modifier, x, y + h/2, 1, 1, buff + 6 * 3);
			lf_modifier_apply_subpixel_geometry_distortion (
				uf->modifier, x + w-1, y + h/2, 1, 1, buff + 8 * 3);
			lf_modifier_apply_subpixel_geometry_distortion (
				uf->modifier, x, y + h-1, 1, 1, buff + 10 * 3);
			lf_modifier_apply_subpixel_geometry_distortion (
				uf->modifier, x + w/2, y + h-1, 1, 1, buff + 12 * 3);
			lf_modifier_apply_subpixel_geometry_distortion (
				uf->modifier, x + w-1, y + h-1, 1, 1, buff + 14 * 3);

			for (yy = 0; yy < 8 * 2 * 3; yy += 2)
			{
				int idx = ufraw_img_get_subarea_idx (in, buff [yy], buff [yy + 1]);
				if (idx >= 0 && idx <= 31)
					ufraw_convert_image_area (uf, idx, phase - 1);
			}

			for (yy = 0; yy < h; yy++)
			{
				if (!lf_modifier_apply_subpixel_geometry_distortion (
					uf->modifier, x, y + yy, w, 1, buff))
					break; // huh?? should never happen

				guint8 *out_buf = out->buffer + (y + yy) * out->rowstride + x * out->depth;

				float *cur, *end = buff + w * 2 * 3;
				for (cur = buff; cur < end; cur += 2 * 3, out_buf += out->depth)
				{
					// roundf() and truncf() are C99 and gcc warns on them
					// if used without -std=c99... oh well...
					int xi = cur [0] + 0.5;
					int yi = cur [1] + 0.5;
					if (xi<0 || xi >= in->width || yi<0 || yi >= in->height)
						out_buf [0] = 0;
					else
						/* Nearest interpolation: don't spend time
						for high-quality interpolation for preview */
						out_buf [0] = (in->buffer + yi * in->rowstride +
						xi * in->depth) [0];

					xi = cur [2] + 0.5;
					yi = cur [3] + 0.5;
					if (xi<0 || xi >= in->width || yi<0 || yi >= in->height)
						out_buf [1] = 0;
					else
						/* Nearest interpolation: don't spend time
						for high-quality interpolation for preview */
						out_buf [1] = (in->buffer + yi * in->rowstride +
						xi * in->depth) [1];

					xi = cur [4] + 0.5;
					yi = cur [5] + 0.5;
					if (xi<0 || xi >= in->width || yi<0 || yi >= in->height)
						out_buf [2] = 0;
					else
						/* Nearest interpolation: don't spend time
						for high-quality interpolation for preview */
						out_buf [2] = (in->buffer + yi * in->rowstride +
						xi * in->depth) [2];
				}
			}

			g_free (buff);
		}
		break;
#else
		// fallback
#endif

	default:
		return in;
	}

	// Mark the subarea as valid
	out->valid |= (1 << saidx);

	return out;
}

#define _SFREE(obj) do {if (obj) {free(obj); obj = 0;}}while(0);

HRESULT c_ufraw_data::_close()
{
	_HR;
	ufraw_data * uf = &_m_ufraw_data;

	if (_m_ufraw_data.raw)
	{
		_CHECK_TRACE_ASSERT_(_m_ufraw_data.raw->_close());
	}
	
	_SFREE(uf->unzippedBuf);
	
	if (_m_ufraw_data.raw)
		delete _m_ufraw_data.raw;
	
	/* We cannot free uf->conf since it might be accessed after the close */
	//    g_free(uf->conf);

	_SFREE(uf->inputExifBuf);
	_SFREE(uf->outputExifBuf);
	_SFREE(uf->image.image);
	int i;
	for (i=ufraw_first_phase+1; i<ufraw_phases_num; i++)
		_SFREE(uf->Images[i].buffer);

	_SFREE(uf->thumb.buffer);

	if (uf->developer)
	{
		_CHECK_TRACE_ASSERT_(uf->developer->_destroy());
		delete uf->developer;
	}

	if (uf->AutoDeveloper)
	{
		_CHECK_TRACE_ASSERT_(uf->AutoDeveloper->_destroy());
		delete uf->AutoDeveloper;
	}

	_SFREE(uf->RawHistogram);
	if ( uf->conf->darkframe!=NULL )
	{
		_CHECK_TRACE_ASSERT_(uf->conf->darkframe->_close());
		delete	uf->conf->darkframe;
		uf->conf->darkframe = NULL;
	}
	return S_OK;
};


static HRESULT ufraw_flip_image_buffer(ufraw_image_data *img, int flip)
{
	/* Following code was copied from dcraw's flip_image()
	* and modified to work with any pixel depth. */

	if (!img)
		return E_POINTER;

	int base, dest, next, row, col;
	guint8 *image = img->buffer;
	int height = img->height;
	int width = img->width;
	int depth = img->depth;
	int size = height * width;
	guint8 hold[8];
	unsigned *flag = g_new0(unsigned, (size+31) >> 5);
	for (base = 0; base < size; base++) {
		if (flag[base >> 5] & (1 << (base & 31)))
			continue;
		dest = base;
		memcpy(hold, image+base*depth, depth);
		while (1)
		{
			if (flip & 4)
			{
				row = dest % height;
				col = dest / height;
			} else {
				row = dest / width;
				col = dest % width;
			}
			if (flip & 2)
				row = height - 1 - row;
			if (flip & 1)
				col = width - 1 - col;
			next = row * width + col;
			if (next == base) break;
			flag[next >> 5] |= 1 << (next & 31);
			memcpy(image+dest*depth, image+next*depth, depth);
			dest = next;
		}
		memcpy(image+dest*depth, hold, depth);
	}
	g_free (flag);
	if (flip & 4)
	{
		img->height = width;
		img->width = height;
		img->rowstride = height * depth;
	}
	return S_OK;
}

HRESULT c_ufraw_data::_flip_orientation(int flip)
{
	ufraw_data * uf = &_m_ufraw_data;
	const char flipMatrix[8][8] = {
		{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* No flip */
		{ 1, 0, 3, 2, 5, 4, 7, 6 }, /* Flip horizontal */
		{ 2, 3, 0, 1, 6, 7, 4, 5 }, /* Flip vertical */
		{ 3, 2, 1, 0, 7, 6, 5, 4 }, /* Rotate 180 */
		{ 4, 6, 5, 7, 0, 2, 1, 3 }, /* Flip over diagonal "\" */
		{ 5, 7, 4, 6, 1, 3, 0, 2 }, /* Rotate 270 */
		{ 6, 4, 7, 5, 2, 0, 3, 1 }, /* Rotate 90 */
		{ 7, 5, 6, 4, 3, 1, 2, 0 }  /* Flip over diagonal "/" */
	};
	uf->conf->orientation = flipMatrix[uf->conf->orientation][flip];
	return S_OK;
};

HRESULT c_ufraw_data::_flip_image(int flip)
{
	_HR;
	ufraw_data * uf = &_m_ufraw_data;
	_CHECK_RET_(this->_flip_orientation(flip));
	_CHECK_RET_(ufraw_flip_image_buffer(&uf->Images[ufraw_first_phase], flip));
	if ( uf->conf->threshold>0 )
	{
		_CHECK_RET_(ufraw_flip_image_buffer(&uf->Images[ufraw_denoise_phase], flip));
	}
	_CHECK_RET_(ufraw_flip_image_buffer(&uf->Images[ufraw_develop_phase], flip));

	return S_OK;
};

HRESULT c_ufraw_data::_set_wb()
{
	_HR;
	ufraw_data * uf = &_m_ufraw_data;
	dcraw_data *raw = (dcraw_data *)(uf->raw->GetDCRawData());

	double rgbWB[3];
	int status, c, cc, i;

	// For manual_wb we calculate chanMul from the temperature/green. 
	// For all other it is the other way around. 

	if (!strcmp(uf->conf->wb, manual_wb))
	{
		Temperature_to_RGB(uf->conf->temperature, rgbWB);
		rgbWB[1] = rgbWB[1] / uf->conf->green;
		//	Suppose we shot a white card at some temperature:
		//	rgbWB[3] = rgb_cam[3][4] * preMul[4] * camWhite[4]
		//	Now we want to make it white (1,1,1), so we replace preMul
		//	with chanMul, which is defined as:
		//	chanMul[4][4] = cam_rgb[4][3] * (1/rgbWB[3][3]) * rgb_cam[3][4]
		//	* preMul[4][4]
		//	We "upgraded" preMul, chanMul and rgbWB to diagonal matrices.
		//	This allows for the manipulation:
		//	(1/chanMul)[4][4] = (1/preMul)[4][4] * cam_rgb[4][3] * rgbWB[3][3]
		//	* rgb_cam[3][4]
		//	We use the fact that rgb_cam[3][4] * (1,1,1,1) = (1,1,1) and get:
		//	(1/chanMul)[4] = (1/preMul)[4][4] * cam_rgb[4][3] * rgbWB[3]
		//
		if (uf->raw_color)
		{
			// If there is no color matrix it is simple 
			for (c=0; c<3; c++)
			{
				uf->conf->chanMul[c] = raw->pre_mul[c] / rgbWB[c];
			}
		} else
		{
			for (c=0; c<raw->colors; c++)
			{
				double chanMulInv = 0;
				for (cc=0; cc<3; cc++)
					chanMulInv += 1/raw->pre_mul[c] * raw->cam_rgb[c][cc]
				* rgbWB[cc];
				uf->conf->chanMul[c] = 1/chanMulInv;
			}
		}
		// Normalize chanMul[] so that DCRAW_MIN(chanMul[]) will be 1.0
		
		double min = uf->conf->chanMul[0];
		
		for (c=1; c<raw->colors; c++)
			if (uf->conf->chanMul[c] < min)
				min = uf->conf->chanMul[c];

		if (min==0.0)
			min = 1.0; // should never happen, just to be safe

		for (c=0; c<raw->colors; c++)
			uf->conf->chanMul[c] /= min;

		if (raw->colors<4)
			uf->conf->chanMul[3] = 0.0;

		uf->conf->WBTuning = 0;
		return S_OK;
	}

	if (!strcmp(uf->conf->wb, spot_wb))
	{
		/* do nothing */
		uf->conf->WBTuning = 0;
	}
	else if ( !strcmp(uf->conf->wb, auto_wb) )
	{
		int p;
		/* Build a raw channel histogram */
		// todo optimize?
		image_type *histogram;
		histogram = g_new0(image_type, uf->rgbMax+1);
		for (i=0; i<raw->raw.height*raw->raw.width; i++)
		{
			gboolean countPixel = TRUE;
			/* The -25 bound was copied from dcraw */
			for (c=0; c<raw->raw.colors; c++)
				if (raw->raw.image[i][c] > uf->rgbMax+raw->black-25)
					countPixel = FALSE;
			if (countPixel)
			{
				for (c=0; c<raw->raw.colors; c++)
				{
					p = DCRAW_MIN(DCRAW_MAX(raw->raw.image[i][c]-raw->black,0),uf->rgbMax);
					histogram[p][c]++;
				}
			}
		}

		for (c=0; c<uf->colors; c++)
		{
			gint64 sum = 0;
			for (i=0; i<uf->rgbMax+1; i++)
				sum += (gint64)i*histogram[i][c];
			if (sum==0)
				uf->conf->chanMul[c] = 1.0;
			else
				uf->conf->chanMul[c] = 1.0/sum;
		}

		g_free(histogram);
		uf->conf->WBTuning = 0;
	} else if ( !strcmp(uf->conf->wb, camera_wb) )
	{
		if (FAILED(hr = uf->raw->_set_color_scale(!strcmp(uf->conf->wb, auto_wb),!strcmp(uf->conf->wb, camera_wb))))
		{
			if (hr == E_IMAGERAW(IMAGERAW_NOCAMWB_ERROR))
			{
				// todo
				//ufraw_message(UFRAW_BATCH_MESSAGE,
				//	_("Cannot use camera white balance, "
				//	"reverting to auto white balance."));

				//ufraw_message(UFRAW_INTERACTIVE_MESSAGE,
				//	_("Cannot use camera white balance, "
				//	"reverting to auto white balance."));
				g_strlcpy(uf->conf->wb, auto_wb, max_name);
					return this->_set_wb();
			}
			return hr;
		}
		for (c=0; c<raw->colors; c++)
			uf->conf->chanMul[c] = raw->post_mul[c];
		uf->conf->WBTuning = 0;
	}
	else 
	{
		int lastTuning = -1;
		char model[max_name];
		if ( strcmp(uf->conf->make,"MINOLTA")==0 &&	( strncmp(uf->conf->model, "ALPHA", 5)==0 ||
			strncmp(uf->conf->model, "MAXXUM", 6)==0 ) ) {
				// Canonize Minolta model names (copied from dcraw)
				g_snprintf(model, max_name, "DYNAX %s",uf->conf->model+6+(uf->conf->model[0]=='M'));
		}
		else
		{
			g_strlcpy(model, uf->conf->model, max_name);
		}

		for (i=0; i<wb_preset_count; i++)
		{
			if (!strcmp(uf->conf->wb, wb_preset[i].name) &&
				!strcmp(uf->conf->make, wb_preset[i].make) &&
				!strcmp(model, wb_preset[i].model) )
			{
					if (uf->conf->WBTuning == wb_preset[i].tuning)
					{
						for (c=0; c<raw->colors; c++)
							uf->conf->chanMul[c] = wb_preset[i].channel[c];
						break;
					} else if (uf->conf->WBTuning < wb_preset[i].tuning)
					{
						if (-1 == lastTuning)
						{
							/* WBTuning was set to a value smaller than possible */
							uf->conf->WBTuning = wb_preset[i].tuning;
							for (c=0; c<raw->colors; c++)
								uf->conf->chanMul[c] = wb_preset[i].channel[c];
							break;
						} else
						{
							/* Extrapolate WB tuning values:
							* f(x) = f(a) + (x-a)*(f(b)-f(a))/(b-a) */
							for (c=0; c<raw->colors; c++)
								uf->conf->chanMul[c] = wb_preset[i].channel[c] +
								(uf->conf->WBTuning - wb_preset[i].tuning) *
								(wb_preset[lastTuning].channel[c] -
								wb_preset[i].channel[c]) /	(wb_preset[lastTuning].tuning -	wb_preset[i].tuning);
							break;
						}
					} else if (uf->conf->WBTuning > wb_preset[i].tuning)
					{
						lastTuning = i;
					}
			} else if (lastTuning != -1)
			{
				/* WBTuning was set to a value larger than possible */
				uf->conf->WBTuning = wb_preset[lastTuning].tuning;
				for (c=0; c<raw->colors; c++)
					uf->conf->chanMul[c] = wb_preset[lastTuning].channel[c];
				break;
			}
		}

		if (i==wb_preset_count)
		{
			if (lastTuning!=-1)
			{
				/* WBTuning was set to a value larger than possible */
				uf->conf->WBTuning = wb_preset[lastTuning].tuning;
				for (c=0; c<raw->colors; c++)
					uf->conf->chanMul[c] = wb_preset[lastTuning].channel[c];
			} else
			{
				g_strlcpy(uf->conf->wb, manual_wb, max_name);
				this->_set_wb();
				return S_FALSE;
			}
		}
	}
	
	// Normalize chanMul[] so that DCRAW_MIN(chanMul[]) will be 1.0
	double min = uf->conf->chanMul[0];
	for (c=1; c<raw->colors; c++)
		if (uf->conf->chanMul[c] < min) min = uf->conf->chanMul[c];

	if (min==0.0)
		min = 1.0; /* should never happen, just to be safe */

	for (c=0; c<raw->colors; c++)
		uf->conf->chanMul[c] /= min;

	if (raw->colors<4)
		uf->conf->chanMul[3] = 0.0;

	// (1/chanMul)[4] = (1/preMul)[4][4] * cam_rgb[4][3] * rgbWB[3]
	// Therefore:
	// rgbWB[3] = rgb_cam[3][4] * preMul[4][4] * (1/chanMul)[4]
	//

	if (uf->raw_color)
	{
		/* If there is no color matrix it is simple */
		for (c=0; c<3; c++)
		{
			rgbWB[c] = raw->pre_mul[c] / uf->conf->chanMul[c];
		}
	} else
	{
		for (c=0; c<3; c++)
		{
			rgbWB[c] = 0;
			for (cc=0; cc<raw->colors; cc++)
				rgbWB[c] += raw->rgb_cam[c][cc] * raw->pre_mul[cc]/ uf->conf->chanMul[cc];
		}
	}
	// From these values we calculate temperature, green values
	RGB_to_Temperature(rgbWB, &uf->conf->temperature, &uf->conf->green);

	return S_OK;
};


HRESULT	c_ufraw_data::_auto_expose()
{
	int sum, stop, wp, c, pMax, pMin, p;
	image_type pix;
	guint16 p16[3], pixtmp[3];
	_HR;

	ufraw_data * uf = &_m_ufraw_data;

	if (uf->conf->autoExposure != apply_state)
		return S_OK;

	// Reset the exposure and luminosityCurve
	uf->conf->exposure = 0;
	
	// If we normalize the exposure then 0 EV also gets normalized
	if ( uf->conf->ExposureNorm>0 )
		uf->conf->exposure = -log((double)1.0*uf->rgbMax/uf->conf->ExposureNorm)/log((double)2);

	_CHECK_TRACE_ASSERT_(this->_developer_prepare(auto_developer));

	// Find the grey value that gives 99% luminosity
	double maxChan = 0;
	for (c=0; c<uf->colors; c++)
		maxChan = DCRAW_MAX(uf->conf->chanMul[c], maxChan);

	for (pMax=uf->rgbMax, pMin=0, p=(pMax+pMin)/2; pMin<pMax-1; p=(pMax+pMin)/2)
	{
		for (c=0; c<uf->colors; c++)
			pix[c] = DCRAW_MIN (p * maxChan/uf->conf->chanMul[c], uf->rgbMax);
		
		_CHECK_TRACE_ASSERT_(uf->AutoDeveloper->develope(p16, pix,16, pixtmp, 1));

		for (c=0, wp=0; c<3; c++)
			wp = DCRAW_MAX(wp, p16[c]);

		if (wp < 0x10000 * 99/100)
			pMin = p;
		else
			pMax = p;
	}

	//	set cutoff at 99% of the histogram
	ufraw_build_raw_histogram(uf);

	stop = uf->RawCount * 1/100;
	//	Calculate the white point
	for (wp=uf->rgbMax, sum=0; wp>1 && sum<stop; wp--)
		sum += uf->RawHistogram[wp];

	//	Set 99% of the luminosity values with luminosity below 99%
	uf->conf->exposure = log((double)p/wp)/log((double)2);

	//	If we are going to normalize the exposure later,
	//  we need to cancel its effect here.
	if ( uf->conf->ExposureNorm>0 )
		uf->conf->exposure -= log((double)(1.0*uf->rgbMax/uf->conf->ExposureNorm))/log((double)2);

	uf->conf->autoExposure = enabled_state;
	//    ufraw_message(UFRAW_SET_LOG, "ufraw_auto_expose: "
	//	    "Exposure %f (white point %d/%d)\n", uf->conf->exposure, wp, p);
	return S_OK;
};


HRESULT	c_ufraw_data::_auto_black()
{
	_HR;
	int sum, stop, bp, c;
	image_type pix;
	guint16 p16[3], pixtmp[3];
	ufraw_data * uf = &_m_ufraw_data;

	if (uf->conf->autoBlack == disabled_state)
		return S_FALSE;

	// Reset the luminosityCurve 
	_CHECK_TRACE_ASSERT_(this->_developer_prepare(auto_developer));

	// Calculate the black point
	ufraw_build_raw_histogram(uf);

	stop = uf->RawCount/256/4;

	for (bp=0, sum=0; bp<uf->rgbMax && sum<stop; bp++)
		sum += uf->RawHistogram[bp];

	double maxChan = 0;
	for (c=0; c<uf->colors; c++)
		maxChan = DCRAW_MAX(uf->conf->chanMul[c], maxChan);

	for (c=0; c<uf->colors; c++)
		pix[c] = DCRAW_MIN (bp * maxChan/uf->conf->chanMul[c], uf->rgbMax);

	_CHECK_TRACE_ASSERT_(uf->AutoDeveloper->develope(p16, pix, 16, pixtmp, 1));
	
	for (c=0, bp=0; c<3; c++)
		bp = DCRAW_MAX(bp, p16[c]);

	CurveDataSetPoint(&uf->conf->curve[uf->conf->curveIndex], 0, (double)bp/0x10000, 0);

	uf->conf->autoBlack = enabled_state;
	//    ufraw_message(UFRAW_SET_LOG, "ufraw_auto_black: "
	//	    "Black %f (black point %d)\n",
	//	    uf->conf->curve[uf->conf->curveIndex].m_anchors[0].x, bp);
	return S_OK;
};

HRESULT	c_ufraw_data::_auto_curve()
{
	_HR;
	ufraw_data * uf = &_m_ufraw_data;
	int sum, stop, steps=8, bp, p, i, j, c;
	image_type pix;
	guint16 p16[3], pixtmp[3];
	CurveData *curve = &uf->conf->curve[uf->conf->curveIndex];

	double decay = 0.90;
	double norm = (1-pow(decay,steps))/(1-decay);

	CurveDataReset(curve);
	_CHECK_TRACE_ASSERT_(this->_developer_prepare(auto_developer));
	
	// Calculate curve points
	ufraw_build_raw_histogram(uf);
	stop = uf->RawCount/256/4;
	double maxChan = 0;
	for (c=0; c<uf->colors; c++)
		maxChan = DCRAW_MAX(uf->conf->chanMul[c], maxChan);

	for (bp=0, sum=0, p=0, i=j=0; i<steps && bp<uf->rgbMax && p<0xFFFF; i++)
	{
		for (; bp<uf->rgbMax && sum<stop; bp++)
			sum += uf->RawHistogram[bp];

		for (c=0; c<uf->colors; c++)
			pix[c] = DCRAW_MIN (bp * maxChan/uf->conf->chanMul[c], uf->rgbMax);

		uf->AutoDeveloper->develope(p16, pix,16, pixtmp, 1);

		for (c=0, p=0; c<3; c++)
			p = DCRAW_MAX(p, p16[c]);

		stop += uf->RawCount * pow(decay,i) / norm;
		
		// Skip adding point if slope is too big (more than 4) 
		if (j>0 && p - curve->m_anchors[j-1].x*0x10000 < (i+1-j)*0x04000/steps)
			continue;

		curve->m_anchors[j].x = (double)p/0x10000;
		curve->m_anchors[j].y = (double)i/steps;
		j++;
	}

	if (bp==0x10000)
	{
		curve->m_numAnchors = j;
	}
	else
	{
		curve->m_anchors[j].x = 1.0;
		// The last point can be up to twice the height of a linear
		// interpolation of the last two points 
		if (j>1)
		{
			curve->m_anchors[j].y = curve->m_anchors[j-1].y +
				2 * (1.0 - curve->m_anchors[j-1].x) *
				(curve->m_anchors[j-1].y - curve->m_anchors[j-2].y) / (curve->m_anchors[j-1].x - curve->m_anchors[j-2].x);

			if (curve->m_anchors[j].y > 1.0)
				curve->m_anchors[j].y = 1.0;
		} else
		{
			curve->m_anchors[j].y = 1.0;
		}
		curve->m_numAnchors = j+1;
	}
	return S_OK;
};


HRESULT c_ufraw_data::_read_embedded()
{
	return E_NOTIMPL;
}

HRESULT c_ufraw_data::_get_thumbnail(void ** data,size_t * datasize)
{
	if (!data || !datasize)
		return E_POINTER;
	
	c_dcraw_data * dcraw_data = NULL;
	
	if (!(dcraw_data = _m_ufraw_data.raw))
		return E_IMAGERAW(IMAGERAW_FILE_NOT_OPENED_ERROR);

	return dcraw_data->_get_thumbnail(data,datasize);
}