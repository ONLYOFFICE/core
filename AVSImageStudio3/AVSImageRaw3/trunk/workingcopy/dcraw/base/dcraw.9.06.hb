/*
   dcraw.h - Dave Coffin's raw photo decoder - header for C++ adaptation
   Copyright 1997-2010 by Dave Coffin, dcoffin a cybercom o net
   Copyright 2004-2011 by Udi Fuchs, udifuchs a gmail o com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This is a adaptation of Dave Coffin's original dcraw.c to C++.
   It can work as either a command-line tool or called by other programs.
 */

#define ushort UshORt
typedef unsigned char uchar;
typedef unsigned short ushort;

/*
 * The following is somewhat ugly because of various requirements:
 * 1. The stand-alone dcraw binary should not depend on glib
 * 2. The amount of changes to dcraw source code should be minimal
 * 3. On win32 fopen needs to be replaced by g_fopen
 * 4. On other systems g_fopen is defined as a macro
 * 5. g_fopen only exists since glib 2.6
 */
#if !defined(DCRAW_NOMAIN) && defined(WIN32)
#include <glib.h>
extern "C" {
#include <glib/gstdio.h>
}
#define fopen g_fopen
#endif

class DCRaw
{
public:
    /* All dcraw's global variables are members of this class. */
    FILE *ifp, *ofp;
    short order;
    /*const*/
    char *ifname, *ifname_display;
    char *meta_data;
    char cdesc[5], desc[512], make[64], model[64], model2[64], artist[64];
    float flash_used, canon_ev, iso_speed, shutter, aperture, focal_len;
    time_t timestamp;
    unsigned shot_order, kodak_cbpp, filters, exif_cfa, unique_id;
    off_t    strip_offset, data_offset;
    off_t    thumb_offset, meta_offset, profile_offset;
    unsigned thumb_length, meta_length, profile_length;
    unsigned thumb_misc, *oprof, fuji_layout, shot_select, multi_out;
    unsigned tiff_nifds, tiff_samples, tiff_bps, tiff_compress;
    unsigned black, cblack[8], maximum, mix_green, raw_color, zero_is_bad;
    unsigned zero_after_ff, is_raw, dng_version, is_foveon, data_error;
    unsigned tile_width, tile_length, gpsdata[32], load_flags;
    ushort raw_height, raw_width, height, width, top_margin, left_margin;
    ushort shrink, iheight, iwidth, fuji_width, thumb_width, thumb_height;
    int flip, tiff_flip, colors;
    double pixel_aspect, aber[4], gamm[6];
    ushort(*image)[4], white[8][8], curve[0x10000], cr2_slice[3], sraw_mul[4];
    float bright, user_mul[4], threshold;
    int half_size, four_color_rgb, document_mode, highlight;
    int verbose, use_auto_wb, use_camera_wb, use_camera_matrix;
    int output_color, output_bps, output_tiff, med_passes;
    int no_auto_bright;
    unsigned greybox[4];
    float cam_mul[4], pre_mul[4], cmatrix[3][4], rgb_cam[3][4];
    int histogram[4][0x2000];
    void (DCRaw::*write_thumb)(), (DCRaw::*write_fun)();
    void (DCRaw::*load_raw)(), (DCRaw::*thumb_load_raw)();
    jmp_buf failure;

    struct decode {
        struct decode *branch[2];
        int leaf;
    } first_decode[2048], *second_decode, *free_decode;

    struct tiff_ifd {
        int width, height, bps, comp, phint, offset, flip, samples, bytes;
    } tiff_ifd[10];

    struct ph1 {
        int format, key_off, black, black_off, split_col, tag_21a;
        float tag_210;
    } ph1;

    int tone_curve_size, tone_curve_offset; /* Nikon Tone Curves UF*/
    int tone_mode_offset, tone_mode_size; /* Nikon ToneComp UF*/

    /* Used by dcraw_message() */
    char *messageBuffer;
    int lastStatus;

    unsigned ifpReadCount;
    unsigned ifpSize;
    unsigned ifpStepProgress;
#define STEPS 50
    void ifpProgress(unsigned readCount);
// Override standard io function for integrity checks and progress report
    size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
    size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
    char *fgets(char *s, int size, FILE *stream);
    int fgetc(FILE *stream);
// dcraw only calls fscanf for single variables
    int fscanf(FILE *stream, const char *format, void *ptr);
// calling with more variables would triger a link error
//int fscanf(FILE *stream, const char *format, void *ptr1, void *ptr2, ...);

    /* Initialization of the variables is done here */
    DCRaw();
    ~DCRaw();
    void dcraw_message(int code, const char *format, ...);
    /* All dcraw functions with the CLASS prefix are members of this class. */
    int fc(int row, int col);
    void merror(void *ptr, const char *where);
    void derror();
    ushort sget2(uchar *s);
    ushort get2();
    unsigned sget4(uchar *s);
    unsigned get4();
    unsigned getint(int type);
    float int_to_float(int i);
    double getreal(int type);
    void read_shorts(ushort *pixel, unsigned count);
    void canon_600_fixed_wb(int temp);
    int canon_600_color(int ratio[2], int mar);
    void canon_600_auto_wb();
    void canon_600_coeff();
    void canon_600_load_raw();
    void remove_zeroes();
    int canon_s2is();
    unsigned getbithuff(int nbits, ushort *huff);
    ushort * make_decoder_ref(const uchar **source);
    ushort * make_decoder(const uchar *source);
    void crw_init_tables(unsigned table, ushort *huff[2]);
    int canon_has_lowbits();
    void canon_compressed_load_raw();
    int ljpeg_start(struct jhead *jh, int info_only);
    void ljpeg_end(struct jhead *jh);
    int ljpeg_diff(ushort *huff);
    ushort * ljpeg_row(int jrow, struct jhead *jh);
    void lossless_jpeg_load_raw();
    void canon_sraw_load_raw();
    void adobe_copy_pixel(int row, int col, ushort **rp);
    void adobe_dng_load_raw_lj();
    void adobe_dng_load_raw_nc();
    void pentax_load_raw();
    void nikon_compressed_load_raw();
    int nikon_is_compressed();
    int nikon_e995();
    int nikon_e2100();
    void nikon_3700();
    int minolta_z2();
    void fuji_load_raw();
    void ppm_thumb();
    void layer_thumb();
    void rollei_thumb();
    void rollei_load_raw();
    int bayer(unsigned row, unsigned col);
    void phase_one_flat_field(int is_float, int nc);
    void phase_one_correct();
    void phase_one_load_raw();
    unsigned ph1_bithuff(int nbits, ushort *huff);
    void phase_one_load_raw_c();
    void hasselblad_load_raw();
    void leaf_hdr_load_raw();
    void sinar_4shot_load_raw();
    void imacon_full_load_raw();
    void packed_load_raw();
    void unpacked_load_raw();
    void nokia_load_raw();
    unsigned pana_bits(int nbits);
    void panasonic_load_raw();
    void olympus_load_raw();
    void minolta_rd175_load_raw();
    void quicktake_100_load_raw();
    void kodak_radc_load_raw();
    void kodak_jpeg_load_raw();
    void kodak_dc120_load_raw();
    void eight_bit_load_raw();
    void kodak_yrgb_load_raw();
    void kodak_262_load_raw();
    int kodak_65000_decode(short *out, int bsize);
    void kodak_65000_load_raw();
    void kodak_ycbcr_load_raw();
    void kodak_rgb_load_raw();
    void kodak_thumb_load_raw();
    void sony_decrypt(unsigned *data, int len, int start, int key);
    void sony_load_raw();
    void sony_arw_load_raw();
    void sony_arw2_load_raw();
    void smal_decode_segment(unsigned seg[2][2], int holes);
    void smal_v6_load_raw();
    int median4(int *p);
    void fill_holes(int holes);
    void smal_v9_load_raw();
    void foveon_decoder(unsigned size, unsigned code);
    void foveon_thumb();
    void foveon_load_camf();
    void foveon_load_raw();
    const char * foveon_camf_param(const char *block, const char *param);
    void * foveon_camf_matrix(unsigned dim[3], const char *name);
    int foveon_fixed(void *ptr, int size, const char *name);
    float foveon_avg(short *pix, int range[2], float cfilt);
    short * foveon_make_curve(double max, double mul, double filt);
    void foveon_make_curves
    (short **curvep, float dq[3], float div[3], float filt);
    int foveon_apply_curve(short *curve, int i);
    void foveon_interpolate();
    void bad_pixels(const char *fname);
    void subtract(const char *fname);
    void gamma_curve(double pwr, double ts, int mode, int imax);
    void pseudoinverse(double(*in)[3], double(*out)[3], int size);
    void cam_xyz_coeff(double cam_xyz[4][3]);
    void colorcheck();
    void hat_transform(float *temp, float *base, int st, int size, int sc);
    void wavelet_denoise();
    void scale_colors();
    void pre_interpolate();
    void border_interpolate(int border);
    void lin_interpolate();
    void vng_interpolate();
    void ppg_interpolate();
    void ahd_interpolate();
    void median_filter();
    void blend_highlights();
    void recover_highlights();
    void tiff_get(unsigned base,
                  unsigned *tag, unsigned *type, unsigned *len, unsigned *save);
    void parse_thumb_note(int base, unsigned toff, unsigned tlen);
    void parse_makernote(int base, int uptag);
    void get_timestamp(int reversed);
    void parse_exif(int base);
    void parse_gps(int base);
    void romm_coeff(float romm_cam[3][3]);
    void parse_mos(int offset);
    void linear_table(unsigned len);
    void parse_kodak_ifd(int base);
    int parse_tiff_ifd(int base);
    int parse_tiff(int base);
    void apply_tiff();
    void parse_minolta(int base);
    void parse_external_jpeg();
    void ciff_block_1030();
    void parse_ciff(int offset, int length);
    void parse_rollei();
    void parse_sinar_ia();
    void parse_phase_one(int base);
    void parse_fuji(int offset);
    int parse_jpeg(int offset);
    void parse_riff();
    void parse_smal(int offset, unsigned fsize);
    void parse_cine();
    char * foveon_gets(int offset, char *str, int len);
    void parse_foveon();
    void adobe_coeff(const char *make, const char *model);
    void simple_coeff(int index);
    short guess_byte_order(int words);
    float find_green(int bps, int bite, int off0, int off1);
    void identify();
#ifndef NO_LCMS
    void apply_profile(const char *input, const char *output);
#endif
    void convert_to_rgb();
    void fuji_rotate();
    void stretch();
    int flip_index(int row, int col);
    void tiff_set(ushort *ntag,
                  ushort tag, ushort type, int count, int val);
    void tiff_head(struct tiff_hdr *th, int full);
    void jpeg_thumb();
    void write_ppm_tiff();
    int main(int argc, const char **argv);
};
