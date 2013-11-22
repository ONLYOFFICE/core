/*!	\file	ufrawdata_core.h
 *	\brief	Работа с ufraw_data (заголовочный файл)
 */

#pragma once

#ifdef HAVE_LENSFUN
#include <lensfun.h>
#endif /* HAVE_LENSFUN */

#include ".\dcraw\dcrawcommon.h"
#include ".\dcraw\nikon_curve.h"
#include "dcrawdata_core.h"
#include <time.h> // for time_t

/* macro to clamp a number between two values */
#ifndef LIM
#define LIM(x,min,max) MAX(min,MIN(x,max))
#endif

#define MAXOUT			255 /* Max output sample */

#define max_curves		20
#define max_anchors		20
#define max_profiles	20
#define max_path		300
#define max_name		80

/* An impossible value for conf float values */
#define NULLF -10000.0

/* Options, like auto-adjust buttons can be in 3 states. Enabled and disabled
* are obvious. Apply means that the option was selected and some function
* has to act accourdingly, before changing to one of the first two states */
enum {
		disabled_state,
		enabled_state,
		apply_state
	};

#define spot_wb		"Spot WB"
#define manual_wb	"Manual WB"
#define camera_wb	"Camera WB"
#define auto_wb		"Auto WB"

class c_developer_data;
class c_ufraw_data;

enum
{	rgb_histogram,
	r_g_b_histogram,
	luminosity_histogram,
	value_histogram,
	saturation_histogram
};

enum
{
	linear_histogram,
	log_histogram
};

/* The following enum should match the dcraw_interpolation enum in dcraw_api.h. */

enum 
{
	ahd_interpolation,
	vng_interpolation,
	four_color_interpolation,
	ppg_interpolation,
	bilinear_interpolation,
	none_interpolation,
	half_interpolation,
	obsolete_eahd_interpolation,
	num_interpolations
};

enum
{
	no_id,
	also_id,
	only_id,
	send_id
};

enum 
{
	manual_curve,
	linear_curve,
	custom_curve,
	camera_curve
};

enum
{
	in_profile,
	out_profile,
	display_profile,
	profile_types
};

enum 
{
	raw_expander,
	live_expander,
	expander_count
};

enum 
{
	ppm_type,
	ppm16_deprecated_type,
	tiff_type,
	tiff16_deprecated_type,
	jpeg_type,
	png_type,
	png16_deprecated_type,
	embedded_jpeg_type,
	embedded_png_type,
	fits_type,
	num_types
};

enum
{
	clip_details,
	restore_lch_details,
	restore_hsv_details,
	restore_types
};

enum
{
	digital_highlights,
	film_highlights,
	highlights_types
};

enum	__DeveloperMode
{
	display_developer,
	file_developer,
	auto_developer
};

typedef __DeveloperMode	DeveloperMode;

enum	__Intent
{
	perceptual_intent,
	relative_intent,
	saturation_intent,
	absolute_intent,
	disable_intent
};
typedef __Intent	Intent;

enum __UFRawPhase
{
	ufraw_first_phase,
	ufraw_denoise_phase,
	ufraw_develop_phase,
	ufraw_lensfun_phase,
	ufraw_phases_num
};

typedef	__UFRawPhase UFRawPhase;


enum __GrayscaleMode
{
	grayscale_none,
	grayscale_lightness,
	grayscale_luminance,
	grayscale_value,
	grayscale_mixer
};

typedef __GrayscaleMode	GrayscaleMode;


struct __wb_data
{
	const char *make;
	const char *model;
	const char *name;
	int tuning;
	double channel[4];
};

typedef	__wb_data	wb_data;

 struct __developer_data{
	DeveloperMode mode;
	unsigned rgbMax, max, exposure, colors, useMatrix;
	int restoreDetails, clipHighlights;
	int rgbWB[4], colorMatrix[3][4];
	double gamma, linear;
//	char profileFile[profile_types][max_path];
	wchar_t profileFile[profile_types][max_path];
	void *profile[profile_types];
	Intent intent[profile_types];
	gboolean updateTransform;
	void *colorTransform;
	double saturation;
#ifdef UFRAW_CONTRAST
	double contrast;
#endif
	CurveData baseCurveData, luminosityCurveData;
	guint16 gammaCurve[0x10000];
	void *luminosityProfile;
	void *TransferFunction[3];
	void *saturationProfile;
	GrayscaleMode grayscaleMode;
	double grayscaleMixer[3];
};

typedef	__developer_data developer_data;

struct __profile_data
{
	char name[max_name];
//	char file[max_path];
	wchar_t file[max_path];
	char productName[max_name];
	double gamma, linear;
	int BitDepth;
};

typedef __profile_data profile_data;

typedef guint16 image_type[4];

#define _CONF_NO_SAVE
#define _CONF_NO_GUI


struct __conf_data{

	~__conf_data();

	/* Internal data */
	int confSize, version;

	/* IMAGE manipulation settings */
	//    int wb;
	char	wb[max_name];
	double	WBTuning;
	double	temperature, green;
	double	chanMul[4];
	double	threshold;

#ifdef UFRAW_CONTRAST
	double contrast;
#endif

	double			exposure, saturation, black; /* black is only used in CMD */
	int				ExposureNorm;
	int				restoreDetails, clipHighlights;
	int				autoExposure, autoBlack;
	int				BaseCurveIndex, BaseCurveCount;
	CurveData		BaseCurve[max_curves];
	int				curveIndex, curveCount;
	CurveData		curve[max_curves];
	int				profileIndex[profile_types], profileCount[profile_types];
	profile_data	profile[profile_types][max_profiles];
	Intent			intent[profile_types];
	int				interpolation;
	int				smoothing;
	char			darkframeFile[max_path];
	//struct			ufraw_struct *darkframe;
	c_ufraw_data	*darkframe;
	int				CropX1, CropY1, CropX2, CropY2;
	double			rotationAngle;
	int				grayscaleMode;
	double			grayscaleMixer[3];

/* SAVE options */
//#ifndef _CONF_NO_SAVE
	char inputFilename[max_path], outputFilename[max_path],
		outputPath[max_path];
	char inputURI[max_path], inputModTime[max_name];
	int type, compression, createID, embedExif, progressiveJPEG;
//#endif

	int shrink, size;
	gboolean overwrite, losslessCompress, embeddedImage;
	gboolean rotate;


	/* GUI settings */

#ifndef _CONF_NO_GUI
	double Zoom;
	int Scale;
	gboolean LockAspect; // True if aspect ratio is locked
	int saveConfiguration;
	int histogram, liveHistogramScale;
	int rawHistogramScale;
	int expander[expander_count];
	gboolean overExp, underExp, blinkOverUnder;
	gboolean RememberOutputPath;
	gboolean WindowMaximized;
	char curvePath[max_path];
	char profilePath[max_path];
	gboolean silent;
	char remoteGimpCommand[max_path];
#endif

	/* EXIF data */
	int orientation;
	float iso_speed, shutter, aperture, focal_len, subject_distance;
	char exifSource[max_name], isoText[max_name], shutterText[max_name],
		apertureText[max_name], focalLenText[max_name],
		focalLen35Text[max_name], lensText[max_name],
		flashText[max_name], whiteBalanceText[max_name];
	char timestampText[max_name], make[max_name], model[max_name];
	time_t timestamp;
	/* Unfortunately dcraw strips make and model, but we need originals too */
	char real_make[max_name], real_model[max_name];

#ifdef HAVE_LENSFUN
	lfDatabase *lensdb; /* mount/camera/lens database */
	lfCamera *camera; /* camera description */
	lfLens *lens; /* lens description */
	lfLensCalibDistortion lens_distortion; /* lens distortion parameters */
	lfLensCalibTCA lens_tca; /* lens tca parameters */
	lfLensCalibVignetting lens_vignetting; /* lens vignetting parameters */
	lfLensType cur_lens_type;
	float lens_scale; /* Additional lens postprocessing scale power-of-two, default 0 */
#endif /* HAVE_LENSFUN */
};

typedef __conf_data	conf_data;


struct __ufraw_image_data {
	guint8 *buffer;
	int height, width, depth, rowstride;
	/*! This bit field marks valid pieces of the image with 1's.
	The variable contains a fixed 4x8 matrix of bits, every bit containing
	the validity of the respective subarea of the whole image. The subarea
	sizes are determined by dividing the width by 4 and height by 8.
	This field must always contain at least 32 bits. */
	long valid;
};

typedef  __ufraw_image_data	ufraw_image_data;


/*!
 *	\todo	image_data should be phased out and replaced by ufraw_image_data
 */
struct __image_data
{
	image_type *image;
	guint8 *buffer;
	int height, width, depth, rowstride;
} ;

typedef __image_data image_data;

struct ufraw_struct {



	int			status;
	char		*message;
	//char		filename[max_path];
	wchar_t		filename[max_path];
	int			initialHeight, initialWidth, rgbMax, colors, raw_color, useMatrix;
	int			rotatedHeight, rotatedWidth;
	gboolean	LoadingID; /* Indication that we are loading an ID file */
	float		rgb_cam[3][4];
	int			ConvertShrink;
	ufraw_image_data	Images[ufraw_phases_num];
	image_data	image;
	image_data	thumb;
	//void		*raw;
	c_dcraw_data * raw;
	gboolean	HaveFilters;
	void		*unzippedBuf;
	gsize		unzippedBufLen;
//	developer_data	*developer;
//	developer_data	*AutoDeveloper;
	
	c_developer_data	*developer;
	c_developer_data	*AutoDeveloper;

	conf_data		*conf;
	void		*widget;
	guchar		*inputExifBuf;
	guint		inputExifBufLen;
	guchar		*outputExifBuf;
	guint		outputExifBufLen;
	int			gimpImage;
	int			*RawHistogram;
	int			RawChanMul[4];
	int			RawCount;

	ufraw_struct()
	{
		raw = NULL;
		developer = NULL;
		AutoDeveloper = NULL;
	}
	~ufraw_struct()
	{
		if (raw)
			delete raw;

		if (developer)
			delete developer;

		if (AutoDeveloper)
			delete AutoDeveloper;

	}

#ifdef HAVE_LENSFUN
	int postproc_ops; /* postprocessing operations (LF_MODIFY_XXX) */
	lfModifier *modifier;
#endif
};

typedef ufraw_struct	ufraw_data;


extern const conf_data conf_default;
extern const wb_data wb_preset[];
extern const int wb_preset_count;


/*!
 *	\brief	Класс-оболочка для структуры ufraw_data,
			все функции работы со структурой переписаны как методы
 */
class c_ufraw_data
{
protected:
	ufraw_data	_m_ufraw_data;
public:
	ufraw_data * GetUFRawData()
	{
		return &_m_ufraw_data;
	}

	~c_ufraw_data()
	{
		_close();
	}

	HRESULT	_open(BSTR filename);

	HRESULT	_config(conf_data *rc, conf_data *conf,conf_data *cmd);

	HRESULT	_load_raw();

	HRESULT _load_darkframe();

	HRESULT	_developer_prepare(DeveloperMode mode);

	HRESULT _convert_image();

	HRESULT	_convert_image_init();

	HRESULT	_convert_image_first_phase(gboolean lensfix);

	HRESULT _convert_image_init_phase();

	ufraw_image_data * _convert_image_area (int saidx, UFRawPhase phase);
	ufraw_image_data * _convert_image_develop();

	HRESULT _close();
	HRESULT _flip_orientation(int flip);

	HRESULT _flip_image(int flip);

	HRESULT _set_wb();

	HRESULT	_auto_expose();
	HRESULT	_auto_black();
	HRESULT	_auto_curve();

	HRESULT	_update_rotated_dimensions();
	HRESULT _read_embedded();

	HRESULT _get_thumbnail(void ** data,size_t * datasize);
};

// from ufrawconf_core.h
void conf_init (conf_data *c);

/*!
 *	\brief	Класс-оболочка для структуры developer_data,
			все функции работы со структурой переписаны как методы
 */
class c_developer_data
{
protected:
	developer_data	_m_developer_data;
public:
	developer_data	* GetDeveloperData()
	{
		return &_m_developer_data;
	}
	static int lcms_message(int ErrorCode, const char *ErrorText)
	{
		return 0;
	};
	HRESULT	_init();
	HRESULT	_destroy();

	HRESULT	_profile(int type, profile_data *p);
	
	HRESULT	_display_profile(unsigned char *profile, int size, char productName[]);
	
	HRESULT	_prepare(conf_data *conf,
					   int rgbMax, float rgb_cam[3][4], int colors, int useMatrix,
					   DeveloperMode mode);

	HRESULT	develope(void *po, guint16 pix[4], int mode, guint16 *buf, int count);

	HRESULT	develop_linear(guint16 in[4], guint16 out[3]);
};

HRESULT conf_set_cmd(conf_data *conf, const conf_data *cmd);
HRESULT conf_copy_save(conf_data *dst, const conf_data *src);
HRESULT conf_copy_transform(conf_data *dst, const conf_data *src);
HRESULT conf_copy_image(conf_data *dst, const conf_data *src);