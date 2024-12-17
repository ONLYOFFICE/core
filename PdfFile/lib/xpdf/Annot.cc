//========================================================================
//
// Annot.cc
//
// Copyright 2000-2003 Glyph & Cog, LLC
//
//========================================================================

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <stdlib.h>
#include <math.h>
#include "gmem.h"
#include "gmempp.h"
#include "GList.h"
#include "Error.h"
#include "Object.h"
#include "Catalog.h"
#include "Gfx.h"
#include "GfxFont.h"
#include "Lexer.h"
#include "PDFDoc.h"
#include "OptionalContent.h"
#include "AcroForm.h"
#include "BuiltinFontTables.h"
#include "FontEncodingTables.h"
#include "Annot.h"

// the MSVC math.h doesn't define this
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//------------------------------------------------------------------------

#define annotFlagHidden    0x0002
#define annotFlagPrint     0x0004
#define annotFlagNoView    0x0020

// distance of Bezier control point from center for circle approximation
// = (4 * (sqrt(2) - 1) / 3) * r
#define bezierCircle 0.55228475

#define lineEndSize1    6
#define lineEndSize2   10
#define lineArrowAngle (M_PI / 6)

//------------------------------------------------------------------------
// AnnotBorderStyle
//------------------------------------------------------------------------

AnnotBorderStyle::AnnotBorderStyle(AnnotBorderType typeA, double widthA,
				   double *dashA, int dashLengthA,
				   double *colorA, int nColorCompsA) {
  type = typeA;
  width = widthA;
  dash = dashA;
  dashLength = dashLengthA;
  color[0] = colorA[0];
  color[1] = colorA[1];
  color[2] = colorA[2];
  color[3] = colorA[3];
  nColorComps = nColorCompsA;
}

AnnotBorderStyle::~AnnotBorderStyle() {
  if (dash) {
    gfree(dash);
  }
}

//------------------------------------------------------------------------
// Annot
//------------------------------------------------------------------------

Annot::Annot(PDFDoc *docA, Dict *dict, Ref *refA, const char* AP, const char* AS) {
  GBool bBorder;
  Object apObj, asObj, obj1, obj2, obj3;
  AnnotBorderType borderType;
  double borderWidth;
  double *borderDash;
  int borderDashLength;
  double borderColor[4];
  int nBorderColorComps;
  double t;
  int i;

  bBorder = gFalse;
  ok = gTrue;
  doc = docA;
  xref = doc->getXRef();
  ref = *refA;
  type = NULL;
  appearanceState = NULL;
  appearBuf = NULL;
  borderStyle = NULL;

  //----- parse the type

  if (dict->lookup("Subtype", &obj1)->isName()) {
    type = new GString(obj1.getName());
  }
  obj1.free();

  //----- parse the rectangle

  if (dict->lookup("Rect", &obj1)->isArray() &&
      obj1.arrayGetLength() == 4) {
    xMin = yMin = xMax = yMax = 0;
    if (obj1.arrayGet(0, &obj2)->isNum()) {
      xMin = obj2.getNum();
    }
    obj2.free();
    if (obj1.arrayGet(1, &obj2)->isNum()) {
      yMin = obj2.getNum();
    }
    obj2.free();
    if (obj1.arrayGet(2, &obj2)->isNum()) {
      xMax = obj2.getNum();
    }
    obj2.free();
    if (obj1.arrayGet(3, &obj2)->isNum()) {
      yMax = obj2.getNum();
    }
    obj2.free();
    if (xMin > xMax) {
      t = xMin; xMin = xMax; xMax = t;
    }
    if (yMin > yMax) {
      t = yMin; yMin = yMax; yMax = t;
    }
  } else {
    error(errSyntaxError, -1, "Bad bounding box for annotation");
    ok = gFalse;
  }
  obj1.free();

  //----- parse the flags

  if (dict->lookup("F", &obj1)->isInt()) {
    flags = obj1.getInt();
  } else {
    flags = 0;
  }
  obj1.free();

  //----- parse the border style

  borderType = annotBorderSolid;
  borderWidth = 1;
  borderDash = NULL;
  borderDashLength = 0;
  nBorderColorComps = 3;
  borderColor[0] = 0;
  borderColor[1] = 0;
  borderColor[2] = 1;
  borderColor[3] = 0;
  if (dict->lookup("BS", &obj1)->isDict()) {
    if (obj1.dictLookup("S", &obj2)->isName()) {
      if (obj2.isName("S")) {
	borderType = annotBorderSolid;
      } else if (obj2.isName("D")) {
	borderType = annotBorderDashed;
      } else if (obj2.isName("B")) {
	borderType = annotBorderBeveled;
      } else if (obj2.isName("I")) {
	borderType = annotBorderInset;
      } else if (obj2.isName("U")) {
	borderType = annotBorderUnderlined;
      }
    }
    obj2.free();
    if (obj1.dictLookup("W", &obj2)->isNum()) {
      borderWidth = obj2.getNum();
    }
    obj2.free();
    if (obj1.dictLookup("D", &obj2)->isArray()) {
      borderDashLength = obj2.arrayGetLength();
      borderDash = (double *)gmallocn(borderDashLength, sizeof(double));
      for (i = 0; i < borderDashLength; ++i) {
	if (obj2.arrayGet(i, &obj3)->isNum()) {
	  borderDash[i] = obj3.getNum();
	} else {
	  borderDash[i] = 1;
	}
	obj3.free();
      }
    }
    obj2.free();
    bBorder = gTrue;
  } else {
    obj1.free();
    if (dict->lookup("Border", &obj1)->isArray()) {
      if (obj1.arrayGetLength() >= 3) {
	if (obj1.arrayGet(2, &obj2)->isNum()) {
	  borderWidth = obj2.getNum();
	}
	obj2.free();
	bBorder = gTrue;
	if (obj1.arrayGetLength() >= 4) {
	  if (obj1.arrayGet(3, &obj2)->isArray()) {
	    borderType = annotBorderDashed;
	    borderDashLength = obj2.arrayGetLength();
	    borderDash = (double *)gmallocn(borderDashLength, sizeof(double));
	    for (i = 0; i < borderDashLength; ++i) {
	      if (obj2.arrayGet(i, &obj3)->isNum()) {
		borderDash[i] = obj3.getNum();
	      } else {
		borderDash[i] = 1;
	      }
	      obj3.free();
	    }
	  } else {
	    // Adobe draws no border at all if the last element is of
	    // the wrong type.
	    borderWidth = 0;
	    bBorder = gFalse;
	  }
	  obj2.free();
	}
      } else {
	// an empty Border array also means "no border"
	borderWidth = 0;
      }
    }
  }
  obj1.free();
  if (dict->lookup("C", &obj1)->isArray() &&
      (obj1.arrayGetLength() == 1 ||
       obj1.arrayGetLength() == 3 ||
       obj1.arrayGetLength() == 4)) {
    nBorderColorComps = obj1.arrayGetLength();
    for (i = 0; i < nBorderColorComps; ++i) {
      if (obj1.arrayGet(i, &obj2)->isNum()) {
	borderColor[i] = obj2.getNum();
      } else {
	borderColor[i] = 0;
      }
      obj2.free();
    }
  } else if (type && type->cmp("FreeText")) {
    bBorder = gFalse;
  }
  obj1.free();
  if (bBorder) {
    borderStyle = new AnnotBorderStyle(borderType, borderWidth,
      borderDash, borderDashLength, borderColor, nBorderColorComps);
  }

  //----- get the appearance state

  dict->lookup("AP", &apObj);
  dict->lookup("AS", &asObj);
  if (asObj.isName()) {
    appearanceState = new GString(AS ? AS : asObj.getName());
  } else if (apObj.isDict()) {
    apObj.dictLookup(AP, &obj1);
    if (obj1.isDict() && obj1.dictGetLength() == 1) {
      appearanceState = new GString(AS ? AS : obj1.dictGetKey(0));
    }
    obj1.free();
  }
  if (!appearanceState) {
    appearanceState = new GString("Off");
  }
  asObj.free();

  //----- get the annotation appearance

  if (apObj.isDict()) {
    apObj.dictLookup(AP, &obj1);
    apObj.dictLookupNF(AP, &obj2);
    if (obj1.isDict()) {
      if (obj1.dictLookupNF(appearanceState->getCString(), &obj3)->isRef()) {
	obj3.copy(&appearance);
      }
      obj3.free();
    } else if (obj2.isRef()) {
      obj2.copy(&appearance);
    }
    obj1.free();
    obj2.free();
  }
  apObj.free();

  //----- get the optional content entry

  dict->lookupNF("OC", &ocObj);
}

Annot::~Annot() {
  if (type) {
    delete type;
  }
  if (appearanceState) {
    delete appearanceState;
  }
  appearance.free();
  if (appearBuf) {
    delete appearBuf;
  }
  if (borderStyle) {
    delete borderStyle;
  }
  ocObj.free();
}

void Annot::generateAnnotAppearance() {
  Object obj;

  appearance.fetch(doc->getXRef(), &obj);
  if (!obj.isStream()) {
    if (type) {
      if (!type->cmp("Line")) {
	generateLineAppearance();
      } else if (!type->cmp("PolyLine")) {
	generatePolyLineAppearance();
      } else if (!type->cmp("Polygon")) {
	generatePolygonAppearance();
      } else if (!type->cmp("FreeText")) {
	generateFreeTextAppearance();
      } else if (!type->cmp("Text")) {
	generateTextAppearance();
      }
    }
  }
  obj.free();
}

//~ this doesn't draw the caption
void Annot::generateLineAppearance() {
  Object annotObj, gfxStateDict, appearDict, obj1, obj2;
  MemStream *appearStream;
  double x1, y1, x2, y2, dx, dy, len, w;
  double lx1, ly1, lx2, ly2;
  double tx1, ty1, tx2, ty2;
  double ax1, ay1, ax2, ay2;
  double bx1, by1, bx2, by2;
  double leaderLen, leaderExtLen, leaderOffLen;
  AnnotLineEndType lineEnd1, lineEnd2;
  GBool fill;

  if (!getObject(&annotObj)->isDict()) {
    annotObj.free();
    return;
  }

  appearBuf = new GString();

  //----- check for transparency
  if (annotObj.dictLookup("CA", &obj1)->isNum()) {
    gfxStateDict.initDict(doc->getXRef());
    gfxStateDict.dictAdd(copyString("ca"), obj1.copy(&obj2));
    appearBuf->append("/GS1 gs\n");
  }
  obj1.free();

  //----- set line style, colors
  if (borderStyle) {
    setLineStyle(borderStyle, &w);
    setStrokeColor(borderStyle->getColor(), borderStyle->getNumColorComps());
  }
  fill = gFalse;
  if (annotObj.dictLookup("IC", &obj1)->isArray()) {
    if (setFillColor(&obj1)) {
      fill = gTrue;
    }
  }
  obj1.free();

  //----- get line properties
  if (annotObj.dictLookup("L", &obj1)->isArray() &&
      obj1.arrayGetLength() == 4) {
    if (obj1.arrayGet(0, &obj2)->isNum()) {
      x1 = obj2.getNum();
    } else {
      obj2.free();
      obj1.free();
      goto err1;
    }
    obj2.free();
    if (obj1.arrayGet(1, &obj2)->isNum()) {
      y1 = obj2.getNum();
    } else {
      obj2.free();
      obj1.free();
      goto err1;
    }
    obj2.free();
    if (obj1.arrayGet(2, &obj2)->isNum()) {
      x2 = obj2.getNum();
    } else {
      obj2.free();
      obj1.free();
      goto err1;
    }
    obj2.free();
    if (obj1.arrayGet(3, &obj2)->isNum()) {
      y2 = obj2.getNum();
    } else {
      obj2.free();
      obj1.free();
      goto err1;
    }
    obj2.free();
  } else {
    obj1.free();
    goto err1;
  }
  obj1.free();
  lineEnd1 = lineEnd2 = annotLineEndNone;
  if (annotObj.dictLookup("LE", &obj1)->isArray() &&
      obj1.arrayGetLength() == 2) {
    lineEnd1 = parseLineEndType(obj1.arrayGet(0, &obj2));
    obj2.free();
    lineEnd2 = parseLineEndType(obj1.arrayGet(1, &obj2));
    obj2.free();
  }
  obj1.free();
  if (annotObj.dictLookup("LL", &obj1)->isNum()) {
    leaderLen = obj1.getNum();
  } else {
    leaderLen = 0;
  }
  obj1.free();
  if (annotObj.dictLookup("LLE", &obj1)->isNum()) {
    leaderExtLen = obj1.getNum();
  } else {
    leaderExtLen = 0;
  }
  obj1.free();
  if (annotObj.dictLookup("LLO", &obj1)->isNum()) {
    leaderOffLen = obj1.getNum();
  } else {
    leaderOffLen = 0;
  }
  obj1.free();

  //----- compute positions
  x1 -= xMin;
  y1 -= yMin;
  x2 -= xMin;
  y2 -= yMin;
  dx = x2 - x1;
  dy = y2 - y1;
  len = sqrt(dx*dx + dy*dy);
  if (len > 0) {
    dx /= len;
    dy /= len;
  }
  if (leaderLen != 0) {
    ax1 = x1 + leaderOffLen * dy;
    ay1 = y1 - leaderOffLen * dx;
    lx1 = ax1 + leaderLen * dy;
    ly1 = ay1 - leaderLen * dx;
    bx1 = lx1 + leaderExtLen * dy;
    by1 = ly1 - leaderExtLen * dx;
    ax2 = x2 + leaderOffLen * dy;
    ay2 = y2 - leaderOffLen * dx;
    lx2 = ax2 + leaderLen * dy;
    ly2 = ay2 - leaderLen * dx;
    bx2 = lx2 + leaderExtLen * dy;
    by2 = ly2 - leaderExtLen * dx;
  } else {
    lx1 = x1;
    ly1 = y1;
    lx2 = x2;
    ly2 = y2;
    ax1 = ay1 = ax2 = ay2 = 0; // make gcc happy
    bx1 = by1 = bx2 = by2 = 0;
  }
  adjustLineEndpoint(lineEnd1, lx1, ly1, dx, dy, w, &tx1, &ty1);
  adjustLineEndpoint(lineEnd2, lx2, ly2, -dx, -dy, w, &tx2, &ty2);

  //----- draw leaders
  if (leaderLen != 0) {
    appearBuf->appendf("{0:.4f} {1:.4f} m {2:.4f} {3:.4f} l\n",
		       ax1, ay1, bx1, by1);
    appearBuf->appendf("{0:.4f} {1:.4f} m {2:.4f} {3:.4f} l\n",
		       ax2, ay2 , bx2, by2);
  }

  //----- draw the line
  appearBuf->appendf("{0:.4f} {1:.4f} m {2:.4f} {3:.4f} l\n",
		     tx1, ty1, tx2, ty2);
  appearBuf->append("S\n");

  //----- draw the arrows
  if (borderStyle && borderStyle->getType() == annotBorderDashed) {
    appearBuf->append("[] 0 d\n");
  }
  drawLineArrow(lineEnd1, lx1, ly1, dx, dy, w, fill);
  drawLineArrow(lineEnd2, lx2, ly2, -dx, -dy, w, fill);

  //----- build the appearance stream dictionary
  appearDict.initDict(doc->getXRef());
  appearDict.dictAdd(copyString("Length"),
		     obj1.initInt(appearBuf->getLength()));
  appearDict.dictAdd(copyString("Subtype"), obj1.initName("Form"));
  obj1.initArray(doc->getXRef());
  obj1.arrayAdd(obj2.initReal(0));
  obj1.arrayAdd(obj2.initReal(0));
  obj1.arrayAdd(obj2.initReal(xMax - xMin));
  obj1.arrayAdd(obj2.initReal(yMax - yMin));
  appearDict.dictAdd(copyString("BBox"), &obj1);
  if (gfxStateDict.isDict()) {
    obj1.initDict(doc->getXRef());
    obj2.initDict(doc->getXRef());
    obj2.dictAdd(copyString("GS1"), &gfxStateDict);
    obj1.dictAdd(copyString("ExtGState"), &obj2);
    appearDict.dictAdd(copyString("Resources"), &obj1);
  }

  //----- build the appearance stream
  appearStream = new MemStream(appearBuf->getCString(), 0,
			       appearBuf->getLength(), &appearDict);
  appearance.free();
  appearance.initStream(appearStream);

 err1:
  annotObj.free();
}

//~ this doesn't handle line ends (arrows)
void Annot::generatePolyLineAppearance() {
  Object annotObj, gfxStateDict, appearDict, obj1, obj2;
  MemStream *appearStream;
  double x1, y1, w;
  int i;

  if (!getObject(&annotObj)->isDict()) {
    annotObj.free();
    return;
  }

  appearBuf = new GString();

  //----- check for transparency
  if (annotObj.dictLookup("CA", &obj1)->isNum()) {
    gfxStateDict.initDict(doc->getXRef());
    gfxStateDict.dictAdd(copyString("ca"), obj1.copy(&obj2));
    appearBuf->append("/GS1 gs\n");
  }
  obj1.free();

  //----- set line style, colors
  if (borderStyle) {
    setLineStyle(borderStyle, &w);
    setStrokeColor(borderStyle->getColor(), borderStyle->getNumColorComps());
  }
  // fill = gFalse;
  // if (annotObj.dictLookup("IC", &obj1)->isArray()) {
  //   if (setFillColor(&obj1)) {
  //     fill = gTrue;
  //   }
  // }
  // obj1.free();

  //----- draw line
  if (!annotObj.dictLookup("Vertices", &obj1)->isArray()) {
    obj1.free();
    goto err1;
  }
  for (i = 0; i+1 < obj1.arrayGetLength(); i += 2) {
    if (!obj1.arrayGet(i, &obj2)->isNum()) {
      obj2.free();
      obj1.free();
      goto err1;
    }
    x1 = obj2.getNum();
    obj2.free();
    if (!obj1.arrayGet(i+1, &obj2)->isNum()) {
      obj2.free();
      obj1.free();
      goto err1;
    }
    y1 = obj2.getNum();
    obj2.free();
    x1 -= xMin;
    y1 -= yMin;
    if (i == 0) {
      appearBuf->appendf("{0:.4f} {1:.4f} m\n", x1, y1);
    } else {
      appearBuf->appendf("{0:.4f} {1:.4f} l\n", x1, y1);
    }
  }
  appearBuf->append("S\n");
  obj1.free();

  //----- build the appearance stream dictionary
  appearDict.initDict(doc->getXRef());
  appearDict.dictAdd(copyString("Length"),
		     obj1.initInt(appearBuf->getLength()));
  appearDict.dictAdd(copyString("Subtype"), obj1.initName("Form"));
  obj1.initArray(doc->getXRef());
  obj1.arrayAdd(obj2.initReal(0));
  obj1.arrayAdd(obj2.initReal(0));
  obj1.arrayAdd(obj2.initReal(xMax - xMin));
  obj1.arrayAdd(obj2.initReal(yMax - yMin));
  appearDict.dictAdd(copyString("BBox"), &obj1);
  if (gfxStateDict.isDict()) {
    obj1.initDict(doc->getXRef());
    obj2.initDict(doc->getXRef());
    obj2.dictAdd(copyString("GS1"), &gfxStateDict);
    obj1.dictAdd(copyString("ExtGState"), &obj2);
    appearDict.dictAdd(copyString("Resources"), &obj1);
  }

  //----- build the appearance stream
  appearStream = new MemStream(appearBuf->getCString(), 0,
			       appearBuf->getLength(), &appearDict);
  appearance.free();
  appearance.initStream(appearStream);

 err1:
  annotObj.free();
}

void Annot::generatePolygonAppearance() {
  Object annotObj, gfxStateDict, appearDict, obj1, obj2;
  MemStream *appearStream;
  double x1, y1;
  int i;

  if (!getObject(&annotObj)->isDict()) {
    annotObj.free();
    return;
  }

  appearBuf = new GString();

  //----- check for transparency
  if (annotObj.dictLookup("CA", &obj1)->isNum()) {
    gfxStateDict.initDict(doc->getXRef());
    gfxStateDict.dictAdd(copyString("ca"), obj1.copy(&obj2));
    appearBuf->append("/GS1 gs\n");
  }
  obj1.free();

  //----- set fill color
  if (!annotObj.dictLookup("IC", &obj1)->isArray()  ||
      !setFillColor(&obj1)) {
    obj1.free();
    goto err1;
  }
  obj1.free();

  //----- fill polygon
  if (!annotObj.dictLookup("Vertices", &obj1)->isArray()) {
    obj1.free();
    goto err1;
  }
  for (i = 0; i+1 < obj1.arrayGetLength(); i += 2) {
    if (!obj1.arrayGet(i, &obj2)->isNum()) {
      obj2.free();
      obj1.free();
      goto err1;
    }
    x1 = obj2.getNum();
    obj2.free();
    if (!obj1.arrayGet(i+1, &obj2)->isNum()) {
      obj2.free();
      obj1.free();
      goto err1;
    }
    y1 = obj2.getNum();
    obj2.free();
    x1 -= xMin;
    y1 -= yMin;
    if (i == 0) {
      appearBuf->appendf("{0:.4f} {1:.4f} m\n", x1, y1);
    } else {
      appearBuf->appendf("{0:.4f} {1:.4f} l\n", x1, y1);
    }
  }
  appearBuf->append("f\n");
  obj1.free();

  //----- build the appearance stream dictionary
  appearDict.initDict(doc->getXRef());
  appearDict.dictAdd(copyString("Length"),
		     obj1.initInt(appearBuf->getLength()));
  appearDict.dictAdd(copyString("Subtype"), obj1.initName("Form"));
  obj1.initArray(doc->getXRef());
  obj1.arrayAdd(obj2.initReal(0));
  obj1.arrayAdd(obj2.initReal(0));
  obj1.arrayAdd(obj2.initReal(xMax - xMin));
  obj1.arrayAdd(obj2.initReal(yMax - yMin));
  appearDict.dictAdd(copyString("BBox"), &obj1);
  if (gfxStateDict.isDict()) {
    obj1.initDict(doc->getXRef());
    obj2.initDict(doc->getXRef());
    obj2.dictAdd(copyString("GS1"), &gfxStateDict);
    obj1.dictAdd(copyString("ExtGState"), &obj2);
    appearDict.dictAdd(copyString("Resources"), &obj1);
  }

  //----- build the appearance stream
  appearStream = new MemStream(appearBuf->getCString(), 0,
			       appearBuf->getLength(), &appearDict);
  appearance.free();
  appearance.initStream(appearStream);

 err1:
  annotObj.free();
}

//~ this doesn't handle rich text
//~ this doesn't handle the callout
//~ this doesn't handle the RD field
void Annot::generateFreeTextAppearance() {
  Object annotObj, gfxStateDict, appearDict, obj1, obj2;
  Object resources, gsResources, fontResources, defaultFont;
  GString *text, *da;
  double lineWidth;
  int quadding, rot;
  MemStream *appearStream;

  if (!getObject(&annotObj)->isDict()) {
    annotObj.free();
    return;
  }

  appearBuf = new GString();

  //----- check for transparency
  if (annotObj.dictLookup("CA", &obj1)->isNum()) {
    gfxStateDict.initDict(doc->getXRef());
    gfxStateDict.dictAdd(copyString("ca"), obj1.copy(&obj2));
	gfxStateDict.dictAdd(copyString("CA"), obj1.copy(&obj2));
    appearBuf->append("/GS1 gs\n");
  }
  obj1.free();

  lineWidth = 0;
  if (borderStyle && borderStyle->getWidth() != 0) {
	lineWidth = 0.1;
	if (borderStyle->getWidth() > 0) {
	  lineWidth = borderStyle->getWidth();
	}
  }

  if (annotObj.dictLookup("C", &obj1)->isArray()) {
	setFillColor(&obj1);
	appearBuf->appendf("{0:.4f} {1:.4f} {2:.4f} {3:.4f} re f\n",
		0.5 * lineWidth, 0.5 * lineWidth,
		xMax - xMin - lineWidth, yMax - yMin - lineWidth);
  }

  //----- draw the text
  if (annotObj.dictLookup("Contents", &obj1)->isString()) {
    text = obj1.getString()->copy();
  } else {
    text = new GString();
  }
  obj1.free();
  if (annotObj.dictLookup("Q", &obj1)->isInt()) {
    quadding = obj1.getInt();
  } else {
    quadding = 0;
  }
  obj1.free();
  if (annotObj.dictLookup("DA", &obj1)->isString()) {
    da = obj1.getString()->copy();
  } else {
    da = new GString();
  }
  appearDict.free(); obj1.free();
  // the "Rotate" field is not defined in the PDF spec, but Acrobat
  // looks at it
  if (annotObj.dictLookup("Rotate", &obj1)->isInt()) {
    rot = obj1.getInt();
  } else {
    rot = 0;
  }
  obj1.free();
  drawText(text, da, quadding, lineWidth, rot);
  delete text;
  delete da;

  //----- draw the border
  if (lineWidth != 0) {
	setLineStyle(borderStyle, &lineWidth);
	appearBuf->appendf("{0:.4f} {1:.4f} {2:.4f} {3:.4f} re S\n",
		0.5 * lineWidth, 0.5 * lineWidth,
		xMax - xMin - lineWidth, yMax - yMin - lineWidth);
	obj1.free();
  }

  //----- build the appearance stream dictionary
  appearDict.initDict(doc->getXRef());
  appearDict.dictAdd(copyString("Length"),
		     obj1.initInt(appearBuf->getLength()));
  appearDict.dictAdd(copyString("Subtype"), obj1.initName("Form"));
  obj1.initArray(doc->getXRef());
  obj1.arrayAdd(obj2.initReal(0));
  obj1.arrayAdd(obj2.initReal(0));
  obj1.arrayAdd(obj2.initReal(xMax - xMin));
  obj1.arrayAdd(obj2.initReal(yMax - yMin));
  appearDict.dictAdd(copyString("BBox"), &obj1);
  resources.initDict(doc->getXRef());
  defaultFont.initDict(doc->getXRef());
  defaultFont.dictAdd(copyString("Type"), obj1.initName("Font"));
  defaultFont.dictAdd(copyString("Subtype"), obj1.initName("Type1"));
  defaultFont.dictAdd(copyString("BaseFont"), obj1.initName("Helvetica"));
  defaultFont.dictAdd(copyString("Encoding"), obj1.initName("WinAnsiEncoding"));
  fontResources.initDict(doc->getXRef());
  fontResources.dictAdd(copyString("xpdf_default_font"), &defaultFont);
  resources.dictAdd(copyString("Font"), &fontResources);
  if (gfxStateDict.isDict()) {
    gsResources.initDict(doc->getXRef());
    gsResources.dictAdd(copyString("GS1"), &gfxStateDict);
    resources.dictAdd(copyString("ExtGState"), &gsResources);
  }
  appearDict.dictAdd(copyString("Resources"), &resources);

  //----- build the appearance stream
  appearStream = new MemStream(appearBuf->getCString(), 0,
			       appearBuf->getLength(), &appearDict);
  appearance.free();
  appearance.initStream(appearStream);

  annotObj.free();
}

void Annot::generateTextAppearance()
{
	Object oAnnotObj, oName, oAppearDict, gfxStateDict, obj1, obj2;
	MemStream* pAppearStream;
	if (!getObject(&oAnnotObj)->isDict() || !oAnnotObj.dictLookup("Name", &oName)->isName())
	{
		oAnnotObj.free(); oName.free();
		return;
	}

	appearBuf = new GString();
	pAppearStream = NULL;

	if (oName.isName("Check"))
	{
		yMin = yMax - 19;
		xMax = xMin + 19;
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 7.1836 1.2061 cm 0 0 m 6.691 11.152 11.31 14.196 v 10.773 15.201 9.626 16.892 8.155 17.587 c 2.293 10.706 -0.255 4.205 y -4.525 9.177 l -6.883 5.608 l h b");
	}
	else if (oName.isName("Checkmark"))
	{
		yMin = yMax - 20;
		xMax = xMin + 20;
		appearBuf->append("q 0.396 0.396 0.396 rg 1 0 0 1 13.5151 16.5 cm 0 0 m -6.7 -10.23 l -8.81 -7 l -13.22 -7 l -6.29 -15 l 4.19 0 l h f Q");
	}
	else if (oName.isName("Circle"))
	{
		yMin = yMax - 20;
		xMax = xMin + 20;
		gfxStateDict.initDict(doc->getXRef());
		oAnnotObj.dictLookup("CA", &obj1);
		gfxStateDict.dictAdd(copyString("ca"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		gfxStateDict.dictAdd(copyString("CA"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		obj1.free();
		appearBuf->append("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d /GS1 gs 1 0 0 1 9.999 3.6387 cm 0 0 m -3.513 0 -6.36 2.85 -6.36 6.363 c -6.36 9.875 -3.513 12.724 0 12.724 c 3.514 12.724 6.363 9.875 6.363 6.363 c 6.363 2.85 3.514 0 0 0 c h f Q ");
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 9.999 3.6387 cm 0 0 m -3.513 0 -6.36 2.85 -6.36 6.363 c -6.36 9.875 -3.513 12.724 0 12.724 c 3.514 12.724 6.363 9.875 6.363 6.363 c 6.363 2.85 3.514 0 0 0 c 0 16.119 m -5.388 16.119 -9.756 11.751 -9.756 6.363 c -9.756 0.973 -5.388 -3.395 0 -3.395 c 5.391 -3.395 9.757 0.973 9.757 6.363 c 9.757 11.751 5.391 16.119 0 16.119 c b");
	}
	else if (oName.isName("Comment"))
	{
		yMin = yMax - 24;
		xMax = xMin + 24;
		gfxStateDict.initDict(doc->getXRef());
		oAnnotObj.dictLookup("CA", &obj1);
		gfxStateDict.dictAdd(copyString("ca"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		gfxStateDict.dictAdd(copyString("CA"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		obj1.free();
		appearBuf->append("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d /GS1 gs 1 0 0 1 9 5.0908 cm 7.74 12.616 m -7.74 12.616 l -8.274 12.616 -8.707 12.184 -8.707 11.649 c -8.707 -3.831 l -8.707 -4.365 -8.274 -4.798 -7.74 -4.798 c 7.74 -4.798 l 8.274 -4.798 8.707 -4.365 8.707 -3.831 c 8.707 11.649 l 8.707 12.184 8.274 12.616 7.74 12.616 c h f Q 0 G ");
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 i 0.60 w 4 M 1 j 0 J [0 100] 1 d 1 0 0 1 9 5.0908 cm 1 0 m -2.325 -2.81 l -2.325 0 l -5.72 0 l -5.72 8.94 l 5.51 8.94 l 5.51 0 l 1 0 l -3.50 5.01 m -3.50 5.59 l 3.29 5.59 l 3.29 5.01 l -3.50 5.01 l -3.50 3.34 m -3.50 3.92 l 2.27 3.92 l 2.27 3.34 l -3.50 3.34 l 7.74 12.616 m -7.74 12.616 l -8.274 12.616 -8.707 12.184 -8.707 11.649 c -8.707 -3.831 l -8.707 -4.365 -8.274 -4.798 -7.74 -4.798 c 7.74 -4.798 l 8.274 -4.798 8.707 -4.365 8.707 -3.831 c 8.707 11.649 l 8.707 12.184 8.274 12.616 7.74 12.616 c b");
	}
	else if (oName.isName("Cross"))
	{
		yMin = yMax - 19;
		xMax = xMin + 19;
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 18.6924 3.1357 cm 0 0 m -6.363 6.364 l 0 12.728 l -2.828 15.556 l -9.192 9.192 l -15.556 15.556 l -18.384 12.728 l -12.02 6.364 l -18.384 0 l -15.556 -2.828 l -9.192 3.535 l -2.828 -2.828 l h b");
	}
	else if (oName.isName("CrossHairs"))
	{
		yMin = yMax - 20;
		xMax = xMin + 20;
		gfxStateDict.initDict(doc->getXRef());
		oAnnotObj.dictLookup("CA", &obj1);
		gfxStateDict.dictAdd(copyString("ca"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		gfxStateDict.dictAdd(copyString("CA"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		obj1.free();
		appearBuf->append("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d /GS1 gs 1 0 0 1 9.9771 1.9443 cm 0 0 m -4.448 0 -8.053 3.604 -8.053 8.053 c -8.053 12.5 -4.448 16.106 0 16.106 c 4.447 16.106 8.054 12.5 8.054 8.053 c 8.054 3.604 4.447 0 0 0 c h f Q ");
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.61 w 4 M 0 j 0 J [] 0 d q 1 0 0 1 9.9771 1.9443 cm 0 0 m -4.448 0 -8.053 3.604 -8.053 8.053 c -8.053 12.5 -4.448 16.106 0 16.106 c 4.447 16.106 8.054 12.5 8.054 8.053 c 8.054 3.604 4.447 0 0 0 c 0 17.716 m -5.336 17.716 -9.663 13.39 -9.663 8.053 c -9.663 2.716 -5.336 -1.61 0 -1.61 c 5.337 -1.61 9.664 2.716 9.664 8.053 c 9.664 13.39 5.337 17.716 0 17.716 c b Q q 1 0 0 1 10.7861 14.8325 cm 0 0 m -1.611 0 l -1.611 -4.027 l -5.638 -4.027 l -5.638 -5.638 l -1.611 -5.638 l -1.611 -9.665 l 0 -9.665 l 0 -5.638 l 4.026 -5.638 l 4.026 -4.027 l 0 -4.027 l h b Q");
	}
	else if (oName.isName("Help"))
	{
		yMin = yMax - 20;
		xMax = xMin + 20;
		gfxStateDict.initDict(doc->getXRef());
		oAnnotObj.dictLookup("CA", &obj1);
		gfxStateDict.dictAdd(copyString("ca"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		gfxStateDict.dictAdd(copyString("CA"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		obj1.free();
		appearBuf->append("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d /GS1 gs 1 0 0 1 12.1465 10.5137 cm -2.146 9.403 m -7.589 9.403 -12.001 4.99 -12.001 -0.453 c -12.001 -5.895 -7.589 -10.309 -2.146 -10.309 c 3.296 -10.309 7.709 -5.895 7.709 -0.453 c 7.709 4.99 3.296 9.403 -2.146 9.403 c h f Q ");
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 12.1465 10.5137 cm 0 0 m -0.682 -0.756 -0.958 -1.472 -0.938 -2.302 c -0.938 -2.632 l -3.385 -2.632 l -3.403 -2.154 l -3.459 -1.216 -3.147 -0.259 -2.316 0.716 c -1.729 1.433 -1.251 2.022 -1.251 2.647 c -1.251 3.291 -1.674 3.715 -2.594 3.751 c -3.202 3.751 -3.937 3.531 -4.417 3.2 c -5.041 5.205 l -4.361 5.591 -3.274 5.959 -1.968 5.959 c 0.46 5.959 1.563 4.616 1.563 3.089 c 1.563 1.691 0.699 0.771 0 0 c -2.227 -6.863 m -2.245 -6.863 l -3.202 -6.863 -3.864 -6.146 -3.864 -5.189 c -3.864 -4.196 -3.182 -3.516 -2.227 -3.516 c -1.233 -3.516 -0.589 -4.196 -0.57 -5.189 c -0.57 -6.146 -1.233 -6.863 -2.227 -6.863 c -2.146 9.403 m -7.589 9.403 -12.001 4.99 -12.001 -0.453 c -12.001 -5.895 -7.589 -10.309 -2.146 -10.309 c 3.296 -10.309 7.709 -5.895 7.709 -0.453 c 7.709 4.99 3.296 9.403 -2.146 9.403 c b");
	}
	else if (oName.isName("Insert"))
	{
		yMin = yMax - 20;
		xMax = xMin + 17;
		appearBuf->append("0 G ");
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 i 0.59 w 4 M 0 j 0 J [] 0 d 1 0 0 1 8.5386 19.8545 cm 0 0 m -8.39 -19.719 l 8.388 -19.719 l h B");
	}
	else if (oName.isName("Key"))
	{
		yMin = yMax - 18;
		xMax = xMin + 13;
		gfxStateDict.initDict(doc->getXRef());
		oAnnotObj.dictLookup("CA", &obj1);
		gfxStateDict.dictAdd(copyString("ca"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		gfxStateDict.dictAdd(copyString("CA"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		obj1.free();
		appearBuf->append("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d /GS1 gs 1 0 0 1 6.5 12.6729 cm 0.001 5.138 m -2.543 5.138 -4.604 3.077 -4.604 0.534 c -4.604 -1.368 -3.449 -3.001 -1.802 -3.702 c -1.802 -4.712 l -0.795 -5.719 l -1.896 -6.82 l -0.677 -8.039 l -1.595 -8.958 l -0.602 -9.949 l -1.479 -10.829 l -0.085 -12.483 l 1.728 -10.931 l 1.728 -3.732 l 1.737 -3.728 1.75 -3.724 1.76 -3.721 c 3.429 -3.03 4.604 -1.385 4.604 0.534 c 4.604 3.077 2.542 5.138 0.001 5.138 c f Q ");
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 6.5 12.6729 cm 0 0 m -1.076 0 -1.95 0.874 -1.95 1.95 c -1.95 3.028 -1.076 3.306 0 3.306 c 1.077 3.306 1.95 3.028 1.95 1.95 c 1.95 0.874 1.077 0 0 0 c 0.001 5.138 m -2.543 5.138 -4.604 3.077 -4.604 0.534 c -4.604 -1.368 -3.449 -3.001 -1.802 -3.702 c -1.802 -4.712 l -0.795 -5.719 l -1.896 -6.82 l -0.677 -8.039 l -1.595 -8.958 l -0.602 -9.949 l -1.479 -10.829 l -0.085 -12.483 l 1.728 -10.931 l 1.728 -3.732 l 1.737 -3.728 1.75 -3.724 1.76 -3.721 c 3.429 -3.03 4.604 -1.385 4.604 0.534 c 4.604 3.077 2.542 5.138 0.001 5.138 c b");
	}
	else if (oName.isName("NewParagraph"))
	{
		yMin = yMax - 20;
		xMax = xMin + 13;
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d q 1 0 0 1 6.4995 20 cm 0 0 m -6.205 -12.713 l 6.205 -12.713 l h b Q q 1 0 0 1 1.1909 6.2949 cm 0 0 m 1.278 0 l 1.353 0 1.362 -0.02 1.391 -0.066 c 2.128 -1.363 3.78 -4.275 3.966 -4.713 c 3.985 -4.713 l 3.976 -4.453 3.957 -3.91 3.957 -3.137 c 3.957 -0.076 l 3.957 -0.02 3.976 0 4.041 0 c 4.956 0 l 5.021 0 5.04 -0.029 5.04 -0.084 c 5.04 -6.049 l 5.04 -6.113 5.021 -6.133 4.947 -6.133 c 3.695 -6.133 l 3.621 -6.133 3.611 -6.113 3.574 -6.066 c 3.052 -4.955 1.353 -2.063 0.971 -1.186 c 0.961 -1.186 l 0.999 -1.68 0.999 -2.146 1.008 -3.025 c 1.008 -6.049 l 1.008 -6.104 0.989 -6.133 0.933 -6.133 c 0.009 -6.133 l -0.046 -6.133 -0.075 -6.123 -0.075 -6.049 c -0.075 -0.066 l -0.075 -0.02 -0.056 0 0 0 c f Q q 1 0 0 1 9.1367 3.0273 cm 0 0 m 0.075 0 0.215 -0.008 0.645 -0.008 c 1.4 -0.008 2.119 0.281 2.119 1.213 c 2.119 1.969 1.633 2.381 0.737 2.381 c 0.354 2.381 0.075 2.371 0 2.361 c h -1.146 3.201 m -1.146 3.238 -1.129 3.268 -1.082 3.268 c -0.709 3.275 0.02 3.285 0.729 3.285 c 2.613 3.285 3.248 2.314 3.258 1.232 c 3.258 -0.27 2.007 -0.914 0.607 -0.914 c 0.327 -0.914 0.057 -0.914 0 -0.904 c 0 -2.789 l 0 -2.836 -0.019 -2.865 -0.074 -2.865 c -1.082 -2.865 l -1.119 -2.865 -1.146 -2.846 -1.146 -2.799 c h f Q");
	}
	else if (oName.isName("Note"))
	{
		yMin = yMax - 20;
		xMax = xMin + 18;
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.61 w 4 M 0 j 0 J [] 0 d q 1 0 0 1 16.959 1.3672 cm 0 0 m 0 -0.434 -0.352 -0.785 -0.784 -0.785 c -14.911 -0.785 l -15.345 -0.785 -15.696 -0.434 -15.696 0 c -15.696 17.266 l -15.696 17.699 -15.345 18.051 -14.911 18.051 c -0.784 18.051 l -0.352 18.051 0 17.699 0 17.266 c h b Q q 1 0 0 1 4.4023 13.9243 cm 0 0 m 9.418 0 l S Q q 1 0 0 1 4.4019 11.2207 cm 0 0 m 9.418 0 l S Q q 1 0 0 1 4.4023 8.5176 cm 0 0 m 9.418 0 l S Q q 1 0 0 1 4.4023 5.8135 cm 0 0 m 9.418 0 l S Q");
	}
	else if (oName.isName("Paragraph"))
	{
		yMin = yMax - 20;
		xMax = xMin + 20;
		gfxStateDict.initDict(doc->getXRef());
		oAnnotObj.dictLookup("CA", &obj1);
		gfxStateDict.dictAdd(copyString("ca"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		gfxStateDict.dictAdd(copyString("CA"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		obj1.free();
		appearBuf->append("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d /GS1 gs 1 0 0 1 19.6973 10.0005 cm 0 0 m 0 -5.336 -4.326 -9.662 -9.663 -9.662 c -14.998 -9.662 -19.324 -5.336 -19.324 0 c -19.324 5.335 -14.998 9.662 -9.663 9.662 c -4.326 9.662 0 5.335 0 0 c h f Q ");
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d q 1 0 0 1 19.6973 10.0005 cm 0 0 m 0 -5.336 -4.326 -9.662 -9.663 -9.662 c -14.998 -9.662 -19.324 -5.336 -19.324 0 c -19.324 5.335 -14.998 9.662 -9.663 9.662 c -4.326 9.662 0 5.335 0 0 c h S Q q 1 0 0 1 11.6787 2.6582 cm 0 0 m -1.141 0 l -1.227 0 -1.244 0.052 -1.227 0.139 c -0.656 1.157 -0.52 2.505 -0.52 3.317 c -0.52 3.594 l -2.833 3.783 -5.441 4.838 -5.441 8.309 c -5.441 10.778 -3.714 12.626 -0.57 13.024 c -0.535 13.508 -0.381 14.129 -0.242 14.389 c -0.207 14.44 -0.174 14.475 -0.104 14.475 c 1.088 14.475 l 1.156 14.475 1.191 14.458 1.175 14.372 c 1.105 14.095 0.881 13.127 0.881 12.402 c 0.881 9.431 0.932 7.324 0.95 4.06 c 0.95 2.298 0.708 0.813 0.189 0.07 c 0.155 0.034 0.103 0 0 0 c b Q");
	}
	else if (oName.isName("RightArrow"))
	{
		yMin = yMax - 20;
		xMax = xMin + 20;
		gfxStateDict.initDict(doc->getXRef());
		oAnnotObj.dictLookup("CA", &obj1);
		gfxStateDict.dictAdd(copyString("ca"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		gfxStateDict.dictAdd(copyString("CA"), obj1.isNum() ? obj1.copy(&obj2) : obj2.initReal(0.6));
		obj1.free();
		appearBuf->append("q 1 1 1 rg 0 i 1 w 4 M 1 j 0 J [] 0 d /GS1 gs 1 0 0 1 3.7856 11.1963 cm 6.214 -10.655 m 11.438 -10.655 15.673 -6.42 15.673 -1.196 c 15.673 4.027 11.438 8.262 6.214 8.262 c 0.991 8.262 -3.244 4.027 -3.244 -1.196 c -3.244 -6.42 0.991 -10.655 6.214 -10.655 c h f Q ");
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 0 j 0 J [] 0 d 1 0 0 1 3.7856 11.1963 cm 0 0 m 8.554 0 l 6.045 2.51 l 7.236 3.702 l 12.135 -1.197 l 7.236 -6.096 l 6.088 -4.949 l 8.644 -2.394 l 0 -2.394 l h 6.214 -10.655 m 11.438 -10.655 15.673 -6.42 15.673 -1.196 c 15.673 4.027 11.438 8.262 6.214 8.262 c 0.991 8.262 -3.244 4.027 -3.244 -1.196 c -3.244 -6.42 0.991 -10.655 6.214 -10.655 c b");
	}
	else if (oName.isName("RightPointer"))
	{
		yMin = yMax - 17;
		xMax = xMin + 20;
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0.59 w 4 M 0 j 0 J [] 0 d 1 0 0 1 1.1871 17.0000 cm 0 0 m 4.703 -8.703 l 0 -17 l 18.813 -8.703 l b");
	}
	else if (oName.isName("Star"))
	{
		yMin = yMax - 19;
		xMax = xMin + 20;
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 9.999 18.8838 cm 0 0 m 3.051 -6.178 l 9.867 -7.168 l 4.934 -11.978 l 6.099 -18.768 l 0 -15.562 l -6.097 -18.768 l -4.933 -11.978 l -9.866 -7.168 l -3.048 -6.178 l b");
	}
	else if (oName.isName("UpArrow"))
	{
		yMin = yMax - 20;
		xMax = xMin + 17;
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 1.1007 6.7185 cm 0 0 m 4.009 0 l 4.009 -6.719 l 11.086 -6.719 l 11.086 0 l 14.963 0 l 7.499 13.081 l b");
	}
	else if (oName.isName("UpLeftArrow"))
	{
		yMin = yMax - 17;
		xMax = xMin + 17;
		if (!oAnnotObj.dictLookup("C", &obj1)->isArray() || !setFillColor(&obj1))
			appearBuf->append("1 0.819611 0 rg");
		obj1.free();
		appearBuf->append(" 0 G 0 i 0.59 w 4 M 1 j 0 J [] 0 d 1 0 0 1 2.8335 1.7627 cm 0 0 m -2.74 15.16 l 12.345 12.389 l 9.458 9.493 l 14.027 4.91 l 7.532 -1.607 l 2.964 2.975 l b");
	}

	oAnnotObj.free(); oName.free();

	oAppearDict.initDict(doc->getXRef());
	oAppearDict.dictAdd(copyString("Length"), obj1.initInt(appearBuf->getLength()));
	oAppearDict.dictAdd(copyString("Subtype"), obj1.initName("Form"));
	obj1.initArray(doc->getXRef());
	obj1.arrayAdd(obj2.initReal(0));
	obj1.arrayAdd(obj2.initReal(0));
	obj1.arrayAdd(obj2.initReal(xMax - xMin));
	obj1.arrayAdd(obj2.initReal(yMax - yMin));
	oAppearDict.dictAdd(copyString("BBox"), &obj1);
	if (gfxStateDict.isDict())
	{
		obj1.initDict(doc->getXRef());
		obj2.initDict(doc->getXRef());
		obj2.dictAdd(copyString("GS1"), &gfxStateDict);
		obj1.dictAdd(copyString("ExtGState"), &obj2);
		oAppearDict.dictAdd(copyString("Resources"), &obj1);
	}

	pAppearStream = new MemStream(appearBuf->getCString(), 0, appearBuf->getLength(), &oAppearDict);
	appearance.free();
	appearance.initStream(pAppearStream);
}

void Annot::setLineStyle(AnnotBorderStyle *bs, double *lineWidth) {
  double *dash;
  double w;
  int dashLength, i;

  w = 0.1;
  if (borderStyle && borderStyle->getWidth() > 0) {
	w = borderStyle->getWidth();
  }
  *lineWidth = w;
  appearBuf->appendf("{0:.4f} w\n", w);
  // this treats beveled/inset/underline as solid
  if (borderStyle && borderStyle->getType() == annotBorderDashed) {
    borderStyle->getDash(&dash, &dashLength);
    appearBuf->append("[");
    for (i = 0; i < dashLength; ++i) {
      appearBuf->appendf(" {0:.4f}", dash[i]);
    }
    appearBuf->append("] 0 d\n");
  }
  appearBuf->append("0 j\n0 J\n");
}

void Annot::setStrokeColor(double *color, int nComps) {
  switch (nComps) {
  case 0:
    appearBuf->append("0 G\n");
    break;
  case 1:
    appearBuf->appendf("{0:.2f} G\n", color[0]);
    break;
  case 3:
    appearBuf->appendf("{0:.2f} {1:.2f} {2:.2f} RG\n",
		       color[0], color[1], color[2]);
    break;
  case 4:
    appearBuf->appendf("{0:.2f} {1:.2f} {2:.2f} {3:.2f} K\n",
		       color[0], color[1], color[2], color[3]);
    break;
  }
}

GBool Annot::setFillColor(Object *colorObj) {
  Object obj;
  double color[4];
  int i;

  if (!colorObj->isArray()) {
    return gFalse;
  }
  for (i = 0; i < colorObj->arrayGetLength() && i < 4; ++i) {
    if (colorObj->arrayGet(i, &obj)->isNum()) {
      color[i] = obj.getNum();
    } else {
      color[i] = 0;
    }
    obj.free();
  }
  switch (colorObj->arrayGetLength()) {
  case 1:
    appearBuf->appendf("{0:.2f} g\n", color[0]);
    return gTrue;
  case 3:
    appearBuf->appendf("{0:.2f} {1:.2f} {2:.2f} rg\n",
		       color[0], color[1], color[2]);
    return gTrue;
  case 4:
    appearBuf->appendf("{0:.2f} {1:.2f} {2:.2f} {3:.3f} k\n",
		       color[0], color[1],
		       color[2], color[3]);
    return gTrue;
  }
  return gFalse;
}

AnnotLineEndType Annot::parseLineEndType(Object *obj) {
  if (obj->isName("None")) {
    return annotLineEndNone;
  } else if (obj->isName("Square")) {
    return annotLineEndSquare;
  } else if (obj->isName("Circle")) {
    return annotLineEndCircle;
  } else if (obj->isName("Diamond")) {
    return annotLineEndDiamond;
  } else if (obj->isName("OpenArrow")) {
    return annotLineEndOpenArrow;
  } else if (obj->isName("ClosedArrow")) {
    return annotLineEndClosedArrow;
  } else if (obj->isName("Butt")) {
    return annotLineEndButt;
  } else if (obj->isName("ROpenArrow")) {
    return annotLineEndROpenArrow;
  } else if (obj->isName("RClosedArrow")) {
    return annotLineEndRClosedArrow;
  } else if (obj->isName("Slash")) {
    return annotLineEndSlash;
  } else {
    return annotLineEndNone;
  }
}

void Annot::adjustLineEndpoint(AnnotLineEndType lineEnd,
			       double x, double y, double dx, double dy,
			       double w, double *tx, double *ty) {
  switch (lineEnd) {
  case annotLineEndNone:
    w = 0;
    break;
  case annotLineEndSquare:
    w *= lineEndSize1;
    break;
  case annotLineEndCircle:
    w *= lineEndSize1;
    break;
  case annotLineEndDiamond:
    w *= lineEndSize1;
    break;
  case annotLineEndOpenArrow:
    w = 0;
    break;
  case annotLineEndClosedArrow:
    w *= lineEndSize2 * cos(lineArrowAngle);
    break;
  case annotLineEndButt:
    w = 0;
    break;
  case annotLineEndROpenArrow:
    w *= lineEndSize2 * cos(lineArrowAngle);
    break;
  case annotLineEndRClosedArrow:
    w *= lineEndSize2 * cos(lineArrowAngle);
    break;
  case annotLineEndSlash:
    w = 0;
    break;
  }
  *tx = x + w * dx;
  *ty = y + w * dy;
}

void Annot::drawLineArrow(AnnotLineEndType lineEnd,
			  double x, double y, double dx, double dy,
			  double w, GBool fill) {
  switch (lineEnd) {
  case annotLineEndNone:
    break;
  case annotLineEndSquare:
    w *= lineEndSize1;
    appearBuf->appendf("{0:.4f} {1:.4f} m\n",
		       x + w*dx + 0.5*w*dy,
		       y + w*dy - 0.5*w*dx);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x + 0.5*w*dy,
		       y - 0.5*w*dx);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x - 0.5*w*dy,
		       y + 0.5*w*dx);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x + w*dx - 0.5*w*dy,
		       y + w*dy + 0.5*w*dx);
    appearBuf->append(fill ? "b\n" : "s\n");
    break;
  case annotLineEndCircle:
    w *= lineEndSize1;
    drawCircle(x + 0.5*w*dx, y + 0.5*w*dy, 0.5*w, fill ? "b" : "s");
    break;
  case annotLineEndDiamond:
    w *= lineEndSize1;
    appearBuf->appendf("{0:.4f} {1:.4f} m\n", x, y);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x + 0.5*w*dx - 0.5*w*dy,
		       y + 0.5*w*dy + 0.5*w*dx);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x + w*dx,
		       y + w*dy);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x + 0.5*w*dx + 0.5*w*dy,
		       y + 0.5*w*dy - 0.5*w*dx);
    appearBuf->append(fill ? "b\n" : "s\n");
    break;
  case annotLineEndOpenArrow:
    w *= lineEndSize2;
    appearBuf->appendf("{0:.4f} {1:.4f} m\n",
		       x + w*cos(lineArrowAngle)*dx + w*sin(lineArrowAngle)*dy,
		       y + w*cos(lineArrowAngle)*dy - w*sin(lineArrowAngle)*dx);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n", x, y);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x + w*cos(lineArrowAngle)*dx - w*sin(lineArrowAngle)*dy,
		       y + w*cos(lineArrowAngle)*dy + w*sin(lineArrowAngle)*dx);
    appearBuf->append("S\n");
    break;
  case annotLineEndClosedArrow:
    w *= lineEndSize2;
    appearBuf->appendf("{0:.4f} {1:.4f} m\n",
		       x + w*cos(lineArrowAngle)*dx + w*sin(lineArrowAngle)*dy,
		       y + w*cos(lineArrowAngle)*dy - w*sin(lineArrowAngle)*dx);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n", x, y);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x + w*cos(lineArrowAngle)*dx - w*sin(lineArrowAngle)*dy,
		       y + w*cos(lineArrowAngle)*dy + w*sin(lineArrowAngle)*dx);
    appearBuf->append(fill ? "b\n" : "s\n");
    break;
  case annotLineEndButt:
    w *= lineEndSize1;
    appearBuf->appendf("{0:.4f} {1:.4f} m\n",
		       x + 0.5*w*dy,
		       y - 0.5*w*dx);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x - 0.5*w*dy,
		       y + 0.5*w*dx);
    appearBuf->append("S\n");
    break;
  case annotLineEndROpenArrow:
    w *= lineEndSize2;
    appearBuf->appendf("{0:.4f} {1:.4f} m\n",
		       x + w*sin(lineArrowAngle)*dy,
		       y - w*sin(lineArrowAngle)*dx);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x + w*cos(lineArrowAngle)*dx,
		       y + w*cos(lineArrowAngle)*dy);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x - w*sin(lineArrowAngle)*dy,
		       y + w*sin(lineArrowAngle)*dx);
    appearBuf->append("S\n");
    break;
  case annotLineEndRClosedArrow:
    w *= lineEndSize2;
    appearBuf->appendf("{0:.4f} {1:.4f} m\n",
		       x + w*sin(lineArrowAngle)*dy,
		       y - w*sin(lineArrowAngle)*dx);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x + w*cos(lineArrowAngle)*dx,
		       y + w*cos(lineArrowAngle)*dy);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x - w*sin(lineArrowAngle)*dy,
		       y + w*sin(lineArrowAngle)*dx);
    appearBuf->append(fill ? "b\n" : "s\n");
    break;
  case annotLineEndSlash:
    w *= lineEndSize1;
    appearBuf->appendf("{0:.4f} {1:.4f} m\n",
		       x + 0.5*w*cos(lineArrowAngle)*dy
		         - 0.5*w*sin(lineArrowAngle)*dx,
		       y - 0.5*w*cos(lineArrowAngle)*dx
		         - 0.5*w*sin(lineArrowAngle)*dy);
    appearBuf->appendf("{0:.4f} {1:.4f} l\n",
		       x - 0.5*w*cos(lineArrowAngle)*dy
		         + 0.5*w*sin(lineArrowAngle)*dx,
		       y + 0.5*w*cos(lineArrowAngle)*dx
		         + 0.5*w*sin(lineArrowAngle)*dy);
    appearBuf->append("S\n");
    break;
  }
}

// Draw an (approximate) circle of radius <r> centered at (<cx>, <cy>).
// <cmd> is used to draw the circle ("f", "s", or "b").
void Annot::drawCircle(double cx, double cy, double r, const char *cmd) {
  appearBuf->appendf("{0:.4f} {1:.4f} m\n",
		     cx + r, cy);
  appearBuf->appendf("{0:.4f} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f} c\n",
		     cx + r, cy + bezierCircle * r,
		     cx + bezierCircle * r, cy + r,
		     cx, cy + r);
  appearBuf->appendf("{0:.4f} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f} c\n",
		     cx - bezierCircle * r, cy + r,
		     cx - r, cy + bezierCircle * r,
		     cx - r, cy);
  appearBuf->appendf("{0:.4f} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f} c\n",
		     cx - r, cy - bezierCircle * r,
		     cx - bezierCircle * r, cy - r,
		     cx, cy - r);
  appearBuf->appendf("{0:.4f} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f} c\n",
		     cx + bezierCircle * r, cy - r,
		     cx + r, cy - bezierCircle * r,
		     cx + r, cy);
  appearBuf->appendf("{0:s}\n", cmd);
}

// Draw the top-left half of an (approximate) circle of radius <r>
// centered at (<cx>, <cy>).
void Annot::drawCircleTopLeft(double cx, double cy, double r) {
  double r2;

  r2 = r / sqrt(2.0);
  appearBuf->appendf("{0:.4f} {1:.4f} m\n",
		     cx + r2, cy + r2);
  appearBuf->appendf("{0:.4f} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f} c\n",
		     cx + (1 - bezierCircle) * r2,
		     cy + (1 + bezierCircle) * r2,
		     cx - (1 - bezierCircle) * r2,
		     cy + (1 + bezierCircle) * r2,
		     cx - r2,
		     cy + r2);
  appearBuf->appendf("{0:.4f} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f} c\n",
		     cx - (1 + bezierCircle) * r2,
		     cy + (1 - bezierCircle) * r2,
		     cx - (1 + bezierCircle) * r2,
		     cy - (1 - bezierCircle) * r2,
		     cx - r2,
		     cy - r2);
  appearBuf->append("S\n");
}

// Draw the bottom-right half of an (approximate) circle of radius <r>
// centered at (<cx>, <cy>).
void Annot::drawCircleBottomRight(double cx, double cy, double r) {
  double r2;

  r2 = r / sqrt(2.0);
  appearBuf->appendf("{0:.4f} {1:.4f} m\n",
		     cx - r2, cy - r2);
  appearBuf->appendf("{0:.4f} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f} c\n",
		     cx - (1 - bezierCircle) * r2,
		     cy - (1 + bezierCircle) * r2,
		     cx + (1 - bezierCircle) * r2,
		     cy - (1 + bezierCircle) * r2,
		     cx + r2,
		     cy - r2);
  appearBuf->appendf("{0:.4f} {1:.4f} {2:.4f} {3:.4f} {4:.4f} {5:.4f} c\n",
		     cx + (1 + bezierCircle) * r2,
		     cy - (1 - bezierCircle) * r2,
		     cx + (1 + bezierCircle) * r2,
		     cy + (1 - bezierCircle) * r2,
		     cx + r2,
		     cy + r2);
  appearBuf->append("S\n");
}

void Annot::drawText(GString *text, GString *da, int quadding, double margin,
		     int rot) {
  GString *text2, *tok;
  GList *daToks, *vBreaks;
  const char *charName;
  double dx, dy, fontSize, fontSize2, x, y, w;
  Gushort charWidth;
  int rgPos, i, j, c;

  // check for a Unicode string
  //~ this currently drops all non-Latin1 characters
  if (text->getLength() >= 2 &&
      text->getChar(0) == '\xfe' && text->getChar(1) == '\xff') {
    text2 = new GString();
    for (i = 2; i+1 < text->getLength(); i += 2) {
      c = ((text->getChar(i) & 0xff) << 8) + (text->getChar(i+1) & 0xff);
      if (c <= 0xff) {
	text2->append((char)c);
      } else {
	text2->append('?');
      }
    }
  } else {
    text2 = text;
  }

  // parse the default appearance string
  rgPos = -1;
  if (da) {
    daToks = new GList();
    i = 0;
    while (i < da->getLength()) {
      while (i < da->getLength() && Lexer::isSpace(da->getChar(i))) {
	++i;
      }
      if (i < da->getLength()) {
	for (j = i + 1;
	     j < da->getLength() && !Lexer::isSpace(da->getChar(j));
	     ++j) ;
	daToks->append(new GString(da, i, j - i));
	i = j;
      }
    }
	for (i = 0; i < daToks->getLength(); ++i) {
	  if (i >= 3 && !((GString *)daToks->get(i))->cmp("rg")) {
	rgPos = i - 3;
	  }
    }
  } else {
    daToks = new GList();
  }

  // setup
  appearBuf->append("q\n");
  if (rot == 90) {
    appearBuf->appendf("0 1 -1 0 {0:.4f} 0 cm\n", xMax - xMin);
    dx = yMax - yMin;
    dy = xMax - xMin;
  } else if (rot == 180) {
    appearBuf->appendf("-1 0 0 -1 {0:.4f} {1:.4f} cm\n",
		       xMax - xMin, yMax - yMin);
    dx = xMax - yMax;
    dy = yMax - yMin;
  } else if (rot == 270) {
    appearBuf->appendf("0 -1 1 0 0 {0:.4f} cm\n", yMax - yMin);
    dx = yMax - yMin;
    dy = xMax - xMin;
  } else { // assume rot == 0
    dx = xMax - xMin;
    dy = yMax - yMin;
  }
  appearBuf->append("BT\n");

  // compute string width
  //~ this assumes we're substituting Helvetica/WinAnsiEncoding for everything
  fontSize = 14;
  w = 0, i = 0;
  vBreaks = new GList();
  double dX = 0, dWordWidth = 0, dKoef = fontSize / 1000.0;
  unsigned int unWordStartPos = 0;
  bool bLineStart = true, bWord = false, bFirstItemOnLine = true;
  while (i < text2->getLength())
  {
	  charName = winAnsiEncoding[text->getChar(i) & 0xff];
	  if (!charName || !builtinFonts[4].widths->getWidth(charName, &charWidth))
		  charWidth = 500;

	  char c = text->getChar(i);
	  if (c == 0x20)
	  {
		  dX += dWordWidth + charWidth * dKoef;
		  bWord             = false;
		  dWordWidth        = 0;
		  bLineStart        = false;
		  bFirstItemOnLine  = false;
	  }
	  else if (c == 0xA || c == 0xD)
	  {
		  bLineStart       = true;
		  bFirstItemOnLine = true;
		  bWord            = false;
		  dX               = 0;
		  dWordWidth       = 0;
		  vBreaks->append(new int(i + 1));
	  }
	  else
	  {
		  double dLetterWidth = charWidth * dKoef;
		  if (dX + dWordWidth + dLetterWidth > dx)
		  {
			  if (bLineStart)
			  {
				  if (bFirstItemOnLine)
				  {
					  if (i != text2->getLength() - 1)
						  vBreaks->append(new int(i + 1));
					  i++;
				  }
				  else
					  vBreaks->append(new int(i));
			  }
			  else
			  {
				  if (bWord)
				  {
					  vBreaks->append(new int(unWordStartPos));
					  i = unWordStartPos;
				  }
				  else
					  vBreaks->append(new int(i));
			  }

			  dX               = 0;
			  bWord            = false;
			  dWordWidth       = 0;
			  bLineStart       = true;
			  bFirstItemOnLine = true;
			  continue;
		  }

		  if (bWord)
			  dWordWidth += charWidth * dKoef;
		  else
		  {
			  unWordStartPos = i;
			  bWord          = true;
			  dWordWidth     = charWidth * dKoef;
		  }

		  bFirstItemOnLine  = false;
	  }

	  i++;
  }
  for (i = 0; i < text2->getLength(); ++i) {
    charName = winAnsiEncoding[text->getChar(i) & 0xff];
    if (charName && builtinFonts[4].widths->getWidth(charName, &charWidth)) {
      w += charWidth;
    } else {
      w += 0.5;
    }
  }

  // write the DA string
  appearBuf->append("/xpdf_default_font 14 Tf\n");
  if (rgPos > 0) {
	appearBuf->append((GString *)daToks->get(rgPos))->append(' ');
	appearBuf->append((GString *)daToks->get(rgPos + 1))->append(' ');
	appearBuf->append((GString *)daToks->get(rgPos + 2))->append(' ');
	appearBuf->append("rg\n");
  }

  unsigned int unLinesCount = vBreaks->getLength() + 1;
  x = margin * 2;
  y = dy - margin - 2 - 0.789571 * fontSize;
  double dLineHeight = 0.789571 * fontSize;
  for (i = 0; i < unLinesCount; ++i)
  {
	  // compute text start position
	  double dLineShiftX = x;
	  w = m_oLinesManager.GetLineWidth(i, fontSize);
	  if (2 == quadding)
		  dLineShiftX += dx - w - margin * 2;
	  else if (1 == quadding)
		  dLineShiftX += (dx - w) / 2;

	  int nInLineCount = m_oLinesManager.GetLineEndPos(unIndex) - m_oLinesManager.GetLineStartPos(unIndex);
	  if (nInLineCount > 0)
	  {
		  // write the font matrix
		  appearBuf->appendf("{0:.4f} {1:.4f} Td\n", dLineShiftX, y);

		  // write the text string
		  appearBuf->append('(');
		  for (i = m_oLinesManager.GetLineStartPos(unIndex); i < text2->getLength(); ++i) {
			c = text2->getChar(i) & 0xff;
			if (c == '(' || c == ')' || c == '\\') {
			  appearBuf->append('\\');
			  appearBuf->append((char)c);
			} else if (c < 0x20 || c >= 0x80) {
			  appearBuf->appendf("\\{0:03o}", c);
			} else {
			  appearBuf->append((char)c);
			}
		  }
		  appearBuf->append(") Tj\n");

		  pField->AddLineToTextAppearance(dLineShiftX, dLineShiftY, pCodes + unLineStart, nInLineCount, ppFonts + unLineStart, NULL);
	  }
	  y -= dLineHeight;
  }

  // cleanup
  appearBuf->append("ET\n");
  appearBuf->append("Q\n");

  if (rgPos > 0) {
	appearBuf->append((GString *)daToks->get(rgPos))->append(' ');
	appearBuf->append((GString *)daToks->get(rgPos + 1))->append(' ');
	appearBuf->append((GString *)daToks->get(rgPos + 2))->append(' ');
	appearBuf->append("RG\n");
  }

  if (daToks) {
    deleteGList(daToks, GString);
  }
  if (vBreaks)
	deleteGList(vBreaks, int);
  if (text2 != text) {
    delete text2;
  }
}

void Annot::draw(Gfx *gfx, GBool printing) {
  GBool oc, isLink;

  // check the flags
  if ((flags & annotFlagHidden) ||
      (printing && !(flags & annotFlagPrint)) ||
      (!printing && (flags & annotFlagNoView))) {
    return;
  }

  // check the optional content entry
  if (doc->getOptionalContent()->evalOCObject(&ocObj, &oc) && !oc) {
    return;
  }

  // draw the appearance stream
  isLink = type && !type->cmp("Link");
#ifdef BUILDING_WASM_MODULE
  if (type && !type->cmp("Stamp"))
  {
    gfx->drawStamp(&appearance);
    return;
  }
#endif
  gfx->drawAnnot(&appearance, isLink ? borderStyle : (AnnotBorderStyle *)NULL,
		 xMin, yMin, xMax, yMax);
}

Object *Annot::getObject(Object *obj) {
  if (ref.num >= 0) {
    xref->fetch(ref.num, ref.gen, obj);
  } else {
    obj->initNull();
  }
  return obj;
}

//------------------------------------------------------------------------
// Annots
//------------------------------------------------------------------------

Annots::Annots(PDFDoc *docA, Object *annotsObj) {
  Annot *annot;
  Object obj1, obj2;
  Ref ref;
  GBool drawWidgetAnnots;
  int size;
  int i;

  doc = docA;
  annots = NULL;
  size = 0;
  nAnnots = 0;

  if (annotsObj->isArray()) {
    // Kludge: some PDF files define an empty AcroForm, but still
    // include Widget-type annotations -- in that case, we want to
    // draw the widgets (since the form code won't).  This really
    // ought to look for Widget-type annotations that are not included
    // in any form field.
    drawWidgetAnnots = !doc->getCatalog()->getForm() ||
                       doc->getCatalog()->getForm()->getNumFields() == 0;
    for (i = 0; i < annotsObj->arrayGetLength(); ++i) {
      if (annotsObj->arrayGetNF(i, &obj1)->isRef()) {
	ref = obj1.getRef();
	obj1.free();
	annotsObj->arrayGet(i, &obj1);
      } else {
	ref.num = ref.gen = -1;
      }
      if (obj1.isDict()) {
	if (drawWidgetAnnots ||
	    !obj1.dictLookup("Subtype", &obj2)->isName("Widget")) {
	  annot = new Annot(doc, obj1.getDict(), &ref);
	  if (annot->isOk()) {
	    if (nAnnots >= size) {
	      size += 16;
	      annots = (Annot **)greallocn(annots, size, sizeof(Annot *));
	    }
	    annots[nAnnots++] = annot;
	  } else {
	    delete annot;
	  }
	}
	obj2.free();
      }
      obj1.free();
    }
  }
}

Annots::~Annots() {
  int i;

  for (i = 0; i < nAnnots; ++i) {
    delete annots[i];
  }
  gfree(annots);
}

Annot *Annots::find(double x, double y) {
  int i;

  for (i = nAnnots - 1; i >= 0; --i) {
    if (annots[i]->inRect(x, y)) {
      return annots[i];
    }
  }
  return NULL;
}

int Annots::findIdx(double x, double y) {
  int i;

  for (i = nAnnots - 1; i >= 0; --i) {
    if (annots[i]->inRect(x, y)) {
      return i;
    }
  }
  return -1;
}

void Annots::generateAnnotAppearances() {
  int i;

  for (i = 0; i < nAnnots; ++i) {
    annots[i]->generateAnnotAppearance();
  }
}

Annot *Annots::findAnnot(Ref *ref) {
  int i;

  for (i = 0; i < nAnnots; ++i) {
    if (annots[i]->match(ref)) {
      return annots[i];
    }
  }
  return NULL;
}
