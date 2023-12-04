//========================================================================
//
// Gfx.cc
//
// Copyright 1996-2016 Glyph & Cog, LLC
//
//========================================================================

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "gmem.h"
#include "gmempp.h"
#include "GString.h"
#include "GList.h"
#include "Trace.h"
#include "GlobalParams.h"
#include "CharTypes.h"
#include "Object.h"
#include "PDFDoc.h"
#include "Array.h"
#include "Dict.h"
#include "Stream.h"
#include "Lexer.h"
#include "Parser.h"
#include "GfxFont.h"
#include "GfxState.h"
#include "OutputDev.h"
#include "Page.h"
#include "Annot.h"
#include "OptionalContent.h"
#include "Error.h"
#include "TextString.h"
#include "Gfx.h"

// the MSVC math.h doesn't define this
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//------------------------------------------------------------------------
// constants
//------------------------------------------------------------------------

// Max recursive depth for a function shading fill.
#define functionMaxDepth 6

// Max delta allowed in any color component for a function shading fill.
#define functionColorDelta (dblToCol(1 / 256.0))

// Number of splits along the t axis for an axial shading fill.
#define axialSplits 256

// Max delta allowed in any color component for an axial shading fill.
#define axialColorDelta (dblToCol(1 / 256.0))

// Max number of splits along the t axis for a radial shading fill.
#define radialMaxSplits 256

// Max delta allowed in any color component for a radial shading fill.
#define radialColorDelta (dblToCol(1 / 256.0))

// Max recursive depth for a Gouraud triangle shading fill.
#define gouraudMaxDepth 6

// Max delta allowed in any color component for a Gouraud triangle
// shading fill.
#define gouraudColorDelta (dblToCol(1 / 256.0))

// Max recursive depth for a patch mesh shading fill.
#define patchMaxDepth 6

// Max delta allowed in any color component for a patch mesh shading
// fill.
#define patchColorDelta (dblToCol(1 / 256.0))

// Max errors (undefined operator, wrong number of args) allowed before
// giving up on a content stream.
#define contentStreamErrorLimit 500

//------------------------------------------------------------------------
// Operator table
//------------------------------------------------------------------------

#ifdef _WIN32 // this works around a bug in the VC7 compiler
#  pragma optimize("",off)
#endif

Operator Gfx::opTab[] = {
  {"\"",  3, {tchkNum,    tchkNum,    tchkString},
          &Gfx::opMoveSetShowText},
  {"'",   1, {tchkString},
          &Gfx::opMoveShowText},
  {"B",   0, {tchkNone},
          &Gfx::opFillStroke},
  {"B*",  0, {tchkNone},
          &Gfx::opEOFillStroke},
  {"BDC", 2, {tchkName,   tchkProps},
          &Gfx::opBeginMarkedContent},
  {"BI",  0, {tchkNone},
          &Gfx::opBeginImage},
  {"BMC", 1, {tchkName},
          &Gfx::opBeginMarkedContent},
  {"BT",  0, {tchkNone},
          &Gfx::opBeginText},
  {"BX",  0, {tchkNone},
          &Gfx::opBeginIgnoreUndef},
  {"CS",  1, {tchkName},
          &Gfx::opSetStrokeColorSpace},
  {"DP",  2, {tchkName,   tchkProps},
          &Gfx::opMarkPoint},
  {"Do",  1, {tchkName},
          &Gfx::opXObject},
  {"EI",  0, {tchkNone},
          &Gfx::opEndImage},
  {"EMC", 0, {tchkNone},
          &Gfx::opEndMarkedContent},
  {"ET",  0, {tchkNone},
          &Gfx::opEndText},
  {"EX",  0, {tchkNone},
          &Gfx::opEndIgnoreUndef},
  {"F",   0, {tchkNone},
          &Gfx::opFill},
  {"G",   1, {tchkNum},
          &Gfx::opSetStrokeGray},
  {"ID",  0, {tchkNone},
          &Gfx::opImageData},
  {"J",   1, {tchkInt},
          &Gfx::opSetLineCap},
  {"K",   4, {tchkNum,    tchkNum,    tchkNum,    tchkNum},
          &Gfx::opSetStrokeCMYKColor},
  {"M",   1, {tchkNum},
          &Gfx::opSetMiterLimit},
  {"MP",  1, {tchkName},
          &Gfx::opMarkPoint},
  {"Q",   0, {tchkNone},
          &Gfx::opRestore},
  {"RG",  3, {tchkNum,    tchkNum,    tchkNum},
          &Gfx::opSetStrokeRGBColor},
  {"S",   0, {tchkNone},
          &Gfx::opStroke},
  {"SC",  -4, {tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum},
          &Gfx::opSetStrokeColor},
  {"SCN", -33, {tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN},
          &Gfx::opSetStrokeColorN},
  {"T*",  0, {tchkNone},
          &Gfx::opTextNextLine},
  {"TD",  2, {tchkNum,    tchkNum},
          &Gfx::opTextMoveSet},
  {"TJ",  1, {tchkArray},
          &Gfx::opShowSpaceText},
  {"TL",  1, {tchkNum},
          &Gfx::opSetTextLeading},
  {"Tc",  1, {tchkNum},
          &Gfx::opSetCharSpacing},
  {"Td",  2, {tchkNum,    tchkNum},
          &Gfx::opTextMove},
  {"Tf",  2, {tchkName,   tchkNum},
          &Gfx::opSetFont},
  {"Tj",  1, {tchkString},
          &Gfx::opShowText},
  {"Tm",  6, {tchkNum,    tchkNum,    tchkNum,    tchkNum,
	      tchkNum,    tchkNum},
          &Gfx::opSetTextMatrix},
  {"Tr",  1, {tchkInt},
          &Gfx::opSetTextRender},
  {"Ts",  1, {tchkNum},
          &Gfx::opSetTextRise},
  {"Tw",  1, {tchkNum},
          &Gfx::opSetWordSpacing},
  {"Tz",  1, {tchkNum},
          &Gfx::opSetHorizScaling},
  {"W",   0, {tchkNone},
          &Gfx::opClip},
  {"W*",  0, {tchkNone},
          &Gfx::opEOClip},
  {"b",   0, {tchkNone},
          &Gfx::opCloseFillStroke},
  {"b*",  0, {tchkNone},
          &Gfx::opCloseEOFillStroke},
  {"c",   6, {tchkNum,    tchkNum,    tchkNum,    tchkNum,
	      tchkNum,    tchkNum},
          &Gfx::opCurveTo},
  {"cm",  6, {tchkNum,    tchkNum,    tchkNum,    tchkNum,
	      tchkNum,    tchkNum},
          &Gfx::opConcat},
  {"cs",  1, {tchkName},
          &Gfx::opSetFillColorSpace},
  {"d",   2, {tchkArray,  tchkNum},
          &Gfx::opSetDash},
  {"d0",  2, {tchkNum,    tchkNum},
          &Gfx::opSetCharWidth},
  {"d1",  6, {tchkNum,    tchkNum,    tchkNum,    tchkNum,
	      tchkNum,    tchkNum},
          &Gfx::opSetCacheDevice},
  {"f",   0, {tchkNone},
          &Gfx::opFill},
  {"f*",  0, {tchkNone},
          &Gfx::opEOFill},
  {"g",   1, {tchkNum},
          &Gfx::opSetFillGray},
  {"gs",  1, {tchkName},
          &Gfx::opSetExtGState},
  {"h",   0, {tchkNone},
          &Gfx::opClosePath},
  {"i",   1, {tchkNum},
          &Gfx::opSetFlat},
  {"j",   1, {tchkInt},
          &Gfx::opSetLineJoin},
  {"k",   4, {tchkNum,    tchkNum,    tchkNum,    tchkNum},
          &Gfx::opSetFillCMYKColor},
  {"l",   2, {tchkNum,    tchkNum},
          &Gfx::opLineTo},
  {"m",   2, {tchkNum,    tchkNum},
          &Gfx::opMoveTo},
  {"n",   0, {tchkNone},
          &Gfx::opEndPath},
  {"q",   0, {tchkNone},
          &Gfx::opSave},
  {"re",  4, {tchkNum,    tchkNum,    tchkNum,    tchkNum},
          &Gfx::opRectangle},
  {"rg",  3, {tchkNum,    tchkNum,    tchkNum},
          &Gfx::opSetFillRGBColor},
  {"ri",  1, {tchkName},
          &Gfx::opSetRenderingIntent},
  {"s",   0, {tchkNone},
          &Gfx::opCloseStroke},
  {"sc",  -4, {tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum,   tchkNum,    tchkNum,    tchkNum,
	       tchkNum},
          &Gfx::opSetFillColor},
  {"scn", -33, {tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN,   tchkSCN,    tchkSCN,    tchkSCN,
	        tchkSCN},
          &Gfx::opSetFillColorN},
  {"sh",  1, {tchkName},
          &Gfx::opShFill},
  {"v",   4, {tchkNum,    tchkNum,    tchkNum,    tchkNum},
          &Gfx::opCurveTo1},
  {"w",   1, {tchkNum},
          &Gfx::opSetLineWidth},
  {"y",   4, {tchkNum,    tchkNum,    tchkNum,    tchkNum},
          &Gfx::opCurveTo2},
};

#ifdef _WIN32 // this works around a bug in the VC7 compiler
#  pragma optimize("",on)
#endif

#define numOps (sizeof(opTab) / sizeof(Operator))

//------------------------------------------------------------------------
// GfxResources
//------------------------------------------------------------------------

GfxResources::GfxResources(XRef *xref, Dict *resDict, GfxResources *nextA) {
  Object obj1, obj2;
  Ref r;

  if (resDict) {
    valid = gTrue;

    // build font dictionary
    fonts = NULL;
    resDict->lookupNF("Font", &obj1);
    if (obj1.isRef()) {
      obj1.fetch(xref, &obj2);
      if (obj2.isDict()) {
	r = obj1.getRef();
	fonts = new GfxFontDict(xref, &r, obj2.getDict());
      }
      obj2.free();
    } else if (obj1.isDict()) {
      fonts = new GfxFontDict(xref, NULL, obj1.getDict());
    }
    obj1.free();

    // get XObject dictionary
    resDict->lookup("XObject", &xObjDict);

    // get color space dictionary
    resDict->lookup("ColorSpace", &colorSpaceDict);

    // get pattern dictionary
    resDict->lookup("Pattern", &patternDict);

    // get shading dictionary
    resDict->lookup("Shading", &shadingDict);

    // get graphics state parameter dictionary
    resDict->lookup("ExtGState", &gStateDict);

    // get properties dictionary
    resDict->lookup("Properties", &propsDict);

  } else {
    valid = gFalse;
    fonts = NULL;
    xObjDict.initNull();
    colorSpaceDict.initNull();
    patternDict.initNull();
    shadingDict.initNull();
    gStateDict.initNull();
    propsDict.initNull();
  }

  next = nextA;
}

GfxResources::~GfxResources() {
  if (fonts) {
    delete fonts;
  }
  xObjDict.free();
  colorSpaceDict.free();
  patternDict.free();
  shadingDict.free();
  gStateDict.free();
  propsDict.free();
}

GfxFont *GfxResources::lookupFont(char *name) {
  GfxFont *font;
  GfxResources *resPtr;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->fonts) {
      if ((font = resPtr->fonts->lookup(name))) {
	return font;
      }
    }
  }
  error(errSyntaxError, -1, "Unknown font tag '{0:s}'", name);
  return NULL;
}

GfxFont *GfxResources::lookupFontByRef(Ref ref) {
  GfxFont *font;
  GfxResources *resPtr;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->fonts) {
      if ((font = resPtr->fonts->lookupByRef(ref))) {
	return font;
      }
    }
  }
  error(errSyntaxError, -1, "Unknown font ref {0:d}.{1:d}", ref.num, ref.gen);
  return NULL;
}

GBool GfxResources::lookupXObject(const char *name, Object *obj) {
  GfxResources *resPtr;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->xObjDict.isDict()) {
      if (!resPtr->xObjDict.dictLookup(name, obj)->isNull())
	return gTrue;
      obj->free();
    }
  }
  error(errSyntaxError, -1, "XObject '{0:s}' is unknown", name);
  return gFalse;
}

GBool GfxResources::lookupXObjectNF(const char *name, Object *obj) {
  GfxResources *resPtr;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->xObjDict.isDict()) {
      if (!resPtr->xObjDict.dictLookupNF(name, obj)->isNull())
	return gTrue;
      obj->free();
    }
  }
  error(errSyntaxError, -1, "XObject '{0:s}' is unknown", name);
  return gFalse;
}

void GfxResources::lookupColorSpace(const char *name, Object *obj,
				    GBool inherit) {
  GfxResources *resPtr;

  //~ should also test for G, RGB, and CMYK - but only in inline images (?)
  if (!strcmp(name, "DeviceGray") ||
      !strcmp(name, "DeviceRGB") ||
      !strcmp(name, "DeviceCMYK")) {
    obj->initNull();
    return;
  }
  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->colorSpaceDict.isDict()) {
      if (!resPtr->colorSpaceDict.dictLookup(name, obj)->isNull()) {
	return;
      }
      obj->free();
    }
    if (!inherit && valid) {
      break;
    }
  }
  obj->initNull();
}

GfxPattern *GfxResources::lookupPattern(const char *name
					) {
  GfxResources *resPtr;
  GfxPattern *pattern;
  Object objRef, obj;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->patternDict.isDict()) {
      if (!resPtr->patternDict.dictLookup(name, &obj)->isNull()) {
	resPtr->patternDict.dictLookupNF(name, &objRef);
	pattern = GfxPattern::parse(&objRef, &obj
				    );
	objRef.free();
	obj.free();
	return pattern;
      }
      obj.free();
    }
  }
  error(errSyntaxError, -1, "Unknown pattern '{0:s}'", name);
  return NULL;
}

GfxShading *GfxResources::lookupShading(const char *name
					) {
  GfxResources *resPtr;
  GfxShading *shading;
  Object obj;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->shadingDict.isDict()) {
      if (!resPtr->shadingDict.dictLookup(name, &obj)->isNull()) {
	shading = GfxShading::parse(&obj
				    );
	obj.free();
	return shading;
      }
      obj.free();
    }
  }
  error(errSyntaxError, -1, "Unknown shading '{0:s}'", name);
  return NULL;
}

GBool GfxResources::lookupGState(const char *name, Object *obj) {
  GfxResources *resPtr;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->gStateDict.isDict()) {
      if (!resPtr->gStateDict.dictLookup(name, obj)->isNull()) {
	return gTrue;
      }
      obj->free();
    }
  }
  error(errSyntaxError, -1, "ExtGState '{0:s}' is unknown", name);
  return gFalse;
}

GBool GfxResources::lookupPropertiesNF(const char *name, Object *obj) {
  GfxResources *resPtr;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->propsDict.isDict()) {
      if (!resPtr->propsDict.dictLookupNF(name, obj)->isNull()) {
	return gTrue;
      }
      obj->free();
    }
  }
  error(errSyntaxError, -1, "Properties '{0:s}' is unknown", name);
  return gFalse;
}

//------------------------------------------------------------------------
// Gfx
//------------------------------------------------------------------------

Gfx::Gfx(PDFDoc *docA, OutputDev *outA, int pageNum, Dict *resDict,
	 double hDPI, double vDPI, PDFRectangle *box,
	 PDFRectangle *cropBox, int rotate,
	 GBool (*abortCheckCbkA)(void *data),
	 void *abortCheckCbkDataA) {
  int i;

  doc = docA;
  xref = doc->getXRef();
  subPage = gFalse;
  printCommands = globalParams->getPrintCommands();

  // start the resource stack
  res = new GfxResources(xref, resDict, NULL);

  // initialize
  out = outA;
  state = new GfxState(hDPI, vDPI, box, rotate, out->upsideDown());
  fontChanged = gFalse;
  clip = clipNone;
  ignoreUndef = 0;
  out->startPage(pageNum, state);
  out->setDefaultCTM(state->getCTM());
  out->updateAll(state);
  for (i = 0; i < 6; ++i) {
    baseMatrix[i] = state->getCTM()[i];
  }
  formDepth = 0;
  markedContentStack = new GList();
  ocState = gTrue;
  parser = NULL;
  contentStreamStack = new GList();
  abortCheckCbk = abortCheckCbkA;
  abortCheckCbkData = abortCheckCbkDataA;

  // set crop box
  if (cropBox) {
    state->moveTo(cropBox->x1, cropBox->y1);
    state->lineTo(cropBox->x2, cropBox->y1);
    state->lineTo(cropBox->x2, cropBox->y2);
    state->lineTo(cropBox->x1, cropBox->y2);
    state->closePath();
    state->clip();
    out->clip(state);
    state->clearPath();
  }
}

Gfx::Gfx(PDFDoc *docA, OutputDev *outA, Dict *resDict,
	 PDFRectangle *box, PDFRectangle *cropBox,
	 GBool (*abortCheckCbkA)(void *data),
	 void *abortCheckCbkDataA) {
  int i;

  doc = docA;
  xref = doc->getXRef();
  subPage = gTrue;
  printCommands = globalParams->getPrintCommands();

  // start the resource stack
  res = new GfxResources(xref, resDict, NULL);

  // initialize
  out = outA;
  state = new GfxState(72, 72, box, 0, gFalse);
  fontChanged = gFalse;
  clip = clipNone;
  ignoreUndef = 0;
  for (i = 0; i < 6; ++i) {
    baseMatrix[i] = state->getCTM()[i];
  }
  formDepth = 0;
  markedContentStack = new GList();
  ocState = gTrue;
  parser = NULL;
  contentStreamStack = new GList();
  abortCheckCbk = abortCheckCbkA;
  abortCheckCbkData = abortCheckCbkDataA;

  // set crop box
  if (cropBox) {
    state->moveTo(cropBox->x1, cropBox->y1);
    state->lineTo(cropBox->x2, cropBox->y1);
    state->lineTo(cropBox->x2, cropBox->y2);
    state->lineTo(cropBox->x1, cropBox->y2);
    state->closePath();
    state->clip();
    out->clip(state);
    state->clearPath();
  }
}

Gfx::~Gfx() {
  if (!subPage) {
    out->endPage();
  }
  while (state->hasSaves()) {
    restoreState();
  }
  delete state;
  while (res) {
    popResources();
  }
  deleteGList(markedContentStack, GfxMarkedContent);
  delete contentStreamStack;
}

void Gfx::display(Object *objRef, GBool topLevel) {
  Object obj1, obj2;
  int i;

  objRef->fetch(xref, &obj1);
  if (obj1.isArray()) {
    for (i = 0; i < obj1.arrayGetLength(); ++i) {
      obj1.arrayGetNF(i, &obj2);
      if (checkForContentStreamLoop(&obj2)) {
	obj2.free();
	obj1.free();
	return;
      }
      obj2.free();
    }
    for (i = 0; i < obj1.arrayGetLength(); ++i) {
      obj1.arrayGet(i, &obj2);
      if (!obj2.isStream()) {
	error(errSyntaxError, -1, "Invalid object type for content stream");
	obj2.free();
	obj1.free();
	return;
      }
      obj2.free();
    }
    contentStreamStack->append(&obj1);
  } else if (obj1.isStream()) {
    if (checkForContentStreamLoop(objRef)) {
      obj1.free();
      return;
    }
    contentStreamStack->append(objRef);
  } else {
    error(errSyntaxError, -1, "Invalid object type for content stream");
    obj1.free();
    return;
  }
  parser = new Parser(xref, new Lexer(xref, &obj1), gFalse);
  go(topLevel);
  delete parser;
  parser = NULL;
  contentStreamStack->del(contentStreamStack->getLength() - 1);
  obj1.free();
}

// If <ref> is already on contentStreamStack, i.e., if there is a loop
// in the content streams, report an error, and return true.
GBool Gfx::checkForContentStreamLoop(Object *ref) {
  Object *objPtr;
  Object obj1;
  int i, j;

  if (ref->isRef()) {
    for (i = 0; i < contentStreamStack->getLength(); ++i) {
      objPtr = (Object *)contentStreamStack->get(i);
      if (objPtr->isRef()) {
	if (ref->getRefNum() == objPtr->getRefNum() &&
	    ref->getRefGen() == objPtr->getRefGen()) {
	  error(errSyntaxError, -1, "Loop in content streams");
	  return gTrue;
	}
      } else if (objPtr->isArray()) {
	for (j = 0; j < objPtr->arrayGetLength(); ++j) {
	  objPtr->arrayGetNF(j, &obj1);
	  if (obj1.isRef()) {
	    if (ref->getRefNum() == obj1.getRefNum() &&
		ref->getRefGen() == obj1.getRefGen()) {
	      error(errSyntaxError, -1, "Loop in content streams");
	      obj1.free();
	      return gTrue;
	    }
	  }
	  obj1.free();
	}
      }
    }
  }
  return gFalse;
}

void Gfx::go(GBool topLevel) {
  Object obj;
  Object args[maxArgs];
  GBool aborted;
  int numArgs, i;
  int errCount;

  // scan a sequence of objects
  opCounter = 0;
  aborted = gFalse;
  errCount = 0;
  numArgs = 0;
  getContentObj(&obj);
  while (!obj.isEOF()) {

    // check for an abort
    ++opCounter;
    if (abortCheckCbk && opCounter > 100) {
      if ((*abortCheckCbk)(abortCheckCbkData)) {
	aborted = gTrue;
	break;
      }
      opCounter = 0;
    }

    // got a command - execute it
    if (obj.isCmd()) {
      if (printCommands) {
	obj.print(stdout);
	for (i = 0; i < numArgs; ++i) {
	  printf(" ");
	  args[i].print(stdout);
	}
	printf("\n");
	fflush(stdout);
      }
      if (!execOp(&obj, args, numArgs)) {
	++errCount;
      }
      obj.free();
      for (i = 0; i < numArgs; ++i)
	args[i].free();
      numArgs = 0;

      // check for too many errors
      if (errCount > contentStreamErrorLimit) {
	error(errSyntaxError, -1,
	      "Too many errors - giving up on this content stream");
	break;
      }

    // got an argument - save it
    } else if (numArgs < maxArgs) {
      args[numArgs++] = obj;

    // too many arguments - something is wrong
    } else {
      error(errSyntaxError, getPos(), "Too many args in content stream");
      if (printCommands) {
	printf("throwing away arg: ");
	obj.print(stdout);
	printf("\n");
	fflush(stdout);
      }
      obj.free();
    }

    // grab the next object
    getContentObj(&obj);
  }
  obj.free();

  // args at end with no command
  if (numArgs > 0) {
    if (!aborted) {
      error(errSyntaxError, getPos(), "Leftover args in content stream");
      if (printCommands) {
	printf("%d leftovers:", numArgs);
	for (i = 0; i < numArgs; ++i) {
	  printf(" ");
	  args[i].print(stdout);
	}
	printf("\n");
	fflush(stdout);
      }
    }
    for (i = 0; i < numArgs; ++i) {
      args[i].free();
    }
  }
}

void Gfx::getContentObj(Object *obj) {
  parser->getObj(obj);
  if (obj->isRef()) {
    error(errSyntaxError, getPos(), "Indirect reference in content stream");
    obj->free();
    obj->initError();
  }
}

// Returns true if successful, false on error.
GBool Gfx::execOp(Object *cmd, Object args[], int numArgs) {
  Operator *op;
  char *name;
  Object *argPtr;
  int i;

  // find operator
  name = cmd->getCmd();
  if (!(op = findOp(name))) {
    if (ignoreUndef > 0) {
      return gTrue;
    }
    error(errSyntaxError, getPos(), "Unknown operator '{0:s}'", name);
    return gFalse;
  }

  // type check args
  argPtr = args;
  if (op->numArgs >= 0) {
    if (numArgs < op->numArgs) {
      error(errSyntaxError, getPos(),
	    "Too few ({0:d}) args to '{1:s}' operator", numArgs, name);
      return gFalse;
    }
    if (numArgs > op->numArgs) {
#if 0
      error(errSyntaxWarning, getPos(),
	    "Too many ({0:d}) args to '{1:s}' operator", numArgs, name);
#endif
      argPtr += numArgs - op->numArgs;
      numArgs = op->numArgs;
    }
  } else {
    if (numArgs > -op->numArgs) {
      error(errSyntaxWarning, getPos(),
	    "Too many ({0:d}) args to '{1:s}' operator",
	    numArgs, name);
    }
  }
  for (i = 0; i < numArgs; ++i) {
    if (!checkArg(&argPtr[i], op->tchk[i])) {
      error(errSyntaxError, getPos(),
	    "Arg #{0:d} to '{1:s}' operator is wrong type ({2:s})",
	    i, name, argPtr[i].getTypeName());
      return gFalse;
    }
  }

  // do it
  (this->*op->func)(argPtr, numArgs);

  return gTrue;
}

Operator *Gfx::findOp(char *name) {
  int a, b, m, cmp;

  a = -1;
  b = numOps;
  cmp = 0; // make gcc happy
  // invariant: opTab[a] < name < opTab[b]
  while (b - a > 1) {
    m = (a + b) / 2;
    cmp = strcmp(opTab[m].name, name);
    if (cmp < 0)
      a = m;
    else if (cmp > 0)
      b = m;
    else
      a = b = m;
  }
  if (cmp != 0)
    return NULL;
  return &opTab[a];
}

GBool Gfx::checkArg(Object *arg, TchkType type) {
  switch (type) {
  case tchkBool:   return arg->isBool();
  case tchkInt:    return arg->isInt();
  case tchkNum:    return arg->isNum();
  case tchkString: return arg->isString();
  case tchkName:   return arg->isName();
  case tchkArray:  return arg->isArray();
  case tchkProps:  return arg->isDict() || arg->isName();
  case tchkSCN:    return arg->isNum() || arg->isName();
  case tchkNone:   return gFalse;
  }
  return gFalse;
}

GFileOffset Gfx::getPos() {
  return parser ? parser->getPos() : -1;
}

//------------------------------------------------------------------------
// graphics state operators
//------------------------------------------------------------------------

void Gfx::opSave(Object args[], int numArgs) {
  saveState();
}

void Gfx::opRestore(Object args[], int numArgs) {
  restoreState();
}

void Gfx::opConcat(Object args[], int numArgs) {
  state->concatCTM(args[0].getNum(), args[1].getNum(),
		   args[2].getNum(), args[3].getNum(),
		   args[4].getNum(), args[5].getNum());
  out->updateCTM(state, args[0].getNum(), args[1].getNum(),
		 args[2].getNum(), args[3].getNum(),
		 args[4].getNum(), args[5].getNum());
  fontChanged = gTrue;
}

void Gfx::opSetDash(Object args[], int numArgs) {
  Array *a;
  int length;
  Object obj;
  double *dash;
  int i;

  a = args[0].getArray();
  length = a->getLength();
  if (length == 0) {
    dash = NULL;
  } else {
    dash = (double *)gmallocn(length, sizeof(double));
    for (i = 0; i < length; ++i) {
      dash[i] = a->get(i, &obj)->getNum();
      obj.free();
    }
  }
  state->setLineDash(dash, length, args[1].getNum());
  out->updateLineDash(state);
}

void Gfx::opSetFlat(Object args[], int numArgs) {
  state->setFlatness((int)args[0].getNum());
  out->updateFlatness(state);
}

void Gfx::opSetLineJoin(Object args[], int numArgs) {
  int lineJoin;

  lineJoin = args[0].getInt();
  if (lineJoin < 0 || lineJoin > 2) {
    lineJoin = 0;
  }
  state->setLineJoin(lineJoin);
  out->updateLineJoin(state);
}

void Gfx::opSetLineCap(Object args[], int numArgs) {
  int lineCap;

  lineCap = args[0].getInt();
  if (lineCap < 0 || lineCap > 2) {
    lineCap = 0;
  }
  state->setLineCap(lineCap);
  out->updateLineCap(state);
}

void Gfx::opSetMiterLimit(Object args[], int numArgs) {
  state->setMiterLimit(args[0].getNum());
  out->updateMiterLimit(state);
}

void Gfx::opSetLineWidth(Object args[], int numArgs) {
  state->setLineWidth(args[0].getNum());
  out->updateLineWidth(state);
}

void Gfx::opSetExtGState(Object args[], int numArgs) {
  Object obj1, obj2, obj3, objRef3, obj4, obj5, backdropColorObj;
  Object args2[2];
  GfxBlendMode mode;
  GBool haveFillOP;
  Function *funcs[4];
  GBool alpha, knockout;
  double opac;
  int i;

  if (!res->lookupGState(args[0].getName(), &obj1)) {
    return;
  }
  if (!obj1.isDict()) {
    error(errSyntaxError, getPos(),
	  "ExtGState '{0:s}' is wrong type", args[0].getName());
    obj1.free();
    return;
  }
  if (printCommands) {
    printf("  gfx state dict: ");
    obj1.print();
    printf("\n");
  }

  // parameters that are also set by individual PDF operators
  if (obj1.dictLookup("LW", &obj2)->isNum()) {
    opSetLineWidth(&obj2, 1);
  }
  obj2.free();
  if (obj1.dictLookup("LC", &obj2)->isInt()) {
    opSetLineCap(&obj2, 1);
  }
  obj2.free();
  if (obj1.dictLookup("LJ", &obj2)->isInt()) {
    opSetLineJoin(&obj2, 1);
  }
  obj2.free();
  if (obj1.dictLookup("ML", &obj2)->isNum()) {
    opSetMiterLimit(&obj2, 1);
  }
  obj2.free();
  if (obj1.dictLookup("D", &obj2)->isArray() &&
      obj2.arrayGetLength() == 2) {
    obj2.arrayGet(0, &args2[0]);
    obj2.arrayGet(1, &args2[1]);
    if (args2[0].isArray() && args2[1].isNum()) {
      opSetDash(args2, 2);
    }
    args2[0].free();
    args2[1].free();
  }
  obj2.free();
  if (obj1.dictLookup("FL", &obj2)->isNum()) {
    opSetFlat(&obj2, 1);
  }
  obj2.free();
  if (obj1.dictLookup("RI", &obj2)->isName()) {
    opSetRenderingIntent(&obj2, 1);
  }
  obj2.free();

  // font
  if (obj1.dictLookup("Font", &obj2)->isArray() &&
      obj2.arrayGetLength() == 2) {
    obj2.arrayGetNF(0, &obj3);
    obj2.arrayGetNF(1, &obj4);
    if (obj3.isRef() && obj4.isNum()) {
      doSetFont(res->lookupFontByRef(obj3.getRef()), obj4.getNum());
    }
    obj3.free();
    obj4.free();
  }
  obj2.free();

  // transparency support: blend mode, fill/stroke opacity
  if (!obj1.dictLookup("BM", &obj2)->isNull()) {
    if (state->parseBlendMode(&obj2, &mode)) {
      state->setBlendMode(mode);
      out->updateBlendMode(state);
    } else {
      error(errSyntaxError, getPos(), "Invalid blend mode in ExtGState");
    }
  }
  obj2.free();
  if (obj1.dictLookup("ca", &obj2)->isNum()) {
    opac = obj2.getNum();
    state->setFillOpacity(opac < 0 ? 0 : opac > 1 ? 1 : opac);
    out->updateFillOpacity(state);
  }
  obj2.free();
  if (obj1.dictLookup("CA", &obj2)->isNum()) {
    opac = obj2.getNum();
    state->setStrokeOpacity(opac < 0 ? 0 : opac > 1 ? 1 : opac);
    out->updateStrokeOpacity(state);
  }
  obj2.free();

  // fill/stroke overprint, overprint mode
  if ((haveFillOP = (obj1.dictLookup("op", &obj2)->isBool()))) {
    if (!state->getIgnoreColorOps()) {
      state->setFillOverprint(obj2.getBool());
      out->updateFillOverprint(state);
    } else {
      error(errSyntaxWarning, getPos(), "Ignoring overprint setting"
	    " in uncolored Type 3 char or tiling pattern");
    }
  }
  obj2.free();
  if (obj1.dictLookup("OP", &obj2)->isBool()) {
    if (!state->getIgnoreColorOps()) {
      state->setStrokeOverprint(obj2.getBool());
      out->updateStrokeOverprint(state);
      if (!haveFillOP) {
	state->setFillOverprint(obj2.getBool());
	out->updateFillOverprint(state);
      }
    } else {
      error(errSyntaxWarning, getPos(), "Ignoring overprint setting"
	    " in uncolored Type 3 char or tiling pattern");
    }
  }
  obj2.free();
  if (obj1.dictLookup("OPM", &obj2)->isInt()) {
    if (!state->getIgnoreColorOps()) {
      state->setOverprintMode(obj2.getInt());
      out->updateOverprintMode(state);
    } else {
      error(errSyntaxWarning, getPos(), "Ignoring overprint setting"
	    " in uncolored Type 3 char or tiling pattern");
    }
  }
  obj2.free();

  // stroke adjust
  if (obj1.dictLookup("SA", &obj2)->isBool()) {
    state->setStrokeAdjust(obj2.getBool());
    out->updateStrokeAdjust(state);
  }
  obj2.free();

  // transfer function
  if (obj1.dictLookup("TR2", &obj2)->isNull()) {
    obj2.free();
    obj1.dictLookup("TR", &obj2);
  }
  if (!obj2.isNull()) {
    if (!state->getIgnoreColorOps()) {
      if (obj2.isName("Default") ||
	  obj2.isName("Identity")) {
	funcs[0] = funcs[1] = funcs[2] = funcs[3] = NULL;
	state->setTransfer(funcs);
	out->updateTransfer(state);
      } else if (obj2.isArray() && obj2.arrayGetLength() == 4) {
	for (i = 0; i < 4; ++i) {
	  obj2.arrayGet(i, &obj3);
	  funcs[i] = Function::parse(&obj3, 1, 1);
	  obj3.free();
	  if (!funcs[i]) {
	    break;
	  }
	}
	if (i == 4) {
	  state->setTransfer(funcs);
	  out->updateTransfer(state);
	}
      } else if (obj2.isName() || obj2.isDict() || obj2.isStream()) {
	if ((funcs[0] = Function::parse(&obj2, 1, 1))) {
	  funcs[1] = funcs[2] = funcs[3] = NULL;
	  state->setTransfer(funcs);
	  out->updateTransfer(state);
	}
      } else {
	error(errSyntaxError, getPos(),
	      "Invalid transfer function in ExtGState");
      }
    } else {
      error(errSyntaxWarning, getPos(), "Ignoring transfer function setting"
	    " in uncolored Type 3 char or tiling pattern");
    }
  }
  obj2.free();

  // soft mask
  if (!obj1.dictLookup("SMask", &obj2)->isNull()) {
    if (obj2.isName("None")) {
      out->clearSoftMask(state);
    } else if (obj2.isDict()) {
      obj2.dictLookup("S", &obj3);
      if (obj3.isName("Alpha")) {
	alpha = gTrue;
      } else if (obj3.isName("Luminosity")) {
	alpha = gFalse;
      } else {
	error(errSyntaxError, getPos(),
	      "Missing S (subtype) entry in soft mask");
	alpha = gFalse;
      }
      obj3.free();
      funcs[0] = NULL;
      if (!obj2.dictLookup("TR", &obj3)->isNull()) {
	if (obj3.isName("Default") ||
	    obj3.isName("Identity")) {
	  funcs[0] = NULL;
	} else {
	  if (!(funcs[0] = Function::parse(&obj3, 1, 1))) {
	    error(errSyntaxError, getPos(),
		  "Invalid transfer function in soft mask in ExtGState");
	    delete funcs[0];
	    funcs[0] = NULL;
	  }
	}
      }
      obj3.free();
      obj2.dictLookup("BC", &backdropColorObj);
      if (obj2.dictLookup("G", &obj3)->isStream()) {
	if (obj3.streamGetDict()->lookup("Group", &obj4)->isDict()) {
	  knockout = gFalse;
	  if (obj4.dictLookup("K", &obj5)->isBool()) {
	    knockout = obj5.getBool();
	  }
	  obj5.free();
	  obj2.dictLookupNF("G", &objRef3);
	  // it doesn't make sense for softmasks to be non-isolated,
	  // because they're blended with a backdrop color, rather
	  // than the original backdrop
	  doSoftMask(&obj3, &objRef3, alpha, gTrue, knockout, funcs[0],
		     &backdropColorObj);
	  objRef3.free();
	  if (funcs[0]) {
	    delete funcs[0];
	  }
	} else {
	  error(errSyntaxError, getPos(),
		"Invalid soft mask in ExtGState - missing group");
	}
	obj4.free();
      } else {
	error(errSyntaxError, getPos(),
	      "Invalid soft mask in ExtGState - missing group");
      }
      obj3.free();
      backdropColorObj.free();
    } else if (!obj2.isNull()) {
      error(errSyntaxError, getPos(), "Invalid soft mask in ExtGState");
    }
  }
  obj2.free();

  obj1.free();
}

void Gfx::doSoftMask(Object *str, Object *strRef, GBool alpha,
		     GBool isolated, GBool knockout,
		     Function *transferFunc, Object *backdropColorObj) {
  Dict *dict, *resDict;
  double m[6], bbox[4];
  Object obj1, obj2;
  int i;

  // check for excessive recursion
  if (formDepth > 20) {
    return;
  }

  // get stream dict
  dict = str->streamGetDict();

  // check form type
  dict->lookup("FormType", &obj1);
  if (!(obj1.isNull() || (obj1.isInt() && obj1.getInt() == 1))) {
    error(errSyntaxError, getPos(), "Unknown form type");
  }
  obj1.free();

  // get bounding box
  dict->lookup("BBox", &obj1);
  if (!obj1.isArray()) {
    obj1.free();
    error(errSyntaxError, getPos(), "Bad form bounding box");
    return;
  }
  for (i = 0; i < 4; ++i) {
    obj1.arrayGet(i, &obj2);
    bbox[i] = obj2.getNum();
    obj2.free();
  }
  obj1.free();

  // get matrix
  dict->lookup("Matrix", &obj1);
  if (obj1.isArray()) {
    for (i = 0; i < 6; ++i) {
      obj1.arrayGet(i, &obj2);
      m[i] = obj2.getNum();
      obj2.free();
    }
  } else {
    m[0] = 1; m[1] = 0;
    m[2] = 0; m[3] = 1;
    m[4] = 0; m[5] = 0;
  }
  obj1.free();

  // get resources
  dict->lookup("Resources", &obj1);
  resDict = obj1.isDict() ? obj1.getDict() : (Dict *)NULL;

  // draw it
  ++formDepth;
  drawForm(strRef, resDict, m, bbox, gTrue, gTrue, isolated, knockout,
	   alpha, transferFunc, backdropColorObj);
  --formDepth;

  obj1.free();
}

void Gfx::opSetRenderingIntent(Object args[], int numArgs) {
  GfxRenderingIntent ri;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring rendering intent setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  ri = parseRenderingIntent(args[0].getName());
  state->setRenderingIntent(ri);
  out->updateRenderingIntent(state);
}

GfxRenderingIntent Gfx::parseRenderingIntent(const char *name) {
  if (!strcmp(name, "AbsoluteColorimetric")) {
    return gfxRenderingIntentAbsoluteColorimetric;
  }
  if (!strcmp(name, "Saturation")) {
    return gfxRenderingIntentSaturation;
  }
  if (!strcmp(name, "Perceptual")) {
    return gfxRenderingIntentPerceptual;
  }
  return gfxRenderingIntentRelativeColorimetric;
}

//------------------------------------------------------------------------
// color operators
//------------------------------------------------------------------------

void Gfx::opSetFillGray(Object args[], int numArgs) {
  GfxColor color;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  state->setFillPattern(NULL);
  state->setFillColorSpace(GfxColorSpace::create(csDeviceGray));
  out->updateFillColorSpace(state);
  color.c[0] = dblToCol(args[0].getNum());
  state->setFillColor(&color);
  out->updateFillColor(state);
}

void Gfx::opSetStrokeGray(Object args[], int numArgs) {
  GfxColor color;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  state->setStrokePattern(NULL);
  state->setStrokeColorSpace(GfxColorSpace::create(csDeviceGray));
  out->updateStrokeColorSpace(state);
  color.c[0] = dblToCol(args[0].getNum());
  state->setStrokeColor(&color);
  out->updateStrokeColor(state);
}

void Gfx::opSetFillCMYKColor(Object args[], int numArgs) {
  GfxColor color;
  int i;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  state->setFillPattern(NULL);
  state->setFillColorSpace(GfxColorSpace::create(csDeviceCMYK));
  out->updateFillColorSpace(state);
  for (i = 0; i < 4; ++i) {
    color.c[i] = dblToCol(args[i].getNum());
  }
  state->setFillColor(&color);
  out->updateFillColor(state);
}

void Gfx::opSetStrokeCMYKColor(Object args[], int numArgs) {
  GfxColor color;
  int i;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  state->setStrokePattern(NULL);
  state->setStrokeColorSpace(GfxColorSpace::create(csDeviceCMYK));
  out->updateStrokeColorSpace(state);
  for (i = 0; i < 4; ++i) {
    color.c[i] = dblToCol(args[i].getNum());
  }
  state->setStrokeColor(&color);
  out->updateStrokeColor(state);
}

void Gfx::opSetFillRGBColor(Object args[], int numArgs) {
  GfxColor color;
  int i;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  state->setFillPattern(NULL);
  state->setFillColorSpace(GfxColorSpace::create(csDeviceRGB));
  out->updateFillColorSpace(state);
  for (i = 0; i < 3; ++i) {
    color.c[i] = dblToCol(args[i].getNum());
  }
  state->setFillColor(&color);
  out->updateFillColor(state);
}

void Gfx::opSetStrokeRGBColor(Object args[], int numArgs) {
  GfxColor color;
  int i;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  state->setStrokePattern(NULL);
  state->setStrokeColorSpace(GfxColorSpace::create(csDeviceRGB));
  out->updateStrokeColorSpace(state);
  for (i = 0; i < 3; ++i) {
    color.c[i] = dblToCol(args[i].getNum());
  }
  state->setStrokeColor(&color);
  out->updateStrokeColor(state);
}

void Gfx::opSetFillColorSpace(Object args[], int numArgs) {
  Object obj;
  GfxColorSpace *colorSpace;
  GfxColor color;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color space setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  state->setFillPattern(NULL);
  res->lookupColorSpace(args[0].getName(), &obj);
  if (obj.isNull()) {
    colorSpace = GfxColorSpace::parse(&args[0]
				      );
  } else {
    colorSpace = GfxColorSpace::parse(&obj
				      );
  }
  obj.free();
  if (colorSpace) {
    state->setFillColorSpace(colorSpace);
    out->updateFillColorSpace(state);
    colorSpace->getDefaultColor(&color);
    state->setFillColor(&color);
    out->updateFillColor(state);
  } else {
    error(errSyntaxError, getPos(), "Bad color space (fill)");
  }
}

void Gfx::opSetStrokeColorSpace(Object args[], int numArgs) {
  Object obj;
  GfxColorSpace *colorSpace;
  GfxColor color;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color space setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  state->setStrokePattern(NULL);
  res->lookupColorSpace(args[0].getName(), &obj);
  if (obj.isNull()) {
    colorSpace = GfxColorSpace::parse(&args[0]
				      );
  } else {
    colorSpace = GfxColorSpace::parse(&obj
				      );
  }
  obj.free();
  if (colorSpace) {
    state->setStrokeColorSpace(colorSpace);
    out->updateStrokeColorSpace(state);
    colorSpace->getDefaultColor(&color);
    state->setStrokeColor(&color);
    out->updateStrokeColor(state);
  } else {
    error(errSyntaxError, getPos(), "Bad color space (stroke)");
  }
}

// Technically, per the PDF spec, the 'sc' operator can only be used
// with device, CIE, and Indexed color spaces.  Some buggy PDF
// generators use it for DeviceN, so we also allow that (same as
// 'scn', minus the optional pattern name argument).
void Gfx::opSetFillColor(Object args[], int numArgs) {
  GfxColor color;
  int i;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  if (numArgs != state->getFillColorSpace()->getNComps()) {
    error(errSyntaxError, getPos(),
	  "Incorrect number of arguments in 'sc' command");
    return;
  }
  state->setFillPattern(NULL);
  for (i = 0; i < numArgs; ++i) {
    color.c[i] = dblToCol(args[i].getNum());
  }
  state->setFillColor(&color);
  out->updateFillColor(state);
}

// Technically, per the PDF spec, the 'SC' operator can only be used
// with device, CIE, and Indexed color spaces.  Some buggy PDF
// generators use it for DeviceN, so we also allow that (same as
// 'SCN', minus the optional pattern name argument).
void Gfx::opSetStrokeColor(Object args[], int numArgs) {
  GfxColor color;
  int i;

  if (numArgs != state->getStrokeColorSpace()->getNComps()) {
    error(errSyntaxError, getPos(),
	  "Incorrect number of arguments in 'SC' command");
    return;
  }
  state->setStrokePattern(NULL);
  for (i = 0; i < numArgs; ++i) {
    color.c[i] = dblToCol(args[i].getNum());
  }
  state->setStrokeColor(&color);
  out->updateStrokeColor(state);
}

void Gfx::opSetFillColorN(Object args[], int numArgs) {
  GfxColor color;
  GfxPattern *pattern;
  int i;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  if (state->getFillColorSpace()->getMode() == csPattern) {
    if (numArgs == 0 || !args[numArgs-1].isName()) {
      error(errSyntaxError, getPos(),
	    "Invalid arguments in 'scn' command");
      return;
    }
    if (numArgs > 1) {
      if (!((GfxPatternColorSpace *)state->getFillColorSpace())->getUnder() ||
	  numArgs - 1 != ((GfxPatternColorSpace *)state->getFillColorSpace())
	                     ->getUnder()->getNComps()) {
	error(errSyntaxError, getPos(),
	      "Incorrect number of arguments in 'scn' command");
	return;
      }
      for (i = 0; i < numArgs - 1 && i < gfxColorMaxComps; ++i) {
	if (args[i].isNum()) {
	  color.c[i] = dblToCol(args[i].getNum());
	}
      }
      state->setFillColor(&color);
      out->updateFillColor(state);
    }
    if ((pattern = res->lookupPattern(args[numArgs-1].getName()
				      ))) {
      state->setFillPattern(pattern);
    }

  } else {
    if (numArgs != state->getFillColorSpace()->getNComps()) {
      error(errSyntaxError, getPos(),
	    "Incorrect number of arguments in 'scn' command");
      return;
    }
    state->setFillPattern(NULL);
    for (i = 0; i < numArgs && i < gfxColorMaxComps; ++i) {
      if (args[i].isNum()) {
	color.c[i] = dblToCol(args[i].getNum());
      }
    }
    state->setFillColor(&color);
    out->updateFillColor(state);
  }
}

void Gfx::opSetStrokeColorN(Object args[], int numArgs) {
  GfxColor color;
  GfxPattern *pattern;
  int i;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring color setting"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }
  if (state->getStrokeColorSpace()->getMode() == csPattern) {
    if (numArgs == 0 || !args[numArgs-1].isName()) {
      error(errSyntaxError, getPos(),
	    "Invalid arguments in 'SCN' command");
      return;
    }
    if (numArgs > 1) {
      if (!((GfxPatternColorSpace *)state->getStrokeColorSpace())
	       ->getUnder() ||
	  numArgs - 1 != ((GfxPatternColorSpace *)state->getStrokeColorSpace())
	                     ->getUnder()->getNComps()) {
	error(errSyntaxError, getPos(),
	      "Incorrect number of arguments in 'SCN' command");
	return;
      }
      for (i = 0; i < numArgs - 1 && i < gfxColorMaxComps; ++i) {
	if (args[i].isNum()) {
	  color.c[i] = dblToCol(args[i].getNum());
	}
      }
      state->setStrokeColor(&color);
      out->updateStrokeColor(state);
    }
    if ((pattern = res->lookupPattern(args[numArgs-1].getName()
				      ))) {
      state->setStrokePattern(pattern);
    }

  } else {
    if (numArgs != state->getStrokeColorSpace()->getNComps()) {
      error(errSyntaxError, getPos(),
	    "Incorrect number of arguments in 'SCN' command");
      return;
    }
    state->setStrokePattern(NULL);
    for (i = 0; i < numArgs && i < gfxColorMaxComps; ++i) {
      if (args[i].isNum()) {
	color.c[i] = dblToCol(args[i].getNum());
      }
    }
    state->setStrokeColor(&color);
    out->updateStrokeColor(state);
  }
}

//------------------------------------------------------------------------
// path segment operators
//------------------------------------------------------------------------

void Gfx::opMoveTo(Object args[], int numArgs) {
  state->moveTo(args[0].getNum(), args[1].getNum());
}

void Gfx::opLineTo(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    error(errSyntaxError, getPos(), "No current point in lineto");
    return;
  }
  state->lineTo(args[0].getNum(), args[1].getNum());
}

void Gfx::opCurveTo(Object args[], int numArgs) {
  double x1, y1, x2, y2, x3, y3;

  if (!state->isCurPt()) {
    error(errSyntaxError, getPos(), "No current point in curveto");
    return;
  }
  x1 = args[0].getNum();
  y1 = args[1].getNum();
  x2 = args[2].getNum();
  y2 = args[3].getNum();
  x3 = args[4].getNum();
  y3 = args[5].getNum();
  state->curveTo(x1, y1, x2, y2, x3, y3);
}

void Gfx::opCurveTo1(Object args[], int numArgs) {
  double x1, y1, x2, y2, x3, y3;

  if (!state->isCurPt()) {
    error(errSyntaxError, getPos(), "No current point in curveto1");
    return;
  }
  x1 = state->getCurX();
  y1 = state->getCurY();
  x2 = args[0].getNum();
  y2 = args[1].getNum();
  x3 = args[2].getNum();
  y3 = args[3].getNum();
  state->curveTo(x1, y1, x2, y2, x3, y3);
}

void Gfx::opCurveTo2(Object args[], int numArgs) {
  double x1, y1, x2, y2, x3, y3;

  if (!state->isCurPt()) {
    error(errSyntaxError, getPos(), "No current point in curveto2");
    return;
  }
  x1 = args[0].getNum();
  y1 = args[1].getNum();
  x2 = args[2].getNum();
  y2 = args[3].getNum();
  x3 = x2;
  y3 = y2;
  state->curveTo(x1, y1, x2, y2, x3, y3);
}

void Gfx::opRectangle(Object args[], int numArgs) {
  double x, y, w, h;

  x = args[0].getNum();
  y = args[1].getNum();
  w = args[2].getNum();
  h = args[3].getNum();
  state->moveTo(x, y);
  state->lineTo(x + w, y);
  state->lineTo(x + w, y + h);
  state->lineTo(x, y + h);
  state->closePath();
}

void Gfx::opClosePath(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    error(errSyntaxError, getPos(), "No current point in closepath");
    return;
  }
  state->closePath();
}

//------------------------------------------------------------------------
// path painting operators
//------------------------------------------------------------------------

void Gfx::opEndPath(Object args[], int numArgs) {
  doEndPath();
}

void Gfx::opStroke(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    //error(errSyntaxError, getPos(), "No path in stroke");
    return;
  }
  if (state->isPath()) {
    if (ocState) {
      if (state->getStrokeColorSpace()->getMode() == csPattern) {
	doPatternStroke();
      } else {
	out->stroke(state);
      }
    }
  }
  doEndPath();
}

void Gfx::opCloseStroke(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    //error(errSyntaxError, getPos(), "No path in closepath/stroke");
    return;
  }
  if (state->isPath()) {
    state->closePath();
    if (ocState) {
      if (state->getStrokeColorSpace()->getMode() == csPattern) {
	doPatternStroke();
      } else {
	out->stroke(state);
      }
    }
  }
  doEndPath();
}

void Gfx::opFill(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    //error(errSyntaxError, getPos(), "No path in fill");
    return;
  }
  if (state->isPath()) {
    if (ocState) {
      if (state->getFillColorSpace()->getMode() == csPattern) {
	doPatternFill(gFalse);
      } else {
	out->fill(state);
      }
    }
  }
  doEndPath();
}

void Gfx::opEOFill(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    //error(errSyntaxError, getPos(), "No path in eofill");
    return;
  }
  if (state->isPath()) {
    if (ocState) {
      if (state->getFillColorSpace()->getMode() == csPattern) {
	doPatternFill(gTrue);
      } else {
	out->eoFill(state);
      }
    }
  }
  doEndPath();
}

void Gfx::opFillStroke(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    //error(errSyntaxError, getPos(), "No path in fill/stroke");
    return;
  }
  if (state->isPath()) {
    if (ocState) {
      if (state->getFillColorSpace()->getMode() == csPattern) {
	doPatternFill(gFalse);
      } else {
	out->fill(state);
      }
      if (state->getStrokeColorSpace()->getMode() == csPattern) {
	doPatternStroke();
      } else {
	out->stroke(state);
      }
    }
  }
  doEndPath();
}

void Gfx::opCloseFillStroke(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    //error(errSyntaxError, getPos(), "No path in closepath/fill/stroke");
    return;
  }
  if (state->isPath()) {
    state->closePath();
    if (ocState) {
      if (state->getFillColorSpace()->getMode() == csPattern) {
	doPatternFill(gFalse);
      } else {
	out->fill(state);
      }
      if (state->getStrokeColorSpace()->getMode() == csPattern) {
	doPatternStroke();
      } else {
	out->stroke(state);
      }
    }
  }
  doEndPath();
}

void Gfx::opEOFillStroke(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    //error(errSyntaxError, getPos(), "No path in eofill/stroke");
    return;
  }
  if (state->isPath()) {
    if (ocState) {
      if (state->getFillColorSpace()->getMode() == csPattern) {
	doPatternFill(gTrue);
      } else {
	out->eoFill(state);
      }
      if (state->getStrokeColorSpace()->getMode() == csPattern) {
	doPatternStroke();
      } else {
	out->stroke(state);
      }
    }
  }
  doEndPath();
}

void Gfx::opCloseEOFillStroke(Object args[], int numArgs) {
  if (!state->isCurPt()) {
    //error(errSyntaxError, getPos(), "No path in closepath/eofill/stroke");
    return;
  }
  if (state->isPath()) {
    state->closePath();
    if (ocState) {
      if (state->getFillColorSpace()->getMode() == csPattern) {
	doPatternFill(gTrue);
      } else {
	out->eoFill(state);
      }
      if (state->getStrokeColorSpace()->getMode() == csPattern) {
	doPatternStroke();
      } else {
	out->stroke(state);
      }
    }
  }
  doEndPath();
}

void Gfx::doPatternFill(GBool eoFill) {
  GfxPattern *pattern;

  // this is a bit of a kludge -- patterns can be really slow, so we
  // skip them if we're only doing text extraction, since they almost
  // certainly don't contain any text
  if (!out->needNonText()) {
    return;
  }

  if (!(pattern = state->getFillPattern())) {
    return;
  }
  switch (pattern->getType()) {
  case 1:
    doTilingPatternFill((GfxTilingPattern *)pattern, gFalse, eoFill, gFalse);
    break;
  case 2:
    doShadingPatternFill((GfxShadingPattern *)pattern, gFalse, eoFill, gFalse);
    break;
  default:
    error(errSyntaxError, getPos(), "Unknown pattern type ({0:d}) in fill",
	  pattern->getType());
    break;
  }
}

void Gfx::doPatternStroke() {
  GfxPattern *pattern;

  // this is a bit of a kludge -- patterns can be really slow, so we
  // skip them if we're only doing text extraction, since they almost
  // certainly don't contain any text
  if (!out->needNonText()) {
    return;
  }

  if (!(pattern = state->getStrokePattern())) {
    return;
  }
  switch (pattern->getType()) {
  case 1:
    doTilingPatternFill((GfxTilingPattern *)pattern, gTrue, gFalse, gFalse);
    break;
  case 2:
    doShadingPatternFill((GfxShadingPattern *)pattern, gTrue, gFalse, gFalse);
    break;
  default:
    error(errSyntaxError, getPos(), "Unknown pattern type ({0:d}) in stroke",
	  pattern->getType());
    break;
  }
}

void Gfx::doPatternText() {
  GfxPattern *pattern;

  // this is a bit of a kludge -- patterns can be really slow, so we
  // skip them if we're only doing text extraction, since they almost
  // certainly don't contain any text
  if (!out->needNonText()) {
    return;
  }

  if (!(pattern = state->getFillPattern())) {
    return;
  }
  switch (pattern->getType()) {
  case 1:
    doTilingPatternFill((GfxTilingPattern *)pattern, gFalse, gFalse, gTrue);
    break;
  case 2:
    doShadingPatternFill((GfxShadingPattern *)pattern, gFalse, gFalse, gTrue);
    break;
  default:
    error(errSyntaxError, getPos(), "Unknown pattern type ({0:d}) in fill",
	  pattern->getType());
    break;
  }
}

void Gfx::doPatternImageMask(Object *ref, Stream *str, int width, int height,
			     GBool invert, GBool inlineImg, GBool interpolate) {
  saveState();

  out->setSoftMaskFromImageMask(state, ref, str,
				width, height, invert, inlineImg, interpolate);

  state->clearPath();
  state->moveTo(0, 0);
  state->lineTo(1, 0);
  state->lineTo(1, 1);
  state->lineTo(0, 1);
  state->closePath();
  doPatternFill(gTrue);
  state->clearPath();

  restoreState();
}

void Gfx::doTilingPatternFill(GfxTilingPattern *tPat,
			      GBool stroke, GBool eoFill, GBool text) {
  GfxPatternColorSpace *patCS;
  GfxColorSpace *cs;
  GfxColor color;
  GfxState *savedState;
  double xMin, yMin, xMax, yMax, x, y, x1, y1, t;
  double cxMin, cyMin, cxMax, cyMax;
  int xi0, yi0, xi1, yi1, xi, yi;
  double *ctm, *btm, *ptm;
  double bbox[4], m[6], ictm[6], m1[6], imb[6];
  double det;
  double xstep, ystep;
  int abortCheckCounter, i;

  // get color space
  patCS = (GfxPatternColorSpace *)(stroke ? state->getStrokeColorSpace()
				          : state->getFillColorSpace());

  // construct a (pattern space) -> (current space) transform matrix
  ctm = state->getCTM();
  btm = baseMatrix;
  ptm = tPat->getMatrix();
  // iCTM = invert CTM
  det = ctm[0] * ctm[3] - ctm[1] * ctm[2];
  if (fabs(det) <= 1e-10) {
    error(errSyntaxError, getPos(), "Singular matrix in tiling pattern fill");
    return;
  }
  det = 1 / det;
  ictm[0] = ctm[3] * det;
  ictm[1] = -ctm[1] * det;
  ictm[2] = -ctm[2] * det;
  ictm[3] = ctm[0] * det;
  ictm[4] = (ctm[2] * ctm[5] - ctm[3] * ctm[4]) * det;
  ictm[5] = (ctm[1] * ctm[4] - ctm[0] * ctm[5]) * det;
  // m1 = PTM * BTM = PTM * base transform matrix
  m1[0] = ptm[0] * btm[0] + ptm[1] * btm[2];
  m1[1] = ptm[0] * btm[1] + ptm[1] * btm[3];
  m1[2] = ptm[2] * btm[0] + ptm[3] * btm[2];
  m1[3] = ptm[2] * btm[1] + ptm[3] * btm[3];
  m1[4] = ptm[4] * btm[0] + ptm[5] * btm[2] + btm[4];
  m1[5] = ptm[4] * btm[1] + ptm[5] * btm[3] + btm[5];
  // m = m1 * iCTM = (PTM * BTM) * (iCTM)
  m[0] = m1[0] * ictm[0] + m1[1] * ictm[2];
  m[1] = m1[0] * ictm[1] + m1[1] * ictm[3];
  m[2] = m1[2] * ictm[0] + m1[3] * ictm[2];
  m[3] = m1[2] * ictm[1] + m1[3] * ictm[3];
  m[4] = m1[4] * ictm[0] + m1[5] * ictm[2] + ictm[4];
  m[5] = m1[4] * ictm[1] + m1[5] * ictm[3] + ictm[5];

  // construct a (device space) -> (pattern space) transform matrix
  det = m1[0] * m1[3] - m1[1] * m1[2];
  if (fabs(det) <= 1e-10) {
    error(errSyntaxError, getPos(), "Singular matrix in tiling pattern fill");
    return;
  }
  det = 1 / det;
  imb[0] = m1[3] * det;
  imb[1] = -m1[1] * det;
  imb[2] = -m1[2] * det;
  imb[3] = m1[0] * det;
  imb[4] = (m1[2] * m1[5] - m1[3] * m1[4]) * det;
  imb[5] = (m1[1] * m1[4] - m1[0] * m1[5]) * det;

  // save current graphics state
  savedState = saveStateStack();

  // set underlying color space (for uncolored tiling patterns); set
  // various other parameters (stroke color, line width) to match
  // Adobe's behavior
  state->setFillPattern(NULL);
  state->setStrokePattern(NULL);
  if (tPat->getPaintType() == 2 && (cs = patCS->getUnder())) {
    state->setFillColorSpace(cs->copy());
    out->updateFillColorSpace(state);
    state->setStrokeColorSpace(cs->copy());
    out->updateStrokeColorSpace(state);
    state->setStrokeColor(state->getFillColor());
    out->updateFillColor(state);
    out->updateStrokeColor(state);
    state->setIgnoreColorOps(gTrue);
  } else {
    state->setFillColorSpace(GfxColorSpace::create(csDeviceGray));
    out->updateFillColorSpace(state);
    state->getFillColorSpace()->getDefaultColor(&color);
    state->setFillColor(&color);
    out->updateFillColor(state);
    state->setStrokeColorSpace(GfxColorSpace::create(csDeviceGray));
    out->updateStrokeColorSpace(state);
    state->getStrokeColorSpace()->getDefaultColor(&color);
    state->setStrokeColor(&color);
    out->updateStrokeColor(state);
  }
  if (!stroke) {
    state->setLineWidth(0);
    out->updateLineWidth(state);
    state->setLineDash(NULL, 0, 0);
    out->updateLineDash(state);
  }

  // clip to current path
  if (stroke) {
    state->clipToStrokePath();
    out->clipToStrokePath(state);
  } else if (!text) {
    state->clip();
    if (eoFill) {
      out->eoClip(state);
    } else {
      out->clip(state);
    }
  }
  state->clearPath();

  // get the clip region, check for empty
  state->getClipBBox(&cxMin, &cyMin, &cxMax, &cyMax);
  if (cxMin > cxMax || cyMin > cyMax) {
    goto err;
  }

  // transform clip region bbox to pattern space
  xMin = xMax = cxMin * imb[0] + cyMin * imb[2] + imb[4];
  yMin = yMax = cxMin * imb[1] + cyMin * imb[3] + imb[5];
  x1 = cxMin * imb[0] + cyMax * imb[2] + imb[4];
  y1 = cxMin * imb[1] + cyMax * imb[3] + imb[5];
  if (x1 < xMin) {
    xMin = x1;
  } else if (x1 > xMax) {
    xMax = x1;
  }
  if (y1 < yMin) {
    yMin = y1;
  } else if (y1 > yMax) {
    yMax = y1;
  }
  x1 = cxMax * imb[0] + cyMin * imb[2] + imb[4];
  y1 = cxMax * imb[1] + cyMin * imb[3] + imb[5];
  if (x1 < xMin) {
    xMin = x1;
  } else if (x1 > xMax) {
    xMax = x1;
  }
  if (y1 < yMin) {
    yMin = y1;
  } else if (y1 > yMax) {
    yMax = y1;
  }
  x1 = cxMax * imb[0] + cyMax * imb[2] + imb[4];
  y1 = cxMax * imb[1] + cyMax * imb[3] + imb[5];
  if (x1 < xMin) {
    xMin = x1;
  } else if (x1 > xMax) {
    xMax = x1;
  }
  if (y1 < yMin) {
    yMin = y1;
  } else if (y1 > yMax) {
    yMax = y1;
  }

  // draw the pattern
  //~ this should treat negative steps differently -- start at right/top
  //~ edge instead of left/bottom (?)
  bbox[0] = tPat->getBBox()[0];
  bbox[1] = tPat->getBBox()[1];
  bbox[2] = tPat->getBBox()[2];
  bbox[3] = tPat->getBBox()[3];
  if (bbox[0] > bbox[2]) {
    t = bbox[0]; bbox[0] = bbox[2]; bbox[2] = t;
  }
  if (bbox[1] > bbox[3]) {
    t = bbox[1]; bbox[1] = bbox[3]; bbox[3] = t;
  }
  xstep = fabs(tPat->getXStep());
  ystep = fabs(tPat->getYStep());
  xi0 = (int)ceil((xMin - bbox[2]) / xstep);
  xi1 = (int)floor((xMax - bbox[0]) / xstep) + 1;
  yi0 = (int)ceil((yMin - bbox[3]) / ystep);
  yi1 = (int)floor((yMax - bbox[1]) / ystep) + 1;
  for (i = 0; i < 4; ++i) {
    m1[i] = m[i];
  }
  if (out->useTilingPatternFill()) {
    m1[4] = m[4];
    m1[5] = m[5];
    out->tilingPatternFill(state, this, tPat->getContentStreamRef(),
			   tPat->getPaintType(), tPat->getTilingType(),
			   tPat->getResDict(),
			   m1, bbox,
			   xi0, yi0, xi1, yi1, xstep, ystep);
  } else {
    abortCheckCounter = 0;
    for (yi = yi0; yi < yi1; ++yi) {
      for (xi = xi0; xi < xi1; ++xi) {
	if (abortCheckCbk) {
	  ++abortCheckCounter;
	  if (abortCheckCounter > 100) {
	    if ((*abortCheckCbk)(abortCheckCbkData)) {
	      goto err;
	    }
	    abortCheckCounter = 0;
	  }
	}
	x = xi * xstep;
	y = yi * ystep;
	m1[4] = x * m[0] + y * m[2] + m[4];
	m1[5] = x * m[1] + y * m[3] + m[5];
	drawForm(tPat->getContentStreamRef(), tPat->getResDict(),
		 m1, bbox);
      }
    }
  }

  // restore graphics state
 err:
  restoreStateStack(savedState);
}

void Gfx::doShadingPatternFill(GfxShadingPattern *sPat,
			       GBool stroke, GBool eoFill, GBool text) {
  GfxShading *shading;
  GfxState *savedState;
  double *ctm, *btm, *ptm;
  double m[6], ictm[6], m1[6];
  double xMin, yMin, xMax, yMax;
  double det;

  shading = sPat->getShading();

  // save current graphics state
  savedState = saveStateStack();

  // clip to current path
  if (stroke) {
    state->clipToStrokePath();
    out->clipToStrokePath(state);
    state->setFillOverprint(state->getStrokeOverprint());
  } else if (!text) {
    state->clip();
    if (eoFill) {
      out->eoClip(state);
    } else {
      out->clip(state);
    }
  }
  state->clearPath();

  // construct a (pattern space) -> (current space) transform matrix
  ctm = state->getCTM();
  btm = baseMatrix;
  ptm = sPat->getMatrix();
  // iCTM = invert CTM
  det = ctm[0] * ctm[3] - ctm[1] * ctm[2];
  if (fabs(det) <= 1e-10) {
    error(errSyntaxError, getPos(), "Singular matrix in shading pattern fill");
    restoreStateStack(savedState);
    return;
  }
  det = 1 / det;
  ictm[0] = ctm[3] * det;
  ictm[1] = -ctm[1] * det;
  ictm[2] = -ctm[2] * det;
  ictm[3] = ctm[0] * det;
  ictm[4] = (ctm[2] * ctm[5] - ctm[3] * ctm[4]) * det;
  ictm[5] = (ctm[1] * ctm[4] - ctm[0] * ctm[5]) * det;
  // m1 = PTM * BTM = PTM * base transform matrix
  m1[0] = ptm[0] * btm[0] + ptm[1] * btm[2];
  m1[1] = ptm[0] * btm[1] + ptm[1] * btm[3];
  m1[2] = ptm[2] * btm[0] + ptm[3] * btm[2];
  m1[3] = ptm[2] * btm[1] + ptm[3] * btm[3];
  m1[4] = ptm[4] * btm[0] + ptm[5] * btm[2] + btm[4];
  m1[5] = ptm[4] * btm[1] + ptm[5] * btm[3] + btm[5];
  // m = m1 * iCTM = (PTM * BTM) * (iCTM)
  m[0] = m1[0] * ictm[0] + m1[1] * ictm[2];
  m[1] = m1[0] * ictm[1] + m1[1] * ictm[3];
  m[2] = m1[2] * ictm[0] + m1[3] * ictm[2];
  m[3] = m1[2] * ictm[1] + m1[3] * ictm[3];
  m[4] = m1[4] * ictm[0] + m1[5] * ictm[2] + ictm[4];
  m[5] = m1[4] * ictm[1] + m1[5] * ictm[3] + ictm[5];

  // set the new matrix
  state->concatCTM(m[0], m[1], m[2], m[3], m[4], m[5]);
  out->updateCTM(state, m[0], m[1], m[2], m[3], m[4], m[5]);

  // clip to bbox
  if (shading->getHasBBox()) {
    shading->getBBox(&xMin, &yMin, &xMax, &yMax);
    state->moveTo(xMin, yMin);
    state->lineTo(xMax, yMin);
    state->lineTo(xMax, yMax);
    state->lineTo(xMin, yMax);
    state->closePath();
    state->clip();
    out->clip(state);
    state->clearPath();
  }

  // set the color space
  state->setFillColorSpace(shading->getColorSpace()->copy());
  out->updateFillColorSpace(state);

  // background color fill
  if (shading->getHasBackground()) {
    state->setFillColor(shading->getBackground());
    out->updateFillColor(state);
    state->getUserClipBBox(&xMin, &yMin, &xMax, &yMax);
    state->moveTo(xMin, yMin);
    state->lineTo(xMax, yMin);
    state->lineTo(xMax, yMax);
    state->lineTo(xMin, yMax);
    state->closePath();
    out->fill(state);
    state->clearPath();
  }

  // perform the fill
  doShFill(shading);

  // restore graphics state
  restoreStateStack(savedState);
}

void Gfx::opShFill(Object args[], int numArgs) {
  GfxShading *shading;
  GfxState *savedState;
  double xMin, yMin, xMax, yMax;

  if (state->getIgnoreColorOps()) {
    error(errSyntaxWarning, getPos(), "Ignoring shaded fill"
	  " in uncolored Type 3 char or tiling pattern");
    return;
  }

  if (!out->needNonText()) {
    return;
  }

  if (!ocState) {
    return;
  }

  if (!(shading = res->lookupShading(args[0].getName()
				     ))) {
    return;
  }

  // save current graphics state
  savedState = saveStateStack();

  // clip to bbox
  if (shading->getHasBBox()) {
    shading->getBBox(&xMin, &yMin, &xMax, &yMax);
    state->moveTo(xMin, yMin);
    state->lineTo(xMax, yMin);
    state->lineTo(xMax, yMax);
    state->lineTo(xMin, yMax);
    state->closePath();
    state->clip();
    out->clip(state);
    state->clearPath();
  }

  // set the color space
  state->setFillColorSpace(shading->getColorSpace()->copy());
  out->updateFillColorSpace(state);

  // perform the fill
  doShFill(shading);

  // restore graphics state
  restoreStateStack(savedState);

  delete shading;
}

void Gfx::doShFill(GfxShading *shading) {
  if (out->shadedFill(state, shading)) {
    return;
  }

  // do shading type-specific operations
  switch (shading->getType()) {
  case 1:
    doFunctionShFill((GfxFunctionShading *)shading);
    break;
  case 2:
    doAxialShFill((GfxAxialShading *)shading);
    break;
  case 3:
    doRadialShFill((GfxRadialShading *)shading);
    break;
  case 4:
  case 5:
    doGouraudTriangleShFill((GfxGouraudTriangleShading *)shading);
    break;
  case 6:
  case 7:
    doPatchMeshShFill((GfxPatchMeshShading *)shading);
    break;
  }
}

void Gfx::doFunctionShFill(GfxFunctionShading *shading) {
  double x0, y0, x1, y1;
  GfxColor colors[4];

  shading->getDomain(&x0, &y0, &x1, &y1);
  shading->getColor(x0, y0, &colors[0]);
  shading->getColor(x0, y1, &colors[1]);
  shading->getColor(x1, y0, &colors[2]);
  shading->getColor(x1, y1, &colors[3]);
  doFunctionShFill1(shading, x0, y0, x1, y1, colors, 0);
}

void Gfx::doFunctionShFill1(GfxFunctionShading *shading,
			    double x0, double y0,
			    double x1, double y1,
			    GfxColor *colors, int depth) {
  GfxColor fillColor;
  GfxColor color0M, color1M, colorM0, colorM1, colorMM;
  GfxColor colors2[4];
  double *matrix;
  double xM, yM;
  int nComps, i, j;

  nComps = shading->getColorSpace()->getNComps();
  matrix = shading->getMatrix();

  // compare the four corner colors
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < nComps; ++j) {
      if (abs(colors[i].c[j] - colors[(i+1)&3].c[j]) > functionColorDelta) {
	break;
      }
    }
    if (j < nComps) {
      break;
    }
  }

  // center of the rectangle
  xM = 0.5 * (x0 + x1);
  yM = 0.5 * (y0 + y1);

  // the four corner colors are close (or we hit the recursive limit)
  // -- fill the rectangle; but require at least one subdivision
  // (depth==0) to avoid problems when the four outer corners of the
  // shaded region are the same color
  if ((i == 4 && depth > 0) || depth == functionMaxDepth) {

    // use the center color
    shading->getColor(xM, yM, &fillColor);
    state->setFillColor(&fillColor);
    out->updateFillColor(state);

    // fill the rectangle
    state->moveTo(x0 * matrix[0] + y0 * matrix[2] + matrix[4],
		  x0 * matrix[1] + y0 * matrix[3] + matrix[5]);
    state->lineTo(x1 * matrix[0] + y0 * matrix[2] + matrix[4],
		  x1 * matrix[1] + y0 * matrix[3] + matrix[5]);
    state->lineTo(x1 * matrix[0] + y1 * matrix[2] + matrix[4],
		  x1 * matrix[1] + y1 * matrix[3] + matrix[5]);
    state->lineTo(x0 * matrix[0] + y1 * matrix[2] + matrix[4],
		  x0 * matrix[1] + y1 * matrix[3] + matrix[5]);
    state->closePath();
    out->fill(state);
    state->clearPath();

  // the four corner colors are not close enough -- subdivide the
  // rectangle
  } else {

    // colors[0]       colorM0       colors[2]
    //   (x0,y0)       (xM,y0)       (x1,y0)
    //         +----------+----------+
    //         |          |          |
    //         |    UL    |    UR    |
    // color0M |       colorMM       | color1M
    // (x0,yM) +----------+----------+ (x1,yM)
    //         |       (xM,yM)       |
    //         |    LL    |    LR    |
    //         |          |          |
    //         +----------+----------+
    // colors[1]       colorM1       colors[3]
    //   (x0,y1)       (xM,y1)       (x1,y1)

    shading->getColor(x0, yM, &color0M);
    shading->getColor(x1, yM, &color1M);
    shading->getColor(xM, y0, &colorM0);
    shading->getColor(xM, y1, &colorM1);
    shading->getColor(xM, yM, &colorMM);

    // upper-left sub-rectangle
    colors2[0] = colors[0];
    colors2[1] = color0M;
    colors2[2] = colorM0;
    colors2[3] = colorMM;
    doFunctionShFill1(shading, x0, y0, xM, yM, colors2, depth + 1);
    
    // lower-left sub-rectangle
    colors2[0] = color0M;
    colors2[1] = colors[1];
    colors2[2] = colorMM;
    colors2[3] = colorM1;
    doFunctionShFill1(shading, x0, yM, xM, y1, colors2, depth + 1);
    
    // upper-right sub-rectangle
    colors2[0] = colorM0;
    colors2[1] = colorMM;
    colors2[2] = colors[2];
    colors2[3] = color1M;
    doFunctionShFill1(shading, xM, y0, x1, yM, colors2, depth + 1);

    // lower-right sub-rectangle
    colors2[0] = colorMM;
    colors2[1] = colorM1;
    colors2[2] = color1M;
    colors2[3] = colors[3];
    doFunctionShFill1(shading, xM, yM, x1, y1, colors2, depth + 1);
  }
}

void Gfx::doAxialShFill(GfxAxialShading *shading) {
  double xMin, yMin, xMax, yMax;
  double x0, y0, x1, y1;
  double dx, dy, mul;
  GBool dxdyZero, horiz;
  double tMin, tMax, tMinExt, tMaxExt, t, tx, ty;
  double sMin, sMax, tmp;
  double ux0, uy0, ux1, uy1, vx0, vy0, vx1, vy1;
  double t0, t1, tt;
  GfxColor colors[axialSplits];
  int abortCheckCounter, nComps, i, j, k;

  // get the clip region bbox
  state->getUserClipBBox(&xMin, &yMin, &xMax, &yMax);

  // compute min and max t values, based on the four corners of the
  // clip region bbox
  shading->getCoords(&x0, &y0, &x1, &y1);
  dx = x1 - x0;
  dy = y1 - y0;
  dxdyZero = fabs(dx) < 0.0001 && fabs(dy) < 0.0001;
  horiz = fabs(dy) < fabs(dx);
  if (dxdyZero) {
    tMinExt = tMaxExt = 0;
    tMin = tMax = 0;
  } else {
    mul = 1 / (dx * dx + dy * dy);
    tMinExt = tMaxExt = ((xMin - x0) * dx + (yMin - y0) * dy) * mul;
    t = ((xMin - x0) * dx + (yMax - y0) * dy) * mul;
    if (t < tMinExt) {
      tMinExt = t;
    } else if (t > tMaxExt) {
      tMaxExt = t;
    }
    t = ((xMax - x0) * dx + (yMin - y0) * dy) * mul;
    if (t < tMinExt) {
      tMinExt = t;
    } else if (t > tMaxExt) {
      tMaxExt = t;
    }
    t = ((xMax - x0) * dx + (yMax - y0) * dy) * mul;
    if (t < tMinExt) {
      tMinExt = t;
    } else if (t > tMaxExt) {
      tMaxExt = t;
    }
    if ((tMin = tMinExt) < 0) {
      tMin = 0;
    }
    if (!shading->getExtend0()) {
      tMinExt = tMin;
    }
    if ((tMax = tMaxExt) > 1) {
      tMax = 1;
    }
    if (!shading->getExtend1()) {
      tMaxExt = tMax;
    }
  }

  // get the function domain
  t0 = shading->getDomain0();
  t1 = shading->getDomain1();

  // Traverse the t axis and do the shading.
  //
  // For each point (tx, ty) on the t axis, consider a line through
  // that point perpendicular to the t axis:
  //
  //     x(s) = tx + s * -dy   -->   s = (x - tx) / -dy
  //     y(s) = ty + s * dx    -->   s = (y - ty) / dx
  //
  // Then look at the intersection of this line with the bounding box
  // (xMin, yMin, xMax, yMax).  For -1 < |dy/dx| < 1, look at the
  // intersection with yMin, yMax:
  //
  //     s0 = (yMin - ty) / dx
  //     s1 = (yMax - ty) / dx
  //
  // else look at the intersection with xMin, xMax:
  //
  //     s0 = (xMin - tx) / -dy
  //     s1 = (xMax - tx) / -dy
  //
  // Each filled polygon is bounded by two of these line segments
  // perpdendicular to the t axis.
  //
  // The t axis is bisected into smaller regions until the color
  // difference across a region is small enough, and then the region
  // is painted with a single color.

  // compute the coordinates of the point on the t axis at t = tMin;
  // then compute the intersection of the perpendicular line with the
  // bounding box
  tx = x0 + tMin * dx;
  ty = y0 + tMin * dy;
  if (dxdyZero) {
    sMin = sMax = 0;
  } else {
    if (horiz) {
      sMin = (yMin - ty) / dx;
      sMax = (yMax - ty) / dx;
    } else {
      sMin = (xMin - tx) / -dy;
      sMax = (xMax - tx) / -dy;
    }
    if (sMin > sMax) {
      tmp = sMin; sMin = sMax; sMax = tmp;
    }
  }
  ux0 = tx - sMin * dy;
  uy0 = ty + sMin * dx;
  vx0 = tx - sMax * dy;
  vy0 = ty + sMax * dx;

  // fill the extension at t0
  if (shading->getExtend0() && tMinExt < tMin) {

    // compute the color at t0
    shading->getColor(t0, &colors[0]);

    // compute the coordinates of the point on the t axis at t =
    // tMinExt; then compute the intersection of the perpendicular
    // line with the bounding box
    tx = x0 + tMinExt * dx;
    ty = y0 + tMinExt * dy;
    if (dxdyZero) {
      sMin = sMax = 0;
    } else {
      if (horiz) {
	sMin = (yMin - ty) / dx;
	sMax = (yMax - ty) / dx;
      } else {
	sMin = (xMin - tx) / -dy;
	sMax = (xMax - tx) / -dy;
      }
      if (sMin > sMax) {
	tmp = sMin; sMin = sMax; sMax = tmp;
      }
    }
    ux1 = tx - sMin * dy;
    uy1 = ty + sMin * dx;
    vx1 = tx - sMax * dy;
    vy1 = ty + sMax * dx;

    // set the color
    state->setFillColor(&colors[0]);
    out->updateFillColor(state);

    // fill the region
    state->moveTo(ux1, uy1);
    state->lineTo(vx1, vy1);
    state->lineTo(vx0, vy0);
    state->lineTo(ux0, uy0);
    state->closePath();
    out->fill(state);
    state->clearPath();
  }

  // traverse the t axis, splitting [tMin, tMax] into axialSplits regions

  // compute the color in the center of each region
  for (i = 0; i < axialSplits; ++i) {
    t = tMin + (tMax - tMin) * (i + 0.5) / axialSplits;
    tt = t0 + (t1 - t0) * t;
    shading->getColor(tt, &colors[i]);
  }

  // each iteration draws one or more regions, starting at i --
  // if the colors are similar, it will combine regions i, i+1, ...
  nComps = shading->getColorSpace()->getNComps();
  abortCheckCounter = 0;
  i = 0;
  while (i < axialSplits) {

    if (abortCheckCbk) {
      ++abortCheckCounter;
      if (abortCheckCounter > 100) {
        if ((*abortCheckCbk)(abortCheckCbkData)) {
          break;
        }
        abortCheckCounter = 0;
      }
    }

    // check for similar colors
    for (j = i + 1; j < axialSplits; ++j) {
      for (k = 0; k < nComps; ++k) {
	if (abs(colors[j].c[k] - colors[i].c[k]) > axialColorDelta) {
	  break;
	}
      }
      if (k < nComps) {
	break;
      }
    }

    // compute the coordinates of the point on the t axis; then
    // compute the intersection of the perpendicular line with the
    // bounding box
    t = tMin + (tMax - tMin) * (double)j / axialSplits;
    tx = x0 + t * dx;
    ty = y0 + t * dy;
    if (dxdyZero) {
      sMin = sMax = 0;
    } else {
      if (horiz) {
	sMin = (yMin - ty) / dx;
	sMax = (yMax - ty) / dx;
      } else {
	sMin = (xMin - tx) / -dy;
	sMax = (xMax - tx) / -dy;
      }
      if (sMin > sMax) {
	tmp = sMin; sMin = sMax; sMax = tmp;
      }
    }
    ux1 = tx - sMin * dy;
    uy1 = ty + sMin * dx;
    vx1 = tx - sMax * dy;
    vy1 = ty + sMax * dx;

    // set the color
    state->setFillColor(&colors[i]);
    out->updateFillColor(state);

    // fill the region
    state->moveTo(ux0, uy0);
    state->lineTo(vx0, vy0);
    state->lineTo(vx1, vy1);
    state->lineTo(ux1, uy1);
    state->closePath();
    out->fill(state);
    state->clearPath();

    // set up for next region
    ux0 = ux1;
    uy0 = uy1;
    vx0 = vx1;
    vy0 = vy1;

    i = j;
  }

  // fill the extension at t1
  if (shading->getExtend1() && tMaxExt > tMax) {

    // compute the color at t1
    shading->getColor(t1, &colors[0]);

    // compute the coordinates of the point on the t axis at t =
    // tMaxExt; then compute the intersection of the perpendicular
    // line with the bounding box
    tx = x0 + tMaxExt * dx;
    ty = y0 + tMaxExt * dy;
    if (dxdyZero) {
      sMin = sMax = 0;
    } else {
      if (horiz) {
	sMin = (yMin - ty) / dx;
	sMax = (yMax - ty) / dx;
      } else {
	sMin = (xMin - tx) / -dy;
	sMax = (xMax - tx) / -dy;
      }
      if (sMin > sMax) {
	tmp = sMin; sMin = sMax; sMax = tmp;
      }
    }
    ux1 = tx - sMin * dy;
    uy1 = ty + sMin * dx;
    vx1 = tx - sMax * dy;
    vy1 = ty + sMax * dx;

    // set the color
    state->setFillColor(&colors[0]);
    out->updateFillColor(state);

    // fill the region
    state->moveTo(ux0, uy0);
    state->lineTo(vx0, vy0);
    state->lineTo(vx1, vy1);
    state->lineTo(ux1, uy1);
    state->closePath();
    out->fill(state);
    state->clearPath();
  }
}

#if defined(__GNUC__) && !defined(__clang__)
// this function makes a lot of sin()/cos() calls, which are slow
// with glibc 2.16 and newer on x86; accuracy isn't terribly
// important here, so tell gcc to use the fast version
#pragma GCC optimize ("fast-math")
#endif

void Gfx::doRadialShFill(GfxRadialShading *shading) {
  double xMin, yMin, xMax, yMax;
  double x0, y0, r0, x1, y1, r1, t0, t1;
  int nComps;
  GfxColor colorA, colorB;
  double xa, ya, xb, yb, ra, rb;
  double ta, tb, sa, sb;
  double sMin, sMax, h;
  double sLeft, sRight, sTop, sBottom, sZero, sDiag;
  GBool haveSLeft, haveSRight, haveSTop, haveSBottom, haveSZero;
  GBool haveSMin, haveSMax;
  GBool enclosed;
  double *ctm;
  double theta, alpha, angle, t;
  int abortCheckCounter, ia, ib, k, n;

  // get the shading info
  shading->getCoords(&x0, &y0, &r0, &x1, &y1, &r1);
  t0 = shading->getDomain0();
  t1 = shading->getDomain1();
  nComps = shading->getColorSpace()->getNComps();

  // Compute the point at which r(s) = 0; check for the enclosed
  // circles case; and compute the angles for the tangent lines.
  h = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
  if (h == 0) {
    enclosed = gTrue;
    theta = 0; // make gcc happy
  } else if (r1 - r0 == 0) {
    enclosed = gFalse;
    theta = 0;
  } else if (fabs(r1 - r0) >= h - 0.0001) {
    enclosed = gTrue;
    theta = 0; // make gcc happy
  } else {
    enclosed = gFalse;
    theta = asin((r1 - r0) / h);
  }
  if (enclosed) {
    alpha = 0;
  } else {
    alpha = atan2(y1 - y0, x1 - x0);
  }

  // compute the (possibly extended) s range
  state->getUserClipBBox(&xMin, &yMin, &xMax, &yMax);
  if (enclosed) {
    sMin = 0;
    sMax = 1;
  } else {
    // solve x(sLeft) + r(sLeft) = xMin
    if ((haveSLeft = fabs((x1 + r1) - (x0 + r0)) > 0.000001)) {
      sLeft = (xMin - (x0 + r0)) / ((x1 + r1) - (x0 + r0));
    } else {
      sLeft = 0; // make gcc happy
    }
    // solve x(sRight) - r(sRight) = xMax
    if ((haveSRight = fabs((x1 - r1) - (x0 - r0)) > 0.000001)) {
      sRight = (xMax - (x0 - r0)) / ((x1 - r1) - (x0 - r0));
    } else {
      sRight = 0; // make gcc happy
    }
    // solve y(sBottom) + r(sBottom) = yMin
    if ((haveSBottom = fabs((y1 + r1) - (y0 + r0)) > 0.000001)) {
      sBottom = (yMin - (y0 + r0)) / ((y1 + r1) - (y0 + r0));
    } else {
      sBottom = 0; // make gcc happy
    }
    // solve y(sTop) - r(sTop) = yMax
    if ((haveSTop = fabs((y1 - r1) - (y0 - r0)) > 0.000001)) {
      sTop = (yMax - (y0 - r0)) / ((y1 - r1) - (y0 - r0));
    } else {
      sTop = 0; // make gcc happy
    }
    // solve r(sZero) = 0
    if ((haveSZero = fabs(r1 - r0) > 0.000001)) {
      sZero = -r0 / (r1 - r0);
    } else {
      sZero = 0; // make gcc happy
    }
    // solve r(sDiag) = sqrt((xMax-xMin)^2 + (yMax-yMin)^2)
    if (haveSZero) {
      sDiag = (sqrt((xMax - xMin) * (xMax - xMin) +
		    (yMax - yMin) * (yMax - yMin)) - r0) / (r1 - r0);
    } else {
      sDiag = 0; // make gcc happy
    }
    // compute sMin
    if (shading->getExtend0()) {
      sMin = 0;
      haveSMin = gFalse;
      if (x0 < x1 && haveSLeft && sLeft < 0) {
	sMin = sLeft;
	haveSMin = gTrue;
      } else if (x0 > x1 && haveSRight && sRight < 0) {
	sMin = sRight;
	haveSMin = gTrue;
      }
      if (y0 < y1 && haveSBottom && sBottom < 0) {
	if (!haveSMin || sBottom > sMin) {
	  sMin = sBottom;
	  haveSMin = gTrue;
	}
      } else if (y0 > y1 && haveSTop && sTop < 0) {
	if (!haveSMin || sTop > sMin) {
	  sMin = sTop;
	  haveSMin = gTrue;
	}
      }
      if (haveSZero && sZero <= 0) {
	if (!haveSMin || sZero > sMin) {
	  sMin = sZero;
	}
      }
    } else {
      sMin = 0;
    }
    // compute sMax
    if (shading->getExtend1()) {
      sMax = 1;
      haveSMax = gFalse;
      if (x1 < x0 && haveSLeft && sLeft > 1) {
	sMax = sLeft;
	haveSMax = gTrue;
      } else if (x1 > x0 && haveSRight && sRight > 1) {
	sMax = sRight;
	haveSMax = gTrue;
      }
      if (y1 < y0 && haveSBottom && sBottom > 1) {
	if (!haveSMax || sBottom < sMax) {
	  sMax = sBottom;
	  haveSMax = gTrue;
	}
      } else if (y1 > y0 && haveSTop && sTop > 1) {
	if (!haveSMax || sTop < sMax) {
	  sMax = sTop;
	  haveSMax = gTrue;
	}
      }
      if (haveSZero && sDiag > 1) {
	if (!haveSMax || sDiag < sMax) {
	  sMax = sDiag;
	}
      }
    } else {
      sMax = 1;
    }
  }

  // compute the number of steps into which circles must be divided to
  // achieve a curve flatness of 0.1 pixel in device space for the
  // largest circle (note that "device space" is 72 dpi when generating
  // PostScript, hence the relatively small 0.1 pixel accuracy)
  ctm = state->getCTM();
  t = fabs(ctm[0]);
  if (fabs(ctm[1]) > t) {
    t = fabs(ctm[1]);
  }
  if (fabs(ctm[2]) > t) {
    t = fabs(ctm[2]);
  }
  if (fabs(ctm[3]) > t) {
    t = fabs(ctm[3]);
  }
  if (r0 > r1) {
    t *= r0;
  } else {
    t *= r1;
  }
  if (t < 1) {
    n = 3;
  } else {
    n = (int)(M_PI / acos(1 - 0.1 / t));
    if (n < 3) {
      n = 3;
    } else if (n > 200) {
      n = 200;
    }
  }

  // setup for the start circle
  ia = 0;
  sa = sMin;
  ta = t0 + sa * (t1 - t0);
  xa = x0 + sa * (x1 - x0);
  ya = y0 + sa * (y1 - y0);
  ra = r0 + sa * (r1 - r0);
  if (ta < t0) {
    shading->getColor(t0, &colorA);
  } else if (ta > t1) {
    shading->getColor(t1, &colorA);
  } else {
    shading->getColor(ta, &colorA);
  }

  // fill the circles
  abortCheckCounter = 0;
  while (ia < radialMaxSplits) {

    if (abortCheckCbk) {
      ++abortCheckCounter;
      if (abortCheckCounter > 100) {
	if ((*abortCheckCbk)(abortCheckCbkData)) {
	  break;
	}
	abortCheckCounter = 0;
      }
    }

    // go as far along the t axis (toward t1) as we can, such that the
    // color difference is within the tolerance (radialColorDelta) --
    // this uses bisection (between the current value, t, and t1),
    // limited to radialMaxSplits points along the t axis; require at
    // least one split to avoid problems when the innermost and
    // outermost colors are the same
    ib = radialMaxSplits;
    sb = sMax;
    tb = t0 + sb * (t1 - t0);
    if (tb < t0) {
      shading->getColor(t0, &colorB);
    } else if (tb > t1) {
      shading->getColor(t1, &colorB);
    } else {
      shading->getColor(tb, &colorB);
    }
    while (ib - ia > 1) {
      for (k = 0; k < nComps; ++k) {
	if (abs(colorB.c[k] - colorA.c[k]) > radialColorDelta) {
	  break;
	}
      }
      if (k == nComps && ib < radialMaxSplits) {
	break;
      }
      ib = (ia + ib) / 2;
      sb = sMin + ((double)ib / (double)radialMaxSplits) * (sMax - sMin);
      tb = t0 + sb * (t1 - t0);
      if (tb < t0) {
	shading->getColor(t0, &colorB);
      } else if (tb > t1) {
	shading->getColor(t1, &colorB);
      } else {
	shading->getColor(tb, &colorB);
      }
    }

    // compute center and radius of the circle
    xb = x0 + sb * (x1 - x0);
    yb = y0 + sb * (y1 - y0);
    rb = r0 + sb * (r1 - r0);

    // use the average of the colors at the two circles
    for (k = 0; k < nComps; ++k) {
      colorA.c[k] = (colorA.c[k] + colorB.c[k]) / 2;
    }
    state->setFillColor(&colorA);
    out->updateFillColor(state);

    if (enclosed) {

      // construct path for first circle (counterclockwise)
      state->moveTo(xa + ra, ya);
      for (k = 1; k < n; ++k) {
	angle = ((double)k / (double)n) * 2 * M_PI;
	state->lineTo(xa + ra * cos(angle), ya + ra * sin(angle));
      }
      state->closePath();

      // construct and append path for second circle (clockwise)
      state->moveTo(xb + rb, yb);
      for (k = 1; k < n; ++k) {
	angle = -((double)k / (double)n) * 2 * M_PI;
	state->lineTo(xb + rb * cos(angle), yb + rb * sin(angle));
      }
      state->closePath();

    } else {

      // construct the first subpath (clockwise)
      state->moveTo(xa + ra * cos(alpha + theta + 0.5 * M_PI),
		    ya + ra * sin(alpha + theta + 0.5 * M_PI));
      for (k = 0; k < n; ++k) {
	angle = alpha + theta + 0.5 * M_PI
	  - ((double)k / (double)n) * (2 * theta + M_PI);
	state->lineTo(xb + rb * cos(angle), yb + rb * sin(angle));
      }
      for (k = 0; k < n; ++k) {
	angle = alpha - theta - 0.5 * M_PI
	  + ((double)k / (double)n) * (2 * theta - M_PI);
	state->lineTo(xa + ra * cos(angle), ya + ra * sin(angle));
      }
      state->closePath();

      // construct the second subpath (counterclockwise)
      state->moveTo(xa + ra * cos(alpha + theta + 0.5 * M_PI),
		    ya + ra * sin(alpha + theta + 0.5 * M_PI));
      for (k = 0; k < n; ++k) {
	angle = alpha + theta + 0.5 * M_PI
	        + ((double)k / (double)n) * (-2 * theta + M_PI);
	state->lineTo(xb + rb * cos(angle), yb + rb * sin(angle));
      }
      for (k = 0; k < n; ++k) {
	angle = alpha - theta - 0.5 * M_PI
	        + ((double)k / (double)n) * (2 * theta + M_PI);
	state->lineTo(xa + ra * cos(angle), ya + ra * sin(angle));
      }
      state->closePath();
    }

    // fill the path
    out->fill(state);
    state->clearPath();

    // step to the next value of t
    ia = ib;
    sa = sb;
    ta = tb;
    xa = xb;
    ya = yb;
    ra = rb;
    colorA = colorB;
  }

  if (enclosed) {
    // extend the smaller circle
    if ((shading->getExtend0() && r0 <= r1) ||
	(shading->getExtend1() && r1 < r0)) {
      if (r0 <= r1) {
	ta = t0;
	ra = r0;
	xa = x0;
	ya = y0;
      } else {
	ta = t1;
	ra = r1;
	xa = x1;
	ya = y1;
      }
      shading->getColor(ta, &colorA);
      state->setFillColor(&colorA);
      out->updateFillColor(state);
      state->moveTo(xa + ra, ya);
      for (k = 1; k < n; ++k) {
	angle = ((double)k / (double)n) * 2 * M_PI;
	state->lineTo(xa + ra * cos(angle), ya + ra * sin(angle));
      }
      state->closePath();
      out->fill(state);
      state->clearPath();
    }

    // extend the larger circle
    if ((shading->getExtend0() && r0 > r1) ||
	(shading->getExtend1() && r1 >= r0)) {
      if (r0 > r1) {
	ta = t0;
	ra = r0;
	xa = x0;
	ya = y0;
      } else {
	ta = t1;
	ra = r1;
	xa = x1;
	ya = y1;
      }
      shading->getColor(ta, &colorA);
      state->setFillColor(&colorA);
      out->updateFillColor(state);
      state->moveTo(xMin, yMin);
      state->lineTo(xMin, yMax);
      state->lineTo(xMax, yMax);
      state->lineTo(xMax, yMin);
      state->closePath();
      state->moveTo(xa + ra, ya);
      for (k = 1; k < n; ++k) {
	angle = ((double)k / (double)n) * 2 * M_PI;
	state->lineTo(xa + ra * cos(angle), ya + ra * sin(angle));
      }
      state->closePath();
      out->fill(state);
      state->clearPath();
    }
  }
}

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC reset_options
#endif

void Gfx::doGouraudTriangleShFill(GfxGouraudTriangleShading *shading) {
  double x0, y0, x1, y1, x2, y2;
  double color0[gfxColorMaxComps];
  double color1[gfxColorMaxComps];
  double color2[gfxColorMaxComps];
  int abortCheckCounter, i;

  abortCheckCounter = 0;
  for (i = 0; i < shading->getNTriangles(); ++i) {
    if (abortCheckCbk) {
      ++abortCheckCounter;
      if (abortCheckCounter > 25) {
	if ((*abortCheckCbk)(abortCheckCbkData)) {
	  break;
	}
	abortCheckCounter = 0;
      }
    }
    shading->getTriangle(i, &x0, &y0, color0,
			 &x1, &y1, color1,
			 &x2, &y2, color2);
    gouraudFillTriangle(x0, y0, color0, x1, y1, color1, x2, y2, color2,
			shading, 0);
  }
}

void Gfx::gouraudFillTriangle(double x0, double y0, double *color0,
			      double x1, double y1, double *color1,
			      double x2, double y2, double *color2,
			      GfxGouraudTriangleShading *shading, int depth) {
  double dx0, dy0, dx1, dy1, dx2, dy2;
  double x01, y01, x12, y12, x20, y20;
  double color01[gfxColorMaxComps];
  double color12[gfxColorMaxComps];
  double color20[gfxColorMaxComps];
  GfxColor c0, c1, c2;
  int nComps, i;

  // recursion ends when:
  // (1) color difference is smaller than gouraudColorDelta; or
  // (2) triangles are smaller than 0.5 pixel (note that "device
  //     space" is 72dpi when generating PostScript); or
  // (3) max recursion depth (gouraudMaxDepth) is hit.
  nComps = shading->getColorSpace()->getNComps();
  shading->getColor(color0, &c0);
  shading->getColor(color1, &c1);
  shading->getColor(color2, &c2);
  for (i = 0; i < nComps; ++i) {
    if (abs(c0.c[i] - c1.c[i]) > gouraudColorDelta ||
	abs(c1.c[i] - c2.c[i]) > gouraudColorDelta) {
      break;
    }
  }
  state->transformDelta(x1 - x0, y1 - y0, &dx0, &dy0);
  state->transformDelta(x2 - x1, y2 - y1, &dx1, &dy1);
  state->transformDelta(x0 - x2, y0 - y2, &dx2, &dy2);
  if (i == nComps ||
      depth == gouraudMaxDepth ||
      (fabs(dx0) < 0.5 && fabs(dy0) < 0.5 &&
       fabs(dx1) < 0.5 && fabs(dy1) < 0.5 &&
       fabs(dx2) < 0.5 && fabs(dy2) < 0.5)) {
    state->setFillColor(&c0);
    out->updateFillColor(state);
    state->moveTo(x0, y0);
    state->lineTo(x1, y1);
    state->lineTo(x2, y2);
    state->closePath();
    out->fill(state);
    state->clearPath();
  } else {
    x01 = 0.5 * (x0 + x1);
    y01 = 0.5 * (y0 + y1);
    x12 = 0.5 * (x1 + x2);
    y12 = 0.5 * (y1 + y2);
    x20 = 0.5 * (x2 + x0);
    y20 = 0.5 * (y2 + y0);
    for (i = 0; i < shading->getNComps(); ++i) {
      color01[i] = 0.5 * (color0[i] + color1[i]);
      color12[i] = 0.5 * (color1[i] + color2[i]);
      color20[i] = 0.5 * (color2[i] + color0[i]);
    }
    gouraudFillTriangle(x0, y0, color0, x01, y01, color01,
			x20, y20, color20, shading, depth + 1);
    gouraudFillTriangle(x01, y01, color01, x1, y1, color1,
			x12, y12, color12, shading, depth + 1);
    gouraudFillTriangle(x01, y01, color01, x12, y12, color12,
			x20, y20, color20, shading, depth + 1);
    gouraudFillTriangle(x20, y20, color20, x12, y12, color12,
			x2, y2, color2, shading, depth + 1);
  }
}

void Gfx::doPatchMeshShFill(GfxPatchMeshShading *shading) {
  int start, abortCheckCounter, i;

  if (shading->getNPatches() > 128) {
    start = 3;
  } else if (shading->getNPatches() > 64) {
    start = 2;
  } else if (shading->getNPatches() > 16) {
    start = 1;
  } else {
    start = 0;
  }
  abortCheckCounter = 0;
  for (i = 0; i < shading->getNPatches(); ++i) {
    if (abortCheckCbk) {
      ++abortCheckCounter;
      if (abortCheckCounter > 25) {
	if ((*abortCheckCbk)(abortCheckCbkData)) {
	  break;
	}
	abortCheckCounter = 0;
      }
    }
    fillPatch(shading->getPatch(i), shading, start);
  }
}

void Gfx::fillPatch(GfxPatch *patch, GfxPatchMeshShading *shading, int depth) {
  GfxPatch patch00, patch01, patch10, patch11;
  GfxColor c00, c01, c10, c11;
  double xx[4][8], yy[4][8];
  double x, y, xMin, yMin, xMax, yMax, xxm, yym;
  int nComps, i, j;
  GBool stop;

  shading->getColor(patch->color[0][0], &c00);
  stop = gFalse;

  // stop subdivision at max depth
  if (depth == patchMaxDepth) {
    stop = gTrue;
  }

  // stop subdivision if colors are close enough
  if (!stop) {
    nComps = shading->getColorSpace()->getNComps();
    shading->getColor(patch->color[0][1], &c01);
    shading->getColor(patch->color[1][0], &c10);
    shading->getColor(patch->color[1][1], &c11);
    for (i = 0; i < nComps; ++i) {
      if (abs(c00.c[i] - c01.c[i]) > patchColorDelta ||
	  abs(c01.c[i] - c11.c[i]) > patchColorDelta ||
	  abs(c11.c[i] - c10.c[i]) > patchColorDelta ||
	  abs(c10.c[i] - c00.c[i]) > patchColorDelta) {
	break;
      }
    }
    if (i == nComps) {
      stop = gTrue;
    }
  }

  // stop subdivision if patch is small enough
  if (!stop) {
    xMin = yMin = xMax = yMax = 0;
    for (j = 0; j < 4; ++j) {
      for (i = 0; i < 4; ++i) {
	state->transformDelta(patch->x[i][j], patch->y[i][j], &x, &y);
	if (i == 0 && j == 0) {
	  xMin = xMax = x;
	  yMin = yMax = y;
	} else {
	  if (x < xMin) {
	    xMin = x;
	  } else if (x > xMax) {
	    xMax = x;
	  }
	  if (y < yMin) {
	    yMin = y;
	  } else if (y > yMax) {
	    yMax = y;
	  }
	}
      }
    }
    if (xMax - xMin < 1 && yMax - yMin < 1) {
      stop = gTrue;
    }
  }

  // draw the patch
  if (stop) {
    state->setFillColor(&c00);
    out->updateFillColor(state);
    state->moveTo(patch->x[0][0], patch->y[0][0]);
    state->curveTo(patch->x[0][1], patch->y[0][1],
		   patch->x[0][2], patch->y[0][2],
		   patch->x[0][3], patch->y[0][3]);
    state->curveTo(patch->x[1][3], patch->y[1][3],
		   patch->x[2][3], patch->y[2][3],
		   patch->x[3][3], patch->y[3][3]);
    state->curveTo(patch->x[3][2], patch->y[3][2],
		   patch->x[3][1], patch->y[3][1],
		   patch->x[3][0], patch->y[3][0]);
    state->curveTo(patch->x[2][0], patch->y[2][0],
		   patch->x[1][0], patch->y[1][0],
		   patch->x[0][0], patch->y[0][0]);
    state->closePath();
    out->fill(state);
    state->clearPath();

  // subdivide the patch
  } else {
    for (i = 0; i < 4; ++i) {
      xx[i][0] = patch->x[i][0];
      yy[i][0] = patch->y[i][0];
      xx[i][1] = 0.5 * (patch->x[i][0] + patch->x[i][1]);
      yy[i][1] = 0.5 * (patch->y[i][0] + patch->y[i][1]);
      xxm = 0.5 * (patch->x[i][1] + patch->x[i][2]);
      yym = 0.5 * (patch->y[i][1] + patch->y[i][2]);
      xx[i][6] = 0.5 * (patch->x[i][2] + patch->x[i][3]);
      yy[i][6] = 0.5 * (patch->y[i][2] + patch->y[i][3]);
      xx[i][2] = 0.5 * (xx[i][1] + xxm);
      yy[i][2] = 0.5 * (yy[i][1] + yym);
      xx[i][5] = 0.5 * (xxm + xx[i][6]);
      yy[i][5] = 0.5 * (yym + yy[i][6]);
      xx[i][3] = xx[i][4] = 0.5 * (xx[i][2] + xx[i][5]);
      yy[i][3] = yy[i][4] = 0.5 * (yy[i][2] + yy[i][5]);
      xx[i][7] = patch->x[i][3];
      yy[i][7] = patch->y[i][3];
    }
    for (i = 0; i < 4; ++i) {
      patch00.x[0][i] = xx[0][i];
      patch00.y[0][i] = yy[0][i];
      patch00.x[1][i] = 0.5 * (xx[0][i] + xx[1][i]);
      patch00.y[1][i] = 0.5 * (yy[0][i] + yy[1][i]);
      xxm = 0.5 * (xx[1][i] + xx[2][i]);
      yym = 0.5 * (yy[1][i] + yy[2][i]);
      patch10.x[2][i] = 0.5 * (xx[2][i] + xx[3][i]);
      patch10.y[2][i] = 0.5 * (yy[2][i] + yy[3][i]);
      patch00.x[2][i] = 0.5 * (patch00.x[1][i] + xxm);
      patch00.y[2][i] = 0.5 * (patch00.y[1][i] + yym);
      patch10.x[1][i] = 0.5 * (xxm + patch10.x[2][i]);
      patch10.y[1][i] = 0.5 * (yym + patch10.y[2][i]);
      patch00.x[3][i] = 0.5 * (patch00.x[2][i] + patch10.x[1][i]);
      patch00.y[3][i] = 0.5 * (patch00.y[2][i] + patch10.y[1][i]);
      patch10.x[0][i] = patch00.x[3][i];
      patch10.y[0][i] = patch00.y[3][i];
      patch10.x[3][i] = xx[3][i];
      patch10.y[3][i] = yy[3][i];
    }
    for (i = 4; i < 8; ++i) {
      patch01.x[0][i-4] = xx[0][i];
      patch01.y[0][i-4] = yy[0][i];
      patch01.x[1][i-4] = 0.5 * (xx[0][i] + xx[1][i]);
      patch01.y[1][i-4] = 0.5 * (yy[0][i] + yy[1][i]);
      xxm = 0.5 * (xx[1][i] + xx[2][i]);
      yym = 0.5 * (yy[1][i] + yy[2][i]);
      patch11.x[2][i-4] = 0.5 * (xx[2][i] + xx[3][i]);
      patch11.y[2][i-4] = 0.5 * (yy[2][i] + yy[3][i]);
      patch01.x[2][i-4] = 0.5 * (patch01.x[1][i-4] + xxm);
      patch01.y[2][i-4] = 0.5 * (patch01.y[1][i-4] + yym);
      patch11.x[1][i-4] = 0.5 * (xxm + patch11.x[2][i-4]);
      patch11.y[1][i-4] = 0.5 * (yym + patch11.y[2][i-4]);
      patch01.x[3][i-4] = 0.5 * (patch01.x[2][i-4] + patch11.x[1][i-4]);
      patch01.y[3][i-4] = 0.5 * (patch01.y[2][i-4] + patch11.y[1][i-4]);
      patch11.x[0][i-4] = patch01.x[3][i-4];
      patch11.y[0][i-4] = patch01.y[3][i-4];
      patch11.x[3][i-4] = xx[3][i];
      patch11.y[3][i-4] = yy[3][i];
    }
    for (i = 0; i < shading->getNComps(); ++i) {
      patch00.color[0][0][i] = patch->color[0][0][i];
      patch00.color[0][1][i] = 0.5 * (patch->color[0][0][i] +
				      patch->color[0][1][i]);
      patch01.color[0][0][i] = patch00.color[0][1][i];
      patch01.color[0][1][i] = patch->color[0][1][i];
      patch01.color[1][1][i] = 0.5 * (patch->color[0][1][i] +
				      patch->color[1][1][i]);
      patch11.color[0][1][i] = patch01.color[1][1][i];
      patch11.color[1][1][i] = patch->color[1][1][i];
      patch11.color[1][0][i] = 0.5 * (patch->color[1][1][i] +
				      patch->color[1][0][i]);
      patch10.color[1][1][i] = patch11.color[1][0][i];
      patch10.color[1][0][i] = patch->color[1][0][i];
      patch10.color[0][0][i] = 0.5 * (patch->color[1][0][i] +
				      patch->color[0][0][i]);
      patch00.color[1][0][i] = patch10.color[0][0][i];
      patch00.color[1][1][i] = 0.5 * (patch00.color[1][0][i] +
				      patch01.color[1][1][i]);
      patch01.color[1][0][i] = patch00.color[1][1][i];
      patch11.color[0][0][i] = patch00.color[1][1][i];
      patch10.color[0][1][i] = patch00.color[1][1][i];
    }
    fillPatch(&patch00, shading, depth + 1);
    fillPatch(&patch10, shading, depth + 1);
    fillPatch(&patch01, shading, depth + 1);
    fillPatch(&patch11, shading, depth + 1);
  }
}

void Gfx::doEndPath() {
  if (state->isCurPt() && clip != clipNone) {
    state->clip();
    if (clip == clipNormal) {
      out->clip(state);
    } else {
      out->eoClip(state);
    }
  }
  clip = clipNone;
  state->clearPath();
}

//------------------------------------------------------------------------
// path clipping operators
//------------------------------------------------------------------------

void Gfx::opClip(Object args[], int numArgs) {
  clip = clipNormal;
}

void Gfx::opEOClip(Object args[], int numArgs) {
  clip = clipEO;
}

//------------------------------------------------------------------------
// text object operators
//------------------------------------------------------------------------

void Gfx::opBeginText(Object args[], int numArgs) {
  state->setTextMat(1, 0, 0, 1, 0, 0);
  state->textMoveTo(0, 0);
  out->updateTextMat(state);
  out->updateTextPos(state);
  fontChanged = gTrue;
}

void Gfx::opEndText(Object args[], int numArgs) {
  out->endTextObject(state);
}

//------------------------------------------------------------------------
// text state operators
//------------------------------------------------------------------------

void Gfx::opSetCharSpacing(Object args[], int numArgs) {
  state->setCharSpace(args[0].getNum());
  out->updateCharSpace(state);
}

void Gfx::opSetFont(Object args[], int numArgs) {
  doSetFont(res->lookupFont(args[0].getName()), args[1].getNum());
}

void Gfx::doSetFont(GfxFont *font, double size) {
  if (!font) {
    state->setFont(NULL, 0);
    return;
  }
  if (printCommands) {
    printf("  font: tag=%s name='%s' %g\n",
	   font->getTag()->getCString(),
	   font->getName() ? font->getName()->getCString() : "???",
	   size);
    fflush(stdout);
  }
  state->setFont(font, size);
  fontChanged = gTrue;
}

void Gfx::opSetTextLeading(Object args[], int numArgs) {
  state->setLeading(args[0].getNum());
}

void Gfx::opSetTextRender(Object args[], int numArgs) {
  state->setRender(args[0].getInt());
  out->updateRender(state);
}

void Gfx::opSetTextRise(Object args[], int numArgs) {
  state->setRise(args[0].getNum());
  out->updateRise(state);
}

void Gfx::opSetWordSpacing(Object args[], int numArgs) {
  state->setWordSpace(args[0].getNum());
  out->updateWordSpace(state);
}

void Gfx::opSetHorizScaling(Object args[], int numArgs) {
  state->setHorizScaling(args[0].getNum());
  out->updateHorizScaling(state);
  fontChanged = gTrue;
}

//------------------------------------------------------------------------
// text positioning operators
//------------------------------------------------------------------------

void Gfx::opTextMove(Object args[], int numArgs) {
  double tx, ty;

  tx = state->getLineX() + args[0].getNum();
  ty = state->getLineY() + args[1].getNum();
  state->textMoveTo(tx, ty);
  out->updateTextPos(state);
}

void Gfx::opTextMoveSet(Object args[], int numArgs) {
  double tx, ty;

  tx = state->getLineX() + args[0].getNum();
  ty = args[1].getNum();
  state->setLeading(-ty);
  ty += state->getLineY();
  state->textMoveTo(tx, ty);
  out->updateTextPos(state);
}

void Gfx::opSetTextMatrix(Object args[], int numArgs) {
  state->setTextMat(args[0].getNum(), args[1].getNum(),
		    args[2].getNum(), args[3].getNum(),
		    args[4].getNum(), args[5].getNum());
  state->textMoveTo(0, 0);
  out->updateTextMat(state);
  out->updateTextPos(state);
  fontChanged = gTrue;
}

void Gfx::opTextNextLine(Object args[], int numArgs) {
  double tx, ty;

  tx = state->getLineX();
  ty = state->getLineY() - state->getLeading();
  state->textMoveTo(tx, ty);
  out->updateTextPos(state);
}

//------------------------------------------------------------------------
// text string operators
//------------------------------------------------------------------------

void Gfx::opShowText(Object args[], int numArgs) {
  if (!state->getFont()) {
    error(errSyntaxError, getPos(), "No font in show");
    return;
  }
  if (fontChanged) {
    out->updateFont(state);
    fontChanged = gFalse;
  }
  if (ocState) {
    out->beginStringOp(state);
    doShowText(args[0].getString());
    out->endStringOp(state);
  } else {
    doIncCharCount(args[0].getString());
  }
}

void Gfx::opMoveShowText(Object args[], int numArgs) {
  double tx, ty;

  if (!state->getFont()) {
    error(errSyntaxError, getPos(), "No font in move/show");
    return;
  }
  if (fontChanged) {
    out->updateFont(state);
    fontChanged = gFalse;
  }
  tx = state->getLineX();
  ty = state->getLineY() - state->getLeading();
  state->textMoveTo(tx, ty);
  out->updateTextPos(state);
  if (ocState) {
    out->beginStringOp(state);
    doShowText(args[0].getString());
    out->endStringOp(state);
  } else {
    doIncCharCount(args[0].getString());
  }
}

void Gfx::opMoveSetShowText(Object args[], int numArgs) {
  double tx, ty;

  if (!state->getFont()) {
    error(errSyntaxError, getPos(), "No font in move/set/show");
    return;
  }
  if (fontChanged) {
    out->updateFont(state);
    fontChanged = gFalse;
  }
  state->setWordSpace(args[0].getNum());
  state->setCharSpace(args[1].getNum());
  tx = state->getLineX();
  ty = state->getLineY() - state->getLeading();
  state->textMoveTo(tx, ty);
  out->updateWordSpace(state);
  out->updateCharSpace(state);
  out->updateTextPos(state);
  if (ocState) {
    out->beginStringOp(state);
    doShowText(args[2].getString());
    out->endStringOp(state);
  } else {
    doIncCharCount(args[2].getString());
  }
}

void Gfx::opShowSpaceText(Object args[], int numArgs) {
  Array *a;
  Object obj;
  int wMode;
  int i;

  if (!state->getFont()) {
    error(errSyntaxError, getPos(), "No font in show/space");
    return;
  }
  if (fontChanged) {
    out->updateFont(state);
    fontChanged = gFalse;
  }
  if (ocState) {
    out->beginStringOp(state);
    wMode = state->getFont()->getWMode();
    a = args[0].getArray();
    for (i = 0; i < a->getLength(); ++i) {
      a->get(i, &obj);
      if (obj.isNum()) {
	if (wMode) {
	  state->textShift(0, -obj.getNum() * 0.001 *
			   state->getFontSize());
	} else {
	  state->textShift(-obj.getNum() * 0.001 *
			   state->getFontSize() *
			   state->getHorizScaling(), 0);
	}
	out->updateTextShift(state, obj.getNum());
      } else if (obj.isString()) {
	doShowText(obj.getString());
      } else {
	error(errSyntaxError, getPos(),
	      "Element of show/space array must be number or string");
      }
      obj.free();
    }
    out->endStringOp(state);
  } else {
    a = args[0].getArray();
    for (i = 0; i < a->getLength(); ++i) {
      a->get(i, &obj);
      if (obj.isString()) {
	doIncCharCount(obj.getString());
      }
      obj.free();
    }
  }
}

void Gfx::doShowText(GString *s) {
  GfxFont *font;
  int wMode;
  double riseX, riseY;
  CharCode code;
  Unicode u[8];
  double x, y, dx, dy, dx2, dy2, curX, curY, tdx, tdy, ddx, ddy;
  double originX, originY, tOriginX, tOriginY;
  double x0, y0, x1, y1;
  double oldCTM[6], newCTM[6];
  double *mat;
  Object charProcRef, charProc;
  Dict *resDict;
  Parser *oldParser;
  GfxState *savedState;
  char *p;
  int render;
  GBool patternFill;
  int len, n, uLen, nChars, nSpaces, i;

  font = state->getFont();
  wMode = font->getWMode();

  if (globalParams->isDroppedFont(font->getName()
				    ? font->getName()->getCString() : "")) {
    doIncCharCount(s);
    return;
  }

  if (out->useDrawChar()) {
    out->beginString(state, s);
  }

  // if we're doing a pattern fill, set up clipping
  render = state->getRender();
  if (!(render & 1) &&
      state->getFillColorSpace()->getMode() == csPattern) {
    patternFill = gTrue;
    saveState();
    // disable fill, enable clipping, leave stroke unchanged
    if ((render ^ (render >> 1)) & 1) {
      render = 5;
    } else {
      render = 7;
    }
    state->setRender(render);
    out->updateRender(state);
  } else {
    patternFill = gFalse;
  }

  state->textTransformDelta(0, state->getRise(), &riseX, &riseY);
  x0 = state->getCurX() + riseX;
  y0 = state->getCurY() + riseY;

  // handle a Type 3 char
  if (font->getType() == fontType3 && out->interpretType3Chars()) {
    mat = state->getCTM();
    for (i = 0; i < 6; ++i) {
      oldCTM[i] = mat[i];
    }
    mat = state->getTextMat();
    newCTM[0] = mat[0] * oldCTM[0] + mat[1] * oldCTM[2];
    newCTM[1] = mat[0] * oldCTM[1] + mat[1] * oldCTM[3];
    newCTM[2] = mat[2] * oldCTM[0] + mat[3] * oldCTM[2];
    newCTM[3] = mat[2] * oldCTM[1] + mat[3] * oldCTM[3];
    mat = font->getFontMatrix();
    newCTM[0] = mat[0] * newCTM[0] + mat[1] * newCTM[2];
    newCTM[1] = mat[0] * newCTM[1] + mat[1] * newCTM[3];
    newCTM[2] = mat[2] * newCTM[0] + mat[3] * newCTM[2];
    newCTM[3] = mat[2] * newCTM[1] + mat[3] * newCTM[3];
    newCTM[0] *= state->getFontSize();
    newCTM[1] *= state->getFontSize();
    newCTM[2] *= state->getFontSize();
    newCTM[3] *= state->getFontSize();
    newCTM[0] *= state->getHorizScaling();
    newCTM[2] *= state->getHorizScaling();
    curX = state->getCurX();
    curY = state->getCurY();
    oldParser = parser;
    p = s->getCString();
    len = s->getLength();
    while (len > 0) {
      n = font->getNextChar(p, len, &code,
			    u, (int)(sizeof(u) / sizeof(Unicode)), &uLen,
			    &dx, &dy, &originX, &originY);
      dx = dx * state->getFontSize() + state->getCharSpace();
      if (n == 1 && *p == ' ') {
	dx += state->getWordSpace();
      }
      dx *= state->getHorizScaling();
      dy *= state->getFontSize();
      state->textTransformDelta(dx, dy, &tdx, &tdy);
      state->transform(curX + riseX, curY + riseY, &x, &y);
      savedState = saveStateStack();
      state->setCTM(newCTM[0], newCTM[1], newCTM[2], newCTM[3], x, y);
      //~ the CTM concat values here are wrong (but never used)
      out->updateCTM(state, 1, 0, 0, 1, 0, 0);
      state->transformDelta(dx, dy, &ddx, &ddy);
#if 0
      // The PDF spec says: "the graphics state shall be inherited
      // from the environment of the text-showing operator that caused
      // the [Type 3] glyph description to be invoked".  However,
      // Acrobat apparently copies the fill color to the stroke color.
      // It looks like Ghostscript does the same.  (This is only
      // relevant for uncached Type 3 glyphs.)  Uncomment this block
      // to make Xpdf mimic Adobe (in violation of the PDF spec).
      state->setStrokeColorSpace(state->getFillColorSpace()->copy());
      out->updateStrokeColorSpace(state);
      state->setStrokeColor(state->getFillColor());
      out->updateStrokeColor(state);
#endif
      if (!out->beginType3Char(state, curX + riseX, curY + riseY, ddx, ddy,
			       code, u, uLen)) {
	((Gfx8BitFont *)font)->getCharProcNF(code, &charProcRef);
	charProcRef.fetch(xref, &charProc);
	if ((resDict = ((Gfx8BitFont *)font)->getResources())) {
	  pushResources(resDict);
	}
	if (charProc.isStream()) {
	  display(&charProcRef, gFalse);
	} else {
	  error(errSyntaxError, getPos(),
		"Missing or bad Type3 CharProc entry");
	}
	out->endType3Char(state);
	if (resDict) {
	  popResources();
	}
	charProc.free();
	charProcRef.free();
      }
      restoreStateStack(savedState);
      curX += tdx;
      curY += tdy;
      state->moveTo(curX, curY);
      p += n;
      len -= n;
    }
    parser = oldParser;

  } else if (out->useDrawChar()) {
    p = s->getCString();
    len = s->getLength();
    while (len > 0) {
      n = font->getNextChar(p, len, &code,
			    u, (int)(sizeof(u) / sizeof(Unicode)), &uLen,
			    &dx, &dy, &originX, &originY);
      if (wMode) {
	dx *= state->getFontSize();
	dy = dy * state->getFontSize() + state->getCharSpace();
	if (n == 1 && *p == ' ') {
	  dy += state->getWordSpace();
	}
      } else {
	dx = dx * state->getFontSize() + state->getCharSpace();
	if (n == 1 && *p == ' ') {
	  dx += state->getWordSpace();
	}
	dx *= state->getHorizScaling();
	dy *= state->getFontSize();
      }
      state->textTransformDelta(dx, dy, &tdx, &tdy);
      originX *= state->getFontSize();
      originY *= state->getFontSize();
      state->textTransformDelta(originX, originY, &tOriginX, &tOriginY);
      out->drawChar(state, state->getCurX() + riseX, state->getCurY() + riseY,
		    tdx, tdy, tOriginX, tOriginY, code, n, u, uLen);
      state->shift(tdx, tdy);
      p += n;
      len -= n;
    }

  } else {
    dx = dy = 0;
    p = s->getCString();
    len = s->getLength();
    nChars = nSpaces = 0;
    while (len > 0) {
      n = font->getNextChar(p, len, &code,
			    u, (int)(sizeof(u) / sizeof(Unicode)), &uLen,
			    &dx2, &dy2, &originX, &originY);
      dx += dx2;
      dy += dy2;
      if (n == 1 && *p == ' ') {
	++nSpaces;
      }
      ++nChars;
      p += n;
      len -= n;
    }
    if (wMode) {
      dx *= state->getFontSize();
      dy = dy * state->getFontSize()
	   + nChars * state->getCharSpace()
	   + nSpaces * state->getWordSpace();
    } else {
      dx = dx * state->getFontSize()
	   + nChars * state->getCharSpace()
	   + nSpaces * state->getWordSpace();
      dx *= state->getHorizScaling();
      dy *= state->getFontSize();
    }
    state->textTransformDelta(dx, dy, &tdx, &tdy);
    out->drawString(state, s);
    state->shift(tdx, tdy);
  }

  if (out->useDrawChar()) {
    out->endString(state);
  }

  if (patternFill) {
    out->saveTextPos(state);
    // tell the OutputDev to do the clipping
    out->endTextObject(state);
    // set up a clipping bbox so doPatternText will work -- assume
    // that the text bounding box does not extend past the baseline in
    // any direction by more than twice the font size
    x1 = state->getCurX() + riseX;
    y1 = state->getCurY() + riseY;
    if (x0 > x1) {
      x = x0; x0 = x1; x1 = x;
    }
    if (y0 > y1) {
      y = y0; y0 = y1; y1 = y;
    }
    state->textTransformDelta(0, state->getFontSize(), &dx, &dy);
    state->textTransformDelta(state->getFontSize(), 0, &dx2, &dy2);
    dx = fabs(dx);
    dx2 = fabs(dx2);
    if (dx2 > dx) {
      dx = dx2;
    }
    dy = fabs(dy);
    dy2 = fabs(dy2);
    if (dy2 > dy) {
      dy = dy2;
    }
    state->clipToRect(x0 - 2 * dx, y0 - 2 * dy, x1 + 2 * dx, y1 + 2 * dy);
    // set render mode to fill-only
    state->setRender(0);
    out->updateRender(state);
    doPatternText();
    restoreState();
    out->restoreTextPos(state);
  }

  opCounter += 10 * s->getLength();
}

// NB: this is only called when ocState is false.
void Gfx::doIncCharCount(GString *s) {
  if (out->needCharCount()) {
    out->incCharCount(s->getLength());
  }
}

//------------------------------------------------------------------------
// XObject operators
//------------------------------------------------------------------------

void Gfx::opXObject(Object args[], int numArgs) {
  char *name;
  Object obj1, obj2, obj3, refObj;
  GBool ocSaved, oc;
#if OPI_SUPPORT
  Object opiDict;
#endif

  if (!ocState && !out->needCharCount()) {
    return;
  }
  name = args[0].getName();
  if (!res->lookupXObject(name, &obj1)) {
    return;
  }
  if (!obj1.isStream()) {
    error(errSyntaxError, getPos(), "XObject '{0:s}' is wrong type", name);
    obj1.free();
    return;
  }

  // check for optional content key
  ocSaved = ocState;
  obj1.streamGetDict()->lookupNF("OC", &obj2);
  if (doc->getOptionalContent()->evalOCObject(&obj2, &oc)) {
    ocState &= oc;
  }
  obj2.free();

#if USE_EXCEPTIONS
  try {
#endif
#if OPI_SUPPORT
    obj1.streamGetDict()->lookup("OPI", &opiDict);
    if (opiDict.isDict()) {
      out->opiBegin(state, opiDict.getDict());
    }
#endif
    obj1.streamGetDict()->lookup("Subtype", &obj2);
    if (obj2.isName("Image")) {
      if (out->needNonText()) {
	res->lookupXObjectNF(name, &refObj);
	doImage(&refObj, obj1.getStream(), gFalse);
	refObj.free();
      }
    } else if (obj2.isName("Form")) {
      res->lookupXObjectNF(name, &refObj);
      if (out->useDrawForm() && refObj.isRef()) {
	if (ocState) {
	  out->drawForm(refObj.getRef());
	}
      } else {
	doForm(&refObj, &obj1);
      }
      refObj.free();
    } else if (obj2.isName("PS")) {
      if (ocState) {
	obj1.streamGetDict()->lookup("Level1", &obj3);
	out->psXObject(obj1.getStream(),
		       obj3.isStream() ? obj3.getStream() : (Stream *)NULL);
      }
    } else if (obj2.isName()) {
      error(errSyntaxError, getPos(),
	    "Unknown XObject subtype '{0:s}'", obj2.getName());
    } else {
      error(errSyntaxError, getPos(),
	    "XObject subtype is missing or wrong type");
    }
    obj2.free();
#if OPI_SUPPORT
    if (opiDict.isDict()) {
      out->opiEnd(state, opiDict.getDict());
    }
    opiDict.free();
#endif
#if USE_EXCEPTIONS
  } catch (GMemException e) {
    obj1.free();
    throw;
  }
#endif
  obj1.free();

  ocState = ocSaved;
}

GBool Gfx::doImage(Object *ref, Stream *str, GBool inlineImg) {
  Dict *dict, *maskDict;
  int width, height;
  int bits, maskBits;
  StreamColorSpaceMode csMode;
  GBool mask, invert;
  GfxColorSpace *colorSpace, *maskColorSpace;
  GfxImageColorMap *colorMap, *maskColorMap;
  Object maskObj, smaskObj, maskRef;
  GBool haveColorKeyMask, haveExplicitMask, haveSoftMask, haveMatte;
  int maskColors[2*gfxColorMaxComps];
  int maskWidth, maskHeight;
  GBool maskInvert;
  Stream *maskStr;
  double matte[gfxColorMaxComps];
  GBool interpolate;
  GfxRenderingIntent riSaved;
  Object obj1, obj2;
  int i, n;

  // check for optional content
  if (!ocState && !inlineImg) {
    return gTrue;
  }

  // get info from the stream
  bits = 0;
  csMode = streamCSNone;
  str->getImageParams(&bits, &csMode);

  // get stream dict
  dict = str->getDict();

  // save the current rendering intent
  riSaved = state->getRenderingIntent();

  // get size
  dict->lookup("Width", &obj1);
  if (obj1.isNull()) {
    obj1.free();
    dict->lookup("W", &obj1);
  }
  if (!obj1.isInt()) {
    goto err2;
  }
  width = obj1.getInt();
  obj1.free();
  if (width <= 0) {
    goto err1;
  }
  dict->lookup("Height", &obj1);
  if (obj1.isNull()) {
    obj1.free();
    dict->lookup("H", &obj1);
  }
  if (!obj1.isInt()) {
    goto err2;
  }
  height = obj1.getInt();
  obj1.free();
  if (height <= 0) {
    goto err1;
  }

  // image or mask?
  dict->lookup("ImageMask", &obj1);
  if (obj1.isNull()) {
    obj1.free();
    dict->lookup("IM", &obj1);
  }
  mask = gFalse;
  if (obj1.isBool())
    mask = obj1.getBool();
  else if (!obj1.isNull())
    goto err2;
  obj1.free();

  // bit depth
  if (bits == 0) {
    dict->lookup("BitsPerComponent", &obj1);
    if (obj1.isNull()) {
      obj1.free();
      dict->lookup("BPC", &obj1);
    }
    if (obj1.isInt()) {
      bits = obj1.getInt();
      if (bits < 1 || bits > 16) {
	goto err2;
      }
    } else if (mask) {
      bits = 1;
    } else {
      goto err2;
    }
    obj1.free();
  }

  // interpolate flag
  dict->lookup("Interpolate", &obj1);
  if (obj1.isNull()) {
    obj1.free();
    dict->lookup("I", &obj1);
  }
  interpolate = obj1.isBool() && obj1.getBool();
  obj1.free();

  // display a mask
  if (mask) {

    // check for inverted mask
    if (bits != 1)
      goto err1;
    invert = gFalse;
    dict->lookup("Decode", &obj1);
    if (obj1.isNull()) {
      obj1.free();
      dict->lookup("D", &obj1);
    }
    if (obj1.isArray()) {
      obj1.arrayGet(0, &obj2);
      invert = obj2.isNum() && obj2.getNum() == 1;
      obj2.free();
    } else if (!obj1.isNull()) {
      goto err2;
    }
    obj1.free();

    // if drawing is disabled, skip over inline image data
    if (!ocState) {
      str->reset();
      n = height * ((width + 7) / 8);
      for (i = 0; i < n; ++i) {
	str->getChar();
      }
      str->close();

    // draw it
    } else {
      if (state->getFillColorSpace()->getMode() == csPattern) {
	doPatternImageMask(ref, str, width, height, invert, inlineImg,
			   interpolate);
      } else {
	out->drawImageMask(state, ref, str, width, height, invert, inlineImg,
			   interpolate);
      }
    }

  } else {

    // rendering intent
    if (dict->lookup("Intent", &obj1)->isName()) {
      opSetRenderingIntent(&obj1, 1);
    }
    obj1.free();

    // get color space and color map
    dict->lookup("ColorSpace", &obj1);
    if (obj1.isNull()) {
      obj1.free();
      dict->lookup("CS", &obj1);
    }
    if (obj1.isName()) {
      res->lookupColorSpace(obj1.getName(), &obj2);
      if (!obj2.isNull()) {
	obj1.free();
	obj1 = obj2;
      } else {
	obj2.free();
      }
    }
    if (!obj1.isNull()) {
      colorSpace = GfxColorSpace::parse(&obj1
					);
    } else if (csMode == streamCSDeviceGray) {
      colorSpace = GfxColorSpace::create(csDeviceGray);
    } else if (csMode == streamCSDeviceRGB) {
      colorSpace = GfxColorSpace::create(csDeviceRGB);
    } else if (csMode == streamCSDeviceCMYK) {
      colorSpace = GfxColorSpace::create(csDeviceCMYK);
    } else {
      colorSpace = NULL;
    }
    obj1.free();
    if (!colorSpace) {
      goto err1;
    }
    if (colorSpace->getMode() == csPattern) {
      error(errSyntaxError, getPos(), "Image with a Pattern color space");
      delete colorSpace;
      goto err1;
    }
    dict->lookup("Decode", &obj1);
    if (obj1.isNull()) {
      obj1.free();
      dict->lookup("D", &obj1);
    }
    colorMap = new GfxImageColorMap(bits, &obj1, colorSpace);
    obj1.free();
    if (!colorMap->isOk()) {
      delete colorMap;
      goto err1;
    }

    // get the mask
    haveColorKeyMask = haveExplicitMask = haveSoftMask = haveMatte = gFalse;
    maskStr = NULL; // make gcc happy
    maskWidth = maskHeight = 0; // make gcc happy
    maskInvert = gFalse; // make gcc happy
    maskColorMap = NULL; // make gcc happy
    dict->lookup("Mask", &maskObj);
    dict->lookup("SMask", &smaskObj);
    if (smaskObj.isStream()) {
      // soft mask
      if (inlineImg) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err1;
      }
      maskStr = smaskObj.getStream();
      maskDict = smaskObj.streamGetDict();
      maskDict->lookup("Width", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("W", &obj1);
      }
      if (!obj1.isInt()) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err2;
      }
      maskWidth = obj1.getInt();
      obj1.free();
      maskDict->lookup("Height", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("H", &obj1);
      }
      if (!obj1.isInt()) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err2;
      }
      maskHeight = obj1.getInt();
      obj1.free();
      if (maskWidth <= 0 || maskHeight <= 0) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err1;
      }
      maskDict->lookup("BitsPerComponent", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("BPC", &obj1);
      }
      if (!obj1.isInt()) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err2;
      }
      maskBits = obj1.getInt();
      obj1.free();
      if (maskBits < 1 || maskBits > 16) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err1;
      }
      maskDict->lookup("ColorSpace", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("CS", &obj1);
      }
      if (obj1.isName()) {
	res->lookupColorSpace(obj1.getName(), &obj2);
	if (!obj2.isNull()) {
	  obj1.free();
	  obj1 = obj2;
	} else {
	  obj2.free();
	}
      }
      if (!obj1.isName("DeviceGray")) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err2;
      }
      maskColorSpace = new GfxDeviceGrayColorSpace();
      obj1.free();
      maskDict->lookup("Decode", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("D", &obj1);
      }
      maskColorMap = new GfxImageColorMap(maskBits, &obj1, maskColorSpace);
      obj1.free();
      if (!maskColorMap->isOk()) {
	delete maskColorMap;
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err1;
      }
      if (maskDict->lookup("Matte", &obj1)->isArray()) {
	if (obj1.arrayGetLength() == colorSpace->getNComps()) {
	  for (i = 0; i < obj1.arrayGetLength(); ++i) {
	    if (obj1.arrayGet(i, &obj2)->isNum()) {
	      matte[i] = obj2.getNum();
	    } else {
	      error(errSyntaxError, getPos(),
		    "Invalid Matte entry in soft mask");
	      matte[i] = 0;
	    }
	    obj2.free();
	  }
	  haveMatte = gTrue;
	} else {
	  error(errSyntaxError, getPos(), "Invalid Matte entry in soft mask");
	}
      }
      obj1.free();
      haveSoftMask = gTrue;
    } else if (maskObj.isArray()) {
      // color key mask
      haveColorKeyMask = gTrue;
      for (i = 0;
	   i+1 < maskObj.arrayGetLength() && i+1 < 2*gfxColorMaxComps;
	   i += 2) {
	maskObj.arrayGet(i, &obj1);
	if (!obj1.isInt()) {
	  obj1.free();
	  haveColorKeyMask = gFalse;
	  break;
	}
	maskColors[i] = obj1.getInt();
	obj1.free();
	if (maskColors[i] < 0 || maskColors[i] >= (1 << bits)) {
	  haveColorKeyMask = gFalse;
	  break;
	}
	maskObj.arrayGet(i+1, &obj1);
	if (!obj1.isInt()) {
	  obj1.free();
	  haveColorKeyMask = gFalse;
	  break;
	}
	maskColors[i+1] = obj1.getInt();
	obj1.free();
	if (maskColors[i+1] < 0 || maskColors[i+1] >= (1 << bits) ||
	    maskColors[i] > maskColors[i+1]) {
	  haveColorKeyMask = gFalse;
	  break;
	}
      }
    } else if (maskObj.isStream()) {
      // explicit mask
      if (inlineImg) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err1;
      }
      maskStr = maskObj.getStream();
      maskDict = maskObj.streamGetDict();
      maskDict->lookup("Width", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("W", &obj1);
      }
      if (!obj1.isInt()) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err2;
      }
      maskWidth = obj1.getInt();
      obj1.free();
      maskDict->lookup("Height", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("H", &obj1);
      }
      if (!obj1.isInt()) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err2;
      }
      maskHeight = obj1.getInt();
      obj1.free();
      if (maskWidth <= 0 || maskHeight <= 0) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err2;
      }
      maskDict->lookup("ImageMask", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("IM", &obj1);
      }
      if (!obj1.isBool() || !obj1.getBool()) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err2;
      }
      obj1.free();
      maskInvert = gFalse;
      maskDict->lookup("Decode", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("D", &obj1);
      }
      if (obj1.isArray()) {
	obj1.arrayGet(0, &obj2);
	maskInvert = obj2.isNum() && obj2.getNum() == 1;
	obj2.free();
      } else if (!obj1.isNull()) {
	delete colorMap;
	maskObj.free();
	smaskObj.free();
	goto err2;
      }
      obj1.free();
      haveExplicitMask = gTrue;
    }

    // if drawing is disabled, skip over inline image data
    if (state->getIgnoreColorOps() || !ocState) {
      if (state->getIgnoreColorOps()) {
	error(errSyntaxWarning, getPos(), "Ignoring image"
	      " in uncolored Type 3 char or tiling pattern");
      }
      if (inlineImg) {
	str->reset();
	n = height * ((width * colorMap->getNumPixelComps() *
		       colorMap->getBits() + 7) / 8);
	str->discardChars(n);
	str->close();
      }

    // draw it
    } else {
      if (haveSoftMask) {
	dict->lookupNF("Mask", &maskRef);
	out->drawSoftMaskedImage(state, ref, str, width, height, colorMap,
				 &maskRef, maskStr, maskWidth, maskHeight,
				 maskColorMap,
				 haveMatte ? matte : (double *)NULL,
				 interpolate);
	maskRef.free();
	delete maskColorMap;
      } else if (haveExplicitMask) {
	dict->lookupNF("Mask", &maskRef);
	out->drawMaskedImage(state, ref, str, width, height, colorMap,
			     &maskRef, maskStr, maskWidth, maskHeight,
			     maskInvert, interpolate);
	maskRef.free();
      } else {
	out->drawImage(state, ref, str, width, height, colorMap,
		       haveColorKeyMask ? maskColors : (int *)NULL, inlineImg,
		       interpolate);
      }
    }

    delete colorMap;
    maskObj.free();
    smaskObj.free();
  }

  // restore rendering intent
  if (state->getRenderingIntent() != riSaved) {
    state->setRenderingIntent(riSaved);
    out->updateRenderingIntent(state);
  }

  if ((i = width * height) > 1000) {
    i = 1000;
  }
  opCounter += i;

  return gTrue;

 err2:
  obj1.free();
 err1:
  error(errSyntaxError, getPos(), "Bad image parameters");

  // restore rendering intent
  if (state->getRenderingIntent() != riSaved) {
    state->setRenderingIntent(riSaved);
    out->updateRenderingIntent(state);
  }

  return gFalse;
}

void Gfx::doForm(Object *strRef, Object *str) {
  Dict *dict;
  GBool transpGroup, isolated, knockout;
  Object matrixObj, bboxObj;
  double m[6], bbox[4];
  Object resObj;
  Dict *resDict;
  Object obj1, obj2, obj3;
  int i;

  // check for excessive recursion
  if (formDepth > 100) {
    return;
  }

  // check for optional content
  if (!ocState && !out->needCharCount()) {
    return;
  }

  // get stream dict
  dict = str->streamGetDict();

  // check form type
  dict->lookup("FormType", &obj1);
  if (!(obj1.isNull() || (obj1.isInt() && obj1.getInt() == 1))) {
    error(errSyntaxError, getPos(), "Unknown form type");
  }
  obj1.free();

  // get bounding box
  dict->lookup("BBox", &bboxObj);
  if (!bboxObj.isArray()) {
    bboxObj.free();
    error(errSyntaxError, getPos(), "Bad form bounding box");
    return;
  }
  for (i = 0; i < 4; ++i) {
    bboxObj.arrayGet(i, &obj1);
    bbox[i] = obj1.getNum();
    obj1.free();
  }
  bboxObj.free();

  // get matrix
  dict->lookup("Matrix", &matrixObj);
  if (matrixObj.isArray()) {
    for (i = 0; i < 6; ++i) {
      matrixObj.arrayGet(i, &obj1);
      m[i] = obj1.getNum();
      obj1.free();
    }
  } else {
    m[0] = 1; m[1] = 0;
    m[2] = 0; m[3] = 1;
    m[4] = 0; m[5] = 0;
  }
  matrixObj.free();

  // get resources
  dict->lookup("Resources", &resObj);
  resDict = resObj.isDict() ? resObj.getDict() : (Dict *)NULL;

  // check for a transparency group
  transpGroup = isolated = knockout = gFalse;
  if (dict->lookup("Group", &obj1)->isDict()) {
    if (obj1.dictLookup("S", &obj2)->isName("Transparency")) {
      transpGroup = gTrue;
      if (obj1.dictLookup("I", &obj3)->isBool()) {
	isolated = obj3.getBool();
      }
      obj3.free();
      if (obj1.dictLookup("K", &obj3)->isBool()) {
	knockout = obj3.getBool();
      }
      obj3.free();
    }
    obj2.free();
  }
  obj1.free();

  // draw it
  ++formDepth;
  drawForm(strRef, resDict, m, bbox, transpGroup, gFalse, isolated, knockout);
  --formDepth;

  resObj.free();
}

void Gfx::drawForm(Object *strRef, Dict *resDict,
		   double *matrix, double *bbox,
		   GBool transpGroup, GBool softMask,
		   GBool isolated, GBool knockout,
		   GBool alpha, Function *transferFunc,
		   Object *backdropColorObj) {
  Parser *oldParser;
  GfxState *savedState;
  GfxColorSpace *blendingColorSpace;
  GfxColor backdropColor;
  Object strObj, groupAttrsObj, csObj, obj1;
  double oldBaseMatrix[6];
  int i;

  // push new resources on stack
  pushResources(resDict);

  // save current graphics state
  saveState();

  // kill any pre-existing path
  state->clearPath();

  // save current parser
  oldParser = parser;

  // set form transformation matrix
  state->concatCTM(matrix[0], matrix[1], matrix[2],
		   matrix[3], matrix[4], matrix[5]);
  out->updateCTM(state, matrix[0], matrix[1], matrix[2],
		 matrix[3], matrix[4], matrix[5]);

  // set form bounding box
  state->moveTo(bbox[0], bbox[1]);
  state->lineTo(bbox[2], bbox[1]);
  state->lineTo(bbox[2], bbox[3]);
  state->lineTo(bbox[0], bbox[3]);
  state->closePath();
  state->clip();
  out->clip(state);
  state->clearPath();

  blendingColorSpace = NULL;
  if (softMask || transpGroup) {
    // get the blending color space
    // NB: this must be done AFTER pushing the resource dictionary,
    //     so that any Default*** color spaces are available
    strRef->fetch(xref, &strObj);
    if (strObj.streamGetDict()->lookup("Group", &groupAttrsObj)->isDict()) {
      if (!groupAttrsObj.dictLookup("CS", &csObj)->isNull()) {
	blendingColorSpace = GfxColorSpace::parse(&csObj
						  );
      }
      csObj.free();
    }
    groupAttrsObj.free();
    strObj.free();

    traceBegin(oldBaseMatrix, softMask ? "begin soft mask" : "begin t-group");
    /*
    if (state->getBlendMode() != gfxBlendNormal) {
      state->setBlendMode(gfxBlendNormal);
      out->updateBlendMode(state);
    }
    if (state->getFillOpacity() != 1) {
      state->setFillOpacity(1);
      out->updateFillOpacity(state);
    }
    if (state->getStrokeOpacity() != 1) {
      state->setStrokeOpacity(1);
      out->updateStrokeOpacity(state);
    }
    */
    out->clearSoftMask(state);
    out->beginTransparencyGroup(state, bbox, blendingColorSpace,
				isolated, knockout, softMask);
  }

  // set new base matrix
  for (i = 0; i < 6; ++i) {
    oldBaseMatrix[i] = baseMatrix[i];
    baseMatrix[i] = state->getCTM()[i];
  }

  // save the state stack -- this handles the case where the form
  // contents have unbalanced q/Q operators
  savedState = saveStateStack();

  // draw the form
  display(strRef, gFalse);

  restoreStateStack(savedState);

  if (softMask || transpGroup) {
    out->endTransparencyGroup(state);
  }

  // restore base matrix
  for (i = 0; i < 6; ++i) {
    baseMatrix[i] = oldBaseMatrix[i];
  }

  // restore parser
  parser = oldParser;

  // restore graphics state
  restoreState();

  // pop resource stack
  popResources();

  if (softMask) {
    for (i = 0; i < gfxColorMaxComps; ++i) {
      backdropColor.c[i] = 0;
    }
    if (backdropColorObj->isArray()) {
      for (i = 0;
	   i < backdropColorObj->arrayGetLength() && i < gfxColorMaxComps;
	   ++i) {
	backdropColorObj->arrayGet(i, &obj1);
	if (obj1.isNum()) {
	  backdropColor.c[i] = dblToCol(obj1.getNum());
	}
	obj1.free();
      }
    } else if (blendingColorSpace) {
      blendingColorSpace->getDefaultColor(&backdropColor);
    }
    //~ else: need to get the parent or default color space (?)
    out->setSoftMask(state, bbox, alpha, transferFunc, &backdropColor);
    traceEnd(oldBaseMatrix, "end soft mask");
  } else if (transpGroup) {
    out->paintTransparencyGroup(state, bbox);
    traceEnd(oldBaseMatrix, "end t-group");
  }

  if (blendingColorSpace) {
    delete blendingColorSpace;
  }

  return;
}

void Gfx::takeContentStreamStack(Gfx *oldGfx) {
  contentStreamStack->append(oldGfx->contentStreamStack);
}

void Gfx::endOfPage() {
  while (state->hasSaves()) {
    restoreState();
  }
  while (markedContentStack->getLength() > 0) {
    opEndMarkedContent(NULL, 0);
  }
}

//------------------------------------------------------------------------
// in-line image operators
//------------------------------------------------------------------------

void Gfx::opBeginImage(Object args[], int numArgs) {
  Stream *str;
  GBool haveLength;
  int c1, c2, c3;

  // NB: this function is run even if ocState is false -- doImage() is
  // responsible for skipping over the inline image data

  // build dict/stream
  str = buildImageStream(&haveLength);

  // display the image
  if (str) {
    if (!doImage(NULL, str, gTrue)) {
      delete str;
  
    // if we have the stream length, skip to end-of-stream and then
    // skip 'EI' in the original stream
    } else if (haveLength) {
      while ((c1 = str->getChar()) != EOF) ;
      delete str;
      str = parser->getStream();
      c1 = str->getChar();
      c2 = str->getChar();
      c3 = str->lookChar();
      while (!(c1 == 'E' && c2 == 'I' && Lexer::isSpace(c3)) && c3 != EOF) {
	c1 = c2;
	c2 = str->getChar();
	c3 = str->lookChar();
      }

    // else, look for the 'EI' tag and skip it
    } else {
      c1 = str->getUndecodedStream()->getChar();
      c2 = str->getUndecodedStream()->getChar();
      c3 = str->getUndecodedStream()->lookChar();
      while (!(c1 == 'E' && c2 == 'I' && Lexer::isSpace(c3)) && c3 != EOF) {
	c1 = c2;
	c2 = str->getUndecodedStream()->getChar();
	c3 = str->getUndecodedStream()->lookChar();
      }
      delete str;
    }
  }
}

Stream *Gfx::buildImageStream(GBool *haveLength) {
  Object dict;
  Object obj, lengthObj;
  char *key;
  int length;
  Stream *str;

  // build dictionary
  dict.initDict(xref);
  getContentObj(&obj);
  while (!obj.isCmd("ID") && !obj.isEOF()) {
    if (!obj.isName()) {
      error(errSyntaxError, getPos(),
	    "Inline image dictionary key must be a name object");
      obj.free();
    } else {
      key = copyString(obj.getName());
      obj.free();
      getContentObj(&obj);
      if (obj.isEOF()) {
	gfree(key);
	break;
      }
      if (obj.isError()) {
	gfree(key);
	obj.free();
      } else {
	dict.dictAdd(key, &obj);
      }
    }
    getContentObj(&obj);
  }
  if (obj.isEOF()) {
    error(errSyntaxError, getPos(), "End of file in inline image");
    obj.free();
    dict.free();
    return NULL;
  }
  obj.free();

  // check for length field
  length = 0;
  *haveLength = gFalse;
  if (!dict.dictLookup("Length", &lengthObj)->isInt()) {
    lengthObj.free();
    dict.dictLookup("L", &lengthObj);
  }
  if (lengthObj.isInt()) {
    length = lengthObj.getInt();
    *haveLength = gTrue;
  }
  lengthObj.free();

  // make stream
  if (!(str = parser->getStream())) {
    error(errSyntaxError, getPos(), "Invalid inline image data");
    dict.free();
    return NULL;
  }
  str = new EmbedStream(str, &dict, *haveLength, (GFileOffset)length);
  str = str->addFilters(&dict);

  return str;
}

void Gfx::opImageData(Object args[], int numArgs) {
  error(errInternal, getPos(), "Got 'ID' operator");
}

void Gfx::opEndImage(Object args[], int numArgs) {
  error(errInternal, getPos(), "Got 'EI' operator");
}

//------------------------------------------------------------------------
// type 3 font operators
//------------------------------------------------------------------------

void Gfx::opSetCharWidth(Object args[], int numArgs) {
  out->type3D0(state, args[0].getNum(), args[1].getNum());
}

void Gfx::opSetCacheDevice(Object args[], int numArgs) {
  state->setIgnoreColorOps(gTrue);
  out->type3D1(state, args[0].getNum(), args[1].getNum(),
	       args[2].getNum(), args[3].getNum(),
	       args[4].getNum(), args[5].getNum());
}

//------------------------------------------------------------------------
// compatibility operators
//------------------------------------------------------------------------

void Gfx::opBeginIgnoreUndef(Object args[], int numArgs) {
  ++ignoreUndef;
}

void Gfx::opEndIgnoreUndef(Object args[], int numArgs) {
  if (ignoreUndef > 0)
    --ignoreUndef;
}

//------------------------------------------------------------------------
// marked content operators
//------------------------------------------------------------------------

void Gfx::opBeginMarkedContent(Object args[], int numArgs) {
  GfxMarkedContent *mc;
  Object obj;
  GBool ocStateNew;
  TextString *s;
  GfxMarkedContentKind mcKind;

  if (printCommands) {
    printf("  marked content: %s ", args[0].getName());
    if (numArgs == 2) {
      args[1].print(stdout);
    }
    printf("\n");
    fflush(stdout);
  }
  mcKind = gfxMCOther;
  if (args[0].isName("OC") && numArgs == 2 && args[1].isName() &&
      res->lookupPropertiesNF(args[1].getName(), &obj)) {
    if (doc->getOptionalContent()->evalOCObject(&obj, &ocStateNew)) {
      ocState &= ocStateNew;
    }
    obj.free();
    mcKind = gfxMCOptionalContent;
  } else if (args[0].isName("Span") && numArgs == 2 && args[1].isDict()) {
    if (args[1].dictLookup("ActualText", &obj)->isString()) {
      s = new TextString(obj.getString());
      out->beginActualText(state, s->getUnicode(), s->getLength());
      delete s;
      mcKind = gfxMCActualText;
    }
    obj.free();
  }
  mc = new GfxMarkedContent(mcKind, ocState);
  markedContentStack->append(mc);
}

void Gfx::opEndMarkedContent(Object args[], int numArgs) {
  GfxMarkedContent *mc;
  GfxMarkedContentKind mcKind;

  if (markedContentStack->getLength() > 0) {
    mc = (GfxMarkedContent *)
             markedContentStack->del(markedContentStack->getLength() - 1);
    mcKind = mc->kind;
    delete mc;
    if (mcKind == gfxMCOptionalContent) {
      if (markedContentStack->getLength() > 0) {
	mc = (GfxMarkedContent *)
	         markedContentStack->get(markedContentStack->getLength() - 1);
	ocState = mc->ocState;
      } else {
	ocState = gTrue;
      }
    } else if (mcKind == gfxMCActualText) {
      out->endActualText(state);
    }
  } else {
    error(errSyntaxWarning, getPos(), "Mismatched EMC operator");
  }
}

void Gfx::opMarkPoint(Object args[], int numArgs) {
  if (printCommands) {
    printf("  mark point: %s ", args[0].getName());
    if (numArgs == 2)
      args[1].print(stdout);
    printf("\n");
    fflush(stdout);
  }
}

//------------------------------------------------------------------------
// misc
//------------------------------------------------------------------------

void Gfx::drawAnnot(Object *strRef, AnnotBorderStyle *borderStyle,
		    double xMin, double yMin, double xMax, double yMax) {
  Dict *dict, *resDict;
  Object str, matrixObj, bboxObj, resObj, obj1;
  double formXMin, formYMin, formXMax, formYMax;
  double x, y, sx, sy, tx, ty;
  double m[6], bbox[4];
  double *borderColor;
  GfxColor color;
  double *dash, *dash2;
  int dashLength;
  int i;

  // this function assumes that we are in the default user space,
  // i.e., baseMatrix = ctm

  // if the bounding box has zero width or height, don't draw anything
  // at all
  if (xMin == xMax || yMin == yMax) {
    return;
  }

  // draw the appearance stream (if there is one)
  strRef->fetch(xref, &str);
  if (str.isStream()) {

    // get stream dict
    dict = str.streamGetDict();

    // get the form bounding box
    dict->lookup("BBox", &bboxObj);
    if (!bboxObj.isArray() || bboxObj.arrayGetLength() != 4) {
      error(errSyntaxError, getPos(), "Bad form bounding box");
      bboxObj.free();
      str.free();
      return;
    }
    for (i = 0; i < 4; ++i) {
      bboxObj.arrayGet(i, &obj1);
      if (obj1.isNum()) {
	bbox[i] = obj1.getNum();
      } else {
	bbox[i] = 0;
      }
      obj1.free();
    }
    bboxObj.free();

    // get the form matrix
    dict->lookup("Matrix", &matrixObj);
    if (matrixObj.isArray()) {
      for (i = 0; i < 6; ++i) {
	matrixObj.arrayGet(i, &obj1);
	m[i] = obj1.getNum();
	obj1.free();
      }
    } else {
      m[0] = 1; m[1] = 0;
      m[2] = 0; m[3] = 1;
      m[4] = 0; m[5] = 0;
    }
    matrixObj.free();

    // transform the four corners of the form bbox to default user
    // space, and construct the transformed bbox
    x = bbox[0] * m[0] + bbox[1] * m[2] + m[4];
    y = bbox[0] * m[1] + bbox[1] * m[3] + m[5];
    formXMin = formXMax = x;
    formYMin = formYMax = y;
    x = bbox[0] * m[0] + bbox[3] * m[2] + m[4];
    y = bbox[0] * m[1] + bbox[3] * m[3] + m[5];
    if (x < formXMin) {
      formXMin = x;
    } else if (x > formXMax) {
      formXMax = x;
    }
    if (y < formYMin) {
      formYMin = y;
    } else if (y > formYMax) {
      formYMax = y;
    }
    x = bbox[2] * m[0] + bbox[1] * m[2] + m[4];
    y = bbox[2] * m[1] + bbox[1] * m[3] + m[5];
    if (x < formXMin) {
      formXMin = x;
    } else if (x > formXMax) {
      formXMax = x;
    }
    if (y < formYMin) {
      formYMin = y;
    } else if (y > formYMax) {
      formYMax = y;
    }
    x = bbox[2] * m[0] + bbox[3] * m[2] + m[4];
    y = bbox[2] * m[1] + bbox[3] * m[3] + m[5];
    if (x < formXMin) {
      formXMin = x;
    } else if (x > formXMax) {
      formXMax = x;
    }
    if (y < formYMin) {
      formYMin = y;
    } else if (y > formYMax) {
      formYMax = y;
    }

    // construct a mapping matrix, [sx 0  0], which maps the transformed
    //                             [0  sy 0]
    //                             [tx ty 1]
    // bbox to the annotation rectangle
    if (formXMin == formXMax) {
      // this shouldn't happen
      sx = 1;
    } else {
      sx = (xMax - xMin) / (formXMax - formXMin);
    }
    if (formYMin == formYMax) {
      // this shouldn't happen
      sy = 1;
    } else {
      sy = (yMax - yMin) / (formYMax - formYMin);
    }
    tx = -formXMin * sx + xMin;
    ty = -formYMin * sy + yMin;

    // the final transform matrix is (form matrix) * (mapping matrix)
    m[0] *= sx;
    m[1] *= sy;
    m[2] *= sx;
    m[3] *= sy;
    m[4] = m[4] * sx + tx;
    m[5] = m[5] * sy + ty;

    // get the resources
    dict->lookup("Resources", &resObj);
    resDict = resObj.isDict() ? resObj.getDict() : (Dict *)NULL;

    // draw it
    drawForm(strRef, resDict, m, bbox);

    resObj.free();
  }
  str.free();

  // draw the border
  if (borderStyle && borderStyle->getWidth() > 0 &&
      borderStyle->getNumColorComps() > 0) {
    borderColor = borderStyle->getColor();
    switch (borderStyle->getNumColorComps()) {
    case 1:
      if (state->getStrokeColorSpace()->getMode() != csDeviceGray) {
	state->setStrokePattern(NULL);
	state->setStrokeColorSpace(GfxColorSpace::create(csDeviceGray));
	out->updateStrokeColorSpace(state);
      }
      break;
    case 3:
      if (state->getStrokeColorSpace()->getMode() != csDeviceRGB) {
	state->setStrokePattern(NULL);
	state->setStrokeColorSpace(GfxColorSpace::create(csDeviceRGB));
	out->updateStrokeColorSpace(state);
      }
      break;
    case 4:
      if (state->getStrokeColorSpace()->getMode() != csDeviceCMYK) {
	state->setStrokePattern(NULL);
	state->setStrokeColorSpace(GfxColorSpace::create(csDeviceCMYK));
	out->updateStrokeColorSpace(state);
      }
      break;
    }
    color.c[0] = dblToCol(borderColor[0]);
    color.c[1] = dblToCol(borderColor[1]);
    color.c[2] = dblToCol(borderColor[2]);
    color.c[3] = dblToCol(borderColor[3]);
    state->setStrokeColor(&color);
    out->updateStrokeColor(state);
    state->setLineWidth(borderStyle->getWidth());
    out->updateLineWidth(state);
    borderStyle->getDash(&dash, &dashLength);
    if (borderStyle->getType() == annotBorderDashed && dashLength > 0) {
      dash2 = (double *)gmallocn(dashLength, sizeof(double));
      memcpy(dash2, dash, dashLength * sizeof(double));
      state->setLineDash(dash2, dashLength, 0);
      out->updateLineDash(state);
    }
    //~ this doesn't currently handle the beveled and engraved styles
    state->clearPath();
    state->moveTo(xMin, yMin);
    state->lineTo(xMax, yMin);
    if (borderStyle->getType() != annotBorderUnderlined) {
      state->lineTo(xMax, yMax);
      state->lineTo(xMin, yMax);
      state->closePath();
    }
    out->stroke(state);
  }
}

void Gfx::saveState() {
  out->saveState(state);
  state = state->save();
}

void Gfx::restoreState() {
  state = state->restore();
  out->restoreState(state);
}

// Create a new state stack, and initialize it with a copy of the
// current state.
GfxState *Gfx::saveStateStack() {
  GfxState *oldState;

  out->saveState(state);
  oldState = state;
  state = state->copy(gTrue);
  return oldState;
}

// Switch back to the previous state stack.
void Gfx::restoreStateStack(GfxState *oldState) {
  while (state->hasSaves()) {
    restoreState();
  }
  delete state;
  state = oldState;
  out->restoreState(state);
}

void Gfx::pushResources(Dict *resDict) {
  res = new GfxResources(xref, resDict, res);
}

void Gfx::popResources() {
  GfxResources *resPtr;

  resPtr = res->getNext();
  delete res;
  res = resPtr;
}
