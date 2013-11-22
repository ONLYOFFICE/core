/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exports.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define joe_FAKE_JUSTINTIMEDEBUGGERS() \
	do { __try { } __except (1) { } } while (0)
#else
#define joe_FAKE_JUSTINTIMEDEBUGGERS()
#endif

/////////////////////////////////////////////////////////////////////////////

#define OPTSMAX	4096
static char outoptsbuf[OPTSMAX + 1];

static void ColorToGrayscale(LPSTR pmem, int cx, int cy);

#define ErrorCode(x) do {BusyUseStatics=false;return E_FAIL;} while (0)	//S_OK

// тэєЄЁхээшх яхЁхьхээvх (ЄхяхЁ№ эх ¤ъёяюЁЄшЁє¦Єё )
static bool BusyUseStatics = false;	// яЁюёЄхщ°р  чр•шЄр threads
static ImageFormat BmpFormat;
static FILE* infile;
int Jp2Status;
long Jp2NumWarnings;	/*num.of corrupt-data warnings*/


#define UINTSWAP(i) \
	((i)<<24 | ((i)&0xFF00)<<8 | (i)>>8&0xFF00 | (i)>>24&0xFF)
#define WORDSWAP(w) ((w)<<8&0xFF00 | (w)>>8&0xFF)

#define	JP2_IHDR_LEN	14
#define	JP2_VALIDATELEN	\
		max(JP2_IHDR_LEN-JP2_BOX_HDRLEN, \
		max(JP2_JP_LEN-JP2_BOX_HDRLEN, 16))

// т GetJp2Format яюы№чєхьё  Єхь цх ётюшь  ъюфюь, ўЄю
// ш т OpenSave.cpp (ыєў°х, ўхь Validate() шч JasPer)

static int bmp_putdata(LPSTR bpMem, jas_image_t *image, UINT w = 0, UINT h = 0, int FrameNum = 0, int SetStep = 0)
{
	UINT i, j, y;
	jas_matrix_t *cmpts[3];
//	int numpad;
	unsigned char red;
	unsigned char grn;
	unsigned char blu;
	int ret;
	uint_fast32_t width;
	uint_fast32_t height;
	uint_fast16_t depth;
	uint_fast16_t cmptno;
	uint_fast16_t numcmpts;

	numcmpts = jas_image_numcmpts(image);
	width = jas_image_cmptwidth(image, 0);
	height = jas_image_cmptheight(image, 0);
	depth = jas_image_cmptprec(image, 0);

	/* Check to ensure that the image to be saved can actually be represented
	  using the BMP format. */
	if (numcmpts != 3 && numcmpts != 1) return -1;
	for (cmptno = 0; cmptno < numcmpts; ++cmptno) {
		if (jas_image_cmptwidth(image, cmptno) != width ||
		  jas_image_cmptheight(image, cmptno) != height ||
		  jas_image_cmptprec(image, cmptno) != depth ||
		  jas_image_cmptsgnd(image, cmptno) != false ||
		  jas_image_cmpttlx(image, cmptno) != 0 ||
		  jas_image_cmpttly(image, cmptno) != 0) {
		//	fprintf(stderr, "The BMP format cannot be used to represent an image with this geometry.\n");
			return -1;
		}
	}
	SetStep = SetStep <= 0? 1 : SetStep;
	UINT savw = (width+SetStep-1) / SetStep;
	UINT savh = (height+SetStep-1) / SetStep;
	w = (w == 0)? savw : w;
	h = (h == 0)? savh : h;

	savw = min(w,savw);
	savh = min(h,savh);

	/* We do not support palettized images. */
//	if (BMP_HASPAL(info) && numcmpts == 3) {
//		fprintf(stderr, "no palettized image support for BMP format\n");
//		return -1;
//	}

	ret = 0;
	for (i = 0; i < numcmpts; ++i) {
		cmpts[i] = 0;
	}

	/* Create temporary matrices to hold component data. */
	for (i = 0; i < numcmpts; ++i) {
		if (!(cmpts[i] = jas_matrix_create(1, width))) {
			ret = -1;
			goto bmp_putdata_done;
		}
	}

	/* Put the image data. */
	for (i = 0; i < savh; ++i) {
		for (cmptno = 0; cmptno < numcmpts; ++cmptno) {
			if (jas_image_readcmpt(image, cmptno, 0, i*SetStep, width,
			  1, cmpts[cmptno])) {
				ret = -1;
				goto bmp_putdata_done;
			}
		}
		y = (savh - 1 - i);
		for (j = 0; j < savw; ++j) {
			if (numcmpts == 3) {
				red = (jas_matrix_getv(cmpts[0], j*SetStep));
				grn = (jas_matrix_getv(cmpts[1], j*SetStep));
				blu = (jas_matrix_getv(cmpts[2], j*SetStep));
				*(bpMem+4*w*y+4*j+0) = blu;
				*(bpMem+4*w*y+4*j+1) = grn;
				*(bpMem+4*w*y+4*j+2) = red;
			} else if (numcmpts == 1) {
				red = (jas_matrix_getv(cmpts[0], j*SetStep));
				*(bpMem+4*w*y+4*j+0) = red;
				*(bpMem+4*w*y+4*j+1) = red;
				*(bpMem+4*w*y+4*j+2) = red;
			} // else { abort(); }
		}
	}

bmp_putdata_done:
	/* Destroy the temporary matrices. */
	for (i = 0; i < numcmpts; ++i) {
		if (cmpts[i]) {
			jas_matrix_destroy(cmpts[i]);
		}
	}

	return ret;
}

static jas_image_t* bmp_getdata(LPSTR bpMem, int width, int height)
{
	jas_image_t *image;
	jas_image_cmptparm_t cmptparms[3];
	jas_image_cmptparm_t *cmptparm;
	uint_fast16_t cmptno;
	uint_fast16_t numcmpts;

	numcmpts = 3;

	for (cmptno = 0, cmptparm = cmptparms; cmptno < numcmpts; ++cmptno,
	  ++cmptparm) {
		cmptparm->tlx = 0;
		cmptparm->tly = 0;
		cmptparm->hstep = 1;
		cmptparm->vstep = 1;
		cmptparm->width = width;
		cmptparm->height = height;
		cmptparm->prec = 8;
		cmptparm->sgnd = false;
	}

	if (!(image = jas_image_create(numcmpts, cmptparms, JAS_IMAGE_CS_RGB)))
		return NULL;

	/* Read the bitmap data. */
//	if (bmp_getdata(in, info, image)) {...}

	int i;
	int j;
	int y;
	jas_matrix_t *cmpts[3];		//= {0,0,0};//joe
	unsigned char red;
	unsigned char grn;
	unsigned char blu;
	int ret;

	ret = 0;
	for (i = 0; i < numcmpts; ++i) {
		cmpts[i] = 0;
	}

	/* Create temporary matrices to hold component data. */
	for (i = 0; i < numcmpts; ++i) {
		if (!(cmpts[i] = jas_matrix_create(1, width))) {
			ret = -1;
			goto bmp_getdata_done;
		}
	}

	for (i = 0; i < height; ++i) {
		y = (height - 1 - i);
		for (j = 0; j < width; ++j) {
			blu = *(bpMem+4*width*y+4*j+0);
			grn = *(bpMem+4*width*y+4*j+1);
			red = *(bpMem+4*width*y+4*j+2);
			if (numcmpts == 3) {
				jas_matrix_setv(cmpts[0], j, red);
				jas_matrix_setv(cmpts[1], j, grn);
				jas_matrix_setv(cmpts[2], j, blu);
			} else {
				jas_matrix_setv(cmpts[0], j, red);
			}
		}
		for (cmptno = 0; cmptno < numcmpts; ++cmptno) {
			if (jas_image_writecmpt(image, cmptno, 0, i, width,
			  1, cmpts[cmptno])) {
				ret = -1;
				goto bmp_getdata_done;
			}
		}
	}

bmp_getdata_done:
	/* Destroy the temporary matrices. */
	for (i = 0; i < numcmpts; ++i) {
		if (cmpts[i]) {
			jas_matrix_destroy(cmpts[i]);
		}
	}

	if (ret) {
		jas_image_destroy(image);
		return NULL;
	}

	return image;
}


static UINT jp2_getbox(UCHAR* buf, UINT& type, bool bBigEndian)
{
	UINT len, tmp;

	if (fread(buf, JP2_BOX_HDRLEN, 1, infile) != 1)
		return 0;

	len = *(UINT*)buf;
	if (bBigEndian) len = UINTSWAP(len);
	type = *((UINT*)buf+1);
	if (bBigEndian) type = UINTSWAP(type);
	if (len == 1) {
		ULARGE_INTEGER extlen;
		if (fread(buf+JP2_BOX_HDRLEN, sizeof(UINT64),
			1, infile) != 1) return 0;
		extlen.HighPart = *((UINT*)buf+3);
		extlen.LowPart = *((UINT*)buf+2);
		if (bBigEndian)
		  tmp = UINTSWAP(extlen.LowPart),
		  extlen.LowPart = UINTSWAP(extlen.HighPart),
		  extlen.HighPart = tmp;
		// сверхбольшие размеры пока запрещаем
		if (extlen.HighPart != 0) return 0;
		if (extlen.LowPart < JP2_BOX_HDRLEN+sizeof(UINT64))
			return 0;
		len = extlen.LowPart-sizeof(UINT64);
		}
	// codestream может быть неопрееделенной длины
	if (len == 0 && type == JP2_BOX_JP2C)
		len = (UINT)-1;
	if (len < JP2_BOX_HDRLEN) return 0;
	return len;
}

static UINT skip_jp2c(bool bBigEndian)
{
	USHORT csid, cslen;

	// поиск конца JP2C codestream
	for (;;) {
		if (fread(&csid, sizeof(USHORT), 1, infile) != 1)
			return 0;
		if (bBigEndian) csid = WORDSWAP(csid);
		if (csid < JPC_MS_MIN || csid > JPC_MS_MAX)
			return 0;

		// удачное чтение JP2C codestream
		if (csid == JPC_MS_EOC) break;

	/* Get the marker segment length and parameters if present. */
	/* Note: It is tacitly assumed that a marker segment cannot have
	  parameters unless it has a length field.  That is, there cannot
	  be a parameters field without a length field and vice versa. */
		if (JPC_MS_HASPARMS(csid)) {
			// Get the length of the marker segment.
			if (fread(&cslen, sizeof(USHORT), 1, infile) != 1)
				return 0;
			if (bBigEndian) cslen = WORDSWAP(cslen);
			if (cslen < 3) return 0;

			if (csid != JPC_MS_SOT)
				// в отличие от jasper - здесь нет страховки
				// от erroneous cslen (зато так быстрее)
				fseek(infile,cslen-2,SEEK_CUR);
			else {	// для JPC_MS_SOT пропускаем весь tile-part
				USHORT isot;
				UINT tplen;
				if (cslen < 10) return 0;
				if (fread(&isot, sizeof(USHORT), 1, infile) != 1)
					return 0;
				if (bBigEndian) isot = WORDSWAP(isot);

				if (fread(&tplen, sizeof(UINT), 1, infile) != 1)
					return 0;
				if (bBigEndian) tplen = UINTSWAP(tplen);
				if (tplen == 0)
					fseek(infile,-2,SEEK_END);
				else {
					if (tplen < 10) return 0;
					fseek(infile,tplen-10,SEEK_CUR);
					}
				}
			}
		}

	return 1;
}

static BOOL get_JPEG2000_info(FILE* in, LPIMAGEFORMAT pFmt)
{
	unsigned char buf[JP2_VALIDATELEN];
	bool bBigEndian = true;		// always for JasPer!
	UINT len, type;
	WORD flags = 0;
	bool dataflag;
	int FrameNo = 0;

	if (in == NULL) return FALSE;
	infile = in;

	fseek(infile,0,SEEK_SET);

	// 1) JP box
	if ((len = jp2_getbox(buf, type, bBigEndian))==0)
		return FALSE;
	len -= JP2_BOX_HDRLEN;

	flags = (len > 0)? 0 : -1;
	dataflag = !(flags & (JP2_BOX_SUPER | JP2_BOX_NODATA));

	if (!dataflag) return FALSE;
	if (len > sizeof(buf)) return FALSE;
	if (len < sizeof(UINT)) return FALSE;
	if (fread(buf, len, 1, infile) != 1) return FALSE;

	UINT magic = *((UINT*)buf);
	if (bBigEndian) magic = UINTSWAP(magic);
	if (type != JP2_BOX_JP || magic != JP2_JP_MAGIC)
		return FALSE;

	// 2) FTYP box
	if ((len = jp2_getbox(buf, type, bBigEndian))==0)
		return FALSE;

	if (type != JP2_BOX_FTYP) return FALSE;
	len -= JP2_BOX_HDRLEN;

	flags = (len > 0)? 0 : -1;
	dataflag = !(flags & (JP2_BOX_SUPER | JP2_BOX_NODATA));

	if (!dataflag) return FALSE;
	if (len > sizeof(buf)) return FALSE;
	if (len < 2*sizeof(UINT)) return FALSE;
	if (fread(buf, 8, 1, infile) != 1) return FALSE;

	UINT majver, minver;
	majver = *((UINT*)buf);
	minver = *((UINT*)buf+1);
	if (bBigEndian) majver = UINTSWAP(majver);
	if (bBigEndian) minver = UINTSWAP(minver);
	// игнорируем compatcodes
	len -= 8;
	while (len > 0) {
		UINT sz = min(len, sizeof(UINT));
		if (fread(buf, sz, 1,
					infile) != 1) return FALSE;
		len -= sz;
		}

	// 3) ищем IHDR box и JP2C codestream
	bool found = false, jp2cfound = false;
	long ipos;
	UINT ilen;
	while (len = jp2_getbox(buf, type, bBigEndian)) {
		UINT hlen = len -= JP2_BOX_HDRLEN;

		if (type == JP2_BOX_JP2C) {
			jp2cfound = true;
			FrameNo++;
			// На случай неопределенной длины JP2C
			if (*(UINT*)buf == 0) {
				if (!skip_jp2c(bBigEndian)) return FALSE;
				continue;
				}
			}
		if (type != JP2_BOX_JP2H) {
			fseek(infile,len,SEEK_CUR);
			continue;
			}
		// entering super-box
		while ((len = jp2_getbox(buf, type, bBigEndian))
					> 0 && hlen >= len) {
			hlen -= len;
			if (*(UINT*)buf == 0) hlen -= sizeof(UINT64);
			len -= JP2_BOX_HDRLEN;

			switch (type) {
			case JP2_BOX_IHDR:
				found = true;
				ipos = ftell(infile);
				ilen = len;
				fseek(infile,len+hlen,SEEK_CUR);
				break;
			default:
				fseek(infile,len,SEEK_CUR);
				break;
				}
			if (found) break;
			}
		}

	if (!found || !jp2cfound) return FALSE;

	// 3a) IHDR box найден
	fseek(infile,ipos,SEEK_SET);
	len = ilen;
	flags = (len > 0)? 0 : -1;
	dataflag = !(flags & (JP2_BOX_SUPER | JP2_BOX_NODATA));

	if (!dataflag) return FALSE;

	if (len > sizeof(buf)) return FALSE;
	if (len < JP2_IHDR_LEN-JP2_BOX_HDRLEN) return FALSE;
	if (fread(buf, len, 1, infile) != 1) return FALSE;

	UINT width, height;
	USHORT numcmpts;
	height = *((UINT*)buf);
	width = *((UINT*)buf+1);
	numcmpts = *(USHORT*)((UINT*)buf+2);
	if (bBigEndian) height = UINTSWAP(height);
	if (bBigEndian) width = UINTSWAP(width);
	if (bBigEndian) numcmpts = WORDSWAP(numcmpts);

	// data for full-resolution image data! or any other level...
	pFmt->sizeX = width;
	pFmt->sizeY = height;
	pFmt->framenum = FrameNo;

	return TRUE;
}

// Retrieve image format
HRESULT GetJp2Format(LPCTSTR fileName, LPIMAGEFORMAT pFmt)
{
	while (BusyUseStatics);	BusyUseStatics = true;
	Jp2Status = 0;
	Jp2NumWarnings = 0;

	if (pFmt==NULL) ErrorCode(-1);

	int minifactor = 1;
	int frameidx = 0, FrameNo;
	FILE * input_file = NULL;

	if (lstrlen(fileName) == 0) ErrorCode(-1);
	if ((input_file = fopen(fileName, "rb")) == NULL)
        ErrorCode(-1);

	if (pFmt)
		frameidx = pFmt->frameidx;

	if (!get_JPEG2000_info(input_file, pFmt))
	{
		if (input_file != NULL)
			fclose(input_file);

		BusyUseStatics = false;
		return E_FAIL;
	}

	pFmt->colors = 32;
	FrameNo = pFmt->framenum;
	pFmt->frameidx = frameidx<0? 0: frameidx>FrameNo? 0: frameidx;

	if (input_file != NULL)
		fclose(input_file);

	HRESULT hr=(Jp2Status==0 && Jp2NumWarnings==0)?S_OK:S_FALSE;
	BusyUseStatics = false;

	return hr;
}



// Load image from file
HRESULT LoadJp2(LPCTSTR fileName, LPSAFEARRAY *saImage, LPIMAGEFORMAT pFmt /*= NULL*/)
{
	// check for valid input parameters
	if (lstrlen(fileName) == 0)
		return E_FAIL;
    if (!saImage || !*saImage)
		return E_FAIL;
	if (SafeArrayGetDim(*saImage) != 3)
		return E_FAIL; 

	// variables
	long lLbound, lUbound;

	// read image size
	SafeArrayGetLBound(*saImage,1,&lLbound);
	SafeArrayGetUBound(*saImage,1,&lUbound);

	// check for valid number of channels in the image
	if (lUbound - lLbound != 3)
		return E_FAIL; 

	// clear error and warning storages
	Jp2Status = 0;
	Jp2NumWarnings = 0;

	// initialize jasper library
	jas_init();

	// connecto to the file
	jas_stream_t *input_file = jas_stream_fopen(fileName, "rb");

	// check for valid opened file
	if (!input_file)
	{
		jas_image_clearfmts();
		return E_FAIL;
	}

	// search format header
	jas_image_getfmt(input_file);

	// read format from file
	int infmt = jas_image_getfmt(input_file);

	// check for valid format
	if (infmt < 0)
	{
		if (input_file) jas_stream_close(input_file);
		jas_image_clearfmts();
		return E_FAIL;
	}

	// variables
	int minifactor = 1;
	int frameidx = 0;

	// figure out the output format, and set up to write it
    ZeroMemory(&BmpFormat, sizeof(BmpFormat));

	// lock the array
	if (SafeArrayLock(*saImage) != S_OK)
	{
		if (input_file) jas_stream_close(input_file);
		jas_image_clearfmts();
		return E_FAIL;
	}

	// read image width
	SafeArrayGetLBound(*saImage, 2, &lLbound);
	SafeArrayGetUBound(*saImage, 2, &lUbound);
	BmpFormat.sizeX = lUbound - lLbound + 1;

	// read image height
	SafeArrayGetLBound(*saImage, 3, &lLbound);
	SafeArrayGetUBound(*saImage, 3, &lUbound);
	BmpFormat.sizeY = lUbound - lLbound + 1;

	// setup image data pointer
	BmpFormat.image_ptr = (UCHAR*)((*saImage)->pvData);
	BmpFormat.colors = 32;

	// read image from file
	jas_image_t *image = jas_image_decode(input_file, infmt, NULL);

	// variables
	int errors = 0;

	// check for valid iamge
	if (!image)
		errors = 1;
	else
	{
		// put the image to joe's decoder
		errors = bmp_putdata((LPSTR)BmpFormat.image_ptr, image, BmpFormat.sizeX, BmpFormat.sizeY, frameidx, minifactor);
	}

	// close all resources
	if (input_file) jas_stream_close(input_file);
	if (image) jas_image_destroy(image);
	jas_image_clearfmts();

	// unlock the array
	if (SafeArrayUnlock(*saImage) != S_OK)
		return E_FAIL;

	// check for gained errors
	if (errors || Jp2Status != 0 || Jp2NumWarnings != 0)
		return E_FAIL;

	// all ok
	return S_OK;
}


// Save image to file
HRESULT SaveJp2(LPCTSTR fileName, LPSAFEARRAY *saImage, LPIMAGEFORMAT pFmt /*= NULL*/)
{
//	FILE * output_file = NULL;

	while (BusyUseStatics);	BusyUseStatics = true;
	Jp2Status = 0;
	Jp2NumWarnings = 0;
//	long num_scanlines;
	long lLbound, lUbound;
	jas_image_t *image;
//	cmdopts_t *cmdopts;
	jas_stream_t *output_file;

	if (saImage==NULL || *saImage==NULL) ErrorCode(-1);

	if (SafeArrayGetDim(*saImage)!= 3) ErrorCode(-1); 

	SafeArrayGetLBound(*saImage,1,&lLbound);
	SafeArrayGetUBound(*saImage,1,&lUbound);
	if (lUbound-lLbound!=3) ErrorCode(-1); 

	if (lstrlen(fileName) == 0) ErrorCode(-1);

joe_FAKE_JUSTINTIMEDEBUGGERS();
//__try {
	jas_init();

	if ((output_file = jas_stream_fopen(fileName, "w+b")) == NULL) {
//		MessageBox( GetActiveWindow(),
//					"Jp2DLL: can't open output file\n",
//					NULL, MB_ICONQUESTION | MB_OK);
		jas_image_clearfmts();
		ErrorCode(-1);	//exit(EXIT_FAILURE);
		}

	int outfmt;
	if ((outfmt = jas_image_strtofmt("jp2")) < 0)
		{ jas_stream_close(output_file);
		jas_image_clearfmts(); ErrorCode(-1); }

	/* Initialize the JPEG compression object with default error handling. */

	/* Figure out the input format, and set up to read it. */
    ZeroMemory(&BmpFormat, sizeof(BmpFormat));

	if ( SafeArrayLock(*saImage) != S_OK )
		{ jas_stream_close(output_file);
		jas_image_clearfmts(); ErrorCode(-1); }

	SafeArrayGetLBound(*saImage,2,&lLbound);
	SafeArrayGetUBound(*saImage,2,&lUbound);
	BmpFormat.sizeX = lUbound - lLbound + 1;

	SafeArrayGetLBound(*saImage,3,&lLbound);
	SafeArrayGetUBound(*saImage,3,&lUbound);
	BmpFormat.sizeY = lUbound - lLbound + 1;

	BmpFormat.image_ptr = (UCHAR *)(*saImage)->pvData;
	BmpFormat.colors = 32;
	*outoptsbuf = '\0';

	// юяЎшш эрёЄЁющъш
	if (pFmt) {
//		if (pFmt->out_color_space == 1)	// Force monochrome output.
//			bGrayscale = true;
//		bit_depth = pFmt->out_bit_depth;

		if (pFmt->quality<100) { // Quality factor
			double dq = (pFmt->quality-100.0)/25.0;
			dq = pow(10,dq);
			sprintf(outoptsbuf,"rate=%.4g ",dq);
			}
		}
//	if (BmpFormat.sizeX > 256)
//		strcat(outoptsbuf,"tilewidth=256 ");
//	if (BmpFormat.sizeY > 256)
//		strcat(outoptsbuf,"tileheight=256 ");

  /* Process data */
int errors = 0;

	if (!(image = bmp_getdata((LPSTR)BmpFormat.image_ptr,
			BmpFormat.sizeX, BmpFormat.sizeY)))
		errors = -1;

	if (!errors)
		errors = jas_image_encode(image, output_file,
		outfmt, outoptsbuf);
	jas_stream_flush(output_file);

	if (output_file != NULL) jas_stream_close(output_file);
	if (image) jas_image_destroy(image);
	jas_image_clearfmts();

	if (errors) {
		if ( SafeArrayUnlock(*saImage) != S_OK ) ErrorCode(-1);
		BusyUseStatics = false;
		return E_FAIL;
		}
//}
//__except (-1) { }

	if ( SafeArrayUnlock(*saImage) != S_OK ) ErrorCode(-1);

	HRESULT hr=(Jp2Status==0 && Jp2NumWarnings==0)?S_OK:S_FALSE;
	BusyUseStatics = false;
	return hr;
}

// Convert color to grayscale : Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
static void ColorToGrayscale(LPSTR pmem, int cx, int cy)
{
	int x, y;	
	UCHAR *p, *pm, col, r, g, b;
	ULONG lcol;
	p = pm = (UCHAR*)pmem;
	for (y = 0; y < cy; y++)
		for (x = 0; x < cx; x++) {
			b = *p++, g = *p++, r = *p++;
			lcol = (114 * b +587 * g +299 * r + 500)/1000;
			if (lcol > 255) lcol = 255;
			col = (UCHAR) lcol;
			*pm++ = col, *pm++ = col, *pm++ = col;
			pm++, p++;
			}
}

