
This is the development area for IMDI, the
Interger Multi-Dimensional Interpolation routines.

They provide a flexible and high performance
system for applying color transforms to typical
raster pixel data. Because they provide a means of
applying arbitrary combination dependent mappings
of multi-channel pixel data, there are many other
possible uses for these sorts of routines as well,
including high quality matting/compositing. For instance,
one could create a smooth, proportional "chroma key"
type of matt for matting one RGB image onto another
by creating a 6 channel to 3 dimensional transform,
that its applied to each pair of pixels from the
source images and produces one combined output pixel.
Additional input or output alpha channels are easy
to add by simply adding more input and/or output
dimensions. The matting calculatons can be almost
arbitrarily complex, and the imdi will apply them
to the pixel data at very high speed.

The system has two parts, one that generates taylored,
optimised source code for the transformation kernels,
and the run time code that matches a transform request
to a compiled kernel, and initialises the appropriate
run time lookup tables.

The kernel source generator is intended to accomodate
various optimisations, such as assembly code, vector
instruction set (ie. MMX, AltiVec etc.) versions, but
at present only generates the more portable 'C' code
kernels.

Both 8 bit per component and 16 bit per component
pixel data is handled, up to 8 input and output
dimensions (but this limit could be trivially raised).

imdi_gen.exe	is the module that triggers the generation of
		optimised source code as configured for the color spaces
		and pixel formats selected. By default creates
		a single imdi_k.c and imdi_k.h file, but if
		given the -i flag, creates a separate file
		for each kernel variant.

cgen.c		C code generator module.

itest.c	regresion test routine.
		Normally runs speed and accuracy tests for
		all configured kernel variants.
		The -q flag makes it run quicker,
		but makes the benchmarking inacurate,
		the -s flag will cause it to stop
		if any routine has unexpectedly low
		accuracy.

cctiff.c	is the utility that takes an ICC device
		profile link, and converts a TIFF file
		from the input colorspace to the output
		space. Both 8 bit and 16 bit TIFF files
		are handled, as well as colorspaces up to
		8 channels in and out.
        This accepts either a device link ICC profile,
        or links an input and output devce ICC profile
        to define the color transform.


greytiff.c  is a utility similar to cctiff, that
        is an example of how to colorimetrically
        convert an RGB file into a monochrome RGB file.



Misc. Notes
-----------

   ITU-T Rec. T.42 specifies the ITULAB encoding in terms of a range
    and offset for each component, which are related to the minimum and
    maximum values as follows:

        minimum = - (range x offset) / 2^n - 1
        maximum = minimum + range

    The Decode field default values depend on the color space. For the
    ITULAB color space encoding, the default values correspond to the
    base range and offset, as specified in ITU-T Rec. T.42 [T.42]. The
    following table gives the base range and offset values for
    BitsPerSample=8 and 12, and the corresponding default minimum and
    maximum default values for the Decode field, calculated using the
    equations above when PhotometricInterpetation=10.

                       +-----------------------------------------------+
                       | ITU-T Rec. T.42  |           Decode           |
 +---------+-----------|   base values    |       default values       |
 | BitsPer + Component +------------------+----------------------------+
 | -Sample |           |  Range | Offset  |      Min     |     Max     |
 +---------+-----------+--------+---------+--------------+-------------+
 |    8    |    L*     |   100  |    0    |       0      |     100     |
 |         +-----------+--------+---------+--------------+-------------+
 |         |    a*     |   170  |   128   |  -21760/255  |  21590/255  |
 |         +-----------+--------+---------+--------------+-------------+
 |         |    b*     |   200  |    96   |  -19200/255  |  31800/255  |
 +---------+-----------+--------+---------+--------------+-------------+
 |   12    |    L*     |   100  |    0    |       0      |     100     |
 |         +-----------+--------+---------+--------------+-------------+
 |         |    a*     |   170  |  2048   | -348160/4095 | 347990/4095 |
 |         +-----------+--------+---------+--------------+-------------+
 |         |    b*     |   200  |  1536   | -307200/4095 | 511800/4095 |
 +---------+-----------+--------+---------+--------------+-------------+

   For example, when PhotometricInterpretation=10 and BitsPerSample=8,
   the default value for Decode is (0, 100, -21760/255, 21590/255,
   -19200/255, 31800/255).

