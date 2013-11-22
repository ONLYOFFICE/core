#include <config.h>
#include "iptc-jpeg.h"

#include <string.h>
#include <stdio.h>

#include "i18n.h"


typedef enum {
	IL_JPEG_SKIP_BYTES,
	IL_JPEG_MARKER,
} IptcJpegState;

#define JPEG_MARKER		0xff
#define JPEG_MARKER_SOI		0xd8
#define JPEG_MARKER_APP0	0xe0
#define JPEG_MARKER_APP1	0xe1
#define JPEG_MARKER_APP13	0xed
#define JPEG_MARKER_APP15	0xef
#define JPEG_MARKER_SOS		0xda

#define JPEG_PS3_ID		"Photoshop 3.0"
#define JPEG_BIM_ID		"8BIM"
#define JPEG_BIM_IPTC_TYPE	0x0404

static int
iptc_jpeg_seek_to_ps3 (FILE * infile, FILE * outfile, int abort_early)
{
	int seek=0, i, s;
	unsigned char buf[256];
	IptcJpegState state = IL_JPEG_MARKER;

	s = i = 0;
	while (1) {
		if ((s-i) < 18) {
			if (ferror(infile) || feof(infile))
				return -1;
			if (s-i > 0)
				memmove (buf, buf+i, s-i);
			s = s-i+(int)fread (buf+s-i, 1, sizeof(buf)-(s-i), infile);
			i = 0;
		}
		
		switch (state) {
		case IL_JPEG_SKIP_BYTES:
			if (seek > s - i) {
				if (outfile)
					if ((int)fwrite (buf + i, 1, s - i, outfile) < s-i)
						return -1;
				seek -= s - i;
				i = s;
			}
			else {
				if (outfile)
					if ((int)fwrite (buf + i, 1, seek, outfile) < seek)
						return -1;
				state = IL_JPEG_MARKER;
				i += seek;
			}
			break;
			
		case IL_JPEG_MARKER:
			if (buf[i] != JPEG_MARKER)
				return -1;
			if (buf[i+1] == JPEG_MARKER_SOI) {
				/* Do nothing */
			}
			else if (buf[i+1] == JPEG_MARKER_APP13 &&
					!memcmp(buf+i+4, JPEG_PS3_ID, 14)) {
				seek = iptc_get_short (buf+i+2,
						IPTC_BYTE_ORDER_MOTOROLA);
				fseek (infile, -s+i, SEEK_CUR);
				return seek - 2;
			}
			else if (buf[i+1] == JPEG_MARKER_SOS) {
				/* No more headers to search, abort */
				fseek (infile, -s+i, SEEK_CUR);
				return 0;
			}
			else {
				if (abort_early && buf[i+1] != JPEG_MARKER_APP0 &&
						buf[i+1] != JPEG_MARKER_APP1) {
					fseek (infile, -s+i, SEEK_CUR);
					return 0;
				}
				/* Uninteresting header, skip it */
				state = IL_JPEG_SKIP_BYTES;
				seek = iptc_get_short(buf+i+2,
						IPTC_BYTE_ORDER_MOTOROLA);
			}

			if (outfile)
				if (fwrite (buf + i, 1, 2, outfile) < 2)
					return -1;
			i += 2;
			break;
		}
	}
	return -1;
}

static int
iptc_jpeg_seek_to_end (FILE * infile, FILE * outfile)
{
	int s;
	unsigned char buf[256];

	if (!infile)
		return -1;
	if (!outfile)
		fseek (infile, 0, SEEK_END);

	while ((s = (int)fread (buf, 1, sizeof(buf), infile))) {
		if ((int)fwrite (buf, 1, s, outfile) < s)
			return -1;
	}

	if (ferror (infile))
		return -1;

	return 0;
}

/**
 * iptc_jpeg_read_ps3:
 * @infile: an open JPEG file with the current position set to the start of the file
 * @buf: an output buffer to store the Photoshop 3.0 data
 * @size: the size of the output buffer
 *
 * Scans the headers of a JPEG file looking for a "Photoshop 3.0" header, and if
 * found, stores the raw contents of this header in the @buf buffer.  The Photoshop
 * 3.0 header is a series of records found in the APP13 section of a JPEG file.
 * If a JPEG file contains IPTC metadata, the metadata is stored in one of the
 * Photoshop 3.0 records.
 *
 * Returns: the number of bytes stored on success, 0 if the PS3 header was
 * not found, or -1 if an error occurred.
 */
int
iptc_jpeg_read_ps3 (FILE * infile, unsigned char * ps3, unsigned int size)
{
	int s;

	if (!infile || !ps3)
		return -1;

	s = iptc_jpeg_seek_to_ps3 (infile, NULL, 0);
	if (s <= 0)
		return s;
	if (fseek (infile, 4, SEEK_CUR) < 0)
		return -1;

	if ((int)size < s)
		return -1;

	if ((int)fread (ps3, 1, s, infile) < s)
		return -1;

	return s;
}

/**
 * iptc_jpeg_ps3_find_iptc:
 * @ps3: the data of a Photoshop 3.0 header to search
 * @ps3_size: size in bytes of @ps3
 * @iptc_len: output parameter, the size in bytes of any found IPTC data
 *
 * Parses a "Photoshop 3.0" header in search of IPTC metadata.
 *
 * Returns: the offset in bytes from the start of @ps3 where a block
 * of IPTC metadata begins, 0 if no IPTC metadata was found, -1 on error.
 */
int iptc_jpeg_ps3_find_iptc (const unsigned char * ps3,
		unsigned int ps3_size, unsigned int * iptc_len)
{
	unsigned int i, s;
	unsigned short bim_type;
	unsigned int bim_size;

	if (!ps3 || ps3_size < 14 || !iptc_len)
		return -1;

	if (memcmp (ps3, JPEG_PS3_ID, 14))
		return -1;

	i = 14;
	while (i < ps3_size) {
		if ((ps3_size - i) < 7 || memcmp (ps3 + i, JPEG_BIM_ID, 4))
			return -1;
		i += 4;
		bim_type = iptc_get_short (ps3 + i, IPTC_BYTE_ORDER_MOTOROLA);
		i += 2;
		s = ps3[i] + 1;
		s += (s & 1);
		if ((ps3_size - i) < s + 4)
			return -1;
		i += s;
		bim_size = iptc_get_long (ps3 + i, IPTC_BYTE_ORDER_MOTOROLA);
		i += 4;
		if ((ps3_size - i) < bim_size)
			return -1;
		if (bim_type == JPEG_BIM_IPTC_TYPE) {
			*iptc_len = bim_size;
			return i;
		}
		bim_size += (bim_size & 1);
		i += bim_size;
	}
	return 0;
}

static int
iptc_jpeg_write_iptc_bim (unsigned char * buf, const unsigned char * iptc,
		unsigned int iptc_size)
{
	int j = 0;

	memcpy (buf + j, JPEG_BIM_ID, 4);
	j += 4;
	iptc_set_short (buf + j, IPTC_BYTE_ORDER_MOTOROLA, JPEG_BIM_IPTC_TYPE);
	j += 2;
	buf[j] = 0;
	buf[j+1] = 0;
	j += 2;
	iptc_set_long (buf + j, IPTC_BYTE_ORDER_MOTOROLA, iptc_size);
	j += 4;
	memcpy (buf + j, iptc, iptc_size);
	j += iptc_size;
	if (iptc_size & 1)
		buf[j++] = 0;

	return j;
}

/**
 * iptc_jpeg_ps3_save_iptc:
 * @ps3: a template PS3 header in which to insert the
 * new iptc data.  Any non-IPTC data in this template will be preserved
 * in the output Photoshop 3.0 header.  If NULL, a PS3 header will be
 * generated from scratch.
 * @ps3_size: size in bytes of @ps3
 * @iptc: the IPTC bytestream to be encapsulated in the PS3 output.  The IPTC
 * bytestream can be generated with iptc_data_save().  If
 * NULL, the output will still be a valid PS3 header, but without any
 * contained IPTC data.
 * @iptc_size: size in bytes of @iptc
 * @buf: output buffer for the generated PS3 header
 * @size: size in bytes of @buf
 *
 * Takes a Photoshop 3.0 header, @ps3, removes any existing IPTC data inside
 * that header, and inserts the new IPTC data from @iptc.  Any other non-IPTC
 * portions of @ps3 are left unmodified.  If @ps3 is NULL, a blank PS3 header
 * is created.  If @iptc is NULL, the output PS3 header will contain no IPTC
 * data, even if @ps3 originally contained some.
 *
 * Returns: the number of bytes written to @buf; -1 on error.
 */
int iptc_jpeg_ps3_save_iptc (const unsigned char * ps3, unsigned int ps3_size,
		const unsigned char * iptc, unsigned int iptc_size,
		unsigned char * buf, unsigned int size)
{
	unsigned int i, j, s;
	unsigned short bim_type;
	unsigned int bim_size;
	int wrote_iptc = 0;

	if (!buf)
		return -1;

	if (!ps3 || ps3_size == 0) {
		ps3 = (unsigned char *) JPEG_PS3_ID;
		ps3_size = 14;
	}
	if (!iptc || iptc_size == 0) {
		iptc = NULL;
		iptc_size = 0;
	}

	if (ps3 && ps3_size < 14)
		return -1;


	if (size < ps3_size + iptc_size + 13)
		return -1;

	if (memcmp (ps3, JPEG_PS3_ID, 14))
		return -1;

	memcpy (buf, ps3, 14);
	i = j = 14;
	while (i < ps3_size) {
		int start = i;
		if ((ps3_size - i) < 7 || memcmp (ps3 + i, JPEG_BIM_ID, 4))
			return -1;
		i += 4;
		bim_type = iptc_get_short (ps3 + i, IPTC_BYTE_ORDER_MOTOROLA);
		i += 2;
		s = ps3[i] + 1;
		s += (s & 1);
		if ((ps3_size - i) < s + 4)
			return -1;
		i += s;
		bim_size = iptc_get_long (ps3 + i, IPTC_BYTE_ORDER_MOTOROLA);
		i += 4;
		if ((ps3_size - i) < bim_size)
			return -1;
		bim_size += (bim_size & 1);
		i += bim_size;

		if (bim_type == JPEG_BIM_IPTC_TYPE && !wrote_iptc) {
			if (!iptc)
				continue;
			j += iptc_jpeg_write_iptc_bim (buf + j,
					iptc, iptc_size);
			wrote_iptc = 1;
		}
		else {
			memcpy (buf + j, ps3 + start, i - start);
			j += i - start;
		}
	}

	if (!wrote_iptc && iptc) {
		j += iptc_jpeg_write_iptc_bim (buf + j, iptc, iptc_size);
	}
	return j;
}

/**
 * iptc_jpeg_save_with_ps3:
 * @infile: the file stream from which the image data is copied
 * @outfile: the output file stream
 * @ps3: the Photoshop 3.0 header to add to the output file
 * @ps3_size: size in bytes of @ps3
 *
 * Takes an existing JPEG file, @infile, removes any existing Photoshop
 * 3.0 header from it, and adds a new PS3 header, writing the output
 * to @outfile.  @infile must be open for reading and is expected to point
 * to the beginning of the JPEG file, which should be different from @outfile,
 * which must be open for writing.  If @ps3 is NULL, the output will contain
 * no PS3 header.  PS3 headers reside in the APP13 section of the JPEG file,
 * which is created if necessary.  All other headers and data will be copied
 * directly from @infile without modification.
 *
 * Returns: 0 on success, -1 on error.  Note that even in error, some data
 * may have been written to @outfile, and its contents should be considered
 * undefined.
 */
int
iptc_jpeg_save_with_ps3 (FILE * infile, FILE * outfile,
		const unsigned char * ps3, unsigned int ps3_size)
{
	int s;

	if (!infile || !outfile)
		return -1;

	/* Copy infile to outfile until we encounter the previous PS3
	 * block, or the right place for the new PS3 block, whichever
	 * comes first. */
	s = iptc_jpeg_seek_to_ps3 (infile, outfile, 1);
	if (s < 0)
		return -1;

	/* Insert the new PS3 block */
	if (ps3) {
		unsigned char buf[4];
		buf[0] = JPEG_MARKER;
		buf[1] = JPEG_MARKER_APP13;
		iptc_set_short (buf+2, IPTC_BYTE_ORDER_MOTOROLA, ps3_size + 2);
		if (fwrite (buf, 1, 4, outfile) < 4)
			return -1;
		if (fwrite (ps3, 1, ps3_size, outfile) < ps3_size)
			return -1;
	}

	if (s > 0) {
		/* Skip over the old PS3 block if we've come upon it. */
		if (fseek (infile, 4 + s, SEEK_CUR) < 0)
			return -1;
	}
	else {
		/* Keep searching for the old PS3 block and skip over it
		 * when we find it. */
		s = iptc_jpeg_seek_to_ps3 (infile, outfile, 0);
		if (s < 0)
			return -1;
		if (s > 0) {
			if (fseek (infile, 4 + s, SEEK_CUR) < 0)
				return -1;
		}
	}

	/* Copy the remainder of the file */
	if (iptc_jpeg_seek_to_end (infile, outfile) < 0)
		return -1;

	return 0;
}



#if 0
static int
iptc_loader_jpeg_search (IptcLoader *ild, unsigned char *buf, unsigned int len)
{
	IptcJpegInfo * jpeg = &ild->type_info.jpeg;
	int i=0;
	while (len - i >= 18) { 
		switch (jpeg->state) {
		case IL_JPEG_SKIP_BYTES:
			if (jpeg->seek > (len - i)) {
				jpeg->seek -= len-i;
				i = len;
			}
			else {
				jpeg->state = jpeg->next_state;
				i += jpeg->seek;
			}
			break;
			
		case IL_JPEG_MARKER:
			if (buf[i] != 0xff)
				return -1;
			if (buf[i+1] == JPEG_MARKER_SOI) {
				i += 2;
			}
			else if (buf[i+1] == JPEG_MARKER_APP13 &&
					!memcmp(buf+i+4, JPEG_PS3_ID, 14)) {
				jpeg->state = IL_JPEG_IN_APP13;
				jpeg->app13_size = iptc_get_short(buf+i+2,
						IPTC_BYTE_ORDER_MOTOROLA);
				jpeg->app13_size -= 16;
				i += 18;
			}
			else if (buf[i+1] >= JPEG_MARKER_APP0 &&
					buf[i+1] <= JPEG_MARKER_APP15) {
				jpeg->state = IL_JPEG_SKIP_BYTES;
				jpeg->next_state = IL_JPEG_MARKER;
				jpeg->seek = iptc_get_short(buf+i+2,
						IPTC_BYTE_ORDER_MOTOROLA);
				i += 2;
			}
			else {
				ild->state = IL_FAILED;
				return i;
			}
			break;

		case IL_JPEG_IN_APP13:
			if (jpeg->app13_size < 14 ||
					memcmp(buf+i, JPEG_BIM_ID, 4)) {
				ild->state = IL_FAILED;
				return i;
			}
			jpeg->bim_type = iptc_get_short(buf+i+4,
					IPTC_BYTE_ORDER_MOTOROLA);
			jpeg->seek = buf[i+6] + 1;
			jpeg->seek += (jpeg->seek & 1);
			jpeg->state = IL_JPEG_SKIP_BYTES;
			jpeg->next_state = IL_JPEG_BIM_DATA;
			i += 6;
			jpeg->app13_size -= 6 + jpeg->seek;
			break;

		case IL_JPEG_BIM_DATA:
			jpeg->seek = iptc_get_long(buf+i,
					IPTC_BYTE_ORDER_MOTOROLA);
			i += 4;
			if (jpeg->bim_type == JPEG_BIM_IPTC_TYPE) {
				jpeg->iptc_location = IL_IPTC_APP13;
				ild->size = jpeg->seek;
				ild->state = IL_IPTC_FOUND;
				return i;
			}
			jpeg->seek += (jpeg->seek & 1);
			jpeg->state = IL_JPEG_SKIP_BYTES;
			jpeg->next_state = IL_JPEG_IN_APP13;
			jpeg->app13_size -= 4 + jpeg->seek;
			break;

		}
	}

	return i;
}
#endif

/*
	retval = -1;
	while ((s = fread (buf, 1, sizeof(buf), infile))) {
		int skip, done = 0, found = 0;
		if (s < 4)
			goto failed;
		if (buf[0] != JPEG_SECTION_MARKER)
			goto failed;

		switch (buf[1]) {
			case JPEG_MARKER_SOI:
				fseek (infile, -s + 2, SEEK_CUR);
				continue;
			case JPEG_MARKER_APP0:
			case JPEG_MARKER_APP1:
			case JPEG_MARKER_APP2:
			case JPEG_MARKER_APP3:
			case JPEG_MARKER_APP4:
			case JPEG_MARKER_APP5:
			case JPEG_MARKER_APP6:
			case JPEG_MARKER_APP7:
			case JPEG_MARKER_APP8:
			case JPEG_MARKER_APP9:
			case JPEG_MARKER_APP10:
			case JPEG_MARKER_APP11:
			case JPEG_MARKER_APP12:
			case JPEG_MARKER_APP14:
			case JPEG_MARKER_APP15:
				skip = iptc_get_short (buf+2,
						IPTC_BYTE_ORDER_MOTOROLA);
				fseek (infile, -s + 2 + skip, SEEK_CUR);
				continue;
			case JPEG_MARKER_APP13:
				skip = iptc_get_short (buf+2,
						IPTC_BYTE_ORDER_MOTOROLA);
				if (s < 18)
					goto failed;
				if (!memcmp(buf+4, JPEG_PS3_ID, 14)) {
					skip -= 2;
					fseek (infile, -s + 4, SEEK_CUR);
					break;
				}
				fseek (infile, -s + 2 + skip, SEEK_CUR);
				continue;
			default:
				retval = 0;
				goto failed;
		}

		if (size < skip)
			goto failed;
		bytes_read = 0;
		while (bytes_read < skip &&
				(s = fread (buf, 1, sizeof(buf), infile))) {
			int copy_size = MIN (s, skip - bytes_read);
			memcpy (ps3 + bytes_read, buf, copy_size);
			bytes_read += copy_size;
		}
		if (bytes_read == skip)
			retval = bytes_read;
		break;
	}
*/
