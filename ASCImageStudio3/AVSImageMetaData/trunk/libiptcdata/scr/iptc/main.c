#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <config.h>

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#else
#define getopt_long(a,b,c,d,e) getopt(a,b,c)
#endif

#include <locale.h>

#if defined(HAVE_ICONV_H) && defined(HAVE_WCHAR_H)
#include <iconv.h>
#include <wchar.h>
#include <langinfo.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>

#include "i18n.h"
#include <libiptcdata/iptc-data.h>
#include <libiptcdata/iptc-jpeg.h>

static char help_str[] = N_("\
Examples:\n\
  iptc image.jpg       # display the IPTC metadata contained in image.jpg\n\
  iptc -m Caption -v \"Foo\" *.jpg\n\
                       # set caption \"Foo\" in all jpegs of the curr. dir.\n\
  iptc -a Keywords -v \"vacation\" *.jpg\n\
  iptc -a 2:25 -v \"vacation\" *.jpg\n\
                       # either command adds keyword \"vacation\" to all jpegs\n\
  iptc -d Keywords:1 image.jpg\n\
                       # removes keyword number 1 (the 2nd) from image.jpg\n\
  iptc -d Keywords:all image.jpg\n\
                       # removes all keywords from image.jpg\n\
\n\
Operations:\n\
  -a, --add=TAG        add new tag with identifier TAG\n\
  -m, --modify=TAG     modify tag with identifier TAG (add if not present)\n\
  -v, --value=VALUE    value for added/modified tag\n\
  -d, --delete=TAG     delete tag with identifier TAG\n\
  -p, --print=TAG      print value of tag with identifier TAG\n\
      --add-version    add IPTC version number into the headers\n\
      --add-encoding   add text encoding specification (UTF-8) into headers\n\
\n\
Options:\n\
  -q, --quiet          produce less verbose output\n\
  -b, --backup         backup any modified files\n\
      --no-sort        do not sort tags before saving\n\
\n\
Informative output:\n\
  -l, --list           list the names of all known tags (i.e. Caption, etc.)\n\
  -L, --list-desc=TAG  print the name and description of TAG\n\
      --help           print this help, then exit\n\
      --version        print iptc program version number, then exit\n\
");

static void
print_help(char ** argv)
{
	printf("%s\n\n%s: %s [%s] [%s]...\n\n%s",
			_("Utility for viewing and modifying the contents of \
IPTC metadata in images"),
			_("Usage"), argv[0], _("OPTIONS"), _("FILE"),
			_(help_str));
}

static void
print_version()
{
	printf("iptc %s\n%s\n", VERSION,
			_("Written by David Moore <dcm@acm.org>"));
}

#if defined(HAVE_ICONV_H) && defined(HAVE_WCHAR_H)

static char *
locale_to_utf8 (char * str)
{
	size_t in_len = strlen (str);
	int out_size = strlen (str) * 4 + 1;
	size_t out_left = out_size;
	char * a;
	char * outstr;
	iconv_t ic;

	ic = iconv_open ("UTF-8", nl_langinfo (CODESET));
	if (ic == (iconv_t) -1)
		return strdup (str);

	outstr = malloc (out_size);
	if (!outstr)
		return NULL;
	
	a = (char *) outstr;
	iconv (ic, (ICONV_CONST char **)&str, &in_len, &a, &out_left);
	outstr[out_size - out_left] = '\0';
	iconv_close (ic);

	return outstr;
}

static char *
str_to_locale (char * str, char * charset, int * len)
{
	size_t in_len = strlen (str);
	unsigned int w_size = (in_len+1) * 4;
	size_t w_left = w_size;
	unsigned int w_len;
	unsigned int out_len;
	wchar_t * wstr;
	char * a;
	char * outstr;
	int i, j;
	iconv_t ic;
	mbstate_t ps;

	ic = iconv_open ("WCHAR_T", charset);
	if (ic == (iconv_t) -1)
		return strdup (str);

	wstr = malloc (w_size);
	if (!wstr)
		return NULL;
	
	a = (char *) wstr;
	iconv (ic, (ICONV_CONST char **)&str, &in_len, &a, &w_left);
	w_len = (w_size - w_left) / 4;
	iconv_close (ic);

	out_len = 2 * w_len + MB_CUR_MAX;
	outstr = malloc (out_len);

	memset (&ps, '\0', sizeof (ps));

	if (len) {
		if (*len < w_len)
			w_len = *len;
		*len = w_len;
	}
	j = 0;
	for (i = 0; i < w_len; i++) {
		int n;
		while (out_len - j < MB_CUR_MAX + 1) {
			out_len *= 2;
			outstr = realloc (outstr, out_len);
			if (!outstr)
				return NULL;
		}
		n = wcrtomb (outstr + j, wstr[i], &ps);
		if (n == -1) {
			outstr[j] = '?';
			n = 1;
		}
		j += n;
	}
	outstr[j] = '\0';

	free (wstr);
	return outstr;
}

#else /* defined(HAVE_ICONV_H) && defined(HAVE_WCHAR_H) */

static char *
locale_to_utf8 (char * str)
{
	return strdup (str);
}

static char *
str_to_locale (char * str, char * charset, int * len)
{
	int in_len = strlen (str);
	char * out_str;

	if (len) {
		if (*len < in_len)
			in_len = *len;
		*len = in_len;
	}

	out_str = strdup (str);
	out_str[in_len] = '\0';
	return out_str;
}

#endif

static int
print_tag_info (IptcRecord r, IptcTag t, int verbose)
{
	const char * name = iptc_tag_get_name (r, t);
	char * desc;
	if (!name)
		return -1;

	printf ("%2d:%03d %s\n", r, t, name);

	if (!verbose)
		return 0;

	desc = iptc_tag_get_description (r, t);
	if (desc) {
		char * convbuf;
		convbuf = str_to_locale (desc, "UTF-8", NULL);
		printf ("\n%s\n", convbuf);
		free (convbuf);
	}
	return 0;
}

static void
print_tag_list ()
{
	int r, t;

	printf("%6.6s %s\n", _("Tag"), _("Name"));
	printf(" ----- --------------------\n");

	for (r = 1; r <= 9; r++) {
		for (t = 0; t < 256; t++) {
			print_tag_info (r, t, 0);
		}
	}
}

static void
count_tag_types (IptcData * d, int * counts)
{
	int i, j;
	for (i = 0; i < d->count; i++) {
		IptcDataSet * ds = d->datasets[i];
		counts[i] = -1;
		for (j = i-1; j >= 0; j--) {
			IptcDataSet * ds2 = d->datasets[j];
			if (ds->record == ds2->record && ds->tag == ds2->tag) {
				if (counts[j] == -1)
					counts[j] = 0;
				counts[i] = counts[j]+1;
				break;
			}
		}
	}
}

static void
print_iptc_data (IptcData * d)
{
	int i;
	char * charset;
	int counts[d->count];

	if (d->count) {
		printf(" %-8.8s %-20.20s %-9.9s %4s  %s\n", _("Tag"), _("Name"),
				_("Type"), _("Size"), _("Value"));
		printf(" -------- -------------------- --------- ----  -----\n");
	}
	
	if (iptc_data_get_encoding (d) == IPTC_ENCODING_UTF8) {
		charset = "UTF-8";
	}
	else {
		/* technically this violates the IPTC IIM spec, but most
		 * other applications are broken. */
		charset = "ISO-8859-1";
	}

	count_tag_types (d, counts);

	for (i=0; i < d->count; i++) {
		IptcDataSet * e = d->datasets[i];
		unsigned char * buf;
		char * convbuf;
		int len;

		printf("%2d:%03d", e->record, e->tag);
		if (counts[i] >= 0)
			printf(":%02d ", counts[i]);
		else
			printf("    ");

		len = 20;
		convbuf = str_to_locale (iptc_tag_get_title (e->record, e->tag),
				"UTF-8", &len);
		printf("%s%*s ", convbuf, 20 - len, "");
		free (convbuf);
		len = 9;
		convbuf = str_to_locale (iptc_format_get_name (iptc_dataset_get_format (e)),
				"UTF-8", &len);
		printf("%s%*s ", convbuf, 9 - len, "");
		free (convbuf);
		printf("%4d  ", e->size);

		switch (iptc_dataset_get_format (e)) {
			case IPTC_FORMAT_BYTE:
			case IPTC_FORMAT_SHORT:
			case IPTC_FORMAT_LONG:
				printf("%d\n", iptc_dataset_get_value (e));
				break;
			case IPTC_FORMAT_BINARY:
                                buf = malloc (3 * e->size + 1);
				iptc_dataset_get_as_str (e, (char *)buf, 3*e->size+1);
				printf("%s\n", buf);
                                free (buf);
				break;
			default:
                                buf = malloc (e->size + 1);
				iptc_dataset_get_data (e, buf, e->size+1);
				convbuf = str_to_locale ((char *)buf, charset, NULL);
                                free (buf);
				printf("%s\n", convbuf);
                                free (convbuf);
				break;
		}
	}
}

typedef enum {
	OP_ADD,
	OP_DELETE,
	OP_PRINT,
	OP_MODIFY
} OpType;

typedef struct _Operation {
	OpType          op;
	IptcRecord      record;
	IptcTag         tag;
	int             num;
	IptcDataSet    *ds;
} Operation;

typedef struct _OpList {
	Operation      *ops;
	int             count;
} OpList;

static void
new_operation (OpList * list, OpType op, IptcRecord record,
		IptcTag tag, int num, IptcDataSet * ds)
{
	Operation * newop;
	if (!list)
		return;

	list->count++;
	list->ops = realloc (list->ops, list->count * sizeof(Operation));
	if (!list->ops)
		return;

	newop = list->ops + list->count - 1;
	newop->op = op;
	newop->record = record;
	newop->tag = tag;
	newop->num = num;
	newop->ds = ds;
}

static int
perform_one_op (IptcData * d, Operation * op, IptcDataSet * ds, char * filename)
{
	if (op->op == OP_ADD || op->op == OP_MODIFY) {
		IptcDataSet * ds_add;
		ds_add = iptc_dataset_copy (op->ds);
		if (ds)
			iptc_data_add_dataset_before (d, ds, ds_add);
		else {
			iptc_data_add_dataset (d, ds_add);
			if (op->op == OP_MODIFY) {
				fprintf (stderr, _("%s: Could not find dataset %d:%d, adding it\n"), filename, op->record, op->tag);
			}
		}
		iptc_dataset_unref (ds_add);
	}
	if (op->op == OP_DELETE || op->op == OP_MODIFY) {
		if (!ds && op->op == OP_DELETE) {
			fprintf(stderr, _("%s: Could not find dataset %d:%d"), filename, op->record, op->tag);
			if (op->num > 0)
				fprintf (stderr, ":%d", op->num);
			fprintf (stderr, "\n");
			return -1;
		}
		if (ds)
			iptc_data_remove_dataset (d, ds);
	}
	if (op->op == OP_PRINT) {
		if (!ds) {
			fprintf(stderr, _("%s: Could not find dataset %d:%d"), filename, op->record, op->tag);
			if (op->num > 0)
				fprintf (stderr, ":%d", op->num);
			fprintf (stderr, "\n");
			return -1;
		}
		fwrite (ds->data, 1, ds->size, stdout);
		printf ("\n");
	}
	return 0;
}

static int
perform_operations (IptcData * d, OpList * list, char * filename)
{
	int i, j, ret = -1;

	if (!d || !list || list->count == 0)
		return 0;

	for (i = 0; i < list->count; i++) {
		Operation * op = list->ops + i;
		IptcDataSet * ds = NULL;

		if (op->record && op->num == -1) {
			ds = iptc_data_get_dataset (d, op->record, op->tag);
			if (!ds) {
				fprintf(stderr, _("%s: Could not find dataset %d:%d\n"), filename, op->record, op->tag);
			}
			while (ds) {
				IptcDataSet * next_ds;
				next_ds = iptc_data_get_next_dataset (d, ds,
						op->record, op->tag);
				if (perform_one_op (d, op, ds, filename) == 0)
					ret = 0;
				iptc_dataset_unref (ds);
				ds = next_ds;
			}
		}
		else {
			if (op->record) {
				ds = iptc_data_get_dataset (d, op->record, op->tag);
				for (j = 0; j < op->num && ds; j++) {
					if (!ds)
						break;
					iptc_dataset_unref (ds);
					ds = iptc_data_get_next_dataset (d, ds,
							op->record, op->tag);
				}
			}

			if (perform_one_op (d, op, ds, filename) == 0)
				ret = 0;
			if (ds)
				iptc_dataset_unref (ds);
		}
	}

	return ret;
}

static void
free_operations (OpList * list)
{
	int i;

	if (!list)
		return;

	for (i = 0; i < list->count; i++) {
		Operation * op = list->ops + i;
		if (op->op == OP_ADD) {
			iptc_dataset_unref (op->ds);
		}
	}
	if (list->ops)
		free (list->ops);
	list->count = 0;
}

static int
parse_tag_id (char * str, IptcRecord *r, IptcTag *t, int *num)
{
	*num = 0;
	if (isdigit (str[0])) {
		char * a;
		*r = strtoul (str, &a, 10);
		if (a[0] != ':' || !isdigit (a[1]))
			return -1;
		*t = strtoul (a + 1, &a, 10);
		if (*r < 1 || *r > 9 || *t < 0 || *t > 255)
			return -1;
		if (a[0] == '\0')
			return 0;
		if (a[0] != ':')
			return -1;
		if (!strcmp (a + 1, "all"))
			*num = -1;
		else if (isdigit (a[1]))
			*num = strtoul (a + 1, NULL, 10);
		else
			return -1;
	}
	else {
		char * name = strdup (str);
		char * a;
		if ((a = strchr (name, ':'))) {
			if (!strcmp (a+1, "all"))
				*num = -1;
			else if (isdigit (a[1]))
				*num = strtoul (a + 1, NULL, 10);
			else {
				free (name);
				return -1;
			}
			*a = '\0';
		}
		if (iptc_tag_find_by_name (name, r, t) < 0) {
			free (name);
			return -1;
		}
		free (name);
	}
	return 0;
}

int
main (int argc, char ** argv)
{
	IptcDataSet * ds = NULL;
	int i;
	IptcRecord record;
	IptcTag tag;
	int tagnum;
	const IptcTagInfo * tag_info;
	unsigned char * buf;
	unsigned char * outbuf;
	int buflen = 256 * 256;
	IptcFormat format;
	char c;
	int modified = 0;
	int add_encoding = 0;
	int add_version = 0;
	int is_quiet = 0;
	int do_backup = 0;
	int no_sort = 0;
	int add_tag = 0;
	int modify_tag = 0;
	OpList oplist = { 0, 0 };
	int retval = 1;

#ifdef HAVE_GETOPT_H
	struct option longopts[] = {
		{ "quiet", no_argument, NULL, 'q' },
		{ "backup", no_argument, NULL, 'b' },
		{ "no-sort", no_argument, NULL, 's' },
		{ "list", no_argument, NULL, 'l' },
		{ "list-desc", required_argument, NULL, 'L' },
		{ "add", required_argument, NULL, 'a' },
		{ "modify", required_argument, NULL, 'm' },
		{ "delete", required_argument, NULL, 'd' },
		{ "print", required_argument, NULL, 'p' },
		{ "value", required_argument, NULL, 'v' },
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'V' },
		{ "add-version", no_argument, NULL, 'A' },
		{ "add-encoding", no_argument, NULL, 'E' },
		{ 0, 0, 0, 0 }
	};
#endif

	setlocale (LC_ALL, "");
	textdomain (IPTC_GETTEXT_PACKAGE);
	bindtextdomain (IPTC_GETTEXT_PACKAGE, IPTC_LOCALEDIR);

	while ((c = getopt_long (argc, argv, "qblL:a:m:d:p:v:", longopts, NULL)) >= 0) {
		char * convbuf;
		switch (c) {
			case 'q':
				is_quiet = 1;
				break;
			case 'b':
				do_backup = 1;
				break;
			case 's':
				no_sort = 1;
				break;
			case 'l':
				print_tag_list ();
				return 0;
			case 'L':
				if (parse_tag_id (optarg, &record, &tag, &tagnum) < 0) {
					fprintf(stderr, _("\"%s\" is not a known tag\n"), optarg);
					return 1;
				}
				if (print_tag_info (record, tag, 1) < 0) {
					fprintf(stderr, _("No information about tag\n"));
				}
				return 0;
			case 'A':
				add_version = 1;
				modified = 1;
				break;
			case 'E':
				add_encoding = 1;
				modified = 1;
				break;
			case 'a':
			case 'm':
			case 'd':
			case 'p':
				if (add_tag || modify_tag) {
					fprintf(stderr, _("Must specify value for add/modify operation\n"));
					return 1;
				}
				if (parse_tag_id (optarg, &record, &tag, &tagnum) < 0) {
					fprintf(stderr, _("\"%s\" is not a known tag\n"), optarg);
					return 1;
				}
				if (c == 'a') {
					add_tag = 1;
					modified = 1;
				}
				else if (c == 'm') {
					modify_tag = 1;
					modified = 1;
				}
				else if (c == 'd') {
					new_operation (&oplist, OP_DELETE,
							record, tag, tagnum, NULL);
					modified = 1;
				}
				else if (c == 'p') {
					new_operation (&oplist, OP_PRINT,
							record, tag, tagnum, NULL);
					is_quiet = 1;
				}

				break;

			case 'v':
				if (!add_tag && !modify_tag) {
					fprintf(stderr, _("Must specify tag to add or modify\n"));
					return 1;
				}
				if (add_tag && modify_tag) {
					fprintf(stderr, _("Must specify value for add/modify operation\n"));
					return 1;
				}
				tag_info = iptc_tag_get_info (record, tag);
				if (!tag_info)
					format = IPTC_FORMAT_UNKNOWN;
				else
					format = tag_info->format;
				ds = iptc_dataset_new ();
				iptc_dataset_set_tag (ds, record, tag);
				switch (format) {
				case IPTC_FORMAT_BYTE:
				case IPTC_FORMAT_SHORT:
				case IPTC_FORMAT_LONG:
					if (!isdigit (*optarg)) {
						fprintf(stderr, _("Value must be an integer\n"));
						iptc_dataset_unref (ds);
						return 1;
					}
					iptc_dataset_set_value (ds,
							strtoul (optarg, NULL, 10),
							IPTC_DONT_VALIDATE);
					break;
				case IPTC_FORMAT_STRING:
					convbuf = locale_to_utf8 (optarg);
					iptc_dataset_set_data (ds, (unsigned char *) convbuf,
							strlen (convbuf),
							IPTC_DONT_VALIDATE);
					free (convbuf);
					break;
				default:
					iptc_dataset_set_data (ds, (unsigned char *) optarg,
							strlen (optarg),
							IPTC_DONT_VALIDATE);
					break;
				}
				if (add_tag) {
					new_operation (&oplist, OP_ADD,
							0, 0, 0, ds);
					add_tag = 0;
				}
				if (modify_tag) {
					new_operation (&oplist, OP_MODIFY,
							record, tag, tagnum, ds);
					modify_tag = 0;
				}
				break;

			case 'h':
				print_help(argv);
				return 0;

			case 'V':
				print_version();
				return 0;

			default:
				print_help(argv);
				return 1;
		}
	}
	if (add_tag || modify_tag) {
		fprintf(stderr, _("Error: Must specify value for add/modify operation\n"));
		print_help (argv);
		return 1;
	}

	if (argc < optind + 1) {
		fprintf(stderr, _("Error: Must specify a file\n"));
		print_help (argv);
		return 1;
	}

	buf = malloc (buflen);
	outbuf = malloc (buflen);

	for (i = optind; i < argc; i++) {
		char * filename = argv[i];
		FILE * infile, * outfile;
		IptcData * d = NULL;
		int ps3_len, iptc_off;
		unsigned int iptc_len;

		infile = fopen(filename, "r");
		if (!infile) {
			fprintf(stderr, _("Error opening %s\n"), filename);
			continue;
		}

		ps3_len = iptc_jpeg_read_ps3 (infile, buf, buflen);
		fclose (infile);
		if (ps3_len < 0) {
			fprintf(stderr, _("Error parsing %s\n"), filename);
			continue;
		}

		if (ps3_len) {
			iptc_off = iptc_jpeg_ps3_find_iptc (buf, ps3_len, &iptc_len);
			if (iptc_off < 0) {
				fprintf(stderr, _("Error parsing headers of %s\n"), filename);
				continue;
			}
			if (iptc_off)
				d = iptc_data_new_from_data (buf + iptc_off, iptc_len);
		}

		if (modified && !d)
			d = iptc_data_new ();

		if (perform_operations (d, &oplist, filename) < 0) {
			iptc_data_unref (d);
			if (!is_quiet)
				fprintf(stderr, _("%s: no changes to save\n"), filename);
			continue;
		}

		if (add_version)
			iptc_data_set_version (d, IPTC_IIM_VERSION);
		
		/* Make sure we specify the text encoding for the data */
		if (add_encoding) {
			IptcEncoding enc = iptc_data_get_encoding (d);
			if (enc == IPTC_ENCODING_UNSPECIFIED) {
				iptc_data_set_encoding_utf8 (d);
			}
			else if (enc != IPTC_ENCODING_UTF8) {
				fprintf (stderr, "%s:\n", filename);
				fprintf (stderr, _("Warning: Strings encoded in UTF-8 "
					"have been added to the IPTC data, but\n"
					"pre-existing data may have been encoded "
					"with a different character set.\n"));
			}
		}

		if (modified && !no_sort)
			iptc_data_sort (d);

		if (!is_quiet && ((optind+1) == argc || !modified)) {
			if (d) {
				printf ("%s:\n", filename);
				print_iptc_data (d);
				if (i + 1 < argc)
					printf ("\n");
			}
			else {
				printf ("%s: %s\n", filename, _("No IPTC data found"));
			}
		}


		if (modified) {
			unsigned char * iptc_buf = NULL;
			char tmpfile[strlen(filename)+8];
			char bakfile[strlen(filename)+8];
			int v;
			
			if (iptc_data_save (d, &iptc_buf, &iptc_len) < 0) {
				fprintf(stderr, "%s: %s\n", filename, _("Failed to generate IPTC bytestream"));
				iptc_data_unref (d);
				
				continue;
			}
			ps3_len = iptc_jpeg_ps3_save_iptc (buf, ps3_len,
					iptc_buf, iptc_len, outbuf, buflen);
			iptc_data_free_buf (d, iptc_buf);
			if (ps3_len < 0) {
				fprintf(stderr, "%s: %s\n", filename, _("Failed to generate PS3 header"));
				iptc_data_unref (d);
				continue;
			}

			infile = fopen (filename, "r");
			if (!infile) {
				fprintf(stderr, "%s: %s\n", filename, _("Failed to reopen file"));
				iptc_data_unref (d);
				continue;
			}
			sprintf(tmpfile, "%s.%d", filename, getpid());
			outfile = fopen (tmpfile, "w");
			if (!outfile) {
				fprintf(stderr, "%s: %s\n", filename, _("Can't open temporary file for writing"));
				fclose (infile);
				iptc_data_unref (d);
				continue;
			}
			
			v = iptc_jpeg_save_with_ps3 (infile, outfile, outbuf, ps3_len);
			fclose (infile);
			fclose (outfile);

			if (v >= 0) {
				struct stat statinfo;
				if (do_backup) {
					sprintf (bakfile, "%s~", filename);
					unlink (bakfile);
					if (link (filename, bakfile) < 0) {
						fprintf (stderr, "%s: %s\n", filename, _("Failed to create backup file, aborting"));
						unlink (tmpfile);
						iptc_data_unref (d);
						continue;
					}
				}
				stat (filename, &statinfo);
				if (rename (tmpfile, filename) < 0) {
					fprintf(stderr, "%s: %s\n", filename, _("Failed to save image"));
					unlink (tmpfile);
					iptc_data_unref (d);
					continue;
				}
				else {
					chown (filename, -1, statinfo.st_gid);
					chmod (filename, statinfo.st_mode);
				}
				if (!is_quiet)
					fprintf(stderr, _("%s: saved\n"), filename);
			}
			else {
				unlink (tmpfile);
				fprintf(stderr, "%s: %s\n", filename, _("Failed to save image"));
			}
		}
		
		if (d)
			iptc_data_unref(d);

		retval = 0;
	}

	free (buf);
	free (outbuf);
	free_operations (&oplist);

	return retval;
}
