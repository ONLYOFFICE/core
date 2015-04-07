
#pragma once
#pragma warning (disable:4018);
#pragma warning (disable:4305);


#include "dcrawcommon.h"
#include <vector>

#define DCRawMemException_strlength 200 

struct RawMemException
{
	char funname[DCRawMemException_strlength];
	RawMemException()
	{
		strcpy_s(funname,DCRawMemException_strlength,"");
	}

	RawMemException(const char * funname_)
	{
		strcpy_s(funname,DCRawMemException_strlength,funname_);
	}

	void Set(const char * funname_)
	{
		strcpy_s(funname,DCRawMemException_strlength,funname_);
	}
};

class DCRaw
{
public:

	/* Metadata */
	unsigned int _exif_ifd_offset;
	unsigned int _exif_base_offset;
	bool _has_exif;

	unsigned int _previous_tag;

	typedef struct ifd_entry
	{
		ifd_entry(unsigned short ifd_tag, unsigned int ifd_base_offset,	unsigned int ifd_ifd_offset)
			:_ifd_tag(ifd_tag), _ifd_base_offset(ifd_base_offset), _ifd_ifd_offset(ifd_ifd_offset)
		{
		}
		unsigned short _ifd_tag;
		unsigned int _ifd_base_offset;
		unsigned int _ifd_ifd_offset;
	} _ifd_entry;
	
	typedef std::vector<_ifd_entry> _ifd_entrys_t;

	_ifd_entrys_t _ifd_entrys;

//////////////////////////////////////////////////////////////////////////////////////////////////
	long __imagesize;

//////////////////////////////////////////////////////////////////////////////////////////////////
	FILE		*ifp;
	short		order;
	char		*ifname;
	char		*meta_data, xtrans[6][6];
	char		cdesc[5], desc[512], make[64], model[64], model2[64], artist[64];
	float		flash_used, canon_ev, iso_speed, shutter, aperture, focal_len;
	time_t		timestamp;
	off_t		strip_offset, data_offset;
	off_t		thumb_offset, meta_offset, profile_offset;
	unsigned	shot_order, kodak_cbpp, exif_cfa, unique_id;
	unsigned	thumb_length, meta_length, profile_length;
	unsigned	thumb_misc, *oprof, fuji_layout, shot_select, multi_out;
	unsigned	tiff_nifds, tiff_samples, tiff_bps, tiff_compress;
	unsigned	black, cblack[4], maximum, mix_green, raw_color, zero_is_bad, use_gamma;
	unsigned	zero_after_ff, is_raw, dng_version, is_foveon, data_error;
	unsigned	tile_width, tile_length, gpsdata[32], load_flags;
	unsigned	flip, tiff_flip, filters, colors;
	ushort		raw_height, raw_width, height, width, top_margin, left_margin;
	ushort		shrink, iheight, iwidth, fuji_width, thumb_width, thumb_height;
	ushort		*raw_image, (*image)[4];
	ushort		white[8][8], curve[0x10000], cr2_slice[3], sraw_mul[4];
       
	double		pixel_aspect, aber[4], gamm[6];

	float		bright, user_mul[4], threshold;

	int			mask[8][4];
	int			half_size, four_color_rgb, document_mode, highlight;
	int			verbose, use_auto_wb, use_camera_wb, use_camera_matrix;
	int			output_color, output_bps, output_tiff, med_passes;
	int			no_auto_bright;
	unsigned	greybox[4];
	float		cam_mul[4], pre_mul[4], cmatrix[3][4], rgb_cam[3][4];
	int			histogram[4][0x2000];

	HRESULT	(DCRaw::*write_thumb)(void ** data,size_t * datasize);

	void	(DCRaw::*load_raw)(), (DCRaw::*thumb_load_raw)();

	struct _decode 
	{
		_decode *branch[2];
		int leaf;
	} first_decode[2048], *second_decode, *free_decode;

	struct _tiff_ifd {
		int width, height, bps, comp, phint, offset, flip, samples, bytes;
		int tile_width, tile_length;
	} tiff_ifd[10];

	struct _ph1 {
		int format, key_off, black, black_off, split_col, tag_21a;
		float tag_210;
	} ph1;

	int tone_curve_size, tone_curve_offset; /* Nikon Tone Curves UF*/
	int tone_mode_offset, tone_mode_size; /* Nikon ToneComp UF*/

	int		lastStatus;

	DCRaw();
	~DCRaw();

	char*	strcasestr (char *haystack, const char *needle);

	int		fcol (int row, int col);
	void	derror();
	void	merror_ (void *ptr, const char *where);
	ushort	sget2 (uchar *s);
	ushort	get2();
	unsigned sget4 (uchar *s);
	unsigned get4();
	unsigned getint (int type);
	float	int_to_float (int i);
	double	getreal (int type);
	void	read_shorts (ushort *pixel, int count);

	void	init_decoder();
	uchar	*make_decoder (const uchar *source, int level);
    ushort  *make_decoder (const uchar *source);

	void	crw_init_tables (unsigned table);

	void	canon_600_fixed_wb (int temp);
	int		canon_600_color (int ratio[2], int mar);
	void	canon_600_auto_wb();
	void	canon_600_coeff();
	void	canon_600_load_raw();
	void	canon_600_correct();
	void	remove_zeroes();
	int		canon_s2is();
    ushort * make_decoder_ref(const uchar **source);
    void     crw_init_tables(unsigned table, ushort *huff[2]);
	unsigned getbits (int nbits);
    unsigned getbithuff (int nbits, ushort *huff);
    unsigned gethuff(unsigned short * h);
	int		canon_has_lowbits();
	void	canon_load_raw();
	int		ljpeg_start (struct jhead *jh, int info_only);
    void    ljpeg_end(struct jhead *jh);
	int     ljpeg_diff(ushort *huff);
	ushort	*ljpeg_row (int jrow, struct jhead *jh);
	void	lossless_jpeg_load_raw();
	void	canon_sraw_load_raw();
	void	lossless_dng_load_raw();
	void	packed_dng_load_raw();
	void	adobe_copy_pixel (unsigned row, unsigned col, ushort **rp);

	void	pentax_tree();
    void    pentax_load_raw();
	void	pentax_k10_load_raw();
	void	nikon_load_raw();
	void	nikon_compressed_load_raw();
	int		nikon_e995();
	int		nikon_e2100();
	void	nikon_3700();
	int		minolta_z2();
	void	nikon_e900_load_raw();

	void	jpeg_thumb (FILE *tfp);
	HRESULT	jpeg_thumb_ (void ** data,size_t * datasize);

	void	ppm_thumb (FILE *tfp);
	HRESULT	ppm_thumb_(void ** data,size_t * datasize);
	
	void	ppm16_thumb(FILE *tfp);
	HRESULT	ppm16_thumb_(void ** data,size_t * datasize);

	void	layer_thumb (FILE *tfp);
	HRESULT	layer_thumb_(void ** data,size_t * datasize);

	void	rollei_thumb (FILE *tfp);
	HRESULT	rollei_thumb_(void ** data,size_t * datasize);

	void	foveon_thumb (FILE *tfp);
	HRESULT	foveon_thumb_ (void ** data,size_t * datasize);

	int		raw (unsigned row, unsigned col);
	void	rollei_load_raw();
	int		bayer (unsigned row, unsigned col);
	void	phase_one_flat_field (int is_float, int nc);
	void	phase_one_correct();
	void	phase_one_load_raw();
	
	unsigned ph1_bits (int nbits);
    unsigned ph1_bithuff(int nbits, ushort *huff);
    unsigned ph1_huff(ushort *h);
	
	void	phase_one_load_raw_c();
	void	hasselblad_load_raw();
	void	leaf_hdr_load_raw();
	void	sinar_4shot_load_raw();
	void	imacon_full_load_raw();
	void	packed_12_load_raw();
    void    packed_load_raw();
	void	unpacked_load_raw();
	void	nokia_load_raw();
	unsigned	pana_bits (int nbits);
	void	panasonic_load_raw();
	void	samsung_load_raw();
	void	canon_rmf_load_raw();
    void    olympus_load_raw();
	void	minolta_rd175_load_raw();
	void	casio_qv5700_load_raw();
	void	quicktake_100_load_raw();
	const int	* make_decoder_int (const int *source, int level);
	void	kodak_radc_load_raw();
	void	kodak_jpeg_load_raw();
	void	lossy_dng_load_raw();
	void	kodak_dc120_load_raw();
	void	kodak_easy_load_raw();
    void    eight_bit_load_raw();
	void	kodak_yrgb_load_raw();
	void	kodak_262_load_raw();
	int		kodak_65000_decode (short *out, int bsize);
	void	kodak_65000_load_raw();
	void	kodak_ycbcr_load_raw();
	void	kodak_rgb_load_raw();
	void	kodak_thumb_load_raw();
	void	sony_decrypt (unsigned *data, int len, int start, int key);
	void	sony_load_raw();
	void	sony_arw_load_raw();
	void	sony_arw2_load_raw();
	void	smal_decode_segment (unsigned seg[2][2], int holes);
	void	smal_v6_load_raw();
	int		median4 (int *p);
	void	fill_holes (int holes);
	void	smal_v9_load_raw();
	void	redcine_load_raw();
	
	void	foveon_decoder (unsigned size, unsigned code);
	void	foveon_sd_load_raw();
	void	foveon_load_camf();
	void	foveon_dp_load_raw();
	void	foveon_huff (ushort *huff);

	const char	* foveon_camf_param (const char *block, const char *param);
	void	* foveon_camf_matrix (unsigned dim[3], const char *name);
	int		foveon_fixed (void *ptr, int size, const char *name);
	float	foveon_avg (short *pix, int range[2], float cfilt);
	short	* foveon_make_curve (double max, double mul, double filt);
	void	foveon_make_curves(short **curvep, float dq[3], float div[3], float filt);
	int		foveon_apply_curve (short *curve, int i);
	void	crop_masked_pixels();
	void	foveon_interpolate();
	void	bad_pixels(char *fname);
	void	subtract(const char *fname);
	void	gamma_curve (double pwr, double ts, int mode, int imax);
	void	pseudoinverse (double (*in)[3], double (*out)[3], int size);
	void	cam_xyz_coeff (double cam_xyz[4][3]);
	void	hat_transform (float *temp, float *base, int st, int size, int sc);
	void	wavelet_denoise();
	void	scale_colors();
	void	pre_interpolate();
	void	border_interpolate (int border);
	void	lin_interpolate();
	void	vng_interpolate();
	void	ppg_interpolate();
	void	ahd_interpolate();
	void	cielab (ushort rgb[3], short lab[3]);
	void	xtrans_interpolate (int passes);
	void	median_filter();
	void	blend_highlights();
	void	recover_highlights();
	void	tiff_get (unsigned base,unsigned *tag, unsigned *type, unsigned *len, unsigned *save);
	void	parse_thumb_note (int base, unsigned toff, unsigned tlen);
	void	parse_makernote (int base, int uptag);
	void	get_timestamp (int reversed);
	void	parse_exif (int base);
	void	parse_gps (int base);
	void	romm_coeff (float romm_cam[3][3]);
	void	parse_mos (int offset);
	void	linear_table (unsigned len);
	int		parse_tiff_ifd (int base);
	void	parse_kodak_ifd (int base);
	int	    parse_tiff (int base);
    void    apply_tiff();
	void	parse_minolta (int base);
	void	parse_external_jpeg();
	void	ciff_block_1030();
	void	parse_ciff (int offset, int length, int depth);
	void	parse_rollei();
	void	parse_sinar_ia();
	void	parse_phase_one (int base);
	void	parse_fuji (int offset);
	int		parse_jpeg (int offset);
	void	parse_riff();
	void	parse_smal (int offset, int fsize);
	void	parse_cine();
	char	* foveon_gets (int offset, char *str, int len);
	void	parse_redcine();
	void	parse_foveon();
	void	adobe_coeff (const char *make, const char *model);
	void	simple_coeff (int index);
	short	guess_byte_order (int words);
    float   find_green(int bps, int bite, int off0, int off1);
	void	identify();
	void	convert_to_rgb();
	void	fuji_rotate();
	void	stretch();
	int		flip_index(int row, int col);
	void	output_image (uchar * buffer,long size);
	void	tiff_set(ushort *ntag,ushort tag, ushort type, int count, int val);
	void	tiff_head(struct tiff_hdr *th, int full);
	
	void	write_ppm_tiff (FILE *ofp);
	void	write_ppm(FILE *ofp);
	void	write_ppm16(FILE *ofp);
	void	write_psd(FILE *ofp);

	int		main(int argc, const char **argv);
};