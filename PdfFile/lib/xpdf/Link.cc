//========================================================================
//
// Link.cc
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <stddef.h>
#include <string.h>
#include "gmem.h"
#include "gmempp.h"
#include "GString.h"
#include "Error.h"
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "Link.h"

//------------------------------------------------------------------------
// LinkAction
//------------------------------------------------------------------------

LinkAction *LinkAction::parseDest(Object *obj) {
  LinkAction *action;

  action = new LinkGoTo(obj);
  if (!action->isOk()) {
    delete action;
    return NULL;
  }
  return action;
}

LinkAction *LinkAction::parseAction(Object *obj, GString *baseURI) {
  LinkAction *action;
  Object obj2, obj3, obj4, obj5;

  if (!obj->isDict()) {
    error(errSyntaxWarning, -1, "Bad annotation action");
    return NULL;
  }

  obj->dictLookup("S", &obj2);

  // GoTo action
  if (obj2.isName("GoTo")) {
    obj->dictLookup("D", &obj3);
    action = new LinkGoTo(&obj3);
    obj3.free();

  // GoToR action
  } else if (obj2.isName("GoToR")) {
    obj->dictLookup("F", &obj3);
    obj->dictLookup("D", &obj4);
    action = new LinkGoToR(&obj3, &obj4);
    obj3.free();
    obj4.free();

  // Launch action
  } else if (obj2.isName("Launch")) {
    action = new LinkLaunch(obj);

  // URI action
  } else if (obj2.isName("URI")) {
    obj->dictLookup("URI", &obj3);
    action = new LinkURI(&obj3, baseURI);
    obj3.free();

  // Named action
  } else if (obj2.isName("Named")) {
    obj->dictLookup("N", &obj3);
    action = new LinkNamed(&obj3);
    obj3.free();

  // Movie action
  } else if (obj2.isName("Movie")) {
    obj->dictLookupNF("Annot", &obj3);
    obj->dictLookup("T", &obj4);
    action = new LinkMovie(&obj3, &obj4);
    obj3.free();
    obj4.free();

  // JavaScript action
  } else if (obj2.isName("JavaScript")) {
    obj->dictLookup("JS", &obj3);
    action = new LinkJavaScript(&obj3);
    obj3.free();

  // SubmitForm action
  } else if (obj2.isName("SubmitForm")) {
    obj->dictLookup("F", &obj3);
    obj->dictLookup("Fields", &obj4);
    obj->dictLookup("Flags", &obj5);
    action = new LinkSubmitForm(&obj3, &obj4, &obj5);
    obj3.free();
    obj4.free();
    obj5.free();

  // Hide action
  } else if (obj2.isName("Hide")) {
    obj->dictLookupNF("T", &obj3);
    obj->dictLookup("H", &obj4);
    action = new LinkHide(&obj3, &obj4);
    obj3.free();
    obj4.free();

  // unknown action
  } else if (obj2.isName()) {
    action = new LinkUnknown(obj2.getName());

  // action is missing or wrong type
  } else {
    error(errSyntaxWarning, -1, "Bad annotation action");
    action = NULL;
  }

  obj2.free();

  if (action && !action->isOk()) {
    delete action;
    return NULL;
  }
  return action;
}

GString *LinkAction::getFileSpecName(Object *fileSpecObj) {
  GString *name;
  Object obj1;

  name = NULL;

  // string
  if (fileSpecObj->isString()) {
    name = fileSpecObj->getString()->copy();

  // dictionary
  } else if (fileSpecObj->isDict()) {
#ifdef _WIN32
    if (!fileSpecObj->dictLookup("DOS", &obj1)->isString()) {
#else
    if (!fileSpecObj->dictLookup("Unix", &obj1)->isString()) {
#endif
      obj1.free();
      fileSpecObj->dictLookup("F", &obj1);
    }
    if (obj1.isString()) {
      name = obj1.getString()->copy();
    } else {
      error(errSyntaxWarning, -1, "Illegal file spec in link");
    }
    obj1.free();

  // error
  } else {
    error(errSyntaxWarning, -1, "Illegal file spec in link");
  }

  // system-dependent path manipulation
  if (name) {
#ifdef _WIN32
    int i, j;

    // "//...."             --> "\...."
    // "/x/...."            --> "x:\...."
    // "/server/share/...." --> "\\server\share\...."
    // convert escaped slashes to slashes and unescaped slashes to backslashes
    i = 0;
    if (name->getChar(0) == '/') {
      if (name->getLength() >= 2 && name->getChar(1) == '/') {
	name->del(0);
	i = 0;
      } else if (name->getLength() >= 2 &&
		 ((name->getChar(1) >= 'a' && name->getChar(1) <= 'z') ||
		  (name->getChar(1) >= 'A' && name->getChar(1) <= 'Z')) &&
		 (name->getLength() == 2 || name->getChar(2) == '/')) {
	name->setChar(0, name->getChar(1));
	name->setChar(1, ':');
	i = 2;
      } else {
	for (j = 2; j < name->getLength(); ++j) {
	  if (name->getChar(j-1) != '\\' &&
	      name->getChar(j) == '/') {
	    break;
	  }
	}
	if (j < name->getLength()) {
	  name->setChar(0, '\\');
	  name->insert(0, '\\');
	  i = 2;
	}
      }
    }
    for (; i < name->getLength(); ++i) {
      if (name->getChar(i) == '/') {
	name->setChar(i, '\\');
      } else if (name->getChar(i) == '\\' &&
		 i+1 < name->getLength() &&
		 name->getChar(i+1) == '/') {
	name->del(i);
      }
    }
#else
    // no manipulation needed for Unix
#endif
  }

  return name;
}

//------------------------------------------------------------------------
// LinkDest
//------------------------------------------------------------------------

LinkDest::LinkDest(Array *a) {
  Object obj1, obj2;

  // initialize fields
  left = bottom = right = top = zoom = 0;
  ok = gFalse;
  changeLeft = changeTop = changeZoom = gFalse;

  // get page
  if (a->getLength() < 2) {
    error(errSyntaxWarning, -1, "Annotation destination array is too short");
    return;
  }
  a->getNF(0, &obj1);
  if (obj1.isInt()) {
    pageNum = obj1.getInt() + 1;
    pageIsRef = gFalse;
  } else if (obj1.isRef()) {
    pageRef.num = obj1.getRefNum();
    pageRef.gen = obj1.getRefGen();
    pageIsRef = gTrue;
  } else {
    error(errSyntaxWarning, -1, "Bad annotation destination");
    goto err2;
  }
  obj1.free();

  // get destination type
  a->get(1, &obj1);

  // XYZ link
  if (obj1.isName("XYZ")) {
    kind = destXYZ;
    if (a->getLength() < 3) {
      changeLeft = gFalse;
    } else {
      a->get(2, &obj2);
      if (obj2.isNull()) {
	changeLeft = gFalse;
      } else if (obj2.isNum()) {
	changeLeft = gTrue;
	left = obj2.getNum();
      } else {
	error(errSyntaxWarning, -1, "Bad annotation destination position");
	goto err1;
      }
      obj2.free();
    }
    if (a->getLength() < 4) {
      changeTop = gFalse;
    } else {
      a->get(3, &obj2);
      if (obj2.isNull()) {
	changeTop = gFalse;
      } else if (obj2.isNum()) {
	changeTop = gTrue;
	top = obj2.getNum();
      } else {
	error(errSyntaxWarning, -1, "Bad annotation destination position");
	goto err1;
      }
      obj2.free();
    }
    if (a->getLength() < 5) {
      changeZoom = gFalse;
    } else {
      a->get(4, &obj2);
      if (obj2.isNull()) {
	changeZoom = gFalse;
      } else if (obj2.isNum()) {
	changeZoom = gTrue;
	zoom = obj2.getNum();
      } else {
	error(errSyntaxWarning, -1, "Bad annotation destination position");
	goto err1;
      }
      obj2.free();
    }

  // Fit link
  } else if (obj1.isName("Fit")) {
    if (a->getLength() < 2) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      goto err2;
    }
    kind = destFit;

  // FitH link
  } else if (obj1.isName("FitH")) {
    if (a->getLength() < 3) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      goto err2;
    }
    kind = destFitH;
    if (a->get(2, &obj2)->isNum()) {
      top = obj2.getNum();
      changeTop = gTrue;
    } else if (obj2.isNull()) {
      changeTop = gFalse;
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    obj2.free();

  // FitV link
  } else if (obj1.isName("FitV")) {
    if (a->getLength() < 3) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      goto err2;
    }
    kind = destFitV;
    if (a->get(2, &obj2)->isNum()) {
      left = obj2.getNum();
      changeLeft = gTrue;
    } else if (obj2.isNull()) {
      changeLeft = gFalse;
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    obj2.free();

  // FitR link
  } else if (obj1.isName("FitR")) {
    if (a->getLength() < 6) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      goto err2;
    }
    kind = destFitR;
    if (a->get(2, &obj2)->isNum()) {
      left = obj2.getNum();
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    obj2.free();
    if (!a->get(3, &obj2)->isNum()) {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    bottom = obj2.getNum();
    obj2.free();
    if (!a->get(4, &obj2)->isNum()) {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    right = obj2.getNum();
    obj2.free();
    if (!a->get(5, &obj2)->isNum()) {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    top = obj2.getNum();
    obj2.free();

  // FitB link
  } else if (obj1.isName("FitB")) {
    if (a->getLength() < 2) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      goto err2;
    }
    kind = destFitB;

  // FitBH link
  } else if (obj1.isName("FitBH")) {
    if (a->getLength() < 3) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      goto err2;
    }
    kind = destFitBH;
    if (a->get(2, &obj2)->isNum()) {
      top = obj2.getNum();
      changeTop = gTrue;
    } else if (obj2.isNull()) {
      changeTop = gFalse;
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    obj2.free();

  // FitBV link
  } else if (obj1.isName("FitBV")) {
    if (a->getLength() < 3) {
      error(errSyntaxWarning, -1, "Annotation destination array is too short");
      goto err2;
    }
    kind = destFitBV;
    if (a->get(2, &obj2)->isNum()) {
      left = obj2.getNum();
      changeLeft = gTrue;
    } else if (obj2.isNull()) {
      changeLeft = gFalse;
    } else {
      error(errSyntaxWarning, -1, "Bad annotation destination position");
      kind = destFit;
    }
    obj2.free();

  // unknown link kind
  } else {
    error(errSyntaxWarning, -1, "Unknown annotation destination type");
    goto err2;
  }

  obj1.free();
  ok = gTrue;
  return;

 err1:
  obj2.free();
 err2:
  obj1.free();
}

LinkDest::LinkDest(LinkDest *dest) {
  kind = dest->kind;
  pageIsRef = dest->pageIsRef;
  if (pageIsRef)
    pageRef = dest->pageRef;
  else
    pageNum = dest->pageNum;
  left = dest->left;
  bottom = dest->bottom;
  right = dest->right;
  top = dest->top;
  zoom = dest->zoom;
  changeLeft = dest->changeLeft;
  changeTop = dest->changeTop;
  changeZoom = dest->changeZoom;
  ok = gTrue;
}

//------------------------------------------------------------------------
// LinkGoTo
//------------------------------------------------------------------------

LinkGoTo::LinkGoTo(Object *destObj) {
  dest = NULL;
  namedDest = NULL;

  // named destination
  if (destObj->isName()) {
    namedDest = new GString(destObj->getName());
  } else if (destObj->isString()) {
    namedDest = destObj->getString()->copy();

  // destination dictionary
  } else if (destObj->isArray()) {
    dest = new LinkDest(destObj->getArray());
    if (!dest->isOk()) {
      delete dest;
      dest = NULL;
    }

  // error
  } else {
    error(errSyntaxWarning, -1, "Illegal annotation destination");
  }
}

LinkGoTo::~LinkGoTo() {
  if (dest)
    delete dest;
  if (namedDest)
    delete namedDest;
}

//------------------------------------------------------------------------
// LinkGoToR
//------------------------------------------------------------------------

LinkGoToR::LinkGoToR(Object *fileSpecObj, Object *destObj) {
  dest = NULL;
  namedDest = NULL;

  // get file name
  fileName = getFileSpecName(fileSpecObj);

  // named destination
  if (destObj->isName()) {
    namedDest = new GString(destObj->getName());
  } else if (destObj->isString()) {
    namedDest = destObj->getString()->copy();

  // destination dictionary
  } else if (destObj->isArray()) {
    dest = new LinkDest(destObj->getArray());
    if (!dest->isOk()) {
      delete dest;
      dest = NULL;
    }

  // error
  } else {
    error(errSyntaxWarning, -1, "Illegal annotation destination");
  }
}

LinkGoToR::~LinkGoToR() {
  if (fileName)
    delete fileName;
  if (dest)
    delete dest;
  if (namedDest)
    delete namedDest;
}


//------------------------------------------------------------------------
// LinkLaunch
//------------------------------------------------------------------------

LinkLaunch::LinkLaunch(Object *actionObj) {
  Object obj1, obj2;

  fileName = NULL;
  params = NULL;

  if (actionObj->isDict()) {
    if (!actionObj->dictLookup("F", &obj1)->isNull()) {
      fileName = getFileSpecName(&obj1);
    } else {
      obj1.free();
#ifdef _WIN32
      if (actionObj->dictLookup("Win", &obj1)->isDict()) {
	obj1.dictLookup("F", &obj2);
	fileName = getFileSpecName(&obj2);
	obj2.free();
	if (obj1.dictLookup("P", &obj2)->isString()) {
	  params = obj2.getString()->copy();
	}
	obj2.free();
      } else {
	error(errSyntaxWarning, -1, "Bad launch-type link action");
      }
#else
      //~ This hasn't been defined by Adobe yet, so assume it looks
      //~ just like the Win dictionary until they say otherwise.
      if (actionObj->dictLookup("Unix", &obj1)->isDict()) {
	obj1.dictLookup("F", &obj2);
	fileName = getFileSpecName(&obj2);
	obj2.free();
	if (obj1.dictLookup("P", &obj2)->isString()) {
	  params = obj2.getString()->copy();
	}
	obj2.free();
      } else {
	error(errSyntaxWarning, -1, "Bad launch-type link action");
      }
#endif
    }
    obj1.free();
  }
}

LinkLaunch::~LinkLaunch() {
  if (fileName)
    delete fileName;
  if (params)
    delete params;
}

//------------------------------------------------------------------------
// LinkURI
//------------------------------------------------------------------------

LinkURI::LinkURI(Object *uriObj, GString *baseURI) {
  GString *uri2;
  int n;
  char c;

  uri = NULL;
  if (uriObj->isString()) {
    uri2 = uriObj->getString();
    n = (int)strcspn(uri2->getCString(), "/:");
    if (n < uri2->getLength() && uri2->getChar(n) == ':') {
      // "http:..." etc.
      uri = uri2->copy();
    } else if (!uri2->cmpN("www.", 4)) {
      // "www.[...]" without the leading "http://"
      uri = new GString("http://");
      uri->append(uri2);
    } else {
      // relative URI
      if (baseURI) {
	uri = baseURI->copy();
	c = uri->getChar(uri->getLength() - 1);
	if (c != '/' && c != '?') {
	  uri->append('/');
	}
	if (uri2->getChar(0) == '/') {
	  uri->append(uri2->getCString() + 1, uri2->getLength() - 1);
	} else {
	  uri->append(uri2);
	}
      } else {
	uri = uri2->copy();
      }
    }
  } else {
    error(errSyntaxWarning, -1, "Illegal URI-type link");
  }
}

LinkURI::~LinkURI() {
  if (uri)
    delete uri;
}

//------------------------------------------------------------------------
// LinkNamed
//------------------------------------------------------------------------

LinkNamed::LinkNamed(Object *nameObj) {
  name = NULL;
  if (nameObj->isName()) {
    name = new GString(nameObj->getName());
  }
}

LinkNamed::~LinkNamed() {
  if (name) {
    delete name;
  }
}

//------------------------------------------------------------------------
// LinkMovie
//------------------------------------------------------------------------

LinkMovie::LinkMovie(Object *annotObj, Object *titleObj) {
  annotRef.num = -1;
  title = NULL;
  if (annotObj->isRef()) {
    annotRef = annotObj->getRef();
  } else if (titleObj->isString()) {
    title = titleObj->getString()->copy();
  } else {
    error(errSyntaxError, -1,
	  "Movie action is missing both the Annot and T keys");
  }
}

LinkMovie::~LinkMovie() {
  if (title) {
    delete title;
  }
}

//------------------------------------------------------------------------
// LinkJavaScript
//------------------------------------------------------------------------

LinkJavaScript::LinkJavaScript(Object *jsObj) {
  char buf[4096];
  int n;

  if (jsObj->isString()) {
    js = jsObj->getString()->copy();
  } else if (jsObj->isStream()) {
    js = new GString();
    jsObj->streamReset();
    while ((n = jsObj->getStream()->getBlock(buf, sizeof(buf))) > 0) {
      js->append(buf, n);
    }
    jsObj->streamClose();
  } else {
    error(errSyntaxError, -1, "JavaScript action JS key is wrong type");
    js = NULL;
  }
}

LinkJavaScript::~LinkJavaScript() {
  if (js) {
    delete js;
  }
}

//------------------------------------------------------------------------
// LinkSubmitForm
//------------------------------------------------------------------------

LinkSubmitForm::LinkSubmitForm(Object *urlObj, Object *fieldsObj,
			       Object *flagsObj) {
  if (urlObj->isString()) {
    url = urlObj->getString()->copy();
  } else {
    error(errSyntaxError, -1, "SubmitForm action URL is wrong type");
    url = NULL;
  }

  if (fieldsObj->isArray()) {
    fieldsObj->copy(&fields);
  } else {
    if (!fieldsObj->isNull()) {
      error(errSyntaxError, -1, "SubmitForm action Fields value is wrong type");
    }
    fields.initNull();
  }

  if (flagsObj->isInt()) {
    flags = flagsObj->getInt();
  } else {
    if (!flagsObj->isNull()) {
      error(errSyntaxError, -1, "SubmitForm action Flags value is wrong type");
    }
    flags = 0;
  }
}

LinkSubmitForm::~LinkSubmitForm() {
  if (url) {
    delete url;
  }
  fields.free();
}

//------------------------------------------------------------------------
// LinkHide
//------------------------------------------------------------------------

LinkHide::LinkHide(Object *fieldsObj, Object *hideFlagObj) {
  if (fieldsObj->isRef() || fieldsObj->isString() || fieldsObj->isArray()) {
    fieldsObj->copy(&fields);
  } else {
    error(errSyntaxError, -1, "Hide action T value is wrong type");
    fields.initNull();
  }

  if (hideFlagObj->isBool()) {
    hideFlag = hideFlagObj->getBool();
  } else {
    error(errSyntaxError, -1, "Hide action H value is wrong type");
    hideFlag = gTrue;
  }
}

LinkHide::~LinkHide() {
  fields.free();
}

//------------------------------------------------------------------------
// LinkUnknown
//------------------------------------------------------------------------

LinkUnknown::LinkUnknown(char *actionA) {
  action = new GString(actionA);
}

LinkUnknown::~LinkUnknown() {
  delete action;
}

//------------------------------------------------------------------------
// Link
//------------------------------------------------------------------------

Link::Link(Dict *dict, GString *baseURI) {
  Object obj1, obj2;
  double t;

  action = NULL;
  ok = gFalse;

  // get rectangle
  if (!dict->lookup("Rect", &obj1)->isArray()) {
    error(errSyntaxError, -1, "Annotation rectangle is wrong type");
    goto err2;
  }
  if (!obj1.arrayGet(0, &obj2)->isNum()) {
    error(errSyntaxError, -1, "Bad annotation rectangle");
    goto err1;
  }
  x1 = obj2.getNum();
  obj2.free();
  if (!obj1.arrayGet(1, &obj2)->isNum()) {
    error(errSyntaxError, -1, "Bad annotation rectangle");
    goto err1;
  }
  y1 = obj2.getNum();
  obj2.free();
  if (!obj1.arrayGet(2, &obj2)->isNum()) {
    error(errSyntaxError, -1, "Bad annotation rectangle");
    goto err1;
  }
  x2 = obj2.getNum();
  obj2.free();
  if (!obj1.arrayGet(3, &obj2)->isNum()) {
    error(errSyntaxError, -1, "Bad annotation rectangle");
    goto err1;
  }
  y2 = obj2.getNum();
  obj2.free();
  obj1.free();
  if (x1 > x2) {
    t = x1;
    x1 = x2;
    x2 = t;
  }
  if (y1 > y2) {
    t = y1;
    y1 = y2;
    y2 = t;
  }

  // look for destination
  if (!dict->lookup("Dest", &obj1)->isNull()) {
    action = LinkAction::parseDest(&obj1);

  // look for action
  } else {
    obj1.free();
    if (dict->lookup("A", &obj1)->isDict()) {
      action = LinkAction::parseAction(&obj1, baseURI);
    }
  }
  obj1.free();

  // check for bad action
  if (action) {
    ok = gTrue;
  }

  return;

 err1:
  obj2.free();
 err2:
  obj1.free();
}

Link::~Link() {
  if (action) {
    delete action;
  }
}

//------------------------------------------------------------------------
// Links
//------------------------------------------------------------------------

Links::Links(Object *annots, GString *baseURI) {
  Link *link;
  Object obj1, obj2, obj3;
  int size;
  int i;

  links = NULL;
  size = 0;
  numLinks = 0;

  if (annots->isArray()) {
    for (i = 0; i < annots->arrayGetLength(); ++i) {
      if (annots->arrayGet(i, &obj1)->isDict()) {
	obj1.dictLookup("Subtype", &obj2);
	obj1.dictLookup("FT", &obj3);
	if (obj2.isName("Link") ||
	    (obj2.isName("Widget") && (obj3.isName("Btn") || obj3.isNull()))) {
	  link = new Link(obj1.getDict(), baseURI);
	  if (link->isOk()) {
	    if (numLinks >= size) {
	      size += 16;
	      links = (Link **)greallocn(links, size, sizeof(Link *));
	    }
	    links[numLinks++] = link;
	  } else {
	    delete link;
	  }
	}
	obj3.free();
	obj2.free();
      }
      obj1.free();
    }
  }
}

Links::~Links() {
  int i;

  for (i = 0; i < numLinks; ++i)
    delete links[i];
  gfree(links);
}

LinkAction *Links::find(double x, double y) {
  int i;

  for (i = numLinks - 1; i >= 0; --i) {
    if (links[i]->inRect(x, y)) {
      return links[i]->getAction();
    }
  }
  return NULL;
}

GBool Links::onLink(double x, double y) {
  int i;

  for (i = 0; i < numLinks; ++i) {
    if (links[i]->inRect(x, y))
      return gTrue;
  }
  return gFalse;
}
