//========================================================================
//
// CMap.cc
//
// Copyright 2001-2003 Glyph & Cog, LLC
//
//========================================================================

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gmem.h"
#include "gmempp.h"
#include "gfile.h"
#include "GString.h"
#include "Error.h"
#include "GlobalParams.h"
#include "PSTokenizer.h"
#include "Object.h"
#include "Stream.h"
#include "CMap.h"

#ifdef CMAP_USE_MEMORY
#include "../../Resources/BaseFonts.h"
#endif

//------------------------------------------------------------------------

struct CMapVectorEntry {
  GBool isVector;
  union {
    CMapVectorEntry *vector;
    CID cid;
  };
};

//------------------------------------------------------------------------

static int getCharFromFile(void *data) {
  return fgetc((FILE *)data);
}

static int getCharFromStream(void *data) {
  return ((Stream *)data)->getChar();
}

//------------------------------------------------------------------------

CMap *CMap::parse(CMapCache *cache, GString *collectionA, Object *obj) {
  CMap *cMap;
  GString *cMapNameA;

  if (obj->isName()) {
    cMapNameA = new GString(obj->getName());
    if (!(cMap = globalParams->getCMap(collectionA, cMapNameA))) {
      error(errSyntaxError, -1,
	    "Unknown CMap '{0:t}' for character collection '{1:t}'",
	    cMapNameA, collectionA);
    }
    delete cMapNameA;
  } else if (obj->isStream()) {
    if (!(cMap = CMap::parse(NULL, collectionA, obj->getStream()))) {
      error(errSyntaxError, -1, "Invalid CMap in Type 0 font");
    }
  } else {
    error(errSyntaxError, -1, "Invalid Encoding in Type 0 font");
    return NULL;
  }
  return cMap;
}

CMap *CMap::parse(CMapCache *cache, GString *collectionA,
		  GString *cMapNameA) {
  FILE *f;
  CMap *cMap;

#ifdef CMAP_USE_MEMORY
  const char* pDataCMap = NULL;
  unsigned int nSizeCMap = 0;
  if (PdfReader::GetBaseCMap(cMapNameA->getCString(), pDataCMap, nSizeCMap)) {
    Object obj;
    obj.initNull();
    BaseStream *str = new MemStream((char*)pDataCMap, 0, nSizeCMap, &obj);
    if (!str)
      return NULL;
    cMap = new CMap(collectionA->copy(), cMapNameA->copy());
    cMap->parse2(cache, &getCharFromStream, str);
    delete str;
    return cMap;
  }
#endif

  if (!(f = globalParams->findCMapFile(collectionA, cMapNameA))) {

    // Check for an identity CMap.
    if (!cMapNameA->cmp("Identity") || !cMapNameA->cmp("Identity-H")) {
      return new CMap(collectionA->copy(), cMapNameA->copy(), 0);
    }
    if (!cMapNameA->cmp("Identity-V")) {
      return new CMap(collectionA->copy(), cMapNameA->copy(), 1);
    }

    error(errSyntaxError, -1,
	  "Couldn't find '{0:t}' CMap file for '{1:t}' collection",
	  cMapNameA, collectionA);
    return NULL;
  }

  cMap = new CMap(collectionA->copy(), cMapNameA->copy());
  cMap->parse2(cache, &getCharFromFile, f);

  fclose(f);

  return cMap;
}

CMap *CMap::parse(CMapCache *cache, GString *collectionA, Stream *str) {
  Object obj1;
  CMap *cMap;

  cMap = new CMap(collectionA->copy(), NULL);

  if (!str->getDict()->lookup("UseCMap", &obj1)->isNull()) {
    cMap->useCMap(cache, &obj1);
  }
  obj1.free();

  str->reset();
  cMap->parse2(cache, &getCharFromStream, str);
  str->close();
  return cMap;
}

void CMap::parse2(CMapCache *cache, int (*getCharFunc)(void *), void *data) {
  PSTokenizer *pst;
  char tok1[256], tok2[256], tok3[256];
  int n1, n2, n3;
  Guint start, end, code;

  pst = new PSTokenizer(getCharFunc, data);
  pst->getToken(tok1, sizeof(tok1), &n1);
  while (pst->getToken(tok2, sizeof(tok2), &n2)) {
    if (!strcmp(tok2, "usecmap")) {
      if (tok1[0] == '/') {
	useCMap(cache, tok1 + 1);
      }
      pst->getToken(tok1, sizeof(tok1), &n1);
    } else if (!strcmp(tok1, "/WMode")) {
      wMode = atoi(tok2);
      pst->getToken(tok1, sizeof(tok1), &n1);
    } else if (!strcmp(tok2, "begincidchar")) {
      while (pst->getToken(tok1, sizeof(tok1), &n1)) {
	if (!strcmp(tok1, "endcidchar")) {
	  break;
	}
	if (!pst->getToken(tok2, sizeof(tok2), &n2) ||
	    !strcmp(tok2, "endcidchar")) {
	  error(errSyntaxError, -1, "Illegal entry in cidchar block in CMap");
	  break;
	}
	if (!(tok1[0] == '<' && tok1[n1 - 1] == '>' &&
	      n1 >= 4 && (n1 & 1) == 0)) {
	  error(errSyntaxError, -1, "Illegal entry in cidchar block in CMap");
	  continue;
	}
	tok1[n1 - 1] = '\0';
	if (sscanf(tok1 + 1, "%x", &code) != 1) {
	  error(errSyntaxError, -1, "Illegal entry in cidchar block in CMap");
	  continue;
	}
	n1 = (n1 - 2) / 2;
	addCIDs(code, code, n1, (CID)atoi(tok2));
      }
      pst->getToken(tok1, sizeof(tok1), &n1);
    } else if (!strcmp(tok2, "begincidrange")) {
      while (pst->getToken(tok1, sizeof(tok1), &n1)) {
	if (!strcmp(tok1, "endcidrange")) {
	  break;
	}
	if (!pst->getToken(tok2, sizeof(tok2), &n2) ||
	    !strcmp(tok2, "endcidrange") ||
	    !pst->getToken(tok3, sizeof(tok3), &n3) ||
	    !strcmp(tok3, "endcidrange")) {
	  error(errSyntaxError, -1, "Illegal entry in cidrange block in CMap");
	  break;
	}
	if (tok1[0] == '<' && tok2[0] == '<' &&
	    n1 == n2 && n1 >= 4 && (n1 & 1) == 0) {
	  tok1[n1 - 1] = tok2[n1 - 1] = '\0';
	  sscanf(tok1 + 1, "%x", &start);
	  sscanf(tok2 + 1, "%x", &end);
	  n1 = (n1 - 2) / 2;
	  addCIDs(start, end, n1, (CID)atoi(tok3));
	}
      }
      pst->getToken(tok1, sizeof(tok1), &n1);
    } else {
      strcpy(tok1, tok2);
    }
  }
  delete pst;
}

CMap::CMap(GString *collectionA, GString *cMapNameA) {
  int i;

  collection = collectionA;
  cMapName = cMapNameA;
  isIdent = gFalse;
  wMode = 0;
  vector = (CMapVectorEntry *)gmallocn(256, sizeof(CMapVectorEntry));
  for (i = 0; i < 256; ++i) {
    vector[i].isVector = gFalse;
    vector[i].cid = 0;
  }
  refCnt = 1;
}

CMap::CMap(GString *collectionA, GString *cMapNameA, int wModeA) {
  collection = collectionA;
  cMapName = cMapNameA;
  isIdent = gTrue;
  wMode = wModeA;
  vector = NULL;
  refCnt = 1;
}

void CMap::useCMap(CMapCache *cache, char *useName) {
  GString *useNameStr;
  CMap *subCMap;

  useNameStr = new GString(useName);
  // if cache is non-NULL, we already have a lock, and we can use
  // CMapCache::getCMap() directly; otherwise, we need to use
  // GlobalParams::getCMap() in order to acqure the lock need to use
  // GlobalParams::getCMap
  if (cache) {
    subCMap = cache->getCMap(collection, useNameStr);
  } else {
    subCMap = globalParams->getCMap(collection, useNameStr);
  }
  delete useNameStr;
  if (!subCMap) {
    return;
  }
  isIdent = subCMap->isIdent;
  if (subCMap->vector) {
    copyVector(vector, subCMap->vector);
  }
  subCMap->decRefCnt();
}

void CMap::useCMap(CMapCache *cache, Object *obj) {
  CMap *subCMap;

  subCMap = CMap::parse(cache, collection, obj);
  if (!subCMap) {
    return;
  }
  isIdent = subCMap->isIdent;
  if (subCMap->vector) {
    copyVector(vector, subCMap->vector);
  }
  subCMap->decRefCnt();
}

void CMap::copyVector(CMapVectorEntry *dest, CMapVectorEntry *src) {
  int i, j;

  for (i = 0; i < 256; ++i) {
    if (src[i].isVector) {
      if (!dest[i].isVector) {
	dest[i].isVector = gTrue;
	dest[i].vector =
	  (CMapVectorEntry *)gmallocn(256, sizeof(CMapVectorEntry));
	for (j = 0; j < 256; ++j) {
	  dest[i].vector[j].isVector = gFalse;
	  dest[i].vector[j].cid = 0;
	}
      }
      copyVector(dest[i].vector, src[i].vector);
    } else {
      if (dest[i].isVector) {
	error(errSyntaxError, -1, "Collision in usecmap");
      } else {
	dest[i].cid = src[i].cid;
      }
    }
  }
}

void CMap::addCIDs(Guint start, Guint end, Guint nBytes, CID firstCID) {
  CMapVectorEntry *vec;
  int byte, byte0, byte1;
  Guint start1, end1, i, j, k;

  start1 = start & 0xffffff00;
  end1 = end & 0xffffff00;
  for (i = start1; i <= end1; i += 0x100) {
    vec = vector;
    for (j = nBytes - 1; j >= 1; --j) {
      byte = (i >> (8 * j)) & 0xff;
      if (!vec[byte].isVector) {
	vec[byte].isVector = gTrue;
	vec[byte].vector =
	    (CMapVectorEntry *)gmallocn(256, sizeof(CMapVectorEntry));
	for (k = 0; k < 256; ++k) {
	  vec[byte].vector[k].isVector = gFalse;
	  vec[byte].vector[k].cid = 0;
	}
      }
      vec = vec[byte].vector;
    }
    byte0 = (i < start) ? (start & 0xff) : 0;
    byte1 = (i + 0xff > end) ? (end & 0xff) : 0xff;
    for (byte = byte0; byte <= byte1; ++byte) {
      if (vec[byte].isVector) {
	error(errSyntaxError, -1, "Invalid CID ({0:x} [{1:d} bytes]) in CMap",
	      i, nBytes);
      } else {
	vec[byte].cid = firstCID + ((i + byte) - start);
      }
    }
  }
}

CMap::~CMap() {
  delete collection;
  if (cMapName) {
    delete cMapName;
  }
  if (vector) {
    freeCMapVector(vector);
  }
}

void CMap::freeCMapVector(CMapVectorEntry *vec) {
  int i;

  for (i = 0; i < 256; ++i) {
    if (vec[i].isVector) {
      freeCMapVector(vec[i].vector);
    }
  }
  gfree(vec);
}

void CMap::incRefCnt() {
#if MULTITHREADED
  gAtomicIncrement(&refCnt);
#else
  ++refCnt;
#endif
}

void CMap::decRefCnt() {
  GBool done;

#if MULTITHREADED
  done = gAtomicDecrement(&refCnt) == 0;
#else
  done = --refCnt == 0;
#endif
  if (done) {
    delete this;
  }
}

GBool CMap::match(GString *collectionA, GString *cMapNameA) {
  return !collection->cmp(collectionA) && !cMapName->cmp(cMapNameA);
}

CID CMap::getCID(char *s, int len, CharCode *c, int *nUsed) {
  CMapVectorEntry *vec;
  CharCode cc;
  int n, i;

  vec = vector;
  cc = 0;
  n = 0;
  while (vec && n < len) {
    i = s[n++] & 0xff;
    cc = (cc << 8) | i;
    if (!vec[i].isVector) {
      *c = cc;
      *nUsed = n;
      return vec[i].cid;
    }
    vec = vec[i].vector;
  }
  if (isIdent && len >= 2) {
    // identity CMap
    *nUsed = 2;
    *c = cc = ((s[0] & 0xff) << 8) + (s[1] & 0xff);
    return cc;
  }
  *nUsed = 1;
  *c = s[0] & 0xff;
  return 0;
}

//------------------------------------------------------------------------

CMapCache::CMapCache() {
  int i;

  for (i = 0; i < cMapCacheSize; ++i) {
    cache[i] = NULL;
  }
}

CMapCache::~CMapCache() {
  int i;

  for (i = 0; i < cMapCacheSize; ++i) {
    if (cache[i]) {
      cache[i]->decRefCnt();
    }
  }
}

CMap *CMapCache::getCMap(GString *collection, GString *cMapName) {
  CMap *cmap;
  int i, j;

  if (cache[0] && cache[0]->match(collection, cMapName)) {
    cache[0]->incRefCnt();
    return cache[0];
  }
  for (i = 1; i < cMapCacheSize; ++i) {
    if (cache[i] && cache[i]->match(collection, cMapName)) {
      cmap = cache[i];
      for (j = i; j >= 1; --j) {
	cache[j] = cache[j - 1];
      }
      cache[0] = cmap;
      cmap->incRefCnt();
      return cmap;
    }
  }
  if ((cmap = CMap::parse(this, collection, cMapName))) {
    if (cache[cMapCacheSize - 1]) {
      cache[cMapCacheSize - 1]->decRefCnt();
    }
    for (j = cMapCacheSize - 1; j >= 1; --j) {
      cache[j] = cache[j - 1];
    }
    cache[0] = cmap;
    cmap->incRefCnt();
    return cmap;
  }
  return NULL;
}
