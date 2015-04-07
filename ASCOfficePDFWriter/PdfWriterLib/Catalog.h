#ifndef _CATALOG_H
#define _CATALOG_H

#pragma once 

#include "Objects.h"
#include "Pages.h"

static const char *PAGE_LAYOUT_NAMES[] = 
{
	"SinglePage",
	"OneColumn",
	"TwoColumnLeft",
	"TwoColumnRight",
	NULL
};


static const char *PAGE_MODE_NAMES[] = 
{
	"UseNone",
	"UseOutlines",
	"UseThumbs",
	"FullScreen",
	"UseOC",
	"UseAttachments",
	NULL
};

//-------- CatalogObject ----------------------------------------------------

typedef Dict Catalog;

Catalog       CatalogNew                (MMgr oMMgr, Xref pXref);
Pages         CatalogGetRoot            (Catalog pCatalog);
PageLayout    CatalogGetPageLayout      (Catalog pCatalog);
unsigned long CatalogSetPageLayout      (Catalog pCatalog, PageLayout eLayout);
PageMode      CatalogGetPageMode        (Catalog pCatalog);
unsigned long CatalogSetPageMode        (Catalog pCatalog, PageMode eMode);
unsigned long CatalogSetOpenAction      (Catalog pCatalog, Destination pOpenAction);
unsigned long CatalogAddPageLabel       (Catalog pCatalog, unsigned int nPageNum, Dict pPageLabel);
unsigned int  CatalogGetViewerPreference(Catalog pCatalog);
unsigned long CatalogSetViewerPreference(Catalog pCatalog, unsigned int nValue);
BOOL          CatalogValidate           (Catalog pCatalog);
#endif /* _CATALOG_H */

