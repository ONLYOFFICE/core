/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/
/*$Id: gdevdevn.h 9071 2008-09-04 15:46:56Z leonardo $ */
/* Include file for common DeviceN process color model devices. */

#ifndef gdevdevn_INCLUDED
# define gdevdevn_INCLUDED

/*
 * Define the maximum number of spot colors supported by this device.
 * This value is arbitrary.  It is set simply to define a limit on
 * on the separation_name_array and separation_order map.
 */
#define GX_DEVICE_MAX_SEPARATIONS GX_DEVICE_COLOR_MAX_COMPONENTS
/*
 * Define the maximum number of process model colorants.  Currently we only
 * have code for DeviceGray, DeviceRGB, and DeviceCMYK.  Thus this value
 * only needs to be 4.  However we are allowing for a future hexachrome
 * device.  (This value does not include spot colors.  See previous value.)
 */
#define MAX_DEVICE_PROCESS_COLORS 6
/*
 * This value enable the use of a compression scheme for representing
 * the colorant values in a device pixel.  Ghostscript represents device
 * pixel using an integer type value.  This limits the maximum number of
 * bits that can be used for a pixel to the largest integer size provided
 * by the compiler.  For most compilers this is 64 bits (a 'long long' or
 * _int64 type).  For devices with 4 or fewer colorantss like DeviceGray,
 * DeviceRGB, and DeviceCMYK, this will easily represent 16 bits per colorant.
 * However if the output device supports spot colorants, this limits us in
 * what we can do.  To allow support for more than 8 colorants at 8 bits
 * per colorant, we use a scheme to compress the colorant values into our
 * 64 bit pixels.  For more information see the header before
 * devn_encode_compressed_color in src/gdevdevn.c.
 *
 * To disable compression of encoded colorant values, change this definition.
 */
#define USE_COMPRESSED_ENCODING (ARCH_SIZEOF_GX_COLOR_INDEX >= 8)

/*
 * Type definitions associated with the fixed color model names.
 */
typedef const char * fixed_colorant_name;
typedef fixed_colorant_name * fixed_colorant_names_list;

/*
 * Structure for holding SeparationNames elements.
 */
typedef struct devn_separation_name_s {
    int size;
    byte * data;
} devn_separation_name;

/*
 * Structure for holding SeparationNames elements.
 */
struct gs_separations_s {
    int num_separations;
    devn_separation_name names[GX_DEVICE_MAX_SEPARATIONS];
};

#ifndef gs_separations_DEFINED
#   define gs_separations_DEFINED
    typedef struct gs_separations_s gs_separations;
#endif

/*
 * Type for holding a separation order map
 */
typedef int gs_separation_map[GX_DEVICE_MAX_SEPARATIONS + MAX_DEVICE_PROCESS_COLORS];

typedef struct gs_devn_params_s {
    /*
     * Bits per component (device colorant).  Currently only 1 and 8 are
     * supported.
     */
    int bitspercomponent;

    /*
     * Pointer to the colorant names for the color model.  This will be
     * null if we have DeviceN type device.  The actual possible colorant
     * names are those in this list plus those in the separation[i].name
     * list (below).
     */
    fixed_colorant_names_list std_colorant_names;
    int num_std_colorant_names;	/* Number of names in list */
    int max_separations;	/* From MaxSeparation parameter */
    /*
     * This value comes from scanning color space resources in PDF files.
     * Thus this value is only valid for PDF files.  The value may also
     * be high if there are color space resources that are defined for
     * a page but which are not actually used.  This value does give us
     * a maximum value for the number of spot colors.
     * From the PageSpotColors parameter.
     */
    int page_spot_colors;

    /*
    * Separation info (if any).
    */
    gs_separations separations;

    /*
     * Separation Order (if specified).
     */
    int num_separation_order_names;
    /*
     * The SeparationOrder parameter may change the logical order of
     * components.
     */
    gs_separation_map separation_order_map;
    /*
     * Pointer to our list of which colorant combinations are being used.
     */
    struct compressed_color_list_s * compressed_color_list;
    /*
     * If the file is using PDF 1.4 transparency compositing and we are using
     * the clist then we need to pass the compressed color list from the PDF
     * 1.4 clist writer device to the PDF 1.4 reader device.  However that
     * device is not created until the clist is read and being processed.
     * We need to temporary hold that data in the output device until after
     * clist reader PDF 1.4 compositing device is created.  The PDF 1.4
     * compositor may also have a different list of separations.
     */
    struct compressed_color_list_s * pdf14_compressed_color_list;
    gs_separations pdf14_separations;
} gs_devn_params_t;

#ifndef gs_devn_params_DEFINED
#  define gs_devn_params_DEFINED
typedef struct gs_devn_params_s gs_devn_params;
#endif

extern fixed_colorant_name DeviceCMYKComponents[];

#include "gsequivc.h"

/*
 * Utility routines for common DeviceN related parameters:
 *   SeparationColorNames, SeparationOrder, and MaxSeparations
 */

/*
 * Convert standard color spaces into DeviceN colorants.
 * Note;  This routine require SeparationOrder map.
 */
void gray_cs_to_devn_cm(gx_device * dev, int * map, frac gray, frac out[]);

void rgb_cs_to_devn_cm(gx_device * dev, int * map,
		const gs_imager_state *pis, frac r, frac g, frac b, frac out[]);

void cmyk_cs_to_devn_cm(gx_device * dev, int * map,
		frac c, frac m, frac y, frac k, frac out[]);

/*
 * Possible values for the 'auto_spot_colors' parameter.
 */
/*
 * Do not automatically include spot colors
 */
#define NO_AUTO_SPOT_COLORS 0
/*
 * Automatically add spot colors up to the number that the device can image.
 * Spot colors over that limit will be handled by the alternate color space
 * for the Separation or DeviceN color space.
 */
#define ENABLE_AUTO_SPOT_COLORS	1
/*
 * Automatically add spot colors up to the GX_DEVICE_MAX_SEPARATIONS value.
 * Note;  Spot colors beyond the number that the device can image will be
 * ignored (i.e. treated like a colorant that is not specified by the
 * SeparationOrder device parameter.
 */
#define ALLOW_EXTRA_SPOT_COLORS 2

/*
 * This routine will check to see if the color component name  match those
 * that are available amoung the current device's color colorants.  
 *
 * Parameters:
 *   dev - pointer to device data structure.
 *   pname - pointer to name (zero termination not required)
 *   nlength - length of the name
 *   component_type - separation name or not
 *   pdevn_params - pointer to device's DeviceN paramters
 *   pequiv_colors - pointer to equivalent color structure (may be NULL)
 *   auto_spot_colors - See comments above.
 *
 * This routine returns a positive value (0 to n) which is the device colorant
 * number if the name is found.  It returns GX_DEVICE_COLOR_MAX_COMPONENTS if
 * the color component is found but is not being used due to the
 * SeparationOrder parameter.  It returns a negative value if not found.
 *
 * This routine will also add separations to the device if space is
 * available.
 */
int devn_get_color_comp_index(gx_device * dev,
    gs_devn_params * pdevn_params, equivalent_cmyk_color_params * pequiv_colors,
    const char * pname, int name_size, int component_type,
    int auto_spot_colors);

/* Utility routine for getting DeviceN parameters */
int devn_get_params(gx_device * pdev, gs_param_list * plist,
		    gs_devn_params * pdevn_params,
		    equivalent_cmyk_color_params * pequiv_colors);

/*
 * Utility routine for handling DeviceN related parameters.  This routine
 * assumes that the device is based upon a standard printer type device.
 * (See the next routine if not.)
 *
 * Note that this routine requires a pointer to the DeviceN parameters within
 * the device structure.  The pointer to the equivalent_cmyk_color_params is
 * optional (it should be NULL if this feature is not used by the device).
 */
int devn_printer_put_params(gx_device * pdev, gs_param_list * plist,
			gs_devn_params * pdevn_params,
			equivalent_cmyk_color_params * pequiv_colors);

/* 
 * Utility routine for handling DeviceN related parameters.  This routine
 * may modify the color_info, devn_params, and the * equiv_colors fields.
 * The pointer to the equivalent_cmyk_color_params is optional (it should be
 * NULL if this feature is not used by the device).
 *
 * Note:  This routine does not restore values in case of a problem.  This
 * is left to the caller.
 */
int devn_put_params(gx_device * pdev, gs_param_list * plist,
			gs_devn_params * pdevn_params,
			equivalent_cmyk_color_params * pequiv_colors);

/*
 * This routine will check to see if the color component name  match those
 * of either the process color model colorants or the names on the
 * SeparationColorNames list.
 *
 * Parameters:
 *   dev - pointer to device data structure.
 *   pname - pointer to name (zero termination not required)
 *   nlength - length of the name
 *
 * This routine returns a positive value (0 to n) which is the device colorant
 * number if the name is found.  It returns a negative value if not found.
 */
int check_pcm_and_separation_names(const gx_device * dev,
		const gs_devn_params * pparams, const char * pname,
		int name_size, int component_type);

/*
 * This routine will extract a specified set of bits from a buffer and pack
 * them into a given buffer.
 *
 * Parameters:
 *   source - The source of the data
 *   dest - The destination for the data
 *   depth - The size of the bits per pixel - must be a multiple of 8
 *   first_bit - The location of the first data bit (LSB).
 *   bit_width - The number of bits to be extracted.
 *   npixel - The number of pixels.
 *
 * Returns:
 *   Length of the output line (in bytes)
 *   Data in dest.
 */
int repack_data(byte * source, byte * dest, int depth, int first_bit,
		int bit_width, int npixel);

/*
 * This utility routine calculates the number of bits required to store
 * color information.  In general the values are rounded up to an even
 * byte boundary except those cases in which mulitple pixels can evenly
 * into a single byte.
 *
 * The parameter are:
 *   ncomp - The number of components (colorants) for the device.  Valid
 * 	values are 1 to GX_DEVICE_COLOR_MAX_COMPONENTS
 *   bpc - The number of bits per component.  Valid values are 1, 2, 4, 5,
 *	and 8.
 * Input values are not tested for validity.
 */
int bpc_to_depth(int ncomp, int bpc);


/*
 * We are encoding color values into a gx_color_index value.  This is being
 * done to allow more than eight 8 bit colorant values to be stored inside
 * of a 64 bit gx_color_index value.
 *
 * This is done by only keeping track of non zero colorant values.  We
 * keep a record of which combinations of colorants are used inside of the
 * device and encode an index for the colorant combinations into the
 * gx_color_index value.
 *
 * See comments preceding devn_encode_compressed_color in gdevdevn.c for
 * more information about how we encode the color information into a
 * gx_color_index value.
 */
/*
 * We are using bytes to pack both which colorants are non zero and the values
 * of the colorants.  Thus do not change this value without recoding the
 * methods used for encoding our colorants.  (This definition is really here
 * to mark locations in the code that are extremely specific to using a byte
 * oriented approach to the encoding.)
 */
#define NUM_ENCODE_LIST_ITEMS 256
#define STD_ENCODED_VALUE 256

/*
 * Since we are byte packing things in a gx_color_index.  We need the number
 * of bytes in a gx_color_index.
 */
#define NUM_GX_COLOR_INDEX_BYTES ARCH_SIZEOF_GX_COLOR_INDEX
#define NUM_GX_COLOR_INDEX_BITS (8 * NUM_GX_COLOR_INDEX_BYTES)

/*
 * Define the highest level in our encoded color colorant list.
 * Since we need at least one byte to store our encoded list of colorants
 * and we are packing colorant values in bytes, the top level of our encoded
 * color colorants that is the size of gx_clor_index - 1.
 */
#define TOP_ENCODED_LEVEL (NUM_GX_COLOR_INDEX_BYTES - 1)

/*
 * The maximum number of colorants that we can encode.
 */
#define MAX_ENCODED_COMPONENTS 14

/*
 * To prevent having a bunch of one or two colorant elements, we set a
 * cutoff for the minimum number of colorants.  If we have less than the
 * cutoff then we add in our process colors on the assumption that it is
 * likely that sometime we will want a combination of the process and spot
 * colors.
 */
#define MIN_ENCODED_COMPONENTS 5

/*
 * Define a value to represent a color value that we cannot encode.  This can
 * occur if either we exceed MAX_ENCODED_COMPONENTS or all of the possible
 * levels of the encoded colorant list are full.
 */
#define NON_ENCODEABLE_COLOR (gx_no_color_index - 1)

/*
 * We keep a bit map of the colorants.  If a bit map will fit into a
 * gx_color_index sized intger then we use one, otherwize we use an array
 * of ints.
 */
#if GX_DEVICE_COLOR_MAX_COMPONENTS <= (ARCH_SIZEOF_GX_COLOR_INDEX * 8)
typedef gx_color_index comp_bit_map_t;
#define set_colorant_present(pbit_map, comp_list, comp_num)\
   (pbit_map)->comp_list |= (((gx_color_index)1) << comp_num)
#define clear_colorant_present(pbit_map, comp_list, comp_num)\
   (pbit_map)->comp_list &= ~(((gx_color_index)1) << comp_num)
#define colorant_present(pbit_map, comp_list, comp_num)\
   ((int)(((pbit_map)->comp_list >> comp_num)) & 1)
/*
 * The compare bit map soutine (for the array case) is too complex for a simple
 * macro.  So it is comditionally compiled using this switch.
 */
#define DEVN_ENCODE_COLOR_USING_BIT_MAP_ARRAY 0

#else
/*
 * If we are trying to handle more colorants than will fit into a gx_color_index,
 * then we bit pack them into uints.  So we need to define some values for
 * defining the number of elements that we need, etc.
 */
#define comp_bit_map_elem_t uint
#define BITS_PER_COMP_BIT_MAP_ELEM (size_of(comp_bit_map_elem_t) * 8)
#define COMP_BIT_MAP_ELEM_MASK (BITS_PER_COMP_BIT_MAP_ELEM - 1)

#define COMP_BIT_MAP_SIZE \
    ((GX_DEVICE_COLOR_MAX_COMPONENTS + COMP_BIT_MAP_ELEM_MASK) / \
     						BITS_PER_COMP_BIT_MAP_ELEM)

/* Bit map list of colorants in the gx_color_index value */
typedef comp_bit_map_elem_t comp_bit_map_t[COMP_BIT_MAP_SIZE];
#define set_colorant_present(pbit_map, comp_list, comp_num)\
   (pbit_map)->comp_list[comp_num / BITS_PER_COMP_BIT_MAP_ELEM] |=\
				(1 << (comp_num & COMP_BIT_MAP_ELEM_MASK))
#define clear_colorant_present(pbit_map, comp_list, comp_num)\
   (pbit_map)->comp_list[comp_num / BITS_PER_COMP_BIT_MAP_ELEM] &=\
				~(1 << (comp_num & COMP_BIT_MAP_ELEM_MASK))
#define colorant_present(pbit_map, comp_list, comp_num)\
   ((pbit_map)->comp_list[comp_num / BITS_PER_COMP_BIT_MAP_ELEM] >>\
				((comp_num & COMP_BIT_MAP_ELEM_MASK)) & 1)
/*
 * The compare bit map soutine is too complex for s simple macro.
 * So it is comditionally compiled using this switch.
 */
#define DEVN_ENCODE_COLOR_USING_BIT_MAP_ARRAY 1
#endif

/*
 * The colorant bit map list struct.
 */
typedef struct comp_bit_map_list_s {
    short num_comp;
    short num_non_solid_comp;
    bool solid_not_100;		/* 'solid' colorants are not 1005 solid */
    comp_bit_map_t colorants;
    comp_bit_map_t solid_colorants;
} comp_bit_map_list_t;

/*
 * Encoded colorant list level element definition.
 *
 * Each list level contains a list of objects.  An object can be either a
 * colorant bit map or a pointer to a sub list.  We start our sub level
 * pointer at zero and work up.  Component bit maps are started at the top and
 * go down.  When they meet in the middle, then this list level element is full.
 * Note:  We start with the bit maps in the upper positions to ensure that
 * gx_no_color_index and NON_ENCODEABLE_COLOR values correspond to 7 colorant
 * cases.  This is less likely to occur and less likely to cause a problem
 * when we tweak the outputs to keep actual gx_color_index values from
 * being one of these two special cases.
 *
 * See comments preceding devn_encode_compressed_color in gdevdevn.c for
 * more information about how we encode the color information into a
 * gx_color_index value.
 */
typedef struct compressed_color_list_s {
    /*
     * The number of colorants for this level of the encoded color list.
     * Note:  Each sub level encodes one fewer colorants.
     */
    int level_num_comp;
    /* The number of sub level list pointers */
    int num_sub_level_ptrs;
    /* The current lower limit for the colorant bit maps */
    int first_bit_map;
    /*
     * Use a union since an object is either a sub level pointer or a colorant
     * bit map but not both.
     */
    union {
	struct compressed_color_list_s * sub_level_ptrs[NUM_ENCODE_LIST_ITEMS];
	comp_bit_map_list_t comp_data[NUM_ENCODE_LIST_ITEMS];
    } u;
} compressed_color_list_t;

/*
 * Encode a list of colorant values into a gx_color_index_value.
 *
 * This routine is designed to pack more than eight 8 bit colorant values into
 * a 64 bit gx_color_index value.  It does this piece of magic by keeping a list
 * of which colorant combinations are actualy used (i.e. which colorants are non
 * zero).  The non zero colorant values and and an 'index' value are packed into
 * the output gx_color_index value.
 *
 * See comments preceding devn_encode_compressed_color in gdevdevn.c for more
 * information about how we encode the color information into a gx_color_index
 * value.
 */
gx_color_index devn_encode_compressed_color(gx_device *pdev,
	const gx_color_value colors[], gs_devn_params * pdevn_params);

/*
 * Decode a gx_color_index value back to a list of colorant values.  This
 * routine assumes that the gx_color_index value is 'encoded' as described
 * for devn_encode_compressed_color.
 *
 * See comments preceding devn_encode_compressed_color in gdevdevn.c for more
 * information about how we encode the color information into a gx_color_index
 * value.
 */
int devn_decode_compressed_color(gx_device * dev, gx_color_index color,
			gx_color_value * out, gs_devn_params * pdevn_params);

/*
 * Unpack a row of 'encoded color' values.  These values are encoded as
 * described for the devn_encode_color routine.
 *
 * The routine takes a raster line of data and expands each pixel into a buffer
 * of 8 bit values for each colorant.
 *
 * See comments preceding devn_encode_color in gdevdevn.c for more information
 * about how we encode the color information into a gx_color_index value.
 */
int devn_unpack_row(gx_device * dev, int num_comp, gs_devn_params * pdevn_params,
					 int width, byte * in, byte * out);

/*
 * Find the bit map for given bit map index.
 */
comp_bit_map_list_t * find_bit_map(gx_color_index index,
	       			compressed_color_list_t * pcomp_list);

/*
 * Allocate an list level element for our encode color list.
 */
compressed_color_list_t * alloc_compressed_color_list_elem(gs_memory_t * mem,
	       							int num_comps);

/*
 * The elements of this array contain the number of bits used to encode a color
 * value in a 'compressed' gx_color_index value.  The index into the array is
 * the number of compressed components.
 */
extern int num_comp_bits[];

/*
 * Values used to decompressed the colorants in our encoded values back into
 * a gx_color value.  The color value will be (comp_bits * entry) >> 8
 * The number of bits in comp_bits are defined in the num_comp_bits table.
 * These values are chosen to expand these bit combinations back to 16 bit values
 * (after shifting right 8 bits).
 */
/*
 * The elements of this array contain factors used to convert compressed color
 * values to gx_color_values.  The index into the array is the number of
 * compressed components.
 */
extern int comp_bit_factor[];

/*
 * A routine for debugging the encoded color colorant list.  This routine
 * dumps the contents of the list.
 */
void print_compressed_color_list(compressed_color_list_t * pcomp_list, int num_comp);

/*
 * Free the elements of a compressed color list.
 */
void free_compressed_color_list(gs_memory_t * mem,
	       	compressed_color_list_t * pcomp_list);

/*
 * Free a set of separation names
 */
void free_separation_names(gs_memory_t * mem, gs_separations * pseparation);

#endif		/* ifndef gdevdevn_INCLUDED */
