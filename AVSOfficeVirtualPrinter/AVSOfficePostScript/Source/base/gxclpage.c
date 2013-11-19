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

/* $Id: gxclpage.c 9062 2008-09-03 11:42:35Z leonardo $ */
/* Page object management */
#include "gdevprn.h"
#include "gxcldev.h"
#include "gxclpage.h"

/* Save a page. */
int
gdev_prn_save_page(gx_device_printer * pdev, gx_saved_page * page,
		   int num_copies)
{
    gx_device_clist *cdev = (gx_device_clist *) pdev;

    /* Make sure we are banding. */
    if (!pdev->buffer_space)
	return_error(gs_error_rangecheck);
    if (strlen(pdev->dname) >= sizeof(page->dname))
	return_error(gs_error_limitcheck);
    {
	gx_device_clist_writer * const pcldev =
	    (gx_device_clist_writer *)pdev;
	int code;

	if ((code = clist_end_page(pcldev)) < 0 ||
	    (code = cdev->common.page_info.io_procs->fclose(pcldev->page_cfile, pcldev->page_cfname, false)) < 0 ||
	    (code = cdev->common.page_info.io_procs->fclose(pcldev->page_bfile, pcldev->page_bfname, false)) < 0
	    )
	    return code;
	/* Save the device information. */
	memcpy(&page->device, pdev, sizeof(gx_device));
	strcpy(page->dname, pdev->dname);
	/* Save the page information. */
	page->info = pcldev->page_info;
	page->info.cfile = 0;
	page->info.bfile = 0;
    }
    /* Save other information. */
    page->num_copies = num_copies;
    return (*gs_clist_device_procs.open_device) ((gx_device *) pdev);
}

/* Render an array of saved pages. */
int
gdev_prn_render_pages(gx_device_printer * pdev,
		      const gx_placed_page * ppages, int count)
{
    gx_device_clist_reader * const pcldev =
	(gx_device_clist_reader *)pdev;

    /* Check to make sure the pages are compatible with the device. */
    {
	int i;
	
	for (i = 0; i < count; ++i) {
	    const gx_saved_page *page = ppages[i].page;

	    /* We would like to fully check the color representation, */
	    /* but we don't have enough information to do that. */
	    if (strcmp(page->dname, pdev->dname) != 0 ||
		memcmp(&page->device.color_info, &pdev->color_info,
		       sizeof(pdev->color_info)) != 0
		)
		return_error(gs_error_rangecheck);
	    /* Currently we don't allow translation in Y. */
	    if (ppages[i].offset.y != 0)
		return_error(gs_error_rangecheck);
	    /* Make sure the band parameters are compatible. */
	    if (page->info.band_params.BandBufferSpace !=
		pdev->buffer_space ||
		page->info.band_params.BandWidth !=
		pdev->width
		)
		return_error(gs_error_rangecheck);
	    /* Currently we require all band heights to be the same. */
	    if (i > 0 && page->info.band_params.BandHeight != 
			 ppages[0].page->info.band_params.BandHeight)
		return_error(gs_error_rangecheck);
	}
    }
    /* Set up the page list in the device. */
    /****** SHOULD FACTOR THIS OUT OF clist_render_init ******/
    pcldev->ymin = pcldev->ymax = 0;
    pcldev->pages = ppages;
    pcldev->num_pages = count;
    pcldev->offset_map = NULL;
    /* Render the pages. */
    {
	int code = (*dev_proc(pdev, output_page))
	    ((gx_device *) pdev, ppages[0].page->num_copies, true);

	/* Delete the temporary files. */
	int i;

	for (i = 0; i < count; ++i) {
	    const gx_saved_page *page = ppages[i].page;

	    pcldev->page_info.io_procs->unlink(page->info.cfname);
	    pcldev->page_info.io_procs->unlink(page->info.bfname);
	}
	return code;
    }
}
