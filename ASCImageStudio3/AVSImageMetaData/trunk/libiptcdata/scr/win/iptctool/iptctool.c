/* TODO: this is a stripped down iptc utility with display only */
/* TODO: there is no getopt on Windows */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <libiptcdata/iptc-data.h>
#include <libiptcdata/iptc-jpeg.h>


unsigned char buf[256*256];
unsigned char outbuf[256*256];

static void
print_usage(char ** argv)
{
	printf("\nUsage: %s FILE\n\n"
			"Tool for showing the contents of IPTC metadata in JPEG images\n",
			argv[0]);
}


static void
print_iptc_data (IptcData * d)
{
	unsigned int i;

	if (d->count) {
		printf("%6.6s %-20.20s %-9.9s %6s  %s\n", "Tag", "Name", "Type",
				"Size", "Value");
		printf(" ----- -------------------- --------- ------  -----\n");
	}
	
	for (i=0; i < d->count; i++) {
		IptcDataSet * e = d->datasets[i];
		char buf[256];

		printf("%2d:%03d %-20.20s %-9.9s %6d  ",
				e->record, e->tag,
				iptc_tag_get_title (e->record, e->tag),
				iptc_format_get_name (iptc_dataset_get_format (e)),
				e->size);
		switch (iptc_dataset_get_format (e)) {
			case IPTC_FORMAT_BYTE:
			case IPTC_FORMAT_SHORT:
			case IPTC_FORMAT_LONG:
				printf("%d\n", iptc_dataset_get_value (e));
				break;
			case IPTC_FORMAT_BINARY:
				iptc_dataset_get_as_str (e, buf, sizeof(buf));
				printf("%s\n", buf);
				break;
			default:
				iptc_dataset_get_data (e, buf, sizeof(buf));
				printf("%s\n", buf);
				break;
		}
	}
}





int
main (int argc, char ** argv)
{
	FILE * infile;
	IptcData * d = NULL;
	IptcDataSet * ds = NULL;
	int ps3_len, iptc_len, iptc_off;

	if (argc < 2) {
		print_usage(argv);
		return 1;
	}

	infile = fopen(argv[1], "rb");
	if (!infile) {
		fprintf(stderr, "Error opening %s\n", argv[1]);
		return 1;
	}

	ps3_len = iptc_jpeg_read_ps3 (infile, buf, sizeof(buf));
	fclose (infile);
	if (ps3_len < 0) {
		fprintf(stderr, "Error: iptc_jpeg_read_ps3: %d\n", ps3_len);
		return 1;
	}

	if (ps3_len) {
		iptc_off = iptc_jpeg_ps3_find_iptc (buf, ps3_len, &iptc_len);
		if (iptc_off < 0) {
			fprintf(stderr, "Error: iptc_jpeg_ps3_find_iptc: %d\n", iptc_off);
			return 2;
		}
		if (iptc_off)
			d = iptc_data_new_from_data (buf + iptc_off, iptc_len);
	}

	if (d)
		print_iptc_data (d);
	else
		printf("No IPTC data found\n");
	
	if (d)
		iptc_data_unref(d);

	return 0;
}
