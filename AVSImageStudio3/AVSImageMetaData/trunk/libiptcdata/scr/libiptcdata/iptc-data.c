/* iptc-data.c
 *
 * Copyright © 2005 David Moore <dcm@acm.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"
#include "iptc-data.h"
#include "iptc-jpeg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct _IptcDataPrivate
{
	unsigned int ref_count;

	IptcLog *log;
	IptcMem *mem;
};

#define IPTC_TAG_MARKER		0x1c

/**
 * iptc_data_new:
 *
 * Allocates a new collection of datasets, which is initialized to be
 * empty.  The default memory allocation functions (malloc, etc.) are
 * used.  If you need custom memory management functions, use
 * iptc_data_new_mem() instead.  This allocation will set the #IptcData
 * refcount to 1, so use iptc_data_unref() when finished with the pointer.
 *
 * Returns: pointer to the new #IptcData object, NULL on error
 */
IptcData *
iptc_data_new (void)
{
	IptcMem *mem = iptc_mem_new_default ();
	IptcData *data = iptc_data_new_mem (mem);

	iptc_mem_unref (mem);

	return data;
}

/**
 * iptc_data_new_mem:
 * @mem: Pointer to an #IptcMem object that defines custom memory managment
 * functions.  The refcount of @mem will be incremented.  It is decremented
 * when the returned #IptcData object is freed.
 *
 * Allocates a new collection of datasets, which is initialized to be
 * empty, using custom memory management functions.  This allocation
 * will set the #IptcData refcount to 1, so use iptc_data_unref() when
 * finished with the object.
 *
 * Returns: pointer to the new #IptcData object, NULL on error
 */
IptcData *
iptc_data_new_mem (IptcMem *mem)
{
	IptcData *data;

	if (!mem) return NULL;

	data = iptc_mem_alloc (mem, (IptcLong) sizeof (IptcData));
	if (!data)
		return NULL;
	data->priv = iptc_mem_alloc (mem,
				(IptcLong) sizeof (IptcDataPrivate));
	if (!data->priv) {
		iptc_mem_free (mem, data);
		return NULL;
	}

	data->priv->ref_count = 1;

	data->priv->mem = mem;
	iptc_mem_ref (mem);

	return data;
}

/**
 * iptc_data_new_from_data:
 * @buf: the buffer of IPTC data to be decoded
 * @size: the length to be read in bytes
 *
 * Allocates a new collection of datasets which is initialized by decoding
 * the contents of @buf.  This allocation will set the #IptcData
 * refcount to 1, so use iptc_data_unref() when finished with the object.
 * This is simply a convenience function that can be duplicated by calling
 * iptc_data_new() followed by iptc_data_load().
 *
 * Returns: pointer to the new #IptcData object.  NULL on error (including
 * parsing errors in the contents of @buf).
 */
IptcData *
iptc_data_new_from_data (const unsigned char *buf, unsigned int size)
{
	IptcData *idata;

	idata = iptc_data_new ();
	iptc_data_load (idata, buf, size);
	return (idata);
}

static int
iptc_data_load_dataset (IptcData *data, IptcDataSet *dataset,
			   const unsigned char *d,
			   unsigned int size)
{
	unsigned int doff, count;

	if (d[0] != IPTC_TAG_MARKER)
		return -1;

	if (size < 5)
		return -1;

	iptc_dataset_set_tag (dataset, d[1], d[2]);
	count = iptc_get_short (d + 3, IPTC_BYTE_ORDER_MOTOROLA);

	iptc_log (data->priv->log, IPTC_LOG_CODE_DEBUG, "IptcData",
		  "Loading dataset %d:%d ('%s')...", dataset->record, dataset->tag,
		  iptc_tag_get_name (dataset->record, dataset->tag));

	doff = 5;
	if (count & (1 << 15)) {
		unsigned int i;
		dataset->size = 0;
		count &= ~(1 << 15);
		if (size < count + doff)
			return -1;
		for (i=0; i < count; i++) {
			dataset->size = (dataset->size << 8) | d[doff+i];
		}
		doff += count;
		if (count > 4) {
			iptc_log (data->priv->log, IPTC_LOG_CODE_CORRUPT_DATA,
				"iptcData", "Field length has size %d bytes",
				count);
		}
	}
	else {
		dataset->size = count;
	}

	if (size < doff + dataset->size)
		return -1;

	iptc_dataset_set_data (dataset, d+doff, dataset->size, IPTC_DONT_VALIDATE);

	return doff + dataset->size;
}

static int
iptc_data_save_dataset (IptcData *data, IptcDataSet *e,
			   unsigned char **d, unsigned int *ds)
{
	unsigned int doff, s;
	unsigned char * buf;

	if (!data || !data->priv) return -1;

	if (e->size >= (1 << 15))
		doff = 9;
	else
		doff = 5;
	s = doff + e->size;

	*d = iptc_mem_realloc (data->priv->mem, *d, *ds + s);
	if (!*d) {
		IPTC_LOG_NO_MEMORY (data->priv->log, "IptcData", *ds);
		return -1;
	}
	buf = *d + *ds;
	*ds += s;

	buf[0] = IPTC_TAG_MARKER;
	buf[1] = e->record;
	buf[2] = e->tag;
	if (e->size >= (1 << 15)) {
		iptc_set_short (buf+3, IPTC_BYTE_ORDER_MOTOROLA, (1 << 15) | 4);
		iptc_set_long (buf+5, IPTC_BYTE_ORDER_MOTOROLA, e->size);
	}
	else {
		iptc_set_short (buf+3, IPTC_BYTE_ORDER_MOTOROLA, e->size);
	}
	
	/* Write the data. Fill unneeded bytes with 0. */
	memcpy (buf + doff, e->data, e->size);
	return 0;
}

/**
 * iptc_data_load:
 * @data: object to be populated with the loaded datasets
 * @buf: data buffer to be parsed, containing IPTC data
 * @size: length of data buffer to be parsed
 *
 * Parses a buffer containing raw IPTC data and adds the datasets
 * to the #IptcData object @data.
 *
 * Returns: 0 on success, -1 on failure.  Note that in the failure
 * case, some datasets may still have been added to @data.
 */
int
iptc_data_load (IptcData *data, const unsigned char *buf,
		     unsigned int size)
{
	if (!data || !data->priv || !buf || !size) return -1;

	iptc_log (data->priv->log, IPTC_LOG_CODE_DEBUG, "IptcData",
		  "Parsing %i byte(s) IPTC data...\n", size);

	while (size > 0 && buf[0] == IPTC_TAG_MARKER) {
		IptcDataSet *dataset;
		int s;

		dataset = iptc_dataset_new_mem (data->priv->mem);
		if (!dataset)
			return -1;
		if (iptc_data_add_dataset (data, dataset) < 0) {
			iptc_dataset_unref (dataset);
			return -1;
		}
		s = iptc_data_load_dataset (data, dataset, buf, size);
		if (s < 0) {
			iptc_data_remove_dataset (data, dataset);
			iptc_dataset_unref (dataset);
			return -1;
		}
		buf += s;
		size -= s;
		iptc_dataset_unref (dataset);
	}

	return 0;
}

/**
 * iptc_data_save:
 * @data: collection of datasets to be saved
 * @buf: pointer to a pointer that will hold the address of the output buffer
 * @size: pointer to an integer that will hold the length of the output buffer
 *
 * Outputs a collection of datasets as an IPTC bytestream.  No memory allocation
 * is required in advance.  @buf should point to a NULL pointer that will be set
 * to the address of the output buffer by this function.  @size will contain
 * this buffer's length after completion.  The object @data is unmodified by this
 * function.  The application should free the output buffer using
 * iptc_data_free_buf() when it is no longer needed.
 *
 * Returns: 0 on success, -1 on failure.  In the failure case, @buf should still
 * be checked for a non-NULL value, and freed using iptc_data_free_buf() if
 * necessary.
 */
int
iptc_data_save (IptcData *data, unsigned char **buf, unsigned int *size)
{
	unsigned int j;

	if (!data || !buf || !size)
		return -1;

	*size = 0;
	*buf = NULL;

	/* Save each dataset */
	iptc_log (data->priv->log, IPTC_LOG_CODE_DEBUG, "IptcData",
		  "Saving %i datasets...", data->count);
	for (j = 0; j < data->count; j++)
		if (iptc_data_save_dataset (data, data->datasets[j], buf, size) < 0)
			return -1;

	iptc_log (data->priv->log, IPTC_LOG_CODE_DEBUG, "IptcData",
		  "Saved %i byte(s) IPTC data.", *size);

	return 0;
}

/**
 * iptc_data_new_from_jpeg:
 * @path: filesystem path of the jpeg file to be read
 *
 * Allocates a new collection of datasets which is initialized by decoding
 * the IPTC data in JPEG file @path.  This allocation will set the #IptcData
 * refcount to 1, so use iptc_data_unref() when finished with the object.
 * This is a convenience function that reads the contents of the file,
 * creates a new #IptcData object, parses the file with
 * iptc_jpeg_read_ps3() and iptc_jpeg_ps3_find_iptc(), and loads the
 * data with iptc_data_load().  If more fine-grained error detection
 * is needed, those functions should be used individually.
 *
 * Returns: pointer to the new #IptcData object.  NULL on error (including
 * parsing errors or if the file did not include IPTC data).
 */
IptcData *
iptc_data_new_from_jpeg (const char *path)
{
	IptcData *d;
	FILE * infile;
	unsigned char * buf;
	int buf_len = 256*256;
	int len, offset;
	unsigned int iptc_len;

	errno_t errNo = fopen_s (&infile, path, "rb");
	if ((0!= errNo)|| (!infile))
		return NULL;

	d = iptc_data_new ();
	if (!d) {
		fclose (infile);
		return NULL;
	}

	buf = iptc_mem_alloc (d->priv->mem, buf_len);
	if (!buf) {
		fclose (infile);
		iptc_data_unref (d);
		return NULL;
	}

	len = iptc_jpeg_read_ps3 (infile, buf, buf_len);
	fclose (infile);

	if (len <= 0) {
		goto failure;
	}

	offset = iptc_jpeg_ps3_find_iptc (buf, len, &iptc_len);
	if (offset <= 0) {
		goto failure;
	}

	iptc_data_load (d, buf + offset, iptc_len);

	iptc_mem_free (d->priv->mem, buf);
	return d;

failure:
	iptc_mem_free (d->priv->mem, buf);
	iptc_data_unref (d);
	return NULL;
}

/**
 * iptc_data_ref:
 * @data: the referenced pointer
 *
 * Increments the reference count of an #IptcData object.  This function
 * should be called whenever a copy of a pointer is made by the application.
 * iptc_data_unref() can then be used when the pointer is no longer needed
 * to ensure that the object is freed once the object is completely unused.
 */
void
iptc_data_ref (IptcData *data)
{
	data->priv->ref_count++;
}

/**
 * iptc_data_unref:
 * @data: the unreferenced pointer
 *
 * Decrements the reference count of an #IptcData object.  The object will
 * automatically be freed when the count reaches 0.  This function should
 * be called whenever a pointer is no longer needed by an application.  It
 * is also good practice to set the local copy of the pointer to NULL to
 * shield against accidently reusing the value.
 */
void
iptc_data_unref (IptcData *data)
{
	data->priv->ref_count--;
	if (!data->priv->ref_count)
		iptc_data_free (data);
}

/**
 * iptc_data_free:
 * @data: the object to free
 *
 * Frees an #IptcData object.  This function should be used only for error
 * handling since iptc_data_unref() provides a safer mechanism for freeing
 * that allows multiple components to have access to an object.  This
 * function decrements the reference count of any datasets contained by the
 * object.  This will generally cause them to be freed as well unless other
 * parts of the application are referencing them individually.
 */
void
iptc_data_free (IptcData *data)
{
	unsigned int i;

	if (!data) return;

	for (i = 0; i < data->count; i++)
		iptc_dataset_unref (data->datasets[i]);
	if (data->priv) {
		IptcMem *mem = data->priv->mem;
		iptc_mem_free (mem, data->datasets);
		iptc_mem_free (mem, data->priv);
		iptc_mem_free (mem, data);
		iptc_mem_unref (mem);
	}
}

/**
 * iptc_data_free_buf:
 * @data: the #IptcData object that allocated the buffer
 * @buf: the buffer to free
 *
 * Frees a temporary buffer created from an #IptcData object by the
 * iptc_data_save() function.
 */
void
iptc_data_free_buf (IptcData *data, unsigned char *buf)
{
	if (!data || !data->priv || !data->priv->mem || !buf)
		return;

	iptc_mem_free (data->priv->mem, buf);
}

/**
 * iptc_data_dump:
 * @data: collection of datasets to print
 * @indent: indentation level of the printout
 *
 * A debugging aid that prints out the contents of a collection
 * of datasets to standard output.
 */
void
iptc_data_dump (IptcData *data, unsigned int indent)
{
	char buf[1024];
	unsigned int i;

	for (i = 0; i < 2 * indent; i++)
		buf[i] = ' ';
	buf[i] = '\0';

	if (!data)
		return;

	printf ("%sDumping iptc data (%i datasets)...\n", buf,
		data->count);
	for (i = 0; i < data->count; i++)
		iptc_dataset_dump (data->datasets[i], indent + 1);
}

static int
iptc_data_dataset_index (IptcData *data, IptcDataSet *ds)
{
	unsigned int i;

	if (!data || !ds)
		return -1;

	/* Search the dataset */
	for (i = 0; i < data->count; i++)
		if (data->datasets[i] == ds)
			return i;

	return -1;
}

static int
iptc_data_add_dataset_index (IptcData *data, IptcDataSet *dataset, unsigned int index)
{
	if (!data || !data->priv || !dataset || dataset->parent ||
			index > data->count)
		return -1;

	dataset->parent = data;
	data->datasets = iptc_mem_realloc (data->priv->mem,
		data->datasets, sizeof (IptcDataSet *) * (data->count + 1));
	if (!data->datasets) return -1;
	if (index != data->count)
		memmove (data->datasets + index + 1, data->datasets + index,
				sizeof(IptcDataSet *) * (data->count - index));
	data->datasets[index] = dataset;
	iptc_dataset_ref (dataset);
	data->count++;

	return 0;
}

/**
 * iptc_data_add_dataset:
 * @data: the collection to which the dataset will be added
 * @ds: dataset to add
 *
 * Adds an #IptcDataSet at the end of an existing collection.  A
 * dataset can only be a member of a single collection.  This
 * function will increment the reference count of @dataset, so if
 * the application no longer intends to use @dataset directly, it
 * should call iptc_dataset_unref() immediately following this call.
 *
 * Returns: 0 on success, -1 on error
 */
int
iptc_data_add_dataset (IptcData *data, IptcDataSet *dataset)
{
	if (!data) return -1;

	if (iptc_data_add_dataset_index (data, dataset, data->count) < 0)
		return -1;

	return 0;
}

/**
 * iptc_data_add_dataset_before:
 * @data: the collection to which the dataset will be added
 * @ds: existing dataset before which @newds will be added
 * @newds: dataset to add
 *
 * Same as iptc_data_add_dataset(), except the @dataset will be added
 * before an existing dataset @ds, so that if they are enumerated,
 * @newds will appear before @ds in the collection.  @ds must be
 * a dataset already present in the collection.
 *
 * Returns: 0 on success, -1 on error
 */
int
iptc_data_add_dataset_before (IptcData *data, IptcDataSet *ds,
	IptcDataSet *newds)
{
	int i = iptc_data_dataset_index (data, ds);

	if (i < 0)
		return -1;

	if (iptc_data_add_dataset_index (data, newds, i) < 0)
		return -1;

	return 0;
}

/**
 * iptc_data_add_dataset_after:
 * @data: the collection to which the dataset will be added
 * @ds: existing dataset after which @newds will be added
 * @newds: dataset to add
 *
 * Same as iptc_data_add_dataset(), except the @dataset will be added
 * after an existing dataset @ds, so that if they are enumerated,
 * @newds will appear after @ds in the collection.  @ds must be a
 * dataset already present in the collection.
 *
 * Returns: 0 on success, -1 on error
 */
int
iptc_data_add_dataset_after (IptcData *data, IptcDataSet *ds,
	IptcDataSet *newds)
{
	int i = iptc_data_dataset_index (data, ds);

	if (i < 0)
		return -1;

	if (iptc_data_add_dataset_index (data, newds, i+1) < 0)
		return -1;

	return 0;
}

/**
 * iptc_data_remove_dataset:
 * @data: the collection from which to remove an entry
 * @ds: the dataset to remove from the collection
 *
 * Removes a dataset from a collection.  This function also calls
 * iptc_dataset_unref() on @ds.
 *
 * Returns: 0 on success, -1 on error
 */
int
iptc_data_remove_dataset (IptcData *data, IptcDataSet *dataset)
{
	unsigned int i;

	if (!data || !data->priv || !dataset ||
		(dataset->parent != data)) return -1;

	/* Search the dataset */
	i = iptc_data_dataset_index (data, dataset);
	if (i < 0) return -1;

	/* Remove the dataset */
	memmove (data->datasets + i, data->datasets + i + 1,
		 sizeof (IptcDataSet *) * (data->count - i - 1));
	data->count--;
	dataset->parent = NULL;
	iptc_dataset_unref (dataset);
	data->datasets = iptc_mem_realloc (data->priv->mem, data->datasets,
					sizeof(IptcDataSet *) * data->count);

	return 0;
}

/**
 * iptc_data_get_next_dataset:
 * @data: collection to search
 * @ds: dataset where the search should originate (can be NULL)
 * @record: record identifier of dataset
 * @tag: tag identifier (dataset number) of dataset
 *
 * Finds the next occurence of a dataset inside a collection with
 * the specified record and tag identifiers following @ds.  This is useful
 * when a collection contains more than one dataset with the same record
 * and tag identifier (for example, the keywords tag appears once for
 * each keyword in the IPTC metadata).  When @ds is NULL, this function
 * is equivalent to iptc_data_get_dataset().
 *
 * Returns: pointer to dataset if found, NULL if no matching dataset found
 */
IptcDataSet *
iptc_data_get_next_dataset (IptcData *data, IptcDataSet *ds,
	IptcRecord record, IptcTag tag)
{
	int i = 0;

	if (!data)
		return NULL;

	if (ds) {
		i = iptc_data_dataset_index (data, ds);
		if (i < 0)
			return NULL;
		i++;
	}

	for (; (unsigned)i < data->count; i++)
		if (data->datasets[i]->record == record && 
				data->datasets[i]->tag == tag) {
			iptc_dataset_ref (data->datasets[i]);
			return (data->datasets[i]);
		}

	return NULL;
}

/**
 * iptc_data_get_dataset:
 * @data: collection to search
 * @record: record identifier of dataset
 * @tag: tag identifier (dataset number) of dataset
 *
 * Finds the first occurence of a dataset inside a collection with
 * the specified record and tag identifiers.
 *
 * Returns: pointer to dataset if found, NULL if no matching dataset found
 */
IptcDataSet *
iptc_data_get_dataset (IptcData *data, IptcRecord record, IptcTag tag)
{
	return iptc_data_get_next_dataset (data, NULL, record, tag);
}

/**
 * iptc_data_foreach_dataset:
 * @data: collection through which to iterate
 * @func: callback function
 * @user_data: arbitrary user data to be passed to the callback
 *
 * Iterates through each dataset in the collection and calls the
 * callback function on that dataset.
 */
void
iptc_data_foreach_dataset (IptcData *data,
			    IptcDataForeachDataSetFunc func, void *user)
{
	unsigned int i;

	if (!data || !func)
		return;

	for (i = 0; i < data->count; i++)
		func (data->datasets[i], user);
}

static
int dataset_compare (const void * d1, const void * d2)
{
	IptcDataSet * set1 = *(IptcDataSet **)d1;
	IptcDataSet * set2 = *(IptcDataSet **)d2;

	if (set1->record != set2->record)
		return set1->record - set2->record;

	return set1->tag - set2->tag;
}

/**
 * iptc_data_sort:
 * @data: collection of datasets to sort
 *
 * Sorts a collection of datasets in ascending order first by record
 * number and second by tag number.  It can be useful to call this
 * function before saving IPTC data in order to maintain a more
 * organized file.
 */
void
iptc_data_sort (IptcData *data)
{
	if (!data)
		return;

	qsort (data->datasets, data->count, sizeof (IptcDataSet *),
			dataset_compare);
}

/**
 * iptc_data_get_encoding:
 * @data: collection of datasets
 *
 * Identifies the character encoding of the collection of datasets as
 * specified by the "character set" dataset (1:90).  The specification
 * allows this dataset to contain control sequences from the ISO 2022
 * standard.  Unfortunately, this standard is very complicated and
 * generally not used by application writers.  Thus, rather than
 * attempt to decode any possible any possible control sequence, this
 * function only attempts to recognize the control sequence that identifies
 * the UTF-8 character set.  If found, this function will return
 * #IPTC_ENCODING_UTF8.  All character-based datasets in record 2 and
 * higher are then expected to contain data encoded in the UTF-8
 * character set.  Otherwise, this function will return
 * #IPTC_ENCODING_UNKNOWN, meaning that any other character set
 * might be used, including ISO 2022 control sequences that have not
 * been decoded.  If dataset 1:90 is not present, this function returns
 * #IPTC_ENCODING_UNSPECIFIED, in which case character-based datasets
 * will usually be plain ASCII, although broken applications may have
 * used some other encoding.
 *
 * Returns: #IPTC_ENCODING_UTF8, #IPTC_ENCODING_UNKNOWN, or
 * #IPTC_ENCODING_UNSPECIFIED.
 */
static unsigned char utf8_invocation[] = "\x1b\x25\x47";

IptcEncoding
iptc_data_get_encoding (IptcData *data)
{
	IptcDataSet * ds;
	IptcEncoding enc = IPTC_ENCODING_UNKNOWN;

	ds = iptc_data_get_dataset (data, IPTC_RECORD_OBJECT_ENV,
			IPTC_TAG_CHARACTER_SET);
	if (!ds)
		return IPTC_ENCODING_UNSPECIFIED;

	if (ds->size == 3 && !memcmp (ds->data, utf8_invocation, 3))
		enc = IPTC_ENCODING_UTF8;

	iptc_dataset_unref (ds);

	return enc;
}

/**
 * iptc_data_set_encoding_utf8:
 * @data: collection of datasets for which to specify the encoding
 *
 * Sets the contents of the "character set" dataset (1:90) to contain
 * the control sequence that indicates UTF-8 as the character encoding
 * for any character-based datasets in record 2 or higher.  If dataset
 * 1:90 is not present, it will be added to the collection.  Any prior
 * value of dataset 1:90 will be overwritten by this function.
 * Note that some third-party applications (notably Picasa) will ignore
 * all your IPTC data if this option is set.
 *
 * Returns: 0 on success, -1 on failure.
 */
int
iptc_data_set_encoding_utf8 (IptcData *data)
{
	IptcDataSet * ds;
	int ret;

	ds = iptc_data_get_dataset (data, IPTC_RECORD_OBJECT_ENV,
			IPTC_TAG_CHARACTER_SET);
	if (ds) {
		ret = iptc_dataset_set_data (ds, utf8_invocation, 3,
				IPTC_DONT_VALIDATE);
		iptc_dataset_unref (ds);
	}
	else {
		ret = iptc_data_add_dataset_with_contents (data,
				IPTC_RECORD_OBJECT_ENV, IPTC_TAG_CHARACTER_SET,
				utf8_invocation, 3, IPTC_DONT_VALIDATE);
	}
	
	return ret;
}

/**
 * iptc_data_set_version:
 * @data: collection of datasets for which to specify the version
 * @version: version number to write
 *
 * Specifies the version of the IIM specification used by this library by
 * setting the value of datasets 1:00 and 2:00.  It is recommended to set the
 * version number to #IPTC_IIM_VERSION, which specifies the version
 * implemented by this library (currently 4).  If datasets 1:00 or 2:00 are
 * not present, they will be added to the collection.  Any prior value of
 * the datasets will be overwritten by this function.  To ensure strict
 * compliance with the standard, this function should be called before
 * saving a collection of datasets.  However, in practice, some applications
 * (notably Picasa) will ignore all your IPTC data if this option is set.
 *
 * Returns: 0 on success, -1 on failure.
 */
int
iptc_data_set_version (IptcData *data, unsigned int version)
{
	IptcDataSet * ds;
	int ret;

	ds = iptc_data_get_dataset (data, IPTC_RECORD_OBJECT_ENV,
			IPTC_TAG_MODEL_VERSION);
	if (ds) {
		ret = iptc_dataset_set_value (ds, version,
				IPTC_DONT_VALIDATE);
		iptc_dataset_unref (ds);
	}
	else {
		ret = iptc_data_add_dataset_with_value (data,
				IPTC_RECORD_OBJECT_ENV, IPTC_TAG_MODEL_VERSION,
				version, IPTC_DONT_VALIDATE);
	}

	if (ret < 0)
		return -1;

	ds = iptc_data_get_dataset (data, IPTC_RECORD_APP_2,
			IPTC_TAG_RECORD_VERSION);
	if (ds) {
		ret = iptc_dataset_set_value (ds, version,
				IPTC_DONT_VALIDATE);
		iptc_dataset_unref (ds);
	}
	else {
		ret = iptc_data_add_dataset_with_value (data,
				IPTC_RECORD_APP_2, IPTC_TAG_RECORD_VERSION,
				version, IPTC_DONT_VALIDATE);
	}
	return ret;
}

/**
 * iptc_data_add_dataset_with_value:
 * @data: collection to which the new dataset should be added
 * @record: record number of the dataset to be added
 * @tag: tag (dataset number) of the dataset to be added
 * @value: value for the dataset
 * @validate: whether or not the passed value should be validated against
 * the IPTC specification for this dataset's tag.
 *
 * This is a convenience function that creates a new dataset with the given
 * record and tag, adds it to the specified collection of datasets, and
 * stores the given numeric value as the contents of the dataset.  It is
 * equivalent to calling this sequence of functions: iptc_dataset_new(),
 * iptc_dataset_set_tag(), iptc_data_add_dataset(),
 * and iptc_dataset_set_value().
 *
 * Returns: -1 on error, 0 if validation failed, the number of bytes copied
 * on success
 */
int
iptc_data_add_dataset_with_value (IptcData *data, IptcRecord record,
		IptcTag tag, unsigned int value, IptcValidate validate)
{
	IptcDataSet * ds;
	int ret;

	ds = iptc_dataset_new_mem (data->priv->mem);
	if (!ds)
		return -1;

	iptc_dataset_set_tag (ds, record, tag);
	if (iptc_data_add_dataset (data, ds) < 0) {
		iptc_dataset_unref (ds);
		return -1;
	}
	
	ret = iptc_dataset_set_value (ds, value, validate);
	iptc_dataset_unref (ds);
	return ret;
}

/**
 * iptc_data_add_dataset_with_contents:
 * @data: collection to which the new dataset should be added
 * @record: record number of the dataset to be added
 * @tag: tag (dataset number) of the dataset to be added
 * @buf: buffer containing the value
 * @size: number of bytes to copy
 * @validate: whether or not the passed value should be validated against
 * the IPTC specification for this dataset's tag.
 *
 * This is a convenience function that creates a new dataset with the given
 * record and tag, adds it to the specified collection of datasets, and
 * stores a copy of the given data buffer as the contents of the dataset.
 * It is equivalent to calling this sequence of functions: iptc_dataset_new(),
 * iptc_dataset_set_tag(), iptc_data_add_dataset(),
 * and iptc_dataset_set_data().
 *
 * Returns: -1 on error, 0 if validation failed, the number of bytes copied
 * on success
 */
int
iptc_data_add_dataset_with_contents (IptcData *data, IptcRecord record,
		IptcTag tag, const unsigned char * buf,
		unsigned int size, IptcValidate validate)
{
	IptcDataSet * ds;
	int ret;

	ds = iptc_dataset_new_mem (data->priv->mem);
	if (!ds)
		return -1;

	iptc_dataset_set_tag (ds, record, tag);
	if (iptc_data_add_dataset (data, ds) < 0) {
		iptc_dataset_unref (ds);
		return -1;
	}
	
	ret = iptc_dataset_set_data (ds, buf, size, validate);
	iptc_dataset_unref (ds);
	return ret;
}

/**
 * iptc_data_log:
 * @data: collection for which the log object should be changed.
 * @log: log object to use for the collection
 *
 * Changes the logging object for a #IptcData object.  The logging
 * object determines how warning and error messages are relayed back
 * to the application.  This function also calls iptc_log_ref() on
 * the new log object and iptc_log_unref() on the previous log
 * object.
 */
void
iptc_data_log (IptcData *data, IptcLog *log)
{
	if (!data || !data->priv) return;
	iptc_log_unref (data->priv->log);
	data->priv->log = log;
	iptc_log_ref (log);
}

/* Used internally within libiptc */
IptcLog *iptc_data_get_log (IptcData *);
IptcLog *
iptc_data_get_log (IptcData *data)
{
	if (!data || !data->priv) return NULL;
	return data->priv->log;
}
