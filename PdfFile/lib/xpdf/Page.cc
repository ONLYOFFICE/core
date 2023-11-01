//========================================================================
//
// Page.cc
//
// Copyright 1996-2007 Glyph & Cog, LLC
//
//========================================================================

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <stddef.h>
#include "gmempp.h"
#include "Trace.h"
#include "GlobalParams.h"
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "PDFDoc.h"
#include "XRef.h"
#include "Link.h"
#include "OutputDev.h"
#ifndef PDF_PARSER_ONLY
#include "Gfx.h"
#include "GfxState.h"
#include "Annot.h"
#include "AcroForm.h"
#endif
#include "Error.h"
#include "Catalog.h"
#include "Page.h"

//------------------------------------------------------------------------
// PDFRectangle
//------------------------------------------------------------------------

void PDFRectangle::clipTo(PDFRectangle *rect) {
  if (x1 < rect->x1) {
    x1 = rect->x1;
  } else if (x1 > rect->x2) {
    x1 = rect->x2;
  }
  if (x2 < rect->x1) {
    x2 = rect->x1;
  } else if (x2 > rect->x2) {
    x2 = rect->x2;
  }
  if (y1 < rect->y1) {
    y1 = rect->y1;
  } else if (y1 > rect->y2) {
    y1 = rect->y2;
  }
  if (y2 < rect->y1) {
    y2 = rect->y1;
  } else if (y2 > rect->y2) {
    y2 = rect->y2;
  }
}

//------------------------------------------------------------------------
// PageAttrs
//------------------------------------------------------------------------

PageAttrs::PageAttrs(PageAttrs *attrs, Dict *dict) {
  Object obj1;

  // get old/default values
  if (attrs) {
    mediaBox = attrs->mediaBox;
    cropBox = attrs->cropBox;
    haveCropBox = attrs->haveCropBox;
    rotate = attrs->rotate;
    attrs->resources.copy(&resources);
  } else {
    // set default MediaBox to 8.5" x 11" -- this shouldn't be necessary
    // but some (non-compliant) PDF files don't specify a MediaBox
    mediaBox.x1 = 0;
    mediaBox.y1 = 0;
    mediaBox.x2 = 612;
    mediaBox.y2 = 792;
    cropBox.x1 = cropBox.y1 = cropBox.x2 = cropBox.y2 = 0;
    haveCropBox = gFalse;
    rotate = 0;
    resources.initNull();
  }

  // media box
  readBox(dict, "MediaBox", &mediaBox);

  // crop box
  if (readBox(dict, "CropBox", &cropBox)) {
    haveCropBox = gTrue;
  }
  if (!haveCropBox) {
    cropBox = mediaBox;
  }

  // other boxes
  bleedBox = cropBox;
  readBox(dict, "BleedBox", &bleedBox);
  trimBox = cropBox;
  readBox(dict, "TrimBox", &trimBox);
  artBox = cropBox;
  readBox(dict, "ArtBox", &artBox);

  // rotate
  dict->lookup("Rotate", &obj1);
  if (obj1.isInt()) {
    rotate = obj1.getInt();
  }
  obj1.free();
  while (rotate < 0) {
    rotate += 360;
  }
  while (rotate >= 360) {
    rotate -= 360;
  }

  // misc attributes
  dict->lookup("LastModified", &lastModified);
  dict->lookup("BoxColorInfo", &boxColorInfo);
  dict->lookup("Group", &group);
  dict->lookup("Metadata", &metadata);
  dict->lookup("PieceInfo", &pieceInfo);
  dict->lookup("SeparationInfo", &separationInfo);
  if (dict->lookup("UserUnit", &obj1)->isNum()) {
    userUnit = obj1.getNum();
    if (userUnit < 1) {
      userUnit = 1;
    }
  } else {
    userUnit = 1;
  }
  obj1.free();

  // resource dictionary
  dict->lookup("Resources", &obj1);
  if (obj1.isDict()) {
    resources.free();
    obj1.copy(&resources);
  }
  obj1.free();
}

PageAttrs::PageAttrs() {
  mediaBox.x1 = mediaBox.y1 = 0;
  mediaBox.x2 = mediaBox.y2 = 50;
  cropBox = mediaBox;
  haveCropBox = gFalse;
  bleedBox = cropBox;
  trimBox = cropBox;
  artBox = cropBox;
  rotate = 0;
  lastModified.initNull();
  boxColorInfo.initNull();
  group.initNull();
  metadata.initNull();
  pieceInfo.initNull();
  separationInfo.initNull();
  resources.initNull();
}

PageAttrs::~PageAttrs() {
  lastModified.free();
  boxColorInfo.free();
  group.free();
  metadata.free();
  pieceInfo.free();
  separationInfo.free();
  resources.free();
}

void PageAttrs::clipBoxes() {
  cropBox.clipTo(&mediaBox);
  bleedBox.clipTo(&mediaBox);
  trimBox.clipTo(&mediaBox);
  artBox.clipTo(&mediaBox);
}

GBool PageAttrs::readBox(Dict *dict, const char *key, PDFRectangle *box) {
  PDFRectangle tmp;
  double t;
  Object obj1, obj2;
  GBool ok;

  dict->lookup(key, &obj1);
  if (obj1.isArray() && obj1.arrayGetLength() == 4) {
    ok = gTrue;
    obj1.arrayGet(0, &obj2);
    if (obj2.isNum()) {
      tmp.x1 = obj2.getNum();
    } else {
      ok = gFalse;
    }
    obj2.free();
    obj1.arrayGet(1, &obj2);
    if (obj2.isNum()) {
      tmp.y1 = obj2.getNum();
    } else {
      ok = gFalse;
    }
    obj2.free();
    obj1.arrayGet(2, &obj2);
    if (obj2.isNum()) {
      tmp.x2 = obj2.getNum();
    } else {
      ok = gFalse;
    }
    obj2.free();
    obj1.arrayGet(3, &obj2);
    if (obj2.isNum()) {
      tmp.y2 = obj2.getNum();
    } else {
      ok = gFalse;
    }
    obj2.free();
    if (ok) {
      if (tmp.x1 > tmp.x2) {
	t = tmp.x1; tmp.x1 = tmp.x2; tmp.x2 = t;
      }
      if (tmp.y1 > tmp.y2) {
	t = tmp.y1; tmp.y1 = tmp.y2; tmp.y2 = t;
      }
      *box = tmp;
    }
  } else {
    ok = gFalse;
  }
  obj1.free();
  return ok;
}

//------------------------------------------------------------------------
// Page
//------------------------------------------------------------------------

Page::Page(PDFDoc *docA, int numA, Dict *pageDict, PageAttrs *attrsA) {
  ok = gTrue;
  doc = docA;
  xref = doc->getXRef();
  num = numA;

  // get attributes
  attrs = attrsA;
  attrs->clipBoxes();

  // annotations
  pageDict->lookupNF("Annots", &annots);
  if (!(annots.isRef() || annots.isArray() || annots.isNull())) {
    error(errSyntaxError, -1,
	  "Page annotations object (page {0:d}) is wrong type ({1:s})",
	  num, annots.getTypeName());
    annots.free();
    goto err2;
  }

  // contents
  pageDict->lookupNF("Contents", &contents);
  if (!(contents.isRef() || contents.isArray() ||
	contents.isNull())) {
    error(errSyntaxError, -1,
	  "Page contents object (page {0:d}) is wrong type ({1:s})",
	  num, contents.getTypeName());
    contents.free();
    goto err1;
  }

  // thumbnail
  pageDict->lookupNF("Thumb", &thumbnail);
  if (!thumbnail.isRef()) {
    if (!thumbnail.isNull()) {
      thumbnail.free();
      thumbnail.initNull();
    }
  }

  return;

 err2:
  annots.initNull();
 err1:
  contents.initNull();
  thumbnail.initNull();
  ok = gFalse;
}

Page::Page(PDFDoc *docA, int numA) {
  doc = docA;
  xref = doc->getXRef();
  num = numA;
  attrs = new PageAttrs();
  annots.initNull();
  contents.initNull();
  thumbnail.initNull();
  ok = gTrue;
}

Page::~Page() {
  delete attrs;
  annots.free();
  contents.free();
  thumbnail.free();
}

Links *Page::getLinks() {
  Links *links;
  Object obj;

  links = new Links(getAnnots(&obj), doc->getCatalog()->getBaseURI());
  obj.free();
  return links;
}

void Page::display(OutputDev *out, double hDPI, double vDPI,
		   int rotate, GBool useMediaBox, GBool crop,
		   GBool printing,
		   GBool (*abortCheckCbk)(void *data),
		   void *abortCheckCbkData) {
  displaySlice(out, hDPI, vDPI, rotate, useMediaBox, crop,
	       -1, -1, -1, -1, printing,
	       abortCheckCbk, abortCheckCbkData);
}

void Page::displaySlice(OutputDev *out, double hDPI, double vDPI,
			int rotate, GBool useMediaBox, GBool crop,
			int sliceX, int sliceY, int sliceW, int sliceH,
			GBool printing,
			GBool (*abortCheckCbk)(void *data),
			void *abortCheckCbkData) {
#ifndef PDF_PARSER_ONLY
  PDFRectangle *mediaBox, *cropBox;
  PDFRectangle box;
  Gfx *gfx;
  Object obj;
  Annots *annotList;
  AcroForm *form;
  GString* sType;
  int i;

  if (!out->checkPageSlice(this, hDPI, vDPI, rotate, useMediaBox, crop,
			   sliceX, sliceY, sliceW, sliceH,
			   printing, abortCheckCbk, abortCheckCbkData)) {
    return;
  }

  traceBegin(this, "begin page");

  rotate += getRotate();
  if (rotate >= 360) {
    rotate -= 360;
  } else if (rotate < 0) {
    rotate += 360;
  }

  makeBox(hDPI, vDPI, rotate, useMediaBox, out->upsideDown(),
	  sliceX, sliceY, sliceW, sliceH, &box, &crop);
  cropBox = getCropBox();

  if (globalParams->getPrintCommands()) {
    mediaBox = getMediaBox();
    printf("***** MediaBox = ll:%g,%g ur:%g,%g\n",
	   mediaBox->x1, mediaBox->y1, mediaBox->x2, mediaBox->y2);
    printf("***** CropBox = ll:%g,%g ur:%g,%g\n",
	   cropBox->x1, cropBox->y1, cropBox->x2, cropBox->y2);
    printf("***** Rotate = %d\n", attrs->getRotate());
  }

  gfx = new Gfx(doc, out, num, attrs->getResourceDict(),
		hDPI, vDPI, &box, crop ? cropBox : (PDFRectangle *)NULL,
		rotate, abortCheckCbk, abortCheckCbkData);
  contents.fetch(xref, &obj);
  if (!obj.isNull()) {
    gfx->saveState();
    gfx->display(&contents);
    gfx->endOfPage();
  }
  obj.free();

  // draw (non-form) annotations
  if (globalParams->getDrawAnnotations()) {
    annotList = new Annots(doc, getAnnots(&obj));
    obj.free();
    annotList->generateAnnotAppearances();
    if (annotList->getNumAnnots() > 0) {
      if (globalParams->getPrintCommands()) {
	printf("***** Annotations\n");
      }
      for (i = 0; i < annotList->getNumAnnots(); ++i) {
	if (abortCheckCbk && (*abortCheckCbk)(abortCheckCbkData)) {
	  break;
	}
#ifdef BUILDING_WASM_MODULE
	sType = annotList->getAnnot(i)->getType();
	if (sType->cmp("Link") == 0 || sType->cmp("Stamp") == 0 || sType->cmp("FileAttachment") == 0 ||
		sType->cmp("Sound") == 0 || sType->cmp("Movie") == 0 || sType->cmp("Screen") == 0 || sType->cmp("PrinterMark") == 0 ||
		sType->cmp("TrapNet") == 0 || sType->cmp("Watermark") == 0 || sType->cmp("3D") == 0 || sType->cmp("Redact") == 0)
#endif
	annotList->getAnnot(i)->draw(gfx, printing);
      }
    }
    delete annotList;
  }

  // draw form fields
  if (globalParams->getDrawFormFields()) {
    if ((form = doc->getCatalog()->getForm())) {
      if (!(abortCheckCbk && (*abortCheckCbk)(abortCheckCbkData))) {
	form->draw(num, gfx, printing);
      }
    }
  }

  delete gfx;
#endif // PDF_PARSER_ONLY

  traceEnd(this, "end page");
}

void Page::makeBox(double hDPI, double vDPI, int rotate,
		   GBool useMediaBox, GBool upsideDown,
		   double sliceX, double sliceY, double sliceW, double sliceH,
		   PDFRectangle *box, GBool *crop) {
  PDFRectangle *mediaBox, *cropBox, *baseBox;
  double kx, ky;

  mediaBox = getMediaBox();
  cropBox = getCropBox();
  if (sliceW >= 0 && sliceH >= 0) {
    baseBox = useMediaBox ? mediaBox : cropBox;
    kx = 72.0 / hDPI;
    ky = 72.0 / vDPI;
    if (rotate == 90) {
      if (upsideDown) {
	box->x1 = baseBox->x1 + ky * sliceY;
	box->x2 = baseBox->x1 + ky * (sliceY + sliceH);
      } else {
	box->x1 = baseBox->x2 - ky * (sliceY + sliceH);
	box->x2 = baseBox->x2 - ky * sliceY;
      }
      box->y1 = baseBox->y1 + kx * sliceX;
      box->y2 = baseBox->y1 + kx * (sliceX + sliceW);
    } else if (rotate == 180) {
      box->x1 = baseBox->x2 - kx * (sliceX + sliceW);
      box->x2 = baseBox->x2 - kx * sliceX;
      if (upsideDown) {
	box->y1 = baseBox->y1 + ky * sliceY;
	box->y2 = baseBox->y1 + ky * (sliceY + sliceH);
      } else {
	box->y1 = baseBox->y2 - ky * (sliceY + sliceH);
	box->y2 = baseBox->y2 - ky * sliceY;
      }
    } else if (rotate == 270) {
      if (upsideDown) {
	box->x1 = baseBox->x2 - ky * (sliceY + sliceH);
	box->x2 = baseBox->x2 - ky * sliceY;
      } else {
	box->x1 = baseBox->x1 + ky * sliceY;
	box->x2 = baseBox->x1 + ky * (sliceY + sliceH);
      }
      box->y1 = baseBox->y2 - kx * (sliceX + sliceW);
      box->y2 = baseBox->y2 - kx * sliceX;
    } else {
      box->x1 = baseBox->x1 + kx * sliceX;
      box->x2 = baseBox->x1 + kx * (sliceX + sliceW);
      if (upsideDown) {
	box->y1 = baseBox->y2 - ky * (sliceY + sliceH);
	box->y2 = baseBox->y2 - ky * sliceY;
      } else {
	box->y1 = baseBox->y1 + ky * sliceY;
	box->y2 = baseBox->y1 + ky * (sliceY + sliceH);
      }
    }
  } else if (useMediaBox) {
    *box = *mediaBox;
  } else {
    *box = *cropBox;
    *crop = gFalse;
  }
}

void Page::processLinks(OutputDev *out) {
  Links *links;
  int i;

  links = getLinks();
  for (i = 0; i < links->getNumLinks(); ++i) {
    out->processLink(links->getLink(i));
  }
  delete links;
}

#ifndef PDF_PARSER_ONLY
void Page::getDefaultCTM(double *ctm, double hDPI, double vDPI,
			 int rotate, GBool useMediaBox, GBool upsideDown) {
  GfxState *state;
  int i;

  rotate += getRotate();
  if (rotate >= 360) {
    rotate -= 360;
  } else if (rotate < 0) {
    rotate += 360;
  }
  state = new GfxState(hDPI, vDPI,
		       useMediaBox ? getMediaBox() : getCropBox(),
		       rotate, upsideDown);
  for (i = 0; i < 6; ++i) {
    ctm[i] = state->getCTM()[i];
  }
  delete state;
}
#endif
